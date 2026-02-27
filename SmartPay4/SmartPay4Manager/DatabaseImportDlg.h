#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\AutoImportData.h"
//*******************************************************************

class CDatabaseImportDlg : public CSSAutoShutdownDialog
{
public:
	CDatabaseImportDlg( CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	enum { IDD = IDD_DATABASEIMPORT };
	CSSComboBox m_comboFileFormat;
	CButton m_buttonView;
	int m_nCsvFileFormat;
	CString m_strImportFilename;
	int m_nImportMethod;
	BOOL m_bImportPhotoID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnButtonView();
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonDefault();
	afx_msg void OnKillFocusImportFilename();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual void CSSAutoShutdownPreInitDialog();
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void SetDisplay();

private:
	CAutoImportData m_data;
};

//*******************************************************************
