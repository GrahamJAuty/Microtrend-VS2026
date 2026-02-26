/**********************************************************************/
#include "..\CommonEcrStock\CashRSPClearBufferDlg.h"
#include "CashRSPFlusher.h"
#include "CashRSPImporter.h"
#include "CashRSPImporterStatusDisplay.h"
#include "WaitLockDlg.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "LockManager.h"
#include "NetworkCSVArray.h"
#include "PosTrayOptions.h"
#include "..\SPosTray5\PosTrayStatusDlg.h"
#include "SPOSVersions.h"
#include "..\SPosStockManager5\StockOptionsIni.h"
#include "..\SPosStockManager5\StockpointCSVArray.h"
#include "SysInfo.h"
#include "..\SPosEcrManager5\ZDeliveryImporterEcrman.h"
#include "..\SPosStockManager5\ZDeliveryImporterStockman.h"
#include "..\SPosStockManager5\ZDeliveryRedirectorStockman.h"
#include "..\SPosEcrManager5\ZSalesImporterEcrman.h"
#include "..\SPosStockManager5\ZSalesImporterStockman.h"
#include "..\SPosStockManager5\ZSalesRedirectorStockman.h"
/**********************************************************************/
#include "CommsHandshaker.h"
/**********************************************************************/
C5000Server CCommsHandshaker::m_5000Server;
bool CCommsHandshaker::m_bInitialisedComms = FALSE;
bool CCommsHandshaker::m_bEnableRealTime = FALSE;
/**********************************************************************/

void CCommsHandshaker::InitialiseComms()
{
	CString strHandshake = "";
	//GetDataProgramPath( strHandshake );
	
	if ( m_5000Server.InitialiseComms( strHandshake, AfxGetMainWnd() ) == TRUE )
		m_bInitialisedComms = TRUE;
}

/**********************************************************************/

void CCommsHandshaker::BeginComms()
{	
	CStringArray arrayCommsSales;
	CStringArray arrayCommsProducts;
	CStringArray arrayCommsCustomer;

#ifdef POSTRAY_UTILITY
	m_bEnableRealTime = ( PosTrayOptions.GetPosTrayRealTimeFlag() == TRUE );
#else
	m_bEnableRealTime = ( PosTrayOptions.GetPosTrayRealTimeFlag() == FALSE );
	m_bEnableRealTime |= ( SysInfo.IsPosTraySystem() == FALSE );
#endif

	if ( TRUE == m_bEnableRealTime )
	{
		for ( int nNetworkIdx = 0; nNetworkIdx < dbNetwork.GetSize(); nNetworkIdx++ )
		{
			int nConnectionType = dbNetwork.GetConnectionType( nNetworkIdx );
			switch( nConnectionType )
			{
			case CONNECTION_TYPE_STANDARD_REALTIME:
				break;

			default:
				continue;
			}

			CString strDataPath;
			dbNetwork.GetSafeFileNetworkPath ( nNetworkIdx, strDataPath );

			CFilenameUpdater FnUp( SysFiles::NetworkCommsLock, nNetworkIdx );
			CString strLockFile = FnUp.GetFilenameToUse();

			int nLocIdxStart, nLocIdxEnd;
			dbLocation.GetNetworkLocIdxRange( nNetworkIdx, nLocIdxStart, nLocIdxEnd );

			CString strLocalPath = dbNetwork.GetFolderPathSyssetCashRSP( nNetworkIdx );
			strLocalPath += "\\";

			if ( strDataPath != "" )
			{	
				CCSV csvOut;
				csvOut.Add ( strDataPath );		//FULL PATH TO REAL TIME SALES, INCLUDING UPAUDIT		
				csvOut.Add ( strLocalPath );	//FULL LOCAL PATH TO WHICH SALES WILL BE COPIED		
				csvOut.Add ( strLockFile );

				int nSPOSVersion = dbNetwork.GetSPOSVersion( nNetworkIdx );
				if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, SPOS_V4_861 ) == TRUE )
				{
					CCSV csvLocs;
					for ( int nLocIdx = nLocIdxStart; nLocIdx <= nLocIdxEnd; nLocIdx++ )
					{
						if ( dbLocation.GetExportControlFlag( nLocIdx ) == TRUE )
						{
							csvLocs.Add( dbLocation.GetFolderPathData( nLocIdx ) );

							CString strPath;
							dbLocation.GetSafeDownloadNowPath( nLocIdx, strPath );
							csvLocs.Add( strPath );

							dbLocation.GetSafeExportControlPath( nLocIdx, strPath );
							csvLocs.Add( strPath );

							csvLocs.Add( dbLocation.GetExportControlDelay( nLocIdx ) );
						}
					}

					csvOut.Add( csvLocs.GetLine() );
				}

				arrayCommsSales.Add ( csvOut.GetLine() );
			}

			//CASHRSP FOLDERS BY TERMINAL
			for ( int nLocIdx = nLocIdxStart; nLocIdx <= nLocIdxEnd; nLocIdx++ )
			{
				CArray<CLocationMasterInfo,CLocationMasterInfo> arrayFolders;
				LocationMasterMap.GetLocationMasterList( nLocIdx, arrayFolders, FALSE, TRUE );
		
				for ( int n = 0; n < arrayFolders.GetSize(); n++ )
				{
					CLocationMasterInfo infoMaster = arrayFolders.GetAt( n );
					CString strCashRSPFolder = infoMaster.GetCashRSPFolder();
				
					if ( strCashRSPFolder != "" )
					{	
						CCSV csvOut;
						csvOut.Add ( strCashRSPFolder );					//FULL PATH TO REAL TIME SALES, INCLUDING UPAUDIT		
						csvOut.Add ( strLocalPath );						//FULL LOCAL PATH TO WHICH SALES WILL BE COPIED		
						csvOut.Add ( strLockFile );
						arrayCommsSales.Add ( csvOut.GetLine() );
					}
				}
			}
			
			if ( EcrmanOptions.GetPluProdCSVImportType() == 2 )
			{
				int nLocIdxStart, nLocIdxEnd;
				dbLocation.GetNetworkLocIdxRange( nNetworkIdx, nLocIdxStart, nLocIdxEnd );

				for ( int nLocIdx = nLocIdxStart; nLocIdx <= nLocIdxEnd; nLocIdx++ )
				{
					if ( dbLocation.GetProdCSVImportFolderType( nLocIdx ) != EODIMPORT_FOLDERTYPE_NONE )
					{
						CString strPath1, strPath2;
						dbNetwork.GetSafeFileNetworkPath ( nNetworkIdx, strPath1 );
						dbLocation.GetSafeTerminalExportPath( nLocIdx, strPath2 );

						CString strDataPath = "";
						strDataPath += strPath1;
						strDataPath += strPath2;

						if ( dbLocation.GetProdCSVImportFolderType( nLocIdx ) == EODIMPORT_FOLDERTYPE_INTRADING )
							strDataPath += "InTrading\\";

						if ( strDataPath != "" )
						{
							CString strLocalPath = dbLocation.GetFolderPathSyssetPendingProductsLocal( nLocIdx );
							strLocalPath += "\\";

							CCSV csvOut;
							csvOut.Add ( strDataPath );		//FULL PATH TO PROD.CSV FILE FROM TERMINAL		
							csvOut.Add ( strLocalPath );	//FULL PATH TO LOCATION PENDING FOLDER		
							csvOut.Add ( strLockFile );

							arrayCommsProducts.Add ( csvOut.GetLine() );
						}
					}
				}
			}

			if ( EcrmanOptions.GetFeaturesCustomerNameImportType() == 2 )
			{
				int nLocIdxStart, nLocIdxEnd;
				dbLocation.GetNetworkLocIdxRange( nNetworkIdx, nLocIdxStart, nLocIdxEnd );

				for ( int nLocIdx = nLocIdxStart; nLocIdx <= nLocIdxEnd; nLocIdx++ )
				{
					CString strPath1, strPath2;
					dbNetwork.GetSafeFileNetworkPath ( nNetworkIdx, strPath1 );
					dbLocation.GetSafeTerminalExportPath( nLocIdx, strPath2 );

					CString strDataPath = "";
					strDataPath += strPath1;
					strDataPath += strPath2;

					if ( dbLocation.GetCustomerImportFolderType( nLocIdx ) == EODIMPORT_FOLDERTYPE_INTRADING )
						strDataPath += "InTrading\\";

					if ( strDataPath != "" )
					{
						CString strLocalPath = dbLocation.GetFolderPathSyssetPendingCustomerLocal( nLocIdx );
						strLocalPath += "\\";

						CCSV csvOut;
						csvOut.Add ( strDataPath );		//FULL PATH TO PROD.CSV FILE FROM TERMINAL		
						csvOut.Add ( strLocalPath );	//FULL PATH TO LOCATION PENDING FOLDER		
						csvOut.Add ( strLockFile );

						arrayCommsCustomer.Add ( csvOut.GetLine() );
					}
				}
			}
		}
	}

	m_5000Server.BeginComms ( arrayCommsSales, arrayCommsProducts, arrayCommsCustomer, TRUE );
}

/**********************************************************************/

void CCommsHandshaker::TerminateComms()
{
	m_5000Server.TerminateComms();
}

/**********************************************************************/

void CCommsHandshaker::TerminateThread()
{
	m_5000Server.TerminateThread();
}

/**********************************************************************/

void CCommsHandshaker::EnableComms()
{
	m_5000Server.EnableComms();
}

/**********************************************************************/

void CCommsHandshaker::DisableComms()
{
	m_5000Server.DisableComms();
}

/**********************************************************************/

void CCommsHandshaker::PauseComms()
{
	m_5000Server.PauseComms();
}

/**********************************************************************/

void CCommsHandshaker::ResumeComms()
{
	m_5000Server.ResumeComms();
}

/**********************************************************************/

void CCommsHandshaker::PMSPulse()
{
	m_5000Server.PMSPulse();
}

/**********************************************************************/

bool CCommsHandshaker::GetConfirmStoppedFlag()
{ 
	return m_5000Server.GetConfirmStoppedFlag(); 
}

/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

long CCommsHandshaker::FetchAllRealTimeSales( int nNetworkIdx, int nType )
{
	return m_5000Server.FetchAllRealTimeSales( nNetworkIdx, nType );
}

/**********************************************************************/

void CCommsHandshaker::ProcessSales()
{
	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = FALSE;
	infoLock.m_nCommsAction = COMMS_ACTION_DISABLE;
	infoLock.m_bStopComms = TRUE;
	infoLock.m_bStopCommsPMSExemption = FALSE;
	infoLock.m_strStopCommsPrompt = "Process CashRSP Buffer";
	infoLock.m_bStopPosTray = TRUE;
	infoLock.m_pParentWnd = NULL;

	CLockManager LockManager( infoLock );
	
	if ( SysInfo.GetProcessCashRSPBufferFlag() == TRUE )
	{
		CArray<int,int> arrayNetworkIdx;
		for ( int nNwkIdx = 0; nNwkIdx < dbNetwork.GetSize(); nNwkIdx++ )
		{
			if ( dbNetwork.GetConnectionType( nNwkIdx ) == CONNECTION_TYPE_STANDARD_REALTIME )
				arrayNetworkIdx.Add( nNwkIdx );
		}

		if ( arrayNetworkIdx.GetSize() > 0 )
		{
			if ( LockManager.GetLock() == FALSE )
				return;

			CCashRSPClearBufferDlg dlgBuffer( arrayNetworkIdx, NULL );
			dlgBuffer.DoModal();
		}
	}
	
	CashRSPImporter.ClearCashRSPVersionCheckerFailFlag();
	for ( int nNwkIdx = 0; nNwkIdx < dbNetwork.GetSize(); nNwkIdx++ )
	{
		CCashRSPImporterStatusDisplay StatusDisplay;
		if ( CashRSPImporter.ProcessNetworkSales(nNwkIdx, LockManager, StatusDisplay ) == FALSE )
			break;
	}

	if ( CashRSPImporter.GetCashRSPVersionCheckerFailFlag() == TRUE )
	{
		CashRSPImporter.ShowCashRSPVersionCheckerFailMessage();
		CashRSPImporter.ClearCashRSPVersionCheckerFailFlag();
		return;
	}

#ifdef STOCKMAN_SYSTEM

	CZSalesImporterStockman ZSalesImporter;
	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
		for ( int nEcrIdx = 0; nEcrIdx < dbLocation.GetTerminalCount( nLocIdx ); nEcrIdx++ )
			ZSalesImporter.LookForSales( nLocIdx, nEcrIdx );
	ZSalesImporter.LookForPMSSales();

	CZDeliveryImporterStockman ZDeliveryImporter;
	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
		for ( int nEcrIdx = 0; nEcrIdx < dbLocation.GetTerminalCount( nLocIdx ); nEcrIdx++ )
			ZDeliveryImporter.LookForDeliveries( nLocIdx, nEcrIdx );

	if ( StockOptions.GetPluRedirectToStockpointFlag() == TRUE )
	{
		CZSalesRedirectorStockman ZSalesRedirector;
		for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
		{
			ZSalesRedirector.LookForStockpointSales( nSpIdx, FALSE );

			if ( SysInfo.IsPMSSystem() == TRUE )
				ZSalesRedirector.LookForStockpointSales( nSpIdx, TRUE );
		}

		CZDeliveryRedirectorStockman ZDeliveryRedirector;
		for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
			ZDeliveryRedirector.LookForStockpointDeliveries( nSpIdx );
	}

#else

	CZSalesImporterEcrman ZSalesImporter;

	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
		for ( int nEcrIdx = 0; nEcrIdx < dbLocation.GetTerminalCount( nLocIdx ); nEcrIdx++ )
			ZSalesImporter.LookForSales( nLocIdx, nEcrIdx );

	ZSalesImporter.LookForPMSSales();

	if ( EcrmanOptions.GetFeaturesPluStockFlag() == TRUE )
	{
		CZDeliveryImporterEcrman ZDeliveryImporter;

		for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
			for ( int nEcrIdx = 0; nEcrIdx < dbLocation.GetTerminalCount( nLocIdx ); nEcrIdx++ )
				ZDeliveryImporter.LookForDeliveries( nLocIdx, nEcrIdx );
	}

#endif
}

/**********************************************************************/
#else
/**********************************************************************/

int CCommsHandshaker::GetTranCountForPosTray( const char* sz )
{ 
	return m_5000Server.GetTranCountForPosTray( sz ); 
}

/**********************************************************************/

bool CCommsHandshaker::ProcessSalesForPosTray( CCashRSPImporterStatusDisplay& StatusDisplay )
{
	CLockManagerInfo infoLock;
	CLockManager LockManager( infoLock );

	CashRSPImporter.ClearCashRSPVersionCheckerFailFlag();
	for (int nNwkIdx = 0; nNwkIdx < dbNetwork.GetSize(); nNwkIdx++)
	{
		if (CashRSPImporter.ProcessNetworkSales(nNwkIdx, LockManager, StatusDisplay) == FALSE)
		{
			return FALSE;
		}
	}

	if (CashRSPImporter.GetCashRSPVersionCheckerFailFlag() == TRUE)
	{
		CashRSPImporter.ClearCashRSPVersionCheckerFailFlag();
	}

	return TRUE;
}

/**********************************************************************/

bool CCommsHandshaker::CheckStoppedForPosTray()
{
	//WaitExit( bFlag, 1000, 20 );

	if ( FALSE == m_bInitialisedComms )
		return TRUE;

	if ( m_5000Server.QuickCheckNoComms() == TRUE )
		return TRUE;

	return m_5000Server.m_bConfirmStopped;
}

/**********************************************************************/
#endif
/**********************************************************************/

bool CCommsHandshaker::ConfirmStoppedOrPMS( const char* szAction, bool bPMS, CWnd* pParent )
{
	if ( TRUE == bPMS )
		return TRUE;
	else
		return ConfirmStopped( szAction, m_5000Server.m_bConfirmStopped, pParent );
}

/**********************************************************************/

bool CCommsHandshaker::ConfirmStopped( const char* szAction, CWnd* pParent )
{
	return ConfirmStopped( szAction, m_5000Server.m_bConfirmStopped, pParent );
}

/**********************************************************************/

bool CCommsHandshaker::ConfirmStopped( const char* szAction, bool& bFlag, CWnd* pParent )
{
	//WaitExit( bFlag, 1000, 20 );

	if ( FALSE == m_bInitialisedComms )
		return TRUE;

	if ( m_5000Server.QuickCheckNoComms() == TRUE )
		return TRUE;

	if ( FALSE == m_5000Server.m_bConfirmStopped )
	{
		CWaitCommsDlg dlg( szAction, pParent );
		dlg.DoModal();
	}

	return m_5000Server.m_bConfirmStopped;
}

/**********************************************************************/
