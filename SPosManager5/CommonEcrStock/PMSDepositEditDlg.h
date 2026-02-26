#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
 
class CPMSDepositEditDlg : public CSSDialog
{
public:
	CPMSDepositEditDlg( CPMSDepositInfo& infoDeposit, bool bAdd, CWnd* pParent = NULL);   

	//{{AFX_DATA(CPMSDepositEditDlg)
	enum { IDD = IDD_PMS_DEPOSIT_EDIT };
	CButton	m_buttonCancel;
	CButton	m_buttonOK;
	double	m_dPayment;
	//}}AFX_DATA
	CSSComboBox m_comboPayment;
	CSSTabbedComboBox m_comboDate;
	CDateTimeCtrl m_datePicker;
	CDateTimeCtrl m_TimePicker1;

	//{{AFX_VIRTUAL(CPMSDepositEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	CString m_strInputLine;
	CString m_strOutputLine;

protected:
	//{{AFX_MSG(CPMSDepositEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg void OnChangeDateCombo();
	afx_msg void OnOpenDateCombo();
	afx_msg void OnChangeDatePicker(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

private:
	void DataInit();
	void SetEditFocus();
	void HandleAdd();

private:
	void UpdateDatePickerDate();
		
private:
	CEdit* GetEditPayment();
	
private:
	int m_nSearchDay;
	int m_nSearchDayBase;
	bool m_bModified;
	bool m_bAdd;
		
private:
	CPMSDepositInfo& m_infoDeposit;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
