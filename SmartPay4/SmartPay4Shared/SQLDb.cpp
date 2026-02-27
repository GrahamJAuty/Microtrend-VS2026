//**********************************************************************
#include "SQLTable_DbVersion\SQLRecordSetDbVersion.h"
//**********************************************************************
#include "FilenameHandler.h"
#include "FileRemover.h"
#include "GlobalState.h"
#include "SQLConnectionOptions.h"
#include "SQLConnectionPool.h"
#include "SQLDefines.h"
#include "SQLExceptionLogger.h"
#include "SQLHelpers.h"
#include "SQLRecordSetConnectionPool.h"
//**********************************************************************
#ifdef SYSTEMTYPE_MANAGER
//**********************************************************************
#include "SQLSchema\SQLBuildSchema.h"
//**********************************************************************
#endif
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
	int nPortNumber = SQLConnectionOptions.GetPortNumber();
	::TrimSpacesFromString(strLANServerName);

	if (strLANServerName == "")
	{
		bLANConnect = FALSE;
	}

	if (FALSE == bLANConnect)
	{
		strConn.Format("DRIVER=%s;Server=localhost\\SPOS_SMP_%6.6d;Database=%s;Trusted_Connection=yes",
			(const char*)SQLConnectionOptions.GetODBCDriverString(g_GlobalState.GetODBCDriverVersion()),
			Sysset.GetSerialNo(),
			(const char*)strName);
	}
	else
	{
		CString strPortNumber = "";

		if ((nPortNumber >= 1) && (nPortNumber <= 65535))
		{
			strPortNumber.Format(",%d", nPortNumber);
		}

		strConn.Format("DRIVER=%s;Server=tcp:%s\\SPOS_SMP_%6.6d%s;Uid=sa;Pwd=%s;Database=%s;Trusted_Connection=no",
			(const char*)SQLConnectionOptions.GetODBCDriverString(g_GlobalState.GetODBCDriverVersion()),
			(const char*)strLANServerName,
			Sysset.GetSerialNo(),
			(const char*)strPortNumber,
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

bool CSQLDb::CreateSmartPayDb()
{
	MakeSubdirectory(m_strSQLFolder);

	char buffer[4096];
	GetFullPathName(m_strDatabasePath, 4096, buffer, NULL);
	CString strDatabasePath = buffer;

	GetFullPathName(m_strLogPath, 4096, buffer, NULL);
	CString strLogFilePath = buffer;

	CString strPart1 = "";
	strPart1.Format("CREATE DATABASE %s ON PRIMARY ",
		(const char*)m_strRootName);

	CString strPart2 = "";
	strPart2.Format("(NAME = %s_data, FILENAME = '%s', SIZE = 10MB, MAXSIZE = UNLIMITED, FILEGROWTH = 64MB) ",
		(const char*)m_strRootName,
		(const char*)strDatabasePath);

	CString strPart3 = "";
	strPart3.Format("LOG ON (NAME = %s_Log, FILENAME = '%s', SIZE = 10MB, MAXSIZE = UNLIMITED, FILEGROWTH = 64MB)",
		(const char*)m_strRootName,
		(const char*)strLogFilePath);

	return (CSQLHelpers::ExecuteSQL(&m_database, strPart1 + strPart2 + strPart3) == SQLCRUD_ERR_NONE);

}

//**********************************************************************

void CSQLDb::Connect(int& nError, bool bCanCreate)
{
	nError = 0;
	
	if (TRUE == m_bDoneConnect)
	{
		return;
	}

	if (OpenDatabase(m_strRootName) == FALSE)
	{
		if (FALSE == bCanCreate)
		{
			nError = 1;
			return;
		}

		if (OpenDatabase("master") == FALSE)
		{
			nError = 2;
			return;
		}

		if (CreateSmartPayDb() == FALSE)
		{
			nError = 3;
			return;
		}

		m_database.Close();

		if (OpenDatabase(m_strRootName) == FALSE)
		{
			nError = 4;
			return;
		}
	}
	else
	{
		if (TRUE == bCanCreate)
		{
			if (g_GlobalState.GetFixedDatabaseOptionsFlag() == FALSE)
			{
				FixDatabaseOptions();
				g_GlobalState.SetFixedDatabaseOptionsFlag(TRUE);
			}
		}

		m_bDoneConnect = TRUE;
	}
}

//**********************************************************************

void CSQLDb::FixDatabaseOptions()
{
	CString SQL = "";

	SQL.Format("ALTER DATABASE [%s] SET AUTO_CLOSE OFF",
		(const char*)m_strRootName);

	CSQLHelpers::ExecuteSQL(&m_database, SQL);

	SQL.Format("ALTER DATABASE [%s] SET AUTO_SHRINK OFF",
		(const char*)m_strRootName);

	CSQLHelpers::ExecuteSQL(&m_database, SQL);
}

//**********************************************************************
#ifndef SYSTEMTYPE_SERVER
#ifndef SYSTEMTYPE_BACKGROUND
//**********************************************************************

void CSQLDb::CreateDatabaseSchema( int& nErrorContext, int& nErrorNumber )
{
	nErrorContext = 0;
	nErrorNumber = 0;
	CSQLBuildSchema BuildSchema{};
	BuildSchema.DoBuild( *this, nErrorContext, nErrorNumber);
}

//**********************************************************************
#endif
#endif
//**********************************************************************

int CSQLDb::GetDatabaseVersion(int& nError)
{
	int nVersion = 0;
	nError = SQLSRVR_CHECKVERSION_ERR_NOLINE;

	CSQLRecordSetDbVersion RecordSet(GetDb(),
		RSParams_DbVersion_NormalByLineId{ 1 });
		
	CSQLRowDbVersion RowVersion;
	if (RecordSet.StepSelectAll(RowVersion) == TRUE)
	{
		nVersion = RowVersion.GetVersion();

		if (nVersion != SMARTPAY_DBVERSION)
		{
			nError = SQLSRVR_CHECKVERSION_ERR_BADVER;
		}
		else
		{
			nError = SQLSRVR_CHECKVERSION_ERR_NONE;
		}
	}

	return nVersion;
}

//**********************************************************************

int CSQLDb::BackupDatabase()
{
	if (OpenDatabase("master") == FALSE)
	{
		return SQLCRUD_ERR_FAIL;
	}

	CString strSQL = "";
	strSQL.Format("BACKUP DATABASE SPOS_SmartPayDb TO DISK = '%s' WITH FORMAT, INIT",
		(const char*) Filenames.GetSQLBackupFilename());

	return CSQLHelpers::ExecuteSQL( &m_database, strSQL);
}

//**********************************************************************

int CSQLDb::RestoreDatabase()
{
	g_GlobalState.SetExecuteSQLLastError("");

	SQLConnectionPool.DeleteAllConnections();

	if (OpenDatabase("master") == FALSE)
	{
		return SQLCRUD_ERR_FAIL;
	}

	char buffer[4096];
	GetFullPathName(m_strDatabasePath, 4096, buffer, NULL);
	CString strDatabasePath = buffer;

	GetFullPathName(m_strLogPath, 4096, buffer, NULL);
	CString strLogFilePath = buffer;

	CString strSQL = "";
	strSQL.Format("RESTORE DATABASE SPOS_SmartPayDb FROM DISK = '%s' WITH REPLACE, MOVE 'SPOS_SmartPayDb_data' TO '%s', MOVE 'SPOS_SmartPayDb_Log' TO '%s'",
		(const char*)Filenames.GetSQLBackupFilename(),
		(const char*)strDatabasePath,
		(const char*)strLogFilePath);

	m_database.SetQueryTimeout(600);
	return CSQLHelpers::ExecuteSQL(&m_database, strSQL);
}

//**********************************************************************
