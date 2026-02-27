//**********************************************************************
#include "SQLPurchaseControlCache.h"
//**********************************************************************
#include "SQLDefines.h"
//**********************************************************************
#include "SQLTable_PurchaseControlRule/SQLRepositoryPurchaseControlRule.h"
//**********************************************************************

bool CSQLPurchaseControlCache::GetPurchaseControlRow(int nRuleID, CSQLRowPurchaseControlRule& Row)
{
	bool bResult = FALSE;

	if (m_arrayStatus.GetSize() == m_arrayRules.GetSize())
	{
		int nPos = 0;
		CSortedIntByInt item;
		item.m_nKey = nRuleID;
		if (m_arrayStatus.Find(item, nPos) == FALSE)
		{
			item.m_nVal = 0;
			m_arrayStatus.InsertAt(nPos, item);
			m_arrayRules.InsertAt(nPos, Row);
		}
		else
		{
			m_arrayStatus.GetAt(nPos, item);
			Row = m_arrayRules.GetAt(nPos);
		}

		switch (item.m_nVal)
		{
		case 0:
			{
				Row.SetRuleID(nRuleID);
				CSQLRepositoryPurchaseControlRule RepoRule;
				if (RepoRule.SelectRow(Row, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
				{
					m_arrayRules.SetAt(nPos, Row);

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

void CSQLPurchaseControlCache::InsertRow(CSQLRowPurchaseControlRule& Row)
{
	if (m_arrayStatus.GetSize() == m_arrayRules.GetSize())
	{
		int nPos = 0;
		CSortedIntByInt item;
		item.m_nKey = Row.GetRuleID();
		if (m_arrayStatus.Find(item, nPos) == FALSE)
		{
			item.m_nVal = 1;
			m_arrayStatus.InsertAt(nPos, item);
			m_arrayRules.InsertAt(nPos, Row);
		}
	}
}

/**********************************************************************/