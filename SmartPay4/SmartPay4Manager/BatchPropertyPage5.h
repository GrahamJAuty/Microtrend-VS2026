#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "afxwin.h"
//$$******************************************************************
#include "..\SmartPay4Shared\AlertText.h"
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************
#include "BatchAccountDetailMemory.h"
//$$******************************************************************

class CBatchPropertyPage5 : public CSSAutoShutdownPropertyPage
{
public:
	CBatchPropertyPage5(CBatchAccountDetailMemory& DetailMemory);
	virtual ~CBatchPropertyPage5() {}
	bool HaveUpdates();
	bool UpdateRecord(CSQLRowAccountFull& RowAccount);
	void AddWarnings ( CStringArray* pArray );
	void SetPointer(CSQLRowAccountFull& RowAccount);

	enum { IDD = IDD_PROPPAGE_BATCH5 };

private:
	CEdit* GetEditMaxSpend();
	CEdit* GetEditOverSpend();

public:
	CButton m_checkSetGroup;
	CButton m_checkSetExpiry;
	CButton m_checkSetDOB;
	CButton m_checkSetMaxSpend;
	CButton m_checkSetOverSpend;
	CButton m_checkSetAlert;
	CButton m_checkSetName;
	
	CButton m_checkClearGroup;
	CButton m_checkClearExpiry;
	CButton m_checkClearDOB;
	CButton m_checkClearMaxSpend;
	CButton m_checkClearOverSpend;
	
	CSSTabbedComboBox m_tabcomboGroup;
	CDateTimeCtrl m_DatePickerExpiry;
	CDateTimeCtrl m_DatePickerDOB;
	CButton m_buttonAlert;
	CEdit m_editAlert;
	
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
	afx_msg void OnToggleSetGroup();
	afx_msg void OnToggleSetExpiry();
	afx_msg void OnToggleSetDob();
	afx_msg void OnToggleSetMaxSpend();
	afx_msg void OnToggleSetOverSpend();
	afx_msg void OnToggleSetAlert();
	afx_msg void OnToggleClearGroup();
	afx_msg void OnToggleClearExpiry();
	afx_msg void OnToggleClearDob();
	afx_msg void OnToggleClearMaxSpend();
	afx_msg void OnToggleClearOverSpend();
	afx_msg void OnButtonAlert();	
	DECLARE_MESSAGE_MAP()

private:
	CString GetAlertText();

private:
	CAlertText m_alertText;
	int m_nAlertCode;

private:
	CBatchAccountDetailMemory& m_Memory;
};

//$$******************************************************************
