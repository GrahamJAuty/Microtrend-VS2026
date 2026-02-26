/**********************************************************************/
#include "SPOSKeyboardMultipleKeySelectInfo.h"
/**********************************************************************/

CSPOSKeyboardMultipleKeySelectInfo::CSPOSKeyboardMultipleKeySelectInfo(void)
{
	m_bGotSelection = FALSE;
	m_nListNo = 1;
	m_nPageNo = 1;
	m_nColStart = 1;
	m_nRowStart = 1;
	m_nCopyHeight = 1;
	m_nCopyWidth = 1;
	m_nStoredListNo = 1;
	m_nStoredPageNo = 1;
	ResetSelectedKeys();
}

/**********************************************************************/

void CSPOSKeyboardMultipleKeySelectInfo::Reset()
{
	ResetSelectedKeys();
	m_arrayKeyExtentsToCopy.RemoveAll();
	m_arrayKeyRecordsToCopy.RemoveAll();
	m_arrayStoredKeyExtentsToCopy.RemoveAll();
	m_arrayStoredKeyRecordsToCopy.RemoveAll();
}

/**********************************************************************/

void CSPOSKeyboardMultipleKeySelectInfo::ResetSelectedKeys()
{
	for ( int nCol = 0; nCol < 20; nCol++ )
		for ( int nRow = 0; nRow < 20; nRow++ )
			m_arraySelected[nCol][nRow] = FALSE;
}

/**********************************************************************/

bool CSPOSKeyboardMultipleKeySelectInfo::IsSelected( int nCol, int nRow )
{
	if ( ( TRUE == m_bGotSelection ) && ( nCol >= 0 ) && ( nCol < 20 ) && ( nRow >= 0 ) && ( nRow < 20 ) )
		return m_arraySelected[nCol][nRow];
	else
		return FALSE;
}

/**********************************************************************/

void CSPOSKeyboardMultipleKeySelectInfo::FlagSelectedKey( int nCol, int nRow, bool bFlag )
{
	if ( ( nCol >= 0 ) && ( nCol < 20 ) && ( nRow >= 0 ) && ( nRow < 20 ) )
		m_arraySelected[nCol][nRow] = bFlag;
}

/**********************************************************************/

bool CSPOSKeyboardMultipleKeySelectInfo::CheckPage( __int64 nListNo, int nPageNo )
{
	return ( ( TRUE == m_bGotSelection ) && ( m_nListNo == nListNo ) && ( m_nPageNo == nPageNo ) );
}

/**********************************************************************/

void CSPOSKeyboardMultipleKeySelectInfo::TagKey( __int64 nListNo, int nPageNo, CSPOSKeyboardKeyExtent& KeyExtent )
{
	ResetSelectedKeys();
	m_arrayKeyExtentsToCopy.RemoveAll();
	m_arrayKeyRecordsToCopy.RemoveAll();
	
	if ( ( TRUE == m_bGotSelection ) && ( nListNo == m_nListNo ) && ( nPageNo == m_nPageNo ) && ( m_KeyExtent1.m_nKeyNo == m_KeyExtent2.m_nKeyNo  ) )
	{
		m_KeyExtent2 = KeyExtent;
	}
	else
	{
		m_KeyExtent1 = KeyExtent;
		m_KeyExtent2 = KeyExtent;
	}

	m_bGotSelection = TRUE;
	m_nListNo = nListNo;
	m_nPageNo = nPageNo;
	
	m_nColStart = m_KeyExtent1.m_nColStart;
	m_nRowStart = m_KeyExtent1.m_nRowStart;
	
	int nColEnd = m_KeyExtent2.m_nColStart + m_KeyExtent2.m_nColSize - 1;
	int nRowEnd = m_KeyExtent2.m_nRowStart + m_KeyExtent2.m_nRowSize - 1;

	if ( m_nColStart > nColEnd )
	{
		int nTemp = m_nColStart;
		m_nColStart = nColEnd;
		nColEnd = nTemp;
		m_nCopyWidth = m_KeyExtent1.m_nColStart + m_KeyExtent1.m_nColSize - m_KeyExtent2.m_nColStart;
	}
	else
	{
		m_nCopyWidth = m_KeyExtent2.m_nColStart + m_KeyExtent2.m_nColSize - m_KeyExtent1.m_nColStart;
	}

	if ( m_nRowStart > nRowEnd )
	{
		int nTemp = m_nRowStart;
		m_nRowStart = nRowEnd;
		nRowEnd = nTemp;
		m_nCopyHeight = m_KeyExtent1.m_nRowStart + m_KeyExtent1.m_nRowSize - m_KeyExtent2.m_nRowStart;
	}
	else
	{
		m_nCopyHeight = m_KeyExtent2.m_nRowStart + m_KeyExtent2.m_nRowSize - m_KeyExtent1.m_nRowStart;
	}

	for ( int nPos = 0; nPos < SPOSKeyboardManager.GetKeymapTable() -> GetKeyCount(); nPos++ )
	{
		CSPOSKeyboardKeyExtent KeyExtent;
		SPOSKeyboardManager.GetKeymapTable() -> GetKeyInfoByIndex( nPos, KeyExtent );

		bool bMatch = TRUE;

		if ( KeyExtent.m_nColStart > nColEnd )
			bMatch = FALSE;
		else if ( KeyExtent.m_nColStart + KeyExtent.m_nColSize - 1 < m_nColStart )
			bMatch = FALSE;
		else if ( KeyExtent.m_nRowStart > nRowEnd )
			bMatch = FALSE;
		else if ( KeyExtent.m_nRowStart + KeyExtent.m_nRowSize - 1 < m_nRowStart )
			bMatch = FALSE;
		
		if ( TRUE == bMatch )
		{
			for ( int nCol = KeyExtent.m_nColStart; nCol < KeyExtent.m_nColStart + KeyExtent.m_nColSize; nCol++ )
				for ( int nRow = KeyExtent.m_nRowStart; nRow < KeyExtent.m_nRowStart + KeyExtent.m_nRowSize; nRow++ )
					FlagSelectedKey( nCol, nRow, TRUE );

			m_arrayKeyExtentsToCopy.Add( KeyExtent );
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardMultipleKeySelectInfo::BuildCopyBuffer()
{
	m_arrayKeyRecordsToCopy.RemoveAll();

	for ( int n = 0; n < m_arrayKeyExtentsToCopy.GetSize(); n++ )
	{
		CSPOSKeyboardKeyRecord KeyRecord;

		int nKeyIdx;
		if ( SPOSKeyboardManager.GetListManager() -> FindKey( m_nListNo, m_nPageNo, m_arrayKeyExtentsToCopy[n].m_nKeyNo, nKeyIdx ) == TRUE )
			SPOSKeyboardManager.GetListManager() -> GetKeyRecord( nKeyIdx, KeyRecord );

		m_arrayKeyRecordsToCopy.Add( KeyRecord );
	}
}

/**********************************************************************/

void CSPOSKeyboardMultipleKeySelectInfo::GetCopyBufferKey(int nIdx, CSPOSKeyboardKeyExtent& KeyExtent, CSPOSKeyboardKeyRecord& KeyRecord)
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayKeyExtentsToCopy.GetSize() ) )
		KeyExtent = m_arrayKeyExtentsToCopy.GetAt( nIdx );

	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayKeyRecordsToCopy.GetSize() ) )
		KeyRecord = m_arrayKeyRecordsToCopy.GetAt( nIdx );
}

/**********************************************************************/

void CSPOSKeyboardMultipleKeySelectInfo::RememberSettings()
{
	m_nStoredListNo = m_nListNo;
	m_nStoredPageNo = m_nPageNo;
	m_StoredKeyExtent1 = m_KeyExtent1;
	m_StoredKeyExtent2 = m_KeyExtent2;
	m_nStoredColStart = m_nColStart;
	m_nStoredRowStart = m_nRowStart;
	m_nStoredCopyHeight = m_nCopyHeight;
	m_nStoredCopyWidth = m_nCopyWidth;

	for ( int x = 0; x < 20; x++ )
		for ( int y = 0; y < 20; y++ )
			m_arrayStoredSelected[x][y] = m_arraySelected[x][y];

	m_arrayStoredKeyExtentsToCopy.RemoveAll();
	for ( int n = 0; n < m_arrayKeyExtentsToCopy.GetSize(); n++ )
		m_arrayStoredKeyExtentsToCopy.Add( m_arrayKeyExtentsToCopy.GetAt(n) );

	m_arrayStoredKeyRecordsToCopy.RemoveAll();
	for ( int n = 0; n < m_arrayKeyRecordsToCopy.GetSize(); n++ )
		m_arrayStoredKeyRecordsToCopy.Add( m_arrayKeyRecordsToCopy.GetAt(n) );
}

/**********************************************************************/

void CSPOSKeyboardMultipleKeySelectInfo::RestoreSettings()
{
	m_nListNo = m_nStoredListNo;
	m_nPageNo = m_nStoredPageNo;
	m_KeyExtent1 = m_StoredKeyExtent1;
	m_KeyExtent2 = m_StoredKeyExtent2;
	m_nColStart = m_nStoredColStart;
	m_nRowStart = m_nStoredRowStart;
	m_nCopyHeight = m_nStoredCopyHeight;
	m_nCopyWidth = m_nStoredCopyWidth;

	for ( int x = 0; x < 20; x++ )
		for ( int y = 0; y < 20; y++ )
			m_arraySelected[x][y] = m_arrayStoredSelected[x][y];

	m_arrayKeyExtentsToCopy.RemoveAll();
	for ( int n = 0; n < m_arrayStoredKeyExtentsToCopy.GetSize(); n++ )
		m_arrayKeyExtentsToCopy.Add( m_arrayStoredKeyExtentsToCopy.GetAt(n) );

	m_arrayKeyRecordsToCopy.RemoveAll();
	for ( int n = 0; n < m_arrayStoredKeyRecordsToCopy.GetSize(); n++ )
		m_arrayKeyRecordsToCopy.Add( m_arrayStoredKeyRecordsToCopy.GetAt(n) );
}

/**********************************************************************/

