#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\AlertText.h"
#include "..\SmartPay4Shared\AllergyText.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************
#include "CommentHandler.h"
//$$******************************************************************

struct CBatchPropertyPage3Conditions
{
public:
	CBatchPropertyPage3Conditions();
	
public:
	void ResetConditions1();
	void ResetConditions2();

public:
	CString m_strField1;
	CString m_strCondition1;
	double m_dValue1;
	CString m_strAndOr;
	CString m_strField2;
	CString m_strCondition2;
	double m_dValue2;
	/*****/
	CString m_strCondition3;
	bool m_bCheckDietary;
	CString m_strDietary;
	bool m_bCheckAllergies;
	int m_nAllergyCodes;
	CString m_strAllergies;
	bool m_bCheckAlert;
	int m_nAlertCode;
	CString m_strAlert;
	int m_nInfoTextIndex;
	CString m_strInfoFilterText;
	int m_nBioRegisterCheck;
};

//$$******************************************************************

class CBatchPropertyPage3 : public CSSAutoShutdownPropertyPage
{
public:
	CBatchPropertyPage3();
	virtual ~CBatchPropertyPage3() {}
	
	enum { IDD = IDD_PROPPAGE_BATCH3 };
	CSSComboBox	m_comboField1;
	CSSComboBox	m_comboCondition1;
	CSSComboBox	m_comboAnd;
	CSSComboBox	m_comboField2;
	CSSComboBox	m_comboCondition2;
	/*****/
	CSSComboBox	m_comboCondition3;
	CButton m_checkDietary;
	CEdit m_editDietary;
	CButton m_checkAllergies;
	CEdit m_editAllergies;
	CButton m_buttonAllergies;
	CButton m_checkAlert;
	CEdit m_editAlert;
	CButton m_buttonAlert;
	CSSComboBox m_comboInfoText;
	CSSComboBox m_comboInfoFilter;
	CStatic m_staticBioRegister;
	CSSComboBox m_comboBioRegister;
	
private:
	CEdit* GetEditValue1() { return GetEdit(IDC_EDIT_VALUE1); }
	CEdit* GetEditValue2() { return GetEdit(IDC_EDIT_VALUE2); }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectField1();
	afx_msg void OnSelectCondition1();
	afx_msg void OnSelectField2();
	afx_msg void OnSelectCondition2();
	afx_msg void OnButtonClear1();
	/*****/
	afx_msg void OnToggleDietary();
	afx_msg void OnToggleAllergies();
	afx_msg void OnButtonAllergies();
	afx_msg void OnToggleAlert();
	afx_msg void OnButtonAlert();
	afx_msg void OnSelectInfo();
	afx_msg void OnDoubleClickStaticDot();
	afx_msg void OnButtonClear2();
	/*****/
	virtual BOOL OnKillActive();

	DECLARE_MESSAGE_MAP()

private:
	void SetAllergyString();
	void SetAlertString();
	void AddInfoSelections();
	void AddToFieldCombos(const char* sz);

public:
	bool CheckConditional(CSQLRowAccountFull& RowAccount);

private:
	CBatchPropertyPage3Conditions m_Conditions;

private:
	CAllergyText m_Allergy;
	CAlertText m_Alert;
	CCommentHandler m_InfoList;
	CString m_strLastInfoFile;
};

//$$******************************************************************
