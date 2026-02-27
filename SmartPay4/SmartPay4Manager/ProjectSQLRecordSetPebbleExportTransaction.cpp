//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetPebbleExportTransaction.h"
//**********************************************************************

CProjectSQLRecordSetPebbleExportTransaction::CProjectSQLRecordSetPebbleExportTransaction() : CSQLRecordSetPebbleExportTransaction(NULL, RSParams_PebbleExportTransaction_NormalNoParams	{})
{
}

//**********************************************************************

void CProjectSQLRecordSetPebbleExportTransaction::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(PebbleExportTransaction::LineID.Label);
	arrayHeader.Add(PebbleExportTransaction::UserID.Label);
	arrayHeader.Add(PebbleExportTransaction::XFerUserID.Label);
	arrayHeader.Add(PebbleExportTransaction::MemberID.Label);
	arrayHeader.Add(PebbleExportTransaction::Date.Label);
	arrayHeader.Add(PebbleExportTransaction::Time.Label);
	arrayHeader.Add(PebbleExportTransaction::CCNo.Label);
	arrayHeader.Add(PebbleExportTransaction::SourceType.Label);
	arrayHeader.Add(PebbleExportTransaction::AppNo.Label);
	arrayHeader.Add(PebbleExportTransaction::Amount.Label);
	arrayHeader.Add(PebbleExportTransaction::Purse1.Label);
	arrayHeader.Add(PebbleExportTransaction::Purse2.Label);
	arrayHeader.Add(PebbleExportTransaction::Cash.Label);
	arrayHeader.Add(PebbleExportTransaction::WebComment.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::PebbleExportTransactions, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowPebbleExportTransaction RowPB;
	while (StepSelectAll(RowPB) == TRUE)
	{
		SQLOut.Add(RowPB.GetLineID());
		SQLOut.Add(RowPB.GetUserID());
		SQLOut.Add(RowPB.GetXFerUserID());
		SQLOut.Add(RowPB.GetMemberID());
		SQLOut.Add(RowPB.GetDate());
		SQLOut.Add(RowPB.GetTime());
		SQLOut.Add(RowPB.GetCCNo());
		SQLOut.Add(RowPB.GetSourceType());
		SQLOut.Add(RowPB.GetAppNo());
		SQLOut.Add(RowPB.GetAmount(), 2);
		SQLOut.Add(RowPB.GetPurse1(), 2);
		SQLOut.Add(RowPB.GetPurse2(), 2);
		SQLOut.Add(RowPB.GetCash(), 2);
		SQLOut.Add(RowPB.GetWebComment());

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, true);
	}
}

//**********************************************************************
