#pragma once
//********************************************************************
#include "resource.h"
//********************************************************************
#include "..\SmartPay4ManagerBgnd\SimsData.h"
//********************************************************************

class CSetupSimsDlg : public CSSAutoShutdownDialog
{
public:
	CSetupSimsDlg( CWnd* pParent = NULL);   // standard constructor
	
	enum { IDD = IDD_SETUP_SIMS };
	CButton m_buttonView;
	CButton m_buttonView2;
	CButton m_checkAddUnknowns;
	CSSComboBox m_comboModify;
	CEdit m_editCardNo;
	CEdit m_editSIMSImport;
	int m_nLocateBy;
	CSSComboBox m_comboSIMS;
	int m_nAdnoTextIndex;
	CSSComboBox m_comboUPN;
	int m_nUPNTextIndex;
	CSSComboBox m_comboPID;
	int m_nPIDTextIndex;
	CSSComboBox m_comboNewIntake;
	int m_nNewIntakeAccount;
	CSSComboBox m_comboYear;
	int m_nYearTextIndex;
	CSSComboBox m_comboReg;
	int m_nRegTextIndex;
	BOOL m_bSaveEmail;
	CSSComboBox m_comboEmail;
	int m_nEmailTextIndex;
	CSSComboBox m_comboDob;
	int m_nDobTextIndex;
	BOOL m_bAddUnknowns;
	int m_nFSMGroupNo;
	int m_nFSMRevertGroupNo;
	CString m_strSimsImportFile;
	BOOL m_bDeleteImportFile;
	CString m_strSimsPhotoFolder;
	BOOL m_bDeletePhotoFile;
	CSSComboBox m_comboLocatePhotoBy;
	int m_nLocatePhotoBy;
	int m_nImportMethod;
	CString m_strStartFromUserID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonBrowse2();
	afx_msg void OnButtonDefault();
	afx_msg void OnButtonView();
	afx_msg void OnButtonView2();
	afx_msg void OnKillFocusSimsImport();
	afx_msg void OnToggleAddUnknowns();
	afx_msg void OnButtonView3();
	afx_msg void OnToggleSaveEmail();
	afx_msg void OnSelectNewIntake();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual void CSSAutoShutdownPreInitDialog();
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void SetDisplay();
	bool ValidateInput();
	void DisplayFile ( int nType );

private:
	CSimsData m_sims;
};

//********************************************************************
