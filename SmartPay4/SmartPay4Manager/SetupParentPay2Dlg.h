#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\ParentPayData.h"
//$$******************************************************************

class CSetupParentPay2Dlg : public CSSAutoShutdownDialog
{
public:
	CSetupParentPay2Dlg(CParentPayData* pData,CWnd* pParent = NULL);
	
// Dialog Data
	enum { IDD = IDD_SETUP_PARENTPAY2 };
	CStatic m_staticAccountID;
	CEdit m_editCardNo;
	CStatic m_staticLegacyLocateBy;
	CSSComboBox m_comboLegacyLocateBy;
	int m_nLegacyLocateBy;
	CSSComboBox m_comboUPN;
	int m_nUPNInfoIndex;
	CSSComboBox m_comboSIMS;
	int m_nRollNoInfoIndex;
	CSSComboBox m_comboPID;
	int m_nMisIDInfoIndex;
	CSSComboBox m_comboYear;
	int m_nYearInfoIndex;
	CSSComboBox m_comboReg;
	int m_nRegInfoIndex;
	CSSComboBox m_comboDob;
	int m_nDobInfoIndex;
	int m_nNewIntakeAccount;
	CSSComboBox m_comboNewIntake;
	BOOL m_bAddUnknowns;
	CString m_strStartFromUserID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnToggleAddUnknowns();
	afx_msg void OnSelectNewIntake();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void SetDisplay();

private:
	CParentPayData* m_pData;
};

//$$******************************************************************

