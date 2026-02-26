#pragma once

class CFilenameHandler
{
public:
	CFilenameHandler(){};

public:
	CString GetServerDataFilename() { return GetSysFilesNumericFilename(1); }					//FILE0001
	CString GetSystemFilenameOld() { return GetSysFilesNumericFilename(7); }					//FILE0007
	CString GetSystemFilename() { return GetSysFilesNumericFilename(1007); }					//FILE1007
	//24 WAS MIFARE DATABASE FILENAME
	CString GetPresentationOptionsFilename() { return GetSysFilesNumericFilename(25); }			//FILE0025
	CString GetServerServiceOptionsFilename() { return GetSysFilesNumericFilename(28); }		//FILE0028
	CString GetServerServiceOptionsLockFilename() { return GetSysFilesNumericFilename(29); }	//FILE0029
	CString GetServerServiceStopFilename() { return GetSysFilesNumericFilename(30); }			//FILE0030
	CString GetServerServiceActionTimeFilename() { return GetSysFilesNumericFilename(31); }		//FILE0031
	CString GetServerServiceErrorTimeFilename() { return GetSysFilesNumericFilename(32); }		//FILE0032
	
public:
	CString GetPCOptionsClientFilename() { return GetHostPCNumericFilename(5001); }				//FILE5001
	
public:
	CString GetServerLogFolder();
	CString GetServerLogFilename();
	CString GetTrayLogFilename();
	CString GetServerLogParamsFilename();
	//CString GetRecordInuseFilename(const char* szCardNo);
	//CString GetFamilyPath(const char* szAccount = "");

private:
	CString GetSysFilesNumericFilename(int nFileNum, CString strSuffix = "");
	CString GetHostPCNumericFilename(int nFileNum, CString strSuffix = "" );
	CString GetHostPCNumericFilenameOld(int nFileNum, CString strSuffix = "");
	CString GetFilename(const char* szFolder, const char* szFilename);
};

//*******************************************************************
extern CFilenameHandler Filenames;
extern CSysset Sysset;
//*******************************************************************
