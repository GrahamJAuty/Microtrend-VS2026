//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryBromComPendingOrder.h"
//**********************************************************************

CProjectSQLRepositoryBromComPendingOrder::CProjectSQLRepositoryBromComPendingOrder() : CSQLRepositoryBromComPendingOrder()
{
	m_strImportTableName = SQLTableNames::BromComPendingOrders;
}

//**********************************************************************

bool CProjectSQLRepositoryBromComPendingOrder::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing BromCom Pending Orders Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring BromCom Pending Orders Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(BromComPendingOrder::OrderID.Label);
	arrayHeader.Add(BromComPendingOrder::Status.Label);
	arrayHeader.Add(BromComPendingOrder::LastModified.Label);
	arrayHeader.Add(BromComPendingOrder::NewOrder.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowBromComPendingOrder RowBC;
		RowBC.SetOrderID(csvIn.GetInt(0));
		RowBC.SetStatus(csvIn.GetString(1));
		RowBC.SetLastModified(csvIn.GetString(2));
		RowBC.SetNewOrderFlag(csvIn.GetBool(3));

		if (InsertRow(RowBC, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
