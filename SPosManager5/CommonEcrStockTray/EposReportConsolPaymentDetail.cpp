/**********************************************************************/
#include "EposReportConsolPaymentDetail.h"
/**********************************************************************/

CEposReportConsolPaymentDetail::CEposReportConsolPaymentDetail()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolPaymentDetail::Reset()
{
	m_bIsBlockTotal = FALSE;
	m_nPaymentNo = 0;
	m_bIsPaymentTotal = FALSE;
	m_strDate = "";
	m_strTime = "";
	m_nLocIdx = 0;
	m_nSEQNo = 0;
	m_nServerNo = 0;
	m_nTableNo = 0;
	m_strTableName = "";
	m_nTender = 0;
	m_nAmount = 0;
	m_nCashback = 0;
	m_nGratuity = 0;
	m_nSurplus = 0;
	m_nChange = 0;
}

/**********************************************************************/

void CEposReportConsolPaymentDetail::Add (CEposReportConsolPaymentDetail& source )
{
	m_nTender += source.m_nTender;
	m_nAmount += source.m_nAmount;
	m_nCashback += source.m_nCashback;
	m_nGratuity += source.m_nGratuity;
	m_nSurplus += source.m_nSurplus;
	m_nChange += source.m_nChange;
}

/**********************************************************************/

int CEposReportConsolPaymentDetail::Compare( CEposReportConsolPaymentDetail& source, int nHint )
{
	if ( m_bIsBlockTotal != source.m_bIsBlockTotal )
		return ( m_bIsBlockTotal ? 1: -1 );

	if ( TRUE == m_bIsBlockTotal )
		return 0;

	if ( m_nPaymentNo != source.m_nPaymentNo )
		return ( m_nPaymentNo > source.m_nPaymentNo ? 1 : -1 );

	if ( m_bIsPaymentTotal != source.m_bIsPaymentTotal )
		return ( m_bIsPaymentTotal ? 1: -1 );
	
	if ( TRUE == m_bIsPaymentTotal )
		return 0;

	if ( m_strDate != source.m_strDate )
		return ( m_strDate > source.m_strDate ? 1 : -1 );

	if ( m_strTime != source.m_strTime )
		return ( m_strTime > source.m_strTime ? 1 : -1 );

	if (m_nLocIdx != source.m_nLocIdx)
		return (m_nLocIdx > source.m_nLocIdx ? 1 : -1);

	if ( m_nSEQNo != source.m_nSEQNo )
		return (m_nSEQNo > source.m_nSEQNo ? 1 : -1);

	if (m_nServerNo != source.m_nServerNo)
		return (m_nServerNo > source.m_nServerNo ? 1 : -1);

	if ( m_nTableNo != source.m_nTableNo )	
		return ( m_nTableNo > source.m_nTableNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CEposReportPaymentDetailBlock::GotData()
{
	return ( m_pDetail -> GetSize() != 0 );
}

/**********************************************************************/
