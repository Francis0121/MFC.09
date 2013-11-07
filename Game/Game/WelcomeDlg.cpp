// WelcomeDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Game.h"
#include "WelcomeDlg.h"
#include "afxdialogex.h"
#include <WtsApi32.h>
#pragma comment(lib, "Wtsapi32.lib")

// CWelcomeDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CWelcomeDlg, CDialogEx)

CWelcomeDlg::CWelcomeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWelcomeDlg::IDD, pParent)
{
	// ������ ����� ������ ��������
	DWORD dwCnt = 0;	
	LPTSTR pStr = NULL;
	WTSQuerySessionInformation( 
		WTS_CURRENT_SERVER_HANDLE, WTS_CURRENT_SESSION, 
		WTSUserName, &pStr, &dwCnt);
	m_nGrade = 0;
	m_strName = pStr;
}

CWelcomeDlg::~CWelcomeDlg()
{
}

void CWelcomeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_GRADE_TOP, m_nGrade);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strName);
}


BEGIN_MESSAGE_MAP(CWelcomeDlg, CDialogEx)
END_MESSAGE_MAP()
