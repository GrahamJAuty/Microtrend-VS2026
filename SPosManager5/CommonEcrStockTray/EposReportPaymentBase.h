#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportBlockMap.h"
#include "EposReportConsolPaymentSummary.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportPaymentBase : public CEposReport
{
public:
	CEposReportPaymentBase( CEposSelectArray& SelectArray );
	~CEposReportPaymentBase();

	virtual void Consolidate();
	virtual bool CreateReport(){ return FALSE; }
	
protected:
	void ConsolidatePaymentBuffer( bool bGotDepositItem, bool bGotNormalItem, bool bGotCustomerRAItem, bool bGotRoomRAItem, bool bGotLoyaltyRAItem, bool bGotSmartPayRAItem, bool bGotSmartPhoneRAItem, bool bGotSptBookRA );
	void ConsolidatePayment( int nType, int nSubType, CEposReportConsolPaySum& infoConsol );
	void ConsolidatePaymentsForCashSummary( CReportConsolidationArray<CEposReportConsolPaySum>& source, CReportConsolidationArray<CEposReportConsolPaySum>& dest );
	
protected:
	void ConsolidatePaidIn( int nSubType, __int64 nValue );
	void ConsolidatePaidOut( int nSubType, __int64 nValue );
	void ConsolidateNet( int nSubType, __int64 nValue );
	void ConsolidateOneVal( int nType, int nSubType, __int64 nValue );

protected:
	void ConsolidatePaidInTotalCash( __int64 nValue );
	void ConsolidatePaidInTotalNonCash( __int64 nValue );
	void ConsolidatePaidOutTotalCash( __int64 nValue );
	void ConsolidatePaidOutTotalNonCash( __int64 nValue );
	void ConsolidatePayIOTotal( int nType, __int64 nValue );

protected:
	void ConsolidateDepositRA( double dValue );
	void ConsolidateDepositRefund( double dValue );
	void ConsolidateCustomerRA( double dValue );
	void ConsolidateRoomRA( double dValue );
	void ConsolidateLoyaltyRA( double dValue );
	void ConsolidateSmartPayRA( double dValue );
	void ConsolidateSmartPhoneRA( double dValue );
	void ConsolidateSptBookRA( double dValue );
	void ConsolidateRA( int nType, double dValue );
	
protected:
	void CreateSalesSection( CEposReportPaySumBlock& Block );

protected:
	void WritePaymentBlock( const char* szTitle, CReportConsolidationArray<CEposReportConsolPaySum>* pPayments, CEposReportConsolPaySum& Totals );
	virtual bool WritePaymentLines( CReportConsolidationArray<CEposReportConsolPaySum>* pPayments, CEposReportConsolPaySum& Totals, bool bShowNonPTR, bool bShowPTR, bool bConsolidate );
	virtual bool WriteNetLines( CReportConsolidationArray<CEposReportConsolPaySumOneVal>* pPayments, CEposReportConsolPaySumOneVal& Totals, bool bShowNonPTR, bool bShowPTR, bool bConsolidate );
	void WriteDepositSummary( double dTotalRA, double dTotalRefund, CEposReportConsolPaySum& PaymentTotals );
	void WriteCustomerSummary( double dTotalRA, CEposReportConsolPaySum& PaymentTotals );
	void WriteRoomSummary( double dTotalRA, CEposReportConsolPaySum& PaymentTotals );
	void WriteLoyaltySummary( double dTotalRA, CEposReportConsolPaySum& PaymentTotals );
	void WriteSmartPaySummary( double dTotalRA, CEposReportConsolPaySum& PaymentTotals );
	void WriteSmartPhoneSummary( double dTotalRA, CEposReportConsolPaySum& PaymentTotals );
	void WriteSptBookSummary( double dTotalRA, CEposReportConsolPaySum& PaymentTotals );
	void WriteRASummary( double dTotalRA, double dTotalRefund, CEposReportConsolPaySum& PaymentTotals );
	void WritePaidInOutSummary( CEposReportPaySumBlock& Block );
	void WriteCashSummary( CEposReportPaySumBlock& Block );
	bool WritePaidInOutDetail( bool bOut, CEposReportPaySumBlock& Block, double& dAmount );
	void PrepareTotalLine( CCSV& csv, int nNum, const char* szText, bool bShowQty, int nQty );
	void WriteTotals( const char* szType, CEposReportConsolPaySum& Totals );
	void WriteTotals( const char* szType, CEposReportConsolPaySumOneVal& Totals );

protected:
	virtual void InitialiseConsolidationBlocks(){}
	virtual void SetConsolidationServer( int nDbIdx, int nLocIdx, int nServerNo ){}

protected:
	void InitialiseConsolidationBlocksStageOne();
	void InitialiseConsolidationBlocksStageTwo();

protected:
	CStringArray m_arrayPaymentBuffer;
	CStringArray m_arrayPaidIOBuffer;

protected:
	CString m_strSimpleUnderline;
	CString m_strDetailUnderline;

protected:
	int m_nServerBlockIdxStart;
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportPaySumBlock,CEposReportPaySumBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;
};

/**********************************************************************/
