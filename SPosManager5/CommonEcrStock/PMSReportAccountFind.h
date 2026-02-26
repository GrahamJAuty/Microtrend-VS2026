#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSAccountFindDlg.h"
/**********************************************************************/
#define PMS_ACCOUNTFIND_COLUMN_BOOKINGREF 0
#define PMS_ACCOUNTFIND_COLUMN_CHECKIN 1
#define PMS_ACCOUNTFIND_COLUMN_NIGHTS 2
#define PMS_ACCOUNTFIND_COLUMN_CHECKOUT 3
#define PMS_ACCOUNTFIND_COLUMN_ROOMNO 4
#define PMS_ACCOUNTFIND_COLUMN_ROOMNAME 5
#define PMS_ACCOUNTFIND_COLUMN_INVOICENUM 6
/**********************************************************************/

class CPMSReportAccountFind
{
public:
	CPMSReportAccountFind( CArray<CPMSAccountFindInfo,CPMSAccountFindInfo>& arrayInfo, CArray<int,int>& m_arrayFilter, bool bUseFilter );
	void SortAccounts();
	void CreateReport();

public:
	void SetSortType( int n ){ m_nSortType = n; }
	void SetRoomFlag( bool b ){ m_bRoom = b; }
	void SetBalanceFlag( bool b ){ m_bBalance = b; }
	void SetLinesFlag( bool b ){ m_bLines = b; }
	void SetInvoiceNumFlag( bool b ){ m_bInvoiceNum = b; }

private:
	void SetSortKey( CPMSAccountFindInfo& infdAccount, int nIdx );
	bool FindAccount( CPMSAccountFindInfo& info, int nIdx, int& nPos );
	
private:
	bool CreateReportInternal();

private:
	void AddColumnBookingRef();
	void AddColumnInvoiceNum();
	void AddColumnCheckIn();
	void AddColumnNights();
	void AddColumnCheckOut();
	void AddColumnRoomNo();
	void AddColumnRoomName();
	void AddColumnGuests();
	void AddColumnGuestName();
	void AddColumnBillingName();
	void AddColumnBalance();
	void AddColumnCR();
	
private:
	CArray<CPMSAccountFindInfo,CPMSAccountFindInfo>& m_arrayInfoList;
	CArray<CPMSAccountFindInfo,CPMSAccountFindInfo> m_arrayInfoSorted;
	CWordArray m_arrayOptionalColumns;
	CReportFile m_ReportFile;
	CString m_strParamsFilename;
	int m_nSortType;
	bool m_bRoom;
	bool m_bBalance;
	bool m_bLines;
	bool m_bInvoiceNum;

private:
	CArray<int,int>& m_arrayFilter;
	bool m_bUseFilter;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
