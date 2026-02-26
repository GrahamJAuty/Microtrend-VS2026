#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CEposReportConsolDepositUnredeemed
{
public:
	CEposReportConsolDepositUnredeemed();

public:
	void Reset();
	int Compare ( CEposReportConsolDepositUnredeemed& source, int nHint = 0 );
	void Add	( CEposReportConsolDepositUnredeemed& source );

public:
	int m_nLocSort;
	CString m_strDate;
	CString m_strTime;
	int m_nDepositID;
	
public:
	double m_dVal;
};

/**********************************************************************/

struct CEposReportConsolDepositCharged
{
public:
	CEposReportConsolDepositCharged();

public:
	void Reset();
	int Compare ( CEposReportConsolDepositCharged& source, int nHint = 0 );
	void Add	( CEposReportConsolDepositCharged& source );

public:
	int m_nLocSort;
	int m_nDepositID;

public:
	int m_nSelectArrayIdx;
	CString m_strDate;
};

/**********************************************************************/

class CEposReportDepositUnredeemed : public CEposReport
{
public:
	CEposReportDepositUnredeemed( CEposSelectArray& SelectArray );

	virtual void Consolidate();
	virtual bool CreateReport();

	void WriteTotal( double dTotal );
	
private:
	CReportConsolidationArray<CEposReportConsolDepositUnredeemed> m_arrayDepositsByDate;
	CReportConsolidationArray<CEposReportConsolDepositCharged> m_arrayDepositsCharged;
};

/**********************************************************************/
