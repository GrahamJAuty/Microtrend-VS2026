#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "EposReportSelect.h"
/**********************************************************************/
#define CONSOLIDATION_OPTION_SALES_ALL 1
#define CONSOLIDATION_OPTION_SALES_NOSYS 2
#define CONSOLIDATION_OPTION_PROMO_NOSYS 3
#define CONSOLIDATION_OPTION_PAYMENT_ALL 4
#define CONSOLIDATION_OPTION_ACTIVITY_ALL 5
/**********************************************************************/

class CReportEposConsolDlg : public CSSDialog
{
public:
	CReportEposConsolDlg( CEposReportSelectInfo& infoReport, CWnd* pParent = NULL);   

	//{{AFX_DATA(CReportEposConsolDlg)
	enum { IDD = IDD_REPORT_EPOS_CONSOL };
	//}}AFX_DATA
	CButton m_checkConsolType1;
	CButton m_checkConsolType2;
	CButton m_checkConsolType3;
	CButton m_checkConsolType4;

	//{{AFX_VIRTUAL(CReportEposConsolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnButtonSave();

public:
	static bool CanDoAdhocReport( CEposReportSelectInfo& infoReport );

private:
	static int GetConsolidationOptionType( CEposReportSelectInfo& infoReport );
	const char* GetCSVLine();

protected:
	//{{AFX_MSG(CReportEposConsolDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nConsolidationOptionType;

private:
	CEposReportSelectInfo& m_infoReport;
	CString m_strCSVLine;

	friend class CPosTrayEposConsolDlg;
};

/**********************************************************************/
#endif
/**********************************************************************/
