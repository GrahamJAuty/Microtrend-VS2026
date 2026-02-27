#pragma once
//**********************************************************************
#include "SQLFieldValidatorPeriodRefresh.h"
//**********************************************************************

class CSQLRowPeriodRefresh
{
public:
	CSQLRowPeriodRefresh();
	
private:
	void Reset();
	
public:
	__int64 GetUserID() { return m_nUserID; }
	CString GetPeriod1Date() { return m_strPeriod1Date; }
	double GetPeriod1Balance() { return m_dPeriod1Balance; }
	CString GetPeriod2Date() { return m_strPeriod2Date; }
	double GetPeriod2Balance() { return m_dPeriod2Balance; }
	CString GetPeriod3Date() { return m_strPeriod3Date; }
	double GetPeriod3Balance() { return m_dPeriod3Balance; }
	CString GetPeriod4Date() { return m_strPeriod4Date; }
	double GetPeriod4Balance() { return m_dPeriod4Balance; }
	CString GetPeriod5Date() { return m_strPeriod5Date; }
	double GetPeriod5Balance() { return m_dPeriod5Balance; }
	CString GetPeriod6Date() { return m_strPeriod6Date; }
	double GetPeriod6Balance() { return m_dPeriod6Balance; }

public:
	CString GetDate(int n);
	double GetBalance(int n);

public:
	void SetDate(int n, CString strDate, bool bFromSQL);
	void SetBalance(int n, double dBalance);

public:
	void SetUserID(__int64 n) { vdtr.SetUserID(n, m_nUserID); }
	
	void SetPeriod1Date(CString str, bool bFromSQL) 
	{ 
		if ((str == "") && (TRUE == bFromSQL))
		{
			m_strPeriod1Date = "";
		}
		else
		{
			vdtr.SetPeriodDate(CheckDate(str), m_strPeriod1Date);
		}
	}

	void SetPeriod2Date(CString str, bool bFromSQL) 
	{ 
		if ((str == "") && (TRUE == bFromSQL))
		{
			m_strPeriod2Date = "";
		}
		else
		{
			vdtr.SetPeriodDate(CheckDate(str), m_strPeriod2Date);
		}
	}

	void SetPeriod3Date(CString str, bool bFromSQL)
	{
		if ((str == "") && (TRUE == bFromSQL))
		{
			m_strPeriod3Date = "";
		}
		else
		{
			vdtr.SetPeriodDate(CheckDate(str), m_strPeriod3Date);
		}
	}

	void SetPeriod4Date(CString str, bool bFromSQL)
	{
		if ((str == "") && (TRUE == bFromSQL))
		{
			m_strPeriod4Date = "";
		}
		else
		{
			vdtr.SetPeriodDate(CheckDate(str), m_strPeriod4Date);
		}
	}

	void SetPeriod5Date(CString str, bool bFromSQL)
	{
		if ((str == "") && (TRUE == bFromSQL))
		{
			m_strPeriod5Date = "";
		}
		else
		{
			vdtr.SetPeriodDate(CheckDate(str), m_strPeriod5Date);
		}
	}

	void SetPeriod6Date(CString str, bool bFromSQL)
	{
		if ((str == "") && (TRUE == bFromSQL))
		{
			m_strPeriod6Date = "";
		}
		else
		{
			vdtr.SetPeriodDate(CheckDate(str), m_strPeriod6Date);
		}
	}

	void SetPeriod1Balance(double d) { vdtr.SetPeriodBalance(d, m_dPeriod1Balance); }
	void SetPeriod2Balance(double d) { vdtr.SetPeriodBalance(d, m_dPeriod2Balance); }
	void SetPeriod3Balance(double d) { vdtr.SetPeriodBalance(d, m_dPeriod3Balance); }
	void SetPeriod4Balance(double d) { vdtr.SetPeriodBalance(d, m_dPeriod4Balance); }
	void SetPeriod5Balance(double d) { vdtr.SetPeriodBalance(d, m_dPeriod5Balance); }
	void SetPeriod6Balance(double d) { vdtr.SetPeriodBalance(d, m_dPeriod6Balance); }


private:
	CString CheckDate(CString strDate);

private:
	__int64 m_nUserID;
	CString m_strPeriod1Date;
	double m_dPeriod1Balance;
	CString m_strPeriod2Date;
	double m_dPeriod2Balance;
	CString m_strPeriod3Date;
	double m_dPeriod3Balance;
	CString m_strPeriod4Date;
	double m_dPeriod4Balance;
	CString m_strPeriod5Date;
	double m_dPeriod5Balance;
	CString m_strPeriod6Date;
	double m_dPeriod6Balance;
	
private:
	CSQLFieldValidatorPeriodRefresh vdtr;
};

//**********************************************************************

