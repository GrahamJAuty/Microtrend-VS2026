#pragma once
/******************************************************************************/
#include "resource.h"
/******************************************************************************/
#include "AuditReportConfig.h"
/******************************************************************************/

class CAuditReportActionDlg : public CSSAutoShutdownDialog
{
public:
	CAuditReportActionDlg(CAuditReportConfig& reportconfig, CWnd* pParent = NULL);
	virtual ~CAuditReportActionDlg();

// Dialog Data
	enum { IDD = IDD_REPORT_AUDIT_ACTION };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButtonAll() { m_listAction.SelectAll(); }
	afx_msg void OnButtonNone() { m_listAction.SelectNone(); }
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void FillList();

private:
	CArray<int, int> m_arrayActions;
	CSSListTaggedSelectCtrl m_listAction;
	CAuditReportConfig& m_reportconfig;
	bool m_bAdjustedRowHeight;
};

/******************************************************************************/
