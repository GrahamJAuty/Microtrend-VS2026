#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "CustomerSelectionListTempSortInfo.h"
#include "EposReports.h"
/**********************************************************************/

struct CCustomerSelectionListAccountInfo
{
public:
	CCustomerSelectionListAccountInfo();
	void Reset();

public:
	int Compare ( CCustomerSelectionListAccountInfo& source, int nHint = 0 );
	void Add	( CCustomerSelectionListAccountInfo& source );

public:
	const char* GetReportName();
	
public:
	int m_nLocIdx;
	int m_nCustomerID;
	
public:
	CString m_strName1;
	CString m_strName2;
	CString m_strAddress1;
	CString m_strAddress2;
	CString m_strAddress3;
	CString m_strPostcode;
	CString m_strEmail;
	bool m_bEmailRedirect;
	int m_nSortIdx;
	double m_dStartBalance;
	COleDateTime m_startTime;

public:
	bool m_bIsSelected;
	bool m_bIsSelectedTemp;
	bool m_bIsInFilter;
	
public:
	double m_dBalance;

private:
	CString m_strReportName;
};

/**********************************************************************/

class CCustomerSelectionListAccount
{
public:
	CCustomerSelectionListAccount(void);
	void BuildList();
	void BuildListCurrentDb();
	void ClearList();
	void BuildSortOrder( int nSortOrder = -1 );

public:
	bool GetOneOffCustomerFlag(){ return m_bOneOffCustomer; }
	bool GetUnknownCustomerFlag(){ return m_bUnknownCustomer; }
	void SetOneOffCustomerFlag( bool b ){ m_bOneOffCustomer = b; }
	void SetUnknownCustomerFlag( bool b ){ m_bUnknownCustomer = b; }

public:
	bool FindCustomer( CCustomerSelectionListAccountInfo& info, int& nPos );
	
	void SetCustomerStatus( int nPos, CCustomerSelectionListAccountInfo& info );
	void GetCustomerStatus( int nPos, CCustomerSelectionListAccountInfo& info );
	void GetCustomerStatus( CCustomerSelectionListAccountInfo& info );

	int GetSize() { return m_arrayCustomers.GetSize(); }

public:
	int GetSortOrderType(){ return m_nSortOrderType; }
	int GetSortOrderSize();
	int GetSortOrderItem( int nIdx );

public:
	void GetCustomerListForLocation( int nLocIdx, CReportConsolidationArray<CEposReportConsolCustomerBalanceID>& arrayDest );

private:
	void BuildListInternal( int LocIdx );

private:
	CReportConsolidationArray<CCustomerSelectionListAccountInfo> m_arrayCustomers;
	CArray<int,int> m_arraySortOrder;

private:
	bool m_bOneOffCustomer;
	bool m_bUnknownCustomer;
	int m_nSortOrderType;
};

/**********************************************************************/
