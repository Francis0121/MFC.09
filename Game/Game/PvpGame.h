#pragma once
#include "Data.h"
#include "ServerSocket.h"
#include "DataSocket.h"
#include "afxwin.h"
#include "GameDoc.h"

// CPvpGame 대화 상자입니다.
//게임진행상태 
enum enGameProcess
{
	GAME_PROCESS_READY = 0,	//게임준비중
	GAME_PROCESS_PLAYING,		//게임중
	GAME_PROCESS_OVER,		//게임오버
};

//게임 차례
enum enGameOrder
{
	GAME_ORDER_I = 0,
	GAME_ORDER_PLAYMATE,
};

class CPvpGame : public CDialogEx
{
	DECLARE_DYNAMIC(CPvpGame)

public:
	CPvpGame(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPvpGame();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PVP };

	CString m_strIpAddr;				// 상대방 IPAddress
	CString m_strName;					// 나의 ID
	CString m_strPlayMateName;			// 상대방 ID
	int m_nPlayMateMatchCount;

	BOOL	m_bNetwork;					// 네트워크가 연결된 상태 
	BOOL	m_bIReqGame;				// 내가 게임을 요청한 경우 TRUE
	int		m_nPortNum;					// Port번호
	CDataSocket m_DataSocket;			
	CServerSocket m_ServerSocket;	

	enGameProcess m_enGameProcess;		// 게임상태 
	enGameOrder m_enGameOrder;			// 게임순서
	int m_nImageType;		// 게임이미지타입

	CListBox m_ctrList;					// Dialog Chatting listBox
	CString m_strMes;					// Dialog Message editBox

	CWnd* m_pParentWnd;					// 부모 CWnd Pointer

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg LRESULT OnAccept(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceive(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonChatSend();

	DECLARE_MESSAGE_MAP()
	
};
