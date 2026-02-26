/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "DefaultCategoryDlg.h"
#include "DefEditStockCategoryDlg.h"
#include "RepSpawn.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "ListDataCategoryDlg.h"
/**********************************************************************/

CSingleListContentHelpersCategory::CSingleListContentHelpersCategory()
{
}

/**********************************************************************/

int CSingleListContentHelpersCategory::GetArraySize()
{
	return DataManager.Category.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersCategory::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		strLine.Format ( "C%3.3d\t%s\t%s",
			nIndex + 1,
			DataManager.Category.GetText(nIndex + 1),
			CTaxArray::GetTaxBandFromNumber( DataManager.Category.GetTaxBand(nIndex + 1), 1, " " ) );

		if ( DataManager.Category.GetReportEnable ( nIndex + 1 ) == FALSE )
			strLine += "\tDisable";

		bResult = TRUE;
	}
		
	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataCategoryDlg::CListDataCategoryDlg( int nCategory, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_bMultiSelect = FALSE;
	m_nInitialCategory = nCategory;
	m_pContentHelpers = new CSingleListContentHelpersCategory();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataCategoryDlg::~CListDataCategoryDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataCategoryDlg::SetTitle()
{
	CString strTitle = "Setup Stock Category Texts ";
	strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), StockOptions.GetGlobalCategoryFlag() ); 
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataCategoryDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Edit" );
	m_button2.SetWindowText ( "De&fault" );
	m_button3.ShowWindow ( SW_HIDE );
	m_button4.SetWindowText ( "&Print" );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button4, 368, 147 );
}

/**********************************************************************/

void CListDataCategoryDlg::SetColumns()
{
	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.InsertColumn ( 0, "No", LVCFMT_LEFT, 50 );
	m_listData.InsertColumn ( 1, "Name", LVCFMT_LEFT, 270 );
	m_listData.InsertColumn ( 2, EcrmanOptions.GetTaxBandString(), LVCFMT_RIGHT, 80 );
	m_listData.InsertColumn ( 3, "Report", LVCFMT_RIGHT, 80 );
}

/**********************************************************************/

void CListDataCategoryDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	if ( m_nInitialCategory >= 1 && m_nInitialCategory <= GetArraySize() )
		SelectLine( m_nInitialCategory - 1 );
}

/**********************************************************************/

void CListDataCategoryDlg::HandleButton1() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 ) 
		return;
	
	CDefEditStockCategoryDlg dlg ( DEFEDIT_STOCKCAT_CATEGORY, nIndex + 1, m_WarnCancelAfterChange, this );
	dlg.SetListDataDlg( this );
	dlg.DoModal();
	SelectLine ( dlg.GetIndex() - 1 );
}

/**********************************************************************/

void CListDataCategoryDlg::HandleButton2()
{
	CDefaultCategoryDlg dlg ( this );
	if ( dlg.DoModal() == IDOK )
	{
		m_WarnCancelAfterChange.SetFlag();

		if ( dlg.m_nDefType == 0 )
		{
			for ( int nCategory = 1; nCategory <= DataManager.Category.GetSize(); nCategory++ )
				DataManager.Category.SetCategory ( nCategory, DataManager.Category.GetDefaultText ( nCategory ), 0, TRUE );
		}
		else
		{
			for ( int nCategory = 1; ( nCategory <= DataManager.Category.GetSize() ) && ( nCategory <= Dept::DeptNo.Max ); nCategory++ )
			{
				int nDeptIdx;
				if ( DataManager.Department.FindDeptByNumber( nCategory, nDeptIdx ) == FALSE )
				{
					CString strText = DataManager.Category.GetText ( nCategory );
					int nTaxBand = DataManager.Category.GetTaxBand ( nCategory );
					DataManager.Category.SetCategory ( nCategory, strText, nTaxBand, FALSE );
				}
				else
				{
					CDepartmentCSVRecord DeptRecord;
					DataManager.Department.GetAt( nDeptIdx, DeptRecord );

					CString strDeptText = DeptRecord.GetReportText();
					CString strDefText = DeptRecord.GetDefaultText();

					if ( strDeptText == strDefText )
						strDeptText = DataManager.Category.GetText ( nCategory );

					bool bReportEnable = DeptRecord.GetReportEnable();
					int nTaxBand = DataManager.Category.GetTaxBand ( nCategory );
					
					DataManager.Category.SetCategory ( nCategory, strDeptText, nTaxBand, bReportEnable );
				}
			}
		}
		SelectLine(GetSafeSelection());
	}
}

/**********************************************************************/

void CListDataCategoryDlg::HandleButton4()
{
	CReportFile ReportFile;

	if ( ReportFile.Open ( Super.ReportFilename() ) == TRUE )
	{
		ReportFile.SetStyle1 ( "Category Texts" );
		ReportFile.AddColumn ( "No", TA_LEFT, 120 );
		ReportFile.AddColumn ( "Text", TA_LEFT, 500 );
		ReportFile.AddColumn ( EcrmanOptions.GetTaxBandString(), TA_RIGHT,200 );
	
		for ( int nCategoryNo = 1; nCategoryNo <= MAX_CATEGORY; nCategoryNo++ )
		{
			CCSV csv ( '\t' );
			csv.Add ( nCategoryNo );
			csv.Add ( DataManager.Category.GetText ( nCategoryNo ) );
			csv.Add ( DataManager.Category.GetTaxBand ( nCategoryNo ) );
			ReportFile.WriteLine ( csv.GetLine() );
		}
		ReportFile.Close();
		
		CFilenameUpdater FnUp ( SysFiles::CategoryPrm );

		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
	}
}

/**********************************************************************/

