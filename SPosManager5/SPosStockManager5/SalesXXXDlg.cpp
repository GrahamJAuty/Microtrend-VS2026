/**********************************************************************/
#include "DateRangeHelpers.h"
#include "NodeTypes.h"
#include "Password.h"
#include "RepSpawn.h"
#include "SalesXXXAutoDlg.h"
#include "SalesXXXImporterStockman.h"
#include "SalesXXXStatsDlg.h"
#include "SalesXXXStatsDiscardDlg.h"
#include "SiteCSVArray.h"
#include "StatusProgress.h"
#include "StockAuditRetrySales.h"
/**********************************************************************/
#include "SalesXXXDlg.h"
/**********************************************************************/

CSalesXXXDlg::CSalesXXXDlg( CWnd* pParent)
	: CDialog(CSalesXXXDlg::IDD, pParent),
	m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo )
{
	//{{AFX_DATA_INIT(CSalesXXXDlg)
	//}}AFX_DATA_INIT
	m_strDateFrom = "";
	m_strDateTo = "";
}

/**********************************************************************/

void CSalesXXXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSalesXXXDlg)
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSalesXXXDlg, CDialog)
	//{{AFX_MSG_MAP(CSalesXXXDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSetDates)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS, OnButtonProcess)
	ON_BN_CLICKED(IDC_BUTTON_DISCARD, OnButtonDiscard)
	ON_BN_CLICKED(IDC_BUTTON_AUTO, OnButtonAuto)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSalesXXXDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_DateSelectorRange.InitialiseDateControls();
	m_DateSelectorRange.AddStandardReportRanges();
	m_DateSelectorRange.SetComboSelection( 0, szRANGE_TODAY );
	
	CWordArray arrayTypes;

	switch( PasswordArray.GetLocationAccessType() )
	{
	case LOCATION_ACCESS_SYSTEM:
		arrayTypes.Add ( NODE_STOCKPOINT );
		arrayTypes.Add ( NODE_SITE );
		arrayTypes.Add ( NODE_DATABASE );
		arrayTypes.Add ( NODE_SYSTEM );
		break;

	case LOCATION_ACCESS_DB:
		arrayTypes.Add ( NODE_STOCKPOINT );
		arrayTypes.Add ( NODE_SITE );
		arrayTypes.Add ( NODE_DATABASE );
		m_LocationSelector.SetSingleDbIdx( PasswordArray.GetLocationAccessDbIdx() );
		break;

	case LOCATION_ACCESS_LOC:
		arrayTypes.Add ( NODE_STOCKPOINT );
		m_LocationSelector.SetSingleSpIdx( PasswordArray.GetLocationAccessSpIdx() );
		break;
	}

	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboLocation, NULL );

	if ( EcrmanOptions.GetFeaturesEposDeliveryType() != 0 )
		SetWindowText( "Retry Sales / Delivery Exceptions" );
	else
		SetWindowText( "Retry Sales Exceptions" );
	
	return TRUE;  
}

/**********************************************************************/

void CSalesXXXDlg::OnSetDates() 
{
	m_DateSelectorRange.UpdateDateControls();
}

/**********************************************************************/

void CSalesXXXDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CSalesXXXDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CSalesXXXDlg::OnButtonAuto() 
{
	CSalesXXXAutoDlg dlg( this );
	dlg.DoModal();
}

/**********************************************************************/

void CSalesXXXDlg::OnButtonView() 
{
	COleDateTime oleDateFrom, oleDateTo;
	m_DatePickerFrom.GetTime ( oleDateFrom );
	m_DatePickerTo.GetTime ( oleDateTo );

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	
	CString strConTitle = LocSelEntity.GetConsolidationTitle();
	
	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );
	
	CStockAuditRetrySales report;
	report.SetAuditMode( AUDIT_MODE_RETRY );
	report.SetAuditDateFrom( oleDateFrom );
	report.SetAuditDateTo( oleDateTo );
	report.SetReportType ( AUDIT_RETRY_SALES );
	report.SetConLevel ( LocSelEntity.GetConType() );
	report.SetTableNo ( LocSelEntity.GetEntityNo() );
	report.SetConLevelText ( strConTitle );
	report.SetHeaderReferenceFilter( "" );
	report.SetLineReferenceFilter( "" );
	
	StatusProgress.Lock( TRUE, "Creating report" );
	bool bResult = ( report.CreateReport( LocSelEntity.GetDbIdx() ) );
	StatusProgress.Unlock();

	if ( bResult == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), report.GetReportParamsFilename(), "", this );
	}
}

/**********************************************************************/

void CSalesXXXDlg::GetSpIdxArray( CWordArray& arraySpIdx )
{
	arraySpIdx.RemoveAll();

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	switch( LocSelEntity.GetConType() )
	{
	case NODE_STOCKPOINT:
		arraySpIdx.Add( LocSelEntity.GetEntityIdx() );
		break;

	case NODE_DATABASE:
		{
			int nStartIdx, nEndIdx;
			dbStockpoint.GetSpIdxRange( LocSelEntity.GetDbIdx(), nStartIdx, nEndIdx );

			for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
				arraySpIdx.Add( nSpIdx );
		}
		break;

	case NODE_SITE:
		{
			int nStartIdx, nEndIdx;
			dbStockpoint.GetSpIdxRange( LocSelEntity.GetDbIdx(), nStartIdx, nEndIdx );

			int nSiteNo = dbSite.GetSiteNo( LocSelEntity.GetEntityIdx() );

			for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
				if ( dbStockpoint.GetSiteNo( nSpIdx ) == nSiteNo )
					arraySpIdx.Add( nSpIdx );
		}
		break;

	case NODE_SYSTEM:
		{
			for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
				arraySpIdx.Add( nSpIdx );
		}
		break;
	}
}

/**********************************************************************/

void CSalesXXXDlg::GetDateInfo( CString& strDate, COleDateTime& oleDateFrom, COleDateTime& oleDateTo )
{
	m_DatePickerFrom.GetTime ( oleDateFrom );
	m_DatePickerTo.GetTime ( oleDateTo );

	m_comboDates.GetWindowText( strDate );

	if ( strDate.Find( "ange", 0 ) != -1 )
	{
		strDate.Format ( "%2.2d/%2.2d/%4.4d - %2.2d/%2.2d/%4.4d",
			oleDateFrom.GetDay(),
			oleDateFrom.GetMonth(),
			oleDateFrom.GetYear(),
			oleDateTo.GetDay(),
			oleDateTo.GetMonth(),
			oleDateTo.GetYear() );
	}
	else if ( strDate.Find( "ustom", 0 ) != -1 )
	{
		strDate.Format ( "%2.2d/%2.2d/%4.4d",
			oleDateFrom.GetDay(),
			oleDateFrom.GetMonth(),
			oleDateFrom.GetYear() );;
	}
}

/**********************************************************************/

void CSalesXXXDlg::OnButtonProcess() 
{
	CString strDate;
	COleDateTime oleDateFrom, oleDateTo;
	GetDateInfo( strDate, oleDateFrom, oleDateTo );
	
	CWordArray arraySpIdx;
	GetSpIdxArray( arraySpIdx );

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	CString strConTitle = LocSelEntity.GetConsolidationTitle();
		
	CString strTitle;
	strTitle.Format ( "Retry Exceptions (%s, %s)",
		(const char*) strConTitle,
		(const char*) strDate );
	
	CStringArray arrayWarning;
	arrayWarning.Add( strTitle );
	arrayWarning.Add( "Sales and wastage exceptions for these dates will be retried." );
	arrayWarning.Add( "" );

	if ( EcrmanOptions.GetFeaturesEposDeliveryType() != 0 )
	{
		arrayWarning.Add( "Delivery exceptions for these dates will be retried." );
		arrayWarning.Add( "" );
	}

	arrayWarning.Add( "Apparent stock levels will be updated as appropriate." );
	arrayWarning.Add( "" );
	arrayWarning.Add( "The stock sales history will be updated as appropriate." );
	arrayWarning.Add( "" );
	arrayWarning.Add( "Sales and adjustment audits will be updated as appropriate." );

	if ( EcrmanOptions.GetFeaturesEposDeliveryType() != 0 )
	{
		arrayWarning.Add( "" );
		arrayWarning.Add( "Delivery audits will be updated as appropriate." );
	}

	if ( Prompter.Warning( arrayWarning, FALSE ) != IDYES )
		return;

	CSalesXXXImporterStockman SalesXXXImporter( FALSE );
	SalesXXXImporter.ProcessSales( arraySpIdx, oleDateFrom, oleDateTo );

	CSalesXXXStatsDlg dlg ( this );
	dlg.m_nNumber1 = SalesXXXImporter.m_nFilesFound;
	dlg.m_nNumber2 = SalesXXXImporter.m_nFilesOpened;
	dlg.m_nNumber3 = SalesXXXImporter.m_nDataLines;
	dlg.m_nNumber4 = SalesXXXImporter.m_nSalesLines;
	dlg.m_nNumber5 = SalesXXXImporter.m_nWasteLines;
	dlg.m_nNumber6 = SalesXXXImporter.m_nDeliveryLines;
	dlg.m_nNumber7 = SalesXXXImporter.m_nFiledLines;
	dlg.DoModal();
}

/**********************************************************************/

void CSalesXXXDlg::OnButtonDiscard() 
{
	CString strDate;
	COleDateTime oleDateFrom, oleDateTo;
	GetDateInfo( strDate, oleDateFrom, oleDateTo );

	CWordArray arraySpIdx;
	GetSpIdxArray( arraySpIdx );
	
	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	CString strConTitle = LocSelEntity.GetConsolidationTitle();
		
	CString strTitle;
	strTitle.Format ( "Discard Exceptions (%s, %s)",
		(const char*) strConTitle,
		(const char*) strDate );
	
	CStringArray arrayWarning;
	arrayWarning.Add( strTitle );
	arrayWarning.Add( "Sales and wastage exceptions for these dates will be discarded." );
	arrayWarning.Add( "" );

	if ( EcrmanOptions.GetFeaturesEposDeliveryType() != 0 )
	{
		arrayWarning.Add( "Delivery exceptions for these dates will be discarded." );
		arrayWarning.Add( "" );
	}

	arrayWarning.Add( "Apparent stock levels and history files will not change." );
	arrayWarning.Add( "" );
	arrayWarning.Add( "You will not be able to access these exceptions again." );
	
	if ( Prompter.Warning( arrayWarning, FALSE ) != IDYES )
		return;

	CSalesXXXImporterStockman SalesXXXImporter( TRUE );
	SalesXXXImporter.ProcessSales( arraySpIdx, oleDateFrom, oleDateTo );

	CSalesXXXStatsDiscardDlg dlg ( this );
	dlg.m_nNumber1 = SalesXXXImporter.m_nFilesFound;
	dlg.m_nNumber2 = SalesXXXImporter.m_nFilesOpened;
	dlg.DoModal();
}

/**********************************************************************/

