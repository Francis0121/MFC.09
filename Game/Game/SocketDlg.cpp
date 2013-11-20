#include "stdafx.h"
#include "Game.h"
#include "SocketDlg.h"
#include "afxdialogex.h"

#include "GameDoc.h"
#include "GameView.h"

// CSocketDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSocketDlg, CDialogEx)

CSocketDlg::CSocketDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSocketDlg::IDD, pParent){
	m_strIpAddr = _T("");
	m_strMes = _T("");
	m_strNetStatus = _T("�����");
	m_nPortNum = 2013;
}

CSocketDlg::~CSocketDlg(){
}

void CSocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MESSAGE, m_ctrMesList);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_ctrMes);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strMes);
	DDX_Text(pDX, IDC_STATIC_NETWORK_STATUS, m_strNetStatus);
	DDX_Text(pDX, IDC_EDIT_IPADDR, m_strIpAddr);
}

BEGIN_MESSAGE_MAP(CSocketDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CSocketDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CSocketDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CSocketDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_START, &CSocketDlg::OnBnClickedButtonStart)
	
	ON_WM_CLOSE()

	ON_MESSAGE(UM_ACCEPT, OnAccept)
	ON_MESSAGE(UM_DATARECEIVE, OnReceive)
END_MESSAGE_MAP()

// CSocketDlg �޽��� ó�����Դϴ�.

BOOL CSocketDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_bIReqGame = FALSE; // ���� ���ӿ�û�ϸ� TRUE 
	m_bNetwork = FALSE;	// ��Ʈ��ũ ���� ����
	EnableWindowConnect(TRUE);
	m_ServerSocket.Init(this, m_nPortNum); // Socket �ʱ�ȭ	
	
	return TRUE; 
}

BOOL CSocketDlg::PreTranslateMessage(MSG* pMsg){
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE){	//ESC Ű�� ������ â�� ������ �ʵ���
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return true;
		}else if(pMsg->wParam == VK_RETURN){ // ENTER Ű�� ������ MESSAGE ����
			OnBnClickedButtonSend();
			return true;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSocketDlg::AddChat(CString strChat){
	m_ctrMesList.AddString(strChat); // ä�� ���ڿ� �߰�
	m_ctrMesList.SetCurSel(m_ctrMesList.GetCount()-1); // ���� ���õ� ��ġ����
}

void CSocketDlg::EnableWindowConnect(BOOL bEnableConnectWindow){
	GetDlgItem(IDC_EDIT_IPADDR)->EnableWindow(bEnableConnectWindow);
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(bEnableConnectWindow);
	
	if(m_bIReqGame==TRUE){
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(!bEnableConnectWindow);
	}else{
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	}
}

LRESULT CSocketDlg::OnAccept(WPARAM wParam, LPARAM lParam){
	m_bNetwork = FALSE;
	m_bIReqGame = FALSE;
	m_strNetStatus = _T("�����");
	m_DataSocket.Close(); //�������̸� ������ ����

	if(!m_ServerSocket.Accept(m_DataSocket)){// ���� ��û�� �޾Ƶ���
		MessageBox(_T("Fail to accept"), _T("Ȯ��"), MB_OK);
		return 0L;
	}
	
	m_DataSocket.Init(this); // ������ ���� �ʱ�ȭ
	m_bNetwork = TRUE; 	//��Ʈ��ũ ���� ����
	m_strNetStatus = _T("����");
	return 0L;
}

LRESULT CSocketDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{
	// ������ ����
	CData data;
	CData data2;
	CString strTemp;
	m_DataSocket >> data;
	
	switch(data.GetGameMode())
	{
	case  MODE_CONNECT: // ���� ����
		//���� ��û�� �Ѱ��
		if(m_bIReqGame==TRUE){
			
			if(data.m_strName=="" ){//���� ID�� ���ٴ� ���� ���Ӽ����� ����
				MessageBox(_T("������û�� �������߽��ϴ�."), _T("Ȯ��"),MB_OK);
				EnableWindowConnect(TRUE);
				m_DataSocket.Close();
				m_bNetwork = FALSE;
				m_strNetStatus = _T("�����");
				return 0L;
			}else{//���� ���̵� �ִ� ��� ���Ӽ���
				m_bNetwork = TRUE;
				m_strNetStatus = _T("����");
				m_strPlayMateName = data.m_strName;
				EnableWindowConnect(FALSE);
				m_ctrMesList.ResetContent();
				strTemp = _T("'") + m_strPlayMateName + _T("' ���� �����߽��ϴ�.");
				AddChat(strTemp);
				UpdateData(FALSE);
			}
		}

		//���� ��û���� ���
		if(m_bIReqGame==FALSE){
		
			if(MessageBox(_T("������û�� �����Ͻðڽ��ϱ�?"), _T("Ȯ��"), MB_YESNO) == IDYES){
				// ���� ��û ����
				data2.SetGameMode(MODE_CONNECT);
				data2.m_strName = m_strName;
				m_DataSocket << data2;

				EnableWindowConnect(FALSE);
				m_ctrMesList.ResetContent(); 
				m_strPlayMateName = data.m_strName;
				strTemp = _T("'") + m_strPlayMateName + _T("' ���� �����߽��ϴ�.");
				AddChat(strTemp);
				m_strNetStatus = _T("����");

				UpdateData(FALSE);
			}else{
				// ���� ��û ����
				data2.SetGameMode(MODE_CONNECT);
				data2.m_strName = _T("");
				m_DataSocket << data2;
				m_DataSocket.Close();//���� ����

				EnableWindowConnect(TRUE);
				m_bNetwork = FALSE;
				m_strNetStatus = _T("�����");
			}
		}
		break;

	case MODE_CHAT: // Chatting Receive
		strTemp = m_strPlayMateName+" >> "+data.m_strMes;
		AddChat(strTemp);
		break;

	case MODE_GAME_START: // ���ӽ���
		data.SetGameMode(MODE_GAME_START);
		m_DataSocket << data;
		m_DataSocket.Close();
		EndDialog(IDOK);
		break;

	case MODE_CLOSE: // ��������
		MessageBox(_T("��������� ��Ʈ���� ������׽��ϴ�."), _T("Ȯ��"),MB_OK);
		m_DataSocket.Close();
		m_bNetwork = FALSE;	
		m_strNetStatus = _T("�����");
		m_strPlayMateName = _T("");
		AddChat(_T("������ ����Ǿ����ϴ�"));
		UpdateData(FALSE);
		EnableWindowConnect(TRUE);
		break;
	}
	return 0L;
}

void CSocketDlg::OnBnClickedButtonConnect(){
	m_bNetwork = FALSE;
	m_strNetStatus = _T("�����");
	m_DataSocket.Close();  //�������̸� ������ ����
	
	if(!m_DataSocket.Create()) { // ���� ����
		MessageBox(_T("Fail to create socket"), _T("Ȯ��"), MB_OK);
		SendMessage(WM_CLOSE,0,0);
		return;
	}

	AddChat(_T("���ӿ�û�߽��ϴ�. ��� ��ٸ�����")); // ���� ��û
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
	UpdateData(TRUE);
	CString strTemp;
	strTemp.Format(m_strIpAddr+_T(" %d"), m_nPortNum);
	AddChat(strTemp);
	if(!m_DataSocket.Connect(m_strIpAddr, m_nPortNum)) {
		MessageBox(_T("�ش� IP�� ������ �� �����ϴ�. \r\nIP�Ǵ� ���� ���α׷� ������¸� Ȯ���ϼ���."), _T("Ȯ��"), MB_OK);
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
		return;
	}
	m_DataSocket.Init(this);

	// ������ �����ϴ��� Ȯ��
	CData data;
	data.SetGameMode(MODE_CONNECT);
	data.m_strName = m_strName;
	m_DataSocket << data;
	m_bIReqGame = TRUE;
	m_ctrMesList.ResetContent();
	EnableWindowConnect(FALSE);
	
	m_bNetwork = TRUE;
	m_strNetStatus = _T("����");
}

void CSocketDlg::OnBnClickedButtonStart(){
	if(m_bNetwork){
		UpdateData(TRUE);
		CData data;
		data.SetGameMode(MODE_GAME_START);
		m_DataSocket << data;
	}
}

void CSocketDlg::OnBnClickedButtonSend(){
	if(m_bNetwork==FALSE){
		MessageBox(_T("��Ʈ��ũ�� ����Ǿ� ���� �ʽ��ϴ�."), _T("Ȯ��"),MB_OK);
		return;
	}

	CData data;
	data.SetGameMode(MODE_CHAT); // ä�ø��� ������
	UpdateData(TRUE); // ��Ʈ�ѿ� �Էµ� ���� ��� ������ �����ϰ� ��ȿ�� üũ
	m_strMes.TrimRight(); // removes trailing newline, space, and tab characters from the string.
	m_strMes.TrimLeft(); // removes newline, space, and tab characters.
	
	if(!m_strMes.IsEmpty()){
		data.m_strMes = m_strMes; // socket �޽��� ����
		m_DataSocket << data; // socket ���� 
		AddChat(m_strName+" >> "+m_strMes); // Message�� List�� �߰��Ѵ�.
		m_strMes.Empty(); // m_strMes �� �� �������� ���� 
		UpdateData(FALSE); // �� ���� ��Ʈ�ѿ� �ݿ��Ѵ�.
	}
}

//Game ����
void CSocketDlg::OnBnClickedButtonExit(){
	// ���� Message Send
	CData data;
	data.SetGameMode(MODE_CLOSE);
	m_DataSocket << data;
	m_DataSocket.Close();
	EndDialog(IDOK);
}

void CSocketDlg::OnClose(){
	OnBnClickedButtonExit();  // [����] ��ư�� ������ �� ȣ��Ǵ� �޽��� �ڵ鷯 ȣ��
}