/**********************************************************************/
 
/**********************************************************************/
#include "EposReportConsolTran.h"
/**********************************************************************/

CEposReportConsolTran::CEposReportConsolTran()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolTran::Reset()
{
	m_strDateTran = "";
	m_strTimeTran = "";
}

/**********************************************************************/

void CEposReportConsolTran::Add ( CEposReportConsolTran& source )
{
}

/**********************************************************************/

int CEposReportConsolTran::Compare( CEposReportConsolTran& source, int nHint )
{
	if ( m_strDateTran != source.m_strDateTran )
		return ( m_strDateTran > source.m_strDateTran ? 1 : -1 );

	if ( m_strTimeTran != source.m_strTimeTran )
		return ( m_strTimeTran > source.m_strTimeTran ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolTranTimeCode::CEposReportConsolTranTimeCode()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolTranTimeCode::Reset()
{
	m_strDateTran = "";
	m_strTimeTran = "";
	m_strTimeCode = "";
	m_nDayShift = 0;
}

/**********************************************************************/

void CEposReportConsolTranTimeCode::Add ( CEposReportConsolTranTimeCode& source )
{
}

/**********************************************************************/

int CEposReportConsolTranTimeCode::Compare( CEposReportConsolTranTimeCode& source, int nHint )
{
	if ( m_strDateTran != source.m_strDateTran )
		return ( m_strDateTran > source.m_strDateTran ? 1 : -1 );

	if ( m_strTimeTran != source.m_strTimeTran )
		return ( m_strTimeTran > source.m_strTimeTran ? 1 : -1 );
	
	if ( m_strTimeCode != source.m_strTimeCode )
		return ( m_strTimeCode > source.m_strTimeCode ? 1 : -1 );

	if ( m_nDayShift != source.m_nDayShift )
		return ( m_nDayShift > source.m_nDayShift ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

bool CEposReportConsolTranTimeCode::TranCompare( CEposReportConsolTranTimeCode& source )
{
	if ( m_strDateTran != source.m_strDateTran )
		return FALSE;

	if ( m_strTimeTran != source.m_strTimeTran )
		return FALSE;
	
	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolTranDateOrTime::CEposReportConsolTranDateOrTime()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolTranDateOrTime::Reset()
{
	m_strDateTran = "";
	m_strTimeTran = "";
	m_strDateOrTime = "";
}

/**********************************************************************/

void CEposReportConsolTranDateOrTime::Add ( CEposReportConsolTranDateOrTime& source )
{
}

/**********************************************************************/

int CEposReportConsolTranDateOrTime::Compare( CEposReportConsolTranDateOrTime& source, int nHint )
{
	if ( m_strDateTran != source.m_strDateTran )
		return ( m_strDateTran > source.m_strDateTran ? 1 : -1 );

	if ( m_strTimeTran != source.m_strTimeTran )
		return ( m_strTimeTran > source.m_strTimeTran ? 1 : -1 );
	
	if ( m_strDateOrTime != source.m_strDateOrTime )
		return ( m_strDateOrTime > source.m_strDateOrTime ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

bool CEposReportConsolTranDateOrTime::TranCompare( CEposReportConsolTranDateOrTime& source )
{
	if ( m_strDateTran != source.m_strDateTran )
		return FALSE;

	if ( m_strTimeTran != source.m_strTimeTran )
		return FALSE;
	
	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolTranBusinessDate::CEposReportConsolTranBusinessDate()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolTranBusinessDate::Reset()
{
	m_strDateTran = "";
	m_strTimeTran = "";
	m_strBusinessDate = "";
}

/**********************************************************************/

void CEposReportConsolTranBusinessDate::Add ( CEposReportConsolTranBusinessDate& source )
{
}

/**********************************************************************/

int CEposReportConsolTranBusinessDate::Compare( CEposReportConsolTranBusinessDate& source, int nHint )
{
	if ( m_strDateTran != source.m_strDateTran )
		return ( m_strDateTran > source.m_strDateTran ? 1 : -1 );

	if ( m_strTimeTran != source.m_strTimeTran )
		return ( m_strTimeTran > source.m_strTimeTran ? 1 : -1 );
	
	if ( m_strBusinessDate != source.m_strBusinessDate )
		return ( m_strBusinessDate > source.m_strBusinessDate ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

bool CEposReportConsolTranBusinessDate::TranCompare( CEposReportConsolTranBusinessDate& source )
{
	if ( m_strDateTran != source.m_strDateTran )
		return FALSE;

	if ( m_strTimeTran != source.m_strTimeTran )
		return FALSE;
	
	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolTranPriceLevel::CEposReportConsolTranPriceLevel()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolTranPriceLevel::Reset()
{
	m_strDateTran = "";
	m_strTimeTran = "";
	m_nPriceLevel = 0;
}

/**********************************************************************/

void CEposReportConsolTranPriceLevel::Add ( CEposReportConsolTranPriceLevel& source )
{
}

/**********************************************************************/

int CEposReportConsolTranPriceLevel::Compare( CEposReportConsolTranPriceLevel& source, int nHint )
{
	if ( m_strDateTran != source.m_strDateTran )
		return ( m_strDateTran > source.m_strDateTran ? 1 : -1 );

	if ( m_strTimeTran != source.m_strTimeTran )
		return ( m_strTimeTran > source.m_strTimeTran ? 1 : -1 );
	
	if ( m_nPriceLevel != source.m_nPriceLevel )
		return ( m_nPriceLevel > source.m_nPriceLevel ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

bool CEposReportConsolTranPriceLevel::TranCompare( CEposReportConsolTranPriceLevel& source )
{
	if ( m_strDateTran != source.m_strDateTran )
		return FALSE;

	if ( m_strTimeTran != source.m_strTimeTran )
		return FALSE;
	
	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolTranMonth::CEposReportConsolTranMonth()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolTranMonth::Reset()
{
	m_strDateTran = "";
	m_strTimeTran = "";
	m_nYear = 0;
	m_nMonth = 0;
}

/**********************************************************************/

void CEposReportConsolTranMonth::Add ( CEposReportConsolTranMonth& source )
{
}

/**********************************************************************/

int CEposReportConsolTranMonth::Compare( CEposReportConsolTranMonth& source, int nHint )
{
	if ( m_strDateTran != source.m_strDateTran )
		return ( m_strDateTran > source.m_strDateTran ? 1 : -1 );

	if ( m_strTimeTran != source.m_strTimeTran )
		return ( m_strTimeTran > source.m_strTimeTran ? 1 : -1 );
	
	if ( m_nYear != source.m_nYear )
		return ( m_nYear > source.m_nYear ? 1 : -1 );

	if ( m_nMonth != source.m_nMonth )
		return ( m_nMonth > source.m_nMonth ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

bool CEposReportConsolTranMonth::TranCompare( CEposReportConsolTranMonth& source )
{
	if ( m_strDateTran != source.m_strDateTran )
		return FALSE;

	if ( m_strTimeTran != source.m_strTimeTran )
		return FALSE;
	
	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolTranServer::CEposReportConsolTranServer()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolTranServer::Reset()
{
	m_strDateTran = "";
	m_strTimeTran = "";
	m_nServerNo = 0;
}

/**********************************************************************/

void CEposReportConsolTranServer::Add (CEposReportConsolTranServer& source )
{
}

/**********************************************************************/

int CEposReportConsolTranServer::Compare( CEposReportConsolTranServer& source, int nHint )
{
	if ( m_strDateTran != source.m_strDateTran )
		return ( m_strDateTran > source.m_strDateTran ? 1 : -1 );

	if ( m_strTimeTran != source.m_strTimeTran )
		return ( m_strTimeTran > source.m_strTimeTran ? 1 : -1 );

	if ( m_nServerNo != source.m_nServerNo )
		return ( m_nServerNo > source.m_nServerNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

bool CEposReportConsolTranServer::TranCompare( CEposReportConsolTranServer& source )
{
	if ( m_strDateTran != source.m_strDateTran )
		return FALSE;

	if ( m_strTimeTran != source.m_strTimeTran )
		return FALSE;

	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolTranPromoItem::CEposReportConsolTranPromoItem()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolTranPromoItem::Reset()
{
	m_strDateTran = "";
	m_strTimeTran = "";
	m_nMixMatchType = 0;
	m_nOfferNo = 0;
	m_nItemNo = 0;
}

/**********************************************************************/

void CEposReportConsolTranPromoItem::Add (CEposReportConsolTranPromoItem& source )
{
}

/**********************************************************************/

int CEposReportConsolTranPromoItem::Compare( CEposReportConsolTranPromoItem& source, int nHint )
{
	if ( m_strDateTran != source.m_strDateTran )
		return ( m_strDateTran > source.m_strDateTran ? 1 : -1 );

	if ( m_strTimeTran != source.m_strTimeTran )
		return ( m_strTimeTran > source.m_strTimeTran ? 1 : -1 );

	if ( m_nMixMatchType != source.m_nMixMatchType )
		return ( m_nMixMatchType > source.m_nMixMatchType ? 1 : -1 );

	if ( m_nOfferNo != source.m_nOfferNo )
		return ( m_nOfferNo > source.m_nOfferNo ? 1 : -1 );
	
	if ( m_nItemNo != source.m_nItemNo )
		return ( m_nItemNo > source.m_nItemNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

bool CEposReportConsolTranPromoItem::TranCompare( CEposReportConsolTranPromoItem& source )
{
	if ( m_strDateTran != source.m_strDateTran )
		return FALSE;

	if ( m_strTimeTran != source.m_strTimeTran )
		return FALSE;

	return TRUE;
}

/**********************************************************************/
