//**********************************************************************
#include "SQLFieldValidatorPINNumber.h"
//**********************************************************************
#include "SQLRowPINNumber.h"
//**********************************************************************

CSQLRowPINNumber::CSQLRowPINNumber()
{
	Reset();
}

//**********************************************************************

void CSQLRowPINNumber::Reset()
{
	m_strPINNumber = "0000";
	m_strUserID = "";
	m_strUsername = "";
	m_strFirstDate = "";
	m_strFirstTime = "";
	m_strLastDate	 = "";
	m_strLastTime = "";
	m_nUseCounter = 0;
}

//**********************************************************************

void CSQLRowPINNumber::SetFirstDate()
{
	CString strDate = CSSDate::GetCurrentDate().GetCSVDate();
	vdtr.SetDate(strDate, m_strFirstDate);
}

//**********************************************************************

void CSQLRowPINNumber::SetFirstTime()
{
	CString strTime = CSSTime::GetCurrentTime().GetCSVTime();
	vdtr.SetDate(strTime, m_strFirstTime);
}

//**********************************************************************

void CSQLRowPINNumber::SetLastDate()
{
	CString strDate = CSSDate::GetCurrentDate().GetCSVDate();
	vdtr.SetDate(strDate, m_strLastDate);
}

//**********************************************************************

void CSQLRowPINNumber::SetLastTime()
{	
	CString strTime = CSSTime::GetCurrentTime().GetCSVTime();
	vdtr.SetDate(strTime, m_strLastTime);
}

//**********************************************************************

bool CSQLRowPINNumber::IsValidPINNumber(const CString strPINNumber)
{
	if ( strPINNumber.GetLength() != 4 )
	{
		return FALSE;
	}

	if (::TestNumeric(strPINNumber) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

//**********************************************************************
