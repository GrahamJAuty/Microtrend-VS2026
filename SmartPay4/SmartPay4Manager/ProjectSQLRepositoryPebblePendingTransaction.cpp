//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryPebblePendingTransaction.h"
//**********************************************************************

CProjectSQLRepositoryPebblePendingTransaction::CProjectSQLRepositoryPebblePendingTransaction(bool bUnmatched) : CSQLRepositoryPebblePendingTransaction(bUnmatched)
{
	if ( FALSE == bUnmatched )
	{
		m_strImportTableName = SQLTableNames::PebblePendingTransactions;
	}
	else
	{
		m_strImportTableName = SQLTableNames::PebbleUnmatchedTransactions;
	}
}

//**********************************************************************

bool CProjectSQLRepositoryPebblePendingTransaction::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	if ( m_strImportTableName == SQLTableNames::PebbleUnmatchedTransactions )
	{
		WorkingDlg.SetCaption1("Preparing Pebble Unmatched Transactions Table");
	}
	else
	{
		WorkingDlg.SetCaption1("Preparing Pebble Pending Transactions Table");
	}

	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}

	if (m_strImportTableName == SQLTableNames::PebbleUnmatchedTransactions)
	{
		WorkingDlg.SetCaption1("Restoring Pebble Unmatched Transactions Table");

	}
	else
	{
		WorkingDlg.SetCaption1("Restoring Pebble Pending Transactions Table");
	}
	
	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(PebblePendingTransaction::TransactionID.Label);
	arrayHeader.Add(PebblePendingTransaction::MemberID.Label);
	arrayHeader.Add(PebblePendingTransaction::DateTime.Label);
	arrayHeader.Add(PebblePendingTransaction::Name.Label);
	arrayHeader.Add(PebblePendingTransaction::Amount.Label);
	arrayHeader.Add(PebblePendingTransaction::IsSale.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowPebblePendingTransaction RowPB;
		RowPB.SetTransactionID(csvIn.GetString(0));
		RowPB.SetMemberID(csvIn.GetString(1));
		RowPB.SetDateTime(csvIn.GetString(2));
		RowPB.SetName(csvIn.GetString(3));
		RowPB.SetAmount(csvIn.GetInt(4));
		RowPB.SetIsSaleFlag(csvIn.GetBool(5));

		if (InsertRow(RowPB, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;	
}

//**********************************************************************
