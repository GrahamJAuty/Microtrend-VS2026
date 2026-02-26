/**********************************************************************/
#include "DepartmentSetCSVArray.h"
/**********************************************************************/
//DEPARTMENT SET HAS TO ALLOW FOR ALL POSSIBLE DEPARTMENTS
//SO THAT DEPARTMENTS WHICH HAVE PREVIOUSLY BEEN DELETED
//CAN STILL BE VIEWED IN REPORTS
/**********************************************************************/
#define DEPARTMENTSET_VERSION 2
/**********************************************************************/

CDepartmentSetCSVRecord::CDepartmentSetCSVRecord()
{
	m_bAllMode = FALSE;
	m_bCreatedEmptyFilter = FALSE;
	m_bConvertedEmptyFilter = FALSE;
	m_bBritanniaHotels = FALSE;

	for (int nIndex = 0; nIndex <= Dept::DeptNo.Max; nIndex++)
	{
		m_arrayIncluded.Add(FALSE);
	}

	ClearRecord();
}

/**********************************************************************/

void CDepartmentSetCSVRecord::ClearRecord()
{
	IncludeAllDepartments( FALSE );	
	m_strName = "";
	m_strFilterName = "";
	m_bReportEnabledSet = FALSE;
	m_bBritanniaHotels = FALSE;
}

/**********************************************************************/

int CDepartmentSetCSVRecord::CreateDepartmentSet ( int nDepartmentFilter )
{
	return CreateDepartmentSetInternal( DataManager.Department, DataManager.DepartmentSet, nDepartmentFilter );
}

/**********************************************************************/

int CDepartmentSetCSVRecord::CreateDepartmentSetSystem( int nDepartmentFilter )
{
	return CreateDepartmentSetInternal( DataManagerNonDb.SystemDepartment, DataManagerNonDb.SystemDepartmentSet, nDepartmentFilter );
}

/**********************************************************************/

int CDepartmentSetCSVRecord::CreateDepartmentSetInternal( CDepartmentCSVArray& DeptArray, CDepartmentSetCSVArray& DeptSetArray, int nDepartmentFilter )
{
	m_bReportEnabledSet = FALSE;
	m_bCreatedEmptyFilter = FALSE;

	IncludeAllDepartments( FALSE );
	int nDepartmentSet = nDepartmentFilter - FIRST_DEPARTMENTSET_FILTER;

	if (nDepartmentFilter == DEPARTMENTSET_BRITANNIA)
	{
		bool bFound = FALSE;
		for (int n = 0; n < DeptSetArray.GetSize(); n++)
		{
			CCSV csv(DeptSetArray.GetAt(n));
			ConvertFromCSV(csv);

			if (m_bBritanniaHotels == TRUE)
			{
				m_strFilterName = m_strName;
				m_bCreatedEmptyFilter = m_bConvertedEmptyFilter;
				bFound = TRUE;
				break;
			}
		}

		if (FALSE == bFound)
		{
			IncludeAllDepartments(TRUE);
			m_strFilterName = "All";
		}
	}
	else if ( nDepartmentFilter == DEPARTMENTSET_ALL )
	{
		IncludeAllDepartments ( TRUE );
		m_strFilterName = "All";
	}
	else if ( nDepartmentFilter == DEPARTMENTSET_REPORT )
	{
		CreateReportEnabledDepartmentSet(DeptArray);
		m_strFilterName = "Report enabled";
		m_bReportEnabledSet = TRUE;
	}
	else if ( nDepartmentFilter >= 0 && nDepartmentFilter < m_arrayIncluded.GetSize() )
	{
		int nDeptIdx = 0;
		if ( DeptArray.FindDeptByNumber ( nDepartmentFilter, nDeptIdx ) )
		{
			CDepartmentCSVRecord Department;
			DeptArray.GetAt ( nDeptIdx, Department );
			m_strFilterName = Department.GetReportText();
		}
		else
		{
			m_strFilterName = "Unknown";
		}

		IncludeDepartment ( nDepartmentFilter, TRUE );
	}
	else if ( ( nDepartmentSet >= 0 ) && ( nDepartmentSet < DeptSetArray.GetSize() ) )
	{
		CCSV csv ( DeptSetArray.GetAt ( nDepartmentSet ) );
		ConvertFromCSV ( csv );
		m_strFilterName = m_strName;
		m_bCreatedEmptyFilter = m_bConvertedEmptyFilter;
	}
	else	
	{
		nDepartmentFilter = DEPARTMENTSET_ALL;
		IncludeAllDepartments ( TRUE );
		m_strFilterName = "All";
	}

	return nDepartmentFilter;
}

/**********************************************************************/

void CDepartmentSetCSVRecord::CreateReportEnabledDepartmentSet( CDepartmentCSVArray& arrayDept )
{
	IncludeAllDepartments( TRUE );
	
	for ( int nDeptNo = Dept::DeptNo.Min; nDeptNo <= Dept::DeptNo.Max; nDeptNo++ )
	{
		int nDeptIdx = 0;
		if ( arrayDept.FindDeptByNumber ( nDeptNo, nDeptIdx ) )
		{
			CDepartmentCSVRecord Department;
			arrayDept.GetAt ( nDeptIdx, Department );
			if ( Department.GetReportEnable() == FALSE )
			{
				IncludeDepartment ( nDeptNo, FALSE );
				m_bAllMode = FALSE;
			}
		}
	}
	m_strFilterName = "Report enabled";
}

/**********************************************************************/

int CDepartmentSetCSVRecord::GetDefaultDepartment()
{
	for (int nDeptNo = 0; nDeptNo < m_arrayIncluded.GetSize(); nDeptNo++)
	{
		if (GetDepartmentStatus(nDeptNo) == TRUE)
		{
			return nDeptNo;
		}
	}

	return 0;
}

/**********************************************************************/

void CDepartmentSetCSVRecord::IncludeAllDepartments( bool bFlag )
{
	m_strName = "";
	for (int nDeptIdx = 0; nDeptIdx < m_arrayIncluded.GetSize(); nDeptIdx++)
	{
		m_arrayIncluded.SetAt(nDeptIdx, bFlag);
	}

	m_bAllMode = bFlag;
}

/**********************************************************************/

const char* CDepartmentSetCSVRecord::GetFilterName( bool bTrailingComma, bool bHideReportEnabled )
{
	m_strFilterNameOut = "";

	if ( ( FALSE == bHideReportEnabled ) || ( FALSE == m_bReportEnabledSet ) )
	{
		if (TRUE == bTrailingComma)
		{
			m_strFilterNameOut = ", ";
		}

		m_strFilterNameOut += m_strFilterName;
	}
	
	return m_strFilterNameOut;
}

/**********************************************************************/

void CDepartmentSetCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	m_bConvertedEmptyFilter = FALSE;

	ClearRecord();

	m_strName = csv.GetString (0);
	
	switch ( csv.GetInt(1) )
	{
	case 1:	
		V1To2ConvertFromCSV ( csv, 1 );	
		break;

	case 2:
		V1To2ConvertFromCSV ( csv, 2 );
		break;
	}
}

/**********************************************************************/

void CDepartmentSetCSVRecord::V1To2ConvertFromCSV ( CCSV& csv, int nVer )
{
	int nOffset = 2;

	if (nVer >= 2)
	{
		m_bBritanniaHotels = csv.GetBool(nOffset++);
	}

	int nCount = csv.GetInt(nOffset++);

	m_bConvertedEmptyFilter = TRUE;

	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		if (IncludeDepartment(csv.GetInt(nOffset++), TRUE) == TRUE)
		{
			m_bConvertedEmptyFilter = FALSE;
		}
	}
}

/**********************************************************************/
	
void CDepartmentSetCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_strName );
	csv.Add ( DEPARTMENTSET_VERSION );				
	csv.Add ( m_bBritanniaHotels );

	CWordArray arrayTemp;

	for (int nDeptNo = 0; nDeptNo <= m_arrayIncluded.GetSize(); nDeptNo++)
	{
		if (GetDepartmentStatus(nDeptNo) == TRUE)
		{
			arrayTemp.Add(nDeptNo);
		}
	}

	csv.Add ( arrayTemp.GetSize() );

	for (int nIndex = 0; nIndex < arrayTemp.GetSize(); nIndex++)
	{
		csv.Add(arrayTemp.GetAt(nIndex));
	}
}

/**********************************************************************/
			
bool CDepartmentSetCSVRecord::IncludeDepartment ( int nDeptNo, bool bFlag )
{
	bool bSetDepartment = FALSE;

	if ( ( nDeptNo >= 0 ) && ( nDeptNo < m_arrayIncluded.GetSize() ) )
	{
		m_arrayIncluded.SetAt ( nDeptNo, bFlag );
		bSetDepartment = bFlag;
	}

	return bSetDepartment;
}

/**********************************************************************/

bool CDepartmentSetCSVRecord::GetDepartmentStatus( int nDeptNo )
{
	bool bResult = FALSE;
	if (nDeptNo >= 0 && nDeptNo < m_arrayIncluded.GetSize())
	{
		bResult = (m_arrayIncluded.GetAt(nDeptNo) != 0);
	}

	return bResult;
}

/**********************************************************************/

CDepartmentSetCSVArray::CDepartmentSetCSVArray() : CSharedCSVArray()
{
}

/**********************************************************************/

int CDepartmentSetCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if (nReply != DB_ERR_SHARE)
	{
		if (nMode == DB_READONLY)	// if file does not exist use default values
		{
			nReply = DB_ERR_NONE;
		}
	}

	return nReply;
}

/**********************************************************************/

const char* CDepartmentSetCSVArray::GetName ( int nIndex )
{
	m_strName = "";
	CString strTemp = GetAt ( nIndex );
	
	if ( strTemp != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		m_strName = csv.GetString ( 0 );
	}

	return m_strName;
}

/**********************************************************************/

bool CDepartmentSetCSVArray::GetBritanniaHotelsFlag(int nIndex)
{
	bool bResult = FALSE;

	if ((nIndex >= 0) && (nIndex < GetSize()))
	{
		CDepartmentSetCSVRecord record;
		GetAt(nIndex, record);
		bResult = record.GetBritanniaHotelsFlag();
	}

	return bResult;
}

/**********************************************************************/

