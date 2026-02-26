#pragma once
/**********************************************************************/

struct CSelectMultipleEntityInfo
{
public:
	CSelectMultipleEntityInfo();
	void Reset();

public:
	int m_nNodeType;
	int m_nDbIdx;
	int m_nNwkIdx;
	int m_nLocIdx;
	int m_nTermIdx;
	int m_nTNo;
	int m_nSpIdx;
	int m_nKbsIdx;
	int m_nKbNo;
	int m_nPasswordIdx;
	__int64 m_nPluNo;
	int m_nPromoNo;
	bool m_bSelected;
	CString m_strText[5];
};

/**********************************************************************/

struct CSelectMultipleEntityFeeder
{
public:
	CSelectMultipleEntityFeeder();
	void Reset();

public:
	int m_nDbNo;
	int m_nNwkNo;
	int m_nLocNo;
	int m_nSpNo;
	int m_nTermIdx;
};

/**********************************************************************/
