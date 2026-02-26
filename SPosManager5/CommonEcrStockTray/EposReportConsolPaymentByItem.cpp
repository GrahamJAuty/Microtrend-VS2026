/**********************************************************************/
#include "EposReportConsolPaymentByItem.h"
/**********************************************************************/

CEposReportConsolPBI_MainBlock::CEposReportConsolPBI_MainBlock()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolPBI_MainBlock::Reset()
{
	m_nPaymentNo = 0;
	m_nPaymentBlock = 0;
	m_bIsZeroItem = FALSE;
	m_nItemNumber = 0;
	m_nTranQty = 0;
	m_dVal = 0.0;
}

/**********************************************************************/

void CEposReportConsolPBI_MainBlock::Add ( CEposReportConsolPBI_MainBlock& source )
{
	m_nTranQty += source.m_nTranQty;
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CEposReportConsolPBI_MainBlock::Compare( CEposReportConsolPBI_MainBlock& source, int nHint )
{
	if ( m_nPaymentNo != source.m_nPaymentNo )
		return ( m_nPaymentNo > source.m_nPaymentNo ? 1 : -1 );

	if ( m_nPaymentBlock != source.m_nPaymentBlock )
		return ( m_nPaymentBlock > source.m_nPaymentBlock ? 1 : -1 );

	if ( m_bIsZeroItem != source.m_bIsZeroItem )
		return ( TRUE == m_bIsZeroItem ) ? 1 : -1;

	if ( m_nItemNumber != source.m_nItemNumber )
		return ( m_nItemNumber > source.m_nItemNumber ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPaymentByItemBufferPayment::CPaymentByItemBufferPayment()
{
	Reset();
}

/**********************************************************************/

void CPaymentByItemBufferPayment::Reset()
{
	m_nPaymentNo = 0;
	m_bIsRedemptionBlock = FALSE;
	m_dVal = 0.0;
}

/**********************************************************************/

void CPaymentByItemBufferPayment::Add ( CPaymentByItemBufferPayment& source )
{
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CPaymentByItemBufferPayment::Compare( CPaymentByItemBufferPayment& source, int nHint )
{
	if ( m_nPaymentNo != source.m_nPaymentNo )
		return ( m_nPaymentNo > source.m_nPaymentNo ? 1 : -1 );

	if ( m_bIsRedemptionBlock != source.m_bIsRedemptionBlock )
		return ( TRUE == m_bIsRedemptionBlock ) ? 1 : -1;
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPaymentByItemBufferSale::CPaymentByItemBufferSale()
{
	Reset();
}

/**********************************************************************/

void CPaymentByItemBufferSale::Reset()
{
	m_nPluNo = 0;
	m_nAuxNo = 0;
	m_dVal = 0.0;
}

/**********************************************************************/

void CPaymentByItemBufferSale::Add ( CPaymentByItemBufferSale& source )
{
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CPaymentByItemBufferSale::Compare( CPaymentByItemBufferSale& source, int nHint )
{
	if ( m_nPluNo != source.m_nPluNo )
		return ( m_nPluNo > source.m_nPluNo ? 1 : -1 );

	if ( m_nAuxNo != source.m_nAuxNo )
		return ( m_nAuxNo > source.m_nAuxNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CEposReportPaymentByItemBlock::GotData()
{
	if ( m_pPaymentsNormal -> GetSize() != 0 )
		return TRUE;

	if ( m_pPaymentsCharge -> GetSize() != 0 )
		return TRUE;

	if ( m_pTotalsNormal -> GetSize() != 0 )
		return TRUE;

	if ( m_pTotalsCharge -> GetSize() != 0 )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

__int64 CEposReportPaymentByItemBlock::GetLineCount()
{
	__int64 nLineCount = 0;
	
	nLineCount += m_pPaymentsNormal -> GetSize();
	nLineCount += m_pTotalsNormal -> GetSize();
	nLineCount += m_pPaymentsCharge -> GetSize();
	nLineCount += m_pTotalsCharge -> GetSize();
	
	return nLineCount;
}

/**********************************************************************/
