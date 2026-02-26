/**********************************************************************/
#include "ReportFilters.h"
/**********************************************************************/

CReportFilters::CReportFilters()
{
	m_bDisablePluFilter = FALSE;
	ResetDbIdx();
}

/**********************************************************************/

bool CReportFilters::SetDatabase( int nDbIdx )
{
	bool bResult = FALSE;
	
	if ( nDbIdx != m_nDbIdx )
	{
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
		
		if ( FALSE == m_bDisablePluFilter )
			PluFilter.SetDatabase( nDbIdx );
		
		DeptFilter.SetDatabase( nDbIdx );
		m_nDbIdx = nDbIdx;
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/
