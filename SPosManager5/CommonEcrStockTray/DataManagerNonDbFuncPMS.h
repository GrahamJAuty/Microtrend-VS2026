#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "DefTextUnit.h"
#include "PMSBookingCSVArray.h"
#include "PMSClosedRoomCSVArray.h"
#include "PMSCustomerCSVArray.h"
#include "PMSInvoiceBookingBuffer.h"
#include "PMSRoomCSVArray.h"
#include "PMSRoomGroup.h"
#include "PMSRoomType.h"
/**********************************************************************/

class CDataManagerNonDbFuncPMS : virtual public CDataManagerBase
{
public:
	CDataManagerNonDbFuncPMS();

public:
	bool OpenPMSClosureReasons( int nType, CDataManagerInfo& info );
	
public:
	bool ClosePMSClosureReasons( CDataManagerInfo& info );
	
public:
	bool WritePMSClosureReasons( CDataManagerInfo& info );
	
public:
	bool OpenPMSClosureReasonsBuffer( CDataManagerInfo& info );
	
public:
	bool ClosePMSClosureReasonsBuffer( CDataManagerInfo& info );
	
public:
	bool WritePMSClosureReasonsBuffer( CDataManagerInfo& info );

public:
	CPMSBookingCSVArray PMSBooking;
	CPMSClosedRoomCSVArray PMSClosure;
	CPMSCustomerCSVArray PMSCustomer;
	CPMSInvoiceBookingBuffer PMSInvoiceBookingBuffer;
	CPMSRoomCSVArray PMSRoom;
	CPMSRoomGroup PMSRoomGroup;
	CPMSRoomType PMSRoomType;

public:
	CDefTextUnit PMSClosureReasons;
	
public:
	CDefTextUnit PMSClosureReasonsBuffer;

public:
	int m_nStatusPMSClosureReasons;
	
public:
	int m_nStatusPMSClosureReasonsBuffer;
};

/**********************************************************************/
#endif
/**********************************************************************/
