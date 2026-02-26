 /**********************************************************************/
#include "EposReportConsolAccountReceive.h"
/**********************************************************************/

CEposReportConsolAccountReceive::CEposReportConsolAccountReceive()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolAccountReceive::Reset()
{
	m_nLocSort = 0;
	m_nType = 0;
	m_strEventDate = "";
	m_strEventTime = "";
	m_strPurchaseDate = "";
	m_strPurchaseTime = "";
	m_nAccountID = 0;
	m_nPaymentType = 0;
	m_nNewDepositStatus = NEWDEPOSIT_AVAILABLE;
	m_dVal = 0.0;
}

/**********************************************************************/

void CEposReportConsolAccountReceive::Add ( CEposReportConsolAccountReceive& source )
{
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CEposReportConsolAccountReceive::Compare( CEposReportConsolAccountReceive& source, int nHint )
{
	if ( m_nLocSort != source.m_nLocSort )
		return ( m_nLocSort > source.m_nLocSort ? 1 : -1 );

	if ( m_nType != source.m_nType )
		return ( m_nType > source.m_nType ? 1 : -1 );

	if ( m_strEventDate != source.m_strEventDate )
		return ( m_strEventDate > source.m_strEventDate ? 1 : -1 );

	if ( m_strEventTime != source.m_strEventTime )
		return ( m_strEventTime > source.m_strEventTime ? 1 : -1 );

	if ( m_strPurchaseDate != source.m_strPurchaseDate )
		return ( m_strPurchaseDate > source.m_strPurchaseDate ? 1 : -1 );

	if ( m_strPurchaseTime != source.m_strPurchaseTime )
		return ( m_strPurchaseTime > source.m_strPurchaseTime ? 1 : -1 );

	if ( m_nAccountID != source.m_nAccountID )
		return ( m_nAccountID > source.m_nAccountID ? 1 : -1 );

	if ( m_nPaymentType != source.m_nPaymentType )
		return ( m_nPaymentType > source.m_nPaymentType ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

bool CEposReportConsolAccountReceive::CompareReceive( CEposReportConsolAccountReceive& source )
{
	if ( m_strEventDate != source.m_strEventDate )
		return FALSE;

	if ( m_strEventTime != source.m_strEventTime )
		return FALSE;

	if ( m_strPurchaseDate != source.m_strPurchaseDate )
		return FALSE;

	if ( m_strPurchaseTime != source.m_strPurchaseTime )
		return FALSE;

	if ( m_nAccountID != source.m_nAccountID )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

const char* CEposReportConsolAccountReceive::GetDisplayEventDate()
{
	m_strDisplayEventDate = "";
	if ( m_strEventDate.GetLength() == 8 )
	{
		m_strDisplayEventDate.Format( "%s/%s/%s",
			m_strEventDate.Right(2),
			m_strEventDate.Mid(4,2),
			m_strEventDate.Left(4) );
	}
	return m_strDisplayEventDate;
}

/**********************************************************************/

const char* CEposReportConsolAccountReceive::GetDisplayEventTime()
{
	m_strDisplayEventTime = "";
	if ( m_strEventTime.GetLength() == 6 )
	{
		m_strDisplayEventTime.Format( "%s:%s",
			m_strEventTime.Left(2),
			m_strEventTime.Mid(2,2) );
	}
	return m_strDisplayEventTime;
}

/**********************************************************************/

const char* CEposReportConsolAccountReceive::GetDisplayPurchaseDate()
{
	m_strDisplayPurchaseDate = "";
	if ( m_strPurchaseDate.GetLength() == 8 )
	{
		m_strDisplayPurchaseDate.Format( "%s/%s/%s",
			m_strPurchaseDate.Right(2),
			m_strPurchaseDate.Mid(4,2),
			m_strPurchaseDate.Left(4) );
	}
	return m_strDisplayPurchaseDate;
}

/**********************************************************************/

const char* CEposReportConsolAccountReceive::GetDisplayPurchaseTime()
{
	m_strDisplayPurchaseTime = "";
	if ( m_strPurchaseTime.GetLength() == 6 )
	{
		m_strDisplayPurchaseTime.Format( "%s:%s:%s",
			m_strPurchaseTime.Left(2),
			m_strPurchaseTime.Mid(2,2),
			m_strPurchaseTime.Right(2) );
	}
	return m_strDisplayPurchaseTime;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolAccountPayment::CEposReportConsolAccountPayment()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolAccountPayment::Reset()
{
	m_nLineType = 0;
	m_nPaymentType = 0;
	m_dVal = 0.0;
}

/**********************************************************************/

void CEposReportConsolAccountPayment::Add ( CEposReportConsolAccountPayment& source )
{
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CEposReportConsolAccountPayment::Compare( CEposReportConsolAccountPayment& source, int nHint )
{
	if ( m_nLineType != source.m_nLineType )
		return ( m_nLineType > source.m_nLineType ? 1 : -1 );

	if ( m_nPaymentType != source.m_nPaymentType )
		return ( m_nPaymentType > source.m_nPaymentType ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolAccountCharged::CEposReportConsolAccountCharged()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolAccountCharged::Reset()
{
	m_nLocSort = 0;
	m_nAccountID = 0;
	m_nNewDepositStatus = NEWDEPOSIT_REFUNDED;
	m_bKeepMe = FALSE;
}

/**********************************************************************/

void CEposReportConsolAccountCharged::Add ( CEposReportConsolAccountCharged& source )
{
}

/**********************************************************************/

int CEposReportConsolAccountCharged::Compare( CEposReportConsolAccountCharged& source, int nHint )
{
	if ( m_nLocSort != source.m_nLocSort )
		return ( m_nLocSort > source.m_nLocSort ? 1 : -1 );

	if ( m_nAccountID != source.m_nAccountID )
		return ( m_nAccountID > source.m_nAccountID ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
