#pragma once


// CStaticDlg ��ȭ �����Դϴ�.

class CStaticDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStaticDlg)

public:
	CStaticDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CStaticDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_STATIC };

	BOOL OnInitDialog();
	void OnReadScoreFile(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strRankList;
};
