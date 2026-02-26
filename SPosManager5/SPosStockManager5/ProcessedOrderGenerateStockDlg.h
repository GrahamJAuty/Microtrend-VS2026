#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CategorySelector.h"
#include "..\CommonEcrStock\LocationSelector.h"
#include "ProcessedOrderGenerateDlgHelpers.h"
#include "ProcessedOrderPreviewDatabase.h"
/**********************************************************************/

class CProcessedOrderGenerateStockDlg : public CSSDialog
{
public:
	CProcessedOrderGenerateStockDlg( CArray<int,int>& arrayEntityIdx, CWnd* pParent = NULL);
	~CProcessedOrderGenerateStockDlg();

	//{{AFX_DATA(CProcessedOrderGenerateStockDlg)
	enum { IDD = IDD_ORDER_GENERATE_STOCK };
	CSSListCtrl m_listDatabase;
	CSSComboBox	m_comboSupplier;
	CSSListMultiSelectCtrl m_listSupplier;
	CButton m_buttonAll;
	CButton m_buttonNone;
	CStatic m_staticPrepare1;
	CStatic m_staticPrepare2;
	CSSComboBoxEx m_comboConLevel;
	CStatic m_staticPrepare3;
	CSSComboBox	m_comboCategory;
	CButton m_buttonDisplay;
	CStatic m_staticOrder1;
	CSSComboBox	m_comboMethod;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	int		m_nConSelect;
	BOOL	m_bConsolidate;
	BOOL	m_bAlphaSort;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CProcessedOrderGenerateStockDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CProcessedOrderGenerateStockDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectDatabase(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	afx_msg void OnButtonDisplay();
	afx_msg void OnSelectSupplierSet();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void GenerateFromStockLevels( CProcessedOrderPreviewCSVArray& OrderArray );
	void BuildSupplierArray();
	void HandleOKInternal();
	
private:
	CCategorySelector m_CategorySelector;
	CLocationSelector m_LocationSelector;
	CWordArray m_wSuppSelectArray;

private:
	CArray<int,int>& m_arrayEntityIdx;
	CArray<CLocationSelectorEntity,CLocationSelectorEntity> m_arrayLocation;

private:
	CProcessedOrderGenerateDlgHelpers m_Helpers;
};

/**********************************************************************/
#endif
/**********************************************************************/
