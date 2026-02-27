//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryPebblePaymentType.h"
//**********************************************************************

CProjectSQLRepositoryPebblePaymentType::CProjectSQLRepositoryPebblePaymentType() : CSQLRepositoryPebblePaymentType()
{
	m_strImportTableName = SQLTableNames::PebblePaymentTypes;
}

//**********************************************************************

bool CProjectSQLRepositoryPebblePaymentType::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing Pebble Payment Type Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring Pebble Payment Type Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(PebblePaymentType::SPOSPaymentType.Label);
	arrayHeader.Add(PebblePaymentType::PebblePaymentName.Label);
	arrayHeader.Add(PebblePaymentType::PebbleTranType.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowPebblePaymentType RowPB;
		RowPB.SetSPOSPaymentType(csvIn.GetInt(0));
		RowPB.SetPebblePaymentName(csvIn.GetString(1));
		RowPB.SetPebbleTranType(csvIn.GetInt(2));

		if (InsertRow(RowPB, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
