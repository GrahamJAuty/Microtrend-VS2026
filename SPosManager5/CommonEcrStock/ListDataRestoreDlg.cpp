/**********************************************************************/
#include "ListDataRestoreDlg.h"
/**********************************************************************/

CSingleListContentHelpersRestore::CSingleListContentHelpersRestore( CStringArray& arrayBackups ) : m_arrayBackups( arrayBackups )
{
}

/**********************************************************************/

int CSingleListContentHelpersRestore::GetArraySize()
{
	return m_arrayBackups.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersRestore::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CCSV csv ( m_arrayBackups.GetAt( nIndex ) );

		CString strExtra = csv.GetString(5);
		if ( "0" == strExtra )
			strExtra = "";

		strLine.Format( "%s\t%s\t%1.1d.%2.2d%s\t%d kb",
			csv.GetString(1),
			csv.GetString(2),
			csv.GetInt(3) / 100,
			csv.GetInt(3) % 100,
			(const char*) strExtra,
			csv.GetInt(4) );

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataRestoreDlg::CListDataRestoreDlg( CStringArray& arrayBackups, CWnd* pParent) : CListDataDlg ( pParent ), m_arrayBackups( arrayBackups )
{
	m_bMultiSelect = FALSE;
	m_pContentHelpers = new CSingleListContentHelpersRestore( m_arrayBackups );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataRestoreDlg::~CListDataRestoreDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataRestoreDlg::SetTitle()
{
	SetWindowText ( "Restore System" );
}

/**********************************************************************/

void CListDataRestoreDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Other" );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.ShowWindow ( SW_HIDE );
	m_button4.ShowWindow ( SW_HIDE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );
	MoveControl( &m_button1, 368, 7 );
}

/**********************************************************************/

void CListDataRestoreDlg::SetColumns()
{
	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.ModifyStyle( 0, LVS_SINGLESEL, 0 );
	m_listData.InsertColumn ( 0, "Date", LVCFMT_LEFT, 120 );
	m_listData.InsertColumn ( 1, "Time", LVCFMT_LEFT, 120 );
	m_listData.InsertColumn ( 2, "Version", LVCFMT_LEFT, 120 );
	m_listData.InsertColumn ( 3, "Size", LVCFMT_RIGHT, 120 );
}

/**********************************************************************/

void CListDataRestoreDlg::HandleButton1()
{
	m_nListSelection = -1;
	EndDialog( IDOK );
}

/**********************************************************************/