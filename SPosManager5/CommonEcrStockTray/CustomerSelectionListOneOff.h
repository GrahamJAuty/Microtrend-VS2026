#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "CustomerSelectionListTempSortInfo.h"
/**********************************************************************/

struct CCustomerSelectionListOneOffInfo
{
public:
	CCustomerSelectionListOneOffInfo();
	void Reset();

public:
	int Compare ( CCustomerSelectionListOneOffInfo& source, int nHint = 0 );
	void Add	( CCustomerSelectionListOneOffInfo& source );

public:
	const char* GetReportName();
	
public:
	int m_nLocIdx;
	int m_nCustomerID;
	
public:
	CString m_strName1;
	CString m_strName2;
	CString m_strPhone1;
	CString m_strPhone2;
	CString m_strPhone3;
	int m_nSortIdx;
	
public:
	bool m_bIsSelected;
	bool m_bIsSelectedTemp;
	bool m_bIsInFilter;
	
private:
	CString m_strReportName;
};

/**********************************************************************/

class CCustomerSelectionListOneOff
{
public:
	CCustomerSelectionListOneOff(void);
	void BuildList();
	void BuildListCurrentDb();
	void ClearList();
	void BuildSortOrder( int nSortOrder = -1, bool bWithFilter = FALSE, CString strID = "", CString strName = "", CString strPhone = "" );

public:
	bool FindCustomer( CCustomerSelectionListOneOffInfo& info, int& nPos );
	void SetCustomerStatus( int nPos, CCustomerSelectionListOneOffInfo& info );
	void GetCustomerStatus( int nPos, CCustomerSelectionListOneOffInfo& info );
	void GetCustomerStatus( CCustomerSelectionListOneOffInfo& info );
	
public:
	bool FindCustomer( CCustomerSelectionListAccountInfo& info, int& nPos );
	void GetCustomerStatus( int nPos, CCustomerSelectionListAccountInfo& info );
	void GetCustomerStatus( CCustomerSelectionListAccountInfo& info );

public:
	int GetSize() { return m_arrayCustomers.GetSize(); }

public:
	int GetSortOrderType(){ return m_nSortOrderType; }
	int GetSortOrderSize();
	int GetSortOrderItem( int nIdx );

private:
	void BuildListInternal( int LocIdx );

private:
	CReportConsolidationArray<CCustomerSelectionListOneOffInfo> m_arrayCustomers;
	CArray<int,int> m_arraySortOrder;

private:
	int m_nSortOrderType;
};

/**********************************************************************/
