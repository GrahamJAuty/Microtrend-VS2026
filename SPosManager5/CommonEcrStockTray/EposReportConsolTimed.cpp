/**********************************************************************/
#include "EposReportConsolTimed.h"
/**********************************************************************/

CEposReportConsolTimed::CEposReportConsolTimed()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolTimed::Reset()
{
	m_strDateOrTime = "";
	m_nTaxDateCode = 0;
	m_nTaxBand = 0;
	m_nTranCount = 0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolTimed::Add (CEposReportConsolTimed& source )
{
	m_nTranCount += source.m_nTranCount;
	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolTimed::Compare( CEposReportConsolTimed& source, int nHint )
{
	if ( m_strDateOrTime != source.m_strDateOrTime )
		return ( ( m_strDateOrTime > source.m_strDateOrTime ) ? 1 : -1 );
	
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

CEposReportConsolTimedTax::CEposReportConsolTimedTax()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolTimedTax::Reset()
{
	m_strDateOrTime = "";
	m_nTranCount = 0;
	m_dTaxAmount = 0.0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolTimedTax::Add (CEposReportConsolTimed& source )
{
	m_nTranCount += source.m_nTranCount;
	BaseAdd(source);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CEposReportTimedBlock::GotData()
{
	if ( m_pSales -> GetSize() != 0 )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

