/**********************************************************************/
 
/**********************************************************************/
#include "EposReportConsolCustomerHistory.h"
/**********************************************************************/

CEposReportConsolCustomerHistory::CEposReportConsolCustomerHistory()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolCustomerHistory::Reset()
{
	m_nTransactionNo = 0;
	m_nReportLineType = 0;
	m_nReportLineSubType = 0;
	m_dQty = 0.0;
	m_dVal = 0.0;
}

/**********************************************************************/

void CEposReportConsolCustomerHistory::Add ( CEposReportConsolCustomerHistory& source )
{
	m_dQty += source.m_dQty;
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CEposReportConsolCustomerHistory::Compare( CEposReportConsolCustomerHistory& source, int nHint )
{
	if ( m_nTransactionNo != source.m_nTransactionNo )
		return ( m_nTransactionNo > source.m_nTransactionNo ? 1 : -1 );

	if ( m_nReportLineType != source.m_nReportLineType )
		return ( m_nReportLineType > source.m_nReportLineType ? 1 : -1 );

	if ( m_nReportLineSubType != source.m_nReportLineSubType )
		return ( m_nReportLineSubType > source.m_nReportLineSubType ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

