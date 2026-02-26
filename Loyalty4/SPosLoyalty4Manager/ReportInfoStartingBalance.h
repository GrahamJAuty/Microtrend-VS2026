#pragma once
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************
#include "AuditRecord.h"
#include "DbReportHandler.h"
//$$******************************************************************

class CReportInfoStartingBalance
{
public:
	CReportInfoStartingBalance();

public:
	//date
	const char* GetDateBalanceString();
	const char* GetDateSearchFromString();
	const char* GetDateSearchToString();
	CSSDate GetDateBalance(){ return m_dateBalance; }
	CSSDate GetDateSearchFrom(){ return m_dateSearchFrom; }
	CSSDate GetDateSearchTo() { return m_dateSearchTo; }
	
public:
	const char* GetReportTitle(){ return m_strReportTitle; }
	const char* GetReportKey(){ return m_strReportKey; }
	const char* GetReportLabel(){ return m_strReportLabel; }
		
public:
	//date
	void SetDateBalance( CTime& date );
	void SetDateSearchFrom(CTime& date);
	void SetDateSearchTo(CTime& date);
	
public:
	void SetReportKey( const char* sz ){ m_strReportKey = sz; }
	void SetReportTitle( const char* sz ){ m_strReportTitle = sz; }
	void SetReportLabel( const char* sz ){ m_strReportLabel = sz; }

public:
	int IsBeforeReportDate(CAuditRecord* atcRecord);
	
private:
	//date
	CSSDate m_dateBalance;
	CSSDate m_dateSearchFrom;
	CSSDate m_dateSearchTo;
	CString m_strDateBalance = "";
	CString m_strDateSearchFrom = "";
	CString m_strDateSearchTo = "";
	
private:
	CString m_strReportTitle = "";
	CString m_strReportLabel = "";
	CString m_strReportKey = "";
};

//$$******************************************************************

