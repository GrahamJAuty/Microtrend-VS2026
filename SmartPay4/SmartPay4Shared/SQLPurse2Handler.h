#pragma once
//*******************************************************************
#include "SQLTable_Account\SQLRowAccountFull.h"
#include "SQLTable_Group\SQLRowGroupFull.h"
//**********************************************************************

class CSQLPurse2Handler  
{
public:
	CSQLPurse2Handler ( CSQLRowAccountFull& SQLRowAccount, CSQLRowGroupFull& SQLRowGroup );

	double GetBalance ( bool bSaveRefresh, const char* szDate = "", const char* szTime = "" );
	bool Purse2Allowed() { return m_bPurse2Allowed; }

	double GetRefreshValue();
//	CString GetRefreshValueText();

	int GetPeriodIndex();
	bool CheckRefreshReqd ( int nRefreshType, const char* szLastRefreshed, bool bPeriodRefreshAdjust );
	bool IsRefreshReqd() { return m_bRefreshReqd; }
	CString GetEODRefreshDate() { return m_strEODRefreshDate;  }

private:
	bool CheckPeriodRefreshReqd();

private:
	CSQLRowAccountFull& m_SQLRowAccount;
	CSQLRowGroupFull& m_SQLRowGroup;
	
	CSSDate m_dateToday;
	CSSTime m_timeToday;

	CString m_strUserID;
	bool m_bPurse2Allowed;

private:
	bool m_bRefreshReqd;
	CString m_strEODRefreshDate;

private:
	int m_nPeriodIndex;
	double m_dPeriodBalance;
};

//**********************************************************************
