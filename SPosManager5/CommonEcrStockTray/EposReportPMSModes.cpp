/**********************************************************************/
#include "LocationCSVArray.h"
#include "PMSOptions.h"
/**********************************************************************/
#include "EposReportPMSModes.h"
/**********************************************************************/

/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/

CEposReportPMSModes::CEposReportPMSModes(void)
{
	m_bEpos = TRUE;
	m_bWantDeposits = FALSE;
	m_bWantComplete = FALSE;
	m_bWantPending = FALSE;
	m_bWantActive = FALSE;
	m_bWantCancelled = FALSE;
	m_nAllowMode = PMS_ALLOWMODE_ALL;
	SetDateMode( PMS_DATEMODE_ACTUAL );
}

/**********************************************************************/

void CEposReportPMSModes::SetDateMode( int n )
{
	switch( n )
	{
	case PMS_DATEMODE_ACTUAL:
	case PMS_DATEMODE_ACCOUNT:
		m_nDateMode = n;
		break;
	}
}

/**********************************************************************/
// RETURN 0 = PMS LOCATION, TIME REJECTED BY FILTER
// RETURN 1 = PMS LOCATION, TIME ACCEPTED BY FILTER
// RETURN 2 = NOT A PMS LOCATION
/**********************************************************************/

int CEposReportPMSModes::GetPMSLocationStatus( CTermFileListInfo& infoFile )
{
	if ( dbLocation.IsPMSPseudoLocation( infoFile.m_nLocIdx ) == FALSE )
		return 2;

	if ( PMS_DATEMODE_ACTUAL == m_nDateMode )
		return 1;
		
	if ( TRUE == infoFile.m_bIsActivePMSFile )
		return 1;
	
	CString strTime;
	strTime.Format( "%2.2d%2.2d00",
		PMSOptions.GetEposReportHour(),
		PMSOptions.GetEposReportMinute() );

	if ( DataManagerNonDb.SessionDateTimeFilter.CheckTime( infoFile.m_strDateTran, strTime ) != 0 )
		return 1;

	return 0;
}

/**********************************************************************/

void CEposReportPMSModes::GetInitialSaleDateTime( bool bIsPMSLocation, CTermFileListInfo& infoFile, bool bSaleTime, CString& strDate, CString& strTime )
{
	strDate = "";
	strTime = "";

	if ( FALSE == bIsPMSLocation )
	{
		if ( TRUE == bSaleTime )
			strDate = infoFile.m_strDateSale;
		else
			strDate = infoFile.m_strDateTran;
	}
	else if ( PMS_DATEMODE_ACCOUNT == m_nDateMode )
	{
		strDate = infoFile.m_strDateTran;

		strTime.Format( "%2.2d%2.2d00",
			PMSOptions.GetEposReportHour(),
			PMSOptions.GetEposReportMinute() );
	}
}

/**********************************************************************/

void CEposReportPMSModes::GetPMSTransactionDate( bool bIsPMSLocation, CTermFileListInfo& infoFile, CString& strDate, CString& strTime )
{
	if ( TRUE == bIsPMSLocation )
	{
		strDate = infoFile.m_strDateTran;
		strTime.Format( "%2.2d%2.2d00",
			PMSOptions.GetEposReportHour(),
			PMSOptions.GetEposReportMinute() );
	}
}

/**********************************************************************/

bool CEposReportPMSModes::CheckPMSTransactionType( bool bIsPMSLocation, int& nCashRSPLineType )
{
	bool bResult = TRUE;

	if ( TRUE == bIsPMSLocation )
	{
		switch( nCashRSPLineType )
		{
		case CASHRSP_LINETYPE_AGEVERIFY:
		case CASHRSP_LINETYPE_NOSALE:
			bResult = FALSE;
			break;

		case CASHRSP_LINETYPE_TRAN_NORMAL:
			bResult = ( m_nDateMode == PMS_DATEMODE_ACCOUNT );
			break;

		case CASHRSP_LINETYPE_TRAN_PMS:
			bResult = ( m_nDateMode == PMS_DATEMODE_ACTUAL );
			nCashRSPLineType = CASHRSP_LINETYPE_TRAN_NORMAL;
			break;
		}
	}

	return bResult;
}

/**********************************************************************/

//CHECK LINE FOR #PMS_TIME HEADER AND PROCESS DATE AND TIME IF FOUND
//RETURNS FLAG TO SAY WHETHER LINE SHOULD BE PROCESSED AS CASHRSP
//WILL RETURN FALSE IN ACTUAL DATE REPORTING MODE IF INCOMING PMSINCLUDE FLAG IS ALREADY FALSE  

bool CEposReportPMSModes::CheckPMSTimeLine( bool bIsPMSLocation, CString& strBuffer, CString& strThisSaleDate, CString& strThisSaleTime, COleDateTime& oleDatePMS, bool& bPMSInclude )
{
	bool bProcessLine = TRUE;

	if ( TRUE == bIsPMSLocation )
	{
		if ( strBuffer.Left(9) == "#PMS_TIME" )
		{
			if ( PMS_DATEMODE_ACTUAL == m_nDateMode )
			{
				CCSV csvTime( strBuffer );
				int nPMSDayCurrent = csvTime.GetInt(1);

				GetOleDateSince2011( nPMSDayCurrent, oleDatePMS );

				strThisSaleDate.Format( "%4.4d%2.2d%2.2d",
					oleDatePMS.GetYear(),
					oleDatePMS.GetMonth(),
					oleDatePMS.GetDay() );

				strThisSaleTime.Format( "%2.2d%2.2d00", csvTime.GetInt(2), csvTime.GetInt(3) );

				bPMSInclude = DataManagerNonDb.SessionDateTimeFilter.CheckDate( strThisSaleDate );
			}
			bProcessLine = FALSE;
		}
		else 
		{
			if ( ( PMS_DATEMODE_ACTUAL == m_nDateMode ) && ( FALSE == bPMSInclude ) )
				bProcessLine = FALSE;
		}
	}
	return bProcessLine;
}

/**********************************************************************/
#else
/**********************************************************************/

void CEposReportPMSModes::GetInitialSaleDateTime( bool bIsPMSLocation, CTermFileListInfo& infoFile, bool bSaleTime, CString& strDate, CString& strTime )
{
	strDate = "";
	strTime = "";

	if ( TRUE == bSaleTime )
		strDate = infoFile.m_strDateSale;
	else
		strDate = infoFile.m_strDateTran;
}

/**********************************************************************/
#endif
/**********************************************************************/
