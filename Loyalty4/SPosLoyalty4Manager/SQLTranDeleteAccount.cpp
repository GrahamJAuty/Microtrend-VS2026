/**********************************************************************/
#include "..\SPosLoyalty4Shared\AESHelpers.h"
#include "..\SPosLoyalty4Shared\AuditRecord_base.h"
#include "..\SPosLoyalty4Shared\ImageHandler.h"
#include "..\SPosLoyalty4Shared\SQLResultInfo.h"
#include "..\SPosLoyalty4Shared\SQLStampOfferBuffer.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_ExternalAccount\SQLRepositoryExternalAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_OfferStamp\SQLRepositoryOfferStamp.h"
#include "..\SPosLoyalty4Shared\SQLTable_StampOfferInfo\SQLRepositoryStampOfferInfo.h"
/**********************************************************************/
#include "DatabaseExport.h"
/**********************************************************************/
#include "SQLTranDeleteAccount.h"
/**********************************************************************/

void CSQLTranDeleteAccount::DoWork(CSQLRowAccountFull& RowAccount, CString strComment)
{
	SQLStampOfferBuffer.Reset();

	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		m_strComment = strComment;

		if (DoDelete(RowAccount, m_arrayAuditBuffer) == TRUE)
		{
			m_RowDeletedAccount = RowAccount;
			m_bCanCommit = TRUE;
		}
	}
}

/**********************************************************************/

bool CSQLTranDeleteAccount::DeleteExternal(CSQLRowAccountFull& RowAccount, CString strComment, CDatabase* pDatabase, CStringArray& arrayAuditBuffer)
{
	m_pDatabase = pDatabase;
	m_strComment = strComment;
	return DoDelete(RowAccount, arrayAuditBuffer, TRUE);
}

/**********************************************************************/

bool CSQLTranDeleteAccount::DoDelete( CSQLRowAccountFull& RowAccount, CStringArray& arrayAuditBuffer, bool bFastDelete )
{
	if (FALSE == bFastDelete)
	{
		CSQLRepositoryAccount repoAccount;
		if (repoAccount.DeleteRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}
	}
	else
	{
		CString strSQL = "";
		strSQL.Format("DELETE FROM %s WHERE %s = %I64d",
			SQLTableNames::Accounts,
			Account::UserID.Label,
			RowAccount.GetUserID());

		if (CSQLHelpers::ExecuteSQL(m_pDatabase, strSQL) != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}
	}
	
	CString strCardNo = SimpleFormatInt64Value(RowAccount.GetUserID());

	CString strAESCardNo = "";
	CAESHelpers::PlainTextToAESHex(strCardNo, strAESCardNo);

	CSQLRowExternalAccount RowExternal;
	RowExternal.SetAESUserID(strAESCardNo);

	CSQLRepositoryExternalAccount repoExt;
	switch (repoExt.DeleteRowByUserID(RowExternal, m_pDatabase).GetSQLError())
	{
	case SQLCRUD_ERR_NONE:
	case SQLCRUD_ERR_NOMATCH:
		break;

	default:
		return FALSE;
	}

	{
		CSQLRepositoryOfferStamp repoStamp;
		CSQLRepositoryStampOfferInfo repoOffer;

		CArray<CSQLRowOfferStamp, CSQLRowOfferStamp> arrayOfferStamps;
		repoStamp.GetOfferStampsByUserID(RowAccount.GetUserID(), arrayOfferStamps, m_pDatabase);

		CString strOfferName = "";
		int nLastOfferID = -1;

		for (int n = 0; n < arrayOfferStamps.GetSize(); n++)
		{
			CSQLRowOfferStamp RowStamps = arrayOfferStamps.GetAt(n);
			
			repoOffer.GetOfferName(RowStamps.GetStampOfferID(), nLastOfferID, strOfferName, m_pDatabase);

			if (repoStamp.DeleteRow(RowStamps, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return FALSE;
			}
			else
			{
				SQLStampOfferBuffer.BufferPCAction(
					APPNO_SO_ADEL,
					RowAccount,
					RowStamps,
					strOfferName,
					RowStamps.GetStampCount() * (-1),
					0);
			}
		}
	}

	CAuditRecord_base atc;
	atc.SetSQL(RowAccount);
	atc.SetApplicationNo(APPNO_DELETE);
	atc.SetComment(m_strComment);
	atc.SaveLineToArray(arrayAuditBuffer);

	return TRUE;
}

/**********************************************************************/

void CSQLTranDeleteAccount::AfterTransaction()
{
	CSQLTranBase::AfterTransaction();
	SQLStampOfferBuffer.WritePCAuditLines(m_pDatabase);
	NonSQLDelete(m_RowDeletedAccount);
}

/**********************************************************************/

void CSQLTranDeleteAccount::NonSQLDeleteExternal(CSQLRowAccountFull& RowAccount, CString strComment)
{
	m_strComment = strComment;
	NonSQLDelete(RowAccount);
}

/**********************************************************************/

void CSQLTranDeleteAccount::NonSQLDelete(CSQLRowAccountFull& RowAccount)
{
	CString strCardNo = SimpleFormatInt64Value(RowAccount.GetUserID());

	::SendToRecycleBin(Filenames.GetPurchaseHistoryFilename(strCardNo));

	CImageHandler image;
	image.DeleteFile(strCardNo);							// delete photo file

	if (System.GetExportOnDeleteFlag() == TRUE)
	{
		CExportDatabase exp1("DEL", NULL);
		exp1.ExportRecord(RowAccount);
	}
}

/**********************************************************************/

