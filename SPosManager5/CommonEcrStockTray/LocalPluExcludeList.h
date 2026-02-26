#pragma once
/*********************************************************************/
 
/**********************************************************************/

class CLocalPluIncludeExcludeList : public CFileStringArray 
{
public:
	CLocalPluIncludeExcludeList();
	void Read( const char* szFilename );
	void Write( const char* szFilename );
	void AddPluNo ( __int64 nPluNo );
	void RemovePluNo ( __int64 nPluNo );

private:
	bool FindPluNo( __int64 nPluNo, int& nIndex );
};

/**********************************************************************/