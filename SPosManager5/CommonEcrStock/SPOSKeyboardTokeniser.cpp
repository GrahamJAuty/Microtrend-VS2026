/**********************************************************************/
#include "SPOSKeyboardTokeniser.h"
/**********************************************************************/

void CSPOSKeyboardTokeniser::Tokenise( CString& strLine )
{
	m_arrayTokens.RemoveAll();

	int nMode = 0;
	CString strToken = "";

	for ( int nPos = 0; nPos < strLine.GetLength(); nPos++ )
	{
		char c = strLine.GetAt( nPos );
		
		if ( nMode == 0 )
		{
			if ( c == ' ' )
			{
				if ( strToken != "" ) 
				{
					m_arrayTokens.Add ( strToken ); 
					strToken = "";
				}
			}
			else if ( c == '<' )
			{
				if ( strToken == "" )
					nMode = 1;
			}
			else if ( c == 0x22 )
			{
				if ( strToken == "" )
					nMode = 2;
			}
			else
				strToken += c;
		}
		else if ( nMode == 1 )
		{
			if ( c == '>' )
			{
				m_arrayTokens.Add ( strToken );
				strToken = "";
				nMode = 0;
			}
			else
				strToken += c;
		}
		else
		{
			if ( c == 0x22 )
			{
				m_arrayTokens.Add ( strToken );
				strToken = "";
				nMode = 0;
			}
			else
				strToken += c;
		}
	}
	if ( strToken != "" )
		m_arrayTokens.Add ( strToken );
}

/**********************************************************************/

bool CSPOSKeyboardTokeniser::GetBool( int nPos )
{
	if ( nPos < 0 || nPos >= m_arrayTokens.GetSize() )
		return FALSE;
	else
		return ( m_arrayTokens.GetAt ( nPos ) == "1" );
}

/**********************************************************************/

int CSPOSKeyboardTokeniser::GetInt( int nPos )
{
	if ( nPos < 0 || nPos >= m_arrayTokens.GetSize() )
		return 0;
	else
		return atoi ( m_arrayTokens.GetAt ( nPos ) );
}

/**********************************************************************/

const char* CSPOSKeyboardTokeniser::GetString( int nPos )
{
	m_strToken = "";
	if ( nPos >= 0 && nPos < m_arrayTokens.GetSize() )
		m_strToken = m_arrayTokens.GetAt ( nPos );

	return m_strToken;
}

/**********************************************************************/

void CSPOSKeyboardTokeniser::GetKeyTexts( int nPos, CStringArray& array )
{
	array.RemoveAll();
	if ( nPos < 0 || nPos >= m_arrayTokens.GetSize() )
		return;

	CString strFullText = m_arrayTokens.GetAt( nPos );

	CString strText = "";
	for ( int nPos2 = 0; nPos2 < strFullText.GetLength(); nPos2++ )
	{
		char c = strFullText.GetAt ( nPos2 );

		if ( c == '|' ) 
		{
			array.Add ( strText );
			strText = "";
		}
		else
			strText += c;
	}
	array.Add ( strText );
}

/**********************************************************************/

