/**********************************************************************/
 
/**********************************************************************/
#include "EposReportConsolPromotionSummary.h"
/**********************************************************************/

CEposReportConsolPromotionSummary::CEposReportConsolPromotionSummary()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolPromotionSummary::Reset()
{
	m_nSection = 0;
	m_nMixMatchType = 0;
	m_nOfferNo = 0;
	m_bOfferTotal = 0;
	m_nDiscType = 0;
	m_nDiscAction = 0;
	m_dAmount = 0.0;
}

/**********************************************************************/

void CEposReportConsolPromotionSummary::Add (CEposReportConsolPromotionSummary& source )
{
	m_dAmount += source.m_dAmount;
}

/**********************************************************************/

int CEposReportConsolPromotionSummary::Compare( CEposReportConsolPromotionSummary& source, int nHint )
{
	if ( m_nSection != source.m_nSection )
		return ( m_nSection > source.m_nSection ? 1 : -1 );

	switch( m_nSection )
	{
	case 1:	//PROMOS

		if ( m_nOfferNo != source.m_nOfferNo )
			return ( m_nOfferNo > source.m_nOfferNo ? 1 : -1 );

		if ( m_bOfferTotal != source.m_bOfferTotal )
			return ( TRUE == m_bOfferTotal ? 1 : -1 );

		if ( TRUE == m_bOfferTotal )
			return 0;
		else
		{
			if ( m_nDiscType != source.m_nDiscType )
				return ( m_nDiscType > source.m_nDiscType ? 1 : -1 );

			if ( m_nDiscAction != source.m_nDiscAction )
				return ( m_nDiscAction > source.m_nDiscAction ? 1 : -1 );
			else
				return 0;
		}
		break;

	case 2: //MIX MATCH

		if ( m_nMixMatchType != source.m_nMixMatchType )
			return ( m_nMixMatchType > source.m_nMixMatchType ? 1 : -1 );

		if ( m_bOfferTotal != source.m_bOfferTotal )
			return ( TRUE == m_bOfferTotal ? 1 : -1 );

		if ( ( TRUE == m_bOfferTotal ) || ( m_nOfferNo == source.m_nOfferNo ) )
			return 0;
		else
			return ( m_nOfferNo > source.m_nOfferNo ? 1 : -1 );
	
	default:
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolPromotionSummaryTotal::CEposReportConsolPromotionSummaryTotal()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolPromotionSummaryTotal::Reset()
{
	m_nSection = 0;
	m_nMixMatchType = 0;
	m_nDiscType = 0;
	m_nDiscAction = 0;
	m_dAmount = 0.0;
}

/**********************************************************************/

void CEposReportConsolPromotionSummaryTotal::Copy( CEposReportConsolPromotionSummary& source ) 
{
	m_nSection = source.m_nSection;
	m_nMixMatchType = source.m_nMixMatchType;
	m_nDiscType = source.m_nDiscType;
	m_nDiscAction = source.m_nDiscAction;
	m_dAmount = source.m_dAmount;
}

/**********************************************************************/

void CEposReportConsolPromotionSummaryTotal::Add (CEposReportConsolPromotionSummaryTotal& source )
{
	m_dAmount += source.m_dAmount;
}

/**********************************************************************/

int CEposReportConsolPromotionSummaryTotal::Compare( CEposReportConsolPromotionSummaryTotal& source, int nHint )
{
	if ( m_nSection != source.m_nSection )
		return ( m_nSection > source.m_nSection ? 1 : -1 );

	switch( m_nSection )
	{
	case 1: //PROMO TOTAL

		if ( m_nDiscType != source.m_nDiscType )
			return ( m_nDiscType > source.m_nDiscType ? 1 : -1 );

		if ( m_nDiscAction != source.m_nDiscAction )
			return ( m_nDiscAction > source.m_nDiscAction ? 1 : -1 );
		else
			return 0;

	case 2: //MIX MATCH TOTALS
		
		if ( m_nMixMatchType != source.m_nMixMatchType )
			return ( m_nMixMatchType > source.m_nMixMatchType ? 1 : -1 );
		else
			return 0;

	default:
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CEposReportPromotionSummaryBlock::GotData()
{
	return ( m_pPromos -> GetSize() != 0 );
}

/**********************************************************************/
