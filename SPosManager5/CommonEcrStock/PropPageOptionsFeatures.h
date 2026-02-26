#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ColourButton.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageOptionsFeatures : public CSSPropertyPage
{
public:
	CPropPageOptionsFeatures();
	~CPropPageOptionsFeatures();

public:
	void SetMainWnd( CWnd* pWnd ){ m_pMainWnd = pWnd; }

public:
	//{{AFX_DATA(CPropPageOptionsFeatures)
	enum { IDD = IDD_PROPPAGE_OPTIONS_FEATURES };

	CTabCtrl m_TabActions;
	
	CStatic m_staticGeneral1;
	CStatic m_staticGeneral2;
	CStatic m_staticGeneral3;
	CStatic m_staticGeneral4;
	CStatic m_staticGeneral5;
	CSSTabbedComboBox m_comboEditing;
	CSSComboBox m_comboContext;
	CSSTabbedComboBox m_comboPluStock;
	CSSTabbedComboBox m_comboDelivery;
	CColourButtonBase m_buttonColourBackground;
	CButton m_checkAllergen;
	CButton m_checkOPNameFirst;

	CButton m_checkMessage;
	CStatic m_staticMessage1;
	CColourButtonBase m_buttonColourMessageBack;
	CStatic m_staticMessage2;
	CColourButtonBase m_buttonColourMessageText;
	CStatic m_staticMessage3;
	CSSComboBox m_comboMessageFont;
	CStatic m_staticMessage4;
	CSSComboBox m_comboMessageSpace;
	CButton m_buttonMessagePreview;

	CButton m_checkStartupMsg;
	CStatic m_staticStartupMsg1;
	CColourButtonBase m_buttonColourStartupMsgBack;
	CStatic m_staticStartupMsg2;
	CColourButtonBase m_buttonColourStartupMsgText;
	CStatic m_staticStartupMsg3;
	CSSComboBox m_comboStartupMsgFont;
	CStatic m_staticStartupMsg4;
	CSSComboBox m_comboStartupMsgSpace;
	CButton m_buttonStartupMsgEdit;
	CButton m_buttonStartupMsgPreview;
	
	CButton	m_checkMixMatch2;
	CButton	m_checkPromotions;
	CButton	m_checkTimePlans;
	CButton	m_checkMixMatchDownload;
	CButton	m_checkTimePlanTemplates;
	CButton m_checkWalletStampRules;
	CButton m_checkGlobalWalletStampRules;
	
	CStatic m_staticKeyboards1;
	CStatic m_staticKeyboards2;
	CSSComboBox m_comboKeyboards;
	CButton	m_checkKeyboardsText;
	CSSComboBox m_comboPluText;
	CButton m_checkKeyboardsShare;
	CButton m_buttonKeyboardsShare;
	
	CButton	m_checkServers;
	CButton	m_checkServersTerminal;
	CButton	m_checkServersPreset;
	CButton m_checkServersLogon;
	CSSComboBox m_comboServers;
	CButton m_buttonMiFare;
	CButton m_checkCustomers;
	CButton m_checkCustomers2;
	CButton m_checkCustomers3;
	CButton m_checkCustomers4;
	CStatic m_staticCustomers1;
	CButton m_checkCustomers5;
	CStatic m_staticCustomers2;	
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageOptionsFeatures)
public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	CEdit* GetEditCustomers1();
	CEdit* GetEditCustomers2();

protected:
	//{{AFX_MSG(CPropPageOptionsFeatures)
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnToggleCustomerAutoDelete1();
	afx_msg void OnToggleCustomerAutoDelete2();
	afx_msg void SetActiveTabControls();
	afx_msg void OnSetupKeyboardsShare();
	afx_msg void OnButtonBackground();
	afx_msg void OnButtonMessageBack();
	afx_msg void OnButtonMessageText();
	afx_msg void OnButtonMessagePreview();
	afx_msg void OnButtonStartupMsgBack();
	afx_msg void OnButtonStartupMsgText();
	afx_msg void OnButtonStartupMsgEdit();
	afx_msg void OnButtonStartupMsgPreview();
	afx_msg void OnToggleWalletStampRule();
	afx_msg void OnButtonMiFare();
	//}}AFX_MSG
	afx_msg long OnColourButtonMessage( WPARAM wIndex, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
	
private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();
	void SetTabControls( bool bActiveTabOnly );

private:
	CWnd* m_pMainWnd;
	int m_nActiveTab;
	int m_nEditingColour;
};

/**********************************************************************/
#endif
/**********************************************************************/
