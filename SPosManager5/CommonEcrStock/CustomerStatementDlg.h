#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CustomerStatementErrorDlg.h"
#include "DateSelectorRange.h"
#include "..\CommonEcrStock\LocationSelector.h"
/**********************************************************************/

class CCustomerStatementDlg : public CSSDialog
{
	DECLARE_DYNAMIC(CCustomerStatementDlg)

public:
	CCustomerStatementDlg(CWnd* pParent = NULL); 
	virtual ~CCustomerStatementDlg();

	enum { IDD = IDD_CUSTOMER_STATEMENT };
	CButton m_radioMonth;
	CButton m_radioOther;
	CSSComboBox m_comboMonth;
	CSSComboBox m_comboOther;
	CDateTimeCtrl m_DatePickerTo;
	CDateTimeCtrl m_DatePickerFrom;
	CSSComboBox m_comboRound;
	CSSComboBoxEx m_comboLocation;
	CSSListCtrl m_listCustomers;
	CEdit m_editLine1;
	CEdit m_editLine2;
	CButton m_checkBalance;
	CEdit m_editFilter;

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnRadioMonth();
	afx_msg void OnRadioOther();
	afx_msg void OnSelectMonth();
	afx_msg void OnSelectDate();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectLocation();
	afx_msg void OnButtonSelectNone();
	afx_msg void OnButtonSelectAll();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonView();
	afx_msg void OnButtonPrint();
	afx_msg void OnButtonEmail();
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocusFilter();
	DECLARE_MESSAGE_MAP()

private:
	void GetReportDates( COleDateTime& dateFrom, COleDateTime& dateTo );
	void UpdateCustomerList();
	void CreateStatement( int nNameTableIdx, int nMode, int& nResult, CString& strError );
	void ShowErrors( int nType, int nAttempted );
	void SaveHeaderLines();
	void EmailMAPI();
	void EmailSMTP();

private:
	CDateSelectorRange m_DateSelectorRange;
	CLocationSelector m_LocationSelector;
	CString m_strFilter;

private:
	CArray<CCustomerStatementErrorInfo,CCustomerStatementErrorInfo> m_arrayErrors;
	int m_nSortType;
	int m_nThisYear;
	int m_nThisMonth;
};

/**********************************************************************/
#endif
/**********************************************************************/
