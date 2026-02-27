//********************************************************************
#include "..\SmartPay4Shared\AESHelpers.h"
#include "..\SmartPay4Shared\GlobalState.h"
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\Prompter.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_ExternalAccount\SQLRepositoryExternalAccount.h"
//********************************************************************
#include "..\SmartPay4ManagerBgnd\LeaversHandler.h"
//********************************************************************
#include "EditRecordPropertySheet.h"
#include "SmartPay4Manager.h"
#include "SQLTranSaveAccount.h"
//********************************************************************
#include "EditRecord.h"
//********************************************************************

CEditRecord::CEditRecord ( const char* szLeaversYear, CWnd* pParent )
{
	m_strLeaversYear = szLeaversYear;
	m_pParent = pParent;
}

//********************************************************************

bool CEditRecord::Edit(__int64 nUserID)
{
	bool bResult = FALSE;

	if (m_strLeaversYear != "")
	{
		bResult = EditLeaverRecord(nUserID);
	}
	else
	{
		bResult = EditNormalRecord(nUserID);
	}

	return bResult;
}

//********************************************************************

void CEditRecord::SelectRowForEdit(CSQLRowAccountFull& RowAccount, CString& strMsg)
{
	strMsg = "";

	CSQLRepositoryAccount RepoAccount;
	switch (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError())
	{
	case SQLCRUD_ERR_NOMATCH:
		strMsg.Format("Cannot find record for account ' %s ' in database!", 
			(const char*) FormatInt64Value(RowAccount.GetUserID()));		// shouldn't happen
		break;

	case SQLCRUD_ERR_DUPLICATE:
		strMsg.Format("Database error, multiple rows for account ' %s '!",
			(const char*) FormatInt64Value(RowAccount.GetUserID()));		// shouldn't happen
		break;

	case SQLCRUD_ERR_NONE:
		break;

	default:
		strMsg.Format("Database error, unable to select account ' %s '!", 
			(const char*) FormatInt64Value(RowAccount.GetUserID()));		// shouldn't happen
		break;
	}
}

//********************************************************************

bool CEditRecord::EditNormalRecord(__int64 nUserID)
{
	// edit current active database 
	CString strTitle = "Edit Account Record";

	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserID(nUserID);

	{
		CString strMsg = "";
		SelectRowForEdit(RowAccount, strMsg);
		
		if (strMsg != "")
		{
			Prompter.Error(strMsg, strTitle);
			return FALSE;
		}
	}

	int nMode = DB_READONLY;
	if (Passwords.IsEditRecordAllowed())
	{
		nMode = DB_READWRITE;
		LogEdit(nUserID, "Start");
	}

	CSQLRowExternalAccount RowExternalOld;

	CString strAESUserID = "";
	CAESHelpers::PlainTextToAESHex(RowAccount.GetUserIDString(), strAESUserID);
	RowExternalOld.SetAESUserID(strAESUserID);

	{
		CSQLRepositoryExternalAccount RepoExt;
		switch (RepoExt.SelectRowByUserID(RowExternalOld, NULL).GetSQLError())
		{
		case SQLCRUD_ERR_EXCEPT_DB:
		case SQLCRUD_ERR_EXCEPT_MISC:
			Prompter.DatabaseAccessError();
			return FALSE;
		}
	}

	CSQLRowExternalAccount RowExternalNew = RowExternalOld;
	
	CEditRecordPropertySheet dlgEdit(RowAccount, RowExternalNew, nMode, NULL, System.GetDefaultAccountEditPage());

	bool bCancel = ( dlgEdit.DoModal() != IDOK );

	bool bSaveReqd = (DB_READWRITE == nMode);

	if (TRUE == bSaveReqd)
	{
		CSQLRowAccountFull RowCheck;
		RowCheck.SetUserID(RowAccount.GetUserID());
	
		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.SelectRow(RowCheck, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			Prompter.Error("Unable to save changes.\n\nUnable to check for changes by another user.");
		}
		else if (RowCheck.CompareRowVersion(RowAccount) == FALSE)
		{
			Prompter.Error("Unable to save changes.\n\nAccount has already been modified by another user.");
		}
		else
		{
			CSQLTranSaveAccount Tran;
			Tran.BeginTrans();
			Tran.DoWork(RowAccount, RowExternalOld, RowExternalNew, dlgEdit.GetNewPINNumber());

			switch (Tran.EndTrans())
			{
			case SQLTRAN_STATE_COMMITOK:
				dlgEdit.UpdateAudit(bCancel);
				break;

			default:
				Tran.ShowErrorMessage("Edit Account");
				break;
			}
		}
	}

	return TRUE;
}

//*******************************************************************

bool CEditRecord::EditLeaverRecord(__int64 nUserID)
{
	bool bReply = FALSE;
	CString strError = "";
	CString strTitle = "";
	strTitle.Format("Edit Leaver Record ( %s )", 
		(const char*) m_strLeaversYear);

	CLeaversHandler data(m_strLeaversYear);								// create leavers folder\files if required

	int nMode = (Passwords.IsEditRecordAllowed() == FALSE) ? DB_READONLY : DB_READWRITE;

	int nLeaverYear = atoi(m_strLeaversYear.Right(4));

	if ((nLeaverYear >= 2000) && (nLeaverYear <= 9999))
	{
		g_GlobalState.SetLeaverYear(nLeaverYear);

		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(nUserID);
		RowAccount.SetLeaverYear(nLeaverYear);

		{
			CString strMsg = "";
			SelectRowForEdit(RowAccount, strMsg);

			if (strMsg != "")
			{
				Prompter.Error(strMsg, strTitle);
				return FALSE;
			}
		}

		CSQLRowExternalAccount RowExternal;
		CEditRecordPropertySheet dlgEdit(RowAccount, RowExternal, nMode, &data, NULL, System.GetDefaultAccountEditPage());			// open leavers database in read\write mode
		bool bCancel = (dlgEdit.DoModal() != IDOK);

		if (DB_READWRITE == nMode)
		{
			CSQLRowAccountFull RowCheck;
			RowCheck.SetUserID(RowAccount.GetUserID());
			RowCheck.SetLeaverYear(RowAccount.GetLeaverYear());

			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.SelectRow(RowCheck, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				Prompter.Error("Unable to save changes.\n\nUnable to check for changes by another user.");
			}
			else if (RowCheck.CompareRowVersion(RowAccount) == FALSE)
			{
				Prompter.Error("Unable to save changes.\n\nAccount has already been modified by another user.");
			}
			else
			{
				RepoAccount.UpdateRow(RowAccount, NULL);
				dlgEdit.UpdateAudit(bCancel);
			}
		}
	}
	else
	{
		bReply = FALSE;
	}

	return bReply;
}

//*******************************************************************

void CEditRecord::LogEdit(__int64 nCardNo, const char* szProcess)
{
	CTime tm = CTime::GetCurrentTime();

	CString strFilename = "";
	strFilename.Format("efilelog-%d%2.2d.dat", tm.GetYear(), tm.GetMonth());

	CString strMsg = "";
	strMsg.Format("[%d:%d] %s", Sysset.GetInstanceNo(), Passwords.GetPasswordInUse(), szProcess);

	::LogSysFileError(strMsg, strFilename, FormatInt64Value(nCardNo));
}

//*******************************************************************

