/**********************************************************************/
#include "..\Smartpay4Shared\AESHelpers.h"
#include "..\Smartpay4Shared\ImageHandler.h"
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\Smartpay4Shared\RedeemPoints.h"
#include "..\Smartpay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\Smartpay4Shared\SQLTable_ExternalAccount\SQLRepositoryExternalAccount.h"
#include "..\Smartpay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/
#include "..\Smartpay4ManagerBgnd\SQLTranDeleteAccount.h"
/**********************************************************************/
#include "BatchUpdate.h"
/**********************************************************************/
#include "SQLTranBatchUpdate.h"
/**********************************************************************/

CSQLTranBatchUpdate::CSQLTranBatchUpdate() : CSQLTranBase()
{
	m_nCount = 0;
}

/**********************************************************************/

void CSQLTranBatchUpdate::DoWork(int nUpdateType, CArray<CSQLRowAccountList, CSQLRowAccountList>& arraySourceItems, CBatchAccountBalanceMemory& BalanceMemory, CBatchAccountDetailMemory& DetailMemory, CString strComment, CString strLeaversYear)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;
		m_nCount = 0;

		m_strLeaversYear = strLeaversYear;
		m_strComment = strComment;

		int nAppNo = 0;
		CString strTitle = "";

		switch (nUpdateType)
		{
		case nBATCHUPDATE_UPDATE:
			strTitle = "Updating records";
			nAppNo = APPNO_BATCHUPDATE;
			break;

		case nBATCHUPDATE_DELETE:
			strTitle = "Deleting records";
			nAppNo = APPNO_BATCHDELETE;
			break;

		case nBATCHUPDATE_REDEEM:
			strTitle = "Redeeming Points";
			nAppNo = APPNO_BATCHREDEEM;
			break;

		default:
			return;
		}

		CWorkingDlg WorkingDlg(strTitle);
		WorkingDlg.Create();

		for (int n = 0; n < arraySourceItems.GetSize(); n++)
		{
			if (arraySourceItems[n].GetCardPickerFlag() == FALSE)
			{
				continue;
			}

			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(arraySourceItems[n].GetUserID());

			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.SelectRow(RowAccount, m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				bool bOK = TRUE;
				
				m_atc.Set(&RowAccount);
				
				switch (nUpdateType)
				{
				case nBATCHUPDATE_DELETE:
					if (DeleteRow(RowAccount) == FALSE)
					{
						return;
					}
					m_arrayDeletedAccounts.Add(RowAccount);
					break;

				case nBATCHUPDATE_UPDATE:
					bOK = BalanceMemory.UpdateRecord(RowAccount) || DetailMemory.UpdateRecord(RowAccount);
					
					if (TRUE == bOK)
					{
						if (RepoAccount.UpdateRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
						{
							return;
						}
						m_atc.AuditChanges(&RowAccount, TRUE);	
					}
					break;

				case nBATCHUPDATE_REDEEM:
					bOK = UpdateRedeemPoints(RowAccount);
					if (TRUE == bOK)
					{
						if (RepoAccount.UpdateRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
						{
							return;
						}
						m_atc.AuditChanges(&RowAccount);
					}
					break;

				default:
					bOK = FALSE;
					break;
				}

				if ( TRUE == bOK )
				{
					//AUDIT LINE FOR DELETE IS CREATED BY DELETE TRANSACTION
					if (nUpdateType != nBATCHUPDATE_DELETE)
					{
						m_atc.SetApplicationNo(nAppNo);
						m_atc.SetOperatorID(Passwords.GetPasswordInUse());
						m_atc.SetLineComment(strComment);
						m_atc.BufferNewLine();										
					}
					++m_nCount;
				}
			}

			if (WorkingDlg.SetCaption2RecordsChecked(m_nCount) == TRUE)
			{
				return;
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/

bool CSQLTranBatchUpdate::DeleteRow(CSQLRowAccountFull& RowAccount)
{
	CSQLTranDeleteAccount TranDelete;
	return TranDelete.DeleteExternal(RowAccount, m_strComment, m_strLeaversYear, m_pDatabase, m_atc);
}

/**********************************************************************/

bool CSQLTranBatchUpdate::UpdateRedeemPoints(CSQLRowAccountFull& RowAccount)
{	
	CRedeemPoints pointsRedeemer;												// points redeem
	pointsRedeemer.m_bDisabled = FALSE;
	pointsRedeemer.m_nTrip = Server.GetActiveRedeemPointsTrip();
	pointsRedeemer.m_dValue = Server.GetRedeemValue();
	pointsRedeemer.m_nPurse = Server.GetRedeemPurse();

	CSQLRowGroupFull RowGroup;
	RowGroup.SetGroupNo(RowAccount.GetGroupNo());

	CSQLRepositoryGroup RepoGroup;
	if (RepoGroup.SelectRow(RowGroup, m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (RowGroup.GetActiveRedeemFlag() == TRUE)							// group specifc redeem points
		{
			pointsRedeemer.m_bDisabled = RowGroup.GetRedeemDisable();
			pointsRedeemer.m_nTrip = RowGroup.GetRedeemTrip();
			pointsRedeemer.m_dValue = RowGroup.GetRedeemValue();
			pointsRedeemer.m_nPurse = RowGroup.GetRedeemPurse();
		}
	}

	if (pointsRedeemer.m_bDisabled == FALSE)
	{
		double dValueAdded;
		int nPointsRedeemed = pointsRedeemer.Redeem(RowAccount.GetPoints(), dValueAdded);

		if (nPointsRedeemed > 0)
		{
			RowAccount.AddToPoints(-nPointsRedeemed);

			switch (pointsRedeemer.m_nPurse)
			{
			case 3:
				RowAccount.AddToPurse3Liability(dValueAdded);
				break;

			case 2:
				RowAccount.AddToPurse2Balance(dValueAdded);
				break;

			default:
				RowAccount.AddToPurse1Liability(dValueAdded);
				break;
			}
			return TRUE;
		}
	}
	return FALSE;
}
//*******************************************************************

void CSQLTranBatchUpdate::AfterTransaction()
{
	CSQLTranBase::AfterTransaction();

	for (int n = 0; n < m_arrayDeletedAccounts.GetSize(); n++)
	{
		CSQLTranDeleteAccount TranDelete;
		CSQLRowAccountFull RowAccount = m_arrayDeletedAccounts.GetAt(n);
		TranDelete.NonSQLDeleteExternal(RowAccount,m_strComment,m_strLeaversYear);
	}
}

/**********************************************************************/
