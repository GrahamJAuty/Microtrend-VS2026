//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryBromComPendingItem.h"
//**********************************************************************

CProjectSQLRepositoryBromComPendingItem::CProjectSQLRepositoryBromComPendingItem() : CSQLRepositoryBromComPendingItem()
{
	m_strImportTableName = SQLTableNames::BromComPendingItems;
}

//**********************************************************************

bool CProjectSQLRepositoryBromComPendingItem::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing BromCom Pending Items Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring BromCom Pending Items Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(BromComPendingItem::OrderID.Label);
	arrayHeader.Add(BromComPendingItem::PersonID.Label);
	arrayHeader.Add(BromComPendingItem::Amount.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowBromComPendingItem RowBC;
		RowBC.SetOrderID(csvIn.GetInt(0));
		RowBC.SetPersonID(csvIn.GetInt(1));
		RowBC.SetAmount(csvIn.GetDouble(2));

		if (InsertRow(RowBC, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
