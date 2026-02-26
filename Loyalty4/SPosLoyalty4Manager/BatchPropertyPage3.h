#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CBatchPropertyPage3 : public CSSPropertyPage
{
public:
	CBatchPropertyPage3();
	virtual ~CBatchPropertyPage3() {}

	enum { IDD = IDD_PROPPAGE_BATCH3 };
	CSSComboBox	m_comboAnd;
	CSSComboBox	m_comboCondition1;
	CSSComboBox	m_comboCondition2;
	CSSComboBox	m_comboCondition3;
	CSSComboBox	m_comboField2;
	CSSComboBox	m_comboField1;
	double	m_dValue1;
	double	m_dValue2;
	CString	m_strCondition1;
	CString	m_strCondition2;
	CString	m_strField1;
	CString	m_strField2;
	CString	m_strAndOr;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboField1();
	afx_msg void OnSelchangeComboField2();
	afx_msg void OnSelchangeComboCondition1();
	afx_msg void OnSelchangeComboCondition2();
	afx_msg void OnButtonClear();

	DECLARE_MESSAGE_MAP()

public:
	void GetSQLConditions(CDbExportHandler& DbExportHandler);

private:
	CEdit* GetEditValue1();
	CEdit* GetEditValue2();

private:
	void ResetConditions();
};

//$$******************************************************************
