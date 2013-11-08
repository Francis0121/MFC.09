
// GameView.h : CGameView 클래스의 인터페이스
//

#pragma once

enum STATUS{
	READY,
	START,
	END
};

class CGameView : public CView
{
protected: // serialization에서만 만들어집니다.
	CGameView();
	DECLARE_DYNCREATE(CGameView)

// 특성입니다.
public:
	CGameDoc* GetDocument() const;
	//현재 마우스가 좌표값으로 그림의 위치를 계산하여 저장하고 있는 인덱스
	UINT m_nRowTempIndex;
	UINT m_nColTempIndex;
	//맞춘 그림의 개수를 세는 변수 선언
	int nMatchCount;
	int isStatus;
	// Game Intro 관련 변수
	clock_t before;
	CString m_strIntro;

// 작업입니다.
public:
	void OnMatching(void);
	void OnSuccess(void);

	void OnTimerStart(void);
	void OnTimerStop(void);
	void OnTimerReset(void);

	void IntroReady(void);
	void StopWatch(void);
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CGameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
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

#ifndef _DEBUG  // GameView.cpp의 디버그 버전
inline CGameDoc* CGameView::GetDocument() const
   { return reinterpret_cast<CGameDoc*>(m_pDocument); }
#endif

