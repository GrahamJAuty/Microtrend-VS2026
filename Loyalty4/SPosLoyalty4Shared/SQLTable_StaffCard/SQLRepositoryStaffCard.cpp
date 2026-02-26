//**********************************************************************
#include "..\SQLConnectionPool.h"
#include "..\SQLDefines.h"
#include "..\SQLNamespaces.h"
#include "..\SQLDb.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRepositoryStaffCard.h"
//**********************************************************************

CSQLRepositoryStaffCard::CSQLRepositoryStaffCard()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryStaffCard::SelectRow(CSQLRowStaffCard& SQLRowSC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetStaffCard RecordSet(pDatabase, 
			RSParams_StaffCard_NormalByDbNoSetNoServerNo{
				SQLRowSC.GetFolderIndexDbNo(),
				SQLRowSC.GetFolderIndexSetNo(),
				SQLRowSC.GetServerNo()
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
				RecordSet.SaveToStaffCardRow(SQLRowSC);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryStaffCard::UpdateRow(CSQLRowStaffCard& SQLRowSC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetStaffCard RecordSet(pDatabase,
			RSParams_StaffCard_NormalByDbNoSetNoServerNo{
				SQLRowSC.GetFolderIndexDbNo(),
				SQLRowSC.GetFolderIndexSetNo(),
				SQLRowSC.GetServerNo()
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
				RecordSet.LoadFromStaffCardRow(SQLRowSC);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryStaffCard::InsertRow(CSQLRowStaffCard& SQLRowSC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetStaffCard RecordSet(pDatabase, RSParams_StaffCard_NormalNoParams{});
		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromStaffCardRow(SQLRowSC);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryStaffCard::DeleteRow(CSQLRowStaffCard& SQLRowSC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetStaffCard RecordSet(pDatabase,
			RSParams_StaffCard_NormalByDbNoSetNoServerNo{
				SQLRowSC.GetFolderIndexDbNo(),
				SQLRowSC.GetFolderIndexSetNo(),
				SQLRowSC.GetServerNo()
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
