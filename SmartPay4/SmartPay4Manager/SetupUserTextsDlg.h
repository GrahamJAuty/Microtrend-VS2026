#pragma once
//******************************************************************
#include "resource.h"
//******************************************************************

class CSetupUserTextsDlg : public CSSAutoShutdownDialog
{
public:
	CSetupUserTextsDlg( CWnd* pParent = NULL);  
	virtual ~CSetupUserTextsDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_USERTEXTS };
	CEdit m_editInfo1;
	CEdit m_editInfo2;
	CEdit m_editInfo3;
	CEdit m_editInfo4;
	CEdit m_editInfo5;
	CEdit m_editInfo6;
	CEdit m_editInfo7;
	CEdit m_editInfo8;
	CEdit m_editDOB;
	CEdit m_editMemberID;
	CStatic m_staticExtRef;
	CEdit m_editExtRef1;
	CEdit m_editExtRef2;
	CEdit m_editTick1;
	CEdit m_editTick2;
	CEdit m_editTick3;
	CEdit m_editTick4;
	CEdit m_editBioConsent;
	CSSTabbedComboBox m_comboBioConsent;
	CButton m_checkExtRef2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual void OnOK();
	afx_msg void OnToggleExtRef2();
	afx_msg void OnSelectBioConsent();
	DECLARE_MESSAGE_MAP()

private:
	void PrepareEditField(CEdit& edit, CString strLabel);
	void UpdateText(CString strLabel, CString strText, bool bUseFlag = TRUE);

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();
};

//******************************************************************
