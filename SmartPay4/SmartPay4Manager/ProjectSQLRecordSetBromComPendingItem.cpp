//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetBromComPendingItem.h"
//**********************************************************************

CProjectSQLRecordSetBromComPendingItem::CProjectSQLRecordSetBromComPendingItem() : CSQLRecordSetBromComPendingItem(NULL, RSParams_BromComPendingItem_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetBromComPendingItem::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(BromComPendingItem::OrderID.Label);
	arrayHeader.Add(BromComPendingItem::PersonID.Label);
	arrayHeader.Add(BromComPendingItem::Amount.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::BromComPendingItems, arrayHeader);

	m_strFilter = "";
	m_strSort = "";

	SQLOut.WriteDelete();

	CSQLRowBromComPendingItem RowBC;
	while (StepSelectAll(RowBC) == TRUE)
	{
		SQLOut.Add(RowBC.GetOrderID());
		SQLOut.Add(RowBC.GetPersonID());
		SQLOut.Add(RowBC.GetAmount(), 2);

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
