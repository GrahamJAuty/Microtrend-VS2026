/**********************************************************************/
#include "..\SPosLoyalty4Shared\AuditReportTexts.h"
#include "..\SPosLoyalty4Shared\SQLStampOfferBuffer.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_CCNo\SQLRepositoryCCNo.h"
#include "..\SPosLoyalty4Shared\SQLTable_OfferStamp\SQLRepositoryOfferStamp.h"
/**********************************************************************/
#include "SQLTranStampManualEdit.h"
/**********************************************************************/

CSQLTranStampManualEdit::CSQLTranStampManualEdit(int nAppNo) : CSQLTranBase()
{
	m_nAppNo = nAppNo;
}

/**********************************************************************/

void CSQLTranStampManualEdit::DoWork(CSQLRowAccountFull& RowAccount, CSQLRowOfferStamp& OfferStampOld, CSQLRowOfferStamp& OfferStampNew, CString strOfferName)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		bool bSameRow = TRUE;
		bSameRow &= (OfferStampOld.GetUserID() == OfferStampNew.GetUserID());
		bSameRow &= (OfferStampOld.GetStampOfferID() == OfferStampNew.GetStampOfferID());
		bSameRow &= (OfferStampOld.GetEdition() == OfferStampNew.GetEdition());
		bSameRow &= (OfferStampOld.GetAwardDate() == OfferStampNew.GetAwardDate());
		bSameRow &= (OfferStampOld.GetExpireDate() == OfferStampNew.GetExpireDate());

		if (FALSE == bSameRow)
		{
			CSQLRepositoryOfferStamp repoOffer;

			if (repoOffer.DeleteRow(OfferStampOld, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}
			else
			{
				SQLStampOfferBuffer.BufferPCAction(
					m_nAppNo,
					RowAccount,
					OfferStampOld,
					strOfferName,
					OfferStampOld.GetStampCount() * (-1),
					0
				);
			}

			int nNewStamps = OfferStampNew.GetStampCount();
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
						m_nAppNo,
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
						m_nAppNo,
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
}

/**********************************************************************/

void CSQLTranStampManualEdit::AfterTransaction()
{
	SQLStampOfferBuffer.WritePCAuditLines(m_pDatabase);
}

/**********************************************************************/
