//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRecordSetPebbleConfig.h"
//**********************************************************************
#include "SQLRepositoryPebbleConfig.h"
//**********************************************************************

CSQLRepositoryPebbleConfig::CSQLRepositoryPebbleConfig()
{
	CString strPath = System.GetWebPaymentPath("", nWEBPAYMENT_PEBBLE);	// creates folder if not found
	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleConfig::SelectRow(CSQLRowPebbleConfig& RowPC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleConfig RecordSet(pDatabase, 
			RSParams_PebbleConfig_NormalByLineId(RowPC.GetLineID()));

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
				RecordSet.SaveToPebbleConfigRow(RowPC);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleConfig::UpdateRow(CSQLRowPebbleConfig& RowPC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleConfig RecordSet(pDatabase,
			RSParams_PebbleConfig_NormalByLineId(RowPC.GetLineID()));

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
				RecordSet.LoadFromPebbleConfigRow(RowPC);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleConfig::InsertRow(CSQLRowPebbleConfig& RowPC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleConfig RecordSet(pDatabase,
			RSParams_PebbleConfig_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromPebbleConfigRow(RowPC);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleConfig::UpsertRow(CSQLRowPebbleConfig& RowPC, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowPC.GetLineID(), pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0 )
		{
			Result = InsertRow(RowPC, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowPC, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleConfig::DeleteRow(CSQLRowPebbleConfig& RowPC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleConfig RecordSet(pDatabase,
			RSParams_PebbleConfig_NormalByLineId(RowPC.GetLineID()));

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

CSQLResultInfo CSQLRepositoryPebbleConfig::RowExists(int nLineID, CDatabase* pDatabase)
{
	CSQLRecordSetPebbleConfig RecordSet(pDatabase,
		RSParams_PebbleConfig_NormalByLineId(nLineID), TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

void CSQLRepositoryPebbleConfig::LoadRow(CDatabase* pDatabase)
{
	m_Row.SetLineID(1);
	if (SelectRow(m_Row, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		m_Row.Reset();
	}
}

//**********************************************************************

__int64 CSQLRepositoryPebbleConfig::SaveRow(CDatabase* pDatabase)
{
	m_Row.SetLineID(1);
	return UpsertRow(m_Row, pDatabase).GetSQLError();
}

//**********************************************************************

bool CSQLRepositoryPebbleConfig::IsSet()
{
	if (m_Row.GetActiveInstallationID() == "")
	{
		return FALSE;
	}

	if (m_Row.GetActiveOrganisationID() == "")
	{
		return FALSE;
	}

	return TRUE;
}

//**********************************************************************

CString CSQLRepositoryPebbleConfig::GetLocateByLabelMisID()
{
	return GetTextLabel(m_Row.GetMISInfoIndex());
}

//*******************************************************************

CString CSQLRepositoryPebbleConfig::GetLocateByLabelUPN()
{
	return GetTextLabel(m_Row.GetUPNInfoIndex());
}

//*******************************************************************

CString CSQLRepositoryPebbleConfig::GetTextLabel(int n)
{
	CString strResult = "";

	switch (n)
	{
	case 1:
		strResult = Account::Info1.Label;
		break;

	case 2:
		strResult = Account::Info2.Label;
		break;

	case 3:
		strResult = Account::Info3.Label;
		break;

	case 4:
		strResult = Account::Info4.Label;
		break;

	case 5:
		strResult = Account::Info5.Label;
		break;

	case 6:
		strResult = Account::Info6.Label;
		break;

	case 7:
		strResult = Account::Info7.Label;
		break;

	case 8:
		strResult = Account::Info8.Label;
		break;
	}

	return strResult;
}

//*******************************************************************

CString CSQLRepositoryPebbleConfig::GetLocateByText(int n)
{
	CString strResult = "";

	switch (n)
	{
	case nPEBBLE_LOCATEBY_UPN:
		strResult = "UPN";
		break;

	case nPEBBLE_LOCATEBY_MIS:
		strResult = "MIS ID";
		break;
	}

	return strResult;
}

//*******************************************************************

CString CSQLRepositoryPebbleConfig::GetSQLFilterDateTime(bool bSale)
{
	COleDateTime dateFilter = COleDateTime::GetCurrentTime();

	CString strDate = ""; 
	CString strTime = "";
	
	if (TRUE == bSale)
	{
		strDate = m_Row.GetLastImportDateSale();
		strTime = m_Row.GetLastImportTimeSale();
	}
	else
	{
		strDate = m_Row.GetLastImportDateTopup();
		strTime = m_Row.GetLastImportTimeTopup();
	}

	{
		if (strDate.GetLength() == 8)
		{
			COleDateTime dateTry = COleDateTime(atoi(strDate.Left(4)), atoi(strDate.Mid(4, 2)), atoi(strDate.Right(2)), 0, 0, 0);
			if (dateTry.m_status == COleDateTime::valid)
			{
				dateFilter = dateTry;
			}
		}
	}

	{
		if (strTime.GetLength() == 6)
		{
			COleDateTime dateTry = COleDateTime(dateFilter.GetYear(), dateFilter.GetMonth(), dateFilter.GetDay(), atoi(strTime.Left(2)), atoi(strTime.Mid(2, 2)), atoi(strTime.Right(2)));
			if (dateTry.m_status == COleDateTime::valid)
			{
				dateFilter = dateTry;
			}
		}
	}

	if (TRUE == bSale)
	{
		if (m_Row.GetFirstImportSaleFlag() == FALSE)
		{
			dateFilter -= COleDateTimeSpan(0, 0, 15, 0);
		}
	}
	else
	{
		if (m_Row.GetFirstImportTopupFlag() == FALSE)
		{
			dateFilter -= COleDateTimeSpan(0, 0, 2, 0);
		}
	}

	CString strResult = "";
	strResult.Format("%4.4d-%2.2d-%2.2dT%2.2d:%2.2d:%2.2d",
		dateFilter.GetYear(),
		dateFilter.GetMonth(),
		dateFilter.GetDay(),
		dateFilter.GetHour(),
		dateFilter.GetMinute(),
		dateFilter.GetSecond());

	return strResult;
}

//**********************************************************************
