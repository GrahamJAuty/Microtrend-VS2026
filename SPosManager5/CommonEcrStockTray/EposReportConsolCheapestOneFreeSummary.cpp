/**********************************************************************/
 
/**********************************************************************/
#include "EposReportConsolCheapestOneFreeSummary.h"
/**********************************************************************/

CEposReportConsolCheapestOneFreeSummary::CEposReportConsolCheapestOneFreeSummary()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolCheapestOneFreeSummary::Reset()
{
	m_nMixMatchNo = 0;
	m_nSubtotalFlag = 0;
	m_nPluNo = 0;
	m_nQty = 0;
	m_dVal = 0.0;
}

/**********************************************************************/

void CEposReportConsolCheapestOneFreeSummary::Add ( CEposReportConsolCheapestOneFreeSummary& source )
{
	m_nQty += source.m_nQty;
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CEposReportConsolCheapestOneFreeSummary::Compare( CEposReportConsolCheapestOneFreeSummary& source, int nHint )
{
	if ( m_nMixMatchNo != source.m_nMixMatchNo )
		return ( m_nMixMatchNo > source.m_nMixMatchNo ? 1 : -1 );

	if ( m_nSubtotalFlag != source.m_nSubtotalFlag )
		return ( m_nSubtotalFlag > source.m_nSubtotalFlag ? 1 : -1 );

	if ( m_nPluNo != source.m_nPluNo )
		return ( m_nPluNo > source.m_nPluNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CEposReportCheapestOneFreeSummaryBlock::GotData()
{
	return ( m_pPromos -> GetSize() != 0 );
}

/**********************************************************************/
