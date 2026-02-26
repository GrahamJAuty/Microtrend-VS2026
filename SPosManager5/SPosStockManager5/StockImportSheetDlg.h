#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "StockImportSheetHelpers.h"
/**********************************************************************/

class CStockImportSheetDlg : public CSSDialog
{
public:
	CStockImportSheetDlg( int nSpIdx, int nStocktakeNo, CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CStockImportSheetDlg)
	enum { IDD = IDD_STOCK_IMPORTSHEET };
	CEdit	m_editDestination;
	CButton m_radioCopy1;
	CButton m_radioCopy2;
	CSSComboBox m_comboCopies;
	CSSComboBox	m_comboSortOrder;
	CButton m_buttonSet;
	CButton m_checkAlphaSort;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CStockImportSheetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	void AddFieldToList( int nItemData, bool bTagged );
	const char* GetSavedSettingsLine(){ return m_strSavedSettingsLine; }

protected:
	//{{AFX_MSG(CStockImportSheetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void OnOK();
	afx_msg void OnRadioCopy1();
	afx_msg void OnRadioCopy2();
	afx_msg void OnButtonSet();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonUp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool CreateSavedSettingsLine();
	void FillSortOrderCombo( int nSortOrder );

public:
	int m_nCopies;
	
private:
	CStockImportSheetHelpers m_ImportSheetHelpers;
	CSSListTaggedSelectCtrl m_listFields;
	CString m_strSavedSettingsLine;
	int m_nSpIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
