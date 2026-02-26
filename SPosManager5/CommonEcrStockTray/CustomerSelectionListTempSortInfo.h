#pragma once
/**********************************************************************/
 
/**********************************************************************/

struct CCustomerSelectionListTempSortInfo
{
public:
	CCustomerSelectionListTempSortInfo();
	void Reset();

public:
	int Compare ( CCustomerSelectionListTempSortInfo& source, int nHint = 0 );
	void Add	( CCustomerSelectionListTempSortInfo& source );
	
public:
	int m_nLocIdx;
	int m_nCustomerID;
	CString m_strName1;
	CString m_strName2;
	CString m_strPhone1;
	CString m_strPhone2;
	CString m_strPhone3;
	int m_nIdx;
};

/**********************************************************************/
