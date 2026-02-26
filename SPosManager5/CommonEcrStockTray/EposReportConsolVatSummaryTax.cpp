/**********************************************************************/
#include "EposReportConsolTaxSummaryTax.h"
/**********************************************************************/

CEposReportConsolTaxSummaryTax::CEposReportConsolTaxSummaryTax()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolTaxSummaryTax::Reset()
{
	m_nTaxBand = 0;
	m_nTaxDateCode = 0;
	m_dQty = 0.0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolTaxSummaryTax::Add (CEposReportConsolTaxSummaryTax& source )
{
	m_dQty += source.m_dQty;
	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolTaxSummaryTax::Compare( CEposReportConsolTaxSummaryTax& source, int nHint )
{
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

CEposReportConsolTaxSummaryTaxItem::CEposReportConsolTaxSummaryTaxItem()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolTaxSummaryTaxItem::Reset()
{
	m_nPluNo = 0;
	m_nModifier = 0;
	m_nTaxDateCode = 0;
	m_dQty = 0.0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolTaxSummaryTaxItem::Add (CEposReportConsolTaxSummaryTaxItem& source )
{
	m_dQty += source.m_dQty;
	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolTaxSummaryTaxItem::Compare( CEposReportConsolTaxSummaryTaxItem& source, int nHint )
{
	if ( m_nPluNo != source.m_nPluNo )
		return ( ( m_nPluNo > source.m_nPluNo ) ? 1 : -1 );

	if ( m_nModifier != source.m_nModifier )
		return ( ( m_nModifier > source.m_nModifier ) ? 1 : -1 );

	if ( m_nTaxDateCode != source.m_nTaxDateCode )
		return ( ( m_nTaxDateCode > source.m_nTaxDateCode ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CEposReportTaxSummaryTaxBlock::GotData()
{
	if ( ( m_pSales -> GetSize() != 0 ) || ( m_pItem -> GetSize() != 0 ) )
		return TRUE;

	return FALSE;
}

/**********************************************************************/
