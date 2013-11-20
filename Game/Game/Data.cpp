#include "stdafx.h"
#include "Game.h"
#include "Data.h"


// ~ CData Constructro Destructor

CData::CData(){
	m_strName = _T("");
	m_strMes = _T("");
}

CData::~CData(){
}

// ~ CData member functions

void CData::Serialize(CArchive &ar){
	
	if(ar.IsStoring()){// Network Post
		switch(m_enGameMode)
		{
		case MODE_CONNECT:
			ar << (int)MODE_CONNECT;
			ar << m_strName;
			ar << m_nImageType;
			for(int i=0; i<36; i++){
				ar << m_nRnd[i];
			}
			break;

		case MODE_CHAT:
			ar << (int)MODE_CHAT;
			ar << m_strMes;
			break;

		case MODE_GAME_START:
			ar << (int)MODE_GAME_START;
			break;

		case MODE_MATE_TURN:
			ar << (int)MODE_MATE_TURN;
			ar << m_nColIndex << m_nRowIndex;
			break;

		case MODE_MATE_TURN_FINISH:
			ar << (int)MODE_MATE_TURN_FINISH;
			ar << m_nColIndex << m_nRowIndex << tempColIndex << tempRowIndex;
			break;

		case MODE_MY_TURN:
			ar << (int)MODE_MY_TURN;
			ar << m_nMatchCount;
			break;

		case MODE_CLOSE:
			ar << (int)MODE_CLOSE;
			break;
		
		case MODE_GIVE_UP:
			ar << (int)MODE_GIVE_UP;
			break;
		}
	}else{ // Network Get
		int m_nGameMode;
		ar >> m_nGameMode;
		m_enGameMode = (enSocketStatus)m_nGameMode;

		switch(m_enGameMode)
		{
		case MODE_CONNECT:
			ar >> m_strName;
			ar >> m_nImageType;
			for(int i=0; i<36; i++){
				ar >> m_nRnd[i];
			}
			break;

		case MODE_CHAT:
			ar >> m_strMes;
			break;

		case MODE_MATE_TURN:
			ar >> m_nColIndex >> m_nRowIndex;
			break;
		
		case MODE_MATE_TURN_FINISH:
			ar >> m_nColIndex >> m_nRowIndex >> tempColIndex >> tempRowIndex;
			break;

		case MODE_MY_TURN:
			ar >> m_nMatchCount;
			break;
		
		case MODE_GAME_START:
			break;
		
		case MODE_GIVE_UP:
			break;
		}
	}
}

void CData::SetGameMode(enSocketStatus enMode){
	m_enGameMode = enMode;
}

enSocketStatus CData::GetGameMode() const{
	return m_enGameMode;
}