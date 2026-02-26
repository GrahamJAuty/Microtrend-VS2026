#pragma once
/**********************************************************************/

class CUnderlineMaker
{
public:
	CUnderlineMaker();
	void Reset();

public:
	void AddBlanks( int n );
	void AddTotals( int n );
	void AddColumns( bool bTotals, int n );
	const char* GetLine();

private:
	int m_nPendingCount;
	bool m_bTotalsSection;
	CArray<int,int> m_arraySections;
	
private:
	CString m_strLine;
};

/**********************************************************************/