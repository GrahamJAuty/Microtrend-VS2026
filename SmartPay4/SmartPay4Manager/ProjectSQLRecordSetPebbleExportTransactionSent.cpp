//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetPebbleExportTransactionSent.h"
//**********************************************************************

CProjectSQLRecordSetPebbleExportTransactionSent::CProjectSQLRecordSetPebbleExportTransactionSent() : CSQLRecordSetPebbleExportTransactionSent(NULL, RSParams_PebbleTransactionSent_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetPebbleExportTransactionSent::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(PebbleExportTransactionSent::LineID.Label);
	arrayHeader.Add(PebbleExportTransactionSent::TranLineID.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::PebbleExportTransactionsSent, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowPebbleExportTransactionSent RowPB;
	while (StepSelectAll(RowPB) == TRUE)
	{
		SQLOut.Add(RowPB.GetLineID());
		SQLOut.Add(RowPB.GetTranLineID());

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
