#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

class CCascadingMacroSelector  
{
public:
	CCascadingMacroSelector( CEdit& edit, CSSComboBox& combo );

public:
	void FillMacroCombo( int nMacroNo );
	
public:
	void SelectMacroFromCombo();
	void SelectMacroFromEditBox();
	void UpdateMacroCombo( int nNewMacroNo );
	void UpdateMacroEditBox();
	int GetMacroNo() { return m_nMacroNo; }
	
private:
	CEdit& m_EditBox;
	CSSComboBox& m_ComboBox;
	int m_nMacroNo;
};

/**********************************************************************/
#endif
/**********************************************************************/
