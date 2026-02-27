#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\BromComData.h"
//*******************************************************************

class CSetupBromComDlg : public CSSAutoShutdownDialog
{
public:
	CSetupBromComDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupBromComDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_BROMCOM };
	CButton m_checkKeepXmlFiles;
	CButton m_checkKeepXmlLogin;
	CEdit m_editSchoolID;
	CEdit m_editUserName;
	CEdit m_editPassword;
	/*****/
	CButton m_buttonAltKey1;
	CButton m_buttonDelete;
	CDateTimeCtrl m_DatePickerFrom;
	CDateTimeCtrl m_TimePickerFrom;
	/*****/
	CButton m_checkPurse1Only;
	CEdit m_editRefresh;


private:
	CEdit* GetEditAdjustStart() { return GetEdit(IDC_EDIT_ADJUSTSTART); }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonAltKey1();
	DECLARE_MESSAGE_MAP()

private:
	virtual void CSSAutoShutdownPreInitDialog();
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	CBromComData m_data;
};

//*******************************************************************
