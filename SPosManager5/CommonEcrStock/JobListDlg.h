#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "EposTaskDatabase.h"
//include "MySSListCtrl.h"
/**********************************************************************/

class CJobListDlg : public CSSDialog
{
public:
	CJobListDlg ( CEposTaskDatabase& TaskDatabase, bool bIsAdhocTask, CWnd* pParent = NULL );
	
	//{{AFX_DATA(CJobListDlg)
	enum { IDD = IDD_JOB_LIST };
	CButton	m_buttonBatchSales;
	CButton	m_checkTime;
	CDateTimeCtrl m_TimePicker;
	CEdit m_editName;
	CButton m_checkSelectWhenRun;
	CButton	m_buttonSelectLoc;
	CButton	m_buttonDownload;
	CButton	m_buttonEOD;
	CString	m_strName;
	BOOL m_bTime;
	int m_nImportType;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CJobListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CJobListDlg)
	virtual void OnOK();
	afx_msg void OnButtonJobSelectLoc();
	afx_msg void OnButtonJobDownload();
	afx_msg void OnButtonDeleteJob();
	virtual BOOL OnInitDialog();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckTime();
	afx_msg void OnButtonJobUpload();
	afx_msg void OnButtonJobEOD();
	afx_msg void OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void AddList();
	
private:
	bool m_bIsAdhocTask;
	CEposTaskDatabase& m_TaskDatabase;
	CSSListCtrl m_listJobs;
	int m_nJobType;

private:
	bool m_bCanDoEODProduct;
	bool m_bCanDoEODCustomer;
};

/**********************************************************************/
#endif
/**********************************************************************/
