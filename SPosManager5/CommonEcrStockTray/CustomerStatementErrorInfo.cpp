/**********************************************************************/
 
/**********************************************************************/
#include "SMTPEmailHelpers.h"
/**********************************************************************/
#include "CustomerStatementErrorInfo.h"
/**********************************************************************/

CCustomerStatementErrorInfo::CCustomerStatementErrorInfo()
{
	m_nNameTableIdx = 0;
	m_nStatementError = 0;
	m_nSMTPError = 0;
	m_strErrorText = "";
}

/**********************************************************************/

void CCustomerStatementErrorInfo::GetErrorText( CString& strText )
{
	strText = "";
	switch( m_nStatementError )
	{
	case CUSTOMER_STATEMENT_ERROR_CREATE:			
		strText = "Unable to create statement";	
		break;

	case CUSTOMER_STATEMENT_ERROR_DISPLAY:	
		strText = "Unable to display statement";
		break;

	case CUSTOMER_STATEMENT_ERROR_PRINT:	
		strText = "Unable to print statement";	
		break;

	case CUSTOMER_STATEMENT_ERROR_DATERANGE:
		strText = "Invalid date range";		
		break;

	case CUSTOMER_STATEMENT_ERROR_DATEBALANCE:	
		strText = "No starting balance available";
		break;

	case CUSTOMER_STATEMENT_ERROR_EMAIL:
		strText = m_strErrorText;		
		break;

	case CUSTOMER_STATEMENT_ERROR_NOADDRESS:
		strText = "No email address specified";	
		break;

	case CUSTOMER_STATEMENT_ERROR_REDIRECT:		
		strText = "Redirected to printer";	
		break;

	case CUSTOMER_STATEMENT_ERROR_NONE:	
		strText = "OK";						
		break;

	case CUSTOMER_STATEMENT_ERROR_SMTP:		
		CSMTPEmailHelpers::GetErrorText( m_nSMTPError, strText );
		break;

	case CUSTOMER_STATEMENT_ERROR_NICKNAME_FIND:	
		strText = "Unknown email nickname";	
		break;

	case CUSTOMER_STATEMENT_ERROR_NICKNAME_EMAIL:
		strText = "No email addresses for nickname";
		break;

	default:								
		strText = "Unknown error";			
		break;
	}
}

/**********************************************************************/
