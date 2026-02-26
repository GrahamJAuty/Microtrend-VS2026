/**********************************************************************/
#include "DateRangeOrRegionPluInfo.h"
/**********************************************************************/

CDateRangeOrRegionPluInfo::CDateRangeOrRegionPluInfo()
{
	m_bShowItem = FALSE;
	m_nDeptNo = 0;
	m_dCostPerPlu = 0.0;
	m_strDisplayPluNo = "";
	m_strReportText = "";

	for ( int nMod = 0; nMod < 10; nMod++ )
		m_nDatabaseTaxBand[nMod] = 0;
}

/**********************************************************************/

int CDateRangeOrRegionPluInfo::GetDatabaseTaxBand(int nMod)
{
	if ( ( nMod >= 0 ) && ( nMod <= 9 ) ) 
		return m_nDatabaseTaxBand[nMod];
	else
		return 0;
}

/**********************************************************************/

bool CDateRangeOrRegionPluInfo::CheckTaxBand( int nTaxFilter, int nTaxBand )
{
	if ( TAXBAND_FILTER_ALL == nTaxFilter )
	{
		return TRUE;	
	}
	else
	{
		int nMask = 0;

		if ((nTaxBand >= 0) && (nTaxBand <= MAX_TAX_BANDS))
		{
			nMask = 1 << nTaxBand;
		}

		return ( ( nTaxFilter & nMask ) != 0 );
	}
}

/**********************************************************************/
