// WelcomeDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Game.h"
#include "WelcomeDlg.h"
#include "afxdialogex.h"

// CWelcomeDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CWelcomeDlg, CDialogEx)

CWelcomeDlg::CWelcomeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWelcomeDlg::IDD, pParent)
	, m_nGrade(0)
{

}

CWelcomeDlg::~CWelcomeDlg()
{
}

void CWelcomeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_GRADE_TOP, m_nGrade);
}


BEGIN_MESSAGE_MAP(CWelcomeDlg, CDialogEx)
END_MESSAGE_MAP()
