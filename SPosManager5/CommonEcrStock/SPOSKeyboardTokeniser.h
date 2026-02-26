#pragma once
/**********************************************************************/

class CSPOSKeyboardTokeniser  
{
public:
	CSPOSKeyboardTokeniser(){};
	void Tokenise( CString& strLine );
	
public:
	int GetInt( int nPos );
	bool GetBool( int nPos );
	const char* GetString( int nPos );
	void GetKeyTexts( int nPos, CStringArray& array );
	int GetSize() { return m_arrayTokens.GetSize(); }

private:
	CStringArray m_arrayTokens;
	CString m_strToken;
};

/**********************************************************************/
