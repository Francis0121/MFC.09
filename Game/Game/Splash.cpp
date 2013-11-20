// Splash.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Game.h"
#include "Splash.h"
#include "afxdialogex.h"


// CSplash ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSplash, CDialogEx)

CSplash::CSplash(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSplash::IDD, pParent)
{

}

CSplash::~CSplash()
{
}

void CSplash::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSplash, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSplash �޽��� ó�����Դϴ�.
BOOL CSplash::OnInitDialog(){

	CDialog::OnInitDialog();
	SetTimer(0, 2000, NULL);
	return TRUE;
}

void CSplash::OnTimer(UINT_PTR nIDEvent){
	EndDialog(0);
	CDialogEx::OnTimer(nIDEvent);
}
