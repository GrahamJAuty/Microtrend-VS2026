//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryChartwellExportPayment.h"
//**********************************************************************

CProjectSQLRepositoryChartwellExportPayment::CProjectSQLRepositoryChartwellExportPayment() : CSQLRepositoryChartwellExportPayment()
{
	m_strImportTableName = SQLTableNames::ChartwellExportPayments;
}

//**********************************************************************

bool CProjectSQLRepositoryChartwellExportPayment::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Chartwell Payments Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Chartwell Payments Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(ChartwellExportPayment::UserID.Label);
	arrayHeader.Add(ChartwellExportPayment::TransactionID.Label);
	arrayHeader.Add(ChartwellExportPayment::TerminalNo.Label);
	arrayHeader.Add(ChartwellExportPayment::Date.Label);
	arrayHeader.Add(ChartwellExportPayment::CreateDateTime.Label);
	arrayHeader.Add(ChartwellExportPayment::GroupNo.Label);
	arrayHeader.Add(ChartwellExportPayment::PaymentPurse1.Label);
	arrayHeader.Add(ChartwellExportPayment::PaymentPurse2.Label);
	arrayHeader.Add(ChartwellExportPayment::PaymentPurse3.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowChartwellExportPayment RowPB;
		RowPB.SetUserID(csvIn.GetInt64(0));
		RowPB.SetTransactionID(csvIn.GetInt64(1));
		RowPB.SetTerminalNo(csvIn.GetInt(2));
		RowPB.SetDate(csvIn.GetString(3));
		RowPB.SetCreateDateTime(csvIn.GetString(4));
		RowPB.SetGroupNo(csvIn.GetInt(5));
		RowPB.SetPaymentPurse1(csvIn.GetDouble(6));
		RowPB.SetPaymentPurse2(csvIn.GetDouble(7));
		RowPB.SetPaymentPurse3(csvIn.GetDouble(8));

		if (InsertRow(RowPB, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
