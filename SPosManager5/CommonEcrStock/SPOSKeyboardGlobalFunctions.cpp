/**********************************************************************/
#include "SPOSKeyboardGlobalFunctions.h"
/**********************************************************************/

void HexToString ( const char* szHex, CString& strDest )
{
	strDest = "";
	CString strHex = szHex;
	int nOffset = 0;

	while ( nOffset < strHex.GetLength() - 1 )
	{
		char c = ::HexToInt ( strHex.Mid ( nOffset, 2 ) );
		
		if (0 == c)
		{
			break;
		}

		nOffset += 2;
		strDest += c;		
	}
}

/**********************************************************************/

void TokeniseKeyTextLine(CString& strText, CStringArray& arrayTokens)
{
	arrayTokens.RemoveAll();

	int nCurrentCharType = 0;
	CString strToken = "";

	for (int i = 0; i < strText.GetLength(); i++)
	{
		char c = strText.GetAt(i);
		int nCharType = (c == ' ' ? 2 : 1);

		if (nCharType != nCurrentCharType)
		{
			if (strToken != "")
			{
				arrayTokens.Add(strToken);
			}

			strToken = "";
			nCurrentCharType = nCharType;
		}

		strToken += c;
	}

	if (strToken != "")
	{
		arrayTokens.Add(strToken);
	}
}

/**********************************************************************/

