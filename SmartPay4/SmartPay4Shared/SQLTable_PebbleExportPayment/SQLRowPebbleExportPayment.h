#pragma once
//**********************************************************************
#include "SQLFieldValidatorPebbleExportPayment.h"
//**********************************************************************
#include "..\SQLTable_ChartwellExportPayment\SQLRowChartwellExportPayment.h"
//**********************************************************************
#define MAX_PEBBLE_EXPORT_PAYMENT 5
//**********************************************************************

class CSQLRowPebbleExportPayment
{
public:
	CSQLRowPebbleExportPayment();
	
public:
	void Reset();
	
public:
	__int64 GetUserID() { return m_nUserID; }
	CString GetDate() { return m_strDate; }
	CString GetTime() { return m_strTime; }
	CString GetCreateDateTime() { return m_strCreateDateTime; }
	int GetCCNo() { return m_nCCNo; }
	int GetPaymentType(int n);
	double GetPaymentAmount(int n);

public:
	void SetUserID(__int64 n) { vdtr.SetUserID(n, m_nUserID); }
	void SetDate(CString str) { vdtr.SetDate(str, m_strDate); }
	void SetTime(CString str) { vdtr.SetTime(str, m_strTime); }
	void SetCreateDateTime(CString str) { vdtr.SetCreateDateTime(str, m_strCreateDateTime); }
	void SetCCNo(int n) { vdtr.SetCCNo(n, m_nCCNo); }
	void SetPaymentType(int n, int nType);
	void SetPaymentAmount(int n, double dAmount);

public:
	void ConsolidatePayment(int nType, double dAmount);
	
private:
	__int64 m_nUserID;
	CString m_strDate;
	CString m_strTime;
	CString m_strCreateDateTime;
	int m_nCCNo;
	int m_nPaymentType[MAX_PEBBLE_EXPORT_PAYMENT];
	double m_dPaymentAmount[MAX_PEBBLE_EXPORT_PAYMENT];
	
private:
	CSQLFieldValidatorPebbleExportPayment vdtr;
};

//**********************************************************************

struct CPendingExportPaymentData
{
public:
	CPendingExportPaymentData()
	{
		m_bPebble = FALSE;
		m_bChartwell = FALSE;
	}

public:
	CSQLRowPebbleExportPayment m_PaymentDataPebble;
	CSQLRowChartwellExportPayment m_PaymentDataChartwell;
	bool m_bPebble;
	bool m_bChartwell;
};

//**********************************************************************
