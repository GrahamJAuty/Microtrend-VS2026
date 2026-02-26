/**********************************************************************/
 
/**********************************************************************/
#include "EposReportConsolCustomerStatement.h"
/**********************************************************************/

CEposReportConsolCustomerStatement::CEposReportConsolCustomerStatement()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolCustomerStatement::Reset()
{
	m_nBlock = 0;
	m_strDate = "";
	m_strTime = "";
	m_nLineNo = 0;
	m_nType = 0;
}

/**********************************************************************/

int CEposReportConsolCustomerStatement::Compare ( CEposReportConsolCustomerStatement& source, int nHint )
{
	if ( source.m_nBlock != m_nBlock )
		return ( ( m_nBlock > source.m_nBlock ) ? 1 : -1 );

	if ( m_nBlock != 2 )
		return 0;

	if ( source.m_strDate != m_strDate )
		return ( ( m_strDate > source.m_strDate ) ? 1 : -1 );

	if ( source.m_strTime != m_strTime )
		return ( ( m_strTime > source.m_strTime ) ? 1 : -1 );

	if ( source.m_nType != m_nType )
		return ( ( m_nType > source.m_nType ) ? 1 : -1 );

	if ( source.m_nLineNo != m_nLineNo )
		return ( ( m_nLineNo > source.m_nLineNo ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

void CEposReportConsolCustomerStatement::Add ( CEposReportConsolCustomerStatement& source )
{
	m_dBalance += source.m_dBalance;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
