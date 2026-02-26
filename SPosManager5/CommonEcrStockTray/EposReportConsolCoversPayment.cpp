 /**********************************************************************/
#include "EposReportConsolCoversPayment.h"
/**********************************************************************/

CEposReportConsolCoversPayment::CEposReportConsolCoversPayment()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolCoversPayment::Reset()
{
	m_nPaymentNo = 0;
	m_nCovers = 0;
	m_nAmount = 0;
}

/**********************************************************************/

void CEposReportConsolCoversPayment::Add (CEposReportConsolCoversPayment& source )
{
	m_nCovers += source.m_nCovers;
	m_nAmount += source.m_nAmount;
}

/**********************************************************************/

int CEposReportConsolCoversPayment::Compare( CEposReportConsolCoversPayment& source, int nHint )
{
	if ( m_nPaymentNo != source.m_nPaymentNo )
		return ( ( m_nPaymentNo > source.m_nPaymentNo ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportCoversPaymentBlock::CEposReportCoversPaymentBlock()
{
	m_pSales = NULL;
}

/**********************************************************************/

bool CEposReportCoversPaymentBlock::GotData()
{
	if (m_pSales->GetSize() != 0)
	{
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

