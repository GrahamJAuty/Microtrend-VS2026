#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CSPOSKeyboardImageLibrary
{
public:
	CSPOSKeyboardImageLibrary(void);
	void Reset();
	
public:
	void Refresh( bool bForce );
	int GetImageCount(){ return m_arrayImageFilenames.GetSize(); }

public:
	const char* GetFilename( int nIdx );
	bool FindFilename( const char* sz, int& nIdx );
	void AddFilename( const char* sz );
	
private:
	bool m_bGotFirstRefresh;
	CString m_strImageFilename;
	CReportConsolidationArray<CSPOSKeyboardSortedString> m_arrayImageFilenames;
};

/**********************************************************************/
