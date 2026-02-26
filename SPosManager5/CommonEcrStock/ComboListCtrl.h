#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include <afxtempl.h>
/**********************************************************************/
class CInPlaceComboNormal;
class CInPlaceComboTabbed;
class CInPlaceEdit;
class CInPlaceEditText;
/**********************************************************************/
#define CELL_TYPE_READONLY 0
#define CELL_TYPE_EDIT 1
#define CELL_TYPE_COMBO_NORMAL 2
#define CELL_TYPE_COMBO_TABBED 3
#define CELL_TYPE_BUTTON 4
#define CELL_TYPE_EDIT_TEXT 5
/**********************************************************************/
#include "WMNumbers.h"
/**********************************************************************/

struct CellTypeInfoTextItem
{
public:
	CellTypeInfoTextItem();

public:
	CString m_strListText;
	CString m_strCellText;
};

/**********************************************************************/

struct CellTypeInfoTextBuffer
{
public:
	CellTypeInfoTextBuffer();
	void AddNoChangeHeader( bool bTabbed );
	void AddYesNoTexts();
	void AddText( const char* szText );
	void AddText( const char* szList, const char* szCell );
	void AddTabbedText( const char* szText );

public:
	int GetSize(){ return m_arrayItems.GetSize(); }
	void GetItem( int n, CellTypeInfoTextItem& item );

public:
	CArray<CellTypeInfoTextItem,CellTypeInfoTextItem> m_arrayItems;
};

/**********************************************************************/

struct CellTypeInfo
{
public:
	CellTypeInfo();
	void ResetForCombo( CellTypeInfoTextBuffer& arrayTexts );

public:
	int m_nCol;
	int m_nRow;
	int m_nType;
	//EDIT CONTROL
	int m_nSSEditType;
	int m_nSSEditWidth;
	CString m_strSSEditFormat;
	//COMBO CONTROL
	CString m_strComboTabs;
	CString m_strComboListText;
	CString m_strComboCellText;
	int m_nComboSelect;
	//CURRENT VALUE
	CString m_strCurrent;
	//VALIDATION
	int m_nMinInt;
	int m_nMaxInt;
	double m_dMinDouble;
	double m_dMaxDouble;
};

/**********************************************************************/

class CComboListCtrl : public CListCtrl
{
public:
	CComboListCtrl();

	// Destructor
	virtual ~CComboListCtrl();

	// Sets the vertical scroll
	void EnableVScroll(bool bEnable = true);

	// Sets the horizontal scroll
	void EnableHScroll(bool bEnable = true);

	void AllowListScroll( bool bEnable );
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboListCtrl)
	//}}AFX_VIRTUAL

protected:
	void PreSubclassWindow();

// Methods
	// Generated message map functions
	//{{AFX_MSG(CComboListCtrl)
	virtual void DrawItem ( LPDRAWITEMSTRUCT lpDrawItemStruct );
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void HandleReturn( int nActionCol );
	void SetEditCell( int nRow, int nCol ){ m_nEditRow = nRow; m_nEditCol = nCol; }
	bool GetInEditFlag(){ return m_bInEdit; }

private:
	void ProcessEmbeddedControl( int nRow, int nCol );

private:
			
// Implementation

	// Returns the row & column index of the column on which mouse click event has occured
	bool HitTestEx(CPoint& rHitPoint, int* pRowIndex, int* pColumnIndex) const;

	// Creates and displays the in place combo box
	CInPlaceComboNormal* ShowInPlaceComboNormal(int iRowIndex, int iColumnIndex, CStringList& rComboItemsList, int iSel = -1);
	CInPlaceComboTabbed* ShowInPlaceComboTabbed(int iRowIndex, int iColumnIndex, CStringList& rComboItemsList, int iSel = -1);

	// Creates and displays the in place edit control
	CInPlaceEdit* ShowInPlaceEdit(int iRowIndex, int iColumnIndex, CString& rstrCurSelection, CellTypeInfo& info );
	CInPlaceEditText* ShowInPlaceEditText(int iRowIndex, int iColumnIndex, CString& rstrCurSelection, CellTypeInfo& info );

	// Calculates the cell rect
	void CalculateCellRect(int iColumnIndex, int iRowIndex, CRect& robCellRect, bool bCombo );

	// Scrolls the list ctrl to bring the in place ctrl to the view
	void ScrollToView(int iColumnIndex, /*int iOffSet, */CRect& obCellRect);

// Attributes
	// The window style of the in place edit ctrl
	DWORD m_dwEditCtrlStyle;

	// The window style of the in place combo ctrl
	DWORD m_dwDropDownCtrlStyle;

	bool m_bAllowListScroll;

public:
	void AdjustRowHeight( LPMEASUREITEMSTRUCT lpMeasureItemStruct, int nAdjust );

private:
	bool m_bInEdit;
	int m_nEditRow;
	int m_nEditCol;
	int m_nCellXMargin;
	int m_nCellYMargin;

private:
	bool m_bAdjustedRowHeight;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
