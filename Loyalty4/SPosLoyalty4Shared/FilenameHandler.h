#pragma once

class CFilenameHandler
{
public:
	CFilenameHandler(){};

public:
	CString GetServerDataFilename() { return GetSysFilesNumericFilename(1); }					//FILE0001
	//2 WAS DATABASE FILENAME
	CString GetColumnHandlerFilename() { return GetSysFilesNumericFilename(3); }				//FILE0003
	CString GetPasswordFilename() { return GetSysFilesNumericFilename(4); }						//FILE0004
	CString GetAuditFilename() { return GetSysFilesNumericFilename(5); }						//FILE0005
	CString GetLegacyCCNoFilename() { return GetSysFilesNumericFilename(6); }					//FILE0006
	CString GetSystemFilenameOld2() { return GetSysFilesNumericFilename(7); }					//FILE0007
	CString GetSystemFilenameOld1() { return GetSysFilesNumericFilename(1007); }				//FILE1007
	CString GetSystemFilenameNew() { return GetSysFilesNumericFilename(2007); }					//FILE2007
	//8 WAS SESSION AUDIT FILENAME
	//9 WAS PLU DATABASE FILENAME
	//10 WAS PLU DATABASE FILENAME
	CString GetLegacyPeriodDataFilename() { return GetSysFilesNumericFilename(11); }			//FILE0011
	CString GetExportHandlerFilename() { return GetSysFilesNumericFilename(12); }				//FILE0012
	CString GetReportDefaultHandlerFilename() { return GetSysFilesNumericFilename(13); }		//FILE0013
	CString GetPluDatabaseListFilename(int nDbNo);												//FILE0014 and FILE0042 to FILE0050
	CString GetUserTextsFilename() { return GetSysFilesNumericFilename(15); }					//FILE0015
	CString GetPhotoIDListFilename() { return GetSysFilesNumericFilename(16); }					//FILE0016
	//17 WAS GET SERVER VERSION FILENAME
	CString GetDBImportExceptionsFilename() { return GetSysFilesNumericFilename(18); }			//FILE0018
	CString GetTerminalListFilename() { return GetSysFilesNumericFilename(19); }				//FILE0019
	CString GetDBaseCommentsFilename() { return GetSysFilesNumericFilename(20); }				//FILE0020
	CString GetAuditCommentsFilename() { return GetSysFilesNumericFilename(21); }				//FILE0021
	CString GetAlertTextFilename() { return GetSysFilesNumericFilename(22); }					//FILE0022
	//23 WAS READER FILENAME / MOVED TO CLIENT PC FOLDER
	//24 WAS MIFARE DATABASE FILENAME
	CString GetPresentationOptionsFilename() { return GetSysFilesNumericFilename(25); }			//FILE0025
	CString GetEmailOptionsFilename() { return GetSysFilesNumericFilename(26); }				//FILE0026
	CString GetEmailAddressesFilename() { return GetSysFilesNumericFilename(27); }				//FILE0026
	CString GetServerServiceOptionsFilename() { return GetSysFilesNumericFilename(28); }		//FILE0028
	CString GetServerServiceOptionsLockFilename() { return GetSysFilesNumericFilename(29); }	//FILE0029
	CString GetServerServiceStopFilename() { return GetSysFilesNumericFilename(30); }			//FILE0030
	CString GetServerServiceActionTimeFilename() { return GetSysFilesNumericFilename(31); }		//FILE0031
	CString GetServerServiceErrorTimeFilename() { return GetSysFilesNumericFilename(32); }		//FILE0032
	CString GetTemporaryLoyaltyRecordFilename() { return GetSysFilesNumericFilename(33); }		//FILE0033
	//34 WAS EXTERNAL ACCOUNT LINK FILENAME
	CString GetBlockedPosPointsFilename() { return GetSysFilesNumericFilename(35); }			//FILE0035
	CString GetSQLScriptLegacyFilename() { return GetSysFilesNumericFilename(36); }				//FIL00036
	CString GetAuditPeriodTokenFilename() { return GetSysFilesNumericFilename(37); }			//FILE0037
	CString GetSQLScriptFallbackFilename() { return GetSysFilesNumericFilename(38); }			//FIL00038
	CString GetSQLExceptionsLogFilename() { return GetSysFilesNumericFilename(39); }			//FIL00039
	CString GetServerUpdatePluReqFilename() { return GetSysFilesNumericFilename(40); }			//FILE0040
	CString GetServerUpdatePluRspFilename() { return GetSysFilesNumericFilename(41); }			//FILE0041
	//FILE0042 to FILE0050 are PLU DATABASE FILENAMES

	CString GetSQLBackupFilename() { return GetSysFilesFilename("LOY_SQLBackup.bak"); }

	CString GetSQLConnectionOptionsFilenameOld() { return GetSysFilesFilename("SQLConnect.ini"); }
	CString GetSQLConnectionOptionsFilenameNew();

public:																								
	CString GetPCOptionsClientFilename() { return GetClientPCNumericFilename(5001); }				//FILE5001
	CString GetPCOptionsHostFilename() { return GetHostPCNumericFilename(5002); }					//FILE6001

public:
	CString GetBackupOptionsFilename() { return GetSysFilesSyssetNumericFilename(1); }			//FILE0001

public:
	CString GetPurchaseHistoryUpdateFilename ( const char* szCardNo );
	CString GetPurchaseHistoryFilename ( const char* szCardNo );

	CString GetReportCardListFilename ( const char* szLabel );
	CString GetReportParamsFilename ( const char* szLabel );
	CString GetReportFilename ( const char* szLabel );

	CString GetAtcArchivePath ( const char* szFilename = "" );

	CString GetExportsPath ( const char* szFilename = "" );
	CString GetImportsPath ( const char* szFilename = "" );
	CString GetFamilyPath ( const char* szAccount = "" );

	CString GetRecordEditFilename ( const char* szCardNo );
	CString GetRecordInuseFilename ( const char* szCardNo );
	CString GetColumnsFilename( int nFileIdx );

	/*****/
	CString GetServerLogFolder();
	CString GetServerLogFilename();
	CString GetServerOldFilename();
	CString GetServerLogParamsFilename();
	
public:
	CString GetServicesFolder();
	CString GetServicesServerOuterLockFolder();
	CString GetServicesServerInnerLockFolder();
	CString GetServicesVersionFilename();
	CString GetLogCopyFolder();

public:
	CString GetUserLogFolder() { return GetUserLogFilename(""); }
	CString GetUserLogFilename(const CString strFilename);
	CString GetUserLogReportFilename() { return GetUserLogFilename("UserLog.rep"); }
	CString GetUserLogParamsFilename() { return GetUserLogFilename("UserLog.prm"); }

public:
	CString GetStampOfferLogFolder() { return GetStampOfferLogFilename(""); }
	CString GetStampOfferLogFilename(const CString strFilename);

public:
	CString GetBackupLogToWriteFilename();
	CString GetBackupLogToWriteFolder();
	CString GetBackupLogToReadFilename();
	CString GetBackupLogParamsFilename();
	
public:
	CString GetSyssetFilename(const char* szFolder, const char* szFilename);
	void CreateSyssetSubdirectory(CString strDir);
	
private:
	CString GetSysFilesSyssetNumericFilename(int nFileNum, CString strSuffix = "");

public:
	CString GetFilename(const char* szFolder, const char* szFilename = "");
	CString GetTempPathFilename(const char* szFilename);
	CString GetSysFilesFilename(const CString strFilename);

public:
	CString GetLogBufferFolder();
	void CreateLogBufferFolder();

private:
	CString GetSysFilesNumericFilename(int nFileNum, CString strSuffix = "" );
	CString GetHostPCNumericFilename(int nFileNum, const CString strSuffix = "");
	CString GetHostPCNumericFilenameOld(int nFileNum, const CString strSuffix = "");
	CString GetClientPCNumericFilename(int nFileNum, const CString strSuffix = "");
	CString GetClientPCNumericFilenameOld(int nFileNum, const CString strSuffix = "");
};

//*******************************************************************
extern CFilenameHandler Filenames;
extern CSysset Sysset;
//*******************************************************************
