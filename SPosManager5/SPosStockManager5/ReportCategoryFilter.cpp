/**********************************************************************/
#include "NodeTypes.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "ReportCategoryFilter.h"
/**********************************************************************/

CReportCategoryFilter::CReportCategoryFilter()
{
	m_nCategoryFilterNo = 1;
	m_bSystemCategoryFilter = FALSE;
	m_bSystemCategoryFilterByName = FALSE;
	m_strSystemCategoryFilterName = "";
	m_CategoryFilter.IncludeCategory(1,TRUE);
}

/**********************************************************************/

void CReportCategoryFilter::SetConsolidationType( int nConType )
{
	if ( NODE_SYSTEM == nConType )
		m_bSystemCategoryFilter = ( StockOptions.GetGlobalCategoryFlag() == FALSE );
	else
		m_bSystemCategoryFilter = FALSE;
}

/**********************************************************************/

void CReportCategoryFilter::SetCategoryFilterNo( int nFilter )
{
	m_nCategoryFilterNo = nFilter;
	m_bSystemCategoryFilterByName = FALSE;
	m_strSystemCategoryFilterName = "";

	if ( TRUE == m_bSystemCategoryFilter )
	{
		m_CategoryFilter.CreateCategorySetSystem( m_nCategoryFilterNo );

		if ( m_CategoryFilter.CreatedEmptyFilter() == TRUE )
		{
			m_bSystemCategoryFilterByName = TRUE;
			m_strSystemCategoryFilterName = m_CategoryFilter.GetName();
		}
	}
	else
	{
		m_CategoryFilter.CreateCategorySet( m_nCategoryFilterNo );
	}
}

/**********************************************************************/

void CReportCategoryFilter::SetDatabase( int nDbIdx )
{
	if ( TRUE == m_bSystemCategoryFilterByName )
	{
		CString strSystemName = m_strSystemCategoryFilterName;
		strSystemName.MakeUpper();

		bool bGotMatch = FALSE;
		for ( int n = 0; n < DataManager.CategorySet.GetSize(); n++ )
		{
			CString strName = DataManager.CategorySet.GetName(n);
			strName.MakeUpper();

			if ( strSystemName == strName )
			{
				DataManager.CategorySet.GetAt( n, m_CategoryFilter );
				bGotMatch = TRUE;
				break;
			}
		}

		if ( FALSE == bGotMatch )
			m_CategoryFilter.IncludeAllCategories( FALSE );
	}
}

/**********************************************************************/

bool CReportCategoryFilter::CheckCategory( int nDeptNo )
{
	return m_CategoryFilter.GetCategoryStatus( nDeptNo );
}

/**********************************************************************/

const char* CReportCategoryFilter::GetFilterNameForReport()
{
	if ( ( FALSE == m_bSystemCategoryFilter ) || ( FALSE == m_bSystemCategoryFilterByName ) )
		m_strFilterNameForReport = m_CategoryFilter.GetFilterName(TRUE,TRUE);
	else
	{
		m_strFilterNameForReport = ", ";
		m_strFilterNameForReport += m_strSystemCategoryFilterName;
	}

	return m_strFilterNameForReport;
}

/**********************************************************************/
