/**********************************************************************/
#include "SPOSKeyboardListManager.h"
/**********************************************************************/
#include "SPOSKeyboardScreenRecord.h"
/**********************************************************************/

CSPOSKeyboardScreenRecord::CSPOSKeyboardScreenRecord()
{
	Reset();
}

/**********************************************************************/

void CSPOSKeyboardScreenRecord::Reset()
{
	m_nScreenNo = 1;
	m_strName = "";

	//TRADING LIST
	m_nTradingScreenStyle = 1;
	m_nTradingListTopLevel = 0;
	m_nTradingListFunction = 0;
	//LOGGED OFF LIST
	m_nLoggedOffScreenStyle = 1;
	m_nLoggedOffListFunction = 0;
	m_nLoggedOffListFunctionLarge = 0;
	//PAYMENT LIST
	m_nPaymentListTopLevel = 0;
	m_nPaymentListFunction = 0;

	m_nListFastBar = 0;
}

/**********************************************************************/

void CSPOSKeyboardScreenRecord::SetScreenNo( int n )
{
	if ( n >= 1 && n <= MAX_SCREEN_NO )
		m_nScreenNo = n;
}

/**********************************************************************/

void CSPOSKeyboardScreenRecord::SetName( const char* szName )
{
	m_strName = szName;
	::TrimSpaces ( m_strName, FALSE );
}

/**********************************************************************/

void CSPOSKeyboardScreenRecord::SetTradingScreenStyle( int n )
{
	if ( n >= 1 && n <= 3 )
		m_nTradingScreenStyle = n;
}

/**********************************************************************/

void CSPOSKeyboardScreenRecord::SetTradingListTopLevel( __int64 n )
{
	if ( n >= 0 && n <= MAX_LIST_NO )
		m_nTradingListTopLevel = n;
}

/**********************************************************************/

void CSPOSKeyboardScreenRecord::SetTradingListFunction( __int64 n )
{
	if ( n >= 0 && n <= MAX_LIST_NO )
		m_nTradingListFunction = n;
}

/**********************************************************************/

void CSPOSKeyboardScreenRecord::SetLoggedOffScreenStyle( int n )
{
	if ( n >= 1 && n <= 2 )
		m_nLoggedOffScreenStyle = n;
}

/**********************************************************************/

void CSPOSKeyboardScreenRecord::SetLoggedOffListFunction( __int64 n )
{
	if ( n >= 0 && n <= MAX_LIST_NO )
		m_nLoggedOffListFunction = n;
}

/**********************************************************************/

void CSPOSKeyboardScreenRecord::SetLoggedOffListFunctionLarge( __int64 n )
{
	if ( n >= 0 && n <= MAX_LIST_NO )
		m_nLoggedOffListFunctionLarge = n;
}

/**********************************************************************/

void CSPOSKeyboardScreenRecord::SetPaymentListTopLevel( __int64 n )
{
	if ( n >= 0 && n <= MAX_LIST_NO )
		m_nPaymentListTopLevel = n;
}

/**********************************************************************/

void CSPOSKeyboardScreenRecord::SetPaymentListFunction( __int64 n )
{
	if ( n >= 0 && n <= MAX_LIST_NO )
		m_nPaymentListFunction = n;
}

/**********************************************************************/

void CSPOSKeyboardScreenRecord::SetListFastBar( __int64 n )
{
	if ( n >= 0 && n <= MAX_LIST_NO )
		m_nListFastBar = n;
}

/**********************************************************************/

const char* CSPOSKeyboardScreenRecord::GetComboText()
{
	m_strComboText.Format ( "S%5.5d,  %s",
		m_nScreenNo % 100000,
		(const char*) m_strName );

	return m_strComboText;
}

/**********************************************************************/

int CSPOSKeyboardScreenRecord::GetTradingListTypeTopLevel()
{
	switch( m_nTradingScreenStyle )
	{
	case 1:
	case 2:
		return LISTTYPE_ITEM_STANDARD;

	case 3:
		return LISTTYPE_ITEM_LARGE;

	default:
		return -1;
	}
}

/**********************************************************************/

int CSPOSKeyboardScreenRecord::GetTradingListTypeFunction()
{
	switch( m_nTradingScreenStyle )
	{
	case 1:
		return LISTTYPE_FUNCTION_STANDARD;

	case 2:
		return LISTTYPE_FUNCTION_LARGE;

	case 3:
		return LISTTYPE_FUNCTION_BOTTOM;

	default:
		return -1;
	}
}

/**********************************************************************/

int CSPOSKeyboardScreenRecord::GetLoggedOffListTypeFunction()
{
	switch( m_nLoggedOffScreenStyle )
	{
	case 1:
		return LISTTYPE_FUNCTION_STANDARD;
		
	case 2:
		return LISTTYPE_ITEM_LARGE;
		
	default:
		return -1;
	}
}

/**********************************************************************/
