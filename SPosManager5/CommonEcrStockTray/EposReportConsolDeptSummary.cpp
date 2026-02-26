 /**********************************************************************/
#include "EposReportConsolDeptSummary.h"
/**********************************************************************/

CEposReportConsolDeptSummary::CEposReportConsolDeptSummary()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolDeptSummary::Reset()
{
	m_nDeptNo = -1;
	m_nTaxBand = 0;
	m_nTaxDateCode = 0;
	/*****/
	m_dQty = 0.0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolDeptSummary::Add (CEposReportConsolDeptSummary& source )
{
	m_dQty += source.m_dQty;
	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolDeptSummary::Compare( CEposReportConsolDeptSummary& source, int nHint )
{
	int nSortDeptNoThis		= ( 0 == m_nDeptNo ) ? 1000 : m_nDeptNo;
	int nSortDeptNoSource	= ( 0 == source.m_nDeptNo ) ? 1000 : source.m_nDeptNo;

	if ( nSortDeptNoThis != nSortDeptNoSource )
		return ( ( nSortDeptNoThis > nSortDeptNoSource ) ? 1 : -1 );
	
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

CEposReportConsolDeptSummaryTax::CEposReportConsolDeptSummaryTax()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolDeptSummaryTax::Reset()
{
	m_nDeptNo = -1;
	/*****/
	m_dQty = 0.0;
	m_dTaxAmount = 0.0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolDeptSummaryTax::Add (CEposReportConsolDeptSummary& source )
{
	m_dQty += source.m_dQty;
	BaseAdd(source);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportDeptSummaryBlock::CEposReportDeptSummaryBlock()
{
	m_pSales = NULL;
}

/**********************************************************************/

bool CEposReportDeptSummaryBlock::GotData()
{
	if (m_pSales->GetSize() != 0)
	{
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/
