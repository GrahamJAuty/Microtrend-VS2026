#pragma once
/**********************************************************************/
#define VOID_SERVER_SALES_BEFORE_LINE 1
#define VOID_SERVER_SALES_AFTER_LINE 2
#define VOID_SERVER_SALES_BEFORE_TOTAL 3
#define VOID_SERVER_SALES_AFTER_TOTAL 4
#define VOID_SERVER_SALES_COMBINED_TOTAL 5
/**********************************************************************/
#define VOID_SERVER_DISCOUNT_BEFORE_LINE 6
#define VOID_SERVER_DISCOUNT_AFTER_LINE 7
#define VOID_SERVER_DISCOUNT_BEFORE_TOTAL 8
#define VOID_SERVER_DISCOUNT_AFTER_TOTAL 9
#define VOID_SERVER_DISCOUNT_COMBINED_TOTAL 10
/**********************************************************************/
#define VOID_SERVER_REFUND_BEFORE_LINE 11
#define VOID_SERVER_REFUND_AFTER_LINE 12
#define VOID_SERVER_REFUND_BEFORE_TOTAL 13
#define VOID_SERVER_REFUND_AFTER_TOTAL 14
#define VOID_SERVER_REFUND_COMBINED_TOTAL 15
/**********************************************************************/
#define VOID_TOTAL_SALES_BEFORE 16
#define VOID_TOTAL_SALES_AFTER 17
#define VOID_TOTAL_SALES_COMBINED 18
/**********************************************************************/
#define VOID_TOTAL_DISCOUNT_BEFORE 19
#define VOID_TOTAL_DISCOUNT_AFTER 20
#define VOID_TOTAL_DISCOUNT_COMBINED 21
/**********************************************************************/
#define VOID_TOTAL_REFUND_BEFORE 22
#define VOID_TOTAL_REFUND_AFTER 23
#define VOID_TOTAL_REFUND_COMBINED 24
/**********************************************************************/

struct CEposReportConsolVoidReason
{
public:
	CEposReportConsolVoidReason();
	void Reset();
	
public:
	int Compare ( CEposReportConsolVoidReason& source, int nHint = 0 );
	void Add	( CEposReportConsolVoidReason& source );

public:
	bool CompareServer( CEposReportConsolVoidReason& source );
	
public:
	bool m_bGrandTotal;
	int m_nServerLocOrDbSort;
	int m_nServerLocOrDbIdx;
	int m_nServerNo;
	int m_nBlock;
	CString m_strReportDate;
	int m_nTableNo;
	int m_nSeqNo;
	CString m_strReportTime;
	CString m_strOtherTime;
	int m_nLineNo;
	
public:
	//ITEM VOID ONLY
	__int64 m_nPluNo;
	int m_nModifier;
	int m_nDeptNo;
	CString m_strPluText;
	double m_dModQty;
	double m_dPluQty;
	CString m_strReason;
	bool m_bWaste;

public:
	//DISCOUNT VOID ONLY
	CString m_strDiscountType;
	CString m_strPromo;
	CString m_strRate;

public:
	//ITEM OR DISCOUNT VOID
	double m_dValue;
};

/**********************************************************************/
