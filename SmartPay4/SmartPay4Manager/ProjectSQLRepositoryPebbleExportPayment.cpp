//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryPebbleExportPayment.h"
//**********************************************************************

CProjectSQLRepositoryPebbleExportPayment::CProjectSQLRepositoryPebbleExportPayment() : CSQLRepositoryPebbleExportPayment()
{
	m_strImportTableName = SQLTableNames::PebbleExportPayments;
}

//**********************************************************************

bool CProjectSQLRepositoryPebbleExportPayment::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Pebble Export Payments Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Pebble Export Payments Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(PebbleExportPayment::UserID.Label);
	arrayHeader.Add(PebbleExportPayment::Date.Label);
	arrayHeader.Add(PebbleExportPayment::Time.Label);
	arrayHeader.Add(PebbleExportPayment::CreateDateTime.Label);
	arrayHeader.Add(PebbleExportPayment::CCNo.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentType1.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentAmount1.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentType2.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentAmount2.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentType3.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentAmount3.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentType4.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentAmount4.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentType5.Label);
	arrayHeader.Add(PebbleExportPayment::PaymentAmount5.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowPebbleExportPayment RowPB;
		RowPB.SetUserID(csvIn.GetInt64(0));
		RowPB.SetDate(csvIn.GetString(1));
		RowPB.SetTime(csvIn.GetString(2));
		RowPB.SetCreateDateTime(csvIn.GetString(3));
		RowPB.SetCCNo(csvIn.GetInt(4));

		int nPos = 5;
		for (int n = 0; n < MAX_PEBBLE_EXPORT_PAYMENT; n++)
		{
			RowPB.SetPaymentType(n, csvIn.GetInt(nPos++));
			RowPB.SetPaymentAmount(n, csvIn.GetDouble(nPos++));
		}

		if (InsertRow(RowPB, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
