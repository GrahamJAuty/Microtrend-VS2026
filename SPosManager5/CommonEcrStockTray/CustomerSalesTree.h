#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "EntityTree.h"
#include "EposReportConsolCustomerHistory.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
	
struct CCustomerSalesConsolidationInfo
{
public:
	CCustomerSalesConsolidationInfo();

public:
	//TREE CONSOLIDATION
	int m_nTreeInt;
	COleDateTime m_oleTreeDate;
	CString m_strTreeString;
	//INFORMATION TO CONSOLIDATE
	CEposReportConsolCustomerHistory m_infoConsol;
	//INTERNAL
	bool m_bEndNode;
	int m_nNextArrayIdx;
	__int64 m_nNodeCount;
	int m_nNextTransNo;
};

/**********************************************************************/

class CCustomerSalesNode
{
public:
	CCustomerSalesNode();

private:
	CEposReportConsolCustomerHistory m_infoConsol;
	int m_nNextArrayIdx;

friend class CCustomerSalesArray;
friend class CCustomerSalesTable;
};

/**********************************************************************/

class CCustomerSalesArray
{
private:
	CCustomerSalesArray( int nSortType );
	~CCustomerSalesArray();
	
	void Clear();
	bool FindNodeString( CString& strLabelToFind, int& nNodeIdx );
	bool FindNodeInt( int nLabelToFind, int& nNodeIdx );
	bool FindNodeDate( COleDateTime oleLabelToFind, int& nNodeIdx );
	
	int ConsolidateString( 
		CCustomerSalesConsolidationInfo& infoNew, 
		CReportConsolidationArray<CEposReportConsolCustomerHistory>& arrayItems,
		CReportConsolidationArray<CEposReportConsolCustomerHistory>& arrayTotals );

	int ConsolidateInt( 
		CCustomerSalesConsolidationInfo& infoNew, 
		CReportConsolidationArray<CEposReportConsolCustomerHistory>& arrayItems,
		CReportConsolidationArray<CEposReportConsolCustomerHistory>& arrayTotals );

	int ConsolidateDate( 
		CCustomerSalesConsolidationInfo& infoNew, 
		CReportConsolidationArray<CEposReportConsolCustomerHistory>& arrayItems,
		CReportConsolidationArray<CEposReportConsolCustomerHistory>& arrayTotals );
	
private:
	CArray<CString,CString>* GetStringKeys() { return (CArray<CString,CString>*) m_pArrayKeys; }
	CArray<int,int>* GetIntKeys()	{ return (CArray<int,int>*) m_pArrayKeys; }
	CArray<COleDateTime,COleDateTime>* GetDateKeys() { return (CArray<COleDateTime,COleDateTime>*) m_pArrayKeys; }

private:
	int m_nSortType;

private:
	void* m_pArrayKeys;
	CArray<CCustomerSalesNode,CCustomerSalesNode> m_arrayNodes;
	
friend class CCustomerSalesTable;
};

/**********************************************************************/

class CCustomerSalesTable : public CPtrArray
{
public:
	CCustomerSalesTable();
	~CCustomerSalesTable();

public:
	void ClearConsolidatedLevels();
	void ClearFieldStructure();

	void AddField( int nType );
	void Consolidate( CEposReportConsolCustomerHistory& infoConsol, bool bSystem );

public:
	void SetBufferString( int nPos, CString& string );
	void SetBufferInt( int nPos, int n );
	void SetBufferDate( int nPos, COleDateTime date );
	
public:
	__int64 GetNodeCount() { return m_nNodeCount; }
	
	int GetArraySize( int nArrayIdx );
	
	const char* GetLabelString( int nArrayIdx, int nNodeIdx );
	int GetLabelInt( int nArrayIdx, int nNodeIdx );
	COleDateTime GetLabelDate( int nArrayIdx, int nNodeIdx );

	void GetSalesTotalForBlock( int nItemIdx, CEposReportConsolCustomerHistory& infoConsol );
	void GetSalesTotalForLine( int nItemIdx, CEposReportConsolCustomerHistory& infoConsol );
	void GetSalesTotalForNode( int nArrayIdx, int nNodeIdx, CEposReportConsolCustomerHistory& infoConsol );
	
	int GetNextArrayIdx( int nArrayIdx, int nNodeIdx );
	
	void FindTransactionItemLines( int nTranArrayNo, int& nStart, int& nEnd );
	void FindTransactionBlockLines( int nTranArrayNo, int& nStart, int& nEnd );

private:
	CCustomerSalesArray* GetArray( int nArrayIdx ) { return (CCustomerSalesArray*) GetAt(nArrayIdx); }

private:
	CByteArray m_arrayFieldTypes;
	CStringArray m_bufferString;
	CArray<int,int> m_bufferInt;
	CArray<COleDateTime,COleDateTime> m_bufferDate;
	
	int m_nNextTransNo;
	CReportConsolidationArray<CEposReportConsolCustomerHistory> m_arrayTranLines;
	CReportConsolidationArray<CEposReportConsolCustomerHistory> m_arrayTranBlocks;
	
private:
	__int64 m_nNodeCount;
};

/**********************************************************************/

