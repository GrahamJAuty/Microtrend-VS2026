 /**********************************************************************/
#include "ClearHistoryEposSales.h"
#include "..\SPosEcrManager5\ClearHistoryPluDelivery.h"
#include "..\SPosStockManager5\ClearHistoryStockAudits.h"
#include "..\SPosStockManager5\ClearHistoryStockSales.h"
#include "..\SPosStockManager5\ClearHistoryStockRetryFiles.h"
#include "MessageLogger.h"
#include "NodeTypes.h"
#include "..\SPosStockManager5\StockOptionsIni.h"
/**********************************************************************/
#include "AutoExpireHistory.h"
/**********************************************************************/

CAutoExpireHistory::CAutoExpireHistory()
{
	Reset();	
}

/**********************************************************************/

void CAutoExpireHistory::Reset()
{
	m_dateMonthLastCheck = COleDateTime ( 1998, 1, 1, 0, 0, 0 );
}

/**********************************************************************/

void CAutoExpireHistory::Read()
{
	CFilenameUpdater FnUp ( SysFiles::AutoExpire );

	CSSFile fileDate;
	if ( fileDate.Open( FnUp.GetFilenameToUse(), "rb" ) == FALSE )
		return;
	
	CString strDate;
	fileDate.ReadString( strDate );

	if ( strDate.GetLength() == 6 )
	{
		int nYear = atoi( strDate.Left(4) );
		int nMonth = atoi( strDate.Right(2) );

		COleDateTime check = COleDateTime( nYear, nMonth, 1, 0, 0, 0 );
		if ( check.GetStatus() == COleDateTime::valid )
			m_dateMonthLastCheck = check;
	}
}

/**********************************************************************/

void CAutoExpireHistory::Write()
{
	CFilenameUpdater FnUp( SysFiles::AutoExpire );

	CSSFile fileDate;
	if ( fileDate.Open( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
	{
		CString strDate;
		strDate = m_dateMonthLastCheck.Format ( "%Y%m" );
		fileDate.WriteLine( strDate );
		fileDate.Close();
	}
}

/**********************************************************************/

void CAutoExpireHistory::ExpireReportsNow()
{
	COleDateTime dateNow = COleDateTime::GetCurrentTime();
	m_dateMonthStart = COleDateTime ( dateNow.GetYear(), dateNow.GetMonth(), 1, 0, 0, 0 );

	if ( m_dateMonthStart > m_dateMonthLastCheck )
	{
		CEposSelectArray SelectArray;
		SelectArray.SelectSystem();
		SelectArray.MakeList();

#ifdef STOCKMAN_SYSTEM
		CheckEposReports( SelectArray );
		CheckStockSalesHistory();
		CheckStockAudits();
		CheckStockRetryFiles();
#else	
		CheckEposReports( SelectArray );
		CheckPluDeliveryAudit();
#endif
		m_dateMonthLastCheck = m_dateMonthStart;
		Write();
	}
}

/**********************************************************************/

void CAutoExpireHistory::GetDeletionYearAndMonth( int nMonthsToKeep, int& nYear, int& nMonth )
{
	nYear = m_dateMonthStart.GetYear() - ( nMonthsToKeep / 12 );
	nMonth = m_dateMonthStart.GetMonth() - ( nMonthsToKeep % 12 );

	if ( nMonth < 1 )
	{
		nMonth += 12;
		nYear -= 1;
	}
}

/**********************************************************************/

void CAutoExpireHistory::LogDeletion( const char* szMsg, int nYear, int nMonth )
{
	CString strLine;
	strLine.Format ( "Auto delete %s 01/%2.2d/%4.4d",
		szMsg, nMonth, nYear );

	CMessageLoggerInfo info;
	info.m_bAddDate = TRUE;
	info.m_bAddTimeOfDay = TRUE;
	info.m_bAddNetworkName = FALSE;
	info.m_nHideLevel = 2;
	info.m_strMessage = strLine;

	MessageLogger.LogSchedulerMessage( info, FALSE, TRUE, FALSE );
}

/**********************************************************************/

void CAutoExpireHistory::CheckEposReports( CEposSelectArray& SelectArray )
{
	int nMonthsToKeep1 = EcrmanOptions.GetAutoExpireTimePluReports();
	int nMonthsToKeep2 = EcrmanOptions.GetAutoExpireTimeSummaryReports();
	int nMonthsToKeep3 = EcrmanOptions.GetAutoExpireTimeDetailReports();

	CLocationSelectorEntity LocSelEntity;
	LocSelEntity.SetSystemNode();

	if ( ( nMonthsToKeep1 > 0 ) && ( nMonthsToKeep1 == nMonthsToKeep2 ) && ( nMonthsToKeep1 == nMonthsToKeep3 ) )
	{
		int nYear, nMonth;
		GetDeletionYearAndMonth( nMonthsToKeep1, nYear, nMonth );

		LogDeletion ( "All Epos Reports", nYear, nMonth ); 

		COleDateTime dateTo = COleDateTime( nYear, nMonth, 1, 0, 0, 0 );
		CClearHistoryEposSales( dateTo, LocSelEntity, SelectArray, TRUE, TRUE, TRUE, FALSE );
	}
	else
	{
		if ( nMonthsToKeep1 > 0 ) 
		{
			int nYear, nMonth;
			GetDeletionYearAndMonth( nMonthsToKeep1, nYear, nMonth );

			LogDeletion ( "Epos PLU Reports", nYear, nMonth );

			COleDateTime dateTo = COleDateTime( nYear, nMonth, 1, 0, 0, 0 );
			CClearHistoryEposSales( dateTo, LocSelEntity, SelectArray, TRUE, FALSE, FALSE, FALSE );
		}

		if ( nMonthsToKeep2 > 0 ) 
		{
			int nYear, nMonth;
			GetDeletionYearAndMonth( nMonthsToKeep2, nYear, nMonth );	

			LogDeletion ( "Epos Transaction Summary Reports", nYear, nMonth ); 

			COleDateTime dateTo = COleDateTime( nYear, nMonth, 1, 0, 0, 0 );
			CClearHistoryEposSales( dateTo, LocSelEntity, SelectArray, FALSE, TRUE, FALSE, FALSE );
		}

		if ( nMonthsToKeep3 > 0 ) 
		{
			int nYear, nMonth;
			GetDeletionYearAndMonth( nMonthsToKeep3, nYear, nMonth );

			LogDeletion ( "Epos Transaction Detail Reports", nYear, nMonth );

			COleDateTime dateTo = COleDateTime( nYear, nMonth, 1, 0, 0, 0 );
			CClearHistoryEposSales( dateTo, LocSelEntity, SelectArray, FALSE, FALSE, TRUE, FALSE );
		}
	}
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CAutoExpireHistory::CheckStockSalesHistory()
{
	int nMonthsToKeep = StockOptions.GetAutoExpireTimeSalesHistory();
	if ( nMonthsToKeep <= 0 ) 
		return;

	int nYear, nMonth;
	GetDeletionYearAndMonth( nMonthsToKeep, nYear, nMonth );

	CLocationSelectorEntity LocSelEntity;
	LocSelEntity.SetSystemNode();

	LogDeletion ( "Stock Sales History", nYear, nMonth ); 
	CClearHistoryStockSales( nYear, nMonth, 1, LocSelEntity );
}

/**********************************************************************/

void CAutoExpireHistory::CheckStockAudits()
{
	int nMonthsToKeep = StockOptions.GetAutoExpireTimeStockAudit();
	if ( nMonthsToKeep <= 0 ) 
		return;

	int nYear, nMonth;
	GetDeletionYearAndMonth( nMonthsToKeep, nYear, nMonth );

	CLocationSelectorEntity LocSelEntity;
	LocSelEntity.SetSystemNode();

	LogDeletion ( "Stock Audits", nYear, nMonth ); 
	CClearHistoryStockAudits( nYear, nMonth, 1, LocSelEntity );
}

/**********************************************************************/

void CAutoExpireHistory::CheckStockRetryFiles()
{
	int nMonthsToKeep = StockOptions.GetAutoExpireTimeSalesRetry();
	if ( nMonthsToKeep <= 0 ) 
		return;

	int nYear, nMonth;
	GetDeletionYearAndMonth( nMonthsToKeep, nYear, nMonth );

	CLocationSelectorEntity LocSelEntity;
	LocSelEntity.SetSystemNode();

	LogDeletion ( "Stock Retry Files", nYear, nMonth ); 
	CClearHistoryStockRetryFiles( nYear, nMonth, 1, LocSelEntity );
}

/**********************************************************************/
#else
/**********************************************************************/

void CAutoExpireHistory::CheckPluDeliveryAudit()
{
	if ( ( EcrmanOptions.GetFeaturesPluAuditFlag() == FALSE ) || ( EcrmanOptions.GetFeaturesPluStockFlag() == FALSE ) )
		return;

	int nMonthsToKeep = EcrmanOptions.GetAutoExpireTimePluDeliveryAudit();
	if ( nMonthsToKeep <= 0 ) 
		return;

	int nYear, nMonth;
	GetDeletionYearAndMonth( nMonthsToKeep, nYear, nMonth );

	CLocationSelectorEntity LocSelEntity;
	LocSelEntity.SetSystemNode();

	LogDeletion ( "Plu Delivery Audit", nYear, nMonth ); 
	CClearHistoryPluDelivery( nYear, nMonth, 1, LocSelEntity );
}

/**********************************************************************/
#endif
/**********************************************************************/
