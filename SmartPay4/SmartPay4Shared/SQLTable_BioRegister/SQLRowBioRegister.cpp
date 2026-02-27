//**********************************************************************
#include "SQLFieldValidatorBioRegister.h"
//**********************************************************************
#include "SQLRowBioRegister.h"
//**********************************************************************

CSQLRowBioRegister::CSQLRowBioRegister()
{
	Reset();
}

//**********************************************************************

void CSQLRowBioRegister::Reset()
{
	m_nUserID = 0;
	m_strUsername = "";
	m_nBioCounter = 0;
	m_strRegDate = "";
	m_strRegTime = "";
	m_strUnRegDate = "";
	m_strUnRegTime = "";
	m_bUnReg = FALSE;
	m_strFirstDate = "";
	m_strFirstTime = "";
	m_strLastDate = "";
	m_strLastTime = "";
}

//**********************************************************************

void CSQLRowBioRegister::SetRegDate()
{
	m_strRegDate = CSSDate::GetCurrentDate().GetCSVDate();
}

//**********************************************************************

void CSQLRowBioRegister::SetRegTime()
{
	m_strRegTime = CSSTime::GetCurrentTime().GetCSVTime();
}

//**********************************************************************

void CSQLRowBioRegister::SetUnRegDate()
{
	m_strUnRegDate = CSSDate::GetCurrentDate().GetCSVDate();
}

//**********************************************************************

void CSQLRowBioRegister::SetUnRegTime()
{
	m_strUnRegTime = CSSTime::GetCurrentTime().GetCSVTime();
}

//**********************************************************************

void CSQLRowBioRegister::SetFirstDate()
{
	m_strFirstDate = CSSDate::GetCurrentDate().GetCSVDate();
	IncrementBioCounter();
}

//**********************************************************************

void CSQLRowBioRegister::SetFirstTime()
{
	m_strFirstTime = CSSTime::GetCurrentTime().GetCSVTime();
}

//**********************************************************************

void CSQLRowBioRegister::SetLastDate()
{
	m_strLastDate = CSSDate::GetCurrentDate().GetCSVDate();
	IncrementBioCounter();
}

//**********************************************************************

void CSQLRowBioRegister::SetLastTime()
{
	m_strLastTime = CSSTime::GetCurrentTime().GetCSVTime();
}

//**********************************************************************

void CSQLRowBioRegister::IncrementBioCounter()
{
	m_nBioCounter++;
}

//**********************************************************************
