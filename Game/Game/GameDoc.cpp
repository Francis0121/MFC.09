
// GameDoc.cpp : CGameDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Game.h"
#endif

#include "GameDoc.h"
#include "GameView.h"
#include "MainFrm.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGameDoc

IMPLEMENT_DYNCREATE(CGameDoc, CDocument)

BEGIN_MESSAGE_MAP(CGameDoc, CDocument)
END_MESSAGE_MAP()


// CGameDoc 생성/소멸

CGameDoc::CGameDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	m_bmCell = CSize(79, 81);
	m_nRow = 3;
	m_nCol = 4;

	m_bRandom = true;
	for(int n = 0; n < m_nRow; n++){
		for(int m = 0; m < m_nCol; m++){
			m_bShow[n][m] = false;
		}
	}

	m_nBmpFirstID = m_nBmpSecondID = 0;
	m_bMouse = false;
}

CGameDoc::~CGameDoc()
{
}

BOOL CGameDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CGameDoc serialization

void CGameDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CGameDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CGameDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGameDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGameDoc 진단

#ifdef _DEBUG
void CGameDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGameDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGameDoc 명령

void CGameDoc::ResizeWindow(void)
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

	CREATESTRUCT st;
	CREATESTRUCT& rst = st;

	pMain->PreCreateWindow(rst);

	rst.lpszName = _T("Game");
	rst.cx = (m_nRow+1) * m_bmCell.cx + 180;
	rst.cy = (m_nCol+1) * m_bmCell.cy + 100;

	CRect rect;
	pMain->GetClientRect(rect);
	pMain->MoveWindow(rect.left, rect.top, rst.cx, rst.cy);
}

void CGameDoc::OnRandom(void){
	//시간을 Seed로 잡아 똑같은 난수 발생을 방지
	srand((unsigned)time(NULL));

	//비트맵 삽입여부/ 격자의 총 수 / 처음 넣을 비트맵의 아이디
	BOOL bInsert = TRUE;
	//격자의 총수를 구한다.
	int nGrating = m_nRow * m_nCol;

	//격자의 수만큼 돌면서 난수 발생
	for(int n=0; n < nGrating; n++){
		//시작 비트맵 아이디에 난수를 더해 비트맵을 변경
		//일단 발생한 난수를 배열에 저장하고 본다.
		m_nRnd[n] = (rand()%(nGrating/2))+1;

		//현재 진행한 격자수까지만 비교한다.
		for(int m = 0; m<n; m++){
			if(m_nRnd[n] == m_nRnd[m]){//만일 같은 값이 있다면 변수에 삽입 불가능한지를 알아본다.
				if(bInsert == FALSE){//bInsert가 FALSE이면 세번쨰 동일한 값이므로 
					n--;//배열에 이미 세번째 저장딘 랜덤값을 취소하기 위해서 첨자를 감소한다.
					bInsert = TRUE;//세번이상은 절대 동일한 값이 저장되어 있지 않을 것이므로
					break;//안쪽 반복문을 벗어난다. 
				}
				bInsert = FALSE;
			}
		}
	}
	
	int nCount = 0;

	for(int n = 0; n < m_nRow; n++){
		for(int m = 0; m<m_nCol; m++){
			m_nBmpID[n][m] = m_nRnd[nCount];
			nCount++;
		}
	}

}