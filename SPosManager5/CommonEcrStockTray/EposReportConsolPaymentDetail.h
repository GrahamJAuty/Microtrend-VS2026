#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
/**********************************************************************/

struct CEposReportConsolPaymentDetail
{
public:
	CEposReportConsolPaymentDetail();
	void Reset();
	
public:
	int Compare ( CEposReportConsolPaymentDetail& source, int nHint = 0 );
	void Add	( CEposReportConsolPaymentDetail& source );

public:
	double GetReportTender() { return (double) m_nTender / 100.0; }
	double GetReportAmount() { return (double) m_nAmount / 100.0; }
	double GetReportGratuity() { return (double) m_nGratuity / 100.0; }
	double GetReportCashback() { return (double) m_nCashback / 100.0; }
	double GetReportSurplus() { return (double) m_nSurplus / 100.0; }
	double GetReportChange() { return (double) m_nChange / 100.0; }
	
public:
	bool m_bIsBlockTotal;
	int m_nPaymentNo;
	bool m_bIsPaymentTotal;
	CString m_strDate;
	CString m_strTime;
	int m_nLocIdx;
	int m_nSEQNo;
	int m_nServerNo;
	int m_nTableNo;

public:
	CString m_strTableName;
	__int64 m_nTender;
	__int64 m_nAmount;
	__int64 m_nGratuity;
	__int64 m_nCashback;
	__int64 m_nSurplus;
	__int64 m_nChange;
};

/**********************************************************************/

struct CEposReportPaymentDetailBlock : public CEposReportBlockMapEntity
{
public:
	bool GotData();

public:
	CReportConsolidationArray<CEposReportConsolPaymentDetail>* m_pDetail;
};

/**********************************************************************/
