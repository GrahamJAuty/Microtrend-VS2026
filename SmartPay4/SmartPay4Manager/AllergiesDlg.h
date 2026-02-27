#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\AllergyText.h"
//$$******************************************************************

class CAllergiesDlg : public CSSAutoShutdownDialog
{
public:
	CAllergiesDlg( const char* szUserID = "", int nAllergyCodes = 0, CWnd* pParent = NULL);  
	
public:
	enum { IDD = IDD_ALLERGIES };
	CButton m_checkAllergen[15];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	afx_msg void OnButtonClear();
	DECLARE_MESSAGE_MAP()

public:
	int GetAllergyCodes() { return m_AllergyText.GetAllergyCodes(); };

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void SetCheckboxText ( int nCode );

private:
	CString m_strUserID;
	CAllergyText m_AllergyText;
};

//$$******************************************************************