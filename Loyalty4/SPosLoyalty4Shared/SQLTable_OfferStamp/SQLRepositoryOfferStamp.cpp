//**********************************************************************
#include "..\SQLConnectionPool.h"
#include "..\SQLDefines.h"
#include "..\SQLDb.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLTable_StampOfferInfo\SQLRepositoryStampOfferInfo.h"
//**********************************************************************
#include "SQLRecordSetOfferStamp.h"
//**********************************************************************
#include "SQLRepositoryOfferStamp.h"
//**********************************************************************

CSQLRepositoryOfferStamp::CSQLRepositoryOfferStamp()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryOfferStamp::SelectRow(CSQLRowOfferStamp& SQLRowOS, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetOfferStamp RecordSet(pDatabase,
			RSParams_OfferStamp_NormalByFullKey{
				SimpleFormatInt64Value(SQLRowOS.GetUserID()),
				SQLRowOS.GetStampOfferID(),
				SQLRowOS.GetEdition(),
				SQLRowOS.GetAwardDate(),
				SQLRowOS.GetExpireDate() });

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
				RecordSet.SaveToOfferStampRow(SQLRowOS);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryOfferStamp::UpdateRow(CSQLRowOfferStamp& SQLRowOS, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetOfferStamp RecordSet(pDatabase,
			RSParams_OfferStamp_NormalByFullKey{
				SimpleFormatInt64Value(SQLRowOS.GetUserID()),
				SQLRowOS.GetStampOfferID(),
				SQLRowOS.GetEdition(),
				SQLRowOS.GetAwardDate(),
				SQLRowOS.GetExpireDate() });

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
				RecordSet.LoadFromOfferStampRow(SQLRowOS);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryOfferStamp::InsertRow(CSQLRowOfferStamp& SQLRowOS, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetOfferStamp RecordSet(pDatabase, RSParams_OfferStamp_NormalNoParams{});
		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromOfferStampRow(SQLRowOS);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryOfferStamp::DeleteRow(CSQLRowOfferStamp& SQLRowOS, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetOfferStamp RecordSet(pDatabase,
			RSParams_OfferStamp_NormalByFullKey{
				SimpleFormatInt64Value(SQLRowOS.GetUserID()),
				SQLRowOS.GetStampOfferID(),
				SQLRowOS.GetEdition(),
				SQLRowOS.GetAwardDate(),
				SQLRowOS.GetExpireDate() });

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

void CSQLRepositoryOfferStamp::GetStampCounts( __int64 nUserID, CReportConsolidationArray<CStampOfferRSPBuffer>& arrayOfferStamps)
{
	arrayOfferStamps.RemoveAll();

	CString strDateToday = "";
	{
		COleDateTime dateToday = COleDateTime::GetCurrentTime();
		strDateToday.Format("%4.4d%2.2d%2.2d",
			dateToday.GetYear(),
			dateToday.GetMonth(),
			dateToday.GetDay());
	}

	{
		CArray<CEnabledStampOfferInfo, CEnabledStampOfferInfo> arrayOffers;
		
		CSQLRepositoryStampOfferInfo repoInfo;
		repoInfo.GetEnabledOffers(arrayOffers);

		for (int n = 0; n < arrayOffers.GetSize(); n++)
		{
			CStampOfferRSPBuffer item;
			item.m_nOfferID = arrayOffers[n].m_nOfferID;
			item.m_nEdition = arrayOffers[n].m_nEdition;
			item.m_strOfferName = "";
			item.m_nStampCount = 0;
			item.m_nThreshold = 0;
			arrayOfferStamps.Consolidate(item);
		}

		CSQLRecordSetConnectionPool rsCount(NULL);
		
		CString strSQL = "";
		strSQL.Format("SELECT %s, %s, SUM(%s) AS Stamps FROM %s WHERE ( %s=%I64d AND %s >= %s) GROUP BY %s, %s",
			OfferStamp::StampOfferID.Label,
			OfferStamp::Edition.Label,
			OfferStamp::StampCount.Label,
			SQLTableNames::OfferStamps,
			OfferStamp::UserID.Label,
			nUserID,
			OfferStamp::ExpireDate.Label,
			(const char*)strDateToday,
			OfferStamp::StampOfferID.Label,
			OfferStamp::Edition.Label);
		
		rsCount.Open(CRecordset::forwardOnly, strSQL, CRecordset::executeDirect);

		while (rsCount.IsEOF() == FALSE)
		{
			CString strOfferID = "";
			CString strEdition = "";
			CString strCount = "";

			short nIndex = 0;
			rsCount.GetFieldValue(nIndex, strOfferID);
			rsCount.GetFieldValue(nIndex + 1, strEdition);
			rsCount.GetFieldValue(nIndex + 2, strCount);

			CStampOfferRSPBuffer item;
			item.m_nOfferID = atoi(strOfferID);
			item.m_nEdition = atoi(strEdition);

			int nPos = 0;
			if (arrayOfferStamps.Find(item, nPos) == TRUE)
			{
				item.m_nStampCount = atoi(strCount);
				arrayOfferStamps.SetAt(nPos, item);
			}

			rsCount.MoveNext();
		}
	}
}

//**********************************************************************

void CSQLRepositoryOfferStamp::GetOfferStampsByUserID(__int64 nUserID, CArray<CSQLRowOfferStamp, CSQLRowOfferStamp>& arrayStamps, CDatabase* pDatabase)
{
	arrayStamps.RemoveAll();

	CSQLRecordSetOfferStamp RecordSet(pDatabase,
		RSParams_OfferStamp_NormalByUserId{SimpleFormatInt64Value(nUserID) });
		
	CSQLRowOfferStamp RowOS;
	while (RecordSet.StepSelectAll(RowOS) == TRUE)
	{
		arrayStamps.Add(RowOS);
	}
}

//**********************************************************************

void CSQLRepositoryOfferStamp::GetOfferStampsByUserIDAndOfferID(__int64 nUserID, int nOfferID, CArray<CSQLRowOfferStamp, CSQLRowOfferStamp>& arrayStamps)
{
	arrayStamps.RemoveAll();

	CSQLRecordSetOfferStamp RecordSet(NULL,
		RSParams_OfferStamp_NormalByUserIdOfferId{
			SimpleFormatInt64Value(nUserID),
			nOfferID });
		
	CSQLRowOfferStamp RowOS;
	while (RecordSet.StepSelectAll(RowOS) == TRUE)
	{
		arrayStamps.Add(RowOS);
	}
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryOfferStamp::CountRowsByUserID(__int64 nUserID, CDatabase* pDatabase)
{
	CSQLRecordSetOfferStamp RecordSet(pDatabase,
		RSParams_OfferStamp_RowCountByUserId{
			SimpleFormatInt64Value(nUserID) });

	return RecordSet.RSCountRowsByRange();
}

//**********************************************************************
