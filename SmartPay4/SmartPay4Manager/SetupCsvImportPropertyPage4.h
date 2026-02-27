#pragma once
//**********************************************************************
#include "..\SmartPay4ManagerBgnd\CSVImportData.h"
//**********************************************************************
#include "SmartPay4Manager.h"
//**********************************************************************

class CSetupCsvImportPropertyPage4 : public CSSAutoShutdownPropertyPage
{
public:
	CSetupCsvImportPropertyPage4();
	void SetPointer ( CCsvImportData* pData );

// Dialog Data
	enum { IDD = IDD_PROPPAGE_CSVIMPORT4 };
	CButton m_buttonBrowse;
	CButton m_buttonView;
	CEdit m_editImportFilename;
	CButton m_radioExisting;
	CButton m_radioImportAll;
	CButton m_radioNew;
	CString m_strImportFilename;
	BOOL m_bEnableImport;
	int m_nImportMethod;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnToggleEnable();
	afx_msg void OnRadioImportall();
	afx_msg void OnRadioNew();
	afx_msg void OnRadioExisting();
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonView();

	DECLARE_MESSAGE_MAP()

private:
	void SetDisplay();

private:
	CCsvImportData* m_pData;
};

//**********************************************************************
