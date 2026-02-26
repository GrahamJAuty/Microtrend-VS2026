#pragma once
/**********************************************************************/
 
/**********************************************************************/
#define CUSTOMER_STATEMENT_HEADER_LINE_COUNT 2
#define CUSTOMER_STATEMENT_HEADER_LINE_MAXLEN 80
/**********************************************************************/

class CCustomerStatementHeaderLines : public CSharedStringArray
{
public:
	CCustomerStatementHeaderLines();
	int Open( const char* szFilename, int nMode );

public:
	const char* GetLine( int nLine );
	void SetLine( int nLine, const char* szLine );

private:
	CString m_strLine;
};

/**********************************************************************/
