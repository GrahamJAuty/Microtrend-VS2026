#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/

class CXMLWrangler
{
public:
	CXMLWrangler( int nNetworkIdx, CSSXMLParser& Parser, const char* szPrefix );

public:
	int GetTransactionCount(){ return m_nTransactionCount; }
	
public:
	bool OpenCashRSPFile();
	bool RenameCashRSPFile();
	void CloseCashRSPFile();

protected:
	const char* GetPennyString( const char* szPoundString, bool bFlipMinus );
	const char* GetStringValue( CXMLNodeArray* pNodeArray, const char* szPath, const char* szElement );
	
protected:
	CSSXMLParser& m_Parser;
	CXMLNodeArray m_arTransaction;

protected:
	int m_nNetworkIdx;
	int m_nTransactionCount;

protected:
	CSSFile m_fileCashRSP;
	bool m_bCashRSPFileOpen;
	CString m_strDestFile1;
	CString m_strDestFile2;

protected:
	CString m_strPennyString;
	CString m_strStringValue;
};

/**********************************************************************/
#endif
/**********************************************************************/
