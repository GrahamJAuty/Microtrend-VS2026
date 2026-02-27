//******************************************************************************
#include "BackgroundOptions.h"
//******************************************************************************

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//******************************************************************

CBackgroundOptions::CBackgroundOptions()
{
	m_strFilename = Filenames.GetBackgroundOptionsFilename();

	m_strStandardStart				= "00:00";
	m_strStandardEnd				= "23:59";
	m_nStandardImportMinutes		= 0;

	m_strNewRecordStart				= "00:00";
	m_strNewRecordEnd				= "23:59";
	m_nNewRecordsMinutes			= 0;

	m_strExportSalesStart			= "00:00";
	m_strExportSalesEnd				= "23:59";
	m_nExportSalesMinutes			= 0;

	m_nWebPaymentMinutes			= 0;
	m_bWebPaymentSuspendOnError		= FALSE;

	m_bEODExport					= FALSE;				// closing balance export
	m_nEODDayFlags					= 0x3e;					// Bit flags 0=sun..6=sat(default mon-fri ON sat-sun OFF )
	m_strDateNextEODCheck			= "";					// next date for eod closing balance
	m_strTimeNextEODCheck			= "17:00";

	m_bEmailLowBalance				= FALSE;				// email low balances
	m_strDateNextLowBalanceCheck	= "";					// next date for email low balances
	m_strTimeNextLowBalanceCheck	= "17:00";

	m_bEmailPurchases				= FALSE;				// email purchases
	m_strDateNextEmailPurchase		= "";					// next date for email purchases
	m_strTimeEmailPurchase			= "17:00";

	m_bEmailPointsAchievement		= FALSE;
	m_strDateNextPointsAchievement	= "";					// next date for points achievement
	m_strTimeNextPointsAchievement	= "17:00";

	m_bEmailWeeklyPoints			= FALSE;
	m_strDateNextWeeklyPoints		= "";					// next date for weekly points update
	m_strTimeNextWeeklyPoints		= "17:00";
}

//******************************************************************
// data saved in sysfiles & Cardnnn folders

void CBackgroundOptions::Read()	
{
	CSSIniFile file;
	if ( file.Read ( m_strFilename ) == TRUE )
	{ 
		m_nStandardImportMinutes		= file.GetInt	 ( "Standard",			m_nStandardImportMinutes );
		m_strStandardStart				= file.GetString ( "StandardStart",		m_strStandardStart );
		m_strStandardEnd				= file.GetString ( "StandardEnd",		m_strStandardEnd );
		m_nNewRecordsMinutes			= file.GetInt	 ( "NewRecords",		m_nNewRecordsMinutes );
		m_strNewRecordStart				= file.GetString ( "NewRecordStart",	m_strNewRecordStart );
		m_strNewRecordEnd				= file.GetString ( "NewRecordEnd",		m_strNewRecordEnd );
		m_nWebPaymentMinutes			= file.GetInt	 ( "WebPayments",		m_nWebPaymentMinutes );
		m_bWebPaymentSuspendOnError		= file.GetBool   ( "WebSuspendOnErr",	m_bWebPaymentSuspendOnError );
		m_strExportSalesStart			= file.GetString ( "SalesStart",		m_strExportSalesStart );
		m_strExportSalesEnd				= file.GetString ( "SalesEnd",			m_strExportSalesEnd );
		m_nExportSalesMinutes			= file.GetInt	 ( "Sales",				m_nExportSalesMinutes );
		m_bEODExport					= file.GetBool   ( "EODExport",			m_bEODExport );
		m_nEODDayFlags					= file.GetInt	 ( "EODDays",			m_nEODDayFlags );
		m_bEmailLowBalance				= file.GetBool   ( "EmailLowBalance",	m_bEmailLowBalance );
		m_bEmailPurchases				= file.GetBool	 ( "EmailPurchases",	m_bEmailPurchases );
		m_strTimeNextEODCheck			= file.GetString ( "EODTime",			m_strTimeNextEODCheck );
		m_strDateNextEODCheck			= file.GetString ( "EODDate",			m_strDateNextEODCheck );
		m_strDateNextLowBalanceCheck	= file.GetString ( "LowBalanceDate",	m_strDateNextLowBalanceCheck );
		m_strTimeNextLowBalanceCheck	= file.GetString ( "LowBalanceTime",	m_strTimeNextLowBalanceCheck );
		m_strTimeEmailPurchase			= file.GetString ( "EmailPurchaseTime",	m_strTimeEmailPurchase );
		m_strDateNextEmailPurchase		= file.GetString ( "EmailPurchaseDate",	m_strDateNextEmailPurchase );
		m_bEmailPointsAchievement		= file.GetBool   ( "PA-Email",			m_bEmailPointsAchievement );
		m_strDateNextPointsAchievement	= file.GetString ( "PA-Date",			m_strDateNextPointsAchievement );
		m_strTimeNextPointsAchievement	= file.GetString ( "PA-Time",			m_strTimeNextPointsAchievement );
		m_bEmailWeeklyPoints			= file.GetBool   ( "WP-Email",			m_bEmailWeeklyPoints );
		m_strDateNextWeeklyPoints		= file.GetString ( "WP-Date",			m_strDateNextWeeklyPoints );
		m_strTimeNextWeeklyPoints		= file.GetString ( "WP-Time",			m_strTimeNextWeeklyPoints );
	}
}

//*********************************************************************

void CBackgroundOptions::Save()
{
	CSSIniFile file;

	file.Set ( "Standard",			m_nStandardImportMinutes );
	file.Set ( "StandardStart",		m_strStandardStart );
	file.Set ( "StandardEnd",		m_strStandardEnd );
	file.Set ( "NewRecords",		m_nNewRecordsMinutes );
	file.Set ( "NewRecordStart",	m_strNewRecordStart );
	file.Set ( "NewRecordEnd",		m_strNewRecordEnd );
	file.Set ( "WebPayments",		m_nWebPaymentMinutes );
	file.Set ( "WebSuspendOnErr",	m_bWebPaymentSuspendOnError );
	file.Set ( "SalesStart",		m_strExportSalesStart );
	file.Set ( "SalesEnd",			m_strExportSalesEnd );
	file.Set ( "Sales",				m_nExportSalesMinutes );
	file.Set ( "EODExport",			m_bEODExport );
	file.Set ( "EODDays",			m_nEODDayFlags );
	file.Set ( "EmailLowBalance",	m_bEmailLowBalance );
	file.Set ( "LowBalanceDate",	m_strDateNextLowBalanceCheck );
	file.Set ( "LowBalanceTime",	m_strTimeNextLowBalanceCheck );		// email low balance
	file.Set ( "EmailPurchases",	m_bEmailPurchases );
	file.Set ( "EmailPurchaseTime", m_strTimeEmailPurchase );			// email purchases
	file.Set ( "EmailPurchaseDate",	m_strDateNextEmailPurchase );
	file.Set ( "EODDate",			m_strDateNextEODCheck );
	file.Set ( "EODTime",			m_strTimeNextEODCheck );			// eod closing balance
	file.Set ( "PA-Email", 			m_bEmailPointsAchievement );
	file.Set ( "PA-Date",			m_strDateNextPointsAchievement );
	file.Set ( "PA-Time",			m_strTimeNextPointsAchievement );
	file.Set ( "WP-Email",			m_bEmailWeeklyPoints );
	file.Set ( "WP-Date",			m_strDateNextWeeklyPoints );
	file.Set ( "WP-Time",			m_strTimeNextWeeklyPoints );


	file.Write ( m_strFilename );
}

//******************************************************************

bool CBackgroundOptions::IsSet()
{
	if ( m_nStandardImportMinutes != 0 )		return TRUE;
	if ( m_nNewRecordsMinutes != 0)				return TRUE;
	if ( m_nWebPaymentMinutes != 0 )			return TRUE;
	if ( m_bEODExport != FALSE )				return TRUE;
	if ( m_bEmailLowBalance != FALSE )			return TRUE;
	if ( m_bEmailPurchases != FALSE )			return TRUE;
	if ( m_bEmailPointsAchievement == TRUE )	return TRUE;
	if ( m_bEmailWeeklyPoints == TRUE )			return TRUE;
	return FALSE;
}

//******************************************************************
// 0=Sun, .... 6=Sat

bool CBackgroundOptions::IsEODDay ( int nDay )
{
	if ( nDay < 0 || nDay > 6 )
		return FALSE;

	int nMask = 0x01;
	nMask = nMask << nDay;

	return ( ( m_nEODDayFlags & nMask ) == nMask ) ? TRUE : FALSE;
}

void CBackgroundOptions::SetEODDay( int nDay, bool bSet )
{
	if ( nDay < 0 || nDay > 6 )
		return;

	int nMask = 0x01;
	nMask = nMask << nDay;
	
	if ( bSet == TRUE )		m_nEODDayFlags |= nMask;
	else					m_nEODDayFlags &= ~nMask;
}

//******************************************************************************
