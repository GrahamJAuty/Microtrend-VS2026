/**********************************************************************/
#include <math.h>
/**********************************************************************/
#include "SPOSKeyboardDefines.h"
/**********************************************************************/
#include "SPOSKeyboardListRecord.h"
/**********************************************************************/

CSPOSKeyboardListRecord::CSPOSKeyboardListRecord()
{
	Reset();
}

/**********************************************************************/

void CSPOSKeyboardListRecord::Reset()
{
	m_nListNo = 1;
	m_strName = "";
	m_nFormat = LIST_FORMAT_3x3;
	m_bStaydown = FALSE;
	m_nNextList = 0;
	m_nMinItems = 0;
	m_nMaxItems = 1;
	m_nPageCount = 1;
	m_nListType = LISTTYPE_ITEM_STANDARD;
	m_nListActionType = LISTACTION_NORMAL;
	m_nTableNoLo = 0;
	m_nTableNoHi = 0;
	m_nTableArea = 0;
	m_bPriority = TRUE;
	m_nStackMode = 0;

	m_nForeground86 = 1;
	m_nBackground86 = 1;
	m_bBorder86 = FALSE;
	m_nFontSize86 = 16;
	m_bBold86 = FALSE;
	m_nWidth86 = 0;
	m_nHeight86 = 0;

	m_nTotalScreenWidth = 1024;
	m_nTotalScreenHeight = 768;

	m_bAllowChange = TRUE;
	m_bOfferPluLink = TRUE;
}

/**********************************************************************/

bool CSPOSKeyboardListRecord::MatchViewModeType( int nMode )
{
	switch( nMode )
	{
	case VIEWMODE_LIST_ITEM_STANDARD:		return ( m_nListType == LISTTYPE_ITEM_STANDARD );
	case VIEWMODE_LIST_ITEM_LARGE:			return ( m_nListType == LISTTYPE_ITEM_LARGE );
	case VIEWMODE_LIST_FUNCTION_STANDARD:	return ( m_nListType == LISTTYPE_FUNCTION_STANDARD );
	case VIEWMODE_LIST_FUNCTION_LARGE:		return ( m_nListType == LISTTYPE_FUNCTION_LARGE );
	case VIEWMODE_LIST_FUNCTION_BOTTOM:		return ( m_nListType == LISTTYPE_FUNCTION_BOTTOM );
	}
	return FALSE;
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetListNo( __int64 n )
{
	if ( n >= 1 && n <= MAX_LIST_NO )
		m_nListNo = n;
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetName( const char* szName )
{
	m_strName = szName;
	::TrimSpaces ( m_strName, FALSE );
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetNextList( int n )
{
	if ( n >= 0 && n <= MAX_LIST_NO )
		m_nNextList = n;
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetMinItems( int n )
{
	if ( n >= 0 && n <= 99 )
		m_nMinItems = n;
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetMaxItems( int n )
{
	if ( n >= 0 && n <= 99 )
		m_nMaxItems = n;
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetPageCount( int n )
{
	if ( n >= 1 && n <= MAX_LIST_PAGES )
		m_nPageCount = n;
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetListType( int n )
{
	switch( n )
	{
	case LISTTYPE_ITEM_STANDARD:
	case LISTTYPE_ITEM_LARGE:
	case LISTTYPE_FUNCTION_STANDARD:
	case LISTTYPE_FUNCTION_LARGE:
	case LISTTYPE_FUNCTION_BOTTOM:
		m_nListType = n;
		break;
	}
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetForeground86( int n )
{
	if ( n >= 1 && n <= 64 )
		m_nForeground86 = n;
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetBackground86( int n )
{
	if ( n >= 0 && n <= 64 )
		m_nBackground86 = n;
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetFontSize86( int n )
{
	switch( n )
	{
	case 8:
	case 10:
	case 11:
	case 12:
	case 14:
	case 16:
	case 20:
	case 24:
		m_nFontSize86 = n;
		break;
	}
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetWidth86( int n )
{
	if ( n >= 0 && n <= 99 )
		m_nWidth86 = n; 
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetHeight86( int n )
{
	if ( n >= 0 && n <= 99 )
		m_nHeight86 = n; 
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetListActionType( int n )
{
	switch( n )
	{
	case LISTACTION_NORMAL:
	case LISTACTION_SERVER:
	case LISTACTION_TABLE:
		m_nListActionType = n;
		break;
	}
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetTableNoLo( int n )
{
	if ( ( n >= 0 ) && ( n <= 999 ) )
		m_nTableNoLo = n;
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetTableNoHi( int n )
{
	if ( ( n >= 0 ) && ( n <= 999 ) )
		m_nTableNoHi = n;
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetTableArea( int n )
{
	if ( ( n >= 0 ) && ( n <= 99 ) )
		m_nTableArea = n;
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetStackMode( int n )
{
	if ( ( n >= 0 ) && ( n <= 1 ) )
		m_nStackMode = n;
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetDefaultListFormat()
{
	switch( m_nListType )
	{
	case LISTTYPE_ITEM_STANDARD:
		SetFormat( LIST_FORMAT_1x5 );
		break;

	case LISTTYPE_ITEM_LARGE:
		SetFormat( LIST_FORMAT_2x1 );
		break;

	case LISTTYPE_FUNCTION_STANDARD:
		SetFormat( LIST_FORMAT_5x3 );
		break;

	case LISTTYPE_FUNCTION_LARGE:
		SetFormat( LIST_FORMAT_5x6 );
		break;

	case LISTTYPE_FUNCTION_BOTTOM:
		SetFormat( LIST_FORMAT_1x10 );
		break;
	}
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetFormat( int nRows, int nCols )
{
	SetFormat( ( nRows * 100 ) + nCols );
}

/**********************************************************************/

void CSPOSKeyboardListRecord::SetFormat( int nFormat )
{
	if ( ValidateFormat( nFormat ) == TRUE )
		m_nFormat = nFormat;
}

/**********************************************************************/

bool CSPOSKeyboardListRecord::ValidateFormat( int nRows, int nCols )
{
	return ValidateFormat( ( nRows * 100 ) + nCols );
}

/**********************************************************************/

bool CSPOSKeyboardListRecord::ValidateFormat( int nFormat )
{
	switch( m_nListType )
	{
	case LISTTYPE_ITEM_STANDARD:
		switch( nFormat )
		{
		case LIST_FORMAT_1x5:
		case LIST_FORMAT_2x3:
		case LIST_FORMAT_2x4:
		case LIST_FORMAT_3x3:
		case LIST_FORMAT_3x4:
		case LIST_FORMAT_4x4:
		case LIST_FORMAT_4x5:
		case LIST_FORMAT_4x6:
		case LIST_FORMAT_5x4:
		case LIST_FORMAT_5x6:
		case LIST_FORMAT_6x5:
		case LIST_FORMAT_5x7:
		case LIST_FORMAT_12x10:
			return TRUE;
		}
		break;
				
	case LISTTYPE_ITEM_LARGE:
		switch( nFormat )
		{
		case LIST_FORMAT_2x1:
		case LIST_FORMAT_3x1:
		case LIST_FORMAT_4x1:
		case LIST_FORMAT_5x1:
		case LIST_FORMAT_6x1:
		case LIST_FORMAT_7x1:
		case LIST_FORMAT_2x2:
		case LIST_FORMAT_3x2:
		case LIST_FORMAT_5x2:
		case LIST_FORMAT_2x3:
		case LIST_FORMAT_3x3:
		case LIST_FORMAT_4x3:
		case LIST_FORMAT_4x4:
		case LIST_FORMAT_5x4:
		case LIST_FORMAT_4x5:
		case LIST_FORMAT_5x5:
		case LIST_FORMAT_6x5:
		case LIST_FORMAT_6x6:
		case LIST_FORMAT_9x6:
		case LIST_FORMAT_7x7:
		case LIST_FORMAT_12x10:
		case LIST_FORMAT_15x12:
			return TRUE;

		case LIST_FORMAT_8x5:
		case LIST_FORMAT_10x5:
		case LIST_FORMAT_12x5:
			switch( SPOSKeyboardManager.GetSPOSVersion() -> GetSPOSVersion() )
			{
			case SPOS_V4_2_714:
			case SPOS_V4_2_919:
			case SPOS_V4_2_959:
			case SPOS_V4_2_1081:
			case SPOS_V4_2_1087:
			case SPOS_V4_2_1384:
			case SPOS_V4_2_1456:
			case SPOS_V4_2_1530:
			case SPOS_V4_2_1579:
			case SPOS_V4_2_1605:
			case SPOS_V4_2_1618:
			case SPOS_V4_2_1622:
			case SPOS_V4_3_18:
			case SPOS_V4_3_93:
			case SPOS_V4_3_104:
				return TRUE;
			}
		}
		break;

	case LISTTYPE_FUNCTION_STANDARD:
		switch( nFormat )
		{
		case LIST_FORMAT_5x3:
			return TRUE;
		}
		break;

	case LISTTYPE_FUNCTION_LARGE:
		switch( nFormat )
		{
		case LIST_FORMAT_5x6:
			return TRUE;
		}
		break;

	case LISTTYPE_FUNCTION_BOTTOM:
		switch( nFormat )
		{
		case LIST_FORMAT_1x10:
		case LIST_FORMAT_1x14:
			return TRUE;
		}
		break;
	}
	return FALSE;
}

/**********************************************************************/

int CSPOSKeyboardListRecord::CalculateListWidth( double dPercent )
{
	double d = ( double(m_nTotalScreenWidth) * dPercent ) / 100.0;
	return int ( floor ( d ) );
}

/**********************************************************************/

int CSPOSKeyboardListRecord::CalculateListHeight( double dPercent )
{
	double d = ( (double) m_nTotalScreenHeight * dPercent ) / 100.0;
	return int ( floor ( d ) );
}

/**********************************************************************/

int CSPOSKeyboardListRecord::CalculateListPos( int nRange, double dPercent )
{
	double d = ( double(nRange) * dPercent ) / 100.0;
	return int ( floor ( d ) );
}

/**********************************************************************/

void CSPOSKeyboardListRecord::GetKeyPixelSize( int& nKeyWidth, int& nKeyHeight, int& nXKeySpace, int& nYKeySpace )
{
	int nListWidth = 500;
	int nListHeight = 500;
	nKeyHeight = 30;
	nKeyWidth = 30;
	nXKeySpace = 10;
	nYKeySpace = 10;

	switch( SPOSKeyboardManager.GetLayoutOptions() -> GetSharpPosResolutionType() )
	{
	case 0:
		m_nTotalScreenWidth = 640;
		m_nTotalScreenHeight = 480;
		break;

	case 1:
		m_nTotalScreenWidth = 800;
		m_nTotalScreenHeight = 600;
		break;

	case 3:
		m_nTotalScreenWidth = 1366;
		m_nTotalScreenHeight = 768;
		break;

	case 4:
		m_nTotalScreenWidth = 1920;
		m_nTotalScreenHeight = 1080;
		break;

	case 5:
		m_nTotalScreenWidth = 1280;
		m_nTotalScreenHeight = 800;
		break;

	case 6:
		m_nTotalScreenWidth = 1920;
		m_nTotalScreenHeight = 1200;
		break;

	case 7:
		m_nTotalScreenWidth = 1024;
		m_nTotalScreenHeight = 600;
		break;

	case 2:
	default:
		m_nTotalScreenWidth = 1024;
		m_nTotalScreenHeight = 768;
		break;
	}

	switch ( m_nFormat )
	{
	case LIST_FORMAT_1x5:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 55 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 17 );
		nKeyHeight = CalculateListPos( nListHeight, 50 );
		break;

	case LIST_FORMAT_1x10:
		nListWidth = CalculateListWidth( 99 - 10 );
		nListHeight = CalculateListHeight( 95.5 - 90.3 );
		nKeyWidth = CalculateListPos( nListWidth, 8.27 );
		nKeyHeight = CalculateListPos( nListHeight, 95 );
		break;

	case LIST_FORMAT_1x14:
		nListWidth = CalculateListWidth( 99 - 10 );
		nListHeight = CalculateListHeight( 95.5 - 90.3 );
		nKeyWidth = CalculateListPos( nListWidth, 5.9 );
		nKeyHeight = CalculateListPos( nListHeight, 95 );
		break;

	case LIST_FORMAT_2x1:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 90 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 40 );
		nKeyHeight = CalculateListPos( nListHeight, 25 );
		break;

	case LIST_FORMAT_2x2:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 90 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 42 );
		nKeyHeight = CalculateListPos( nListHeight, 25 );
		break;

	case LIST_FORMAT_2x3:
		switch( m_nListType )
		{
		case LISTTYPE_ITEM_STANDARD:
			nListWidth = CalculateListWidth( 99 - 34 );
			nListHeight = CalculateListHeight( 55 - 7 );
			nKeyWidth = CalculateListPos( nListWidth, 27 );
			nKeyHeight = CalculateListPos( nListHeight, 38 );
			break;

		case LISTTYPE_ITEM_LARGE:
			nListWidth = CalculateListWidth( 99 - 34 );
			nListHeight = CalculateListHeight( 90 - 7 );
			nKeyWidth = CalculateListPos( nListWidth, 30 );
			nKeyHeight = CalculateListPos( nListHeight, 45 );
			break;
		}
		break;

	case LIST_FORMAT_2x4:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 55 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 21 );
		nKeyHeight = CalculateListPos( nListHeight, 42 );
		break;

	case LIST_FORMAT_3x1:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 90 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 40 );
		nKeyHeight = CalculateListPos( nListHeight, 20 );
		break;

	case LIST_FORMAT_3x2:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 90 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 45.3 );
		nKeyHeight = CalculateListPos( nListHeight, 31 );
		break;

	case LIST_FORMAT_3x3:
		switch( m_nListType )
		{
		case LISTTYPE_ITEM_STANDARD:
			nListWidth = CalculateListWidth( 99 - 34 );
			nListHeight = CalculateListHeight( 55 - 7 );
			nKeyWidth = CalculateListPos( nListWidth, 28 );
			nKeyHeight = CalculateListPos( nListHeight, 28 );
			break;

		case LISTTYPE_ITEM_LARGE:
			nListWidth = CalculateListWidth( 99 - 34 );
			nListHeight = CalculateListHeight( 90 - 7 );
			nKeyWidth = CalculateListPos( nListWidth, 30 );
			nKeyHeight = CalculateListPos( nListHeight, 30 );
			break;
		}
		break;

	case LIST_FORMAT_3x4:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 55 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 21.5 );
		nKeyHeight = CalculateListPos( nListHeight, 28 );
		break;

	case LIST_FORMAT_4x1:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 90 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 40 );
		nKeyHeight = CalculateListPos( nListHeight, 17 );
		break;

	case LIST_FORMAT_4x3:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 90 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 30 );
		nKeyHeight = CalculateListPos( nListHeight, 23 );
		break;

	case LIST_FORMAT_4x4:
		switch( m_nListType )
		{
		case LISTTYPE_ITEM_STANDARD:
			nListWidth = CalculateListWidth( 99 - 34 );
			nListHeight = CalculateListHeight( 55 - 7 );
			nKeyWidth = CalculateListPos( nListWidth, 21.5 );
			nKeyHeight = CalculateListPos( nListHeight, 21 );
			break;

		case LISTTYPE_ITEM_LARGE:
			nListWidth = CalculateListWidth( 99 - 34 );
			nListHeight = CalculateListHeight( 90 - 7 );
			nKeyWidth = CalculateListPos( nListWidth, 22.5 );
			nKeyHeight = CalculateListPos( nListHeight, 23 );
			break;
		}
		break;

	case LIST_FORMAT_4x5:
		switch( m_nListType )
		{
		case LISTTYPE_ITEM_STANDARD:
			nListWidth = CalculateListWidth( 99 - 34 );
			nListHeight = CalculateListHeight( 55 - 7 );
			nKeyWidth = CalculateListPos( nListWidth, 17.5 );
			nKeyHeight = CalculateListPos( nListHeight, 21 );
			break;

		case LISTTYPE_ITEM_LARGE:
			nListWidth = CalculateListWidth( 99 - 34 );
			nListHeight = CalculateListHeight( 90 - 7 );
			nKeyWidth = CalculateListPos( nListWidth, 18 );
			nKeyHeight = CalculateListPos( nListHeight, 23 );
			break;
		}
		break;

	case LIST_FORMAT_4x6:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 55 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 14 );
		nKeyHeight = CalculateListPos( nListHeight, 21 );
		break;

	case LIST_FORMAT_5x1:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 90 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 40 );
		nKeyHeight = CalculateListPos( nListHeight, 16 );
		break;

	case LIST_FORMAT_5x2:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 90 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 45.3 );
		nKeyHeight = CalculateListPos( nListHeight, 18.5 );
		break;

	case LIST_FORMAT_5x3:
		nListWidth = CalculateListWidth( 68 - 34 );
		nListHeight = CalculateListHeight( 90 - 55 );
		nKeyWidth = CalculateListPos( nListWidth, 26 );
		nKeyHeight = CalculateListPos( nListHeight, 16 );
		break;

	case LIST_FORMAT_5x4:
		switch( m_nListType )
		{
		case LISTTYPE_ITEM_STANDARD:
			nListWidth = CalculateListWidth( 99 - 34 );
			nListHeight = CalculateListHeight( 55 - 7 );
			nKeyWidth = CalculateListPos( nListWidth, 21.5 );
			nKeyHeight = CalculateListPos( nListHeight, 17 );
			break;

		case LISTTYPE_ITEM_LARGE:
			nListWidth = CalculateListWidth( 99 - 34 );
			nListHeight = CalculateListHeight( 90 - 7 );
			nKeyWidth = CalculateListPos( nListWidth, 22.5 );
			nKeyHeight = CalculateListPos( nListHeight, 18.5 );
			break;
		}
		break;

	case LIST_FORMAT_5x5:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 90 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 18 );
		nKeyHeight = CalculateListPos( nListHeight, 18 );
		break;

	case LIST_FORMAT_5x6:
		switch( m_nListType )
		{
		case LISTTYPE_ITEM_STANDARD:
			nListWidth = CalculateListWidth( 99 - 34 );
			nListHeight = CalculateListHeight( 55 - 7 );
			nKeyWidth = CalculateListPos( nListWidth, 14 );
			nKeyHeight = CalculateListPos( nListHeight, 18 );
			break;

		case LISTTYPE_FUNCTION_LARGE:
			nListWidth = CalculateListWidth( 99 - 32 );
			nListHeight = CalculateListHeight( 90 - 55 );
			nKeyWidth = CalculateListPos( nListWidth, 14 );
			nKeyHeight = CalculateListPos( nListHeight, 17 );
			break;
		}
		break;

	case LIST_FORMAT_5x7:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 55 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 12.5 );
		nKeyHeight = CalculateListPos( nListHeight, 18 );
		break;

	case LIST_FORMAT_6x1:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 90 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 40 );
		nKeyHeight = CalculateListPos( nListHeight, 14.7 );
		break;

	case LIST_FORMAT_6x5:
		switch( m_nListType )
		{
		case LISTTYPE_ITEM_STANDARD:
			nListWidth = CalculateListWidth( 99 - 34 );
			nListHeight = CalculateListHeight( 55 - 7 );
			nKeyWidth = CalculateListPos( nListWidth, 17.7 );
			nKeyHeight = CalculateListPos( nListHeight, 14.5 );
			break;

		case LISTTYPE_ITEM_LARGE:
			nListWidth = CalculateListWidth( 99 - 34 );
			nListHeight = CalculateListHeight( 90 - 7 );
			nKeyWidth = CalculateListPos( nListWidth, 18 );
			nKeyHeight = CalculateListPos( nListHeight, 15.5 );
			break;
		}
		break;

	case LIST_FORMAT_6x6:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 90 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 15 );
		nKeyHeight = CalculateListPos( nListHeight, 15.5 );
		break;

	case LIST_FORMAT_7x1:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 90 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 40 );
		nKeyHeight = CalculateListPos( nListHeight, 12.7 );
		break;

	case LIST_FORMAT_7x7:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 90 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 14.2 );
		nKeyHeight = CalculateListPos( nListHeight, 13.5 );
		break;

	case LIST_FORMAT_8x5:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 90 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 21.2 );
		nKeyHeight = CalculateListPos( nListHeight, 8 );
		break;

	case LIST_FORMAT_9x6:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 90 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 16.4 );
		nKeyHeight = CalculateListPos( nListHeight, 10.5 );
		break;

	case LIST_FORMAT_10x5:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 90 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 21.2 );
		nKeyHeight = CalculateListPos( nListHeight, 8 );
		break;

	case LIST_FORMAT_12x5:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 90 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 21.2 );
		nKeyHeight = CalculateListPos( nListHeight, 8 );
		break;

	case LIST_FORMAT_12x10:
		switch( m_nListType )
		{
		case LISTTYPE_ITEM_STANDARD:
		case LISTTYPE_ITEM_LARGE:
			nListWidth = CalculateListWidth( 99 - 34 );
			nListHeight = CalculateListHeight( 90 - 7 );
			nKeyWidth = CalculateListPos( nListWidth, 9.8 );
			nKeyHeight = CalculateListPos( nListHeight, 8 );
			break;
		}
		break;

	case LIST_FORMAT_15x12:
		nListWidth = CalculateListWidth( 99 - 34 );
		nListHeight = CalculateListHeight( 89.5 - 7 );
		nKeyWidth = CalculateListPos( nListWidth, 8 );
		nKeyHeight = CalculateListPos( nListHeight, 6.6 );
		break;
	}
}

/**********************************************************************/

int CSPOSKeyboardListRecord::GetMaxNewPages( int nCurrentPages )
{
	if ( nCurrentPages >= MAX_LIST_PAGES )
		return -1;

	int nKeysPerPage = GetKeysPerPage();
	if ( nKeysPerPage == 0 )
		return -1;
	
	int nSpareKeys = ( MAX_LIST_KEYS - ( nKeysPerPage * nCurrentPages ) );
	int nSparePages = nSpareKeys / nKeysPerPage;

	if ( nSparePages > MAX_LIST_PAGES - nCurrentPages )
		nSparePages = MAX_LIST_PAGES - nCurrentPages;

	if ( nSparePages <= 0 )
		return -2;

	return nSparePages;
}

/**********************************************************************/

const char* CSPOSKeyboardListRecord::GetPageError( int n, int nPageCount )
{
	m_strPageError = "";

	switch( n )
	{
	case 1:
		m_strPageError.Format( "You cannot add more than %d pages to a list.", 
			MAX_LIST_PAGES );
		break;
		
	case 2:
		m_strPageError.Format( "You cannot add more than %d keys to a list.\n\nAdding another page to this list would give a total of %d keys.",
			MAX_LIST_KEYS,
			( nPageCount + 1 ) * GetKeysPerPage() );
		break;

		
	}

	return m_strPageError;
}

/**********************************************************************/
