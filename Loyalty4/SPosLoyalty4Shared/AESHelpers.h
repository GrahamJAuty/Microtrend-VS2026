//$$******************************************************************
//$$** exact same file used by LoyaltyManager / LoyaltyServer ********
//$$******************************************************************
#pragma once
//$$******************************************************************

class CAESHelpers
{
public:
	static CString GetPassword() { return "5cDUJM9ner*UdV6%"; }
	static bool PlainTextToAESHex(CString strPlainText, CString& strAESHex);
	static bool AESHexToPlainText(CString strAESHex, CString& strPlainText);
};

//$$******************************************************************
