#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CServerSelectionInfo
{
public:
	CServerSelectionInfo();
	void Reset();
	
public:
	int Compare ( CServerSelectionInfo& source, int nHint = 0 );
	void Add	( CServerSelectionInfo& source );
	
public:
	int m_nDbNo;
	int m_nNwkNo;
	int m_nLocNo;
	int m_nServerNo;
	CString m_strServerName;
	double m_dHourlyRate;
	int m_nDailyHours;
	bool m_bSelected;
	bool m_bMidnightShift;
};

/**********************************************************************/

class CServerSelectionList
{
public:
	CServerSelectionList(void);
	void BuildList();
	void ClearList();

public:
	bool FindServer( CServerSelectionInfo& info, int& nPos );
	
	void GetServerStatus( int nPos, CServerSelectionInfo& info );
	void SetServerStatus( int nPos, CServerSelectionInfo& info );
	
	void GetServerStatus( CServerSelectionInfo& info );
	void GetServerStatus( CServerSelectionInfo& info, int nDbNo, int nNwkNo, int nLocNo, int nServerNo );
	void SetServerStatus( CServerSelectionInfo& info );

	int GetSize() { return m_arrayServers.GetSize(); }

	bool GetUnknownServerFlag(){ return m_bUnknownServer; }
	void SetUnknownServerFlag( bool b ){ m_bUnknownServer = b; }

	int FindSortedName( int nServerNo, const char* szName );
	const char* GetSortedName( int nNameIdx );

	int GetServerNameIdx( int nLocIdx, int nServer );
	void GetServerConsolidationString( int nDbIdx, int nLocIdx, int nServerNo, CString& strServer );
	void ProcessServerLabel( CString& strLabel, int& nServerNo, CString& strServerName );

	const char* GetServerName( int nConLevel, int nEntityIdx, int nServerNo, bool bDeferredOnly );
	
private:
	CReportConsolidationArray<CServerSelectionInfo> m_arrayServers;
	bool m_bUnknownServer;

	CReportConsolidationArray<CSortedIntAndStringItem> m_arraySortedNames;
	CString m_strServerName;
};

/**********************************************************************/
