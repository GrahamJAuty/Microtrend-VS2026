//**********************************************************************
#include "SQLStampOfferCache.h"
//**********************************************************************
#include "SQLDefines.h"
//**********************************************************************
#include "SQLTable_StampOfferInfo/SQLRepositoryStampOfferInfo.h"
//**********************************************************************

bool CSQLStampOfferCache::GetStampOfferRow(int nStampOfferID, CSQLRowStampOfferInfo& Row)
{
	bool bResult = FALSE;

	if (m_arrayStatus.GetSize() == m_arrayOffers.GetSize())
	{
		int nPos = 0;
		CSortedIntByInt item;
		item.m_nKey = nStampOfferID;
		if (m_arrayStatus.Find(item, nPos) == FALSE)
		{
			item.m_nVal = 0;
			m_arrayStatus.InsertAt(nPos, item);
			m_arrayOffers.InsertAt(nPos, Row);
		}
		else
		{
			m_arrayStatus.GetAt(nPos, item);
			Row = m_arrayOffers.GetAt(nPos);
		}

		switch (item.m_nVal)
		{
		case 0:
			{
				CSQLRepositoryStampOfferInfo repoOffer;
				Row.SetStampOfferID(nStampOfferID);
				if (repoOffer.SelectRow(Row, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
				{
					m_arrayOffers.SetAt(nPos, Row);

					item.m_nVal = 1;
					m_arrayStatus.SetAt(nPos, item);

					bResult = TRUE;
				}
				else
				{
					item.m_nVal = 2;
					m_arrayStatus.SetAt(nPos, item);
				}
			}
			break;

		case 1:
			bResult = TRUE;
			break;

		case 2:
		default:
			bResult = FALSE;
			break;
		}
	}

	return bResult;
}

/**********************************************************************/

void CSQLStampOfferCache::InsertRow(CSQLRowStampOfferInfo& Row)
{
	if (m_arrayStatus.GetSize() == m_arrayOffers.GetSize())
	{
		int nPos = 0;
		CSortedIntByInt item;
		item.m_nKey = Row.GetStampOfferID();
		if (m_arrayStatus.Find(item, nPos) == FALSE)
		{
			item.m_nVal = 1;
			m_arrayStatus.InsertAt(nPos, item);
			m_arrayOffers.InsertAt(nPos, Row);
		}
	}
}

/**********************************************************************/