#pragma once
/**********************************************************************/
#include "ConsolidationMap.h"
#include "ReportConsolidationMap.h"
/**********************************************************************/

class CGlobalState
{
public:
	CGlobalState()
	{
	}

	int GetLeaverYear() const
	{
		return m_nLeaverYear;
	}

	bool IsLeaverMode() const
	{
		return (m_nLeaverYear > 0);
	}

	void SetLeaverYear(int nYear)
	{
		if ((nYear >= 0) && (nYear <= 9999))
		{
			m_nLeaverYear = nYear;
		}
	}

	void ClearLeaverYear()
	{
		m_nLeaverYear = 0;
	}

public:
	void BuildUserIDCache();
	void ClearUserIDCache();
	bool CheckUserIDCache(__int64 nUserID);

public:
	void SetServerDesktopAppFlag(bool b) { m_bServerDesktopApp = b; }
	bool IsServerDesktopApp() { return m_bServerDesktopApp; }

public:
	void SetFixedDatabaseOptionsFlag(bool b) { m_bFixedDatabaseOptions = b; }
	bool GetFixedDatabaseOptionsFlag() { return m_bFixedDatabaseOptions; }

public:
	CString GetExecuteSQLLastError() { return m_strExecuteSQLLastError; }
	void SetExecuteSQLLastError(CString strError) { m_strExecuteSQLLastError = strError; }

public:
	int GetODBCDriverVersion() { return m_nODBCDriverVersion; }
	void DetermineODBCDriverVersion(int nProgramType);

private:
	void DetermineODBCDriverVersion(CReportUniqueMap<CMapKeyInt, CMapDataEmpty>& mapVersions);

private:
	CReportUniqueMap<CMapKeyInt64, CMapDataEmpty> m_UserIDCache;
	int m_nLeaverYear= 0;
	int m_nODBCDriverVersion = 17;
	bool m_bServerDesktopApp = FALSE;
	bool m_bFixedDatabaseOptions = FALSE;
	CString m_strExecuteSQLLastError = "";
};

/**********************************************************************/
extern CGlobalState g_GlobalState;
/**********************************************************************/