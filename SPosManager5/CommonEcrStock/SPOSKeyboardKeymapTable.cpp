/**********************************************************************/
#include "SPOSKeyboardDefines.h"
/**********************************************************************/
#include "SPOSKeyboardKeymapTable.h"
/**********************************************************************/

CSPOSKeyboardKeymapTable::CSPOSKeyboardKeymapTable()
{
	m_bGotKeymap = FALSE;
	m_nColumnCount = 0;
	m_nRowCount = 0;
	m_nListWidth = 0;
	m_nListHeight = 0;
}

/**********************************************************************/

CSPOSKeyboardKeymapTable::~CSPOSKeyboardKeymapTable()
{
}

/**********************************************************************/

void CSPOSKeyboardKeymapTable::Reset()
{
	m_KeyExtentMap.RemoveAll();
	m_SingleKeyMap.RemoveAll();
}

/**********************************************************************/

int CSPOSKeyboardKeymapTable::GetSharpKeyPos( int nCol, int nRow )
{
	if ( nCol < 0 || nCol >= m_nColumnCount || nRow < 0 || nRow >= m_nRowCount )
		return 0;

	return ( nCol ) + ( nRow * m_nRowCount ) + 1;
}

/**********************************************************************/

int CSPOSKeyboardKeymapTable::GetCol ( int nSharpKeyPos )
{
	if ( nSharpKeyPos < 1 || nSharpKeyPos > POS_KEY_COUNT || m_nColumnCount == 0 )
		return 0;

	return ( ( nSharpKeyPos - 1 ) % m_nColumnCount );
}

/**********************************************************************/

int CSPOSKeyboardKeymapTable::GetRow ( int nSharpKeyPos )
{
	if ( nSharpKeyPos < 1 || nSharpKeyPos > POS_KEY_COUNT || m_nRowCount == 0 )
		return 0;

	return ( ( nSharpKeyPos - 1 ) / m_nColumnCount );
}

/**********************************************************************/

void CSPOSKeyboardKeymapTable::BuildKeymap( __int64 nListNo, int nPageNo, int nZoomType )
{
	if ( m_bGotKeymap )
		return;

	switch( nZoomType )
	{
	case 1:		m_dZoomFactor = 0.75;	break;
	case 3:		m_dZoomFactor = 2.0;	break;
	default:	m_dZoomFactor = 1.0;	break;
	}

	m_SingleKeyMap.RemoveAll();

	SPOSKeyboardManager.GetListManager() -> GetKeyExtentMap( nListNo, nPageNo, m_KeyExtentMap );
	
	int nListIdx;
	if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == FALSE )
		return;

	CSPOSKeyboardListRecord ListRecord;
	SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );
	m_nRowCount = ListRecord.GetRowCount();
	m_nColumnCount = ListRecord.GetColumnCount();
	
	if ( ( m_nColumnCount == 0 ) || ( m_nRowCount == 0 ) )
		return;

	int nKeyWidthDsp, nKeyHeightDsp, nKeyXSpace, nKeyYSpace;
	ListRecord.GetKeyPixelSize( nKeyWidthDsp, nKeyHeightDsp, nKeyXSpace, nKeyYSpace );

	//BLANK SPACE BETWEEN KEYS		
	int nXSpaceDsp = nKeyWidthDsp + nKeyXSpace;
	int nYSpaceDsp = nKeyHeightDsp + nKeyYSpace;

	//SIZE OF LIST IN PIXELS
	m_nListWidth = ( ( nKeyWidthDsp + nKeyXSpace ) * m_nColumnCount ) - nKeyXSpace;
	m_nListHeight = ( ( nKeyHeightDsp + nKeyYSpace ) * m_nRowCount ) - nKeyYSpace;

	//ASSUMED ORIGIN
	int nXOriginDsp = 40 + SPOSKeyboardManager.GetLayoutOptions() -> GetXMarginDisplay();
	int nYOriginDsp = 40;

	switch( nZoomType )
	{
	case 3:
		nXSpaceDsp += nKeyWidthDsp;
		nYSpaceDsp += nKeyHeightDsp;
		nKeyWidthDsp *= 2;
		nKeyHeightDsp *= 2;
		break;

	case 1:
		nXSpaceDsp -= ( nKeyWidthDsp / 4 );
		nYSpaceDsp -= ( nKeyHeightDsp / 4 );
		nKeyWidthDsp -= ( nKeyWidthDsp / 4 );
		nKeyHeightDsp -= ( nKeyHeightDsp / 4 );
		break;
	}

	//CREATE SINGLE KEY MAP
	int nSharpKeyPos = 1;
	for ( int nRow = 0; nRow < m_nRowCount; nRow++ )
	{
		for ( int nCol = 0; nCol < m_nColumnCount; nCol++ )
		{
			int nUpperLeftX = nXOriginDsp + ( nXSpaceDsp * nCol );
			int nUpperLeftY = nYOriginDsp + ( nYSpaceDsp * nRow );
			int nLowerRightX = nXOriginDsp + ( nXSpaceDsp * nCol ) + nKeyWidthDsp - 1;
			int nLowerRightY = nYOriginDsp + ( nYSpaceDsp * nRow ) + nKeyHeightDsp - 1;
	
			CSPOSKeyboardKeyExtent KeyExtent;
			KeyExtent.m_nKeyNo = nSharpKeyPos++;
			KeyExtent.m_rectDisplay.SetRect( nUpperLeftX, nUpperLeftY, nLowerRightX, nLowerRightY );
			m_SingleKeyMap.Add( KeyExtent );
		}
	}

	//GET BIGGER KEY POSITIONS
	for ( int n = 0; n < m_KeyExtentMap.GetSize(); n++ )
	{
		int nColStart = m_KeyExtentMap[n].m_nColStart;
		int nRowStart = m_KeyExtentMap[n].m_nRowStart;
		int nColEnd = nColStart + m_KeyExtentMap[n].m_nColSize - 1;
		int nRowEnd = nRowStart + m_KeyExtentMap[n].m_nRowSize - 1;

		int nUpperLeftX = nXOriginDsp + ( nXSpaceDsp * nColStart );
		int nUpperLeftY = nYOriginDsp + ( nYSpaceDsp * nRowStart );
		int nLowerRightX = nXOriginDsp + ( nXSpaceDsp * nColEnd ) + nKeyWidthDsp - 1;
		int nLowerRightY = nYOriginDsp + ( nYSpaceDsp * nRowEnd ) + nKeyHeightDsp - 1;
			
		m_KeyExtentMap[n].m_rectDisplay.SetRect( nUpperLeftX, nUpperLeftY, nLowerRightX, nLowerRightY );
	}
	
	m_bGotKeymap = TRUE;
}

/**********************************************************************/

int CSPOSKeyboardKeymapTable::GetKeyAtPoint ( CPoint point )
{
	for ( int n = 0; n < m_KeyExtentMap.GetSize(); n++ )
	{
		CRect rect = m_KeyExtentMap[n].m_rectDisplay;
		rect.NormalizeRect();

		if ( rect.PtInRect ( point ) == TRUE )
			return m_KeyExtentMap[n].m_nKeyNo;
	}
	return 0;
}

/**********************************************************************/

int CSPOSKeyboardKeymapTable::GetSharpKeyPosAtPoint ( CPoint point )
{
	for ( int n = 0; n < m_SingleKeyMap.GetSize(); n++ )
	{
		CRect rect = m_SingleKeyMap[n].m_rectDisplay;
		rect.NormalizeRect();

		if ( rect.PtInRect ( point ) == TRUE )
			return m_SingleKeyMap[n].m_nKeyNo;
	}
	return 0;
}

/**********************************************************************/

void CSPOSKeyboardKeymapTable::GetKeyInfoBySharpKeyPos ( int nSharpKeyPos, CSPOSKeyboardKeyExtent& KeyExtent )
{	
	for ( int n = 0; n < m_KeyExtentMap.GetSize(); n++ )
	{
		if ( m_KeyExtentMap[n].m_nKeyNo == nSharpKeyPos )
		{
			KeyExtent = m_KeyExtentMap[n];
			return;
		}
	}

	KeyExtent.m_nKeyNo = 0;
	KeyExtent.m_nRowStart = 0;
	KeyExtent.m_nColStart = 0;
	KeyExtent.m_nRowSize = 0;
	KeyExtent.m_nColSize = 0;	
	KeyExtent.m_rectDisplay.SetRectEmpty();
}

/**********************************************************************/

void CSPOSKeyboardKeymapTable::GetKeyInfoByIndex( int nPos, CSPOSKeyboardKeyExtent& KeyExtent )
{
	if ( nPos >= 0 && nPos < m_KeyExtentMap.GetSize() )
		KeyExtent = m_KeyExtentMap.GetAt( nPos );
	else
	{
		KeyExtent.m_nKeyNo = 0;
		KeyExtent.m_nRowStart = 0;
		KeyExtent.m_nColStart = 0;
		KeyExtent.m_nRowSize = 0;
		KeyExtent.m_nColSize = 0;
		KeyExtent.m_rectDisplay.SetRectEmpty();
	}
}

/**********************************************************************/
