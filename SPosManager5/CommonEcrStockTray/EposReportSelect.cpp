 /**********************************************************************/
#include "ReportTypes.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportSelect.h"
/**********************************************************************/

CEposReportSelectInfo::CEposReportSelectInfo()
{
	Reset();
}

/**********************************************************************/

void CEposReportSelectInfo::Reset()
{
	m_nReportType = REPORT_TYPE_PLUSALES;
	
	m_bDefaultSelect = TRUE;
	m_bCurrentSelect = TRUE;
	m_nDefaultSortOrder = 9999;
	m_nCurrentSortOrder = 9999;
	m_strExportFilename = "";
	m_strConsolPrefsDefault = "";
	m_strConsolPrefsSaved = "";
	m_strConsolPrefsAdhoc = "";
	m_strConsolPrefsSummary = "";
	m_bSystemEnable = TRUE;
	
	m_nCustomReportSerialNo = 0;
	m_nCustomReportFamily = EPOS_CUSTOM_FAMILY_NONE;
	m_strCustomReportName = "";
	m_strCustomReportParams = "";
}

/**********************************************************************/

void CEposReportSelectInfo::Add (CEposReportSelectInfo& source )
{
}

/**********************************************************************/

int CEposReportSelectInfo::Compare( CEposReportSelectInfo& source, int nHint )
{
	if (m_nReportType != source.m_nReportType)
	{
		return ((m_nReportType > source.m_nReportType) ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/

void CEposReportSelectInfo::SetCustomReportFamily( int n )
{
	switch( n )
	{
	case EPOS_CUSTOM_FAMILY_NONE:
	case EPOS_CUSTOM_FAMILY_HOURLY:
	case EPOS_CUSTOM_FAMILY_HOURLYGRP:
	case EPOS_CUSTOM_FAMILY_HOURLYDPT:
	case EPOS_CUSTOM_FAMILY_COVERS_HOURLY:
	case EPOS_CUSTOM_FAMILY_COVERS_HOURLYGRP:
	case EPOS_CUSTOM_FAMILY_COVERS_HOURLYDPT:
	case EPOS_CUSTOM_FAMILY_PLU_PRICEBAND:
	case EPOS_CUSTOM_FAMILY_GENERAL:
	case EPOS_CUSTOM_FAMILY_TRANSACTION:
	case EPOS_CUSTOM_FAMILY_DEPOSIT:
	case EPOS_CUSTOM_FAMILY_MIXMATCH:
	case EPOS_CUSTOM_FAMILY_DISCOUNT:
	case EPOS_CUSTOM_FAMILY_LOYALTYRECON:
	case EPOS_CUSTOM_FAMILY_PAYMENTDETAIL:
		m_nCustomReportFamily = n;
		break;
	}
}

/**********************************************************************/

void CEposReportSelectInfo::SetReportType( int n )
{
	if ((n >= 0) && (n <= 9999))
	{
		m_nReportType = n;
	}
}

/**********************************************************************/

void CEposReportSelectInfo::SetConsolPrefsDefault( const char* sz )
{
	CString str = sz;
	if (str.GetLength() <= EPOS_MAXLEN_CONSOL_PREFS)
	{
		m_strConsolPrefsDefault = str;
	}
}

/**********************************************************************/

void CEposReportSelectInfo::SetConsolPrefsSaved( const char* sz )
{
	CString str = sz;
	if (str.GetLength() <= EPOS_MAXLEN_CONSOL_PREFS)
	{
		m_strConsolPrefsSaved = str;
	}
}

/**********************************************************************/

void CEposReportSelectInfo::SetConsolPrefsAdhoc( const char* sz )
{
	CString str = sz;
	if (str.GetLength() <= EPOS_MAXLEN_CONSOL_PREFS)
	{
		m_strConsolPrefsAdhoc = str;
	}
}

/**********************************************************************/

void CEposReportSelectInfo::SetConsolPrefsSummary( const char* sz )
{
	CString str = sz;
	if (str.GetLength() <= EPOS_MAXLEN_CONSOL_PREFS)
	{
		m_strConsolPrefsSummary = str;
	}
}

/**********************************************************************/

void CEposReportSelectInfo::SetCustomReportName( const char* sz )
{
	CString str = sz;
	if (str.GetLength() <= EPOS_CUSTOM_MAXLEN_NAME)
	{
		m_strCustomReportName = str;
	}
}

/**********************************************************************/

void CEposReportSelectInfo::SetCustomReportParams( const char* sz )
{
	CString str = sz;
	if (str.GetLength() <= EPOS_CUSTOM_MAXLEN_PARAMS)
	{
		m_strCustomReportParams = str;
	}
}

/**********************************************************************/

void CEposReportSelectInfo::SetExportFilename( const char* sz )
{
	CString str = sz;
	if (str.GetLength() <= EPOS_MAXLEN_EXPORT_FILENAME)
	{
		m_strExportFilename = str;
	}
}

/**********************************************************************/

void CEposReportSelectInfo::SetCurrentSortOrder( int n )
{
	if ((n >= 0) && (n <= 9999))
	{
		m_nCurrentSortOrder = n;
	}
}

/**********************************************************************/

void CEposReportSelectInfo::SetDefaultSortOrder( int n )
{
	if ((n >= 0) && (n <= 9999))
	{
		m_nDefaultSortOrder = n;
	}
}

/**********************************************************************/

bool CEposReportSelectInfo::LoadConsolPrefs()
{
	bool bResult = FALSE;

	if ( m_strConsolPrefsSaved == "" )
	{
		bResult = TRUE;

		CString strFilename = "";
		bool bCustomReport = FALSE;
		if ( m_nReportType >= EPOS_CUSTOM_FIRST_REPORT )
		{
			CFilenameUpdater FnUp( SysFiles::EcrReportConCust, 0, m_nReportType - EPOS_CUSTOM_FIRST_REPORT );
			strFilename = FnUp.GetFilenameToUse();
			bCustomReport = TRUE;
		}
		else
		{
			CFilenameUpdater FnUp( SysFiles::EcrReportConSys, 0, m_nReportType );
			strFilename = FnUp.GetFilenameToUse();
		}

		CSSFile file;
		CString strBuffer = "";
		if ( file.Open( strFilename, "rb" ) == TRUE )
		{
			file.ReadString( strBuffer );

			if ( TRUE == bCustomReport )
			{
				CCSV csv( strBuffer );

				if ( csv.GetSize() > 0 )
				{
					if ( csv.GetInt(0) == m_nCustomReportSerialNo )
					{
						csv.RemoveAt(0);
						strBuffer = csv.GetLine();
					}
					else
						strBuffer = "";
				}
			}
		}
						
		SetConsolPrefsSaved( strBuffer );
		SetConsolPrefsAdhoc( strBuffer );
	
		if ( m_strConsolPrefsSaved == "" )
		{
			SetConsolPrefsSaved( m_strConsolPrefsDefault );
			SetConsolPrefsAdhoc( m_strConsolPrefsDefault );
		}
	}

	return bResult;
}		

/**********************************************************************/

bool CEposReportSelectInfo::SaveConsolPrefs()
{
	bool bResult = FALSE;

	CString strFilename = "";
	bool bCustomReport = FALSE;
	if ( m_nReportType >= EPOS_CUSTOM_FIRST_REPORT )
	{
		CFilenameUpdater FnUp( SysFiles::EcrReportConCust, 0, m_nReportType - EPOS_CUSTOM_FIRST_REPORT );
		strFilename = FnUp.GetFilenameToUse();
		bCustomReport = TRUE;
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::EcrReportConSys, 0, m_nReportType );
		strFilename = FnUp.GetFilenameToUse();
	}

	CSSFile file;
	if ( file.Open( strFilename, "wb" ) == TRUE )
	{
		CString strPrefs = m_strConsolPrefsSaved;

		if ( TRUE == bCustomReport )
		{
			CCSV csv( strPrefs );
			csv.InsertAt( 0, m_nCustomReportSerialNo );
			strPrefs = csv.GetLine();
		}

		file.WriteLine( strPrefs );
		bResult = TRUE;
	}

	return bResult;
}		

/**********************************************************************/

void CEposReportSelectInfo::InitialiseCustomReportConsolPrefs()
{
	m_strConsolPrefsSaved = "";
	m_strConsolPrefsAdhoc = "";

	switch( m_nCustomReportFamily )
	{
	case EPOS_CUSTOM_FAMILY_GENERAL:
		m_strConsolPrefsDefault = "1,1,0,1,0";
		m_strConsolPrefsSummary = "1,0,0,0,1";
		break;

	case EPOS_CUSTOM_FAMILY_MIXMATCH:
	case EPOS_CUSTOM_FAMILY_DISCOUNT:
	case EPOS_CUSTOM_FAMILY_PAYMENTDETAIL:
	case EPOS_CUSTOM_FAMILY_PLU_PRICEBAND:
		m_strConsolPrefsDefault = "1,0,0,1";
		m_strConsolPrefsSummary = "1,0,0,1";
		break;

	case EPOS_CUSTOM_FAMILY_HOURLY:
	case EPOS_CUSTOM_FAMILY_HOURLYGRP:
	case EPOS_CUSTOM_FAMILY_HOURLYDPT:
	case EPOS_CUSTOM_FAMILY_COVERS_HOURLY:
	case EPOS_CUSTOM_FAMILY_COVERS_HOURLYGRP:
	case EPOS_CUSTOM_FAMILY_COVERS_HOURLYDPT:
		m_strConsolPrefsDefault = "1,0,0,1,1";
		m_strConsolPrefsSummary = "1,0,0,0,1";
		break;

	default:
		m_strConsolPrefsDefault = "";
		m_strConsolPrefsSummary = "";
		break;
	}
}

/**********************************************************************/

void CEposReportSelectInfo::LoadPosTrayConsolPrefs( CPosTrayEposConsolParamsInfo& infoParams, CPosTrayTask& Task )
{
	m_strConsolPrefsAdhoc = "";

	if ( ( FALSE == infoParams.m_bOverride ) || ( Task.GetBatchJobSetConsolType() == FALSE ) )
	{
		switch( Task.GetBatchPreferConsolType() )
		{
		case POSTRAY_EPOS_CONSOL_DEFAULT:	m_strConsolPrefsAdhoc = m_strConsolPrefsDefault;	break;
		case POSTRAY_EPOS_CONSOL_SAVED:		m_strConsolPrefsAdhoc = m_strConsolPrefsSaved;		break;
		case POSTRAY_EPOS_CONSOL_SYSTEM:	m_strConsolPrefsAdhoc = m_strConsolPrefsSummary;	break;
		}
	}
	else if ( FALSE == infoParams.m_bCustomPrefs )
	{
		switch( infoParams.m_nConsolPrefsType )
		{
		case POSTRAY_EPOS_CONSOL_DEFAULT:	m_strConsolPrefsAdhoc = m_strConsolPrefsDefault;	break;
		case POSTRAY_EPOS_CONSOL_SAVED:		m_strConsolPrefsAdhoc = m_strConsolPrefsSaved;		break;
		case POSTRAY_EPOS_CONSOL_SYSTEM:	m_strConsolPrefsAdhoc = m_strConsolPrefsSummary;	break;
		}
	}
	else
		m_strConsolPrefsAdhoc = infoParams.m_strCustomPrefs;

	if ( m_strConsolPrefsAdhoc == "" )
		m_strConsolPrefsAdhoc = m_strConsolPrefsDefault;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportSortedInfo::CEposReportSortedInfo()
{
	Reset();
}

/**********************************************************************/

void CEposReportSortedInfo::Reset()
{
	m_nCurrentSortOrder = 9999;
	m_nDefaultSortOrder = 9999;
	m_nReportType = REPORT_TYPE_PLUSALES;
	m_nMoveCount = 0;
}

/**********************************************************************/

void CEposReportSortedInfo::Add (CEposReportSortedInfo& source )
{
}

/**********************************************************************/

int CEposReportSortedInfo::Compare( CEposReportSortedInfo& source, int nHint )
{
	if ( m_nCurrentSortOrder != source.m_nCurrentSortOrder )
		return ( ( m_nCurrentSortOrder > source.m_nCurrentSortOrder ) ? 1 : -1 );

	if ( m_nDefaultSortOrder != source.m_nDefaultSortOrder )
		return ( ( m_nDefaultSortOrder > source.m_nDefaultSortOrder ) ? 1 : -1 );

	if ( m_nReportType != source.m_nReportType )
		return ( ( m_nReportType > source.m_nReportType ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportSelect::CEposReportSelect(void)
{
	m_bExternalListsValid = FALSE;
	m_nNextCustomReportSerialNo = 1;

	bool bModifier = ( SysInfo.GetMaxBasePluLen() != 0 );
	bool bVIP = ( EcrmanOptions.GetReportsVIPType() != 0 );
	bool bReason = ( EcrmanOptions.GetReportsEODReasonsFlag() == TRUE );
	bool bCascade = ( DealerFlags.GetCascadeReportFlag() == TRUE );
	bool bEOD = ( EcrmanOptions.GetReportsEODSessionFlag() == TRUE );
	
	m_nLastReportType = -1;
	m_nLastReportPos = -1;

	if ( SysInfo.IsEcrLiteSystem() == TRUE )
	{
		AddDefaultReport( REPORT_TYPE_PLUSALES,					"1,0,0,1",		"1,0,0,1",		TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PLUSALES_DEPT_ITEM,		"1,0,0,1",		"1,0,0,1",		TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PLUSALES_MODIFIER,		"1,0,0,1",		"1,0,0,1",		TRUE,	bModifier );
		AddDefaultReport( REPORT_TYPE_PLUSALES_DEPT_SUMMARY,	"1,0,0,1,1",	"1,0,0,1,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_HOURLY,					"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_COVERS_DAILY,				"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PAYMENT_SUMMARY,			"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_TRANSACTION_NORMAL,		"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_TRANSACTION_DISCREPANCY,	"",				"",				FALSE,	TRUE );	//DEFAULT HIDE
	}
	else
	{
		AddDefaultReport( REPORT_TYPE_PLUSALES,					"1,0,0,1",		"1,0,0,1",		TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PLUSALES_NOZEROSKIP,		"1,0,0,1",		"1,0,0,1",		FALSE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PLUSALES_DEPT_ITEM,		"1,0,0,1",		"1,0,0,1",		TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PLUSALES_TAX_ITEM,		"1,0,0,1",		"1,0,0,1",		TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PLUSALES_TRANSERVER,		"1,0,0,1",		"1,0,0,1",		TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_COVERS_PLU_TRANSERVER,	"1,0,0,1",		"1,0,0,1",		TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PLUSALES_CUSTOMER,		"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PLUSALES_MODIFIER,		"1,0,0,1",		"1,0,0,1",		TRUE,	bModifier );
		AddDefaultReport( REPORT_TYPE_PLUSALES_DEPT_SUMMARY,	"1,0,0,1,1",	"1,0,0,1,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_TAX_SUMMARY_TAX,			"1,0,0,1,1",	"1,0,0,1,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_TAX_SUMMARY_PRICE,		"1,0,0,1,1",	"1,0,0,1,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_SPTBOOK_CASCADE,			"1,0,0,1,1",	"1,0,0,1,1",	TRUE,	bCascade );
		AddDefaultReport( REPORT_TYPE_VIPSALES,					"1,0,0,1",		"1,0,0,1",		TRUE,	bVIP );
		AddDefaultReport( REPORT_TYPE_VIPSALES_DEPT_ITEM,		"1,0,0,1",		"1,0,0,1",		TRUE,	bVIP );
		AddDefaultReport( REPORT_TYPE_VIPSALES_TRANSERVER,		"1,0,0,1",		"1,0,0,1",		TRUE,	bVIP );
		AddDefaultReport( REPORT_TYPE_VIPSALES_MODIFIER,		"1,0,0,1",		"1,0,0,1",		TRUE,	bVIP && bModifier );
		AddDefaultReport( REPORT_TYPE_VIPSALES_DEPT_SUMMARY,	"1,0,0,1,1",	"1,0,0,1,1",	TRUE,	bVIP );
		AddDefaultReport( REPORT_TYPE_PLUWASTE,					"1,0,0,1",		"1,0,0,1",		TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PLUWASTE_DEPT_ITEM,		"1,0,0,1",		"1,0,0,1",		TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PLUWASTE_TRANSERVER,		"1,0,0,1",		"1,0,0,1",		TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PLUWASTE_MODIFIER,		"1,0,0,1",		"1,0,0,1",		TRUE,	bModifier );
		AddDefaultReport( REPORT_TYPE_PLUWASTE_DEPT_SUMMARY,	"1,0,0,1,1",	"1,0,0,1,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_HOURLY,					"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_HOURLY_GROUP,				"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_HOURLY_DEPT,				"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_DAILY,					"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_DAILY_GROUP,				"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_DAILY_DEPT,				"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_COVERS_GROUP,				"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_COVERS_DEPT,				"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_COVERS_HOURLY,			"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_COVERS_HOURLY_GROUP,		"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_COVERS_HOURLY_DEPT,		"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_COVERS_DAILY,				"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_COVERS_DAILY_GROUP,		"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_COVERS_DAILY_DEPT,		"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_COVERS_MONTHLY,			"1,0,0,1,1",	"1,0,0,0,1",	FALSE,	TRUE );	//DEFAULT HIDE
		AddDefaultReport( REPORT_TYPE_COVERS_PRICELEVEL,		"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_COVERS_PAYMENT,			"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_TRANSERVER_SUMMARY,		"1,0,0,0,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_GENERAL_GROUP,			"1,1,0,1,0",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_GENERAL_DEPT,				"1,1,0,1,0",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_GENERAL_GROUP_QTY,		"1,1,0,1,0",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_GENERAL_DEPT_QTY,			"1,1,0,1,0",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_WEEKLY_GROUP,				"1,1,0,1,0",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_WEEKLY_DEPT,				"1,1,0,1,0",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_WEEKLY_GROUP_QTY,			"1,1,0,1,0",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_WEEKLY_DEPT_QTY,			"1,1,0,1,0",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PAYMENT_SUMMARY,			"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PAYMENT_SERVER,			"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PAYMENT_GROUP_SUMMARY,	"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PAYMENT_DETAIL,			"1,0,0,1,1",	"1,0,0,0,1",	FALSE,	TRUE ); //DEFAULT HIDE
		AddDefaultReport( REPORT_TYPE_PAYMENT_ESTIMATE_GROUP,	"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PAYMENT_ESTIMATE_DEPT,	"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PAYMENT_ESTIMATE_ITEM,	"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PAYMENT_ESTIMATE_TAX,		"1,0,0,1,1",	"1,0,0,0,1",	TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_DEPOSIT_PURCHASED,		"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_DEPOSIT_REDEEMED,			"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_DEPOSIT_UNREDEEMED,		"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_DEPOSIT_BOOKING_PURCHASE,	"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_DEPOSIT_BOOKING_EVENT,	"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_CUSTOMER_RA,				"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_CUSTOMER_SPEND,			"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_CUSTOMER_ACTIVITY,		"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_CUSTOMER_BALANCE,			"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_PROMOTION_SUMMARY,		"1,0,0,1",		"1,0,0,1",		TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_MIXMATCH_GROUP,			"1,0,0,1",		"1,0,0,1",		TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_MIXMATCH_DEPT,			"1,0,0,1",		"1,0,0,1",		TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_MIXMATCH_ITEM,			"1,0,0,1",		"1,0,0,1",		TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_MIXMATCH_GROUP_COVERS,	"1,0,0,1",		"1,0,0,1",		TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_MIXMATCH_DEPT_COVERS,		"1,0,0,1",		"1,0,0,1",		TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_MIXMATCH_ITEM_COVERS,		"1,0,0,1",		"1,0,0,1",		TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_CHEAPESTONEFREE_SUMMARY,	"1,0,0,1",		"1,0,0,1",		TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_CHEAPESTONEFREE_DETAIL,	"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_TRANSERVER_VOID,			"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_TRANSERVER_VOID_REASON,	"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_TRANSERVER_REFUND,		"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_TRANSERVER_REFUND_REASON,	"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_TRANSERVER_DISCOUNT,		"1,0,0,1",		"1,0,0,1",		TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_TRANSERVER_DISCOUNT_REASON,"",			"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_TRANSERVER_WASTAGE_REASON,"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_TRANSERVER_VIP_REASON,	"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_NOSALE,					"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_AGEVERIFY,				"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_EODSTATUS,				"",				"",				FALSE,	bEOD );
		AddDefaultReport( REPORT_TYPE_REASON_VOID,				"",				"",				TRUE,	bReason );
		AddDefaultReport( REPORT_TYPE_REASON_REFUND,			"",				"",				TRUE,	bReason );
		AddDefaultReport( REPORT_TYPE_REASON_PROMOTION,			"",				"",				TRUE,	bReason );
		AddDefaultReport( REPORT_TYPE_REASON_WASTAGE,			"",				"",				TRUE,	bReason );
		AddDefaultReport( REPORT_TYPE_REASON_VIP,				"",				"",				TRUE,	bReason && bVIP );
		AddDefaultReport( REPORT_TYPE_TRANSACTION_NORMAL,		"",				"",				TRUE,	TRUE );
		AddDefaultReport( REPORT_TYPE_TRANSACTION_DISCREPANCY,	"",				"",				FALSE,	TRUE );	//DEFAULT HIDE
	}
}

/**********************************************************************/

void CEposReportSelect::AddDefaultReport( int nReportType, const char* szPrefsDefault, const char* szPrefsSummary, bool bDefaultSelect, bool bSystemEnable )
{
	CEposReportSelectInfo info;
	info.SetReportType( nReportType );
	info.SetDefaultSelect( bDefaultSelect );
	info.SetCurrentSelect( bDefaultSelect );
	info.SetDefaultSortOrder( m_arrayReports.GetSize() );
	info.SetCurrentSortOrder( 9999 );
	info.SetExportFilename( "" );
	info.SetSystemEnableFlag( bSystemEnable );
	info.SetCustomReportSerialNo( 0 );
	info.SetCustomReportFamily( EPOS_CUSTOM_FAMILY_NONE );
	info.SetCustomReportName( "" );
	info.SetCustomReportParams( "" );
	info.SetConsolPrefsDefault( szPrefsDefault );
	info.SetConsolPrefsSaved( "" );
	info.SetConsolPrefsAdhoc( "" );
	info.SetConsolPrefsSummary( szPrefsSummary );

	if ( ( nReportType == m_nLastReportType + 1 ) && ( m_nLastReportPos != -1 ) )
		m_arrayReports.InsertAt( ++m_nLastReportPos, info );
	else
		m_nLastReportPos = m_arrayReports.Consolidate( info );

	m_nLastReportType = nReportType;
}

/**********************************************************************/

void CEposReportSelect::BuildExternalReportLists()
{
	m_arraySelected.RemoveAll();
	m_arrayAvailable.RemoveAll();

	CReportConsolidationArray<CEposReportSortedInfo> arrayTempSelected;
	CReportConsolidationArray<CEposReportSortedInfo> arrayTempAvailable;

	for ( int n = 0; n < m_arrayReports.GetSize(); n++ )
	{
		CEposReportSelectInfo infoReport;
		m_arrayReports.GetAt( n, infoReport );

		if ( TRUE == infoReport.GetSystemEnableFlag() )
		{
			if ( TRUE == infoReport.GetCurrentSelect() )
			{
				CEposReportSortedInfo infoSorted;
				infoSorted.m_nCurrentSortOrder = infoReport.GetCurrentSortOrder();
				infoSorted.m_nDefaultSortOrder = infoReport.GetDefaultSortOrder();
				infoSorted.m_nReportType = infoReport.GetReportType();
				arrayTempSelected.Consolidate( infoSorted );
			}
			else
			{
				CEposReportSortedInfo infoSorted;
				infoSorted.m_nCurrentSortOrder = infoReport.GetDefaultSortOrder();
				infoSorted.m_nDefaultSortOrder = infoReport.GetDefaultSortOrder();
				infoSorted.m_nReportType = infoReport.GetReportType();
				arrayTempAvailable.Consolidate( infoSorted );
			}
		}
	}

	for ( int n = 0; n < arrayTempSelected.GetSize(); n++ )
	{
		CEposReportSortedInfo infoSorted;
		arrayTempSelected.GetAt( n, infoSorted );
		m_arraySelected.Add( infoSorted.m_nReportType );
	}

	for ( int n = 0; n < arrayTempAvailable.GetSize(); n++ )
	{
		CEposReportSortedInfo infoSorted;
		arrayTempAvailable.GetAt( n, infoSorted );
		m_arrayAvailable.Add( infoSorted.m_nReportType );
	}

	m_bExternalListsValid = TRUE;
}

/**********************************************************************/

int CEposReportSelect::GetSelectedReportCount()
{
	if ( FALSE == m_bExternalListsValid )
		BuildExternalReportLists();

	return m_arraySelected.GetSize();
}

/**********************************************************************/

int CEposReportSelect::GetSelectedReportType( int nPos )
{
	if ( FALSE == m_bExternalListsValid )
		BuildExternalReportLists();

	if ( ( nPos >= 0 ) && ( nPos < m_arraySelected.GetSize() ) )
		return m_arraySelected.GetAt( nPos );
	else
		return REPORT_TYPE_PLUSALES;
}

/**********************************************************************/

int CEposReportSelect::GetAvailableReportCount()
{
	if ( FALSE == m_bExternalListsValid )
		BuildExternalReportLists();

	return m_arrayAvailable.GetSize();
}

/**********************************************************************/

int CEposReportSelect::GetAvailableReportType( int nPos )
{
	if ( FALSE == m_bExternalListsValid )
		BuildExternalReportLists();

	if ( ( nPos >= 0 ) && ( nPos < m_arrayAvailable.GetSize() ) )
		return m_arrayAvailable.GetAt( nPos );
	else
		return REPORT_TYPE_PLUSALES;
}

/**********************************************************************/

void CEposReportSelect::UpdateReportStatus( CArray<CSortedIntWithFlag,CSortedIntWithFlag>& arraySelected, CArray<CSortedIntWithFlag,CSortedIntWithFlag>& arrayAvailable )
{
	m_bExternalListsValid = FALSE;

	for ( int n = 0; n < m_arrayReports.GetSize(); n++ )
	{
		CEposReportSelectInfo infoReport;
		m_arrayReports.GetAt( n, infoReport );
		infoReport.SetCurrentSelect( infoReport.GetDefaultSelect() );
		infoReport.SetCurrentSortOrder( 9999 );
		m_arrayReports.SetAt( n, infoReport );
	}

	for ( int n = 0; n < arraySelected.GetSize(); n++ )
	{
		CEposReportSelectInfo infoReport;

		CSortedIntWithFlag item = arraySelected.GetAt(n);
		infoReport.SetReportType( item.m_nInt );

		int nPos;
		if ( m_arrayReports.Find( infoReport, nPos ) == TRUE )
		{
			m_arrayReports.GetAt( nPos, infoReport );
			infoReport.SetCurrentSelect( TRUE );
			infoReport.SetCurrentSortOrder( n );
			m_arrayReports.SetAt( nPos, infoReport );
		}
	}

	for ( int n = 0; n < arrayAvailable.GetSize(); n++ )
	{
		CEposReportSelectInfo infoReport;

		CSortedIntWithFlag item = arrayAvailable.GetAt(n);
		infoReport.SetReportType( item.m_nInt );

		int nPos;
		if ( m_arrayReports.Find( infoReport, nPos ) == TRUE )
		{
			m_arrayReports.GetAt( nPos, infoReport );
			infoReport.SetCurrentSelect( FALSE );
			infoReport.SetCurrentSortOrder( 9999 );
			m_arrayReports.SetAt( nPos, infoReport );
		}
	}
}

/**********************************************************************/

void CEposReportSelect::ApplyDefaultSortOrder( CArray<CSortedIntWithFlag,CSortedIntWithFlag>& arrayToSort )
{
	CReportConsolidationArray<CEposReportSortedInfo> arrayTemp;

	for ( int n = 0; n < arrayToSort.GetSize(); n++ )
	{	
		CSortedIntWithFlag item = arrayToSort.GetAt(n);
		int nReportType = item.m_nInt;

		CEposReportSelectInfo infoReport;
		infoReport.SetReportType( nReportType );
		infoReport.SetDefaultSortOrder( 9999 );

		int nPos = 0;
		if ( m_arrayReports.Find( infoReport, nPos ) == TRUE )
			m_arrayReports.GetAt( nPos, infoReport );
		
		CEposReportSortedInfo infoSorted;
		infoSorted.m_nReportType = nReportType;
		infoSorted.m_nCurrentSortOrder = 0;
		infoSorted.m_nDefaultSortOrder = infoReport.GetDefaultSortOrder();
		infoSorted.m_nMoveCount = item.m_nFlag;
		arrayTemp.Consolidate( infoSorted );
	}

	arrayToSort.RemoveAll();
	for( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CEposReportSortedInfo infoSorted;
		arrayTemp.GetAt( n, infoSorted );

		CSortedIntWithFlag item;
		item.m_nInt = infoSorted.m_nReportType;
		item.m_nFlag = infoSorted.m_nMoveCount;

		arrayToSort.Add( item );
	}
}

/**********************************************************************/

void CEposReportSelect::LoadReportSelection()
{
	m_bExternalListsValid = FALSE;

	CSSFile fileReportsNew, fileReportsBeta, fileReportsOld;
	CFilenameUpdater FnUpNew( SysFiles::EposReportList );
	CFilenameUpdater FnUpBeta( SysFiles::EposReportListBeta );
	CFilenameUpdater FnUpOld( SysFiles::EposReportListOld );

	if ( fileReportsNew.Open ( FnUpNew.GetFilenameToUse(), "rb" ) == TRUE ) 
	{
		CString strBuffer;
		while ( fileReportsNew.ReadString ( strBuffer ) == TRUE )
		{
			CCSV csv( strBuffer );

			int nVersion = csv.GetInt(0);

			switch( nVersion )
			{
			case 1:
			case 2:
			case 3:
				{
					CEposReportSelectInfo info;
					info.SetReportType( csv.GetInt(1) );

					if ( info.GetReportType() == 9999 )
						m_nNextCustomReportSerialNo = csv.GetInt(2);
					else if ( info.GetReportType() < EPOS_CUSTOM_FIRST_REPORT )
					{
						int nPos;
						if ( m_arrayReports.Find( info, nPos ) == TRUE )
						{
							m_arrayReports.GetAt( nPos, info );
							info.SetCurrentSelect( csv.GetBool(2) );
							info.SetCurrentSortOrder( csv.GetInt(3) );

							if ( 3 == nVersion )
								info.SetExportFilename( csv.GetString(4) );

							m_arrayReports.SetAt( nPos, info );
						}
					}
					else
					{
						int nPos;
						if ( m_arrayReports.Find( info, nPos ) == FALSE )
						{
							int nOffset = 2;
							info.SetCurrentSelect( csv.GetBool(nOffset++) );
							info.SetCurrentSortOrder( csv.GetInt(nOffset++) );

							if ( 3 == nVersion )
								info.SetExportFilename( csv.GetString( nOffset++) );
							
							if ( 1 == nVersion ) 
								nOffset++;
							else
								info.SetCustomReportSerialNo( csv.GetInt(nOffset++) );
							
							info.SetCustomReportFamily( csv.GetInt(nOffset++) );
							info.SetCustomReportName( csv.GetString(nOffset++) );
							info.SetCustomReportParams( csv.GetString(nOffset++) );
							info.SetCurrentSelect( TRUE );
							info.SetDefaultSelect( TRUE );

							bool bAcceptCustom = TRUE;
							switch( info.GetCustomReportFamily() )
							{
							case EPOS_CUSTOM_FAMILY_HOURLY:
							case EPOS_CUSTOM_FAMILY_TRANSACTION:
								bAcceptCustom = TRUE;
								break;

							case EPOS_CUSTOM_FAMILY_GENERAL:
							case EPOS_CUSTOM_FAMILY_DEPOSIT:
							case EPOS_CUSTOM_FAMILY_MIXMATCH:
							case EPOS_CUSTOM_FAMILY_DISCOUNT:
							case EPOS_CUSTOM_FAMILY_HOURLYGRP:
							case EPOS_CUSTOM_FAMILY_HOURLYDPT:
							case EPOS_CUSTOM_FAMILY_COVERS_HOURLY:
							case EPOS_CUSTOM_FAMILY_COVERS_HOURLYGRP:
							case EPOS_CUSTOM_FAMILY_COVERS_HOURLYDPT:
							case EPOS_CUSTOM_FAMILY_PLU_PRICEBAND:
							case EPOS_CUSTOM_FAMILY_LOYALTYRECON:
							case EPOS_CUSTOM_FAMILY_PAYMENTDETAIL:
								bAcceptCustom = ( SysInfo.IsEcrLiteSystem() == FALSE );
								break;

							default:
								bAcceptCustom = FALSE;
								break;
							}
							
							if ( TRUE == bAcceptCustom )
							{
								info.InitialiseCustomReportConsolPrefs();
								m_arrayReports.InsertAt( nPos, info );
							}
						}
					}
				}
				break;
			}
		}

		fileReportsNew.Close();
	}
	else if ( fileReportsBeta.Open ( FnUpBeta.GetFilenameToUse(), "rb" ) == TRUE ) 
	{
		CString strBuffer;
		while ( fileReportsBeta.ReadString ( strBuffer ) == TRUE )
		{
			CCSV csv( strBuffer );

			CEposReportSelectInfo info;
			info.SetReportType( csv.GetInt(0) );

			if ( info.GetReportType() < EPOS_CUSTOM_FIRST_REPORT )
			{
				int nPos;
				if ( m_arrayReports.Find( info, nPos ) == TRUE )
				{
					m_arrayReports.GetAt( nPos, info );
					info.SetCurrentSelect( csv.GetBool(1) );
					info.SetCurrentSortOrder( csv.GetInt(2) );
					m_arrayReports.SetAt( nPos, info );
				}
			}
			else
			{
				int nPos;
				if ( m_arrayReports.Find( info, nPos ) == FALSE )
				{
					info.SetCurrentSelect( csv.GetBool(1) );
					info.SetCurrentSortOrder( csv.GetInt(2) );
					info.SetCustomReportSerialNo(0);
					info.SetCustomReportFamily( csv.GetInt(3) );
					info.SetCustomReportName( csv.GetString(4) );
					info.SetCustomReportParams( csv.GetString(5) );
					info.SetCurrentSelect( TRUE );
					info.SetDefaultSelect( TRUE );
					info.InitialiseCustomReportConsolPrefs();
					m_arrayReports.InsertAt( nPos, info );
				}
			}
		}

		fileReportsBeta.Close();
	}
	else if ( fileReportsOld.Open ( FnUpOld.GetFilenameToUse(), "rb" ) == TRUE ) 
	{
		CString strBuffer;
		fileReportsOld.ReadString ( strBuffer );
		fileReportsOld.Close();

		for ( int n = 0; n < m_arrayReports.GetSize(); n++ )
		{
			CEposReportSelectInfo info;
			m_arrayReports.GetAt( n, info );
			info.SetCurrentSelect( info.GetSystemEnableFlag() );
			info.SetCurrentSortOrder( 9999 );
			m_arrayReports.SetAt( n, info );
		}

		CCSV csv( strBuffer );
		for ( int n = 0; n < csv.GetSize(); n++ )
		{
			CEposReportSelectInfo info;
			info.SetReportType( csv.GetInt(n) );

			int nPos;
			if ( m_arrayReports.Find( info, nPos ) == TRUE )
			{
				m_arrayReports.GetAt( nPos, info );
				info.SetCurrentSelect( TRUE );
				info.SetCurrentSortOrder( n );
				m_arrayReports.SetAt( nPos, info );
			}
		}
	}
}

/**********************************************************************/

void CEposReportSelect::SaveReportSelection()
{
	CSSFile fileReports;
	CFilenameUpdater FnUp( SysFiles::EposReportList );
	if ( fileReports.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
	{
		for ( int nPos = 0; nPos < m_arrayReports.GetSize(); nPos++ )
		{
			CEposReportSelectInfo info;
			m_arrayReports.GetAt( nPos, info );

			if ( info.GetReportType() < EPOS_CUSTOM_FIRST_REPORT )
			{
				CString strExportFilename = info.GetExportFilename();
				if ( ( TRUE == info.GetCurrentSelect() ) || ( TRUE == info.GetDefaultSelect() ) || ( strExportFilename != "" ) )
				{
					CCSV csv;
					csv.Add( 3 );
					csv.Add( info.GetReportType() );
					csv.Add( info.GetCurrentSelect() );
					csv.Add( info.GetCurrentSortOrder() );
					csv.Add( strExportFilename );
					fileReports.WriteLine( csv.GetLine() );
				}
			}
			else
			{
				CCSV csv;
				csv.Add( 3 );
				csv.Add( info.GetReportType() );
				csv.Add( info.GetCurrentSelect() );
				csv.Add( info.GetCurrentSortOrder() );
				csv.Add( info.GetExportFilename() );
				csv.Add( info.GetCustomReportSerialNo() );
				csv.Add( info.GetCustomReportFamily() );
				csv.Add( info.GetCustomReportName() );
				csv.Add( info.GetCustomReportParams() );
				fileReports.WriteLine( csv.GetLine() );
			}
		}

		CCSV csv;
		csv.Add( 3 );
		csv.Add( 9999 );
		csv.Add( m_nNextCustomReportSerialNo );
		fileReports.WriteLine( csv.GetLine() );
	}
}

/**********************************************************************/

bool CEposReportSelect::AddCustomReport( CEposReportSelectInfo& infoReport, int nFamily )
{
	bool bCreated = FALSE;

	for ( int n = 0; n < 40; n++ )
	{
		infoReport.SetReportType ( EPOS_CUSTOM_FIRST_REPORT + n );
	
		int nPos;
		if ( m_arrayReports.Find( infoReport, nPos ) == FALSE )
		{
			CString strName;
			strName.Format( "Custom Report %d", n + 1 );
			infoReport.SetCustomReportSerialNo( m_nNextCustomReportSerialNo++ );
			infoReport.SetCustomReportFamily( nFamily );
			infoReport.SetCustomReportName( strName );
			m_arrayReports.InsertAt( nPos, infoReport );
			bCreated = TRUE;
			break;
		}
	}
	
	return bCreated;	
}

/**********************************************************************/

void CEposReportSelect::RemoveCustomReport( CEposReportSelectInfo& infoReport )
{
	if ( infoReport.GetReportType() >= EPOS_CUSTOM_FIRST_REPORT )
	{
		int nPos;
		if ( m_arrayReports.Find( infoReport, nPos ) == TRUE )
			m_arrayReports.RemoveAt(nPos);
	}	
}

/**********************************************************************/

const char* CEposReportSelect::GetCustomReportName( int nReportType )
{
	CEposReportSelectInfo infoReport;
	infoReport.SetReportType( nReportType );
	
	int nPos = 0;
	if ( m_arrayReports.Find( infoReport, nPos ) == TRUE )
	{
		m_arrayReports.GetAt( nPos, infoReport );
		m_strCustomReportName = infoReport.GetCustomReportName();
	}
	else
	{
		m_strCustomReportName.Format("Unknown Report (%d)", nReportType);
	}

	return m_strCustomReportName;
}

/**********************************************************************/

int CEposReportSelect::GetCustomReportFamily(int nReportType)
{
	int nFamily = -1;

	CEposReportSelectInfo infoReport;
	infoReport.SetReportType(nReportType);

	int nPos = 0;
	if (m_arrayReports.Find(infoReport, nPos) == TRUE)
	{
		m_arrayReports.GetAt(nPos, infoReport);
		nFamily = infoReport.GetCustomReportFamily();
	}
	
	return nFamily;
}

/**********************************************************************/

bool CEposReportSelect::GetReportInfo( CEposReportSelectInfo& infoReport )
{
	bool bResult = FALSE;
	
	int nPos;
	if ( m_arrayReports.Find( infoReport, nPos ) == TRUE )
	{
		m_arrayReports.GetAt(nPos,infoReport);
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

bool CEposReportSelect::SetReportInfo( CEposReportSelectInfo& infoReport )
{
	bool bResult = FALSE;
	
	int nPos;
	if ( m_arrayReports.Find( infoReport, nPos ) == TRUE )
	{
		m_arrayReports.SetAt(nPos,infoReport);
		bResult = TRUE;
	}
		
	return bResult;
}

/**********************************************************************/

void CEposReportSelect::GetDefaultExportFilename( int nReportType, CString& strName )
{
	strName.Format( "E%1.1d%3.3d",
		( nReportType >= EPOS_CUSTOM_FIRST_REPORT ) ? 2 : 1,
		( nReportType >= EPOS_CUSTOM_FIRST_REPORT ) ? nReportType + 1 - EPOS_CUSTOM_FIRST_REPORT : nReportType );
}

/**********************************************************************/
