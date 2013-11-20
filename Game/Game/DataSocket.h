#pragma once
#include "afxsock.h"

#define UM_DATARECEIVE (WM_USER+11)
class CData;

class CDataSocket : public CSocket
{
public:
	CWnd		*m_pWnd;
	CSocketFile *m_pFile;
	CArchive	*m_pArchiveIn;
	CArchive	*m_pArchiveOut;

public:
	CDataSocket();
	virtual ~CDataSocket();

public:
	void operator << (CData &data);
	void operator >> (CData &data);
	void Flush();
	void Send(CData *pData);
	void Receive(CData *pData);
	void Init(CWnd *pWnd);

	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};
