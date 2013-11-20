#pragma once
#include "afxwin.h"


// CStaticDlg 대화 상자입니다.

class CStaticDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStaticDlg)

public:
	CStaticDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CStaticDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_STATIC };

	CString m_strRankList;
	CString m_strPractice;
	CListBox m_ctrPList;

	BOOL OnInitDialog();
	void OnReadScoreFile(CString pszFileName, int mode);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg void OnLbnDblclkListGrade();

	DECLARE_MESSAGE_MAP()
};
