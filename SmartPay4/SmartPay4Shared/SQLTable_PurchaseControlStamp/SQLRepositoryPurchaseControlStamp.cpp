//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "..\SQLConnectionPool.h"
//**********************************************************************
#include "..\SQLTable_PurchaseControlRule\SQLRepositoryPurchaseControlRule.h"
//**********************************************************************
#include "SQLRecordSetPurchaseControlStamp.h"
//**********************************************************************
#include "SQLRepositoryPurchaseControlStamp.h"
//**********************************************************************

CSQLRepositoryPurchaseControlStamp::CSQLRepositoryPurchaseControlStamp()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPurchaseControlStamp::SelectRow(CSQLRowPurchaseControlStamp& RowPCS, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPurchaseControlStamp RecordSet(pDatabase, 
			RSParams_PurchaseControlStamp_NormalByUserIDRuleIDDates{
				SimpleFormatInt64Value(RowPCS.GetUserID()),
				RowPCS.GetRuleID(),
				RowPCS.GetAwardDate(),
				RowPCS.GetExpireDate()
			});

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
				RecordSet.SaveToPurchaseControlStampRow(RowPCS);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPurchaseControlStamp::UpdateRow(CSQLRowPurchaseControlStamp& RowPCS, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPurchaseControlStamp RecordSet(pDatabase,
			RSParams_PurchaseControlStamp_NormalByUserIDRuleIDDates{
				SimpleFormatInt64Value(RowPCS.GetUserID()),
				RowPCS.GetRuleID(),
				RowPCS.GetAwardDate(),
				RowPCS.GetExpireDate()
			});

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
				RecordSet.LoadFromPurchaseControlStampRow(RowPCS);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPurchaseControlStamp::InsertRow(CSQLRowPurchaseControlStamp& RowPCS, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPurchaseControlStamp RecordSet(pDatabase,
			RSParams_PurchaseControlStamp_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromPurchaseControlStampRow(RowPCS);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPurchaseControlStamp::UpsertRow(CSQLRowPurchaseControlStamp& RowPCS, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowPCS, pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0 )
		{
			Result = InsertRow(RowPCS, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowPCS, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPurchaseControlStamp::DeleteRow(CSQLRowPurchaseControlStamp& RowPCS, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPurchaseControlStamp RecordSet(pDatabase,
			RSParams_PurchaseControlStamp_NormalByUserIDRuleIDDates{
				SimpleFormatInt64Value(RowPCS.GetUserID()),
				RowPCS.GetRuleID(),
				RowPCS.GetAwardDate(),
				RowPCS.GetExpireDate()
			});

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

CSQLResultInfo CSQLRepositoryPurchaseControlStamp::RowExists(CSQLRowPurchaseControlStamp& RowPCS, CDatabase* pDatabase)
{
	CSQLRecordSetPurchaseControlStamp RecordSet(pDatabase,
		RSParams_PurchaseControlStamp_NormalByUserIDRuleIDDates{
			SimpleFormatInt64Value(RowPCS.GetUserID()),
			RowPCS.GetRuleID(),
			RowPCS.GetAwardDate(),
			RowPCS.GetExpireDate()
		}, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

void CSQLRepositoryPurchaseControlStamp::GetStampCounts( __int64 nUserID, CReportConsolidationArray<CPurchaseControlRSPBuffer>& arrayPurchaseControlStamps)
{
	arrayPurchaseControlStamps.RemoveAll();

	CString strDateToday = "";
	{
		COleDateTime dateToday = COleDateTime::GetCurrentTime();
		strDateToday.Format("%4.4d%2.2d%2.2d",
			dateToday.GetYear(),
			dateToday.GetMonth(),
			dateToday.GetDay());
	}

	{
		CArray<CEnabledPurchaseControlInfo, CEnabledPurchaseControlInfo> arrayRules;
		
		CSQLRepositoryPurchaseControlRule RepoRule;
		RepoRule.GetEnabledRules(arrayRules);

		for (int n = 0; n < arrayRules.GetSize(); n++)
		{
			CPurchaseControlRSPBuffer item;
			item.m_nRuleID = arrayRules[n].m_nRuleID;
			item.m_strRuleName = "";
			item.m_nStampCount = 0;
			item.m_nThreshold = 0;
			arrayPurchaseControlStamps.Consolidate(item);
		}

		CSQLRecordSetConnectionPool rsCount(NULL);
		
		CString strSQL = "";
		strSQL.Format("SELECT %s, SUM(%s) AS Stamps FROM %s WHERE ( %s=%I64d AND %s >= %s) GROUP BY %s",
			PurchaseControlStamp::RuleID.Label,
			PurchaseControlStamp::StampCount.Label,
			SQLTableNames::PurchaseControlStamps,
			PurchaseControlStamp::UserID.Label,
			nUserID,
			PurchaseControlStamp::ExpireDate.Label,
			(const char*)strDateToday,
			PurchaseControlStamp::RuleID.Label);
			
		rsCount.Open(CRecordset::forwardOnly, strSQL, CRecordset::executeDirect);

		while (rsCount.IsEOF() == FALSE)
		{
			CString strRuleID = "";
			CString strCount = "";

			short nIndex = 0;
			rsCount.GetFieldValue(nIndex, strRuleID);
			rsCount.GetFieldValue(nIndex + 1, strCount);

			CPurchaseControlRSPBuffer item;
			item.m_nRuleID = atoi(strRuleID);
		
			int nPos = 0;
			if (arrayPurchaseControlStamps.Find(item, nPos) == TRUE)
			{
				item.m_nStampCount = atoi(strCount);
				arrayPurchaseControlStamps.SetAt(nPos, item);
			}

			rsCount.MoveNext();
		}
	}
}

//**********************************************************************

void CSQLRepositoryPurchaseControlStamp::GetPurchaseControlStampsByUserID(__int64 nUserID, CArray<CSQLRowPurchaseControlStamp, CSQLRowPurchaseControlStamp>& arrayStamps, CDatabase* pDatabase)
{
	arrayStamps.RemoveAll();

	CSQLRecordSetPurchaseControlStamp RecordSet(NULL,
		RSParams_PurchaseControlStamp_NormalByUserID
		{
			SimpleFormatInt64Value(nUserID)
		});

	CSQLRowPurchaseControlStamp RowPCS;
	while (RecordSet.StepSelectAll(RowPCS) == TRUE)
	{
		arrayStamps.Add(RowPCS);
	}
}

//**********************************************************************

void CSQLRepositoryPurchaseControlStamp::GetPurchaseControlStampsByUserIDAndRuleID(__int64 nUserID, int nRuleID, CArray<CSQLRowPurchaseControlStamp, CSQLRowPurchaseControlStamp>& arrayStamps)
{
	arrayStamps.RemoveAll();

	CSQLRecordSetPurchaseControlStamp RecordSet(NULL, 
		RSParams_PurchaseControlStamp_NormalByUserIDRuleID
		{
			SimpleFormatInt64Value(nUserID),
			nRuleID
		});

	CSQLRowPurchaseControlStamp RowPCS;
	while (RecordSet.StepSelectAll(RowPCS) == TRUE)
	{
		arrayStamps.Add(RowPCS);
	}
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPurchaseControlStamp::CountRowsByUserID(__int64 nUserID, CDatabase* pDatabase)
{
	CSQLRecordSetPurchaseControlStamp RecordSet(pDatabase,
		RSParams_PurchaseControlStamp_CountRowsByUserID
		{
			SimpleFormatInt64Value(nUserID)
		});

	return RecordSet.RSCountRowsByRange();
}

//**********************************************************************
