#pragma once
/**********************************************************************/
#define REP_ACNT_DEPOSIT 1
#define REP_ACNT_BOOKING_PURCHASE 2
#define REP_ACNT_BOOKING_EVENT 3
#define REP_ACNT_CUSTOMER_ID 4
#define REP_ACNT_CUSTOMER_NAME 5
/**********************************************************************/
#define NEWDEPOSIT_AVAILABLE 0
#define NEWDEPOSIT_REFUNDED 1
#define NEWDEPOSIT_REDEEMED 2
/**********************************************************************/

struct CEposReportConsolAccountReceive
{
public:
	CEposReportConsolAccountReceive();

public:
	void Reset();
	int Compare ( CEposReportConsolAccountReceive& source, int nHint = 0 );
	void Add	( CEposReportConsolAccountReceive& source );

public:
	bool CompareReceive( CEposReportConsolAccountReceive& source );

public:
	const char* GetDisplayEventDate();
	const char* GetDisplayEventTime();
	const char* GetDisplayPurchaseDate();
	const char* GetDisplayPurchaseTime();

public:
	int m_nLocSort;
	int m_nType;
	CString m_strEventDate;
	CString m_strEventTime;
	CString m_strPurchaseDate;
	CString m_strPurchaseTime;
	int m_nAccountID;
	int m_nPaymentType;
	int m_nNewDepositStatus;

public:
	double m_dVal;

private:
	CString m_strDisplayEventDate;
	CString m_strDisplayEventTime;
	CString m_strDisplayPurchaseDate;
	CString m_strDisplayPurchaseTime;
};

/**********************************************************************/

struct CEposReportConsolAccountPayment
{
public:
	CEposReportConsolAccountPayment();

public:
	void Reset();
	int Compare ( CEposReportConsolAccountPayment& source, int nHint = 0 );
	void Add	( CEposReportConsolAccountPayment& source );

public:
	bool CompareReceive( CEposReportConsolAccountPayment& source );

public:
	int m_nLineType;
	int m_nPaymentType;
	
public:
	double m_dVal;
};

/**********************************************************************/

struct CEposReportConsolAccountCharged
{
public:
	CEposReportConsolAccountCharged();

public:
	void Reset();
	int Compare ( CEposReportConsolAccountCharged& source, int nHint = 0 );
	void Add	( CEposReportConsolAccountCharged& source );

public:
	int m_nLocSort;
	int m_nAccountID;
	int m_nNewDepositStatus;
	bool m_bKeepMe;
};

/**********************************************************************/
