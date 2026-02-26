#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CPriceSetHeaderManager
{
public:
	CPriceSetHeaderManager();

public:
	void LoadHeaderForLocationEdit( int nLocIdx );
	void LoadHeaderForDatabaseEdit( int nDbIdx, int nSet );
	void CheckHeaderChanges ( bool& bAdded, bool& bRemoved, bool& bWasEmpty );

public:
	void UpdateLocationSpecificPriceSetFiles ( int nLocIdx, int nLocalPluSetCount );
	void CreateLocationSpecificPriceSetFiles ( int nLocIdx, int nLocalPluSetCount );
	void UpdateDatabasePriceSetFile ( int nDbIdx, int nSet );
	void CreateDatabasePriceSetFile( int nDbIdx, int nSet );

public:
	const char* GetNewHeader() { return m_strNewHeader; }
	void SetNewHeader( const char* szNewHeader ){ m_strNewHeader = szNewHeader; }

protected:
	CReportConsolidationArray<CSortedStringItem> m_arrayFields;
	CString m_strCurrentHeader;
	CString m_strNewHeader;
};

/**********************************************************************/

