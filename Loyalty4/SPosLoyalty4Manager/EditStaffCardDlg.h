#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_StaffCard\SQLRowStaffCard.h"
//*******************************************************************

class CEditStaffCardDlg : public CSSDialog
{
public:
	CEditStaffCardDlg( CSQLRowStaffCard& RowSC, bool bEdit, CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_EDIT_STAFFCARD };
	CSSTabbedComboBox m_comboDatabase;
	CSSTabbedComboBox m_comboFolderSet;
	CEdit m_editName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonFind();
	afx_msg void OnKillFocusAccount();
	afx_msg void OnSelectDatabase();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditServer();
	CEdit* GetEditAccount();

private:
	CSQLRowStaffCard& m_RowSC;
	bool m_bEdit;
};

//*******************************************************************
