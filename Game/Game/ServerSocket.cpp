#include "stdafx.h"
#include "Game.h"
#include "ServerSocket.h"

CServerSocket::CServerSocket()
{
}

CServerSocket::~CServerSocket()
{
}

void CServerSocket::Init(CWnd *pWnd, int nPortNum){
	m_pWnd = pWnd;

	Create(nPortNum);
	Listen();
}

void CServerSocket::OnAccept(int nErrorCode) {
	m_pWnd->SendMessage(UM_ACCEPT);
	
	CSocket::OnAccept(nErrorCode);
}

