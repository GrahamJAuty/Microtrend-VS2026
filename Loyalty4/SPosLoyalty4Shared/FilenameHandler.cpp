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

//** pnn/temp01/filename
CString CFilenameHandler::GetTempPathFilename ( const char* szFilename )
{
	CString strFilename = "";
	strFilename.Format ( "%s\\%s", 
		(const char*) Sysset.GetTempPath(), szFilename );

	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetPurchaseHistoryUpdateFilename ( const char* szCardNo )
{
	CString strFilename = "";
	strFilename.Format ( "%s\\History\\%s.$$$", 
		(const char*) Sysset.GetProgramPath(),
		(const char*) System.FormatCardNo ( szCardNo ) );
	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetPurchaseHistoryFilename ( const char* szCardNo )
{
	CString strFilename = "";
	strFilename.Format ( "%s\\History\\%s.dat", 
		(const char*)Sysset.GetProgramPath(),
		(const char*)System.FormatCardNo ( szCardNo ) );
	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetReportCardListFilename ( const char* szLabel )
{
	CString strFilename = "";
	strFilename.Format ( "%s\\SysFiles\\%s.lst",
		(const char*)Sysset.GetProgramPath(), 
		szLabel );
	
	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetReportParamsFilename ( const char* szLabel )
{
	CString strFilename = "";
	strFilename.Format ( "%s\\SysFiles\\%s.prm", 
		(const char*)Sysset.GetProgramPath(),
		szLabel  );
	
	return strFilename;
}

//*******************************************************************
// get filename to hold report --p16\temp01\Rnnn.rep

CString CFilenameHandler::GetReportFilename ( const char* szLabel )
{
	CString strFilename = "";
	strFilename.Format ( "%s\\%s.rep",
		(const char*)Sysset.GetTempPath(),
		szLabel );

	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetAtcArchivePath ( const char* szFilename ) 
{
	CString strPath = "";
	strPath.Format ( "%s\\SysFiles\\Archive",
		(const char*)Sysset.GetProgramPath() );		// eg "<data path\>Pnn\SysFiles\Archive"

	CString strFilename = "";
	if (strlen(szFilename) == 0)
	{
		strFilename = strPath;
	}
	else
	{
		strFilename.Format("%s\\%s", 
			(const char*)strPath,
			szFilename);
	}

	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetFamilyPath ( const char* szAccount )
{
	CString strAccount = szAccount;
	
	if (strAccount != "")
	{
		strAccount.Format("%s.dat", 
			(const char*) System.FormatCardNo(szAccount));
	}

	return GetFilename ( "Family", strAccount );
}

//*******************************************************************

CString CFilenameHandler::GetImportsPath ( const char* szFilename )
{ 
	return GetFilename ( "Imports", szFilename ); 
}

//*******************************************************************

CString CFilenameHandler::GetExportsPath ( const char* szFilename ) 
{ 
	return GetFilename ( "Exports", szFilename ); 
}

//*******************************************************************

CString CFilenameHandler::GetFilename ( const char* szFolder, const char* szFilename )
{
	CString strFilename = "";

	if (strlen(szFilename) == 0)
	{
		strFilename.Format("%s\\%s", 
			(const char*) Sysset.GetProgramPath(), 
			szFolder);
	}
	else
	{
		strFilename.Format("%s\\%s\\%s", 
			(const char*) Sysset.GetProgramPath(),
			szFolder, 
			szFilename);
	}

	return strFilename;
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

CString CFilenameHandler::GetRecordEditFilename ( const char* szCardNo )
{
	CString strFilename = "";
	strFilename.Format ( "%s\\SysFiles\\ed1-%s.dat", 
		(const char*) Sysset.GetProgramPath(),
		(const char*) System.FormatCardNo(szCardNo) );
	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetRecordInuseFilename ( const char* szCardNo )
{
	CString strFilename = "";
	strFilename.Format ( "%s\\SysFiles\\ed1-%s.run", 
		(const char*) Sysset.GetProgramPath(),
		(const char*) System.FormatCardNo(szCardNo) );
	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetColumnsFilename(int nFileIdx)
{
	CString strFilename = "";
	strFilename.Format("FL025%2.2X.dat", nFileIdx);
	return GetSysFilesFilename(strFilename);
}

//*******************************************************************

CString CFilenameHandler::GetSysFilesFilename(CString strFilename)
{
	return GetFilename("SysFiles", strFilename);
}

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

CString CFilenameHandler::GetHostPCNumericFilename(int nFileNum, CString strSuffix)
{
	CString strResult = "";

	strResult.Format("%s\\FILE%4.4d%s.dat",
		(const char*)Sysset.GetPCPathHost(),
		nFileNum,
		(const char*)strSuffix);

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetHostPCNumericFilenameOld(int nFileNum, CString strSuffix)
{
	CString strResult = "";

	strResult.Format("%s%4.4d%s.dat",
		(const char*)Sysset.GetPCPathHost(),
		nFileNum,
		(const char*)strSuffix);

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetClientPCNumericFilename(int nFileNum, CString strSuffix)
{
	CString strResult = "";

	strResult.Format("%s\\FILE%4.4d%s.dat",
		(const char*)Sysset.GetPCPathClient(),
		nFileNum,
		(const char*)strSuffix);

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetClientPCNumericFilenameOld(int nFileNum, CString strSuffix)
{
	CString strResult = "";

	strResult.Format("%s%4.4d%s.dat",
		(const char*)Sysset.GetPCPathClient(),
		nFileNum,
		(const char*)strSuffix);

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetServerLogFolder()
{
	CString strResult = "";

	strResult.Format("%s\\ServerLog",
		(const char*)Sysset.GetSyssetProgramPath());

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetServerLogFilename()
{
	CString strResult = "";

	strResult.Format("%s\\ServerLog\\ServerLog.rep",
		(const char*)Sysset.GetSyssetProgramPath());

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetServerOldFilename()
{
	CString strResult = "";

	strResult.Format("%s\\ServerLog\\ServerOld.rep",
		(const char*)Sysset.GetSyssetProgramPath());

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetServerLogParamsFilename()
{
	CString strResult = "";

	strResult.Format("%s\\ServerLog\\ServerLog.prm",
		(const char*)Sysset.GetSyssetProgramPath());

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

CString CFilenameHandler::GetUserLogFilename(CString strFilename)
{
	CString strResult = "";

	strResult.Format("%s\\UserLog\\%s",
		(const char*)Sysset.GetSyssetProgramPath(),
		(const char*)strFilename);

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetStampOfferLogFilename(CString strFilename)
{
	CString strResult = "";

	strResult.Format("%s\\StampOfferLog\\%s",
		(const char*)Sysset.GetProgramPath(),
		(const char*)strFilename);

	return strResult;
}

//*******************************************************************

CString CFilenameHandler::GetBackupLogToWriteFilename()
{
	CString strFilename = "Sysset\\P47\\BackupLog\\Backup.Log";
	return strFilename;
}

//*******************************************************************

CString CFilenameHandler::GetBackupLogToWriteFolder()
{
	CString strFilename = "Sysset\\P47\\BackupLog";
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

CString CFilenameHandler::GetPluDatabaseListFilename(int nDbNo)
{
	int nFilenum = 0;

	switch (nDbNo)
	{
	case 1:
		nFilenum = 14;
		break;

	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
		nFilenum = 40 + nDbNo;
		break;
	}

	CString strFilename = "";
	if (nFilenum != 0)
	{
		strFilename = GetSysFilesNumericFilename(nFilenum);
	}

	return strFilename;
}

//*******************************************************************
