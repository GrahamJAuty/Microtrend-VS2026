#pragma once
/**********************************************************************/
/*														*/
/*	eg	szTableName = "Products"								*/
/*		szHeader = "[PLU],[Description],[Price1],[Dept]"			*/
/*			(ie the fields you want extract data from in the		*/
/*			 order you want them)							*/
/*														*/
/**********************************************************************/

class CSQLIn
{
public:
	CSQLIn ( CSSFile& file, const char* szTableName, CStringArray& arrayFields);
	bool ReadLine ( CString& strBuf );

private:
	bool ParseSqlInsert ( const char* szBuf );
	void GetSqlData ( CString& strdata );
	void GetTextInBrackets ( CString& strText, const char* szBuf, int& i, int nLen );
	bool IsWhiteSpace( char c );

private:
	CSSFile& m_file;
	CString m_strTableName;
	CString m_strHeader;
	CString m_strColumnList;
	CString m_strValues;
	CString m_strPrevColumnList;
	CUIntArray m_uiIndexArray;
	char m_cQuote;
};

/**********************************************************************/

