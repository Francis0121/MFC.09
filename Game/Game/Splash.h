#pragma once


// CSplash ��ȭ �����Դϴ�.

class CSplash : public CDialogEx
{
	DECLARE_DYNAMIC(CSplash)

public:
	CSplash(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSplash();

	BOOL OnInitDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_LOADING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
