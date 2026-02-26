#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
/*														*/
/*	eg	szTableName = "Products"								*/
/*		szHeader = "[PLU],[Description],[Price1],[Dept]"			*/
/*														*/
/**********************************************************************/

class CSQLOut
{
public:
	CSQLOut ( CSSFile& file, const char* szTableName, const char* szHeader );
	
	void SetHeader( const char* szHeader );
	
	int Add ( const char* szText );
	int Add ( int nNum );
	int Add ( __int64 nNum );
	int Add ( long lNum );
	int Add ( double dNum, int nDp );
	int Add ( double dNum, int nDp, bool bMinimize );
	int Add ( bool b );
	int AddHexString( const char* szText );

	int AddCondition ( const char* szText );
	int AddCondition ( int nNum );
	int AddCondition ( long lNum );
	int AddCondition ( double dNum, int nDp );
	int AddCondition ( double dNum, int nDp, bool bMinimize );
	int AddCondition ( bool b );

	bool WriteFileBegin();
	bool WriteFileEnd();

	bool WriteDelete();
	bool WriteInsert();
	bool WriteModify();

private:
	CString FormatString ( const char* szText );
	CString FormatString ( int nNum );
	CString FormatString ( __int64 nNum );
	CString FormatString ( long lNum );
	CString FormatString ( double dNum, int nDp, bool bMinimize );
	CString FormatString ( bool b );
	int AddString ( const char* szText, BYTE ucType );
	CString GetString ( int nIndex );
	CString GetQuotedString ( const char* szText, bool bQuote, bool bHex );

private:
	CSSFile& m_file;
	CString m_strTableName;
	CString m_strHeader;
	CByteArray m_ucColumnType;
	CStringArray m_strValues;
	bool m_bRemoveZeros;
	bool m_bMinimizeFloat;
	char m_cQuote;
};

/**********************************************************************/
#endif
/**********************************************************************/
