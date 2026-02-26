#pragma once
/**********************************************************************/
#define DEPARTMENT_SELECTOR_NORMAL 0
#define DEPARTMENT_SELECTOR_SINGLE 1
#define DEPARTMENT_SELECTOR_REPORT 2
/**********************************************************************/

class CDepartmentSelector  
{
public:
	CDepartmentSelector( CEdit& edit, CSSComboBox& combo, bool bModifierMode = FALSE );

public:
	void FillDepartmentComboDatabase( int nDepartmentFilter, int nMode, bool bMatchString = FALSE, bool bBritannia = FALSE);
	void FillDepartmentComboSystem( int nDepartmentFilter, bool bMatchString = FALSE, bool bBritannia = FALSE);
	
public:
	void SelectDepartmentFromCombo();
	void SelectDepartmentFromEditBox();
	void UpdateDepartmentCombo( int nNewDepartmentFilter );
	void UpdateDepartmentEditBox();
	int GetDepartmentFilter() { return m_nDepartmentFilter; }

private:
	void FillDepartmentComboInternal( CDepartmentCSVArray& DeptArray, CDepartmentSetCSVArray& DeptSetArray, int nDepartmentFilter, int nMode, bool bMatchString, bool bBritannia = FALSE);
	int GetItemDataToUse( int n );

private:
	CEdit& m_EditBox;
	CSSComboBox& m_ComboBox;
	int m_nDepartmentFilter;
	bool m_bModifierMode;
};

/**********************************************************************/
