#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CPINRangeDlg : public CSSAutoShutdownDialog
{
public:
	CPINRangeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_PINRANGE };
	CButton m_checkKeep;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();
	virtual void CSSAutoShutdownLogOff();

private:
	CSSTabbedComboBox m_tabcomboGroupAdd;
	CSSTabbedComboBox m_tabcomboGroupRemove;
	CEdit m_editCardNoAddFrom;
	CEdit m_editCardNoAddTo;
	CEdit m_editCardNoRemoveFrom;
	CEdit m_editCardNoRemoveTo;
};

//$$******************************************************************

