/**********************************************************************/
#include "..\SPosLoyalty4Shared\AuditReportTexts.h"
#include "..\SPosLoyalty4Shared\SQLStampOfferBuffer.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_CCNo\SQLRepositoryCCNo.h"
#include "..\SPosLoyalty4Shared\SQLTable_OfferStamp\SQLRepositoryOfferStamp.h"
/**********************************************************************/
#include "SQLTranStampManualAdd.h"
/**********************************************************************/

void CSQLTranStampManualAdd::DoWork( CSQLRowAccountFull& RowAccount, CSQLRowOfferStamp& OfferStampNew, CString strOfferName)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		int nNewStamps = OfferStampNew.GetStampCount();
		
		CSQLRepositoryOfferStamp repoOffer;
		if (repoOffer.SelectRow(OfferStampNew, m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			OfferStampNew.SetStampCount(OfferStampNew.GetStampCount() + nNewStamps);

			if (repoOffer.UpdateRow(OfferStampNew, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}
			else
			{
				SQLStampOfferBuffer.BufferPCAction(
					APPNO_SO_MADD,
					RowAccount,
					OfferStampNew,
					strOfferName,
					nNewStamps,
					OfferStampNew.GetStampCount()
				);
			}
		}
		else
		{
			if (repoOffer.InsertRow(OfferStampNew, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}
			else
			{
				SQLStampOfferBuffer.BufferPCAction(
					APPNO_SO_MADD,
					RowAccount,
					OfferStampNew,
					strOfferName,
					nNewStamps,
					nNewStamps
				);
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/

void CSQLTranStampManualAdd::AfterTransaction()
{
	SQLStampOfferBuffer.WritePCAuditLines(m_pDatabase);
}

/**********************************************************************/
