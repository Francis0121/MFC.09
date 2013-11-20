// StaticDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game.h"
#include "StaticDlg.h"
#include "afxdialogex.h"
#include "Record.h"

// Sort Alogrithm
// CArray 정렬템플릿함수
template<class T>
void QSortCArray( T& t, int (__cdecl *compare )(const void *elem1, const void *elem2 ) )
{
    if( t.GetSize() <= 0 ) return;
 
    qsort( t.GetData(), t.GetSize(), sizeof( t[0] ), compare );
}
 
// Record 자료형비교함수
int CompareRecord( const void *elem1, const void *elem2 )
{
    Record* r1 = (Record*) elem1;
    Record* r2 = (Record*) elem2;

	if( r1->CalculateTime() > r2->CalculateTime() ) 
		return +1;
	if( r1->CalculateTime() < r2->CalculateTime() ) 
		return -1;
 
    return 0;
}

// CStaticDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CStaticDlg, CDialogEx)

CStaticDlg::CStaticDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStaticDlg::IDD, pParent)
	, m_strRankList(_T(""))
	, m_strPractice(_T("")){
	
}

CStaticDlg::~CStaticDlg(){
}

void CStaticDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_RANKIN_LIST, m_strRankList);
	DDX_Text(pDX, IDC_STATIC_PRACTICE_LIST, m_strPractice);
	DDX_Control(pDX, IDC_LIST_GRADE, m_ctrPList);
}


BOOL CStaticDlg::OnInitDialog(){
	CDialog::OnInitDialog();

	m_ctrPList.AddString(_T("고급"));
	m_ctrPList.AddString(_T("중급"));	
	m_ctrPList.AddString(_T("초급"));

	int nFind =	m_ctrPList.FindStringExact(-1, _T("고급"));
	m_ctrPList.SetCurSel(nFind);
	OnReadScoreFile(_T("practice"), nFind);
	OnReadScoreFile(_T("score"), -1);

	return TRUE;
}

void CStaticDlg::OnReadScoreFile(CString pszFileName, int mode){

	CStdioFile file; 
	CFileException e;
	CFileStatus status;
	
	if(mode != -1)
		pszFileName.Format(pszFileName+("_%d"), mode);

	if(CFile::GetStatus(pszFileName, status)){//파일이 존재하는 경우 읽어오기만함
		if(!(file.Open(pszFileName, CFile::modeRead | CFile::shareDenyNone, &e))) { 
			TRACE( _T("Can't open file %s, error = %u\n"), pszFileName, e.m_cause );
			return;
		} 
	}else{//파일이 존재하지 않는다면 생성한다
		if(!(file.Open(pszFileName, CFile::modeCreate | CFile::modeRead | CFile::shareDenyNone, &e))) { 
			TRACE( _T("Can't open file %s, error = %u\n"), pszFileName, e.m_cause );
			return;
		} 
	}

	CString _buf;
	CArray<Record, Record> array;
	Record record;
	while (file.ReadString(_buf)){
		CString name, time, strHour, strMinute, strSecond, strTimeset;
		name = _T("5");
		int hour = _ttoi(name);
		AfxExtractSubString(name, _buf, 0, ',');
		AfxExtractSubString(time, _buf, 1, ',');
		AfxExtractSubString(strHour, time, 0, ':');
		AfxExtractSubString(strMinute, time, 1, ':');
		AfxExtractSubString(strSecond, time, 2, ':');
		AfxExtractSubString(strTimeset, time, 3, ':');
		
		record = Record(name, _ttoi(strHour), _ttoi(strMinute), _ttoi(strSecond), _ttoi(strTimeset));
		array.Add(record);
	}

	QSortCArray( array, CompareRecord );
	int printSize = array.GetSize() < 5 ? array.GetSize() : 5;
	for(int i=0; i< printSize; i++ )
    {
		CString strTemp;
		strTemp.Format(_T("%d. %s (%d:%d:%d:%d) \n"), i+1,  array[i].name,array[i].hour, array[i].minute, array[i].second, array[i].timeset );
		if(mode == -1){
			m_strRankList += strTemp;
		}else{
			m_strPractice += strTemp;
		}
    }

	UpdateData(FALSE);
	
	file.Close();
}

BEGIN_MESSAGE_MAP(CStaticDlg, CDialogEx)
	ON_LBN_DBLCLK(IDC_LIST_GRADE, &CStaticDlg::OnLbnDblclkListGrade)
END_MESSAGE_MAP()

void CStaticDlg::OnLbnDblclkListGrade(){
	int nSel = m_ctrPList.GetCurSel();
	m_strPractice = _T("");
	OnReadScoreFile(_T("practice"),nSel);
}
