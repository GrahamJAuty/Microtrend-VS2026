#pragma once
/**********************************************************************/

struct CDataNodeInfoKeyboard
{
public:
	CDataNodeInfoKeyboard();

public:
	bool Compare( CDataNodeInfoKeyboard& source );
	void Clear();

public:
	int m_nSPOSVersion;
	int m_nNodeType;
	int m_nDbIdx;
	int m_nLocIdx;
	int m_nKbsIdx;
	int m_nKbNo;
};

/**********************************************************************/