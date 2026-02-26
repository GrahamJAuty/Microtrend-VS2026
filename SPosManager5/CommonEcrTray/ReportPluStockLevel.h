#pragma once
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
#include "DepartmentSetCSVArray.h"
#include "EntityTree.h"
/**********************************************************************/
#define REPORT_PLUSTOCK_APPARENT 1
#define REPORT_PLUSTOCK_VALUATION 2
#define REPORT_PLUSTOCK_REORDER 3
#define REPORT_PLUSTOCK_DEL_AUDIT 4
#define REPORT_PLUSTOCK_DEL_TOTAL 5
#define REPORT_PLUSTOCK_DEL_VALUE 6
#define REPORT_PLUSTOCK_APPARENT_DATE 7
#define REPORT_PLUSTOCK_VALUATION_DATE 8
#define REPORT_PLUSTOCK_REPLENISH_DATE 9
/**********************************************************************/

struct CPluStockLevelBuffer
{
public:
	CPluStockLevelBuffer();

public:
	CString m_strZeroDateTime;
	bool m_bWorkBackFromCurrent;
	double m_dApparentQty;
	double m_dDeliveryQty;
};

/**********************************************************************/

class CReportPluStockLevel  
{
public:
	CReportPluStockLevel();
	void SetReportType( int nType );
	virtual ~CReportPluStockLevel();

public:
	void ConsolidateStockLevels();
	void ConsolidateDeliveryTotals();
	bool CreateReport();
	const char* GetReportKey() { return m_strKey; }
	const char* GetEmailSubject(){ return m_strEmailSubject; }

public:
	void SetPluRangeFlag ( bool bFlag ) { m_bPluRange = bFlag; }
	void SetPluFrom ( __int64 nPluNo ) { m_nPluFrom = nPluNo; }
	void SetPluTo ( __int64 nPluNo ) { m_nPluTo = nPluNo; }
	void SetDepartmentFilter ( int nFilter ) { m_DepartmentSet.CreateDepartmentSet ( nFilter ); }
	void SetDepartmentSortFlag( bool bFlag ) { m_bDepartmentSort = bFlag; }
	void SetSupplierFilter( int n ) { m_nSupplierFilter = n; }
	void SetSupplierSortFlag( bool bFlag ) { m_bSupplierSort = bFlag; }
	void SetZeroSkipFlag( bool bFlag ) { m_bZeroSkip = bFlag; }
	void SetDbIdx( int n ) { m_nDbIdx = n; }
	void SetReportName( const char* szName ) { m_strReportName = szName; }
	void SetDateTo( COleDateTime dateTo ) { m_oleDateTo = dateTo; }
	void SetDateFrom( COleDateTime dateFrom ) { m_oleDateFrom = dateFrom; }

private:
	bool IsValuationReport();

public:
	void SetStockLevelArray( CArray<CPluStockLevelBuffer,CPluStockLevelBuffer>* pArray ) { m_pStockLevelArray = pArray; }

private:
	void ProcessPluStockArray( int nDepth, int nBranchIdx, CEntityTreePluStockNode BlockTotalsNode );
	void WriteBlankLine( bool bOnlyIfNeeded );
	void CreateReportLine( CEntityTreePluStockNode CurrentNode );
	const char* GetHeaderLine( int nDepth );
	
	void WriteHeader( int nDepth );
	void WriteReportLine( const char* szLine );
	void WriteTotals( int nDepth, CEntityTreePluStockNode BlockTotalsNode );
	
private:
	CWordArray m_arrayReportFields;
	CWordArray m_arrayHeaderFlags;
	CWordArray m_arrayTotalFlags;
	CEntityTreePluStock m_PluStockTable;

private:
	CString m_strReportName;
	CString m_strKey;
	CString m_strEmailSubject;
	bool m_bGotReportLine;

	bool m_bPluRange;
	__int64 m_nPluFrom;
	__int64 m_nPluTo;

	bool m_bDepartmentSort;
	CDepartmentSetCSVRecord m_DepartmentSet;

	CArray<CPluStockLevelBuffer,CPluStockLevelBuffer>* m_pStockLevelArray;
	
	bool m_bSupplierSort;
	int m_nSupplierFilter;

private:
	COleDateTime m_oleDateFrom;
	COleDateTime m_oleDateTo;

private:
	CReportFile m_ReportFile;
	int m_nProgress;

private:
	bool m_bLastLineWasBlank;
	bool m_bZeroSkip;
	int m_nReportType;
	int m_nDbIdx;

private:
	CString m_strHeader;
	int m_nLabelDeptNo;
	CString m_strLabelDeptName;
	CString m_strLabelSuppName;
	int m_nLabelPluIdx;
	__int64 m_nLabelPluNo;
};

/**********************************************************************/
#endif
/**********************************************************************/
