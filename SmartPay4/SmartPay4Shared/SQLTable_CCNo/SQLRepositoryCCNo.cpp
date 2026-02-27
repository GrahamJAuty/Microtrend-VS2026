//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLConnectionPool.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
#include "..\SQLDb.h"
//**********************************************************************
#include "SQLRepositoryCCno.h"
//**********************************************************************

CSQLRepositoryCCNo::CSQLRepositoryCCNo()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryCCNo::SelectRow(CSQLRowCCNo& RowCC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetCCNo RecordSet(pDatabase, RSParams_CCNo_NormalByType{RowCC.GetTypeString()});
		
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
				RecordSet.SaveToCCNoRow(RowCC);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryCCNo::UpdateRow(CSQLRowCCNo& RowCC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetCCNo RecordSet(pDatabase, RSParams_CCNo_NormalByType{ RowCC.GetTypeString() });
		
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
			RecordSet.LoadFromCCNoRow(RowCC);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryCCNo::InsertRow(CSQLRowCCNo& RowCC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetCCNo RecordSet(pDatabase, RSParams_CCNo_NormalNoParams{});
		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromCCNoRow(RowCC);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryCCNo::UpsertRow(CSQLRowCCNo& RowCC, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowCC.GetTypeString(), pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0)
		{
			Result = InsertRow(RowCC, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowCC, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryCCNo::DeleteRow(CSQLRowCCNo& RowCC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetCCNo RecordSet(pDatabase, RSParams_CCNo_NormalByType{ RowCC.GetTypeString() });
		
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
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryCCNo::RowExists(CString strType, CDatabase* pDatabase)
{
	CSQLRecordSetCCNo RecordSet(pDatabase, 
		RSParams_CCNo_NormalByType{ strType },
		TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

bool CSQLRepositoryCCNo::ImportLegacyCCNo(CDatabase* pDatabase)
{
	CSSIniFile iniFile;
	CString strFilename = Filenames.GetLegacyCCNoFilename();

	if (FileExists(strFilename) == FALSE)
	{
		return TRUE;
	}

	if (iniFile.Read(strFilename) == TRUE)
	{
		CSQLRowCCNo RowCC;
		
		RowCC.SetType(CCNO_TYPE_SERVER);
		RowCC.SetNumber(iniFile.GetInt("sccno",0));
		RowCC.IncrementCCNo();
		if (UpsertRow(RowCC, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		RowCC.SetType(CCNO_TYPE_PC);
		RowCC.SetNumber(iniFile.GetInt("pccno", 0));
		RowCC.IncrementCCNo();
		if (UpsertRow(RowCC, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		RowCC.SetType(CCNO_TYPE_POS);
		RowCC.SetNumber(iniFile.GetInt("tccno", 0));
		RowCC.IncrementCCNo();
		if (UpsertRow(RowCC, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		RowCC.SetType(CCNO_TYPE_KIOSK);
		RowCC.SetNumber(iniFile.GetInt("kccno", 0));
		RowCC.IncrementCCNo();
		if (UpsertRow(RowCC, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		RowCC.SetType(CCNO_TYPE_EODBALANCE);
		RowCC.SetNumber(iniFile.GetInt("edcno", 0));
		RowCC.IncrementCCNo();
		if (UpsertRow(RowCC, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		RowCC.SetType(CCNO_TYPE_EXTERNAL);
		RowCC.SetNumber(iniFile.GetInt("eccno", 0));
		RowCC.IncrementCCNo();
		if (UpsertRow(RowCC, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		RowCC.SetType(CCNO_TYPE_VENDING);
		RowCC.SetNumber(iniFile.GetInt("vccno", 0));
		RowCC.IncrementCCNo();
		if (UpsertRow(RowCC, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}
	}

	return TRUE;
}

//**********************************************************************

int CSQLRepositoryCCNo::GetNextCCNoForPC(CDatabase* pDatabase)
{
	int nResult = 0;

	CSQLRowCCNo RowCC;
	RowCC.SetType(CCNO_TYPE_PC);
	
	//ROW MAY NOT EXIST
	SelectRow(RowCC, pDatabase);
	RowCC.IncrementCCNo();
	RowCC.SetNumber(nResult);
	UpsertRow(RowCC, pDatabase);

	return nResult;
}

//**********************************************************************