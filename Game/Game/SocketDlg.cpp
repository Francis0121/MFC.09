#include "stdafx.h"
#include "Game.h"
#include "SocketDlg.h"
#include "afxdialogex.h"

#include "GameDoc.h"
#include "GameView.h"

// CSocketDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSocketDlg, CDialogEx)

CSocketDlg::CSocketDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSocketDlg::IDD, pParent){
	m_strIpAddr = _T("");
	m_strMes = _T("");
	m_strNetStatus = _T("대기중");
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

// CSocketDlg 메시지 처리기입니다.

BOOL CSocketDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_bIReqGame = FALSE; // 내가 게임요청하면 TRUE 
	m_bNetwork = FALSE;	// 네트워크 연결 상태
	EnableWindowConnect(TRUE);
	m_ServerSocket.Init(this, m_nPortNum); // Socket 초기화	
	
	return TRUE; 
}

BOOL CSocketDlg::PreTranslateMessage(MSG* pMsg){
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE){	//ESC 키를 눌러도 창이 꺼지지 않도록
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return true;
		}else if(pMsg->wParam == VK_RETURN){ // ENTER 키를 누르면 MESSAGE 전송
			OnBnClickedButtonSend();
			return true;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSocketDlg::AddChat(CString strChat){
	m_ctrMesList.AddString(strChat); // 채팅 문자열 추가
	m_ctrMesList.SetCurSel(m_ctrMesList.GetCount()-1); // 현재 선택된 위치변경
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
	m_strNetStatus = _T("대기중");
	m_DataSocket.Close(); //게임중이면 종료후 연결

	if(!m_ServerSocket.Accept(m_DataSocket)){// 접속 요청을 받아들임
		MessageBox(_T("Fail to accept"), _T("확인"), MB_OK);
		return 0L;
	}
	
	m_DataSocket.Init(this); // 데이터 소켓 초기화
	m_bNetwork = TRUE; 	//네트워크 모드로 설정
	m_strNetStatus = _T("접속");
	return 0L;
}

LRESULT CSocketDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{
	// 데이터 수신
	CData data;
	CData data2;
	CString strTemp;
	m_DataSocket >> data;
	
	switch(data.GetGameMode())
	{
	case  MODE_CONNECT: // 게임 연결
		//게임 요청을 한경우
		if(m_bIReqGame==TRUE){
			
			if(data.m_strName=="" ){//받은 ID가 없다는 것을 게임수락을 거절
				MessageBox(_T("대전요청을 거절당했습니다."), _T("확인"),MB_OK);
				EnableWindowConnect(TRUE);
				m_DataSocket.Close();
				m_bNetwork = FALSE;
				m_strNetStatus = _T("대기중");
				return 0L;
			}else{//받은 아이디가 있는 경우 게임수락
				m_bNetwork = TRUE;
				m_strNetStatus = _T("접속");
				m_strPlayMateName = data.m_strName;
				EnableWindowConnect(FALSE);
				m_ctrMesList.ResetContent();
				strTemp = _T("'") + m_strPlayMateName + _T("' 님이 접속했습니다.");
				AddChat(strTemp);
				UpdateData(FALSE);
			}
		}

		//게임 요청받은 경우
		if(m_bIReqGame==FALSE){
		
			if(MessageBox(_T("대전요청을 수락하시겠습니까?"), _T("확인"), MB_YESNO) == IDYES){
				// 대전 요청 수락
				data2.SetGameMode(MODE_CONNECT);
				data2.m_strName = m_strName;
				m_DataSocket << data2;

				EnableWindowConnect(FALSE);
				m_ctrMesList.ResetContent(); 
				m_strPlayMateName = data.m_strName;
				strTemp = _T("'") + m_strPlayMateName + _T("' 님이 접속했습니다.");
				AddChat(strTemp);
				m_strNetStatus = _T("접속");

				UpdateData(FALSE);
			}else{
				// 대전 요청 거절
				data2.SetGameMode(MODE_CONNECT);
				data2.m_strName = _T("");
				m_DataSocket << data2;
				m_DataSocket.Close();//소켓 종료

				EnableWindowConnect(TRUE);
				m_bNetwork = FALSE;
				m_strNetStatus = _T("대기중");
			}
		}
		break;

	case MODE_CHAT: // Chatting Receive
		strTemp = m_strPlayMateName+" >> "+data.m_strMes;
		AddChat(strTemp);
		break;

	case MODE_GAME_START: // 게임시작
		data.SetGameMode(MODE_GAME_START);
		m_DataSocket << data;
		m_DataSocket.Close();
		EndDialog(IDOK);
		break;

	case MODE_CLOSE: // 게임종료
		MessageBox(_T("상대측에서 네트웍을 종료시켰습니다."), _T("확인"),MB_OK);
		m_DataSocket.Close();
		m_bNetwork = FALSE;	
		m_strNetStatus = _T("대기중");
		m_strPlayMateName = _T("");
		AddChat(_T("접속이 종료되었습니다"));
		UpdateData(FALSE);
		EnableWindowConnect(TRUE);
		break;
	}
	return 0L;
}

void CSocketDlg::OnBnClickedButtonConnect(){
	m_bNetwork = FALSE;
	m_strNetStatus = _T("대기중");
	m_DataSocket.Close();  //게임중이면 종료후 연결
	
	if(!m_DataSocket.Create()) { // 소켓 생성
		MessageBox(_T("Fail to create socket"), _T("확인"), MB_OK);
		SendMessage(WM_CLOSE,0,0);
		return;
	}

	AddChat(_T("접속요청했습니다. 잠시 기다리세요")); // 접속 요청
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
	UpdateData(TRUE);
	CString strTemp;
	strTemp.Format(m_strIpAddr+_T(" %d"), m_nPortNum);
	AddChat(strTemp);
	if(!m_DataSocket.Connect(m_strIpAddr, m_nPortNum)) {
		MessageBox(_T("해당 IP에 접속할 수 없습니다. \r\nIP또는 상대방 프로그램 실행상태를 확인하세요."), _T("확인"), MB_OK);
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
		return;
	}
	m_DataSocket.Init(this);

	// 게임을 수락하는지 확인
	CData data;
	data.SetGameMode(MODE_CONNECT);
	data.m_strName = m_strName;
	m_DataSocket << data;
	m_bIReqGame = TRUE;
	m_ctrMesList.ResetContent();
	EnableWindowConnect(FALSE);
	
	m_bNetwork = TRUE;
	m_strNetStatus = _T("접속");
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
		MessageBox(_T("네트워크에 연결되어 있지 않습니다."), _T("확인"),MB_OK);
		return;
	}

	CData data;
	data.SetGameMode(MODE_CHAT); // 채팅모드로 변경후
	UpdateData(TRUE); // 컨트롤에 입력된 값을 멤버 변수에 저장하고 유효성 체크
	m_strMes.TrimRight(); // removes trailing newline, space, and tab characters from the string.
	m_strMes.TrimLeft(); // removes newline, space, and tab characters.
	
	if(!m_strMes.IsEmpty()){
		data.m_strMes = m_strMes; // socket 메시지 변경
		m_DataSocket << data; // socket 전송 
		AddChat(m_strName+" >> "+m_strMes); // Message를 List에 추가한다.
		m_strMes.Empty(); // m_strMes 를 빈 공간으로 한후 
		UpdateData(FALSE); // 그 값을 컨트롤에 반영한다.
	}
}

//Game 종료
void CSocketDlg::OnBnClickedButtonExit(){
	// 종료 Message Send
	CData data;
	data.SetGameMode(MODE_CLOSE);
	m_DataSocket << data;
	m_DataSocket.Close();
	EndDialog(IDOK);
}

void CSocketDlg::OnClose(){
	OnBnClickedButtonExit();  // [종료] 버튼이 눌렸을 때 호출되는 메시지 핸들러 호출
}