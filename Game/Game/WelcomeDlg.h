#pragma once


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
	CString m_strName;
};
