#pragma once


// CStaticDlg 대화 상자입니다.

class CStaticDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStaticDlg)

public:
	CStaticDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CStaticDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_STATIC };

	BOOL OnInitDialog();
	void OnReadScoreFile(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strRankList;
};
