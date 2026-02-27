//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetBromComTransaction.h"
//**********************************************************************

CProjectSQLRecordSetBromComTransaction::CProjectSQLRecordSetBromComTransaction() : CSQLRecordSetBromComTransaction(NULL, RSParams_BromComTransaction_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetBromComTransaction::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(BromComTransaction::LineID.Label);
	arrayHeader.Add(BromComTransaction::PersonID.Label);
	arrayHeader.Add(BromComTransaction::Details.Label);
	arrayHeader.Add(BromComTransaction::Amount.Label);
	arrayHeader.Add(BromComTransaction::Balance.Label);
	arrayHeader.Add(BromComTransaction::DateTime.Label);
	arrayHeader.Add(BromComTransaction::Type.Label);
	arrayHeader.Add(BromComTransaction::TransactionID.Label);
	arrayHeader.Add(BromComTransaction::WebComment.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::BromComTransactions, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowBromComTransaction RowBC;
	while (StepSelectAll(RowBC) == TRUE)
	{
		SQLOut.Add(RowBC.GetLineID());
		SQLOut.Add(RowBC.GetPersonID());
		SQLOut.Add(RowBC.GetDetails());
		SQLOut.Add(RowBC.GetAmount(), 2);
		SQLOut.Add(RowBC.GetBalance(), 2);
		SQLOut.Add(RowBC.GetDateTime());
		SQLOut.Add(RowBC.GetType());
		SQLOut.Add(RowBC.GetTransactionID());
		SQLOut.Add(RowBC.GetWebComment());

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
