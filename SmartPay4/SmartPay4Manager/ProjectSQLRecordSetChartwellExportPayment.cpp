//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetChartwellExportPayment.h"
//**********************************************************************

CProjectSQLRecordSetChartwellExportPayment::CProjectSQLRecordSetChartwellExportPayment() : CSQLRecordSetChartwellExportPayment(NULL, RSParams_ChartwellExportPayment_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetChartwellExportPayment::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)				
{
	CStringArray arrayHeader;
	arrayHeader.Add(ChartwellExportPayment::UserID.Label);
	arrayHeader.Add(ChartwellExportPayment::TransactionID.Label);
	arrayHeader.Add(ChartwellExportPayment::TerminalNo.Label);
	arrayHeader.Add(ChartwellExportPayment::Date.Label);
	arrayHeader.Add(ChartwellExportPayment::CreateDateTime.Label);
	arrayHeader.Add(ChartwellExportPayment::GroupNo.Label);
	arrayHeader.Add(ChartwellExportPayment::PaymentPurse1.Label);
	arrayHeader.Add(ChartwellExportPayment::PaymentPurse2.Label);
	arrayHeader.Add(ChartwellExportPayment::PaymentPurse3.Label);

	CString strTableName = SQLTableNames::ChartwellExportPayments;

	CSQLOut SQLOut(fileOut, strTableName, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowChartwellExportPayment RowPB;
	while (StepSelectAll(RowPB) == TRUE)
	{
		SQLOut.Add(RowPB.GetUserID());
		SQLOut.Add(RowPB.GetTransactionID());
		SQLOut.Add(RowPB.GetTerminalNo());
		SQLOut.Add(RowPB.GetDate());
		SQLOut.Add(RowPB.GetCreateDateTime());
		SQLOut.Add(RowPB.GetGroupNo());
		SQLOut.Add(RowPB.GetPaymentPurse1(), 2);
		SQLOut.Add(RowPB.GetPaymentPurse2(), 2);
		SQLOut.Add(RowPB.GetPaymentPurse3(), 2);

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
