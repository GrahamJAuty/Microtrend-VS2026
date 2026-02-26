#pragma once
/**********************************************************************/
#include "EposSelectArray.h"
#include "FNBDatabaseOptions.h"
#include "..\CommonEcrStock\KeyboardMap.h"
#include "NodeTypes.h"
#include "PosTrayTask.h"
#include "S4LabourLocationOptions.h"
/**********************************************************************/

class CLocationSelectorMiniLookup
{
public:
	CLocationSelectorMiniLookup();
	CLocationSelectorMiniLookup( int nConType, int nEntityNo );
	
public:
	void Reset();

public:
	int GetConType(){ return m_nConType; }
	int GetEntityNo(){ return m_nEntityNo; }
	
protected:
	int m_nConType;
	int m_nEntityNo;
};

/**********************************************************************/

class CLocationSelectorLookup : public CLocationSelectorMiniLookup
{
public:
	CLocationSelectorLookup();
	CLocationSelectorLookup( CPosTrayTask& Task );
	CLocationSelectorLookup( int nDbIdx, CFNBDatabaseOptions& Options );
	
public:
	void Reset();

public:
	void SetConTypeToLocation(){ m_nConType = NODE_LOCATION; }

public:
	int GetDbNo(){ return m_nDbNo; }
	int GetNwkNo(){ return m_nNwkNo; }
	int GetTermIdx(){ return m_nTermIdx; }
	
protected:
	int m_nDbNo;
	int m_nNwkNo;
	int m_nTermIdx;
};

/**********************************************************************/

class CLocationSelectorEntity : public CLocationSelectorLookup
{
public:
	CLocationSelectorEntity();
	CLocationSelectorEntity( CLocationSelectorLookup& LocSelLookup );
	bool SimpleCompare( CLocationSelectorEntity& other );
	
public:
	void Reset();

public:
	void SetSystemNode();
	void SetSystemFilterNode();
	void SetTimeSettingNode();
	void SetReportListNode();
	void SetDatabaseNode( int nDbIdx );
	void SetLocationNode( int nLocIdx );
	void SetTerminalNode( int nTermIdx );
	void SetNetworkNode( int nNwkIdx );
	void SetReportpointNode( int nRpIdx );
	void SetLocationSetNode( int nLsIdx );
	void SetKeyboardMapNode( int nMapIdx );
	void SetInvalidNode();

#ifdef STOCKMAN_SYSTEM
	void SetStockpointNode( int nSpIdx );
	void SetSiteNode( int nSiteIdx );
#endif

public:
	int GetDbIdx(){ return m_nDbIdx; }
	int GetEntityIdx(){ return m_nEntityIdx; }
	const char* GetEntityName(){ return m_strEntityName; }

public:
	bool IsInvalid(){ return NODE_PLACEHOLDER == m_nConType; }
	const char* GetConsolidationTitle( bool bBrackets = FALSE );
	void GetEPOSSelectArray( CEposSelectArray& SelectArray );
	
private:
	int m_nDbIdx;
	int m_nEntityIdx;
	CString m_strEntityName;
	CString m_strConsolidationTitle;
};

/**********************************************************************/
