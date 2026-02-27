//**********************************************************************
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLIn.h"
//**********************************************************************
#include "ProjectSQLRepositoryExternalAccount.h"
//**********************************************************************

CProjectSQLRepositoryExternalAccount::CProjectSQLRepositoryExternalAccount() : CSQLRepositoryExternalAccount()
{
	m_strImportTableName = SQLTableNames::ExternalAccounts;
}

//**********************************************************************

bool CProjectSQLRepositoryExternalAccount::ImportFromScript(CSSFile& fileIn, CDatabase* pDatabase, CWorkingDlg& WorkingDlg)
{
	WorkingDlg.SetCaption1("Preparing External Accounts Table");
	if (CSQLHelpers::DeleteTableRows(m_strImportTableName, pDatabase) == FALSE)
	{
		return FALSE;
	}
	WorkingDlg.SetCaption1("Restoring External Accounts Table");

	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(ExternalAccount::Username.Label);
	arrayHeader.Add(ExternalAccount::Password.Label);
	arrayHeader.Add(ExternalAccount::UserID.Label);

	CSQLIn SQLIn(fileIn, m_strImportTableName, arrayHeader);

	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		CSQLRowExternalAccount RowAccount;
		RowAccount.SetAESUsername(csvIn.GetString(0));
		RowAccount.SetPasswordHash(csvIn.GetString(1));
		RowAccount.SetAESUserID(csvIn.GetString(2));

		if (InsertRow(RowAccount, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}

		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}

	return TRUE;
}

//**********************************************************************
