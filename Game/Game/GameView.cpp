
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

// CGameView

IMPLEMENT_DYNCREATE(CGameView, CView)

	BEGIN_MESSAGE_MAP(CGameView, CView)
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
		ON_WM_LBUTTONDOWN()
		ON_WM_CREATE()
		ON_WM_TIMER()

		ON_COMMAND(ID_NEW_GAME, &CGameView::OnNewGame)
		ON_COMMAND(ID_STATICS_RANKING, &CGameView::OnStaticsRanking)
		ON_COMMAND(ID_GRADE_TOP, &CGameView::OnGradeTop)
		ON_COMMAND(ID_GRADE_MIDDEL, &CGameView::OnGradeMiddel)
		ON_COMMAND(ID_GRADE_LOW, &CGameView::OnGradeLow)
		ON_COMMAND(ID_MODE_RANKING, &CGameView::OnModeRanking)
		ON_COMMAND(ID_MODE_PVP, &CGameView::OnModePvp)
		ON_COMMAND(ID_TYPE_DOOLY, &CGameView::OnTypeDooly)
		ON_COMMAND(ID_TYPE_POCKETMON, &CGameView::OnTypePocketmon)
		ON_COMMAND(ID_TYPE_ONEPIECE, &CGameView::OnTypeOnepiece)
		ON_COMMAND(ID_PVP_CHAT, &CGameView::OnPvpChat)
		ON_COMMAND(ID_PVP_GIVEUP, &CGameView::OnPvpGiveup)

		ON_UPDATE_COMMAND_UI(ID_GRADE_TOP, &CGameView::OnUpdateGradeTop)
		ON_UPDATE_COMMAND_UI(ID_GRADE_MIDDEL, &CGameView::OnUpdateGradeMiddel)
		ON_UPDATE_COMMAND_UI(ID_GRADE_LOW, &CGameView::OnUpdateGradeLow)
		ON_UPDATE_COMMAND_UI(ID_MODE_RANKING, &CGameView::OnUpdateModeRanking)
		ON_UPDATE_COMMAND_UI(ID_MODE_PVP, &CGameView::OnUpdateModePvp)
		ON_UPDATE_COMMAND_UI(ID_TYPE_DOOLY, &CGameView::OnUpdateTypeDooly)
		ON_UPDATE_COMMAND_UI(ID_TYPE_POCKETMON, &CGameView::OnUpdateTypePocketmon)
		ON_UPDATE_COMMAND_UI(ID_TYPE_ONEPIECE, &CGameView::OnUpdateTypeOnepiece)
	END_MESSAGE_MAP()

	// CGameView ����/�Ҹ�

	CGameView::CGameView(){
		m_pGame = NULL;			// Socket �̿� Dialog
		nMatchCount = 0;		
		isStatus = READY;		// Intro �̿� ���� ����
		before = clock();		// Intro �̿� �ð� ����
		m_strIntro = _T("");	// Intro ����
	}

	CGameView::~CGameView(){
		m_pGame->Disconnect();
		delete m_pGame;
	}

	BOOL CGameView::PreCreateWindow(CREATESTRUCT& cs){
		return CView::PreCreateWindow(cs);
	}

	// CGameView �׸���

	void CGameView::OnDraw(CDC* pDC){
		CGameDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		CDC memDC;
		CBitmap* pOldBmp;
		
		pDC->TextOutW(100, 10, pDoc->GetName()); // �̸� ���

		if(pDoc->m_bRandom){ // Local ��忡�� ���� ���۽� Random, matchCount �ʱ�ȭ 
			nMatchCount = 0;
			pDoc->OnRandom();
			pDoc->m_bRandom = false;
		}

		memDC.CreateCompatibleDC(pDC);
		int nCount = 0;
		for(int n = 0; n < pDoc->m_nRow ; n++){

			for(int m = 0; m < pDoc->m_nCol ; m++){

				if(pDoc->m_bShow[n][m] == true || isStatus == READY){
					pDoc->m_bmp[n][m].LoadBitmap(pDoc->GetType() + pDoc->m_nRnd[nCount]);
					pOldBmp = memDC.SelectObject(&pDoc->m_bmp[n][m]);

					pDC->BitBlt(
						pDoc->m_bmCell.cx*(m+1),
						pDoc->m_bmCell.cy*(n+1), 
						pDoc->m_bmCell.cx, 
						pDoc->m_bmCell.cy, &memDC, 0 ,0, SRCCOPY);
					pDoc->m_bmp[n][m].DeleteObject();
				}

				nCount++;

				if(pDoc->m_bShow[n][m] == false && isStatus != READY){
					CBitmap bmp;
					bmp.LoadBitmap(pDoc->GetType());
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

		pDoc->ResizeWindow();
	}

	// CGameView ����

#ifdef _DEBUG
	void CGameView::AssertValid() const{
		CView::AssertValid();
	}

	void CGameView::Dump(CDumpContext& dc) const{
		CView::Dump(dc);
	}

	CGameDoc* CGameView::GetDocument() const {
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameDoc)));
		return (CGameDoc*)m_pDocument;
	}
#endif //_DEBUG

	// CGameView �޽��� ó����
	void CGameView::OnNewPvpGame(){
		
		IsNetworkGame(); // ��Ʈ��ũ ���������� Check

		CGameDoc *pDoc = GetDocument();
		CSocketDlg dlg;
		dlg.m_strName = pDoc->GetName();
		int res = dlg.DoModal();

		if(res == IDOK){
			m_pGame->SetNewGame(dlg.m_strIpAddr, dlg.m_strName, dlg.m_strPlayMateName);
			
			BOOL bIReqGame = dlg.m_bIReqGame;
			if(bIReqGame == TRUE){//��û�ѻ���� ����� Start()����
				m_pGame->Start();
			}
			Invalidate(FALSE);
		}
	}

	void CGameView::PvpMessage(CString you, int youC){
		CGameDoc *pDoc = GetDocument();
		CClientDC dc(this);
		CString tempStr;
		tempStr.Format(pDoc->m_strName + _T(" : %d  --- %d : ") + you, nMatchCount, youC);
		dc.TextOutW(100, 100, tempStr);
	}

	bool CGameView::IsNetworkGame(){
		if(m_pGame->GetGameProcess() == GAME_PROCESS_PLAYING){
			MessageBox(_T("�����������̹Ƿ� �ش��ɸ� ������ �� �����ϴ�."),_T("Ȯ��"),MB_OK);
			return true;
		}
		return false;
	}

	void CGameView::OnLButtonDown(UINT nFlags, CPoint point){
		// �غ���¶�� Ŭ������ �ʴ´�.
		if(isStatus == READY) return;

		// Socket �����߿� ���� �����ΰ�� Ŭ������ �ʴ´�.
		if(m_pGame->GetGameProcess() == GAME_PROCESS_PLAYING && m_pGame->GetGameOrder() == GAME_ORDER_PLAYMATE) return;

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

			//Socket ���� ���̶�� ���浵 ���̰� ���ش�.
			if(m_pGame->GetGameProcess() == GAME_PROCESS_PLAYING)
				m_pGame->ShowClickImage();
		}else{
			pDoc->m_bMouse = false;
			pDoc->m_nBmpSecondID = pDoc->m_nBmpID[pDoc->m_nRowIndex][pDoc->m_nColIndex];
			
			//Socket ���� ���̶�� ���浵 ���̰� ���ش�.
			if(m_pGame->GetGameProcess() == GAME_PROCESS_PLAYING)
				m_pGame->ShowClickImage();
		}
	
		OnMatching();
		CView::OnLButtonDown(nFlags, point);
	}

	void CGameView::OnMatching(void){
		CGameDoc* pDoc = GetDocument();

		if(pDoc->m_nBmpFirstID == pDoc->m_nBmpSecondID 
			&& pDoc->m_bMouse == false){//Ŭ���̺�Ʈ �ʱ�ȭ
			pDoc->m_bShow[m_nRowTempIndex][m_nColTempIndex] = true;
			pDoc->m_bShow[pDoc->m_nRowIndex][pDoc->m_nColIndex] = true;
			pDoc->m_nBmpFirstID = pDoc->m_nBmpSecondID = 0;
			m_nRowTempIndex = m_nColTempIndex =0;

			nMatchCount++;
			//Socket ���� ���̶�� ������ ����ٰ� �˷��ش�.
			if(m_pGame->GetGameProcess() == GAME_PROCESS_PLAYING)
				m_pGame->MatchingIncCount();
			
		}else if(pDoc->m_nBmpFirstID != pDoc->m_nBmpSecondID 
			&& pDoc->m_bMouse == false){
			CDC* pDC= GetDC();
			CDC memDC;
			CBitmap bmp;

			memDC.CreateCompatibleDC(pDC);
			bmp.LoadBitmapW(pDoc->GetType() + pDoc->m_nBmpSecondID);

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
			//Socket ���� ���̶�� ����͵� �����ش�.
			if(m_pGame->GetGameProcess() == GAME_PROCESS_PLAYING)
				m_pGame->HideClickImage(m_nRowTempIndex, m_nColTempIndex, pDoc->m_nRowIndex,pDoc->m_nColIndex);
			pDoc->m_nBmpFirstID = pDoc->m_nBmpSecondID = 0;
			m_nRowTempIndex = m_nColTempIndex = 0;
		}
		
		Invalidate();

		// Local ���ӿ����� ����ȴ�.
		if(nMatchCount == (pDoc->m_nRow * pDoc->m_nCol)/2){
			OnSuccess();	
		}
	}

	void CGameView::OnSuccess(void){
		CGameDoc* pDoc = GetDocument();
		CString strTemp;

		if(pDoc->GetMode() == PRACTICE){//�������
			OnTimerStop();
			pDoc->OnWriteScoreFile(_T("practice"), pDoc->GetGrade());
			strTemp.Format(_T("�ɸ��ð��� %d:%d:%d:%d�Դϴ�.\n���ӳ�! �ٽ� �����Ͻðڽ��ϱ�?"), 
				pDoc->GetHour(), pDoc->GetMinute(), pDoc->GetSecond(), pDoc->GetTimeset());	

			if(AfxMessageBox(strTemp, MB_YESNO) == IDYES){ //������ �ٽ� �����ϴ°��
				pDoc->InitializeGame();
				Invalidate();
				OnTimerStart();
			}
		}else if(pDoc->GetMode() == RANKING){//��ŷ���
			OnTimerStop();	// Timer�� �ϴ� ����
			
			if(pDoc->m_nCol < 6){ // ���� 6�� �������� ��� Timer ����
				pDoc->m_nCol++;	// ��� ���� ����
				pDoc->m_nRow++;

				pDoc->InitializeGame();
				Invalidate();
				OnTimerStart();
			}else{//��ŷ��� ����
				pDoc->OnWriteScoreFile(_T("score"), -1);
				strTemp.Format(_T("�ɸ��ð��� %d:%d:%d:%d�Դϴ�.\n��ŷ�� Ȯ���Ͻðڽ��ϱ�?"), 
					pDoc->GetHour(), pDoc->GetMinute(), pDoc->GetSecond(), pDoc->GetTimeset());
	
				if(AfxMessageBox(strTemp, MB_YESNO) == IDYES){ // ��ŷȮ���ϴ°�� ���� �ٽ� ���� X
					CStaticDlg dlg;
					dlg.DoModal();
				}else{ // Ȯ������ ���� ��� ��ŷ��带 �ٽ� �����Ѵ�.
					pDoc->m_nCol = 3;
					pDoc->m_nRow = 2;

					pDoc->InitializeGame();
					Invalidate();
					OnTimerStart();
				}
			}
		}
	}

	int CGameView::OnCreate(LPCREATESTRUCT lpCreateStruct){
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		CGameDoc *pDoc = GetDocument();	
		CWelcomeDlg dlg;
		int res = dlg.DoModal();
		
		pDoc->SetName(dlg.m_strName == "" ? _T("Guest") : dlg.m_strName); // �̸� �Է¹���
		pDoc->SetMode(dlg.m_nMode); // ��� �Է¹���

		m_pGame = new CPvpGame;
		m_pGame->Create(this);
		
		if(res == IDOK){
			if(dlg.m_nMode == PRACTICE){
				pDoc->SetGrade(dlg.m_nGrade);
				pDoc->UpdateGrade();
				OnTimerStart();
			}else if(dlg.m_nMode == RANKING){//��� RANKING
				pDoc->m_nCol = 3;
				pDoc->m_nRow = 2;
				OnTimerStart();
			}else{// ��� PVP
				pDoc->m_nCol = 6;
				pDoc->m_nRow = 5;
				OnNewPvpGame();
			}
		}else{//���õ��� �ʾҴٸ� �������� ����
			pDoc->SetGrade(LOW);
			pDoc->UpdateGrade();
			OnTimerStart();
		}
	}

	void CGameView::OnTimerStart(void){
		before = clock();
		isStatus = READY;
		SetTimer(100, 100, NULL);
	}

	void CGameView::OnTimerStop(void){
		KillTimer(100);
	}

	void CGameView::OnTimer(UINT_PTR nIDEvent){
		switch (isStatus)
		{
		case READY: // Local Intro
			IntroReady();
			break;
		case START: // Local �������	
			StopWatch();
			break;
		case END:
			break;
		}
		CView::OnTimer(nIDEvent);
	}

	void CGameView::IntroReady(void){
		double time = ((double) clock()-before) / CLOCKS_PER_SEC;
		
		if(time < 1){
			m_strIntro = _T("Ready");
		}else if(time < 2){
			m_strIntro = _T("3               ");
		}else if(time < 3){
			m_strIntro = _T("2");
		}else if(time < 4){
			m_strIntro = _T("1");
		}else{
			isStatus = START;
			m_strIntro = _T("    ");
			Invalidate();
		}		
		CClientDC dc(this);
		dc.TextOutW(100, 30, m_strIntro);
	}
	
	void CGameView::StopWatch(void){
		CGameDoc *pDoc = GetDocument();
		CString strTemp;
		int hour = pDoc->GetHour(), 
			minute = pDoc->GetMinute(),
			second = pDoc->GetSecond(),
			timeset = pDoc->GetTimeset();
		
		strTemp.Format(_T("%d:%d:%d:%d"), hour, minute, second, ++timeset);
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
		dc.TextOutW(10, 10, strTemp);
	}

	// ~ Menu Function
	void CGameView::OnNewGame(){
		if(IsNetworkGame()) return; // ��Ʈ��ũ ���������� Check
		CGameDoc *pDoc = GetDocument();
		if(pDoc->GetMode() == RANKING){
			pDoc->m_nRow = 2;
			pDoc->m_nCol = 3;
		}
		pDoc->InitializeGame();
		Invalidate();
		OnTimerStop();
		OnTimerStart();
	}
	
	void CGameView::OnStaticsRanking(){
		if(IsNetworkGame()) return; // ��Ʈ��ũ ���������� Check
		OnTimerStop();// Dialog �� ������ Game Stop	
		CStaticDlg dlg;
		dlg.DoModal();
	}

	// ~ Menu Mode Event

	void CGameView::OnModeRanking(){ // Ranking ���
		if(IsNetworkGame()) return; // ��Ʈ��ũ ���������� Check
		CGameDoc *pDoc = GetDocument();
		pDoc->SetMode(RANKING);
		pDoc->UpdateGrade();
		Invalidate();
		OnTimerStop();
		OnTimerStart();
	}

	void CGameView::OnModePvp(){ // PVP 
		if(IsNetworkGame()) return; // ��Ʈ��ũ ���������� Check
		CGameDoc *pDoc = GetDocument();
		pDoc->SetMode(PVP);
		pDoc->UpdateGrade();
		Invalidate();
		OnTimerStop();
		OnNewPvpGame();
	}

	void CGameView::OnGradeTop(){ // Practice Top ���
		if(IsNetworkGame()) return; // ��Ʈ��ũ ���������� Check
		CGameDoc *pDoc = GetDocument();
		pDoc->SetMode(PRACTICE);
		pDoc->SetGrade(TOP);
		pDoc->UpdateGrade();
		Invalidate();
		OnTimerStop();
		OnTimerStart();
	}

	void CGameView::OnGradeMiddel(){ // Practice Middle ���
		if(IsNetworkGame()) return; // ��Ʈ��ũ ���������� Check
		CGameDoc *pDoc = GetDocument();
		pDoc->SetMode(PRACTICE);
		pDoc->SetGrade(MIDDLE);
		pDoc->UpdateGrade();
		Invalidate();
		OnTimerStop();
		OnTimerStart();
	}

	void CGameView::OnGradeLow(){ // Practice Low ���
		if(IsNetworkGame()) return; // ��Ʈ��ũ ���������� Check
		CGameDoc *pDoc = GetDocument();
		pDoc->SetMode(PRACTICE);
		pDoc->SetGrade(LOW);
		pDoc->UpdateGrade();
		Invalidate();
		OnTimerStop();
		OnTimerStart();
	}

	// ~ Menu Mode Check

	void CGameView::OnUpdateModeRanking(CCmdUI *pCmdUI){
		CGameDoc *pDoc = GetDocument();
		pCmdUI->SetCheck(pDoc->GetMode() == RANKING);
	}

	void CGameView::OnUpdateModePvp(CCmdUI *pCmdUI){
		CGameDoc *pDoc = GetDocument();
		pCmdUI->SetCheck(pDoc->GetMode() == PVP);
	}

	void CGameView::OnUpdateGradeTop(CCmdUI *pCmdUI){
		CGameDoc *pDoc = GetDocument();
		if(pDoc->GetMode() == PRACTICE)
			pCmdUI->SetCheck(pDoc->GetGrade() == TOP);
	}

	void CGameView::OnUpdateGradeMiddel(CCmdUI *pCmdUI){
		CGameDoc *pDoc = GetDocument();
		if(pDoc->GetMode() == PRACTICE)
			pCmdUI->SetCheck(pDoc->GetGrade() == MIDDLE);	
	}

	void CGameView::OnUpdateGradeLow(CCmdUI *pCmdUI){
		CGameDoc *pDoc = GetDocument();
		if(pDoc->GetMode() == PRACTICE)
			pCmdUI->SetCheck(pDoc->GetGrade() == LOW);
	}

	// ~ Menu Type Event

	void CGameView::OnTypeDooly(){
		if(IsNetworkGame()) return; // ��Ʈ��ũ ���������� Check
		CGameDoc *pDoc = GetDocument();
		pDoc->SetType(DOOLY);
		pDoc->InitializeGame();
		Invalidate();
		OnTimerStop();
		OnTimerStart();
	}
	
	void CGameView::OnTypePocketmon(){
		if(IsNetworkGame()) return; // ��Ʈ��ũ ���������� Check
		CGameDoc *pDoc = GetDocument();
		pDoc->SetType(POCKETMON);
		pDoc->InitializeGame();
		Invalidate();
		OnTimerStop();
		OnTimerStart();
	}

	void CGameView::OnTypeOnepiece(){
		if(IsNetworkGame()) return; // ��Ʈ��ũ ���������� Check
		CGameDoc *pDoc = GetDocument();
		pDoc->SetType(ONEPIECE);
		pDoc->InitializeGame();
		Invalidate();
		OnTimerStop();
		OnTimerStart();
	}

	// ~ Menu Type Check

	void CGameView::OnUpdateTypeDooly(CCmdUI *pCmdUI){
		CGameDoc *pDoc = GetDocument();
		pCmdUI->SetCheck(pDoc->GetType() == DOOLY);
	}

	void CGameView::OnUpdateTypePocketmon(CCmdUI *pCmdUI){
		CGameDoc *pDoc = GetDocument();
		pCmdUI->SetCheck(pDoc->GetType() == POCKETMON);
	}

	void CGameView::OnUpdateTypeOnepiece(CCmdUI *pCmdUI){
		CGameDoc *pDoc = GetDocument();
		pCmdUI->SetCheck(pDoc->GetType() == ONEPIECE);
	}

	// ~ Menu Pvp

	void CGameView::OnPvpChat(){
		if(m_pGame->GetGameProcess() != GAME_PROCESS_READY){
			m_pGame->ShowWindow(SW_SHOW);
		}else{
			MessageBox(_T("�������� �ƴϹǷ� ��� �� �� �����ϴ�."),_T("Ȯ��"),MB_OK);
			return;
		}
	}

	void CGameView::OnPvpGiveup(){
		if(m_pGame->GetGameProcess() != GAME_PROCESS_READY){
			m_pGame->GiveUpGame();
		}else{
			MessageBox(_T("�������� �ƴϹǷ� ��� �� �� �����ϴ�."),_T("Ȯ��"),MB_OK);
			return;
		}
	}
