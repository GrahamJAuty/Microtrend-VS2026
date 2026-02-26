/**********************************************************************/
#include "DefEditStockCategoryDlg.h"
#include "RepSpawn.h"
/**********************************************************************/
#include "ListDataSystemCategoryDlg.h"
/**********************************************************************/

CSingleListContentHelpersSystemCategory::CSingleListContentHelpersSystemCategory()
{
}

/**********************************************************************/

int CSingleListContentHelpersSystemCategory::GetArraySize()
{
	return DataManagerNonDb.SystemCategory.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersSystemCategory::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		strLine.Format ( "C%3.3d\t%s",
			nIndex + 1,
			DataManagerNonDb.SystemCategory.GetText(nIndex + 1) );

		if ( DataManagerNonDb.SystemCategory.GetReportEnable ( nIndex + 1 ) == FALSE )
			strLine += "\tDisable";

		bResult = TRUE;
	}
		
	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataSystemCategoryDlg::CListDataSystemCategoryDlg( int nCategory, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_bMultiSelect = FALSE;
	m_nInitialCategory = nCategory;
	m_pContentHelpers = new CSingleListContentHelpersSystemCategory();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataSystemCategoryDlg::~CListDataSystemCategoryDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataSystemCategoryDlg::SetTitle()
{
	SetWindowText ( "Setup System Stock Category Texts" );
}

/**********************************************************************/

void CListDataSystemCategoryDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Edit" );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.ShowWindow ( SW_HIDE );
	m_button4.SetWindowText ( "&Print" );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button4, 368, 147 );
}

/**********************************************************************/

void CListDataSystemCategoryDlg::SetColumns()
{
	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.InsertColumn ( 0, "No", LVCFMT_LEFT, 50 );
	m_listData.InsertColumn ( 1, "Name", LVCFMT_LEFT, 350 );
	m_listData.InsertColumn ( 2, "Report", LVCFMT_RIGHT, 80 );
}

/**********************************************************************/

void CListDataSystemCategoryDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	if ( m_nInitialCategory >= 1 && m_nInitialCategory <= GetArraySize() )
		SelectLine( m_nInitialCategory - 1 );
}

/**********************************************************************/

void CListDataSystemCategoryDlg::HandleButton1() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 ) 
		return;
	
	CDefEditStockCategoryDlg dlg ( DEFEDIT_STOCKCAT_SYSTEM, nIndex + 1, m_WarnCancelAfterChange, this );
	dlg.SetListDataDlg( this );
	dlg.DoModal();
	SelectLine ( dlg.GetIndex() - 1 );
}

/**********************************************************************/

void CListDataSystemCategoryDlg::HandleButton4()
{
	CReportFile ReportFile;

	if ( ReportFile.Open ( Super.ReportFilename() ) == TRUE )
	{
		ReportFile.SetStyle1 ( "System Category Texts" );
		ReportFile.AddColumn ( "No", TA_LEFT, 120 );
		ReportFile.AddColumn ( "Text", TA_LEFT, 500 );
		
		for ( int nCategoryNo = 1; nCategoryNo <= MAX_CATEGORY; nCategoryNo++ )
		{
			CCSV csv ( '\t' );
			csv.Add ( nCategoryNo );
			csv.Add ( DataManagerNonDb.SystemCategory.GetText ( nCategoryNo ) );
			ReportFile.WriteLine ( csv.GetLine() );
		}
		ReportFile.Close();
		
		CFilenameUpdater FnUp ( SysFiles::CategoryPrm );

		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
	}
}

/**********************************************************************/

