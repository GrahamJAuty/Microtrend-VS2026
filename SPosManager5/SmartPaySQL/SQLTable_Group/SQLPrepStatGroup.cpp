//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLDb.h"
//**********************************************************************
#include "SQLPrepStatGroup.h"
//**********************************************************************

CSQLPrepStatGroup::CSQLPrepStatGroup()
{
}

//**********************************************************************

CSQLResultInfo CSQLPrepStatGroup::SelectRow(CSQLRowGroupFull& SQLRowGroup, CDatabase* pDatabase)
{
	CSQLResultInfo Result;

	try
	{
		CSQLRecordSetGroupFull RecordSet(pDatabase,
			RSParams_Group_NormalByGroupNo{ SQLRowGroup.GetGroupNo() });
			
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
				RecordSet.SaveToGroupRow(SQLRowGroup);

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

