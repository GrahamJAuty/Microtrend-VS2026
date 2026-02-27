//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLTable_Group\SQLRepositoryGroup.h"
//**********************************************************************
#include "SQLFieldValidatorAccount.h"
//**********************************************************************
#include "SQLRowAccountFull.h"
//**********************************************************************
#define COLOUR_NORMAL		0x00		// black
#define COLOUR_INACTIVE		0x09		// light blue
#define COLOUR_EXPIRED		0x0c		// light red
#define COLOUR_ALERT		0x0d		// cyan
//**********************************************************************
static const char* szDAYS[] = { "", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
//**********************************************************************

CSQLRowAccountFull::CSQLRowAccountFull()
{
	Reset();
}

//**********************************************************************

CSQLRowAccountFull::CSQLRowAccountFull(const CSQLRowAccountFull& source)
{
	*this = source;
}

//**********************************************************************

CSQLRowAccountFull& CSQLRowAccountFull::operator= (const CSQLRowAccountFull& source)
{
	m_nLeaverYear = source.m_nLeaverYear;
	m_nUserID = source.m_nUserID;
	m_strUsername = source.m_strUsername;
	m_strForename = source.m_strForename;
	m_strSurname = source.m_strSurname;
	m_strDOB = source.m_strDOB;
	m_strExpiryDate = source.m_strExpiryDate;
	m_strGender = source.m_strGender;
	m_nGroupNo = source.m_nGroupNo;
	m_strDayGroups = source.m_strDayGroups;
	m_nGroupShiftStatus = source.m_nGroupShiftStatus;
	m_strGroupShiftStartDate = source.m_strGroupShiftStartDate;
	m_strGroupShiftEndDate = source.m_strGroupShiftEndDate;
	m_nGroupShiftFutureGroup = source.m_nGroupShiftFutureGroup;
	m_strEmail = source.m_strEmail;
	m_strInfo1 = source.m_strInfo1;
	m_strInfo2 = source.m_strInfo2;
	m_strInfo3 = source.m_strInfo3;
	m_strInfo4 = source.m_strInfo4;
	m_strInfo5 = source.m_strInfo5;
	m_strInfo6 = source.m_strInfo6;
	m_strInfo7 = source.m_strInfo7;
	m_strInfo8 = source.m_strInfo8;
	m_bTick1 = source.m_bTick1;
	m_bTick2 = source.m_bTick2;
	m_bTick3 = source.m_bTick3;
	m_bTick4 = source.m_bTick4;
	m_nAllergyCodes = source.m_nAllergyCodes;
	m_strDietary = source.m_strDietary;
	m_bInactive = source.m_bInactive;
	/*****/
	m_strMemberID = source.m_strMemberID;
	m_strExternalRef1 = source.m_strExternalRef1;
	m_strExternalRef2 = source.m_strExternalRef2;
	m_nPreviousUserID = source.m_nPreviousUserID;
	m_nNextUserID = source.m_nNextUserID;
	m_strLegacyPebblePurseID = source.m_strLegacyPebblePurseID;
	/*****/
	m_nPurse1Allow = source.m_nPurse1Allow;
	m_dPurse1Liability = source.m_dPurse1Liability;
	m_dPurse1Credit = source.m_dPurse1Credit;
	m_strPurse1LastSpendDate = source.m_strPurse2LastSpendDate;
	m_strPurse1LastSpendTime = source.m_strPurse2LastSpendTime;
	m_dPurse1LastSpend = source.m_dPurse1LastSpend;
	m_dPurse1SpendToDate = source.m_dPurse2SpendToDate;
	/*****/
	m_nPurse2Allow = source.m_nPurse2Allow;
	m_dPurse2Balance = source.m_dPurse2Balance;
	m_strPurse2LastSpendDate = source.m_strPurse2LastSpendDate;
	m_strPurse2LastSpendTime = source.m_strPurse2LastSpendTime;
	m_dPurse2LastSpend = source.m_dPurse2LastSpend;
	m_dPurse2SpendToDate = source.m_dPurse2SpendToDate;
	m_nPurse2RefreshFlags = source.m_nPurse2RefreshFlags;
	m_strPurse2RefreshedDate = source.m_strPurse2RefreshedDate;
	m_strPurse2RefreshedTime = source.m_strPurse2RefreshedTime;
	/*****/
	m_nPurse3Allow = source.m_nPurse3Allow;
	m_dPurse3Liability = source.m_dPurse3Liability;
	m_dPurse3Credit = source.m_dPurse3Credit;
	m_strPurse3LastSpendDate = source.m_strPurse3LastSpendDate;
	m_strPurse3LastSpendTime = source.m_strPurse3LastSpendTime;
	m_dPurse3LastSpend = source.m_dPurse3LastSpend;
	m_dPurse3SpendToDate = source.m_dPurse3SpendToDate;
	/*****/
	m_dCashSpendToDate = source.m_dCashSpendToDate;
	/*****/
	m_nPoints = source.m_nPoints;
	m_nPointsToDate = source.m_nPointsToDate;
	/*****/
	m_bOwnMaxSpend = source.m_bOwnMaxSpend;
	/*****/
	m_dMaxSpendPurse1Period1 = source.m_dMaxSpendPurse1Period1;
	m_dMaxSpendPurse1Period2 = source.m_dMaxSpendPurse1Period2;
	m_dMaxSpendPurse1Period3 = source.m_dMaxSpendPurse1Period3;
	m_dMaxSpendPurse1Period4 = source.m_dMaxSpendPurse1Period4;
	m_dMaxSpendPurse1Period5 = source.m_dMaxSpendPurse1Period5;
	m_dMaxSpendPurse1Period6 = source.m_dMaxSpendPurse1Period6;
	/*****/
	m_dMaxSpendPurse3Period1 = source.m_dMaxSpendPurse3Period1;
	m_dMaxSpendPurse3Period2 = source.m_dMaxSpendPurse3Period2;
	m_dMaxSpendPurse3Period3 = source.m_dMaxSpendPurse3Period3;
	m_dMaxSpendPurse3Period4 = source.m_dMaxSpendPurse3Period4;
	m_dMaxSpendPurse3Period5 = source.m_dMaxSpendPurse3Period5;
	m_dMaxSpendPurse3Period6 = source.m_dMaxSpendPurse3Period6;
	/*****/
	m_dMaxOverdraftPurse1Period1 = source.m_dMaxOverdraftPurse1Period1;
	m_dMaxOverdraftPurse1Period2 = source.m_dMaxOverdraftPurse1Period2;
	m_dMaxOverdraftPurse1Period3 = source.m_dMaxOverdraftPurse1Period3;
	m_dMaxOverdraftPurse1Period4 = source.m_dMaxOverdraftPurse1Period4;
	m_dMaxOverdraftPurse1Period5 = source.m_dMaxOverdraftPurse1Period5;
	m_dMaxOverdraftPurse1Period6 = source.m_dMaxOverdraftPurse1Period6;
	/*****/
	m_dMaxOverdraftPurse3Period1 = source.m_dMaxOverdraftPurse3Period1;
	m_dMaxOverdraftPurse3Period2 = source.m_dMaxOverdraftPurse3Period2;
	m_dMaxOverdraftPurse3Period3 = source.m_dMaxOverdraftPurse3Period3;
	m_dMaxOverdraftPurse3Period4 = source.m_dMaxOverdraftPurse3Period4;
	m_dMaxOverdraftPurse3Period5 = source.m_dMaxOverdraftPurse3Period5;
	m_dMaxOverdraftPurse3Period6 = source.m_dMaxOverdraftPurse3Period6;
	/*****/
	m_nAlertCode = source.m_nAlertCode;
	m_strAlertDate = source.m_strAlertDate;
	m_strAlertTime = source.m_strAlertTime;

	for (int n = 0; n < source.m_arrayRowVersion.GetSize(); n++)
	{
		m_arrayRowVersion.Add(source.m_arrayRowVersion.GetAt(n));
	}

	return *this;
}

//**********************************************************************

void CSQLRowAccountFull::Reset()
{
	m_nLeaverYear = 2000;
	m_nUserID = 0;
	m_strUsername = "";
	m_strForename = "";
	m_strSurname = "";
	m_strDOB = "";
	m_strExpiryDate = "";
	m_strGender = "";
	m_nGroupNo = 1;
	m_strDayGroups = "";
	m_nGroupShiftStatus = 0;
	m_strGroupShiftStartDate = "";
	m_strGroupShiftEndDate = "";
	m_nGroupShiftFutureGroup = 0;
	m_strEmail = "";
	m_strInfo1 = "";
	m_strInfo2 = "";
	m_strInfo3 = "";
	m_strInfo4 = "";
	m_strInfo5 = "";
	m_strInfo6 = "";
	m_strInfo7 = "";
	m_strInfo8 = "";
	m_bTick1 = FALSE;
	m_bTick2 = FALSE;
	m_bTick3 = FALSE;
	m_bTick4 = FALSE;
	m_nAllergyCodes = 0;
	m_strDietary = "";
	m_bInactive = FALSE;
	/*****/
	m_strMemberID = "";
	m_strExternalRef1 = "";
	m_strExternalRef2 = "";
	m_nPreviousUserID = 0;
	m_nNextUserID = 0;
	m_strLegacyPebblePurseID = "";
	/*****/
	m_nPurse1Allow = 0;
	m_dPurse1Liability = 0.0;
	m_dPurse1Credit = 0.0;
	m_strPurse1LastSpendDate = "";
	m_strPurse1LastSpendTime = "";
	m_dPurse1LastSpend = 0.0;
	m_dPurse1SpendToDate = 0.0;
	/*****/
	m_nPurse2Allow = 0;
	m_dPurse2Balance = 0.0;
	m_strPurse2LastSpendDate = "";
	m_strPurse2LastSpendTime = "";
	m_dPurse2LastSpend = 0.0;
	m_dPurse2SpendToDate = 0.0;
	m_nPurse2RefreshFlags = 0;
	m_strPurse2RefreshedDate = "";
	m_strPurse2RefreshedTime = "";
	/*****/
	m_nPurse3Allow = 0;
	m_dPurse3Liability = 0.0;
	m_dPurse3Credit = 0.0;
	m_strPurse3LastSpendDate = "";
	m_strPurse3LastSpendTime = "";
	m_dPurse3LastSpend = 0.0;
	m_dPurse3SpendToDate = 0.0;
	/*****/
	m_dCashSpendToDate = 0.0;
	/*****/
	m_nPoints = 0;
	m_nPointsToDate = 0;
	/*****/
	m_bOwnMaxSpend = FALSE;
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
	m_nAlertCode = 0;
	m_strAlertDate = "";
	m_strAlertTime = "";
	/*****/
	m_arrayRowVersion.RemoveAll();
}

//**********************************************************************

void CSQLRowAccountFull::SetRowVersion(CByteArray& arraySource)
{
	m_arrayRowVersion.RemoveAll();
	for (int n = 0; n < arraySource.GetSize(); n++)
	{
		m_arrayRowVersion.Add(arraySource.GetAt(n));
	}
}

//**********************************************************************

bool CSQLRowAccountFull::CompareRowVersion(CSQLRowAccountFull& RowSource)
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

CString CSQLRowAccountFull::GetUserIDString()
{
	CString str = ::FormatInt64Value(m_nUserID);
	return str;
}

//**********************************************************************

bool CSQLRowAccountFull::HasUserExpired()
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

bool CSQLRowAccountFull::IsRefreshAllowed(int nDay)
{
	bool bResult = FALSE;

	if ( (nDay >= 1 ) && ( nDay <= 7) )
	{
		int nMask = 0x01 << ( nDay - 1);
		int nFlags = GetPurse2RefreshFlags();
		bResult = ((nFlags & nMask) == nMask);
	}
	return bResult;
}

//**********************************************************************

bool CSQLRowAccountFull::IsPurseAllowed(int nPurseNo, int nPeriodIndex)
{
	int nPurseFlags = 0;
	switch (nPurseNo)
	{
	case 1:	nPurseFlags = GetPurse1Allow();	break;
	case 2:	nPurseFlags = GetPurse2Allow();	break;
	case 3:	nPurseFlags = GetPurse3Allow();	break;
	}

	int nMask = 0x01 << nPeriodIndex;

	return ((nPurseFlags & nMask) == nMask);
}

//**********************************************************************

void CSQLRowAccountFull::SetPurseAllowed(int nPurseNo, int nPeriodIndex, bool bSet)
{
	int nFlags = 0;
	
	switch (nPurseNo)
	{
	case 1:	nFlags = GetPurse1Allow();	break;
	case 2:	nFlags = GetPurse2Allow();	break;
	case 3:	nFlags = GetPurse3Allow();	break;
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
	case 1:	SetPurse1Allow(nFlags);	break;
	case 2:	SetPurse2Allow(nFlags);	break;
	case 3:	SetPurse3Allow(nFlags);	break;
	}
}

//**********************************************************************

bool CSQLRowAccountFull::GetLastUsed(CSSDate& date, CSSTime& time)
{
	CSSDateTime datePurse1(GetPurse1LastSpendDate(), GetPurse1LastSpendTime());
	CSSDateTime datePurse2(GetPurse2LastSpendDate(), GetPurse2LastSpendTime());
	CSSDateTime datePurse3(GetPurse3LastSpendDate(), GetPurse3LastSpendTime());

	if (datePurse1.IsSet() == TRUE)						// purse1 used
	{
		if ((datePurse1 >= datePurse2) && (datePurse1 >= datePurse3))
		{
			date.SetDate(GetPurse1LastSpendDate());
			time.SetTime(GetPurse1LastSpendTime());
			return TRUE;
		}
	}

	if (datePurse2.IsSet() == TRUE)
	{
		if ((datePurse2 >= datePurse1) && (datePurse2 >= datePurse3))
		{
			date.SetDate(GetPurse2LastSpendDate());
			time.SetTime(GetPurse2LastSpendTime());
			return TRUE;
		}
	}

	if (datePurse3.IsSet() == TRUE)
	{
		if ((datePurse3 >= datePurse1) && (datePurse3 >= datePurse2))
		{
			date.SetDate(GetPurse3LastSpendDate());
			time.SetTime(GetPurse3LastSpendTime());
			return TRUE;
		}
	}

	return FALSE;
}

//*********************************************************************

void CSQLRowAccountFull::SetGender(CString strGender)
{
	strGender.MakeUpper();

	if (strGender.Left(1) == "M")
	{
		m_strGender = "M";
	}
	else if (strGender.Left(1) == "F")
	{
		m_strGender = "F";
	}
	else
	{
		m_strGender = "";
	}
}

//*********************************************************************

CString CSQLRowAccountFull::GetGenderText()
{
	CString strGender = GetGender();

	if (strGender == "M")
	{
		strGender = "Male";
	}
	else if (strGender == "F")
	{
		strGender = "Female";
	}
	else
	{
		strGender = "unknown";
	}

	return strGender;
}

//*******************************************************************

int CSQLRowAccountFull::GetDailyGroupNo(int nDayIndex)
{
	int nGroupNo = 0;

	if ((nDayIndex >= 0) && (nDayIndex <= 6))
	{
		CCSV csv(GetDayGroups());

		if (csv.IsEmpty(nDayIndex) != TRUE)
		{
			nGroupNo = csv.GetInt(nDayIndex);
		}
	}

	return nGroupNo;
}

//*******************************************************************

void CSQLRowAccountFull::SetDailyGroupNo(int nDayIndex, int nGroupNo)
{
	if ((nDayIndex >= 0) && (nDayIndex <= 6))
	{
		CCSV csv(GetDayGroups());

		if (nGroupNo < Account::GroupNo.Min)
		{
			csv.SetAt(nDayIndex, "");
		}
		else if (nGroupNo <= Account::GroupNo.Max)
		{
			csv.SetAt(nDayIndex, nGroupNo);
		}

		SetDayGroups(csv.GetLine());
	}
}

//*******************************************************************

void CSQLRowAccountFull::SetRefreshAllowed(int nDay, bool bSet)
{
	if ((nDay >= 1) && (nDay <= 7))
	{
		int nFlags = GetPurse2RefreshFlags();

		int nMask = 0x01 << ( nDay - 1 );

		if (bSet == TRUE)
		{
			nFlags |= nMask;
		}
		else
		{
			nFlags &= ~nMask;
		}

		SetPurse2RefreshFlags(nFlags);
	}
}

//*******************************************************************

void CSQLRowAccountFull::SetUserAlert(int nAlertCode)
{
	if (nAlertCode == 0)
	{
		SetAlertCode(0);
		SetAlertDate("");
		SetAlertTime("");
	}
	else
	{
		SetAlertCode(nAlertCode);
		SetAlertDate(CSSDate::GetCurrentDate().GetDate());
		SetAlertTime(CSSTime::GetCurrentTime().GetTime());
	}
}

//*********************************************************************

int CSQLRowAccountFull::GetRecordStatusColour()
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

//*********************************************************************

CString CSQLRowAccountFull::GetGroupNoString()
{
	CString strMsg = "";
	strMsg.Format("%d", GetGroupNo());
	return strMsg;
}

//*********************************************************************

CString CSQLRowAccountFull::GetPurseBalanceString(int nPurseNo)
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

CString CSQLRowAccountFull::GetPointsString()
{
	CString strMsg = "";
	strMsg.Format("%d", GetPoints());		// v1.05b 23/07/2015
	return strMsg;
}

//*******************************************************************

CString CSQLRowAccountFull::GetTotalBalanceString()
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

CString CSQLRowAccountFull::GetAccountDisplayName()
{
	CString strReply = "";

	CString strUserName = GetUsername();
	CString strSurname = GetSurname();

	if (strUserName.GetLength() > strSurname.GetLength())
	{
		strReply = strUserName;
	}
	else
	{
		CString strForename = GetForename();
		if (strForename != "")
		{
			strReply = strForename;
			strReply += " ";
		}

		if (strSurname != "")
		{
			strReply += strSurname;
		}
	}

	return strReply;
}

//*******************************************************************

CString CSQLRowAccountFull::GetAgeNextBirthday()
{
	CString strAge = "";

	int nAge = GetAge(TRUE);

	if (nAge != -1)
	{
		strAge.Format("%d", nAge);
	}

	return strAge;
}

//*******************************************************************

int CSQLRowAccountFull::GetAge(bool bNextBDay)
{
	CSSDate dob(GetDOB());

	if (dob.IsSet() == FALSE)
	{
		return -1;
	}

	CSSDate date;
	date.SetCurrentDate();

	int nYear1 = dob.GetYear();				// birthday year
	int nYear2 = date.GetYear();				// todays year

	int nAge = nYear2 - nYear1;					// age next birthday

	if (bNextBDay == FALSE)					// adjust for birth not yet this year
	{
		if (dob.GetMonth() > date.GetMonth())	// month not yet reached his year
		{
			--nAge;
		}

		else if (dob.GetMonth() == date.GetMonth())	// birthday in this month
		{
			if (dob.GetDay() > date.GetDay())		// day of month not yet reached
			{
				--nAge;
			}
		}
	}
	else											// age next birthday
	{
		if (dob.GetMonth() < date.GetMonth())	// month already gone this year
		{
			++nAge;
		}
		else if (dob.GetMonth() == date.GetMonth())	// birthday in this month
		{
			if (dob.GetDay() < date.GetDay())		// day of month alreday gone				++Age;
			{
				++nAge;
			}
		}
	}

	return nAge;
}

//*********************************************************************

CString CSQLRowAccountFull::GetDOBDayOfWeek()
{
	CSSDate dob(GetDOB());

	if (dob.IsSet() == FALSE)
	{
		return "";
	}

	CSSDate date;
	date.SetCurrentDate();								// current date

	int nYear = date.GetYear();							// current year
	if (dob.GetMonth() < date.GetMonth())				// month already gone this year
	{
		++nYear;
	}
	else if (dob.GetMonth() == date.GetMonth())		// birthday in this month
	{
		if (dob.GetDay() < date.GetDay())				// day of month alreday gone				++Age;
		{
			++nYear;
		}
	}

	CTime time(nYear, dob.GetMonth(), dob.GetDay(), 0, 0, 0);
	return szDAYS[time.GetDayOfWeek()];
}

//*********************************************************************

CString CSQLRowAccountFull::GetRefreshTextDays()
{
	int nCount = 0;
	CString strMsg = "";

	for (int nDay = 1; nDay <= 7; nDay++)
	{
		if (IsRefreshAllowed(nDay) == TRUE)
		{
			if (nCount > 0)
			{
				strMsg += ",";
			}

			strMsg += szDAYS[nDay];
			++nCount;
		}
	}
	return strMsg;
}

//*********************************************************************

CString CSQLRowAccountFull::GetInfo(int n)
{
	CString strText = "";

	switch (n)
	{
	case 1:		strText = GetInfo1();		break;
	case 2:		strText = GetInfo2();		break;
	case 3:		strText = GetInfo3();		break;
	case 4:		strText = GetInfo4();		break;
	case 5:		strText = GetInfo5();		break;
	case 6:		strText = GetInfo6();		break;
	case 7:		strText = GetInfo7();		break;
	case 8:		strText = GetInfo8();		break;
	}
	return strText;
}

//*********************************************************************

void CSQLRowAccountFull::RevaluePurse2(double dValue, const char* szDate, const char* szTime)
{
	if (lstrlen(szDate) == 0)
	{
		SetPurse2RefreshedDate(CSSDate::GetCurrentDate().GetDate());
	}
	else
	{
		SetPurse2RefreshedDate(szDate);
	}

	if (lstrlen(szTime) == 0)
	{
		SetPurse2RefreshedTime(CSSTime::GetCurrentTime().GetTime());
	}
	else
	{
		SetPurse2RefreshedTime(szTime);
	}

	SetPurse2Balance(dValue);
	SetPurse2LastSpend(0.0);
}

//*******************************************************************

void CSQLRowAccountFull::AllocatePurse1Spend(double dSpend, double& dLiabilitySpend, double& dCreditSpend)
{
	dLiabilitySpend = 0.0;
	dCreditSpend = 0.0;

	if (dSpend < 0.0)
	{
		dLiabilitySpend = dSpend;
	}
	else
	{
		double dCredit = GetPurse1Credit();

		if (dSpend < dCredit)
		{
			dCreditSpend = dSpend;
		}
		else
		{
			dCreditSpend = dCredit;
			dLiabilitySpend = dSpend - dCreditSpend;
		}
	}
}

//******************************************************************************************

void CSQLRowAccountFull::AllocatePurse3Spend(double dSpend, double& dLiabilitySpend, double& dCreditSpend)
{
	dLiabilitySpend = 0.0;
	dCreditSpend = 0.0;

	if (dSpend < 0.0)
	{
		dLiabilitySpend = dSpend;
	}
	else
	{
		double dCredit = GetPurse3Credit();

		if (dSpend < dCredit)
		{
			dCreditSpend = dSpend;
		}
		else
		{
			dCreditSpend = dCredit;
			dLiabilitySpend = dSpend - dCreditSpend;
		}
	}
}

//******************************************************************************************

void CSQLRowAccountFull::UpdatePoints(int nPoints)
{
	AddToPoints(nPoints);
	AddToPointsToDate(nPoints);
}

//******************************************************************************************

void CSQLRowAccountFull::CopyGroupMaxSpendSettings()
{
	CSQLRowGroupFull RowGroup;
	RowGroup.SetGroupNo(GetGroupNo());

	CSQLRepositoryGroup RepoGroup;
	if ( RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
	{
		for (int nPeriod = 1; nPeriod <= 6; nPeriod++)
		{
			for (int nPurse = 1; nPurse <= 3; nPurse++)
			{
				SetPurseAllowed(nPurse, nPeriod - 1, RowGroup.IsPurseAllowed(nPurse, nPeriod - 1));
			}
		}

		SetMaxSpendPurse1Period1(RowGroup.GetMaxSpendPurse1Period1());
		SetMaxSpendPurse1Period2(RowGroup.GetMaxSpendPurse1Period2());
		SetMaxSpendPurse1Period3(RowGroup.GetMaxSpendPurse1Period3());
		SetMaxSpendPurse1Period4(RowGroup.GetMaxSpendPurse1Period4());
		SetMaxSpendPurse1Period5(RowGroup.GetMaxSpendPurse1Period5());
		SetMaxSpendPurse1Period6(RowGroup.GetMaxSpendPurse1Period6());

		SetMaxSpendPurse3Period1(RowGroup.GetMaxSpendPurse3Period1());
		SetMaxSpendPurse3Period2(RowGroup.GetMaxSpendPurse3Period2());
		SetMaxSpendPurse3Period3(RowGroup.GetMaxSpendPurse3Period3());
		SetMaxSpendPurse3Period4(RowGroup.GetMaxSpendPurse3Period4());
		SetMaxSpendPurse3Period5(RowGroup.GetMaxSpendPurse3Period5());
		SetMaxSpendPurse3Period6(RowGroup.GetMaxSpendPurse3Period6());

		SetMaxOverdraftPurse1Period1(RowGroup.GetMaxOverdraftPurse1Period1());
		SetMaxOverdraftPurse1Period2(RowGroup.GetMaxOverdraftPurse1Period2());
		SetMaxOverdraftPurse1Period3(RowGroup.GetMaxOverdraftPurse1Period3());
		SetMaxOverdraftPurse1Period4(RowGroup.GetMaxOverdraftPurse1Period4());
		SetMaxOverdraftPurse1Period5(RowGroup.GetMaxOverdraftPurse1Period5());
		SetMaxOverdraftPurse1Period6(RowGroup.GetMaxOverdraftPurse1Period6());

		SetMaxOverdraftPurse3Period1(RowGroup.GetMaxOverdraftPurse3Period1());
		SetMaxOverdraftPurse3Period2(RowGroup.GetMaxOverdraftPurse3Period2());
		SetMaxOverdraftPurse3Period3(RowGroup.GetMaxOverdraftPurse3Period3());
		SetMaxOverdraftPurse3Period4(RowGroup.GetMaxOverdraftPurse3Period4());
		SetMaxOverdraftPurse3Period5(RowGroup.GetMaxOverdraftPurse3Period5());
		SetMaxOverdraftPurse3Period6(RowGroup.GetMaxOverdraftPurse1Period6());
	}
}

//******************************************************************************************

void CSQLRowAccountFull::SetRefreshRef(const char* szDaysText)
{
	ClearRefreshFlags();								// clear current setting

	CCSV csv(szDaysText);
	for (int i = 0; i < csv.GetSize(); i++)
	{
		CString strDay = csv.GetString(i);				// get single day
		strDay.TrimLeft(' ');						// just in case

		int nDay;
		for (nDay = 1; nDay <= 7; nDay++)		// assume days are set to "Mon,Tue.."
		{
			if (strDay.Left(3) == szDAYS[nDay])
			{
				SetRefreshAllowed(nDay, TRUE);
				break;
			}
		}

		nDay = 0;										// look for days set to "1,2..."
		switch (atoi(strDay))
		{
		case 1:		nDay = 2;		break;				// Monday
		case 2:		nDay = 3;		break;				// Tuesday
		case 3:		nDay = 4;		break;				// Wednesday
		case 4:		nDay = 5;		break;				// Thursday
		case 5:		nDay = 6;		break;				// Friday
		case 6:		nDay = 7;		break;				// Saturday
		case 7:		nDay = 1;		break;				// Sunday
		}

		if (nDay != 0)
		{
			SetRefreshAllowed(nDay, TRUE);
		}
	}
}

//******************************************************************************************

void CSQLRowAccountFull::SetDailyGroups(const char* szText)
{
	CCSV csvData(szText);
	int nCount = csvData.GetSize();							// number of elements

	if (nCount > 0 && nCount <= 7)
	{
		CCSV csv(GetDayGroups());						// current data

		for (int nDayIndex = 0; nDayIndex < nCount; nDayIndex++)
		{
			int nGroupNo = csvData.GetInt(nDayIndex);

			if (nGroupNo < Account::GroupNo.Min)
			{
				csv.SetAt(nDayIndex, "");
			}
			else if (nGroupNo <= Account::GroupNo.Max)
			{
				csv.SetAt(nDayIndex, nGroupNo);
			}
		}

		SetDayGroups(csv.GetLine());
	}
}

//******************************************************************************************

bool CSQLRowAccountFull::GetTick(int n)
{
	bool bReply = FALSE;

	switch (n)
	{
	case 1:		bReply = GetTick1();		break;
	case 2:		bReply = GetTick2();		break;
	case 3:		bReply = GetTick3();		break;
	case 4:		bReply = GetTick4();		break;
	}
	return bReply;
}

//******************************************************************************************

void CSQLRowAccountFull::SetInfo(int n, const char* szText)
{
	switch (n)
	{
	case 1:		SetInfo1(szText);		break;
	case 2:		SetInfo2(szText);		break;
	case 3:		SetInfo3(szText);		break;
	case 4:		SetInfo4(szText);		break;
	case 5:		SetInfo5(szText);		break;
	case 6:		SetInfo6(szText);		break;
	case 7:		SetInfo7(szText);		break;
	case 8:		SetInfo8(szText);		break;
	}
}

//******************************************************************************************

bool CSQLRowAccountFull::IsStringEmpty(CString strText)
{
	strText.TrimLeft(' ');
	return (strText == "");
}

//*********************************************************************

bool CSQLRowAccountFull::IsRecordEmpty()
{
	if (IsStringEmpty(GetMemberID()) == FALSE)				return FALSE;
	if (IsStringEmpty(GetExternalRef1()) == FALSE)			return FALSE;
	if (IsStringEmpty(GetExternalRef2()) == FALSE)			return FALSE;
	if (IsStringEmpty(GetUsername()) == FALSE)				return FALSE;
	if (IsStringEmpty(GetForename()) == FALSE)				return FALSE;
	if (IsStringEmpty(GetSurname()) == FALSE)				return FALSE;
	if (IsStringEmpty(GetGender()) == FALSE)				return FALSE;
	if (GetPoints() != 0)									return FALSE;
	if (GetPointsToDate() != 0)								return FALSE;
	if (GetCashSpendToDate() != 0.0)						return FALSE;
	
	if (GetPurse1Liability() != 0.0)						return FALSE;
	if (GetPurse1Credit() != 0.0)							return FALSE;
	if (IsStringEmpty(GetPurse1LastSpendDate()) == FALSE)	return FALSE;
	if (IsStringEmpty(GetPurse1LastSpendTime()) == FALSE)	return FALSE;
	if (GetPurse1LastSpend() != 0.0)						return FALSE;
	if (GetPurse1SpendToDate() != 0.0)						return FALSE;

	if (GetPurse2Balance() != 0.0)							return FALSE;
	if (IsStringEmpty(GetPurse2LastSpendDate()) == FALSE)	return FALSE;
	if (IsStringEmpty(GetPurse2LastSpendTime()) == FALSE)	return FALSE;
	if (IsStringEmpty(GetPurse2RefreshedDate()) == FALSE)	return FALSE;
	if (IsStringEmpty(GetPurse2RefreshedTime()) == FALSE)	return FALSE;
	if (GetPurse2LastSpend() != 0.0)						return FALSE;
	if (GetPurse2SpendToDate() != 0.0)						return FALSE;

	if (GetPurse3Liability() != 0.0)						return FALSE;
	if (GetPurse3Credit() != 0.0)							return FALSE;
	if (IsStringEmpty(GetPurse3LastSpendDate()) == FALSE)	return FALSE;
	if (IsStringEmpty(GetPurse3LastSpendTime()) == FALSE)	return FALSE;
	if (GetPurse3LastSpend() != 0.0)						return FALSE;
	if (GetPurse3SpendToDate() != 0.0)						return FALSE;

	if (GetMaxOverdraftPurse1Period1() != 0.0)				return FALSE;
	if (GetMaxSpendPurse1Period1() != 0.0)					return FALSE;
	if (GetAllergyCodes() != 0) 							return FALSE;
	if (GetAlertCode() != 0)								return FALSE;
	if (IsStringEmpty(GetDietary()) == FALSE)				return FALSE;

	if (IsStringEmpty(GetExpiryDate()) == FALSE)			return FALSE;
	if (IsStringEmpty(GetDOB()) == FALSE	)				return FALSE;
	if (IsStringEmpty(GetDayGroups()) == FALSE)				return FALSE;

	if (IsStringEmpty(GetEmail()) == FALSE)					return FALSE;
	if (IsStringEmpty(GetInfo1()) == FALSE)					return FALSE;
	if (IsStringEmpty(GetInfo2()) == FALSE)					return FALSE;
	if (IsStringEmpty(GetInfo3()) == FALSE)					return FALSE;
	if (IsStringEmpty(GetInfo4()) == FALSE)					return FALSE;
	if (IsStringEmpty(GetInfo5()) == FALSE)					return FALSE;
	if (IsStringEmpty(GetInfo6()) == FALSE)					return FALSE;
	if (IsStringEmpty(GetInfo7()) == FALSE)					return FALSE;
	if (IsStringEmpty(GetInfo8()) == FALSE)					return FALSE;

	return TRUE;
}

//******************************************************************************************

CString CSQLRowAccountFull::GetUsername()
{
	CString strName = m_strUsername;

	if (strName == "")
	{
		strName = m_strSurname;
	}

	return strName;
}

//******************************************************************************************

