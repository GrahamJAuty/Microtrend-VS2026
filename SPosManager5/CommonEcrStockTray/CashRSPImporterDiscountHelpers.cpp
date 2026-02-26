/**********************************************************************/
#include "PriceHelpers.h"
/**********************************************************************/
#include "CashRSPImporterDiscountHelpers.h"
/**********************************************************************/

CCashRSPImporterDiscountShareItem::CCashRSPImporterDiscountShareItem()
{
	m_nIndex = 0;
	m_dSales = 0.0;
	m_dDiscount = 0.0;
	m_bAllowDiscount = FALSE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CCashRSPImporterDiscountShareList::CCashRSPImporterDiscountShareList()
{
	Reset();
}

/**********************************************************************/

void CCashRSPImporterDiscountShareList::Reset()
{
	m_dTotalSales = 0.0;
	m_dDiscountableSales = 0.0;
	m_arrayItems.RemoveAll();
}

/**********************************************************************/

void CCashRSPImporterDiscountShareList::AddItem( int nIndex, double dSales, bool bAllowDiscount )
{
	if ( ( m_arrayItems.GetSize() == 0  ) || ( CPriceHelpers::CompareDoubles( dSales, 0.0, 3 ) != 0 ) )
	{
		CCashRSPImporterDiscountShareItem item;
		item.m_nIndex = nIndex;
		item.m_dSales = dSales;
		item.m_bAllowDiscount = bAllowDiscount;
		m_arrayItems.Add( item );

		m_dTotalSales += dSales;

		if ( TRUE == bAllowDiscount )
		{
			m_dDiscountableSales += dSales;
		}
	}
}

/**********************************************************************/

void CCashRSPImporterDiscountShareList::FilterList()
{
	if ( GotDiscountableSales() == TRUE )
	{
		for ( int n = m_arrayItems.GetSize() - 1; n >= 0; n-- )
		{
			if ( FALSE == m_arrayItems[n].m_bAllowDiscount )
			{
				m_arrayItems.RemoveAt(n);
			}
		}
	}
	else
	{
		m_dDiscountableSales = m_dTotalSales;
	}
}

/**********************************************************************/

bool CCashRSPImporterDiscountShareList::GotDiscountableSales()
{
	return ( CPriceHelpers::CompareDoubles( m_dDiscountableSales, 0.0, 3 ) != 0 );
}

/**********************************************************************/

void CCashRSPImporterDiscountShareList::ShareDiscount( double dDiscountAmount, bool bForceFractions )
{
	if ( m_arrayItems.GetSize() == 1 )
	{
		m_arrayItems[0].m_dDiscount = dDiscountAmount;
	}
	else if ( ( EcrmanOptions.GetReportsCashRSPDiscountPennyFlag() == TRUE ) && ( FALSE == bForceFractions) )
	{
		CString strDiscountPence;
		strDiscountPence.Format( "%.0f", dDiscountAmount * 100.0 );			
		int nDiscountPence = atoi( strDiscountPence );
		int nDiscountAllocated = 0;

		CReportConsolidationArray<CSortedIntByInt> arrayDiscountRemaindersPositive( TRUE );
		CReportConsolidationArray<CSortedIntByInt> arrayDiscountRemaindersNegative( TRUE );

		for ( int nIdx = 0; nIdx < m_arrayItems.GetSize(); nIdx++ )
		{
			double dDiscountShare = m_arrayItems[nIdx].m_dSales;
			dDiscountShare *= dDiscountAmount;
			dDiscountShare /= m_dDiscountableSales;

			bool bNeg = ( dDiscountShare < 0.0 );
			
			if ( TRUE == bNeg )
			{
				dDiscountShare *= -1;
			}
						
			int nDiscountSharePence = int( floor ( dDiscountShare * 100.0 ) );
			double dRoundedDiscountShare = ( double( nDiscountSharePence ) ) / 100.0;
			int nDiscountRemainder = int( floor ( ( dDiscountShare - dRoundedDiscountShare ) * 1000000 ) );

			if ( TRUE == bNeg )
			{
				dRoundedDiscountShare *= -1;
				nDiscountSharePence *= -1;
			}

			nDiscountAllocated += nDiscountSharePence;
			m_arrayItems[nIdx].m_dDiscount = dRoundedDiscountShare;
			
			CSortedIntByInt item;
			item.m_nKey = -nDiscountRemainder;
			item.m_nVal = nIdx;

			if ( FALSE == bNeg )
			{
				arrayDiscountRemaindersPositive.Consolidate(item);
			}
			else
			{
				arrayDiscountRemaindersNegative.Consolidate(item);
			}
		}
			
		{
			int nPos = 0;
			while( nDiscountAllocated < nDiscountPence )
			{
				CSortedIntByInt item;
				arrayDiscountRemaindersPositive.GetAt( nPos, item );
				m_arrayItems[ item.m_nVal ].m_dDiscount += 0.01;
				nDiscountAllocated++;
				
				nPos++;
				if ( nPos >= arrayDiscountRemaindersPositive.GetSize() )
				{
					nPos = 0;
				}
			}
		}

		{
			int nPos = 0;
			while( nDiscountAllocated > nDiscountPence )
			{
				CSortedIntByInt item;
				arrayDiscountRemaindersNegative.GetAt( nPos, item );
				m_arrayItems[ item.m_nVal ].m_dDiscount -= 0.01;
				nDiscountAllocated--;
				
				nPos++;
				if ( nPos >= arrayDiscountRemaindersNegative.GetSize() )
				{
					nPos = 0;
				}
			}
		}
	}
	else
	{
		for ( int nIdx = 0; nIdx < m_arrayItems.GetSize(); nIdx++ )
		{
			double dDiscountShare = m_arrayItems[nIdx].m_dSales;
			dDiscountShare *= dDiscountAmount;
			dDiscountShare /= m_dDiscountableSales;		

			m_arrayItems[nIdx].m_dDiscount = dDiscountShare;															
		}
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CCashRSPImporterDiscountHelperSales::CCashRSPImporterDiscountHelperSales()
{
	Reset();
}

/**********************************************************************/

void CCashRSPImporterDiscountHelperSales::Reset()
{
	m_nMMPluNo = 0;
	m_nPluNo = 0;
	m_nAnalysisCategory = 0;
	m_nItemServerNo = 0;
	m_strDescription = "";
	m_strAttributeText = "";
	m_nVIPStatus = 0;
	m_nPriceBand = 0;
	m_strTaxBand = "";
	m_nMixMatchType = 0;
	m_nMixMatchNo = 0;	
	m_strSaleDate = "";
	m_strSaleTime = "";
	/*****/
	m_nDeptNo = 0;
	m_nGroupNo = 0;
	m_dQty = 0.0;
	m_dVal = 0.0;
}

/**********************************************************************/

void CCashRSPImporterDiscountHelperSales::Add ( CCashRSPImporterDiscountHelperSales& source )
{
	m_dQty += source.m_dQty;
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CCashRSPImporterDiscountHelperSales::Compare( CCashRSPImporterDiscountHelperSales& source, int nHint )
{
	if (m_nMMPluNo != source.m_nMMPluNo)
		return (m_nMMPluNo > source.m_nMMPluNo ? 1 : -1);

	if ( m_nPluNo != source.m_nPluNo )
		return ( m_nPluNo > source.m_nPluNo ? 1 : -1 );

	if ( m_nAnalysisCategory != source.m_nAnalysisCategory )
		return ( m_nAnalysisCategory > source.m_nAnalysisCategory ? 1 : -1 );

	if ( m_nItemServerNo != source.m_nItemServerNo )
		return ( m_nItemServerNo > source.m_nItemServerNo ? 1 : -1 );

	if ( m_strDescription != source.m_strDescription )
		return ( m_strDescription > source.m_strDescription ? 1 : -1 );

	if (m_strAttributeText != source.m_strAttributeText)
		return (m_strAttributeText > source.m_strAttributeText ? 1 : -1);

	if ( m_nVIPStatus != source.m_nVIPStatus )
		return ( m_nVIPStatus > source.m_nVIPStatus ? 1 : -1 );

	if ( m_nPriceBand != source.m_nPriceBand )
		return ( m_nPriceBand > source.m_nPriceBand ? 1 : -1 );

	if ( m_strTaxBand != source.m_strTaxBand )
		return ( m_strTaxBand > source.m_strTaxBand ? 1 : -1 );

	if ( m_nMixMatchType != source.m_nMixMatchType )
		return ( m_nMixMatchType > source.m_nMixMatchType ? 1 : -1 );

	if ( m_nMixMatchNo != source.m_nMixMatchNo )
		return ( m_nMixMatchNo > source.m_nMixMatchNo ? 1 : -1 );
	
	if ( m_strSaleDate != source.m_strSaleDate )
		return ( m_strSaleDate > source.m_strSaleDate ? 1 : -1 );

	if ( m_strSaleTime != source.m_strSaleTime )
		return ( m_strSaleTime > source.m_strSaleTime ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CCashRSPImporterDiscountHelperPendingItem::CCashRSPImporterDiscountHelperPendingItem()
{
	Reset();
}

/**********************************************************************/

void CCashRSPImporterDiscountHelperPendingItem::Reset()
{
	m_nPluNo = 0;
	m_nAnalysisCategory = 0;
	m_nPromoNo = 0;
	m_strDiscountType = "";
	m_nDiscountRate = 0;
	m_dAmountAsDouble = 0.0;
	m_nAmountAsInt = 0;
	m_nReasonCode = 0;
	m_strReasonText = "";
}

/**********************************************************************/

void CCashRSPImporterDiscountHelperPendingItem::Add ( CCashRSPImporterDiscountHelperPendingItem& source )
{
	m_dAmountAsDouble += source.m_dAmountAsDouble;
	m_nAmountAsInt += source.m_nAmountAsInt;
}

/**********************************************************************/

int CCashRSPImporterDiscountHelperPendingItem::Compare( CCashRSPImporterDiscountHelperPendingItem& source, int nHint )
{
	if ( m_nPluNo != source.m_nPluNo )
		return ( m_nPluNo > source.m_nPluNo ? 1 : -1 );

	if ( m_nAnalysisCategory != source.m_nAnalysisCategory )
		return ( m_nAnalysisCategory > source.m_nAnalysisCategory ? 1 : -1 );

	if ( m_nPromoNo != source.m_nPromoNo )
		return ( m_nPromoNo > source.m_nPromoNo ? 1 : -1 );

	if ( m_strDiscountType != source.m_strDiscountType )
		return ( m_strDiscountType > source.m_strDiscountType ? 1 : -1 );

	if ( m_nDiscountRate != source.m_nDiscountRate )
		return ( m_nDiscountRate > source.m_nDiscountRate ? 1 : -1 );

	if ( m_nReasonCode != source.m_nReasonCode )
		return ( m_nReasonCode > source.m_nReasonCode ? 1 : -1 );

	if ( m_strReasonText != source.m_strReasonText )
		return ( m_strReasonText > source.m_strReasonText ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CCashRSPImporterDiscountHelperPendingSubtotal::CCashRSPImporterDiscountHelperPendingSubtotal()
{
	Reset();
}

/**********************************************************************/

void CCashRSPImporterDiscountHelperPendingSubtotal::Reset()
{
	m_nPromoNo = 0;
	m_strCategories = "";
	m_dVal = 0.0;
	m_nDiscountMode = 0;
	m_strDiscountType = "";
	m_nDiscountRate = 0;
	m_bVATable = TRUE;
}

/**********************************************************************/

void CCashRSPImporterDiscountHelperPendingSubtotal::Add ( CCashRSPImporterDiscountHelperPendingSubtotal& source )
{
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CCashRSPImporterDiscountHelperPendingSubtotal::Compare( CCashRSPImporterDiscountHelperPendingSubtotal& source, int nHint )
{
	if ( m_nPromoNo != source.m_nPromoNo )
		return ( m_nPromoNo > source.m_nPromoNo ? 1 : -1 );

	if ( m_strCategories != source.m_strCategories )
		return ( m_strCategories > source.m_strCategories ? 1 : -1 );

	if ( m_nDiscountMode != source.m_nDiscountMode )
		return ( m_nDiscountMode > source.m_nDiscountMode ? 1 : -1 );

	if ( m_strDiscountType != source.m_strDiscountType )
		return ( m_strDiscountType > source.m_strDiscountType ? 1 : -1 );

	if ( m_nDiscountRate != source.m_nDiscountRate )
		return ( m_nDiscountRate > source.m_nDiscountRate ? 1 : -1 );

	if ( m_bVATable != source.m_bVATable )
		return ( m_bVATable ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CCashRSPImporterDiscountHelperToWrite::CCashRSPImporterDiscountHelperToWrite()
{
	Reset();
}

/**********************************************************************/

void CCashRSPImporterDiscountHelperToWrite::Reset()
{
	m_nSaleIdx = 0;
	m_nPluNo = 0;
	m_nAnalysisCategory = 0;
	m_strItemDescription = "";
	m_strAttributeText = "";
	m_nVIPStatus = 0;
	m_nPromoNo = 0;
	m_nDiscountMode = 0;
	m_strDiscountType = "";
	m_nDiscountRate = 0;
	m_bVATable = TRUE;
	/*****/
	m_dDiscountedItemQty = 0.0;
	m_dVal = 0.0;
}	

/**********************************************************************/

void CCashRSPImporterDiscountHelperToWrite::Add ( CCashRSPImporterDiscountHelperToWrite& source )
{
	m_dDiscountedItemQty += source.m_dDiscountedItemQty;
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CCashRSPImporterDiscountHelperToWrite::Compare( CCashRSPImporterDiscountHelperToWrite& source, int nHint )
{
	if ( m_nSaleIdx != source.m_nSaleIdx )
		return ( m_nSaleIdx > source.m_nSaleIdx ? 1 : -1 );

	if ( m_nPluNo != source.m_nPluNo )
		return ( m_nPluNo > source.m_nPluNo ? 1 : -1 );

	if ( m_nAnalysisCategory != source.m_nAnalysisCategory )
		return ( m_nAnalysisCategory > source.m_nAnalysisCategory ? 1 : -1 );

	if ( m_strItemDescription != source.m_strItemDescription )
		return ( m_strItemDescription > source.m_strItemDescription ? 1 : -1 );

	if (m_strAttributeText != source.m_strAttributeText)
		return (m_strAttributeText > source.m_strAttributeText ? 1 : -1);

	if ( m_nVIPStatus != source.m_nVIPStatus )
		return ( m_nVIPStatus > source.m_nVIPStatus ? 1 : -1 );

	if ( m_nPromoNo != source.m_nPromoNo )
		return ( m_nPromoNo > source.m_nPromoNo ? 1 : -1 );

	if ( m_nDiscountMode != source.m_nDiscountMode )
		return ( m_nDiscountMode > source.m_nDiscountMode ? 1 : -1 );

	if ( m_strDiscountType != source.m_strDiscountType )
		return ( m_strDiscountType > source.m_strDiscountType ? 1 : -1 );

	if ( m_nDiscountRate != source.m_nDiscountRate )
		return ( m_nDiscountRate > source.m_nDiscountRate ? 1 : -1 );

	if ( m_bVATable != source.m_bVATable )
		return ( m_bVATable ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CCashRSPMixMatchTotals::CCashRSPMixMatchTotals()
{
	Reset();
}

/**********************************************************************/

void CCashRSPMixMatchTotals::Reset()
{
	m_nPromoNo = 0;
	m_nOfferNo = 0;
	m_nPluNo = 0;
	m_dAmountAsDouble = 0.0;
	m_nAmountAsInt = 0;
	m_bConsolidateDouble = FALSE;
}

/**********************************************************************/

void CCashRSPMixMatchTotals::Add(CCashRSPMixMatchTotals& source)
{
	if (TRUE == source.m_bConsolidateDouble)
	{
		SetAmountAsDouble(m_dAmountAsDouble + source.m_dAmountAsDouble);
	}
	else
	{
		SetAmountAsInt(m_nAmountAsInt + source.m_nAmountAsInt);
	}

	if (source.m_nPluNo != 0)
	{
		m_nPluNo = source.m_nPluNo;
	}
}

/**********************************************************************/

void CCashRSPMixMatchTotals::SetAmountAsInt(int n)
{
	m_nAmountAsInt = n;
	m_dAmountAsDouble = n;
	m_bConsolidateDouble = FALSE;
}

/**********************************************************************/

void CCashRSPMixMatchTotals::SetAmountAsDouble(double d)
{
	m_dAmountAsDouble = d;

	CString str = "";
	str.Format("%.0f", m_dAmountAsDouble);
	m_nAmountAsInt = atoi(str);

	m_bConsolidateDouble = TRUE;
}

/**********************************************************************/

int CCashRSPMixMatchTotals::Compare(CCashRSPMixMatchTotals& source, int nHint)
{
	if (m_nPromoNo != source.m_nPromoNo)
	{
		return (m_nPromoNo > source.m_nPromoNo ? 1 : -1);
	}

	if (m_nOfferNo != source.m_nOfferNo)
	{
		return (m_nOfferNo > source.m_nOfferNo ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CCashRSPMixMatchItems::CCashRSPMixMatchItems()
{
	Reset();
}

/**********************************************************************/

void CCashRSPMixMatchItems::Reset()
{
	m_nPromoNo = 0;
	m_nOfferNo = 0;
	m_nPluNo = 0;
	m_strTaxBand = "A";
	m_nOfferType = 0;
	m_nDiscountRate = 0;
	m_dAmount = 0.0;
}

/**********************************************************************/

void CCashRSPMixMatchItems::Add(CCashRSPMixMatchItems& source)
{
	m_nOfferType = source.m_nOfferType;
	m_nDiscountRate = source.m_nDiscountRate;
	m_dAmount += source.m_dAmount;
}

/**********************************************************************/

int CCashRSPMixMatchItems::Compare(CCashRSPMixMatchItems& source, int nHint)
{
	if (m_nPromoNo != source.m_nPromoNo)
	{
		return (m_nPromoNo > source.m_nPromoNo ? 1 : -1);
	}

	if (m_nOfferNo != source.m_nOfferNo)
	{
		return (m_nOfferNo > source.m_nOfferNo ? 1 : -1);
	}

	if (m_nPluNo != source.m_nPluNo)
	{
		return (m_nPluNo > source.m_nPluNo ? 1 : -1);
	}

	if (m_strTaxBand != source.m_strTaxBand)
	{
		return (m_strTaxBand > source.m_strTaxBand ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CCashRSPPromoTotals::CCashRSPPromoTotals()
{
	Reset();
}

/**********************************************************************/

void CCashRSPPromoTotals::Reset()
{
	m_nPromoNo = 0;
	m_nPromoTotal = 0;
	m_nPromoMMTotal = 0;
	m_bCanUsePromoRules = FALSE;
}

/**********************************************************************/

void CCashRSPPromoTotals::Add(CCashRSPPromoTotals& source)
{
	m_nPromoTotal += source.m_nPromoTotal;
	m_nPromoMMTotal += source.m_nPromoMMTotal;
}

/**********************************************************************/

int CCashRSPPromoTotals::Compare(CCashRSPPromoTotals& source, int nHint)
{
	if (m_nPromoNo != source.m_nPromoNo)
	{
		return (m_nPromoNo > source.m_nPromoNo ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CCashRSPPromoItems::CCashRSPPromoItems()
{
	Reset();
}

/**********************************************************************/

void CCashRSPPromoItems::Reset()
{
	m_nPromoNo = 0;
	m_nPluNo = 0;
	m_strDiscountMode = "";
	m_strDiscountType = "";
	m_nDiscountRate = 0;
	m_dAmount = 0.0;
	m_bMicrotrendTaxFlag = FALSE;
}

/**********************************************************************/

void CCashRSPPromoItems::Add(CCashRSPPromoItems& source)
{
	m_dAmount += source.m_dAmount;
}

/**********************************************************************/

int CCashRSPPromoItems::Compare(CCashRSPPromoItems& source, int nHint)
{
	if (m_nPromoNo != source.m_nPromoNo)
	{
		return (m_nPromoNo > source.m_nPromoNo ? 1 : -1);
	}

	if (m_nPluNo != source.m_nPluNo)
	{
		return (m_nPluNo > source.m_nPluNo ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

