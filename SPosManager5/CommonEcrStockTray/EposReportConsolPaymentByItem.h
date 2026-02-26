#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
/**********************************************************************/
#define PBI_MAINBLOCK_LINE_ITEM 0
#define PBI_MAINBLOCK_LINE_EXCEPTION 1
#define PBI_MAINBLOCK_TOTAL_ITEM 2
#define PBI_MAINBLOCK_TOTAL_EXCEPTION 3
#define PBI_MAINBLOCK_TOTAL_ALL 4
/**********************************************************************/

struct CEposReportConsolPBI_MainBlock
{
public:
	CEposReportConsolPBI_MainBlock();

public:
	void Reset();
	int Compare ( CEposReportConsolPBI_MainBlock& source, int nHint = 0 );
	void Add	( CEposReportConsolPBI_MainBlock& source );

public:
	void CountAsTran(bool b) { m_nTranQty = b ? 1 : 0; }
	
public:
	int m_nPaymentNo;
	int m_nPaymentBlock;
	bool m_bIsZeroItem;

public:
	int m_nTranQty;
	__int64 m_nItemNumber;

public:
	double m_dVal;
};

/**********************************************************************/

struct CPaymentByItemBufferPayment
{
public:
	CPaymentByItemBufferPayment();

public:
	void Reset();
	int Compare ( CPaymentByItemBufferPayment& source, int nHint = 0 );
	void Add	( CPaymentByItemBufferPayment& source );
	
public:
	int m_nPaymentNo;
	bool m_bIsRedemptionBlock;
	
public:
	double m_dVal;
};

/**********************************************************************/

struct CPaymentByItemBufferSale
{
public:
	CPaymentByItemBufferSale();

public:
	void Reset();
	int Compare ( CPaymentByItemBufferSale& source, int nHint = 0 );
	void Add	( CPaymentByItemBufferSale& source );
	
public:
	__int64 m_nPluNo;
	int m_nAuxNo;
	
public:
	double m_dVal;
};

/**********************************************************************/

struct CEposReportPaymentByItemBlock : public CEposReportBlockMapEntity
{
public:
	bool GotData();
	__int64 GetLineCount();

public:
	CReportConsolidationArray<CEposReportConsolPBI_MainBlock>* m_pPaymentsNormal;
	CReportConsolidationArray<CEposReportConsolPBI_MainBlock>* m_pTotalsNormal;
	CReportConsolidationArray<CEposReportConsolPBI_MainBlock>* m_pPaymentsCharge;
	CReportConsolidationArray<CEposReportConsolPBI_MainBlock>* m_pTotalsCharge;
};

/**********************************************************************/
