
// GameView.cpp : CGameView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Game.h"
#endif

#include "GameDoc.h"
#include "GameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGameView

IMPLEMENT_DYNCREATE(CGameView, CView)

BEGIN_MESSAGE_MAP(CGameView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CGameView 생성/소멸

CGameView::CGameView()
{
	nMatchCount = 0;
}

CGameView::~CGameView()
{
}

BOOL CGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CGameView 그리기

void CGameView::OnDraw(CDC* pDC)
{
	CGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	int nCount = 0;
	
	CDC memDC;
	CBitmap* pOldBmp;

	if(pDoc->m_bRandom){
		nMatchCount=0;
		pDoc->OnRandom();
		pDoc->m_bRandom = false;
	}

	// IDB_BITMAP1이 노란색 카드라는 가정 하에 nCount에 초기값 1을 준다.

	memDC.CreateCompatibleDC(pDC);

	for(int n=0; n< pDoc->m_nRow;n++)
	{
		for(int m = 0; m<pDoc->m_nCol;m++){
			
			if(pDoc->m_bShow[n][m] == true){
				pDoc->m_bmp[n][m].LoadBitmap(IDB_BITMAP1 + pDoc->m_nRnd[nCount]);
				pOldBmp = memDC.SelectObject(&pDoc->m_bmp[n][m]);

				//왼쪽과 위쪽에 여백을 주기 위해서 1을 더했다.
				pDC->BitBlt(pDoc->m_bmCell.cx*(m+1), pDoc->m_bmCell.cy*(n+1), 
						pDoc->m_bmCell.cx, pDoc->m_bmCell.cy, &memDC, 0 ,0, SRCCOPY);
				pDoc->m_bmp[n][m].DeleteObject();
			}
			
			nCount++;

			if(pDoc->m_bShow[n][m] == false){
				CBitmap bmp;
				bmp.LoadBitmap(IDB_BITMAP1);
				pOldBmp = memDC.SelectObject(&bmp);
				pDC->BitBlt(pDoc->m_bmCell.cx*(m+1), pDoc->m_bmCell.cy*(n+1), 
					pDoc->m_bmCell.cx, pDoc->m_bmCell.cy, &memDC, 0 ,0, SRCCOPY);
				bmp.DeleteObject();
			}
		}
	}
	//memDC.SelectObject(pOldBmp);
	pDoc->ResizeWindow();
}


// CGameView 인쇄

BOOL CGameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CGameView 진단

#ifdef _DEBUG
void CGameView::AssertValid() const
{
	CView::AssertValid();
}

void CGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGameDoc* CGameView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameDoc)));
	return (CGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CGameView 메시지 처리기


void CGameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CGameDoc* pDoc = GetDocument();

	//현재 마우스가 가지고 있는 인덱스
	pDoc->m_nRowIndex = point.y / pDoc->m_bmCell.cy -1;
	pDoc->m_nColIndex = point.x / pDoc->m_bmCell.cx -1;

	//이전 클릭한 곳에 다시 클릭, 두번 이상 클릭, 격자 밖에 클릭했다면 리턴
	if(pDoc->m_bShow[pDoc->m_nRowIndex][pDoc->m_nColIndex] == true
		|| pDoc ->m_nBmpSecondID != 0
		|| pDoc -> m_nRowIndex > pDoc->m_nRow -1
		|| pDoc -> m_nColIndex > pDoc->m_nCol -1
		|| pDoc -> m_nRowIndex < 0
		|| pDoc -> m_nColIndex < 0 )
		return;

	//마우스가 가지고 있는 익덱스 값에 그림을 보여줌
	pDoc ->m_bShow[pDoc->m_nRowIndex][pDoc->m_nColIndex] = true;

	CRect rect;
	SetRect(&rect, 
		pDoc->m_nRowIndex * (pDoc->m_bmCell.cx +1),
		pDoc->m_nColIndex * (pDoc->m_bmCell.cy +1),
		pDoc->m_nRowIndex * (pDoc->m_bmCell.cx +2),
		pDoc->m_nColIndex * (pDoc->m_bmCell.cy +2));
	InvalidateRect(rect);

	if(pDoc->m_nBmpFirstID == 0){
		pDoc->m_bMouse = true;

		pDoc->m_nBmpFirstID = pDoc->m_nBmpID[pDoc->m_nRowIndex][pDoc->m_nColIndex];
		
		m_nRowTempIndex = pDoc->m_nRowIndex;
		m_nColTempIndex = pDoc->m_nColIndex;
		
	}else{
		pDoc->m_bMouse = false;
		pDoc->m_nBmpSecondID = 
			pDoc->m_nBmpID[pDoc->m_nRowIndex][pDoc->m_nColIndex];
	
	}
	OnMatching();
	CView::OnLButtonDown(nFlags, point);
}



void CGameView::OnMatching(void){
	CGameDoc* pDoc = GetDocument();

	if(pDoc->m_nBmpFirstID == pDoc->m_nBmpSecondID && pDoc->m_bMouse == false){
		//클릭이벤트 초기화
		pDoc->m_bShow[m_nRowTempIndex][m_nColTempIndex] = true;
		pDoc->m_bShow[pDoc->m_nRowIndex][pDoc->m_nColIndex] = true;
		pDoc->m_nBmpFirstID = pDoc -> m_nBmpSecondID = 0;
		m_nRowTempIndex = m_nColTempIndex =0;

		nMatchCount++;
	}else if(pDoc->m_nBmpFirstID != pDoc->m_nBmpSecondID && pDoc->m_bMouse == false){
		CDC* pDC= GetDC();
		CDC memDC;
		CBitmap bmp;

		memDC.CreateCompatibleDC(pDC);
		bmp.LoadBitmapW(IDB_BITMAP1 + pDoc->m_nBmpSecondID);

		CBitmap* pOldBmp = memDC.SelectObject(&bmp);
		pDC->BitBlt(
			pDoc->m_bmCell.cx*(pDoc->m_nColIndex+1),
			pDoc->m_bmCell.cy*(pDoc->m_nRowIndex+1), 
			pDoc->m_bmCell.cx, 
			pDoc->m_bmCell.cy, &memDC, 0 ,0, SRCCOPY);
		bmp.DeleteObject();
		memDC.SelectObject(pOldBmp);
		Sleep(400);

		pDoc->m_bShow[m_nRowTempIndex][m_nColTempIndex] = false;
		pDoc->m_bShow[pDoc->m_nRowIndex][pDoc->m_nColIndex] = false;
		pDoc->m_nBmpFirstID = pDoc -> m_nBmpSecondID = 0;
		m_nRowTempIndex = m_nColTempIndex =0;
	}

	Invalidate();

	if(nMatchCount == (pDoc->m_nRow*pDoc->m_nCol)/2){
		OnSuccess();	
	}
}

void CGameView::OnSuccess(void){
	int res = AfxMessageBox(_T("게임오버! 다시시작하시겠습니까?"), MB_YESNO);
	if(res == IDYES){
		CGameDoc* pDoc = GetDocument();
		pDoc->m_bRandom = true;

		for(int n=0; n<pDoc->m_nRow; n++){
			for(int m=0; m<pDoc->m_nCol; m++){
				pDoc->m_bShow[n][m] = false;	
			}
		}

		pDoc->m_nBmpFirstID = pDoc->m_nBmpSecondID =0;
		pDoc->m_bMouse = false;
		Invalidate();
	}
}

