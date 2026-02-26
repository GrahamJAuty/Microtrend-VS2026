#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
/**********************************************************************/
#define PBT_TOTALBLOCK_PAYMENT 0
#define PBT_TOTALBLOCK_TAXBAND 998
#define PBT_TOTALBLOCK_ALL 999
/**********************************************************************/

struct CEposReportConsolPBT_MainBlock
{
public:
	CEposReportConsolPBT_MainBlock();

public:
	void Reset();
	int Compare ( CEposReportConsolPBT_MainBlock& source, int nHint = 0 );
	void Add	( CEposReportConsolPBT_MainBlock& source );
	
public:
	int m_nPaymentNo;
	bool m_bTotal;
	bool m_bPushDown;
	int m_nTaxBand;

public:
	double m_dVal;
	double m_dTax;
};

/**********************************************************************/

struct CEposReportConsolPBT_TotalBlock
{
public:
	CEposReportConsolPBT_TotalBlock();

public:
	void Reset();
	int Compare ( CEposReportConsolPBT_TotalBlock& source, int nHint = 0 );
	void Add	( CEposReportConsolPBT_TotalBlock& source );
	
public:
	int m_nBlockType;
	int m_nTaxBand;

public:
	double m_dVal;
	double m_dTax;
};

/**********************************************************************/

struct CPaymentByTaxBandBufferPayment
{
public:
	CPaymentByTaxBandBufferPayment();

public:
	void Reset();
	int Compare ( CPaymentByTaxBandBufferPayment& source, int nHint = 0 );
	void Add	( CPaymentByTaxBandBufferPayment& source );
	
public:
	int m_nPaymentNo;
	
public:
	double m_dVal;
};

/**********************************************************************/

struct CPaymentByTaxBandBufferSale
{
public:
	CPaymentByTaxBandBufferSale();

public:
	void Reset();
	int Compare ( CPaymentByTaxBandBufferSale& source, int nHint = 0 );
	void Add	( CPaymentByTaxBandBufferSale& source );
	
public:
	int m_nTaxBand;
	bool m_bFiltered;
	
public:
	double m_dVal;
	double m_dTax;
};

/**********************************************************************/

struct CEposReportPaymentByTaxBandBlock : public CEposReportBlockMapEntity
{
public:
	bool GotData();
	__int64 GetLineCount();

public:
	CReportConsolidationArray<CEposReportConsolPBT_MainBlock>* m_pPayments;
	CReportConsolidationArray<CEposReportConsolPBT_TotalBlock>* m_pTotals;
};

/**********************************************************************/
