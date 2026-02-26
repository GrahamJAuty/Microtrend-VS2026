#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CommsErrors.h"
#include "Consolidation.h"
#include "JobListCSVArray.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CDownloadExclusionItem
{
public:
	CDownloadExclusionItem();
	void Reset();

public:
	int Compare ( CDownloadExclusionItem& source, int nHint = 0 );
	void Add	( CDownloadExclusionItem& source );
	
public:
	int m_nDownloadType;
	int m_nSPOSVersion;
	
public:
	int m_nCount;
};

/**********************************************************************/

class CCommsExportBase
{
public:
	CCommsExportBase( int nNetworkIdx, int nLocIdx, int nTerminalNo, bool bImmediate, bool bFromMemory );
	CCommsExportBase( CJobListCSVRecord& JobInfo, bool bFromMemory );

private:
	void Init( int nNetworkIdx, int nLocIdx, int nTerminalNo, bool bImmediate, bool bFromMemory );

public:
	const char* GetSourceFolder(){ return m_strSourceFolder; }
	const char* GetSourceFilename(){ return m_strSourceFilename; }
	const char* GetDestFolder(){ return m_strDestFolder; }

public:
	virtual int PrepareFile(){ return COMMS_ERROR_NODATA; }

public:
	static bool CheckPluNo( int nSPOSVersion, __int64 nSSPluNo, __int64& nSharpPluNo, bool& bModifiable );
	
public:
	void TallyDownloadExclusion( int nDownloadType, int nSPOSVersion, int nCount = 1 );
	void TallyDownloadAccept( int nCount = 1 );
	int GetDownloadExclusionCount(){ return m_arrayDownloadExclusions.GetSize(); }
	void GetDownloadExclusion( int n, CDownloadExclusionItem& item );

protected:
	int m_nNetworkIdx;
	int m_nLocIdx;
	int m_nTerminalNo;
	bool m_bImmediate;
	bool m_bFromMemory;

protected:
	CString m_strSourceFolder;
	CString m_strSourceFilename;
	CString m_strDestFolder;
	
protected:
	CReportConsolidationArray<CDownloadExclusionItem> m_arrayDownloadExclusions;
};

/**********************************************************************/
#endif
/**********************************************************************/
