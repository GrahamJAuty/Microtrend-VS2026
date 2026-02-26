/**********************************************************************/
#include "LocationCSVArray.h"
/**********************************************************************/
#include "ReportCostFinder.h"
/**********************************************************************/

CReportCostFinder::CReportCostFinder(void)
{
	m_nPreviousPluNo = -1;
	m_nPreviousPluIdx = -1;
	m_nPreviousLocIdx = -1;
	m_nPreviousLocInfoIdx = -1;
	m_nLocationsWithPriceSets = 0;
}

/**********************************************************************/

void CReportCostFinder::Init()
{
	m_nPreviousPluNo = -1;
	m_nPreviousPluIdx = -1;
	m_nPreviousLocIdx = -1;
	m_nPreviousLocInfoIdx = -1;
	m_nLocationsWithPriceSets = 0;

	m_arrayPluInfo.RemoveAll();
	m_arrayLocations.RemoveAll();
	
	CReportCostFinderPluInfo infoPlu;
	infoPlu.m_nCostType = 0;
	infoPlu.m_dMasterCost = 0.0;
	infoPlu.m_strLocCosts = "";

	for ( int n = 0; n < DataManager.Plu.GetSize(); n++ )
		m_arrayPluInfo.Add( infoPlu );
}

/**********************************************************************/

void CReportCostFinder::AddNoPriceSetLocation()
{
	CReportCostFinderLocInfo infoLoc;
	infoLoc.m_nLocIdx = m_nPreviousLocIdx;
	infoLoc.m_nColumn = -1;
	m_arrayLocations.Add( infoLoc );
	m_nPreviousLocInfoIdx = m_arrayLocations.GetSize() - 1;
}

/**********************************************************************/

double CReportCostFinder::GetPluCostByPluNo( __int64 nPluNo, int nLocIdx )
{
	if ( nPluNo != m_nPreviousPluNo )
	{
		m_nPreviousPluNo = nPluNo;
		if ( DataManager.Plu.FindPluByNumber( nPluNo, m_nPreviousPluIdx ) == FALSE )
		{
			m_nPreviousPluIdx = -1;
			return 0.0;
		}
	}

	return GetPluCostByPluIdx( m_nPreviousPluIdx, nLocIdx );
}

/**********************************************************************/

double CReportCostFinder::GetPluCostByPluIdx( int nPluIdx, int nLocIdx )
{
	if ( ( nPluIdx < 0 ) || ( nPluIdx >= m_arrayPluInfo.GetSize() ) )
		return 0.0;

	__int64 nPluNo = DataManager.Plu.GetPluNoInt( nPluIdx );

	CReportCostFinderPluInfo infoPlu = m_arrayPluInfo.GetAt( nPluIdx );
	
	if ( 1 == infoPlu.m_nCostType )
		return infoPlu.m_dMasterCost;

	if ( 0 == infoPlu.m_nCostType )
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( nPluIdx, PluRecord );

		if ( PluRecord.GetReportCostEnable() == FALSE )
		{
			infoPlu.m_nCostType = 1;
			infoPlu.m_dMasterCost = DataManager.Plu.GetPluPurchaseCost( PluRecord );
			m_arrayPluInfo.SetAt( nPluIdx, infoPlu );
			return infoPlu.m_dMasterCost;
		}

		infoPlu.m_nCostType = 2;
		infoPlu.m_dMasterCost = PluRecord.GetReportCost();
		m_arrayPluInfo.SetAt( nPluIdx, infoPlu );
	}

	if ( 2 != infoPlu.m_nCostType )
		return 0.0;

	if ( nLocIdx != m_nPreviousLocIdx )
	{
		bool bFound = FALSE;
		m_nPreviousLocIdx = nLocIdx;
		for ( m_nPreviousLocInfoIdx = 0; m_nPreviousLocInfoIdx < m_arrayLocations.GetSize(); m_nPreviousLocInfoIdx++ )
		{
			if ( m_arrayLocations[m_nPreviousLocInfoIdx].m_nLocIdx == nLocIdx )
			{
				bFound = TRUE;
				break;
			}
		}

		if ( FALSE == bFound )
			m_nPreviousLocInfoIdx = -1;
	}
	
	if ( m_nPreviousLocInfoIdx != -1 )
	{
		int nColumn = m_arrayLocations[m_nPreviousLocInfoIdx].m_nColumn;

		if ( nColumn == -1 )
			return infoPlu.m_dMasterCost;
		else
		{
			CCSV csv( infoPlu.m_strLocCosts, ',', '"', nColumn + 1 );
			CString strPrice = csv.GetString(nColumn);
			
			if ( strPrice == "" )
				return infoPlu.m_dMasterCost;
			else
				return atof( strPrice );
		}
	}

	CString strFieldFilename = dbLocation.GetFilePathDataLocalPluField( nLocIdx, dbLocation.GetDefaultPriceSet( nLocIdx ) );

	CSSFile filePriceSet;
	if ( filePriceSet.Open( strFieldFilename, "rb" ) == FALSE )
	{
		AddNoPriceSetLocation();
		return infoPlu.m_dMasterCost;
	}

	CString strBuffer;
	if ( filePriceSet.ReadString( strBuffer ) == FALSE )
	{
		AddNoPriceSetLocation();
		return infoPlu.m_dMasterCost;
	}

	int nReportCostPos;
	{
		bool bFound = FALSE;
		CCSV csv( strBuffer );
		
		for ( nReportCostPos = 1; nReportCostPos < csv.GetSize(); nReportCostPos++ )
		{
			CString strField = csv.GetString( nReportCostPos );
			if ( strField == Plu::ReportCost.Label )
			{
				bFound = TRUE;
				break;
			}
		}

		if ( FALSE == bFound )
		{
			AddNoPriceSetLocation();
			return infoPlu.m_dMasterCost;
		}
	}

	{
		CReportCostFinderLocInfo infoLoc;
		infoLoc.m_nLocIdx = nLocIdx;
		infoLoc.m_nColumn = m_nLocationsWithPriceSets;
		m_arrayLocations.Add( infoLoc );
		m_nPreviousLocInfoIdx = m_arrayLocations.GetSize() - 1;
	}

	double dResult = infoPlu.m_dMasterCost;
	while ( filePriceSet.ReadString( strBuffer ) == TRUE )
	{
		CCSV csv( strBuffer );
		__int64 nPriceSetPluNo = csv.GetInt(0);
		double dPrice = csv.GetDouble(nReportCostPos);

		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber( nPriceSetPluNo, nPluIdx ) == TRUE )
		{
			infoPlu = m_arrayPluInfo.GetAt( nPluIdx );
			CCSV csvPrices( infoPlu.m_strLocCosts );
			csvPrices.SetAt( m_nLocationsWithPriceSets, dPrice, 5, FALSE );
			infoPlu.m_strLocCosts = csvPrices.GetLine();
			m_arrayPluInfo.SetAt( nPluIdx, infoPlu );

			if ( nPriceSetPluNo == nPluNo )
				dResult = dPrice;
		}
	}

	m_nLocationsWithPriceSets++;

	return dResult;
}

/**********************************************************************/
