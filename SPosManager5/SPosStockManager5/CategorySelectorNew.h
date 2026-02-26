#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#define NEWCATEGORY_SELECTOR_NORMAL 0
#define NEWCATEGORY_SELECTOR_SINGLE 1
#define NEWCATEGORY_SELECTOR_REPORT 2
/**********************************************************************/
#include "DefTextCategory.h"
/**********************************************************************/

class CCategorySelectorNew  
{
public:
	CCategorySelectorNew( CSSComboBox& combo );

public:
	void FillCategoryComboDatabase( int nCategoryFilter, int nMode, bool bMatchString = FALSE );
	void FillCategoryComboSystem( int nCategoryFilter, bool bMatchString = FALSE );
	
public:
	void UpdateCategoryCombo( int nNewCategoryFilter );
	int GetCategoryFilter();
	
private:
	void FillCategoryComboInternal( CDefTextCategory& CatArray, CCategorySetCSVArray& CatSetArray, int nCategoryFilter, int nMode, bool bMatchString );
	
private:
	CSSComboBox& m_ComboBox;
};

/**********************************************************************/
#endif
/**********************************************************************/
