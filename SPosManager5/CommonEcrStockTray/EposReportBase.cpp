/**********************************************************************/
#include "CashRSPImporter.h"
#include "LocationCSVArray.h"
#include "TerminalDateRangeListBuilder.h"
#include "NodeTypes.h"
#include "PMSOptions.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "EposReportBase.h"
/**********************************************************************/

static const char* RepText[] =
{
	"PLU Sales",							/* report 001 */
	"PLU Sales by Dept",					/* report 002 */
	"PLU Sales by Server",					/* report 003 */
	"PLU Sales by Modifier",				/* report 004 */
	"Department Sales Summary",				/* report 005 */
	"Hourly Sales Summary",					/* report 006 */
	"Server Sales Summary",					/* report 007 */
	"Payment Summary",						/* report 008 */
	"Payments by Server",					/* report 009 */
	"Covers Hourly Summary",				/* report 010 */
	"Voids by Server",						/* report 011 */
	"Refunds by Server",					/* report 012 */
	"Transaction Log",						/* report 013 */
	"Discounts by Server",					/* report 014 */
	"Payments by Dept Group (Estimated)",	/* report 015 */
	"Covers Group Summary",					/* report 016 */
	"No Sales by Server",					/* report 017 */
	"EOD Void List",						/* report 018 */
	"EOD Refund List",						/* report 019 */
	"EOD VIP Sales List",					/* report 020 */
	"VIP Sales",							/* report 021 */
	"VIP Sales by Dept",					/* report 022 */
	"VIP Sales by Server",					/* report 023 */
	"VIP Sales by Modifier",				/* report 024 */
	"VIP Department Sales Summary",			/* report 025 */
	"Guest Account",						/* report 026 */
	"General Summary by Dept",				/* report 027 */
	"Weekday Summary by Dept",				/* report 028 */
	"Payments by Dept (Estimated)",			/* report 029 */
	"General Summary by Group",				/* report 030 */
	"Weekday Summary by Group",				/* report 031 */
	"PLU Wastage",							/* report 032 */
	"PLU Wastage by Dept",					/* report 033 */
	"PLU Wastage by Server",				/* report 034 */
	"PLU Wastage by Modifier",				/* report 035 */
	"Department Wastage Summary",			/* report 036 */
	"Rebuild Sales History",				/* report 037 */
	"Transaction Log Discrepancy",			/* report 038 */
	"Age Verification by Server",			/* report 039 */
	"Deposits Purchased",					/* report 040 */
	"Deposits Redeemed",					/* report 041 */
	"Deposits Unredeemed",					/* report 042 */
	"Subtotal Discount Item Detail",		/* report 043 : OBSOLETE */
	"Subtotal Discount Group Summary",		/* report 044 : OBSOLETE */
	"Subtotal Discount Item Summary",		/* report 045 : OBSOLETE */
	"Customer Payments (RA)",				/* report 046 */
	"Customer Post to Account",				/* report 047 */
	"Customer Activity",					/* report 048 */
	"Customer Balance",						/* report 049 */
	"New Report for V3.06",					/* report 050 : OBSOLETE */
	"PLU Sales by Customer",				/* report 051 */
	"Payments by Item (Estimated)",			/* report 052 */
	"PLU Sales by Item Server",				/* report 053 */
	"PLU Wastage by Item Server",			/* report 054 */
	"Item Server Sales Summary",			/* report 055 */
	"Refunds by Item Server",				/* report 056 */
	"Voids by Item Server",					/* report 057 */
	"Discounts by Item Server",				/* report 058 */
	"VIP Sales by Item Server",				/* report 059 */
	"Covers Count by Month",				/* report 060 */
	"Promotion Summary",					/* report 061 */
	"Cheapest One Free Summary",			/* report 062 */
	"Cheapest One Free Detail",				/* report 063 */
	"Voids by Server with Reasons",			/* report 064 */
	"Voids by Item Server with Reasons",	/* report 065 */
	"Refunds by Server with Reasons",		/* report 066 */
	"Refunds by Item Server with Reasons",	/* report 067 */
	"Customer Statement",					/* report 068 */
	"General Activity by Dept",				/* report 069 */
	"General Activity by Group",			/* report 070 */
	"Weekday Activity by Dept",				/* report 071 */
	"Weekday Activity by Group",			/* report 072 */
	"Mix Match Usage by Item",				/* report 073 */
	"Mix Match Usage by Dept",				/* report 074 */
	"Mix Match Usage by Group",				/* report 075 */
	"%T Summary by Tax Band",				/* report 076 */
	"%T Summary by Price Level",			/* report 077 */
	"PLU Sales by %T Band",					/* report 078 */
	"Sports Booker Cascade",				/* report 079 */
	"Payment Detail",						/* report 080 */
	"Covers by Payment Method",				/* report 081 */
	"PLU Sales (no zero skip)",				/* report 082 */
	"Payments by Report Group",				/* report 083 */
	"Bookings by Deposit Date",				/* report 085 */
	"EOD Promotion List",					/* report 085 */
	"EOD Wastage List",						/* report 086 */
	"Discounts by Server with Reasons",		/* report 087 */
	"Discounts by Item Server with Reasons",/* report 088 */
	"Wastage by Server with Reasons",		/* report 089 */
	"Wastage by Item Server with Reasons",	/* report 090 */
	"VIP Sales by Server with Reasons",		/* report 091 */
	"VIP Sales by Item Server with Reasons",/* report 092 */
	"Bookings by Event Date",				/* report 093 */
	"Covers Hourly Group Summary",			/* report 094 */
	"PLU Sales by Server with Covers",		/* report 095 */
	"PLU Sales by Item Server with Covers",	/* report 096 */
	"New Discount",							/* report 097 */
	"Covers by Price Level (Estimated)",	/* report 098 */
	"Location EOD Status",					/* report 099 */
	"Covers Daily Summary",					/* report 100 */
	"Covers Daily Group Summary",			/* report 101 */
	"Hourly Group Summary",					/* report 102 */
	"Daily Group Summary",					/* report 103 */
	"Daily Sales Summary",					/* report 104 */
	"Covers Department Summary",			/* report 105 */
	"Covers Hourly Department Summary",		/* report 106 */
	"Covers Daily Department Summary",		/* report 107 */
	"Hourly Department Summary",			/* report 108 */
	"Daily Department Summary",				/* report 109 */
	"Mix Match Usage by Item with Covers",	/* report 110 */
	"Mix Match Usage by Dept with Covers",	/* report 111 */
	"Mix Match Usage by Group with Covers",	/* report 112 */
	"Payments by %T Band (Estimated)",		/* report 113 */
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolTermFile::CEposReportConsolTermFile()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolTermFile::Reset()
{
	m_nSelArrayIdx = 0;
	m_nTNo = 0;
	m_nFileType = 0;
	m_strDateFolder = "";
}

/**********************************************************************/

void CEposReportConsolTermFile::Add (CEposReportConsolTermFile& source )
{
}

/**********************************************************************/

int CEposReportConsolTermFile::Compare( CEposReportConsolTermFile& source, int nHint )
{
	if ( m_nSelArrayIdx != source.m_nSelArrayIdx )
		return ( ( m_nSelArrayIdx > source.m_nSelArrayIdx ) ? 1 : -1 );

	if ( m_nTNo != source.m_nTNo )
		return ( ( m_nTNo > source.m_nTNo ) ? 1 : -1 );

	if ( m_nFileType != source.m_nFileType )
		return ( ( m_nFileType > source.m_nFileType ) ? 1 : -1 );

	if ( m_strDateFolder != source.m_strDateFolder )
		return ( ( m_strDateFolder > source.m_strDateFolder ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
CString CEposReport::m_strReportNameTemp;
/**********************************************************************/

CEposReport::CEposReport( CEposSelectArray& SelectArray ) : m_SelectArray ( SelectArray ) 
{
	m_nBlankColumnsBeforeDate = 0;
	
	m_bEODMode = FALSE;

	m_nConLevel = NODE_SYSTEM;
	m_strConLevelTitle = "";
	m_strSessionTitle = "";

	m_ReportFilters.PluFilter.SetMasterFilterNo(0);

	m_bShowCreationTime = FALSE;
	
	m_bItemServer = FALSE;
	m_bSaleTime = TRUE;
	
	SetPMSPendingFlag( FALSE );
	SetPMSActiveFlag( FALSE );
	SetPMSCompleteFlag( FALSE );
	SetPMSCancelledFlag( FALSE );
	SetPMSEposFlag( TRUE );
	SetPMSAllowMode( PMS_ALLOWMODE_ALL );
	SetPMSDateMode( PMS_DATEMODE_ACTUAL );
	SetPMSDepositsFlag( FALSE );

	m_nCreateReportTarget = 0;
	m_nCreateReportCount = 0;
	m_nCreateReportMiniTarget = 0;
	m_nCreateReportMiniCount = 0;

	m_bGraphMode = FALSE;
	m_nTermDateType = 0;

	m_CashRSPVersionChecker.Reset();
	
	switch( DealerFlags.GetServerConsolidationType() )
	{
	case SERVER_SORT_BYCODE:
		m_nServerReportType = SERVER_REPORT_CODE;
		break;

	case SERVER_SORT_BYNAME:
		m_nServerReportType = SERVER_REPORT_NAME;
		break;

	case SERVER_SORT_BYFILE:
	default:
		switch( EcrmanOptions.GetFeaturesServerSettingsType() )
		{
		case SERVER_LOC:
			m_nServerReportType = SERVER_REPORT_LOC;
			break;

		case SERVER_DB:
			m_nServerReportType = SERVER_REPORT_DB;
			break;

		case SERVER_SYS:
		case SERVER_NONE:
		default:
			m_nServerReportType = SERVER_REPORT_SYS;
			break;
		}
	}
}

/**********************************************************************/

CEposReport::~CEposReport()
{
}

/**********************************************************************/

void CEposReport::AddReportColumn(const char* szName, UINT nAlign, int nWidth, bool bTotal)
{
	m_ReportFile.AddColumn(szName, nAlign, nWidth);

	if (TRUE == bTotal)
	{
		m_UnderlineMaker.AddTotals(1);
	}
	else
	{
		m_UnderlineMaker.AddBlanks(1);
	}
}

/**********************************************************************/

void CEposReport::SetEposReportSelectInfo( CEposReportSelectInfo& info )
{
	m_EposReportSelectInfo = info;
}

/**********************************************************************/

const char* CEposReport::GetReportNameExternal ( int nReportType )
{
	if ((nReportType > 0) && (nReportType <= sizeof(RepText) / sizeof(const char*)))
	{
		m_strReportNameTemp = RepText[nReportType - 1];
	}
	else
	{
		m_strReportNameTemp = "Unknown Report";
	}

	m_strReportNameTemp = EcrmanOptions.GetTaxString( m_strReportNameTemp );

	return m_strReportNameTemp;
}

/**********************************************************************/

const char* CEposReport::GetReportNameInternal ( int nReportType )
{
	if ( m_EposReportSelectInfo.GetReportType() >= EPOS_CUSTOM_FIRST_REPORT )
		m_strReportName = m_EposReportSelectInfo.GetCustomReportName();
	else
		m_strReportName = GetReportNameExternal( nReportType );

	return m_strReportName;
}

/**********************************************************************/

void CEposReport::SetDates ( COleDateTime oleDateFrom, COleDateTime oleDateTo, CSessionCSVRecord& ReportSession )
{
	DataManagerNonDb.SessionDateTimeFilter.SetDates( oleDateFrom, oleDateTo, ReportSession );
	m_bEODMode = ReportSession.GetEODFlag();
}

/**********************************************************************/

bool CEposReport::SimpleTimeCheck( int nLocIdx, CString& strDate, CString& strTime )
{
	bool bResult = FALSE;

	if ( FALSE == m_bEODMode )
	{
		bResult = ( DataManagerNonDb.SessionDateTimeFilter.CheckTime( strDate, strTime  ) != 0 );
	}
	else
	{
		CString strDateTime = strDate + strTime;
		bResult = DataManagerNonDb.EODDateTimeFilterArray.CheckTime( nLocIdx, strDateTime );
	}

	return bResult;
}

/**********************************************************************/

const char* CEposReport::GetReportTitle( bool bForGraph )
{
	CSortedDateTimeItem DateFrom = DataManagerNonDb.SessionDateTimeFilter.GetDateFrom();
	CSortedDateTimeItem DateTo = DataManagerNonDb.SessionDateTimeFilter.GetDateTo();

	CString strType = "";
	if ( bForGraph == FALSE )
	{
		strType = GetReportNameInternal( m_nReportType );
		strType += " ";
	}

	m_strTitle.Format ( "%s (%s, %2.2d/%2.2d/%4.4d - %2.2d/%2.2d/%4.4d, %s)",
		(const char*) strType,
		(const char*) m_strConLevelTitle,
		DateFrom.m_oleDateTime.GetDay(),
		DateFrom.m_oleDateTime.GetMonth(),
		DateFrom.m_oleDateTime.GetYear(),
		DateTo.m_oleDateTime.GetDay(),
		DateTo.m_oleDateTime.GetMonth(),
		DateTo.m_oleDateTime.GetYear(),
		(const char*) m_strSessionTitle );
			
	return m_strTitle;
}

/**********************************************************************/

const char* CEposReport::GetParamsFilename()
{
	if ( m_nReportType < EPOS_CUSTOM_FIRST_REPORT )
	{
		CFilenameUpdater FnUp( SysFiles::EcrReportPrm, 1, m_nReportType );
		m_strParamsFilename = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::EcrCustomReportPrm, 1, m_nReportType + 1 - EPOS_CUSTOM_FIRST_REPORT );
		m_strParamsFilename = FnUp.GetFilenameToUse();
	}
	return m_strParamsFilename;
}

/**********************************************************************/

void CEposReport::FormatCSVTime ( const char* szTime, CString& strTime )
{
	CString strCSVTime ( szTime );		/* convert HHMM to HH:MM */

	strTime.Format ( "%s:%s",
		(const char*) strCSVTime.Left(2),
		(const char*) strCSVTime.Right(2) );
}

/**********************************************************************/

void CEposReport::ShowEposDetails()
{
	m_ReportFile.RequestBlankLines(2);

	CTerminalDateRangeListBuilder TerminalList(m_SelectArray, m_bEODMode, m_nTermDateType, m_PMSModes.GetEposFlag(), m_PMSModes.GetAllowMode(), m_PMSModes.GetDateMode());
	TerminalList.CreateTerminalList();

	for (int n = 0; n < TerminalList.GetLineCount(); n++)
	{
		m_ReportFile.WriteReportMiscLine(TerminalList.GetLine(n));
	}

	CString strGuestAccountStatus = TerminalList.GetGuestAccountStatus();

	if (strGuestAccountStatus != "")
	{
		m_ReportFile.RequestBlankLines(1);
		m_ReportFile.WriteReportMiscLine(strGuestAccountStatus);
	}

	if (TRUE == m_bShowCreationTime)
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		CString strCreationTime;
		strCreationTime.Format("<..>Report created at %2.2d:%2.2d:%2.2d on %2.2d/%2.2d/%4.4d",
			timeNow.GetHour(),
			timeNow.GetMinute(),
			timeNow.GetSecond(),
			timeNow.GetDay(),
			timeNow.GetMonth(),
			timeNow.GetYear());

		m_ReportFile.RequestBlankLines(1);
		m_ReportFile.WriteReportMiscLine(strCreationTime);
	}

	m_ReportFile.PreventEmptyFile();
}

/**********************************************************************/

const char* CEposReport::GetServerHeader( int nServerLoc, int nServerNo, int nForceDbIdx, bool bFullLine )
{
	m_strServerHeader = "";

	CString strDatabase = "";
	if ( NODE_SYSTEM == m_nConLevel )
	{
		if ( nForceDbIdx != -1 )
		{
			strDatabase = dbDatabase.GetName( nForceDbIdx );
			strDatabase += ", ";
		}
	}

	switch( m_nServerReportType )
	{
	case SERVER_REPORT_CODE:
		m_strServerHeader.Format( "%sServer %4.4d", 
			(const char*) strDatabase, 
			nServerNo );
		break;

	case SERVER_REPORT_NAME:
		m_strServerHeader.Format( "%sServer %4.4d, %s",
			(const char*) strDatabase,
			nServerNo,
			DataManagerNonDb.ServerNameTable.GetSortedName( nServerLoc ) );
		break;

	case SERVER_REPORT_DB:
		{
			int nDbNo = dbDatabase.GetDbNo( nServerLoc );
				
			CString str = "";
			switch( m_nConLevel )
			{
			case NODE_SYSTEM:
			case NODE_DATABASE:
				str = dbDatabase.GetName( nServerLoc );	
				break;

			default:
				str = m_strConLevelTitle;
				break;
			}
			
			CServerSelectionInfo info;
			DataManagerNonDb.ServerNameTable.GetServerStatus( info, nDbNo, 0, 0, nServerNo );

			m_strServerHeader.Format( "%s, Server %4.4d, %s",
				(const char*) str,
				nServerNo,
				(const char*) info.m_strServerName );
		}
		break;

	case SERVER_REPORT_LOC:
		{
			int nDbNo = dbLocation.GetDbNo( nServerLoc );
			int nNwkNo = dbLocation.GetNetworkNo( nServerLoc );
			int nLocNo = dbLocation.GetLocNo( nServerLoc );
			
			CString str = "";
			switch( m_nConLevel )
			{
			case NODE_LOCATION_TERMINAL:
				str = m_strConLevelTitle;
				break;

			default:
				str = dbLocation.GetName( nServerLoc );	
				break;
			}
		
			CServerSelectionInfo info;
			DataManagerNonDb.ServerNameTable.GetServerStatus( info, nDbNo, nNwkNo, nLocNo, nServerNo );

			m_strServerHeader.Format( "%s, Server %4.4d, %s",
				(const char*) str,
				nServerNo,
				(const char*) info.m_strServerName );
		}
		break;

	case SERVER_REPORT_SYS:
	default:
		{
			CServerSelectionInfo info;
			DataManagerNonDb.ServerNameTable.GetServerStatus( info, 0, 0, 0, nServerNo );

			m_strServerHeader.Format( "%s Server %4.4d, %s",
				(const char*) strDatabase,
				nServerNo,
				(const char*) info.m_strServerName );
		}
		break;
	}

	if ( ( TRUE == bFullLine ) && ( m_strServerHeader != "" ) )
	{
		m_strServerHeader.Insert( 0, "<..>" );
	}

	return m_strServerHeader;
}

/**********************************************************************/

const char* CEposReport::GetServerColumnName( int nServerLoc, int nServerNo )
{
	m_strServerHeader = "";

	switch( m_nServerReportType )
	{
	case SERVER_REPORT_CODE:
		m_strServerHeader.Format( "Server %4.4d", 
			nServerNo );
		break;

	case SERVER_REPORT_NAME:
		m_strServerHeader.Format( "%4.4d, %s",
			nServerNo,
			DataManagerNonDb.ServerNameTable.GetSortedName( nServerLoc ) );
		break;

	case SERVER_REPORT_DB:
		{
			int nDbNo = dbDatabase.GetDbNo( nServerLoc );
				
			CServerSelectionInfo info;
			DataManagerNonDb.ServerNameTable.GetServerStatus( info, nDbNo, 0, 0, nServerNo );

			m_strServerHeader.Format( "%4.4d, %s",
				nServerNo,
				(const char*) info.m_strServerName );
		}
		break;

	case SERVER_REPORT_LOC:
		{
			int nDbNo = dbLocation.GetDbNo( nServerLoc );
			int nNwkNo = dbLocation.GetNetworkNo( nServerLoc );
			int nLocNo = dbLocation.GetLocNo( nServerLoc );
			
			CServerSelectionInfo info;
			DataManagerNonDb.ServerNameTable.GetServerStatus( info, nDbNo, nNwkNo, nLocNo, nServerNo );

			m_strServerHeader.Format( "%4.4d, %s",
				nServerNo,
				(const char*) info.m_strServerName );
		}
		break;

	case SERVER_REPORT_SYS:
	default:
		{
			CServerSelectionInfo info;
			DataManagerNonDb.ServerNameTable.GetServerStatus( info, 0, 0, 0, nServerNo );

			m_strServerHeader.Format( "%4.4d, %s",
				nServerNo,
				(const char*) info.m_strServerName );
		}
		break;
	}

	return m_strServerHeader;
}

/**********************************************************************/

bool CEposReport::IsDepartmentPaymentReport()
{
	switch( m_nReportType )
	{
	case REPORT_TYPE_GENERAL_DEPT:
	case REPORT_TYPE_GENERAL_DEPT_QTY:
	case REPORT_TYPE_WEEKLY_DEPT:
	case REPORT_TYPE_WEEKLY_DEPT_QTY:
	case REPORT_TYPE_PAYMENT_ESTIMATE_DEPT:
		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CEposReport::IsWastageReport()
{
	switch( m_nReportType )
	{
	case REPORT_TYPE_PLUWASTE:
	case REPORT_TYPE_PLUWASTE_DEPT_ITEM:
	case REPORT_TYPE_PLUWASTE_TRANSERVER:
	case REPORT_TYPE_PLUWASTE_ITEMSERVER:
	case REPORT_TYPE_PLUWASTE_MODIFIER:
	case REPORT_TYPE_PLUWASTE_DEPT_SUMMARY:
		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CEposReport::ValidateAccountDate( int nLocIdx, CString& strDate, CString& strTime, COleDateTime& timeTransaction, COleDateTime& dateTransaction )
{
	bool bResult = FALSE;

	if ( SimpleTimeCheck ( nLocIdx, strDate, strTime ) == TRUE )
	{
		CString strDateTime = strDate + strTime;
		if ( GetOleDateTimeFromString( strDateTime, timeTransaction ) == TRUE )
		{
			dateTransaction = COleDateTime(
				timeTransaction.GetYear(),
				timeTransaction.GetMonth(),
				timeTransaction.GetDay(),
				0, 0, 0 );

			bResult = TRUE;
		}
	}
		
	return bResult;
}

/**********************************************************************/

__int64 CEposReport::GetSeparatedPromoPluNo( __int64 nPromoNo )
{
	if ( ( nPromoNo < 1 ) || ( nPromoNo > 999 ) )
		nPromoNo = 0;

	__int64 nPluNo = 99000000000000 + nPromoNo;

	return nPluNo;
}

/**********************************************************************/

__int64 CEposReport::GetSeparatedMixMatchPluNo( __int64 nMixMatchNo )
{
	if ( ( nMixMatchNo < 1 ) || ( nMixMatchNo > 999 ) )
		nMixMatchNo = 0;

	__int64 nPluNo = 98000000000000 + nMixMatchNo;

	return nPluNo;
}

/**********************************************************************/

int CEposReport::GetDatabaseTaxBand( CPluSalesLine& csv, int nLineType )
{
	__int64 nPluNo = 0;
	int nMod = 0;

	if ( CSVPLUDATA_PROMO_TAXABLE == nLineType )
	{
		nPluNo = GetSeparatedPromoPluNo( csv.GetPromoNoSeparate() );
	}
	else
	{
		CSQLPluNoInfo infoPluNo;
		infoPluNo.m_nEposPluNo = csv.GetPluNoNew();
		::ProcessPluNo( infoPluNo, FALSE, TRUE );
		nPluNo = infoPluNo.m_nBasePluNo;
		nMod = infoPluNo.m_nModifier;
	}

	int nTaxBand = 0;

	int nPluIdx;
	if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE )
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( nPluIdx, PluRecord );
		nTaxBand = PluRecord.GetModifierTaxBandIntForReport(nMod);
	}
	
	return nTaxBand;
}

/**********************************************************************/

void CEposReport::SetPreferConsolType( int n )
{
	switch(n)
	{
	case EPOSREPORT_CONSOL_DEFAULT:
	case EPOSREPORT_CONSOL_SAVED:
	case EPOSREPORT_CONSOL_ADHOC:
	case EPOSREPORT_CONSOL_SYSTEM:
		m_nPreferConsolType = n;
		break;
	}
}

/**********************************************************************/

void CEposReport::UpdateCreateReportProgress()
{
	if ( m_nCreateReportCount < m_nCreateReportTarget )
		m_nCreateReportCount++;

	m_nCreateReportMiniCount++;
	if ( m_nCreateReportMiniCount >= m_nCreateReportMiniTarget )
	{
		StatusProgress.SetPos( m_nCreateReportCount, m_nCreateReportTarget );
		m_nCreateReportMiniCount = 0;
	}
}

/**********************************************************************/

bool CEposReport::CheckPluSaleDateTime( bool bIsPMSLocation, CTermFileListInfo& infoFile, CPluSalesLine& csv, COleDateTime& oleDatePMS, CString strThisSaleDate, CString& strThisSaleTime )
{
	bool bAccept = TRUE;

	//NORMAL CASHRSP SALES TAKES DATE FROM FILE AND TIME FROM CSV LINE
	if ( FALSE == bIsPMSLocation )
	{
		if ( TRUE == m_bSaleTime )
			strThisSaleTime = csv.GetSaleTime();
		else
			strThisSaleTime = csv.GetTranTime();
		
		bAccept = SimpleTimeCheck( infoFile.m_nLocIdx, strThisSaleDate, strThisSaleTime );
	}
	//PMS ACTUAL TIME MODE TAKES DATE AND TIME FROM LATEST #PMS_TIME LINE
	else if ( m_PMSModes.GetDateMode() == PMS_DATEMODE_ACTUAL )
	{
		//FOR CONSISTENCY WITH OTHER REPORTING MODES, MARK DATES AS HAVING SALES WHEN
		//WE FIND A SALE WITH THAT CALENDAR DATE, EVEN IF IT DOES NOT MATCH THE TIME
		//RANGE OR OTHER FILTERS
		m_SelectArray.ProcessStartDate( infoFile.m_nSelectArrayIdx, oleDatePMS );
		m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, oleDatePMS );

		bAccept = ( DataManagerNonDb.SessionDateTimeFilter.CheckTime( strThisSaleDate, strThisSaleTime ) != 0 );
	}

	return bAccept;
}

/**********************************************************************/

bool CEposReport::CheckReasonInfoDateTime( CTermFileListInfo& infoFile, CReasonInfo& ReasonInfo, CString strThisSaleDate, CString& strThisSaleTime )
{
	if ( TRUE == m_bSaleTime )
		strThisSaleTime = ReasonInfo.GetTimeAction();
	else
		strThisSaleTime = ReasonInfo.GetTimeTran();
		
	return SimpleTimeCheck( infoFile.m_nLocIdx, strThisSaleDate, strThisSaleTime );
}

/**********************************************************************/

void CEposReport::LoadTimeSliceSettings( const char* szSettings, CEposReportCustomSettingsHourly& Settings, int& nTimeSliceType, CTimeMap& TimeMap )
{
	CString strParams = szSettings;
	Settings.SetSettingsCSVLine( strParams );

	nTimeSliceType = Settings.GetTimeSliceType();
	CString strEntityID = Settings.GetTimeSliceEntityID();
	
	if ( SH_CUSTOM == nTimeSliceType )
	{
		bool bFound = FALSE;
		for ( int n = 0; n < DataManagerNonDb.TimeSliceList.GetSize(); n++ )
		{
			if ( DataManagerNonDb.TimeSliceList.GetReportType(n) == SH_CUSTOM )
			{
				if ( strEntityID == DataManagerNonDb.TimeSliceList.GetEntityID(n) )
				{
					int nTimeSliceFileNo = DataManagerNonDb.TimeSliceList.GetTimeSliceFileNo(n);
					TimeMap.BuildMap( nTimeSliceFileNo );
					bFound = TRUE;
					break;
				}
			}
		}

		if ( FALSE == bFound )
			nTimeSliceType = SH_TIMESLICE_30MIN;
	}
}

/**********************************************************************/

const char* CEposReport::GetTimeSliceString( int nTimeSliceType, CTimeMap& TimeMap, const char* szTime )
{
	if ( SH_CUSTOM != nTimeSliceType )
	{
		FormatCSVTime ( szTime, m_strTimeSliceString );

		m_strTimeSliceString += " - ";
		m_strTimeSliceString += m_strTimeSliceString.Left(3);
		
		switch( nTimeSliceType )
		{
		case SH_TIMESLICE_10MIN:
			m_strTimeSliceString += m_strTimeSliceString.Mid(3,1);
			m_strTimeSliceString += "9";
			break;

		case SH_TIMESLICE_15MIN:
			
			switch( atoi(m_strTimeSliceString.Mid(3,2)) )
			{
			case 0:		m_strTimeSliceString += "14";	break;
			case 15:	m_strTimeSliceString += "29";	break;
			case 30:	m_strTimeSliceString += "44";	break;
			case 45:	
			default:	m_strTimeSliceString += "59";	break;
			}
			break;

		case SH_TIMESLICE_60MIN:
			m_strTimeSliceString += "59";
			break;

		case SH_TIMESLICE_30MIN:
		default:
			
			if ( m_strTimeSliceString.Mid(3,2) == "00" )
				m_strTimeSliceString += "29";
			else
				m_strTimeSliceString += "59";

			break;
		}
	}
	else
	{
		m_strTimeSliceString = TimeMap.GetSortOrderName( szTime );
	}

	return m_strTimeSliceString;
}

/**********************************************************************/

void CEposReport::GetTimeSliceTimeCode( CString& strSaleTime, CString& strSODLabel, int nTimeSliceType, CTimeMap& TimeMap, CString& strTimeCode, int& nDayShift )
{
	int nHour = atoi( strSaleTime.Left(2) );
	int nMinute = atoi( strSaleTime.Mid(2,2) );

	//ROUND TO TIME SLICE IF REQUIRED
	if ( SH_CUSTOM != nTimeSliceType )
	{
		switch( nTimeSliceType )
		{
		case SH_TIMESLICE_10MIN:
			nMinute -= ( nMinute % 10 );
			break;

		case SH_TIMESLICE_15MIN:
			nMinute -= ( nMinute % 15 );
			break;

		case SH_TIMESLICE_60MIN:
			nMinute = 0;
			break;

		case SH_TIMESLICE_30MIN:
		default:
			nMinute -= ( nMinute % 30 );
			break;
		}
	}

	if ( SH_CUSTOM == nTimeSliceType )
	{
		strTimeCode = TimeMap.GetSortOrder( nHour, nMinute );
		nDayShift = 0;
	}
	else
	{
		strTimeCode.Format( "%2.2d%2.2d", nHour, nMinute );
		nDayShift = ( strTimeCode < strSODLabel ) ? 1 : 0;
	}
}

/**********************************************************************/

const char* CEposReport::GetTotalTypeName( bool bFullLine, int nEntityType, bool bSystemToGrand, bool bTotal )
{
	m_strTotalTypeName = bFullLine ? "<..>" : "";

	switch( nEntityType )
	{
	case NODE_DATABASE:				m_strTotalTypeName += "Database";		break;
	case NODE_LOCATION:				m_strTotalTypeName += "Location";		break;
	case NODE_REPORTPOINT:			m_strTotalTypeName += "Report Point";	break;
	case NODE_LOCATION_TERMINAL:	m_strTotalTypeName += "Terminal";		break;
	case NODE_SYSTEM:				m_strTotalTypeName += bSystemToGrand ? "Grand" : "System"; break;
	default:						m_strTotalTypeName += "";				break;
	}

	if (TRUE == bTotal)
	{
		m_strTotalTypeName += " Total";
	}

	return m_strTotalTypeName;
}
	
/**********************************************************************/
