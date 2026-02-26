#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "JobListCSVArray.h"
#include "PluInfoResourceWrapper.h"
/**********************************************************************/

class CCommsExportPluInfo 
{
public:
	CCommsExportPluInfo( CJobListCSVRecord& JobInfo, bool bFromMemory );
	const char* GetSourcePath() { return m_strSourcePath; }
	const char* GetDestPath() { return m_strDestPath; }
	
public:
	int PrepareFile( int nParam1 );
	int GetFileInfoCount(){ return m_arrayFileInfo.GetSize(); }
	void GetFileInfo( int n, CPluInfoDownloadHelper& item );
	int GetResourceInfoCount(){ return m_arrayResourceInfo.GetSize(); }
	void GetResourceInfo( int n, CPluInfoDownloadHelper& item );

private:
	void PrepareFileList( int nSPOSVersion, int nParam1 );
	
private:
	int m_nNetworkIdx;
	int m_nLocIdx;
	int m_nTerminalNo;
	bool m_bImmediate;
	bool m_bFromMemory;

private:
	CArray<CPluInfoDownloadHelper> m_arrayFileInfo;
	CArray<CPluInfoDownloadHelper> m_arrayResourceInfo;
	CString m_strFilename;

private:
	CString m_strEffectiveDate;

private:
	CString m_strSourcePath;
	CString m_strDestPath;
};

/**********************************************************************/
#endif
/**********************************************************************/

