#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSInvoiceArray.h"
#include "PMSRoomStatusInfo.h"
#include "RepCSV.h"
/**********************************************************************/

class CPMSReportInvoice
{
public:
	CPMSReportInvoice( int nRoomGridRoomIdx, int nRoomGridAccountIdx, int nInvoiceType, bool bPendingBooking );
	
	void Consolidate();
	bool CreateReport();

	const char* GetParamsFilename();

private:
	void CreateItemInvoice();
	void CreateGroupInvoice();
	void CreateTransactionInvoice();

private:
	const char* GetCurrencyString( double dAmount, bool bBottomLine = FALSE );

private:
	CPMSInvoiceArray m_arrayInvoiceItems;
	
private:
	CReportFile m_ReportFile;

private:
	int m_nReportType;
	int m_nInvoiceType;
	int m_nAccountIdx;
	CString m_strParamsFilename;

private:
	CPMSRoomStatusInfo m_infoRoom;
	CPMSRoomAccountInfo m_infoAccount;
	CString m_strCurrency;
	bool m_bPendingBooking;
};

/**********************************************************************/
#endif
/**********************************************************************/
