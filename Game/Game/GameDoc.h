
// GameDoc.h : CGameDoc 클래스의 인터페이스
//


#pragma once

enum GRADE{
	TOP,
	MIDDLE,
	LOW
};

class CGameDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CGameDoc();
	DECLARE_DYNCREATE(CGameDoc)

// 특성입니다.
public:
	CBitmap m_bmp[6][6]; //Bitmap 저장 최대 6 x 6 인 상태
	CSize m_bmCell; // 셀에 들어갈 그림 하나의 크기

	int m_nRow; // 가로
	int m_nCol; // 세로
	
	int m_nRnd[36]; //최대 36개의 그림 위치를 랜덤함수로 얻어낸 값을 저장하기 위한 변수
	
	UINT m_nBmpID[6][6]; //그림의 ID값을 6X6의 2차원 배열에 저장한다. 최대 36개까지 저장된다.
	bool m_bRandom; //게임중 랜덤의 유무 파악
	bool m_bShow[6][6]; //그림이 보여야 할지 말아야 할지를 결정하는 변수
	int m_nRowIndex;
	int m_nColIndex;

	UINT m_nBmpFirstID; //첫번째 눌린 그림의 아이디 값과
	UINT m_nBmpSecondID; //두번째 눌린 그림의 아이디 값이 매치하는지 알아보기 위해서
	bool m_bMouse; //true이면 첫번째 마우스 버튼이 눌린 것이고 false이면 두번쨰 마우스 버튼이 눌린 것임

	int m_nGrade; //난이도 변수

	int m_nH, m_nM, m_nS, m_nTimeSet; // 시간

// 작업입니다.
public:
	void ResizeWindow(void);
	void OnRandom(void);
	void UpdateGrade(void);
	void InitializeGame(void);

	//Getter,Setter
	void SetGrade(int grade);
	int GetGrade();
	void SetRandom(bool random);
	bool GetRandom();
	double GetRecordPerSecond();

	int GetHour(void);
	void SetHour(int hour);
	int GetMinute(void);
	void SetMinute(int minute);
	int GetSecond(void);
	void SetSecond(int second);
	int GetTimeset(void);
	void SetTimeset(int timeset);

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CGameDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
