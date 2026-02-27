#pragma once
//**********************************************************************
#include "..\SmartPay4ManagerBgnd\CSVImportData.h"
//**********************************************************************
#include "SmartPay4Manager.h"
//**********************************************************************

class CSetupCsvImportPropertyPage2 : public CSSAutoShutdownPropertyPage
{
public:
	CSetupCsvImportPropertyPage2();
	void SetPointer ( CCsvImportData* pData );

// Dialog Data
	enum { IDD = IDD_PROPPAGE_CSVIMPORT2 };
	CSSComboBox m_comboLocate;
	CButton m_buttonBrowse;
	CButton m_buttonView;
	CButton m_checkAddUnknowns;
	CButton m_checkDeleteFile;
	CEdit m_editCardNo;
	CEdit m_editImportFilename;
	CString m_strImportFilename;
	BOOL m_bDeleteFile;
	BOOL m_bEnableImport;
	int m_nLocateOn;
	BOOL m_bAddUnknowns;
	CString m_strStartFromUserID;
	CEdit m_editHeader;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnToggleEnable();
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonView();
	afx_msg void OnKillFocusImportFilename();
	afx_msg void OnToggleAddUnknowns();

	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditCache() { return GetEdit(IDC_EDIT_CACHE); }

private:
	void SetDisplay();

private:
	CCsvImportData* m_pData;	
};

//**********************************************************************
