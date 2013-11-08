
// GameDoc.cpp : CGameDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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


	// CGameDoc ����/�Ҹ�

	CGameDoc::CGameDoc() : m_strName(_T(""))
	{
		m_bmCell = CSize(79, 81);
		m_nRow = 5;
		m_nCol = 6;
		m_nMode = PRACTICE;
		m_nGrade = TOP;
		m_bRandom = true;
		m_nType = POCKETMON;

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

		// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
		// SDI ������ �� ������ �ٽ� ����մϴ�.

		return TRUE;
	}




	// CGameDoc serialization

	void CGameDoc::Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{
			// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
		}
		else
		{
			// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
		}
	}

#ifdef SHARED_HANDLERS

	// ����� �׸��� �����մϴ�.
	void CGameDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
	{
		// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
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

	// �˻� ó���⸦ �����մϴ�.
	void CGameDoc::InitializeSearchContent()
	{
		CString strSearchContent;
		// ������ �����Ϳ��� �˻� �������� �����մϴ�.
		// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

		// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
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

	// CGameDoc ����

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


	// CGameDoc ���
	void CGameDoc::UpdateGrade(void){
		if(m_nMode == PRACTICE){
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
		}else{
			m_nRow = 2;
			m_nCol = 3;
		}

		InitializeGame();
	}

	void CGameDoc::InitializeGame(void){

		m_bRandom = true;
		for(int n=0; n<m_nRow; n++)
			for(int m=0; m<m_nCol; m++)
				m_bShow[n][m] = false;	

		m_nBmpFirstID = m_nBmpSecondID = 0;
		m_bMouse = false;
		
		if(m_nMode == PRACTICE || (m_nMode == RANKING && m_nCol == 3)){
			m_nH = m_nM = m_nS = m_nTimeSet = 0;
		}
	}

	void CGameDoc::ResizeWindow(void)
	{

		CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

		CREATESTRUCT st;// WB_CREATE �޽��� �߻��� ���� �ü���� ��ǥ, �޴�, �ν��Ͻ� �ڵ�Ŭ���� �̸� ���� ��Ÿ���� �Ѱ��ִ°�
		CREATESTRUCT& rst = st;

		pMain->PreCreateWindow(rst);

		rst.lpszName = _T("Game");
		rst.cx = (m_nRow+1) * m_bmCell.cx + 180;
		rst.cy = (m_nCol+1) * m_bmCell.cy + 100;

		RECT rcTemp ;
        LONG x, y;

        GetWindowRect(GetDesktopWindow(), &rcTemp) ;
   
        x = LONG((rcTemp.right - rst.cx) / 2) ;
		y = LONG((rcTemp.bottom - rst.cy) / 2) ;

		pMain->MoveWindow(x, y, rst.cx, rst.cy);
	}

	void CGameDoc::OnRandom(void){
		//�ð��� Seed�� ��� �Ȱ��� ���� �߻��� ����
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
	
	void CGameDoc::OnWriteScoreFile(void){

		CStdioFile file; 
		CFileException e;
		CFileStatus status;
		TCHAR* pszFileName = _T("score");

		if(CFile::GetStatus(pszFileName, status)){//������ �����ϴ� ��� �о���⸸��
			if(!(file.Open(pszFileName, CFile::modeReadWrite | CFile::shareDenyNone, &e))) { 
				TRACE( _T("Can't open file %s, error = %u\n"), pszFileName, e.m_cause );
				return;
			} 
		}else{//������ �������� �ʴ´ٸ� �����Ѵ�
			if(!(file.Open(pszFileName, CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone, &e))) { 
				TRACE( _T("Can't open file %s, error = %u\n"), pszFileName, e.m_cause );
				return;
			} 
		}

		file.SeekToEnd(); // ���� ������ �̵��Ѵ�.

		CString time;
		time.Format(_T("%d:%d:%d:%d"),  m_nH, m_nM, m_nS, m_nTimeSet);

		file.WriteString(m_strName);
		file.WriteString(_T("\t"));
		file.WriteString(time);
		file.WriteString(_T("\n"));

		file.Close();
	}

	// ~ Getter Setter
	int CGameDoc::GetGrade(){
		return m_nGrade;
	}

	void CGameDoc::SetGrade(int grade){
		m_nGrade = grade;
	}
	
	CString CGameDoc::GetName(void){
		return m_strName;
	}
	
	void CGameDoc::SetName(CString name){
		m_strName = name;
	}

	void CGameDoc::SetMode(int mode){
		m_nMode = mode;
	}
	
	int CGameDoc::GetMode(){
		return m_nMode;
	}

	int CGameDoc::GetType(){
		return m_nType;
	}
	
	void CGameDoc::SetType(int type){
		m_nType = type;
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