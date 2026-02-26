/**********************************************************************/
#include "EposReportPluSalesInfo.h"
/**********************************************************************/

CEposReportPluSalesInfo::CEposReportPluSalesInfo()
{
	Clear();
}

/**********************************************************************/

void CEposReportPluSalesInfo::Clear()
{
	m_dQty = 0.0;
	m_dTotalQty = 0.0;
	m_dValBeforeDiscount = 0.0;
	m_dDiscountAmount = 0.0;
	m_dPremiumAmount = 0.0;
	m_dTax = 0.0;
	m_dCost = 0.0;
	m_bGotData = FALSE;
};

/**********************************************************************/

void CEposReportPluSalesInfo::Consolidate( CEposReportPluSalesInfo& PluSalesInfo )
{
	m_dQty +=				PluSalesInfo.m_dQty;
	m_dTotalQty +=			PluSalesInfo.m_dTotalQty;
	m_dValBeforeDiscount += PluSalesInfo.m_dValBeforeDiscount;
	m_dDiscountAmount +=	PluSalesInfo.m_dDiscountAmount;
	m_dPremiumAmount +=		PluSalesInfo.m_dPremiumAmount;
	m_dTax +=				PluSalesInfo.m_dTax;
	m_dCost +=				PluSalesInfo.m_dCost;
	m_bGotData |=			PluSalesInfo.m_bGotData;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportPluByPriceBandInfo::CEposReportPluByPriceBandInfo()
{
	Clear();
}

/**********************************************************************/

void CEposReportPluByPriceBandInfo::Clear()
{
	m_dQty = 0.0;
	m_dTotalQty = 0.0;
	m_dValBeforeDiscount = 0.0;
	m_dDiscountAmount = 0.0;
	m_dPremiumAmount = 0.0;
	m_dTax = 0.0;
	m_dCost = 0.0;

	for (int n = 0; n < MAX_PLU_PRICES; n++)
	{
		m_dPriceBandQty[n] = 0.0;
		m_dPriceBandVal[n] = 0.0;
	}

	m_bGotData = FALSE;
};

/**********************************************************************/

void CEposReportPluByPriceBandInfo::Consolidate(CEposReportPluByPriceBandInfo& PluSalesInfo)
{
	m_dQty += PluSalesInfo.m_dQty;
	m_dTotalQty += PluSalesInfo.m_dTotalQty;
	m_dValBeforeDiscount += PluSalesInfo.m_dValBeforeDiscount;
	m_dDiscountAmount += PluSalesInfo.m_dDiscountAmount;
	m_dPremiumAmount += PluSalesInfo.m_dPremiumAmount;
	m_dTax += PluSalesInfo.m_dTax;
	m_dCost += PluSalesInfo.m_dCost;

	for (int n = 0; n < MAX_PLU_PRICES; n++)
	{
		m_dPriceBandQty[n] += PluSalesInfo.m_dPriceBandQty[n];
		m_dPriceBandVal[n] += PluSalesInfo.m_dPriceBandVal[n];
	}

	m_bGotData |= PluSalesInfo.m_bGotData;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportPluSalesInfoArray::CEposReportPluSalesInfoArray()
{
	for ( int nLevel = 0; nLevel <= MAX_PLU_PRICES + MAX_SBPLU_PRICES; nLevel++ )
		m_arrayInfo[nLevel] = NULL;
}

/**********************************************************************/

CEposReportPluSalesInfoArray::~CEposReportPluSalesInfoArray()
{
	Clear();
}

/**********************************************************************/

void CEposReportPluSalesInfoArray::Clear()
{
	for ( int nLevel = 0; nLevel <= MAX_PLU_PRICES + MAX_SBPLU_PRICES; nLevel++ )
	{
		if ( m_arrayInfo[nLevel] != NULL )
			delete( m_arrayInfo[nLevel] );

		m_arrayInfo[nLevel] = NULL;
	}
}

/**********************************************************************/

void CEposReportPluSalesInfoArray::Clear( int nLevel )
{
	if ( ( nLevel >= 0 ) && ( nLevel <= MAX_PLU_PRICES + MAX_SBPLU_PRICES ) )
	{
		if ( m_arrayInfo[nLevel] != NULL )
			delete( m_arrayInfo[nLevel] );

		m_arrayInfo[nLevel] = NULL;
	}
}

/**********************************************************************/

void CEposReportPluSalesInfoArray::Consolidate( int nLevel, CEposReportPluSalesInfo& PluSalesInfo )
{
	if ( ( nLevel >= 0 ) && ( nLevel <= MAX_PLU_PRICES + MAX_SBPLU_PRICES ) )
	{
		if ( m_arrayInfo[ nLevel ] != NULL )
			m_arrayInfo[nLevel] -> Consolidate( PluSalesInfo );
		else
		{
			CEposReportPluSalesInfo* pInfo = new CEposReportPluSalesInfo();
			
			if ( pInfo != NULL )
			{
				m_arrayInfo[nLevel] = pInfo;
				pInfo -> Consolidate( PluSalesInfo );
			}
		}
	}
}

/**********************************************************************/

void CEposReportPluSalesInfoArray::Consolidate( CEposReportPluSalesInfoArray& other )
{
	for ( int nLevel = 0; nLevel <= MAX_PLU_PRICES + MAX_SBPLU_PRICES; nLevel++ )
	{
		if ( other.m_arrayInfo[nLevel] != NULL )
			Consolidate( nLevel, *other.m_arrayInfo[nLevel] );
	}
}

/**********************************************************************/

bool CEposReportPluSalesInfoArray::GetSalesInfo( int nLevel, CEposReportPluSalesInfo& PluSalesInfo )
{
	bool bResult = FALSE;
	PluSalesInfo.Clear();

	if ( ( nLevel >= 0 ) && ( nLevel <= MAX_PLU_PRICES + MAX_SBPLU_PRICES ) )
	{
		if ( m_arrayInfo[ nLevel ] != NULL )
		{
			PluSalesInfo = *m_arrayInfo[ nLevel ];
			bResult = TRUE;
		}
	}
	
	return bResult;
}

/**********************************************************************/
