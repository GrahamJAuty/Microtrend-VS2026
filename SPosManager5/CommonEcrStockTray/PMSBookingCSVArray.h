#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSDefines.h"
#include "PMSInvoiceLineInfo.h"
#include "PMSStructs.h"
/**********************************************************************/
#define PMS_BOOKING_VERSION 10
/**********************************************************************/
#define PMS_BOOKING_STATUS_PENDING 0
#define PMS_BOOKING_STATUS_ACTIVE 1
#define PMS_BOOKING_STATUS_COMPLETE 2
#define PMS_BOOKING_STATUS_CANCELLED 3
#define PMS_BOOKING_STATUS_CLOSED 999
/**********************************************************************/
#define PMS_CHECKOUT_BALANCE_ZERO 0
#define PMS_CHECKOUT_BALANCE_DEBIT 1
#define PMS_CHECKOUT_BALANCE_CREDIT 2
#define PMS_CHECKOUT_BALANCE_MIXED 3
/**********************************************************************/
#define PMS_BOOKING_RATE_ROOM 0
#define PMS_BOOKING_RATE_BB 1
#define PMS_BOOKING_RATE_DBB 2
#define PMS_BOOKING_RATE_FREE 3
/**********************************************************************/
#define PMS_TARIFFTYPE_STANDARD 0
#define PMS_TARIFFTYPE_SINGLE 1
#define PMS_TARIFFTYPE_EXTRABED 2
#define PMS_TARIFFTYPE_EXTRACOT 3
/**********************************************************************/

namespace PMSBooking
{
	const recInt		BookingId =			{ "BookingId", 1, 99999999 };
	const recInt		BookingPart =		{ "BookingPart", 0, 9 };
	const recInt		CustomerId =		{ "CustomerId", 1, 99999999 };
	const recInt		StartDay =			{ "StartDay", 0, 99999 };
	const recInt		Nights =			{ "Nights", 0, 366 };
	const recInt		Guests =			{ "Guests", 1, 7 };
	const recInt		RoomSleeps =		{ "RoomSleeps", 0, 6 };
	const recInt		TariffType =		{ "TariffType", 0, 3 };
	const recString		BookingNote =		{ "BookingNote", 0, 50 };
	const recInt		InvoiceNum =		{ "InvoiceNum", 1, 99999999 };
	const recInt		DepositType =		{ "DepositType", 0, 50 };
	const recDouble		DepositValue =		{ "DepositValue", 0.0, 9999.99 };
	const recInt		DepositDay =		{ "DepositDay", 0, 99999 };
	const recInt		DepositHour =		{ "DepositHour", 0, 23 };
	const recInt		DepositMinute =		{ "DepositMinute", 0, 59 };
	const recDouble		DinnerAllowance =	{ "DinnerAllowance", 0.0, 100.0 };
};

/**********************************************************************/

struct CPMSBookingPartInfo
{
public:
	CPMSBookingPartInfo();

public:
	int GetRoomNo()				{ return m_nRoomNo; }
	int GetStartDay()			{ return m_nStartDay; }
	int GetNights()				{ return m_nNights; }
	
	void SetRoomNo				( int n );
	void SetStartDay			( int n );
	void SetNights				( int n );
	
private:
	int m_nRoomNo;
	int m_nStartDay;
	int m_nNights;
};

/**********************************************************************/

struct CPMSDepositInfo
{
public:
	CPMSDepositInfo();

public:
	int Compare( CPMSDepositInfo& infoOther );

public:
	int GetPMSDay()				{ return m_nPMSDay; }
	int GetHour()				{ return m_nHour; }
	int GetMinute()				{ return m_nMinute; }
	bool GetRefundFlag()		{ return m_bRefund; }
	int GetPaymentType()		{ return m_nPaymentType; }
	double GetValue()			{ return m_dValue; }
	int GetIndex()				{ return m_nIndex; }

public:
	void SetPMSDay				( int n );
	void SetHour				( int n );
	void SetMinute				( int n );
	void SetRefundFlag			( bool b );
	void SetPaymentType			( int n );
	void SetValue				( double d );
	void SetIndex				( int n );

private:
	int m_nPMSDay;
	int m_nHour;
	int m_nMinute;
	bool m_bRefund;
	int m_nPaymentType;
	double m_dValue;
	int m_nIndex;
};

/**********************************************************************/

class CPMSBookingCSVRecord : public CCSVRecord
{
public:
	CPMSBookingCSVRecord(); 
	virtual ~CPMSBookingCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv );

public:
	void ClearRecord( bool bClearDeposits );
	void RemoveEmptyParts();
	
public:
	//QUICK ACCESS FIELDS
	int GetBookingId()				{ return m_nBookingId; }
	int GetInternalStatus()			{ return m_nInternalStatus; }
	int GetBookingStatus()			{ return ( m_nInternalStatus % 100 ); }
	bool GetTrainingFlag()			{ return ( m_nInternalStatus >= 100 ); }
	int GetCheckoutBalanceType()	{ return m_nCheckoutBalanceType; }
	//BOOKING DETAIL
	int GetCustomerId()				{ return m_nCustomerId; }
	int GetGuests()					{ return m_nGuests; }
	int GetSearchStartDay()			{ return m_nSearchStartDay; }
	int GetSearchNights()			{ return m_nSearchNights; }
	int GetSearchRoomSleeps()		{ return m_nSearchRoomSleeps; }
	int GetSearchRoomSubType()		{ return m_nSearchRoomSubType; }
	int GetTariffType()				{ return m_nTariffType; }
	int GetRoomRateType()			{ return m_nRoomRateType; }
	double GetRoomRate()			{ return ( PMS_BOOKING_RATE_FREE != m_nRoomRateType ) ? m_dRoomRate : 0.0; }
	double GetExtraBedRate();
	int GetInvoiceNum()				{ return m_nInvoiceNum; }
	double GetDinnerAllowance()		{ return m_dDinnerAllowance; }
	const char* GetDinnerDepartments() { return m_strDinnerDepartments; }
	//SUB ACCOUNTS
	bool GetMultipleAccountsFlag()	{ return m_bMultipleAccounts; }
	int GetAccountCount()			{ return m_nAccountCount; }
	int GetAccountCovers( int n );
	//OTHER
	const char* GetBookingNote()	{ return m_strBookingNote; }
	int GetEmailStartDay()			{ return m_nEmailStartDay; }
	int GetEmailNights()			{ return m_nEmailNights; }

	double CalculateDepositValue();
	int SortDepositTransactions( int nTransactionToTrack );
	
	//QUICK ACCESS FIELDS
	void SetBookingId( int n );
	void SetInternalStatus( int n );
	void SetBookingStatus( int n );
	void SetTrainingFlag( bool b );
	void SetCheckoutBalanceType( int n );
	//BOOKING DETAILS
	void SetCustomerId( int n );
	void SetGuests( int n );
	void SetSearchStartDay( int n );
	void SetSearchNights( int n );
	void SetSearchRoomSleeps( int n );
	void SetSearchRoomSubType( int n );
	void SetTariffType( int n );
	void SetRoomRateType( int n );
	void SetRoomRate( double d );
	void SetExtraBedRate( double d );
	void SetInvoiceNum( int n );
	void SetDinnerAllowance( double d );
	void SetDinnerDepartments( const char* sz );
	//SUB ACCOUNTS
	void SetMultipleAccountsFlag( bool b );
	void SetAccountCount( int n );
	void SetAccountCovers( int n, int nCovers );
	//OTHER
	void SetBookingNote( const char* sz );
	void SetEmailStartDay( int n );
	void SetEmailNights( int n );

	int GetDepositCount()			{ return m_arrayDeposits.GetSize(); }
	void GetDepositInfo				( int nIdx, CPMSDepositInfo& infoDeposit );
	void SetDepositInfo				( int nIdx, CPMSDepositInfo& infoDeposit );
	void AddDepositInfo				( CPMSDepositInfo& infoDeposit );
	void RemoveDepositInfo			( int nIdx );

	int GetPartCount()				{ return m_arrayParts.GetSize(); }
	void GetFinalPartInfo			( CPMSBookingPartInfo& infoPart );
	void SetFinalPartInfo			( CPMSBookingPartInfo& infoPart );
	void RemoveFinalPartInfo		();
	void GetPartInfo				( int nIdx, CPMSBookingPartInfo& infoPart );
	void SetPartInfo				( int nIdx, CPMSBookingPartInfo& infoPart );
	void AddPartInfo				( CPMSBookingPartInfo& infoPart );

	bool GetAccomodationSpend( double& dRoomSpend, double& dExtraBedSpend, int& nStartDay, int& nStayLength );
	void GetAccomodationInvoiceLine( int nStartDay, int nNights, CPMSInvoiceLineInfo& infoTrans, CPMSInvoiceLineInfo& infoAccom );
	void GetExtraBedInvoiceLine( int nStartDay, int nNights, CPMSInvoiceLineInfo& infoTrans, CPMSInvoiceLineInfo& infoExtraBed );
	void GetAccomodationTaxPeriods( CArray<CPMSBookingPartInfo,CPMSBookingPartInfo>& arrayParts );
	void GetDepositInvoiceArray( CArray<CPMSInvoiceLineInfo,CPMSInvoiceLineInfo>& arrayDeposit );
	bool GetDinnerAdjustmentLine( CString& strDate, double dCredit, CPMSInvoiceLineInfo& infoTrans, CPMSInvoiceLineInfo& infoCredit, bool bCredit );
	bool SplitAccommodationLine( CPMSInvoiceLineInfo& infoAccom, CPMSInvoiceLineInfo& infoBreakfast );

	bool UpdateInvoiceNum();

public:
	int GetValidationError()			{ return m_nValidationError; }
	int GetValidationGuests()			{ return m_nValidationGuests; }
	int GetValidationCheckInDay()		{ return m_nValidationCheckInDay; }
	int GetValidationCheckOutDay()		{ return m_nValidationCheckOutDay; }
	int GetValidationMinCheckOutDay()	{ return m_nValidationMinCheckOutDay; }
	int GetValidationMaxCheckOutDay()	{ return m_nValidationMaxCheckOutDay; }

public:
	bool CheckDinnerSettings( int& nDinnerTaxBand );

private:
	void V1To5ConvertFromCSV( CCSV& csv, int nVer );
	void V6To7ConvertFromCSV( CCSV& csv, int nVer );
	void V8To10ConvertFromCSV( CCSV& csv, int nVer );
	
public:
	void BuildRoomListForEditing ( CArray<CPMSBookSearchListInfo,CPMSBookSearchListInfo>& arrayRooms );
	void ValidateForEditing( int nDayNow, bool bAllowClosed = FALSE );

public:
	bool CheckEmailDates();
	void SetEmailDates();

private:
	//QUICK ACCESS FIELDS
	int m_nBookingId;
	int m_nInternalStatus;
	int m_nCheckoutBalanceType;
	//BOOKING DETAIL
	int m_nCustomerId;
	int m_nGuests;
	int m_nSearchStartDay;
	int m_nSearchNights;
	int m_nSearchRoomSleeps;
	int m_nSearchRoomSubType;
	int m_nTariffType;
	int m_nRoomRateType;
	double m_dRoomRate;
	double m_dExtraBedRate;
	int m_nInvoiceNum;
	double m_dDinnerAllowance;
	CString m_strDinnerDepartments;
	//SUB ACCOUNTS
	bool m_bMultipleAccounts;
	int m_nAccountCount;
	int m_arrayCovers[ MAX_PMS_ACCOUNTS_PER_ROOM ];
	//OTHER
	CString m_strBookingNote;
	CString m_strBookingNoteHTML;
	int m_nEmailStartDay;
	int m_nEmailNights;
	
	CArray<CPMSBookingPartInfo,CPMSBookingPartInfo> m_arrayParts;
	CArray<CPMSDepositInfo,CPMSDepositInfo> m_arrayDeposits;
	bool m_bDinnerDepartment[100];

private:
	int m_nValidationError;
	int m_nValidationCheckInDay;
	int m_nValidationGuests;
	int m_nValidationCheckOutDay;
	int m_nValidationMinCheckOutDay;
	int m_nValidationMaxCheckOutDay;
};

/**********************************************************************/

class CPMSBookingCSVArray : public CSharedCSVArray
{
public:
	CPMSBookingCSVArray();
	~CPMSBookingCSVArray();
	int Open( const char* szFilename, int nMode );

public:
	bool FindBookingById( int nId, int& nBookingIdx );
	int FindFirstFreeId();

public:
	int GetBookingId ( int nIdx );
	void QuickGetAt( int nIdx, CPMSBookingCSVRecord& BookingRecord, bool bBuildCalendar );
	
public:
	bool GetRepairDatabaseFlag(){ return m_bRepairDatabase; }
	void RebuildFromBookingFiles();
	void ValidateBookingDatabase();

public:
	static void InsertAndSaveBooking( CPMSBookingCSVRecord& BookingRecord );
	static void RemoveAndSaveBooking( int nBookingId );
	
public:
	void BuildInternalTables();
	
public:
	static int GetCurrentDayNumber();
	
private:
	CStringArray m_arrayEditBuffer;
	bool m_bRepairDatabase;
};

/**********************************************************************/
#endif
/**********************************************************************/
