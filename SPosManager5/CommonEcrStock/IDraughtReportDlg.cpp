/**********************************************************************/
#include "DateRangeHelpers.h"
//#include "EcrmanOptionsIni.h"
//#include "globalfunctions.h"
#include "IDraughtExportManager.h"
#include "IDraughtOptions.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "RepSpawn.h"
/**********************************************************************/
#include "IDraughtReportDlg.h"
/**********************************************************************/

CIDraughtReportDlg::CIDraughtReportDlg(CWnd* pParent)
	: CDialog(CIDraughtReportDlg::IDD, pParent),
	m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo )
{
	//{{AFX_DATA_INIT(CIDraughtReportDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CIDraughtReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIDraughtReportDlg)
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CIDraughtReportDlg, CDialog)
	//{{AFX_MSG_MAP(CIDraughtReportDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, OnButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, OnButtonCheck)
	ON_BN_CLICKED(IDC_BUTTON_LOG, OnButtonLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CIDraughtReportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_DateSelectorRange.InitialiseDateControls();
	m_DateSelectorRange.SetMode( DATE_SELECTOR_MODE_STANDARD );
	m_DateSelectorRange.AddPastReportRanges();
	m_DateSelectorRange.SetComboSelection( 0, szRANGE_CUSTOM_RANGE );

	COleDateTime dateNow = COleDateTime::GetCurrentTime();
	COleDateTime dateTo = COleDateTime( dateNow.GetYear(), dateNow.GetMonth(), dateNow.GetDay(), 0, 0, 0 );
	
	CDateRangeHelpers DateRangeHelpers;
	COleDateTime dateFrom = dateTo;
	DateRangeHelpers.AddDaysToTime( dateFrom, -14 );

	m_DatePickerTo.SetTime( dateTo );
	m_DatePickerFrom.SetTime( dateFrom );

	m_listReports.SubclassDlgItem ( IDC_LIST, this );	
	m_listReports.InsertColumn ( 0, "Date", LVCFMT_LEFT, 80 );
	m_listReports.InsertColumn ( 1, "Files Exported", LVCFMT_RIGHT, 100 );
	m_listReports.SetExtendedStyle ( LVS_EX_FULLROWSELECT );
	
	OnButtonUpdate();

	if ( m_listReports.GetItemCount() > 0 )
	{
		m_listReports.SetSelectionMark( 0 );
		m_listReports.SetItemState( 0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );
		m_listReports.EnsureVisible( 0, FALSE );
		m_listReports.SetFocus();
	}

	return FALSE;  
}

/**********************************************************************/

void CIDraughtReportDlg::OnSelectDate() 
{
	m_DateSelectorRange.UpdateDateControls();
}

/**********************************************************************/

void CIDraughtReportDlg::GetReportDates()
{
	COleDateTime dateFrom,dateTo;
	m_DatePickerFrom.GetTime( dateFrom );
	m_DatePickerTo.GetTime( dateTo );
	m_OleDateFrom = dateFrom;
	m_OleDateTo = dateTo;
}

/**********************************************************************/

void CIDraughtReportDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CIDraughtReportDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CIDraughtReportDlg::OnButtonUpdate()
{
	m_arrayDates.RemoveAll();
	m_listReports.DeleteAllItems();
	GetReportDates();

	CString strDateFrom;
	strDateFrom.Format ( "%4.4d%2.2d%2.2d",
		m_OleDateFrom.GetYear(),
		m_OleDateFrom.GetMonth(),
		m_OleDateFrom.GetDay() );

	CString strDateTo;
	strDateTo.Format ( "%4.4d%2.2d%2.2d",
		m_OleDateTo.GetYear(),
		m_OleDateTo.GetMonth(),
		m_OleDateTo.GetDay() );

	if ( strDateFrom <= strDateTo )
	{
		CString strSentPath = "IDraught\\Sent\\";
		GetDataProgramPath( strSentPath );

		CFileFind finder;
		BOOL bWorking = finder.FindFile ( strSentPath + "*.csv" );

		while ( bWorking == TRUE )
		{
			bWorking = finder.FindNextFile();

			if ( finder.IsDirectory() == TRUE )
				continue;

			if ( finder.IsDots() == TRUE )
				continue;

			CString strFilename = finder.GetFileName();
			if ( strFilename.GetLength() != 17 )
				continue;

			CString strDate = strFilename.Left(8);
			if ( ::TestNumeric( strFilename.Left(8) ) == FALSE )
				continue;

			if ( ( strDate < strDateFrom ) || ( strDate > strDateTo ) )
				continue;

			CSortedStringCounter item;
			item.m_strItem = strDate;
			m_arrayDates.Consolidate( item );
		}

		for ( int n = m_arrayDates.GetSize() - 1; n >= 0; n-- )
		{
			CSortedStringCounter item;
			m_arrayDates.GetAt( n, item );
			CString strDate = item.m_strItem;

			CString strDisplayDate;
			strDisplayDate.Format( "%s/%s/%s",
				(const char*) strDate.Right(2),
				(const char*) strDate.Mid(4,2),
				(const char*) strDate.Left(4) );

			CString strFileCount;
			strFileCount.Format( "%d", item.m_nCount );

			int nPos = m_listReports.GetItemCount();
			m_listReports.InsertItem ( nPos, "" );
			m_listReports.SetItem ( nPos, 0, LVIF_TEXT, strDisplayDate, 0, 0, 0, NULL );
			m_listReports.SetItem ( nPos, 1, LVIF_TEXT, strFileCount, 0, 0, 0, NULL );
		}
	}
}

/**********************************************************************/

void CIDraughtReportDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ShowReport();
	*pResult = 0;
}

/**********************************************************************/

void CIDraughtReportDlg::OnButtonView() 
{
	ShowReport();

	int nSel = m_listReports.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_arrayDates.GetSize() ) )
	{
		m_listReports.SetSelectionMark( nSel );
		m_listReports.SetItemState( nSel, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );
		m_listReports.EnsureVisible( nSel, FALSE );
		m_listReports.SetFocus();
	}
}

/**********************************************************************/

void CIDraughtReportDlg::ShowReport()
{
	int nSel = m_listReports.GetCurSel();
	if ( nSel < 0 || nSel >= m_arrayDates.GetSize() )
		return;

	int nIdx = m_arrayDates.GetSize() - nSel - 1;

	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return;

	CSortedStringCounter item;
	m_arrayDates.GetAt( nIdx, item );
	CString strDate = item.m_strItem;

	CString strTitle;
	strTitle.Format( "i Draught Sales Export for %s/%s/%s",
		(const char*) strDate.Right(2),
		(const char*) strDate.Mid(4,2),
		(const char*) strDate.Left(4) );

	ReportFile.SetStyle1 ( strTitle );
		
	ReportFile.AddColumn ( "Site ID", TA_LEFT, 350 );
	ReportFile.AddColumn ( "Time", TA_LEFT, 350 );
	ReportFile.AddColumn ( "Plu No", TA_LEFT, 350 );
	ReportFile.AddColumn ( "Description", TA_LEFT, 600 );
	ReportFile.AddColumn ( "Qty", TA_RIGHT, 200 );
		
	CString strSentPath = "IDraught\\Sent\\";
	GetDataProgramPath( strSentPath );

	CReportConsolidationArray<CSortedStringItem> arrayFilenames;

	CFileFind finder;
	BOOL bWorking = finder.FindFile ( strSentPath + strDate + "*.csv" );
	
	while ( bWorking == TRUE )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDirectory() == TRUE )
			continue;

		if ( finder.IsDots() == TRUE )
			continue;

		CString strFilename = finder.GetFileName();

		if ( strFilename.GetLength() == 17 )
		{
			CSortedStringItem item;
			item.m_strItem = strFilename;
			arrayFilenames.Consolidate( item );
		}
	}

	for ( int n = 0; n < arrayFilenames.GetSize(); n++ )
	{
		CSortedStringItem item;
		arrayFilenames.GetAt( n, item );
		CString strFilename = item.m_strItem;

		int nNetworkIdx;
		int nNetworkNo = atoi( strFilename.Mid( 10, 3 ) );
		CString strNetworkName = "";
		if ( dbNetwork.FindNetworkByNumber( nNetworkNo, nNetworkIdx ) == TRUE )
			strNetworkName.Format( "<..>N%3.3d, %s", nNetworkNo, dbNetwork.GetName( nNetworkIdx ) );
		else
			strNetworkName.Format( "<..>N%3.3d, Unknown Netwotk", nNetworkNo );

		ReportFile.WriteLine( strNetworkName );
		ReportFile.WriteLine( "<LI>" );

		CSSFile fileSent;
		if ( fileSent.Open( strSentPath + strFilename, "rb" ) == TRUE )
		{
			CString strBuffer;
			while( fileSent.ReadString( strBuffer ) == TRUE )
			{
				CCSV csvIn( strBuffer );

				CCSV csvOut ( '\t' );
				csvOut.Add( csvIn.GetString(0) );
				csvOut.Add( csvIn.GetString(1) );
				csvOut.Add( csvIn.GetString(2) );
				csvOut.Add( csvIn.GetString(3) );
				csvOut.Add( csvIn.GetString(4) );
				ReportFile.WriteLine ( csvOut.GetLine() );
			}
		}
		else
			ReportFile.WriteLine( "<..>Unable to open file" );

		ReportFile.WriteLine( "" );
	}
	
	ReportFile.Close();

	CReportManager ReportManager;
	CFilenameUpdater FnUp ( SysFiles::IDraughtReportPrm );
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}

/**********************************************************************/

void CIDraughtReportDlg::OnButtonCheck()
{
	if ( IDraughtOptions.GetFiddleLevel() != 0 )
	{
		if ( Prompter.OkCancel( "Reset export dates" ) == IDOK )
			IDraughtExportManager.ResetExportDatesForTesting();
	}

	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return;

	CArray<CIDraughtExportStatus,CIDraughtExportStatus> arrayStatus;
	IDraughtExportManager.BuildNetworkStatusArray( arrayStatus );

	ReportFile.SetStyle1 ( "i Draught Export Network Status" );
		
	ReportFile.AddColumn ( "No", TA_LEFT, 150 );
	ReportFile.AddColumn ( "Name", TA_LEFT, 400 );
	ReportFile.AddColumn ( "Last Export", TA_LEFT, 350 );
	ReportFile.AddColumn ( "Status", TA_LEFT, 700 );
	
	for ( int n = 0; n < arrayStatus.GetSize(); n++ )
	{
		CString strNo;
		strNo.Format( "N%3.3d", dbNetwork.GetNetworkNo( n ) );

		CCSV csv( '\t' );
		csv.Add( strNo );
		csv.Add( dbNetwork.GetName( n ) );
		
		switch( arrayStatus[n].m_nStatus )
		{
		case IDRAUGHT_EXPORT_STATUS_OK:
			{
				COleDateTime dateExport = arrayStatus[n].m_dateLastExport;

				COleDateTime dateInitial;
				IDraughtExportManager.SetInitialExportDate( dateInitial );

				CString strDate = "";

				if ( dateExport > dateInitial )
				{
					strDate.Format( "%2.2d/%2.2d/%4.4d",
						dateExport.GetDay(),
						dateExport.GetMonth(),
						dateExport.GetYear() );
				}

				csv.Add( strDate );
				csv.Add( "OK" );
			}
			break;

		case IDRAUGHT_EXPORT_STATUS_NOT_REALTIME:
			csv.Add( "" );
			csv.Add( "Not configured for real time comms" );
			break;

		case IDRAUGHT_EXPORT_STATUS_NO_LOCATIONS:
			csv.Add( "" );
			csv.Add( "Network contains no locations" );
			break;

		case IDRAUGHT_EXPORT_STATUS_NO_DBLINK:
			csv.Add( "" );
			csv.Add( "Locations not linked to databases" );
			break;

		case IDRAUGHT_EXPORT_STATUS_BAD_DATE:
			csv.Add( "" );
			csv.Add( "Unable to read last export date" );
			break;

		case IDRAUGHT_EXPORT_STATUS_WAIT_DATE:
			csv.Add( "" );
			csv.Add( "Waiting to check last export date" );
			break;
		}

		ReportFile.WriteLine( csv.GetLine() );
		ReportFile.WriteLine( "<LI>" );
	}

	ReportFile.WriteLine( "" );

	switch( dbLocation.GetIDraughtStatus() )
	{
	case IDRAUGHT_STATUS_INVALID_TERMINAL:
		ReportFile.WriteLine( "<..>IDraught Export is disabled due to duplicated terminal numbers." );
		break;

	case IDRAUGHT_STATUS_INVALID_SITE:
		ReportFile.WriteLine( "<..>IDraught Export is disabled due to duplicated or missing site IDs." );
		break;

	case IDRAUGHT_STATUS_INVALID_DBLINK:
		ReportFile.WriteLine( "<..>IDraught Export is disabled due to one or more sites having no database link." );
		break;
	}

	switch( dbLocation.GetIDraughtStatus() )
	{
	case IDRAUGHT_STATUS_VALID:
	case IDRAUGHT_STATUS_UNKNOWN:
		{
			COleDateTime dateNow = COleDateTime::GetCurrentTime();
			COleDateTime dateExport = COleDateTime( dateNow.GetYear(), dateNow.GetMonth(), dateNow.GetDay(), 0, 0, 0 );

			CDateRangeHelpers DateRangeHelpers;
			DateRangeHelpers.AddDaysToTime( dateExport, 1 );
			dateExport += COleDateTimeSpan ( 0, EcrmanOptions.GetReportsDayStartHour(), 0, 0 );
			dateExport += COleDateTimeSpan ( 0, IDraughtOptions.GetExportDelayTime(), 0, 0 );

			CString strMsg;
			strMsg.Format( "<..>IDraught Export for today (%2.2d/%2.2d/%4.4d) is due at %2.2d:00 on %2.2d/%2.2d/%4.4d",
				dateNow.GetDay(),
				dateNow.GetMonth(),
				dateNow.GetYear(),
				dateExport.GetHour(),
				dateExport.GetDay(),
				dateExport.GetMonth(),
				dateExport.GetYear() );

			ReportFile.WriteLine( "" );
			ReportFile.WriteLine( strMsg );
		}
		break;
	}

	ReportFile.Close();

	CReportManager ReportManager;
	CFilenameUpdater FnUp ( SysFiles::IDraughtReportPrm );
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}

/**********************************************************************/

void CIDraughtReportDlg::OnButtonLog()
{
	CFilenameUpdater FnUp ( SysFiles::IDraughtLogPrm );

	if ( ::FileExists ( Super.IDraughtLogAll() ) == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.IDraughtLogAll(), FnUp.GetFilenameToUse(), "", this );
	}
	else
		Prompter.Error ( "The i Draught Log file was not found." );
}

/**********************************************************************/
