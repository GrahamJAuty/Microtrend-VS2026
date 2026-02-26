#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSBookingCSVArray.h"
#include "PMSRoomCSVArray.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CPMSTariffMapItem
{
public:
	CPMSTariffMapItem();
	void Reset();

public:
	int Compare ( CPMSTariffMapItem& source, int nHint = 0 );
	void Add	( CPMSTariffMapItem& source );
	
public:
	int m_nTariffType;
	int m_nRoomRateTypes;
};

/**********************************************************************/

class CPMSTariffMap
{
public:
	CPMSTariffMap();

public:
	void BuildMap( CPMSBookingCSVRecord& Booking );
	void BuildMap( CPMSRoomCSVRecord& Room );
	void BuildMap( CPMSBookingCSVRecord& Booking, CPMSRoomCSVRecord& Room );

public:
	void GetTariffTypeList( CArray<int,int>& arrayTariffs );
	void GetRoomRateTypeList( int nTariff, CArray<int,int>& arrayRoomRates );

private:
	void BuildMapInternal( CPMSBookingCSVRecord& Booking );
	void BuildMapInternal( CPMSRoomCSVRecord& Room );
	void BuildMapInternalRoom( int nTariffType );
	
private:
	CReportConsolidationArray<CPMSTariffMapItem> m_Map;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
