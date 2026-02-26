#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorRange.h"
#include "SessionCSVArray.h"
/**********************************************************************/

class CPMSPrintPaymentsDlg : public CSSDialog
{
public:
	CPMSPrintPaymentsDlg(CWnd* pParent = NULL);   

	//{{AFX_DATA(CPMSPrintPaymentsDlg)
	enum { IDD = IDD_PMS_PRINT_PAYMENTS };
	CSSComboBox	m_comboRound;
	CSSComboBox	m_comboDates;
	CDateTimeCtrl	m_DatePickerTo;
	CDateTimeCtrl	m_DatePickerFrom;
	CSSComboBox m_comboSession;
	CButton m_checkTraining;
	BOOL m_bPending;
	BOOL m_bActive;
	BOOL m_bComplete;
	BOOL m_bCancelled;
	BOOL m_bDeposit;
	BOOL m_bPayment;
	BOOL m_bTraining;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPMSPrintPaymentsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPMSPrintPaymentsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectDate();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDefault();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonDisplay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void GetReportDates();
	bool GetAdhocReportSession();

private:
	void SetDefaults();
	const char* GetCSVLine();
	
private:
	COleDateTime m_OleDateFrom;
	COleDateTime m_OleDateTo;
	CSessionCSVRecord m_ReportSession;
	CSessionCSVRecord m_ReportSessionAdhoc;

private:
	CDateSelectorRange m_DateSelectorRange;
	
private:
	CString m_strFieldSelect;
};

/**********************************************************************/

class CCSVPMSPrintPaymentsKey : public CCSV
{
public:
	CCSVPMSPrintPaymentsKey ( const char* szCSVLine ) : CCSV(szCSVLine){}
	bool IncludePending(){ return GetBool(0); }
	bool IncludeActive(){ return GetBool(1); }
	bool IncludeComplete(){ return GetBool(2); }
	bool IncludeDeposit(){ return GetBool(3); }
	bool IncludePayment(){ return GetBool(4); }
	bool IncludeTraining(){ return GetBool(5); }
	bool IncludeCancelled(){ return GetBool(6); }
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
