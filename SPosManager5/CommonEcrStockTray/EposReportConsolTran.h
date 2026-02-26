#pragma once
/**********************************************************************/

struct CEposReportConsolTran
{
public:
	CEposReportConsolTran();

public:
	void Reset();
	int Compare ( CEposReportConsolTran& source, int nHint = 0 );
	void Add	( CEposReportConsolTran& source );
	
public:
	CString m_strDateTran;
	CString m_strTimeTran;
};

/**********************************************************************/

struct CEposReportConsolTranTimeCode
{
public:
	CEposReportConsolTranTimeCode();

public:
	void Reset();
	int Compare ( CEposReportConsolTranTimeCode& source, int nHint = 0 );
	void Add	( CEposReportConsolTranTimeCode& source );

public:
	bool TranCompare ( CEposReportConsolTranTimeCode& source );
	
public:
	CString m_strDateTran;
	CString m_strTimeTran;
	CString m_strTimeCode;
	int m_nDayShift;
};

/**********************************************************************/

struct CEposReportConsolTranDateOrTime
{
public:
	CEposReportConsolTranDateOrTime();

public:
	void Reset();
	int Compare ( CEposReportConsolTranDateOrTime& source, int nHint = 0 );
	void Add	( CEposReportConsolTranDateOrTime& source );

public:
	bool TranCompare ( CEposReportConsolTranDateOrTime& source );
	
public:
	CString m_strDateTran;
	CString m_strTimeTran;
	CString m_strDateOrTime;
};

/**********************************************************************/

struct CEposReportConsolTranBusinessDate
{
public:
	CEposReportConsolTranBusinessDate();

public:
	void Reset();
	int Compare ( CEposReportConsolTranBusinessDate& source, int nHint = 0 );
	void Add	( CEposReportConsolTranBusinessDate& source );

public:
	bool TranCompare ( CEposReportConsolTranBusinessDate& source );
	
public:
	CString m_strDateTran;
	CString m_strTimeTran;
	CString m_strBusinessDate;
};

/**********************************************************************/

struct CEposReportConsolTranPriceLevel
{
public:
	CEposReportConsolTranPriceLevel();

public:
	void Reset();
	int Compare ( CEposReportConsolTranPriceLevel& source, int nHint = 0 );
	void Add	( CEposReportConsolTranPriceLevel& source );

public:
	bool TranCompare ( CEposReportConsolTranPriceLevel& source );
	
public:
	CString m_strDateTran;
	CString m_strTimeTran;
	int m_nPriceLevel;
};

/**********************************************************************/

struct CEposReportConsolTranMonth
{
public:
	CEposReportConsolTranMonth();

public:
	void Reset();
	int Compare ( CEposReportConsolTranMonth& source, int nHint = 0 );
	void Add	( CEposReportConsolTranMonth& source );

public:
	bool TranCompare ( CEposReportConsolTranMonth& source );
	
public:
	CString m_strDateTran;
	CString m_strTimeTran;
	int m_nYear;
	int m_nMonth;
};

/**********************************************************************/

struct CEposReportConsolTranServer
{
public:
	CEposReportConsolTranServer();
	void Reset();

public:
	int Compare ( CEposReportConsolTranServer& source, int nHint = 0 );
	void Add	( CEposReportConsolTranServer& source );

public:
	bool TranCompare( CEposReportConsolTranServer& source );
	
public:
	CString m_strDateTran;
	CString m_strTimeTran;
	int m_nServerNo;
};

/**********************************************************************/

struct CEposReportConsolTranPromoItem
{
public:
	CEposReportConsolTranPromoItem();

public:
	void Reset();
	int Compare ( CEposReportConsolTranPromoItem& source, int nHint = 0 );
	void Add	( CEposReportConsolTranPromoItem& source );

public:
	bool TranCompare ( CEposReportConsolTranPromoItem& source );
	
public:
	CString m_strDateTran;
	CString m_strTimeTran;
	bool m_bHeader;
	int m_nMixMatchType;
	int m_nOfferNo;
	__int64 m_nItemNo;
};

/**********************************************************************/
