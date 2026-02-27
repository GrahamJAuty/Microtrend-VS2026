//**********************************************************************
#include "SQLFieldValidatorPeriodRefresh.h"
//**********************************************************************
#include "SQLRowPeriodRefresh.h"
//**********************************************************************

CSQLRowPeriodRefresh::CSQLRowPeriodRefresh()
{
	Reset();
}

//**********************************************************************

void CSQLRowPeriodRefresh::Reset()
{
	m_nUserID = 0;
	m_strPeriod1Date = "";
	m_dPeriod1Balance = 0.0;
	m_strPeriod2Date = "";
	m_dPeriod2Balance = 0.0;
	m_strPeriod3Date = "";
	m_dPeriod3Balance = 0.0;
	m_strPeriod4Date = "";
	m_dPeriod4Balance = 0.0;
	m_strPeriod5Date = "";
	m_dPeriod5Balance = 0.0;
	m_strPeriod6Date = "";
	m_dPeriod6Balance = 0.0;
}

//**********************************************************************

CString CSQLRowPeriodRefresh::GetDate(int n)
{
	switch (n)
	{
	case 0:	return m_strPeriod1Date;
	case 1:	return m_strPeriod2Date;
	case 2:	return m_strPeriod3Date;
	case 3:	return m_strPeriod4Date;
	case 4:	return m_strPeriod5Date;
	case 5:	return m_strPeriod6Date;
	}

	return "";
}

//**********************************************************************

double CSQLRowPeriodRefresh::GetBalance(int n)
{
	switch (n)
	{
	case 0:	return m_dPeriod1Balance;
	case 1:	return m_dPeriod2Balance;
	case 2:	return m_dPeriod3Balance;
	case 3:	return m_dPeriod4Balance;
	case 4:	return m_dPeriod5Balance;
	case 5:	return m_dPeriod6Balance;
	}

	return 0.0;
}

//**********************************************************************

CString CSQLRowPeriodRefresh::CheckDate(CString strDate)
{
	CSSDate date(strDate);

	if (date.IsSet() == FALSE)
	{
		date.SetCurrentDate();
		strDate = date.GetDate();
	}

	return strDate;
}

//**********************************************************************

void CSQLRowPeriodRefresh::SetDate(int n, CString strDate, bool bFromSQL)
{
	switch (n)
	{
	case 0:	SetPeriod1Date(strDate, bFromSQL);	break;
	case 1:	SetPeriod2Date(strDate, bFromSQL);	break;
	case 2:	SetPeriod3Date(strDate, bFromSQL);	break;
	case 3:	SetPeriod4Date(strDate, bFromSQL);	break;
	case 4:	SetPeriod5Date(strDate, bFromSQL);	break;
	case 5:	SetPeriod6Date(strDate, bFromSQL);	break;
	}
}

//**********************************************************************

void CSQLRowPeriodRefresh::SetBalance(int n, double dBalance )
{
	switch (n)
	{
	case 0:	SetPeriod1Balance(dBalance);	break;
	case 1:	SetPeriod2Balance(dBalance);	break;
	case 2:	SetPeriod3Balance(dBalance);	break;
	case 3:	SetPeriod4Balance(dBalance);	break;
	case 4:	SetPeriod5Balance(dBalance);	break;
	case 5:	SetPeriod6Balance(dBalance);	break;
	}
}

//**********************************************************************