#pragma once
/**********************************************************************/
#include "AfxTempl.h"
/**********************************************************************/
#include "EposSelectArrayFeeder.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#define EPOSSELECT_TERMINAL 0
#define EPOSSELECT_DB_MANUAL 1
#define EPOSSELECT_DB_IMPORT 2
/**********************************************************************/

struct CEposSelectArrayLocSort
{
public:
	CEposSelectArrayLocSort();
	void Reset();

public:
	int Compare ( CEposSelectArrayLocSort& source, int nHint = 0 );
	void Add	( CEposSelectArrayLocSort& source );
	
public:
	int m_nSortIdx;
	int m_nLocIdx;
	int m_nTermIdx;
	int m_nSelArrayIdx;
};

/**********************************************************************/

struct CEposSelectArrayListItem
{
public:
	CEposSelectArrayListItem();
	void Reset();

public:
	int m_nLineType;
	int m_nDbIdx;
	int m_nLocIdx;
	int m_nTermIdx;
	int m_nLineCount;
	COleDateTime m_dateStart;
	COleDateTime m_dateEnd;
};

/**********************************************************************/

class CEposSelectArray
{
public:
	CEposSelectArray();
	void RemoveAll();

public:
	void SelectTerminal( int nLocIdx, int nTermIdx ){ m_Feeder.SelectTerminal( nLocIdx, nTermIdx ); }
	void SelectLocation( int nLocIdx ){ m_Feeder.SelectLocation( nLocIdx ); }
	void SelectDatabase( int nDbIdx ){ m_Feeder.SelectDatabase( nDbIdx ); }
	void SelectSystem(){ m_Feeder.SelectSystem(); }

public:
	const char* GetLocationString( int nLocIdx ){ return m_Feeder.GetLocationString( nLocIdx ); }
	bool SetLocationString( int nLocIdx, const char* sz ){ return m_Feeder.SetLocationString( nLocIdx, sz ); }

public:
	void SelectDatabaseForNonEposSales( int nDbIdx, bool bManual, bool bImport ){ m_Feeder.SelectDatabaseForNonEposSales( nDbIdx, bManual, bImport ); }

public:
	void TagOriginalSelectionStatus( bool bSelectAllTerminals ){ m_Feeder.TagOriginalSelectionStatus( bSelectAllTerminals ); }
	bool GetOriginalSelectionStatus( int nLocIdx, int nTermIdx ){ return m_Feeder.GetOriginalSelectionStatus( nLocIdx, nTermIdx ); }
	bool GetSelectionStatus( int nLocIdx, int nTermIdx ){ return m_Feeder.GetSelectionStatus( nLocIdx, nTermIdx ); }

public:
	bool IsEmpty(){ return m_Feeder.IsEmpty(); }
	bool GotTerminals( int nLocIdx ){ return m_Feeder.GotTerminals( nLocIdx ); }

public:
	void MakeList( bool bAllowSort = FALSE, bool bOneTerminalPerLocation = FALSE );
	void MakeListLocation( int nLocIdx, bool bOneTerminalPerLocation = FALSE );
	
	int GetListSize() { return m_arrayListItems.GetSize(); }
	void GetListItem( int nIndex, CEposSelectArrayListItem& item );
	void SetListItem( int nIndex, CEposSelectArrayListItem& item );
	void ProcessStartDate( int nIndex, COleDateTime date );
	void ProcessEndDate ( int nIndex, COleDateTime date );
	void ProcessLineCount( int nIndex, int nCount );
	const char* GetDateInfo( CEposSelectArrayListItem& ListItem, bool& bHideNoData );
	void MergeDateInfo(CEposSelectArrayListItem& ListItemSource, CEposSelectArrayListItem& ListItemDest, int& nTerminals, int& nWithData);
	void ResetDateInfo();

public:
	void BuildLocSortArray();
	int GetLocSortArraySize(){ return m_arrayLocSort.GetSize(); }
	int GetLocSortIdx( int nSortIdx );
		
private:
	CString m_strDateInfo;

private:
	CEposSelectArrayFeeder m_Feeder;
	CArray<CEposSelectArrayListItem> m_arrayListItems;
	CReportConsolidationArray<CEposSelectArrayLocSort> m_arrayLocSort;
};

/**********************************************************************/
