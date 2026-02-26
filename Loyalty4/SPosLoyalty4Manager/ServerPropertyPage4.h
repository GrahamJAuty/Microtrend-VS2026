#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CServerPropertyPage4 : public CSSPropertyPage
{
public:
	CServerPropertyPage4();
	
	enum { IDD = IDD_PROPPAGE_SERVER4 };
	
	CStatic m_staticRenewPlu[17];
	CStatic m_staticRenewX[17];
	CSSComboBox m_comboRenewType[17];
	CDateTimeCtrl m_dtpickRenew[17];
	CSSComboBox m_comboRenewFixedYear[17];
	/**********/
	CString m_strRenewPluNo[17];
	int m_nRenewMultiplier[17];
	int m_nRenewYearDayNumber[17];
	int m_nRenewType[17];
	int m_nRenewFixedYear[17];
	/**********/
	int m_staticRenewPluID[17];
	int m_staticRenewXID[17];
	int m_comboRenewTypeID[17];
	int m_dtpickRenewID[17];
	int m_comboRenewFixedYearID[17];
	int m_editRenewPluID[17];
	int m_editRenewMultiplierID[17];
	
protected:

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	/**********/
	afx_msg void OnSelectRenewType1();
	afx_msg void OnSelectRenewType2();
	afx_msg void OnSelectRenewType3();
	afx_msg void OnSelectRenewType4();
	afx_msg void OnSelectRenewType5();
	afx_msg void OnSelectRenewType6();
	afx_msg void OnSelectRenewType7();
	afx_msg void OnSelectRenewType8();
	afx_msg void OnSelectRenewType9();
	afx_msg void OnSelectRenewType10();
	afx_msg void OnSelectRenewType11();
	afx_msg void OnSelectRenewType12();
	afx_msg void OnSelectRenewType13();
	afx_msg void OnSelectRenewType14();
	afx_msg void OnSelectRenewType15();
	afx_msg void OnSelectRenewType16();
	
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditRenewPlu(int n);
	CEdit* GetEditRenewMultiplier(int n);
	
private:
	void LoadRenewalSettings();
	void SaveRenewalSettings();
	void SelectRenewType( int n );
};

//$$******************************************************************
