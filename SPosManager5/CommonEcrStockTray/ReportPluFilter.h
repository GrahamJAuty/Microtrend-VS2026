#pragma once
/**********************************************************************/
#include "GlobalFunctions.h"
/**********************************************************************/

class CReportPluFilter
{
public:
	CReportPluFilter();

public:
	int GetCurrentFilterNo(){ return m_nCurrentFilterNo; }

public:
	void SetConsolidationType( int nConType );
	void SetMasterFilterNo( int nFilter );
	void SetDatabase( int nDbIdx );
	
public:
	bool CheckPlu( CSQLPluNoInfo& infoPlu );
	
private:
	bool m_bSystemPluFilter;
	bool m_bSystemPluFilterByName;
	CString m_strSystemPluFilterName;
	int m_nMasterFilterNo;
	int m_nCurrentFilterNo;
};

/**********************************************************************/

