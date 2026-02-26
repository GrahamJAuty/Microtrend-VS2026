#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "LocationMasterMap.h"
//include "MySSDialog.h"
/**********************************************************************/

class CLocationMasterDlg : public CSSDialog
{
public:
	CLocationMasterDlg( int nLocIdx, int nTNo, bool bShowLocName, CWnd* pParent = NULL); 

	//{{AFX_DATA(CLocationMasterDlg)
	enum { IDD = IDD_LOCATION_MASTER };
	CTabCtrl m_TabSettings;
	/*****/
	CStatic m_staticCashRSP1;
	CEdit m_editReportName;
	CButton m_checkEnableCashRSP;
	CEdit m_editPathCashRSP;
	CButton m_buttonBrowseCashRSP;
	CString m_strReportName;
	CString	m_strPathCashRSP;
	/*****/
	CStatic m_staticMaster1;
	CStatic m_staticMaster2;
	CStatic m_staticMaster3;
	CButton	m_checkEnableMaster;
	CEdit	m_editMasterName;
	CEdit	m_editPathCommsIn;
	CEdit	m_editPathCommsInNow;
	CButton m_buttonBrowseCommsIn;
	CButton m_buttonBrowseCommsInNow;
	CString	m_strMasterName;
	CString	m_strPathCommsIn;
	CString	m_strPathCommsInNow;
	/*****/
	CButton m_buttonOK;
	CButton m_buttonCancel;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CLocationMasterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CLocationMasterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
	/*****/
	afx_msg void OnToggleEnableCashRSP();
	afx_msg void OnButtonBrowseCashRSP();
	/*****/
	afx_msg void OnToggleEnableMaster();
	afx_msg void OnButtonBrowseCommsIn();
	afx_msg void OnButtonBrowseCommsInNow();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SelectTab( int nPage );

private:
	int m_nLocIdx;
	int m_nTNo;
	int m_nTabPage;
	CLocationMasterInfo m_infoMaster;
	bool m_bInitialEnableMaster;
	bool m_bInitialEnableCashRSP;
	bool m_bShowLocName;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/

