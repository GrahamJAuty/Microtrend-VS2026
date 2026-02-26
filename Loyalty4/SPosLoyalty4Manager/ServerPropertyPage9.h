#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CServerPropertyPage9 : public CSSPropertyPage
{
public:
	CServerPropertyPage9();
	virtual ~CServerPropertyPage9();

	enum { IDD = IDD_PROPPAGE_SERVER9 };
	CButton m_checkAddUnknowns;
	CButton m_checkUnknownCardGroup;
	CSSTabbedComboBox m_comboGroup;
	CComboBox m_comboIdMinLen;
	CEdit m_editPrompt1;
	CEdit m_editPrompt2;
	CEdit m_editPrompt3;
	CEdit m_editPending;
	CEdit m_editCreate;

private:
	CEdit* GetEditIDMin();
	CEdit* GetEditIDMax();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	/*****/
	afx_msg void OnToggleAddUnknowns();
	afx_msg void OnKillFocusEditIDMin();
	afx_msg void OnKillFocusEditIDMax();
	
	DECLARE_MESSAGE_MAP()

private:
	int ExtractGroupNo();
};

//$$******************************************************************

