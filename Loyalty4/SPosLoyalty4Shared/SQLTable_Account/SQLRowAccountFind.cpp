//**********************************************************************
#include "..\SQLDefines.h"
//**********************************************************************
#include "SQLRowAccountFind.h"
//**********************************************************************

CSQLRowAccountFind::CSQLRowAccountFind()
{
	m_nUserID = 0;
	m_strFullname = "";
	m_strForename = "";
	m_strSurname = "";
	m_strExpiryDate = "";
	m_nGroupNo = 1;
	m_bInactive = FALSE;
	/*****/
	m_strInfo1 = "";
	m_strInfo2 = "";
	m_strInfo3 = "";
	m_strInfo4 = "";
	m_strAddress1 = "";
	m_strAddress2 = "";
	m_strAddress3 = "";
	m_strAddress4 = "";
	m_strAddress5 = "";
	m_strPhone1 = "";
	m_strPhone2 = "";
	m_strAlertCodes = "";
	/*****/
	m_nHotlistCode = 0;
	/*****/
	m_dPurse1SpendToDate = 0.0;
	m_dPurse2SpendToDate = 0.0;
	m_dCashSpendToDate = 0.0;
	m_nPointsToDate = 0;
}

//**********************************************************************

bool CSQLRowAccountFind::HasUserExpired()
{
	bool bHasExpired = FALSE;

	CSSDate dateExpiry(GetExpiryDate());
	if (dateExpiry.IsSet() == TRUE)
	{
		CSSDate dateNow;
		dateNow.SetCurrentDate();
		if (dateExpiry < dateNow)
		{
			bHasExpired = TRUE;
		}
	}

	return bHasExpired;
}

//**********************************************************************

int CSQLRowAccountFind::GetRecordStatusColour()
{
	int nColour = COLOUR_NORMAL;

	if (System.GetEnableAccountColourFlag() == TRUE)
	{
		if (IsCardHotlisted() == TRUE)
		{
			nColour = COLOUR_HOTLISTED;
		}
		else if (HasUserExpired() == TRUE)
		{
			nColour = COLOUR_EXPIRED;
		}
		else if (IsAlertSet() == TRUE)
		{
			nColour = COLOUR_ALERT;
		}
		else if (GetInactive() == TRUE)
		{
			nColour = COLOUR_INACTIVE;
		}
		else if (IsCardUsed() == FALSE)
		{
			nColour = COLOUR_NOTUSED;
		}
	}

	return nColour;
}

//******************************************************************************************

bool CSQLRowAccountFind::IsCardUsed()
{
	if (GetPointsToDate() != 0)
	{
		return TRUE;
	}

	if (GetPurse1SpendToDate() != 0.0)
	{
		return TRUE;
	}

	if (GetPurse2SpendToDate() != 0)
	{
		return TRUE;
	}

	if (GetCashSpendToDate() != 0)
	{
		return TRUE;
	}

	return FALSE;
}

//**********************************************************************
