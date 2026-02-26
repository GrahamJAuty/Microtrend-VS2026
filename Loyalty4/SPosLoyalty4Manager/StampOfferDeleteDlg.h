#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_OfferStamp\SQLRecordSetOfferStamp.h"
//$$******************************************************************

class CStampOfferDeleteDlg : public CSSDialog
{
public:
	CStampOfferDeleteDlg(bool bAllAccounts, __int64 nAccountNo, int nOfferID, CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	enum { IDD = IDD_STAMPOFFER_DELETE };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
	CButton m_checkAction1;
	CButton m_checkAction2;

private:
	void DeleteExpiredStamps();
	void DeletePreviousEditionStamps();
	void DeleteRecords(CSQLRecordSetOfferStamp& RecordSet, int nAppNo, int& nRows, int& nStamps);
	CString GetAccountFilter();

private:
	bool m_bAllAccounts;
	__int64 m_nUserID;
	int m_nOfferID;

private:
	CString m_strResults;
};

//$$******************************************************************

