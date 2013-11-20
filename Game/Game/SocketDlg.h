#pragma once
#include "afxwin.h"
#include "Data.h"
#include "ServerSocket.h"
#include "DataSocket.h"

// CSocketDlg ��ȭ �����Դϴ�.

class CSocketDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSocketDlg)

public:
	CSocketDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSocketDlg();

// ��ȭ ���� �������Դϴ�.
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
	CString m_strMes; // ä�� Message
	CString m_strIpAddr; // Ip �ּ�
	CString m_strNetStatus; // ���� ����
	CString m_strName; // ����� ��
	CString m_strPlayMateName; // ���� ����� ��

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnClose();

	afx_msg LRESULT OnAccept(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceive(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};
