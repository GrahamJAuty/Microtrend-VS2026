#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "EposReportSelect.h"
/**********************************************************************/

class CEposReportCustomDepositDlg : public CDialog
{
public:
	CEposReportCustomDepositDlg( CEposReportSelect& EposReportSelect, CEposReportSelectInfo& infoReport, bool bAdhoc, CWnd* pParent = NULL); 
	virtual ~CEposReportCustomDepositDlg();

	//{{AFX_DATA(CEposReportCustomDepositDlg)
	enum { IDD = IDD_EPOS_REPORT_CUSTOM_DEPOSIT };
	CEdit m_editName;
	int m_nReportType;
	CButton m_checkDate;
	CButton m_checkRefund;
	CButton m_checkRedeem;
	CButton m_checkPayment;
	CButton m_checkEndDate;
	CButton m_checkAdhoc;
	CButton m_buttonSave;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CEposReportCustomDepositDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

private:
	bool GetUpdatedSettings( CEposReportSelectInfo& infoReport );

protected:
	//{{AFX_MSG(CEposReportCustomDepositDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
private:
	CEposReportSelect& m_EposReportSelect;
	CEposReportSelectInfo& m_infoReport;
	bool m_bAdhoc;
};

/**********************************************************************/
#endif
/**********************************************************************/

