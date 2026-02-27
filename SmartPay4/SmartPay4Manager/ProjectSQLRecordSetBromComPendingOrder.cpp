//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetBromComPendingOrder.h"
//**********************************************************************

CProjectSQLRecordSetBromComPendingOrder::CProjectSQLRecordSetBromComPendingOrder() : CSQLRecordSetBromComPendingOrder(NULL, RSParams_BromComPendingOrder_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetBromComPendingOrder::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(BromComPendingOrder::OrderID.Label);
	arrayHeader.Add(BromComPendingOrder::Status.Label);
	arrayHeader.Add(BromComPendingOrder::LastModified.Label);
	arrayHeader.Add(BromComPendingOrder::NewOrder.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::BromComPendingOrders, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowBromComPendingOrder RowBC;
	while (StepSelectAll(RowBC) == TRUE)
	{
		SQLOut.Add(RowBC.GetOrderID());
		SQLOut.Add(RowBC.GetStatus());
		SQLOut.Add(RowBC.GetLastModified());
		SQLOut.Add(RowBC.GetNewOrderFlag());

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
