#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CSetupEposPathsDlg : public CSSDialog
{
public:
	CSetupEposPathsDlg(CWnd* pParent = NULL);  
	
	enum { IDD = IDD_SETUP_EPOSPATHS };

	CTabCtrl m_tabFolders;
	CEdit m_editLoyReqPath;
	CEdit m_editLoyLogPath;
	CButton m_checkExternal;
	CSSTabbedComboBox m_comboDatabase;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectDatabase();
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnButtonBrowseReq();
	afx_msg void OnButtonTestReq();
	afx_msg void OnButtonBrowseLog();
	afx_msg void OnButtonTestLog();
	afx_msg void OnButtonLabel();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	void GetEditPathTexts();
	void SetEditPathTexts();

private:
	CString m_strReqPath[PLU_DBNO_COUNT + 1][EPOSREQPATH_COUNT + 1];
	CString m_strLogPath[PLU_DBNO_COUNT + 1][EPOSREQPATH_COUNT + 1];
	bool m_bExtPath[PLU_DBNO_COUNT + 1][EPOSREQPATH_COUNT + 1];
	int m_nActiveTab;
	int m_nCurrentDbNo;
};

//$$******************************************************************

