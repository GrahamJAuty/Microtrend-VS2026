//**********************************************************************
#include "..\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorGroup.h"
//**********************************************************************
#include "SQLRowGroup.h"
//**********************************************************************
static const char* szDAYS[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char* szWEEKDAYS[] = { "Sunday", "Monday", "Tueday", "Wednesday", "Thursday", "Friday", "Saturday" };
static const char* szMONTHS[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
//**********************************************************************

CSQLRowGroup::CSQLRowGroup()
{
	Reset();
}

//**********************************************************************

void CSQLRowGroup::Reset()
{
	m_nGroupNo = 1;
	m_strGroupName = "";
	m_nSchemeNo = Group::SchemeNo.Min;
	m_bStampOfferFlag = FALSE;
	/*****/
	m_nRefreshType = Group::RefreshType.Min;
	m_nRefreshFlags = Group::RefreshFlags.Min;
	m_dRefreshValue = Group::RefreshValue.Min;
	m_nRefreshAppend = 0;
	/*****/
	m_nPurse1Allowed = Group::Purse1Allowed.Min;
	m_nPurse2Allowed = Group::Purse2Allowed.Min;
	/*****/
	m_bNoPluBonus = FALSE;
	m_bNoDeptBonus = FALSE;
	/*****/
	m_nMaxSpendType = Group::MaxSpendType.Min;
	/*****/
	m_dMaxSpendPeriod1 = 0.0;
	m_dMaxSpendPeriod2 = 0.0;
	m_dMaxSpendPeriod3 = 0.0;
	m_dMaxSpendPeriod4 = 0.0;
	m_dMaxSpendPeriod5 = 0.0;
	m_dMaxSpendPeriod6 = 0.0;
	/*****/
	m_dMaxOverdraftPeriod1 = 0.0;
	m_dMaxOverdraftPeriod2 = 0.0;
	m_dMaxOverdraftPeriod3 = 0.0;
	m_dMaxOverdraftPeriod4 = 0.0;
	m_dMaxOverdraftPeriod5 = 0.0;
	m_dMaxOverdraftPeriod6 = 0.0;
	/*****/
	m_bRewardFlag = FALSE;
	m_bRewardDisable = FALSE;
	m_nRewardType = Group::RewardType.Min;
	m_dRewardTrip = 0.0;
	m_nRewardPoints = 0;
	m_dRewardBonus = 0.0;
	m_dRewardLimit = 0.0;
	m_dRewardTrigger = 0.0;
	/*****/
	m_bRedeemFlag = FALSE;
	m_bRedeemDisable = FALSE;
	m_nRedeemType = 0;
	m_nRedeemTrip = 0;
	m_dRedeemValue = 0.0;
}

//**********************************************************************

bool CSQLRowGroup::IsRefreshAllowed(int nDay)
{
	bool bResult = FALSE;

	if ((nDay >= 0) && (nDay <= 6))
	{
		int nFlags = GetRefreshFlags();
		int nMask = 0x01 << nDay;
		bResult = ((nFlags & nMask) == nMask);
	}

	return bResult;
}

//**********************************************************************

int CSQLRowGroup::GetRefreshWeeklyDayNo(int nDefaultDayNo)
{
	for (int nDay = 0; nDay < 7; nDay++)
	{
		if (IsRefreshAllowed(nDay) == TRUE)
		{
			return nDay;
		}
	}
	return nDefaultDayNo;										// 0=Sun, 1-Mon etc
}

//**********************************************************************

void CSQLRowGroup::SetRefreshWeeklyDayNo(int nDayNo)
{
	ClearRefreshFlags();
	SetRefreshAllowed(nDayNo, TRUE);
}

//**********************************************************************

void CSQLRowGroup::GetRefreshYearDate(int& nDayNo, int& nMonthNo)
{
	int nFactor = GetRefreshFlags();

	if (ValidateYearlyRefreshFactor(nFactor) == TRUE)
	{
		nMonthNo = nFactor / 100;
		nDayNo = nFactor - (nMonthNo * 100);
	}
	else
	{
		nDayNo = 1;
		nMonthNo = 1;
	}
}

//**********************************************************************

void CSQLRowGroup::SetRefreshYearDate(int nDayNo, int nMonthNo)
{
	if ((nMonthNo >= 1) && (nMonthNo <= 12))
	{
		if ((nDayNo >= 1) && (nDayNo <= 31))
		{
			int nFactor = (nMonthNo * 100) + nDayNo;

			if (ValidateYearlyRefreshFactor(nFactor) == TRUE)
			{
				SetRefreshFlags(nFactor);
			}
		}
	}
}

//**********************************************************************

bool CSQLRowGroup::ValidateYearlyRefreshFactor(int nFactor)
{
	if (nFactor < 101)
	{
		return FALSE;
	}

	int nMonth = nFactor / 100;
	
	int nMaxDay = 0;
	switch (nMonth)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		nMaxDay = 31;
		break;

	case 4:
	case 6:
	case 9:
	case 11:
		nMaxDay = 30;
		break;

	case 2:
		nMaxDay = 28;
		break;
	}

	if (0 == nMaxDay)
	{
		return FALSE;
	}

	return ((nMaxDay != 0) && ((nFactor % 100) <= nMaxDay));
}

//**********************************************************************

void CSQLRowGroup::SetRefreshAllowed(int nDay, bool bSet)
{
	if ((nDay >= 0) && (nDay <= 6))
	{
		int nFlags = GetRefreshFlags();
		int nMask = 0x01 << nDay;

		if (bSet == TRUE)
		{
			nFlags |= nMask;
		}
		else
		{
			nFlags &= ~nMask;
		}

		nFlags &= 0x7F;

		SetRefreshFlags(nFlags);
	}
}

//**********************************************************************

CString CSQLRowGroup::GetRefreshDaysText()
{
	int nCount = 0;
	CString strMsg;

	for (int nDay = 0; nDay < 7; nDay++)
	{
		if (IsRefreshAllowed(nDay) == TRUE)
		{
			if (nCount > 0)
			{
				strMsg += ", ";
			}

			strMsg += szDAYS[nDay];
			++nCount;
		}
	}
	return strMsg;
}

//**********************************************************************

CString CSQLRowGroup::GetRefreshWeekText()
{
	CString strMsg = "";
	for (int nDay = 0; nDay < 7; nDay++)
	{
		if (IsRefreshAllowed(nDay) == TRUE)
		{
			strMsg = szWEEKDAYS[nDay];
			break;
		}
	}
	return strMsg;
}

//**********************************************************************

CString CSQLRowGroup::GetRefreshMonthText()
{
	CString strMsg = "Once per Month";
	return strMsg;
}

//**********************************************************************

CString CSQLRowGroup::GetRefreshYearText()
{
	int nDayNo = 0;
	int nMonthNo = 0;
	GetRefreshYearDate(nDayNo, nMonthNo);

	CString strMsg;
	strMsg.Format("%2.2d %s", nDayNo, szMONTHS[nMonthNo - 1]);

	return strMsg;
}

//**********************************************************************

CString CSQLRowGroup::GetRefreshTypeText()
{
	CString strText = "";
	switch (GetRefreshType())
	{
	case nREFRESHTYPE_NORMAL:	
		strText = "No";			
		break;

	case nREFRESHTYPE_DAILY:
		strText = "Daily";	
		break;

	case nREFRESHTYPE_WEEKLY:
		strText = "Weekly";	
		break;

	case nREFRESHTYPE_MONTHLY:
		strText = "Monthly";	
		break;

	case nREFRESHTYPE_YEARLY:
		strText = "Yearly";		
		break;
	}

	return strText;
}

//**********************************************************************

bool CSQLRowGroup::IsPurseAllowed(int nPurseNo, int nPeriodIndex)
{
	int nFlags = 0;

	if (1 == nPurseNo)
	{
		nFlags = GetPurse1Allowed();
	}
	else
	{
		nFlags = GetPurse2Allowed();
	}

	int nMask = 0x01;
	nMask = nMask << nPeriodIndex;
	return ((nFlags & nMask) == nMask);
}

//**********************************************************************
#ifdef LOYSERVER
//**********************************************************************

bool CSQLRowGroup::IsPurseAllowedAtTime(int nPurseNo, CSSTime& time)
{
	if (2 == nPurseNo)
	{
		if (System.GetEnablePurse2Flag() == FALSE)
		{
			return FALSE;
		}
	}

	if (GetMaxSpendType() == nSPENDTYPE_PERIOD)
	{
		int nPeriod = SQLRowSetAuditPeriod.GetPeriodIndex(time.GetTime());
		return IsPurseAllowed(nPurseNo, nPeriod);
	}

	return TRUE;
}

//**********************************************************************
#endif
//**********************************************************************

void CSQLRowGroup::SetPurseAllowed(int nPurseNo, int nPeriodIndex, bool bSet)
{
	int nFlags = 0;
	
	if (1 == nPurseNo)
	{
		nFlags = GetPurse1Allowed();
	}
	else
	{
		nFlags = GetPurse2Allowed();
	}

	int nMask = 0x01;
	nMask = nMask << nPeriodIndex;
	
	if (TRUE == bSet)
	{
		nFlags |= nMask;
	}
	else
	{
		nFlags &= ~nMask;
	}

	if (1 == nPurseNo)
	{
		SetPurse1Allowed(nFlags);
	}
	else
	{
		SetPurse2Allowed(nFlags);
	}
}

//*********************************************************************

void CSQLRowGroup::Add(CSQLRowGroup& source)
{
}

/**********************************************************************/

int CSQLRowGroup::Compare(CSQLRowGroup& source, int nHint)
{
	if (m_nGroupNo != source.m_nGroupNo)
	{
		return (m_nGroupNo > source.m_nGroupNo ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
