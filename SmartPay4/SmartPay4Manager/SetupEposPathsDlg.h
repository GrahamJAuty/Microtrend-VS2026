#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
//$$******************************************************************

class CSetupEposPathsDlg : public CSSAutoShutdownDialog
{
public:
	CSetupEposPathsDlg(CWnd* pParent = NULL);  
	
	enum { IDD = IDD_SETUP_EPOSPATHS };

	CTabCtrl m_tabFolders;
	CEdit m_editLoyReqPath;
	CEdit m_editLoyLogPath;
	CButton m_checkExternal;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnButtonBrowseReq();
	afx_msg void OnButtonTestReq();
	afx_msg void OnButtonBrowseLog();
	afx_msg void OnButtonTestLog();
	afx_msg void OnButtonLabel();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	BOOL CSSAutoShutdownPostInitDialog();

private:
	void GetEditPathTexts();
	void SetEditPathTexts();

private:
	CString m_strReqPath[EPOSREQPATH_COUNT + 1];
	CString m_strLogPath[EPOSREQPATH_COUNT + 1];
	bool m_bExtPath[EPOSREQPATH_COUNT + 1];
	int m_nActiveTab;
};

//$$******************************************************************

