#pragma once
/**********************************************************************/

class CKeyboardHelpers  
{
public:
	static void CreatePluFile( int nDbIdx, int nLocIdx, CString& strPluFilename, bool bKbSet );
	static void RemovePluFile( int nDbIdx );
	static void CloneFolder( const char* szSource, const char* szDest );
};

/**********************************************************************/

