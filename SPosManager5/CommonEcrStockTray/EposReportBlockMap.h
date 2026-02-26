#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "EposSelectArray.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#define BLOCKMAP_SPECIAL_NONE 0
#define BLOCKMAP_SPECIAL_REPPOINT 1
#define BLOCKMAP_SPECIAL_DORBIERE 2
/**********************************************************************/

struct CEposReportBlockMapTempIndex
{
public:
	CEposReportBlockMapTempIndex();

public:
	void Reset();
	int Compare ( CEposReportBlockMapTempIndex& source, int nHint = 0 );
	void Add	( CEposReportBlockMapTempIndex& source );
	
public:
	int m_nDbIdx;
	int m_nRpIdx;
	int m_nLocIdx;
	int m_nTermIdx;
	int m_nSelArrayIdx;
};

/**********************************************************************/

struct CEposReportBlockMapEntity
{
public:
	void Copy( CEposReportBlockMapEntity& source );
	void GetHeaderText( CString& strHeader, bool bFullLine = TRUE );

public:
	int m_nEntityType;
	int m_nDbIdx;
	int m_nRpIdx;
	int m_nLocIdx;
	int m_nTermIdx;
	bool m_bAddToTotal;
};

/**********************************************************************/

struct CEposReportBlockMapSortInfo
{
public:
	CEposReportBlockMapSortInfo();
	void Reset();

public:
	int Compare ( CEposReportBlockMapSortInfo& source, int nHint = 0 );
	void Add	( CEposReportBlockMapSortInfo& source );
		
public:
	bool m_bIsSystemBlock;
	int m_nDbSortIdx;
	bool m_bIsDbBlock;
	int m_nRpSortIdx;
	int m_nLocSortIdx;
	int m_nMapIdx;
};

/**********************************************************************/

class CEposReportBlockMap
{
public:
	CEposReportBlockMap(void);
	~CEposReportBlockMap(void);

public:
	void SetDorbiereMode();

public:
	void SetConsolidateByTerminalFlag( bool b ){ m_bConsolidateByTerminal = b; }
	void SetConsolidateByLocationFlag( bool b ){ m_bConsolidateByLocation = b; }
	void SetConsolidateByDatabaseFlag( bool b ){ m_bConsolidateByDatabase = b; }
	void SetConsolidateBySystemFlag( bool b ){ m_bConsolidateBySystem = b; }

public:
	void BuildMap( CEposSelectArray& SelectArray );
	void WriteToFile();

public:
	int GetBlockMapEntityCount(){ return m_arrayBlockMapEntity.GetSize(); }
	void GetBlockMapEntity( int nIdx, CEposReportBlockMapEntity& entity );
	void GetBlockListForTerminal( int nSelArrayIdx, CArray<int,int>& BlockList );

public:
	int GetSortArraySize(){ return m_arraySortInfo.GetSize(); }
	int GetSortArrayIdx( int nIdx );
	void BuildSortArray();

private:
	void LinkTerminalsToBlock( int nNodeType );

private:
	CStringArray m_mapTerminalToBlock;
	CArray<CEposReportBlockMapEntity> m_arrayBlockMapEntity;
	CReportConsolidationArray<CEposReportBlockMapSortInfo> m_arraySortInfo;

private:
	CReportConsolidationArray<CEposReportBlockMapTempIndex> m_arrayTempIndex;
	CReportConsolidationArray<CSortedIntByString> m_arrayDuplicateBlockCheck;
	int m_nTempIdx;
	int m_nTempRpStart;
	int m_nTempLocStart;
	int m_nTempDbStart;

private:
	bool m_bConsolidateByTerminal;
	bool m_bConsolidateByLocation;
	bool m_bConsolidateByDatabase;
	bool m_bConsolidateBySystem;

private:
	int m_nSpecialMode;
};

/**********************************************************************/

