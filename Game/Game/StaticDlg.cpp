// StaticDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Game.h"
#include "StaticDlg.h"
#include "afxdialogex.h"


// CStaticDlg ��ȭ �����Դϴ�.

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

	if(CFile::GetStatus(pszFileName, status)){//������ �����ϴ� ��� �о���⸸��
		if(!(file.Open(pszFileName, CFile::modeRead | CFile::shareDenyNone, &e))) { 
			TRACE( _T("Can't open file %s, error = %u\n"), pszFileName, e.m_cause );
			return;
		} 
	}else{//������ �������� �ʴ´ٸ� �����Ѵ�
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


// CStaticDlg �޽��� ó�����Դϴ�.
