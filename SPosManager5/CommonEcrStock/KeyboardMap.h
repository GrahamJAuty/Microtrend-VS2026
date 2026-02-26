#pragma once
/**********************************************************************/
#define KBD_MAP_NULL 0
#define KBD_MAP_LOC 1
#define KBD_MAP_SET 2
/**********************************************************************/

struct CKeyboardMapInfo
{
public:
	CKeyboardMapInfo();
	//bool Compare( CKeyboardMapInfo& test );

public:
	int m_nSPOSVersion;
	int m_nEntryType;
	int m_nDbIdx;
	int m_nLocIdx;
	int m_nKbsIdx;
};

/**********************************************************************/

class CKeyboardMap  
{
public:
	CKeyboardMap();

public:
	void ClearMap();
	void BuildMap();

public:
	int GetMapInfoCount(){ return m_arrayMapInfo.GetSize(); }
	void GetMapInfo( int nIdx, CKeyboardMapInfo& info );

public:
	int GetLocInfoCount(){ return m_arrayLocInfo.GetSize(); }
	void GetLocInfo( int nIdx, CKeyboardMapInfo& info );

private:
	void BuildMapDatabase( int nDbIdx );

private:
	bool m_bIsBuilt;
	CArray<CKeyboardMapInfo,CKeyboardMapInfo> m_arrayMapInfo;
	CArray<CKeyboardMapInfo,CKeyboardMapInfo> m_arrayLocInfo;
	CArray<int,int> m_arrayKbSetInfo;
};

/**********************************************************************/

