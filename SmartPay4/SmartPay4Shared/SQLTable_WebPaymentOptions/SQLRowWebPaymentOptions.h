#pragma once
//**********************************************************************
#include "SQLFieldValidatorWebPaymentOptions.h"
//**********************************************************************

class CSQLRowWebPaymentOptions
{
public:
	CSQLRowWebPaymentOptions();
	
public:
	void Reset();
	CString GetError() { return m_strError; }
	
public:
	int GetLineID() { return m_nLineID;  }
	int GetAccountLinkDays() { return m_nAccountLinkDays; }
	CString GetAccountLinkStart() { return m_strAccountLinkStart; }
	CString GetAccountLinkEnd() { return m_strAccountLinkEnd; }
	CString GetAccountLinkLastDate() { return m_strAccountLinkLastDate; }
	CString GetAccountLinkLastTime() { return m_strAccountLinkLastTime; }
	int GetPaymentDays() { return m_nPaymentDays; }
	CString GetPaymentStart() { return m_strPaymentStart; }
	CString GetPaymentEnd() { return m_strPaymentEnd; }
	CString GetPaymentLastDate() { return m_strPaymentLastDate; }
	CString GetPaymentLastTime() { return m_strPaymentLastTime; }
	CString GetBalanceLastDate() { return m_strBalanceLastDate; }
	CString GetBalanceLastTime() { return m_strBalanceLastTime; }
	CString GetTransactionLastDate() { return m_strTransactionLastDate; }
	CString GetTransactionLastTime() { return m_strTransactionLastTime; }
	bool GetExclude1Flag() { return m_bExclude1Flag; }
	CString GetExclude1Start() { return m_strExclude1Start; }
	CString GetExclude1End() { return m_strExclude1End; }
	bool GetExclude2Flag() { return m_bExclude2Flag; }
	CString GetExclude2Start() { return m_strExclude2Start; }
	CString GetExclude2End() { return m_strExclude2End; }
	bool GetExclude3Flag() { return m_bExclude3Flag; }
	CString GetExclude3Start() { return m_strExclude3Start; }
	CString GetExclude3End() { return m_strExclude3End; }
	bool GetExclude4Flag() { return m_bExclude4Flag; }
	CString GetExclude4Start() { return m_strExclude4Start; }
	CString GetExclude4End() { return m_strExclude4End; }
	CString GetExcludeUploadStart() { return m_strExcludeUploadStart; }
	CString GetExcludeUploadEnd() { return m_strExcludeUploadEnd; }
	bool GetEODAfterPaymentFlag() { return m_bEODAfterPaymentFlag; }
	CString GetEODAfterTime() { return m_strEODAfterTime; }
	bool GetPromptBeforeAccessFlag() { return m_bPromptBeforeAccessFlag; }
	bool GetImportAccountsFlag() { return m_bImportAccountsFlag; }

public:
	void SetLineID(int n) { vdtr.SetLineID(n, m_nLineID); }
	void SetAccountLinkDays(int n) { vdtr.SetDayFlags(n, m_nAccountLinkDays); }
	void SetAccountLinkStart(CString str) { vdtr.SetTimeString(str, m_strAccountLinkStart); }
	void SetAccountLinkEnd(CString str) { vdtr.SetTimeString(str, m_strAccountLinkEnd); }
	void SetAccountLinkLastDate(CString str) { vdtr.SetDateString(str, m_strAccountLinkLastDate); }
	void SetAccountLinkLastTime(CString str) { vdtr.SetTimeString(str, m_strAccountLinkLastTime); }
	void SetPaymentDays(int n) { vdtr.SetDayFlags(n, m_nPaymentDays); }
	void SetPaymentStart(CString str) { vdtr.SetTimeString(str, m_strPaymentStart); }
	void SetPaymentEnd(CString str) { vdtr.SetTimeString(str, m_strPaymentEnd); }
	void SetPaymentLastDate(CString str) { vdtr.SetDateString(str, m_strPaymentLastDate); }
	void SetPaymentLastTime(CString str) { vdtr.SetTimeString(str, m_strPaymentLastTime); }
	void SetBalanceLastDate(CString str) { vdtr.SetDateString(str, m_strBalanceLastDate); }
	void SetBalanceLastTime(CString str) { vdtr.SetTimeString(str, m_strBalanceLastTime); }
	void SetTransactionLastDate(CString str) { vdtr.SetDateString(str, m_strTransactionLastDate); }
	void SetTransactionLastTime(CString str) { vdtr.SetTimeString(str, m_strTransactionLastTime); }
	void SetExclude1Flag(bool b) { m_bExclude1Flag = b; }
	void SetExclude1Start(CString str) { vdtr.SetTimeString(str, m_strExclude1Start); }
	void SetExclude1End(CString str) { vdtr.SetTimeString(str, m_strExclude1End); }
	void SetExclude2Flag(bool b) { m_bExclude2Flag = b; }
	void SetExclude2Start(CString str) { vdtr.SetTimeString(str, m_strExclude2Start); }
	void SetExclude2End(CString str) { vdtr.SetTimeString(str, m_strExclude2End); }
	void SetExclude3Flag(bool b) { m_bExclude3Flag = b; }
	void SetExclude3Start(CString str) { vdtr.SetTimeString(str, m_strExclude3Start); }
	void SetExclude3End(CString str) { vdtr.SetTimeString(str, m_strExclude3End); }
	void SetExclude4Flag(bool b) { m_bExclude4Flag = b; }
	void SetExclude4Start(CString str) { vdtr.SetTimeString(str, m_strExclude4Start); }
	void SetExclude4End(CString str) { vdtr.SetTimeString(str, m_strExclude4End); }
	void SetExcludeUploadStart(CString str) { vdtr.SetTimeString(str, m_strExcludeUploadStart); }
	void SetExcludeUploadEnd(CString str) { vdtr.SetTimeString(str, m_strExcludeUploadEnd); }
	void SetEODAfterPaymentFlag(bool b) { m_bEODAfterPaymentFlag = b; }
	void SetEODAfterTime(CString str) { vdtr.SetTimeString(str, m_strEODAfterTime); }
	void SetPromptBeforeAccessFlag(bool b) { m_bPromptBeforeAccessFlag = b; }
	void SetImportAccountsFlag(bool b) { m_bImportAccountsFlag = b; }
	/*****/
	void SetRowVersion(CByteArray& arraySource);

public:
	bool AccountLinkAllowed(int m_nWebPaymentType = -1);
	bool EODAllowed(int m_nWebPaymentType = -1);
	bool IsAccountLinkDay(int nDay);
	void SetAccountLinkDay(int nDay, bool bSet);
	bool IsPaymentDay(int nDay);
	void SetPaymentDay(int nDay, bool bSet);
	bool ImportAccountsService();
	CString GetImportMenuCaption();
	
	bool CheckTimeInRange(bool bCheck, CSSTime& timeNow, CString& strStart, CString& strEnd);
	bool TimeForEndOfDay();
	bool TimeForUpload();
	bool TimeForImports();
	bool TimeForAccountLink();

	void SetLastPaymentDone();
	void SetLastTransactionDone();
	void ClearLastTransactionDone();
	void SetLastBalanceDone();
	void ClearLastBalanceDone();
	void SetLastAccountLinkDone();
	void ClearLastAccountLinkDone();

	bool CompareRowVersion(CSQLRowWebPaymentOptions& RowSource);
	CString GetRowVersionString();

private:
	int m_nLineID = 1;
	int m_nAccountLinkDays = 0x3E;	
	CString m_strAccountLinkStart = "00:00";
	CString m_strAccountLinkEnd = "23:59";
	CString m_strAccountLinkLastDate = "";
	CString m_strAccountLinkLastTime = "";
	int m_nPaymentDays = 0x3E;
	CString m_strPaymentStart = "00:00";
	CString m_strPaymentEnd = "23:59";
	CString m_strPaymentLastDate = "";
	CString m_strPaymentLastTime = "";
	CString m_strBalanceLastDate = "";
	CString m_strBalanceLastTime = "";
	CString m_strTransactionLastDate = "";
	CString m_strTransactionLastTime = "";
	bool m_bExclude1Flag = FALSE;
	CString m_strExclude1Start = "00:00";
	CString m_strExclude1End = "00:00";
	bool m_bExclude2Flag = FALSE;
	CString m_strExclude2Start = "00:00";
	CString m_strExclude2End = "00:00";
	bool m_bExclude3Flag = FALSE;
	CString m_strExclude3Start = "00:00";
	CString m_strExclude3End = "00:00";
	bool m_bExclude4Flag = FALSE;
	CString m_strExclude4Start = "00:00";
	CString m_strExclude4End = "00:00";
	CString m_strExcludeUploadStart = "00:00";
	CString m_strExcludeUploadEnd = "00:00";
	bool m_bEODAfterPaymentFlag = TRUE;
	CString m_strEODAfterTime = "16:00";
	bool m_bPromptBeforeAccessFlag = TRUE;
	bool m_bImportAccountsFlag = TRUE;
	/*****/
	CByteArray m_arrayRowVersion;
	
private:
	CSQLFieldValidatorWebPaymentOptions vdtr;
	CString m_strError = "";
};

//**********************************************************************

