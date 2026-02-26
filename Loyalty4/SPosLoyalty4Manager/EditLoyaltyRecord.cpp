/******************************************************/
#include "..\SPosLoyalty4Shared\AESHelpers.h"
#include "..\SPosLoyalty4Shared\SQLConnectionPool.h"
#include "..\SPosLoyalty4Shared\SQLDefines.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_ExternalAccount\SQLRepositoryExternalAccount.h"
/******************************************************/
#include "LoyaltyManager.h"
#include "EditAccountPropertySheet.h"
#include "PasswordData.h"
#include "PhotoIDFilenameStack.h"
#include "SQLTranSaveAccount.h"
/******************************************************/
#include "EditLoyaltyRecord.h"
/******************************************************/

CEditLoyaltyRecord::CEditLoyaltyRecord(void)
{
	m_strNewDisplayLine = "";
	m_nNewColour = 0;
}

/******************************************************/

bool CEditLoyaltyRecord::EditRecord(int nAction, __int64 nUserID, CWnd* pParent)
{
	CString strTitle = "Edit Account Record";

	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserID(nUserID);

	{
		CSQLRepositoryAccount repoAccount;
		CSQLResultInfo Result = repoAccount.SelectRow(RowAccount, NULL);

		CString strMsg = "";

		switch (Result.GetSQLError())
		{
		case SQLCRUD_ERR_NONE:
			break;

		case SQLCRUD_ERR_EXCEPT_DB:
		case SQLCRUD_ERR_EXCEPT_MISC:
			Prompter.DatabaseAccessError();
			return FALSE;

		case SQLCRUD_ERR_NOMATCH:
			strMsg.Format("Cannot find record for account ' %s ' in database!",
				(const char*)SimpleFormatInt64Value(nUserID));		// shouldn't happen
			Prompter.Error(strMsg, strTitle);
			return FALSE;

		case SQLCRUD_ERR_DUPLICATE:
			strMsg.Format("Database error, multiple rows for account ' %s '!",
				(const char*)SimpleFormatInt64Value(nUserID));		// shouldn't happen
			Prompter.Error(strMsg, strTitle);
			return FALSE;

		default:
			strMsg.Format("Database error, unable to select account ' %s '!",
				(const char*)SimpleFormatInt64Value(nUserID));		// shouldn't happen
			Prompter.Error(strMsg, strTitle);
			return FALSE;
		}
	}

	CSQLRowExternalAccount RowExternalOld;

	CString strAESUserID = "";
	CAESHelpers::PlainTextToAESHex(RowAccount.GetUserIDString(), strAESUserID);
	RowExternalOld.SetAESUserID(strAESUserID);

	{
		CSQLRepositoryExternalAccount repoExt;
		switch (repoExt.SelectRowByUserID(RowExternalOld, NULL).GetSQLError())
		{
		case SQLCRUD_ERR_EXCEPT_DB:
		case SQLCRUD_ERR_EXCEPT_MISC:
			Prompter.DatabaseAccessError();
			return FALSE;
		}
	}

	CSQLRowExternalAccount RowExternalNew = RowExternalOld;

	CPhotoIDFilenameStack PhotoIDFilenameStack;
	CEditAccountPropertySheet dlgEdit(RowAccount, RowExternalNew, PhotoIDFilenameStack, NULL, System.GetDefaultAccountEditPage());
	bool bDoneUpdate = FALSE;

	if (dlgEdit.DoModal() == IDOK)
	{
		CSQLRowAccountFull RowCheck;
		RowCheck.SetUserID(RowAccount.GetUserID());
		bool bCanUpdate = FALSE;

		CSQLRepositoryAccount repoAccount;
		switch (repoAccount.SelectRow(RowCheck, NULL).GetSQLError())
		{
		case SQLCRUD_ERR_NONE:
			bCanUpdate = TRUE;
			break;

		case SQLCRUD_ERR_EXCEPT_DB:
		case SQLCRUD_ERR_EXCEPT_MISC:
			Prompter.DatabaseAccessError();
			bCanUpdate = FALSE;
			break;

		default:
			Prompter.DatabaseUpdateError("Unable to check for changes by another user.");
			bCanUpdate = FALSE;
			break;
		}

		if (TRUE == bCanUpdate)
		{
			bCanUpdate = RowCheck.CompareRowVersion(RowAccount);

			if (FALSE == bCanUpdate)
			{
				Prompter.DatabaseUpdateError("Account has already been modified by another user.");
			}
		}

		if (TRUE == bCanUpdate)
		{
			CSQLTranSaveAccount Tran;
			Tran.BeginTrans();
			Tran.DoWork(RowAccount, RowExternalOld, RowExternalNew);

			switch (Tran.EndTrans())
			{
			case SQLTRAN_STATE_COMMITOK:
				bDoneUpdate = TRUE;
				break;

			default:
				Tran.ShowErrorMessage("Edit Account");
				break;
			}
		}

		if (TRUE == bDoneUpdate)
		{
			if (PhotoIDFilenameStack.GetChangeFlag() == TRUE)
			{
				PhotoIDFilenameStack.PromoteFile(2);
			}

			dlgEdit.UpdateAudit();
		}
	}

	PhotoIDFilenameStack.RemoveOldFiles();
	PhotoIDFilenameStack.RemoveFile(2);

	return bDoneUpdate;
}

/******************************************************/
