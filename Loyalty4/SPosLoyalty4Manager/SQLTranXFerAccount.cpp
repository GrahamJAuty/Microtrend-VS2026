/**********************************************************************/
#include "..\SPosLoyalty4Shared\ImageHandler.h"
#include "..\SPosLoyalty4Shared\LoyaltyAuditAppend.h"
#include "..\SPosLoyalty4Shared\LoyaltyAuditFile.h"
#include "..\SPosLoyalty4Shared\SQLStampOfferBuffer.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_CCNo\SQLRepositoryCCNo.h"
#include "..\SPosLoyalty4Shared\SQLTable_OfferStamp\SQLRepositoryOfferStamp.h"
#include "..\SPosLoyalty4Shared\SQLTable_StampOfferInfo\SQLRepositoryStampOfferInfo.h"
/**********************************************************************/
#include "TLogFile.h"
/**********************************************************************/
#include "SQLTranXFerAccount.h"
/**********************************************************************/

void CSQLTranXFerAccount::DoWork(CRecordTransferInfo& RecordXferData, bool bNeedCreate, bool bHotlistReplace, bool bDoHotlist, CString strHotlistComment, __int64 nUserIDFrom, __int64 nUserIDTo)
{
	SQLStampOfferBuffer.Reset();

	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_nUserIDFrom = nUserIDFrom;
		m_nUserIDTo = nUserIDTo;
		m_bHotlistReplace = bHotlistReplace;
		m_bCopyPurchaseHistory = RecordXferData.m_bSetCopyPurchaseHistory;

		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		CSQLRowAccountFull RowAccountFrom, RowAccountTo;
		RowAccountFrom.SetUserID(nUserIDFrom);
		RowAccountTo.SetUserID(nUserIDTo);

		CSQLRepositoryAccount repoAccount;

		if (repoAccount.SelectRow(RowAccountFrom, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}

		if (TRUE == bNeedCreate)
		{
			if (repoAccount.InsertRow(RowAccountTo, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}

			if (TRUE == bHotlistReplace)
			{
				CAuditRecord atc;
				atc.SetSQL(RowAccountTo);
				atc.SetApplicationNo(APPNO_INSERTREPLACE);
				atc.SetComment(strHotlistComment);
				atc.SaveLineToArray(m_arrayAuditBuffer);
			}
		}

		if (repoAccount.SelectRow(RowAccountTo, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}

		Transfer(RecordXferData, RowAccountFrom, TRUE);
		Transfer(RecordXferData, RowAccountTo, FALSE);

		if (TRUE == RecordXferData.m_bSetOfferStamps)
		{
			if (TransferStamps(RowAccountFrom, RowAccountTo) == FALSE)
			{
				return;
			}
		}

		if (TRUE == bDoHotlist)
		{
			RowAccountFrom.SetHotlistCode(1);

			CAuditRecord atc;
			atc.SetSQL(RowAccountFrom);				// audit hotlisted details
			atc.SetApplicationNo(APPNO_HOTLIST);
			atc.SetComment(strHotlistComment);
			atc.SaveLineToArray(m_arrayAuditBuffer);
		}

		if (repoAccount.UpdateRow(RowAccountFrom, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}

		if (repoAccount.UpdateRow(RowAccountTo, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/

void CSQLTranXFerAccount::Transfer(CRecordTransferInfo& RecordXferData, CSQLRowAccountFull& RowAccount, bool bXferOut)
{
	CString strExtraComment = "";

	CAuditRecord atc;
	atc.SetSQL(RowAccount);

	if (bXferOut == TRUE)					
	{
		TransferValues(RecordXferData, RowAccount, TRUE);

		if (m_bHotlistReplace == TRUE)
		{
			RowAccount.SetNextUserID(m_nUserIDTo);
			atc.SetApplicationNo(APPNO_HOTXFEROUT);
		}
		else
		{
			atc.SetApplicationNo(APPNO_XFEROUT);
		}
	}
	else												// transfer to new card
	{
		TransferValues(RecordXferData, RowAccount, FALSE);

		if (TRUE == RecordXferData.m_bSetMaxSpend)
		{
			RowAccount.SetMaxSpend(RecordXferData.m_dMaxSpend);
		}

		if (TRUE == RecordXferData.m_bSetOverSpend)
		{
			RowAccount.SetMaxOverdraft(RecordXferData.m_dOverSpend);
		}

		if (TRUE == RecordXferData.m_bSetName)
		{
			RowAccount.SetFullname(RecordXferData.m_strName);
		}

		if (TRUE == RecordXferData.m_bSetGroup)
		{
			RowAccount.SetGroupNo(RecordXferData.m_nGroupNo);
		}

		if (TRUE == RecordXferData.m_bSetScheme)
		{
			RowAccount.SetSchemeNo(RecordXferData.m_nSchemeNo);
		}

		if (TRUE == RecordXferData.m_bSetAddress1)
		{
			RowAccount.SetAddress1(RecordXferData.m_strAddress1);
		}

		if (TRUE == RecordXferData.m_bSetAddress2)
		{
			RowAccount.SetAddress2(RecordXferData.m_strAddress2);
		}

		if (TRUE == RecordXferData.m_bSetAddress3)
		{
			RowAccount.SetAddress3(RecordXferData.m_strAddress3);
		}

		if (TRUE == RecordXferData.m_bSetAddress4)
		{
			RowAccount.SetAddress4(RecordXferData.m_strAddress4);
		}

		if (TRUE == RecordXferData.m_bSetAddress5)
		{
			RowAccount.SetAddress5(RecordXferData.m_strAddress5);
		}

		if (TRUE == RecordXferData.m_bSetPhone1)
		{
			RowAccount.SetPhone1(RecordXferData.m_strPhone1);
		}

		if (TRUE == RecordXferData.m_bSetPhone2)
		{
			RowAccount.SetPhone2(RecordXferData.m_strPhone2);
		}

		if (TRUE == RecordXferData.m_bSetExpiry)
		{
			RowAccount.SetExpiryDate(RecordXferData.m_strExpiry);
		}

		if (TRUE == RecordXferData.m_bSetInfo1)
		{
			RowAccount.SetInfo1(RecordXferData.m_strInfo1);
		}

		if (TRUE == RecordXferData.m_bSetInfo2)
		{
			RowAccount.SetInfo2(RecordXferData.m_strInfo2);
		}

		if (TRUE == RecordXferData.m_bSetInfo3)
		{
			RowAccount.SetInfo3(RecordXferData.m_strInfo3);
		}

		if (TRUE == RecordXferData.m_bSetInfo4)
		{
			RowAccount.SetInfo4(RecordXferData.m_strInfo4);
		}

		if (TRUE == RecordXferData.m_bSetDOB)
		{
			RowAccount.SetDOB(RecordXferData.m_strDOB);
		}

		if (TRUE == RecordXferData.m_bSetAlert)
		{
			RowAccount.SetAlertCodes(RecordXferData.m_strAlertCodes);
		}

		if (TRUE == RecordXferData.m_bSetInactive)
		{
			RowAccount.SetInactive(RecordXferData.m_bInactive);
		}

		if (TRUE == RecordXferData.m_bSetGender)
		{
			RowAccount.SetGender(RecordXferData.m_strGender);
		}

		if (TRUE == m_bHotlistReplace)
		{
			RowAccount.SetPurse1LastSpend(RecordXferData.m_dPurse1LastSpend);
			RowAccount.SetPurse1LastSpendDate(RecordXferData.m_strPurse1DateLastUsed);
			RowAccount.SetPurse1LastSpendTime(RecordXferData.m_strPurse1TimeLastUsed);
			RowAccount.SetPurse2LastSpend(RecordXferData.m_dPurse2LastSpend);
			RowAccount.SetPurse2LastSpendDate(RecordXferData.m_strPurse2DateLastUsed);
			RowAccount.SetPurse2LastSpendTime(RecordXferData.m_strPurse2TimeLastUsed);
			RowAccount.SetPurse2RefreshedDate(RecordXferData.m_strDateLastRefreshed);
			RowAccount.SetPurse2RefreshedTime(RecordXferData.m_strTimeLastRefreshed);

			RowAccount.SetPreviousUserID(m_nUserIDFrom);
			atc.SetApplicationNo(APPNO_HOTXFERIN);
		}
		else
		{
			atc.SetApplicationNo(APPNO_XFERIN);
		}
	}

	atc.SetComment(RecordXferData.m_strComment);
	atc.AuditChanges(RowAccount);
	atc.SaveLineToArray(m_arrayAuditBuffer);
}

/**********************************************************************/

bool CSQLTranXFerAccount::TransferStamps(CSQLRowAccountFull& RowAccountFrom, CSQLRowAccountFull& RowAccountTo)
{
	CSQLRepositoryOfferStamp repoStamp;
	CArray<CSQLRowOfferStamp, CSQLRowOfferStamp> arrayStamps;
	repoStamp.GetOfferStampsByUserID(RowAccountFrom.GetUserID(), arrayStamps, m_pDatabase);

	CString strOfferName = "";
	int nLastOfferID = -1;
	int nAppNo = m_bHotlistReplace ? APPNO_SO_HOTL : APPNO_SO_XFER;

	for (int n = 0; n < arrayStamps.GetSize(); n++)
	{
		CSQLRowOfferStamp RowStamps = arrayStamps.GetAt(n);

		CSQLRepositoryStampOfferInfo repoInfo;
		repoInfo.GetOfferName(RowStamps.GetStampOfferID(), nLastOfferID, strOfferName, m_pDatabase);
		
		if (repoStamp.DeleteRow(RowStamps, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}
		else
		{
			SQLStampOfferBuffer.BufferPCAction(
				nAppNo,
				RowAccountFrom,
				RowStamps,
				strOfferName,
				RowStamps.GetStampCount() * (-1),
				0);
		}

		RowStamps.SetUserID(RowAccountTo.GetUserID());
		int nStamps = RowStamps.GetStampCount();

		if (repoStamp.SelectRow(RowStamps, m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			RowStamps.SetStampCount(RowStamps.GetStampCount() + nStamps);

			if (repoStamp.UpdateRow(RowStamps, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return FALSE;
			}
			else
			{
				SQLStampOfferBuffer.BufferPCAction(
					nAppNo,
					RowAccountTo,
					RowStamps,
					strOfferName,
					nStamps,
					RowStamps.GetStampCount());
			}
		}
		else
		{
			if (repoStamp.InsertRow(RowStamps, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return FALSE;
			}
			else
			{
				SQLStampOfferBuffer.BufferPCAction(
					nAppNo,
					RowAccountTo,
					RowStamps,
					strOfferName,
					RowStamps.GetStampCount(),
					RowStamps.GetStampCount());
			}
		}
	}

	return TRUE;
}

/**********************************************************************/

void CSQLTranXFerAccount::TransferValues(CRecordTransferInfo& RecordXferData, CSQLRowAccountFull& RowAccount, bool bXferOut)
{
	int nMultiplier = bXferOut ? -1 : 1;

	if (TRUE == RecordXferData.m_bSetPoints)
	{
		RowAccount.AddToPoints(RecordXferData.m_nPoints * nMultiplier);
	}

	if (TRUE == RecordXferData.m_bSetPointsTD)
	{
		RowAccount.AddToPointsToDate(RecordXferData.m_nPointsTD * nMultiplier);
	}

	if (TRUE == RecordXferData.m_bSetPurse1)
	{
		RowAccount.AddToPurse1(RecordXferData.m_dPurse1 * nMultiplier);
	}

	if (TRUE == RecordXferData.m_bSetPurse1TD)
	{
		RowAccount.AddToPurse1SpendToDate(RecordXferData.m_dPurse1TD * nMultiplier);
	}

	if (TRUE == RecordXferData.m_bSetPurse2)
	{
		RowAccount.AddToPurse2(RecordXferData.m_dPurse2 * nMultiplier);
	}

	if (TRUE == RecordXferData.m_bSetPurse2TD)
	{
		RowAccount.AddToPurse2SpendToDate(RecordXferData.m_dPurse2TD * nMultiplier);
	}

	if (TRUE == RecordXferData.m_bSetCashTD)
	{
		RowAccount.AddToCashSpendToDate(RecordXferData.m_dCashTD * nMultiplier);
	}
}

/**********************************************************************/

void CSQLTranXFerAccount::AfterTransaction()
{
	CSQLTranBase::AfterTransaction();

	SQLStampOfferBuffer.WritePCAuditLines(m_pDatabase);

	if (TRUE == m_bHotlistReplace)
	{
		CString strFromID = SimpleFormatInt64Value(m_nUserIDFrom);
		CString strToID = SimpleFormatInt64Value(m_nUserIDFrom);

		if (TRUE == m_bCopyPurchaseHistory)
		{
			// will merge in any outstanding updates
			CTLogFile tlogFile(strFromID);						
			
			CString strFromFilename = Filenames.GetPurchaseHistoryFilename(strFromID);
			CString strToFilename = Filenames.GetPurchaseHistoryFilename(strToID);

			::CopyFile(strFromFilename, strToFilename, FALSE);
		}

		// copy photo file if found
		CImageHandler image;
		image.CopyFile(strFromID, strToID);
	}
}

/**********************************************************************/
