#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_PurchaseControlStamp\SQLRecordSetPurchaseControlStamp.h"
//$$******************************************************************

class CPurchaseControlStampModifyDlg : public CSSAutoShutdownDialog
{
public:
	CPurchaseControlStampModifyDlg(bool bAllAccounts, __int64 nAccountNo, int nRuleID, CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	enum { IDD = IDD_PURCHASECONTROLSTAMP_MODIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnToggleAction1();
	afx_msg void OnSelectNewExpiry();
	afx_msg void OnClickStaticTo();
	DECLARE_MESSAGE_MAP()
	CButton m_checkAction1;
	CDateTimeCtrl m_DatePickerCurrentFrom;
	CDateTimeCtrl m_DatePickerCurrentTo;
	CButton m_checkCurrentNoExpire;
	CDateTimeCtrl m_DatePickerNew;
	CSSComboBox m_comboNewExpiry;
	CButton m_checkAction2;

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void ModifyStampExpiryDates();
	void MakeAllStampsCurrentEdition();
	void UpdateRecords(CSQLRecordSetPurchaseControlStamp& RecordSet, int nAppNo, int nEditionOrExpiry, CString strNewDate, int& nRows, int& nStamps);
	CString GetAccountFilter();

private:
	bool m_bAllAccounts;
	__int64 m_nUserID;
	int m_nRuleID;

private:
	CString m_strResults;
};

//$$******************************************************************

