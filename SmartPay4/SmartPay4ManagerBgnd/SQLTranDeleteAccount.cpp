/**********************************************************************/
#include "..\SmartPay4Shared\AESHelpers.h"
#include "..\SmartPay4Shared\GlobalState.h"
#include "..\SmartPay4Shared\ImageHandler.h"
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\SQLResultInfo.h"
#include "..\SmartPay4Shared\SQLPurchaseControlBuffer.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_BioRegister\SQLRepositoryBioRegister.h"
#include "..\SmartPay4Shared\SQLTable_ExternalAccount\SQLRepositoryExternalAccount.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlStamp\SQLRepositoryPurchaseControlStamp.h"
#include "..\SmartPay4Shared\SQLTable_PeriodRefresh\SQLRepositoryPeriodRefresh.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLRepositoryPurchaseControlRule.h"
/**********************************************************************/
#include "..\SmartPay4ManagerBgnd\DatabaseExport.h"
/**********************************************************************/
#include "LeaversHandler.h"
/**********************************************************************/
#include "SQLTranDeleteAccount.h"
/**********************************************************************/

void CSQLTranDeleteAccount::DoWork(CSQLRowAccountFull& RowAccount, CString strComment, CString strLeaversYear )
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		m_strComment = strComment;
		m_strLeaversYear = strLeaversYear;

		if (DoDelete(RowAccount,m_atc) == TRUE)
		{
			m_bCanCommit = TRUE;
			m_RowDeletedAccount = RowAccount;
		}
	}
}

/**********************************************************************/

bool CSQLTranDeleteAccount::DeleteExternal(CSQLRowAccountFull& RowAccount, CString strComment, CString strLeaversYear, CDatabase* pDatabase, CSQLAuditRecord_base& atc)
{
	m_pDatabase = pDatabase;
	m_strComment = strComment;
	m_strLeaversYear = strLeaversYear;
	return DoDelete(RowAccount,atc);
}

/**********************************************************************/

bool CSQLTranDeleteAccount::DoDelete(CSQLRowAccountFull& RowAccount, CSQLAuditRecord_base& atc)
{
	//DELETE ACCOUNT ROW
	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.DeleteRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		return FALSE;
	}

	//DELETE LINKED PREODAY ACCOUNTS
	CString strCardNo = FormatInt64Value(RowAccount.GetUserID());

	CString strAESCardNo = "";
	CAESHelpers::PlainTextToAESHex(strCardNo, strAESCardNo);

	CSQLRowExternalAccount RowExternal;
	RowExternal.SetAESUserID(strAESCardNo);

	CSQLRepositoryExternalAccount RepoExt;
	switch (RepoExt.DeleteRowByUserID(RowExternal, m_pDatabase).GetSQLError())
	{
	case SQLCRUD_ERR_NONE:
	case SQLCRUD_ERR_NOMATCH:
		break;

	default:
		return FALSE;
	}

	{
		CSQLRepositoryPurchaseControlStamp RepoStamp;
		CSQLRepositoryPurchaseControlRule RepoRule;

		CArray<CSQLRowPurchaseControlStamp, CSQLRowPurchaseControlStamp> arrayPurchaseControlStamps;
		RepoStamp.GetPurchaseControlStampsByUserID(RowAccount.GetUserID(), arrayPurchaseControlStamps, m_pDatabase);

		CString strRuleName = "";
		int nLastRuleID = -1;

		for (int n = 0; n < arrayPurchaseControlStamps.GetSize(); n++)
		{
			CSQLRowPurchaseControlStamp RowStamps = arrayPurchaseControlStamps.GetAt(n);

			RepoRule.GetRuleName(RowStamps.GetRuleID(), nLastRuleID, strRuleName, m_pDatabase);

			if (RepoStamp.DeleteRow(RowStamps, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return FALSE;
			}
			else
			{
				SQLPurchaseControlBuffer.BufferPCAction(
					APPNO_SO_ADEL,
					RowAccount,
					RowStamps,
					strRuleName,
					RowStamps.GetStampCount() * (-1),
					0);
			}
		}
	}

	//FIX PREVIOUS AND NEXT CARD LINKS

	if (RowAccount.GetNextUserID() != 0)
	{
		CSQLRowAccountFull RowNext;
		RowNext.SetUserID(RowAccount.GetNextUserID());

		__int64 nSQLError = RepoAccount.SelectRow(RowNext, m_pDatabase).GetSQLError();
		switch (nSQLError)
		{
		case SQLCRUD_ERR_NONE:
		case SQLCRUD_ERR_NOMATCH:
			break;

		default:
			return FALSE;
		}

		if (SQLCRUD_ERR_NONE == nSQLError)
		{
			if (RepoAccount.SelectRow(RowNext, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return FALSE;
			}

			RowNext.SetPreviousUserID(RowAccount.GetPreviousUserID());

			if (RepoAccount.UpdateRow(RowNext, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return FALSE;
			}
		}
	}

	if (RowAccount.GetPreviousUserID() != 0)
	{
		CSQLRowAccountFull RowPrevious;
		RowPrevious.SetUserID(RowAccount.GetPreviousUserID());

		__int64 nSQLError = RepoAccount.SelectRow(RowPrevious, m_pDatabase).GetSQLError();

		switch (nSQLError)
		{
		case SQLCRUD_ERR_NONE:
		case SQLCRUD_ERR_NOMATCH:
			break;

		default:
			return FALSE;
		}

		if (SQLCRUD_ERR_NONE == nSQLError)
		{
			if (RepoAccount.SelectRow(RowPrevious, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return FALSE;
			}

			RowPrevious.SetNextUserID(RowAccount.GetNextUserID());

			if (RepoAccount.UpdateRow(RowPrevious, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return FALSE;
			}
		}
	}

	//MOVE TO LEAVERS DATABASE IF REQUIRED
	if (m_strLeaversYear != "")
	{
		int nLeaversYear = atoi(m_strLeaversYear.Right(4));

		if ((nLeaversYear >= 2000) && (nLeaversYear <= 9999))
		{
			g_GlobalState.SetLeaverYear(nLeaversYear);
			RowAccount.SetLeaverYear(nLeaversYear);

			__int64 nSQLError = RepoAccount.UpsertRow(RowAccount, m_pDatabase).GetSQLError();

			g_GlobalState.ClearLeaverYear();
			
			if (nSQLError != SQLCRUD_ERR_NONE)
			{
				return FALSE;
			}
		}
	}

	//PERIOD REFRESH
	CSQLRowPeriodRefresh SQLRowRefresh;
	SQLRowRefresh.SetUserID(RowAccount.GetUserID());

	CSQLRepositoryPeriodRefresh RepoRefresh;
	switch (RepoRefresh.DeleteRow(SQLRowRefresh, m_pDatabase).GetSQLError())
	{
	case SQLCRUD_ERR_NONE:
	case SQLCRUD_ERR_NOMATCH:
		break;

	default:
		return FALSE;
	}

	//BIOMETRIC DATA

	switch (System.GetInterfaceType())
	{
	case nINTERFACE_BIOMETRIC:
	case nINTERFACE_DUAL:
	{
		CSQLRowBioRegister RowBioRegister;
		RowBioRegister.SetUserID(RowAccount.GetUserID());

		CSQLRepositoryBioRegister Repo;
		switch (Repo.DeleteRow(RowBioRegister, m_pDatabase).GetSQLError())
		{
		case SQLCRUD_ERR_NONE:
		case SQLCRUD_ERR_NOMATCH:
			break;

		default:
			return FALSE;
		}
	}
	break;
	}

	int nAppNo = (m_strLeaversYear == "") ? APPNO_DELETE : APPNO_DELETELEAVER;

	atc.Set(&RowAccount);											
	atc.SetApplicationNo(nAppNo);
	atc.SetOperatorID(Passwords.GetPasswordInUse());
	atc.SetLineComment(m_strComment);
	atc.BufferNewLine();

	return TRUE;
}

/**********************************************************************/

void CSQLTranDeleteAccount::AfterTransaction()
{
	CSQLTranBase::AfterTransaction();
	NonSQLDelete(m_RowDeletedAccount);
}

/**********************************************************************/

void CSQLTranDeleteAccount::NonSQLDeleteExternal(CSQLRowAccountFull& RowAccount, CString strComment, CString strLeaversYear)
{
	m_strComment = strComment;
	m_strLeaversYear = strLeaversYear;
	NonSQLDelete(RowAccount);
}

/**********************************************************************/

void CSQLTranDeleteAccount::NonSQLDelete(CSQLRowAccountFull& RowAccount)
{
	__int64 nUserID = RowAccount.GetUserID();

	// create leavers folder\files if required
	CLeaversHandler data(m_strLeaversYear);						
	
	// Purchase history
	CString strSourceFilename = Filenames.GetPurchaseHistoryFilename(FormatInt64Value(nUserID));	// check purchase history 
	if (::FileExists(strSourceFilename) == TRUE)
	{
		// see if moving to leavers database
		if (m_strLeaversYear != "")								
		{
			// move to leavers folders
			CString strDestFilename = data.GetPurchaseHistoryFilename(FormatInt64Value(nUserID));
			::MoveFile(strSourceFilename, strDestFilename);			
		}
		else
		{
			CFileRecycler::SendToRecycleBin(strSourceFilename);						// delete if not moving to leavers folder
		}
	}

	if (m_strLeaversYear != "")
	{
		m_atc.Set(&RowAccount);								
		m_atc.SetApplicationNo(APPNO_DELETELEAVER);
		m_atc.SetOperatorID(Passwords.GetPasswordInUse());
		m_atc.SetLineComment(m_strComment);
		
		CLeaversHandler data(m_strLeaversYear);	
		m_atc.SaveSessionLine(data.m_strAuditFilename);	
	}

	// Photo ID
	CImageHandler image(FALSE);
	if (m_strLeaversYear != "")												// see if moving to leavers database
	{
		strSourceFilename = image.GetImageFilepath(FormatInt64Value(nUserID));		// get first image
		if (strSourceFilename != "")										// see if have an image
		{
			CString strFilename = ::StripPathname(strSourceFilename);		// get just filename 
			CString strDestFilename = data.GetPhotoIDFilename(strFilename);
			::MoveFile(strSourceFilename, strDestFilename);
		}
	}
	else
	{
		image.DeleteFile(FormatInt64Value(nUserID));						// delete any photo files
	}

	// export deleted data
	if (System.GetExportOnDeleteFlag() == TRUE)
	{
		CExportDatabase exp1("DEL", NULL);
		exp1.ExportRecord(RowAccount);
	}

	CExportDatabase exp2("DEL2", NULL);				// biometic delete file
	exp2.ExportRecord(RowAccount);
}

/**********************************************************************/
