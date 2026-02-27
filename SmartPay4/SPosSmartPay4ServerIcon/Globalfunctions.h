#pragma once
//*******************************************************************

bool FileExists(const char* szFile);
void TrimSpacesFromString(CString& strText, bool bRemoveLeadingZeros = FALSE );
void GetPCPathClient(CString& strPath);
void GetDataProgramPath(CString& strPath);
CString GetTidyAppName(bool bDummy);

CString GetRunOnceFilename();
CString CreateRunOnceTimeStamp();
CString GetSimpleDateTimeNowString();

void CreateSubdirectory(const char* szDir);

void GetMessageLogDateTime(CString& strDate, CString& strTime);

//*******************************************************************
