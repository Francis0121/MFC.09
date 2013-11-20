#pragma once
#include "afxwin.h"
#include "Data.h"
#include "ServerSocket.h"
#include "DataSocket.h"

// CSocketDlg 대화 상자입니다.

class CSocketDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSocketDlg)

public:
	CSocketDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSocketDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SOCKET };

private:
	int	m_nPortNum;
	BOOL m_bNetwork;
	CDataSocket m_DataSocket;
	CServerSocket m_ServerSocket;

	void AddChat(CString strChat);
	void EnableWindowConnect(BOOL bEnableConnectWindow);

public:
	CWnd *m_pParent;
	CEdit m_ctrMes;
	CListBox m_ctrMesList;

	BOOL m_bIReqGame;
	CString m_strMes; // 채팅 Message
	CString m_strIpAddr; // Ip 주소
	CString m_strNetStatus; // 연결 상태
	CString m_strName; // 사용자 명
	CString m_strPlayMateName; // 상대방 사용자 명

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnClose();

	afx_msg LRESULT OnAccept(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceive(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};
