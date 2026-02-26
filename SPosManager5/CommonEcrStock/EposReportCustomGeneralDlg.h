#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "EposReportSelect.h"
//include "MyComboBox.h"
/**********************************************************************/

class CEposReportCustomGeneralDlg : public CDialog
{
public:
	CEposReportCustomGeneralDlg( CEposReportSelect& EposReportSelect, CEposReportSelectInfo& infoReport, bool bAdhoc, CWnd* pParent = NULL); 
	virtual ~CEposReportCustomGeneralDlg();

	//{{AFX_DATA(CEposReportCustomGeneralDlg)
	enum { IDD = IDD_EPOS_REPORT_CUSTOM_GENERAL };
	CEdit m_editName;
	CButton m_checkWeekday;
	CButton m_checkQuantity;
	CButton m_checkTaxBand;
	CButton m_checkTaxAmount;
	CButton m_checkPromoDetail;
	CButton m_checkPromoSummary;
	CButton m_checkPromoPositive;
	CButton m_checkReconcile;
	CButton m_checkSeparateChange;
	CButton m_checkCCDVariance;
	CButton m_checkVoid;
	CSSComboBox m_comboVoidType;
	CButton m_checkNoSale;
	CButton m_checkAdhoc;
	CButton m_checkConsolDept;
	CButton m_checkConsolGroupEpos;
	CButton m_checkConsolGroupReport;
	CButton m_checkConsolGroupConsol;
	CButton m_checkSalesPromo;
	CButton m_buttonSave;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CEposReportCustomGeneralDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

private:
	bool GetUpdatedSettings( CEposReportSelectInfo& infoReport );

protected:
	//{{AFX_MSG(CEposReportCustomGeneralDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCheckEposGroup();
	afx_msg void OnCheckReportGroup();
	afx_msg void OnCheckReconcile();
	afx_msg void OnCheckVoid();
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

