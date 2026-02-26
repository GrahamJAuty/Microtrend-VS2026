/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\AESHelpers.h"
#include "..\SPosLoyalty4Shared\ImageHandler.h"
#include "..\SPosLoyalty4Shared\LoyaltyAuditAppend.h"
#include "..\SPosLoyalty4Shared\RedeemPoints.h"
#include "..\SPosLoyalty4Shared\SQLStampOfferBuffer.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_ExternalAccount\SQLRepositoryExternalAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
/**********************************************************************/
#include "AuditRecord.h"
#include "BatchUpdate.h"
#include "DatabaseExport.h"
#include "PasswordData.h"
#include "SQLTranDeleteAccount.h"
#include "WorkingDlg.h"
/**********************************************************************/
#include "SQLTranBatchUpdate.h"
/**********************************************************************/

CSQLTranBatchUpdate::CSQLTranBatchUpdate() : CSQLTranBase()
{
	m_nCount = 0;
	m_strComment = "";
}

/**********************************************************************/

void CSQLTranBatchUpdate::DoWork(int nUpdateType, CArray<CSQLRowAccountList, CSQLRowAccountList>& arraySourceItems, CBatchAccountBalanceMemory& BalanceMemory, CBatchAccountDetailMemory& DetailMemory, CString strComment)
{
	m_nCount = 0;
	SQLStampOfferBuffer.Reset();

	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;
		m_nCount = 0;

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

		CString strCardNo = "";
		for ( int n = 0; n < arraySourceItems.GetSize(); n++ )
		{ 
			if (arraySourceItems[n].GetACListCardPickerFlag() == FALSE)
			{
				continue;
			}

			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(arraySourceItems[n].GetACListUserID());

			CSQLRepositoryAccount repoAccount;
			if (repoAccount.SelectRow(RowAccount, m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				bool bOK = TRUE;
				
				CAuditRecord atc;
				atc.SetSQL(RowAccount);								// set audit is for a pc line

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
						if (repoAccount.UpdateRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
						{
							return;
						}
						atc.AuditChanges(RowAccount, TRUE);	
					}
					break;

				case nBATCHUPDATE_REDEEM:
					bOK = UpdateRedeemPoints(RowAccount);
					if (TRUE == bOK)
					{
						if (repoAccount.UpdateRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
						{
							return;
						}
						atc.AuditChanges(RowAccount);
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
						atc.SetApplicationNo(nAppNo);
						atc.SetOperatorNo(Passwords.GetPasswordInUse());
						atc.SetComment(strComment);
						atc.SaveLineToArray(m_arrayAuditBuffer);
					}
					++m_nCount;
				}
			}

			if ((m_nCount % 10) == 0)
			{
				if (WorkingDlg.SetCaption2RecordsChecked(m_nCount) == TRUE)
				{
					return;
				}
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/

bool CSQLTranBatchUpdate::DeleteRow(CSQLRowAccountFull& RowAccount)
{
	CSQLTranDeleteAccount TranDelete;
	return TranDelete.DeleteExternal(RowAccount, m_strComment, m_pDatabase, m_arrayAuditBuffer);
}

/**********************************************************************/

bool CSQLTranBatchUpdate::UpdateRedeemPoints(CSQLRowAccountFull& RowAccount)
{
	CRedeemPoints pointsRedeemer;												// points redeem
	pointsRedeemer.m_bDisabled = FALSE;
	pointsRedeemer.m_nTrip = Server.GetRedeemPointsTrip();
	pointsRedeemer.m_dValue = Server.GetRedeemPointsValue();
	pointsRedeemer.m_nType = Server.GetRedeemPointsType();

	{
		CSQLRowGroup RowGroup;
		RowGroup.SetGroupNo(RowAccount.GetGroupNo());

		CSQLRepositoryGroup repoGroup;
		if (repoGroup.SelectRow(RowGroup, m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			// group specifc redeem points
			if (RowGroup.GetRedeemFlag() == TRUE)
			{
				pointsRedeemer.m_bDisabled = RowGroup.GetRedeemDisable();
				pointsRedeemer.m_nTrip = RowGroup.GetRedeemTrip();
				pointsRedeemer.m_dValue = RowGroup.GetRedeemValue();
				pointsRedeemer.m_nType = RowGroup.GetRedeemType();
			}
		}
	}

	if (pointsRedeemer.m_nType == nREDEEM_FAMILY)								// no redemption value for Family promotion
	{
		pointsRedeemer.m_bDisabled = TRUE;
	}

	if (pointsRedeemer.m_bDisabled == FALSE)
	{
		double dValueAdded = 0.0;
		int nPointsRedeemed = pointsRedeemer.Redeem(RowAccount.GetPoints(), dValueAdded);

		if (nPointsRedeemed > 0)
		{
			RowAccount.AddToPoints(-nPointsRedeemed);

			if ( System.GetEnablePurse2Flag() && ( Server.GetSpendBonusToPurse2Flag()))
			{
				RowAccount.AddToPurse2(dValueAdded);
			}
			else
			{
				RowAccount.AddToPurse1(dValueAdded);
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

	SQLStampOfferBuffer.WritePCAuditLines(m_pDatabase);

	CWorkingDlg dlgWorking("Deleting Auxillary Account Files", FALSE);
	dlgWorking.Create();

	for (int n = 0; n < m_arrayDeletedAccounts.GetSize(); n++)
	{
		if ((n % 10) == 0)
		{
			dlgWorking.SetCaption2RecordsOfTotal(n + 1, m_arrayDeletedAccounts.GetSize());
		}

		CSQLTranDeleteAccount TranDelete;
		CSQLRowAccountFull RowAccount = m_arrayDeletedAccounts.GetAt(n);
		TranDelete.NonSQLDeleteExternal(RowAccount, m_strComment);
	}
}

/**********************************************************************/

