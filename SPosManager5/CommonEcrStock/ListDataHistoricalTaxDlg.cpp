/**********************************************************************/
#include "DepartmentSetEditDlg.h"
#include "DatabaseCSVArray.h"
#include "EditHistoricalTaxDlg.h"
#include "RepSpawn.h"
/**********************************************************************/
#include "ListDataHistoricalTaxDlg.h"
/**********************************************************************/

CSingleListContentHelpersHistoricalTax::CSingleListContentHelpersHistoricalTax()
{
}

/**********************************************************************/

int CSingleListContentHelpersHistoricalTax::GetArraySize()
{
	return DataManager.HistoricalTaxRates.GetTaxDateCount();
}

/**********************************************************************/

bool CSingleListContentHelpersHistoricalTax::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		strLine = DataManager.HistoricalTaxRates.GetDisplayLine ( nIndex + 1, FALSE );
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersHistoricalTax::DeleteDatabaseEntry( int nIndex )
{
	int nDateCode = nIndex + 1;
	if ( ( nDateCode >= 1 ) && ( nDateCode <= DataManager.HistoricalTaxRates.GetTaxDateCount() ) )
	{
		DataManager.HistoricalTaxRates.DeleteDate ( nDateCode );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersHistoricalTax::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Start Date", LVCFMT_LEFT, 140 );
	list.InsertColumn ( 1, "Rate A", LVCFMT_RIGHT, 85 );
	list.InsertColumn ( 2, "Rate B", LVCFMT_RIGHT, 85 );
	list.InsertColumn ( 3, "Rate C", LVCFMT_RIGHT, 85 );
	list.InsertColumn ( 4, "Rate D", LVCFMT_RIGHT, 85 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataHistoricalTaxDlg::CListDataHistoricalTaxDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_pContentHelpers = new CSingleListContentHelpersHistoricalTax();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	CString strRate = EcrmanOptions.GetTaxString( "historical %T rate" );
	
	m_pDisplayHelpers -> SetTopSingleText( strRate );
	m_pDisplayHelpers -> SetTopPluralText( strRate + "s" );
	m_pDisplayHelpers -> SetBottomSingleText( "rate" );
	m_pDisplayHelpers -> SetBottomPluralText( "rates" );
}

/**********************************************************************/

CListDataHistoricalTaxDlg::~CListDataHistoricalTaxDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataHistoricalTaxDlg::SetTitle()
{
	CString strTitle = EcrmanOptions.GetTaxString( "Setup Historical %T Rates " );
	strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), EcrmanOptions.GetGlobalTaxFlag() ); 
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataHistoricalTaxDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "&Print" );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 147 );
}

/**********************************************************************/

void CListDataHistoricalTaxDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataHistoricalTaxDlg::HandleButton1() 
{
	COleDateTime date = COleDateTime::GetCurrentTime();

	CString strDate;
	strDate.Format( "%4.4d%2.2d%2.2d",
		date.GetYear(),
		date.GetMonth(),
		date.GetDay() );

	CTaxDateInfo info;
	info.m_strDate = strDate;

	for ( int n = 0; n < MAX_TAX_BANDS; n++ )
		info.m_dRate[n] = 0.0;

	CEditHistoricalTaxDlg dlg ( info, TRUE, this );
	if ( dlg.DoModal() == IDOK )
	{
		int nIndex = DataManager.HistoricalTaxRates.SetTaxDateInfo( info );
		m_listData.SetItemCountEx ( GetArraySize() );
		SelectLine( nIndex );

		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/

void CListDataHistoricalTaxDlg::HandleButton2() 
{
	int nSelection = GetSafeSelection();
	if ( nSelection == -1 )
		return;

	int nDateCode = nSelection + 1;

	CTaxDateInfo info;
	DataManager.HistoricalTaxRates.GetTaxDateInfo( nDateCode, info );

	CEditHistoricalTaxDlg dlg ( info, FALSE, this );
	if ( dlg.DoModal() == IDOK )
	{
		DataManager.HistoricalTaxRates.DeleteDate( nDateCode );

		int nIndex = DataManager.HistoricalTaxRates.SetTaxDateInfo( info );
		m_listData.SetItemCountEx ( GetArraySize() );
		SelectLine( nIndex );

		m_WarnCancelAfterChange.SetFlag();
	}
}
		
/**********************************************************************/
	
void CListDataHistoricalTaxDlg::HandleButton3() 
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataHistoricalTaxDlg::HandleButton4() 
{
	bool bResult = PrepareReport();

	CFilenameUpdater FnUp ( SysFiles::HistoricalTaxPrm );

	if ( bResult == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
	}
}

/**********************************************************************/

bool CListDataHistoricalTaxDlg::PrepareReport()
{
	CReportFile ReportFile;
	if (ReportFile.Open(Super.ReportFilename()) == FALSE)
		return FALSE;

	ReportFile.SetStyle1(EcrmanOptions.GetTaxString("Historical %T Rates"));
	ReportFile.AddColumn("Start Date", TA_LEFT, 200);
	ReportFile.AddColumn("Rate A", TA_RIGHT, 200);
	ReportFile.AddColumn("Rate B", TA_RIGHT, 200);
	ReportFile.AddColumn("Rate C", TA_RIGHT, 200);
	ReportFile.AddColumn("Rate D", TA_RIGHT, 200);
	ReportFile.AddColumn("Rate E", TA_RIGHT, 200);
	ReportFile.AddColumn("Rate F", TA_RIGHT, 200);
	ReportFile.AddColumn("Rate G", TA_RIGHT, 200);
	ReportFile.AddColumn("Rate H", TA_RIGHT, 200);
	ReportFile.AddColumn("Rate I", TA_RIGHT, 200);
	ReportFile.AddColumn("Rate J", TA_RIGHT, 200);
	ReportFile.AddColumn("Rate K", TA_RIGHT, 200);
	ReportFile.AddColumn("Rate L", TA_RIGHT, 200);

	CCSV csv('\t');
	csv.Add("Database");

	for (int nTaxRate = 1; nTaxRate <= MAX_TAX_BANDS; nTaxRate++)
	{
		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo(nTaxRate, TaxRateInfo);

		CString str;
		str.Format("%.4f", TaxRateInfo.m_dTaxRate);
		csv.Add(str);
	}

	ReportFile.WriteLine(csv.GetLine());
	ReportFile.WriteLine("<LI>");

	for (int nDateCode = 1; nDateCode <= DataManager.HistoricalTaxRates.GetTaxDateCount(); nDateCode++)
	{
		CTaxDateInfo info;
		DataManager.HistoricalTaxRates.GetTaxDateInfo(nDateCode, info);

		CString strDate;
		strDate.Format("%s/%s/%s",
			(const char*)info.m_strDate.Right(2),
			(const char*)info.m_strDate.Mid(4, 2),
			(const char*)info.m_strDate.Left(4));

		CCSV csv('\t');
		csv.Add(strDate);

		for (int n = 0; n < MAX_TAX_BANDS; n++)
		{
			CString str;
			str.Format("%.4f", DataManager.HistoricalTaxRates.GetTaxRate(nDateCode, n + 1));
			csv.Add(str);
		}

		ReportFile.WriteLine(csv.GetLine());
		ReportFile.WriteLine("<LI>");
	}

	ReportFile.Close();
	return TRUE;
}

/**********************************************************************/
