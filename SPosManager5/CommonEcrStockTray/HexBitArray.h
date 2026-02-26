#pragma once
/**********************************************************************/

class CHexBitArray
{
public:
	CHexBitArray( int nBits );
	void Clear(){ m_arrayChunks.RemoveAll(); }
	
public:
	bool GetBit( int nBit );
	void SetBit( int nBit, bool bFlag );

public:
	void TranslateFromHexString( CString& strHex );
	void TranslateToHexString( CString& strHex );
	
private:
	int m_nBits;
	CArray<int,int> m_arrayChunks;
};

/**********************************************************************/