
// GameDoc.h : CGameDoc Ŭ������ �������̽�
//


#pragma once

enum GRADE{
	TOP,
	MIDDLE,
	LOW
};

class CGameDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CGameDoc();
	DECLARE_DYNCREATE(CGameDoc)

// Ư���Դϴ�.
public:
	CBitmap m_bmp[6][6]; //Bitmap ���� �ִ� 6 x 6 �� ����
	CSize m_bmCell; // ���� �� �׸� �ϳ��� ũ��

	int m_nRow; // ����
	int m_nCol; // ����
	
	int m_nRnd[36]; //�ִ� 36���� �׸� ��ġ�� �����Լ��� �� ���� �����ϱ� ���� ����
	
	UINT m_nBmpID[6][6]; //�׸��� ID���� 6X6�� 2���� �迭�� �����Ѵ�. �ִ� 36������ ����ȴ�.
	bool m_bRandom; //������ ������ ���� �ľ�
	bool m_bShow[6][6]; //�׸��� ������ ���� ���ƾ� ������ �����ϴ� ����
	int m_nRowIndex;
	int m_nColIndex;

	UINT m_nBmpFirstID; //ù��° ���� �׸��� ���̵� ����
	UINT m_nBmpSecondID; //�ι�° ���� �׸��� ���̵� ���� ��ġ�ϴ��� �˾ƺ��� ���ؼ�
	bool m_bMouse; //true�̸� ù��° ���콺 ��ư�� ���� ���̰� false�̸� �ι��� ���콺 ��ư�� ���� ����

	int m_nGrade; //���̵� ����

	int m_nH, m_nM, m_nS, m_nTimeSet; // �ð�

// �۾��Դϴ�.
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

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CGameDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
