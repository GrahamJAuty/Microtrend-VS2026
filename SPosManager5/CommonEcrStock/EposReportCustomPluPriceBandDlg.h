#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "EposReportSelect.h"
/**********************************************************************/

class CEposReportCustomPluPriceBandDlg : public CSSDialog
{
public:
	CEposReportCustomPluPriceBandDlg( CEposReportSelect& EposReportSelect, CEposReportSelectInfo& infoReport, bool bAdhoc, CWnd* pParent = NULL); 
	virtual ~CEposReportCustomPluPriceBandDlg();

	//{{AFX_DATA(CEposReportCustomPluPriceBandDlg)
	enum { IDD = IDD_EPOS_REPORT_CUSTOM_PLUPRICEBAND };
	CEdit m_editName;
	CSSComboBox m_comboType;
	CButton m_checkAdhoc;
	CButton m_buttonSave;
	CButton m_checkPriceBand[10];
	CButton m_checkColumnNames;
	CButton m_checkServerSort;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CEposReportCustomPluPriceBandDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

private:
	bool GetUpdatedSettings( CEposReportSelectInfo& infoReport );

protected:
	//{{AFX_MSG(CEposReportCustomPluPriceBandDlg)
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

