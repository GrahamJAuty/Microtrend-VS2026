//$$******************************************************************
#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CSystemPropertyPage1 : public CSSPropertyPage
{
public:
	CSystemPropertyPage1();
	virtual ~CSystemPropertyPage1();

	bool ChangedServerSettings() { return m_bChangedServerSettings; }
	//FIDDLE int GetChangedPaxtonMode() { return m_nChangedPaxtonMode; }
	int GetChangedPaxtonMode(){ return 0; }

// Dialog Data
	enum { IDD = IDD_PROPPAGE_SYSTEM1 };
	
	CSSComboBox m_comboInterface;
	CButton m_buttonReader;
	CStatic m_staticPrefixText;
	CEdit m_editCardNoPrefix;
	CButton m_checkCheckDigit;
	CButton m_checkShowQRInfo;
	CStatic m_staticPaxton;
	CButton m_checkQREmbed;
	CEdit m_editQREmbed;
	/*****/
	CButton m_checkEnablePurse2;
	CButton m_checkPendingRefresh;
	/*****/
	CSSComboBox m_comboSignOn;
	CButton m_buttonPasswords;
	/*****/
	CButton m_checkSrvImportUnknown;
	CButton m_checkExternalAccount;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	/*****/
	afx_msg void OnButtonAltKey1();
	afx_msg void OnButtonInterface();
	afx_msg void OnSelectInterface();
	afx_msg void OnButtonReader();
	afx_msg void OnToggleQREmbed();
	/*****/
	afx_msg void OnSelectSignOn();
	afx_msg void OnButtonPasswords();
	
	DECLARE_MESSAGE_MAP()

private:
	void SetDisplay();
	int GetInterfaceTypeFromCombo();
	void FillSignOnCombo( bool MiFare, int nSel );

private:
	bool m_bChangedServerSettings;
	int m_nChangedPaxtonMode;
	int m_nCurrentInterface;
};

//$$******************************************************************
