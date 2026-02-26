#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ProcessedOrderArray.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

struct CSupplierOrderInfo
{
	int m_nSuppIdx;
	int m_nSuppNo;
	int m_nLineCount;
};

/**********************************************************************/

class CSingleListContentHelpersOrderList : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersOrderList( bool bClosed, bool& bFiltered );
	
public:
	int GetSpIdx(){ return m_nSpIdx; }
	int GetDbIdx(){ return m_nDbIdx; }
	void SetSpIdx( int n ){ m_nSpIdx = n; }
	void SetDbIdx( int n ){ m_nDbIdx = n; }
	
public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){}
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent ){}

public:
	void BuildSupplierList();
	void GetOrderInfo( int nIndex, CSupplierOrderInfo& info );
	void UpdateOrderLines( int nSuppIdx );

private:
	int m_nSpIdx;
	int m_nDbIdx;
	CArray <CSupplierOrderInfo,CSupplierOrderInfo> m_arrayAll;
	CArray <CSupplierOrderInfo,CSupplierOrderInfo> m_arrayFiltered;
	CArray <CProcessedOrder,CProcessedOrder> m_arrayOrders;
	bool& m_bFiltered;

private:
	int m_nOrderType;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

struct CRepeatOrderInfo
{
	CString m_strDate;
	CString m_strTime;
	CString m_strDateDisplay;
	CString m_strTimeDisplay;
	CString m_strReference;
	CString m_strInfoFilename;
	CString m_strDataFilename;
	int m_nLineCount;
	int m_nConLevel;
	int m_nTableNo;
	bool m_bAllowSaveTemplate;
};

/**********************************************************************/

class CSingleListContentHelpersOrderRepeat : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersOrderRepeat();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );	
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
	
public:
	void RebuildList();
	int GetOrderInfoArraySize();
	void GetOrderInfo( int nIndex, CRepeatOrderInfo& info );
	const char* GetOrderFolder(){ return m_strOrderFolder; }
	int GetDeletedArraySize();
	void GetDeletedItem( int nIndex, CSortedStringItem& item );

public:
	int GetDbIdx(){ return m_nDbIdx; }
	void SetDbIdx( int nDbIdx );

private:
	CString m_strOrderFolder;
	CArray<CRepeatOrderInfo,CRepeatOrderInfo> m_arrayOrderInfo;
	CReportConsolidationArray<CSortedStringItem> m_arrayDeleted;
	int m_nDbIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
