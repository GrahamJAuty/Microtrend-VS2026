/**********************************************************************/
#include "EposReportConsolPluSalesModifier.h"
/**********************************************************************/

CEposReportConsolPluSalesModifier::CEposReportConsolPluSalesModifier()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolPluSalesModifier::Reset()
{
	m_nPluNo = -1;
	m_nModifier = -1;
	m_nTaxBand = 0;
	m_nTaxDateCode = -1;
	m_nPriceBand = -1;
	m_nLocIdx = -1;
	m_dQty = 0.0;
	m_dItemCost = 0.0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolPluSalesModifier::Add (CEposReportConsolPluSalesModifier& source )
{
	m_dQty += source.m_dQty;
	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolPluSalesModifier::Compare( CEposReportConsolPluSalesModifier& source, int nHint )
{
	if ( m_nPluNo != source.m_nPluNo )
		return ( ( m_nPluNo > source.m_nPluNo ) ? 1 : -1 );

	if ( m_nModifier != source.m_nModifier )
		return ( ( m_nModifier > source.m_nModifier ) ? 1 : -1 );

	if ( m_nTaxBand != source.m_nTaxBand )
		return ( ( m_nTaxBand > source.m_nTaxBand ) ? 1 : -1 );

	if ( m_nTaxDateCode != source.m_nTaxDateCode )
		return ( ( m_nTaxDateCode > source.m_nTaxDateCode ) ? 1 : -1 );

	if ( m_nPriceBand != source.m_nPriceBand )
		return ( ( m_nPriceBand > source.m_nPriceBand ) ? 1 : -1 );

	if ( m_nLocIdx != source.m_nLocIdx )
		return ( ( m_nLocIdx > source.m_nLocIdx ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

bool CEposReportConsolPluSalesModifier::ComparePlu( CEposReportConsolPluSalesModifier& source )
{
	if ( m_nPluNo != source.m_nPluNo )
		return FALSE;

	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportPluSalesModifierBlock::CEposReportPluSalesModifierBlock()
{
	m_nCurrentPluNo = 0;
	m_pSales = NULL;
	m_pItems = NULL;
}

/**********************************************************************/

bool CEposReportPluSalesModifierBlock::GotData()
{
	if ((m_pSales->GetSize() != 0) || (m_pItems->GetSize() != 0))
	{
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

