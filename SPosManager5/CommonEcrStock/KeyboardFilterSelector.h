/**********************************************************************/
#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
//include "MyComboBoxEx.h"
/**********************************************************************/

struct CKeyboardFilterSelectorLoc
{
public:
	CKeyboardFilterSelectorLoc() {
		m_nNodeType = 0;
		m_nEntityIdx = 0;
		m_strKeyboards = "";
	}

public:
	int m_nNodeType;
	int m_nEntityIdx;
	CString m_strKeyboards;
};

/**********************************************************************/

class CKeyboardFilterSelector
{
public:
	CKeyboardFilterSelector(void);
	
public:
	void FillCombos( CSSComboBoxEx* pComboLocation, CComboBox* pComboKeyboard );
	
private:
	void AddKeyboardLocation( int nNodeType, int nEntityIdx );

public:
	void SelectLocation();
	void GetSelection( CKeyboardFilterSelectorLoc& Loc, int& nKbNo );
	void SetSelection( CKeyboardFilterSelectorLoc& Loc, int nKbNo );

private:
	CSSComboBoxEx* m_pComboLocation;
	CComboBox* m_pComboKeyboard;
	CImageList m_ImageList;

private:
	CArray<CKeyboardFilterSelectorLoc,CKeyboardFilterSelectorLoc> m_arrayKbdLocs;
	int m_nLocComboCurrentSelection;
};

/**********************************************************************/
#endif
/**********************************************************************/