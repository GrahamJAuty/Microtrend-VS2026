#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CategorySelector.h"
#include "..\CommonEcrStock\ComboListCtrl.h"
#include "StockFilterArray.h"
/**********************************************************************/

class CStockRangeCustomFieldDlg : public CSSDialog
{
public:
	CStockRangeCustomFieldDlg( CStockFilterArray& FilterArray, CWnd* pParent = NULL);   
	~CStockRangeCustomFieldDlg();

	//{{AFX_DATA(CStockRangeCustomFieldDlg)
	enum { IDD = IDD_STOCK_RANGE_CUSTOM_FIELD };
	CSSComboBox	m_comboCategory;
	CString	m_strStockCodeFrom;
	CString	m_strStockCodeTo;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockRangeCustomFieldDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	bool GetModifiedFlag() { return m_bModifiedStock; }

protected:
	//{{AFX_MSG(CStockRangeCustomFieldDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillFocusStockCode();
	//}}AFX_MSG
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT PopulateComboList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT GetCellType(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	BOOL PreTranslateMessage(MSG* pMsg);

private:
	void FillCategoryCombo();

private:
	void AddList();
	void AddField( CCSV& csv );
	void AddYesNoField( int nType, const char* szText);
	void AddCustomField( int nFieldNo );
	void AddCategoryField();
	void AddStockUnitField();
	int UpdateProgram();

private:
	CComboListCtrl m_listSettings;
	CCategorySelector m_CategorySelector;

private:
	CArray<int,int> m_arrayCustomFieldValues[6];
	CWordArray m_arrayCategoryNo;
	
private:
	CArray<CellTypeInfo,CellTypeInfo> m_arrayCellTypes;
	CStockFilterArray& m_FilterArray;
	bool m_bModifiedStock;	
};

/**********************************************************************/
#endif
/**********************************************************************/

