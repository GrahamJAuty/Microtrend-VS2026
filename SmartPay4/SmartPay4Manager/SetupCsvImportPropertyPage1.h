#pragma once
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\CSVImportData.h"
//$$******************************************************************
#include "SmartPay4Manager.h"
//$$******************************************************************

class CSetupCsvImportPropertyPage1 : public CSSAutoShutdownPropertyPage
{
public:
	CSetupCsvImportPropertyPage1();
	virtual ~CSetupCsvImportPropertyPage1();
	void SetPointer ( CCsvImportData* pData );

// Dialog Data
	enum { IDD = IDD_PROPPAGE_CSVIMPORT1 };
	CSSComboBox m_comboLocate;
	CButton m_buttonView;
	CButton m_buttonBrowse;
	CButton m_checkDeleteFile;
	CButton m_checkEnableImport;
	CButton m_radioImportAll;
	CButton m_radioImportNew;
	CButton m_radioImportExisting;
	CEdit m_editImportFilename;
	CEdit m_editStartFromUserID;
	CEdit m_editHeader;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnToggleEnable();
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonView();
	afx_msg void OnKillFocusFilename();
	
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditCache() { return GetEdit(IDC_EDIT_CACHE); }

private:
	void SetDisplay();

private:
	CCsvImportData* m_pData;
};

//$$******************************************************************
