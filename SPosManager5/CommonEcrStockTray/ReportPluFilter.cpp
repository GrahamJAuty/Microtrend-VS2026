/**********************************************************************/
#include "NodeTypes.h"
#include "OfferGroupsCSVArray.h"
/**********************************************************************/
#include "ReportPluFilter.h"
/**********************************************************************/

CReportPluFilter::CReportPluFilter()
{
	m_bSystemPluFilter = FALSE;
	m_bSystemPluFilterByName = FALSE;
	m_nMasterFilterNo = 0;
	m_nCurrentFilterNo = 0;
}

/**********************************************************************/

void CReportPluFilter::SetConsolidationType( int nConType )
{
	m_bSystemPluFilter = ( NODE_SYSTEM == nConType );
}

/**********************************************************************/

void CReportPluFilter::SetMasterFilterNo( int nFilter )
{
	m_nMasterFilterNo = nFilter;
	m_nCurrentFilterNo = nFilter;
	
	if ( TRUE == m_bSystemPluFilter )
		m_bSystemPluFilterByName = DataManagerNonDb.SystemPluFilterItems.IsFilterEmpty( m_nMasterFilterNo );
	else
		m_bSystemPluFilterByName = FALSE;

	if ( TRUE == m_bSystemPluFilterByName )
	{
		m_strSystemPluFilterName = "";

		int nIdx;
		if ( DataManagerNonDb.SystemPluFilterNames.FindTableByNumber( m_nMasterFilterNo, nIdx ) == TRUE )
		{
			COfferGroupsCSVRecord Filter( NODE_PLUFILTER_NAME );
			DataManagerNonDb.SystemPluFilterNames.GetAt( nIdx, Filter );
			m_strSystemPluFilterName = Filter.GetGroupName();
		}
	}
}

/**********************************************************************/

void CReportPluFilter::SetDatabase( int nDbIdx )
{
	if ( TRUE == m_bSystemPluFilterByName )
	{
		CString strSystemName = m_strSystemPluFilterName;
		strSystemName.MakeUpper();

		m_nCurrentFilterNo = 0;
		
		for ( int n = 0; n < DataManager.DbPluFilterNames.GetSize(); n++ )
		{
			COfferGroupsCSVRecord Filter( NODE_PLUFILTER_NAME );;
			DataManager.DbPluFilterNames.GetAt( n, Filter );
			
			CString strName = Filter.GetGroupName();
			strName.MakeUpper();

			if ( strSystemName == strName )
			{
				m_nCurrentFilterNo = Filter.GetGroupNo();
				break;
			}
		}
	}
}

/**********************************************************************/

bool CReportPluFilter::CheckPlu( CSQLPluNoInfo& infoPlu )
{
	if ( 0 == m_nCurrentFilterNo )
		return TRUE;
	else if ( ( TRUE == m_bSystemPluFilter ) && ( FALSE == m_bSystemPluFilterByName ) )
		return DataManagerNonDb.SystemPluFilterItems.CheckPluNo( m_nCurrentFilterNo, infoPlu );
	else
		return DataManager.DbPluFilterItems.CheckPluNo( m_nCurrentFilterNo, infoPlu );
}

/**********************************************************************/
