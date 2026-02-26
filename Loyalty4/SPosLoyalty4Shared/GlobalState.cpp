/**********************************************************************/
#include "MessageLogger.h"
#include "SQLConnectionOptions.h"
/**********************************************************************/
#include "SQLTable_Account\SQLRecordSetAccountUserID.h"
/**********************************************************************/
#include "GlobalState.h"
/**********************************************************************/

//Read connection options before calling this function
void CGlobalState::DetermineODBCDriverVersion(bool bServer)
{
	CReportUniqueMap<CMapKeyInt, CMapDataEmpty> mapVersions;
	SQLConnectionOptions.GetODBCDriverVersions(mapVersions);

	if (mapVersions.GetSize() == 0)
	{
		mapVersions.Insert(CMapKeyInt(DEF_OBDC_DRIVER_VERSION), CMapDataEmpty());		
	}

	int nIdealVersion = 0;
	{
		auto it = mapVersions.end();
		--it;
		nIdealVersion = it->first.m_nID;
	}

	m_nODBCDriverVersion = 0;
	DetermineODBCDriverVersion(mapVersions);

	CString strVersion = "";
	strVersion.Format("%d", m_nODBCDriverVersion);

	CString strMsg = "";
	strMsg += "Loyalty ";
	strMsg += (bServer) ? "Server " : "Manager ";
	strMsg += "using ODBC Driver Version ";
	strMsg += strVersion;

	if (m_nODBCDriverVersion < nIdealVersion)
	{
		CString strIdeal = "";
		strIdeal.Format(" (preferred version %d)", nIdealVersion);
		strMsg += strIdeal;
	}

	MessageLogger.LogSystemMessage(strMsg);

}

/**********************************************************************/

void CGlobalState::DetermineODBCDriverVersion(CReportUniqueMap<CMapKeyInt, CMapDataEmpty>& mapVersions)
{
	SQLHENV hEnv;
	SQLRETURN ret;
	SQLCHAR driverDesc[256], driverAttr[256];
	SQLSMALLINT descLen, attrLen;

	// Allocate environment handle
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	if ((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO))
	{
		auto it = mapVersions.end();
		--it;
		m_nODBCDriverVersion = it->first.m_nID;
		return;
	}

	// Set the ODBC version environment attribute
	SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

	// Enumerate installed drivers
	SQLUSMALLINT direction = SQL_FETCH_FIRST;
	while (SQLDrivers(hEnv, direction, driverDesc, sizeof(driverDesc), &descLen,
		driverAttr, sizeof(driverAttr), &attrLen) == SQL_SUCCESS)
	{
		CString strDriver = (char*)driverDesc;
		strDriver.MakeUpper();

		if (strDriver.Left(11) == "ODBC DRIVER")
		{
			int nPos = strDriver.Find("FOR SQL SERVER");
			if (nPos > 12)
			{
				CString strVersion = strDriver.Mid(12, nPos - 12);
				strVersion.Trim();

				int nVer = atoi(strVersion);
				if ((nVer > m_nODBCDriverVersion) && (nVer <= 99))
				{
					if ( mapVersions.Find(CMapKeyInt(nVer)) != mapVersions.end() )
					{
						m_nODBCDriverVersion = nVer;
					}
				}
			}
		}

		direction = SQL_FETCH_NEXT;
	}

	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

/**********************************************************************/

void CGlobalState::BuildUserIDCache()
{
	ClearUserIDCache();

	try
	{
		CSQLRecordSetAccountUserID RecordSet(NULL,
			RSParams_AccountUserId_NormalNoParams{});

		__int64 nUserID = 0;

		while (RecordSet.StepSelect(nUserID, TRUE) == TRUE)
		{
			m_UserIDCache.Insert(CMapKeyInt64(nUserID), CMapDataEmpty());
		}
	}
	catch (CDBException* pe)
	{
		pe->Delete();
	}
}

/**********************************************************************/

void CGlobalState::ClearUserIDCache()
{
	m_UserIDCache.RemoveAll();
}

/**********************************************************************/

bool CGlobalState::CheckUserIDCache(__int64 nUserID)
{
	return (m_UserIDCache.Find(CMapKeyInt64(nUserID)) != m_UserIDCache.end());
}

/**********************************************************************/
