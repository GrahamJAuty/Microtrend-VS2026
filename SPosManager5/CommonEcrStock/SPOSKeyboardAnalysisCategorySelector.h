#pragma once
/**********************************************************************/
#define CATEGORY_SELECTOR_NORMAL 0
#define CATEGORY_SELECTOR_SINGLE 1
#define CATEGORY_SELECTOR_REPORT 2
/**********************************************************************/
#define CATEGORYSET_ALL 9999
/**********************************************************************/

class CSPOSKeyboardAnalysisCategorySelector  
{
public:
	CSPOSKeyboardAnalysisCategorySelector( CEdit& edit, CSSComboBox& combo );
	void FillCategoryCombo( int nDepartmentFilter );
	void SelectCategoryFromCombo();
	void SelectCategoryFromEditBox();
	void UpdateCategoryCombo( int nNewCategoryFilter );
	void UpdateCategoryEditBox();
	int GetCategoryFilter() { return m_nCategoryFilter; }
	
private:
	CEdit& m_EditBox;
	CSSComboBox& m_ComboBox;
	int m_nCategoryFilter;
};

/**********************************************************************/
