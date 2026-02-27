//$$******************************************************************
#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\AlertText.h"
#include "..\SmartPay4Shared\AllergyText.h"
#include "..\SmartPay4ManagerBgnd\DbExportHandler.h"
//$$******************************************************************
#include "CommentHandler.h"
//$$******************************************************************

class CExportPropertyPage3 : public CSSAutoShutdownPropertyPage
{
public:
	CExportPropertyPage3();
	virtual ~CExportPropertyPage3();
	void SetPointer ( CDbExportHandler* pDbExportHandler );

	enum { IDD = IDD_PROPPAGE_EXPORT3 };
	
	CSSComboBox m_comboField1;
	CSSComboBox m_comboCondition1;
	CSSComboBox m_comboAnd;
	CSSComboBox m_comboField2;
	CSSComboBox m_comboCondition2;
	/*****/
	CSSComboBox m_comboGender;
	CSSComboBox m_comboInfoText;
	CSSComboBox m_comboInfoFilter;
	CStatic m_staticBioRegister1;
	CStatic m_staticBioRegister2;
	CStatic m_staticBioRegister3;
	CSSComboBox m_comboBioRegister;
	CButton m_checkDietary;
	CEdit m_editDietary;
	CButton m_checkAllergies;
	CButton m_buttonAllergies;
	CEdit m_editAllergies;
	CButton m_checkAlert;
	CButton m_buttonAlert;
	CEdit m_editAlert;
	int m_nAllergyCodes;
	int m_nAlertCode;

private:
	CEdit* GetEditValue1();
	CEdit* GetEditValue2();
	CEdit* GetEditPoints1();
	CEdit* GetEditPoints2();

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnSelectField1();
	afx_msg void OnSelectCondition1();
	afx_msg void OnSelectField2();
	afx_msg void OnSelectCondition2();
	/*****/
	afx_msg void OnSelectInfo();
	afx_msg void OnDoubleClickStaticDot();
	afx_msg void OnToggleDietary();
	afx_msg void OnToggleAllergies();
	afx_msg void OnButtonAllergies();
	afx_msg void OnToggleAlert();
	afx_msg void OnButtonAlert();
	/*****/
	afx_msg void OnButtonClear();

	DECLARE_MESSAGE_MAP()

private:
	void ExtractConditionsToControls();
	CString GetAllergyString();
	CString GetAlertString();
	void AddInfoSelections( CString strInfoFilterText);
	bool IsPointsField(CSSComboBox& combo, CString& strField);

private:
	CDbExportHandler* m_pDbExportHandler;
	CAllergyText m_Allergy;
	CAlertText m_Alert;
	CCommentHandler m_InfoList;
	CString m_strLastInfoFile;
};

//$$******************************************************************
