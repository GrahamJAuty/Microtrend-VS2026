/**********************************************************************/
#pragma once
/**********************************************************************/

struct CKeyboardJobInfo
{
public:
	CKeyboardJobInfo();
	int GetJobNo();
	int GetKbNoFromJobNo( int nJobNo );

public:
	int m_nLocIdx;
	int m_nKbNo;
};

/**********************************************************************/
