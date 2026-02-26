/**********************************************************************/
#include "EposReportConsolPluSalesDept.h"
/**********************************************************************/

CEposReportConsolPluSalesDept::CEposReportConsolPluSalesDept()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolPluSalesDept::Reset()
{
	m_nDeptNo = -1;
	m_nPluNo = -1;
	m_nTaxBand = 0;
	m_nTaxDateCode = -1;
	m_nLocIdx = -1;
	m_dQty = 0.0;
	m_dItemCost = 0.0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolPluSalesDept::Add (CEposReportConsolPluSalesDept& source )
{
	m_dQty += source.m_dQty;
	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolPluSalesDept::Compare( CEposReportConsolPluSalesDept& source, int nHint )
{
	int nSortDeptNoThis		= ( 0 == m_nDeptNo ) ? 1000 : m_nDeptNo;
	int nSortDeptNoSource	= ( 0 == source.m_nDeptNo ) ? 1000 : source.m_nDeptNo;

	if ( nSortDeptNoThis != nSortDeptNoSource )
		return ( ( nSortDeptNoThis > nSortDeptNoSource ) ? 1 : -1 );

	if ( m_nPluNo != source.m_nPluNo )
		return ( ( m_nPluNo > source.m_nPluNo ) ? 1 : -1 );

	if ( m_nTaxBand != source.m_nTaxBand )
		return ( ( m_nTaxBand > source.m_nTaxBand ) ? 1 : -1 );

	if ( m_nTaxDateCode != source.m_nTaxDateCode )
		return ( ( m_nTaxDateCode > source.m_nTaxDateCode ) ? 1 : -1 );

	if ( m_nLocIdx != source.m_nLocIdx )
		return ( ( m_nLocIdx > source.m_nLocIdx ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

bool CEposReportConsolPluSalesDept::ComparePluNoTax( CEposReportConsolPluSalesDept& source )
{
	if ( m_nPluNo != source.m_nPluNo )
		return FALSE;

	if ( m_nTaxBand != source.m_nTaxBand )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

bool CEposReportConsolPluSalesDept::CompareDeptNo( CEposReportConsolPluSalesDept& source )
{
	return ( m_nDeptNo == source.m_nDeptNo );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportPluSalesDeptBlock::CEposReportPluSalesDeptBlock()
{
	m_nCurrentPluNo = 0;
	m_pSales = NULL;
	m_pItems = NULL;
}

/**********************************************************************/

bool CEposReportPluSalesDeptBlock::GotData()
{
	if ( ( m_pSales -> GetSize() != 0 ) || ( m_pItems -> GetSize() != 0 ) )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

