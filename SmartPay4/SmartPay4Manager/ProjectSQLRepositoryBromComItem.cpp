//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryBromComItem.h"
//**********************************************************************

CProjectSQLRepositoryBromComItem::CProjectSQLRepositoryBromComItem() : CSQLRepositoryBromComItem()
{
	m_strImportTableName = SQLTableNames::BromComItems;
}

//**********************************************************************

bool CProjectSQLRepositoryBromComItem::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing BromCom Items Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring BromCom Items Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(BromComItem::OrderID.Label);
	arrayHeader.Add(BromComItem::PersonID.Label);
	arrayHeader.Add(BromComItem::Amount.Label);
	arrayHeader.Add(BromComItem::Status.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowBromComItem RowBC;
		RowBC.SetOrderID(csvIn.GetInt(0));
		RowBC.SetPersonID(csvIn.GetInt(1));
		RowBC.SetAmount(csvIn.GetDouble(2));
		RowBC.SetStatus(csvIn.GetString(3));

		if (InsertRow(RowBC, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
