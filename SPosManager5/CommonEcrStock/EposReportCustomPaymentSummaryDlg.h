#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "EposReportSelect.h"
/**********************************************************************/

class CEposReportCustomPaymentSummaryDlg : public CSSDialog
{
public:
	CEposReportCustomPaymentSummaryDlg( CEposReportSelect& EposReportSelect, CEposReportSelectInfo& infoReport, bool bAdhoc, CWnd* pParent = NULL); 
	virtual ~CEposReportCustomPaymentSummaryDlg();
	//{{AFX_DATA(CEposReportCustomPaymentSummaryDlg)
	enum { IDD = IDD_EPOS_REPORT_CUSTOM_PAYMENTSUMMARY };
	CEdit m_editName;
	CButton m_checkDate;
	CButton m_checkAdhoc;
	CButton m_buttonSave;
	CButton m_radioSubType1;
	CButton m_radioSubType2;
	CButton m_radioSubType3;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CEposReportCustomPaymentSummaryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

private:
	bool GetUpdatedSettings( CEposReportSelectInfo& infoReport );

protected:
	//{{AFX_MSG(CEposReportCustomPaymentSummaryDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonSave();
	afx_msg void OnRadioSubType1();
	afx_msg void OnRadioSubType2();
	afx_msg void OnRadioSubType3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetSubType(int n);
		
private:
	CEposReportSelect& m_EposReportSelect;
	CEposReportSelectInfo& m_infoReport;
	bool m_bAdhoc;
};

/**********************************************************************/
#endif
/**********************************************************************/

