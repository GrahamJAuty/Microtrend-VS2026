//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryBromComTransaction.h"
//**********************************************************************

CProjectSQLRepositoryBromComTransaction::CProjectSQLRepositoryBromComTransaction() : CSQLRepositoryBromComTransaction()
{
	m_strImportTableName = SQLTableNames::BromComTransactions;
}

//**********************************************************************

bool CProjectSQLRepositoryBromComTransaction::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing BromCom Transactions Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring BromCom Transactions Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(BromComTransaction::LineID.Label);
	arrayHeader.Add(BromComTransaction::PersonID.Label);
	arrayHeader.Add(BromComTransaction::Details.Label);
	arrayHeader.Add(BromComTransaction::Amount.Label);
	arrayHeader.Add(BromComTransaction::Balance.Label);
	arrayHeader.Add(BromComTransaction::DateTime.Label);
	arrayHeader.Add(BromComTransaction::Type.Label);
	arrayHeader.Add(BromComTransaction::TransactionID.Label);
	arrayHeader.Add(BromComTransaction::WebComment.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowBromComTransaction RowBC;
		RowBC.SetLineID(csvIn.GetInt64(0));
		RowBC.SetPersonID(csvIn.GetInt(1));
		RowBC.SetDetails(csvIn.GetString(2));
		RowBC.SetAmount(csvIn.GetDouble(3));
		RowBC.SetBalance(csvIn.GetDouble(4));
		RowBC.SetDateTime(csvIn.GetString(5));
		RowBC.SetType(csvIn.GetString(6));
		RowBC.SetTransactionID(csvIn.GetInt(7));
		RowBC.SetWebComment(csvIn.GetString(8));

		if (InsertRow(RowBC, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
