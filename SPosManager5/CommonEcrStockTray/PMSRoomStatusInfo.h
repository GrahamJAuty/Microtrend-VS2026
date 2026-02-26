#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#define PMS_ACCOUNT_VERSION 1
/**********************************************************************/
#include "PMSDefines.h"
/**********************************************************************/

class CPMSRoomAccountInfo
{
public:
	CPMSRoomAccountInfo();

public:
	void Reset();

public:
	int GetCustomerId()						{ return m_nCustomerId; }
	const char* GetBillingName()			{ return m_strBillingName; }
	const char* GetGuestName()				{ return m_strGuestName; }
	int GetGuestCount()						{ return m_nGuestCount; }
	double GetTotalSpend()					{ return m_dTotalAccomSpend + m_dTotalExtraBedSpend + m_dTotalItemSpend; }
	double GetTotalAllowance()				{ return m_dTotalAllowance; }
	double GetTotalPaid()					{ return m_dTotalDepositPaid + m_dTotalPostCheckInPaid; }
	CTime GetBalanceTime()					{ return m_timeBalance; }

public:
	double CalculateBalance()				{ return GetTotalSpend() - ( GetTotalPaid() + GetTotalAllowance() ); }
	void ResetCustomerName();

public:
	void SetCustomerId ( int n )			{ m_nCustomerId = n; }
	void SetGuestCount( int n )				{ m_nGuestCount = n; }
	void SetTotalAccomSpend( double d )		{ m_dTotalAccomSpend = d; }
	void SetTotalExtraBedSpend( double d )	{ m_dTotalExtraBedSpend = d; }
	void SetTotalItemSpend( double d )		{ m_dTotalItemSpend = d; }
	void SetTotalAllowance( double d )		{ m_dTotalAllowance = d; }
	void SetTotalDepositPaid( double d )	{ m_dTotalDepositPaid = d; }
	void SetTotalPostCheckInPaid( double d ){ m_dTotalPostCheckInPaid = d; }
	void SetBalanceTime( CTime t )			{ m_timeBalance = t; }

private:
	int m_nCustomerId;
	CString m_strBillingName;
	CString m_strGuestName;
	int m_nGuestCount;
	double m_dTotalAccomSpend;
	double m_dTotalExtraBedSpend;
	double m_dTotalItemSpend;
	double m_dTotalAllowance;
	double m_dTotalDepositPaid;
	double m_dTotalPostCheckInPaid;
	CTime m_timeBalance;
};

/**********************************************************************/

class CPMSRoomStatusInfo
{
public:
	CPMSRoomStatusInfo();

public:
	void Reset();
	
public:
	int GetRoomNo()							{ return m_nRoomNo; }
	const char* GetRoomName()				{ return m_strRoomName; }
	double GetRoomOnlyRate()				{ return m_dRoomOnlyRate; }
	double GetRoomBBRate()					{ return m_dRoomBBRate; }
	double GetRoomDBBRate()					{ return m_dRoomDBBRate; }
	double GetRoomOnlySingleRate()			{ return m_dRoomOnlySingleRate; }
	double GetRoomBBSingleRate()			{ return m_dRoomBBSingleRate; }
	double GetRoomDBBSingleRate()			{ return m_dRoomDBBSingleRate; }
	double GetRoomExtraBedRate()			{ return m_dRoomExtraBedRate; }
	double GetRoomExtraCotRate()			{ return m_dRoomExtraCotRate; }
	int GetBookingId()						{ return m_nBookingId; }
	int GetStartDay()						{ return m_nStartDay; }
	int GetNights()							{ return m_nNights; }
	int GetGuestCount()						{ return m_nGuestCount; }
	int GetSafeAccountCount();				
	bool GetMultipleAccountsFlag()			{ return m_bMultipleAccounts; }
	
public:
	void SetRoomNo( int n )					{ m_nRoomNo = n; }
	void SetRoomName( const char* sz )		{ m_strRoomName = sz; }
	void SetRoomOnlyRate( double d )		{ m_dRoomOnlyRate = d; }
	void SetRoomBBRate( double d )			{ m_dRoomBBRate = d; }
	void SetRoomDBBRate( double d )			{ m_dRoomDBBRate = d; }
	void SetRoomOnlySingleRate( double d )	{ m_dRoomOnlySingleRate = d; }
	void SetRoomBBSingleRate( double d )	{ m_dRoomBBSingleRate = d; }
	void SetRoomDBBSingleRate( double d )	{ m_dRoomDBBSingleRate = d; }
	void SetRoomExtraBedRate( double d )	{ m_dRoomExtraBedRate = d; }
	void SetRoomExtraCotRate( double d )	{ m_dRoomExtraCotRate = d; }
	void SetBookingId( int n )				{ m_nBookingId = n; }
	void SetStartDay( int n )				{ m_nStartDay = n; }
	void SetNights( int n )					{ m_nNights = n; }
	void SetGuestCount( int n )				{ m_nGuestCount = n; }
	void SetAccountCount( int n )			{ m_nAccountCount = n; }
	void SetMultipleAccountsFlag( bool b )	{ m_bMultipleAccounts = b; }
	
private:
	bool GetRoomStatusValidFlag()			{ return m_bRoomStatusValid; }
	int GetExpectedBookingId()				{ return m_nExpectedBookingId; }
	const char* GetExpectedBillingName()	{ return m_strExpectedBillingName; }
	const char* GetExpectedGuestName()		{ return m_strExpectedGuestName; }
	int GetAvailableNights()				{ return m_nAvailableNights; }
	bool GetClosureFlag()					{ return m_bClosure; }
	int GetClosureId()						{ return m_nClosureId; }

private:
	void SetRoomStatusValidFlag( bool b )			{ m_bRoomStatusValid = b; } 
	void SetExpectedBookingId( int n )				{ m_nExpectedBookingId = n; }
	void SetExpectedBillingName( const char* sz )	{ m_strExpectedBillingName = sz; }
	void SetExpectedGuestName( const char* sz )		{ m_strExpectedGuestName = sz; }
	void SetAvailableNights( int n )				{ m_nAvailableNights = n; }
	void SetClosureFlag( bool b )					{ m_bClosure = b; }
	void SetClosureId( int n )						{ m_nClosureId = n; }

private:
	int m_nRoomNo;
	CString m_strRoomName;
	double m_dRoomOnlyRate;
	double m_dRoomBBRate;
	double m_dRoomDBBRate;
	double m_dRoomOnlySingleRate;
	double m_dRoomBBSingleRate;
	double m_dRoomDBBSingleRate;
	double m_dRoomExtraBedRate;
	double m_dRoomExtraCotRate;
	int m_nBookingId;
	int m_nStartDay;
	int m_nNights;
	int m_nGuestCount;
	int m_nAccountCount;
	bool m_bMultipleAccounts;

private:
	bool m_bRoomStatusValid;
	int m_nExpectedBookingId;
	CString m_strExpectedBillingName;
	CString m_strExpectedGuestName;
	int m_nAvailableNights;
	bool m_bClosure;
	int m_nClosureId;

	friend class CPMSRoomStatusTable;
};

/**********************************************************************/
#endif
/**********************************************************************/
