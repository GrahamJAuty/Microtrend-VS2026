//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "ProjectSQLRecordSetExternalAccount.h"
//**********************************************************************

CProjectSQLRecordSetExternalAccount::CProjectSQLRecordSetExternalAccount() : CSQLRecordSetExternalAccount(NULL, RSParams_ExternalAccount_NormalNoParams	{})
{
}

//**********************************************************************

void CProjectSQLRecordSetExternalAccount::ExportToScript(CSSFile& fileOut, CWorkingDlg& WorkingDlg)
{
	CStringArray arrayHeader;
	arrayHeader.Add(ExternalAccount::Username.Label);
	arrayHeader.Add(ExternalAccount::Password.Label);
	arrayHeader.Add(ExternalAccount::UserID.Label);

	CSQLOut SQLOut(fileOut, SQLTableNames::ExternalAccounts, arrayHeader);

	SQLOut.WriteDelete();

	CSQLRowExternalAccount RowAccount;
	while (StepSelectAll(RowAccount) == TRUE)
	{
		SQLOut.Add(RowAccount.GetAESUsername());
		SQLOut.Add(RowAccount.GetPasswordHash());
		SQLOut.Add(RowAccount.GetAESUserID());

		SQLOut.WriteInsert();
		WorkingDlg.IncrementRecordsChecked(10, FALSE);
	}
}

//**********************************************************************
