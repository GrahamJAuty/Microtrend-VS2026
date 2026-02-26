//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
#include "..\SQLRecordSetExists.h"
//**********************************************************************
#include "SQLAccountManager.h"
//**********************************************************************
#include "SQLPrepStatLeaver.h"
//**********************************************************************

CSQLPrepStatLeaver::CSQLPrepStatLeaver( CString strTableName )
{
	m_strTableName = strTableName;

	m_strPrimaryKeyFilter = "";
	m_strPrimaryKeyFilter += Account::LeaverYear.Label;
	m_strPrimaryKeyFilter += " = ? AND ";
	m_strPrimaryKeyFilter += Account::UserID.Label;
	m_strPrimaryKeyFilter += " = ?";
}

//**********************************************************************

CSQLResultInfo CSQLPrepStatLeaver::SelectRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase)
{
	CSQLResultInfo Result;

	try
	{
		CSQLRecordSetAccountFull RecordSet(m_strTableName, pDatabase, ACCOUNTSET_NORMAL_LEAVER);
		RecordSet.m_strFilter = m_strPrimaryKeyFilter;
		RecordSet.m_strParamLeaverYear.Format("%d", SQLAccountManager.GetLeaverYear());
		RecordSet.m_strParamUserID = SQLRowAccount.GetUserIDString();
	
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

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLPrepStatLeaver::SelectRowFind(CSQLRowAccountFind& SQLRowAccount, CDatabase* pDatabase)
{
	CSQLResultInfo Result;

	try
	{
		CSQLRecordSetAccountFull RecordSet(m_strTableName, pDatabase, ACCOUNTSET_FIND_LEAVER,2);
		RecordSet.m_strFilter = m_strPrimaryKeyFilter;
		RecordSet.m_strParamExport[0].Format("%d", SQLAccountManager.GetLeaverYear());
		RecordSet.m_strParamExport[1] = FormatInt64Value(SQLRowAccount.GetUserID());

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

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLPrepStatLeaver::UpdateRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase)
{
	CSQLResultInfo Result;

	try
	{
		CSQLRecordSetAccountFull RecordSet(m_strTableName, pDatabase, ACCOUNTSET_NORMAL_LEAVER);
		RecordSet.m_strFilter = m_strPrimaryKeyFilter;
		RecordSet.m_strParamLeaverYear.Format("%d", SQLAccountManager.GetLeaverYear());
		RecordSet.m_strParamUserID = SQLRowAccount.GetUserIDString();

		Result.SetSQLError(SQLCRUD_ERR_NONE);

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else if (RecordSet.IsEOF() == TRUE)
		{
			Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
		}
		else
		{
			RecordSet.Edit();
			RecordSet.LoadFromAccountRow(SQLRowAccount);
			Result.SetSQLError(RecordSet.WrappedUpdate());
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

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLPrepStatLeaver::InsertRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase)
{
	CSQLResultInfo Result;

	try
	{
		CSQLRecordSetAccountFull RecordSet(m_strTableName, pDatabase,ACCOUNTSET_NORMAL_LEAVER);
		RecordSet.m_strFilter = "1 = 0";

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromAccountRow(SQLRowAccount);
			RecordSet.m_nLeaverYear = SQLAccountManager.GetLeaverYear();
			Result.SetSQLError(RecordSet.WrappedUpdate());
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

	return Result;
}

//**********************************************************************

//THIS FUNCTION IS FOR USE WITH DATABASE RESTORE
//THE DIFFERENCE AS PER NORMAL INSERT IS THAT THE YEAR IS TAKEN FROM THE LEAVERS RECORD
CSQLResultInfo CSQLPrepStatLeaver::InsertDirect(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase)
{
	CSQLResultInfo Result;

	try
	{
		CSQLRecordSetAccountFull RecordSet(m_strTableName, pDatabase, ACCOUNTSET_NORMAL_LEAVER);
		RecordSet.m_strFilter = "1 = 0";

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromAccountRow(SQLRowAccount);
			Result.SetSQLError(RecordSet.WrappedUpdate());
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

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLPrepStatLeaver::UpsertRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(SQLRowAccount.GetUserID(), pDatabase);

	if ( Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0 )
		{
			Result = InsertRow(SQLRowAccount, pDatabase);
		}
		else
		{
			Result = UpdateRow(SQLRowAccount, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLPrepStatLeaver::DeleteRow(CSQLRowAccountFull& SQLRowAccount, CDatabase* pDatabase)
{
	CSQLResultInfo Result;

	try
	{
		CSQLRecordSetAccountFull RecordSet(m_strTableName, pDatabase,ACCOUNTSET_NORMAL_LEAVER);
		RecordSet.m_strFilter = m_strPrimaryKeyFilter;
		RecordSet.m_strParamLeaverYear.Format("%d", SQLAccountManager.GetLeaverYear());
		RecordSet.m_strParamUserID = SQLRowAccount.GetUserIDString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else if (RecordSet.IsEOF() == FALSE)
		{
			RecordSet.Delete();
			RecordSet.Close();
			Result.SetSQLError(SQLCRUD_ERR_NONE);
		}
		else
		{
			Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
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

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLPrepStatLeaver::RowExists(__int64 nUserID, CDatabase* pDatabase)
{
	CString strYear = "";
	strYear.Format("%d", SQLAccountManager.GetLeaverYear());
	CSQLRecordSetExists RecordSet(pDatabase, 2);
	return RecordSet.CheckSQLRow(m_strTableName, m_strPrimaryKeyFilter, strYear, FormatInt64Value(nUserID));
}

//**********************************************************************

CSQLResultInfo CSQLPrepStatLeaver::CountRows(CDatabase* pDatabase)
{
	CString strYear = "";
	strYear.Format("%d", SQLAccountManager.GetLeaverYear());
	CSQLRecordSetExists RecordSet(pDatabase, 2);
	return RecordSet.CountSQLRows(m_strTableName, Account::LeaverYear.Label, strYear, strYear);
}

//**********************************************************************

void CSQLPrepStatLeaver::ClearUserIDCache()
{
	m_UserIDCache.RemoveAll();
}

//**********************************************************************

void CSQLPrepStatLeaver::BuildUserIDCache()
{
	ClearUserIDCache();

	try
	{
		CString strSQL = "";
		strSQL.Format("SELECT %s FROM %s WHERE %s = %d",
			(const char*)Account::UserID.Label,
			(const char*)m_strTableName,
			(const char*)Account::LeaverYear.Label,
			SQLAccountManager.GetLeaverYear());

		CSQLRecordSetConnectionPool rsUserID(NULL);

		if (rsUserID.Open(CRecordset::forwardOnly, strSQL, CRecordset::executeDirect) == TRUE)
		{
			while (rsUserID.IsEOF() == FALSE)
			{
				short nIndex = 0;
				CString strResult = "";
				{
					rsUserID.GetFieldValue(nIndex, strResult);

					CSortedInt64Item item;
					item.m_nItem = _atoi64(strResult);
					m_UserIDCache.Consolidate(item);

					rsUserID.MoveNext();
				}
			}

			rsUserID.Close();
		}
	}
	catch (CDBException* pe)
	{
		pe->Delete();
	}
}

//**********************************************************************

bool CSQLPrepStatLeaver::CheckUserIDCache(__int64 nUserID)
{
	CSortedInt64Item item;
	item.m_nItem = nUserID;

	int nPos = 0;
	return m_UserIDCache.Find(item, nPos);
}

//**********************************************************************

