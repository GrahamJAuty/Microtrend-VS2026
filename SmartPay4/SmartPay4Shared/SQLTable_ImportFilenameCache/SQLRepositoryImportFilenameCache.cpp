//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "..\SQLConnectionPool.h"
#include "..\SQLWhereBuilder.h"
//**********************************************************************
#include "SQLRepositoryImportFilenameCache.h"
//**********************************************************************

CSQLRepositoryImportFilenameCache::CSQLRepositoryImportFilenameCache()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryImportFilenameCache::SelectRow(CSQLRowImportFilenameCache& RowIFC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetImportFilenameCache RecordSet(pDatabase, 
			RSParams_ImportFilenameCache_NormalByLineId{ SimpleFormatInt64Value(RowIFC.GetLineID()) });

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
				RecordSet.SaveToImportFilenameCacheRow(RowIFC);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryImportFilenameCache::UpdateRow(CSQLRowImportFilenameCache& RowIFC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetImportFilenameCache RecordSet(pDatabase,
			RSParams_ImportFilenameCache_NormalByLineId{ SimpleFormatInt64Value(RowIFC.GetLineID()) });

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
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
				RecordSet.Edit();
				RecordSet.LoadFromImportFilenameCacheRow(RowIFC);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryImportFilenameCache::InsertRow(CSQLRowImportFilenameCache& RowIFC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetImportFilenameCache RecordSet(pDatabase,
			RSParams_ImportFilenameCache_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromImportFilenameCacheRow(RowIFC);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryImportFilenameCache::DeleteRow(CSQLRowImportFilenameCache& RowIFC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetImportFilenameCache RecordSet(pDatabase,
			RSParams_ImportFilenameCache_NormalByLineId{ SimpleFormatInt64Value(RowIFC.GetLineID()) });

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			if (RecordSet.IsEOF() == FALSE)
			{
				RecordSet.Delete();
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}
			else
			{
				Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryImportFilenameCache::RowExists(__int64 nLineID, CDatabase* pDatabase)
{
	CSQLRecordSetImportFilenameCache RecordSet(pDatabase, 
		RSParams_ImportFilenameCache_NormalByLineId{ SimpleFormatInt64Value(nLineID) }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

bool CSQLRepositoryImportFilenameCache::CheckCache(int nType, CString strFilename, int nHours)
{
	bool bReply = FALSE;

	int nConnection = -1;
	CDatabase* pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);

	if ( nConnection != -1 )
	{
		COleDateTime dateFilter = COleDateTime::GetCurrentTime();
		dateFilter -= COleDateTimeSpan(0, nHours, 0, 0);

		CString strSQL = "";
		strSQL.Format("DELETE FROM %s WHERE %s = %d AND %s < '%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d'",
			SQLTableNames::ImportFilenameCache,
			ImportFilenameCache::Type.Label,
			nType,
			ImportFilenameCache::DateTime.Label,
			dateFilter.GetYear(), dateFilter.GetMonth(), dateFilter.GetDay(),
			dateFilter.GetHour(), dateFilter.GetMinute(), dateFilter.GetSecond());

		CSQLHelpers::ExecuteSQL(pDatabase, strSQL);
		SQLConnectionPool.FreeConnection(nConnection);
	}

	CSQLBuilderBind BuilderBind;
	CSQLWhereBuilder WhereBuilder;
	WhereBuilder.AppendConditionInt(BuilderBind, ImportFilenameCache::Type.Label, szEQUAL, nType);
	WhereBuilder.AppendAnd();
	WhereBuilder.AppendConditionString(BuilderBind, ImportFilenameCache::Filename.Label, szEQUAL, strFilename);

	CString strParamType = "";
	{
		CSQLBuilderBindField Field;
		BuilderBind.GetField(0, Field);
		strParamType = Field.GetFieldAsString();
	}

	CString strParamFilename = "";
	{
		CSQLBuilderBindField Field;
		BuilderBind.GetField(1, Field);
		strParamFilename = Field.GetFieldAsString();
	}

	CSQLRecordSetImportFilenameCache RSCache(NULL, 
		RSParams_ImportFilenameCache_NormalByTypeAndFilename{
			strParamType, 
			strParamFilename });
	
	CSQLRowImportFilenameCache RowIFC;
	RSCache.m_strFilter = WhereBuilder.GetWhereString();
	bReply = RSCache.StepSelectAll(RowIFC);
	return bReply;
}

//**********************************************************************
