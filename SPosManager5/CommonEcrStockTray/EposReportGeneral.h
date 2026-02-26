#pragma once
/**********************************************************************/
#include "BusinessDateInfo.h"
#include "EntityTree.h"
#include "EposReportBase.h"
#include "EposReportBlockMap.h"
#include "EposReportConsolParams.h"
#include "EposReportCustomSettings.h"
#include "ReportConsolidationArray.h"
#include "ReportPluInfoFinder.h"
/**********************************************************************/
#define GENREP_SALES_HEADER 0
#define GENREP_SALES_LINE 1
#define GENREP_SALES_TOTAL 2
/**********************************************************************/
#define GENREP_PAYMENT_LINE 1
#define GENREP_PAYMENT_TOTAL 2
/**********************************************************************/
#define GENREP_PROMO_LINE_SUBT 1
#define GENREP_PROMO_LINE_ITEM 2
#define GENREP_PROMO_LINE_MIXMATCH 3
#define GENREP_PROMO_TOTAL_SUBT 4
#define GENREP_PROMO_TOTAL_ITEM 5
#define GENREP_PROMO_TOTAL_MIXMATCH 6
#define GENREP_PROMO_TOTAL_ALL 7
/**********************************************************************/
#define GENREP_VOID_SALE_LEGACY 1
#define GENREP_VOID_SALE_TRAN 2
#define GENREP_VOID_SALE_BEFORE 3
#define GENREP_VOID_SALE_AFTER 4
#define GENREP_VOID_SALE_TOTAL 5
#define GENREP_VOID_REFUND_LEGACY 6
#define GENREP_VOID_REFUND_TRAN 7
#define GENREP_VOID_REFUND_BEFORE 8
#define GENREP_VOID_REFUND_AFTER 9
#define GENREP_VOID_REFUND_TOTAL 10
/**********************************************************************/
#define GENREP_RECON_SALE 1
#define GENREP_RECON_RA_DEPOSIT 2
#define GENREP_RECON_RA_CUSTOMER 3
#define GENREP_RECON_RA_ROOM 4
#define GENREP_RECON_RA_LOYALTY 5
#define GENREP_RECON_RA_SMARTPAY 6
#define GENREP_RECON_RA_SMARTPHONE 7
#define GENREP_RECON_RA_SPTBOOK 8
#define GENREP_RECON_GRATUITY 9
#define GENREP_RECON_CASHBACK 10
#define GENREP_RECON_SURPLUS 11
#define GENREP_RECON_CHANGE 12
#define GENREP_RECON_SALE_FILTERED 13
/**********************************************************************/
#define GENREP_RECON_PAYIN01 101
#define GENREP_RECON_PAYIN02 102
#define GENREP_RECON_PAYIN03 103
#define GENREP_RECON_PAYIN04 104
#define GENREP_RECON_PAYIN05 105
#define GENREP_RECON_PAYIN06 106
#define GENREP_RECON_PAYIN07 107
#define GENREP_RECON_PAYIN08 108
#define GENREP_RECON_PAYIN09 109
#define GENREP_RECON_PAYIN10 110
#define GENREP_RECON_PAYIN11 111
#define GENREP_RECON_PAYIN12 112
#define GENREP_RECON_PAYIN13 113
#define GENREP_RECON_PAYIN14 114
#define GENREP_RECON_PAYIN15 115
#define GENREP_RECON_PAYIN16 116
#define GENREP_RECON_PAYIN17 117
#define GENREP_RECON_PAYIN18 118
#define GENREP_RECON_PAYIN19 119
#define GENREP_RECON_PAYIN20 120
#define GENREP_RECON_PAYINXX 121
/**********************************************************************/
#define GENREP_RECON_CHARGE_PAID 201
#define GENREP_RECON_REFUND_DEPOSIT 202
#define GENREP_RECON_CHARGE_DEPOSIT 203
#define GENREP_RECON_CHARGE_CUSTOMER 204
#define GENREP_RECON_CHARGE_ROOM 205
#define GENREP_RECON_CHARGE_LOYALTY 206
#define GENREP_RECON_CHARGE_SMARTPAY 207
#define GENREP_RECON_CHARGE_SMARTPHONE 208
#define GENREP_RECON_CHARGE_SPTBOOK 209
/**********************************************************************/
#define GENREP_RECON_PAYOUT01 301
#define GENREP_RECON_PAYOUT02 302
#define GENREP_RECON_PAYOUT03 303
#define GENREP_RECON_PAYOUT04 304
#define GENREP_RECON_PAYOUT05 305
#define GENREP_RECON_PAYOUT06 306
#define GENREP_RECON_PAYOUT07 307
#define GENREP_RECON_PAYOUT08 308
#define GENREP_RECON_PAYOUT09 309
#define GENREP_RECON_PAYOUT10 310
#define GENREP_RECON_PAYOUT11 311
#define GENREP_RECON_PAYOUT12 312
#define GENREP_RECON_PAYOUT13 313
#define GENREP_RECON_PAYOUT14 314
#define GENREP_RECON_PAYOUT15 315
#define GENREP_RECON_PAYOUT16 316
#define GENREP_RECON_PAYOUT17 317
#define GENREP_RECON_PAYOUT18 318
#define GENREP_RECON_PAYOUT19 319
#define GENREP_RECON_PAYOUT20 320
#define GENREP_RECON_PAYOUTXX 321
/**********************************************************************/
#define GENREP_RECON_HOLTS_PROMO 401
/**********************************************************************/
#define GENREP_RECON_BALANCE 501
/**********************************************************************/

struct CEposReportConsolGeneralMisc
{
public:
	CEposReportConsolGeneralMisc();
	void Reset();

public:
	int Compare ( CEposReportConsolGeneralMisc& source, int nHint = 0 );
	void Add	( CEposReportConsolGeneralMisc& source );
	
public:
	int m_nLineType;
	int m_nLineData;
	
public:
	double m_dQty[8];
	double m_dValue[8];
};

/**********************************************************************/

struct CEposReportConsolGeneralVariance
{
public:
	CEposReportConsolGeneralVariance();
	void Reset();

public:
	int Compare ( CEposReportConsolGeneralVariance& source, int nHint = 0 );
	void Add	( CEposReportConsolGeneralVariance& source );
	
public:
	bool m_bTotal;
	int m_nMethodNo;
	CString m_strDescription;
	
public:
	double m_dValue[8];
};

/**********************************************************************/

struct CEposReportConsolGeneralTaxPlu
{
public:
	CEposReportConsolGeneralTaxPlu();
	void Reset();

public:
	int Compare ( CEposReportConsolGeneralTaxPlu& source, int nHint = 0 );
	void Add	( CEposReportConsolGeneralTaxPlu& source );
	
public:
	int m_nTaxDateCode;
	__int64 m_nPluNo;
	int m_nModifier;
	
public:
	double m_dQty[8];
	double m_dValue[8];
};

/**********************************************************************/

struct CEposReportConsolGeneralTaxBand
{
public:
	CEposReportConsolGeneralTaxBand();
	void Reset();

public:
	int Compare ( CEposReportConsolGeneralTaxBand& source, int nHint = 0 );
	void Add	( CEposReportConsolGeneralTaxBand& source );
	
public:
	int m_nTaxBand;
	int m_nTaxDateCode;
	
public:
	double m_dQty[8];
	double m_dValue[8];
};

/**********************************************************************/

struct CEposReportGeneralBlock
{
public:
	CEposReportGeneralBlock();

public:
	bool GotData();
	__int64 GetLineCount( CEposReportCustomSettingsGeneral& Settings );

public:
	int m_nEntityType;
	int m_nDbIdx;
	int m_nLocIdx;
	int m_nRpIdx;
	int m_nTermIdx;
	int m_nOriginalTaxBandSize;
	CEntityTreeGeneralSales* m_pSales;
	CEntityTreeGeneralSales* m_pPromoDetail;
	CReportConsolidationArray<CEposReportConsolGeneralTaxBand>* m_pTaxByBand;
	CReportConsolidationArray<CEposReportConsolGeneralTaxPlu>* m_pTaxByPlu;
	CReportConsolidationArray<CEposReportConsolGeneralMisc>* m_pPayment;
	CReportConsolidationArray<CEposReportConsolGeneralVariance>* m_pVariance;
	CReportConsolidationArray<CEposReportConsolGeneralVariance>* m_pVarianceForEx;
	CReportConsolidationArray<CEposReportConsolGeneralMisc>* m_pPromoSummary;
	CReportConsolidationArray<CEposReportConsolGeneralMisc>* m_pVoid;
	CReportConsolidationArray<CEposReportConsolGeneralMisc>* m_pNoSale;
	CReportConsolidationArray<CEposReportConsolGeneralMisc>* m_pRecon;
};

/**********************************************************************/

struct CDeptGroupLinks
{
	int m_nReportGroup;
	int m_nConsolGroup;
};

/**********************************************************************/

struct CGeneralReportSalesBlockEntity
{
public:
	CGeneralReportSalesBlockEntity();

public:
	int m_nEntityType;
	int m_nEntityNum;
	CString m_strHeader;
	CString m_strMixMatchType;
	CString m_strTotal;
};

/**********************************************************************/

class CEposReportGeneral : public CEposReport
{
public:
	CEposReportGeneral( CEposSelectArray& SelectArray, int nReportType );
	CEposReportGeneral( CEposSelectArray& SelectArray, int nReportType, const char* szCustomParams );
	~CEposReportGeneral();

public:
	void Consolidate();

protected:
	void ConsolidateTransactions();
	void ConsolidateSales();

protected:
	void ConsolidateItemSale( int nWeekday, CArray<int,int>& arrayGroupOrDept, __int64 nPluNo, int nModifier, int nTaxDateCode, int nTaxBand, double dQty, double dAmount );
	void ConsolidateItemPromo( int nWeekday, CArray<int,int>& arrayGroupOrDept, int nPromoNo, int nLineType, double dAmount );
	void ConsolidateSpecialItem( int nWeekday, int nType, double dQty, double dAmount, bool bRefund );
	virtual void ConsolidatePaymentBuffer( int nWeekday );
	void ConsolidatePayment( int nWeekday, int nType, double dQty, double dValue );
	void ConsolidateVariance( int nWeekday, bool bFX, bool bTotal, int nMethodNo, const char* szDescription, double dValue );
	void ConsolidateVoid( int nWeekday, int nType, int nTotalType, double dQty, double dValue );
	void ConsolidateNoSale( CTermFileListInfo& infoFile, CCSVEposTermLine& csvIn );
	void ConsolidateReconcile( int nWeekday, int nType, double dQty, double dValue );

public:
	virtual bool CreateReport();

protected:
	void CreateSalesSection( CEposReportGeneralBlock& Block, bool bPromo );
	void ProcessSalesTreeBranch( int nDepth, int nBranchIdx, CEntityTreeGeneralSalesNode& BlockTotalNode, CGeneralReportSalesBlockEntity& BlockEntity );
	void CreatePaymentSection( CEposReportGeneralBlock& Block );
	void CreateVarianceSection( CEposReportGeneralBlock& Block, bool bForEx );
	void CreateTaxSection( CEposReportGeneralBlock& Block, bool bTaxAmount );
	void CreatePromoSection( CEposReportGeneralBlock& Block, CString& strHeader );
	void CreateVoidSection( CEposReportGeneralBlock& Block );
	void CreateNoSaleSection( CEposReportGeneralBlock& Block );
	void CreateReconciliationSection( CEposReportGeneralBlock& Block, CString& strHeader );
	
protected:
	virtual void GetConsolidationParams( CEposReportConsolParamsStandard& Params );
	void InitialiseHelperStrings();
	void InitialiseConsolidationBlocks();
	bool GetWeekday( int nLocIdx, CString& strTranDate, CString& strTranTime, int& nWeekday );

protected:
	void ResetDeptGroupLinks();
	int GetReportGroupForDeptNo( int nDeptNo );
	int GetConsolGroupForDeptNo( int nDeptNo );
	void GetGroupNumberAndName( CGeneralReportSalesBlockEntity& BlockEntity );
	
protected:
	void AppendLineTotals( CCSV& csvOut, double qQty[8], double dVal[8], bool bHideQty, bool bShowFractionQty, bool bHideVal, bool bNoSale = FALSE );
	void AppendValue( CCSV& csvOut, double dValue, bool bHideVal );
	void AppendQty( CCSV& csvOut, double dQty, bool bHideQty, bool bShowFraction );

protected:
	CStringArray m_arrayPaymentBuffer;
	CStringArray m_arrayPaidIOBuffer;

protected:
	CString m_strWeekDays[7];
	CString m_strUnderline;
	CString m_strReconUnderline;
	
protected:
	CBusinessDateInfo m_BusinessDateInfo;
	CArray<CDeptGroupLinks,CDeptGroupLinks> m_arrayDeptGroupLinks;
	CArray<int,int> m_arraySalesLevels;
	CArray<int,int> m_arrayPromoLevels;
	CEposReportCustomSettingsGeneral m_ReportSettings;
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportGeneralBlock,CEposReportGeneralBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;
	bool m_bCanConsolidateGlobally;
	
protected:
	bool m_bCreatingPromoSection;
	CEntityTreeGeneralSales* m_pSalesOrPromoSection;
	CArray<int,int>* m_pSalesOrPromoLevels;
	CArray<CGeneralReportSalesBlockEntity,CGeneralReportSalesBlockEntity> m_arraySalesHeaders;
	CArray<CGeneralReportSalesBlockEntity,CGeneralReportSalesBlockEntity> m_arraySalesTotals;
	CReportPluInfoFinder m_PluInfoFinder;
};

/**********************************************************************/
