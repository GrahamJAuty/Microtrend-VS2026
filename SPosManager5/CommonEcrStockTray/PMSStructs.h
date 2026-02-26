#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/

struct CPMSBookSearchListInfo
{
	int m_nRoomIdx;
	int m_nRoomNo;
	int m_nSleeps;
	int m_nStartDay;
	int m_nNights;
	bool m_bExtraBed;
	bool m_bExtraCot;
	CString m_strRoomName;
	CString m_strDate;
	CString m_strType;
	bool m_bFullStay;
};

/**********************************************************************/

struct CPMSBookingRoom
{
	int m_nRoomNo;
	int m_nNights;
};

/**********************************************************************/

struct CPMSCalendarBooking
{
	int m_nRoomNo;
	int m_nStartDay;
	int m_nNights;
	int m_nBookingId;
	int m_nBookingPart;
	int m_nStatus;
	bool m_bEditing;
	bool m_bIsFinalPart;
};

/**********************************************************************/

struct CPMSBookingQueryInfo
{
	int m_nRoomNo;
	int m_nStartDay;
	int m_nMinNights;
	int m_nMaxNights;
	int m_nBookingId;
	int m_nBookingPart;
	int m_nStatus;
	bool m_bIsFinalPart;
};

/**********************************************************************/

struct CPMSBookingResponseInfo
{
	bool m_bSuccess;
	int m_nStartDay;
	int m_nMaxNights;
	int m_nMinStartDay;
	int m_nMaxStartDay;
	int m_nBookingClashId;
	int m_nBookingClashPart;
	int m_nBookingClashArrayIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
