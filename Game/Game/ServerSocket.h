#pragma once
#include "afxsock.h"
#define UM_ACCEPT (WM_USER+10)

class CServerSocket : public CSocket
{
public:
	CWnd *m_pWnd;

public:
	CServerSocket();
	virtual ~CServerSocket();
	
public:
	virtual void OnAccept(int nErrorCode);

	void Init(CWnd *pWnd, int nPortNum);
};
