#pragma once
#include "afxwin.h"


// CWelcomeDlg 대화 상자입니다.

class CWelcomeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWelcomeDlg)

public:
	CWelcomeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWelcomeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WELCOME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
