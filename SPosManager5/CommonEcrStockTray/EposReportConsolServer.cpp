/**********************************************************************/
#include "EposReportConsolServer.h"
/**********************************************************************/

CEposReportConsolServer::CEposReportConsolServer()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolServer::Reset()
{
	m_nServerLocOrDbIdx = 0;
	m_nServerNo = 0;
	m_nTaxBand = 0;
	m_nTaxDateCode = 0;
	/*****/
	m_nTranCount = 0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolServer::Add (CEposReportConsolServer& source )
{
	m_nTranCount += source.m_nTranCount;
	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolServer::Compare( CEposReportConsolServer& source, int nHint )
{
	if ( m_nServerLocOrDbIdx != source.m_nServerLocOrDbIdx )
		return ( m_nServerLocOrDbIdx > source.m_nServerLocOrDbIdx ? 1 : -1 );

	if ( m_nServerNo != source.m_nServerNo )
		return ( m_nServerNo > source.m_nServerNo ? 1 : -1 );

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

CEposReportConsolServerTax::CEposReportConsolServerTax()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolServerTax::Reset()
{
	m_nServerLocOrDbIdx = 0;
	m_nServerNo = 0;
	/*****/
	m_nTranCount = 0;
	m_dTaxAmount = 0.0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolServerTax::Add (CEposReportConsolServer& source )
{
	m_nTranCount += source.m_nTranCount;
	BaseAdd(source);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CEposReportServerBlock::GotData()
{
	if ( m_pSales -> GetSize() != 0 )
		return TRUE;

	return FALSE;
}

/**********************************************************************/


