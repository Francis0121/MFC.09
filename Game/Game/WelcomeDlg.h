#pragma once


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
	CString m_strName;
};
