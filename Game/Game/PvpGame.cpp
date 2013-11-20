// PvpGame.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Game.h"
#include "PvpGame.h"
#include "afxdialogex.h"
#include "GameView.h"
// CPvpGame ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPvpGame, CDialogEx)

CPvpGame::CPvpGame(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPvpGame::IDD, pParent)
{
	m_enGameProcess = GAME_PROCESS_READY;
	m_strMes = _T("");
	m_nPlayMateMatchCount = 0;
}

CPvpGame::~CPvpGame()
{
}

void CPvpGame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHAT_LIST, m_ctrList);
	DDX_Text(pDX, IDC_EDIT_CHAT_MSG, m_strMes);
}

BEGIN_MESSAGE_MAP(CPvpGame, CDialogEx)
	ON_MESSAGE(UM_ACCEPT, OnAccept)
	ON_MESSAGE(UM_DATARECEIVE, OnReceive)
	ON_BN_CLICKED(IDC_BUTTON_CHAT_SEND, &CPvpGame::OnBnClickedButtonChatSend)
END_MESSAGE_MAP()

// CPvpGame �޽��� ó�����Դϴ�.

BOOL CPvpGame::Create(CWnd* pParentWnd){
	m_ServerSocket.Init(this, m_nPortNum);
	m_pParentWnd = pParentWnd;
	return CDialog::Create(IDD, pParentWnd);
}

bool CPvpGame::SetNewGame(
		CString strIPAddr, CString strName, 
		CString strPlayMateName){
	m_strIpAddr = strIPAddr;
	m_strName = strName;
	m_strPlayMateName = strPlayMateName;
	return true;
}

enGameProcess CPvpGame::GetGameProcess() const{
	return m_enGameProcess;
}

enGameOrder CPvpGame::GetGameOrder() const{
	return m_enGameOrder;
}

bool CPvpGame::Start(){
	m_bNetwork = FALSE;
	m_bIReqGame = TRUE;
	
	m_DataSocket.Close(); //�������̸� ������ ����
	
	if(!m_DataSocket.Create()) {// ���� ����
		MessageBox(_T("Fail to create socket"),_T("Ȯ��"),MB_OK);
		return false;
	}

	if(!m_DataSocket.Connect(m_strIpAddr, m_nPortNum)) {// ���� ��û
		MessageBox(_T("�ش� IP�� ������ �� �����ϴ�. \r\nIP�Ǵ� ���� ���α׷� ������¸� Ȯ���ϼ���."),_T("Ȯ��"),MB_OK);
		return false;
	}
	m_DataSocket.Init(this);
	m_bNetwork = TRUE;	
	return true;
}

//Disconnect �Ҹ��ڿ��� ȣ����..
BOOL CPvpGame::Disconnect(){
	if(m_bNetwork)
	{
		CData data;
		data.SetGameMode(MODE_CLOSE);
		m_DataSocket << data;
		m_bNetwork = FALSE;
		m_DataSocket.Close();
		return TRUE;
	}
	return FALSE;
}

LRESULT CPvpGame::OnAccept(WPARAM wParam, LPARAM lParam){
	m_bNetwork = FALSE;
	m_bIReqGame = FALSE;
	
	m_DataSocket.Close(); //�������̸� ������ ����

	// ���� ��û�� �޾Ƶ���
	if(!m_ServerSocket.Accept(m_DataSocket)){
		MessageBox(_T("Fail to accept"), _T("Ȯ��"),MB_OK);
		return 0L;
	}
	
	m_DataSocket.Init(this);// ������ ���� �ʱ�ȭ
	m_bNetwork = TRUE; //��Ʈ��ũ ���� ����	

	//���ӿ�û�� ���� ���� �޼����� ������ 
	CData data;
	data.SetGameMode(MODE_CONNECT);
	data.m_strName = m_strName;
	m_DataSocket << data;

	return 0L;
}

LRESULT CPvpGame::OnReceive(WPARAM wParam, LPARAM lParam){
	// ������ ����
	CData data;
	CData data2;
	CString strTemp;
	m_DataSocket>>data;

	CGameView *pView = (CGameView *)m_pParentWnd;
	CGameDoc *pDoc = pView -> GetDocument();
	int nCount = 0;
	int total = pView->nMatchCount + data.m_nMatchCount;

	switch(data.GetGameMode())
	{
	//���� 
	case  MODE_CONNECT:
		//���� ��û�� �Ѱ��
		if(m_bIReqGame==TRUE){
			data2.SetGameMode(MODE_CONNECT);
			data2.m_strName = m_strName;
			m_nImageType = pDoc->GetRandomImage();
			data2.m_nImageType = m_nImageType;
			for(int i=0; i<36; i++){
				data2.m_nRnd[i] = pDoc->m_nRnd[i];
			}
			m_DataSocket << data2;
			m_strPlayMateName = data.m_strName;
			UpdateData(FALSE);
			MessageBox(m_strName+_T("��! ���� �ϼ���."), _T("Ȯ��"), MB_OK);
			m_enGameOrder = GAME_ORDER_I;
		}else if(m_bIReqGame==FALSE){
			m_bNetwork = TRUE;
			m_strPlayMateName = data.m_strName;
			m_nImageType = data.m_nImageType;
			for(int i=0; i<36; i++){
				pDoc->m_nRnd[i] = data.m_nRnd[i];
			}
			UpdateData(FALSE);
			MessageBox(m_strPlayMateName+"���� �����մϴ�.", _T("Ȯ��"), MB_OK);
			m_enGameOrder = GAME_ORDER_PLAYMATE;
		}
		
		// �� �ʱ�ȭ
		for(int n = 0; n < pDoc->m_nRow; n++){
			for(int m = 0; m<pDoc->m_nCol; m++){
				pDoc->m_nBmpID[n][m] = pDoc->m_nRnd[nCount];
				nCount++;
			}
		}
		pDoc->SetType(m_nImageType);
		pDoc->m_bRandom = false;
		pView->isStatus = START;
		pView->nMatchCount = 0;
		pView->Invalidate();
		m_enGameProcess = GAME_PROCESS_PLAYING;
		break;
	
	case MODE_CHAT:
		ShowWindow(SW_SHOW);
		strTemp = m_strPlayMateName+" >> "+data.m_strMes;
		AddChat(strTemp);
		break;
	
	case MODE_MATE_TURN:
		pDoc -> m_bShow[data.m_nRowIndex][data.m_nColIndex] = true;
		pView -> Invalidate();
		break;

	case MODE_MATE_TURN_FINISH:
		pDoc -> m_bShow[data.m_nRowIndex][data.m_nColIndex] = false;
		pDoc -> m_bShow[data.tempRowIndex][data.tempColIndex] = false;
		pView -> Invalidate();
		m_enGameOrder = GAME_ORDER_I;
		break;

	case MODE_MY_TURN:
		m_nPlayMateMatchCount = data.m_nMatchCount;
		if(total == (pDoc->m_nRow * pDoc->m_nCol)/2){
			if(pView->nMatchCount > data.m_nMatchCount){
				MessageBox(m_strName+_T("�� �¸��Ͽ����ϴ�!"));
			}else{
				MessageBox(m_strName+_T("�� �й��Ͽ����ϴ�."));
			}
			data.SetGameMode(MODE_MY_TURN);
			data.m_nMatchCount = pView -> nMatchCount;
			m_DataSocket << data;
			m_DataSocket.Close();
			m_bNetwork = FALSE;	
			m_enGameProcess = GAME_PROCESS_READY;
			m_nPlayMateMatchCount = 0;
			UpdateData(FALSE);
			SendMessage(WM_CLOSE,0,0);
		}
		break;
	case MODE_GIVE_UP:
		MessageBox(m_strPlayMateName+_T("�Բ��� ����Ͽ����ϴ�."), _T("Ȯ��"), MB_OK);
		m_DataSocket.Close();
		m_bNetwork = FALSE;	
		m_enGameProcess = GAME_PROCESS_READY;
		UpdateData(FALSE);
		for(int n = 0; n < pDoc->m_nRow; n++)
			for(int m = 0; m<pDoc->m_nCol; m++)
				pDoc->m_nBmpID[n][m] = true;
		pView->isStatus = READY;
		pView->Invalidate();
		SendMessage(WM_CLOSE,0,0);
		break;

	case MODE_CLOSE:
		MessageBox(m_strPlayMateName+_T("�Բ��� ��Ʈ���� ������׽��ϴ�."), _T("Ȯ��"), MB_OK);
		m_DataSocket.Close();
		m_bNetwork = FALSE;	
		m_enGameProcess = GAME_PROCESS_READY;
		UpdateData(FALSE);
		SendMessage(WM_CLOSE,0,0);
		break;
		
	}
	return 0L;
}

void CPvpGame::AddChat(CString strChat){
	m_ctrList.AddString(strChat);
	m_ctrList.SetCurSel(m_ctrList.GetCount()-1);
}

void CPvpGame::OnShowWindow(BOOL bShow, UINT nStatus) {
	CDialog::OnShowWindow(bShow, nStatus);
	this->SetWindowPos(&wndTopMost,0,0,0,0, SWP_NOMOVE | SWP_NOSIZE );	
}

void CPvpGame::ShowClickImage(){
	CGameView *pView = (CGameView *)m_pParentWnd;
	CGameDoc *pDoc = pView -> GetDocument();
	CData data;
	data.SetGameMode(MODE_MATE_TURN);
	data.m_nColIndex = pDoc->m_nColIndex;
	data.m_nRowIndex = pDoc->m_nRowIndex;
	m_DataSocket << data;
}

void CPvpGame::HideClickImage(int row, int col, int row2, int col2){
	CData data;
	data.SetGameMode(MODE_MATE_TURN_FINISH);
	data.m_nColIndex = col;
	data.m_nRowIndex = row;
	data.tempColIndex = col2;
	data.tempRowIndex = row2;
	m_DataSocket << data;

	m_enGameOrder = GAME_ORDER_PLAYMATE;
}

void CPvpGame::MatchingIncCount(){
	CGameView *pView = (CGameView *)m_pParentWnd;
	CGameDoc *pDoc = pView -> GetDocument();
	CData data;
	data.SetGameMode(MODE_MY_TURN);
	data.m_nMatchCount = pView -> nMatchCount;
	m_DataSocket << data;
}

void CPvpGame::OnBnClickedButtonChatSend(){
	if(m_bNetwork==FALSE) return;
	
	CString strTemp;
	CData data;
	data.SetGameMode(MODE_CHAT);
	UpdateData(TRUE);
	m_strMes.TrimRight();
	m_strMes.TrimLeft();
	if(!m_strMes.IsEmpty())
	{
		data.m_strMes = m_strMes;
		m_DataSocket << data;
		strTemp = m_strName+" >> "+m_strMes;
		AddChat(strTemp);
		m_strMes.Empty();
		UpdateData(FALSE);
	}	
}

void CPvpGame::GiveUpGame(){
	CData data;
	data.SetGameMode(MODE_GIVE_UP);
	m_DataSocket << data;
	m_DataSocket.Close();
	m_bNetwork = FALSE;	
	m_enGameProcess = GAME_PROCESS_READY;
	UpdateData(FALSE);
	CGameView *pView = (CGameView *)m_pParentWnd;
	CGameDoc *pDoc = pView -> GetDocument();
	for(int n = 0; n < pDoc->m_nRow; n++)
		for(int m = 0; m<pDoc->m_nCol; m++)
			pDoc->m_nBmpID[n][m] = true;
	pView->isStatus = READY;
	pView->Invalidate();
	SendMessage(WM_CLOSE,0,0);
}


BOOL CPvpGame::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE){	//ESC Ű�� ������ â�� ������ �ʵ���
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return true;
		}else if(pMsg->wParam == VK_RETURN){ // ENTER Ű�� ������ MESSAGE ����
			OnBnClickedButtonChatSend();
			return true;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
