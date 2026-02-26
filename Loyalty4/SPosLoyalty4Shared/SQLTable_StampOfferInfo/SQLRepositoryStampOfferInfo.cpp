//**********************************************************************
#include "..\SQLConnectionPool.h"
#include "..\SQLDefines.h"
#include "..\SQLNamespaces.h"
#include "..\SQLDb.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRecordSetStampOfferInfo.h"
//**********************************************************************
#include "SQLRepositoryStampOfferInfo.h"
//**********************************************************************

CSQLRepositoryStampOfferInfo::CSQLRepositoryStampOfferInfo()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryStampOfferInfo::SelectRow(CSQLRowStampOfferInfo& SQLRowOS, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetStampOfferInfo RecordSet(pDatabase,
			RSParams_StampOffer_NormalByOfferId{ SQLRowOS.GetStampOfferID() });
			
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
				RecordSet.SaveToStampOfferInfoRow(SQLRowOS);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryStampOfferInfo::UpdateRow(CSQLRowStampOfferInfo& SQLRowOS, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetStampOfferInfo RecordSet(pDatabase,
			RSParams_StampOffer_NormalByOfferId{ SQLRowOS.GetStampOfferID() });

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
				RecordSet.LoadFromStampOfferInfoRow(SQLRowOS);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryStampOfferInfo::InsertRow(CSQLRowStampOfferInfo& SQLRowOS, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetStampOfferInfo RecordSet(pDatabase, RSParams_StampOffer_NormalNoParams{});
		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromStampOfferInfoRow(SQLRowOS);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryStampOfferInfo::DeleteRow(CSQLRowStampOfferInfo& SQLRowOS, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetStampOfferInfo RecordSet(pDatabase,
			RSParams_StampOffer_NormalByOfferId{ SQLRowOS.GetStampOfferID() });

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

void CSQLRepositoryStampOfferInfo::ImportStampOfferInfo(CString strFilename)
{
	CSSFile fileImport;
	if (fileImport.Open(strFilename, "rb") == FALSE)
	{
		return;
	}

	CString strHeader = "";
	if (fileImport.ReadString(strHeader) == FALSE)
	{
		return;
	}

	CStringArray Columns;
	Columns.Add("STAMPOFFERID");
	Columns.Add("EDITION");
	Columns.Add("ENABLE");
	Columns.Add("THRESHOLD");
	Columns.Add("EXPIRY");
	Columns.Add("DESCRIPTION");

	strHeader.MakeUpper();
	CCSV csv(strHeader);

	int nMatched = 0;
	for (; nMatched < Columns.GetSize() && nMatched < csv.GetSize(); nMatched++)
	{
		if (Columns.GetAt(nMatched) != csv.GetString(nMatched))
		{
			break;
		}
	}

	if (nMatched < 6)
	{
		return;
	}

	bool bDoneDelete = FALSE;

	{
		int nConnection = -1;
		CDatabase* pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);

		if (nConnection != -1)
		{
			bDoneDelete = (CSQLHelpers::ExecuteSQL(pDatabase, "DELETE FROM StampOfferInfo") == SQLCRUD_ERR_NONE);
			SQLConnectionPool.FreeConnection(nConnection);
		}
	}

	if (TRUE == bDoneDelete)
	{
		CString strStampOfferInfo = "";
		while (fileImport.ReadString(strStampOfferInfo) == TRUE)
		{
			CCSV csv(strStampOfferInfo);

			int nOfferID = csv.GetInt(0);
			if ((nOfferID < OfferStamp::StampOfferID.Min) || (nOfferID > OfferStamp::StampOfferID.Max))
			{
				continue;
			}

			CSQLRowStampOfferInfo RowStampOfferInfo;
			RowStampOfferInfo.SetStampOfferID(nOfferID);
			RowStampOfferInfo.SetEdition(csv.GetInt(1));
			RowStampOfferInfo.SetEnableFlag(csv.GetBool(2));
			RowStampOfferInfo.SetThreshold(csv.GetInt(3));
			RowStampOfferInfo.SetExpiry(csv.GetInt(4));
			RowStampOfferInfo.SetDescription(csv.GetString(5));

			InsertRow(RowStampOfferInfo, NULL);
		}
	}
}

//**********************************************************************

void CSQLRepositoryStampOfferInfo::GetEnabledOffers(CArray<CEnabledStampOfferInfo,CEnabledStampOfferInfo>& arrayOffers)
{
	arrayOffers.RemoveAll();

	CSQLRecordSetConnectionPool rsOffers(NULL);

	CString strSQL = "";
	strSQL.Format("SELECT %s, %s FROM %s WHERE %s = 1",
		StampOfferInfo::StampOfferID.Label,
		StampOfferInfo::Edition.Label,
		SQLTableNames::StampOfferInfo,
		StampOfferInfo::Enabled.Label);

	rsOffers.Open(CRecordset::forwardOnly, strSQL, CRecordset::executeDirect);

	while ( rsOffers.IsEOF() == FALSE )
	{
		CString strOfferID = "";
		CString strEdition = "";
	
		short nIndex = 0;
		rsOffers.GetFieldValue(nIndex, strOfferID);
		rsOffers.GetFieldValue(nIndex + 1, strEdition);
		
		CEnabledStampOfferInfo info;
		info.m_nOfferID = atoi(strOfferID);
		info.m_nEdition = atoi(strEdition);
		arrayOffers.Add(info);

		rsOffers.MoveNext();
	}
}

//**********************************************************************

void CSQLRepositoryStampOfferInfo::GetOfferName(int nOfferID, int& nLastOfferID, CString& strOfferName, CDatabase* pDatabase)
{
	if (nOfferID != nLastOfferID)
	{
		strOfferName = "";
		{
			CSQLRowStampOfferInfo RowOffer;
			RowOffer.SetStampOfferID(nOfferID);

			CSQLRepositoryStampOfferInfo repoInfo;
			if (repoInfo.SelectRow(RowOffer, pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				strOfferName = RowOffer.GetDisplayDescription();
			}
		}
		nLastOfferID = nOfferID;
	}
}

//**********************************************************************

CString CSQLRepositoryStampOfferInfo::GetOfferName(int nOfferID, CDatabase* pDatabase)
{
	CString strOfferName = "";
	int nLastOfferID = nOfferID - 1;
	GetOfferName(nOfferID, nLastOfferID, strOfferName, pDatabase);
	return strOfferName;
}

//**********************************************************************