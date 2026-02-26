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

public:
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsAll;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsItem;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsAccount;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsDeposit;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsCustomer;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsRoom;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsLoyalty;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsSmartPay;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsSmartPhone;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsSptBook;
	CReportConsolidationArray<CEposReportConsolPaySum>* m_pPaymentsMixed;
	
	CReportConsolidationArray<CEposReportConsolPaySumOneVal>* m_pPaidIn;
	CReportConsolidationArray<CEposReportConsolPaySumOneVal>* m_pPaidOut;
	CReportConsolidationArray<CEposReportConsolPaySumOneVal>* m_pNet;
	
	bool m_bGotPaidIOTotal;
	__int64 m_nPaidInTotalCash;
	__int64 m_nPaidInTotalNonCash;
	__int64 m_nPaidOutTotalCash;
	__int64 m_nPaidOutTotalNonCash;
	
	bool m_bGotDepositRA;
	bool m_bGotCustomerRA;
	bool m_bGotRoomRA;
	bool m_bGotLoyaltyRA;
	bool m_bGotSmartPayRA;
	bool m_bGotSmartPhoneRA;
	bool m_bGotSptBookRA;
	double m_dDepositRA;
	double m_dDepositRefund;
	double m_dCustomerRA;
	double m_dRoomRA;
	double m_dLoyaltyRA;
	double m_dSmartPayRA;
	double m_dSmartPhoneRA;
	double m_dSptBookRA;
};

/**********************************************************************/

