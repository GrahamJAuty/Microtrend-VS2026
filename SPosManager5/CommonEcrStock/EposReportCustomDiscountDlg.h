#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "EposReportSelect.h"
/**********************************************************************/

class CEposReportCustomDiscountDlg : public CDialog
{
public:
	CEposReportCustomDiscountDlg( CEposReportSelect& EposReportSelect, CEposReportSelectInfo& infoReport, bool bAdhoc, CWnd* pParent = NULL); 
	virtual ~CEposReportCustomDiscountDlg();

	//{{AFX_DATA(CEposReportCustomDiscountDlg)
	enum { IDD = IDD_EPOS_REPORT_CUSTOM_DISCOUNT };
	CEdit m_editName;
	CButton m_checkServer;
	CButton m_checkItem;
	CButton m_checkOffer[9];
	CButton m_checkAdhoc;
	CButton m_buttonSave;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CEposReportCustomDiscountDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

private:
	bool GetUpdatedSettings( CEposReportSelectInfo& infoReport );

protected:
	//{{AFX_MSG(CEposReportCustomDiscountDlg)
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

