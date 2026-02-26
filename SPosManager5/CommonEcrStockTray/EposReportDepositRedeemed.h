#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CEposReportConsolDepositRedeemed
{
public:
	CEposReportConsolDepositRedeemed();

public:
	void Reset();
	int Compare ( CEposReportConsolDepositRedeemed& source, int nHint = 0 );
	void Add	( CEposReportConsolDepositRedeemed& source );

public:
	bool CompareDeposit( CEposReportConsolDepositRedeemed& source );

public:
	int m_nLocSort;
	CString m_strDate;
	CString m_strTime;
	int m_nDepositID;

public:
	double m_dTender;
	double m_dAmount;
};

/**********************************************************************/

class CEposReportDepositRedeemed : public CEposReport
{
public:
	CEposReportDepositRedeemed( int nSpecialItemType, CEposSelectArray& SelectArray );

	virtual void Consolidate();
	virtual bool CreateReport();

	void WriteTotal( double dTotalTender, double dTotalAmount, int nCustomerColumns );
	
private:
	int m_nSpecialItemType;
	CReportConsolidationArray<CEposReportConsolDepositRedeemed> m_ConsolArray;
};

/**********************************************************************/
