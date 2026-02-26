/**********************************************************************/
#include "EposReportConsolMixMatchItem.h"
/**********************************************************************/

CEposReportConsolMMTranByDateTime::CEposReportConsolMMTranByDateTime()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolMMTranByDateTime::Reset()
{
	m_strTranDate = "";
	m_strTranTime = "";
	m_nTranID = 0;
}

/**********************************************************************/

void CEposReportConsolMMTranByDateTime::Add ( CEposReportConsolMMTranByDateTime& source )
{
}

/**********************************************************************/

int CEposReportConsolMMTranByDateTime::Compare( CEposReportConsolMMTranByDateTime& source, int nHint )
{
	if ( m_strTranDate != source.m_strTranDate )
		return ( ( m_strTranDate > source.m_strTranDate ) ? 1 : -1 );

	if ( m_strTranTime != source.m_strTranTime )
		return ( ( m_strTranTime > source.m_strTranTime ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolMMCoversByTran::CEposReportConsolMMCoversByTran()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolMMCoversByTran::Reset()
{
	m_nTranID = 0;
	m_nCovers = 0;
}

/**********************************************************************/

void CEposReportConsolMMCoversByTran::Add ( CEposReportConsolMMCoversByTran& source )
{
}

/**********************************************************************/

int CEposReportConsolMMCoversByTran::Compare( CEposReportConsolMMCoversByTran& source, int nHint )
{
	if ( m_nTranID != source.m_nTranID )
		return ( ( m_nTranID > source.m_nTranID ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolMMCoversByOffer::CEposReportConsolMMCoversByOffer()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolMMCoversByOffer::Reset()
{
	m_nMixMatchType = 0;
	m_nOfferNo = 0;
	m_nTranID = 0;
	m_nCovers = 0;
}

/**********************************************************************/

void CEposReportConsolMMCoversByOffer::Add ( CEposReportConsolMMCoversByOffer& source )
{
}

/**********************************************************************/

int CEposReportConsolMMCoversByOffer::Compare( CEposReportConsolMMCoversByOffer& source, int nHint )
{
	if ( m_nMixMatchType != source.m_nMixMatchType )
		return ( ( m_nMixMatchType > source.m_nMixMatchType ) ? 1 : -1 );

	if ( m_nOfferNo != source.m_nOfferNo )
		return ( ( m_nOfferNo > source.m_nOfferNo ) ? 1 : -1 );

	if ( m_nTranID != source.m_nTranID )
		return ( ( m_nTranID > source.m_nTranID ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolMMTranByOfferItem::CEposReportConsolMMTranByOfferItem()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolMMTranByOfferItem::Reset()
{
	m_nOfferItemID = 0;
	m_nTranID = 0;
}

/**********************************************************************/

void CEposReportConsolMMTranByOfferItem::Add ( CEposReportConsolMMTranByOfferItem& source )
{
}

/**********************************************************************/

int CEposReportConsolMMTranByOfferItem::Compare( CEposReportConsolMMTranByOfferItem& source, int nHint )
{
	if ( m_nOfferItemID != source.m_nOfferItemID )
		return ( ( m_nOfferItemID > source.m_nOfferItemID ) ? 1 : -1 );

	if ( m_nTranID != source.m_nTranID )
		return ( ( m_nTranID > source.m_nTranID ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolMMDiscountedItem::CEposReportConsolMMDiscountedItem()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolMMDiscountedItem::Reset()
{
	m_bBlockTotal = FALSE;
	m_nMixMatchType = 0;
	m_nOfferNo = 0;
	m_nOfferLineType = MMITEMCONSOL_OFFER_ITEM;
	m_nOfferItemID = 0;
	m_nItemNo = 0;
	m_nTranQty = 0;
	m_dItemQty = 0.0;
	m_dVal = 0.0;
	m_nOfferCovers = 0;
}

/**********************************************************************/

void CEposReportConsolMMDiscountedItem::Add ( CEposReportConsolMMDiscountedItem& source )
{
	m_nTranQty += source.m_nTranQty;
	m_dItemQty += source.m_dItemQty;
	m_dVal += source.m_dVal;
	m_nOfferCovers += source.m_nOfferCovers;
}

/**********************************************************************/

int CEposReportConsolMMDiscountedItem::Compare( CEposReportConsolMMDiscountedItem& source, int nHint )
{
	if ( m_bBlockTotal != source.m_bBlockTotal )
		return ( ( TRUE == m_bBlockTotal ) ? 1 : -1 );

	if ( TRUE == m_bBlockTotal )
		return 0;

	if ( m_nMixMatchType != source.m_nMixMatchType )
		return ( ( m_nMixMatchType > source.m_nMixMatchType ) ? 1 : -1 );

	if ( m_nOfferNo != source.m_nOfferNo )
		return ( ( m_nOfferNo > source.m_nOfferNo ) ? 1 : -1 );

	if ( m_nOfferLineType != source.m_nOfferLineType )
		return ( ( m_nOfferLineType > source.m_nOfferLineType ) ? 1 : -1 );

	switch( m_nOfferLineType )
	{
	case MMITEMCONSOL_OFFER_HEADER:
	case MMITEMCONSOL_OFFER_TOTAL:
		return 0;
	}

	if ( m_nItemNo != source.m_nItemNo )
		return ( ( m_nItemNo > source.m_nItemNo ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolMMUsageCount::CEposReportConsolMMUsageCount()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolMMUsageCount::Reset()
{
	m_nOfferNo = 0;
	m_nOfferType = 0;
	m_strUniqueID = "";
	m_nCount = 0;
}

/**********************************************************************/

void CEposReportConsolMMUsageCount::Add(CEposReportConsolMMUsageCount& source)
{
	m_nCount += source.m_nCount;
}

/**********************************************************************/

int CEposReportConsolMMUsageCount::Compare(CEposReportConsolMMUsageCount& source, int nHint)
{
	if (m_nOfferNo != source.m_nOfferNo)
		return ((m_nOfferNo > source.m_nOfferNo) ? 1 : -1);

	if (m_nOfferType != source.m_nOfferType)
		return ((m_nOfferType > source.m_nOfferType) ? 1 : -1);

	if (m_strUniqueID != source.m_strUniqueID)
		return ((m_strUniqueID > source.m_strUniqueID) ? 1 : -1);
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolMMUniqueID::CEposReportConsolMMUniqueID()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolMMUniqueID::Reset()
{
	m_nLocIdx = 0;
	m_strUniqueID = "";
}

/**********************************************************************/

void CEposReportConsolMMUniqueID::Add(CEposReportConsolMMUniqueID& source)
{
}

/**********************************************************************/

int CEposReportConsolMMUniqueID::Compare(CEposReportConsolMMUniqueID& source, int nHint)
{
	if (m_nLocIdx != source.m_nLocIdx)
		return ((m_nLocIdx > source.m_nLocIdx) ? 1 : -1);

	if (m_strUniqueID != source.m_strUniqueID)
		return ((m_strUniqueID > source.m_strUniqueID) ? 1 : -1);
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CEposReportMixMatchItemBlock::GotData()
{
	if (m_pSales->GetSize() != 0)
	{
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

