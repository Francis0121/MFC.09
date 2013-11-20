#pragma once
#include "afx.h"

enum enSocketStatus
{
	MODE_CONNECT = 0,
	MODE_CHAT,
	MODE_GAME_START,
	MODE_CLOSE,
	MODE_MATE_TURN,
	MODE_MATE_TURN_FINISH,
	MODE_MY_TURN,
	MODE_GIVE_UP
};

class CData : public CObject
{
public:
	CData();
	virtual ~CData();

public:
	enSocketStatus GetGameMode() const;
	void SetGameMode(enSocketStatus enMode);
	virtual void Serialize(CArchive &ar);

	enSocketStatus m_enGameMode;
	int m_nImageType;
	int m_nRnd[36];

	CString m_strName;
	CString m_strMes;
	
	int m_nRowIndex;
	int m_nColIndex;
	int tempRowIndex;
	int tempColIndex;

	int m_nMatchCount;
};

