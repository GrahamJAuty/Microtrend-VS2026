//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
//**********************************************************************
#include "SQLRecordSetChartwellExportPayment.h"
//**********************************************************************
#include "SQLPrepStatChartwellExportPayment.h"
//**********************************************************************

CSQLPrepStatChartwellExportPayment::CSQLPrepStatChartwellExportPayment()
{
}

//**********************************************************************

CSQLResultInfo CSQLPrepStatChartwellExportPayment::SelectRow(CSQLRowChartwellExportPayment& SQLRowPB, CDatabase* pDatabase)
{
	CSQLResultInfo Result;

	try
	{
		CString strUserID = "";
		strUserID.Format("%I64d", SQLRowPB.GetUserID());

		CString strTransactionID = "";
		strTransactionID.Format("%I64d", SQLRowPB.GetTransactionID());

		CSQLRecordSetChartwellExportPayment RecordSet(pDatabase, 
			RSParams_ChartwellExportPayment_NormalByTranDetail{
				strUserID,
				strTransactionID,
				SQLRowPB.GetTerminalNo(),
				SQLRowPB.GetDate()
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
				RecordSet.SaveToChartwellExportPaymentRow(SQLRowPB);

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

