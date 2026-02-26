#pragma once
/**********************************************************************/
 
/**********************************************************************/

struct CNetworkTerminalInfo
{
	int m_nNwkNo;
	int m_nTNo;
	int m_nLocIdx;
	int m_nUseCount;
};

/**********************************************************************/

class CNetworkTerminalMap 
{
public:
	CNetworkTerminalMap();

public:
	void BuildMap();
	
public:
	void AddTerminal( int nNwkNo, int nTNo, int nLocIdx );
	void RemoveTerminal( int nNwkNo, int nTNo );
	void RemoveNetwork( int nNwkNo );

public:
	int GetLocIdx( int nNwkNo, int nTNo );
	int FindFreeTerminalNumber( int nNwkNo );
	void IncrementLocIdx( int nLocIdxFrom );
	void DecrementLocIdx( int nLocIdxFrom );

private:
	bool FindEntry( CNetworkTerminalInfo& info, int& nIdx );
	void FiddleWriteMap( const char* szWhy );
	
private:
	CArray<CNetworkTerminalInfo,CNetworkTerminalInfo> m_arrayMap;
};

/**********************************************************************/
extern CNetworkTerminalMap NetworkTerminalMap;
/**********************************************************************/
