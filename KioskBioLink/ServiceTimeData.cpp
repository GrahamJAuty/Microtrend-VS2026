//*******************************************************************
//*** exactly same file used in KioskCardLink \ KioskBioLink ********
//*******************************************************************

#include "SystemData.h"
#include "ServiceTimeData.h"

//*******************************************************************

CServiceTimeData::CServiceTimeData(void)
{
	for ( int nIndex = 0 ; nIndex < nMAX_SERVICETIMES ; nIndex++ )
	{
		Times[nIndex].bReqd			= FALSE;
		Times[nIndex].nStart		= 0;
		Times[nIndex].nEnd			= 2359;
		Times[nIndex].bBalanceOnly	= FALSE;
	}

	Times[0].bReqd		= TRUE;							// allow 00:00 to 23:59 as default in service range
	m_nCurrentIndex		= 0;
}

//*********************************************************************

bool CServiceTimeData::Read()
{
	bool bReply = FALSE;

	CSSFile file;
	if ( file.Open ( System.m_strPayLinkServiceFilename, "rb" ) == TRUE )
	{
		for ( int nIndex = 0 ; nIndex < nMAX_SERVICETIMES ; nIndex++ )
		{
			CCSV csv;
			if ( ( bReply = file.Read ( csv ) ) == FALSE )
				break;

			Times[nIndex].bReqd			= csv.GetBool(0);
			Times[nIndex].nStart		= csv.GetInt(1);
			Times[nIndex].nEnd			= csv.GetInt(2);
			Times[nIndex].bBalanceOnly	= csv.GetBool(3);
		}
		file.Close();
	}
	return bReply;
}

//*********************************************************************

bool CServiceTimeData::Write()
{
	bool bReply = FALSE;

	CSSFile file;
	if ( file.Open ( System.m_strPayLinkServiceFilename, "wb" ) == TRUE )
	{
		for ( int nIndex = 0 ; nIndex < nMAX_SERVICETIMES ; nIndex++ )
		{
			CCSV csv;
			csv.Add ( Times[nIndex].bReqd );
			csv.Add ( Times[nIndex].nStart );
			csv.Add ( Times[nIndex].nEnd );
			csv.Add ( Times[nIndex].bBalanceOnly );
			bReply = file.Write ( csv );
		}
		file.Close();
	}

	return bReply;
}

/**********************************************************************/

void CServiceTimeData::GetStartTime ( int nIndex, int& nHour, int &nMins )
{
	if ( nIndex >= 0 && nIndex < nMAX_SERVICETIMES )
	{
		nHour = Times[nIndex].nStart / 100;
		nMins = Times[nIndex].nStart % 100;
	}
}

void CServiceTimeData::GetEndTime ( int nIndex, int& nHour, int &nMins )
{
	if ( nIndex >= 0 && nIndex < nMAX_SERVICETIMES )
	{
		nHour = Times[nIndex].nEnd / 100;
		nMins = Times[nIndex].nEnd % 100;
	}
}

/**********************************************************************/

void CServiceTimeData::SetStartTime ( int nIndex, int nHour, int nMins )
{
	if ( nIndex >= 0 && nIndex < nMAX_SERVICETIMES )
	{
		if ( nHour >=0 && nHour < 24 )
		{
			if ( nMins >=0 && nMins < 60 )
			{
				Times[nIndex].nStart = ( nHour * 100 ) + nMins;
			}
		}
	}
}

void CServiceTimeData::SetEndTime ( int nIndex, int nHour, int nMins )
{
	if ( nIndex >= 0 && nIndex < nMAX_SERVICETIMES )
	{
		if ( nHour >=0 && nHour < 24 )
		{
			if ( nMins >=0 && nMins < 60 )
			{
				Times[nIndex].nEnd = ( nHour * 100 ) + nMins;
			}
		}
	}
}

/**********************************************************************/
// see if time in 1 of 5 Service ranges

bool CServiceTimeData::InService()
{
	CSSTime time;
	time.SetCurrentTime();

	int nIndex = InService ( time.GetHour(), time.GetMinute() );

	if ( nIndex == -1 )
		return FALSE;

	m_nCurrentIndex = nIndex;						// remember index for IsValidateOnly required
	return TRUE;
}

int CServiceTimeData::InService ( int nHour, int nMins )
{
	int nTimeNow = ( nHour * 100 ) + nMins;

	for ( int nIndex = 0 ; nIndex < nMAX_SERVICETIMES ; nIndex++ )
	{
		if ( CheckPeriod ( nIndex, nTimeNow ) == TRUE )
			return nIndex;
	}

	return -1;
}

/**********************************************************************/

bool CServiceTimeData::CheckPeriod ( int nIndex, int nTimeNow )
{
	if ( Times[nIndex].bReqd == TRUE )
	{
		if ( nTimeNow >= Times[nIndex].nStart && nTimeNow <= Times[nIndex].nEnd )
			return TRUE;
	}
	return FALSE;
}

/**********************************************************************/
