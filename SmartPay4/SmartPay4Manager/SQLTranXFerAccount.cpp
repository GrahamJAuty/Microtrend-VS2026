/**********************************************************************/
#include "..\SmartPay4Shared\ImageHandler.h"
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\TLogFile.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlStamp\SQLRepositoryPurchaseControlStamp.h"
#include "..\SmartPay4Shared\SQLTable_PeriodRefresh\SQLRepositoryPeriodRefresh.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLRepositoryPurchaseControlRule.h"
#include "..\SmartPay4Shared\SQLResultInfo.h"
#include "..\SmartPay4Shared\SQLPurchaseControlBuffer.h"
/**********************************************************************/
#include "SQLTranXFerAccount.h"
/**********************************************************************/

void CSQLTranXFerAccount::DoWork(CRecordXferData& RecordXferData, bool bNeedCreate, bool bHotlistReplace, bool bDoHotlist, int nHotlistType, CString strHotlistComment, __int64 nUserIDFrom, __int64 nUserIDTo)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_nUserIDFrom = nUserIDFrom;
		m_nUserIDTo = nUserIDTo;
		m_bHotlistReplace = bHotlistReplace;
		m_bCopyPurchaseHistory = RecordXferData.m_bCopyPurchaseHistory;

		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		CSQLRowAccountFull RowAccountFrom, RowAccountTo;
		RowAccountFrom.SetUserID(nUserIDFrom);
		RowAccountTo.SetUserID(nUserIDTo);

		CSQLRepositoryAccount RepoAccount;

		if (RepoAccount.SelectRow(RowAccountFrom, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}

		if (TRUE == bNeedCreate)
		{
			if (RepoAccount.InsertRow(RowAccountTo, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}

			if (TRUE == bHotlistReplace)
			{
				m_atc.Set(&RowAccountTo);
				m_atc.SetApplicationNo(APPNO_HOTLISTREPLACE);
				m_atc.SetOperatorID(Passwords.GetPasswordInUse());
				m_atc.SetLineComment(FormatInt64Value(nUserIDTo));
				m_atc.BufferNewLine();
			}
		}

		if (RepoAccount.SelectRow(RowAccountTo, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}

		Transfer(RecordXferData, RowAccountFrom, RowAccountTo.GetUserIDString(), TRUE);
		Transfer(RecordXferData, RowAccountTo, RowAccountFrom.GetUserIDString(), FALSE);

		if (TRUE == RecordXferData.m_bPurchaseControl)
		{
			if (TransferStamps(RowAccountFrom, RowAccountTo) == FALSE)
			{
				return;
			}
		}

		if (TRUE == bDoHotlist)
		{
			RowAccountFrom.SetAlertCode(nHotlistType);
			m_atc.Set(&RowAccountFrom);
			m_atc.SetApplicationNo(APPNO_HOTLIST);
			m_atc.SetOperatorID(Passwords.GetPasswordInUse());
			m_atc.SetAuditLineExtraText(nHotlistType);
			m_atc.SetLineComment(strHotlistComment);
			m_atc.BufferNewLine();
		}

		if (TRUE == RecordXferData.m_bMemberID)
		{
			RowAccountFrom.SetMemberID("");
		}

		if (RepoAccount.UpdateRow(RowAccountFrom, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}

		if (RepoAccount.UpdateRow(RowAccountTo, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/

void CSQLTranXFerAccount::Transfer(CRecordXferData& RecordXferData, CSQLRowAccountFull& RowAccount, CString strOtherID, bool bXferOut)
{
	CString strExtraComment = "";

	m_atc.Set(&RowAccount);
	m_atc.SetXFerUserID(strOtherID);

	if (bXferOut == TRUE)					
	{
		TransferValues(RecordXferData, RowAccount, TRUE);

		if (m_bHotlistReplace == TRUE)
		{
			RowAccount.SetNextUserID(m_nUserIDTo);
			m_atc.SetApplicationNo(APPNO_HOTXFEROUT);
			strExtraComment = System.FormatCardNo(FormatInt64Value(m_nUserIDTo));
		}
		else
		{
			m_atc.SetApplicationNo(APPNO_XFEROUT);
		}
	}
	else												// transfer to new card
	{
		TransferValues(RecordXferData, RowAccount, FALSE);

		if (TRUE == RecordXferData.m_bMemberID)
		{
			RowAccount.SetMemberID(RecordXferData.m_strMemberID);
		}

		if (TRUE == RecordXferData.m_bUserName)
		{
			RowAccount.SetUsername(RecordXferData.m_strUserName);
		}

		if (TRUE == RecordXferData.m_bForename)
		{
			RowAccount.SetForename(RecordXferData.m_strForename);
		}

		if (TRUE == RecordXferData.m_bSurname)
		{
			RowAccount.SetSurname(RecordXferData.m_strSurname);
		}

		if (TRUE == RecordXferData.m_bGroup)
		{
			RowAccount.SetGroupNo(RecordXferData.m_nGroupNo);					// default group number
			RowAccount.SetDailyGroups(RecordXferData.m_strDayGroups);			// alternative daily groups
		}

		if (TRUE == RecordXferData.m_bExpiry)
		{
			RowAccount.SetExpiryDate(RecordXferData.m_strExpiryDate);
		}

		if (TRUE == RecordXferData.m_bDOB)
		{
			RowAccount.SetDOB(RecordXferData.m_strDOB);
		}

		if (TRUE == RecordXferData.m_bTransferGender)
		{
			RowAccount.SetGender(RecordXferData.m_strGender);
		}

		if (TRUE == RecordXferData.m_bEmail)
		{
			RowAccount.SetEmail(RecordXferData.m_strEmail);
		}

		for (int n = 1; n <= MAXINFOTICK; n++)
		{
			if (TRUE == RecordXferData.m_bInfoText[n])
			{
				RowAccount.SetInfo(n, RecordXferData.m_strInfo[n]);
			}
		}

		if (TRUE == RecordXferData.m_bTick[1])
		{
			RowAccount.SetTick1(RecordXferData.m_bRecordTick[1]);
		}

		if (TRUE == RecordXferData.m_bTick[2])
		{
			RowAccount.SetTick2(RecordXferData.m_bRecordTick[2]);
		}

		if (TRUE == RecordXferData.m_bTick[3])
		{
			RowAccount.SetTick3(RecordXferData.m_bRecordTick[3]);
		}

		if (TRUE == RecordXferData.m_bTick[4])
		{
			RowAccount.SetTick4(RecordXferData.m_bRecordTick[4]);
		}

		if (TRUE == RecordXferData.m_bDietary)
		{
			RowAccount.SetDietary(RecordXferData.m_strDietary);
		}

		if (TRUE == RecordXferData.m_bAllergyCodes)
		{
			RowAccount.SetAllergyCodes(RecordXferData.m_nAllergyCodes);
		}

		if (TRUE == RecordXferData.m_bMaxSpend)
		{
			RowAccount.SetOwnMaxSpend(RecordXferData.m_bOwnMaxSpend);
			/*****/
			RowAccount.SetMaxSpendPurse1Period1(RecordXferData.m_dMaxSpend[1]);
			RowAccount.SetMaxSpendPurse1Period2(RecordXferData.m_dMaxSpend[2]);
			RowAccount.SetMaxSpendPurse1Period3(RecordXferData.m_dMaxSpend[3]);
			RowAccount.SetMaxSpendPurse1Period4(RecordXferData.m_dMaxSpend[4]);
			RowAccount.SetMaxSpendPurse1Period5(RecordXferData.m_dMaxSpend[5]);
			RowAccount.SetMaxSpendPurse1Period6(RecordXferData.m_dMaxSpend[6]);
			/*****/
			RowAccount.SetMaxOverdraftPurse1Period1(RecordXferData.m_dOverSpend[1]);
			RowAccount.SetMaxOverdraftPurse1Period2(RecordXferData.m_dOverSpend[2]);
			RowAccount.SetMaxOverdraftPurse1Period3(RecordXferData.m_dOverSpend[3]);
			RowAccount.SetMaxOverdraftPurse1Period4(RecordXferData.m_dOverSpend[4]);
			RowAccount.SetMaxOverdraftPurse1Period5(RecordXferData.m_dOverSpend[5]);
			RowAccount.SetMaxOverdraftPurse1Period6(RecordXferData.m_dOverSpend[6]);
			/*****/
			RowAccount.SetMaxSpendPurse3Period1(RecordXferData.m_dMaxSpendP3[1]);
			RowAccount.SetMaxSpendPurse3Period2(RecordXferData.m_dMaxSpendP3[2]);
			RowAccount.SetMaxSpendPurse3Period3(RecordXferData.m_dMaxSpendP3[3]);
			RowAccount.SetMaxSpendPurse3Period4(RecordXferData.m_dMaxSpendP3[4]);
			RowAccount.SetMaxSpendPurse3Period5(RecordXferData.m_dMaxSpendP3[5]);
			RowAccount.SetMaxSpendPurse3Period6(RecordXferData.m_dMaxSpendP3[6]);
			/*****/
			RowAccount.SetMaxOverdraftPurse3Period1(RecordXferData.m_dOverSpendP3[1]);
			RowAccount.SetMaxOverdraftPurse3Period2(RecordXferData.m_dOverSpendP3[2]);
			RowAccount.SetMaxOverdraftPurse3Period3(RecordXferData.m_dOverSpendP3[3]);
			RowAccount.SetMaxOverdraftPurse3Period4(RecordXferData.m_dOverSpendP3[4]);
			RowAccount.SetMaxOverdraftPurse3Period5(RecordXferData.m_dOverSpendP3[5]);
			RowAccount.SetMaxOverdraftPurse3Period6(RecordXferData.m_dOverSpendP3[6]);
			/*****/
			for (int nPeriod = 1; nPeriod <= 6; nPeriod++)
			{
				for (int nPurse = 1; nPurse <= 3; nPurse++)
				{
					RowAccount.SetPurseAllowed(nPurse, nPeriod - 1, RecordXferData.m_bPurseAllowed[nPeriod][nPurse]);
				}
			}
		}

		if (TRUE == m_bHotlistReplace)
		{
			m_atc.SetApplicationNo(APPNO_HOTXFERIN);
			strExtraComment = System.FormatCardNo(FormatInt64Value(m_nUserIDFrom));

			RowAccount.SetPreviousUserID(m_nUserIDFrom);
			RowAccount.SetPurse2RefreshedDate(RecordXferData.m_strRefreshedDate);
			RowAccount.SetPurse2RefreshedTime(RecordXferData.m_strRefreshedTime);

			{
				CSQLRowPeriodRefresh RowRefresh;
				RowRefresh.SetUserID(m_nUserIDFrom);

				CSQLRepositoryPeriodRefresh RepoPeriod;
				switch (RepoPeriod.SelectRow(RowRefresh, m_pDatabase).GetSQLError())
				{
				case SQLCRUD_ERR_NOMATCH:
					break;

				case SQLCRUD_ERR_NONE:
				{
					RowRefresh.SetUserID(m_nUserIDTo);
					if (RepoPeriod.UpsertRow(RowRefresh, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
					{
						return;
					}
				}
				break;

				default:
					return;
				}
			}

			if ((TRUE == RecordXferData.m_bPurse1Liability) || (TRUE == RecordXferData.m_bPurse1Credit))
			{
				RowAccount.SetPurse1LastSpend(RecordXferData.m_dPurse1Spend);
				RowAccount.SetPurse1LastSpendDate(RecordXferData.m_strPurse1DateLastUsed);
				RowAccount.SetPurse1LastSpendTime(RecordXferData.m_strPurse1TimeLastUsed);
			}

			if (TRUE == RecordXferData.m_bPurse2)
			{
				RowAccount.SetPurse2LastSpend(RecordXferData.m_dPurse2Spend);
				RowAccount.SetPurse2LastSpendDate(RecordXferData.m_strPurse2DateLastUsed);
				RowAccount.SetPurse2LastSpendTime(RecordXferData.m_strPurse2TimeLastUsed);
			}

			if ((TRUE == RecordXferData.m_bPurse3Liability) || (TRUE == RecordXferData.m_bPurse3Credit))
			{
				RowAccount.SetPurse3LastSpend(RecordXferData.m_dPurse3Spend);
				RowAccount.SetPurse3LastSpendDate(RecordXferData.m_strPurse3DateLastUsed);
				RowAccount.SetPurse3LastSpendTime(RecordXferData.m_strPurse3TimeLastUsed);
			}
		}
		else
		{
			if (TRUE == RecordXferData.m_bAlertCode)
			{
				RowAccount.SetAlertCode(RecordXferData.m_nAlertCode);
			}
			m_atc.SetApplicationNo(APPNO_XFERIN);
		}
	}

	CString strComment = RecordXferData.m_strComment;			// get user audit comment
	if (strExtraComment != "")									// see if additional comment needs to be added
	{
		if (strComment == "")
		{
			strComment = strExtraComment;
		}
		else
		{
			CString strMsg = "";
			strMsg.Format("%s ( %s )", 
				(const char*) strComment, 
				(const char*) strExtraComment);
			strComment = strMsg;
		}
	}
	m_atc.SetLineComment(strComment);
	m_atc.AuditChanges(&RowAccount);
	m_atc.BufferNewLine();
}

/**********************************************************************/

void CSQLTranXFerAccount::TransferValues(CRecordXferData& RecordXferData, CSQLRowAccountFull& RowAccount, bool bXferOut)
{
	int nMultiplier = bXferOut ? -1 : 1;

	if (TRUE == RecordXferData.m_bPurse1Liability)
	{
		RowAccount.AddToPurse1Liability(RecordXferData.m_dPurse1Liability * nMultiplier);
	}

	if (TRUE == RecordXferData.m_bPurse1Credit)
	{
		RowAccount.AddToPurse1Credit(RecordXferData.m_dPurse1Credit * nMultiplier);
	}

	if (TRUE == RecordXferData.m_bPurse1SpendTD)
	{
		RowAccount.AddToPurse1SpendToDate(RecordXferData.m_dPurse1SpendTD * nMultiplier);
	}

	if (TRUE == RecordXferData.m_bPurse2)
	{
		RowAccount.AddToPurse2Balance(RecordXferData.m_dPurse2 * nMultiplier);
	}

	if (TRUE == RecordXferData.m_bPurse2SpendTD)
	{
		RowAccount.AddToPurse2SpendToDate(RecordXferData.m_dPurse2SpendTD * nMultiplier);
	}

	if (TRUE == RecordXferData.m_bPurse3Liability)
	{
		RowAccount.AddToPurse3Liability(RecordXferData.m_dPurse3Liability * nMultiplier);
	}

	if (TRUE == RecordXferData.m_bPurse3Credit)
	{
		RowAccount.AddToPurse3Credit(RecordXferData.m_dPurse3Credit * nMultiplier);
	}

	if (TRUE == RecordXferData.m_bPurse3SpendTD)
	{
		RowAccount.AddToPurse3SpendToDate(RecordXferData.m_dPurse3SpendTD * nMultiplier);
	}

	if (TRUE == RecordXferData.m_bPoints)
	{
		RowAccount.AddToPoints(RecordXferData.m_nPoints * nMultiplier);
	}

	if (TRUE == RecordXferData.m_bPointsTD)
	{
		RowAccount.AddToPointsToDate(RecordXferData.m_nPointsTD * nMultiplier);
	}
}

/**********************************************************************/

bool CSQLTranXFerAccount::TransferStamps(CSQLRowAccountFull& RowAccountFrom, CSQLRowAccountFull& RowAccountTo)
{
	CSQLRepositoryPurchaseControlStamp RepoStamp;
	CSQLRepositoryPurchaseControlRule RepoRule;

	CArray<CSQLRowPurchaseControlStamp, CSQLRowPurchaseControlStamp> arrayStamps;
	RepoStamp.GetPurchaseControlStampsByUserID(RowAccountFrom.GetUserID(), arrayStamps, m_pDatabase);

	CString strRuleName = "";
	int nLastRuleID = -1;
	int nAppNo = m_bHotlistReplace ? APPNO_SO_HOTL : APPNO_SO_XFER;

	for (int n = 0; n < arrayStamps.GetSize(); n++)
	{
		CSQLRowPurchaseControlStamp RowStamps = arrayStamps.GetAt(n);

		RepoRule.GetRuleName(RowStamps.GetRuleID(), nLastRuleID, strRuleName, m_pDatabase);

		if (RepoStamp.DeleteRow(RowStamps, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return FALSE;
		}
		else
		{
			SQLPurchaseControlBuffer.BufferPCAction(
				nAppNo,
				RowAccountFrom,
				RowStamps,
				strRuleName,
				RowStamps.GetStampCount() * (-1),
				0);
		}

		RowStamps.SetUserID(RowAccountTo.GetUserID());
		int nStamps = RowStamps.GetStampCount();

		if (RepoStamp.SelectRow(RowStamps, m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			RowStamps.SetStampCount(RowStamps.GetStampCount() + nStamps);

			if (RepoStamp.UpdateRow(RowStamps, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return FALSE;
			}
			else
			{
				SQLPurchaseControlBuffer.BufferPCAction(
					nAppNo,
					RowAccountTo,
					RowStamps,
					strRuleName,
					nStamps,
					RowStamps.GetStampCount());
			}
		}
		else
		{
			if (RepoStamp.InsertRow(RowStamps, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return FALSE;
			}
			else
			{
				SQLPurchaseControlBuffer.BufferPCAction(
					nAppNo,
					RowAccountTo,
					RowStamps,
					strRuleName,
					RowStamps.GetStampCount(),
					RowStamps.GetStampCount());
			}
		}
	}

	return TRUE;
}

/**********************************************************************/

void CSQLTranXFerAccount::AfterTransaction()
{
	CSQLTranBase::AfterTransaction();

	if (TRUE == m_bHotlistReplace)
	{
		CString strUserIDFrom = FormatInt64Value(m_nUserIDFrom);
		CString strUserIDTo = FormatInt64Value(m_nUserIDTo);

		CImageHandler image(FALSE);
		image.CopyFile(strUserIDFrom,strUserIDTo);	

		if (TRUE == m_bCopyPurchaseHistory)
		{
			CTLogFile tlogFile(strUserIDFrom);			
			
			CString strFromFilename = Filenames.GetPurchaseHistoryFilename(strUserIDFrom);
			CString strToFilename = Filenames.GetPurchaseHistoryFilename(strUserIDTo);

			::CopyFile(strFromFilename, strToFilename, FALSE);
		}
	}
}

/**********************************************************************/
