/**********************************************************************/
 
/**********************************************************************/
//#include "DataManagerNonDb.h"
 
#include "TimeSliceEditDlg.h"
#include "TimeSliceQuickSetDlg.h"
/**********************************************************************/
#include "ListTimeSliceDlg.h"
/**********************************************************************/

CListTimeSliceDlg::CListTimeSliceDlg( const char* szName, CWnd* pParent)
	: CDialog(CListTimeSliceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListTimeSliceDlg)
	//}}AFX_DATA_INIT
	m_bIsModified = FALSE;
	m_strName = szName;
}

/**********************************************************************/

void CListTimeSliceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListTimeSliceDlg)
	//}}AFX_DATA_MAP
	DDX_Text( pDX, IDC_EDIT_NAME, m_strName );
	DDV_MaxChars( pDX, m_strName, 32 );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CListTimeSliceDlg, CDialog)
	//{{AFX_MSG_MAP(CListTimeSliceDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST, OnKillFocusList)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST, OnSetFocusList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CListTimeSliceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	
	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listData.InsertColumn ( 0, "Time Slice Name", LVCFMT_LEFT, 250 );
	m_listData.InsertColumn ( 1, "Start", LVCFMT_RIGHT, 70 );
	m_listData.InsertColumn ( 2, "End", LVCFMT_RIGHT, 70 );
	m_listData.InsertColumn ( 3, "Group", LVCFMT_RIGHT, 70 );
	
	m_listData.SetItemCountEx ( GetArraySize() );
	m_listData.SetCurSel(0);

	return TRUE;  
}

/**********************************************************************/

void CListTimeSliceDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )  
   {
	   m_listData.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

int CListTimeSliceDlg::GetArraySize()
{
	return DataManagerNonDb.TimeSliceDetail.GetSize();
}

/**********************************************************************/

void CListTimeSliceDlg::SelectLine ( int nIndex )
{
	int nSize = m_listData.GetItemCount();

	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listData.SetCurSel ( nIndex );
	else 
		m_listData.SetCurSel ( nSize - 1 );
	
	m_listData.Invalidate();
}

/**********************************************************************/

void CListTimeSliceDlg::OnButton1() 
{
	int nInsertPos = GetSafeSelection() + 1;
	if ( nInsertPos == 0 ) nInsertPos = DataManagerNonDb.TimeSliceDetail.GetSize();

	CTimeSliceCSVRecord NewRecord;
	NewRecord.SetName ( "" );
	
	CTimeSliceEditDlg dlg ( NewRecord, this );
	dlg.DoModal();
	
	DataManagerNonDb.TimeSliceDetail.InsertAt ( nInsertPos, NewRecord );
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nInsertPos );
}

/**********************************************************************/

void CListTimeSliceDlg::OnButton2() 
{
	int nSelection = GetSafeSelection();
	if ( nSelection == -1 )
		return;
	
	CTimeSliceCSVRecord TimeSliceRecord;
	DataManagerNonDb.TimeSliceDetail.GetAt ( nSelection, TimeSliceRecord );
	
	CTimeSliceEditDlg dlg ( TimeSliceRecord, this );
	dlg.DoModal();
	
	DataManagerNonDb.TimeSliceDetail.SetAt ( nSelection, TimeSliceRecord );
	SelectLine ( nSelection );
}

/**********************************************************************/

void CListTimeSliceDlg::OnButton3() 
{
	int nSelection = GetSafeSelection();
	if ( nSelection == -1 ) 
		return;

	if ( Prompter.YesNo ( "Are you sure you wish to delete this time slice" ) == IDYES )
	{
		DataManagerNonDb.TimeSliceDetail.RemoveAt ( nSelection );
		m_listData.SetItemCountEx ( GetArraySize() );
	
		if ( nSelection > DataManagerNonDb.TimeSliceDetail.GetSize() - 1 )
			nSelection--;

		SelectLine ( nSelection );
	}
}

/**********************************************************************/

void CListTimeSliceDlg::OnButton4() 
{
	int nSelection = GetSafeSelection ( 1, GetArraySize() - 1 );
	if ( nSelection == -1 )
		return;
	
	CTimeSliceCSVRecord TempRecord1; 
	CTimeSliceCSVRecord TempRecord2; 
	DataManagerNonDb.TimeSliceDetail.GetAt ( nSelection, TempRecord1 );
	DataManagerNonDb.TimeSliceDetail.GetAt ( nSelection - 1, TempRecord2 );
	DataManagerNonDb.TimeSliceDetail.SetAt ( nSelection, TempRecord2 );
	DataManagerNonDb.TimeSliceDetail.SetAt ( nSelection - 1, TempRecord1 );
	SelectLine ( nSelection - 1 );
}

/**********************************************************************/

void CListTimeSliceDlg::OnButton5() 
{
	int nSelection = GetSafeSelection ( 0, GetArraySize() - 2 );
	if ( nSelection == -1 ) 
		return;

	CTimeSliceCSVRecord TempRecord1; 
	CTimeSliceCSVRecord TempRecord2; 
	DataManagerNonDb.TimeSliceDetail.GetAt ( nSelection, TempRecord1 );
	DataManagerNonDb.TimeSliceDetail.GetAt ( nSelection + 1, TempRecord2 );
	DataManagerNonDb.TimeSliceDetail.SetAt ( nSelection, TempRecord2 );
	DataManagerNonDb.TimeSliceDetail.SetAt ( nSelection + 1, TempRecord1 );
	SelectLine ( nSelection + 1 );
}

/**********************************************************************/

void CListTimeSliceDlg::OnButton6() 
{
	CTimeSliceQuickSetDlg dlg( this );
	if ( dlg.DoModal() != IDOK )
		return;

	switch( dlg.m_nAction )
	{
	case 1:
		if ( GetArraySize() != 0 )
		{
			if ( CheckCreateMap( "create an hourly time slice map" ) == FALSE )
				dlg.m_nAction = 0;
		}
		break;

	case 2:
		if ( GetArraySize() != 0 )
		{
			if ( CheckCreateMap( "create a half hourly time slice map" ) == FALSE )
				dlg.m_nAction = 0;
		}
		break;

	case 3:
		if ( GetArraySize() != 0 )
		{
			if ( CheckCreateMap( "create a quarter hourly time slice map" ) == FALSE )
				dlg.m_nAction = 0;
		}
		break;

	case 4:
		if ( GetArraySize() != 0 )
		{
			if ( CheckCreateMap( "create a time slice map from existing report sessions" ) == FALSE )
				dlg.m_nAction = 0;
		}
		break;
	}

	switch( dlg.m_nAction )
	{
	case 1:		DataManagerNonDb.TimeSliceDetail.CreateHourlyMap();		break;
	case 2:		DataManagerNonDb.TimeSliceDetail.CreateHalfHourlyMap();	break;
	case 3:		DataManagerNonDb.TimeSliceDetail.CreateQuarterHourlyMap();	break;
	case 4:		DataManagerNonDb.TimeSliceDetail.CreateReportSessionMap();	break;
	}

	switch( dlg.m_nAction )
	{
	case 0:
	case 4:
		if ( dlg.m_bSort != 0 )
			DataManagerNonDb.TimeSliceDetail.SortTimeSlices();
		break;
	}

	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine(0);
}

/**********************************************************************/

int CListTimeSliceDlg::GetSafeSelection()
{
	return GetSafeSelection ( 0, GetArraySize() - 1 );
}

/**********************************************************************/

int CListTimeSliceDlg::GetSafeSelection( int nMin, int nMax )
{
	int nSel = m_listData.GetCurSel();
	if ( ( nSel < nMin ) || ( nSel > nMax ) || ( nSel >= m_listData.GetItemCount() ) )
		nSel = -1;
	
	return nSel;
}

/**********************************************************************/

void CListTimeSliceDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLine( nIndex );
		CCSV csv ( m_strDisplayLine, '\t' );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

bool CListTimeSliceDlg::PrepareLine( int nIndex )
{
	if ( nIndex < 0 || nIndex >= DataManagerNonDb.TimeSliceDetail.GetSize() )
		return FALSE;

	CTimeSliceCSVRecord ReportRecord;
	DataManagerNonDb.TimeSliceDetail.GetAt( nIndex, ReportRecord );

	CCSV csvOut ( '\t' );
	csvOut.Add( ReportRecord.GetName() );

	CString strStart;
	strStart.Format( "%2.2d:%2.2d",
		ReportRecord.GetStart() / 60,
		ReportRecord.GetStart() % 60 );

	csvOut.Add( strStart );
	
	CString strEnd;
	strEnd.Format( "%2.2d:%2.2d",
		ReportRecord.GetEnd() / 60,
		ReportRecord.GetEnd() % 60 );

	csvOut.Add( strEnd );

	if ( ReportRecord.GetSortOrder() != 0 )
		csvOut.Add( ReportRecord.GetSortOrder() );
	
	m_strDisplayLine = csvOut.GetLine();
	return TRUE;
}

/**********************************************************************/

void CListTimeSliceDlg::OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDefID ( IDC_BUTTON2 );
	*pResult = 0;
}

/**********************************************************************/

void CListTimeSliceDlg::OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	SetDefID ( IDOK );
	*pResult = 0;
}

/**********************************************************************/

void CListTimeSliceDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnButton2();
	*pResult = 0;
}

/**********************************************************************/

void CListTimeSliceDlg::OnOK() 
{
	UpdateData( TRUE );
	m_nListSelection = m_listData.GetCurSel();
	EndDialog ( IDOK );
}

/**********************************************************************/

void CListTimeSliceDlg::OnCancel() 
{
	EndDialog ( IDCANCEL );
}

/**********************************************************************/

bool CListTimeSliceDlg::CheckCreateMap( const char* szType )
{
	CString str = "Are you sure you wish to ";
	str += szType;
	str += "\n\nAll existing time slices in this map will be deleted if you proceed.";

	return ( Prompter.YesNo( str ) == IDYES );
}

/**********************************************************************/
