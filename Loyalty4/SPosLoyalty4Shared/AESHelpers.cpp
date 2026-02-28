//$$******************************************************************
//$$*** exact same file used by LoyaltyManager / LoyaltyServer *******
//$$******************************************************************
#include "AESHelpers.h"
//$$******************************************************************

bool CAESHelpers::PlainTextToAESHex(CString strPlainText, CString& strAESHex)
{
	strAESHex = "";

	SolutionGlobalFunctions::TrimSpacesFromString(strPlainText, FALSE);
	
	int nTextLen = strPlainText.GetLength();

	if (nTextLen == 0)
	{
		return FALSE;
	}

	unsigned char* pCodedText = new unsigned char[nTextLen + 16];

	if (pCodedText == NULL)
	{
		return FALSE;
	}

	int nCodedLen = CSSAES::Encrypt((const unsigned char*)(LPCTSTR)strPlainText, nTextLen, pCodedText, GetPassword());

	for (int n = 0; n < nCodedLen; n++)
	{
		CString strHex = "";
		strHex.Format("%2.2x", pCodedText[n]);
		strAESHex += strHex;
	}

	delete pCodedText;

	return TRUE;
}

//$$******************************************************************

bool CAESHelpers::AESHexToPlainText(CString strAESHex, CString& strPlainText)
{
	strPlainText = "";

	int nHexLength = strAESHex.GetLength();
	if ((nHexLength == 0 ) || ((nHexLength % 2) != 0))
	{
		return FALSE;
	}

	int nCharLength = nHexLength / 2;

	unsigned char* pPlainText = new unsigned char[nCharLength];

	if (pPlainText == NULL)
	{
		return FALSE;
	}

	unsigned char* pCodedText = new unsigned char[nCharLength];

	if (pCodedText == NULL)
	{
		delete pPlainText;
		return FALSE;
	}
	
	for (int n = 0; n < nCharLength; n++)
	{
		pCodedText[n] = HexToInt(strAESHex.Mid(n * 2, 2));
	}

	int nDecodedLen = CSSAES::Decrypt(pCodedText, nCharLength, pPlainText, GetPassword());

	for (int n = 0; n < nDecodedLen; n++)
	{
		strPlainText += pPlainText[n];
	}

	delete pPlainText;
	delete pCodedText;

	return TRUE;
}

//$$******************************************************************
