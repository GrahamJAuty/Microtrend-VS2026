#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************
#include "AlertText.h"
#include "BatchAccountDetailMemory.h"
//$$******************************************************************

class CBatchPropertyPage5 : public CSSPropertyPage
{
public:
	CBatchPropertyPage5( CSQLRowAccountFull& RowAccount, CBatchAccountDetailMemory& Memory);
	virtual ~CBatchPropertyPage5() {}
	bool HaveUpdates();
	bool UpdateRecord(CSQLRowAccountFull& RowAccount);

	enum { IDD = IDD_PROPPAGE_BATCH5 };

private:
	CEdit* GetEditMaxSpend();
	CEdit* GetEditOverSpend();
	
public:
	CButton m_checkSetName;
	CButton m_checkSetGroup;
	CButton m_checkSetScheme;
	CButton m_checkSetExpiry;
	CButton m_checkDateExpiry;
	CDateTimeCtrl m_datePickerExpiry;
	CButton m_checkSetDOB;
	CButton m_checkDateDOB;
	CDateTimeCtrl m_datePickerDOB;
	CButton m_checkSetMaxSpend;
	CButton m_checkSetOverSpend;
	CButton m_checkSetAlert;
	CButton m_checkSetHotlist;
	CButton m_checkSetInactive;
	CButton m_checkSetGender;
	
	CEdit m_editName;
	CSSTabbedComboBox m_comboGroup;
	CSSTabbedComboBox m_comboScheme;

	CButton m_buttonAlert;
	CEdit m_editAlert;
	CSSComboBox m_comboHotlist;
	CSSComboBox m_comboInactive;
	CSSComboBox m_comboGender;

public:
	void RememberGroupNo();

private:
	void UpdateMemory();
	void SetEditDouble( CEdit* pEdit, double d );
	void SetEditInt( CEdit* pEdit, int n );
	double GetEditDouble( CEdit* pEdit );
	int GetEditInt( CEdit* pEdit );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnSelectGroup();
	afx_msg void OnToggleSetName();
	afx_msg void OnToggleSetGroup();
	afx_msg void OnToggleSetScheme();
	afx_msg void OnToggleSetExpiry();
	afx_msg void OnToggleSetDOB();
	afx_msg void OnToggleDateExpiry();
	afx_msg void OnToggleDateDOB();
	afx_msg void OnToggleSetMaxSpend();
	afx_msg void OnToggleSetOverSpend();
	afx_msg void OnToggleSetAlert();
	afx_msg void OnToggleSetHotlist();
	afx_msg void OnToggleSetInactive();
	afx_msg void OnToggleSetGender();
	afx_msg void OnButtonAlert();	
	DECLARE_MESSAGE_MAP()

private:
	CAlertText m_alertText;
	CString m_strAlertCodes;
	COleDateTime m_dateToday;

private:
	CBatchAccountDetailMemory& m_Memory;
	CSQLRowAccountFull& m_RowAccount;
};

//$$******************************************************************
