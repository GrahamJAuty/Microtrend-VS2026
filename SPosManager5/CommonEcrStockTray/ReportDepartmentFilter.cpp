/**********************************************************************/
#include "NodeTypes.h"
/**********************************************************************/
#include "ReportDepartmentFilter.h"
/**********************************************************************/

CReportDepartmentFilter::CReportDepartmentFilter()
{
	m_nDeptFilterNo = 1;
	m_bSystemDeptFilter = FALSE;
	m_bSystemDeptFilterByName = FALSE;
	m_strSystemDeptFilterName = "";
	m_DeptFilter.IncludeDepartment(1,TRUE);
}

/**********************************************************************/

void CReportDepartmentFilter::SetConsolidationType(int nConType)
{
	if (NODE_SYSTEM == nConType)
	{
		m_bSystemDeptFilter = (EcrmanOptions.GetGlobalDepartmentFlag() == FALSE);
	}
	else
	{
		m_bSystemDeptFilter = FALSE;
	}
}

/**********************************************************************/

void CReportDepartmentFilter::SetDeptFilterNo( int nFilter )
{
	m_nDeptFilterNo = nFilter;
	m_bSystemDeptFilterByName = FALSE;
	m_strSystemDeptFilterName = "";

	if ( TRUE == m_bSystemDeptFilter )
	{
		m_DeptFilter.CreateDepartmentSetSystem( m_nDeptFilterNo );

		if ( m_DeptFilter.CreatedEmptyFilter() == TRUE )
		{
			m_bSystemDeptFilterByName = TRUE;
			m_strSystemDeptFilterName = m_DeptFilter.GetName();
		}
	}
	else
	{
		m_DeptFilter.CreateDepartmentSet( m_nDeptFilterNo );
	}
}

/**********************************************************************/

void CReportDepartmentFilter::SetDatabase( int nDbIdx )
{
	if ( TRUE == m_bSystemDeptFilterByName )
	{
		CString strSystemName = m_strSystemDeptFilterName;
		strSystemName.MakeUpper();

		bool bGotMatch = FALSE;
		for ( int n = 0; n < DataManager.DepartmentSet.GetSize(); n++ )
		{
			CString strName = DataManager.DepartmentSet.GetName(n);
			strName.MakeUpper();

			if ( strSystemName == strName )
			{
				DataManager.DepartmentSet.GetAt( n, m_DeptFilter );
				bGotMatch = TRUE;
				break;
			}
		}

		if (FALSE == bGotMatch)
		{
			m_DeptFilter.IncludeAllDepartments(FALSE);
		}
	}
}

/**********************************************************************/

bool CReportDepartmentFilter::CheckDepartment( int nDeptNo )
{
	return m_DeptFilter.GetDepartmentStatus( nDeptNo );
}

/**********************************************************************/

const char* CReportDepartmentFilter::GetFilterNameForReport()
{
	if ((FALSE == m_bSystemDeptFilter) || (FALSE == m_bSystemDeptFilterByName))
	{
		m_strFilterNameForReport = m_DeptFilter.GetFilterName(TRUE, TRUE);
	}
	else
	{
		m_strFilterNameForReport = ", ";
		m_strFilterNameForReport += m_strSystemDeptFilterName;
	}

	return m_strFilterNameForReport;
}

/**********************************************************************/
