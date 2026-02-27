#pragma once
//**********************************************************************
#include "SQLFieldValidatorEposTerminal.h"
//**********************************************************************
#define nRECORDTYPE_EPOS	1				// Epos sale
#define nRECORDTYPE_KIOSK	2				// Kiosk record
#define nRECORDTYPE_VENDING	3				// vending record
//**********************************************************************

class CSQLRowEposTerminal
{
public:
	CSQLRowEposTerminal();
	
private:
	void Reset();
	
public:
	int GetTerminalNo() { return m_nTerminalNo; }
	int GetTerminalType() { return m_nTerminalType; }
	CString GetTerminalName() { return m_strTerminalName; }
	CString GetLastUsedDate() { return m_strLastUsedDate; }
	CString GetLastUsedTime() { return m_strLastUsedTime; }
	int GetCCNo() { return m_nCCNo; }
	__int64 GetUserID() { return m_nUserID; }
	double GetCardValue() { return m_dCardValue; }
	int GetTotalCounter() { return m_nTotalCounter; }
	double GetTotalInDrawer() { return m_dTotalInDrawer; }
	bool GetDisableFlag() { return m_bDisable; }
	bool GetTerminalOnFlag() { return m_bTerminalOn; }
	bool GetInServiceFlag() { return m_bInService; }
	bool GetDoorOpenFlag() { return m_bDoorOpen; }

public:
	void SetTerminalNo(int n) { vdtr.SetTerminalNo(n, m_nTerminalNo); }
	void SetTerminalType(int n) { vdtr.SetTerminalType(n, m_nTerminalType); }
	void SetTerminalName(CString str) { vdtr.SetTerminalName(str, m_strTerminalName); }
	void SetLastUsedDate(CString str);
	void SetLastUsedTime(CString str);
	void SetLastUsedDateAndTimeToNow();
	void SetCCNo(int n) { vdtr.SetCCNo(n, m_nCCNo); }
	void SetUserID(__int64 n) { vdtr.SetUserID(n, m_nUserID); }
	void SetCardValue(double d) { vdtr.SetCardValue(d, m_dCardValue); }
	void SetTotalCounter(int n) { vdtr.SetTotalCounter(n, m_nTotalCounter); }
	void SetTotalInDrawer(double d) { vdtr.SetTotalInDrawer(d, m_dTotalInDrawer); }
	void SetDisableFlag(bool b) { m_bDisable = b; }
	void SetTerminalOnFlag(bool b) { m_bTerminalOn = b; }
	void SetInServiceFlag(bool b) { m_bInService = b; }
	void SetDoorOpenFlag(bool b) { m_bDoorOpen = b; }

public:
	CString GetCSVDate();
	CString GetCSVTime();
	void SetCashRemoved(int nCCNo, double dGT, const char* szCashFilename);
	void UpdateTotalInDrawer(double dTotal);
	int IncrementCCNo();
	CString GetTerminalText();
	bool IsDateSet();

private:
	bool ValidateDate(CString strDate);
	bool ValidateTime(CString strDate);

private:
	int m_nTerminalNo;
	int m_nTerminalType;
	CString m_strTerminalName;
	CString m_strLastUsedDate;
	CString m_strLastUsedTime;
	int m_nCCNo;
	__int64 m_nUserID;
	double m_dCardValue;
	int m_nTotalCounter;
	double m_dTotalInDrawer;
	bool m_bDisable;
	bool m_bTerminalOn;
	bool m_bInService;
	bool m_bDoorOpen;
	
private:
	CSQLFieldValidatorEposTerminal vdtr;
};

//**********************************************************************

struct CPendingEposTerminalData
{
public:
	CPendingEposTerminalData()
	{
		m_bGotChange = FALSE;
	};

public:
	CSQLRowEposTerminal m_RowEposTerminal;
	bool m_bGotChange;
};

//**********************************************************************
