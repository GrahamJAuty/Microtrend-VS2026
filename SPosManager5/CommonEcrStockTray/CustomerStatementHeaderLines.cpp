/**********************************************************************/
 
/**********************************************************************/
#include "CustomerStatementHeaderLines.h"
/**********************************************************************/

CCustomerStatementHeaderLines::CCustomerStatementHeaderLines()
{
}

/**********************************************************************/

int CCustomerStatementHeaderLines::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );
	
	int nReply = CSharedStringArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	int nSize = GetSize();
	
	if ( nSize > CUSTOMER_STATEMENT_HEADER_LINE_COUNT )
		RemoveAt( CUSTOMER_STATEMENT_HEADER_LINE_COUNT, GetSize() - CUSTOMER_STATEMENT_HEADER_LINE_COUNT );
	
	for ( int nIndex = GetSize(); nIndex < CUSTOMER_STATEMENT_HEADER_LINE_COUNT; nIndex++ )			//make up shortfall
		Add( "" );

	return nReply;
}

/**********************************************************************/

const char* CCustomerStatementHeaderLines::GetLine ( int nLine )
{
	m_strLine = "";

	if ( ( nLine >= 0 ) && ( nLine < GetSize() ) )
		m_strLine = GetAt( nLine );

	return m_strLine;
}

/**********************************************************************/

void CCustomerStatementHeaderLines::SetLine ( int nLine, const char* szLine )
{
	if ( ( nLine >= 0 ) && ( nLine < GetSize() ) )
	{
		CString strLine = szLine;
		if ( strLine.GetLength() <= CUSTOMER_STATEMENT_HEADER_LINE_MAXLEN )
			SetAt( nLine, strLine );
	}
}

/**********************************************************************/


