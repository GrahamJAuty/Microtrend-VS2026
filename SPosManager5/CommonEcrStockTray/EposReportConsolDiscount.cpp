 
/**********************************************************************/
#include "EposReportConsolDiscount.h"
/**********************************************************************/

CEposReportConsolDiscount::CEposReportConsolDiscount()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolDiscount::Reset()
{
	m_nServerLocOrDbSort = 0;
	m_nServerLocOrDbIdx = 0;
	m_nServerNo = 0;
	m_nLineType = 0;
	m_nDbIdx = 0;
	m_strDate = "";
	m_strTime = "";
	m_nDiscountType = 0;
	m_nDiscountMode = 0;
	m_nPromoNo = 0;
	m_nPluNo = 0;
	m_nLineNo = 0;
	m_strText = "";
	m_nAmount = 0;
	m_nRate = 0;
	m_strReason = "";
}

/**********************************************************************/

void CEposReportConsolDiscount::Add (CEposReportConsolDiscount& source )
{
	m_nAmount += source.m_nAmount;
}

/**********************************************************************/

bool CEposReportConsolDiscount::CompareServer( CEposReportConsolDiscount& source )
{
	if ( m_nServerLocOrDbSort != source.m_nServerLocOrDbSort )
		return FALSE;

	if ( m_nServerLocOrDbIdx != source.m_nServerLocOrDbIdx )
		return FALSE;

	if ( m_nServerNo != source.m_nServerNo )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

int CEposReportConsolDiscount::Compare( CEposReportConsolDiscount& source, int nHint )
{
	if ( m_nServerLocOrDbSort != source.m_nServerLocOrDbSort )
		return ( m_nServerLocOrDbSort > source.m_nServerLocOrDbSort ? 1 : -1 );

	if ( m_nServerLocOrDbIdx != source.m_nServerLocOrDbIdx )
		return ( m_nServerLocOrDbIdx > source.m_nServerLocOrDbIdx ? 1 : -1 );

	if ( m_nServerNo != source.m_nServerNo )
		return ( m_nServerNo > source.m_nServerNo ? 1 : -1 );

	if ( m_nLineType != source.m_nLineType )
		return ( m_nLineType > source.m_nLineType ? 1 : -1 );

	if ( m_nDbIdx != source.m_nDbIdx )
		return ( m_nDbIdx > source.m_nDbIdx ? 1 : -1 );

	if ( m_strDate != source.m_strDate )
		return ( m_strDate > source.m_strDate ? 1 : -1 );

	if ( m_strTime != source.m_strTime )
		return ( m_strTime > source.m_strTime ? 1 : -1 );

	if ( m_nDiscountType != source.m_nDiscountType )
		return ( m_nDiscountType > source.m_nDiscountType ? 1 : -1 );

	if ( m_nDiscountMode != source.m_nDiscountMode )
		return ( m_nDiscountMode > source.m_nDiscountMode ? 1 : -1 );

	if ( m_nPromoNo != source.m_nPromoNo )
		return ( m_nPromoNo > source.m_nPromoNo ? 1 : -1 );

	if ( m_nPluNo != source.m_nPluNo )
		return ( m_nPluNo > source.m_nPluNo ? 1 : -1 );

	if ( m_nLineNo != source.m_nLineNo )
		return ( m_nLineNo > source.m_nLineNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolDiscountNew::CEposReportConsolDiscountNew()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolDiscountNew::Reset()
{
	m_strDate = "";
	m_strTime = "";
	m_nMixMatchType = 0;
	m_nDiscountType = 0;
	m_nDiscountMode = 0;
	m_nOfferNo = 0;
	m_nPluNo = 0;
	m_nLineNo = 0;
	m_strText = "";
	m_nRate = 0;
	m_dValue = 0.0;
}

/**********************************************************************/

void CEposReportConsolDiscountNew::Add (CEposReportConsolDiscountNew& source )
{
	m_dValue += source.m_dValue;
}

/**********************************************************************/

int CEposReportConsolDiscountNew::Compare( CEposReportConsolDiscountNew& source, int nHint )
{
	if ( m_strDate != source.m_strDate )
		return ( m_strDate > source.m_strDate ? 1 : -1 );

	if ( m_strTime != source.m_strTime )
		return ( m_strTime > source.m_strTime ? 1 : -1 );

	if ( m_nMixMatchType != source.m_nMixMatchType )
		return ( m_nMixMatchType > source.m_nMixMatchType ? 1 : -1 );

	if ( m_nDiscountType != source.m_nDiscountType )
		return ( m_nDiscountType > source.m_nDiscountType ? 1 : -1 );

	if ( m_nDiscountMode != source.m_nDiscountMode )
		return ( m_nDiscountMode > source.m_nDiscountMode ? 1 : -1 );

	if ( m_nOfferNo != source.m_nOfferNo )
		return ( m_nOfferNo > source.m_nOfferNo ? 1 : -1 );

	if ( m_nPluNo != source.m_nPluNo )
		return ( m_nPluNo > source.m_nPluNo ? 1 : -1 );

	if ( m_nLineNo != source.m_nLineNo )
		return ( m_nLineNo > source.m_nLineNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CEposReportDiscountBlock::GotData()
{
	if ( ( m_pDiscount -> GetSize() != 0 ) || ( m_pTotals -> GetSize() != 0 ) )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

