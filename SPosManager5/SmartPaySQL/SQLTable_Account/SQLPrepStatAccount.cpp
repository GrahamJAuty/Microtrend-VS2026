//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLConnectionPool.h"
//**********************************************************************
#include "SQLPrepStatAccount.h"
//**********************************************************************

CSQLPrepStatAccount::CSQLPrepStatAccount()
{
}

//**********************************************************************

CSQLResultInfo CSQLPrepStatAccount::SelectRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase)
{
	CSQLResultInfo Result;
	Result.SetSQLError(SQLCRUD_ERR_NONE);

	int nConnection = -1;

	try
	{
		CSQLRecordSetAccountFull RecordSet(pDatabase,
			RSParams_AccountFull_ById{SQLRowAccount.GetUserIDString() });
		
		nConnection = RecordSet.GetConnection();
	
		if (RecordSet.Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			if (RecordSet.IsEOF() == TRUE)
			{
				Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
			}
			else
			{
				RecordSet.SaveToAccountRow(SQLRowAccount);

				RecordSet.MoveNext();
				if (RecordSet.IsEOF() == FALSE)
				{
					Result.SetSQLError(SQLCRUD_ERR_DUPLICATE);
				}
			}

			RecordSet.Close();
		}
	}
	catch (CDBException* pe)
	{
		pe->Delete();
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_DB);
	}
	catch (...)
	{
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_MISC);
	}

	if (nConnection != -1)
	{
		switch (Result.GetSQLError())
		{
		case SQLCRUD_ERR_EXCEPT_DB:
		case SQLCRUD_ERR_EXCEPT_MISC:
		case SQLCRUD_ERR_RESET:
			SQLConnectionPool.DeleteConnection(nConnection);
			break;
		}
	}

	return Result;
}

//**********************************************************************


