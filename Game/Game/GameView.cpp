
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

	// CGameView 생성/소멸

	CGameView::CGameView(){
		m_pGame = NULL;			// Socket 이용 Dialog
		nMatchCount = 0;		
		isStatus = READY;		// Intro 이용 게임 상태
		before = clock();		// Intro 이용 시간 측정
		m_strIntro = _T("");	// Intro 문구
	}

	CGameView::~CGameView(){
		m_pGame->Disconnect();
		delete m_pGame;
	}

	BOOL CGameView::PreCreateWindow(CREATESTRUCT& cs){
		return CView::PreCreateWindow(cs);
	}

	// CGameView 그리기

	void CGameView::OnDraw(CDC* pDC){
		CGameDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		CDC memDC;
		CBitmap* pOldBmp;
		
		pDC->TextOutW(100, 10, pDoc->GetName()); // 이름 출력

		if(pDoc->m_bRandom){ // Local 모드에서 게임 시작시 Random, matchCount 초기화 
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

	// CGameView 진단

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

	// CGameView 메시지 처리기
	void CGameView::OnNewPvpGame(){
		
		IsNetworkGame(); // 네트워크 게임중인지 Check

		CGameDoc *pDoc = GetDocument();
		CSocketDlg dlg;
		dlg.m_strName = pDoc->GetName();
		int res = dlg.DoModal();

		if(res == IDOK){
			m_pGame->SetNewGame(dlg.m_strIpAddr, dlg.m_strName, dlg.m_strPlayMateName);
			
			BOOL bIReqGame = dlg.m_bIReqGame;
			if(bIReqGame == TRUE){//요청한사람이 나라면 Start()실행
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
			MessageBox(_T("게임진행중이므로 해당명령를 실행할 수 없습니다."),_T("확인"),MB_OK);
			return true;
		}
		return false;
	}

	void CGameView::OnLButtonDown(UINT nFlags, CPoint point){
		// 준비상태라면 클릭되지 않는다.
		if(isStatus == READY) return;

		// Socket 게임중에 상대방 차례인경우 클릭되지 않는다.
		if(m_pGame->GetGameProcess() == GAME_PROCESS_PLAYING && m_pGame->GetGameOrder() == GAME_ORDER_PLAYMATE) return;

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

			//Socket 게임 중이라면 상대방도 보이게 해준다.
			if(m_pGame->GetGameProcess() == GAME_PROCESS_PLAYING)
				m_pGame->ShowClickImage();
		}else{
			pDoc->m_bMouse = false;
			pDoc->m_nBmpSecondID = pDoc->m_nBmpID[pDoc->m_nRowIndex][pDoc->m_nColIndex];
			
			//Socket 게임 중이라면 상대방도 보이게 해준다.
			if(m_pGame->GetGameProcess() == GAME_PROCESS_PLAYING)
				m_pGame->ShowClickImage();
		}
	
		OnMatching();
		CView::OnLButtonDown(nFlags, point);
	}

	void CGameView::OnMatching(void){
		CGameDoc* pDoc = GetDocument();

		if(pDoc->m_nBmpFirstID == pDoc->m_nBmpSecondID 
			&& pDoc->m_bMouse == false){//클릭이벤트 초기화
			pDoc->m_bShow[m_nRowTempIndex][m_nColTempIndex] = true;
			pDoc->m_bShow[pDoc->m_nRowIndex][pDoc->m_nColIndex] = true;
			pDoc->m_nBmpFirstID = pDoc->m_nBmpSecondID = 0;
			m_nRowTempIndex = m_nColTempIndex =0;

			nMatchCount++;
			//Socket 게임 중이라면 상대방이 맞췄다고 알려준다.
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
			//Socket 게임 중이라면 상대방것도 가려준다.
			if(m_pGame->GetGameProcess() == GAME_PROCESS_PLAYING)
				m_pGame->HideClickImage(m_nRowTempIndex, m_nColTempIndex, pDoc->m_nRowIndex,pDoc->m_nColIndex);
			pDoc->m_nBmpFirstID = pDoc->m_nBmpSecondID = 0;
			m_nRowTempIndex = m_nColTempIndex = 0;
		}
		
		Invalidate();

		// Local 게임에서만 적용된다.
		if(nMatchCount == (pDoc->m_nRow * pDoc->m_nCol)/2){
			OnSuccess();	
		}
	}

	void CGameView::OnSuccess(void){
		CGameDoc* pDoc = GetDocument();
		CString strTemp;

		if(pDoc->GetMode() == PRACTICE){//연습모드
			OnTimerStop();
			pDoc->OnWriteScoreFile(_T("practice"), pDoc->GetGrade());
			strTemp.Format(_T("걸린시간은 %d:%d:%d:%d입니다.\n게임끝! 다시 시작하시겠습니까?"), 
				pDoc->GetHour(), pDoc->GetMinute(), pDoc->GetSecond(), pDoc->GetTimeset());	

			if(AfxMessageBox(strTemp, MB_YESNO) == IDYES){ //게임을 다시 시작하는경우
				pDoc->InitializeGame();
				Invalidate();
				OnTimerStart();
			}
		}else if(pDoc->GetMode() == RANKING){//랭킹모드
			OnTimerStop();	// Timer는 일단 정지
			
			if(pDoc->m_nCol < 6){ // 행이 6개 이전까지 계속 Timer 측정
				pDoc->m_nCol++;	// 행과 열을 증가
				pDoc->m_nRow++;

				pDoc->InitializeGame();
				Invalidate();
				OnTimerStart();
			}else{//랭킹모드 종료
				pDoc->OnWriteScoreFile(_T("score"), -1);
				strTemp.Format(_T("걸린시간은 %d:%d:%d:%d입니다.\n랭킹을 확인하시겠습니까?"), 
					pDoc->GetHour(), pDoc->GetMinute(), pDoc->GetSecond(), pDoc->GetTimeset());
	
				if(AfxMessageBox(strTemp, MB_YESNO) == IDYES){ // 랭킹확인하는경우 게임 다시 시작 X
					CStaticDlg dlg;
					dlg.DoModal();
				}else{ // 확인하지 않은 경우 랭킹모드를 다시 시작한다.
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
		
		pDoc->SetName(dlg.m_strName == "" ? _T("Guest") : dlg.m_strName); // 이름 입력받음
		pDoc->SetMode(dlg.m_nMode); // 모드 입력받음

		m_pGame = new CPvpGame;
		m_pGame->Create(this);
		
		if(res == IDOK){
			if(dlg.m_nMode == PRACTICE){
				pDoc->SetGrade(dlg.m_nGrade);
				pDoc->UpdateGrade();
				OnTimerStart();
			}else if(dlg.m_nMode == RANKING){//모드 RANKING
				pDoc->m_nCol = 3;
				pDoc->m_nRow = 2;
				OnTimerStart();
			}else{// 모드 PVP
				pDoc->m_nCol = 6;
				pDoc->m_nRow = 5;
				OnNewPvpGame();
			}
		}else{//선택되지 않았다면 연습모드로 시작
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
		case START: // Local 기록측정	
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
		if(IsNetworkGame()) return; // 네트워크 게임중인지 Check
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
		if(IsNetworkGame()) return; // 네트워크 게임중인지 Check
		OnTimerStop();// Dialog 가 켜지면 Game Stop	
		CStaticDlg dlg;
		dlg.DoModal();
	}

	// ~ Menu Mode Event

	void CGameView::OnModeRanking(){ // Ranking 모드
		if(IsNetworkGame()) return; // 네트워크 게임중인지 Check
		CGameDoc *pDoc = GetDocument();
		pDoc->SetMode(RANKING);
		pDoc->UpdateGrade();
		Invalidate();
		OnTimerStop();
		OnTimerStart();
	}

	void CGameView::OnModePvp(){ // PVP 
		if(IsNetworkGame()) return; // 네트워크 게임중인지 Check
		CGameDoc *pDoc = GetDocument();
		pDoc->SetMode(PVP);
		pDoc->UpdateGrade();
		Invalidate();
		OnTimerStop();
		OnNewPvpGame();
	}

	void CGameView::OnGradeTop(){ // Practice Top 모드
		if(IsNetworkGame()) return; // 네트워크 게임중인지 Check
		CGameDoc *pDoc = GetDocument();
		pDoc->SetMode(PRACTICE);
		pDoc->SetGrade(TOP);
		pDoc->UpdateGrade();
		Invalidate();
		OnTimerStop();
		OnTimerStart();
	}

	void CGameView::OnGradeMiddel(){ // Practice Middle 모드
		if(IsNetworkGame()) return; // 네트워크 게임중인지 Check
		CGameDoc *pDoc = GetDocument();
		pDoc->SetMode(PRACTICE);
		pDoc->SetGrade(MIDDLE);
		pDoc->UpdateGrade();
		Invalidate();
		OnTimerStop();
		OnTimerStart();
	}

	void CGameView::OnGradeLow(){ // Practice Low 모드
		if(IsNetworkGame()) return; // 네트워크 게임중인지 Check
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
		if(IsNetworkGame()) return; // 네트워크 게임중인지 Check
		CGameDoc *pDoc = GetDocument();
		pDoc->SetType(DOOLY);
		pDoc->InitializeGame();
		Invalidate();
		OnTimerStop();
		OnTimerStart();
	}
	
	void CGameView::OnTypePocketmon(){
		if(IsNetworkGame()) return; // 네트워크 게임중인지 Check
		CGameDoc *pDoc = GetDocument();
		pDoc->SetType(POCKETMON);
		pDoc->InitializeGame();
		Invalidate();
		OnTimerStop();
		OnTimerStart();
	}

	void CGameView::OnTypeOnepiece(){
		if(IsNetworkGame()) return; // 네트워크 게임중인지 Check
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
			MessageBox(_T("게임중이 아니므로 사용 할 수 없습니다."),_T("확인"),MB_OK);
			return;
		}
	}

	void CGameView::OnPvpGiveup(){
		if(m_pGame->GetGameProcess() != GAME_PROCESS_READY){
			m_pGame->GiveUpGame();
		}else{
			MessageBox(_T("게임중이 아니므로 사용 할 수 없습니다."),_T("확인"),MB_OK);
			return;
		}
	}
