#pragma once
/**********************************************************************/

class CSQLGroupShiftChecker 
{
public:
	CSQLGroupShiftChecker();

public:
	void CheckGroupShifts();

private:
	COleDateTime m_oleDateLastCheck;
};

/**********************************************************************/
extern CSQLGroupShiftChecker SQLGroupShiftChecker;
/**********************************************************************/
