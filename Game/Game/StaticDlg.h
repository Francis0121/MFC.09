#pragma once
#include "afxwin.h"


// CStaticDlg ��ȭ �����Դϴ�.

class CStaticDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStaticDlg)

public:
	CStaticDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CStaticDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_STATIC };

	CString m_strRankList;
	CString m_strPractice;
	CListBox m_ctrPList;

	BOOL OnInitDialog();
	void OnReadScoreFile(CString pszFileName, int mode);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	afx_msg void OnLbnDblclkListGrade();

	DECLARE_MESSAGE_MAP()
};
