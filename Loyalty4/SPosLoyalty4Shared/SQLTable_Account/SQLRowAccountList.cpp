//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLRowAccountList.h"
//**********************************************************************

CSQLRowAccountList::CSQLRowAccountList()
{
	m_nUserID = 0;
	m_strFullname = "";
	m_nGroupNo = 1;
	m_strExpiryDate = "";
	m_strInfo1 = "";
	m_strInfo2 = "";
	m_strInfo3 = "";
	m_strInfo4 = "";
	m_strAddress1 = "";
	m_strAddress5 = "";
	m_bInactive = FALSE;
	/*****/
	m_dPurse1Balance = 0.0;
	m_dPurse1SpendToDate = 0.0;
	/*****/
	m_dPurse2Balance = 0.0;
	m_dPurse2SpendToDate = 0.0;
	/*****/
	m_dCashSpendToDate = 0.0;
	/*****/
	m_nPoints = 0;
	m_nPointsToDate = 0;
	/*****/
	m_nHotlistCode = 0;
	/*****/
	m_strAlertCodes = "";
	/*****/
	m_bCardPickerFlag = FALSE;
	m_bDataLoadedFlag = FALSE;
}

//**********************************************************************

bool CSQLRowAccountList::HasACListCardExpired()
{
	bool bHasExpired = FALSE;

	CSSDate dateExpiry(GetACListExpiryDate());
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

bool CSQLRowAccountList::IsACListCardUsed()
{
	if (GetACListPointsToDate() != 0)
	{
		return TRUE;
	}
	
	if (GetACListPurse1SpendToDate() != 0.0)
	{
		return TRUE;
	}

	if (GetACListPurse2SpendToDate() != 0)
	{
		return TRUE;
	}

	if (GetACListCashSpendToDate() != 0)
	{
		return TRUE;
	}

	return FALSE;
}

//**********************************************************************

int CSQLRowAccountList::GetACListRecordStatusColour()
{
	int nColour = COLOUR_NORMAL;
	
	if (System.GetEnableAccountColourFlag() == TRUE)
	{
		if (IsACListCardHotlisted() == TRUE)
		{
			nColour = COLOUR_HOTLISTED;
		}
		else if (HasACListCardExpired() == TRUE)
		{
			nColour = COLOUR_EXPIRED;
		}
		else if (IsACListAlertSet() == TRUE)
		{
			nColour = COLOUR_ALERT;
		}
		else if (GetACListInactive() == TRUE)
		{
			nColour = COLOUR_INACTIVE;
		}
		else if (IsACListCardUsed() == FALSE)
		{
			nColour = COLOUR_NOTUSED;
		}
	}

	return nColour;
}

//******************************************************************************************

CString CSQLRowAccountList::GetACListRecordStatusText()
{
	CString strText = "    ";

	if (IsACListCardHotlisted() == TRUE)
	{
		strText += "HOTLISTED";
	}
	else if (HasACListCardExpired() == TRUE)
	{
		strText += "EXPIRED";
	}
	else if (IsACListAlertSet() == TRUE)
	{
		strText += "ALERT";
	}
	else if (GetACListInactive() == TRUE)
	{
		strText += "INACTIVE";
	}
	else if (IsACListCardUsed() == FALSE)
	{
		strText += "UNUSED";
	}
	else
	{
		strText = "";
	}

	return strText;
}

//******************************************************************************************

CString CSQLRowAccountList::GetACListGroupNoString()
{
	CString strMsg = "";
	strMsg.Format("%d", GetACListGroupNo());
	return strMsg;
}

//*********************************************************************

CString CSQLRowAccountList::GetACListPurseBalanceString(int nPurseNo)
{
	double dPurse = 0.0;

	switch (nPurseNo)
	{
	case 1:
		dPurse = GetACListPurse1Balance();
		break;

	case 2:
		dPurse = GetACListPurse2Balance();
		break;
	}

	CString strMsg = "";
	strMsg.Format("%.2f", dPurse);
	return strMsg;
}

//*******************************************************************

CString CSQLRowAccountList::GetACListPointsString()
{
	CString strMsg = "";
	strMsg.Format("%d", GetACListPoints());		// v1.05b 23/07/2015
	return strMsg;
}

//*******************************************************************

CString CSQLRowAccountList::GetACListTotalBalanceString()
{
	CString strText = "";

	double dBalance = GetACListPurse1Balance() + GetACListPurse2Balance();

	strText.Format("%.2lf", dBalance);

	return strText;
}

//*******************************************************************

CString CSQLRowAccountList::GetACListPointsToDateString()
{
	CString strText = "";
	strText.Format("%d", m_nPointsToDate);
	return strText;
}

//*******************************************************************

CString CSQLRowAccountList::GetACListSpendToDateString()
{
	CString strText = "";
	strText.Format("%.2f", m_dPurse1SpendToDate + m_dPurse2SpendToDate + m_dCashSpendToDate);
	return strText;
}

//*******************************************************************
