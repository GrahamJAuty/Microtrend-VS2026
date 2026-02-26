//**********************************************************************
#include "SQLTable_DbVersion\SQLRecordSetDbVersion.h"
//**********************************************************************
#include "ChartwellsOptions.h"
#include "FilenameHandler.h"
#include "FileRemover.h"
#include "SQLConnectionOptions.h"
#include "SQLConnectionPool.h"
#include "SQLDefines.h"
#include "SQLHelpers.h"
#include "SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLDb.h"
//**********************************************************************

CSQLDb::CSQLDb()
{
	m_bDoneConnect = FALSE;
	m_strRootName = "SPOS_SmartPayDb";
	m_strRootFilename = "SmartPayDb";

	m_strSQLFolder = Sysset.GetDataPath();
	m_strSQLFolder += "\\SQL";

	m_strDatabasePath = m_strSQLFolder;
	m_strDatabasePath += "\\";
	m_strDatabasePath += m_strRootFilename;

	m_strLogPath = m_strDatabasePath;
	m_strDatabasePath += ".mdf";
	m_strLogPath += "log.ldf";

	m_bInUse = FALSE;
}

//**********************************************************************

CSQLDb::~CSQLDb()
{
	Close();
}

//**********************************************************************

void CSQLDb::Close()
{
	if (m_database.IsOpen())
	{
		m_database.Close();
	}
}

//**********************************************************************

bool CSQLDb::OpenDatabase(CString strName)
{
	CString strConn = "";

	bool bLANConnect = SQLConnectionOptions.GetLANConnectFlag();
	CString strLANServerName = SQLConnectionOptions.GetLANServerName();
	CString strPassword = SQLConnectionOptions.GetPasswordPT();
	int nQueryTimeout = SQLConnectionOptions.GetQueryTimeout();
	::TrimSpaces(strLANServerName, FALSE);

	if (strLANServerName == "")
	{
		bLANConnect = FALSE;
	}

	int nSerialNo = ChartwellsOptions.GetSmartPaySerialNo();

	if (FALSE == bLANConnect)
	{
		strConn.Format("DRIVER=%s;Server=localhost\\SPOS_SMP_%6.6d;Database=%s;Trusted_Connection=yes",
			(const char*)SQLConnectionOptions.GetODBCDriverString(),
			nSerialNo,
			(const char*)strName);
	}
	else
	{
		strConn.Format("DRIVER=%s;Server=%s\\SPOS_SMP_%6.6d;Uid=sa;Pwd=%s;Database=%s;Trusted_Connection=no",
			(const char*)SQLConnectionOptions.GetODBCDriverString(),
			(const char*)strLANServerName,
			nSerialNo,
			(const char*)strPassword,
			(const char*)strName);
	}

	bool bResult = FALSE;

	try
	{
		bResult = m_database.OpenEx( strConn, CDatabase::noOdbcDialog);

		if ((nQueryTimeout >= 1) && (nQueryTimeout <= 300))
		{
			m_database.SetQueryTimeout(nQueryTimeout);
		}
	}
	catch (CDBException* ex)
	{
		CString strAction = "OpenDatabase: ";

		if (FALSE == bLANConnect)
		{
			strAction += "Local";
		}
		else
		{
			strAction += "LAN";
		}

		CSQLHelpers::LogSQLException(strAction, ex->m_strError);

		ex ->Delete();
		bResult = FALSE;
	}

	return bResult;
}

//**********************************************************************

void CSQLDb::Connect(int& nError)
{
	nError = 0;
	
	if (TRUE == m_bDoneConnect)
	{
		return;
	}

	if (OpenDatabase(m_strRootName) == FALSE)
	{
		nError = 1;
		return;
	}
}

//**********************************************************************

void CSQLDb::GetDatabaseVersion(int& nError, int& nVersion, int& nExternalVersion, bool bCheckExternal)
{
	nError = SQLSRVR_CHECKVERSION_ERR_NOLINE;
	nVersion = 0;
	nExternalVersion = 0;

	if (FALSE == bCheckExternal)
	{
		CSQLRecordSetDbVersion RecordSet(GetDb(),
			RSParams_DbVersion_NormalByLineId{ 1 });

		CSQLRowDbVersion RowVersion;
		if (RecordSet.StepSelectAll(RowVersion) == TRUE)
		{
			nVersion = RowVersion.GetVersion();
			nError = SQLSRVR_CHECKVERSION_ERR_NONE;
		}
	}
	else
	{ 
		CSQLRecordSetDbVersion RecordSet(GetDb(),
			RSParams_DbVersion_ExtVersionByLineId{ 1 });

		CSQLRowDbVersion RowVersion;
		if (RecordSet.StepSelectAll(RowVersion) == TRUE)
		{
			nVersion = RowVersion.GetVersion();
			nExternalVersion = RowVersion.GetExternalVersion1();
			nError = SQLSRVR_CHECKVERSION_ERR_NONE;
		}
	}
}

//**********************************************************************

