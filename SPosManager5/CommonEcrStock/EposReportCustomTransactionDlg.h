#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "EposReportSelect.h"
/**********************************************************************/

class CEposReportCustomTransactionDlg : public CSSDialog
{
public:
	CEposReportCustomTransactionDlg( CEposReportSelect& EposReportSelect, CEposReportSelectInfo& infoReport, bool bAdhoc, CWnd* pParent = NULL); 
	virtual ~CEposReportCustomTransactionDlg();

	//{{AFX_DATA(CEposReportCustomTransactionDlg)
	enum { IDD = IDD_EPOS_REPORT_CUSTOM_TRANSACTION };
	CEdit m_editName;
	CEdit m_editFilterText;
	CButton m_checkItemServer;
	CButton m_checkItemSaleDate;
	CButton m_checkModText;
	CButton m_checkDiscrepancy;
	CButton m_checkVersion;
	CButton m_checkAdhoc;
	CButton m_buttonSave;
	int m_nSortType;
	int m_nTimeType;
	int m_nTextType;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CEposReportCustomTransactionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

private:
	void UpdateEditBox( CEdit* pEdit, int nValue, int nWidth );
	int GetEditBoxInt( CEdit* pEdit );

private:
	CEdit* GetEditSeqNoFrom();
	CEdit* GetEditSeqNoTo();
	CEdit* GetEditTranServerFrom();
	CEdit* GetEditTranServerTo();
	CEdit* GetEditItemServerFrom();
	CEdit* GetEditItemServerTo();
	bool GetUpdatedSettings( CEposReportSelectInfo& infoReport );

protected:
	//{{AFX_MSG(CEposReportCustomTransactionDlg)
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

