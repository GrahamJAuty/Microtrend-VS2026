#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "DepartmentSelector.h"
#include "..\CommonEcrStock\LocationSelector.h"
/**********************************************************************/

class CDeptAndPluComboHelpers
{
public:
	CDeptAndPluComboHelpers(void);
	~CDeptAndPluComboHelpers(void);

public:
	void SetControlPointers( CSSComboBox* pComboPluFilter, CLocationSelector* pLocationSelector, CDepartmentSelector* pDepartmentSelector );
	
public:
	void UpdateCombos();
	int GetPluFilterNo();
	void SetPluFilterNo( int nFilterNo );
	
private:
	void ReloadPluFilterCombo( bool bSystem );

private:
	CSSComboBox* m_pComboPluFilter;
	CLocationSelector* m_pLocationSelector;
	CDepartmentSelector* m_pDepartmentSelector;

private:
	bool m_bCurrentSystemFlag;
	int m_nCurrentDbIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
