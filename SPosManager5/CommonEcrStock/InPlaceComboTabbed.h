#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

class CInPlaceComboTabbed : public CSSTabbedComboBox
{
public:
	static CInPlaceComboTabbed* GetInstance(); 

	static void DeleteInstance(); 

	BOOL ShowComboCtrl(DWORD dwStyle, const CRect& rCellRect, CWnd* pParentWnd, UINT uiResourceID,
					   int iRowIndex, int iColumnIndex, CStringList* pDropDownList, int iCurSel = -1);

	//{{AFX_VIRTUAL(CInPlaceComboTabbed)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CInPlaceComboTabbed)
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
	CInPlaceComboTabbed();

	// Hide the copy constructor and operator =
	CInPlaceComboTabbed (CInPlaceComboTabbed&) {}
	//operator = (CInPlaceCombo) {}

	virtual ~CInPlaceComboTabbed();

	int m_iRowIndex;
	int m_iColIndex;
	BOOL m_bESC;
	
	static CInPlaceComboTabbed* m_pInPlaceCombo;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
