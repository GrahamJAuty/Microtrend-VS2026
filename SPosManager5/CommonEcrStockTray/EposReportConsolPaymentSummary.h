#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
/**********************************************************************/

struct CEposReportConsolPaySum
{
public:
	CEposReportConsolPaySum();
	void Reset();
	
public:
	int Compare ( CEposReportConsolPaySum& source, int nHint = 0 );
	void Add	( CEposReportConsolPaySum& source );

public:
	int GetReportQty() { return m_nQty; }
	double GetReportTender() { return (double) m_nTender / 100.0; }
	double GetReportAmount() { return (double) m_nAmount / 100.0; }
	double GetReportGratuity() { return (double) m_nGratuity / 100.0; }
	double GetReportCashback() { return (double) m_nCashback / 100.0; }
	double GetReportSurplus() { return (double) m_nSurplus / 100.0; }
	double GetReportChange() { return (double) m_nChange / 100.0; }
	double GetReportDepositSpend() { return (double) m_nDepositSpend / 100.0; }
	double GetReportCustomerSpend() { return (double) m_nCustomerSpend / 100.0; }
	double GetReportRoomSpend() { return (double) m_nRoomSpend / 100.0; }
	double GetReportLoyaltySpend() { return (double) m_nLoyaltySpend / 100.0; }
	double GetReportSmartPaySpend() { return (double) m_nSmartPaySpend / 100.0; }
	double GetReportSmartPhoneSpend() { return (double) m_nSmartPhoneSpend / 100.0; }
	double GetReportSptBookSpend() { return (double) m_nSptBookSpend / 100.0; }
	
public:
	int m_nReportGroup;
	int m_nSubType;
	bool m_bIsCash;
	
public:
	int m_nQty;
	__int64 m_nTender;
	__int64 m_nAmount;
	__int64 m_nGratuity;
	__int64 m_nCashback;
	__int64 m_nSurplus;
	__int64 m_nChange;
	__int64 m_nDepositSpend;
	__int64 m_nCustomerSpend;
	__int64 m_nRoomSpend;
	__int64 m_nLoyaltySpend;
	__int64 m_nSmartPaySpend;
	__int64 m_nSmartPhoneSpend;
	__int64 m_nSptBookSpend;
	double m_dRA;
};

/**********************************************************************/

struct CEposReportConsolPaySumOneVal
{
public:
	CEposReportConsolPaySumOneVal();
	void Reset();
	
public:
	int Compare ( CEposReportConsolPaySumOneVal& source, int nHint = 0 );
	void Add	( CEposReportConsolPaySumOneVal& source );

public:
	double GetReportValue() { return (double) m_nValue / 100.0; }
	
public:
	int m_nReportGroup;
	int m_nSubType;
	
public:
	__int64 m_nValue;
};

/**********************************************************************/

struct CEposReportPaymentSummaryDateBlockIndex
{
public:
	CEposReportPaymentSummaryDateBlockIndex();
	void Reset();

public:
	int Compare(CEposReportPaymentSummaryDateBlockIndex& source, int nHint = 0);
	void Add(CEposReportPaymentSummaryDateBlockIndex& source);

public:
	int m_nBaseBlockIdx;
	CString m_strDate;

public:
	int m_nDateBlockIdx;
};

/**********************************************************************/

struct CEposReportConsolServerBlockMap
{
public:
	CEposReportConsolServerBlockMap();
	void Reset();

public:
	int Compare( CEposReportConsolServerBlockMap& source, int nHint = 0 );
	void Add( CEposReportConsolServerBlockMap& source );

public:
	int m_nServerLocOrDbSort;
	int m_nServerLocOrDbIdx;
	int m_nServerNo;
	int m_nBlockIdxStart;
};

/**********************************************************************/

struct CEposReportPaySumBlock : public CEposReportBlockMapEntity
{
public:
	bool GotData();
	__int64 GetLineCount();
	void PrepareForUse();

public:
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsAll = nullptr;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsItem = nullptr;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsAccount = nullptr;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsDeposit = nullptr;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsCustomer = nullptr;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsRoom = nullptr;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsLoyalty = nullptr;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsSmartPay = nullptr;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsSmartPhone = nullptr;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsSptBook = nullptr;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsMixed = nullptr;
	
	CReportConsolidationArray<CEposReportConsolPaySumOneVal>* m_pPaidIn = nullptr;
	CReportConsolidationArray<CEposReportConsolPaySumOneVal>* m_pPaidOut = nullptr;
	CReportConsolidationArray<CEposReportConsolPaySumOneVal>* m_pNet = nullptr;
	
	bool m_bGotPaidIOTotal = FALSE;
	__int64 m_nPaidInTotalCash = 0;
	__int64 m_nPaidInTotalNonCash = 0;
	__int64 m_nPaidOutTotalCash = 0;
	__int64 m_nPaidOutTotalNonCash = 0;
	
	bool m_bGotDepositRA = FALSE;
	bool m_bGotCustomerRA = FALSE;
	bool m_bGotRoomRA = FALSE;
	bool m_bGotLoyaltyRA = FALSE;
	bool m_bGotSmartPayRA = FALSE;
	bool m_bGotSmartPhoneRA = FALSE;
	bool m_bGotSptBookRA = FALSE;
	double m_dDepositRA = 0.0;
	double m_dDepositRefund = 0.0;
	double m_dCustomerRA = 0.0;
	double m_dRoomRA = 0.0;
	double m_dLoyaltyRA = 0.0;
	double m_dSmartPayRA = 0.0;
	double m_dSmartPhoneRA = 0.0;
	double m_dSptBookRA = 0.0;

private:
	bool m_bDonePrepareForUse = FALSE;
};

/**********************************************************************/

