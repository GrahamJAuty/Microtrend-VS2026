#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "CategorySelectorNew.h"
#include "..\CommonEcrStock\LocationSelector.h"
/**********************************************************************/

class CCategoryComboHelpers
{
public:
	CCategoryComboHelpers(void);
	~CCategoryComboHelpers(void);

public:
	void SetControlPointers( CLocationSelector* pLocationSelector, CCategorySelectorNew* pCategorySelector );
	void EnableCustomFields(){ m_bDoCustomFields = TRUE; }
	
public:
	void UpdateCombos();
	
private:
	CLocationSelector* m_pLocationSelector;
	CCategorySelectorNew* m_pCategorySelector;

private:
	bool m_bCurrentSystemFlag;
	int m_nCurrentDbIdx;
	bool m_bDoCustomFields;
};

/**********************************************************************/
#endif
/**********************************************************************/
