#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "JobListCSVArray.h"
#include "KeyboardMap.h"
/**********************************************************************/

class CCommsExportKeyboard 
{
public:
	CCommsExportKeyboard( CJobListCSVRecord& JobInfo, int nKbNo, CKeyboardMap& KeyboardMap );
	
public:
	int PreparePaths();
	const char* GetSourcePath() { return m_strSourcePath; }
	const char* GetDestPath() { return m_strDestPath; }
	
private:
	int m_nNetworkIdx;
	int m_nLocIdx;
	int m_nTerminalNo;
	int m_nKbNo;
	bool m_bImmediate;
	CKeyboardMap& m_KeyboardMap;

private:
	CString m_strSourcePath;
	CString m_strDestPath;
};

/**********************************************************************/
#endif
/**********************************************************************/
