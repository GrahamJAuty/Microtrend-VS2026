#pragma once
/**********************************************************************/
#define CUSTOMER_STATEMENT_ERROR_NONE 0
#define CUSTOMER_STATEMENT_ERROR_CREATE 1
#define CUSTOMER_STATEMENT_ERROR_DISPLAY 2
#define CUSTOMER_STATEMENT_ERROR_PRINT 3
#define CUSTOMER_STATEMENT_ERROR_DATERANGE 4
#define CUSTOMER_STATEMENT_ERROR_DATEBALANCE 5
#define CUSTOMER_STATEMENT_ERROR_EMAIL 6
#define CUSTOMER_STATEMENT_ERROR_NOADDRESS 7
#define CUSTOMER_STATEMENT_ERROR_REDIRECT 8
#define CUSTOMER_STATEMENT_ERROR_SMTP 9
/**********************************************************************/
#define CUSTOMER_STATEMENT_ERROR_NICKNAME_FIND 10
#define CUSTOMER_STATEMENT_ERROR_NICKNAME_EMAIL 11
/**********************************************************************/

struct CCustomerStatementErrorInfo
{
public:
	CCustomerStatementErrorInfo();
	void GetErrorText( CString& strText );

public:
	int m_nNameTableIdx;
	int m_nStatementError;
	int m_nSMTPError;
	CString m_strErrorText;
};

/**********************************************************************/