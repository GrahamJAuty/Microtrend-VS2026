#pragma once
/**********************************************************************/
#include "resource.h"
/**********************************************************************/
#include "..\SmartPay4ManagerBgnd\WondeData.h"
/**********************************************************************/

class CSetupWondeDlg : public CSSAutoShutdownDialog
{
public:
	CSetupWondeDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_SETUP_WONDE };
	CButton m_buttonView;
	CButton m_checkSaveEmail2;
	CButton m_checkTestMode;
	CEdit m_editCardNoPupil;
	CEdit m_editCardNoStaff;
	CEdit m_editImportFile;
	CEdit m_editStaffGroupNo;
	BOOL m_bTestMode;
	CString m_strImportFile;
	BOOL m_bDeleteImportFile;
	int m_nImportMethod;

	CSSComboBox m_comboMISID;
	int m_nMISIDIndex;
	CSSComboBox m_comboSIMS;
	int m_nAdnoTextIndex;
	CSSComboBox m_comboUPN;
	int m_nUPNTextIndex;
	CSSComboBox m_comboWebPayment1;
	int m_nWebPaymentIndex1;
	CSSComboBox m_comboYear;
	int m_nYearTextIndex;
	CSSComboBox m_comboReg;
	int m_nRegTextIndex;
	CSSComboBox m_comboHouse;
	int m_nHouseTextIndex;
	BOOL m_bSaveEmail;
	CSSComboBox m_comboEmail;
	int m_nEmailTextIndex;
	int m_nFSMGroupNo;
	int m_nFSMRevertGroupNo;

	int m_nStaffGroupNo;
	CSSComboBox m_comboWebPayment2;
	int m_nWebPaymentIndex2;
	BOOL m_bSaveEmail2;
	CSSComboBox m_comboEmail2;
	int m_nEmail2TextIndex;

	CString m_strPhotoFolder;
	BOOL m_bDeletePhotoFile;

	CButton m_buttonAltKey1;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonBrowse2();
	afx_msg void OnButtonView();
	afx_msg void OnButtonView2();
	afx_msg void OnKillFocusImportFile();
	afx_msg void OnToggleSaveEmail();
	afx_msg void OnToggleSaveEmail2();
	afx_msg void OnButtonAltKey1();
	afx_msg void OnSelectNewIntakePupil();
	afx_msg void OnSelectNewIntakeStaff();
	afx_msg void OnToggleDisableStaff();
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
	CWondeData m_data;

public:
	BOOL m_bDisableStaffImport;
	CSSComboBox m_comboLocateOn;
	int m_nLocateOnIndex;
	CString m_strPupilStartFrom;
	CString m_strStaffStartFrom;
	int m_nNewIntakePupil;
	int m_nNewIntakeStaff;
	CSSComboBox m_comboNewIntakePupil;
	CSSComboBox m_comboNewIntakeStaff;
};

/**********************************************************************/
