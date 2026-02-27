//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryPebbleExportTransactionSent.h"
//**********************************************************************

CProjectSQLRepositoryPebbleExportTransactionSent::CProjectSQLRepositoryPebbleExportTransactionSent() : CSQLRepositoryPebbleExportTransactionSent()
{
	m_strImportTableName = SQLTableNames::PebbleExportTransactionsSent;
}

//**********************************************************************

bool CProjectSQLRepositoryPebbleExportTransactionSent::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CArray<CSQLRowPebbleExportTransactionSent, CSQLRowPebbleExportTransactionSent&>& arrayPebbleExportTransactionSent, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Pebble Sent Transactions Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Pebble Sent Transactions Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(PebbleExportTransactionSent::LineID.Label);
	arrayHeader.Add(PebbleExportTransactionSent::TranLineID.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowPebbleExportTransactionSent RowPB;
		RowPB.SetLineID(csvIn.GetInt64(0));
		RowPB.SetTranLineID(csvIn.GetInt64(1));
		arrayPebbleExportTransactionSent.Add(RowPB);

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
