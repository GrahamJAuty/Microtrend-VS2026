/**********************************************************************/
#include "EposReportConsolPluSalesTax.h"
/**********************************************************************/

CEposReportConsolPluSalesTax::CEposReportConsolPluSalesTax()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolPluSalesTax::Reset()
{
	m_nTaxBand = 0;
	m_nTaxDateCode = -1;
	m_nPluNo = -1;
	m_nLocIdx = -1;
	m_dQty = 0.0;
	m_dItemCost = 0.0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolPluSalesTax::Add (CEposReportConsolPluSalesTax& source )
{
	m_dQty += source.m_dQty;
	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolPluSalesTax::Compare( CEposReportConsolPluSalesTax& source, int nHint )
{
	if ( m_nTaxBand != source.m_nTaxBand )
		return ( ( m_nTaxBand > source.m_nTaxBand ) ? 1 : -1 );

	if ( m_nTaxDateCode != source.m_nTaxDateCode )
		return ( ( m_nTaxDateCode > source.m_nTaxDateCode ) ? 1 : -1 );

	if ( m_nPluNo != source.m_nPluNo )
		return ( ( m_nPluNo > source.m_nPluNo ) ? 1 : -1 );

	if ( m_nLocIdx != source.m_nLocIdx )
		return ( ( m_nLocIdx > source.m_nLocIdx ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

bool CEposReportConsolPluSalesTax::ComparePluNoTax( CEposReportConsolPluSalesTax& source )
{
	if ( m_nPluNo != source.m_nPluNo )
		return FALSE;

	if ( m_nTaxBand != source.m_nTaxBand )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

bool CEposReportConsolPluSalesTax::CompareTaxBand( CEposReportConsolPluSalesTax& source )
{
	return ( m_nTaxBand == source.m_nTaxBand );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportPluSalesTaxBlock::CEposReportPluSalesTaxBlock()
{
	m_nCurrentPluNo = 0;
	m_pSales = NULL;
	m_pItems = NULL;
}

/**********************************************************************/

bool CEposReportPluSalesTaxBlock::GotData()
{
	if ((m_pSales->GetSize() != 0) || (m_pItems->GetSize() != 0))
	{
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

