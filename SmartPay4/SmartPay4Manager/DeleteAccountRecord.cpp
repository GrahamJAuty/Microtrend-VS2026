/*********************************************************************/
#include "..\SmartPay4ManagerBgnd\DeleteRecord.h"
#include "..\SmartPay4ManagerBgnd\LeaversHandler.h"
/*********************************************************************/
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
/*********************************************************************/
#include "CommentDlg.h"
#include "DeleteLeaverDlg.h"
#include "SmartPay4Manager.h"
/*********************************************************************/
#include "DeleteAccountRecord.h"
/*********************************************************************/

CDeleteAccountRecord::CDeleteAccountRecord(CWnd* pParent )
{
	m_pParent = pParent;
}

/*********************************************************************/

void CDeleteAccountRecord::DeleteRecord(__int64 nUserID)
{
	int nReply = IDCANCEL;
	CString strComment = "";		// no audit comment
	CString strLeaversYear = "";	// don't move to a leavers database

	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserID(nUserID);

	CSQLRepositoryAccount RepoAccount;
	switch (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError())
	{
	case SQLCRUD_ERR_NONE:
	case SQLCRUD_ERR_DUPLICATE:
		break;

	default:
	{
		CString strMsg = "";
		strMsg.Format("Cannot find record for account ' %s ' in database!", 
			(const char*) FormatInt64Value(nUserID));
		Prompter.Error(strMsg, "Delete Account");
		return;
	}
	break;
	}

	CString strName = RowAccount.GetAccountDisplayName();
	
	if (strName == "")
	{
		strName = "account";
	}

	CString strCaption = "";
	strCaption.Format("Delete UserID %s", 
		(const char*) FormatInt64Value(nUserID));

	if (System.GetKeepLeaversDatabaseFlag() == TRUE)
	{
		CDeleteLeaverDlg dlg(strCaption, m_pParent);
		dlg.m_strMessage.Format("Remove %s from current database", 
			(const char*) strName);

		if ((nReply = dlg.DoModal()) == IDOK)
		{
			strComment = dlg.m_strComment;
			strLeaversYear = dlg.m_strLeaversYear;
		}
	}
	else
	{
		CCommentDlg dlg(strCaption, m_pParent);
		dlg.m_strMessage.Format("Delete %s from database", 
			(const char*) strName);

		if ((nReply = dlg.DoModal()) == IDOK)
		{
			strComment = dlg.m_strComment;
		}
	}

	if (nReply == IDOK)
	{
		CDeleteRecord DeleteRecord(RowAccount, strComment, strLeaversYear, m_pParent);
	}
}

//*******************************************************************
