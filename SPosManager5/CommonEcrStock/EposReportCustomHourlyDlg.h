#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "EposReportSelect.h"
//include "MyComboBox.h"
/**********************************************************************/

class CEposReportCustomHourlyDlg : public CDialog
{
public:
	CEposReportCustomHourlyDlg( int nFamily, CEposReportSelect& EposReportSelect, CEposReportSelectInfo& infoReport, bool bAdhoc, CWnd* pParent = NULL); 
	virtual ~CEposReportCustomHourlyDlg();

	//{{AFX_DATA(CEposReportCustomHourlyDlg)
	enum { IDD = IDD_EPOS_REPORT_CUSTOM_HOURLY };
	CEdit m_editName;
	CSSComboBox m_comboTimeSlice;
	CButton m_checkAdhoc;
	CButton m_buttonSave;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CEposReportCustomHourlyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

private:
	bool GetUpdatedSettings( CEposReportSelectInfo& infoReport );

protected:
	//{{AFX_MSG(CEposReportCustomHourlyDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
private:
	int m_nFamily;
	CEposReportSelect& m_EposReportSelect;
	CEposReportSelectInfo& m_infoReport;
	bool m_bAdhoc;
};

/**********************************************************************/
#endif
/**********************************************************************/

