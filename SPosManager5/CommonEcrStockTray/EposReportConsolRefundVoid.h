#pragma once
/**********************************************************************/
#define EPOSREPORT_VOIDTYPE_NONE 0
#define EPOSREPORT_VOIDTYPE_TRAN 1
#define EPOSREPORT_VOIDTYPE_ITEM_SALE 2
#define EPOSREPORT_VOIDTYPE_ITEM_REFUND 3
#define EPOSREPORT_VOIDTYPE_WASTE_SALE 4
#define EPOSREPORT_VOIDTYPE_WASTE_REFUND 5
#define EPOSREPORT_VOIDTYPE_DELIVERY_SALE 6
#define EPOSREPORT_VOIDTYPE_DELIVERY_REFUND 7
#define EPOSREPORT_VOIDTYPE_DISCOUNT 8
/**********************************************************************/

struct CEposReportConsolRefundVoid
{
public:
	CEposReportConsolRefundVoid();
	void Reset();
	
public:
	int Compare ( CEposReportConsolRefundVoid& source, int nHint = 0 );
	void Add	( CEposReportConsolRefundVoid& source );

public:
	bool CompareServer( CEposReportConsolRefundVoid& source );
	
public:
	int m_nServerLocOrDbSort;
	int m_nServerLocOrDbIdx;
	int m_nServerNo;
	int m_nBlock;
	CString m_strDate;
	CString m_strTime;
	int m_nLineVoidBAType;
	int m_nLineNo;
	
public:
	__int64 m_nPluNo;
	int m_nModifier;
	int m_nDeptNo;
	int m_nAuthServerNo;
	CString m_strText;
	double m_dModQty;
	double m_dPluQty;
	double m_dValue;
};

/**********************************************************************/
