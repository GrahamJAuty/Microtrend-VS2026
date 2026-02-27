#pragma once

class CFilenameHandler
{
public:
	CFilenameHandler(){};

public:
	CString GetBgndServiceOptionsFilename() { return GetSysFilesNumericFilename(64); }		//FILE0064
	CString GetBgndServiceOptionsLockFilename() { return GetSysFilesNumericFilename(65); }	//FILE0065
	CString GetBgndServiceStopFilename() { return GetSysFilesNumericFilename(66); }			//FILE0066
	//FILE0067 UNUSED
	CString GetServerServiceOptionsFilename() { return GetSysFilesNumericFilename(68); }		//FILE0068
	CString GetServerServiceOptionsLockFilename() { return GetSysFilesNumericFilename(69); }	//FILE0069
	CString GetServerServiceStopFilename() { return GetSysFilesNumericFilename(70); }			//FILE0070
	//FILE0071 UNUSED
	CString GetBgndServiceActionTimeFilename() { return GetSysFilesNumericFilename(72); }	//FILE0072
	CString GetServerServiceActionTimeFilename() { return GetSysFilesNumericFilename(73); }	//FILE0073
	CString GetBgndServiceErrorTimeFilename() { return GetSysFilesNumericFilename(74); }	//FILE0074
	CString GetServerServiceErrorTimeFilename() { return GetSysFilesNumericFilename(75); }	//FILE0075
	CString GetBgndServiceBusyTimeFilename() { return GetSysFilesNumericFilename(91); }		//FILE0091

public:
	CString GetPCOptionsClientFilename() { return GetClientPCNumericFilename(5001); }		//FILE5001
	
public:
	CString GetServerLogFolder();
	CString GetServerLogFilename();
	CString GetServerLogParamsFilename();
	CString GetBackgroundLogFilename();
	CString GetBackgroundLogParamsFilename();

public:
	CString GetBackgroundLogFolder();

public:
	CString GetTrayLogFilename();

private:
	CString GetSysFilesNumericFilename(int nFileNum, CString strSuffix = "");
	CString GetClientPCNumericFilename(int nFileNum, CString strSuffix = "" );
	CString GetFilename(const char* szFolder, const char* szFilename);
};

//*******************************************************************
extern CFilenameHandler Filenames;
extern CSysset Sysset;
//*******************************************************************
