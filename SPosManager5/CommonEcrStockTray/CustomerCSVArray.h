#pragma once
/**********************************************************************/
#define CUSTOMER_VERSION 9
/**********************************************************************/
#define CUSTOMER_LINETYPE_HEADER 0
#define CUSTOMER_LINETYPE_CUSTOMER 1
/**********************************************************************/
#define CUSTOMER_DELETIONSTATUS_PROTECTED 0
#define CUSTOMER_DELETIONSTATUS_CANDELETE 1
#define CUSTOMER_DELETIONSTATUS_DELETENOW 2
#define CUSTOMER_DELETIONSTATUS_DELETED 3
/**********************************************************************/
#define CUSTOMER_SORT_ID_INC 0
#define CUSTOMER_SORT_ID_DEC 1
#define CUSTOMER_SORT_FIRSTNAME_INC 2
#define CUSTOMER_SORT_FIRSTNAME_DEC 3
#define CUSTOMER_SORT_LASTNAME_INC 4
#define CUSTOMER_SORT_LASTNAME_DEC 5
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

namespace Customer
{
	const recInt		CustomerID =		{ "CustomerID", 1, 99999999 };
	const recInt		LineType =			{ "LineType", 0, 1 };
	const recString		CustomerName1 =		{ "CustomerName1", 1, 40 };
	const recString		CustomerName2 =		{ "CustomerName2", 1, 40 };
	const recString		Address =			{ "Address", 1, 80 };
	const recString		Postcode =			{ "Postcode", 1, 10 };
	const recString		Email =				{ "Email", 1, 80 };
	const recString		Phone =				{ "Phone", 0, 25 };
	const recBool		EmailRedirect =		{ "EmailRedirect" };
	const recBool		SpecifyStartDay =	{ "SpecifyStartDay" };
	const recInt		StartDay =			{ "StartDay", 0, 99999 };
	const recInt		StartHour =			{ "StartHour", 0, 23 };
	const recInt		StartMinute =		{ "StartMinute", 0, 59 };
	const recDouble		StartBalance =		{ "StartBalance", -99999.99, 99999.99 };
	const recInt		DeletionStatus =	{ "DeletionStatus", 0, 3 };
	const recString		TempDate =			{ "TempDate", 0, 8 };
	const recInt		PosCreationDay =	{ "PosCreationDay", -5000, 99999 };
	const recInt		PosCreationTime =	{ "PosCreationTime", 0, 1439 };
	const recInt		RecordCreationDay =	{ "RecordCreationDay", 0, 99999 };
	const recInt		RecordCreationTime ={ "RecordCreationTime", -1, 1439 };
	const recBool		AutoDeleteFlag =	{ "AutoDeleteFlag" };
	const recInt		AutoDeleteDay =		{ "AutoDeleteDay", 0, 99999 };
};

/**********************************************************************/

struct CCustomerDeletionNote
{
public:
	CCustomerDeletionNote();
	void Reset();
	
public:
	int Compare ( CCustomerDeletionNote& source, int nHint = 0 );
	void Add	( CCustomerDeletionNote& source );
	
public:
	int m_nCustomerID;

public:
	CString m_strDate;
};

/**********************************************************************/

class CCustomerCSVRecord : public CCSVRecord
{
public:
	CCustomerCSVRecord(); 
	virtual ~CCustomerCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 

public:
	void ClearRecord();
	
public:
	int GetCustomerID()				{ return m_nCustomerID; }
	int GetLineType()				{ return m_nLineType; }
	const char* GetCustomerName1()	{ return m_strCustomerName1; }
	const char* GetCustomerName2()	{ return m_strCustomerName2; }
	const char* GetAddress1()		{ return m_strAddress1; }
	const char* GetAddress2()		{ return m_strAddress2; }
	const char* GetAddress3()		{ return m_strAddress3; }
	const char* GetPostcode()		{ return m_strPostcode; }
	const char* GetEmail()			{ return m_strEmail; }
	bool GetEmailRedirectFlag()		{ return m_bEmailRedirect; }
	const char* GetPhone1()			{ return m_strPhone1; }
	const char* GetPhone2()			{ return m_strPhone2; }
	const char* GetPhone3()			{ return m_strPhone3; }
	bool GetSpecifyStartDayFlag()	{ return m_bSpecifyStartDay; }
	int GetStartDay()				{ return m_bSpecifyStartDay ? m_nStartDay : 0; }
	int GetStartHour()				{ return m_bSpecifyStartDay ? m_nStartHour : 0; }
	int GetStartMinute()			{ return m_bSpecifyStartDay ? m_nStartMinute : 0; }
	double GetStartBalance()		{ return m_dStartBalance; }
	int GetDeletionStatus()			{ return m_nDeletionStatus; }
	const char* GetTempDate()		{ return m_strTempDate; }
	bool GetMoveToOtherArrayFlag()	{ return m_bMoveToOtherArray; }
	int GetPosCreationDay()			{ return m_nPosCreationDay; }
	int GetPosCreationTime()		{ return m_nPosCreationTime; }
	int GetRecordCreationDay()		{ return m_nRecordCreationDay; }
	int GetRecordCreationTime()		{ return m_nRecordCreationTime; }
	bool GetAutoDeleteFlag()		{ return m_bAutoDeleteFlag; }
	int GetAutoDeleteDay()			{ return m_nAutoDeleteDay; }

	const char* GetDisplayName1();
	const char* GetDisplayName2();
	const char* GetDisplayNameFull();
	const char* GetDisplayDateBalance();
	const char* GetDisplayPosDateCreated();
	const char* GetDisplayRecordDateCreated();
	const char* GetDisplayDateAutoDelete();

	void SetCustomerID( int n );
	void SetLineType( int n );
	void SetCustomerName1( const char* sz );
	void SetCustomerName2( const char* sz );
	void SetAddress1( const char* sz );
	void SetAddress2( const char* sz );
	void SetAddress3( const char* sz );
	void SetPostcode( const char* sz );
	void SetEmail( const char* sz );
	void SetEmailRedirectFlag( bool b );
	void SetSpecifyStartDayFlag( bool b );
	void SetPhone1( const char* sz );
	void SetPhone2( const char* sz );
	void SetPhone3( const char* sz );
	void SetStartDay( int n );
	void SetStartHour( int n );
	void SetStartMinute( int n );
	void SetStartBalance( double d );
	void SetPosCreationTime( double d );
	void SetPosCreationDay( int n );
	void SetPosCreationTime( int n );
	void SetRecordCreationDay( int n );
	void SetRecordCreationTime( int n );
	void SetAutoDeleteFlag( bool b ){ m_bAutoDeleteFlag = b; }
	void SetAutoDeleteDay( int n );

	void SetDeletionStatus( int n );
	void SetTempDate( const char* sz );
	void SetMoveToOtherArrayFlag( bool b );

	void InitCreateAndDeleteDates( bool bOneOff );

public:
	int GetLatestCreationDay(){ return max( m_nPosCreationDay, m_nRecordCreationDay ); }
	
private:
	void V1ConvertFromCSV( CCSV& csv );
	void V2ConvertFromCSV( CCSV& csv );
	void V3ConvertFromCSV( CCSV& csv );
	void V4To9ConvertFromCSV( CCSV& csv, int nVer );
	
private:
	int m_nCustomerID;
	int m_nLineType;
	CString m_strCustomerName1;
	CString m_strCustomerName2;
	CString m_strAddress1;
	CString m_strAddress2;
	CString m_strAddress3;
	CString m_strPhone1;
	CString m_strPhone2;
	CString m_strPhone3;
	CString m_strPostcode;
	CString m_strEmail;
	bool m_bEmailRedirect;
	bool m_bSpecifyStartDay;
	int m_nStartDay;
	int m_nStartHour;
	int m_nStartMinute;
	double m_dStartBalance;
	int m_nPosCreationDay;
	int m_nPosCreationTime;
	int m_nRecordCreationDay;
	int m_nRecordCreationTime;
	bool m_bAutoDeleteFlag;
	int m_nAutoDeleteDay;

	int m_nDeletionStatus;
	CString m_strTempDate;
	bool m_bMoveToOtherArray;

	CString m_strDisplayName1;
	CString m_strDisplayName2;
	CString m_strDisplayNameFull;
	CString m_strDisplayDateBalance;
	CString m_strDisplayPosDateCreated;
	CString m_strDisplayRecordDateCreated;
	CString m_strDisplayDateAutoDelete;
};

/**********************************************************************/

class CCustomerCSVArray : public CSharedCSVArray
{
public:
	CCustomerCSVArray();
	~CCustomerCSVArray();

	int Open( const char* szFilename, int nMode );

public:
	int GetCustomerID( int nIdx );
	bool FindCustomerByID( int nCustomerID, int& nCustomerIdx );
	
public:
	const char* GetDisplayName1( int nCustomerID );
	const char* GetDisplayName2( int nCustomerID );
	const char* GetDisplayNameFull( int nCustomerID );

public:
	void LoadHeaderStartDateRecord();
	void GetStartDateRecord( CCustomerCSVRecord& Record );
	void SetStartDateRecord( CCustomerCSVRecord& Record );
	void SaveHeaderStartDateRecord();

public:
	void CreateCustomerReport( bool bOneOff, int nLocIdx, COleDateTime oleDateFrom, COleDateTime oleDateTo, bool bSPosDate, bool bEcrmanDate, CReportFile& ReportFile );
	
public:
	void AddFromIDList( CArray<int,int>& arrayIDs, CCustomerCSVArray& arrayBlock, bool bOneOff );
	void Sync( CCustomerCSVArray& source, CCustomerCSVArray& block, bool bAdd, bool bRemove, int& nAdded, int& nRemoved, int& nFirstNewID, bool bAuto );
	void SetDeletionStatus( int nStatus );
	void ClearMoveToOtherArrayFlags();
	void MoveCustomers( CCustomerCSVArray& Destination );

public:
	bool IsRecentlyDeleted( int nCustomerID );
	void FlagDeletion( int nCustomerID, const char* szDate );
	void MoveDeletionsToHelperArray();
	void MoveDeletionsToMainArray();

private:
	CCustomerCSVRecord m_StartDateRecord;
	CString m_strDisplayName1;
	CString m_strDisplayName2;
	CString m_strDisplayNameFull;

private:
	CReportConsolidationArray<CCustomerDeletionNote> m_arrayRecentDeletions;
};

/**********************************************************************/
