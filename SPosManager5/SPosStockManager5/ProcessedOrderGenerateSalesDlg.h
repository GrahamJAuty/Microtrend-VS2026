#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CategorySelector.h"
#include "DateSelectorRange.h"
#include "..\CommonEcrStock\LocationSelector.h"
//include "MySSDialog.h"
#include "ProcessedOrderGenerateDlgHelpers.h"
#include "ProcessedOrderPreviewDatabase.h"
/**********************************************************************/

class CProcessedOrderGenerateSalesDlg : public CSSDialog
{
public:
	CProcessedOrderGenerateSalesDlg( CArray<int,int>& arrayEntityIdx, CWnd* pParent = NULL);
	~CProcessedOrderGenerateSalesDlg(); 

	//{{AFX_DATA(CProcessedOrderGenerateSalesDlg)
	enum { IDD = IDD_ORDER_GENERATE_SALES };
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
	CStatic m_staticPrepare4;
	CSSComboBox	m_comboDates;
	CStatic m_staticPrepare5;
	CDateTimeCtrl m_DatePickerFrom;
	CStatic m_staticPrepare6;
	CDateTimeCtrl m_DatePickerTo;
	CButton	m_checkReorderItems;
	CButton m_buttonDisplay;
	CStatic m_staticOrder1;
	CSSComboBox	m_comboMethod;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	int		m_nConSelect;
	BOOL	m_bReorderItems;
	BOOL	m_bConsolidate;
	BOOL	m_bAlphaSort;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CProcessedOrderGenerateSalesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CProcessedOrderGenerateSalesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectDatabase(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	afx_msg void OnButtonDisplay();
	afx_msg void OnSelectSupplierSet();
	afx_msg void OnSelectDate();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool GenerateFromAuditedSales( CProcessedOrderPreviewCSVArray& OrderArray );
	void BuildSupplierArray();
	void HandleOKInternal();
	
	void RemoveTemporarySalesFiles( bool bCopy );

	void GenerateFromAuditedSalesInternal ( int& nMatched, int& nGenerated, CProcessedOrderPreviewCSVArray& OrderArray );

private:
	CCategorySelector m_CategorySelector;
	CLocationSelector m_LocationSelector;
	CWordArray m_wSuppSelectArray;
	CDateSelectorRange m_DateSelectorRange;
	
private:
	CString m_strDisplayDateFrom;
	CString m_strDisplayDateTo;
	CString m_strMonthFrom;
	CString m_strMonthTo;
	int m_nMDayFrom;
	int m_nMDayTo;

private:
	CArray<int,int>& m_arrayEntityIdx;
	CArray<CLocationSelectorEntity,CLocationSelectorEntity> m_arrayLocation;

private:
	CProcessedOrderGenerateDlgHelpers m_Helpers;
};

/**********************************************************************/
#endif
/**********************************************************************/

