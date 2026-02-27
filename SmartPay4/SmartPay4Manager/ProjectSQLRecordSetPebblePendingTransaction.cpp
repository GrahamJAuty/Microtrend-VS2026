//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetPebblePendingTransaction.h"
//**********************************************************************

CProjectSQLRecordSetPebblePendingTransaction::CProjectSQLRecordSetPebblePendingTransaction(bool bUnmatched) : CSQLRecordSetPebblePendingTransaction(NULL, RSParams_PebblePendingTransaction_NormalNoParams{}, bUnmatched)
{
}

//**********************************************************************

void CProjectSQLRecordSetPebblePendingTransaction::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(PebblePendingTransaction::TransactionID.Label);
	arrayHeader.Add(PebblePendingTransaction::MemberID.Label);
	arrayHeader.Add(PebblePendingTransaction::DateTime.Label);
	arrayHeader.Add(PebblePendingTransaction::Name.Label);
	arrayHeader.Add(PebblePendingTransaction::Amount.Label);
	arrayHeader.Add(PebblePendingTransaction::IsSale.Label);

	CString strTableName = GetDefaultSQL();

	CSQLOut SQLOut(fileOut, strTableName, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowPebblePendingTransaction RowPB;
	while (StepSelectAll(RowPB) == TRUE)
	{
		SQLOut.Add(RowPB.GetTransactionID());
		SQLOut.Add(RowPB.GetMemberID());
		SQLOut.Add(RowPB.GetDateTime());
		SQLOut.Add(RowPB.GetName());
		SQLOut.Add(RowPB.GetAmount());
		SQLOut.Add(RowPB.GetIsSaleFlag());

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
