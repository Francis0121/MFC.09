#include "stdafx.h"
#include "Game.h"
#include "DataSocket.h"
#include "Data.h"

// CDataSocket

CDataSocket::CDataSocket()
{
	m_pWnd = NULL;
	m_pFile = NULL;
	m_pArchiveIn = NULL;
	m_pArchiveOut = NULL;
}

CDataSocket::~CDataSocket()
{
	if(m_pFile != NULL) {
		delete m_pFile;
		m_pFile = NULL;
	}
	if(m_pArchiveIn != NULL) {
		m_pArchiveIn->Abort();
		delete m_pArchiveIn;
		m_pArchiveIn = NULL;
	}
	if(m_pArchiveOut != NULL) {
		m_pArchiveOut->Abort();
		delete m_pArchiveOut;
		m_pArchiveOut = NULL;
	}	
}

// CDataSocket member functions

void CDataSocket::Init(CWnd *pWnd)
{
	m_pWnd = pWnd;
	m_pFile = new CSocketFile(this);
	m_pArchiveIn = new CArchive(m_pFile, CArchive::load);
	m_pArchiveOut = new CArchive(m_pFile, CArchive::store);
}

void CDataSocket::Receive(CData * pData)
{
	if(m_pArchiveIn != NULL)
		pData->Serialize(*m_pArchiveIn);
}

void CDataSocket::operator >>(CData & data)
{
	Receive (&data);
}

void CDataSocket::Send(CData * pData)
{
	if(m_pArchiveOut != NULL)
		pData->Serialize(*m_pArchiveOut);
}

void CDataSocket::Flush()
{
	if(m_pArchiveOut != NULL)
		m_pArchiveOut->Flush();
}

void CDataSocket::operator <<(CData & data)
{
	Send (&data);
	Flush();
}

void CDataSocket::OnReceive(int nErrorCode) 
{
	m_pWnd->SendMessage(UM_DATARECEIVE);
	CSocket::OnReceive(nErrorCode);
}

void CDataSocket::OnClose(int nErrorCode) 
{
	if(m_pFile != NULL) {
		delete m_pFile;
		m_pFile = NULL;
	}
	if(m_pArchiveIn != NULL) {
		m_pArchiveIn->Abort();
		delete m_pArchiveIn;
		m_pArchiveIn = NULL;
	}
	if(m_pArchiveOut != NULL) {
		m_pArchiveOut->Abort();
		delete m_pArchiveOut;
		m_pArchiveOut = NULL;
	}	
	
	CSocket::OnClose(nErrorCode);
}
