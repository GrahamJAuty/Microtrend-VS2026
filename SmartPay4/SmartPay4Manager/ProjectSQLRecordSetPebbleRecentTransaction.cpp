//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetPebbleRecentTransaction.h"
//**********************************************************************

CProjectSQLRecordSetPebbleRecentTransaction::CProjectSQLRecordSetPebbleRecentTransaction() : CSQLRecordSetPebbleRecentTransaction(NULL, RSParams_PebbleRecentTransaction_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetPebbleRecentTransaction::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(PebbleRecentTransaction::TransactionID.Label);
	arrayHeader.Add(PebbleRecentTransaction::DateTime.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::PebbleRecentTransactions, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowPebbleRecentTransaction RowPB;
	while (StepSelectAll(RowPB) == TRUE)
	{
		SQLOut.Add(RowPB.GetTransactionID());
		SQLOut.Add(RowPB.GetDateTime());

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
