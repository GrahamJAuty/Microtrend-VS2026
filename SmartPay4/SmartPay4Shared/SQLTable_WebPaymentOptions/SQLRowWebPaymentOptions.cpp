//**********************************************************************
#include "SQLRowWebPaymentOptions.h"
//**********************************************************************
static const char* szERROR1 = "This option is currently suspended as the time is outside the allotted period for web access!";
static const char* szERROR2 = "Balances have already been sent today!";
static const char* szERROR4 = "This option is currently suspended as today is outside the allotted days for web access!";
//$$******************************************************************

CSQLRowWebPaymentOptions::CSQLRowWebPaymentOptions()
{
	Reset();
}

//**********************************************************************

void CSQLRowWebPaymentOptions::Reset()
{
	m_nLineID = 1;
	m_nAccountLinkDays = 0x3E;
	m_strAccountLinkStart = "00:00";
	m_strAccountLinkEnd = "23:59";
	m_strAccountLinkLastDate = "";
	m_strAccountLinkLastTime = "";
	m_nPaymentDays = 0x3E;
	m_strPaymentStart = "00:00";
	m_strPaymentEnd = "23:59";
	m_strPaymentLastDate = "";
	m_strPaymentLastTime = "";
	m_strBalanceLastDate = "";
	m_strBalanceLastTime = "";
	m_strTransactionLastDate = "";
	m_strTransactionLastTime = "";
	m_bExclude1Flag = FALSE;
	m_strExclude1Start = "00:00";
	m_strExclude1End = "00:00";
	m_bExclude2Flag = FALSE;
	m_strExclude2Start = "00:00";
	m_strExclude2End = "00:00";
	m_bExclude3Flag = FALSE;
	m_strExclude3Start = "00:00";
	m_strExclude3End = "00:00";
	m_bExclude4Flag = FALSE;
	m_strExclude4Start = "00:00";
	m_strExclude4End = "00:00";
	m_strExcludeUploadStart = "00:00";
	m_strExcludeUploadEnd = "00:00";
	m_bEODAfterPaymentFlag = TRUE;
	m_strEODAfterTime = "16:00";
	m_bPromptBeforeAccessFlag = TRUE;
	m_bImportAccountsFlag = TRUE;
}

//**********************************************************************

bool CSQLRowWebPaymentOptions::AccountLinkAllowed(int nWebPaymentType)
{
	bool bAllowed = FALSE;

	if (-1 == nWebPaymentType)
	{
		nWebPaymentType = System.GetWebPaymentType();
	}

	switch (nWebPaymentType)
	{
	case nWEBPAYMENT_PEBBLE:
		bAllowed = TRUE;
		break;

	case nWEBPAYMENT_PARENTPAY:
	case nWEBPAYMENT_SQUID:
	case nWEBPAYMENT_SCHOOLCOMMS:
	case nWEBPAYMENT_WISEPAY:
	case nWEBPAYMENT_BUCKINGHAM:
	case nWEBPAYMENT_TUCASI:
	case nWEBPAYMENT_TUCASIv2:
	case nWEBPAYMENT_PARENTMAIL:
	case nWEBPAYMENT_BROMCOM:
	default:
		break;
	}

	return bAllowed;
}

//**********************************************************************

bool CSQLRowWebPaymentOptions::IsAccountLinkDay(int nDay)
{
	if ((nDay < 0) || (nDay > 6))
	{
		return FALSE;
	}

	int nMask = 0x01 << nDay;

	return ((m_nAccountLinkDays & nMask) == nMask);
}

//**********************************************************************

void CSQLRowWebPaymentOptions::SetAccountLinkDay(int nDay, bool bSet)
{
	if ((nDay < 0) || (nDay > 6))
	{
		return;
	}

	int nMask = 0x01 << nDay;

	if (true == bSet)
	{
		m_nAccountLinkDays |= nMask;
	}
	else
	{
		m_nAccountLinkDays &= ~nMask;
	}
}

//$$******************************************************************

bool CSQLRowWebPaymentOptions::IsPaymentDay(int nDay)
{
	if ((nDay < 0) || (nDay > 6))
	{
		return FALSE;
	}

	int nMask = 0x01 << nDay;

	return ((m_nPaymentDays & nMask) == nMask);
}

//$$******************************************************************

void CSQLRowWebPaymentOptions::SetPaymentDay(int nDay, bool bSet)
{
	if ((nDay < 0) || (nDay > 6))
	{
		return;
	}

	int nMask = 0x01 << nDay;

	if (true == bSet)
	{
		m_nPaymentDays |= nMask;
	}
	else
	{
		m_nPaymentDays &= ~nMask;
	}
}

//$$******************************************************************

bool CSQLRowWebPaymentOptions::ImportAccountsService()
{
	switch (System.GetWebPaymentType())
	{
	case nWEBPAYMENT_PARENTPAY:
	case nWEBPAYMENT_PARENTMAIL:
	case nWEBPAYMENT_SQUID:
	case nWEBPAYMENT_WISEPAY:
	case nWEBPAYMENT_PEBBLE:
		return TRUE;

	case nWEBPAYMENT_TUCASI:
	case nWEBPAYMENT_TUCASIv2:
	case nWEBPAYMENT_BUCKINGHAM:
	case nWEBPAYMENT_SCHOOLCOMMS:
	case nWEBPAYMENT_BROMCOM:
	default:
		return FALSE;
	}
}

//$$******************************************************************

bool CSQLRowWebPaymentOptions::EODAllowed(int nWebPaymentType)
{
	bool bEODAllowed = TRUE;

	if (-1 == nWebPaymentType)
	{
		nWebPaymentType = System.GetWebPaymentType();
	}

	switch (nWebPaymentType)
	{
	case nWEBPAYMENT_PARENTPAY:
	case nWEBPAYMENT_SQUID:
	case nWEBPAYMENT_SCHOOLCOMMS:
	case nWEBPAYMENT_WISEPAY:
	case nWEBPAYMENT_BUCKINGHAM:
	case nWEBPAYMENT_PEBBLE:
		bEODAllowed = FALSE;
		break;

	case nWEBPAYMENT_TUCASI:
	case nWEBPAYMENT_TUCASIv2:
	case nWEBPAYMENT_PARENTMAIL:
	case nWEBPAYMENT_BROMCOM:
	default:
		break;
	}

	return bEODAllowed;
}

//$$******************************************************************

void CSQLRowWebPaymentOptions::SetLastPaymentDone()
{
	m_strPaymentLastDate = CSSDate::GetCurrentDate().GetDate();
	m_strPaymentLastTime = CSSTime::GetCurrentTime().GetTime();
}

//$$******************************************************************

void CSQLRowWebPaymentOptions::SetLastTransactionDone()
{
	m_strTransactionLastDate = CSSDate::GetCurrentDate().GetDate();
	m_strTransactionLastTime = CSSTime::GetCurrentTime().GetTime();
}

//$$******************************************************************

void CSQLRowWebPaymentOptions::ClearLastTransactionDone()
{
	m_strTransactionLastDate = "";
	m_strTransactionLastTime = "";
}

//$$******************************************************************

void CSQLRowWebPaymentOptions::SetLastBalanceDone()
{
	m_strBalanceLastDate = CSSDate::GetCurrentDate().GetDate();
	m_strBalanceLastTime = CSSTime::GetCurrentTime().GetTime();
}

//$$******************************************************************

void CSQLRowWebPaymentOptions::ClearLastBalanceDone()
{
	m_strBalanceLastDate = "";
	m_strBalanceLastTime = "";
}

//$$******************************************************************

void CSQLRowWebPaymentOptions::SetLastAccountLinkDone()
{
	m_strAccountLinkLastDate = CSSDate::GetCurrentDate().GetDate();
	m_strAccountLinkLastTime = CSSTime::GetCurrentTime().GetTime();
}

//$$******************************************************************

void CSQLRowWebPaymentOptions::ClearLastAccountLinkDone()
{
	m_strAccountLinkLastDate = "";
	m_strAccountLinkLastTime = "";
}

//$$******************************************************************

CString CSQLRowWebPaymentOptions::GetImportMenuCaption()
{
	CString strMenuCaption = "N/A";

	switch (System.GetWebPaymentType())
	{
	case nWEBPAYMENT_SQUID:
	case nWEBPAYMENT_PARENTMAIL:
		strMenuCaption = "Link Accounts";
		break;

	case nWEBPAYMENT_PARENTPAY:
	case nWEBPAYMENT_WISEPAY:
		strMenuCaption = "Import Account Details";
		break;

	case nWEBPAYMENT_PEBBLE:
		strMenuCaption = "Link Pebble Members";
		break;

	case nWEBPAYMENT_TUCASI:
	case nWEBPAYMENT_TUCASIv2:
	case nWEBPAYMENT_BUCKINGHAM:
	case nWEBPAYMENT_SCHOOLCOMMS:
	case nWEBPAYMENT_BROMCOM:
	default:
		break;
	}

	return 	strMenuCaption;
}

//$$******************************************************************

bool CSQLRowWebPaymentOptions::CheckTimeInRange(bool bCheck, CSSTime& timeNow, CString& strStart, CString& strEnd)
{
	if (FALSE == bCheck)
	{
		return FALSE;
	}

	CSSTime timeCheck;

	timeCheck.SetTime(strStart);
	if (timeNow < timeCheck)
	{
		return FALSE;
	}

	timeCheck.SetTime(strEnd);
	if (timeNow > timeCheck)
	{
		return FALSE;
	}

	return TRUE;
}

//$$******************************************************************

bool CSQLRowWebPaymentOptions::TimeForEndOfDay()
{
	if (System.GetWebPaymentType() == nWEBPAYMENT_SCHOOLCOMMS)		// schoolcomms can be sent anytime
	{
		return TRUE;
	}

	m_strError = "";

	CSSTime timeNow;
	timeNow.SetCurrentTime();

	CSSTime timeCheck;
	timeCheck.SetTime(m_strEODAfterTime);

	if (timeNow < timeCheck)
	{
		m_strError = szERROR1;
	}

	CSSDate dateToday;
	dateToday.SetCurrentDate();

	CSSDate dateLast(m_strBalanceLastDate);
	if (dateLast >= dateToday)
	{
		if (m_strError != "")
		{
			m_strError += "\n\n";
		}

		m_strError += szERROR2;
	}

	return (m_strError == "");
}

//$$******************************************************************

bool CSQLRowWebPaymentOptions::TimeForUpload()
{
	if ((m_strExcludeUploadStart == "00:00") && (m_strExcludeUploadEnd == "00:00"))
	{
		return TRUE;
	}

	CSSTime timeNow;
	timeNow.SetCurrentTime();

	if (CheckTimeInRange(TRUE, timeNow, m_strExcludeUploadStart, m_strExcludeUploadEnd) == TRUE)
	{
		return FALSE;
	}

	return TRUE;
}

//$$******************************************************************

bool CSQLRowWebPaymentOptions::TimeForImports()
{
	CSSDate dateNow{};
	dateNow.SetCurrentDate();

	int nDay = dateNow.GetDayOfWeek() - 1;							// 1=sun .. 7 = sat, 0=not set
	if (IsPaymentDay(nDay) == FALSE)
	{
		m_strError = szERROR4;										// outside allotted day
		return FALSE;
	}

	CSSTime timeNow;
	timeNow.SetCurrentTime();

	if (CheckTimeInRange(TRUE, timeNow, m_strPaymentStart, m_strPaymentEnd) == FALSE)
	{
		m_strError = szERROR1;
		return FALSE;
	}

	if (CheckTimeInRange(m_bExclude1Flag, timeNow, m_strExclude1Start, m_strExclude1End) == TRUE)
	{
		m_strError = szERROR1;
		return FALSE;
	}

	if (CheckTimeInRange(m_bExclude2Flag, timeNow, m_strExclude2Start, m_strExclude2End) == TRUE)
	{
		m_strError = szERROR1;
		return FALSE;
	}

	if (CheckTimeInRange(m_bExclude3Flag, timeNow, m_strExclude3Start, m_strExclude3End) == TRUE)
	{
		m_strError = szERROR1;
		return FALSE;
	}

	if (CheckTimeInRange(m_bExclude4Flag, timeNow, m_strExclude4Start, m_strExclude4End) == TRUE)
	{
		m_strError = szERROR1;
		return FALSE;
	}

	return TRUE;
}

//$$******************************************************************

bool CSQLRowWebPaymentOptions::TimeForAccountLink()
{
	CSSDate dateNow{};
	dateNow.SetCurrentDate();

	int nDay = dateNow.GetDayOfWeek() - 1;							// 1=sun .. 7 = sat, 0=not set
	if (IsAccountLinkDay(nDay) == FALSE)
	{
		m_strError = szERROR4;										// outside allotted day
		return FALSE;
	}

	CSSTime timeNow;
	timeNow.SetCurrentTime();

	if (CheckTimeInRange(TRUE, timeNow, m_strAccountLinkStart, m_strAccountLinkEnd) == FALSE)
	{
		m_strError = szERROR1;
		return FALSE;
	}

	return TRUE;
}

//$$******************************************************************

void CSQLRowWebPaymentOptions::SetRowVersion(CByteArray& arraySource)
{
	m_arrayRowVersion.RemoveAll();
	for (int n = 0; n < arraySource.GetSize(); n++)
	{
		m_arrayRowVersion.Add(arraySource.GetAt(n));
	}
}

//**********************************************************************

bool CSQLRowWebPaymentOptions::CompareRowVersion(CSQLRowWebPaymentOptions& RowSource)
{
	bool bResult = FALSE;

	if (m_arrayRowVersion.GetSize() == RowSource.m_arrayRowVersion.GetSize())
	{
		bResult = TRUE;
		for (int n = 0; n < m_arrayRowVersion.GetSize(); n++)
		{
			if (m_arrayRowVersion.GetAt(n) != RowSource.m_arrayRowVersion.GetAt(n))
			{
				bResult = FALSE;
				break;
			}
		}
	}

	return bResult;
}

//**********************************************************************

CString CSQLRowWebPaymentOptions::GetRowVersionString()
{
	CString strRowVersion = "";
	for (int n = 0; n < m_arrayRowVersion.GetSize(); n++)
	{
		CString strPart = "";
		strPart.Format("%02X", m_arrayRowVersion.GetAt(n));
		strRowVersion += strPart;
	}
	return strRowVersion;
}

//**********************************************************************
