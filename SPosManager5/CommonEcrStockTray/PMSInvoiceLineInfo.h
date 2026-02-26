#pragma once
/**********************************************************************/
#include "PMSDefines.h"
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/

struct CPMSInvoiceLineInfo
{
public:
	CPMSInvoiceLineInfo();

public:
	bool IsBlankItemLine();

public:
	int m_nSection;
	COleDateTime m_dateSale;
	int m_nTransactionNo;
	int m_nLineType;
	__int64 m_nBasePluNo;
	int m_nDeptNo;
	int m_nGroupNo;
	int m_nTaxBand;
	int m_nModifier;
	int m_nLevel;
	CString m_strDescription;
	double m_dEposPluQty;
	double m_dBasePluQty;
	double m_dValue;
	double m_dDiscount;
	double m_dAllowance;
};

/**********************************************************************/
#endif
/**********************************************************************/
