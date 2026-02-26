#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CSystemPropertyPage3 : public CSSPropertyPage
{
public:
	CSystemPropertyPage3();
	virtual ~CSystemPropertyPage3();

	enum { IDD = IDD_PROPPAGE_SYSTEM3 };
	CButton m_buttonOnExit1;
	CButton m_buttonOnExit2;
	CButton m_buttonOnExit3;
	CButton m_buttonOnDelete;
	CButton m_checkExportOnExit;
	CButton m_checkExportOnDelete;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnToggleExportOnExit();
	afx_msg void OnButtonOnExit1();
	afx_msg void OnButtonOnExit2();
	afx_msg void OnButtonOnExit3();
	afx_msg void OnToggleExportOnDelete();
	afx_msg void OnButtonOnDelete();
	DECLARE_MESSAGE_MAP()
	
private:
	void EditLabel ( const char* szLabel );
	void SetButtonTexts();
};

//$$******************************************************************
