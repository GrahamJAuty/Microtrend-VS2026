//**********************************************************************
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryPebbleExportTransaction.h"
//**********************************************************************

CProjectSQLRepositoryPebbleExportTransaction::CProjectSQLRepositoryPebbleExportTransaction() : CSQLRepositoryPebbleExportTransaction()
{
	m_strImportTableName = SQLTableNames::PebbleExportTransactions;
}

//**********************************************************************

bool CProjectSQLRepositoryPebbleExportTransaction::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CArray<CSQLRowPebbleExportTransaction, CSQLRowPebbleExportTransaction&>& arrayPebbleExportTransaction, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Pebble Export Transactions Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Pebble Export Transactions Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(PebbleExportTransaction::LineID.Label);
	arrayHeader.Add(PebbleExportTransaction::UserID.Label);
	arrayHeader.Add(PebbleExportTransaction::XFerUserID.Label);
	arrayHeader.Add(PebbleExportTransaction::MemberID.Label);
	arrayHeader.Add(PebbleExportTransaction::Date.Label);
	arrayHeader.Add(PebbleExportTransaction::Time.Label);
	arrayHeader.Add(PebbleExportTransaction::CCNo.Label);
	arrayHeader.Add(PebbleExportTransaction::SourceType.Label);
	arrayHeader.Add(PebbleExportTransaction::AppNo.Label);
	arrayHeader.Add(PebbleExportTransaction::Amount.Label);
	arrayHeader.Add(PebbleExportTransaction::Purse1.Label);
	arrayHeader.Add(PebbleExportTransaction::Purse2.Label);
	arrayHeader.Add(PebbleExportTransaction::Cash.Label);
	arrayHeader.Add(PebbleExportTransaction::WebComment.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowPebbleExportTransaction RowPB;
		RowPB.SetLineID(csvIn.GetInt64(0));
		RowPB.SetUserID(csvIn.GetInt64(1));
		RowPB.SetXFerUserID(csvIn.GetInt64(2));
		RowPB.SetMemberID(csvIn.GetString(3));
		RowPB.SetDate(csvIn.GetString(4));
		RowPB.SetTime(csvIn.GetString(5));
		RowPB.SetCCNo(csvIn.GetInt(6));
		RowPB.SetSourceType(csvIn.GetInt(7));
		RowPB.SetAppNo(csvIn.GetInt(8));
		RowPB.SetAmount(csvIn.GetDouble(9));
		RowPB.SetPurse1(csvIn.GetDouble(10));
		RowPB.SetPurse2(csvIn.GetDouble(11));
		RowPB.SetCash(csvIn.GetDouble(12));
		RowPB.SetWebComment(csvIn.GetString(13));
		arrayPebbleExportTransaction.Add(RowPB);		

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************

bool CProjectSQLRepositoryPebbleExportTransaction::ProcessImportedTransactions(CDatabase* pDatabase, CArray<CSQLRowPebbleExportTransaction, CSQLRowPebbleExportTransaction&>& arrayPebbleExportTransaction, CArray<CSQLRowPebbleExportTransactionSent, CSQLRowPebbleExportTransactionSent&>& arrayPebbleExportTransactionSent)
{
	CReportConsolidationArray<CSortedInt64Item> arraySent;
	for (int n = 0; n < arrayPebbleExportTransactionSent.GetSize(); n++)
	{
		CSortedInt64Item item;
		item.m_nItem = arrayPebbleExportTransactionSent[n].GetLineID();
		arraySent.Consolidate(item);
	}

	for (int n = 0; n < arrayPebbleExportTransaction.GetSize(); n++)
	{
		CSortedInt64Item item;
		item.m_nItem = arrayPebbleExportTransaction[n].GetLineID();

		int nPos = 0;
		if (arraySent.Find(item, nPos) == FALSE)
		{
			if (InsertRow(arrayPebbleExportTransaction[n], pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

//**********************************************************************
