#pragma once
/**********************************************************************/
 
/**********************************************************************/
#define DEPOSITNAME_VERSION 4
/**********************************************************************/
#define DEPOSITINFO_LINES 4
/**********************************************************************/

namespace DepositName
{
	const recInt		DepositID =					{ "DepositID", 1, 99999999 };
	const recInt		ServerNo =					{ "ServerNo", 0, 9999 };
	const recInt		DepositAmount =				{ "DepositAmount", 0, 999999 };
	const recString		TaxBand =					{ "TaxBand", 0, 1 };
	const recInt		TaxAmount =					{ "TaxAmount", 0, 999999 };
	const recString		BuyerName =					{ "BuyerName", 0, 80 };
	const recString		BookingDate =				{ "BookingDate", 0, 20 };
	const recString		BookingTime =				{ "BookingTime", 0, 20 };
	const recString		CustomerInfo =				{ "CustomerInfo", 0, 40 };
};

/**********************************************************************/

class CDepositNameCSVRecord : public CCSVRecord
{
public:
	CDepositNameCSVRecord(); 
	virtual ~CDepositNameCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
public:
	int GetDepositID()						{ return m_nDepositID; }
	int GetServerNo()						{ return m_nServerNo; }
	int GetDepositAmount()					{ return m_nDepositAmount; }
	const char* GetTaxBand()				{ return m_strTaxBand; }
	int GetTaxAmount()						{ return m_nTaxAmount; }
	const char* GetBuyerName()				{ return m_strBuyerName; }
	const char* GetBookingDate()			{ return m_strBookingDate; }
	const char* GetBookingTime()			{ return m_strBookingTime; }
	const char* GetCustomerInfo( int n );
	const char* GetCustomerInfoDisplayLine();

	void SetDepositID( int n );
	void SetServerNo( int n );
	void SetDepositAmount( int n );
	void SetTaxBand( const char* sz );
	void SetTaxAmount( int n );
	void SetBuyerName( const char* szName );
	void SetBookingDate( const char* szDate );
	void SetBookingTime( const char* szTime );
	void SetCustomerInfo( int n, const char* sz );
	
public:
	void ClearRecord();

private:
	void V1to4ConvertFromCSV( CCSV& csv, int nVer );
	
private:
	int m_nDepositID;
	int m_nServerNo;
	int m_nDepositAmount;
	CString m_strTaxBand;
	int m_nTaxAmount;
	CString m_strBuyerName;
	CString m_strBookingDate;
	CString m_strBookingTime;
	CString m_strCustomerInfo[ DEPOSITINFO_LINES ];

private:
	CString m_strCustomerInfoOut;
};

/**********************************************************************/

class CDepositNameCSVArray : public CSharedCSVArray
{
public:
	CDepositNameCSVArray();
	~CDepositNameCSVArray();
	int Open( const char* szFilename, int nMode );

public:
	bool FindDepositByID( int nDepositID, int& nDepositIdx );
	void GetDepositByID( int nDepositID, CDepositNameCSVRecord& DepositRecord );
	
public:
	const char* GetBuyerNameByDepositID( int nDepositID );

private:
	CString m_strBuyerName;
};

/**********************************************************************/
