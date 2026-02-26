/**********************************************************************/
#include "EposReportConsolCoversDataPoint.h"
/**********************************************************************/

CEposReportConsolCoversDataPoint::CEposReportConsolCoversDataPoint()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolCoversDataPoint::Reset()
{
	m_nDataPoint = 0;
	m_nTaxDateCode = 0;
	m_nTaxBand = 0;
	m_nCoversCount = 0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolCoversDataPoint::Add (CEposReportConsolCoversDataPoint& source )
{
	m_nCoversCount += source.m_nCoversCount;
	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolCoversDataPoint::Compare( CEposReportConsolCoversDataPoint& source, int nHint )
{
	if ( m_nDataPoint != source.m_nDataPoint )
		return ( ( m_nDataPoint > source.m_nDataPoint ) ? 1 : -1 );

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

CEposReportConsolCoversDataPointTax::CEposReportConsolCoversDataPointTax()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolCoversDataPointTax::Reset()
{
	m_nDataPoint = 0;
	m_nCoversCount = 0;
	m_dTaxAmount = 0.0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolCoversDataPointTax::Add (CEposReportConsolCoversDataPoint& source )
{
	m_nCoversCount += source.m_nCoversCount;
	BaseAdd(source);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolCoversAdjust::CEposReportConsolCoversAdjust()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolCoversAdjust::Reset()
{
	m_nTaxDateCode = 0;
	m_nTaxBand = 0;
	m_dValue = 0.0;
}

/**********************************************************************/

void CEposReportConsolCoversAdjust::Add (CEposReportConsolCoversAdjust& source )
{
	m_dValue += source.m_dValue;
}

/**********************************************************************/

int CEposReportConsolCoversAdjust::Compare( CEposReportConsolCoversAdjust& source, int nHint )
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

CEposReportCoversDataPointBlock::CEposReportCoversDataPointBlock()
{
	m_nCoversCount = 0;
	m_pSales = NULL;
	m_pAdjustPremium = NULL;
	m_pAdjustDiscount = NULL;
}

/**********************************************************************/

bool CEposReportCoversDataPointBlock::GotData()
{
	if ( m_pSales -> GetSize() != 0 )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

