
// GameView.h : CGameView 클래스의 인터페이스
//

#pragma once


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
// 작업입니다.
public:
	void OnMatching(void);
	void OnSuccess(void);
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
};

#ifndef _DEBUG  // GameView.cpp의 디버그 버전
inline CGameDoc* CGameView::GetDocument() const
   { return reinterpret_cast<CGameDoc*>(m_pDocument); }
#endif

