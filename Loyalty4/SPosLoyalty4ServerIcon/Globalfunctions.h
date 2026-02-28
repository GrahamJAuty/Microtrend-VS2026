#pragma once
//*******************************************************************

void GetPCPathHost(CString& strPath);
void GetPCPathClient(CString& strPath);
void GetDataProgramPath(CString& strPath);
CString GetTidyAppName(bool bDummy);
void CreateSubdirectory(const char* szDir);

CString GetRunOnceFilename();
CString CreateRunOnceTimeStamp();

//*******************************************************************
