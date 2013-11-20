// Splash.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game.h"
#include "Splash.h"
#include "afxdialogex.h"


// CSplash 대화 상자입니다.

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


// CSplash 메시지 처리기입니다.
BOOL CSplash::OnInitDialog(){

	CDialog::OnInitDialog();
	SetTimer(0, 2000, NULL);
	return TRUE;
}

void CSplash::OnTimer(UINT_PTR nIDEvent){
	EndDialog(0);
	CDialogEx::OnTimer(nIDEvent);
}
