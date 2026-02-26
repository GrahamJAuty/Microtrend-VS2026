/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLResultInfo.h"
#include "..\SPosLoyalty4Shared\SQLStampOfferBuffer.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_StaffGiftException\SQLRepositoryStaffGiftException.h"
/**********************************************************************/
#include "EposLoyaltyFile.h"
#include "SQLStaffAccountBuffer.h"
#include "SQLTranSaveLogUpdates.h"
/**********************************************************************/

void CSQLTranSaveLogUpdates::DoWork(CSQLRowAccountFull& RowAccount)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		//MAKE SURE CUSTOMER ACCOUNT HAS NOT BEEN CHANGED ELSEWHERE 
		
		CSQLRowAccountFull RowCheck;
		RowCheck.SetUserID(RowAccount.GetUserID());

		CSQLRepositoryAccount repoAccount;

		if (repoAccount.SelectRow(RowCheck, m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			if (RowAccount.CompareRowVersion(RowCheck) == FALSE)
			{
				return;
			}
		}

		//UPDATE CUSTOMER ACCOUNT

		if (repoAccount.UpdateRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}
		
		//CHECK STAFF GIFTS

		for (int n = 0; n < SQLStaffAccountBuffer.GetStaffGiftCount(); n++)
		{
			if (SQLStaffAccountBuffer.ApplyStaffGift(n, m_pDatabase) == FALSE)
			{
				return;
			}
		}

		//CHECK OFFER STAMP AWARDS

		for (int n = 0; n < SQLStampOfferBuffer.GetOfferStampAwardCount(); n++)
		{
			if (SQLStampOfferBuffer.ApplyOfferStampAward(n, m_pDatabase) == FALSE)
			{
				return;
			}
		}

		//CHECK OFFER STAMP REDEMPTION
		int nLastOfferID = -1;
		__int64 nLastPluNo = -1;
		for (int n = 0; n < SQLStampOfferBuffer.GetOfferStampRedeemCount(); n++)
		{
			if (SQLStampOfferBuffer.ApplyOfferStampRedeem(n, nLastOfferID, nLastPluNo, m_pDatabase) == FALSE)
			{
				return;
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/
