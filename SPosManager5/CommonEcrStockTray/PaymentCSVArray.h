#pragma once
/**********************************************************************/
#include "MinimumSPOSVersion.h"
#include "NamespacePayment.h"
/**********************************************************************/
#define PAYMENT_VERSION 12
/**********************************************************************/
#define PAYMENT_TYPE_CASH 0
#define PAYMENT_TYPE_CHEQUE 1
#define PAYMENT_TYPE_EFT 2
#define PAYMENT_TYPE_EFT_CNP 3
#define PAYMENT_TYPE_ROOM_AUTO 4
#define PAYMENT_TYPE_ROOM_MANUAL 5
#define PAYMENT_TYPE_LOYALTY 6
#define PAYMENT_TYPE_CURRENCY 7
#define PAYMENT_TYPE_DEPOSIT 8
#define PAYMENT_TYPE_EFT_SHARE 9
#define PAYMENT_TYPE_ACCOUNT 10
#define PAYMENT_TYPE_CREDITNOTE 11
#define PAYMENT_TYPE_GIFTCARD 12
#define PAYMENT_TYPE_EXTERNAL_APP 13
#define PAYMENT_TYPE_EXTERNAL_PAY 15
#define PAYMENT_TYPE_VOUCHERCART 16
#define PAYMENT_TYPE_OTHER 99
/**********************************************************************/
	
class CPaymentCSVRecord : public CCSVRecord
{
public:
	CPaymentCSVRecord(); 
	virtual ~CPaymentCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	void CheckMinimumSPOSVersion( CMinimumSPOSVersion& version );
	
public:
	int GetPaymentNo()				{ return m_nPaymentNo; }
	const char* GetEPOSName()		{ return m_strEPOSName; }
	const char* GetReportName()		{ return m_strReportName; }
	int GetPaymentType()			{ return m_nPaymentType; }
	int GetReportGroup()			{ return m_nReportGroup; }
	double GetLimit()				{ return m_dLimit; }
	bool GetTender()				{ return m_bTender; }
	bool GetOverTender()			{ return m_bOverTender; }
	bool GetChange()				{ return m_bChange; }
	bool GetGratuity()				{ return m_bGratuity; }
	bool GetDrawer()				{ return m_bDrawer; }
	bool GetEFTCashback()			{ return m_bEFTCashback; }
	bool GetNonBnk()				{ return m_bNonBnk; }
	bool GetReceipt()				{ return m_bReceipt; }
	bool GetAccountAllow()			{ return m_bAccountAllow; }
	bool GetCashDeclarationFlag()	{ return m_bCashDeclaration; }
	bool GetHandheld()				{ return m_bHandheld; }
	bool GetCashLimitFlag()			{ return m_bCashLimit; }
	bool GetChangeAsCashbackFlag()	{ return m_bChangeAsCashback; }
	int GetFunctionNo()				{ return m_nFunctionNo; }
	int GetForexNo()				{ return m_nForexNo; }
	int GetDepositChangeType()		{ return m_nDepositChangeType; }
	int GetDepositChangeVoucherMin(){ return m_nDepositChangeVoucherMin; }
	const char* GetExternalParams()	{ return m_strExternalParams; }
	
	const char* GetDisplayName();
	const char* GetDisplayType();

	void SetPaymentNo( int n );
	void SetEPOSName( const char* szName );
	void SetReportName( const char* szName );
	void SetPaymentType( int n );
	void SetReportGroup( int n );
	void SetLimit( double d );
	
	void SetTender( bool b )				{ m_bTender = b; }
	void SetOverTender( bool b )			{ m_bOverTender = b; }
	void SetChange( bool b )				{ m_bChange = b; }
	void SetGratuity( bool b )				{ m_bGratuity = b; }
	void SetDrawer( bool b )				{ m_bDrawer = b; }
	void SetEFTCashback( bool b )			{ m_bEFTCashback = b; }
	void SetNonBnk( bool b )				{ m_bNonBnk = b; }
	void SetReceipt( bool b )				{ m_bReceipt = b; }
	void SetAccountAllow( bool b )			{ m_bAccountAllow = b; }
	void SetCashDeclarationFlag( bool b )	{ m_bCashDeclaration = b; }
	void SetHandheld( bool b )				{ m_bHandheld = b; }
	void SetCashLimitFlag( bool b )			{ m_bCashLimit = b; }
	void SetChangeAsCashbackFlag( bool b )	{ m_bChangeAsCashback = b; }
	void SetFunctionNo( int n );
	void SetForexNo( int n );
	void SetDepositChangeType( int n );
	void SetDepositChangeVoucherMin( int n );
	void SetExternalParams( const char* sz );
	
public:
	void ImportSQLText( const char* szText );
	const char* GetSQLText();
	void GetPaymentTexts( CStringArray& arrayTexts );
	void SetPaymentTexts( CStringArray& arrayTexts );
	
private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	void V2ConvertFromCSV( CCSV& csv );
	void V3ConvertFromCSV( CCSV& csv );
	void V4To12ConvertFromCSV( CCSV& csv, int nVer );

private:
	int m_nPaymentNo;
	CString m_strEPOSName;
	CString m_strReportName;
	int m_nPaymentType;
	int m_nReportGroup;
	double m_dLimit;
	bool m_bTender;
	bool m_bOverTender;
	bool m_bChange;
	bool m_bGratuity;
	bool m_bDrawer;
	bool m_bEFTCashback;
	bool m_bNonBnk;
	bool m_bReceipt;
	bool m_bAccountAllow;
	bool m_bCashDeclaration;
	bool m_bHandheld;
	bool m_bCashLimit;
	bool m_bChangeAsCashback;
	int m_nFunctionNo;
	int m_nForexNo;
	int m_nDepositChangeType;
	int m_nDepositChangeVoucherMin;
	CString m_strExternalParams;

	CStringArray m_arrayPaymentTexts;
	CString m_strSQLText;
	CString m_strDisplayName;
	CString m_strDisplayType;
};

/**********************************************************************/

class CPaymentCSVArray : public CSharedCSVArray
{
public:
	CPaymentCSVArray();
	~CPaymentCSVArray();
	int Open( const char* szFilename, int nMode );
	void WriteSMPaymentFile();
	void WriteSMPaymentGroupFile();

public:
	bool FindPaymentByNumber( int nPaymentNo, int& nPaymentIdx );
	int FindFirstFreeNumber();

public:
	bool IsPMSPaymentType( int nPaymentNo );

public:
	const char* GetDisplayName( int nPayment );

private:
	void SetPMSPaymentFlags();
	
private:
	CString m_strDisplayName;
	CArray<bool,bool> m_arrayPMSPaymentFlag;
};

/**********************************************************************/
