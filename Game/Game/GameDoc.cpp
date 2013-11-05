
// GameDoc.cpp : CGameDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Game.h"
#endif

#include "GameDoc.h"
#include "GameView.h"
#include "MainFrm.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGameDoc

IMPLEMENT_DYNCREATE(CGameDoc, CDocument)

BEGIN_MESSAGE_MAP(CGameDoc, CDocument)
END_MESSAGE_MAP()


// CGameDoc ����/�Ҹ�

CGameDoc::CGameDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.
	m_bmCell = CSize(79, 81);
	m_nRow = 3;
	m_nCol = 4;

	m_bRandom = true;
	for(int n = 0; n < m_nRow; n++){
		for(int m = 0; m < m_nCol; m++){
			m_bShow[n][m] = false;
		}
	}

	m_nBmpFirstID = m_nBmpSecondID = 0;
	m_bMouse = false;
}

CGameDoc::~CGameDoc()
{
}

BOOL CGameDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CGameDoc serialization

void CGameDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CGameDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// �˻� ó���⸦ �����մϴ�.
void CGameDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGameDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGameDoc ����

#ifdef _DEBUG
void CGameDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGameDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGameDoc ���

void CGameDoc::ResizeWindow(void)
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

	CREATESTRUCT st;
	CREATESTRUCT& rst = st;

	pMain->PreCreateWindow(rst);

	rst.lpszName = _T("Game");
	rst.cx = (m_nRow+1) * m_bmCell.cx + 180;
	rst.cy = (m_nCol+1) * m_bmCell.cy + 100;

	CRect rect;
	pMain->GetClientRect(rect);
	pMain->MoveWindow(rect.left, rect.top, rst.cx, rst.cy);
}

void CGameDoc::OnRandom(void){
	//�ð��� Seed�� ��� �Ȱ��� ���� �߻��� ����
	srand((unsigned)time(NULL));

	//��Ʈ�� ���Կ���/ ������ �� �� / ó�� ���� ��Ʈ���� ���̵�
	BOOL bInsert = TRUE;
	//������ �Ѽ��� ���Ѵ�.
	int nGrating = m_nRow * m_nCol;

	//������ ����ŭ ���鼭 ���� �߻�
	for(int n=0; n < nGrating; n++){
		//���� ��Ʈ�� ���̵� ������ ���� ��Ʈ���� ����
		//�ϴ� �߻��� ������ �迭�� �����ϰ� ����.
		m_nRnd[n] = (rand()%(nGrating/2))+1;

		//���� ������ ���ڼ������� ���Ѵ�.
		for(int m = 0; m<n; m++){
			if(m_nRnd[n] == m_nRnd[m]){//���� ���� ���� �ִٸ� ������ ���� �Ұ��������� �˾ƺ���.
				if(bInsert == FALSE){//bInsert�� FALSE�̸� ������ ������ ���̹Ƿ� 
					n--;//�迭�� �̹� ����° ����� �������� ����ϱ� ���ؼ� ÷�ڸ� �����Ѵ�.
					bInsert = TRUE;//�����̻��� ���� ������ ���� ����Ǿ� ���� ���� ���̹Ƿ�
					break;//���� �ݺ����� �����. 
				}
				bInsert = FALSE;
			}
		}
	}
	
	int nCount = 0;

	for(int n = 0; n < m_nRow; n++){
		for(int m = 0; m<m_nCol; m++){
			m_nBmpID[n][m] = m_nRnd[nCount];
			nCount++;
		}
	}

}