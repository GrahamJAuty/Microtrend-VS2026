/**********************************************************************/
#include "StatusProgress.h"
/**********************************************************************/
#include "SQLIn.h"
/**********************************************************************/

/*
INSERT Products
([PLU],[Description],[Price1],[Price2],[Price3],[Price4]
 ,[Dept],[VAT],[ProdType],[ProdCat],[ListMenu],[ListQual],[ListExtra],[Dsp1]
 ,[Dsp2],[Dsp3],[Dsp4],[Dsp5],[Dsp6],[Dsp7],[Dsp8],[Dsp9],[DspPriority]
 ,[Discount],[PriceType],[MMOffer],[MMBucket],[FreeText],[V$SoldOut],[V$StkDone]
 ,[V$StkDirty],[V$StockID],[V$ATpos])
 VALUES (10, 'S&P Ribs', 6.02, 5.9, 5.9, 0, 1, 'A', 0, 0, 0, 0, 0, 1, 0
 , 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, '', 0, 0, 0, 0, 0, 0)
--->GO
*/
/**********************************************************************/

CSQLIn::CSQLIn ( CSSFile& file, const char* szTableName, const char* szHeader )
	: m_file(file), m_strTableName(szTableName), m_strHeader(szHeader)
{
	m_strPrevColumnList = "";
	m_cQuote = '\'';			// single quote
}

/**********************************************************************/

bool CSQLIn::IsWhiteSpace( char c )
{
	return ( ( c == ' ' ) || ( c == '\t' ) || ( c == '\r' ) || ( c == '\n' ) );
}

/**********************************************************************/

bool CSQLIn::ReadLine ( CString& strData, bool bProgress )
{	
	CString strBuf;

	while ( m_file.ReadLine ( strBuf ) == TRUE )
	{
		if ( TRUE == bProgress )
		{
			StatusProgress.SetPos( m_file.GetPercentPosition(), 100 );
		}

		int nPos = 0;
		while ( nPos < strBuf.GetLength() )
		{
			char c = strBuf[nPos];

			if ( IsWhiteSpace(c) == FALSE )
				break;

			nPos++;
		}

		if ( nPos > strBuf.GetLength() - 6 )
			continue;

		if ( strBuf.Mid( nPos, 6 ) != "INSERT" )
			continue;
		
		CString strTableName = "";
		
		nPos += 6;		
		for ( ; nPos < strBuf.GetLength() ; nPos++ )	// find table name
		{
			char c = strBuf[nPos];

			if ( c == '(' )
				break;

			if ( IsWhiteSpace(c) == FALSE )
				strTableName += c;
		}
			
		if ( strTableName != m_strTableName )	
			continue;

		CString strLine = strBuf;
		while ( m_file.ReadLine ( strBuf ) == TRUE )
		{
			strLine += strBuf;
			if ( strBuf.Left(6) == "--->GO" )
			{
				if ( ParseSqlInsert ( strLine ) == TRUE )
				{
					GetSqlData ( strData );
					return TRUE;		// found required table
				}
			}
		}
	}

	return FALSE;		// end of file
}

/**********************************************************************/

bool CSQLIn::ParseSqlInsert ( const char* szBuf )
{
	CString strBuf;

	bool bInQuotes = 0;

	unsigned int nLen1 = strlen( szBuf );
	for ( unsigned int i = 0 ; i < nLen1 ; i++ )	// discard white space
	{
		char c = szBuf[i];

		if ( c == m_cQuote )		// single quote
			bInQuotes ^= 1;		// toggle in quotes flag

		if ( bInQuotes == 1 )		// inside quotes
		{
			strBuf += c;			// keep everything
		}
		else						// outside quotes
		{
			if ( IsWhiteSpace(c) == FALSE )
				strBuf += c;
		}
	}

	int n = 0;
	int nLen2 = strBuf.GetLength();

	GetTextInBrackets ( m_strColumnList, strBuf, n, nLen2 );

	if ( strBuf.Mid(n+1,6) == "VALUES" )
	{
		GetTextInBrackets ( m_strValues, strBuf, n, nLen2 );
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

void CSQLIn::GetSqlData ( CString& strData )
{
	if ( m_strColumnList != m_strPrevColumnList )	// no need to recreate index if column list has not changed
	{
		CCSV csvHeader ( m_strHeader );			// "[PLU],[Description],[Price1],[Dept]"
		CCSV csvColumnList ( m_strColumnList );		// sql full column list
		m_uiIndexArray.RemoveAll();				// clear index array;

		for ( int n = 0 ; n < csvHeader.GetSize() ; n ++ )
		{
			int i;
			for ( i = 0 ; i < csvColumnList.GetSize() ; i++ )	// find columns
			{
				CString strColumn = csvColumnList.GetString(i);

				if ( strColumn == csvHeader.GetString(n) )
				{
					m_uiIndexArray.Add(i);
					break;
				}
			}
			if ( i == csvColumnList.GetSize() )	// not found
				m_uiIndexArray.Add ( -1 );		// -1 = not found
		}
		m_strPrevColumnList = m_strColumnList;
	}

	CCSV csvValues ( m_strValues, ',', m_cQuote );	// sql values (needs single quote delimiter)
	CCSV csvOut;

	for ( int i = 0 ; i < m_uiIndexArray.GetSize() ; i++ )
	{
		int nIndex = m_uiIndexArray.GetAt(i);

		if ( nIndex != -1 )
			csvOut.Add ( csvValues.GetString(nIndex) );
		else
			csvOut.Add ( "" );		// not found
	}

	strData = csvOut.GetLine();
}

/**********************************************************************/

void CSQLIn::GetTextInBrackets ( CString& strText, const char* szBuf, int& i, int nLen )
{
	strText = "";

	bool bInQuotes = 0;
	bool bInBrackets = FALSE;

	for ( i ; i < nLen ; i++ )
	{
		char c = szBuf[i];

		if ( c == m_cQuote )		// single quote
			bInQuotes ^= 1;		// toggle in quotes flag

		if ( bInQuotes == 1 )		// inside quotes
		{
			strText += c;			// keep everything
		}
		else						// outside quotes
		{
			if ( bInBrackets == TRUE )
			{
				if ( c == ')' )
					break;

				strText += c;
			}
			else
			{
				if ( c == '(' )
					bInBrackets = TRUE;
			}
		}
	}
}

/**********************************************************************/
