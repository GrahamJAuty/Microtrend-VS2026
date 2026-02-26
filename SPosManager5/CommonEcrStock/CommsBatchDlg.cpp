/**********************************************************************/
#include <process.h>
/**********************************************************************/
#include "Append.h"
#include "BatchSalesExporter.h"
#include "CommsErrors.h"
#include "CommsExportAllergen.h"
#include "CommsExportBarcode.h"
#include "CommsExportBillText.h"
#include "CommsExportButtonImages.h"
#include "CommsExportCategory.h"
#include "CommsExportDept.h"
#include "CommsExportFunction.h"
#include "CommsExportGroup.h"
#include "CommsExportKeyboard.h"
#include "CommsExportLoyaltyScheme.h"
#include "CommsExportMacro.h"
#include "CommsExportPromoEtc.h"
#include "CommsExportModifiers.h"
#include "CommsExportPaidInOut.h"
#include "CommsExportPayment.h"
#include "CommsExportPlu.h"
#include "CommsExportPluInfo.h"
#include "CommsExportReason.h"
#include "CommsExportServerName.h"
#include "CommsExportServerPreset.h"
#include "CommsExportTableArea.h"
#include "CommsExportTableName.h"
#include "CommsExportTax.h"
#include "CommsExportTimePlan.h"
#include "DateRangeHelpers.h"
#include "EODFileMover.h"
#include "SPOSVersions.h"
#include "FileRemover.h"
#include "FtpConnect.h"
#include "FtpConnectDlg.h"
#include "HttpHelpers.h"
#include "JobNumbers.h"
#include "KeyboardJobInfo.h"
#include "LocationCSVArray.h"
#include "MessageLogger.h"
#include "NetworkCSVArray.h"
#include "PluInfoResourceWrapper.h"
#include "RepSpawn.h"
#include "SmartEntertainmentWebAPISalesDate.h"
#include "SmartEntertainmentXMLWrangler.h"
#include "SpecialChangesHelper.h"
#include "SportsBookerTransaction.h"
#include "SportsBookerWebAPISalesDateTime.h"
#include "SportsBookerXMLWrangler.h"
#include "SysInfo.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "CommsBatchDlg.h"
/**********************************************************************/
static const UINT SCHED_TIMER = 1;
/**********************************************************************/
static const int STOPPED = 0;
static const int RUNNING = 1;
/**********************************************************************/

CCommsBatchDlg::CCommsBatchDlg( CEposTaskDatabase& TaskDatabase, CString& strDate, bool bFromMemory, CKeyboardMap& KeyboardMap, int nDbIdx, int nAutoMode, int nAutoCloseDelay, CWnd* pParent)
	: CDialog(CCommsBatchDlg::IDD, pParent), m_KeyboardMap( KeyboardMap )
{
	//{{AFX_DATA_INIT(CCommsBatchDlg)
	//}}AFX_DATA_INIT
	m_pDownloadMasters = NULL;
	DataInit( TaskDatabase, strDate, bFromMemory, KeyboardMap, nDbIdx, nAutoMode, nAutoCloseDelay );
}

/**********************************************************************/

CCommsBatchDlg::CCommsBatchDlg( CEposTaskDatabase& TaskDatabase, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>* pDownloadMasters, CString& strDate, CKeyboardMap& KeyboardMap, CWnd* pParent )
	: CDialog(CCommsBatchDlg::IDD, pParent), m_KeyboardMap( KeyboardMap )
{
	//{{AFX_DATA_INIT(CCommsBatchDlg)
	//}}AFX_DATA_INIT
	m_pDownloadMasters = pDownloadMasters;
	DataInit( TaskDatabase, strDate, FALSE, KeyboardMap, -1, 0, 0 );
}

/**********************************************************************/

void CCommsBatchDlg::DataInit( CEposTaskDatabase& TaskDatabase, CString& strDate, bool bFromMemory, CKeyboardMap& KeyboardMap, int nDbIdx, int nAutoMode, int nAutoCloseDelay )
{
	CFilenameUpdater FnUp( SysFiles::TempCommsLog );
	m_strTempLogFilename = FnUp.GetFilenameToUse();
	CFileRemover FileRemover( m_strTempLogFilename );

	CReportFile ReportFile;
	ReportFile.Open ( m_strTempLogFilename );
	ReportFile.SetStyle1 ( "Communications Log" );
	ReportFile.AddColumn ( "",TA_LEFT, 300 );
	ReportFile.AddColumn ( "",TA_LEFT, 900 );
	ReportFile.AddColumn ( "",TA_RIGHT, 400 );
	ReportFile.AddColumn ( "",TA_LEFT, 1200 );
	ReportFile.WriteLine ( "" );
	ReportFile.Close();

	m_nAutoMode = nAutoMode;
	if ( ( m_nAutoMode < 0 ) || ( m_nAutoMode > 2 ) )
		m_nAutoMode = 0;

	m_nAutoCloseDelay = nAutoCloseDelay;
	if ( ( m_nAutoCloseDelay < 1 ) || ( m_nAutoCloseDelay > 60 ) )
		m_nAutoCloseDelay = 5;

	m_bCanAutoClose = FALSE;
	m_bGotSales = FALSE;
	m_bCanRestart = TRUE;

	CEposTaskHeader TaskHeader;
	TaskDatabase.GetEditableTaskHeader( TaskHeader );
	m_nHours = TaskHeader.GetHours();
	m_nMinutes = TaskHeader.GetMinutes();
	m_strTaskName = TaskHeader.GetName();
	m_bFromMemory = bFromMemory;

	int nJobCount = m_JobArray.BuildFromTaskRecord( TaskDatabase, strDate, m_arrayJobSetupInfo, nDbIdx, m_pDownloadMasters );

	int nRejects = m_arrayJobSetupInfo.GetSize() - nJobCount;
	CString strJobs1 = ( nJobCount == 1 ) ? "Job" : "Jobs";
	CString strJobs2 = ( nRejects == 1 ) ? "Job" : "Jobs";

	m_strJobCount.Format ( "%d %s accepted, %d %s rejected, click here for details",
		nJobCount, (const char*) strJobs1, nRejects, (const char*) strJobs2 );
		 
	m_nStatus = STOPPED;
}

/**********************************************************************/

void CCommsBatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommsBatchDlg)
	DDX_Control(pDX, IDC_BUTTON_STOP, m_buttonStop);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDC_LIST, m_listJobs);
	DDX_Control(pDX, IDC_BUTTON_RETRY, m_buttonRetry);
	DDX_Control(pDX, IDC_STATIC_INFO, m_staticInfoJobCount);
	DDX_Control(pDX, IDC_STATIC_INFO2, m_staticInfo2);
	DDX_Control(pDX, IDC_STATIC_INFO3, m_staticInfo3);
	DDX_Control(pDX, IDC_STATIC_INFO4, m_staticInfo4);
	DDX_Control(pDX, IDC_STATIC_INFO5, m_staticInfo5);
	DDX_Control(pDX, IDC_BUTTON_START, m_buttonStart);
	DDX_Control(pDX, IDC_BUTTON_TIMELOG, m_buttonTimelog);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CCommsBatchDlg, CDialog)
	//{{AFX_MSG_MAP(CCommsBatchDlg)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_TIMELOG, OnButtonTimelog)
	ON_BN_CLICKED(IDC_BUTTON_RETRY, OnButtonRetry)
	ON_LBN_SELCHANGE(IDC_LIST, OnSelectJob)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STATIC_INFO, OnStaticInfo)
	ON_MESSAGE(WM_START_COMMS, OnStartComms)
	ON_MESSAGE(WM_CLOSE_COMMS, OnCloseComms)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CCommsBatchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	if (( m_hTimer = SetTimer ( SCHED_TIMER, 1000, NULL )) == 0 )
	{
		Prompter.Error ( "Unable to start timer for scheduler" );
		EndDialog ( IDOK );
		return TRUE;
	}

	m_buttonStop.EnableWindow( FALSE );

	int nTabStops [ 5 ] = { 16, 32, 250, 320 };
	m_listJobs.SetTabStops ( 4, nTabStops );

	m_nStatus = STOPPED;

	BuildList();
	m_listJobs.SetCurSel(0);
	OnSelectJob();

	CString strTitle;
	strTitle.Format ( "Batch Communications (%s)", (const char*) m_strTaskName );
	SetWindowText( strTitle );

	m_staticInfoJobCount.SetWindowText( m_strJobCount );

	if ( m_nAutoMode != 0 )
		PostMessage( WM_START_COMMS, 0, 0 );

	return TRUE;  
}

/**********************************************************************/

void CCommsBatchDlg::OnTimer(UINT nIDEvent) 
{
	switch ( nIDEvent )
	{
	case SCHED_TIMER:
		if ( CTime::GetCurrentTime() >= m_JobReadyTime )
			m_bJobReady = TRUE;

		if ( TRUE == m_bCanAutoClose )
		{
			COleDateTime timeNow = COleDateTime::GetCurrentTime();

			if ( timeNow >= m_timeAutoClose )
			{
				m_bCanAutoClose = FALSE;
				PostMessage( WM_CLOSE_COMMS, 0, 0 );
			}
		}

		break;
	}
}

/**********************************************************************/

long CCommsBatchDlg::OnStartComms( WPARAM w, LPARAM l )
{
	OnButtonStart();

	if ( 2 == m_nAutoMode )
	{
		m_timeAutoClose = COleDateTime::GetCurrentTime();
		m_timeAutoClose += COleDateTimeSpan( 0, 0, 0, m_nAutoCloseDelay );
		m_bCanAutoClose = TRUE;
	}

	return 0;
}

/**********************************************************************/

void CCommsBatchDlg::OnButtonStart() 
{
	m_bCanAutoClose = FALSE;

	FtpConnect.Disconnect();
	HandleStart();
	FtpConnect.Disconnect();

	if ( m_buttonStart.IsWindowEnabled() == TRUE )
	{
		GotoDlgCtrl( GetDlgItem( IDC_BUTTON_TIMELOG ) );
		GotoDlgCtrl( GetDlgItem( IDC_BUTTON_START ) );
	}
	else
		GotoDlgCtrl( GetDlgItem( IDC_BUTTON_TIMELOG ) );			
}

/**********************************************************************/

void CCommsBatchDlg::HandleStart()
{
	m_buttonStart.EnableWindow( FALSE );
	m_buttonRetry.EnableWindow( FALSE );
	m_buttonCancel.EnableWindow( FALSE );
	m_buttonStop.EnableWindow( TRUE );

	m_nStatus = RUNNING;

	if ( m_nHours != 24 )
	{
		CTime timeNow = CTime::GetCurrentTime();
		int nHourNow = timeNow.GetHour();
		int nMinuteNow = timeNow.GetMinute();

		m_JobReadyTime = CTime ( timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay(), m_nHours, m_nMinutes, 0, -1 );

		if ( nHourNow > m_nHours )
		{
			CDateRangeHelpers DateRangeHelpers;
			DateRangeHelpers.AddDaysToTime( m_JobReadyTime, 1 );
		}
		else if ( ( nHourNow == m_nHours ) && ( nMinuteNow > m_nMinutes ) )
		{
			CDateRangeHelpers DateRangeHelpers;
			DateRangeHelpers.AddDaysToTime( m_JobReadyTime, 1 );
		}

		m_bJobReady = FALSE;

		CString strInfo;
		strInfo.Format ( "Waiting for start time %2.2d:%2.2d",
			m_JobReadyTime.GetHour(),
			m_JobReadyTime.GetMinute() );

		ShowStatusHeaderInfo( strInfo );
	
		WaitExit( m_bJobReady, 10 );

		if ( m_nStatus == STOPPED )
		{
			m_buttonStart.EnableWindow( m_bCanRestart );
			m_buttonRetry.EnableWindow( TRUE );
			m_buttonStop.EnableWindow( FALSE );
			m_buttonCancel.EnableWindow( TRUE );
			ShowStatusHeaderInfo( "" );
			return;
		}
	}

	m_bJobReady = TRUE;	
	ShowStatusHeaderInfo( "" );

	m_bCanRestart = FALSE;
	AttemptJobs ( 0, m_JobArray.GetSize() - 1 );
	ShowStatusHeaderInfo( "" );
	OnSelectJob();

	m_buttonStart.EnableWindow( m_bCanRestart );
	m_buttonRetry.EnableWindow( TRUE );
	m_buttonStop.EnableWindow( FALSE );
	m_buttonCancel.EnableWindow( TRUE );

	m_nStatus = STOPPED;
}

/**********************************************************************/

void CCommsBatchDlg::OnButtonRetry()
{
	m_bCanAutoClose = FALSE;

	FtpConnect.Disconnect();
	HandleRetry();
	FtpConnect.Disconnect();

	if ( m_buttonRetry.IsWindowEnabled() == TRUE )
	{
		GotoDlgCtrl( GetDlgItem( IDC_BUTTON_TIMELOG ) );
		GotoDlgCtrl( GetDlgItem( IDC_BUTTON_RETRY ) );
	}
	else
		GotoDlgCtrl( GetDlgItem( IDC_BUTTON_TIMELOG ) );
}

/**********************************************************************/

void CCommsBatchDlg::HandleRetry() 
{
	int nSelection = m_listJobs.GetCurSel();
	if ( nSelection < 0 || nSelection >= m_JobArray.GetSize() )
		return;

	CJobListCSVRecord JobInfo;
	m_JobArray.GetAt ( nSelection, JobInfo );

	CString strPrompt;
	switch ( JobInfo.GetLineType() )
	{
	case JOBLIST_LINETYPE_JOB:			strPrompt = "Are you sure you wish to retry this job";	break;
	case JOBLIST_LINETYPE_LOCATION:		strPrompt = "Are you sure you wish to retry all\nunfinished jobs at this location";	break;
	case JOBLIST_LINETYPE_NETWORK:		strPrompt = "Are you sure you wish to retry all\nunfinished jobs on this network";	break;
	default:	return;
	}

	if ( Prompter.YesNo ( strPrompt ) != IDYES )
		return;

	int nEndIdx;
	int nStartIdx = nSelection;
	
	switch ( JobInfo.GetLineType() )
	{
	case JOBLIST_LINETYPE_JOB:
		nEndIdx = nStartIdx;
		break;

	case JOBLIST_LINETYPE_LOCATION:
		{
			for ( nEndIdx = nStartIdx + 1; nEndIdx < m_JobArray.GetSize(); nEndIdx++ )
			{
				CJobListCSVRecord JobInfo;
				m_JobArray.GetAt ( nEndIdx, JobInfo );
				if ( JobInfo.GetLineType() != JOBLIST_LINETYPE_JOB )
					break;
			}
			nEndIdx--;
		}
		break;

	case JOBLIST_LINETYPE_NETWORK:
		{
			for ( nEndIdx = nStartIdx + 1; nEndIdx < m_JobArray.GetSize(); nEndIdx++ )
			{
				CJobListCSVRecord JobInfo;
				m_JobArray.GetAt ( nEndIdx, JobInfo );
				if ( JobInfo.GetLineType() == JOBLIST_LINETYPE_NETWORK )
					break;
			}
			nEndIdx--;
		}
		break;
	}

	m_nStatus = RUNNING;

	m_buttonStart.EnableWindow( FALSE );
	m_buttonRetry.EnableWindow( FALSE );
	m_buttonCancel.EnableWindow( FALSE );
	m_buttonStop.EnableWindow( TRUE );

	AttemptJobs ( nStartIdx, nEndIdx );
	ShowStatusHeaderInfo( "" );
	OnSelectJob();

	m_buttonStart.EnableWindow( m_bCanRestart );
	m_buttonRetry.EnableWindow( TRUE );
	m_buttonCancel.EnableWindow( TRUE );
	m_buttonStop.EnableWindow( FALSE );

	m_nStatus = STOPPED;
}

/**********************************************************************/

void CCommsBatchDlg::AttemptJobs( int nStartIdx, int nEndIdx )
{	
	int nCurrentNetworkIdx = -1;
	int nCurrentConnectionType = -1;
	bool bWantDisconnect = FALSE;
	CString strNetworkPath = "";

	CSSFile fileLock;
	bool bGotFileLock = FALSE;

	for ( int nIndex = nStartIdx; nIndex <= nEndIdx; nIndex++ )
	{
		CJobListCSVRecord JobInfo;
		m_JobArray.GetAt ( nIndex, JobInfo );

		if ( JobInfo.GetLineType() != JOBLIST_LINETYPE_JOB )
			continue;

		if ( JobInfo.CanRetryJob() == FALSE )
			continue;

		JobInfo.SetStatus ( JOBLIST_STATUS_NOTFINISHED );
		m_JobArray.SetAt ( nIndex, JobInfo );
		UpdateLine ( nIndex );
		m_listJobs.RedrawWindow();

		int nNetworkIdx = JobInfo.GetNetworkIdx();
		int nLocIdx = JobInfo.GetLocIdx();
		int nConnectionType = dbNetwork.GetConnectionType( nNetworkIdx );

		if ( ( nNetworkIdx != nCurrentNetworkIdx ) || ( nConnectionType != nCurrentConnectionType ) || ( TRUE == bWantDisconnect ) )
		{
			//CLOSE ANY PREVIOUS CONNECTION
			if ( nCurrentNetworkIdx != -1 )
			{
				switch ( nCurrentConnectionType )
				{
				case CONNECTION_TYPE_STANDARD_REALTIME:
				case CONNECTION_TYPE_STANDARD_BATCH:
					m_Connect.Disconnect( TRUE );
					break;
				}
				
				if ( bGotFileLock == TRUE )
				{
					fileLock.Close();
					bGotFileLock = FALSE;
				}
			}

			bWantDisconnect = FALSE;

			CString strNetworkName = dbNetwork.GetName( nNetworkIdx );
			dbNetwork.GetSafeFileNetworkPath( nNetworkIdx, strNetworkPath );
			
			CString strConnectPhone = "";
			CString strConnectEntryName = "";
			CString strConnectServer = "";
			CString strConnectUserName = "";
			CString strConnectPassword = "";
			int nConnectFTPPortNo = 21;

			bool bConnectOK = FALSE;
			bool bAttemptConnect = TRUE;	

			//RX SALES IS THE ONLY JOB THAT NEEDS A NETWORK LOCK
			//IT WILL ALWAYS BE THE FIRST JOB FOR A BATCH COMMS NETWORK
			if ( JobInfo.GetJobType() == RX_SALES )
			{
				CFilenameUpdater FnUp( SysFiles::NetworkCommsLock, nNetworkIdx );
				
				if ( fileLock.Open( FnUp.GetFilenameToUse(), "wb" ) == FALSE )
				{
					LogCommsError( JobInfo, COMMS_ERROR_NETWORKLOCK );
					bAttemptConnect = FALSE;
				}
				else
					bGotFileLock = TRUE;
			}

			if ( bAttemptConnect == TRUE )
			{
				switch( nConnectionType )
				{
				case CONNECTION_TYPE_SPTBOOK_WEB:
				case CONNECTION_TYPE_SMARTENT_WEB:
					bConnectOK = TRUE;
					nCurrentNetworkIdx = nNetworkIdx;
					nCurrentConnectionType = nConnectionType;
					break;

				case CONNECTION_TYPE_STANDARD_REALTIME:
				case CONNECTION_TYPE_STANDARD_BATCH:
					if ( m_Connect.Connect( nNetworkIdx, strNetworkName, strNetworkPath, &m_staticInfoJobCount ) == TRUE )
					{
						bConnectOK = TRUE;
						nCurrentNetworkIdx = nNetworkIdx;
						nCurrentConnectionType = nConnectionType;
					}
					break;
				}
			}

			//FAIL CONNECT MEANS FAIL ALL JOBS AT THIS LOCATION
			if ( bConnectOK == FALSE )
			{
				switch ( nConnectionType )
				{
				case CONNECTION_TYPE_STANDARD_REALTIME:
				case CONNECTION_TYPE_STANDARD_BATCH:
					m_Connect.Disconnect();
					break;
				}
				
				if ( bGotFileLock == TRUE )
				{
					fileLock.Close();
					bGotFileLock = FALSE;
				}

				nCurrentNetworkIdx = -1;
				nCurrentConnectionType = -1;
				for ( ; nIndex <= nEndIdx; nIndex++ )
				{
					m_JobArray.GetAt ( nIndex, JobInfo );
					if ( JobInfo.GetNetworkIdx() != nNetworkIdx )
						break;
					
					if ( JobInfo.CanRetryJob() == TRUE )
					{
						JobInfo.SetStatus ( JOBLIST_STATUS_FAILED );
						JobInfo.IncAttempts();

						m_JobArray.SetAt ( nIndex, JobInfo );
					}

					UpdateLine ( nIndex );
				}
				continue;
			}
		}

		int nResult = JOBLIST_STATUS_FAILED;
		C5000Client Client ( nNetworkIdx, strNetworkPath, &m_staticInfoJobCount );

		CKeyboardJobInfo infoKb;
		int nKbNo = infoKb.GetKbNoFromJobNo( JobInfo.GetJobType() );

		bool bRealTime = FALSE;
		switch( nCurrentConnectionType )
		{
		case CONNECTION_TYPE_STANDARD_REALTIME:
			bRealTime = TRUE;
			break;
		}

		if ( nKbNo != 0 )
			nResult = ExecuteDownloadKeyboard ( Client, JobInfo, nKbNo );
		else
		{
			switch ( JobInfo.GetJobType() )
			{
			case RX_SALES:		
				switch( nCurrentConnectionType )
				{
				case CONNECTION_TYPE_SPTBOOK_WEB:
					nResult = ExecuteReceiveSalesSptBook( JobInfo );
					break;

				case CONNECTION_TYPE_SMARTENT_WEB:
					nResult = ExecuteReceiveSalesSmartEnt( JobInfo );
					break;

				case CONNECTION_TYPE_STANDARD_BATCH:
					nResult = ExecuteReceiveSalesBatch( Client, JobInfo );		
					break;

				default:
					nResult = JOBLIST_STATUS_FAILED;
					break;
				}
				break;

			case RX_REASON:		
				nResult = ExecuteReceiveReasons ( Client, JobInfo );		
				break;

			case RX_EODPLU:		
				nResult = ExecuteReceiveEODPlu ( Client, JobInfo );		
				break;

			case RX_EODCUSTOMER:		
				nResult = ExecuteReceiveEODCustomer ( Client, JobInfo );		
				break;
		
			case DN_PLU:
			case DN_PLU_SET1:
			case DN_PLU_SET2:
			case DN_PLU_SET3:		nResult = ExecuteDownloadPlu ( Client, JobInfo, bRealTime );	break;
			case DN_IMAGES:			nResult = ExecuteDownloadButtonImages ( Client, JobInfo );		break;
			case DN_PLU_INFO:		nResult = ExecuteDownloadPluInfo ( Client, JobInfo );			break;
			case DN_DEPT:			nResult = ExecuteDownloadDept( Client, JobInfo );				break;
			case DN_GROUP:			nResult = ExecuteDownloadGroup( Client, JobInfo );				break;
			case DN_ALLERGEN:		nResult = ExecuteDownloadAllergen( Client, JobInfo );			break;
			case DN_LOYALTY_SCHEME:	nResult = ExecuteDownloadLoyaltyScheme( Client, JobInfo );		break;
			case DN_TABLE_AREA:		nResult = ExecuteDownloadTableArea( Client, JobInfo );			break;
			case DN_TABLE_NAME:		nResult = ExecuteDownloadTableName( Client, JobInfo );			break;
			case DN_MIX:			nResult = ExecuteDownloadMixMatch( Client, JobInfo );			break;
			case DN_PROMO:			nResult = ExecuteDownloadPromotion( Client, JobInfo );			break;
			case DN_WALLETSTAMPRULE:nResult = ExecuteDownloadWalletStampRule(Client, JobInfo);			break;
			case DN_ALLOFFER:		nResult = ExecuteDownloadAllOffer(Client, JobInfo);				break;
			case DN_TPLAN:			nResult = ExecuteDownloadTimePlan( Client, JobInfo );			break;
			case DN_BAR:			nResult = ExecuteDownloadBarcode( Client, JobInfo );			break;
			case DN_ACAT:			nResult = ExecuteDownloadCategory( Client, JobInfo );			break;
			case DN_RECEIPT:		nResult = ExecuteDownloadReceipt( Client, JobInfo );			break;
			case DN_SERVER_PRESET:	nResult = ExecuteDownloadServerPreset( Client, JobInfo );		break;
			case DN_FUNCTION:		nResult = ExecuteDownloadFunction( Client, JobInfo );			break;
			case DN_MACRO:			nResult = ExecuteDownloadMacro( Client, JobInfo );				break;
			case DN_REASON:			nResult = ExecuteDownloadReason(Client, JobInfo);				break;
			case DN_PAIDIO:			nResult = ExecuteDownloadPaidInOutText( Client, JobInfo );		break;
			case DN_MOD:			nResult = ExecuteDownloadModifiers( Client, JobInfo );			break;
			case DN_PAY:			nResult = ExecuteDownloadPayment( Client, JobInfo );			break;
			case DN_TAX:			nResult = ExecuteDownloadTax( Client, JobInfo );				break;
			case DN_SERVER_NAME:	nResult = ExecuteDownloadServerName( Client, JobInfo );			break;
			}
		}

		ClearJobDetailInfo();

		JobInfo.SetStatus ( nResult );
		if ( nResult == JOBLIST_STATUS_FAILED )
			JobInfo.IncAttempts();

		m_JobArray.SetAt ( nIndex, JobInfo );
		UpdateLine ( nIndex );

		if ( m_nStatus == STOPPED )
			break;
	}

	if ( nCurrentNetworkIdx != -1 )
	{
		switch ( nCurrentConnectionType )
		{
		case CONNECTION_TYPE_STANDARD_REALTIME:
		case CONNECTION_TYPE_STANDARD_BATCH:
			m_Connect.Disconnect( TRUE );
			break;
		}
		
		if ( bGotFileLock == TRUE )
			fileLock.Close();
	}

	if ( m_nStatus == STOPPED )
		LogUserStop();
	
	ShowStatusHeaderInfo( "" );
	OnSelectJob();
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadPlu ( C5000Client& Client, CJobListCSVRecord& JobInfo, bool bRealTime )
{
	int nLocIdx = JobInfo.GetLocIdx();
	int nParam1 = JobInfo.GetParam1();
	bool bIsImmediate = JobInfo.GetIsImmediateFlag();
	
	CCommsExportPlu Export ( m_JobArray, JobInfo, m_bFromMemory );

	if( DealerFlags.GetSpecialChangesOnlyFlag() == TRUE )
		SpecialChangesHelper.InitialiseDownloadJob( nLocIdx, JobInfo.GetTerminalNo(), nParam1 );

	int nJobResult = JOBLIST_STATUS_FAILED;
	int nExportResult = Export.PrepareFile( nParam1 );
	LogDownloadExclusions( Export, JobInfo );
	
	if ( nExportResult == COMMS_ERROR_NODATA )
	{
		LogCommsError ( JobInfo, nExportResult );
		nJobResult = JOBLIST_STATUS_NODATA;
	}
	else if ( nExportResult != COMMS_ERROR_NONE )
	{
		LogCommsError ( JobInfo, nExportResult );
		nJobResult = JOBLIST_STATUS_FAILED;
	}
	else
	{
		C5000ClientPutFileInfo infoPutFile( Export, m_JobArray, JobInfo );

		if ( Client.PutFile( infoPutFile ) )
			nJobResult = JOBLIST_STATUS_FINISHED;
		
		if ( JOBLIST_STATUS_FINISHED == nJobResult )
		{
			if ( TRUE == bRealTime )
			{
				if ( dbLocation.GetExportControlFlag( nLocIdx ) == TRUE )
				{
					CSSFile fileToken;
					CFilenameUpdater FnUp( SysFiles::HandheldToken1, nLocIdx );
					if ( fileToken.Open( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
						fileToken.WriteLine( "Microtrend" );
				}
			}

			if ( TRUE == bIsImmediate )
				dbLocation.SetPluPresetDate( nLocIdx, JobInfo.GetTerminalNo() );

			if( DealerFlags.GetSpecialChangesOnlyFlag() == TRUE )
				SpecialChangesHelper.FinaliseDownloadJob();

			nJobResult = JOBLIST_STATUS_FINISHED;
		}
	}
	
	if ( TRUE == bIsImmediate )
		Export.RenameTempFile();
	
	return nJobResult;
}	

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadPluInfo( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportPluInfo Export ( JobInfo, m_bFromMemory );

	int nJobResult = JOBLIST_STATUS_NOTFINISHED;
	int nExportResult = Export.PrepareFile( JobInfo.GetParam1() );
	
	if ( nExportResult == COMMS_ERROR_NODATA )
	{
		LogCommsError ( JobInfo, nExportResult );
		nJobResult = JOBLIST_STATUS_NODATA;
	}
	else if ( nExportResult != COMMS_ERROR_NONE )
	{
		LogCommsError ( JobInfo, nExportResult );
		nJobResult = JOBLIST_STATUS_FAILED;
	}
	else
	{
		m_bStopFlag = FALSE;
		CString strSourceFolder = Export.GetSourcePath();
		CString strDestFolder = Export.GetDestPath();
		
		CPluInfoResourceWrapper ResourceWrapper;

		for ( int nResourceIdx = 0; ( nResourceIdx < Export.GetResourceInfoCount() ) && ( FALSE == m_bStopFlag ); nResourceIdx++ )
		{
			CPluInfoDownloadHelper item;
			Export.GetResourceInfo( nResourceIdx, item );

			CString strResourceFolder = "";
			CStringArray arrayDestSubFolders;
			ResourceWrapper.GetDownloadFolderList( item, strResourceFolder, arrayDestSubFolders );

			CCSV csvFilenames( item.m_strFilenames );

			for ( int nFolderIdx = 0; ( nFolderIdx < arrayDestSubFolders.GetSize() ) && ( FALSE == m_bStopFlag ); nFolderIdx++ )
			{
				CString strDestSubFolder = arrayDestSubFolders.GetAt( nFolderIdx );

				for ( int nFileIdx = 0; ( nFileIdx < csvFilenames.GetSize() ) && ( FALSE == m_bStopFlag ); nFileIdx++ )
				{
					C5000ClientPutFileInfo infoPutFile;
					infoPutFile.m_strSourceFolder = strResourceFolder;
					infoPutFile.m_strSourceFilename = csvFilenames.GetString(nFileIdx);
					infoPutFile.m_strDestFolder = strDestFolder;
					infoPutFile.m_strDestFilename = "";
					infoPutFile.m_strDestSubFolder = strDestSubFolder;
					infoPutFile.m_bCreateSubFolders = ( 0 == nFileIdx );

					ShowJobDetailInfo( infoPutFile );

					if ( Client.PutFile ( infoPutFile ) == FALSE )
					{
						m_bStopFlag = TRUE;
						nJobResult = JOBLIST_STATUS_FAILED;
					}
					
					ClearJobDetailInfo();
					WaitExit( m_bStopFlag, 10, 2 );
				}
			}
		}

		for ( int nFileIdx = 0; ( nFileIdx < Export.GetFileInfoCount() ) && ( FALSE == m_bStopFlag ); nFileIdx++ )
		{
			CPluInfoDownloadHelper item;
			Export.GetFileInfo( nFileIdx, item );

			CString strSourceFile = "";
			CStringArray arrayDestFiles;
			ResourceWrapper.GetDownloadFileList( item, strSourceFile, arrayDestFiles );

			for ( int nDestIdx = 0; ( nDestIdx < arrayDestFiles.GetSize() ) && ( FALSE == m_bStopFlag ); nDestIdx++ )
			{
				C5000ClientPutFileInfo infoPutFile;
				infoPutFile.m_strSourceFolder = strSourceFolder;
				infoPutFile.m_strSourceFilename = strSourceFile;
				infoPutFile.m_strDestFolder = strDestFolder;
				infoPutFile.m_strDestFilename = arrayDestFiles.GetAt(nDestIdx);

				ShowJobDetailInfo( infoPutFile );

				if ( Client.PutFile ( infoPutFile ) == FALSE )
				{
					m_bStopFlag = TRUE;
					nJobResult = JOBLIST_STATUS_FAILED;
				}
				
				ClearJobDetailInfo();
				WaitExit( m_bStopFlag, 10, 2 );
			}
		}

		if ( FALSE == m_bStopFlag )
		{
			dbLocation.SetPluInfoDate( JobInfo.GetLocIdx(), JobInfo.GetTerminalNo() );
			nJobResult = JOBLIST_STATUS_FINISHED;
		}
	}

	return nJobResult;
}	

/**********************************************************************/

void CCommsBatchDlg::ShowJobDetailInfo( C5000ClientPutFileInfo& infoPutFile )
{
	CString strSource, strDest;
	infoPutFile.GetDownloadTexts( strSource, strDest );
	m_staticInfo2.SetWindowText( "Source" );
	m_staticInfo3.SetWindowText( strSource );
	m_staticInfo4.SetWindowText( "Target" );
	m_staticInfo5.SetWindowText( strDest );
}

/**********************************************************************/

void CCommsBatchDlg::ShowJobDetailInfo( const char* szInfo )
{
	m_staticInfo2.SetWindowText( "" );
	m_staticInfo3.SetWindowText( szInfo );
	m_staticInfo4.SetWindowText( "" );
	m_staticInfo5.SetWindowText( "" );
}

/**********************************************************************/

void CCommsBatchDlg::ClearJobDetailInfo()
{
	ShowJobDetailInfo( "" );
}

/**********************************************************************/

void CCommsBatchDlg::ShowStatusHeaderInfo( const char* szInfo )
{
	CString strInfo = szInfo;
	if ( strInfo == "" )
		m_staticInfoJobCount.SetWindowText( m_strJobCount );
	else
		m_staticInfoJobCount.SetWindowText( strInfo );
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadGeneral( CCommsExportBase& Export, C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	int nResult = JOBLIST_STATUS_FAILED;

	int nExportResult = Export.PrepareFile();
	LogDownloadExclusions( Export, JobInfo );
	if ( nExportResult != COMMS_ERROR_NONE )
	{
		LogCommsError ( JobInfo, nExportResult );
		nResult = JOBLIST_STATUS_FAILED;
	}
	else 
	{
		C5000ClientPutFileInfo infoPutFile( Export, m_JobArray, JobInfo );

		ShowJobDetailInfo( infoPutFile );

		if ( Client.PutFile ( infoPutFile ) )
			nResult = JOBLIST_STATUS_FINISHED;
		else
			nResult = JOBLIST_STATUS_FAILED;
		
		ClearJobDetailInfo();
	}

	return nResult;
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadDept ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportDept Export ( JobInfo, m_bFromMemory );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}	

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadGroup ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportGroup Export ( JobInfo, m_bFromMemory );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}	

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadAllergen ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportAllergen Export ( JobInfo, m_bFromMemory );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}	

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadLoyaltyScheme ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportLoyaltyScheme Export ( JobInfo, m_bFromMemory );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}	

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadTableArea ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportTableArea Export ( JobInfo, m_bFromMemory );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}	

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadTableName ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportTableName Export ( JobInfo, m_bFromMemory );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}	

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadMixMatch ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportPromoEtc Export ( JobInfo, m_bFromMemory, DNTYPE_MIXMATCH );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}	

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadPromotion ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportPromoEtc Export ( JobInfo, m_bFromMemory, DNTYPE_PROMO );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}	

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadWalletStampRule(C5000Client& Client, CJobListCSVRecord& JobInfo)
{
	CCommsExportPromoEtc Export(JobInfo, m_bFromMemory, DNTYPE_WALLETSTAMPRULE);
	return ExecuteDownloadGeneral(Export, Client, JobInfo);
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadAllOffer(C5000Client& Client, CJobListCSVRecord& JobInfo)
{
	CCommsExportPromoEtc Export(JobInfo, m_bFromMemory, DNTYPE_ALLOFFER);
	return ExecuteDownloadGeneral(Export, Client, JobInfo);
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadTimePlan ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportTimePlan Export ( JobInfo, m_bFromMemory );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadBarcode ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportBarcode Export ( JobInfo, m_bFromMemory );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadCategory ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportCategory Export ( JobInfo, m_bFromMemory );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}	

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadReceipt ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportBillText Export ( JobInfo, m_bFromMemory );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}	

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadServerPreset ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportServerPreset Export ( JobInfo, m_bFromMemory );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}	

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadFunction ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportFunction Export ( JobInfo, m_bFromMemory );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}	

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadMacro ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportMacro Export ( JobInfo, m_bFromMemory );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}	

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadReason(C5000Client& Client, CJobListCSVRecord& JobInfo)
{
	CCommsExportReason Export(JobInfo, m_bFromMemory);
	return ExecuteDownloadGeneral(Export, Client, JobInfo);
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadPaidInOutText ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportPaidInOut Export ( JobInfo, m_bFromMemory );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadModifiers ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportModifiers Export ( JobInfo, m_bFromMemory );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}	

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadPayment ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportPayment Export ( JobInfo, m_bFromMemory );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}	

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadServerName( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportServerName Export ( JobInfo, m_bFromMemory );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}	

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadTax ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	CCommsExportTax Export ( JobInfo, m_bFromMemory );
	return ExecuteDownloadGeneral( Export, Client, JobInfo );
}	

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadKeyboard ( C5000Client& Client, CJobListCSVRecord& JobInfo, int nKbNo )
{
	int nNetworkIdx = JobInfo.GetNetworkIdx();
	int nSPOSVersion = dbNetwork.GetSPOSVersion( nNetworkIdx );

	if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, SPOS_V4 ) == TRUE )
		return ExecuteDownloadKeyboardInternal( Client, JobInfo, nKbNo );
	else
		return JOBLIST_STATUS_FAILED;
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadKeyboardInternal ( C5000Client& Client, CJobListCSVRecord& JobInfo, int nKbNo )
{
	m_KeyboardMap.BuildMap();

	CCommsExportKeyboard Export ( JobInfo, nKbNo, m_KeyboardMap );
	int nResult = Export.PreparePaths();
	
	if ( nResult != COMMS_ERROR_NONE )
	{	
		LogCommsError ( JobInfo, nResult );
		return JOBLIST_STATUS_FAILED;
	}

	CString strSourcePath = Export.GetSourcePath();
	strSourcePath += "\\";

	CCSV csvSQL;
	
	int nNetWkIdx = JobInfo.GetNetworkIdx();
	int nSPOSVersion = dbNetwork.GetSPOSVersion( nNetWkIdx );

	csvSQL.Add( "kbdv4.sql" );
	csvSQL.Add( "kbdv3_4.sql" );		
	csvSQL.Add( "kbdv3.sql" );

	CString strKbdFilename = "";
	for ( int n = 0; n < csvSQL.GetSize(); n++ )
	{
		CString strTemp = csvSQL.GetString(n);
		if ( ::FileExists( strSourcePath + strTemp ) == TRUE )
		{
			strKbdFilename = strTemp;
			break;
		}
	}

	if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, SPOS_V4 ) == TRUE )
	{
		CString strExe = "KbdPos04.pgm";

		CString strTempFilename = "TEMP_";
		strTempFilename += strKbdFilename;
		
		SPOSKeyboardManager.PrepareDownload( strSourcePath, strKbdFilename, strTempFilename, nSPOSVersion, ( JobInfo.GetParam1() == 1 ) );
		strKbdFilename = strTempFilename;
	}

	if ( "" == strKbdFilename )
	{
		LogCommsError( JobInfo, COMMS_ERROR_NODATA );
		return JOBLIST_STATUS_FAILED;
	}

	CString strDestPath = Export.GetDestPath();
	CString strDate = m_JobArray.GetDateFolder( JobInfo );

	C5000ClientPutFileInfo infoPutFile;
	infoPutFile.m_strSourceFolder = strSourcePath;
	infoPutFile.m_strSourceFilename = strKbdFilename;
	infoPutFile.m_strDestFolder = strDestPath;
	infoPutFile.m_strDestSubFolder = strDate;
	infoPutFile.m_bCreateSubFolders = TRUE;

	ShowJobDetailInfo( infoPutFile );
	
	if ( Client.PutFile ( infoPutFile ) == FALSE )
		return JOBLIST_STATUS_FAILED;
	else
		return JOBLIST_STATUS_FINISHED;
	
	ClearJobDetailInfo();
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteDownloadButtonImages ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	m_bStopFlag = FALSE;

	CCommsExportButtonImages Export ( JobInfo );

	int nExportResult = Export.PrepareFile();
	
	if ( nExportResult != COMMS_ERROR_NONE )
	{
		LogCommsError ( JobInfo, nExportResult );
		return JOBLIST_STATUS_FAILED;
	}
	else
	{
		C5000ClientPutFileInfo infoPutFile;
		infoPutFile.m_strSourceFolder = Export.GetSourcePath();
		infoPutFile.m_strDestFolder = Export.GetDestPath();
		infoPutFile.m_strDestSubFolder = m_JobArray.GetDateFolder( JobInfo );
		
		for ( int n = 0; n < Export.GetFilenameCount(); n++ )
		{
			infoPutFile.m_strSourceFilename = Export.GetFilename(n);
			infoPutFile.m_bCreateSubFolders = ( 0 == n );

			ShowJobDetailInfo( infoPutFile );

			if ( Client.PutFile ( infoPutFile ) == FALSE )
				return JOBLIST_STATUS_FAILED;
		
			ClearJobDetailInfo();
			WaitExit( m_bStopFlag, 10, 2 );
		}

		return ( FALSE == m_bStopFlag ) ? JOBLIST_STATUS_FINISHED : JOBLIST_STATUS_NOTFINISHED;
	}
}	

/**********************************************************************/

int CCommsBatchDlg::ExecuteReceiveSalesBatch ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{	
	CString strDestPath = dbNetwork.GetFolderPathSyssetCashRSP ( JobInfo.GetNetworkIdx() );
	strDestPath += "\\";

	if ( Client.Receive ( "remote\\", strDestPath ) )
	{
		m_bGotSales = TRUE;

		if ( DealerFlags.GetBatchExportFlag() == TRUE )
			BatchSalesExporter.ExportBatchSalesExternal( JobInfo.GetNetworkIdx() );

		return JOBLIST_STATUS_FINISHED;
	}
	else
		return JOBLIST_STATUS_FAILED;
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteReceiveSalesSptBook( CJobListCSVRecord& JobInfo )
{
	int nNetworkIdx = JobInfo.GetNetworkIdx();

	int nDbIdx;
	if ( dbNetwork.CanDoWebAPIComms( nNetworkIdx, nDbIdx ) == FALSE )
	{
		CString strError = "No database selected for Sports Booker ";
		LogCommsError( JobInfo, strError );
		return JOBLIST_STATUS_FAILED;
	}

	{
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
		if ( DataManager.Department.GetSportsBookerDeptNo() == 0 )
		{
			CString strError = "No default department selected for Sports Booker";
			LogCommsError( JobInfo, strError  );
			return JOBLIST_STATUS_FAILED;
		}

		if ( DataManager.EposGroup.GetSportsBookerGroupNo() == 0 )
		{
			CString strError = "No default group selected for Sports Booker";
			LogCommsError( JobInfo, strError  );
			return JOBLIST_STATUS_FAILED;
		}
	}
	
	CString strIPAddress = "";
	strIPAddress += dbNetwork.GetSportsBookerSite( nNetworkIdx );
	strIPAddress += ".sports-booker.com";

	CSportsBookerWebAPISalesDateTime datetime;
	
	switch( datetime.Read( nNetworkIdx ) )
	{
	case SB_READDATES_OK:
		break;

	case SB_READDATES_NOFILE:
		{
			CString strError = "Missing Sports Booker Date File";
			LogCommsError( JobInfo, strError );
		}
		return JOBLIST_STATUS_FAILED;

	case SB_READDATES_ERROR:
		{
			CString strError = "Invalid Sports Booker Date File";
			LogCommsError( JobInfo, strError );
		}
		return JOBLIST_STATUS_FAILED;
	}

	switch( datetime.GetDateTimeStatus() )
	{
	case SB_WEBDATE_FIRST:
	case SB_WEBDATE_NEXT:
		break;

	case SB_WEBDATE_NONE:
	default:
		{
			CString strError = "Web Sales Start Date Not Set";
			LogCommsError( JobInfo, strError );
		}
		return JOBLIST_STATUS_FAILED;
	}

	CString strObjectName = "";
	CString strJobDateTime = datetime.GetWebAPIDateTimeString(); 
	strObjectName += "/webservice/index.php?webService=getPosTransactions&username=";
	strObjectName += dbNetwork.GetSportsBookerUserName( nNetworkIdx );
	strObjectName += "&password=";
	strObjectName += dbNetwork.GetSportsBookerPassword( nNetworkIdx );
	strObjectName += "&dateTime=";
	strObjectName += strJobDateTime;
	strObjectName += "&submit=submit";

	bool bUseHttps = TRUE;
	CStringArray arHeaders;
	CString strBody = "";

	CFilenameUpdater FnUp( SysFiles::SportsBookerSalesXML, nNetworkIdx );
	CString strLoginReply = FnUp.GetFilenameToUse();

	ShowJobDetailInfo( "Retreiving XML Transactions" );

	int nTimeOut = dbNetwork.GetSportsBookerTimeOut( nNetworkIdx );

	if ( DealerFlags.GetLogWebAPICallsFlag() == TRUE )
		MessageLogger.LogWebAPICall( strIPAddress + strObjectName );

	CHttpHelpers HttpHelpers;
	DWORD dwTickCountBefore = GetTickCount();
	CString strReply = HttpHelpers.HttpGet ( bUseHttps, strIPAddress, strObjectName, arHeaders, strBody, strLoginReply, nTimeOut );
	DWORD dwTickCountAfter = GetTickCount();

	if ( strReply != "" )
	{
		CString strHeader = "";
		strHeader.Format( "HTTPGet error (%s) (%d)",
			(const char*) strReply,
			nTimeOut );
		
		LogSptBookCommsMessage( JobInfo, strHeader, "", strJobDateTime, "" );
		return JOBLIST_STATUS_FAILED;
	}

	ShowJobDetailInfo( "Parsing XML Transactions" );
	CSSXMLParser Parser;
	strReply = Parser.ParseFile ( strLoginReply, TRUE );

	if ( strReply != "" )
	{
		CString strHeader = "";
		strHeader += "XML Parsing error (";
		strHeader += strReply;
		strHeader += ")";
		LogSptBookCommsMessage( JobInfo, strHeader, "", strJobDateTime, "" );
		return JOBLIST_STATUS_FAILED;
	}
	
	CXMLNodeArray arError;

	int nErrorCount = Parser.GetNode ( NULL, arError, "xml.Errors.Error" );

	if ( nErrorCount > 0 )
	{
		bool bNoTransactions = FALSE;
		CString strHeader = "Sports Booker API Error";
		for( int nErrorIdx = 0; nErrorIdx < nErrorCount; nErrorIdx++ )
		{
			CXMLNodeArray* pError = arError.GetAt( nErrorIdx );
			CString strError = Parser.GetValue ( pError, "Description" );

			if ( FALSE == bNoTransactions )
			{
				CString strTest = strError;
				strTest.MakeUpper();
				
				if ( strTest.Left(20) == "NO TRANSACTIONS HAVE" )
					bNoTransactions = TRUE;
			}

			LogSptBookCommsMessage( JobInfo, strHeader, strError, strJobDateTime, "" );
		}
		return ( bNoTransactions ) ? JOBLIST_STATUS_FINISHED : JOBLIST_STATUS_FAILED;
	}

	CSportsBookerXMLWrangler XMLWrangler( nNetworkIdx, Parser );

	if ( XMLWrangler.OpenCashRSPFile() == FALSE )
	{
		LogSptBookCommsMessage( JobInfo, "Unable to create CashRSP File", "", strJobDateTime, ""  );
		return JOBLIST_STATUS_FAILED;
	}

	for ( int nTranIdx = 0; nTranIdx < XMLWrangler.GetTransactionCount(); nTranIdx++ )
	{
		CString strInfo;
		strInfo.Format( "Converting XML transaction (%d of %d)",
			nTranIdx + 1,
			XMLWrangler.GetTransactionCount() );
		ShowJobDetailInfo( strInfo );

		CSportsBookerTransaction Transaction;
		XMLWrangler.GetTransaction( nTranIdx, Transaction );
		XMLWrangler.WriteCashRSPFile( Transaction );
		datetime.ProcessXMLDateTime( Transaction.GetDate() );
	}

	XMLWrangler.CloseCashRSPFile();
	
	if ( XMLWrangler.RenameCashRSPFile() == FALSE )
	{	
		LogSptBookCommsMessage( JobInfo, "Unable to rename CashRSP File", "", strJobDateTime, "" );
		return JOBLIST_STATUS_FAILED;
	}
			
	datetime.Write();	

	CString strHeader;
	strHeader.Format( "Ok (%d) (%d)", XMLWrangler.GetTransactionCount(), ( dwTickCountAfter - dwTickCountBefore ) / 1000 );
	LogSptBookCommsMessage( JobInfo, strHeader, "", strJobDateTime, datetime.GetWebAPIDateTimeString() );
	
	return JOBLIST_STATUS_FINISHED;
}
	
/**********************************************************************/

int CCommsBatchDlg::ExecuteReceiveSalesSmartEnt( CJobListCSVRecord& JobInfo )
{
	int nNetworkIdx = JobInfo.GetNetworkIdx();

	//SPORTS BOOKER FUNCTION ALSO WORKS FOR SMART ENTERTAINMENT
	int nDbIdx;
	if ( dbNetwork.CanDoWebAPIComms( nNetworkIdx, nDbIdx ) == FALSE )
	{
		CString strError = "No database selected for Smart Entertainment ";
		LogCommsError( JobInfo, strError );
		return JOBLIST_STATUS_FAILED;
	}

	{
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
	}
	
	CString strIPAddress = "admin.bookmyparty.co.uk";
	
	CSmartEntertainmentWebAPISalesDate SmartEntDate;
	
	switch( SmartEntDate.Read( nNetworkIdx ) )
	{
	case SE_READDATES_OK:
		break;

	case SE_READDATES_NOFILE:
		{
			CString strError = "Missing Smart Entertainment Date File";
			LogCommsError( JobInfo, strError );
		}
		return JOBLIST_STATUS_FAILED;

	case SE_READDATES_ERROR:
		{
			CString strError = "Invalid Smart Entertainment Date File";
			LogCommsError( JobInfo, strError );
		}
		return JOBLIST_STATUS_FAILED;
	}

	if ( SmartEntDate.GetDateStatus() != SE_WEBDATE_NEXT )
	{
		CString strError = "Web Sales Start Date Not Set";
		LogCommsError( JobInfo, strError );
		return JOBLIST_STATUS_FAILED;
	}

	CString strObjectName = "";
	CString strJobDateTime = SmartEntDate.GetWebAPIDateString(); 
	strObjectName += "/getpayments.ashx?";
	strObjectName += "DateTime=";
	strObjectName += strJobDateTime;
	strObjectName += "&username=";
	strObjectName += dbNetwork.GetSportsBookerUserName( nNetworkIdx );
	strObjectName += "&password=";
	strObjectName += dbNetwork.GetSportsBookerPassword( nNetworkIdx );
	
	bool bUseHttps = FALSE;
	CStringArray arHeaders;
	CString strBody = "";

	CFilenameUpdater FnUp( SysFiles::SmartEntertainmentSalesXML, nNetworkIdx );
	CString strLoginReply = FnUp.GetFilenameToUse();

	//BUILD LIST OF PREVIOUS TRANSACTIONS
	CReportConsolidationArray<CSortedStringItem> arrayTrans;
	{
		CString strTermSalesFile;
		strTermSalesFile = dbNetwork.GetFolderPathData( nNetworkIdx );
		strTermSalesFile += "\\TRMSAVE1.001";

		if ( ::FileExists( strTermSalesFile ) == TRUE )
		{
			CSSFile fileSales;
			if ( fileSales.Open( strTermSalesFile, "rb" ) == FALSE )
			{
				CString strError = "Unable to read term sales file";
				LogCommsError( JobInfo, strError );
			}

			__int64 nSize = fileSales.GetLength();

			CString strSearchDate;
			strSearchDate = strJobDateTime.Mid(2,2) + strJobDateTime.Mid(5,2) + strJobDateTime.Right(2);
			bool bPossibleDuplicate = FALSE;

			CString strBuffer;
			while( fileSales.ReadString( strBuffer ) == TRUE )
			{
				if ( strBuffer.Left(5) == "$TRAN" )
				{
					CString strInfo;
					strInfo.Format ( "Checking previous sales file %d%%", fileSales.GetPercentPosition() );
					ShowJobDetailInfo( strInfo );

					if ( FALSE == bPossibleDuplicate )
					{
						CCSV csvTran( strBuffer );
						CString strCSVDate = csvTran.GetString(7);
						if ( strCSVDate.GetLength() == 8 )
						{
							CString strTranDate = strCSVDate.Right(2) + strCSVDate.Mid(3,2) + strCSVDate.Left(2);

							if ( strTranDate >= strSearchDate )
								bPossibleDuplicate = TRUE;
						}
					}

					if ( TRUE == bPossibleDuplicate )
					{
						CSortedStringItem item;
						item.m_strItem = strBuffer;
						arrayTrans.Consolidate( item );
					}
				}	
			}
		}
	}
	
	ShowJobDetailInfo( "Retreiving XML Transactions" );
	
	if ( DealerFlags.GetLogWebAPICallsFlag() == TRUE )
		MessageLogger.LogWebAPICall( strIPAddress + strObjectName );

	int nTimeOut = dbNetwork.GetSportsBookerTimeOut( nNetworkIdx );

	CHttpHelpers HttpHelpers;
	DWORD dwTickCountBefore = GetTickCount();
	CString strReply = HttpHelpers.HttpGet ( bUseHttps, strIPAddress, strObjectName, arHeaders, strBody, strLoginReply, nTimeOut );
	DWORD dwTickCountAfter = GetTickCount();

	if ( strReply != "" )
	{
		CString strHeader = "";
		strHeader.Format( "HTTPGet error (%s) (%d)",
			(const char*) strReply,
			nTimeOut );

		LogSptBookCommsMessage( JobInfo, strHeader, "", strJobDateTime, "" );
		return JOBLIST_STATUS_FAILED;
	}
	
	ShowJobDetailInfo( "Parsing XML Transactions" );
	CSSXMLParser Parser;
	strReply = Parser.ParseFile ( strLoginReply, TRUE );

	if ( strReply != "" )
	{
		CString strHeader = "";
		strHeader += "XML Parsing error (";
		strHeader += strReply;
		strHeader += ")";
		LogSptBookCommsMessage( JobInfo, strHeader, "", strJobDateTime, "" );
		return JOBLIST_STATUS_FAILED;
	}
	
	CXMLNodeArray arError;
	int nErrorCount = Parser.GetNode ( NULL, arError, "Errors.Error" );

	if ( nErrorCount > 0 )
	{
		bool bNoTransactions = FALSE;
		CString strHeader = "Smart Entertainment API Error";
		for( int nErrorIdx = 0; nErrorIdx < nErrorCount; nErrorIdx++ )
		{
			CXMLNodeArray* pError = arError.GetAt( nErrorIdx );
			CString strError = Parser.GetValue ( pError, "Description" );

			if ( FALSE == bNoTransactions )
			{
				CString strTest = strError;
				strTest.MakeUpper();
				
				if ( strTest.Left(20) == "NO TRANSACTIONS HAVE" )
					bNoTransactions = TRUE;
			}

			LogSmartEntCommsMessage( JobInfo, strHeader, strError, strJobDateTime, "" );
		}
		return ( bNoTransactions ) ? JOBLIST_STATUS_FINISHED : JOBLIST_STATUS_FAILED;
	}

	CSmartEntertainmentXMLWrangler XMLWrangler( nNetworkIdx, Parser );

	if ( XMLWrangler.OpenCashRSPFile() == FALSE )
	{
		LogSmartEntCommsMessage( JobInfo, "Unable to create CashRSP File", "", strJobDateTime, ""  );
		return JOBLIST_STATUS_FAILED;
	}

	int nDuplicates = 0;
	for ( int nTranIdx = 0; nTranIdx < XMLWrangler.GetTransactionCount(); nTranIdx++ )
	{
		CString strInfo;
		strInfo.Format( "Converting XML transaction (%d of %d)",
			nTranIdx + 1,
			XMLWrangler.GetTransactionCount() );
		ShowJobDetailInfo( strInfo );

		int nPaymentCount = XMLWrangler.GetPaymentCount( nTranIdx );

		for ( int nPayIdx = 0; nPayIdx < nPaymentCount; nPayIdx++ )
		{
			CSmartEntertainmentTransaction Transaction;
			XMLWrangler.GetTransaction( nTranIdx, nPayIdx, Transaction );

			bool bDuplicate = FALSE;
			XMLWrangler.WriteCashRSPFile( Transaction, arrayTrans, bDuplicate );

			if ( TRUE == bDuplicate )
				nDuplicates++;
		}
	}

	if ( nDuplicates != 0 )
	{
		CString strMsg;
		strMsg.Format( " Ignored duplicate transactions (%d)", nDuplicates );
		LogSmartEntCommsMessage( JobInfo, strMsg, "", "", "" );
	}

	XMLWrangler.CloseCashRSPFile();
	
	if ( XMLWrangler.RenameCashRSPFile() == FALSE )
	{	
		LogSmartEntCommsMessage( JobInfo, "Unable to rename CashRSP File", "", strJobDateTime, "" );
		return JOBLIST_STATUS_FAILED;
	}
			
	{
		COleDateTime date = COleDateTime::GetCurrentTime();
		SmartEntDate.SetNextReadDate( date );
		SmartEntDate.Write();
	}

	CString strHeader;
	strHeader.Format( "Ok (%d) (%d)", XMLWrangler.GetTransactionCount(), ( dwTickCountAfter - dwTickCountBefore ) / 1000 );
	LogSmartEntCommsMessage( JobInfo, strHeader, "", strJobDateTime, SmartEntDate.GetWebAPIDateString() );
	
	return JOBLIST_STATUS_FINISHED;
}
	
/**********************************************************************/

int CCommsBatchDlg::ExecuteReceiveReasons ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{	
	int nNetworkIdx = JobInfo.GetNetworkIdx();

	switch( dbNetwork.GetConnectionType( nNetworkIdx ) )
	{
	case CONNECTION_TYPE_STANDARD_REALTIME:
		return ExecuteReceiveReasonsRealTime( Client, JobInfo );

	case CONNECTION_TYPE_STANDARD_BATCH:
		return ExecuteReceiveReasonsBatch( Client, JobInfo );

	default:
		LogCommsError( JobInfo, COMMS_ERROR_NOCOMMS );
		return JOBLIST_STATUS_FAILED;
	}
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteReceiveReasonsRealTime ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	int nNetworkIdx = JobInfo.GetNetworkIdx();
	int nParam1 = JobInfo.GetParam1();

	CEODFileMover EODFileMover;
	EODFileMover.MoveReasonFilesNetwork( nNetworkIdx, nParam1 );

	CMessageLoggerInfo info;
	info.m_nNetworkIdx = JobInfo.GetNetworkIdx();
	info.m_bAddDate = TRUE;
	info.m_bAddTimeOfDay = TRUE;
	info.m_bAddNetworkName = TRUE;
	info.m_strMessage = JobInfo.GetJobName( RX_REASON, nParam1, 0 );
	info.m_strStatus.Format( "Ok (%d)", EODFileMover.GetFileCount() );
	MessageLogger.LogSchedulerMessage( info, TRUE, TRUE, TRUE );

	return JOBLIST_STATUS_FINISHED;
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteReceiveReasonsBatch( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	int nNetworkIdx = JobInfo.GetNetworkIdx();
	int nParam1 = JobInfo.GetParam1();

	if ( Client.ReceiveReasonFiles ( "remote\\", nNetworkIdx, nParam1 ) == TRUE )
		return JOBLIST_STATUS_FINISHED;
	else
		return JOBLIST_STATUS_FAILED;
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteReceiveEODPlu ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{	
	int nNetworkIdx = JobInfo.GetNetworkIdx();

	switch( dbNetwork.GetConnectionType( nNetworkIdx ) )
	{
	case CONNECTION_TYPE_STANDARD_REALTIME:
		return ExecuteReceiveEODPluRealTime( Client, JobInfo );

	case CONNECTION_TYPE_STANDARD_BATCH:
		return ExecuteReceiveEODPluBatch( Client, JobInfo );

	default:
		LogCommsError( JobInfo, COMMS_ERROR_NOCOMMS );
		return JOBLIST_STATUS_FAILED;
	}
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteReceiveEODPluRealTime ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	int nLocIdx = JobInfo.GetLocIdx();
	int nNwkIdx = JobInfo.GetNetworkIdx();
	
	CEODFileMover EODFileMover;
	EODFileMover.MoveEODPluFileLocation( nNwkIdx, nLocIdx );

	CMessageLoggerInfo info;
	info.m_nNetworkIdx = nNwkIdx;
	info.m_strMessage = JobInfo.GetLogLine();
	info.m_bAddDate = TRUE;
	info.m_bAddTimeOfDay = TRUE;
	info.m_bAddNetworkName = TRUE;
	info.m_strStatus.Format( "Ok (%d)", EODFileMover.GetFileCount() );
	MessageLogger.LogSchedulerMessage( info, TRUE, TRUE, TRUE );

	return JOBLIST_STATUS_FINISHED;
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteReceiveEODPluBatch( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	int nLocIdx = JobInfo.GetLocIdx();

	if ( Client.ReceiveEODPluFile ( "remote\\", nLocIdx ) )
	{
		m_bGotSales = TRUE;
		return JOBLIST_STATUS_FINISHED;
	}
	else
		return JOBLIST_STATUS_FAILED;
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteReceiveEODCustomer ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{	
	int nNetworkIdx = JobInfo.GetNetworkIdx();

	switch( dbNetwork.GetConnectionType( nNetworkIdx ) )
	{
	case CONNECTION_TYPE_STANDARD_REALTIME:
		return ExecuteReceiveEODCustomerRealTime( Client, JobInfo );

	case CONNECTION_TYPE_STANDARD_BATCH:
		return ExecuteReceiveEODCustomerBatch( Client, JobInfo );

	default:
		LogCommsError( JobInfo, COMMS_ERROR_NOCOMMS );
		return JOBLIST_STATUS_FAILED;
	}
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteReceiveEODCustomerRealTime ( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	int nLocIdx = JobInfo.GetLocIdx();
	int nNwkIdx = JobInfo.GetNetworkIdx();
	
	CEODFileMover EODFileMover;
	EODFileMover.MoveEODCustomerFileLocation( nNwkIdx, nLocIdx );

	CMessageLoggerInfo info;
	info.m_nNetworkIdx = nNwkIdx;
	info.m_strMessage = JobInfo.GetLogLine();
	info.m_bAddDate = TRUE;
	info.m_bAddTimeOfDay = TRUE;
	info.m_bAddNetworkName = TRUE;
	info.m_strStatus.Format( "Ok (%d)", EODFileMover.GetFileCount() );
	MessageLogger.LogSchedulerMessage( info, TRUE, TRUE, TRUE );

	return JOBLIST_STATUS_FINISHED;
}

/**********************************************************************/

int CCommsBatchDlg::ExecuteReceiveEODCustomerBatch( C5000Client& Client, CJobListCSVRecord& JobInfo )
{
	int nLocIdx = JobInfo.GetLocIdx();

	if ( Client.ReceiveEODCustomerFile ( "remote\\", nLocIdx ) )
	{
		m_bGotSales = TRUE;
		return JOBLIST_STATUS_FINISHED;
	}
	else
		return JOBLIST_STATUS_FAILED;
}

/**********************************************************************/

void CCommsBatchDlg::LogCommsError( CJobListCSVRecord& JobInfo, int nError )
{
	CString strError;
	switch ( nError )
	{
	case COMMS_ERROR_BADNETIDX:			strError = "Bad network index";					break;
	case COMMS_ERROR_BADLOCIDX:			strError = "Bad location index";				break;
	case COMMS_ERROR_BADDBLINK:			strError = "Bad Database link";					break;
	case COMMS_ERROR_NODATA:			strError = "No data";							break;
	case COMMS_ERROR_NOCOMMS:			strError = "Comms disabled";					break;
	case COMMS_ERROR_CREATEFILE:		strError = "Unable to create file";				break;
	case COMMS_ERROR_PRICESETRANGE:		strError = "Price Set range error";				break;
	case COMMS_ERROR_NOKEYBOARD:		strError = "Keyboard not found";				break;
	case COMMS_ERROR_NOKBDMAN:			strError = "Keyboard Manager not found";		break;
	case COMMS_ERROR_BADKBDMAN:			strError = "Incompatible Keyboard Manager";		break;
	case COMMS_ERROR_NETWORKLOCK:		strError = "Network locked by another user";	break;
	default:							strError = "Unknown error";						break;
	}

	LogCommsError( JobInfo, strError );
}

/**********************************************************************/

void CCommsBatchDlg::LogSptBookCommsMessage( CJobListCSVRecord& JobInfo, const char* szHeader, const char* szError, const char* szPreviousTime, const char* szNextTime )
{
	CString strJobType = "SYSTEM: ";
	strJobType += JobInfo.GetLogLine();

	CMessageLoggerInfo info;
	info.m_nNetworkIdx = JobInfo.GetNetworkIdx();
	info.m_bAddDate = TRUE;
	info.m_bAddTimeOfDay = TRUE;
	info.m_bAddNetworkName = TRUE;
	info.m_strMessage = strJobType;
	info.m_strStatus = szHeader;
	MessageLogger.LogSchedulerMessage( info, TRUE, TRUE, TRUE );

	{
		CString strError = szError;
		if ( strError != "" )
		{
			info.m_strMessage = "<..>==> ";
			info.m_strMessage += strError;
			info.m_bAddDate = FALSE;
			info.m_bAddTimeOfDay = FALSE;
			info.m_bAddNetworkName = FALSE;
			info.m_strStatus = "";
			MessageLogger.LogSchedulerMessage( info, TRUE, TRUE, TRUE );
		}
	}

	{
		CString strTime = szPreviousTime;

		if ( strTime != "" )
		{
			info.m_strMessage = "<..>==> Previous sale time : ";
			info.m_strMessage += strTime;
			info.m_bAddDate = FALSE;
			info.m_bAddTimeOfDay = FALSE;
			info.m_bAddNetworkName = FALSE;
			info.m_strStatus = "";
			MessageLogger.LogSchedulerMessage( info, TRUE, TRUE, TRUE );
		}
	}

	{
		CString strTime = szNextTime;

		if ( strTime != "" )
		{
			info.m_strMessage = "<..>==> Latest sale time : ";
			info.m_strMessage += strTime;
			info.m_bAddDate = FALSE;
			info.m_bAddTimeOfDay = FALSE;
			info.m_bAddNetworkName = FALSE;
			info.m_strStatus = "";
			MessageLogger.LogSchedulerMessage( info, TRUE, TRUE, TRUE );
		}
	}
}

/**********************************************************************/

void CCommsBatchDlg::LogSmartEntCommsMessage( CJobListCSVRecord& JobInfo, const char* szHeader, const char* szError, const char* szPreviousTime, const char* szNextTime )
{
	CString strJobType = "SYSTEM: ";
	strJobType += JobInfo.GetLogLine();

	CMessageLoggerInfo info;
	info.m_nNetworkIdx = JobInfo.GetNetworkIdx();
	info.m_bAddDate = TRUE;
	info.m_bAddTimeOfDay = TRUE;
	info.m_bAddNetworkName = TRUE;
	info.m_strMessage = strJobType;
	info.m_strStatus = szHeader;
	MessageLogger.LogSchedulerMessage( info, TRUE, TRUE, TRUE );

	{
		CString strError = szError;
		if ( strError != "" )
		{
			info.m_strMessage = "<..>==> ";
			info.m_strMessage += strError;
			info.m_bAddDate = FALSE;
			info.m_bAddTimeOfDay = FALSE;
			info.m_bAddNetworkName = FALSE;
			info.m_strStatus = "";
			MessageLogger.LogSchedulerMessage( info, TRUE, TRUE, TRUE );
		}
	}

	{
		CString strTime = szPreviousTime;

		if ( strTime != "" )
		{
			info.m_strMessage = "<..>==> Previous read date : ";
			info.m_strMessage += strTime;
			info.m_bAddDate = FALSE;
			info.m_bAddTimeOfDay = FALSE;
			info.m_bAddNetworkName = FALSE;
			info.m_strStatus = "";
			MessageLogger.LogSchedulerMessage( info, TRUE, TRUE, TRUE );
		}
	}

	{
		CString strTime = szNextTime;

		if ( strTime != "" )
		{
			info.m_strMessage = "<..>==> Next read date : ";
			info.m_strMessage += strTime;
			info.m_bAddDate = FALSE;
			info.m_bAddTimeOfDay = FALSE;
			info.m_bAddNetworkName = FALSE;
			info.m_strStatus = "";
			MessageLogger.LogSchedulerMessage( info, TRUE, TRUE, TRUE );
		}
	}
}

/**********************************************************************/

void CCommsBatchDlg::LogCommsError( CJobListCSVRecord& JobInfo, CString& strError )
{
	CString strJobType = "SYSTEM: ";
	strJobType += JobInfo.GetLogLine();

	CMessageLoggerInfo info;
	info.m_nNetworkIdx = JobInfo.GetNetworkIdx();
	info.m_bAddDate = TRUE;
	info.m_bAddTimeOfDay = TRUE;
	info.m_bAddNetworkName = TRUE;
	info.m_strMessage = strJobType;
	info.m_strStatus = strError;
	MessageLogger.LogSchedulerMessage( info, TRUE, TRUE, TRUE );
}

/**********************************************************************/

void CCommsBatchDlg::LogDownloadExclusions( CCommsExportBase& Export, CJobListCSVRecord& JobInfo )
{
	int nCount = Export.GetDownloadExclusionCount();

	if ( nCount != 0 )
	{
		int nRejectCount = 0;
		int nAcceptCount = 0;
		for ( int n = 0; n < nCount; n++ )
		{
			CDownloadExclusionItem item;
			Export.GetDownloadExclusion( n, item );

			switch (item.m_nSPOSVersion)
			{
			case 9999:
				nAcceptCount = item.m_nCount;
				break;

			case 9998:
			case 9997:
			{
				CString strError = "";
				strError.Format("Rejected %d %s Time Plan Entr%s because of no free slots.",
					item.m_nCount,
					(9998 == item.m_nSPOSVersion) ? "Promo" : "Mix Match",
					(item.m_nCount != 1) ? "ies" : "y");
					
				nRejectCount += item.m_nCount;
				LogDownloadExclusion(JobInfo, strError);
			}
			break;

			default:
			{
				CString strName = "";
				CNetworkCSVRecord::GetTreeNameExternal(CONNECTION_TYPE_STANDARD_NONE, item.m_nSPOSVersion, strName, FALSE);

				CString strType = "";
				switch (item.m_nDownloadType)
				{
				case NODE_OFFER_ENTRY:	strType = "offer item";		break;
				case NODE_PROMOTION:	strType = "promotion";		break;
				default:				strType = "record";			break;
				}

				CString strError = "";
				strError.Format("Rejected %d %s%s requiring %s or later.",
					item.m_nCount,
					(const char*)strType,
					(item.m_nCount != 1) ? "s" : "",
					(const char*)strName);

				nRejectCount += item.m_nCount;
				LogDownloadExclusion(JobInfo, strError);
			}
			break;
			}
		}

		CString strError;
		strError.Format( "Accepted %d record%s for download.",
			nAcceptCount,
			( nAcceptCount != 1 ) ? "s" : "" );
		LogDownloadExclusion( JobInfo, strError );

		JobInfo.SetRejectedRecordCount( nRejectCount );
	}
}

/**********************************************************************/

void CCommsBatchDlg::LogDownloadExclusion( CJobListCSVRecord& JobInfo, CString& strError )
{
	CString strJobType = "SYSTEM: ";
	strJobType += JobInfo.GetDownloadExclusionLogLine();

	CMessageLoggerInfo info;
	info.m_nNetworkIdx = JobInfo.GetNetworkIdx();
	info.m_bAddDate = TRUE;
	info.m_bAddTimeOfDay = TRUE;
	info.m_bAddNetworkName = TRUE;
	info.m_strMessage = strJobType;
	info.m_strStatus = strError;
	MessageLogger.LogSchedulerMessage( info, TRUE, TRUE, TRUE );
}

/**********************************************************************/

void CCommsBatchDlg::LogUserStop()
{
	CMessageLoggerInfo info;
	info.m_nNetworkIdx = 0;
	info.m_bAddDate = TRUE;
	info.m_bAddTimeOfDay = TRUE;
	info.m_bAddNetworkName = TRUE;
	info.m_strMessage = "SYSTEM: Batch Comms terminated by user";
	info.m_strStatus = "";
	MessageLogger.LogSchedulerMessage( info, FALSE, TRUE, TRUE );
}

/**********************************************************************/

void CCommsBatchDlg::OnButtonTimelog() 
{
	m_bCanAutoClose = FALSE;

	if ( ::FileExists ( m_strTempLogFilename ) == TRUE )
	{	
		CReportManager ReportManager;
		CFilenameUpdater FnUp ( SysFiles::SchedLogPrm );
		ReportManager.DisplayReport ( m_strTempLogFilename, FnUp.GetFilenameToUse(), "", this );
	}
	else
		Prompter.Error ( "The Comms Log file was not found." );
}

/**********************************************************************/

void CCommsBatchDlg::BuildList()
{
	m_listJobs.ResetContent();

	for ( int nIndex = 0; nIndex < m_JobArray.GetSize(); nIndex++ )
		m_listJobs.AddString ( m_JobArray.GetDisplayLine( nIndex ) );
}

/**********************************************************************/

void CCommsBatchDlg::UpdateLine( int nIndex )
{
	if ( nIndex < 0 || nIndex >= m_JobArray.GetSize() )
		return;

	int nSelection = m_listJobs.GetCurSel();
	int nTopIndex = m_listJobs.GetTopIndex();
	
	m_listJobs.DeleteString(nIndex);
	m_listJobs.InsertString ( nIndex, m_JobArray.GetDisplayLine( nIndex ) );

	m_listJobs.SetCurSel( nSelection );
	m_listJobs.SetTopIndex ( nTopIndex );
}

/**********************************************************************/

void CCommsBatchDlg::OnSelectJob() 
{
	m_buttonRetry.EnableWindow ( CheckRetryStatus() );
}

/**********************************************************************/

bool CCommsBatchDlg::CheckRetryStatus()
{
	bool bResult = FALSE;

	int nSelection = m_listJobs.GetCurSel();
	if ( ( nSelection >= 0 ) && ( nSelection < m_JobArray.GetSize() ) )
	{
		CJobListCSVRecord JobInfo;
		m_JobArray.GetAt ( nSelection, JobInfo );

		switch ( JobInfo.GetLineType() )
		{
		case JOBLIST_LINETYPE_BLANK:
			bResult = FALSE;
			break;

		case JOBLIST_LINETYPE_JOB:
			bResult = JobInfo.CanRetryJob();
			break;
			
		case JOBLIST_LINETYPE_LOCATION:
			{
				for ( int nIndex = nSelection + 1; nIndex < m_JobArray.GetSize(); nIndex++ )
				{
					m_JobArray.GetAt ( nIndex, JobInfo );
				
					if ( JobInfo.GetLineType() != JOBLIST_LINETYPE_JOB )
					{
						bResult = FALSE;
						break;
					}
					
					if ( JobInfo.CanRetryJob() == TRUE )
					{
						bResult = TRUE;
						break;
					}
				}
			}
			break;

		case JOBLIST_LINETYPE_NETWORK:
			{
				for ( int nIndex = nSelection + 1; nIndex < m_JobArray.GetSize(); nIndex++ )
				{
					m_JobArray.GetAt ( nIndex, JobInfo );

					if ( JobInfo.GetLineType() == JOBLIST_LINETYPE_NETWORK )
					{
						bResult = FALSE;
						break;
					}

					if ( JobInfo.GetLineType() == JOBLIST_LINETYPE_JOB )
					{
						if ( JobInfo.CanRetryJob() == TRUE )
						{
							bResult = TRUE;
							break;
						}
					}
				}
			}
			break;

		default:
			bResult = FALSE;
			break;
		}
	}

	return bResult;
}

/**********************************************************************/

void CCommsBatchDlg::OnButtonStop() 
{
	m_bJobReady = TRUE;
	m_bStopFlag = TRUE;
	m_nStatus = STOPPED;
}

/**********************************************************************/

void CCommsBatchDlg::OnStaticInfo() 
{
	if ( m_nStatus != STOPPED )
		return;

	m_bCanAutoClose = FALSE;

	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
	{
		Prompter.Error ( "Unable to create report" );
		return;
	}

	CString strTitle;
	strTitle.Format ( "Batch Communications Setup (%s)", (const char*) m_strTaskName );
	ReportFile.SetStyle1 ( strTitle );
	ReportFile.AddColumn ( "Location", TA_LEFT, 700 );
	ReportFile.AddColumn ( "Job Type", TA_LEFT, 700 );
	ReportFile.AddColumn ( "Status", TA_LEFT, 1000 );

	int nLastNwkIdx = -1;
	int nLastLocIdx = -1;
	
	for ( int n = 0; n < m_arrayJobSetupInfo.GetSize(); n++ )
	{
		int nJobType = m_arrayJobSetupInfo[n].m_nJobType;
		int nParam1 = m_arrayJobSetupInfo[n].m_nParam1;
		int nNwkIdx = m_arrayJobSetupInfo[n].m_nNwkIdx;
		int nLocIdx = m_arrayJobSetupInfo[n].m_nLocIdx;
		int nStatus = m_arrayJobSetupInfo[n].m_nStatus;
		bool bSales = ( nJobType == RX_SALES );

		if ( ( nNwkIdx != nLastNwkIdx ) || ( nLocIdx != nLastLocIdx ) )
		{
			if ( n != 0 )
				ReportFile.WriteLine( "<LI>" );

			nLastNwkIdx = nNwkIdx;
			nLastLocIdx = nLocIdx;
		}

		CCSV csv ( '\t' );

		CString strNetworkType;
		dbNetwork.GetSPOSVersionTreeName( nNwkIdx, strNetworkType, FALSE );

		CString strLocation;

		if ( bSales == FALSE )
		{
			strLocation.Format ( "%s, %s (%s)",
				dbNetwork.GetName( nNwkIdx ),
				dbLocation.GetName( nLocIdx ),
				(const char*) strNetworkType );
		}
		else
		{
			strLocation.Format ( "%s (%s)",
				dbNetwork.GetName( nNwkIdx ),
				(const char*) strNetworkType );
		}

		csv.Add( strLocation );

		CJobListCSVRecord JobListRecord;
		csv.Add( JobListRecord.GetJobName( nJobType, nParam1, 0 ) );

		switch( nStatus )
		{
		case JOBLIST_SETUP_ACCEPTED:	
			csv.Add ( "Accepted" );
			break;

		case JOBLIST_SETUP_NEED_COMMS:
			csv.Add ( "REJECTED: This network is configured for no comms" );
			break;

		case JOBLIST_SETUP_NEED_REALTIME:
			csv.Add ( "REJECTED: This network is configured for real time sales" );
			break;

		case JOBLIST_SETUP_NEED_BATCH:
			csv.Add ( "REJECTED: This job is not available for real time networks" );
			break;

		case JOBLIST_SETUP_NEED_V4_992:
			csv.Add ( "REJECTED: This job requires SPOS V4.1.992 or later" );
			break;

		case JOBLIST_SETUP_NEED_V4_2606:
			csv.Add ( "REJECTED: This job requires SPOS V4.1.2606 or later" );
			break;

		case JOBLIST_SETUP_NEED_V4_2_1530:
			csv.Add("REJECTED: This job requires SPOS V4.2.1530 or later");
			break;

		case JOBLIST_SETUP_NEED_V4_2_1579:
			csv.Add("REJECTED: This job requires SPOS V4.2.1579 or later");
			break;

		case JOBLIST_SETUP_NEED_DBLINK:
			csv.Add ( "REJECTED: This location is not linked to a database" );
			break;

		case JOBLIST_SETUP_NEED_FIRSTDB:
			csv.Add ( "REJECTED: This job is for the first database only" );
			break;

		case JOBLIST_SETUP_NEED_DBPRICESET:
			csv.Add ( "REJECTED: This price set is not location specific" );
			break;

		case JOBLIST_SETUP_NEED_KBDLINK:
			csv.Add ( "REJECTED: This price set is not linked to a keyboard" );
			break;

		case JOBLIST_SETUP_REJECT_SPTBOOK:
			csv.Add ( "REJECTED: This job is not available for Sports Booker networks" );
			break;

		case JOBLIST_SETUP_REJECT_SMARTENT:
			csv.Add ( "REJECTED: This job is not available for Smart Entertainment networks" );
			break;

		case JOBLIST_SETUP_NEED_SPTBOOK:
			csv.Add ( "REJECTED: The Sports Booker network must be linked to a database" );
			break;

		case JOBLIST_SETUP_NEED_SMARTENT:
			csv.Add ( "REJECTED: The Smart Entertainment network must be linked to a database" );
			break;

		case JOBLIST_SETUP_UNKNOWNJOB:
			{
				CString strMsg;
				strMsg.Format ( "REJECTED: Job Type %d is not recognised", nJobType ); 
				csv.Add ( strMsg  );
			}
			break;

		case JOBLIST_SETUP_NEED_IMMEDIATE:
			{
				CString strMsg;
				strMsg.Format ( "REJECTED: This job cannot be performed as a deferred task" ); 
				csv.Add ( strMsg  );
			}
			break;

		case JOBLIST_SETUP_LOC_DISABLE:
			{
				CString strMsg;
				strMsg.Format ( "REJECTED: This job is not enabled for this location" ); 
				csv.Add ( strMsg  );
			}
			break;

		default:
			csv.Add ( "Unknown" );
			break;
		}

		ReportFile.WriteLine( csv.GetLine() );
	}

	if ( m_arrayJobSetupInfo.GetSize() > 0 )
		ReportFile.WriteLine( "<LI>" );

	ReportFile.WriteLine( "" );
	ReportFile.Close();

	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), "", "", this );
}

/**********************************************************************/

long CCommsBatchDlg::OnCloseComms( WPARAM w, LPARAM l )
{
	OnCancel();
	return 0;
}

/**********************************************************************/

void CCommsBatchDlg::OnOK() 
{
	OnCancel();
}

/**********************************************************************/

void CCommsBatchDlg::OnCancel()
{
	if ( m_hTimer > 0 )
	{
		KillTimer ( m_hTimer );
		m_hTimer = 0;
	}

	EndDialog ( IDOK );
}

/**********************************************************************/
