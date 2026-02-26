//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLFieldValidatorAccount.h"
//**********************************************************************
#include "SQLRowAccountFull.h"
//**********************************************************************
static const char* szDAYS[] = { "", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
//**********************************************************************

CSQLRowAccountFull::CSQLRowAccountFull()
{
	Reset();
}

//**********************************************************************

CSQLRowAccountFull::CSQLRowAccountFull( const CSQLRowAccountFull& source )
{
	*this = source;
}

//**********************************************************************

CSQLRowAccountFull& CSQLRowAccountFull::operator= (const CSQLRowAccountFull& source)
{
	m_nUserID = source.m_nUserID;
	m_strFullname = source.m_strFullname;
	m_strForename = source.m_strForename;
	m_strSurname = source.m_strSurname;
	m_strDOB = source.m_strDOB;
	m_strExpiryDate = source.m_strExpiryDate;
	m_strGender = source.m_strGender;
	m_nGroupNo = source.m_nGroupNo;
	m_nSchemeNo = source.m_nSchemeNo;
	m_strInfo1 = source.m_strInfo1;
	m_strInfo2 = source.m_strInfo2;
	m_strInfo3 = source.m_strInfo3;
	m_strInfo4 = source.m_strInfo4;
	m_strAddress1 = source.m_strAddress1;
	m_strAddress2 = source.m_strAddress2;
	m_strAddress3 = source.m_strAddress3;
	m_strAddress4 = source.m_strAddress4;
	m_strAddress5 = source.m_strAddress5;
	m_strPhone1 = source.m_strPhone1;
	m_strPhone2 = source.m_strPhone2;
	m_bInactive = source.m_bInactive;
	/*****/
	m_strExternalRef = source.m_strExternalRef;
	m_strQRInfo = source.m_strQRInfo;
	m_nPreviousUserID = source.m_nPreviousUserID;
	m_nNextUserID = source.m_nNextUserID;
	/*****/
	m_dPurse1Balance = source.m_dPurse1Balance;
	m_strPurse1LastSpendDate = source.m_strPurse1LastSpendDate;
	m_strPurse1LastSpendTime = source.m_strPurse1LastSpendTime;
	m_dPurse1LastSpend = source.m_dPurse1LastSpend;
	m_dPurse1SpendToDate = source.m_dPurse1SpendToDate;
	/*****/
	m_dPurse2Balance = source.m_dPurse2Balance;
	m_strPurse2LastSpendDate = source.m_strPurse2LastSpendDate;
	m_strPurse2LastSpendTime = source.m_strPurse2LastSpendTime;
	m_dPurse2LastSpend = source.m_dPurse2LastSpend;
	m_dPurse2SpendToDate = source.m_dPurse2SpendToDate;
	m_strPurse2RefreshedDate = source.m_strPurse2RefreshedDate;
	m_strPurse2RefreshedTime = source.m_strPurse2RefreshedTime;
	/*****/
	m_strCashLastSpendDate = source.m_strCashLastSpendDate;
	m_strCashLastSpendTime = source.m_strCashLastSpendTime;
	m_dCashLastSpend = source.m_dCashLastSpend;
	m_dCashSpendToDate = source.m_dCashSpendToDate;;
	/*****/
	m_nPoints = source.m_nPoints;
	m_nPointsToDate = source.m_nPointsToDate;
	/*****/
	m_nHotlistCode = source.m_nHotlistCode;
	m_strHotlistDate = source.m_strHotlistDate;
	m_strHotlistTime = source.m_strHotlistTime;
	/*****/
	m_dMaxSpend = source.m_dMaxSpend;
	m_dMaxOverdraft = source.m_dMaxOverdraft;
	/*****/
	m_strAlertCodes = source.m_strAlertCodes;
	/*****/
	m_arrayRowVersion.RemoveAll();

	for (int n = 0; n < source.m_arrayRowVersion.GetSize(); n++)
	{
		m_arrayRowVersion.Add(source.m_arrayRowVersion.GetAt(n));
	}

	return *this;
}

//**********************************************************************

void CSQLRowAccountFull::Reset()
{
	m_nUserID = 0;
	m_strFullname = "";
	m_strForename = "";
	m_strSurname = "";
	m_strDOB = "";
	m_strExpiryDate = "";
	m_strGender = "";
	m_nGroupNo = 1;
	m_nSchemeNo = 0;
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
	m_bInactive = FALSE;
	/*****/
	m_strExternalRef = "";
	m_strQRInfo = "";
	m_nPreviousUserID = 0;
	m_nNextUserID = 0;
	/*****/
	m_dPurse1Balance = 0.0;
	m_strPurse1LastSpendDate = "";
	m_strPurse1LastSpendTime = "";
	m_dPurse1LastSpend = 0.0;
	m_dPurse1SpendToDate = 0.0;
	/*****/
	m_dPurse2Balance = 0.0;
	m_strPurse2LastSpendDate = "";
	m_strPurse2LastSpendTime = "";
	m_dPurse2LastSpend = 0.0;
	m_dPurse2SpendToDate = 0.0;
	m_strPurse2RefreshedDate = "";
	m_strPurse2RefreshedTime = "";
	/*****/
	m_strCashLastSpendDate = "";
	m_strCashLastSpendTime = "";
	m_dCashLastSpend = 0.0;
	m_dCashSpendToDate = 0.0;
	/*****/
	m_nPoints = 0;
	m_nPointsToDate = 0;
	/*****/
	m_nHotlistCode = 0;
	m_strHotlistDate = "";
	m_strHotlistTime = "";
	/*****/
	m_dMaxSpend = 0.0;
	m_dMaxOverdraft = 0.0;
	/*****/
	m_strAlertCodes = "";
	/*****/
	m_arrayRowVersion.RemoveAll();
}

//**********************************************************************

double CSQLRowAccountFull::GetPurse1Balance()
{
	return m_dPurse1Balance;
}

//**********************************************************************

void CSQLRowAccountFull::SetRowVersion( CByteArray& arraySource)
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
	CString str = ::SimpleFormatInt64Value(m_nUserID);
	return str;
}

//**********************************************************************

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

bool CSQLRowAccountFull::GetLastUsed(CSSDate& date, CSSTime& time)
{
	CSSDateTime datePurse1(GetPurse1LastSpendDate(), GetPurse1LastSpendTime());
	CSSDateTime datePurse2(GetPurse2LastSpendDate(), GetPurse2LastSpendTime());
	CSSDateTime dateCash(GetCashLastSpendDate(), GetCashLastSpendTime());

	CSSDateTime dateLastUsed = datePurse1;

	int nDateToUse(datePurse1.IsSet() ? 1 : 0);

	if (datePurse2.IsSet() == TRUE)
	{
		if ((0 == nDateToUse) || (datePurse2 > dateLastUsed))
		{
			dateLastUsed = datePurse2;
			nDateToUse = 2;
		}
	}

	if (dateCash.IsSet() == TRUE)
	{
		if ((0 == nDateToUse) || (dateCash > dateLastUsed))
		{
			nDateToUse = 3;
		}
	}

	switch (nDateToUse)
	{
	case 1:
		date.SetDate(GetPurse1LastSpendDate());
		time.SetTime(GetPurse1LastSpendTime());
		return TRUE;

	case 2:
		date.SetDate(GetPurse2LastSpendDate());
		time.SetTime(GetPurse2LastSpendTime());
		return TRUE;

	case 3:
		date.SetDate(GetCashLastSpendDate());
		time.SetTime(GetCashLastSpendTime());
		return TRUE;

	case 0:
	default:
		return FALSE;
	}
}

//$$********************************************************************

bool CSQLRowAccountFull::HasCardExpired()
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

//$$********************************************************************

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

//$$********************************************************************

void CSQLRowAccountFull::DivideCardName(const char* szCardName, CString& strFirstName, CString& strSecondName)
{
	CString strFullName = szCardName;
	int nLength = strFullName.GetLength();

	strFirstName = "";
	strSecondName = "";

	int i = 0;
	for (i = nLength - 1; i >= 0; i--)
	{
		char c = strFullName.GetAt(i);
		if (c == ' ')									// look for space in cardname ( 'Fred Smith' )
		{
			break;
		}

		strSecondName += c;								// compile surname backwards
	}
	strSecondName.MakeReverse();						// get surname right way round

	if (i > 0)										// see if any cardname left 
	{
		strFirstName = strFullName.Left(i);
	}
}

//$$********************************************************************

void CSQLRowAccountFull::RefreshPurse2(double dValue, const char* szDate, const char* szTime)
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
	SetPurse2LastSpend(0);
	SetPurse2LastSpendDate("");
	SetPurse2LastSpendTime("");
}

//$$********************************************************************

CString CSQLRowAccountFull::GetAgeNextBirthday()
{
	int nAge = GetAge(TRUE);

	CString strAge = "";

	if (nAge != -1)
	{
		strAge.Format("%d", nAge);
	}

	return strAge;
}

//$$********************************************************************

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

//$$********************************************************************
// get day of week for next birtday

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

//$$********************************************************************

bool CSQLRowAccountFull::GetBoolData(const char* szData)
{
	CString strData = szData;
	strData.MakeUpper();
	return ((strData == "1") || (strData == "X") || (strData.Left(1) == "Y") || (strData.Left(1) == "T"));
}

//$$********************************************************************

void CSQLRowAccountFull::UpdatePoints(int nPoints)
{
	AddToPoints(nPoints);
	AddToPointsToDate(nPoints);
}

//$$********************************************************************

void CSQLRowAccountFull::SetGender(int nGender)
{
	switch (nGender)
	{
	case 1:
		m_strGender = "M";
		break;

	case 2:
		m_strGender = "F";
		break;

	default:
		m_strGender = "";
		break;
	}
}

//$$********************************************************************

void CSQLRowAccountFull::SetHotlistCard(int nHotlistCode)
{
	if ( 0 == nHotlistCode)
	{
		SetHotlistCode(0);
		SetHotlistDate("");
		SetHotlistTime("");
	}
	else
	{
		SetHotlistCode(nHotlistCode);
		SetHotlistDate(CSSDate::GetCurrentDate().GetDate());
		SetHotlistTime(CSSTime::GetCurrentTime().GetTime());
	}
}

//$$********************************************************************

bool CSQLRowAccountFull::IsRecordEmpty()
{
	if (IsStringEmpty(GetFullname()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetForename()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetSurname()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetExternalRef()) == FALSE)
	{
		return FALSE;
	}

	if (GetPoints() != 0)
	{
		return FALSE;
	}

	if (GetPointsToDate() != 0)
	{
		return FALSE;
	}

	if (GetPurse1Balance() != 0.0)
	{
		return FALSE;
	}

	if (GetPurse2Balance() != 0.0)
	{
		return FALSE;
	}

	if (GetTotalSpendToDate() != 0.0)
	{
		return FALSE;
	}

	if (GetMaxOverdraft() != 0.0)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetAddress1()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetAddress2()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetAddress3()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetAddress4()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetAddress5()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetPhone1()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetPhone2()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetInfo1()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetInfo2()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetInfo3()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetInfo4()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetQRInfo()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetExpiryDate()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetDOB()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetPurse1LastSpendDate()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetPurse1LastSpendTime()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetPurse2LastSpendDate()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetPurse2LastSpendTime()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetCashLastSpendDate()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetCashLastSpendTime()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetHotlistDate()) == FALSE)
	{
		return FALSE;
	}

	if (IsStringEmpty(GetHotlistTime()) == FALSE)
	{
		return FALSE;
	}

	if (GetPreviousUserID() != 0)
	{
		return FALSE;
	}

	if (GetNextUserID() != 0)
	{
		return FALSE;
	}

	return TRUE;
}

//$$********************************************************************

bool CSQLRowAccountFull::IsStringEmpty(const char* szText)
{
	CString strText = szText;
	strText.TrimLeft(' ');
	return (strText == "") ? TRUE : FALSE;
}

//$$********************************************************************

bool CSQLRowAccountFull::IsBirthdayToday()
{
	bool bIsBirthday = FALSE;

	CSSDate dateDob(GetDOB());
	if (dateDob.IsSet() == TRUE)
	{
		CSSDate dateNow;
		dateNow.SetCurrentDate();
		if (dateDob.GetMonth() == dateNow.GetMonth())
		{
			if (dateDob.GetDay() == dateNow.GetDay())
			{
				bIsBirthday = TRUE;
			}
		}
	}

	return bIsBirthday;
}

//$$********************************************************************

CString CSQLRowAccountFull::GetGroupNoString()
{
	CString strBuf = "";
	strBuf.Format("%d", GetGroupNo());
	return strBuf;
}

//$$********************************************************************
