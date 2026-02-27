//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetPebbleExportPayment.h"
//**********************************************************************

CProjectSQLRecordSetPebbleExportPayment::CProjectSQLRecordSetPebbleExportPayment() : CSQLRecordSetPebbleExportPayment(NULL, RSParams_PebbleExportPayment_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetPebbleExportPayment::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)			
{
	CStringArray arrayHeader;
	arrayHeader.Add(PebbleExportPayment::UserID.Label);
	arrayHeader.Add(PebbleExportPayment::Date.Label);
	arrayHeader.Add(PebbleExportPayment::Time.Label);
	arrayHeader.Add(PebbleExportPayment::CreateDateTime.Label);
	arrayHeader.Add(PebbleExportPayment::CCNo.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentType1.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentAmount1.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentType2.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentAmount2.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentType3.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentAmount3.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentType4.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentAmount4.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentType5.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentAmount5.Label);

	CString strTableName = SQLTableNames::PebbleExportPayments;

	CSQLOut SQLOut(fileOut, strTableName, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowPebbleExportPayment RowPB;
	while (StepSelectAll(RowPB) == TRUE)
	{
		SQLOut.Add(RowPB.GetUserID());
		SQLOut.Add(RowPB.GetDate());
		SQLOut.Add(RowPB.GetTime());
		SQLOut.Add(RowPB.GetCreateDateTime());
		SQLOut.Add(RowPB.GetCCNo());

		for (int n = 0; n < MAX_PEBBLE_EXPORT_PAYMENT; n++)
		{
			SQLOut.Add(RowPB.GetPaymentType(n));
			SQLOut.Add(RowPB.GetPaymentAmount(n), 2);
		}

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
