#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

struct CFamilyEditInfo
{
public:
	CString m_strForename;
	CString m_strSurname;
	CString m_strDOB;
	CString m_strGender;
	int m_nMemberType;
};

//$$******************************************************************

class CFamilyEditDlg : public CSSDialog
{
public:
	CFamilyEditDlg(const char* szTitle, CFamilyEditInfo& info, CWnd* pParent = NULL);   
	virtual ~CFamilyEditDlg();

	enum { IDD = IDD_FAMILYEDIT };
	CEdit m_editForename;
	CEdit m_editSurname;
	CSSComboBox m_comboGender;
	CSSComboBox m_comboMemberType;
	CString m_strDob;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditDOB();

public:
	CFamilyEditInfo& m_editInfo;
	
private:
	CString m_strTitle;
};

//$$******************************************************************
