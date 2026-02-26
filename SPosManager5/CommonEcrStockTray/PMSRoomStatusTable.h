#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSBookingCSVArray.h"
#include "PMSRoomStatusInfo.h"
/**********************************************************************/

class CPMSRoomStatusTable
{
public:
	CPMSRoomStatusTable();

public:
	void GetRoomInfo( int nRoomIdx, CPMSRoomStatusInfo& info );
	void SetRoomInfo( int nRoomIdx, CPMSRoomStatusInfo& info );
	
public:
	void GetAccountInfo( int nRoomIdx, int nAccountIdx, CPMSRoomAccountInfo& info );
	void SetAccountInfo( int nRoomIdx, int nAccountIdx, CPMSRoomAccountInfo& info );

public:
	void CheckIn( int nRoomIdx, CPMSBookingCSVRecord& BookingRecord );
	void CheckOut( int nRoomIdx );

public:
	void UpdateAccountBalance( int nRoomIdx, bool bForceCheck );
	bool UpdateAccountBalance( int nRoomIdx, int nAccountIdx, bool bForceCheck );
	bool UpdateAccountBalance( int nRoomIdx, int nAccountIdx, CPMSBookingCSVRecord& BookingRecord );
	int GetBalanceForBooking( CPMSBookingCSVRecord& BookingRecord, int nAccountIdx, double& dAccomSpend, double& dExtraBedSpend, double& dItemSpend, double& dDepositPaid, double& dPostCheckInPaid, double& dAllowance, CTime& timeCheck, bool bForceCheck );

public:
	void InitialiseRooms();
	void LoadRoomAccountSettingsLive( CPMSBookingCSVRecord& BookingRecord, bool bAccounts );
	void LoadRoomAccountSettingsBuffer( CPMSBookingCSVRecord& BookingRecord, bool bBalance );

private:
	void LoadRoomAccountSettingsInternal( int nRoomIdx, CPMSBookingCSVRecord& Booking );
	
public:
	void WriteDownloadFileRoom( int nRoomIdx );
	void WriteDownloadFileAccount( int nRoomIdx, int nAccountIdx );
	
public:
	bool ValidateRoomAndAccountIdx( int nRoomIdx, int nAccountIdx );
	const char* GetDownloadFilename( int nRoomIdx, int nAccountIdx );

public:
	void InvalidateRoomStatus();
	bool QueryRoomStatus( int nRoomIdx, CPMSRoomStatusInfo& infoRoom );
	int GetExpectedBookingId( int nRoomIdx );
	const char* GetExpectedBillingName( int nRoomIdx );
	const char* GetExpectedGuestName( int nRoomIdx );
	int GetAvailableNights( int nRoomIdx );
	bool GetClosureFlag( int nRoomIdx );
	int GetClosureId( int nRoomIdx );
	
private:
	void UpdateExpectedBookingId( int nRoomIdx );

private:
	CPMSRoomStatusInfo m_arrayRooms[ MAX_PMS_ROOMS + 1 ];
	CPMSRoomAccountInfo m_arrayAccounts[ MAX_PMS_ROOMS + 1 ][ MAX_PMS_ACCOUNTS_PER_ROOM ];

private:
	CString m_strDownloadFilename;
	CString m_strExpectedBillingName;
	CString m_strExpectedGuestName;
	int m_nExpectedBookingDayNumberCheck;
};

/**********************************************************************/
extern CPMSRoomStatusTable PMSRoomStatusTable;
/**********************************************************************/
#endif
/**********************************************************************/
