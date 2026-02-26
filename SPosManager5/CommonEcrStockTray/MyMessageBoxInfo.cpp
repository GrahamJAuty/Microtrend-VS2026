/**********************************************************************/
#include "MyMessageBoxInfo.h"
/**********************************************************************/

CMyMessageBoxInfo::CMyMessageBoxInfo()
{
	m_strTitle = "";
	m_strText = "Message";
	m_colourBack = COLORREF( 0xFFFFFF );
	m_colourText = COLORREF( 0xFF0000 );
	m_nFontSize = 10;
	m_nLineGap = 5;
	m_nIconType = 0;
	m_nButtonMode = MB_OK;
	m_nDefButton = 1;
}

/**********************************************************************/

const char* CMyMessageBoxInfo::GetText( int nIdx )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayTexts.GetSize() ) )
		m_strText = m_arrayTexts.GetAt( nIdx );
	else
		m_strText = "";

	return m_strText;
}

/**********************************************************************/

void CMyMessageBoxInfo::SetTitle( const char* szTitle )
{
	m_strTitle = szTitle;
}

/**********************************************************************/

void CMyMessageBoxInfo::SetTexts( CStringArray& arrayTexts )
{
	m_arrayTexts.RemoveAll();

	for ( int n = 0; n < arrayTexts.GetSize(); n++ )
		m_arrayTexts.Add( arrayTexts.GetAt(n) );

	if ( m_arrayTexts.GetSize() == 0 )
		m_arrayTexts.Add( "Message" );
}

/**********************************************************************/

void CMyMessageBoxInfo::SetTexts( const char* szText )
{
	m_arrayTexts.RemoveAll();
	CString strToken = "";

	CString strText = szText;
	for ( int i = 0; i < strText.GetLength(); i++ )
	{
		char c = strText.GetAt(i);

		if ( c == '\n' )
		{
			m_arrayTexts.Add( strToken );
			strToken = "";
		}
		else
		{
			strToken += c;
		}
	}

	if ( strToken != "" )
		m_arrayTexts.Add( strToken );

	if ( m_arrayTexts.GetSize() == 0 )
		m_arrayTexts.Add( "Message" );
}

/**********************************************************************/

void CMyMessageBoxInfo::SetBackColour( COLORREF cr )
{
	m_colourBack = cr;
}

/**********************************************************************/

void CMyMessageBoxInfo::SetTextColour( COLORREF cr )
{
	m_colourText = cr;
}

/**********************************************************************/

void CMyMessageBoxInfo::SetFontSize( int n )
{
	switch( n )
	{
	case 8:
	case 10:
	case 11:
	case 12:
	case 14:
		m_nFontSize = n;
		break;
	}
}

/**********************************************************************/

void CMyMessageBoxInfo::SetLineGap( int n )
{
	if ( ( n >= 3 ) && ( n <= 15 ) )
		m_nLineGap = n;
}

/**********************************************************************/

void CMyMessageBoxInfo::SetIconType( int n )
{
	switch( n )
	{
	case 0:
	case MB_ICONQUESTION:
	case MB_ICONEXCLAMATION:
	case MB_ICONINFORMATION:
	case MB_ICONSTOP:
		m_nIconType = n;
		break;
	}
}

/**********************************************************************/

void CMyMessageBoxInfo::SetButtonMode( int n )
{
	switch( n )
	{
	case MB_OK:
	case MB_OKCANCEL:
	case MB_YESNO:
	case MB_YESNOCANCEL:
		m_nButtonMode = n;
		break;
	}
}

/**********************************************************************/

void CMyMessageBoxInfo::SetDefButton( int n )
{
	if ( ( n >= 1 ) && ( n <= 3 ) )
		m_nDefButton = n;
}

/**********************************************************************/