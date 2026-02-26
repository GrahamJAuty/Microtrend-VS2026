/**********************************************************************/
#include "DepartmentSetCSVArray.h"
/**********************************************************************/
#include "DepartmentSelector.h"
/**********************************************************************/

CDepartmentSelector::CDepartmentSelector( CEdit& edit, CSSComboBox& combo, bool bModifierMode ) : m_EditBox ( edit ), m_ComboBox ( combo )
{
	m_nDepartmentFilter = DEPARTMENTSET_ALL;
	m_bModifierMode = bModifierMode;
}

/**********************************************************************/

int CDepartmentSelector::GetItemDataToUse( int n )
{
	return m_bModifierMode ? n + 1 : n;
}

/**********************************************************************/

void CDepartmentSelector::FillDepartmentComboDatabase( int nDepartmentFilter, int nMode, bool bMatchString, bool bBritannia)
{
	FillDepartmentComboInternal( DataManager.Department, DataManager.DepartmentSet, nDepartmentFilter, nMode, bMatchString, bBritannia );
}

/**********************************************************************/

void CDepartmentSelector::FillDepartmentComboSystem( int nDepartmentFilter, bool bMatchString, bool bBritannia )
{
	FillDepartmentComboInternal( DataManagerNonDb.SystemDepartment, DataManagerNonDb.SystemDepartmentSet, nDepartmentFilter, DEPARTMENT_SELECTOR_REPORT, bMatchString, bBritannia );
}

/**********************************************************************/

void CDepartmentSelector::FillDepartmentComboInternal( CDepartmentCSVArray& DeptArray, CDepartmentSetCSVArray& DeptSetArray, int nDepartmentFilter, int nMode, bool bMatchString, bool bBritannia )
{
	if ( nMode != DEPARTMENT_SELECTOR_SINGLE )
	{
		m_bModifierMode = FALSE;
	}

	CDepartmentSetCSVRecord DepartmentSet;
	DepartmentSet.CreateReportEnabledDepartmentSet(DeptArray);

	CString strOldString = "";
	if ( TRUE == bMatchString )
	{
		m_ComboBox.GetWindowText( strOldString );
		strOldString.MakeUpper();
	}
	
	m_ComboBox.ResetContent();

	for ( int nIndex = 0; nIndex < DeptArray.GetSize(); nIndex++ )
	{
		CDepartmentCSVRecord Department;
		DeptArray.GetAt( nIndex, Department );
		if (Department.GetDeptNo() == 0)
		{
			continue;
		}

		if ( ( nMode != DEPARTMENT_SELECTOR_REPORT ) || DepartmentSet.GetDepartmentStatus ( nIndex ) == TRUE )
		{
			CString strText = Department.GetReportText();

			if ( strText != Department.GetDefaultText() )
			{
				int nInsertionPoint = m_ComboBox.AddString ( strText );
				m_ComboBox.SetItemData ( nInsertionPoint, GetItemDataToUse( Department.GetDeptNo() ) );
			}
		}
	}

	for ( int nIndex = 0; nIndex < DeptArray.GetSize(); nIndex++ )
	{
		CDepartmentCSVRecord Department;
		DeptArray.GetAt( nIndex, Department );
		if (Department.GetDeptNo() == 0)
		{
			continue;
		}

		if ( ( nMode != DEPARTMENT_SELECTOR_REPORT ) || DepartmentSet.GetDepartmentStatus ( nIndex ) == TRUE )
		{
			CString strText = Department.GetReportText();

			if ( strText == Department.GetDefaultText() )
			{
				int nInsertionPoint = m_ComboBox.InsertString ( -1, strText );
				m_ComboBox.SetItemData ( nInsertionPoint, GetItemDataToUse( Department.GetDeptNo() ) );
			}
		}
	}

	int nDeptIdx;
	if ( DeptArray.FindDeptByNumber ( 0, nDeptIdx ) )
	{
		if ( DepartmentSet.GetDepartmentStatus(0) )
		{
			CDepartmentCSVRecord Department;
			DeptArray.GetAt ( nDeptIdx, Department );
			m_ComboBox.InsertString (0, Department.GetReportText() );
			m_ComboBox.SetItemData(0, GetItemDataToUse(0));
		}
	}

	if ( TRUE == m_bModifierMode )
	{
		m_ComboBox.InsertString (0, "As Base Plu" );
		m_ComboBox.SetItemData(0,0);
	}

	switch ( nMode )
	{
	case DEPARTMENT_SELECTOR_NORMAL:
		m_ComboBox.InsertString ( 0, "All" );
		m_ComboBox.SetItemData ( 0, DEPARTMENTSET_ALL );
		break;

	case DEPARTMENT_SELECTOR_REPORT:
		m_ComboBox.InsertString ( 0, "Report enabled" );
		m_ComboBox.SetItemData ( 0, DEPARTMENTSET_REPORT );
		break;

	case DEPARTMENT_SELECTOR_SINGLE:
	default:
		break;
	}

	switch ( nMode )
	{
	case DEPARTMENT_SELECTOR_NORMAL:
	case DEPARTMENT_SELECTOR_REPORT:
		{
			int nDepartmentSetPos = 1;
			for (int nIndex = 0; nIndex < DeptSetArray.GetSize(); nIndex++)
			{
				bool bInclude = TRUE;
				if (TRUE == bBritannia)
				{
					bInclude = DeptSetArray.GetBritanniaHotelsFlag(nIndex);
				}

				if (TRUE == bInclude)
				{
					m_ComboBox.InsertString(nDepartmentSetPos, DeptSetArray.GetName(nIndex));
					m_ComboBox.SetItemData(nDepartmentSetPos++, FIRST_DEPARTMENTSET_FILTER + nIndex);
				}
			}
		}
		break;

	case DEPARTMENT_SELECTOR_SINGLE:
	default:
		break;
	}

	m_ComboBox.SetCurSel(0);
	m_nDepartmentFilter = m_ComboBox.GetItemData(0);

	bool bFound = FALSE;
	
	if ( FALSE == bMatchString )
	{
		for ( int nIndex = 0; nIndex < m_ComboBox.GetCount(); nIndex++ )
		{
			if ( m_ComboBox.GetItemData ( nIndex ) == (WORD) nDepartmentFilter )
			{
				m_ComboBox.SetCurSel( nIndex );
				m_nDepartmentFilter = nDepartmentFilter;
				bFound = TRUE;
				break;
			}
		}
	}
	else if ( strOldString != "" )
	{
		for ( int nIndex = 0; nIndex < m_ComboBox.GetCount(); nIndex++ )
		{
			CString strNewString = "";
			m_ComboBox.GetLBText(nIndex,strNewString);
			strNewString.MakeUpper();

			if ( strNewString == strOldString )
			{
				m_ComboBox.SetCurSel( nIndex );
				m_nDepartmentFilter = m_ComboBox.GetItemData(nIndex);
				bFound = TRUE;
				break;
			}
		}
	}

	if ( ( bFound == FALSE ) && ( nMode == DEPARTMENT_SELECTOR_SINGLE ) )
	{
		m_ComboBox.InsertString ( 0, "Unknown" );
		m_ComboBox.SetItemData ( 0, nDepartmentFilter  );
		m_ComboBox.SetCurSel(0);
		m_nDepartmentFilter = nDepartmentFilter;
	}

	UpdateDepartmentEditBox();
}

/**********************************************************************/

void CDepartmentSelector::UpdateDepartmentCombo( int nNewDepartmentFilter)
{
	for ( int nIndex = 0; nIndex <= m_ComboBox.GetCount(); nIndex++ )
	{
		if ( m_ComboBox.GetItemData ( nIndex ) == ( WORD ) nNewDepartmentFilter )
		{
			m_ComboBox.SetCurSel ( nIndex );
			m_nDepartmentFilter = nNewDepartmentFilter;
			return;
		}
	}
}

/**********************************************************************/

void CDepartmentSelector::UpdateDepartmentEditBox()
{
	CString strDeptNo = "";

	int nDepartmentFilter = m_nDepartmentFilter;
	if ( TRUE == m_bModifierMode )
	{
		nDepartmentFilter--;
	}

	if ( ( nDepartmentFilter >= Dept::DeptNo.Min ) && ( nDepartmentFilter <= Dept::DeptNo.Max ) )
	{
		strDeptNo.Format ( "%d", nDepartmentFilter );
	}

	m_EditBox.SetWindowText ( strDeptNo );
}

/**********************************************************************/

void CDepartmentSelector::SelectDepartmentFromCombo() 
{
	int nIndex = m_ComboBox.GetCurSel();
	
	if ( nIndex != CB_ERR )
	{
		int nNewDepartmentFilter = m_ComboBox.GetItemData ( nIndex );

		if ( nNewDepartmentFilter != m_nDepartmentFilter )
		{
			m_nDepartmentFilter = nNewDepartmentFilter;
			UpdateDepartmentEditBox();
		}
	}
}

/**********************************************************************/

void CDepartmentSelector::SelectDepartmentFromEditBox() 
{
	CString strDeptNo;
	m_EditBox.GetWindowText( strDeptNo );
	
	::TrimSpaces ( strDeptNo, FALSE );
	
	if ( strDeptNo == "" )
	{
		UpdateDepartmentEditBox();
		return;
	}

	int nNewDepartmentFilter = atoi ( strDeptNo );

	if ( TRUE == m_bModifierMode )
	{
		nNewDepartmentFilter++;
	}

	if ( nNewDepartmentFilter != m_nDepartmentFilter )
	{
		UpdateDepartmentCombo( nNewDepartmentFilter );
		UpdateDepartmentEditBox();
	}
}

/**********************************************************************/
