/**********************************************************************/
#include "EposReportConsolCoversDaily.h"
/**********************************************************************/

CEposReportConsolCoversDaily::CEposReportConsolCoversDaily()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolCoversDaily::Reset()
{
	m_strBusinessDate = "";
	m_nTaxDateCode = 0;
	m_nTaxBand = 0;
	m_nCoversCount = 0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolCoversDaily::Add (CEposReportConsolCoversDaily& source )
{
	m_nCoversCount += source.m_nCoversCount;
	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolCoversDaily::Compare( CEposReportConsolCoversDaily& source, int nHint )
{
	if ( m_strBusinessDate != source.m_strBusinessDate )
		return ( ( m_strBusinessDate > source.m_strBusinessDate ) ? 1 : -1 );
	
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

CEposReportConsolCoversDailyTax::CEposReportConsolCoversDailyTax()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolCoversDailyTax::Reset()
{
	m_strBusinessDate = "";
	m_nCoversCount = 0;
	m_dTaxAmount = 0.0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolCoversDailyTax::Add (CEposReportConsolCoversDaily& source )
{
	m_nCoversCount += source.m_nCoversCount;
	BaseAdd(source);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportCoversDailyBlock::CEposReportCoversDailyBlock()
{
	m_nCoversCount = 0;
	m_pSales = NULL;
}

/**********************************************************************/

bool CEposReportCoversDailyBlock::GotData()
{
	if ( m_pSales -> GetSize() != 0 )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

