#pragma once
#include "afxwin.h"


// CWelcomeDlg ��ȭ �����Դϴ�.

class CWelcomeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWelcomeDlg)

public:
	CWelcomeDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CWelcomeDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_WELCOME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_nGrade;
	int m_nMode;
	CString m_strName;
	CButton m_ctrRadioTop;
	CButton m_ctrRadioMiddle;
	CButton m_ctrRadioLow;
	afx_msg void OnBnClickedRadioModeRank();
	afx_msg void OnBnClickedRadioModePractice();
	afx_msg void OnBnClickedRadioModePvp();
};
