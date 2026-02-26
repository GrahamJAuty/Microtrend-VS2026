#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_OfferStamp\SQLRecordSetOfferStamp.h"
//$$******************************************************************

class CStampOfferModifyDlg : public CSSDialog
{
public:
	CStampOfferModifyDlg(bool bAllAccounts, __int64 nAccountNo, int nOfferID, CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	enum { IDD = IDD_STAMPOFFER_MODIFY };

protected:
	virtual BOOL OnInitDialog();
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
	void ModifyStampExpiryDates();
	void MakeAllStampsCurrentEdition();
	void UpdateRecords(CSQLRecordSetOfferStamp& RecordSet, int nAppNo, int nEditionOrExpiry, CString strNewDate, int& nRows, int& nStamps);
	CString GetAccountFilter();

private:
	bool m_bAllAccounts;
	__int64 m_nUserID;
	int m_nOfferID;

private:
	CString m_strResults;
};

//$$******************************************************************

