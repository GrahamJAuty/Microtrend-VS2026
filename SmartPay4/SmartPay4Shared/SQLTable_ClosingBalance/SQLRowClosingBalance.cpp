//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRowClosingBalance.h"
//**********************************************************************

CSQLRowClosingBalance::CSQLRowClosingBalance()
{
	Reset();
}

//**********************************************************************

void CSQLRowClosingBalance::Reset()
{
	m_strKeyDate = "";
	/*****/
	m_strDate = "";
	m_strTime = "";
	/*****/
	m_strArchiveDate = "";
	m_strArchiveTime = "";
	/*****/
	m_dPurse1LiabilityAudit = 0.0;
	m_dPurse1CreditAudit = 0.0;
	/*****/
	m_dPurse2BalanceAudit = 0.0;
	/*****/
	m_dPurse3LiabilityAudit = 0.0;
	m_dPurse3CreditAudit = 0.0;
	/*****/
	m_bHaveAdjust = FALSE;
	/*****/
	m_dPurse1LiabilityAdjust = 0.0;
	m_dPurse1CreditAdjust = 0.0;
	/*****/
	m_dPurse3LiabilityAdjust = 0.0;
	m_dPurse3CreditAdjust = 0.0;
}

//**********************************************************************

CString CSQLRowClosingBalance::MakeKeyDate(const CString strDate)
{
	CSSDate date(strDate);
	return MakeKeyDate(date);
}

//**********************************************************************

CString CSQLRowClosingBalance::MakeKeyDate(CSSDate& date)
{
	CString strKey = "";

	if (date.IsSet() == TRUE)
	{
		strKey.Format("%4.4d%2.2d%2.2d", date.GetYear(), date.GetMonth(), date.GetDay());
	}

	return strKey;
}

//**********************************************************************

CString CSQLRowClosingBalance::MakeKeyDate(const COleDateTime& date)
{
	CString strKey = "";

	if (date.GetStatus() == COleDateTime::valid)
	{
		strKey.Format("%4.4d%2.2d%2.2d", date.GetYear(), date.GetMonth(), date.GetDay());
	}

	return strKey;
}

//**********************************************************************

CString CSQLRowClosingBalance::MakeDbDate(const COleDateTime& date)
{
	CString strDate = "";

	if (date.GetStatus() == COleDateTime::valid)
	{
		strDate.Format("%2.2d/%2.2d/%4.4d", date.GetDay(), date.GetMonth(), date.GetYear());
	}

	return strDate;
}

//**********************************************************************

CString CSQLRowClosingBalance::MakeDbTime(const COleDateTime& date)
{
	CString strTime = "";

	if (date.GetStatus() == COleDateTime::valid)
	{
		strTime.Format("%2.2d:%2.2d", date.GetHour(),date.GetMinute());
	}

	return strTime;
}

//**********************************************************************

void CSQLRowClosingBalance::SetArchived()
{
	CSSDate date;
	date.SetCurrentDate();
	SetDate(date.GetCSVDate());

	CSSTime time;
	time.SetCurrentTime();
	SetTime(time.GetCSVTime());
}

//**********************************************************************
