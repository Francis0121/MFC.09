
// GameView.cpp : CGameView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Game.h"
#endif

#include "GameDoc.h"
#include "GameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Dialog
#include "WelcomeDlg.h"


// CGameView

IMPLEMENT_DYNCREATE(CGameView, CView)

	BEGIN_MESSAGE_MAP(CGameView, CView)
		// ǥ�� �μ� ����Դϴ�.
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
		ON_WM_LBUTTONDOWN()
		ON_WM_CREATE()
		ON_COMMAND(ID_GRADE_TOP, &CGameView::OnGradeTop)
		ON_COMMAND(ID_GRADE_MIDDEL, &CGameView::OnGradeMiddel)
		ON_COMMAND(ID_GRADE_LOW, &CGameView::OnGradeLow)
		ON_UPDATE_COMMAND_UI(ID_GRADE_TOP, &CGameView::OnUpdateGradeTop)
		ON_UPDATE_COMMAND_UI(ID_GRADE_MIDDEL, &CGameView::OnUpdateGradeMiddel)
		ON_UPDATE_COMMAND_UI(ID_GRADE_LOW, &CGameView::OnUpdateGradeLow)
		ON_WM_TIMER()
	END_MESSAGE_MAP()

	// CGameView ����/�Ҹ�

	CGameView::CGameView()
	{
		nMatchCount = 0;
	}

	CGameView::~CGameView()
	{
	}

	BOOL CGameView::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
		//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

		return CView::PreCreateWindow(cs);
	}

	// CGameView �׸���

	void CGameView::OnDraw(CDC* pDC)
	{
		CGameDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
		int nCount = 0;

		CDC memDC;
		CBitmap* pOldBmp;

		if(pDoc->m_bRandom){
			nMatchCount=0;
			pDoc->OnRandom();
			pDoc->m_bRandom = false;
		}

		// IDB_BITMAP20 �� ����� ī���� ���� �Ͽ� nCount�� �ʱⰪ 1�� �ش�.

		memDC.CreateCompatibleDC(pDC);

		for(int n = 0; n < pDoc->m_nRow ; n++)
		{
			for(int m = 0; m < pDoc->m_nCol ; m++){

				if(pDoc->m_bShow[n][m] == true){

					pDoc->m_bmp[n][m].LoadBitmap(IDB_BITMAP20 + pDoc->m_nRnd[nCount]);
					pOldBmp = memDC.SelectObject(&pDoc->m_bmp[n][m]);

					pDC->BitBlt(
						pDoc->m_bmCell.cx*(m+1),
						pDoc->m_bmCell.cy*(n+1), 
						pDoc->m_bmCell.cx, 
						pDoc->m_bmCell.cy, &memDC, 0 ,0, SRCCOPY);
					pDoc->m_bmp[n][m].DeleteObject();
				}

				nCount++;

				if(pDoc->m_bShow[n][m] == false){
					CBitmap bmp;
					bmp.LoadBitmap(IDB_BITMAP20);
					pOldBmp = memDC.SelectObject(&bmp);
					pDC->BitBlt(
						pDoc->m_bmCell.cx*(m+1), 
						pDoc->m_bmCell.cy*(n+1), 
						pDoc->m_bmCell.cx, 
						pDoc->m_bmCell.cy, &memDC, 0 ,0, SRCCOPY);
					bmp.DeleteObject();
				}
			}
		}
		//memDC.SelectObject(pOldBmp);
		pDoc->ResizeWindow();
	}


	// CGameView �μ�

	BOOL CGameView::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// �⺻���� �غ�
		return DoPreparePrinting(pInfo);
	}

	void CGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	}

	void CGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
	}


	// CGameView ����

#ifdef _DEBUG
	void CGameView::AssertValid() const
	{
		CView::AssertValid();
	}

	void CGameView::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}

	CGameDoc* CGameView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameDoc)));
		return (CGameDoc*)m_pDocument;
	}
#endif //_DEBUG


	// CGameView �޽��� ó����


	void CGameView::OnLButtonDown(UINT nFlags, CPoint point)
	{
		CGameDoc* pDoc = GetDocument();

		//���� ���콺�� ������ �ִ� �ε���
		pDoc->m_nRowIndex = point.y / pDoc->m_bmCell.cy -1;
		pDoc->m_nColIndex = point.x / pDoc->m_bmCell.cx -1;

		//���� Ŭ���� ���� �ٽ� Ŭ��, �ι� �̻� Ŭ��, ���� �ۿ� Ŭ���ߴٸ� ����
		if(pDoc->m_bShow[pDoc->m_nRowIndex][pDoc->m_nColIndex] == true
			|| pDoc ->m_nBmpSecondID != 0
			|| pDoc -> m_nRowIndex > pDoc->m_nRow -1
			|| pDoc -> m_nColIndex > pDoc->m_nCol -1
			|| pDoc -> m_nRowIndex < 0
			|| pDoc -> m_nColIndex < 0 )
			return;

		//���콺�� ������ �ִ� �͵��� ���� �׸��� ������
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
			//Ŭ���̺�Ʈ �ʱ�ȭ
			pDoc->m_bShow[m_nRowTempIndex][m_nColTempIndex] = true;
			pDoc->m_bShow[pDoc->m_nRowIndex][pDoc->m_nColIndex] = true;
			pDoc->m_nBmpFirstID = pDoc->m_nBmpSecondID = 0;
			m_nRowTempIndex = m_nColTempIndex =0;

			nMatchCount++;
		}else if(pDoc->m_nBmpFirstID != pDoc->m_nBmpSecondID && pDoc->m_bMouse == false){
			CDC* pDC= GetDC();
			CDC memDC;
			CBitmap bmp;

			memDC.CreateCompatibleDC(pDC);
			bmp.LoadBitmapW(IDB_BITMAP20 + pDoc->m_nBmpSecondID);

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
			pDoc->m_nBmpFirstID = pDoc->m_nBmpSecondID = 0;
			m_nRowTempIndex = m_nColTempIndex =0;
		}

		Invalidate();

		if(nMatchCount == (pDoc->m_nRow * pDoc->m_nCol)/2){
			OnSuccess();	
		}
	}

	void CGameView::OnSuccess(void){

		OnTimerStop();
		CGameDoc* pDoc = GetDocument();
		CString str;
		str.Format(_T("�ɸ��ð��� %d:%d:%d:%d�Դϴ�\n���ӿ���! �ٽ� �����Ͻðڽ��ϱ�?"), 
			pDoc->GetHour(), pDoc->GetMinute(), pDoc->GetSecond(), pDoc->GetTimeset());

		int res = AfxMessageBox(str, MB_YESNO);
		if(res == IDYES){
			pDoc->InitializeGame();
			Invalidate();
			
			OnTimerReset();
			OnTimerStart();
		}
	}

	void CGameView::OnTimerStart(void){
		SetTimer(100, 100, NULL);
	}

	void CGameView::OnTimerStop(void){
		KillTimer(100);
	}

	void CGameView::OnTimerReset(void){
		CGameDoc *pDoc = GetDocument();
		pDoc->SetHour(0);
		pDoc->SetMinute(0);
		pDoc->SetSecond(0);
		pDoc->SetTimeset(0);
	}

	int CGameView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
		CWelcomeDlg dlg;
		int res = dlg.DoModal();

		CGameDoc *pDoc = GetDocument();	
		if(res == IDOK){
			pDoc->SetGrade(dlg.m_nGrade);
			pDoc->UpdateGrade();
			OnTimerStart();
		}else{
			pDoc->SetGrade(LOW); // Ȯ���� ������ �ʴ´ٸ� ������ LOW
			OnTimerStart();
		}
		return 0;
	}

	void CGameView::OnTimer(UINT_PTR nIDEvent){

		CGameDoc *pDoc = GetDocument();
		CString str;
		int hour = pDoc->GetHour();
		int minute = pDoc->GetMinute();
		int second = pDoc->GetSecond();
		int timeset = pDoc->GetTimeset();
		str.Format(_T("%d:%d:%d:%d"), hour, minute, second, ++timeset);
		pDoc->SetTimeset(timeset);

		if(minute == 60){
			pDoc->SetHour(++hour);
			pDoc->SetMinute(0);
		}

		if(second == 60){
			pDoc->SetMinute(++minute);
			pDoc->SetSecond(0);
		}

		if(timeset == 10){
			pDoc->SetSecond(++second);
			pDoc->SetTimeset(0);
		}

		CClientDC dc(this);
		dc.TextOutW(10, 10, str);

		CView::OnTimer(nIDEvent);
	}

	// Grade ���̵� ���� Menu Event
	void CGameView::OnGradeTop(){
		CGameDoc *pDoc = GetDocument();

		pDoc->SetGrade(TOP);
		pDoc->UpdateGrade();

		Invalidate();
		
		OnTimerStop();
		OnTimerReset();
		OnTimerStart();
	}

	void CGameView::OnGradeMiddel(){
		CGameDoc *pDoc = GetDocument();

		pDoc->SetGrade(MIDDLE);
		pDoc->UpdateGrade();

		Invalidate();

		OnTimerStop();
		OnTimerReset();
		OnTimerStart();
	}

	void CGameView::OnGradeLow(){
		CGameDoc *pDoc = GetDocument();

		pDoc->SetGrade(LOW);
		pDoc->UpdateGrade();

		Invalidate();
		
		OnTimerStop();
		OnTimerReset();
		OnTimerStart();
	}

	// Grade ���̵� ���� Update 
	void CGameView::OnUpdateGradeTop(CCmdUI *pCmdUI){
		CGameDoc *pDoc = GetDocument();
		pCmdUI->SetCheck(pDoc->GetGrade() == TOP);
	}

	void CGameView::OnUpdateGradeMiddel(CCmdUI *pCmdUI){
		CGameDoc *pDoc = GetDocument();
		pCmdUI->SetCheck(pDoc->GetGrade() == MIDDLE);	
	}

	void CGameView::OnUpdateGradeLow(CCmdUI *pCmdUI){
		CGameDoc *pDoc = GetDocument();
		pCmdUI->SetCheck(pDoc->GetGrade() == LOW);
	}


