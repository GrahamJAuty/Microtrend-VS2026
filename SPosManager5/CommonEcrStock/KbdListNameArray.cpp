/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
/**********************************************************************/
#include "KbdListNameArray.h"
/**********************************************************************/

CKbdListNameArray::CKbdListNameArray()
{
	m_bFakeRead = FALSE;
}

/**********************************************************************/

void CKbdListNameArray::Read(int nDbIdx)
{
	if (TRUE == m_bFakeRead)
	{
		ReloadFromMemory();
		m_bFakeRead = FALSE;
	}
	else
	{
		m_array.RemoveAll();

		CString strFilename = dbDatabase.GetFolderPathData(nDbIdx);
		strFilename += "\\listname.dat";

		CSSFile fileNames;
		if (fileNames.Open(strFilename, "rb") == FALSE)
			return;

		CCSV csv;
		CString strLabel = "";
		while (fileNames.Read(csv, 3))
		{
			CSortedIntAndStringByInt item;
			item.m_nItem = csv.GetInt(0);
			item.m_strData = csv.GetString(1);
			item.m_nData = csv.GetInt(2);
			m_array.Consolidate(item);
		}
	}
}

/**********************************************************************/

void CKbdListNameArray::Read( int nDbIdx, int nLocIdx, int nPriceSet )
{
	if (TRUE == m_bFakeRead)
	{
		ReloadFromMemory();
		m_bFakeRead = FALSE;
	}
	else
	{
		int nKbd = 0;

		switch (nPriceSet)
		{
		case 1:
			if (dbLocation.GetDbPriceSet1(nLocIdx) == 0)
				nKbd = dbLocation.GetPriceSetKeyboard1(nLocIdx);
			break;

		case 2:
			if (dbLocation.GetDbPriceSet2(nLocIdx) == 0)
				nKbd = dbLocation.GetPriceSetKeyboard2(nLocIdx);
			break;

		case 3:
			if (dbLocation.GetDbPriceSet3(nLocIdx) == 0)
				nKbd = dbLocation.GetPriceSetKeyboard3(nLocIdx);
			break;
		}

		if (0 != nKbd)
		{
			m_array.RemoveAll();

			CString strFilename = dbLocation.GetFolderPathDataKeyboard(nLocIdx, nKbd);
			strFilename += "\\listname.dat";

			CSSFile fileNames;
			if (fileNames.Open(strFilename, "rb") == FALSE)
				return;

			CCSV csv;
			CString strLabel = "";
			while (fileNames.Read(csv, 2))
			{
				CSortedIntAndStringByInt item;
				item.m_nItem = csv.GetInt(0);
				item.m_strData = csv.GetString(1);
				item.m_nData = csv.GetInt(2);
				m_array.Consolidate(item);
			}
		}
	}
}

/**********************************************************************/

void CKbdListNameArray::ReloadFromMemory()
{
	m_array.RemoveAll();

	for (int nListIdx = 0; nListIdx < SPOSKeyboardManager.GetListManager() -> GetListCount(); nListIdx++)
	{
		CSPOSKeyboardListRecord ListRecord;
		SPOSKeyboardManager.GetListManager() -> GetListRecord(nListIdx, ListRecord);

		CSortedIntAndStringByInt item;

		CCSV csv;
		csv.Add(ListRecord.GetListNo());
		item.m_nItem = csv.GetInt(0);

		item.m_strData = ListRecord.GetName();
		item.m_nData = ListRecord.GetOfferPluLinkFlag() ? 0 : 1;
		m_array.Consolidate(item);
	}
}

/**********************************************************************/

int CKbdListNameArray::GetListNo( int n )
{
	int nResult = 0;

	if ( ( n >= 0 ) && ( n < m_array.GetSize() ) )
	{
		CSortedIntAndStringByInt item;
		m_array.GetAt( n, item );
		nResult = item.m_nItem;
	}

	return nResult;
}

/**********************************************************************/

const char* CKbdListNameArray::GetListName( int n )
{
	m_strName = "";

	if ( ( n >= 0 ) && ( n < m_array.GetSize() ) )
	{
		CSortedIntAndStringByInt item;
		m_array.GetAt( n, item );
		m_strName = item.m_strData;
	}

	return m_strName;
}

/**********************************************************************/

bool CKbdListNameArray::GetOfferPluLinkFlag(int n)
{
	bool bResult = FALSE;

	if ((n >= 0) && (n < m_array.GetSize()))
	{
		CSortedIntAndStringByInt item;
		m_array.GetAt(n, item);
		bResult = (item.m_nData != 1);
	}

	return bResult;
}

/**********************************************************************/
