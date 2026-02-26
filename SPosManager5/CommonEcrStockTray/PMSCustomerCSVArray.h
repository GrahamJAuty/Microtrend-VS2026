#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "MaxLengths.h"
/**********************************************************************/
#define PMS_CUSTOMER_VERSION 2
/**********************************************************************/
	
namespace PMSCustomer
{
	const recInt		CustomerId =	{ "CustomerId", 1, 999999999 };
	const recString		GuestName =		{ "GuestName", 0, MAX_LENGTH_ADDRESS_NAME };
	const recString		BillingName =	{ "BillingName", 0, MAX_LENGTH_ADDRESS_NAME };
	const recString		Address =		{ "Address", 0, MAX_LENGTH_ADDRESS_STREET };
	const recString		Postcode =		{ "Postcode", 0, MAX_LENGTH_ADDRESS_POSTCODE };
	const recString		Phone =			{ "Phone", 0, MAX_LENGTH_ADDRESS_PHONE };
	const recString		Fax =			{ "Fax", 0, MAX_LENGTH_ADDRESS_FAX };
	const recString		Email =			{ "Email", 0, MAX_LENGTH_ADDRESS_EMAIL };
};

/**********************************************************************/

class CPMSCustomerCSVRecord : public CCSVRecord
{
public:
	CPMSCustomerCSVRecord(); 
	virtual ~CPMSCustomerCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
public:
	int GetCustomerId()				{ return m_nCustomerId; }
	const char* GetGuestName()		{ return m_strGuestName; }
	const char* GetBillingName()	{ return m_strBillingName; }
	const char* GetAddress1()		{ return m_strAddress1; }
	const char* GetAddress2()		{ return m_strAddress2; }
	const char* GetAddress3()		{ return m_strAddress3; }
	const char* GetPostcode()		{ return m_strPostcode; }
	const char* GetPhone()			{ return m_strPhone; }
	const char* GetFax()			{ return m_strFax; }
	const char* GetEmail()			{ return m_strEmail; }

	void SetCustomerId( int n );
	void SetGuestName( const char* sz );
	void SetBillingName( const char* sz );
	void SetAddress1( const char* sz );
	void SetAddress2( const char* sz );
	void SetAddress3( const char* sz );
	void SetPostcode( const char* sz );
	void SetPhone( const char* sz );
	void SetFax( const char* sz );
	void SetEmail( const char* sz );

public:
	void ClearRecord();
	
private:
	void V1ConvertFromCSV( CCSV& csv );
	void V2ConvertFromCSV( CCSV& csv );
	
private:
	int m_nCustomerId;
	CString m_strGuestName;
	CString m_strBillingName;
	CString m_strAddress1;
	CString m_strAddress2;
	CString m_strAddress3;
	CString m_strPostcode;
	CString m_strPhone;
	CString m_strFax;
	CString m_strEmail;
};

/**********************************************************************/

class CPMSCustomerCSVArray : public CSharedCSVArray
{
public:
	CPMSCustomerCSVArray();
	~CPMSCustomerCSVArray();
	int Open( const char* szFilename, int nMode );

public:
	void CreateCustomerBackup();
	void RebuildFromCustomerFiles();
	void ValidateCustomerDatabase();
	void ReadAndUpdateCustomerDatabase();

public:
	static void InsertAndSaveCustomer( CPMSCustomerCSVRecord& CustomerRecord );
	static void RemoveAndSaveCustomer( int nCustomerId );

public:
	int GetCustomerId( int nIdx );
	bool FindCustomerById( int nCustomerId, int& nCustomerIdx );
	int FindFirstFreeId();
};

/**********************************************************************/
#endif
/**********************************************************************/
