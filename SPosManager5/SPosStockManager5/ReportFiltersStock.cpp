/**********************************************************************/
#include "ReportFiltersStock.h"
/**********************************************************************/

CReportFiltersStock::CReportFiltersStock()
{
	ResetDbIdx();
}

/**********************************************************************/

bool CReportFiltersStock::SetDatabase( int nDbIdx )
{
	bool bResult = FALSE;
	
	if ( nDbIdx != m_nDbIdx )
	{
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
		CategoryFilter.SetDatabase( nDbIdx );
		m_nDbIdx = nDbIdx;
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/
