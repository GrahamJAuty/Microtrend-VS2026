#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorRange.h"
#include "..\CommonEcrStock\LocationSelector.h"
#include "..\CommonEcrStock\PresetReportBase.h"
#include "StockAuditExceptions.h"
/**********************************************************************/

class CReportAuditDlg : public CSSDialog
{
public:
	CReportAuditDlg( int nPresetNo, bool bAutoRunPreset, CWnd* pParent = NULL);   

	//{{AFX_DATA(CReportAuditDlg)

	CSSListCtrl	m_listReports;
	
	CStatic			m_staticPreset;
	CButton			m_buttonPreset;
	CSSComboBox		m_comboPreset;

	CSSComboBoxEx	m_comboConLevel;

	CSSComboBox		m_comboDates;
	CDateTimeCtrl	m_DatePickerTo;
	CDateTimeCtrl	m_DatePickerFrom;
	
	CEdit			m_editHeader;
	CEdit			m_editLine;
	CButton			m_checkPageBreak;

	CButton			m_checkStock;
	CEdit			m_editStockCodeFrom;
	CButton			m_buttonBrowseFrom;
	CEdit			m_editStockCodeTo;
	CButton			m_buttonBrowseTo;
	
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CReportAuditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CReportAuditDlg)
	
	virtual BOOL OnInitDialog();
	
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnSelectReport(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnButtonPreset();
	afx_msg void OnSelectPreset();
	
	afx_msg void OnSelectConLevel();
	
	afx_msg void OnSetDates();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnToggleStock();
	afx_msg void OnKillFocusStockCode();
	afx_msg void OnButtonBrowseFrom();
	afx_msg void OnButtonBrowseTo();
	
	afx_msg void OnButtonPrint();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetDefaultSelection ( int nDefSelectIdx, int nDefSelectType );

public:
	void CreatePreset( CPresetReportInfo& base );

private:
	void SelectReport();
	void SetStartDate();
	void GetConDetails();

	void FillPresetCombo( int nSelection );

	void PrintAudit( CStockAuditBaseReport* pReport );
	void PrintOrderDeliveryAudit();
	void PrintDeliveryAudit( bool bDetail );
	void PrintDeliverySummary();
	void PrintReturnsAudit();
	void PrintAdjustmentStockAudit();
	void PrintAdjustmentPluAudit();
	void PrintOpeningAudit();
	void PrintSalesAudit();
	void PrintStocktakingAudit();
	void PrintTransferAudit();
	void PrintItemActivityAudit();
	void PrintExceptionsAudit();
	void PrintSalesExceptionsAudit();

private:
	CDateSelectorRange m_DateSelectorRange;
	CLocationSelector m_LocationSelector;
	int m_nReportType;
	CWordArray m_wSuppSelectArray;
	int m_nDefSelectIdx;
	int m_nDefSelectType;
	int m_nSpIdx;

private:
	bool m_bSelectHeader;
	bool m_bSelectLine;
	bool m_bSelectPageBreak;

private:
	CStockAuditExceptionTypes m_ExceptionTypes;

private:
	int m_nPresetNo;
	int m_nPresetError;
	bool m_bAutoRunPreset;
};

/**********************************************************************/
#endif
/**********************************************************************/
