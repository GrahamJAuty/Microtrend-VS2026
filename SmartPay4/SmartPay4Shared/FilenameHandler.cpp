//*******************************************************************
#include "FilenameHandler.h"
//*******************************************************************
#ifndef SYSTEMTYPE_EMAILBATCHSEND
//*******************************************************************
#include "ServerData.h"
//*******************************************************************

//** pnn/temp01/filename
CString CFilenameHandler::GetTempPathFilename ( CString strFilename )
{
	CString strResult;	
	strResult.Format ( "%s\\%s", (const char*) Sysset.GetTempPath(), (const char*) strFilename );
	return strResult;
}

//*******************************************************************

//** pnn/temp01/Rnnn.rep
CString CFilenameHandler::GetTempReportPath ( CString strFilename, CString strFileExt )
{
	CString strResult;	
	strResult.Format ( "%s\\%s.%s", (const char*) Sysset.GetTempPath(), (const char*) strFilename, (const char*) strFileExt );
	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetReportCardListFilename ( CString strLabel, CString strSuffix )
{
	return GetSysFilesFilename( strLabel + strSuffix + ".lst" );
}

//*******************************************************************

CString CFilenameHandler::GetReportParamsFilename ( CString strLabel )
{
	return GetSysFilesFilename( strLabel + ".prm" );
}

//*******************************************************************
// get filename to hold report --p16\temp01\Rnnn.rep

CString CFilenameHandler::GetReportFilename ( CString strLabel )
{
	CString strResult;	
	strResult.Format ( "%s\\%s.rep", (const char*) Sysset.GetTempPath(), (const char*) strLabel  );
	return strResult;
}

//*******************************************************************
//	"c:\\Program Files\\Microtrend\\Loyx500\\Pxx\\History\\*.ecr - any updates from server;
//	"c:\\Program Files\\Microtrend\\Loyx500\\Pxx\\History\\nnnnnnnnnn.ecr - update from server;
//	"c:\\Program Files\\Microtrend\\Loyx500\\Pxx\\History\\nnnnnnnnnn.dat  - main history file;

//	"c:\\Program Files\\Microtrend\\Loyx500\\Pxx\\ParentPay\\nnnnnnnnnn.ecr  - web site update file
//	"c:\\Program Files\\Microtrend\\Loyx500\\Pxx\\Tucasi\\nnnnnnnnnn.ecr  - web site update file
//	"c:\\Program Files\\Microtrend\\Loyx500\\Pxx\\WisePay\\nnnnnnnnnn.ecr  - web site update file

CString CFilenameHandler::GetPurchaseHistoryUpdateFilename ( CString strCardNo )
{
	CString strMask = strCardNo;
	if ( strMask != "*" )								// see if looking for any updates
	{
		strMask = System.FormatCardNo(strCardNo);		// no - format individual cardno required
	}

	CString strResult;	
	strResult.Format ( "%s\\History\\%s.ecr", (const char*) Sysset.GetProgramPath(), (const char*) strMask );
	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetPurchaseHistoryFilename ( CString strCardNo )
{
	CString strResult;	
	strResult.Format ( "%s\\History\\%s.dat", (const char*) Sysset.GetProgramPath(), (const char*) System.FormatCardNo(strCardNo) );
	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetTempCashHistoryFilename ( CString strCardNo )
{
	CString strName;		
	strName.Format ( "TMP%d-%s", Sysset.GetInstanceNo(), (const char*) System.FormatCardNo(strCardNo) );

	CString strResult;	
	strResult = Filenames.GetPurchaseHistoryFilename ( strName );		// cash account = "TMPn-000000.dat"
	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetRecordEditFilename ( CString strCardNo )
{
	CString strFilename;
	strFilename.Format( "ed1-%s.dat", (const char*) System.FormatCardNo(strCardNo) );
	return GetSysFilesFilename( strFilename );
}

//*******************************************************************

CString CFilenameHandler::GetRecordInUseFilename ( CString strCardNo )
{
	CString strFilename;
	strFilename.Format( "ed1-%s.run", (const char*) System.FormatCardNo(strCardNo) );
	return GetSysFilesFilename( strFilename );
}

//*******************************************************************

CString CFilenameHandler::GetSessionAuditFilename ( CString strCardNo )
{
	CString strFilename;
	strFilename.Format( "ed2-%s.dat", (const char*) System.FormatCardNo(strCardNo) );
	return GetSysFilesFilename( strFilename );
}

//*******************************************************************

CString CFilenameHandler::GetReportInUseFilename ( CString strLabel )
{
	CString strFilename;
	strFilename.Format( "ex-%s.run", (const char*) strLabel );
	return GetSysFilesFilename( strFilename );
}

//*******************************************************************

CString CFilenameHandler::GetWebPaymentPath ( CString strFilename, int nWebPaymentType )
{
	CString strFolder = "";

	if ( nWebPaymentType == nWEBPAYMENT_NONE ) 
	{
		nWebPaymentType = System.GetWebPaymentType();
	}

	switch ( nWebPaymentType )
	{
	case nWEBPAYMENT_PARENTPAY:		strFolder = "ParentPay";	break;
	case nWEBPAYMENT_TUCASI:		strFolder = "Tucasi";		break;
	case nWEBPAYMENT_WISEPAY:		strFolder = "WisePay";		break;
	case nWEBPAYMENT_PARENTMAIL:	strFolder = "ParentMail";	break;
	case nWEBPAYMENT_SCHOOLCOMMS:	strFolder = "Schoolcomms";	break;
	case nWEBPAYMENT_SQUID:			strFolder = "sQuid";		break;
	case nWEBPAYMENT_BUCKINGHAM:	strFolder = "Buckingham";	break;
	case nWEBPAYMENT_TUCASIv2:		strFolder = "TucasiJson";	break;
	case nWEBPAYMENT_EXPORTSALES:	strFolder = "SalesExp";		break;
	case nWEBPAYMENT_BROMCOM:		strFolder = "BromCom";		break;
	case nWEBPAYMENT_PEBBLE:		strFolder = "Pebble";		break;
	}

	return GetFilename ( strFolder, strFilename );
}

//*******************************************************************

CString CFilenameHandler::GetWebPaymentExceptionIndicator()
{
	CString strFilename = GetWebPaymentPath ( "Ex-flag.dat" );
	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetWebPaymentStopIndicator()
{
	CString strFilename = GetWebPaymentPath ( "Rx-stop.dat" );
	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetWebPaymentRxErrorLog()
{
	CString strFilename = GetWebPaymentPath ( "RxError.log" );
	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetWebPaymentImportFilename ( CString strExt, CString strAccountID )
{
	CTime tm = CTime::GetCurrentTime();

	CString strFilename;	
	strFilename.Format ( "PayAcc%s-%4.4d%2.2d%2.2d-%2.2d%2.2d%2.2d.%s", (const char*) strAccountID, tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond(), (const char*) strExt );

	return GetWebPaymentPath ( strFilename );
}

//*******************************************************************

CString CFilenameHandler::GetWebPaymentImportFilenameNum(CString strExt, int nNum)
{
	CTime tm = CTime::GetCurrentTime();

	CString strFilename;
	strFilename.Format("PayAcc%d_%4.4d%2.2d%2.2d-%2.2d%2.2d%2.2d.%s", nNum, tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond(), (const char*)strExt);

	return GetWebPaymentPath(strFilename);
}

//*******************************************************************
// audit lines used to update webpayment site
// .dat = actual audit lines
// .dbs = converted lines in webpaymentatctrasnaction database format

CString CFilenameHandler::GetWebPaymentUpdatesFilename ( CString strExt )
{
	return GetWebPaymentUpdatesFilename ( System.GetWebPaymentType(), strExt );
}

//*******************************************************************

CString CFilenameHandler::GetWebPaymentUpdatesFilename ( int nWebPaymentType, CString strExt )
{
	CString strName = "";
	CString strReply = "";

	switch ( nWebPaymentType )
	{
	case nWEBPAYMENT_PARENTMAIL:
	case nWEBPAYMENT_SQUID:
	case nWEBPAYMENT_PARENTPAY:
	case nWEBPAYMENT_TUCASI:
	case nWEBPAYMENT_TUCASIv2:
	case nWEBPAYMENT_SCHOOLCOMMS:
	case nWEBPAYMENT_WISEPAY:
	case nWEBPAYMENT_BROMCOM:
	case nWEBPAYMENT_PEBBLE:
	case nWEBPAYMENT_EXPORTSALES:
		strName.Format ( "webupdates.%s", (const char*) strExt );	
		strReply = GetWebPaymentPath ( strName, nWebPaymentType );							
		break;

//	case nWEBPAYMENT_BUCKINGHAM:
	default:
		break;
	}

	return strReply;
}

//*******************************************************************

CString CFilenameHandler::GetCashFilename ( int nTerminalNo )
{
	CString strFilename;
	strFilename.Format( "file0024.%3.3d", nTerminalNo );
	return GetSysFilesFilename( strFilename );
}

//*******************************************************************

CString CFilenameHandler::GetImportsPath ( CString strFilename )	
{ 
	return GetFilename ( "Imports", strFilename ); 
}

//*******************************************************************

CString CFilenameHandler::GetExportsPath ( CString strFilename ) 
{ 
	return GetFilename ( "Exports", strFilename ); 
}

//*******************************************************************

CString CFilenameHandler::GetPhotoIDPath ( CString strFilename )
{
	CString strResult;

	Server.Read( GetServerDataFilename() );

	if ( Server.GetPhotoIDFolderType() != 2 )
	{
		strResult = GetFilename( "PhotoID", strFilename );
	}
	else
	{
		if ( strFilename.GetLength() == 0 )	
		{
			strResult = Server.GetSmartPayUNCImagePath();
		}
		else								
		{
			strResult.Format ( "%s\\%s", (const char*) Server.GetSmartPayUNCImagePath(), (const char*) strFilename );
		}
	}

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetAtcArchivePath ( CString strFilename ) 
{
	return GetFilename( "SysFiles\\Archive", strFilename );
}

//*******************************************************************
// Filename can be dateKey = yyyymmddd / EODnnnnn with .dat extension

CString CFilenameHandler::GetEODReportPath ( CString strFilename, CString strExt ) 
{
	CString strFullFilename;
	strFullFilename.Format( "%s.%s", (const char*) strFilename, (const char*) strExt );
	return GetFilename( "EOD", strFullFilename );
}

//*******************************************************************
// file to hold list of audit lines when cannot find matching purchase history
// szKey = "yyyymmdd" or "consol"

CString CFilenameHandler::GetEODNoSalesFilename ( CString strKey )
{
	CString strFilename;	
	strFilename.Format ( "%s-ns", (const char*) strKey );
	return GetEODReportPath ( strFilename, "dat" );
}

//*******************************************************************

CString CFilenameHandler::GetEmailLogDatabasePathname ( int nLogNo )
{
	switch( nLogNo )
	{
	case 1:		return GetEmailLogLowBalanceFilename();		// email low balance alert log database
	case 2:		return GetEmailLogPurchaseFilename();		// itemised purchases log database
	case 3:		return GetEmailLogPointsFilename();			// points achievement log database
	case 4:		return GetEmailLogWeeklyPointsFilename();	// weekly points update log database
	default:	return "";
	}
}

//*******************************************************************
// eg "<data path\>Pnn\Leavers"

CString CFilenameHandler::GetLeaversPathname ( CString strFilename )
{
	return GetFilename( "Leavers", strFilename );
}

//*******************************************************************

CString CFilenameHandler::InfoListSelections ( int n )
{
	CString strFile;	
	strFile.Format ( "file0051-%d.dat", n );
	return GetSysFilesFilename ( strFile );
}

//*******************************************************************

CString CFilenameHandler::GetPluPurchaseListFilename ( CString strLabel )
{
	CString strFile;	
	strFile.Format ( "file0014-%s.dat", (const char*) strLabel );	// SysFiles\\file0014Rep136.dat"		
	return GetSysFilesFilename( strFile );
}

//*******************************************************************

CString CFilenameHandler::GetColumnsFilename( int nFileIdx )
{
	CString strFilename;
	strFilename.Format( "FL058%2.2X.dat", nFileIdx );
	return GetSysFilesFilename( strFilename );
}

//*******************************************************************
//*******************************************************************
//*******************************************************************

CString CFilenameHandler::GetSysFilesFilename( CString strFilename )
{
	return GetFilename( "SysFiles", strFilename );
}

//*******************************************************************

CString CFilenameHandler::GetTempPathNumericFilename( int nFileNum, CString strSuffix )
{
	CString strResult;

	strResult.Format( "%s\\File%4.4d%s.dat",
		(const char*) Sysset.GetTempPath(),
		nFileNum,
		(const char*) strSuffix );

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetHostPCNumericFilename(int nFileNum, CString strSuffix)
{
	CString strResult;

	strResult.Format("%s\\FILE%4.4d%s.dat",
		(const char*)Sysset.GetPCPathHost(),
		nFileNum,
		(const char*)strSuffix);

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetHostPCNumericFilenameOld( int nFileNum, CString strSuffix )
{
	CString strResult;

	strResult.Format( "%s%4.4d%s.dat",
		(const char*) Sysset.GetPCPathHost(),
		nFileNum,
		(const char*) strSuffix );

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetClientPCNumericFilename(int nFileNum, CString strSuffix)
{
	CString strResult;

	strResult.Format("%s\\FILE%4.4d%s.dat",
		(const char*)Sysset.GetPCPathClient(),
		nFileNum,
		(const char*)strSuffix);

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetClientPCNumericFilenameOld(int nFileNum, CString strSuffix)
{
	CString strResult;

	strResult.Format("%s%4.4d%s.dat",
		(const char*)Sysset.GetPCPathClient(),
		nFileNum,
		(const char*)strSuffix);

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetUserLogFilename(CString strFilename)
{
	CString strResult = "";

	strResult.Format("%s\\UserLog\\%s",
		(const char*)Sysset.GetSyssetProgramPath(),
		(const char*)strFilename);

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetBackgroundLogFolder()
{
	CString strResult = "";
	
	strResult.Format( "%s\\BgndLog",
		(const char*) Sysset.GetSyssetProgramPath() );
		
	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetBackgroundLogFilename()
{
	CString strResult = "";
	
	strResult.Format( "%s\\BgndLog\\BgndLog.rep",
		(const char*) Sysset.GetSyssetProgramPath() );

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetBackgroundOldFilename()
{
	CString strResult = "";
	
	strResult.Format( "%s\\BgndLog\\BgndOld.rep",
		(const char*) Sysset.GetSyssetProgramPath() );

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetBackgroundLogParamsFilename()
{
	CString strResult = "";
	
	strResult.Format( "%s\\BgndLog\\BgndLog.prm",
		(const char*) Sysset.GetSyssetProgramPath() );

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetServerLogFolder()
{
	CString strResult = "";
	
	strResult.Format( "%s\\ServerLog",
		(const char*) Sysset.GetSyssetProgramPath() );
		
	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetServerLogFilename()
{
	CString strResult = "";
	
	strResult.Format( "%s\\ServerLog\\ServerLog.rep",
		(const char*) Sysset.GetSyssetProgramPath() );

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetServerOldFilename()
{
	CString strResult = "";
	
	strResult.Format( "%s\\ServerLog\\ServerOld.rep",
		(const char*) Sysset.GetSyssetProgramPath() );

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetServerLogParamsFilename()
{
	CString strResult = "";
	
	strResult.Format( "%s\\ServerLog\\ServerLog.prm",
		(const char*) Sysset.GetSyssetProgramPath() );

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetServicesFolder()
{
	CString strPath = "";
	strPath += Sysset.GetSyssetProgramPath();
	strPath += "\\WinServices";
	return strPath;
}

//*******************************************************************

CString CFilenameHandler::GetServicesServerOuterLockFolder()
{
	CString strPath = "";
	strPath += GetServicesFolder();
	strPath += "\\Server1";
	return strPath;
}

//*******************************************************************

CString CFilenameHandler::GetServicesServerInnerLockFolder()
{
	CString strPath = "";
	strPath += GetServicesFolder();
	strPath += "\\Server2";
	return strPath;
}

//*******************************************************************

CString CFilenameHandler::GetServicesBackgroundLockFolder()
{
	CString strPath = "";
	strPath += GetServicesFolder();
	strPath += "\\Bgnd";
	return strPath;
}

//*******************************************************************

CString CFilenameHandler::GetServicesVersionFilename()
{
	CString strPath = "";
	strPath += GetServicesFolder();
	strPath += "\\Version.dat";
	return strPath;
}

//*******************************************************************

CString CFilenameHandler::GetLogCopyFolder()
{
	CString strPath = "";
	strPath += Sysset.GetSyssetProgramPath();
	strPath += "\\LogCopy";
	return strPath;
}

//*******************************************************************

CString CFilenameHandler::GetSQLDatabaseFilename()
{
	CString strResult = GetFilename("SysFiles", "SmartPay.db");
	return strResult;
}

//*******************************************************************

void CFilenameHandler::CreateSyssetSubdirectory(CString strDir)
{
	CString strPath = "";
	strPath += Sysset.GetSyssetProgramPath();
	strPath += "\\";
	strPath += strDir;

	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}
}

//*******************************************************************

CString CFilenameHandler::GetSyssetFilename(const char* szFolder, const char* szFilename)
{
	CString strFilename;

	if (strlen(szFilename) == 0)
	{
		strFilename.Format("%s\\%s", 
			(const char*) Sysset.GetSyssetProgramPath(), 
			szFolder);
	}
	else
	{
		strFilename.Format("%s\\%s\\%s", 
			(const char*) Sysset.GetSyssetProgramPath(),
			szFolder, 
			szFilename);
	}

	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetSysFilesSyssetNumericFilename(int nFileNum, CString strSuffix)
{
	CString strResult = "";

	strResult.Format("%s%4.4d%s.dat",
		(const char*)GetSyssetFilename("SysFiles", "File"),
		nFileNum,
		(const char*)strSuffix);

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetBackupLogToWriteFilename()
{
	CString strFilename = "Sysset\\P46\\BackupLog\\Backup.Log";
	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetBackupLogToWriteFolder()
{
	CString strFilename = "Sysset\\P46\\BackupLog";
	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetBackupLogToReadFilename()
{
	CString strFilename = "";
	strFilename.Format("%s\\BackupLog\\Backup.Log", 
		(const char*) Sysset.GetSyssetProgramPath());

	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetBackupLogParamsFilename()
{
	CString strFilename = "";
	strFilename.Format("%s\\BackupLog\\Backup.Prm", 
		(const char*) Sysset.GetSyssetProgramPath());
	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetLogBufferFolder()
{
	CString strFilename = "";
	strFilename.Format("%s\\LogBuffer", 
		(const char*) Sysset.GetProgramPath());

	return strFilename;
}

//*******************************************************************

void CFilenameHandler::CreateLogBufferFolder()
{
	CreateSubdirectory(GetLogBufferFolder());
}

//*******************************************************************

CString CFilenameHandler::GetSQLConnectionOptionsFilenameNew()
{
	CString strFilename = "";
	strFilename += Sysset.GetDataPath();
	strFilename += "\\SQL\\SQLCONNECT.INI";
	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetPurchaseControlLogFilename(CString strFilename)
{
	CString strResult = "";

	strResult.Format("%s\\PurchaseControlLog\\%s",
		(const char*)Sysset.GetProgramPath(),
		(const char*)strFilename);

	return strResult;
}

//*******************************************************************
#endif
//*******************************************************************

CString CFilenameHandler::GetSysFilesNumericFilename(int nFileNum, CString strSuffix)
{
	CString strResult = "";

	strResult.Format("%s%4.4d%s.dat",
		(const char*)GetFilename("SysFiles", "File"),
		nFileNum,
		(const char*)strSuffix);

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetEmailLogFilename(CString strFilename)
{
	CString strResult = "";

	strResult.Format("%s\\Email\\%s",
		(const char*)Sysset.GetSyssetProgramPath(),
		(const char*)strFilename);

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetFilename(const CString strFolder, const CString strFilename)
{
	CString strResult;

	if (strFilename.GetLength() == 0)
	{
		strResult.Format("%s\\%s", (const char*)Sysset.GetProgramPath(), (const char*)strFolder);
	}
	else
	{
		strResult.Format("%s\\%s\\%s", (const char*)Sysset.GetProgramPath(), (const char*)strFolder, (const char*)strFilename);
	}

	return strResult;
}

//*******************************************************************

