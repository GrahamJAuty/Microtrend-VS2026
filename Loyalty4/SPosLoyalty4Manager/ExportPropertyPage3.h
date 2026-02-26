#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "DbExportHandler.h"
//$$******************************************************************

class CExportPropertyPage3 : public CSSPropertyPage
{
public:
	CExportPropertyPage3();
	virtual ~CExportPropertyPage3();
	void SetPointer ( CDbExportHandler* pDbExportHandler );

	enum { IDD = IDD_PROPPAGE_EXPORT3 };
	CSSComboBox	m_comboAnd;
	CSSComboBox	m_comboCondition2;
	CSSComboBox	m_comboCondition1;
	CSSComboBox	m_comboField2;
	CSSComboBox	m_comboField1;
	double	m_dValue1;
	double	m_dValue2;
	CString	m_strCondition2;
	CString	m_strField1;
	CString	m_strField2;
	CString	m_strCondition1;
	CString	m_strAndOr;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnSelectField1();
	afx_msg void OnSelectField2();
	afx_msg void OnSelectCondition1();
	afx_msg void OnSelectCondition2();
	afx_msg void OnButtonClear();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditValue1();
	CEdit* GetEditValue2();

private:
	CDbExportHandler* m_pDbExportHandler;

private:
	void ExtractConditions();
};

//$$******************************************************************
