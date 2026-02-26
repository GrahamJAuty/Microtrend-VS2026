#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "EposReportSelect.h"
/**********************************************************************/

class CEposReportCustomPaymentDetailDlg : public CSSDialog
{
public:
	CEposReportCustomPaymentDetailDlg( CEposReportSelect& EposReportSelect, CEposReportSelectInfo& infoReport, bool bAdhoc, CWnd* pParent = NULL); 
	virtual ~CEposReportCustomPaymentDetailDlg();

	//{{AFX_DATA(CEposReportCustomPaymentDetailDlg)
	enum { IDD = IDD_EPOS_REPORT_CUSTOM_PAYMENTDETAIL };
	CEdit m_editName;
	CButton m_checkTableNo;
	CButton m_checkTableName;
	CButton m_checkTableFilter;
	CButton m_checkPayNumFilter;
	CEdit m_editPayNum;
	CButton m_checkPayType[18];
	CButton m_checkSEQNo;
	CButton m_checkServer;
	CButton m_checkAdhoc;
	CButton m_buttonSave;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CEposReportCustomPaymentDetailDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

private:
	CEdit* GetEditTableFrom() { return GetEdit(IDC_EDIT_TABLEFROM); }
	CEdit* GetEditTableTo() { return GetEdit(IDC_EDIT_TABLETO); }

private:
	bool GetUpdatedSettings( CEposReportSelectInfo& infoReport );

protected:
	//{{AFX_MSG(CEposReportCustomPaymentDetailDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonSave();
	afx_msg void OnToggleTableFilter();
	afx_msg void OnTogglePayNumFilter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
private:
	DWORD m_checkPayTypeID[18];

private:
	CEposReportSelect& m_EposReportSelect;
	CEposReportSelectInfo& m_infoReport;
	bool m_bAdhoc;
};

/**********************************************************************/
#endif
/**********************************************************************/

