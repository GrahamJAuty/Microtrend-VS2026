#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/

class CInPlaceComboNormal : public CSSComboBox
{
public:
	static CInPlaceComboNormal* GetInstance(); 

	static void DeleteInstance(); 

	BOOL ShowComboCtrl(DWORD dwStyle, const CRect& rCellRect, CWnd* pParentWnd, UINT uiResourceID,
					   int iRowIndex, int iColumnIndex, CStringList* pDropDownList, int iCurSel = -1);

	//{{AFX_VIRTUAL(CInPlaceCombo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CInPlaceCombo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCloseup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	//UNCOMMENT THESE IF NECESSARY TO PREVENT ASSERTS
	//void DrawItem(LPDRAWITEMSTRUCT) {}
	void MeasureItem(LPMEASUREITEMSTRUCT) {}
	//int CompareItem(LPCOMPAREITEMSTRUCT) { return 0;  }
	
private:
	CInPlaceComboNormal();

	// Hide the copy constructor and operator =
	CInPlaceComboNormal (CInPlaceComboNormal&) {}
	//operator = (CInPlaceCombo) {}

	virtual ~CInPlaceComboNormal();

	int m_iRowIndex;
	int m_iColumnIndex;
	BOOL m_bESC;
	
	static CInPlaceComboNormal* m_pInPlaceCombo;

	CString m_strWindowText;

	CStringList m_DropDownList;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
