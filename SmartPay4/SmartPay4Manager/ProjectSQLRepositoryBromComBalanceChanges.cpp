//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryBromComBalanceChanges.h"
//**********************************************************************

CProjectSQLRepositoryBromComBalanceChanges::CProjectSQLRepositoryBromComBalanceChanges() : CSQLRepositoryBromComBalanceChanges()
{
	m_strImportTableName = SQLTableNames::BromComBalanceChanges;
}

//**********************************************************************

bool CProjectSQLRepositoryBromComBalanceChanges::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing BromCom Balance Changes Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring BromCom Balance Changes Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(BromComBalanceChanges::PersonID.Label);
	arrayHeader.Add(BromComBalanceChanges::AccountID.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowBromComBalanceChanges RowBC;
		RowBC.SetPersonID(csvIn.GetInt(0));
		RowBC.SetAccountID(csvIn.GetInt64(1));

		if (InsertRow(RowBC, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
