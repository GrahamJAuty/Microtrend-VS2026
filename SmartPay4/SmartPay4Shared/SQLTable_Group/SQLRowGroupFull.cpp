//**********************************************************************
#include "..\Consolidation.h"
#include "..\ReportConsolidationArray.h"
//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorGroup.h"
//**********************************************************************
#include "SQLRowGroupFull.h"
//**********************************************************************
static const char* szDAYS[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char* szWEEKDAYS[] = { "Sunday", "Monday", "Tueday", "Wednesday", "Thursday", "Friday", "Saturday" };
static const char* szMONTHS[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
//**********************************************************************

CSQLRowGroupFull::CSQLRowGroupFull()
{
	Reset();
}

//**********************************************************************

void CSQLRowGroupFull::Reset()
{
	m_nGroupNo = 1;
	m_strGroupName = "";
	m_nUserType = Group::UserType.Min;
	m_nSetNo = Group::SetNo.Min;
	m_strShiftGroups = "";
	m_nPriceLevel = Group::PriceLevel.Min;
	m_nVatShiftFlag = 0;
	m_bPurchaseControlFlag = FALSE;
	m_bAllowRevalKioskFlag = FALSE;
	/*****/
	m_nRefreshType = Group::RefreshType.Min;
	m_nRefreshFlags = Group::RefreshFlags.Min;
	m_dRefreshValue = Group::RefreshValue.Min;
	m_nRefreshAppendFlag = 0;
	m_nRefreshTypePeriod1 = Group::RefreshTypePeriod1.Min;
	m_nRefreshTypePeriod2 = Group::RefreshTypePeriod2.Min;
	m_nRefreshTypePeriod3 = Group::RefreshTypePeriod3.Min;
	m_nRefreshTypePeriod4 = Group::RefreshTypePeriod4.Min;
	m_nRefreshTypePeriod5 = Group::RefreshTypePeriod5.Min;
	m_nRefreshTypePeriod6 = Group::RefreshTypePeriod6.Min;
	m_dRefreshValuePeriod1 = 0.0;
	m_dRefreshValuePeriod2 = 0.0;
	m_dRefreshValuePeriod3 = 0.0;
	m_dRefreshValuePeriod4 = 0.0;
	m_dRefreshValuePeriod5 = 0.0;
	m_dRefreshValuePeriod6 = 0.0;
	/*****/
	m_nPurse1Allowed = Group::Purse1Allowed.Min;
	m_nPurse2Allowed = Group::Purse2Allowed.Min;
	m_nPurse3Allowed = Group::Purse3Allowed.Min;
	/*****/
	m_nMaxSpendType = Group::MaxSpendType.Min;
	/*****/
	m_dMaxSpendPurse1Period1 = 0.0;
	m_dMaxSpendPurse1Period2 = 0.0;
	m_dMaxSpendPurse1Period3 = 0.0;
	m_dMaxSpendPurse1Period4 = 0.0;
	m_dMaxSpendPurse1Period5 = 0.0;
	m_dMaxSpendPurse1Period6 = 0.0;
	/*****/
	m_dMaxSpendPurse3Period1 = 0.0;
	m_dMaxSpendPurse3Period2 = 0.0;
	m_dMaxSpendPurse3Period3 = 0.0;
	m_dMaxSpendPurse3Period4 = 0.0;
	m_dMaxSpendPurse3Period5 = 0.0;
	m_dMaxSpendPurse3Period6 = 0.0;
	/*****/
	m_dMaxOverdraftPurse1Period1 = 0.0;
	m_dMaxOverdraftPurse1Period2 = 0.0;
	m_dMaxOverdraftPurse1Period3 = 0.0;
	m_dMaxOverdraftPurse1Period4 = 0.0;
	m_dMaxOverdraftPurse1Period5 = 0.0;
	m_dMaxOverdraftPurse1Period6 = 0.0;
	/*****/
	m_dMaxOverdraftPurse3Period1 = 0.0;
	m_dMaxOverdraftPurse3Period2 = 0.0;
	m_dMaxOverdraftPurse3Period3 = 0.0;
	m_dMaxOverdraftPurse3Period4 = 0.0;
	m_dMaxOverdraftPurse3Period5 = 0.0;
	m_dMaxOverdraftPurse3Period6 = 0.0;
	/*****/
	m_strDeptInhibitPurse1 = "";
	m_strDeptInhibitPurse2 = "";
	m_strDeptInhibitPurse3 = "";
	m_strDeptInhibitCash = "";
	/*****/
	m_dLowBalancePurse1 = 0.0;
	m_dLowBalancePurse2 = 0.0;
	m_dLowBalancePurse3 = 0.0;
	/*****/
	m_nPointsAchievement = 0;
	/*****/
	m_bFileRevalueFlag = FALSE;
	m_bRevalueDisable = FALSE;
	m_nRevalueType = Group::RevalueType.Min;
	m_dRevalueTrip1 = 0.0;
	m_dRevalueTrip2 = 0.0;;
	m_nRevaluePoints = 0;
	m_dRevalueBonus = 0.0;
	/*****/
	m_bFileRewardFlag = FALSE;
	m_bRewardDisable = FALSE;
	m_nRewardType = Group::RewardType.Min;
	m_dRewardTrip = 0.0;
	m_nRewardPoints = 0;
	m_dRewardBonus = 0.0;
	m_dRewardLimit = 0.0;
	m_dRewardTrigger = 0.0;
	/*****/
	m_bFileRedeemFlag = FALSE;
	m_bRedeemDisable = FALSE;
	m_nRedeemTrip = 0;
	m_dRedeemValue = 0.0;
	m_nRedeemPurse = 1;
}

//**********************************************************************

void CSQLRowGroupFull::SetNewRecord()
{
	SetUserType(Group::UserType.Min);
	SetSetNo(Group::SetNo.Min);
	SetPriceLevel(Group::PriceLevel.Min);
	SetRefreshFlags(Group::RefreshFlags.Min);
	SetPurse1Allowed(Group::Purse1Allowed.Max);
	SetPurse2Allowed(Group::Purse2Allowed.Min);
	SetPurse3Allowed(Group::Purse3Allowed.Min);
}

//**********************************************************************

void CSQLRowGroupFull::SetRefreshAllowed(int nDay, bool bSet)
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

	SetRefreshFlags(nFlags);
}

//**********************************************************************

CString CSQLRowGroupFull::GetRefreshDaysText()
{
	int nCount = 0;
	CString strMsg = "";

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

CString CSQLRowGroupFull::GetRefreshWeekText()
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

int CSQLRowGroupFull::GetRefreshWeeklyDayNo(int nDefaultDayNo)
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

void CSQLRowGroupFull::SetRefreshWeeklyDayNo(int nDayNo)
{
	ClearRefreshFlags();
	SetRefreshAllowed(nDayNo, TRUE);
}

//**********************************************************************

CString CSQLRowGroupFull::GetRefreshMonthText()
{
	CString strMsg = "Once per Month";
	return strMsg;
}

//**********************************************************************

void CSQLRowGroupFull::GetRefreshYearDate(int& nDayNo, int& nMonthNo)
{
	int nFactor = GetRefreshFlags();
	if (nFactor > 100 && nFactor <= 1231)				// 01Jan = ( 1 x 100 + 1 ) = 31Dec = ( 12 x 100 + 31 )
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

void CSQLRowGroupFull::SetRefreshYearDate(int nDayNo, int nMonthNo)
{
	int nFactor = (nMonthNo * 100) + nDayNo;
	SetRefreshFlags(nFactor);
}

//**********************************************************************

CString CSQLRowGroupFull::GetRefreshYearText()
{
	int nDayNo = 0;
	int nMonthNo = 0;
	GetRefreshYearDate(nDayNo, nMonthNo);

	CString strMsg;
	strMsg.Format("%2.2d %s", nDayNo, szMONTHS[nMonthNo - 1]);

	return strMsg;
}

//**********************************************************************

bool CSQLRowGroupFull::IsRefreshAllowed(int nDay)
{
	bool bResult = FALSE;

	if (nDay >= 0)
	{
		int nMask = 0x01 << nDay;
		int nFlags = GetRefreshFlags();
		bResult = ((nFlags & nMask) == nMask);
	}
	return bResult;
}

//**********************************************************************

double CSQLRowGroupFull::GetRefreshValueByPeriod(int nIndex)
{
	double dResult = 0.0;

	switch (nIndex)
	{
	case 0:	dResult = m_dRefreshValuePeriod1;	break;
	case 1:	dResult = m_dRefreshValuePeriod2;	break;
	case 2:	dResult = m_dRefreshValuePeriod3;	break;
	case 3:	dResult = m_dRefreshValuePeriod4;	break;
	case 4:	dResult = m_dRefreshValuePeriod5;	break;
	case 5:	dResult = m_dRefreshValuePeriod6;	break;
	}

	return dResult;
}

//**********************************************************************

void CSQLRowGroupFull::SetRefreshValueByPeriod(int nIndex, double dValue)
{
	switch (nIndex)
	{
	case 0:	SetRefreshValuePeriod1(dValue);	break;
	case 1:	SetRefreshValuePeriod2(dValue);	break;
	case 2:	SetRefreshValuePeriod3(dValue);	break;
	case 3:	SetRefreshValuePeriod4(dValue);	break;
	case 4:	SetRefreshValuePeriod5(dValue);	break;
	case 5:	SetRefreshValuePeriod6(dValue);	break;
	}
}

//**********************************************************************

int CSQLRowGroupFull::GetRefreshTypeByPeriod(int nIndex)
{
	int nResult = 0;

	switch (nIndex)
	{
	case 0:	nResult = m_nRefreshTypePeriod1;	break;
	case 1:	nResult = m_nRefreshTypePeriod2;	break;
	case 2:	nResult = m_nRefreshTypePeriod3;	break;
	case 3:	nResult = m_nRefreshTypePeriod4;	break;
	case 4:	nResult = m_nRefreshTypePeriod5;	break;
	case 5:	nResult = m_nRefreshTypePeriod6;	break;
	}

	return nResult;
}

//**********************************************************************

void CSQLRowGroupFull::SetRefreshTypeByPeriod(int nIndex, int nType)
{
	switch (nIndex)
	{
	case 0:	SetRefreshTypePeriod1(nType);	break;
	case 1:	SetRefreshTypePeriod2(nType);	break;
	case 2:	SetRefreshTypePeriod3(nType);	break;
	case 3:	SetRefreshTypePeriod4(nType);	break;
	case 4:	SetRefreshTypePeriod5(nType);	break;
	case 5:	SetRefreshTypePeriod6(nType);	break;
	}
}

//**********************************************************************

CString CSQLRowGroupFull::GetRefreshTypeText()
{
	CString strText = "";
	switch (GetRefreshType())
	{
	case nREFRESHTYPE_DISABLED:		strText = "No";			break;
	case nREFRESHTYPE_DAILY:		strText = "Daily";		break;
	case nREFRESHTYPE_WEEKLY:		strText = "Weekly";		break;
	case nREFRESHTYPE_MONTHLY:		strText = "Monthly";	break;
	case nREFRESHTYPE_YEARLY:		strText = "Yearly";		break;
	case nREFRESHTYPE_MANUAL:		strText = "Manual";		break;
	case nREFRESHTYPE_PERIOD:		strText = "Period";		break;
	case nREFRESHTYPE_EOD:			strText = "EOD";		break;
	}
	return strText;
}

//**********************************************************************

bool CSQLRowGroupFull::ValidateDeptInhibitList(const char* szText)
{
	bool bReply = TRUE;
	CString strList = szText;

	for (int i = 0; i < strList.GetLength(); i++)
	{
		unsigned char uc = strList.GetAt(i);

		if (uc == ':' || uc == '-' || uc == ' ')				// see if separator
		{
			continue;
		}

		if (uc < '0' || uc > '9')
		{
			bReply = FALSE;
			break;
		}
	}
	return bReply;
}

//*********************************************************************
#ifndef SYSTEMTYPE_SERVER
//*********************************************************************

void CSQLRowGroupFull::DisplayDeptListError()
{
	Prompter.Error(
		"The list contains invalid characters!\n\nUse only digits 0 through 9, '-' or ':'.\n\nFor example:  1:20:25-30",
		"Department Inhibit List"
	);
}

//*********************************************************************
#endif
//*********************************************************************

bool CSQLRowGroupFull::IsPurseAllowed(int nPurseNo, int nPeriodIndex)
{
	int nFlags = 0;
	switch (nPurseNo)
	{
	case 1:	nFlags = m_nPurse1Allowed;	break;
	case 2:	nFlags = m_nPurse2Allowed;	break;
	case 3:	nFlags = m_nPurse3Allowed;	break;
	}

	int nMask = 0x01 << nPeriodIndex;

	return ((nFlags & nMask) == nMask) ? TRUE : FALSE;
}

//*********************************************************************

void CSQLRowGroupFull::SetPurseAllowed(int nPurseNo, int nPeriodIndex, bool bSet)
{
	int nFlags = 0;
	switch (nPurseNo)
	{
	case 1:	nFlags = m_nPurse1Allowed;	break;
	case 2:	nFlags = m_nPurse2Allowed;	break;
	case 3:	nFlags = m_nPurse3Allowed;	break;
	}

	int nMask = 0x01 << nPeriodIndex;
	
	if (bSet == TRUE)
	{
		nFlags |= nMask;
	}
	else
	{
		nFlags &= ~nMask;
	}

	switch (nPurseNo)
	{
	case 1:	SetPurse1Allowed(nFlags);	break;
	case 2:	SetPurse2Allowed(nFlags);	break;
	case 3:	SetPurse3Allowed(nFlags);	break;
	}
}

//*********************************************************************

bool CSQLRowGroupFull::IsPurse2Refreshable()
{
	bool bReply = TRUE;						//  assume Purse2 can be refreshed

	switch (GetRefreshType())
	{
	case nREFRESHTYPE_DISABLED:
	case nREFRESHTYPE_MANUAL:
		bReply = FALSE;
		break;

	case nREFRESHTYPE_DAILY:
	case nREFRESHTYPE_WEEKLY:
	case nREFRESHTYPE_MONTHLY:
	case nREFRESHTYPE_YEARLY:
	case nREFRESHTYPE_PERIOD:
	case nREFRESHTYPE_EOD:
	default:
		bReply = TRUE;
		break;
	}
	return bReply;
}

//*********************************************************************

CString CSQLRowGroupFull::GetListboxText()
{	
	CString strBuf = "";
	strBuf.Format("%3.3d : %s", m_nGroupNo, (const char*)m_strGroupName);
	return strBuf;
}

//*********************************************************************

CString CSQLRowGroupFull::GetRefreshPeriodTypeText(int nIndex)
{
	CString strText = "";

	switch (GetRefreshTypeByPeriod(nIndex))
	{
	case 0:	strText = "Daily";		break;
	case 1:	strText = "Weekly";		break;
	case 2:	strText = "Monthly";	break;
	case 3:	strText = "Yearly";		break;
	}

	return strText;
}

//*********************************************************************

bool CSQLRowGroupFull::HavePeriodRefresh()
{
	bool bReply = FALSE;

	for (int n = 0; n < 6; n++)
	{
		// see if period refesh value set
		if (GetRefreshValueByPeriod(n) != 0)
		{
			bReply = TRUE;
			break;
		}
	}

	return bReply;
}

//*********************************************************************

void CSQLRowGroupFull::ValidateShiftGroups(CString strIn, CString& strOut, CString& strError)
{
	strOut = "";
	strError = "";

	CReportConsolidationArray<CSortedIntItem> arrayShiftGroups;

	CCSV csvIn(strIn, ':');

	for (int n = 0; n < csvIn.GetSize(); n++)
	{
		CString str = csvIn.GetString(n);

		if (str == "")
		{
			continue;
		}

		if (::TestNumeric(str) == FALSE)
		{
			strError = "Please enter only numbers, separated by colons.";
			return;
		}

		str.TrimLeft("0");

		if ((str.GetLength() < 1) || (str.GetLength() > 2))
		{
			strError = "Valid group numbers are from 1 to 99.";
			return;
		}

		int nGroupNo = atoi(str);
		if (nGroupNo == m_nGroupNo)
		{
			strError = "You cannot shift a group to itself.";
			return;
		}

		CSortedIntItem item;
		item.m_nItem = nGroupNo;
		arrayShiftGroups.Consolidate(item);

		if (arrayShiftGroups.GetSize() > 10)
		{
			strError = "Please specify a maximum of 10 shift groups, separated by colons.";
			return;
		}
	}

	CCSV csvOut(':');
	for (int n = 0; n < arrayShiftGroups.GetSize(); n++)
	{
		CSortedIntItem item;
		arrayShiftGroups.GetAt(n, item);
		csvOut.Add(item.m_nItem);
	}

	strOut = csvOut.GetLine();
}

//*******************************************************************

bool CSQLRowGroupFull::GetActiveRevalueFlag()
{
	if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
	{
		return FALSE;
	}
	else
	{
		return m_bFileRevalueFlag;
	}
}

//*******************************************************************

bool CSQLRowGroupFull::GetActiveRewardFlag()
{
	if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
	{
		return FALSE;
	}
	else
	{
		return m_bFileRewardFlag;
	}
}

//*******************************************************************

bool CSQLRowGroupFull::GetActiveRedeemFlag()
{
	if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
	{
		return FALSE;
	}
	else
	{
		return m_bFileRedeemFlag;
	}
}

//*******************************************************************
