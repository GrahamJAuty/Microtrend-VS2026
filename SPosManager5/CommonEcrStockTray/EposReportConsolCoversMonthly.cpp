 /**********************************************************************/
#include "EposReportConsolCoversMonthly.h"
/**********************************************************************/

CEposReportConsolCoversMonthly::CEposReportConsolCoversMonthly()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolCoversMonthly::Reset()
{
	m_nYear = 2001;
	m_nMonth = 1;
	m_nQty = 0;
}

/**********************************************************************/

void CEposReportConsolCoversMonthly::Add ( CEposReportConsolCoversMonthly& source )
{
	m_nQty += source.m_nQty;
}

/**********************************************************************/

int CEposReportConsolCoversMonthly::Compare( CEposReportConsolCoversMonthly& source, int nHint )
{
	if ( m_nYear != source.m_nYear )
		return ( m_nYear > source.m_nYear ? 1 : -1 );

	if ( m_nMonth != source.m_nMonth )
		return ( m_nMonth > source.m_nMonth ? 1 : -1 );
	else
		return 0;
}


/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportCoversMonthlyBlock::CEposReportCoversMonthlyBlock()
{
	m_nCovers = 0;
	m_pSales = NULL;
}

/**********************************************************************/

bool CEposReportCoversMonthlyBlock::GotData()
{
	if ( m_pSales -> GetSize() != 0 )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

