#pragma once
/**********************************************************************/
#include "AnalysisCategoryCSVArray.h"
#include "CashRSPImporterBatchInfo.h"
#include "CashRSPImporterDailyLogPluItem.h"
#include "CashRSPImporterDiscountHelpers.h"
#include "CashRSPImporterItem.h"
#include "CashRSPImporterLinePosItem.h"
#include "CashRSPSpooler.h"
#include "CashRSPVersionChecker.h"
#include "Consolidation.h"
#include "PromotionsCSVArray.h"
#include "RepCSV.h"
/**********************************************************************/

struct CCashRSPImporterTransaction
{
public:
	CCashRSPImporterTransaction();

public:
	void ResetTransaction( int nType, const char* szHeader );
	void ProcessTransactionHeader( CCSVEposTermLine& csvIn, CCashRSPImporterBatchInfo& BatchInfo, CCashRSPSpooler& CashRSPSpooler, bool bRemoveDuplicates );
	void SetCurrentPromoNo( int nPromoNo, CPromotionsCSVArray& arrayPromotions );
	int GetCurrentTransactionVersionNo() { return m_CashRSPVersionChecker.GetCurrentTransactionVersionNo(); }
	
public:
	bool m_bProcessTransaction;
	bool m_bGotDuplicate;
	int m_nTransactionType;
	CString m_strVersionHeader;
	CString m_strTransactionHeader;
	bool m_bIsPaidIO;
	bool m_bIsDelivery;
	CString m_strCSVDate;
	CString m_strDateTran;
	CString m_strDateItem;
	CString m_strTimeTran;
	CString m_strTNoSalesHistory;
	int m_nCovers;
	int m_nCustomerNo;
	int m_nServerNo;
	int m_nPaymentNo;
	int m_nSeqNo;
	int m_nTableNo;
	int m_nItemPayRAType;
	int m_nHeaderValue;
	double m_dTotalSalesValue;
	double m_dSubtotalDiscountValue;
	double m_dLoyaltyDiscountValue;
	bool m_bManualMode;
	CString m_strLoyaltyID;
	int m_nLineTNo;
	CReportConsolidationArray<CCashRSPImporterDiscountHelperSales> m_arrayDiscountHelperSales;
	CReportConsolidationArray<CCashRSPImporterDiscountHelperSales> m_arrayCheapestOneFree;
	CReportConsolidationArray<CCashRSPImporterDiscountHelperSales> m_arrayMMPromoDiscountHelperSales;
	CReportConsolidationArray<CCashRSPImporterDiscountHelperPendingItem> m_arrayDiscountHelperPendingItem;
	CReportConsolidationArray<CCashRSPImporterDiscountHelperPendingSubtotal> m_arrayDiscountHelperPendingSubtotal;
	CReportConsolidationArray<CCashRSPImporterDiscountHelperToWrite> m_arrayDiscountHelperToWrite;
	CReportConsolidationArray<CCashRSPMixMatchTotals> m_arrayMixMatchTotals;
	CReportConsolidationArray<CCashRSPMixMatchItems> m_arrayMixMatchItems;
	CReportConsolidationArray<CCashRSPPromoTotals> m_arrayPromoTotals;
	CReportConsolidationArray<CCashRSPPromoItems> m_arrayPromoItems;
	CCashRSPVersionChecker m_CashRSPVersionChecker;
	
	int m_nCurrentPromoPluType;
	bool m_bCurrentPromoSCTaxFix;
	bool m_bCurrentPromoDiscountForceTaxable;
	int m_nCurrentPromoNo;
	int m_nCurrentPromoDeptNo;
	int m_nCurrentPromoSalesQty;
	bool m_bGotNormalItem;

public:
	CArray<int,int> m_arrayLinePosDiscountItem;
	CArray<int,int> m_arrayLinePosDiscountSubtotal;
	CArray<int,int> m_arrayLinePosDiscountLoyalty;
	CArray<int,int> m_arrayLinePosDiscountPromo;
	CArray<int,int> m_arrayLinePosDiscountMixMatch;
	CArray<int,int> m_arrayLinePosDiscountVoid;
	CArray<CCashRSPImporterLinePosItem,CCashRSPImporterLinePosItem> m_arrayLinePosItem;
	CArray<int,int> m_arrayLinePosPayment;
	CArray<CCashRSPImporterItem,CCashRSPImporterItem> m_bufferItems;

public:
	CStringArray m_bufferDailyLogTerm;
	CArray<CCashRSPImporterDailyLogPluItem> m_bufferDailyLogSales;
	CArray<CCashRSPImporterDailyLogPluItem> m_bufferDailyLogReasons;
	CStringArray m_bufferDailyLogMixMatchUsage;
	CStringArray m_bufferDailyLogAccounts;
	CStringArray m_bufferDailyLogAccountsPay;

public:
	CStringArray m_bufferPendingStockSales;
	CStringArray m_bufferPendingStockDelivery;
};

/**********************************************************************/