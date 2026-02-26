#pragma once
/**********************************************************************/

struct CSelectLocInfo
{
public:
	CSelectLocInfo();

public:
	int m_nLocIdx;
	bool m_bSelected;
};

/**********************************************************************/

struct CSelectKbSetInfo
{
public:
	CSelectKbSetInfo();

public:
	int m_nNodeType;
	int m_nLocOrDbIdx;
	int m_nKbsIdx;
	int m_nKbNo;
	bool m_bSelected;
};

/**********************************************************************/
