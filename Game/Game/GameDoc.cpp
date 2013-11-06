
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

	CGameDoc::CGameDoc(){
		m_bmCell = CSize(79, 81);
		m_nRow = 5;
		m_nCol = 6;
		m_nGrade = TOP; 
		m_bRandom = true;

		for(int n = 0; n < m_nRow; n++)
			for(int m = 0; m < m_nCol; m++)
				m_bShow[n][m] = false;

		m_nBmpFirstID = m_nBmpSecondID = 0;
		m_bMouse = false;

		m_nH = m_nM = m_nS = m_nTimeSet = 0;
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
	void CGameDoc::UpdateGrade(void){
		switch (m_nGrade)
		{
		case LOW:
			m_nRow = 3;
			m_nCol = 4;
			break;	
		case MIDDLE:
			m_nRow = 4;
			m_nCol = 5;
			break;
		case TOP:
			m_nRow = 5;
			m_nCol = 6;
			break;
		}

		InitializeGame();
	}

	void CGameDoc::InitializeGame(void){
		m_bRandom = true;
		for(int n=0; n<m_nRow; n++)
			for(int m=0; m<m_nCol; m++)
				m_bShow[n][m] = false;	

		m_nBmpFirstID = m_nBmpSecondID =0;
		m_bMouse = false;
		
		m_nH = m_nM = m_nS = m_nTimeSet = 0;
	}

	void CGameDoc::ResizeWindow(void)
	{

		CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

		CREATESTRUCT st;// WB_CREATE 메시지 발생시 윈도 운영체제가 좌표, 메뉴, 인스턴스 핸들클래스 이름 윈도 스타일을 넘겨주는곳
		CREATESTRUCT& rst = st;

		pMain->PreCreateWindow(rst);

		rst.lpszName = _T("Game");
		rst.cx = (m_nRow+1) * m_bmCell.cx + 180;
		rst.cy = (m_nCol+1) * m_bmCell.cy + 100;

		CRect rect;
		pMain->GetClientRect(rect);
		pMain->MoveWindow(rect.left+200, rect.top+100, rst.cx, rst.cy);
	}

	void CGameDoc::OnRandom(void){
		//시간을 Seed로 잡아 똑같은 난수 발생을 방지
		srand((unsigned)time(NULL));

		BOOL bInsert = TRUE;
		int nGrating = m_nRow * m_nCol;

		for(int n=0; n < nGrating; n++){
			m_nRnd[n] = (rand()%(nGrating/2))+1;

			for(int m = 0; m<n; m++){
				if(m_nRnd[n] == m_nRnd[m]){
					if(bInsert == FALSE){
						n--;
						bInsert = TRUE;
						break;
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

	// ~ Getter Setter
	int CGameDoc::GetGrade(){
		return m_nGrade;
	}

	void CGameDoc::SetGrade(int grade){
		m_nGrade = grade;
	}

	bool CGameDoc::GetRandom(){
		return m_bRandom;
	}

	void CGameDoc::SetRandom(bool random){
		m_bRandom = random;
	}

	int CGameDoc::GetHour(void){
		return m_nH;
	}
	void CGameDoc::SetHour(int hour){
		m_nH = hour;
	}
	int CGameDoc::GetMinute(void){
		return m_nM;
	}
	void CGameDoc::SetMinute(int minute){
		m_nM = minute;
	}
	int CGameDoc::GetSecond(void){
		return m_nS;
	}
	void CGameDoc::SetSecond(int second){
		m_nS = second;
	}
	int CGameDoc::GetTimeset(void){
		return m_nTimeSet;
	}
	void CGameDoc::SetTimeset(int timeset){
		m_nTimeSet = timeset;
	}