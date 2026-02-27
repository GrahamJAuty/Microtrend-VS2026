#pragma once
//*******************************************************************
#include "CardLink.h"
#include "SystemData.h"
//*******************************************************************

class CSetupDlg : public CSSDialog
{
	DECLARE_DYNAMIC(CSetupDlg)

public:
	CSetupDlg(CSystemData* pSystemData, CWnd* pParent); 
	virtual ~CSetupDlg();

	enum { IDD = IDD_SETUPDLG };
	int m_nTerminalNo = 0;
	int m_nCCNo = 0;
	CButton m_checkEnablePassword;
	CButton m_checkShowPoints;
	CButton m_checkShowGroup;
	CButton m_checkShowPhotoID;
	CButton m_checkEnableKeypadEntry;
	CButton m_checkHideAccountID;
	CButton m_checkEnableEnrolment;
	CButton m_checkShowAvailableSpend;
	CButton m_checkDisableAlerts;
	CButton m_checkShowDateTime;
	CButton m_checkNoSmallCoinWarn;
	CButton m_checkTopupPurse1;
	CButton m_checkTopupPurse3;
	CButton m_checkShowUserIDCancel;
	CString m_strSetupPassword = "";
	CString m_strEnrolmentPrompt = "";
	CString m_strLoyReqFolder = "";
	CString m_strLoyLogFolder = "";
	CString m_strNotifyPath = "";
	CString m_strImportFile = "";
	double m_dCID = 0.0;
	double m_dGT = 0.0;
	int m_nRefreshInterval = 0;
	int m_nEnquiryTimeout = 0;
	long m_lPaylinkSerialNo = 0;
	int m_nInterfaceType = 0;
	CString m_strCardNoText = "";
	CString m_strPurse1Text = "";
	CString m_strPurse3Text = "";

private:
	CEdit m_editImportFile;
	CEdit m_editPassword;
	CEdit m_editCCNo;
	CEdit m_editSerialNo;
	CEdit m_editPurse1Text;
	CEdit m_editPurse3Text;
	CEdit m_editCardNoText;
	CEdit m_editRefreshInterval;
	CEdit m_editEnrolPrompt;
	CEdit m_editNotifyPath;
	CEdit m_editLoyREQFolder;
	CEdit m_editLoyLOGFolder;
	CEdit m_editTerminalNo;
	CButton m_buttonTerminal;
	CButton m_buttonTestBiometric;
	CEdit* GetEditGT() { return GetEdit(IDC_EDIT_GT); }	
	CComboBox m_comboPINPad;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnTogglePasswordReqd();
	afx_msg void OnClickStaticCCNo();
	afx_msg void OnClickStaticSerialNo();
	afx_msg void OnButtonBrowse1();
	afx_msg void OnButtonBrowse2();
	afx_msg void OnButtonBrowse3();
	afx_msg void OnButtonService();
	afx_msg void OnButtonTestBiometric();
	afx_msg void OnClickStaticImportFile();
	afx_msg void OnClickStaticGT();
	afx_msg void OnButtonTerminal();
	afx_msg void OnSelectInterface();
	afx_msg void OnToggleEnrolment();
	afx_msg void OnButtonTest1();
	afx_msg void OnButtonTest2();
	afx_msg void OnButtonTest3();
	afx_msg void OnTogglePurse1Topup();
	afx_msg void OnTogglePurse3Topup();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

private:
	void SetDisplay();
	bool TestPath ( const char* szTitle, const char* szPath );

private:
	CSystemData* m_pSystemData;
	int m_nOldCCNo = 0;
	double m_dOldGT = 0.0;
};

//*******************************************************************
