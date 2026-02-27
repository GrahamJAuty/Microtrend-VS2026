#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CSystemPropertyPage3 : public CSSAutoShutdownPropertyPage
{
public:
	CSystemPropertyPage3();
	
	enum { IDD = IDD_PROPPAGE_SYSTEM3 };
	CStatic m_staticPurses;
	CButton m_checkShowPurseTopup;
	CButton m_checkEditPurse1;
	CButton m_checkEditPurse2;
	CButton m_checkEnablePurse3;
	CButton m_checkEditPurse3;
	/*****/
	CStatic m_staticPoints;
	CButton m_checkEnablePoints;
	CButton m_checkEditPoints;
	CButton m_checkShowPointsTopup;
	CButton m_checkStatusColour;
	/*****/
	CStatic m_staticMisc;
	CStatic m_staticDefault;
	CSSComboBox m_comboPage;
	CButton m_checkEnableBatchUpdates;
	CButton m_buttonAltKey1;
	CButton m_checkEnableNoAudit;
	CButton m_checkExternal;
	CButton m_checkPromptCancel;
	CButton m_checkAuditImportFilter;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	afx_msg void OnToggleEnablePurse3();
	afx_msg void OnToggleEnablePoints();
	afx_msg void OnButtonAltKey1();
	
	DECLARE_MESSAGE_MAP()

private:
	void SetDisplay();
	void DisplayPebbleMode();

private:
	bool m_bPebbleMode;
};

//$$******************************************************************
