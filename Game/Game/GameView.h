
// GameView.h : CGameView Ŭ������ �������̽�
//

#pragma once

enum STATUS{
	READY,
	START,
	END
};

class CGameView : public CView
{
protected: // serialization������ ��������ϴ�.
	CGameView();
	DECLARE_DYNCREATE(CGameView)

// Ư���Դϴ�.
public:
	CGameDoc* GetDocument() const;
	//���� ���콺�� ��ǥ������ �׸��� ��ġ�� ����Ͽ� �����ϰ� �ִ� �ε���
	UINT m_nRowTempIndex;
	UINT m_nColTempIndex;
	//���� �׸��� ������ ���� ���� ����
	int nMatchCount;
	int isStatus;
	// Game Intro ���� ����
	clock_t before;
	CString m_strIntro;

// �۾��Դϴ�.
public:
	void OnMatching(void);
	void OnSuccess(void);

	void OnTimerStart(void);
	void OnTimerStop(void);
	void OnTimerReset(void);

	void IntroReady(void);
	void StopWatch(void);
// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CGameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGradeTop();
	afx_msg void OnGradeMiddel();
	afx_msg void OnGradeLow();
	afx_msg void OnUpdateGradeTop(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGradeMiddel(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGradeLow(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNewGame();
	afx_msg void OnModeRanking();
	afx_msg void OnUpdateModeRanking(CCmdUI *pCmdUI);
	afx_msg void OnStaticsRanking();
	afx_msg void OnTypeDooly();
	afx_msg void OnUpdateTypeDooly(CCmdUI *pCmdUI);
	afx_msg void OnTypePocketmon();
	afx_msg void OnUpdateTypePocketmon(CCmdUI *pCmdUI);
	afx_msg void OnTypeOnepiece();
	afx_msg void OnUpdateTypeOnepiece(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // GameView.cpp�� ����� ����
inline CGameDoc* CGameView::GetDocument() const
   { return reinterpret_cast<CGameDoc*>(m_pDocument); }
#endif

