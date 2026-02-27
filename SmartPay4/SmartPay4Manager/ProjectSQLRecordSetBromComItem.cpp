//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetBromComItem.h"
//**********************************************************************

CProjectSQLRecordSetBromComItem::CProjectSQLRecordSetBromComItem() : CSQLRecordSetBromComItem(NULL, RSParams_BromComItem_NormalNoParams{})
{
}

//**********************************************************************

void CProjectSQLRecordSetBromComItem::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(BromComItem::OrderID.Label);
	arrayHeader.Add(BromComItem::PersonID.Label);
	arrayHeader.Add(BromComItem::Amount.Label);
	arrayHeader.Add(BromComItem::Status.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::BromComItems, arrayHeader);

	m_strFilter = "";
	m_strSort = "";

	SQLOut.WriteDelete();

	CSQLRowBromComItem RowBC;
	while (StepSelectAll(RowBC) == TRUE)
	{
		SQLOut.Add(RowBC.GetOrderID());
		SQLOut.Add(RowBC.GetPersonID());
		SQLOut.Add(RowBC.GetAmount(), 2);
		SQLOut.Add(RowBC.GetStatus());

		SQLOut.WriteInsert();
		fileOut.WriteLine("");

		WorkingDlg.IncrementRecordsChecked(10, TRUE);
	}
}

//**********************************************************************
