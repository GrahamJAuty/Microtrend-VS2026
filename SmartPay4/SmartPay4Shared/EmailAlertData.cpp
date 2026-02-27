//*******************************************************************
#include "EmailAlertData.h"
//*******************************************************************

CEmailAlertData::CEmailAlertData()
{
	m_nEmailContactNo = 0;										// email address contactNo 1-5
	m_strSignatureText1 = "End of Message.";
	m_bEnableLowBalanceAlerts = FALSE;
	m_strLowBalanceEmailText = "Low Balance Alert";
	m_bPromptLowBalanceAlerts = TRUE;
	m_nLowBalanceTickNo = 0;									// 0=not used else 1 - 6
	m_bLowBalancePurse1Reqd = TRUE;
	m_bLowBalancePurse2Reqd = FALSE;
	m_bLowBalancePurse3Reqd = FALSE;
	m_bLowBalanceShowPurse1 = TRUE;
	m_bLowBalanceShowPurse2 = FALSE;
	m_bLowBalanceShowPurse3 = FALSE;
	m_bLowBalanceShowPoints = FALSE;
	m_bLowBalancePurse1Symbol = FALSE;
	m_bLowBalancePurse2Symbol = FALSE;
	m_bLowBalancePurse3Symbol = FALSE;
	m_strLowBalancePurse1Text = "Purse 1";
	m_strLowBalancePurse2Text = "Purse 2";
	m_strLowBalancePurse3Text = "Purse 3";

	m_bEnablePointsAchievement = FALSE;
	m_bPromptPointsAchievement = TRUE;
	m_nPointsAchievementTickNo = 0;								// 0=not defined else 1 - 6
	m_strPointsAchievementEmailText = "Points Achievement";

	m_bEnableWeeklyPoints = FALSE;
	m_bPromptWeeklyPoints = TRUE;
	m_strWeeklyPointsEmailText = "Points Notification";
	m_nWeeklyPointsTickNo = 0;									// 0=not defined else 1 - 6
	m_nWeeklyPointsDay = 5;										// 0=sunday, 1-monday .. 5-Friday
	m_bWeeklyPointsExcludeNoChange = TRUE;

	m_bEnablePurchaseAlerts = FALSE;
	m_strPurchaseEmailText = "Purchase Notification";
	m_bPromptPurchaseAlerts = TRUE;
	m_nPurchaseTickNo = 0;										// 0=not defined else 1 - 6
	m_nPurchaseDay = 5;											// 0=sunday, 1-monday .. 5-Friday
	m_strPurchaseStartFrom = "";
	m_bPurchaseExcludeZeroPriced = FALSE;
	
	m_strFilename = Filenames.GetEmailAlertDataFilename();
}

//*******************************************************************

CString CEmailAlertData::CheckIndicatorConflict( int nPageNo )
{
	CString strResult = "";

	if ( ( nPageNo >= 0 ) && ( nPageNo <= 3 ) )
	{	
		CArray<int,int> arrayTickNo;
		arrayTickNo.Add( m_nLowBalanceTickNo );
		arrayTickNo.Add( m_nPointsAchievementTickNo );
		arrayTickNo.Add( m_nWeeklyPointsTickNo );
		arrayTickNo.Add( m_nPurchaseTickNo );

		int nTickNo = arrayTickNo.GetAt( nPageNo ); 
		
		if ( nTickNo != 0 )
		{
			CStringArray arrayAlertTypes;
			arrayAlertTypes.Add( "Low Balance" );
			arrayAlertTypes.Add( "Points Achievement" );
			arrayAlertTypes.Add( "Weekly Points Update" );
			arrayAlertTypes.Add( "Itemised Purchases" );

			for ( int n = 0; n <= 3; n++ )
			{
				if ( n != nPageNo )
				{
					if ( nTickNo == arrayTickNo.GetAt(n) )
					{
						strResult.Format( "Conflict between %s and %s database indicator",
							(const char*) arrayAlertTypes.GetAt(nPageNo),
							(const char*) arrayAlertTypes.GetAt(n) );

						break;
					}
				}
			}
		}
	}

	return strResult;
}

//*******************************************************************

void CEmailAlertData::Read()	
{
	CSSIniFile file;
	if ( file.Read ( m_strFilename ) == TRUE )
	{ 
		m_nEmailContactNo				= file.GetInt   ( "EM_ContactNo",	m_nEmailContactNo );
		m_strSignatureText1				= file.GetString( "SigText1",		m_strSignatureText1 );
		m_bEnableLowBalanceAlerts		= file.GetBool  ( "LB_Enable",		m_bEnableLowBalanceAlerts );
		m_bPromptLowBalanceAlerts		= file.GetBool  ( "LB_Prompt",		m_bPromptLowBalanceAlerts );
		m_strLowBalanceEmailText		= file.GetString( "LB_Subject",		m_strLowBalanceEmailText );
		m_nLowBalanceTickNo				= file.GetInt   ( "LB_TickNo",		m_nLowBalanceTickNo );
		m_bLowBalancePurse1Reqd			= file.GetBool  ( "LB_Purse1",		m_bLowBalancePurse1Reqd );
		m_bLowBalancePurse2Reqd			= file.GetBool  ( "LB_Purse2",		m_bLowBalancePurse2Reqd );
		m_bLowBalancePurse3Reqd			= file.GetBool  ( "LB_Purse3",		m_bLowBalancePurse3Reqd );
		m_bLowBalanceShowPurse1			= file.GetBool  ( "LBS_Purse1",		m_bLowBalanceShowPurse1 );
		m_bLowBalanceShowPurse2			= file.GetBool  ( "LBS_Purse2",		m_bLowBalanceShowPurse2 );
		m_bLowBalanceShowPurse3			= file.GetBool  ( "LBS_Purse3",		m_bLowBalanceShowPurse3 );
		m_bLowBalanceShowPoints			= file.GetBool  ( "LBS_Points",		m_bLowBalanceShowPoints );
		m_bLowBalancePurse1Symbol		= file.GetBool  ( "LB_Purse1Symb",	m_bLowBalancePurse1Symbol );
		m_bLowBalancePurse2Symbol		= file.GetBool  ( "LB_Purse2Symb",	m_bLowBalancePurse2Symbol );
		m_bLowBalancePurse3Symbol		= file.GetBool  ( "LB_Purse3Symb",	m_bLowBalancePurse3Symbol );
		m_strLowBalancePurse1Text		= file.GetString( "LB_Purse1Text",	m_strLowBalancePurse1Text );
		m_strLowBalancePurse2Text		= file.GetString( "LB_Purse2Text",	m_strLowBalancePurse2Text );
		m_strLowBalancePurse3Text		= file.GetString( "LB_Purse3Text",	m_strLowBalancePurse3Text );
		
		m_bEnablePurchaseAlerts			= file.GetBool  ( "EP_Enable",		m_bEnablePurchaseAlerts );
		m_strPurchaseEmailText			= file.GetString( "EP_Subject",		m_strPurchaseEmailText );
		m_bPromptPurchaseAlerts			= file.GetBool  ( "EP_Prompt",		m_bPromptPurchaseAlerts );
		m_nPurchaseTickNo				= file.GetInt   ( "EP_TickNo",		m_nPurchaseTickNo );
		m_nPurchaseDay					= file.GetInt   ( "EP_DayNo",		m_nPurchaseDay );
		m_strPurchaseStartFrom			= file.GetString( "EP_Start",		CSSDate::GetCurrentDate().GetDate() );
		m_bPurchaseExcludeZeroPriced	= file.GetBool  ( "EP_ExZero",		m_bPurchaseExcludeZeroPriced );

		m_bEnablePointsAchievement		= file.GetBool  ( "PA_Enable",		m_bEnablePointsAchievement );
		m_bPromptPointsAchievement		= file.GetBool  ( "PA_Prompt",		m_bPromptPointsAchievement );
		m_strPointsAchievementEmailText	= file.GetString( "PA_Subject",		m_strPointsAchievementEmailText );
		m_nPointsAchievementTickNo		= file.GetInt   ( "PA_TickNo",		m_nPointsAchievementTickNo );

		m_bEnableWeeklyPoints			= file.GetBool	 ( "WP_Enable",		m_bEnableWeeklyPoints ); 
		m_bPromptWeeklyPoints			= file.GetBool	 ( "WP_Prompt",		m_bPromptWeeklyPoints );
		m_strWeeklyPointsEmailText		= file.GetString ( "WP_Subject",	m_strWeeklyPointsEmailText ); 
		m_nWeeklyPointsTickNo			= file.GetInt	 ( "WP_TickNo",		m_nWeeklyPointsTickNo );
		m_nWeeklyPointsDay				= file.GetInt	 ( "WP_DayNo",		m_nWeeklyPointsDay );
		m_bWeeklyPointsExcludeNoChange	= file.GetBool	 ( "WP_ExNoChng",	m_bWeeklyPointsExcludeNoChange );
	}
}

//*********************************************************************

void CEmailAlertData::Save()
{
	CSSIniFile file;

	file.Set ( "EM_ContactNo",		m_nEmailContactNo );;
	file.Set ( "SigText1",			m_strSignatureText1 );
	file.Set ( "LB_Enable",			m_bEnableLowBalanceAlerts );
	file.Set ( "LB_Prompt",			m_bPromptLowBalanceAlerts );
	file.Set ( "LB_Subject",		m_strLowBalanceEmailText );
	file.Set ( "LB_TickNo",			m_nLowBalanceTickNo );
	file.Set ( "LB_Purse1",			m_bLowBalancePurse1Reqd );
	file.Set ( "LB_Purse2",			m_bLowBalancePurse2Reqd );
	file.Set ( "LB_Purse3",			m_bLowBalancePurse3Reqd );
	file.Set ( "LBS_Purse1",		m_bLowBalanceShowPurse1 );
	file.Set ( "LBS_Purse2",		m_bLowBalanceShowPurse2 );
	file.Set ( "LBS_Purse3",		m_bLowBalanceShowPurse3 );
	file.Set ( "LBS_Points",		m_bLowBalanceShowPoints );
	file.Set ( "LB_Purse1Symb",		m_bLowBalancePurse1Symbol );
	file.Set ( "LB_Purse2Symb",		m_bLowBalancePurse2Symbol );
	file.Set ( "LB_Purse3Symb",		m_bLowBalancePurse3Symbol );
	file.Set ( "LB_Purse1Text",		m_strLowBalancePurse1Text );
	file.Set ( "LB_Purse2Text",		m_strLowBalancePurse2Text );
	file.Set ( "LB_Purse3Text",		m_strLowBalancePurse3Text );
	
	file.Set ( "EP_Enable",			m_bEnablePurchaseAlerts );
	file.Set ( "EP_Subject",		m_strPurchaseEmailText );
	file.Set ( "EP_Prompt",			m_bPromptPurchaseAlerts );
	file.Set ( "EP_TickNo",			m_nPurchaseTickNo );
	file.Set ( "EP_DayNo",			m_nPurchaseDay );
	file.Set ( "EP_Start",			m_strPurchaseStartFrom );
	file.Set ( "EP_ExZero",			m_bPurchaseExcludeZeroPriced );

	file.Set ( "PA_Enable",			m_bEnablePointsAchievement );
	file.Set ( "PA_Prompt",			m_bPromptPointsAchievement );
	file.Set ( "PA_Subject",		m_strPointsAchievementEmailText );
	file.Set ( "PA_TickNo",			m_nPointsAchievementTickNo );

	file.Set ( "WP_Enable",			m_bEnableWeeklyPoints ); 
	file.Set ( "WP_Prompt",			m_bPromptWeeklyPoints );
	file.Set ( "WP_Subject",		m_strWeeklyPointsEmailText ); 
	file.Set ( "WP_TickNo",			m_nWeeklyPointsTickNo );
	file.Set ( "WP_DayNo",			m_nWeeklyPointsDay );
	file.Set ( "WP_ExNoChng",		m_bWeeklyPointsExcludeNoChange );

	file.Write ( m_strFilename );
}

//*********************************************************************

bool CEmailAlertData::AlertsRequired()
{
//	if ( SendLowBalancesReqd() == TRUE )		return TRUE;
	if ( m_bEnableLowBalanceAlerts == TRUE )	return TRUE;
	if ( m_bEnablePurchaseAlerts == TRUE )		return TRUE;
	if ( m_bEnablePointsAchievement == TRUE )	return TRUE;
	if ( m_bEnableWeeklyPoints == TRUE )		return TRUE;
	return FALSE;
}

//*********************************************************************

bool CEmailAlertData::SendLowBalancesReqd()
{
	if ( m_bEnableLowBalanceAlerts == FALSE )
	{
		return FALSE;
	}

	if ( m_bLowBalancePurse1Reqd == FALSE && m_bLowBalancePurse2Reqd == FALSE && m_bLowBalancePurse3Reqd == FALSE )
	{
		return FALSE;
	}

	return TRUE;
}

//*********************************************************************

bool CEmailAlertData::PointsBalanceReqd()
{
	return m_bLowBalanceShowPoints;
}

//*********************************************************************

bool CEmailAlertData::SendPurchasesReqd()
{
	if ( m_bEnablePurchaseAlerts == FALSE )
	{
		return FALSE;
	}

	return TRUE;
}

//*********************************************************************

bool CEmailAlertData::SendPointsAchievementReqd()
{
	if ( m_bEnablePointsAchievement == FALSE )	
	{
		return FALSE;
	}

	return TRUE;
}

//*********************************************************************

bool CEmailAlertData::SendWeeklyPointsReqd()
{
	if ( m_bEnableWeeklyPoints == FALSE )	
	{
		return FALSE;
	}

	return TRUE;
}

//*********************************************************************
