//*********************************************************************
#include "EmptyLogFileCache.h"
//*********************************************************************

bool CEmptyLogFileCache::CheckCache ( CSortedIntAndStringItem& item )
{
	item.m_strItem.MakeUpper();
	
	int nPos = 0;
	return m_arrayTransactions.Find(item, nPos);
}

//*******************************************************************

void CEmptyLogFileCache::AddToCache(CSortedIntAndStringItem& item)
{
	item.m_strItem.MakeUpper();
	m_arrayTransactions.Consolidate(item);
}

//*******************************************************************

void CEmptyLogFileCache::RemoveFromCache(CSortedIntAndStringItem& item)
{
	item.m_strItem.MakeUpper();
	
	int nPos = 0;
	if (m_arrayTransactions.Find(item, nPos) == TRUE)
	{
		m_arrayTransactions.RemoveAt(nPos);
	}
}

//*******************************************************************

void CEmptyLogFileCache::WriteCacheToFile()
{
	/*
	CSSFile fileCache;
	if (fileCache.Open(".\\cache2.txt", "ab") == TRUE)
	{
		fileCache.WriteLine("*****");

		for (int n = 0; n < m_arrayTransactions.GetSize(); n++)
		{
			CSortedIntAndStringItem item;
			m_arrayTransactions.GetAt(n, item);

			CString str = "";
			str.Format("%d  %s",
				item.m_nItem,
				(const char*)item.m_strItem);

			fileCache.WriteLine(str);
		}
	}
	*/
}

//*******************************************************************
