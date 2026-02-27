//*********************************************************************
#include "TransactionCache.h"
//*********************************************************************

bool CTransactionCache::CheckCache ( CString strTransaction )
{
	strTransaction.MakeUpper();

	CSortedStringByString item;
	item.m_strKey = strTransaction;

	int nPos = 0;
	return m_arrayTransactions.Find(item, nPos);
}

//*******************************************************************

void CTransactionCache::AddToCache( CString strTransaction)
{
	strTransaction.MakeUpper();

	CSortedStringByString item;
	item.m_strKey = strTransaction;
	item.m_strData.Format("%14.14I64d", GetTickCount64());
	m_arrayTransactions.Consolidate(item);
}

//*******************************************************************

void CTransactionCache::PurgeCache()
{
	ULONGLONG time = GetTickCount64();
	if (time > 20000)
	{
		time -= 20000;

		CString strTime = "";
		strTime.Format("%14.14I64d", time);

		for (int n = 0; n < m_arrayTransactions.GetSize(); n++)
		{
			CSortedStringByString item;
			m_arrayTransactions.GetAt(n, item);

			if (item.m_strData < strTime)
			{
				m_arrayTransactions.RemoveAt(n);
				n--;
			}
		}
	}
}

//*******************************************************************

void CTransactionCache::WriteCacheToFile()
{
	/*
	CSSFile fileCache;
	if (fileCache.Open(".\\cache.txt", "ab") == TRUE)
	{
		fileCache.WriteLine("*****");

		for (int n = 0; n < m_arrayTransactions.GetSize(); n++)
		{
			CSortedStringByString item;
			m_arrayTransactions.GetAt(n, item);

			CString str = item.m_strKey;
			str += "   ";
			str += item.m_strData;

			fileCache.WriteLine(str);
		}
	}
	*/
}

//*******************************************************************
