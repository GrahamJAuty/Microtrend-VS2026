#pragma once
//*******************************************************************
#define szFILENAME_IMPORTCSV		"cardxfer.csv"
#define szFILENAME_IMPORTSIMS		"simsxfer.csv"
#define szFILENAME_IMPORTTOPUP		"topuxfer.csv"
#define szFILENAME_IMPORTWONDE		"wonde.json"
//*******************************************************************

class CFilenameHandler
{

public:
	CFilenameHandler() {};

public:
	CString GetSystemFilenameOld() { return GetSysFilesNumericFilename(1007); }			//FILE1007
	CString GetEmailOptionsFilename() { return GetSysFilesNumericFilename(59); }		//FILE0059
	CString GetEmailAddressesFilename() { return GetSysFilesNumericFilename(60); }		//FILE0060
	CString GetEmailLockFilename() { return GetSysFilesNumericFilename(61) + ".001"; }	//FILE0061
	CString GetEmailLogRetryTokenFilename() { return GetSysFilesNumericFilename(62); }	//FILE0062
	CString GetEmailPendingCountFilename() { return GetSysFilesNumericFilename(63); }	//FILE0063
	CString GetSystemFilenameNew() { return GetSysFilesNumericFilename(90); }			//FILE0090

public:
	CString GetEmailLogAllFilename() { return GetEmailLogFilename("EmailLog.rep"); }
	CString GetEmailLogOldFilename() { return GetEmailLogFilename("EmailLog.old"); }

public:
	CString GetFilename(const CString strFolder, const CString strFilename = "");

private:
	CString GetSysFilesNumericFilename(int nFileNum, const CString strSuffix = "");

private:
	CString GetEmailLogFilename(const CString strFilename);

#ifndef SYSTEMTYPE_EMAILBATCHSEND
	
public:
	CString GetSQLDatabaseFilename();
	CString GetServerDataFilename(){ return GetSysFilesNumericFilename(1); }			//FILE0001
	//FILE0002 = UNUSED (WAS DATABASE FILE)
	CString GetColumnHandlerFilename(){ return GetSysFilesNumericFilename(3); }			//FILE0002
	CString GetPasswordFilename(){ return GetSysFilesNumericFilename(4); }				//FILE0004
	CString GetAuditFilename(){ return GetSysFilesNumericFilename(5); }					//FILE0005
	CString GetLegacyCCNoFilename(){ return GetSysFilesNumericFilename(6); }			//FILE0006
	//FILE1007 = SYSTEM DATA (ABOVE)
	//FILE0008 = UNUSED (SESSION EDIT)
	//FILE0009 = UNUSED (WAS GROUP FILENAME)
	//FILE0010 = UNUSED (WAS PLU FILENAME)
	CString GetAuditPeriodTokenFilename(){ return GetSysFilesNumericFilename(11); }		//FILE0011
	CString GetExportHandlerFilename(){ return GetSysFilesNumericFilename(12); }		//FILE0012
	CString GetReportDefaultHandlerFilename(){ return GetSysFilesNumericFilename(13); }	//FILE0013
	CString GetPluRankingListFilename(){ return GetSysFilesNumericFilename(14); }		//FILE0014
	//FILE0015 = UNUSED (WAS USER TEXTS FILENAME)
	CString GetPhotoIDListFilename(){ return GetSysFilesNumericFilename(16); }			//FILE0016
	CString GetServerVersionFilename(){ return GetSysFilesNumericFilename(17); }		//FILE0017
	CString GetDBImportExceptionsFilename(){ return GetSysFilesNumericFilename(18); }	//FILE0018
	//FILE0019 = UNUSED (ALLERGY)
	CString GetAlertFilename(){ return GetSysFilesNumericFilename(20); }				//FILE0020
	CString GetGroupSetFilename(){ return GetSysFilesNumericFilename(21); }				//FILE0021
	CString GetUserTypeFilename(){ return GetSysFilesNumericFilename(22); }				//FILE0022
	//FILE0023 = UNUSED (WAS MEMBER DBASE FILENAME)
	CString GetLegacyEposTerminalFilename(){ return GetSysFilesNumericFilename(24); }	//FILE0024
	CString GetAuditCommentsFilename(){ return GetSysFilesNumericFilename(25); }		//FILE0025
	CString GetDBaseCommentsFilename(){ return GetSysFilesNumericFilename(26); }		//FILE0026
	CString GetTerminalListFilename(){ return GetSysFilesNumericFilename(27); }			//FILE0027
	//FILE0028 = UNUSED (WAS MEMBER2 DBASE FILENAME)
	CString GetLegacyWebPaymentOptionsFilename(){ return GetSysFilesNumericFilename(29); }	//FILE0029
	CString GetCSVDatabaseImportDataFilename(){ return GetSysFilesNumericFilename(30); }//FILE0030
	CString GetSIMSImportDataFilename(){ return GetSysFilesNumericFilename(31); }		//FILE0031
	CString GetBackgroundOptionsFilename(){ return GetSysFilesNumericFilename(32); }	//FILE0032
	CString GetCSVImportDataFilename(){ return GetSysFilesNumericFilename(33); }		//FILE0033
	//FILE0034 = UNUSED (READER)
	//FILE0035 = UNUSED (WAS MIFARE DBASE FILENAME, FILE0035a WAS MIFARE DBASE2)
	//FILE0036 = UNUSED (WAS CLOSING BALANCE FILENAME)
	CString GetEODTextsFilename(){ return GetSysFilesNumericFilename(37); }				//FILE0037
	//FILE0038 = UNUSED (RECORD EDIT FILENAME)
	CString GetEmailAlertDataFilename(){ return GetSysFilesNumericFilename(39); }		//FILE0039
	CString GetEmailLogLowBalanceFilename(){ return GetSysFilesNumericFilename(40); }	//FILE0040
	CString GetEmailLogPurchaseFilename(){ return GetSysFilesNumericFilename(41); }		//FILE0041
	CString GetEmailLogPointsFilename(){ return GetSysFilesNumericFilename(42); }		//FILE0042
	CString GetEmailLogWeeklyPointsFilename(){ return GetSysFilesNumericFilename(43); }	//FILE0043
	CString GetTerminalSetFilename(){ return GetSysFilesNumericFilename(44); }			//FILE0044
	CString GetTerminalGroupSetFilename(){ return GetSysFilesNumericFilename(45); }		//FILE0045
	//FILE0046 = UNUSED (WAS REFRESH DATABASE FILENAME)
	CString GetCustomImportFieldsFilename(){ return GetSysFilesNumericFilename(47); }	//FILE0047
	//FILE0048 = UNUSED (WAS BIOREGISTER FILENAME)
	CString GetWondeDataFilename(){ return GetSysFilesNumericFilename(49); }			//FILE0049
	CString GetHiddenAuditFilename(){ return GetSysFilesNumericFilename(50); }			//FILE0050
	//FILE0051-n = (USED TO HOLD INFO SELECTIONS)
	//FILE0052 = UNUSED (ERROR LOG)
	CString GetPaymentAuditFilename(){ return GetSysFilesNumericFilename(53); }			//FILE0053
	CString GetPaymentTypeFilename(){ return GetSysFilesNumericFilename(54); }			//FILE0054
	CString GetPaymentGroupFilename(){ return GetSysFilesNumericFilename(55); }			//FILE0055
	CString GetPaymentListFilename(){ return GetSysFilesNumericFilename(56); }			//FILE0056
	CString GetAllergyDatabaseFilename(){ return GetSysFilesNumericFilename(57); }		//FILE0057
	CString GetPresentationOptionsFilename(){ return GetSysFilesNumericFilename(58); }	//FILE0058
	//FILE0059 = EMAIL OPTIONS (ABOVE)
	//FILE0060 = EMAIL ADDRESSES (ABOVE)
	//FILE0061 = EMAIL LOCK FILENAME (ABOVE)
	//FILE0062 = EMAIL LOG RETRY FILENAME (ABOVE)
	//FILE0063 = EMAIL PENDING COUNT FILENAME (ABOVE)
	CString GetBgndServiceOptionsFilename(){ return GetSysFilesNumericFilename(64); }		//FILE0064
	CString GetBgndServiceOptionsLockFilename(){ return GetSysFilesNumericFilename(65); }	//FILE0065
	CString GetBgndServiceStopFilename(){ return GetSysFilesNumericFilename(66); }			//FILE0066
	//FILE0067 UNUSED
	CString GetServerServiceOptionsFilename(){ return GetSysFilesNumericFilename(68); }		//FILE0068
	CString GetServerServiceOptionsLockFilename(){ return GetSysFilesNumericFilename(69); }	//FILE0069
	CString GetServerServiceStopFilename(){ return GetSysFilesNumericFilename(70); }		//FILE0070
	//FILE0071 UNUSED
	CString GetBgndServiceActionTimeFilename(){ return GetSysFilesNumericFilename(72); }	//FILE0072
	CString GetServerServiceActionTimeFilename(){ return GetSysFilesNumericFilename(73); }	//FILE0073
	CString GetBgndServiceErrorTimeFilename() { return GetSysFilesNumericFilename(74); }	//FILE0074
	CString GetServerServiceErrorTimeFilename() { return GetSysFilesNumericFilename(75); }	//FILE0075
	//FILE0076 UNUSED WAS EXTERNAL ACCOUNT LINK FILENAME
	CString GetEODRefreshDateFilename() { return GetSysFilesNumericFilename(77); }			//FILE0077
	CString GetSQLExceptionsLogFilenameBromCom() { return GetSysFilesNumericFilename(78); }	//FILE0078
	CString GetSQLExceptionsOldFilenameBromCom() { return GetSysFilesNumericFilename(79); }	//FILE0079
	CString GetSQLScriptLegacyFilename() { return GetSysFilesNumericFilename(80); }			//FILE0080
	CString GetSQLScriptFallbackFilename() { return GetSysFilesNumericFilename(81); }		//FILE0081
	CString GetGroupShiftLogFilename() { return GetSysFilesNumericFilename(82); }			//FILE0082
	CString GetSQLExceptionsLogFilenameGeneral() { return GetSysFilesNumericFilename(83); }	//FILE0083
	CString GetServerUpdatePluReqFilename() { return GetSysFilesNumericFilename(84); }		//FILE0084
	CString GetServerUpdatePluRspFilename() { return GetSysFilesNumericFilename(85); }		//FILE0085
	CString GetRestErrorLogFilename() { return GetSysFilesNumericFilename(86); }			//FILE0086
	CString GetPebbleJSONDumpFilename() { return GetSysFilesNumericFilename(87); }			//FILE0087
	CString GetPebbleTranDumpFilename() { return GetSysFilesNumericFilename(88); }			//FILE0088
	CString GetPebbleInvalidPaymentLogFilename() { return GetSysFilesNumericFilename(89); }
	//FILE0090 = SYSTEM DATA (ABOVE)
	CString GetBgndServiceBusyTimeFilename() { return GetSysFilesNumericFilename(91); }		//FILE0091
	CString GetSQLBackupFilename() { return GetSysFilesFilename("SMP_SQLBackup.bak"); }

	CString GetSQLConnectionOptionsFilenameOld() { return GetSysFilesFilename("SQLConnect.ini"); }
	CString GetSQLConnectionOptionsFilenameNew();
	
public:
	CString GetTerminalGroupSetTempFilename(){ return GetTempPathNumericFilename(45); }	//FILE0045

public:
	CString GetPCOptionsClientFilename(){ return GetClientPCNumericFilename(5001); }
	CString GetPCOptionsHostFilename() { return GetHostPCNumericFilename(5002); }
	
public:
	CString GetBackupOptionsFilename() { return GetSysFilesSyssetNumericFilename(1); }			//FILE0001

public:
	CString GetEmailLogParamsFilename(){ return GetEmailLogFilename( "EmailLog.prm" ); }

public:
	CString GetUserLogFolder() { return GetUserLogFilename(""); }
	CString GetUserLogFilename(const CString strFilename);
	CString GetUserLogParamsFilename() { return GetUserLogFilename("UserLog.prm"); }
	CString GetUserLogReportFilename() { return GetUserLogFilename("UserLog.rep"); }

public:
	CString GetBackingUpFilename(){ return GetSysFilesFilename( "loy.no" ); }
	CString GetBackgroundTimeTableFilename(){ return GetSysFilesFilename( "pfile002.dat" ); }

public:
	CString GetWebPaymentExceptionIndicator();
	CString GetWebPaymentStopIndicator();
	CString GetWebPaymentRxErrorLog();

	CString GetWebPaymentPath ( CString strFilename, int nWebPaymentType = nWEBPAYMENT_NONE );
	CString GetWebPaymentImportFilename ( CString strExt, CString strAccountID = "" );
	CString GetWebPaymentImportFilenameNum(CString strExt, int nNum);
	CString GetWebPaymentUpdatesFilename ( CString strExt );
	CString GetWebPaymentUpdatesFilename ( int nWebPaymentType, CString strExt );

	CString GetRecordEditFilename ( CString strCardNo );
	CString	GetRecordInUseFilename ( CString strCardNo );
	CString GetSessionAuditFilename ( CString strCardNo );

	CString GetPurchaseHistoryFilename ( CString strCardNo );
	CString GetPurchaseHistoryUpdateFilename ( CString strCardNo );
	CString GetTempCashHistoryFilename ( CString strCardNo = "0" );

	CString GetReportCardListFilename ( CString strLabel, CString strSuffix = "" );
	CString GetReportParamsFilename ( CString strLabel );
	CString GetReportFilename ( CString strLabel );
	CString GetTempReportPath ( CString strFilename, CString strFileExt = "rep"  );			// folder for program instance to hold reports

	CString GetCashFilename ( int nTerminalNo );

	CString GetExportsPath ( CString strFilename = "" );
	CString GetImportsPath ( CString strFilename = "" );
	CString GetPhotoIDPath ( CString strFilename = "" );
	CString GetLeaversPathname ( CString strFilename = "" );

	CString GetAtcArchivePath ( CString strFilename = "" ) ;
	CString GetEODReportPath ( CString strFilename, CString strExt = "rep" );
	CString GetEODNoSalesFilename ( CString strKey );

	CString GetEmailLogDatabasePathname ( int nLogNo );
	CString GetColumnsFilename( int nFileIdx );

	CString InfoListSelections ( int n );

	CString GetReportInUseFilename ( CString strLabel );

	CString GetPluPurchaseListFilename ( CString strLabel );

	CString GetBackgroundLogFolder();
	CString GetBackgroundLogFilename();
	CString GetBackgroundOldFilename();
	CString GetBackgroundLogParamsFilename();

	CString GetServerLogFolder();
	CString GetServerLogFilename();
	CString GetServerOldFilename();
	CString GetServerLogParamsFilename();

public:
	CString GetServicesFolder();
	CString GetServicesServerOuterLockFolder();
	CString GetServicesServerInnerLockFolder();
	CString GetServicesBackgroundLockFolder();
	CString GetServicesVersionFilename();
	CString GetLogCopyFolder();

public:
	CString GetBackupLogToWriteFilename();
	CString GetBackupLogToWriteFolder();
	CString GetBackupLogToReadFilename();
	CString GetBackupLogParamsFilename();

public:
	CString GetTempPathFilename(CString strFilename);
	CString GetSysFilesFilename(const CString strFilename);

public:
	CString GetLogBufferFolder();
	void CreateLogBufferFolder();

public:
	CString GetSyssetFilename(const char* szFolder, const char* szFilename);
	void CreateSyssetSubdirectory(CString strDir);

private:
	CString GetSysFilesSyssetNumericFilename(int nFileNum, CString strSuffix = "");

private:
	CString GetTempPathNumericFilename( int nFileNum, const CString strSuffix = "" );
	CString GetHostPCNumericFilename(int nFileNum, const CString strSuffix = "");
	CString GetHostPCNumericFilenameOld(int nFileNum, const CString strSuffix = "");
	CString GetClientPCNumericFilename( int nFileNum, const CString strSuffix = "" );
	CString GetClientPCNumericFilenameOld(int nFileNum, const CString strSuffix = "");

public:
	CString GetPurchaseControlLogFolder() { return GetPurchaseControlLogFilename(""); }
	CString GetPurchaseControlLogFilename(const CString strFilename);

#endif

};

//*******************************************************************

extern CFilenameHandler Filenames;
extern CSysset Sysset;

//*******************************************************************
