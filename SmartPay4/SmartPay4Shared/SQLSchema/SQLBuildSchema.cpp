//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLConnectionPool.h"
#include "..\SQLDefines.h"
#include "..\SQLDb.h"
#include "..\SQLHelpers.h"
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLBuilderCreateTable.h"
#include "SQLTranBuildSchema.h"
#include "SQLTranMigrateSchema.h"
//**********************************************************************
#include "SQLBuildSchema.h"
//**********************************************************************

CSQLBuildSchema::CSQLBuildSchema()
{
}

//**********************************************************************

void CSQLBuildSchema::DoBuild(CSQLDb& SQLDb, int& nErrorContext, int& nErrorNumber)
{
	long lTableCount = 0;

	{
		CSQLRecordSetConnectionPool rsCount(SQLDb.GetDb());
		rsCount.Open(CRecordset::forwardOnly, "SELECT COUNT(*) FROM sys.tables", CRecordset::executeDirect);

		if (rsCount.IsEOF())
		{
			nErrorContext = SQLSRVR_ERRCONTEXT_BUILDSCHEMA;
			nErrorNumber = SQLSRVR_BUILDSCHEMA_ERR_COUNTTABLES;
			return;
		}

		short nIndex = 0;
		CString strCount = "";
		{
			rsCount.GetFieldValue(nIndex, strCount);
		}

		lTableCount = atol(strCount);
		rsCount.Close();
	}

	if (0 == lTableCount)
	{
		if (BuildCurrentSchema() == TRUE)
		{
			nErrorContext = 0;
			nErrorNumber = 0;
		}
		else
		{
			nErrorContext = SQLSRVR_ERRCONTEXT_BUILDSCHEMA;
			nErrorNumber = SQLSRVR_BUILDSCHEMA_ERR_CREATESCHEMA;
		}
	}
	else
	{
		int nDbVersion = SQLDb.GetDatabaseVersion(nErrorNumber);

		switch (nErrorNumber)
		{
		case SQLSRVR_CHECKVERSION_ERR_NONE:
		case SQLSRVR_CHECKVERSION_ERR_BADVER:
			break;

		default:
			nErrorContext = SQLSRVR_ERRCONTEXT_CHECKVERSION;
			return;
		}

		if ((nDbVersion > SMARTPAY_DBVERSION) || (nDbVersion < 1))
		{
			nErrorContext = SQLSRVR_ERRCONTEXT_CHECKVERSION;
			nErrorNumber = SQLSRVR_CHECKVERSION_ERR_BADVER;
			return;
		}

		while (nDbVersion < SMARTPAY_DBVERSION)
		{
			int nDummy = 0;
			CSQLHelpers::SetLogFileSize(nDummy);

			if (Migrate(nDbVersion + 1) == TRUE)
			{
				nErrorContext = 0;
				nErrorNumber = 0;
				nDbVersion++;
			}
			else
			{
				nErrorContext = SQLSRVR_ERRCONTEXT_BUILDSCHEMA;
				nErrorNumber = SQLSRVR_BUILDSCHEMA_ERR_CREATESCHEMA;
				break;
			}

			CSQLHelpers::ShrinkLogFile();
		}
	}
}

//**********************************************************************

bool CSQLBuildSchema::BuildCurrentSchema()
{
	CSQLTranBuildSchema Tran;
	Tran.BeginTrans();
	Tran.DoWork();

	return (Tran.EndTrans() == SQLTRAN_STATE_COMMITOK);
}

//**********************************************************************

bool CSQLBuildSchema::Migrate(int nTargetVersion)
{
	int nDummy = 0;
	CSQLHelpers::SetLogFileSize(nDummy);

	CSQLTranMigrateSchema Tran;
	Tran.BeginTrans();
	Tran.DoWork(nTargetVersion);
	Tran.EndTrans();

	CSQLHelpers::ShrinkLogFile();

	return (Tran.GetState() == SQLTRAN_STATE_COMMITOK);
}

//**********************************************************************