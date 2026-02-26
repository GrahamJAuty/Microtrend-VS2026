#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "JobListCSVArray.h"
/**********************************************************************/

class CCommsExportButtonImages 
{
public:
	CCommsExportButtonImages( CJobListCSVRecord& JobInfo );
	const char* GetSourcePath() { return m_strSourcePath; }
	const char* GetDestPath() { return m_strDestPath; }

public:
	int PrepareFile();
	int GetFilenameCount(){ return m_arrayFilenames.GetSize(); }
	const char* GetFilename( int n );

private:
	void PrepareFileList();

private:
	CString m_strSourcePath;
	CString m_strDestPath;
	int m_nNetworkIdx;
	int m_nLocIdx;
	int m_nTerminalNo;
	bool m_bImmediate;

private:
	CStringArray m_arrayFilenames;
	CString m_strFilename;
};

/**********************************************************************/
#endif
/**********************************************************************/

