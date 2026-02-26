/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSRoomStatusInfo.h"
/**********************************************************************/

CPMSRoomAccountInfo::CPMSRoomAccountInfo()
{
	Reset();
}

/**********************************************************************/

void CPMSRoomAccountInfo::Reset()
{
	m_nCustomerId = 0;
	m_strBillingName = "";
	m_strGuestName = "";
	m_nGuestCount = 1;
	m_dTotalAccomSpend = 0.0;
	m_dTotalExtraBedSpend = 0.0;
	m_dTotalItemSpend = 0.0;
	m_dTotalDepositPaid = 0.0;
	m_dTotalPostCheckInPaid = 0.0;
	m_dTotalAllowance = 0.0;
	m_timeBalance = CTime( 1980, 1, 1, 0, 0, 0, -1 );
}

/**********************************************************************/

void CPMSRoomAccountInfo::ResetCustomerName()
{
	m_strBillingName = "";
	m_strGuestName = "";

	if ( m_nCustomerId != 0 )
	{
		int nCustomerIdx;
		if ( DataManagerNonDb.PMSCustomer.FindCustomerById( m_nCustomerId, nCustomerIdx ) == TRUE )
		{
			CPMSCustomerCSVRecord Customer;
			DataManagerNonDb.PMSCustomer.GetAt( nCustomerIdx, Customer );
			m_strBillingName = Customer.GetBillingName();
			m_strGuestName = Customer.GetGuestName();
		}
	}
}

/**********************************************************************/

CPMSRoomStatusInfo::CPMSRoomStatusInfo()
{
	Reset();
}

/**********************************************************************/

void CPMSRoomStatusInfo::Reset()
{
	m_nRoomNo = 1;
	m_strRoomName = "";
	m_dRoomOnlyRate = 0.0;
	m_dRoomBBRate = 0.0;
	m_dRoomDBBRate = 0.0;
	m_dRoomOnlySingleRate = 0.0;
	m_dRoomBBSingleRate = 0.0;
	m_dRoomDBBSingleRate = 0.0;
	m_dRoomExtraBedRate = 0.0;
	m_dRoomExtraCotRate = 0.0;
	m_nBookingId = 0;
	m_nStartDay = 0;
	m_nNights = 1;
	m_nGuestCount = 0;
	m_nAccountCount = 0;
	m_bMultipleAccounts = 0;

	m_bRoomStatusValid = FALSE;
	m_nExpectedBookingId = 0;
	m_strExpectedBillingName = "";
	m_strExpectedGuestName = "";
	m_nAvailableNights = 0;
	m_bClosure = FALSE;
	m_nClosureId = 0;
}

/**********************************************************************/

int CPMSRoomStatusInfo::GetSafeAccountCount()
{
	if ( m_nAccountCount < 0 )
		return 0;
	else if ( m_nAccountCount > MAX_PMS_ACCOUNTS_PER_ROOM )
		return MAX_PMS_ACCOUNTS_PER_ROOM;
	else
		return m_nAccountCount;
}
	
/**********************************************************************/
#endif
/**********************************************************************/
