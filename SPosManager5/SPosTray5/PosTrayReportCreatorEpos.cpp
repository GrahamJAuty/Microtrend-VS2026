/**********************************************************************/
#include "CommsHandshaker.h"
#include "DatabaseCSVArray.h"
#include "EposReportSptBookCascade.h"
#include "LocationCSVArray.h"
#include "LocationSelectorEntity.h"
#include "NodeTypes.h"
#include "PMSOptions.h"
#include "PosTrayDateRange.h"
#include "PosTrayOptions.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PosTrayReportCreatorEpos.h"
/**********************************************************************/

CPosTrayReportCreatorEpos::CPosTrayReportCreatorEpos( COleDateTime& dateTask, CPosTrayTask& Task ) : m_Task( Task )
{
	m_dateTask = dateTask;
	m_nSkippedReportsEmpty = 0;
	m_nSkippedReportsNoChange = 0;

	m_EposReportSelect.LoadReportSelection();
	m_ParamsArray.SetBatchNo( Task.GetBatchNo() );
	m_ParamsArray.Read( FALSE );

	//EMAIL FOLDER
	{
		m_strEmailFolder = "PosTray\\Email";
		::GetSyssetProgramPath( m_strEmailFolder );
		::CreateSubdirectory( m_strEmailFolder );

		m_strEmailFolder += "\\EPOSREPORTS";
		::CreateSubdirectory( m_strEmailFolder );
	}

	//PRINT FOLDER
	{
		m_strPrintFolder = "PosTray\\Print";
		::GetSyssetProgramPath( m_strPrintFolder );
		::CreateSubdirectory( m_strPrintFolder );

		COleDateTime timeNow = COleDateTime::GetCurrentTime();
		m_strPrintFilenamePrefix.Format( "%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d_1%3.3d_",
			timeNow.GetYear(),
			timeNow.GetMonth(),
			timeNow.GetDay(),
			timeNow.GetHour(),
			timeNow.GetMinute(),
			timeNow.GetSecond(),
			m_Task.GetTaskNo() );

		m_nNextPrintFileNumber = 1;
	}
	
	//EXPORT FOLDER
	{
		m_strExportFolderTarget = m_Task.GetTaskExportFolder();
		::TrimSpaces( m_strExportFolderTarget, FALSE );
		
		if ( m_strExportFolderTarget == "" )
		{
			m_strExportFolderTarget = "PosTray\\Exports";
			::GetSyssetProgramPath( m_strExportFolderTarget );
		}

		::CreateSubdirectory( m_strExportFolderTarget );

		m_strExportFolderPending = "PosTray\\Pending";
		::GetSyssetProgramPath( m_strExportFolderPending );
		::CreateSubdirectory( m_strExportFolderPending );
		m_strExportFolderPending += "\\EposReports";
		::CreateSubdirectory( m_strExportFolderPending );
	}

	//MD5CACHE
	m_strMD5CacheFolder = "";
	if ( PosTrayOptions.GetSkipNoChangeFlag() == TRUE )
	{
		m_strMD5CacheFolder = "PosTray\\MD5Cache";
		::GetSyssetProgramPath(m_strMD5CacheFolder);
		::CreateSubdirectory(m_strMD5CacheFolder);
	}
}

/**********************************************************************/

CPosTrayReportCreatorEpos::~CPosTrayReportCreatorEpos(void)
{
}

/**********************************************************************/

bool CPosTrayReportCreatorEpos::ProcessTask()
{
	{
		CDataManagerInfo info;
		DataManagerNonDb.OpenReportSession( DB_READONLY, info );
		DataManagerNonDb.OpenTimeSliceList( DB_READONLY, info );
		DataManagerNonDb.OpenSystemPluFilterNames( DB_READONLY, info );
		DataManagerNonDb.OpenSystemPluFilterItems( DB_READONLY, info );
		DataManagerNonDb.OpenSystemDepartment( DB_READONLY, info );
		DataManagerNonDb.OpenSystemDepartmentSet( DB_READONLY, info );
	}

	DataManagerNonDb.ServerNameTable.BuildList();
	DataManagerNonDb.CustomerNameTable.BuildList();
	DataManagerNonDb.CustomerNameTable.BuildList();
	DataManagerNonDb.EODDateTimeFilterArray.PrepareForUse();
	DataManagerNonDb.SessionDateTimeFilter.Reset();
		
	bool bResult = ProcessTaskInternal();

	{
		CDataManagerInfo info;
		DataManagerNonDb.CloseReportSession( info );
		DataManagerNonDb.CloseTimeSliceList( info );
		DataManagerNonDb.CloseSystemPluFilterNames( info );
		DataManagerNonDb.CloseSystemPluFilterItems(info );
		DataManagerNonDb.CloseSystemDepartment( info );
		DataManagerNonDb.CloseSystemDepartmentSet( info );
	}

	DataManagerNonDb.ServerNameTable.ClearList();
	DataManagerNonDb.CustomerNameTable.ClearList();
	DataManagerNonDb.CustomerNameTable.ClearList();
	DataManagerNonDb.EODDateTimeFilterArray.ClearAfterUse();
	DataManagerNonDb.SessionDateTimeFilter.Reset();

	return bResult;
}

/**********************************************************************/

bool CPosTrayReportCreatorEpos::ProcessTaskInternal()
{
	m_nDeptFilter = m_Task.GetBatchDeptFilter();
	m_nPluFilter = m_Task.GetBatchPluFilter();
	m_bSaleTime = m_Task.GetBatchSaleTimeFlag();
	
	CPosTrayDateRange dateRange;
	
	if ( PosTrayOptions.GetBusinessDateFlag() == TRUE )
		dateRange.SetDateToday( m_dateTask, EcrmanOptions.GetReportsDayStartHour(), EcrmanOptions.GetReportsDayStartMinute() );
	else
		dateRange.SetDateToday( m_dateTask );

	dateRange.GetRange( m_Task.GetBatchDateType(), m_oleDateFrom, m_oleDateTo, m_oleFutureDateTo );

	{
		CString strSession = m_Task.GetBatchSessionEntity();
		
		m_ReportSession.ClearRecord();
		
		if ( strSession == "EOD" )
		{
			m_ReportSession.SetEODFlag( TRUE );
		}
		else if ( strSession != "" )
		{
			for ( int n = 0; n < DataManagerNonDb.ReportSession.GetSize(); n++ )
			{
				CSessionCSVRecord arrayRecord;
				DataManagerNonDb.ReportSession.GetAt( n, arrayRecord );

				if ( strSession == arrayRecord.GetEntityID() )
				{
					DataManagerNonDb.ReportSession.GetAt( n, m_ReportSession );				
					break;
				}
			}
		}
		else
		{
			strSession = m_Task.GetBatchSessionLegacy();
			
			for ( int n = 0; n < DataManagerNonDb.ReportSession.GetSize(); n++ )
			{
				CSessionCSVRecord ReportSession;
				DataManagerNonDb.ReportSession.GetAt( n, ReportSession );

				if ( strSession == ReportSession.GetName() )
				{
					DataManagerNonDb.ReportSession.GetAt( n, m_ReportSession );
					break;
				}
			}
		}

		CString strName = m_ReportSession.GetName();
		if ( strName == "" )
		{
			strName.Format ( "%2.2d:%2.2d to %2.2d:%2.2d",
				m_ReportSession.GetSessionStartHour(),
				m_ReportSession.GetSessionStartMinute(),
				m_ReportSession.GetSessionEndHour(),
				m_ReportSession.GetSessionEndMinute() );

			m_ReportSession.SetName( strName );
		}
	}

	if ( m_Task.GetBatchReportConFlag() == TRUE )
		if ( ProcessNormalReports() == FALSE )
			return FALSE;

	if ( ( m_Task.GetBatchReportEcrFlag() == TRUE ) || ( m_Task.GetBatchReportLocFlag() == TRUE ) || ( m_Task.GetBatchReportAllFlag() == TRUE ) )
		if ( ProcessQuickReports() == FALSE )
			return FALSE;
	
	return TRUE;
}

/**********************************************************************/

bool CPosTrayReportCreatorEpos::ProcessNormalReports()
{
	CLocationSelectorLookup lookup( m_Task );
	CLocationSelectorEntity entity( lookup );

	if ( entity.IsInvalid() == TRUE )
		return FALSE;

	entity.GetEPOSSelectArray( m_SelectArray );
	CString strTitle = entity.GetConsolidationTitle();
	
	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( entity.GetDbIdx(), info, FALSE );

	CCSV csvReports( m_Task.GetBatchReportList() );

	CReportConsolidationArray<CSortedIntByString> arrayEmailByReport;
	m_Task.CreateEmailReportMap(arrayEmailByReport);

	for ( int n = 0; n < csvReports.GetSize(); n++ )
	{
		int nReportType = csvReports.GetInt(n);

		bool bCanCreate = TRUE;
		if ( REPORT_TYPE_CUSTOMER_BALANCE == nReportType )
		{
			switch( lookup.GetConType() )
			{
			case NODE_LOCATION_TERMINAL:
			case NODE_REPORTPOINT:
				bCanCreate = FALSE;
				break;
			}
		}
	
		if ( TRUE == bCanCreate )
		{
			CPosTrayTaskConsolInfo infoQuick;
			infoQuick.m_nQuickType = 0;
			infoQuick.m_nDbNo = 0;
			infoQuick.m_nNwkNo = 0;
			infoQuick.m_nLocNo = 0;
			infoQuick.m_nTermNo = 0;
			CreateReport ( nReportType, lookup.GetConType(), strTitle, arrayEmailByReport, infoQuick );
		}
	}

	return TRUE;	
}

/**********************************************************************/

bool CPosTrayReportCreatorEpos::ProcessQuickReports()
{
	CLocationSelectorLookup lookup( m_Task );
	CLocationSelectorEntity entity( lookup );

	if ( entity.IsInvalid() == TRUE )
		return FALSE;

	int nConType = entity.GetConType();

	int nFirstDbIdx, nLastDbIdx;
	if ( NODE_SYSTEM == nConType )
	{
		nFirstDbIdx = 0;
		nLastDbIdx = dbDatabase.GetSize() - 1;
	}
	else
	{
		nFirstDbIdx = entity.GetDbIdx();
		nLastDbIdx = nFirstDbIdx;
	}

	CStringArray arrayDatabases;
						
	{
		CEposSelectArray LocalSelectArray;
		entity.GetEPOSSelectArray ( LocalSelectArray );

		for ( int nDbIdx = nFirstDbIdx; nDbIdx <= nLastDbIdx; nDbIdx++ )
		{
			CString strLocations = "";

			CArray<int,int> Set;
			dbLocation.GetDatabaseLocIdxSet ( nDbIdx, Set );

			CCSV csvDatabase;
			for ( int n = 0; n < Set.GetSize(); n++ )
			{
				int nLocIdx = Set.GetAt(n);
				CString strTerminals = LocalSelectArray.GetLocationString(nLocIdx);
				
				if ( strTerminals != "" )
				{
					csvDatabase.Add( nLocIdx );
					csvDatabase.Add( strTerminals );
				}
			}

			arrayDatabases.Add( csvDatabase.GetLine() );
		}
	}

	bool bTerminal = m_Task.GetBatchReportEcrFlag();
	bool bLocation = m_Task.GetBatchReportLocFlag();
	bool bDatabase = m_Task.GetBatchReportAllFlag();

	CCSV csvReports( m_Task.GetBatchReportList() );

	CReportConsolidationArray<CSortedIntByString> arrayEmailByReport;
	m_Task.CreateEmailReportMap(arrayEmailByReport);

	for ( int n = 0; n < csvReports.GetSize(); n++ )
	{
		int nReportType = csvReports.GetInt(n);

		for ( int x = 0; x < arrayDatabases.GetSize(); x++ )
		{
			CString strLocations = arrayDatabases.GetAt(x);

			if ( strLocations != "" )
			{
				int nDbIdx = nFirstDbIdx + x;
				
				CCSV csvLocations( strLocations );
				int nLocCount = csvLocations.GetSize() / 2;

				for ( int y = 0; y < nLocCount * 2; y += 2 )
				{
					int nLocIdx = csvLocations.GetInt(y);
					CString strTerminals = csvLocations.GetString(y + 1);

					//REPORTS BY TERMINAL
					if ( ( TRUE == bTerminal ) && ( REPORT_TYPE_CUSTOMER_BALANCE != nReportType ) )
					{
						CArray<int,int> arrayTermIdx;
						for ( int z = 0; z < strTerminals.GetLength(); z++ )
						{
							if ( strTerminals.GetAt(z) == '1' )
								arrayTermIdx.Add(z);
						}

						bool bShowTerminals = TRUE;
						if ( arrayTermIdx.GetSize() < 2 )
						{
							if ( TRUE == bLocation )
								bShowTerminals = FALSE;
							else if ( ( TRUE == bDatabase ) && ( 1 == nLocCount ) )
								bShowTerminals = FALSE;
						}

						if ( TRUE == bShowTerminals )
						{
							for ( int t = 0; t < arrayTermIdx.GetSize(); t++ )
							{
								int nTermIdx = arrayTermIdx.GetAt(t);
					
								m_SelectArray.RemoveAll();
								m_SelectArray.SelectTerminal ( nLocIdx, nTermIdx );

								CString strTitle;
								strTitle.Format ( "%s, %s", dbLocation.GetName( nLocIdx ), dbLocation.GetTerminalName( nLocIdx, nTermIdx ) );
							
								CPosTrayTaskConsolInfo infoQuick;
								infoQuick.m_nQuickType = 1;
								infoQuick.m_nDbNo = dbLocation.GetDbNo( nLocIdx );
								infoQuick.m_nNwkNo = dbLocation.GetNetworkNo( nLocIdx );
								infoQuick.m_nLocNo = dbLocation.GetLocNo( nLocIdx );
								infoQuick.m_nTermNo = dbLocation.GetTNo( nLocIdx, nTermIdx );
								CreateReport ( nReportType, nConType, strTitle, arrayEmailByReport, infoQuick );
							}
						}
					}

					if ( TRUE == bLocation )
					{
						if ( ( FALSE == bDatabase ) || ( nLocCount >= 2 ) )
						{
							m_SelectArray.RemoveAll();
							m_SelectArray.SetLocationString( nLocIdx, strTerminals );
						
							CString strTitle;
							strTitle.Format ( "%s", dbLocation.GetName( nLocIdx ) );

							CPosTrayTaskConsolInfo infoQuick;
							infoQuick.m_nQuickType = 2;
							infoQuick.m_nDbNo = dbLocation.GetDbNo( nLocIdx );
							infoQuick.m_nNwkNo = dbLocation.GetNetworkNo( nLocIdx );
							infoQuick.m_nLocNo = dbLocation.GetLocNo( nLocIdx );
							infoQuick.m_nTermNo = 0;
							CreateReport ( nReportType, nConType, strTitle, arrayEmailByReport, infoQuick );
						}
					}
				}

				if ( TRUE == bDatabase )
				{
					m_SelectArray.RemoveAll();

					for ( int y = 0; y < nLocCount * 2; y += 2 )
					{
						m_SelectArray.SetLocationString( csvLocations.GetInt(y), csvLocations.GetString(y+1) );
					}
		
					CString strTitle;
					strTitle.Format ( "%s", dbDatabase.GetName( nDbIdx ) );

					CPosTrayTaskConsolInfo infoQuick;
					infoQuick.m_nQuickType = 3;
					infoQuick.m_nDbNo = dbDatabase.GetDbNo( nDbIdx );
					infoQuick.m_nNwkNo = 0;
					infoQuick.m_nLocNo = 0;
					infoQuick.m_nTermNo = 0;
					CreateReport ( nReportType, nConType, strTitle, arrayEmailByReport, infoQuick );
				}
			}
		}
	}

	return TRUE;
}

/**********************************************************************/

bool CPosTrayReportCreatorEpos::CreateReport( int nReportType, int nConLevel, CString& strTitle, CReportConsolidationArray<CSortedIntByString>& arrayEmailByReport, CPosTrayTaskConsolInfo& infoQuick )
{
	CString strSessionTitle = m_ReportSession.GetName();

	CEposReportSelectInfo infoReport;
	infoReport.SetReportType( nReportType );
	bool bResult = m_EposReportSelect.GetReportInfo( infoReport );

	CEposReport* pReport = NULL;
	bool bFixStartDate = FALSE;

	CString strTaskFilename = m_Task.GetTaskExportFilename();
	::TrimSpaces( strTaskFilename, FALSE );

	if ( TRUE == bResult )
	{
		//SYSTEM PREFERENCES
		infoReport.LoadConsolPrefs();

		//TASK SPECIFIC PREFERENCES
		CPosTrayEposConsolParamsInfo infoParams;
		m_ParamsArray.GetParams( infoReport, infoParams );
		infoReport.LoadPosTrayConsolPrefs( infoParams, m_Task );
		m_EposReportSelect.SetReportInfo( infoReport );
		
		if ( nReportType >= EPOS_CUSTOM_FIRST_REPORT )
		{
			bResult = FALSE;

			switch( infoReport.GetCustomReportFamily() )
			{
#ifndef POSTRAY_UTILITY
			case EPOS_CUSTOM_FAMILY_LOYALTYRECON:
				pReport = new CEposReportLoyaltyRecon(m_SelectArray, infoReport.GetCustomReportParams());
				bResult = TRUE;
				break;
#endif

			case EPOS_CUSTOM_FAMILY_HOURLY:
				pReport = new CEposReportHourly( m_SelectArray, infoReport.GetCustomReportParams() );
				bResult = TRUE;
				break;

			case EPOS_CUSTOM_FAMILY_HOURLYGRP:
				pReport = new CEposReportHourlyGroup( m_SelectArray, infoReport.GetCustomReportParams(), FALSE );
				bResult = TRUE;
				break;

			case EPOS_CUSTOM_FAMILY_HOURLYDPT:
				pReport = new CEposReportHourlyGroup( m_SelectArray, infoReport.GetCustomReportParams(), TRUE );
				bResult = TRUE;
				break;

			case EPOS_CUSTOM_FAMILY_COVERS_HOURLY:
				pReport = new CEposReportCoversHourly( m_SelectArray, infoReport.GetCustomReportParams() );
				bResult = TRUE;
				break;

			case EPOS_CUSTOM_FAMILY_COVERS_HOURLYGRP:
				pReport = new CEposReportCoversHourlyGroup( m_SelectArray, infoReport.GetCustomReportParams(), FALSE );
				bResult = TRUE;
				break;

			case EPOS_CUSTOM_FAMILY_COVERS_HOURLYDPT:
				pReport = new CEposReportCoversHourlyGroup( m_SelectArray, infoReport.GetCustomReportParams(), TRUE );
				bResult = TRUE;
				break;

			case EPOS_CUSTOM_FAMILY_GENERAL:
				pReport = new CEposReportGeneral( m_SelectArray, nReportType, infoReport.GetCustomReportParams() );
				bResult = TRUE;
				break;

			case EPOS_CUSTOM_FAMILY_TRANSACTION:
				pReport = new CEposReportTransactionSorted( m_SelectArray, nReportType, infoReport.GetCustomReportParams() );
				bResult = TRUE;
				break;

			case EPOS_CUSTOM_FAMILY_PAYMENTDETAIL:
				pReport = new CEposReportPaymentDetail(m_SelectArray, infoReport.GetCustomReportParams());
				bResult = TRUE;
				break;

			case EPOS_CUSTOM_FAMILY_PLU_PRICEBAND:
				{
					CString strParams = infoReport.GetCustomReportParams();
					CEposReportCustomSettingsPluPriceBand Settings;
					Settings.SetSettingsCSVLine(strParams);

					if (Settings.GetSortByServerFlag() == FALSE)
					{
						pReport = new CEposReportPluByPriceBandSummary(m_SelectArray, infoReport.GetCustomReportParams());
					}
					else
					{
						pReport = new CEposReportPluByPriceBandServer(m_SelectArray, infoReport.GetCustomReportParams());
					}
				}
				break;

			case EPOS_CUSTOM_FAMILY_DEPOSIT:
				{
					CString strParams = infoReport.GetCustomReportParams();
					CEposReportCustomSettingsDeposit Settings;
					Settings.SetSettingsCSVLine( strParams );
					
					switch( Settings.GetReportType() )
					{
					case 1:
						pReport = new CEposReportAccountBookingPurchase( m_SelectArray, strParams );
						bFixStartDate = FALSE;
						break;

					case 2:
						pReport = new CEposReportAccountBookingPurchase( m_SelectArray, strParams );
						bFixStartDate = TRUE;
						break;

					case 3:
						pReport = new CEposReportAccountBookingEvent( m_SelectArray, strParams );
						bFixStartDate = FALSE;
						break;

					case 0:
					default:
						pReport = new CEposReportAccountReceiveDeposit( m_SelectArray, strParams );
						bFixStartDate = FALSE;
						break;
					}
				}
				bResult = TRUE;
				break;

			case EPOS_CUSTOM_FAMILY_MIXMATCH:
				{
					CString strParams = infoReport.GetCustomReportParams();

					CEposReportCustomSettingsMixMatch Settings;
					Settings.SetSettingsCSVLine( strParams );
					
					int nReportType = 0;

					if ( Settings.GetCoversFlag() == FALSE )
					{
						switch( Settings.GetReportType() )
						{
						case 1:		nReportType = REPORT_TYPE_MIXMATCH_DEPT;	break;
						case 2:		nReportType = REPORT_TYPE_MIXMATCH_ITEM;	break;
						case 0:		
						default:	nReportType = REPORT_TYPE_MIXMATCH_GROUP;	break;
						}
					}
					else
					{
						switch( Settings.GetReportType() )
						{
						case 1:		nReportType = REPORT_TYPE_MIXMATCH_DEPT_COVERS;		break;
						case 2:		nReportType = REPORT_TYPE_MIXMATCH_ITEM_COVERS;		break;
						case 0:		
						default:	nReportType = REPORT_TYPE_MIXMATCH_GROUP_COVERS;	break;
						}
					}

					pReport = new CEposReportMixMatchItem( m_SelectArray, nReportType, strParams );
				}
				bResult = TRUE;
				break;

			case EPOS_CUSTOM_FAMILY_DISCOUNT:
				{
					CString strParams = infoReport.GetCustomReportParams();

					CEposReportCustomSettingsDiscount Settings;
					Settings.SetSettingsCSVLine( strParams );
					
					if ( Settings.GetByServerFlag() == TRUE )
						pReport = new CEposReportDiscountServer( m_SelectArray, strParams );
					else
						pReport = new CEposReportDiscountSummary( m_SelectArray, strParams );
				}
				bResult = TRUE;
				break;

			default:
				bResult = FALSE;
				break;
			}	
		}
		else
		{
			switch ( nReportType )
			{
			case REPORT_TYPE_PLUSALES:				
			case REPORT_TYPE_PLUSALES_NOZEROSKIP:
			case REPORT_TYPE_PLUWASTE:
			case REPORT_TYPE_VIPSALES:					
				pReport = new CEposReportPluVipSalesSummary( m_SelectArray, nReportType );		
				break;

			case REPORT_TYPE_PLUSALES_TRANSERVER:
			case REPORT_TYPE_PLUWASTE_TRANSERVER:
			case REPORT_TYPE_COVERS_PLU_TRANSERVER:
			case REPORT_TYPE_VIPSALES_TRANSERVER:
				pReport = new CEposReportPluVipSalesServer( m_SelectArray, nReportType );	
				break;

			case REPORT_TYPE_PLUSALES_DEPT_ITEM:		
				pReport = new CEposReportPluVipSalesDept( m_SelectArray, REPORT_TYPE_PLUSALES_DEPT_ITEM );	
				break;

			case REPORT_TYPE_PLUSALES_TAX_ITEM:		
				pReport = new CEposReportPluSalesTax( m_SelectArray );	
				break;

			case REPORT_TYPE_PLUSALES_CUSTOMER:			
				pReport = new CEposReportPluSalesCustomer( m_SelectArray );		
				break;

			case REPORT_TYPE_PLUSALES_MODIFIER:			
				pReport = new CEposReportPluVipSalesModifier( m_SelectArray, REPORT_TYPE_PLUSALES_MODIFIER );		
				break;

			case REPORT_TYPE_PLUSALES_DEPT_SUMMARY:		
				pReport = new CEposReportPluVipDeptSummary( m_SelectArray, REPORT_TYPE_PLUSALES_DEPT_SUMMARY );	
				break;

			case REPORT_TYPE_TAX_SUMMARY_TAX:		
				pReport = new CEposReportTaxSummaryTax( m_SelectArray );	
				break;

			case REPORT_TYPE_TAX_SUMMARY_PRICE:		
				pReport = new CEposReportTaxSummaryPrice( m_SelectArray );	
				break;

			case REPORT_TYPE_VIPSALES_DEPT_ITEM:		
				pReport = new CEposReportPluVipSalesDept( m_SelectArray, REPORT_TYPE_VIPSALES_DEPT_ITEM );			
				break;

			case REPORT_TYPE_VIPSALES_MODIFIER:			
				pReport = new CEposReportPluVipSalesModifier( m_SelectArray, REPORT_TYPE_VIPSALES_MODIFIER );			
				break;

			case REPORT_TYPE_VIPSALES_DEPT_SUMMARY:		
				pReport = new CEposReportPluVipDeptSummary( m_SelectArray, REPORT_TYPE_VIPSALES_DEPT_SUMMARY  );			
				break;

			case REPORT_TYPE_PLUWASTE_DEPT_ITEM:		
				pReport = new CEposReportPluVipSalesDept( m_SelectArray, REPORT_TYPE_PLUWASTE_DEPT_ITEM );	
				break;

			case REPORT_TYPE_PLUWASTE_MODIFIER:			
				pReport = new CEposReportPluVipSalesModifier( m_SelectArray, REPORT_TYPE_PLUWASTE_MODIFIER );	
				break;

			case REPORT_TYPE_PLUWASTE_DEPT_SUMMARY:		
				pReport = new CEposReportPluVipDeptSummary( m_SelectArray, REPORT_TYPE_PLUWASTE_DEPT_SUMMARY );	
				break;

			case REPORT_TYPE_HOURLY:					
				pReport = new CEposReportHourly( m_SelectArray );				
				break;

			case REPORT_TYPE_HOURLY_GROUP:					
				pReport = new CEposReportHourlyGroup( m_SelectArray, FALSE );				
				break;

			case REPORT_TYPE_HOURLY_DEPT:					
				pReport = new CEposReportHourlyGroup( m_SelectArray, TRUE );				
				break;

			case REPORT_TYPE_DAILY:					
				pReport = new CEposReportDaily( m_SelectArray );				
				break;

			case REPORT_TYPE_DAILY_GROUP:					
				pReport = new CEposReportDailyGroup( m_SelectArray, FALSE );				
				break;

			case REPORT_TYPE_DAILY_DEPT:					
				pReport = new CEposReportDailyGroup( m_SelectArray, TRUE );				
				break;

			case REPORT_TYPE_SPTBOOK_CASCADE:					
				pReport = new CEposReportSptBookCascade( m_SelectArray );				
				break;

			case REPORT_TYPE_COVERS_GROUP:				
				pReport = new CEposReportCoversGroup( m_SelectArray, FALSE );			
				break;

			case REPORT_TYPE_COVERS_DEPT:				
				pReport = new CEposReportCoversGroup( m_SelectArray, TRUE );			
				break;

			case REPORT_TYPE_COVERS_HOURLY:				
				pReport = new CEposReportCoversHourly( m_SelectArray );					
				break;

			case REPORT_TYPE_COVERS_HOURLY_GROUP:				
				pReport = new CEposReportCoversHourlyGroup( m_SelectArray, FALSE );				
				break;

			case REPORT_TYPE_COVERS_HOURLY_DEPT:				
				pReport = new CEposReportCoversHourlyGroup( m_SelectArray, TRUE );				
				break;

			case REPORT_TYPE_COVERS_DAILY:				
				pReport = new CEposReportCoversDaily( m_SelectArray );					
				break;

			case REPORT_TYPE_COVERS_DAILY_GROUP:				
				pReport = new CEposReportCoversDailyGroup( m_SelectArray, FALSE );				
				break;

			case REPORT_TYPE_COVERS_DAILY_DEPT:				
				pReport = new CEposReportCoversDailyGroup( m_SelectArray, TRUE );				
				break;

			case REPORT_TYPE_COVERS_MONTHLY:				
				pReport = new CEposReportCoversMonthly( m_SelectArray );				
				break;

			case REPORT_TYPE_COVERS_PRICELEVEL:				
				pReport = new CEposReportCoversPriceLevel( m_SelectArray );			
				break;

			case REPORT_TYPE_COVERS_PAYMENT:				
				pReport = new CEposReportCoversPayment( m_SelectArray );			
				break;

			case REPORT_TYPE_TRANSERVER_SUMMARY:					
				pReport = new CEposReportServer( m_SelectArray );				
				break;

			case REPORT_TYPE_GENERAL_GROUP:				
			case REPORT_TYPE_GENERAL_DEPT:		
			case REPORT_TYPE_GENERAL_GROUP_QTY:				
			case REPORT_TYPE_GENERAL_DEPT_QTY:			
			case REPORT_TYPE_WEEKLY_GROUP:				
			case REPORT_TYPE_WEEKLY_DEPT:
			case REPORT_TYPE_WEEKLY_GROUP_QTY:				
			case REPORT_TYPE_WEEKLY_DEPT_QTY:	
				pReport = new CEposReportGeneral( m_SelectArray, nReportType );		
				break;

			case REPORT_TYPE_PAYMENT_SUMMARY:			
				pReport = new CEposReportPaymentSummary( m_SelectArray );	
				break;

			case REPORT_TYPE_PAYMENT_SERVER:			
				pReport = new CEposReportPaymentServer( m_SelectArray );			
				break;

			case REPORT_TYPE_PAYMENT_GROUP_SUMMARY:			
				pReport = new CEposReportPaymentReportGroup( m_SelectArray );	
				break;

			case REPORT_TYPE_PAYMENT_ESTIMATE_GROUP:				
			case REPORT_TYPE_PAYMENT_ESTIMATE_DEPT:				
			case REPORT_TYPE_PAYMENT_ESTIMATE_ITEM:	
				pReport = new CEposReportPaymentByItem( m_SelectArray, nReportType ); 
				break;

			case REPORT_TYPE_PAYMENT_ESTIMATE_TAX:		
				pReport = new CEposReportPaymentByTaxBand( m_SelectArray ); 
				break;

			case REPORT_TYPE_PAYMENT_DETAIL:				
				pReport = new CEposReportPaymentDetail( m_SelectArray ); 
				break;

			case REPORT_TYPE_DEPOSIT_PURCHASED:			
				pReport = new CEposReportAccountReceiveDeposit( m_SelectArray ); 
				break;

			case REPORT_TYPE_DEPOSIT_BOOKING_PURCHASE:			
				pReport = new CEposReportAccountBookingPurchase( m_SelectArray ); 
				break;

			case REPORT_TYPE_DEPOSIT_BOOKING_EVENT:			
				pReport = new CEposReportAccountBookingEvent( m_SelectArray ); 
				break;

			case REPORT_TYPE_DEPOSIT_REDEEMED:			
				pReport = new CEposReportDepositRedeemed( CASHRSP_ITEMPAYTYPE_DEPOSIT, m_SelectArray );		
				break;

			case REPORT_TYPE_DEPOSIT_UNREDEEMED:		
				pReport = new CEposReportDepositUnredeemed( m_SelectArray );	
				break;

			case REPORT_TYPE_CUSTOMER_RA:				
				pReport = new CEposReportAccountReceiveCustomer( m_SelectArray );		
				break;

			case REPORT_TYPE_CUSTOMER_SPEND:			
				pReport = new CEposReportDepositRedeemed( CASHRSP_ITEMPAYTYPE_CUSTOMER, m_SelectArray );		
				break;

			case REPORT_TYPE_CUSTOMER_ACTIVITY:			
				pReport = new CEposReportCustomerActivity( m_SelectArray );		
				break;

			case REPORT_TYPE_CUSTOMER_BALANCE:			
				pReport = new CEposReportCustomerBalance( m_SelectArray );		
				break;

			case REPORT_TYPE_PROMOTION_SUMMARY:				
				pReport = new CEposReportPromotionSummary( m_SelectArray );				
				break;

			case REPORT_TYPE_MIXMATCH_GROUP:
			case REPORT_TYPE_MIXMATCH_DEPT:
			case REPORT_TYPE_MIXMATCH_ITEM:
			case REPORT_TYPE_MIXMATCH_GROUP_COVERS:
			case REPORT_TYPE_MIXMATCH_DEPT_COVERS:
			case REPORT_TYPE_MIXMATCH_ITEM_COVERS:
				pReport = new CEposReportMixMatchItem( m_SelectArray, nReportType );				
				break;

			case REPORT_TYPE_CHEAPESTONEFREE_SUMMARY:				
				pReport = new CEposReportCheapestOneFreeSummary( m_SelectArray );				
				break;

			case REPORT_TYPE_CHEAPESTONEFREE_DETAIL:				
				pReport = new CEposReportCheapestOneFreeDetail( m_SelectArray );				
				break;
			
			case REPORT_TYPE_TRANSERVER_VOID:						
				pReport = new CEposReportVoid( m_SelectArray );					
				break;

			case REPORT_TYPE_TRANSERVER_VOID_REASON:						
				pReport = new CEposReportVoidReason( m_SelectArray );					
				break;

			case REPORT_TYPE_TRANSERVER_REFUND:					
				pReport = new CEposReportRefund( m_SelectArray );				
				break;

			case REPORT_TYPE_TRANSERVER_REFUND_REASON:
			case REPORT_TYPE_TRANSERVER_VIP_REASON:
			case REPORT_TYPE_TRANSERVER_WASTAGE_REASON:
				pReport = new CEposReportMiscReason( nReportType, m_SelectArray );				
				break;

			case REPORT_TYPE_TRANSERVER_DISCOUNT:					
				pReport = new CEposReportDiscountServer( m_SelectArray );				
				break;

			case REPORT_TYPE_TRANSERVER_DISCOUNT_REASON:					
				pReport = new CEposReportDiscountReason( m_SelectArray );				
				break;

			case REPORT_TYPE_NOSALE:					
				pReport = new CEposReportNoSale( m_SelectArray );				
				break;

			case REPORT_TYPE_AGEVERIFY:					
				pReport = new CEposReportAgeVerify( m_SelectArray );			
				break;

			case REPORT_TYPE_EODSTATUS:					
				pReport = new CEposReportEODStatus( m_SelectArray );				
				break;

			case REPORT_TYPE_TRANSACTION_NORMAL:
			case REPORT_TYPE_TRANSACTION_DISCREPANCY:	
				pReport = new CEposReportTransactionSorted( m_SelectArray, nReportType );	
				break;

			case REPORT_TYPE_REASON_VOID:				
			case REPORT_TYPE_REASON_REFUND:
			case REPORT_TYPE_REASON_PROMOTION:
			case REPORT_TYPE_REASON_WASTAGE:
			case REPORT_TYPE_REASON_VIP:				
				pReport = new CEposReportReason( m_SelectArray, nReportType );	
				break;

			default: 
				bResult = FALSE;
				break;
			}
		}
	}

	if ( TRUE == bResult )
	{
		pReport -> SetPreferConsolType( m_Task.GetBatchPreferConsolType() );	
	
		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		pReport -> SetEposReportSelectInfo( infoReport );
		pReport -> SetPreferConsolType( EPOSREPORT_CONSOL_ADHOC );
		pReport -> SetShowCreationTimeFlag( m_Task.GetBatchReportTimeFlag() );
		pReport -> SetCreateMD5HashFlag(PosTrayOptions.GetSkipNoChangeFlag());
		bResult = CreateReportInternal ( pReport, nConLevel, strTitle, strSessionTitle, bFixStartDate, infoQuick );
		CString strReportTitle = pReport -> GetReportTitle( FALSE );

		if (TRUE == bResult)
		{
			if ((PosTrayOptions.GetSkipEmptyFlag() == TRUE) && (pReport->GotDataLine() == FALSE))
			{
				m_nSkippedReportsEmpty++;
			}
			else
			{
				bool bSkipped = FALSE;

				if (PosTrayOptions.GetSkipNoChangeFlag() == TRUE)
				{
					CString strMD5Hash = pReport->GetMD5Hash();

					CString strFilename = "";
					strFilename.Format("%s\\EPOS_%4.4d_%3.3d_%1.1d%3.3d%3.3d%3.3d%4.4d.dat",
						(const char*)m_strMD5CacheFolder,
						m_Task.GetTaskNo(),
						nReportType,
						infoQuick.m_nQuickType,
						infoQuick.m_nDbNo,
						infoQuick.m_nNwkNo,
						infoQuick.m_nLocNo,
						infoQuick.m_nTermNo);

					{
						CSSFile fileMD5;
						if (fileMD5.Open(strFilename, "rb") == TRUE)
						{
							CString strPreviousMD5 = "";
							if (fileMD5.ReadString(strPreviousMD5) == TRUE)
							{
								if (strPreviousMD5 == strMD5Hash)
								{
									m_nSkippedReportsNoChange++;
									bSkipped = TRUE;
								}
							}
						}
					}

					if ( FALSE == bSkipped )
					{
						CSSFile fileMD5;
						if (fileMD5.Open(strFilename, "wb") == TRUE)
						{
							fileMD5.WriteLine(strMD5Hash);
						}
					}
				}

				if (FALSE == bSkipped)
				{
					CReportFileDoc ReportFileDoc(Super.ReportFilename());

					int nEmailFormat = m_Task.GetTaskFormatEmail();
					if ((0 == nEmailFormat) || (1 == nEmailFormat))
					{
						CString strReportID = "";
						strReportID.Format("%d", nReportType);

						int nEmailIdx = 1;
						{
							int nPos = 0;
							CSortedIntByString item;
							item.m_strItem = strReportID;
							if (arrayEmailByReport.Find(item, nPos) == TRUE)
							{
								arrayEmailByReport.GetAt(nPos, item);
								nEmailIdx = item.m_nItem;
							}
						}

						if ((nEmailIdx >= 1) && (nEmailIdx <= 5))
						{
							CString strEmailFilename;
							strEmailFilename.Format("\\%3.3d_%1.1d%3.3d%3.3d%3.3d%4.4d_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d.%s",
								nReportType,
								infoQuick.m_nQuickType,
								infoQuick.m_nDbNo,
								infoQuick.m_nNwkNo,
								infoQuick.m_nLocNo,
								infoQuick.m_nTermNo,
								timeNow.GetYear(),
								timeNow.GetMonth(),
								timeNow.GetDay(),
								timeNow.GetHour(),
								timeNow.GetMinute(),
								timeNow.GetSecond(),
								(1 == nEmailFormat) ? "htm" : "csv");

							if (0 == nEmailFormat)
							{
								ReportFileDoc.SaveAsCSV(m_strEmailFolder + strEmailFilename);
							}
							else
							{
								CTabRuler TabRuler;
								CReportHelpers ReportHelpers;
								CString strParamsFilename = pReport->GetParamsFilename();
								CString strParamsKey = "";
								ReportHelpers.CreateTabRuler(ReportFileDoc, strParamsFilename, strParamsKey, TabRuler);
								ReportFileDoc.SaveAsHTML(m_strEmailFolder + strEmailFilename, TabRuler);
							}

							CString strNKNFilename;
							strNKNFilename = strEmailFilename.Left(strEmailFilename.GetLength() - 3);
							strNKNFilename += "nkn";

							CSSFile fileNKN;
							if (fileNKN.Open(m_strEmailFolder + strNKNFilename, "wb") == TRUE)
							{
								fileNKN.WriteLine(m_Task.GetTaskEmailNickname(nEmailIdx - 1));
								fileNKN.WriteLine(strReportTitle);
								fileNKN.Close();
							}
						}
					}

					int nExportFormat = m_Task.GetTaskFormatExport();
					if ((0 == nExportFormat) || (1 == nExportFormat))
					{
						CString strFilenameBody = infoReport.GetExportFilename();
						::TrimSpaces(strFilenameBody, FALSE);

						if (strFilenameBody == "")
						{
							CEposReportSelect::GetDefaultExportFilename(nReportType, strFilenameBody);
						}

						CString strExportFilenameTarget = "";
						strExportFilenameTarget += m_Task.GetTaskExportFilename();
						strExportFilenameTarget += strFilenameBody;
						strExportFilenameTarget += m_Task.GetExportFilenameSuffix(m_dateTask, infoQuick);

						CString strSlash = "\\";
						CString strExportFilenamePending = "";
						strExportFilenamePending += strSlash;
						strExportFilenamePending += CSSMD5::CalcMD5(m_strExportFolderTarget + strSlash + strExportFilenameTarget);
						strExportFilenamePending += (1 == nExportFormat) ? ".htm" : ".csv";

						CString str001Filename;
						str001Filename = strExportFilenamePending.Left(strExportFilenamePending.GetLength() - 3);
						str001Filename += "001";

						CSSFile file001;
						if (file001.Open(m_strExportFolderPending + str001Filename, "wb") == TRUE)
						{
							file001.WriteLine(strExportFilenameTarget + strExportFilenamePending.Right(4));
							file001.WriteLine(m_Task.GetFilename(FALSE));
							file001.Close();
						}

						if (0 == nExportFormat)
						{
							ReportFileDoc.SaveAsCSV(m_strExportFolderPending + strExportFilenamePending);
						}
						else
						{
							CTabRuler TabRuler;
							CReportHelpers ReportHelpers;
							CString strParamsFilename = pReport->GetParamsFilename();
							CString strParamsKey = "";
							ReportHelpers.CreateTabRuler(ReportFileDoc, strParamsFilename, strParamsKey, TabRuler);
							ReportFileDoc.SaveAsHTML(m_strExportFolderPending + strExportFilenamePending, TabRuler);
						}
					}

					if (TRUE == m_Task.GetTaskAutoPrintFlag())
					{
						CString strPrintFilename = "";
						strPrintFilename.Format("%s\\%s%6.6d",
							(const char*)m_strPrintFolder,
							(const char*)m_strPrintFilenamePrefix,
							m_nNextPrintFileNumber++);

						{
							CSSFile fileHelper;
							if (fileHelper.Open(strPrintFilename + ".dat", "wb") == TRUE)
							{
								fileHelper.WriteLine(pReport->GetParamsFilename());
								fileHelper.WriteLine("");
							}
						}

						::CopyFile(Super.ReportFilename(), strPrintFilename + ".rep", FALSE);
					}
				}
			}
		}
	}

	if (NULL != pReport)
	{
		delete pReport;
	}

	return bResult;
}

/**********************************************************************/

bool CPosTrayReportCreatorEpos::CreateReportInternal( CEposReport* pReport, int nConLevel, CString& strTitle, CString& strSessionTitle, bool bFixStartDate, CPosTrayTaskConsolInfo& infoQuick )
{	
	COleDateTime dateTo = m_oleDateTo;
	COleDateTime dateFrom = m_oleDateFrom;
	
	if ( TRUE == bFixStartDate )
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();
		dateFrom = COleDateTime( 2016, 1, 1, 0, 0, 0 );
		dateTo = COleDateTime( timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay(), 0, 0, 0 );
	}
	else if ( pReport -> GetReportType() == REPORT_TYPE_DEPOSIT_BOOKING_EVENT )
	{
		dateTo = m_oleFutureDateTo;
	}

	if ( ( pReport -> GetReportType() == REPORT_TYPE_EODSTATUS ) && ( m_ReportSession.GetEODFlag() == TRUE ) )
	{
		CSessionCSVRecord DefaultSession;
		pReport -> SetDates ( dateFrom, dateTo, DefaultSession );
		pReport -> SetSessionTitle( "00:00 to 23:59" );
	}
	else
	{	
		pReport -> SetDates ( dateFrom, dateTo, m_ReportSession );
		pReport -> SetSessionTitle( strSessionTitle );
	}
		
	pReport -> SetConLevelTitle( strTitle );
	pReport -> SetItemServerFlag( m_Task.GetBatchItemServerFlag() );
	pReport -> SetConLevel( nConLevel );

	if ( FALSE == SysInfo.IsPMSSystem() )
	{
		pReport -> SetPMSPendingFlag( FALSE );
		pReport -> SetPMSActiveFlag( FALSE );
		pReport -> SetPMSCompleteFlag( FALSE );
		pReport -> SetPMSCancelledFlag( FALSE );
		pReport -> SetPMSEposFlag( TRUE );
		pReport -> SetPMSDateMode( PMS_DATEMODE_ACTUAL );
	}

#ifdef COMPILE_PMS

	if ( TRUE == SysInfo.IsPMSSystem() )
	{
		pReport -> SetPMSPendingFlag( m_Task.GetBatchPMSPendingFlag() );
		pReport -> SetPMSActiveFlag( m_Task.GetBatchPMSActiveFlag() );
		pReport -> SetPMSCompleteFlag( m_Task.GetBatchPMSCompleteFlag() );
		pReport -> SetPMSCancelledFlag( m_Task.GetBatchPMSCancelledFlag() );
		
		if ( PMSOptions.GetFixedTimeReportEnableFlag() == TRUE )
		{
			switch( m_Task.GetBatchPMSType() )
			{
			case 1:
				pReport -> SetPMSEposFlag( FALSE );
				pReport -> SetPMSDateMode( 1 );
				pReport -> SetPMSPendingFlag( FALSE );
				pReport -> SetPMSActiveFlag( FALSE );
				pReport -> SetPMSCompleteFlag( TRUE );
				pReport -> SetPMSCancelledFlag( FALSE );
				break;

			case 2:
				pReport -> SetPMSEposFlag( TRUE );
				pReport -> SetPMSDateMode( 0 );
				break;

			case 0:
			default:
				pReport -> SetPMSEposFlag( FALSE );
				pReport -> SetPMSDateMode( 0 );
				break;
			}
		}
		else
		{
			pReport -> SetPMSEposFlag( 1 == m_Task.GetBatchPMSType() );
			pReport -> SetPMSDateMode( 0 );
		}
	}

#endif

	{
		pReport -> m_ReportFilters.PluFilter.SetConsolidationType( nConLevel );
		pReport -> m_ReportFilters.PluFilter.SetMasterFilterNo( m_nPluFilter );

		pReport -> m_ReportFilters.DeptFilter.SetConsolidationType( nConLevel );
		pReport -> m_ReportFilters.DeptFilter.SetDeptFilterNo( m_nDeptFilter  );
	}

	pReport -> SetSaleTimeFlag( m_bSaleTime );
	
	{
		int nTermDateType = m_Task.GetBatchTermDateType();
		if ((nTermDateType < 0) || (nTermDateType > 2))
		{
			nTermDateType = 0;
		}

		pReport->SetTermDateType(nTermDateType);
	}
	
	
	pReport -> Consolidate();
	
	bool bResult = pReport -> CreateReport();

	if ( pReport -> GetCashRSPVersionCheckerFailFlag() == TRUE )
		return FALSE;
	
	return bResult;
}

/**********************************************************************/
