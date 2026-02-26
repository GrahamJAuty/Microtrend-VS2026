/**********************************************************************/
#include "EposReportConsolTaxSummaryPrice.h"
/**********************************************************************/

CEposReportConsolTaxSummaryPrice::CEposReportConsolTaxSummaryPrice()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolTaxSummaryPrice::Reset()
{
	m_nPriceLevel = 0;
	m_nTaxBand = 0;
	m_nTaxDateCode = 0;
	m_dQty = 0.0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolTaxSummaryPrice::Add (CEposReportConsolTaxSummaryPrice& source )
{
	m_dQty += source.m_dQty;
	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolTaxSummaryPrice::Compare( CEposReportConsolTaxSummaryPrice& source, int nHint )
{
	if ( m_nPriceLevel != source.m_nPriceLevel )
		return ( ( m_nPriceLevel > source.m_nPriceLevel ) ? 1 : -1 );

	if ( m_nTaxBand != source.m_nTaxBand )
		return ( ( m_nTaxBand > source.m_nTaxBand ) ? 1 : -1 );

	if ( m_nTaxDateCode != source.m_nTaxDateCode )
		return ( ( m_nTaxDateCode > source.m_nTaxDateCode ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolTaxSummaryPriceItem::CEposReportConsolTaxSummaryPriceItem()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolTaxSummaryPriceItem::Reset()
{
	m_nPriceLevel = 0;
	m_nPluNo = 0;
	m_nModifier = 0;
	m_nTaxDateCode = 0;
	m_dQty = 0.0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolTaxSummaryPriceItem::Add (CEposReportConsolTaxSummaryPriceItem& source )
{
	m_dQty += source.m_dQty;
	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolTaxSummaryPriceItem::Compare( CEposReportConsolTaxSummaryPriceItem& source, int nHint )
{
	if ( m_nPluNo != source.m_nPluNo )
		return ( ( m_nPluNo > source.m_nPluNo ) ? 1 : -1 );

	if ( m_nModifier != source.m_nModifier )
		return ( ( m_nModifier > source.m_nModifier ) ? 1 : -1 );

	if ( m_nPriceLevel != source.m_nPriceLevel )
		return ( ( m_nPriceLevel > source.m_nPriceLevel ) ? 1 : -1 );

	if ( m_nTaxDateCode != source.m_nTaxDateCode )
		return ( ( m_nTaxDateCode > source.m_nTaxDateCode ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CEposReportTaxSummaryPriceBlock::GotData()
{
	if ( ( m_pSales -> GetSize() != 0 ) || ( m_pItem -> GetSize() != 0 ) )
		return TRUE;

	return FALSE;
}

/**********************************************************************/
