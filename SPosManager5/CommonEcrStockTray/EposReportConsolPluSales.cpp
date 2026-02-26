/**********************************************************************/
#include "EposReportConsolPluSales.h"
/**********************************************************************/

CEposReportConsolPluSalesBase::CEposReportConsolPluSalesBase()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolPluSalesBase::Reset()
{
	m_nPluNo = -1;
	m_bDummyItem = FALSE;
	m_nDeptNo = -1;
	m_nPriceBand = -1;
	m_nTaxBand = 0;
	m_nTaxDateCode = -1;
	m_nLocIdx = -1;
	m_dQty = 0.0;
	m_dItemCost = 0.0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolPluSalesBase::Add(CEposReportConsolPluSalesBase& source)
{
	m_dQty += source.m_dQty;
	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolPluSalesBase::Compare( CEposReportConsolPluSalesBase& source, int nHint )
{
	int nSortDeptNoThis		= ( 0 == m_nDeptNo ) ? 1000 : m_nDeptNo;
	int nSortDeptNoSource	= ( 0 == source.m_nDeptNo ) ? 1000 : source.m_nDeptNo;

	if ( m_nPluNo != source.m_nPluNo )
		return ( ( m_nPluNo > source.m_nPluNo ) ? 1 : -1 );

	if ( m_bDummyItem != source.m_bDummyItem )
		return ( m_bDummyItem ) ? 1 : -1;

	if ( nSortDeptNoThis != nSortDeptNoSource )
		return ( ( nSortDeptNoThis > nSortDeptNoSource ) ? 1 : -1 );

	if ( m_nTaxBand != source.m_nTaxBand )
		return ( ( m_nTaxBand > source.m_nTaxBand ) ? 1 : -1 );

	if ( m_nPriceBand != source.m_nPriceBand )
		return ( ( m_nPriceBand > source.m_nPriceBand ) ? 1 : -1 );

	if ( m_nTaxDateCode != source.m_nTaxDateCode )
		return ( ( m_nTaxDateCode > source.m_nTaxDateCode ) ? 1 : -1 );

	if ( m_nLocIdx != source.m_nLocIdx )
		return ( ( m_nLocIdx > source.m_nLocIdx ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

bool CEposReportConsolPluSalesBase::ComparePluDeptTax( CEposReportConsolPluSalesBase& source )
{
	if ( m_nPluNo != source.m_nPluNo )
		return FALSE;

	if ( m_bDummyItem != source.m_bDummyItem )
		return FALSE;

	if ( m_nDeptNo != source.m_nDeptNo )
		return FALSE;

	if ( m_nTaxBand != source.m_nTaxBand )
		return FALSE;

	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportPluSalesBlock::CEposReportPluSalesBlock()
{
	m_nCurrentPluNo = 0;
	m_nCurrentDeptNo = 0;
	m_nCovers = 0;
	m_pSales = NULL;
	m_pItems = NULL;
}

/**********************************************************************/

bool CEposReportPluSalesBlock::GotData()
{
	if ( ( m_pSales -> GetSize() != 0 ) || ( m_pItems -> GetSize() != 0 ) )
		return TRUE;

	return FALSE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CEposReportVIPSalesBlock::GotData()
{
	if ( ( m_pSales -> GetSize() != 0 ) || ( m_pItems -> GetSize() != 0 ) )
		return TRUE;

	return FALSE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolPluByPriceBandBase::CEposReportConsolPluByPriceBandBase()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolPluByPriceBandBase::Reset()
{
	m_nPluNo = -1;
	m_bDummyItem = FALSE;
	m_nDeptNo = -1;
	m_nTaxBand = 0;
	m_nTaxDateCode = -1;
	m_nLocIdx = -1;
	m_dTotalQty = 0.0;
	m_dItemCost = 0.0;

	for (int n = 0; n < 10; n++)
	{
		m_dPriceBandQty[n] = 0.0;
		m_dPriceBandVal[n] = 0.0;
	}

	BaseReset();
}

/**********************************************************************/

void CEposReportConsolPluByPriceBandBase::Add(CEposReportConsolPluByPriceBandBase& source)
{
	m_dTotalQty += source.m_dTotalQty;

	for (int n = 0; n < 10; n++)
	{
		m_dPriceBandQty[n] += source.m_dPriceBandQty[n];
		m_dPriceBandVal[n] += source.m_dPriceBandVal[n];
	}

	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolPluByPriceBandBase::Compare(CEposReportConsolPluByPriceBandBase& source, int nHint)
{
	int nSortDeptNoThis = (0 == m_nDeptNo) ? 1000 : m_nDeptNo;
	int nSortDeptNoSource = (0 == source.m_nDeptNo) ? 1000 : source.m_nDeptNo;

	if (m_nPluNo != source.m_nPluNo)
		return ((m_nPluNo > source.m_nPluNo) ? 1 : -1);

	if (m_bDummyItem != source.m_bDummyItem)
		return (m_bDummyItem) ? 1 : -1;

	if (nSortDeptNoThis != nSortDeptNoSource)
		return ((nSortDeptNoThis > nSortDeptNoSource) ? 1 : -1);

	if (m_nTaxBand != source.m_nTaxBand)
		return ((m_nTaxBand > source.m_nTaxBand) ? 1 : -1);

	if (m_nTaxDateCode != source.m_nTaxDateCode)
		return ((m_nTaxDateCode > source.m_nTaxDateCode) ? 1 : -1);

	if (m_nLocIdx != source.m_nLocIdx)
		return ((m_nLocIdx > source.m_nLocIdx) ? 1 : -1);
	else
		return 0;
}

/**********************************************************************/

bool CEposReportConsolPluByPriceBandBase::ComparePluDeptTax(CEposReportConsolPluByPriceBandBase& source)
{
	if (m_nPluNo != source.m_nPluNo)
		return FALSE;

	if (m_bDummyItem != source.m_bDummyItem)
		return FALSE;

	if (m_nDeptNo != source.m_nDeptNo)
		return FALSE;

	if (m_nTaxBand != source.m_nTaxBand)
		return FALSE;

	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportPluByPriceBandBlock::CEposReportPluByPriceBandBlock()
{
	m_nCurrentPluNo = 0;
	m_nCurrentDeptNo = 0;
	m_nCovers = 0;
	m_pSales = NULL;
	m_pItems = NULL;
}

/**********************************************************************/

bool CEposReportPluByPriceBandBlock::GotData()
{
	if ((m_pSales->GetSize() != 0) || (m_pItems->GetSize() != 0))
	{
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolPluSalesCustomerID::CEposReportConsolPluSalesCustomerID()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolPluSalesCustomerID::Reset()
{
	m_nLocIdx = -1;
	m_nSortType = 0;
	m_nCustomerID = -1;
	m_nPluNo = -1;
	m_nDeptNo = -1;
	m_nPriceBand = -1;
	m_nTaxBand = 0;
	m_nTaxDateCode = -1;
	m_dQty = 0.0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolPluSalesCustomerID::Add (CEposReportConsolPluSalesCustomerID& source )
{
	m_dQty += source.m_dQty;
	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolPluSalesCustomerID::Compare( CEposReportConsolPluSalesCustomerID& source, int nHint )
{
	int nSortDeptNoThis		= ( 0 == m_nDeptNo ) ? 1000 : m_nDeptNo;
	int nSortDeptNoSource	= ( 0 == source.m_nDeptNo ) ? 1000 : source.m_nDeptNo;

	if ( m_nLocIdx != source.m_nLocIdx )
		return ( ( m_nLocIdx > source.m_nLocIdx ) ? 1 : -1 );

	if ( m_nSortType != source.m_nSortType )
		return ( ( m_nSortType > source.m_nSortType ) ? 1 : -1 );

	if ( m_nCustomerID != source.m_nCustomerID )
		return ( ( m_nCustomerID > source.m_nCustomerID ) ? 1 : -1 );

	if ( m_nPluNo != source.m_nPluNo )
		return ( ( m_nPluNo > source.m_nPluNo ) ? 1 : -1 );

	if ( nSortDeptNoThis != nSortDeptNoSource )
		return ( ( nSortDeptNoThis > nSortDeptNoSource ) ? 1 : -1 );

	if ( m_nTaxBand != source.m_nTaxBand )
		return ( ( m_nTaxBand > source.m_nTaxBand ) ? 1 : -1 );

	if ( m_nPriceBand != source.m_nPriceBand )
		return ( ( m_nPriceBand > source.m_nPriceBand ) ? 1 : -1 );

	if ( m_nTaxDateCode != source.m_nTaxDateCode )
		return ( ( m_nTaxDateCode > source.m_nTaxDateCode ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolPluSalesCustomerSortIdx::CEposReportConsolPluSalesCustomerSortIdx()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolPluSalesCustomerSortIdx::Reset()
{
	m_nLocSortIdx = -1;
	m_nSortIdxType = -1;
	m_nSortIdxNum = -1;
	m_nCustomerID = -1;
	m_nPluNo = -1;
	m_nDeptNo = -1;
	m_nPriceBand = -1;
	m_nTaxBand = 0;
	m_nTaxDateCode = -1;
	m_nLocIdx = -1;
	m_dQty = 0.0;
	m_dItemCost = 0.0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolPluSalesCustomerSortIdx::Add (CEposReportConsolPluSalesCustomerSortIdx& source )
{
	m_dQty += source.m_dQty;
	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolPluSalesCustomerSortIdx::Compare( CEposReportConsolPluSalesCustomerSortIdx& source, int nHint )
{
	int nSortDeptNoThis		= ( 0 == m_nDeptNo ) ? 1000 : m_nDeptNo;
	int nSortDeptNoSource	= ( 0 == source.m_nDeptNo ) ? 1000 : source.m_nDeptNo;

	if ( m_nLocSortIdx != source.m_nLocSortIdx )
		return ( ( m_nLocSortIdx > source.m_nLocSortIdx ) ? 1 : -1 );

	if ( m_nSortIdxType != source.m_nSortIdxType )
		return ( ( m_nSortIdxType > source.m_nSortIdxType ) ? 1 : -1 );

	if ( m_nSortIdxNum != source.m_nSortIdxNum )
		return ( ( m_nSortIdxNum > source.m_nSortIdxNum ) ? 1 : -1 );

	if ( m_nCustomerID != source.m_nCustomerID )
		return ( ( m_nCustomerID > source.m_nCustomerID ) ? 1 : -1 );

	if ( m_nPluNo != source.m_nPluNo )
		return ( ( m_nPluNo > source.m_nPluNo ) ? 1 : -1 );

	if ( nSortDeptNoThis != nSortDeptNoSource )
		return ( ( nSortDeptNoThis > nSortDeptNoSource ) ? 1 : -1 );

	if ( m_nTaxBand != source.m_nTaxBand )
		return ( ( m_nTaxBand > source.m_nTaxBand ) ? 1 : -1 );

	if ( m_nPriceBand != source.m_nPriceBand )
		return ( ( m_nPriceBand > source.m_nPriceBand ) ? 1 : -1 );

	if ( m_nTaxDateCode != source.m_nTaxDateCode )
		return ( ( m_nTaxDateCode > source.m_nTaxDateCode ) ? 1 : -1 );
	
	if ( m_nLocIdx != source.m_nLocIdx )
		return ( ( m_nLocIdx > source.m_nLocIdx ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

bool CEposReportConsolPluSalesCustomerSortIdx::CompareLocation ( CEposReportConsolPluSalesCustomerSortIdx& source )
{
	if ( m_nLocSortIdx != source.m_nLocSortIdx )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

bool CEposReportConsolPluSalesCustomerSortIdx::CompareCustomer ( CEposReportConsolPluSalesCustomerSortIdx& source )
{
	if ( m_nLocSortIdx != source.m_nLocSortIdx )
		return FALSE;

	if ( m_nSortIdxType != source.m_nSortIdxType )
		return FALSE;

	if ( m_nSortIdxNum != source.m_nSortIdxNum )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

bool CEposReportConsolPluSalesCustomerSortIdx::ComparePluDeptTax( CEposReportConsolPluSalesCustomerSortIdx& source )
{
	if ( m_nLocSortIdx != source.m_nLocSortIdx )
		return FALSE;

	if ( m_nSortIdxType != source.m_nSortIdxType )
		return FALSE;

	if ( m_nSortIdxNum != source.m_nSortIdxNum )
		return FALSE;

	if ( m_nCustomerID != source.m_nCustomerID )
		return FALSE;

	if ( m_nPluNo != source.m_nPluNo )
		return FALSE;

	if ( m_nDeptNo != source.m_nDeptNo )
		return FALSE;

	if ( m_nTaxBand != source.m_nTaxBand )
		return FALSE;

	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/


