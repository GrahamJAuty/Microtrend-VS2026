/**********************************************************************/
#include "EposReportConsolPaymentByTaxBand.h"
/**********************************************************************/

CEposReportConsolPBT_MainBlock::CEposReportConsolPBT_MainBlock()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolPBT_MainBlock::Reset()
{
	m_nPaymentNo = 0;
	m_bTotal = FALSE;
	m_bPushDown = FALSE;
	m_nTaxBand = 0;
	m_dVal = 0.0;
	m_dTax = 0.0;
}

/**********************************************************************/

void CEposReportConsolPBT_MainBlock::Add ( CEposReportConsolPBT_MainBlock& source )
{
	m_dVal += source.m_dVal;
	m_dTax += source.m_dTax;
}

/**********************************************************************/

int CEposReportConsolPBT_MainBlock::Compare( CEposReportConsolPBT_MainBlock& source, int nHint )
{
	if ( m_nPaymentNo != source.m_nPaymentNo )
		return ( m_nPaymentNo > source.m_nPaymentNo ? 1 : -1 );

	if ( m_bTotal != source.m_bTotal )
		return ( TRUE == m_bTotal ) ? 1 : -1;

	if ( TRUE == m_bTotal )
		return 0;

	if ( m_bPushDown != source.m_bPushDown )
		return ( TRUE == m_bPushDown ) ? 1 : -1;

	if ( m_nTaxBand != source.m_nTaxBand )
		return ( m_nTaxBand > source.m_nTaxBand ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolPBT_TotalBlock::CEposReportConsolPBT_TotalBlock()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolPBT_TotalBlock::Reset()
{
	m_nBlockType = PBT_TOTALBLOCK_PAYMENT;
	m_nTaxBand = 0;
	m_dVal = 0.0;
	m_dTax = 0.0;
}

/**********************************************************************/

void CEposReportConsolPBT_TotalBlock::Add ( CEposReportConsolPBT_TotalBlock& source )
{
	m_dVal += source.m_dVal;
	m_dTax += source.m_dTax;
}

/**********************************************************************/

int CEposReportConsolPBT_TotalBlock::Compare( CEposReportConsolPBT_TotalBlock& source, int nHint )
{
	if ( m_nBlockType != source.m_nBlockType )
		return ( m_nBlockType > source.m_nBlockType ? 1 : -1 );

	if ( m_nTaxBand != source.m_nTaxBand )
		return ( m_nTaxBand > source.m_nTaxBand ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPaymentByTaxBandBufferPayment::CPaymentByTaxBandBufferPayment()
{
	Reset();
}

/**********************************************************************/

void CPaymentByTaxBandBufferPayment::Reset()
{
	m_nPaymentNo = 0;
	m_dVal = 0.0;
}

/**********************************************************************/

void CPaymentByTaxBandBufferPayment::Add ( CPaymentByTaxBandBufferPayment& source )
{
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CPaymentByTaxBandBufferPayment::Compare( CPaymentByTaxBandBufferPayment& source, int nHint )
{
	if ( m_nPaymentNo != source.m_nPaymentNo )
		return ( m_nPaymentNo > source.m_nPaymentNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPaymentByTaxBandBufferSale::CPaymentByTaxBandBufferSale()
{
	Reset();
}

/**********************************************************************/

void CPaymentByTaxBandBufferSale::Reset()
{
	m_nTaxBand = 0;
	m_bFiltered = FALSE;
	m_dVal = 0.0;
	m_dTax = 0.0;
}

/**********************************************************************/

void CPaymentByTaxBandBufferSale::Add ( CPaymentByTaxBandBufferSale& source )
{
	m_dVal += source.m_dVal;
	m_dTax += source.m_dTax;
}

/**********************************************************************/

int CPaymentByTaxBandBufferSale::Compare( CPaymentByTaxBandBufferSale& source, int nHint )
{
	if ( m_nTaxBand != source.m_nTaxBand )
		return ( m_nTaxBand > source.m_nTaxBand ? 1 : -1 );

	if ( m_bFiltered != source.m_bFiltered )
		return ( TRUE == m_bFiltered ) ? 1 : -1;
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CEposReportPaymentByTaxBandBlock::GotData()
{
	if ( m_pPayments -> GetSize() != 0 )
		return TRUE;

	if ( m_pTotals -> GetSize() != 0 )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

__int64 CEposReportPaymentByTaxBandBlock::GetLineCount()
{
	__int64 nLineCount = 0;
	
	nLineCount += m_pPayments -> GetSize();
	nLineCount += m_pTotals -> GetSize();
	
	return nLineCount;
}

/**********************************************************************/
