//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\..\SmartPay4ManagerBgnd\LegacyWebPaymentOptions.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRecordSetWebPaymentOptions.h"
//**********************************************************************
#include "SQLRepositoryWebPaymentOptions.h"
//**********************************************************************

CSQLRepositoryWebPaymentOptions::CSQLRepositoryWebPaymentOptions()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryWebPaymentOptions::SelectRow(CSQLRowWebPaymentOptions& RowPC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetWebPaymentOptions RecordSet(pDatabase,
			RSParams_WebPaymentOptions_NormalByLineId(RowPC.GetLineID()));

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
				RecordSet.SaveToWebPaymentOptionsRow(RowPC);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryWebPaymentOptions::UpdateRow(CSQLRowWebPaymentOptions& RowPC, CDatabase* pDatabase, CString strVersion)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetWebPaymentOptions RecordSet(pDatabase,
			RSParams_WebPaymentOptions_NormalByLineId(RowPC.GetLineID()));

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
				RecordSet.LoadFromWebPaymentOptionsRow(RowPC);
				Result.SetSQLError(RecordSet.WrappedUpdate(strVersion));
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryWebPaymentOptions::InsertRow(CSQLRowWebPaymentOptions& RowPC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetWebPaymentOptions RecordSet(pDatabase,
			RSParams_WebPaymentOptions_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromWebPaymentOptionsRow(RowPC);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryWebPaymentOptions::UpsertRow(CSQLRowWebPaymentOptions& RowPC, CDatabase* pDatabase, CString strVersion)
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
			Result = UpdateRow(RowPC, pDatabase, strVersion);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryWebPaymentOptions::DeleteRow(CSQLRowWebPaymentOptions& RowPC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetWebPaymentOptions RecordSet(pDatabase,
			RSParams_WebPaymentOptions_NormalByLineId(RowPC.GetLineID()));

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

CSQLResultInfo CSQLRepositoryWebPaymentOptions::RowExists(int nLineID, CDatabase* pDatabase)
{
	CSQLRecordSetWebPaymentOptions RecordSet(pDatabase,
		RSParams_WebPaymentOptions_NormalByLineId(nLineID), TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

void CSQLRepositoryWebPaymentOptions::LoadRow(CDatabase* pDatabase)
{
	m_Row.SetLineID(1);
	if (SelectRow(m_Row, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		m_Row.Reset();
	}
}

//**********************************************************************

__int64 CSQLRepositoryWebPaymentOptions::SaveRow(CDatabase* pDatabase, bool bCheckVersion)
{
	CString strVersion = "";
	if (TRUE == bCheckVersion)
	{
		strVersion = m_Row.GetRowVersionString();
	}

	m_Row.SetLineID(1);
	return UpsertRow(m_Row, pDatabase, strVersion).GetSQLError();
}

//**********************************************************************

bool CSQLRepositoryWebPaymentOptions::CheckVersionChange()
{
	bool bResult = FALSE;

	CSQLRowWebPaymentOptions TempRow;
	TempRow.SetLineID(1);
	
	if (SelectRow(TempRow, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		bResult = TRUE;
	}
	else
	{
		bResult = (TempRow.CompareRowVersion(m_Row) == FALSE);
	}

	return bResult;
}

//**********************************************************************

bool CSQLRepositoryWebPaymentOptions::ImportLegacyOptions(CDatabase* pDatabase)
{
	CSQLRowWebPaymentOptions RowOptions;

	CLegacyWebPaymentOptions LegacyOptions;
	LegacyOptions.Read();

	//ACCOUNT LINK SETTINGS ARE NOT IN LEGACY DAT
	LoadRow(pDatabase);
	m_Row.SetPaymentDays(LegacyOptions.m_nPaymentDayFlags);
	m_Row.SetPaymentStart(LegacyOptions.m_strPaymentStart);
	m_Row.SetPaymentEnd(LegacyOptions.m_strPaymentEnd);
	m_Row.SetPaymentLastDate(LegacyOptions.m_strDateLastPayment);
	m_Row.SetPaymentLastTime(LegacyOptions.m_strTimeLastPayment);
	m_Row.SetBalanceLastDate(LegacyOptions.m_strDateLastBalance);
	m_Row.SetBalanceLastTime(LegacyOptions.m_strTimeLastBalance);
	m_Row.SetTransactionLastDate(LegacyOptions.m_strDateLastTransaction);
	m_Row.SetTransactionLastTime(LegacyOptions.m_strTimeLastTransaction);
	m_Row.SetExclude1Flag(LegacyOptions.m_bExclude1Reqd);
	m_Row.SetExclude1Start(LegacyOptions.m_strExclude1Start);
	m_Row.SetExclude1End(LegacyOptions.m_strExclude1End);
	m_Row.SetExclude2Flag(LegacyOptions.m_bExclude2Reqd);
	m_Row.SetExclude2Start(LegacyOptions.m_strExclude2Start);
	m_Row.SetExclude2End(LegacyOptions.m_strExclude2End);
	m_Row.SetExclude3Flag(LegacyOptions.m_bExclude3Reqd);
	m_Row.SetExclude3Start(LegacyOptions.m_strExclude3Start);
	m_Row.SetExclude3End(LegacyOptions.m_strExclude3End);
	m_Row.SetExclude4Flag(LegacyOptions.m_bExclude4Reqd);
	m_Row.SetExclude4Start(LegacyOptions.m_strExclude4Start);
	m_Row.SetExclude4End(LegacyOptions.m_strExclude4End);
	m_Row.SetExcludeUploadStart(LegacyOptions.m_strExcludeUploadStart);
	m_Row.SetExcludeUploadEnd(LegacyOptions.m_strExcludeUploadEnd);
	m_Row.SetEODAfterPaymentFlag(LegacyOptions.m_bEODAfterPayments);
	m_Row.SetEODAfterTime(LegacyOptions.m_strSendEODAfter);
	m_Row.SetPromptBeforeAccessFlag(LegacyOptions.m_bPromptBeforeAccess);
	m_Row.SetImportAccountsFlag(LegacyOptions.m_bImportAccounts);

	return (SaveRow(pDatabase) == SQLCRUD_ERR_NONE);
}

//**********************************************************************
