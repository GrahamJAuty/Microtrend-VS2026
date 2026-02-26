#pragma once
/**********************************************************************/

struct CSortedLocation
{
public:
	CSortedLocation();
	void Reset();

public:
	int Compare ( CSortedLocation& source, int nHint = 0 );
	void Add	( CSortedLocation& source );
	
public:
	int m_nDbNo;
	int m_nBlock;
	CString m_strName;
	int m_nIdx;

public:
	bool m_bSelected;
};

/**********************************************************************/
