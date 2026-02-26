/**********************************************************************/
 
/**********************************************************************/
#include "OfferGroupItem.h"
/**********************************************************************/

COfferGroupItem::COfferGroupItem()
{
	Reset();
}

/**********************************************************************/

void COfferGroupItem::Reset()
{
	m_nItemNo = 0;
	m_strPluNo = "";
	m_nModifiers = 0;
}

/**********************************************************************/

void COfferGroupItem::Add ( COfferGroupItem& source )
{
}

/**********************************************************************/

int COfferGroupItem::Compare( COfferGroupItem& source, int nHint )
{
	if ( m_nItemNo != source.m_nItemNo )
		return ( m_nItemNo > source.m_nItemNo ? 1 : -1 );

	if ( m_strPluNo != source.m_strPluNo )
		return ( m_strPluNo > source.m_strPluNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

