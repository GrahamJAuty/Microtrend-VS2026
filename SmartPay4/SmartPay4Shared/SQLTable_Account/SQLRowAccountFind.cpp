//**********************************************************************
#include "SQLRowAccountFind.h"
//**********************************************************************
#define COLOUR_NORMAL		0x00		// black
#define COLOUR_INACTIVE		0x09		// light blue
#define COLOUR_EXPIRED		0x0c		// light red
#define COLOUR_ALERT		0x0d		// cyan
//**********************************************************************

CSQLRowAccountFind::CSQLRowAccountFind()
{
	m_nLeaverYear = 0;
	m_nUserID = 0;
	m_strUsername = "";
	m_strForename = "";
	m_strSurname = "";
	m_strExpiryDate = "";
	m_nGroupNo = 1;
	m_bInactive = FALSE;
	/*****/
	m_strMemberID = "";
	m_strInfo1 = "";
	m_strInfo2 = "";
	m_strInfo3 = "";
	m_strInfo4 = "";
	m_strInfo5 = "";
	m_strInfo6 = "";
	m_strInfo7 = "";
	m_strInfo8 = "";
	m_strMemberID = "";
	m_nAlertCode = 0;
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
		if (HasUserExpired() == TRUE)
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
	}

	return nColour;
}

//******************************************************************************************

CString CSQLRowAccountFind::GetUsername()
{
	CString strName = m_strUsername;

	if (strName == "")
	{
		strName = m_strSurname;
	}

	return strName;
}

//******************************************************************************************
