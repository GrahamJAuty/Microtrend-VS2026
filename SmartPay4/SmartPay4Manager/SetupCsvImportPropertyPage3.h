#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4ManagerBgnd\CSVImportData.h"
//**********************************************************************
#include "SmartPay4Manager.h"
//**********************************************************************

class CSetupCsvImportPropertyPage3 : public CSSAutoShutdownPropertyPage
{
public:
	CSetupCsvImportPropertyPage3();
	virtual ~CSetupCsvImportPropertyPage3();
	void SetPointer ( CCsvImportData* pData );

// Dialog Data
	enum { IDD = IDD_PROPPAGE_CSVIMPORT3 };
	CSSComboBox m_comboLocate;
	CButton m_buttonBrowse;
	CButton m_buttonView;
	CButton m_checkDeleteFile;
	CButton m_checkIgnoreCase;
	CEdit m_editCardNo;
	CEdit m_editImportFilename;
	CEdit m_editMenuText;
	CButton m_radioExisting;
	CButton m_radioImportAll;
	CButton m_radioNew;
	int m_nLocateOn;
	BOOL m_bEnableCustomImport;
	CString m_strMenuText;
	CString m_strImportFilename;
	BOOL m_bDeleteFile;
	int m_nImportMethod;
	BOOL m_bIgnoreCase;
	CString m_strStartFromUserID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnDoubleClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonView();
	afx_msg void OnKillFocusImportFilename();
	afx_msg void OnToggleEnable();

	DECLARE_MESSAGE_MAP()

private:
	void SetDisplay();

private:
	CCsvImportData* m_pData;
	CSSListCtrl m_listbox;

	CMemberDatabase m_dbCustomList;
};

//**********************************************************************
