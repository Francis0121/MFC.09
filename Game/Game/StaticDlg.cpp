// StaticDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game.h"
#include "StaticDlg.h"
#include "afxdialogex.h"


// CStaticDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CStaticDlg, CDialogEx)

CStaticDlg::CStaticDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStaticDlg::IDD, pParent)
	, m_strRankList(_T(""))
{

}

CStaticDlg::~CStaticDlg()
{
}

void CStaticDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_RANKIN_LIST, m_strRankList);
}


BOOL CStaticDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	
	OnReadScoreFile();

	return TRUE;
}

void CStaticDlg::OnReadScoreFile(void){

	CStdioFile file; 
	CFileException e;
	CFileStatus status;
	TCHAR* pszFileName = _T("score");

	if(CFile::GetStatus(pszFileName, status)){//파일이 존재하는 경우 읽어오기만함
		if(!(file.Open(pszFileName, CFile::modeRead | CFile::shareDenyNone, &e))) { 
			TRACE( _T("Can't open file %s, error = %u\n"), pszFileName, e.m_cause );
			return;
		} 
	}else{//파일이 존재하지 않는다면 생성한다
		if(!(file.Open(pszFileName, CFile::modeCreate | CFile::modeRead | CFile::shareDenyNone, &e))) { 
			TRACE( _T("Can't open file %s, error = %u\n"), pszFileName, e.m_cause );
			return;
		} 
	}

	CString _buf;

	while (file.ReadString(_buf))
	{
		m_strRankList += _buf +_T("\n");
		UpdateData(FALSE);
	}

	file.Close();
}

BEGIN_MESSAGE_MAP(CStaticDlg, CDialogEx)
END_MESSAGE_MAP()


// CStaticDlg 메시지 처리기입니다.
