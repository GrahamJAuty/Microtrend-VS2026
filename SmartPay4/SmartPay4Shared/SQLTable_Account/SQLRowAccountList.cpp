//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRowAccountList.h"
//**********************************************************************
#define COLOUR_NORMAL		0x00		// black
#define COLOUR_INACTIVE		0x09		// light blue
#define COLOUR_EXPIRED		0x0c		// light red
#define COLOUR_ALERT		0x0d		// cyan
//**********************************************************************

CSQLRowAccountList::CSQLRowAccountList()
{
	m_nUserID = 0;
	m_strUsername = "";
	m_strForename = "";
	m_strSurname = "";
	m_strExpiryDate = "";
	m_nGroupNo = 1;
	m_bInactive = FALSE;
	/*****/
	m_dPurse1Liability = 0.0;
	m_dPurse1Credit = 0.0;
	/*****/
	m_dPurse2Balance = 0.0;
	/*****/
	m_dPurse3Liability = 0.0;
	m_dPurse3Credit = 0.0;
	/*****/
	m_nPoints = 0;
	/*****/
	m_nAlertCode = Account::AlertCode.Min;
	/*****/
	m_strMemberID = "";
	m_strInfo1 = "";
	/*****/
	m_bCardPickerFlag = FALSE;
}

//**********************************************************************

bool CSQLRowAccountList::HasUserExpired()
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

int CSQLRowAccountList::GetRecordStatusColour()
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

CString CSQLRowAccountList::GetGroupNoString()
{
	CString strMsg = "";
	strMsg.Format("%d", GetGroupNo());
	return strMsg;
}

//*********************************************************************

CString CSQLRowAccountList::GetPurseBalanceString(int nPurseNo)
{
	double dPurse = 0.0;

	switch (nPurseNo)
	{
	case 1:
		dPurse = GetPurse1Balance();
		break;

	case 2:
		dPurse = GetPurse2Balance();
		break;

	case 3:
		dPurse = GetPurse3Balance();
		break;
	}

	CString strMsg = "";
	strMsg.Format("%.2f", dPurse);
	return strMsg;
}

//*******************************************************************

CString CSQLRowAccountList::GetPointsString()
{
	CString strMsg = "";
	strMsg.Format("%d", GetPoints());		// v1.05b 23/07/2015
	return strMsg;
}

//*******************************************************************

CString CSQLRowAccountList::GetTotalBalanceString()
{
	CString strText = "";

	double dBalance = GetPurse1Balance() + GetPurse2Balance();

	if (TRUE == System.GetEnablePurse3Flag())
	{
		dBalance += GetPurse3Balance();
	}

	strText.Format("%.2lf", dBalance);

	return strText;
}

//*******************************************************************

CString CSQLRowAccountList::GetUsername()
{
	CString strName = m_strUsername;

	if (strName == "")
	{
		strName = m_strSurname;
	}

	return strName;
}

//******************************************************************************************

CString CSQLRowAccountList::GetACListRecordStatusText()
{
	CString strText = "    ";

	if (HasUserExpired() == TRUE)
	{
		strText += "EXPIRED";
	}
	else if (IsAlertSet() == TRUE)
	{
		strText += "ALERT";
	}
	else if (GetInactive() == TRUE)
	{
		strText += "INACTIVE";
	}
	else
	{
		strText = "";
	}
	
	return strText;
}

//******************************************************************************************