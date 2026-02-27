//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryPebbleRecentTransaction.h"
//**********************************************************************

CProjectSQLRepositoryPebbleRecentTransaction::CProjectSQLRepositoryPebbleRecentTransaction() : CSQLRepositoryPebbleRecentTransaction()
{
	m_strImportTableName = SQLTableNames::PebbleRecentTransactions;
}

//**********************************************************************

bool CProjectSQLRepositoryPebbleRecentTransaction::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Pebble Recent Transactions Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Pebble Recent Transactions Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(PebbleRecentTransaction::TransactionID.Label);
	arrayHeader.Add(PebbleRecentTransaction::DateTime.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowPebbleRecentTransaction RowPB;
		RowPB.SetTransactionID(csvIn.GetString(0));
		RowPB.SetDateTime(csvIn.GetString(1));

		if (InsertRow(RowPB, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
