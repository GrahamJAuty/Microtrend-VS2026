//********************************************************************
// Compile Cashless Posting values
//*********************************************************************
#pragma once
//*********************************************************************
#include "CardCounterDatabase.h"
#include "SmartPayAuditFile.h"
#include "SQLAuditRecord.h"
#include "TaxCodes.h"
#include "TNoDatabase.h"
#include "WorkingDlg.h"
//*********************************************************************

struct CEODGroupSale
{
public:
	CEODGroupSale();

public:
	int m_nHeadCount;
	int m_nPossibleCount;
	double m_dPurse1Liability;
	double m_dPurse1Credit;
	double m_dPurse2;
	double m_dPurse3Liability;
	double m_dPurse3Credit;
	double m_dCash;
};

//*******************************************************************
#define CASHLESS_TOTALTYPE_OVERALL 0
#define CASHLESS_TOTALTYPE_PURSE1LIABILITY 1
#define CASHLESS_TOTALTYPE_PURSE1CREDIT 2
#define CASHLESS_TOTALTYPE_PURSE2 3
#define CASHLESS_TOTALTYPE_PURSE3LIABILITY 4
#define CASHLESS_TOTALTYPE_PURSE3CREDIT 5
#define CASHLESS_TOTALTYPE_CASH 6
//*******************************************************************

struct CEODCashlessTotals
{
public:
	CEODCashlessTotals();

public:
	void Reset();

public:
	double m_dAdded[7];
	double m_dSubtracted[7];
};

//*******************************************************************
#define EODENTRY_ADJUST 0
#define EODENTRY_PCTOPUP 1
#define EODENTRY_IMPORTTOPUP 2
#define EODENTRY_ECRTOPUP 3
#define EODENTRY_CREDIT 4
#define EODENTRY_MEAL 5
#define EODENTRY_WEBPAYMENT 6
#define EODENTRY_ECRSALE 7
#define EODENTRY_VENDING 8
#define EODENTRY_DELETION 9
#define EODENTRY_ADDEDBONUS 10
#define EODENTRY_SPENDBONUS 11
#define EODENTRY_KIOSKADDED 12
#define EODENTRY_PEBBLEORDER 13
//*******************************************************************
#define EODENTRY_TYPE_COUNT 14
//*******************************************************************
#define EOD_PURSE0 0
#define EOD_PURSE1LIABILITY 1
#define EOD_PURSE1CREDIT 2
#define EOD_PURSE2 3
#define EOD_PURSE3LIABILITY 4
#define EOD_PURSE3CREDIT 5
//*******************************************************************

struct CEODEntry
{
public:
	CEODEntry();

public:
	void Reset();

public:
	double m_dAddCash;
	double m_dAddPurse0;
	double m_dAddPurse1Liability;
	double m_dAddPurse1Credit;
	double m_dAddPurse2;
	double m_dAddPurse3Liability;
	double m_dAddPurse3Credit;
	/*****/
	double m_dSubCash;
	double m_dSubPurse0;
	double m_dSubPurse1Liability;
	double m_dSubPurse1Credit;
	double m_dSubPurse2;
	double m_dSubPurse3Liability;
	double m_dSubPurse3Credit;
};

//*******************************************************************

class CEODCashlessConsolidator
{
public:
	CEODCashlessConsolidator ( bool bSecondaryReport );
	~CEODCashlessConsolidator();
	int ConsolidateFromAudit ( CSmartPayAuditFile* pAudit, CTime DateTimeFrom, CTime DateTimeTo, bool bFirstEOD, bool bRegenerate, CWorkingDlg* pWorking, int& nRecordsChecked );
	int ConsolidateFromAudit2 ( CSmartPayAuditFile* pAudit, CWorkingDlg* pWorkingDlg );

	void CalculateTotals();
	void ResetCardCounter();

	CString GetSecondaryFilename ( int nSetNo );
	void SetNoSalesFilename ( const char* szKey );
	CString GetNoSalesFilename() { return m_strNoSalesFilename; }

private:
	int Create ( const char* szReportFile );
	void ResetCounters();
	void OpenCardCounter();
	void CloseCardCounter();
	void AddColumnText();

	void SaveSecondaryData ( int nTno );

	bool ProcessLine( bool bCheckEOD, CTime DateTimeEOD, CTime DateTimeAudit, bool& bFinished );
	void ProcessPcLine ( int nAppNo );
	bool ProcessEcrLine ( int nAppNo );
	bool ProcessKioskLine ( int nAppNo );
	void ProcessServerLine ( int nAppNo );
	bool ProcessVendingLine ( int nAppNo );

	void AllocatePcLineValue(double dValue, int nPurseType, int ActionType, bool bInvertNegative);
	void AllocatePcLineValuePositive(double dValue, int nPurseType, int ActionType);
	void AllocatePcLineValueNegative(double dValue, int nPurseType, int ActionType);
	void AllocatePcLineValueInternal(double dValue, int nPurseType, int ActionType, bool bInvertNegative, bool bForcePositive, bool bForceNegative );

	void AddToGroupSetTotal ( CSQLAuditRecord* pAtc );
	void AddToSalesTaxes ( CSQLAuditRecord* pAtc );

	void GetPossibleCount();

public:
	bool m_bSecondarySaveReqd;

	CEODCashlessTotals m_CashlessTotals;
	CEODGroupSale m_GroupSet[10];
	CEODEntry m_EODEntries[EODENTRY_TYPE_COUNT];

	double m_dTotalSubCash;							// total none account cash sales
	double m_dMealSales;							// sales via pc meals
	double m_dVendingSales;							// sales via vending
	double m_dSalesUnknown;							// sales with no VAT code ( Eg Discount )
	double m_dNotFoundSales;						// audit transaction but no purchase  history line found
	double m_dSales[nMAX_TAXCODES];					// sales by tax code

private:
	bool m_bSecondaryReport;

	CSQLAuditRecord m_atc;
	
	CCardCounterDatabase m_dbCardCounter;
	CString m_strCardCounterFilename;

	CTNoDatabase m_TNoDatabase; 

	CString m_strNoSalesFilename;
};

//********************************************************************

