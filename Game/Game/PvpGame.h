#pragma once
#include "Data.h"
#include "ServerSocket.h"
#include "DataSocket.h"
#include "afxwin.h"
#include "GameDoc.h"

// CPvpGame ��ȭ �����Դϴ�.
//����������� 
enum enGameProcess
{
	GAME_PROCESS_READY = 0,	//�����غ���
	GAME_PROCESS_PLAYING,		//������
	GAME_PROCESS_OVER,		//���ӿ���
};

//���� ����
enum enGameOrder
{
	GAME_ORDER_I = 0,
	GAME_ORDER_PLAYMATE,
};

class CPvpGame : public CDialogEx
{
	DECLARE_DYNAMIC(CPvpGame)

public:
	CPvpGame(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPvpGame();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_PVP };

	CString m_strIpAddr;				// ���� IPAddress
	CString m_strName;					// ���� ID
	CString m_strPlayMateName;			// ���� ID
	int m_nPlayMateMatchCount;

	BOOL	m_bNetwork;					// ��Ʈ��ũ�� ����� ���� 
	BOOL	m_bIReqGame;				// ���� ������ ��û�� ��� TRUE
	int		m_nPortNum;					// Port��ȣ
	CDataSocket m_DataSocket;			
	CServerSocket m_ServerSocket;	

	enGameProcess m_enGameProcess;		// ���ӻ��� 
	enGameOrder m_enGameOrder;			// ���Ӽ���
	int m_nImageType;		// �����̹���Ÿ��

	CListBox m_ctrList;					// Dialog Chatting listBox
	CString m_strMes;					// Dialog Message editBox

	CWnd* m_pParentWnd;					// �θ� CWnd Pointer

public:
	bool SetNewGame(CString strIpAddr, CString strName, CString strPlayMateName);
	bool Start();
	void AddChat(CString strChat);
	void OnShowWindow(BOOL bShow, UINT nStatus);
	BOOL Disconnect();
	enGameProcess CPvpGame::GetGameProcess() const;
	enGameOrder CPvpGame::GetGameOrder() const;
	virtual BOOL Create(CWnd* pParentWnd);
	void ShowClickImage();
	void HideClickImage(int row, int col, int row2, int col2);
	void MatchingIncCount();
	void GiveUpGame();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg LRESULT OnAccept(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceive(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonChatSend();

	DECLARE_MESSAGE_MAP()
	
};
