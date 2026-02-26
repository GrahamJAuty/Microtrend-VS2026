/**********************************************************************/
#include "EposReportConsolCoversHourly.h"
/**********************************************************************/

CEposReportConsolCoversHourly::CEposReportConsolCoversHourly()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolCoversHourly::Reset()
{
	m_nDayShift = -1;
	m_strTimeCode = "";
	m_nTaxDateCode = 0;
	m_nTaxBand = 0;
	m_nCoversCount = 0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolCoversHourly::Add (CEposReportConsolCoversHourly& source )
{
	m_nCoversCount += source.m_nCoversCount;
	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolCoversHourly::Compare( CEposReportConsolCoversHourly& source, int nHint )
{
	if ( m_nDayShift != source.m_nDayShift )
		return ( ( m_nDayShift > source.m_nDayShift ) ? 1 : -1 );

	if ( m_strTimeCode != source.m_strTimeCode )
		return ( ( m_strTimeCode > source.m_strTimeCode ) ? 1 : -1 );
	
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

CEposReportConsolCoversHourlyTax::CEposReportConsolCoversHourlyTax()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolCoversHourlyTax::Reset()
{
	m_nDayShift = -1;
	m_strTimeCode = "";
	m_nCoversCount = 0;
	m_dTaxAmount = 0.0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolCoversHourlyTax::Add (CEposReportConsolCoversHourly& source )
{
	m_nCoversCount += source.m_nCoversCount;
	BaseAdd(source);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportCoversHourlyBlock::CEposReportCoversHourlyBlock()
{
	m_nCoversCount = 0;
	m_pSales = NULL;
}

/**********************************************************************/

bool CEposReportCoversHourlyBlock::GotData()
{
	if ( m_pSales -> GetSize() != 0 )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

