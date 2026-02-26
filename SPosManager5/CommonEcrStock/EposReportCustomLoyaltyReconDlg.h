#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "EposReportSelect.h"
/**********************************************************************/

class CEposReportCustomLoyaltyReconDlg : public CSSDialog
{
public:
	CEposReportCustomLoyaltyReconDlg( CEposReportSelect& EposReportSelect, CEposReportSelectInfo& infoReport, bool bAdhoc, CWnd* pParent = NULL); 
	virtual ~CEposReportCustomLoyaltyReconDlg();

	//{{AFX_DATA(CEposReportCustomLoyaltyReconDlg)
	enum { IDD = IDD_EPOS_REPORT_CUSTOM_LOYALTYRECON };
	CEdit m_editName;
	CStatic m_staticFolder;
	CStatic m_staticSelect;
	CButton m_buttonBrowse;
	CButton m_buttonTest;
	CStatic m_staticOptions;
	CEdit m_editREQFolder;
	CButton m_checkAdhoc;
	CButton m_buttonSave;
	CButton m_checkUnrecon;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CEposReportCustomLoyaltyReconDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

private:
	bool GetUpdatedSettings( CEposReportSelectInfo& infoReport );

protected:
	//{{AFX_MSG(CEposReportCustomLoyaltyReconDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonTest();
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

