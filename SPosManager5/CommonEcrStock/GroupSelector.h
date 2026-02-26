#pragma once
/**********************************************************************/
#include "GroupCSVArrayBase.h"
//include "MyComboBox.h"
/**********************************************************************/

class CGroupSelector  
{
public:
	CGroupSelector( CEdit& edit, CSSComboBox& combo, int nGroupType );
	void FillGroupCombo( int nGroupFilter );
	void SelectGroupFromCombo();
	void SelectGroupFromEditBox();
	void UpdateGroupCombo( int nNewGroupFilter );
	void UpdateGroupEditBox();
	int GetGroupFilter() { return m_nGroupFilter; }

private:
	CGroupCSVArrayBase* GetGroupArray();
	
private:
	CEdit& m_EditBox;
	CSSComboBox& m_ComboBox;
	int m_nGroupFilter;

private:
	int m_nGroupType;
	int m_nGroupNoMin;
	int m_nGroupNoMax;
};

/**********************************************************************/

