#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CSystemPropertyPage1 : public CSSAutoShutdownPropertyPage
{
public:
	CSystemPropertyPage1();
	bool RestartServerReqd() { return m_bRestartServerReqd; }

	enum { IDD = IDD_PROPPAGE_SYSTEM1 };

	CButton m_buttonReader;
	CSSComboBox m_comboInterface;
	CButton m_checkAutoDelPCtrlType;
	CStatic m_staticAutoDelPCtrlType;
	CSSComboBox m_comboAutoDelPCtrlType;
	CButton m_checkBioConsent;
	CButton m_checkPINNumbers;
	/*****/
	CButton m_checkGroupSets;
	CButton m_buttonGroupSets;
	CButton m_checkGroupShiftDateRange;
	CStatic m_staticGroupShift;
	CEdit m_editGroupShift;
	CButton m_checkGroupShiftOnDuty;
	/*****/
	CButton m_checkLogOff;
	CSSComboBox m_comboLogOff;
	CStatic m_staticLogOff1;
	CStatic m_staticLogOff2;
	/*****/
	CButton m_checkPasswords;
	CButton m_buttonPasswords;
	/*****/
	CButton m_checkBackground;
	CButton m_buttonBackground;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	/*****/
	afx_msg void OnButtonInterface();
	afx_msg void OnSelectInterface();
	afx_msg void OnToggleAutoDelPCtrl();
	afx_msg void OnButtonReader();
	/*****/
	afx_msg void OnButtonUserTypes();
	afx_msg void OnToggleGroupSets();
	afx_msg void OnButtonGroupSets();
	afx_msg void OnToggleGroupShiftDateRange();
	/*****/
	afx_msg void OnToggleLogOff();
	/*****/
	afx_msg void OnTogglePasswords();
	afx_msg void OnButtonPasswords();
	/*****/
	afx_msg void OnToggleBackground();
	afx_msg void OnButtonBackground();
	/*****/
	afx_msg void OnButtonAltKey1();
	afx_msg void OnButtonAltKey2();
	afx_msg void OnButtonAltKey3();
	afx_msg void OnButtonAltKey4();

	DECLARE_MESSAGE_MAP()

private:
	void SetDisplay();

private:
	bool m_bShowGroupShiftOnDuty;
	bool m_bRestartServerReqd;
	int m_nCurrentInterface;
};

//$$******************************************************************
