/**********************************************************************/
#include "WalletStampRulePluMapper.h"
/**********************************************************************/

void CWalletStampRulePluMapper::BuildMap()
{
	m_PluMap.RemoveAll();
	m_arrayUsedOfferGroups.RemoveAll();

	CDataManagerInfo info;
	DataManager.ReopenWalletStampRule(info);
	DataManager.ReopenOfferGroup(info);
	DataManager.ReopenOfferGroupEntry(info);

	for (int n = 0; n < DataManager.WalletStampRule.GetSize(); n++)
	{
		CWalletStampRuleCSVRecord Rule;
		DataManager.WalletStampRule.GetAt(n, Rule);

		int nOfferGroupNo = Rule.GetOfferGroupNo();

		if (nOfferGroupNo <= 0)
		{
			continue;
		}

		{
			CSortedIntItem item;
			item.m_nItem = nOfferGroupNo;
			int nPos = 0;
			if (m_arrayUsedOfferGroups.Find(item, nPos) == TRUE)
			{
				continue;
			}

			m_arrayUsedOfferGroups.InsertAt(nPos, item);
		}

		int nIdx1, nIdx2;
		DataManager.OfferGroupEntry.GetGroupSubRange(nOfferGroupNo, OFFERENTRY_PLU, nIdx1, nIdx2);

		for (int n = nIdx1; n <= nIdx2; n++)
		{
			COfferGroupEntriesCSVRecord entry;
			DataManager.OfferGroupEntry.GetAt(n, entry);

			__int64 nSharpPluNo = 0;
			bool bModifiable = FALSE;
			CMinimumSPOSVersion Version;
			entry.CheckMinimumSPOSVersion(Version, nSharpPluNo, bModifiable);

			if (FALSE == bModifiable)
			{
				CSortedIntByInt64 item;
				item.m_nKey = nSharpPluNo;
				item.m_nVal = Rule.GetRuleID();

				int nPos = 0;
				if (m_PluMap.Find(item, nPos) == FALSE)
				{
					m_PluMap.InsertAt(nPos, item);
				}
			}
			else
			{
				for (int nMod = 0; nMod <= 9; nMod++)
				{
					if (entry.GetSingleModifierFlag(nMod) == TRUE)
					{
						CSortedIntByInt64 item;
						item.m_nKey = nSharpPluNo + nMod;
						item.m_nVal = Rule.GetRuleID();

						int nPos = 0;
						if (m_PluMap.Find(item, nPos) == FALSE)
						{
							m_PluMap.InsertAt(nPos, item);
						}
					}
				}
			}
		}
	}
}

/**********************************************************************/

int CWalletStampRulePluMapper::GetRuleIDByPlu(__int64 nPluNo)
{
	int nResult = 0;

	CSortedIntByInt64 item;
	item.m_nKey = nPluNo;

	int nPos = 0;
	if (m_PluMap.Find(item, nPos) == TRUE)
	{
		m_PluMap.GetAt(nPos, item);
		nResult = item.m_nVal;
	}

	return nResult;
}

/**********************************************************************/
