//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_AuditPeriod\SQLRepositoryAuditPeriod.h"
#include "..\SPosLoyalty4Shared\SQLTable_CCNo\SQLRepositoryCCNo.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\PresentationOptions.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\SQLTable_PluInfo\SQLRepositoryPluInfo.h"
//$$******************************************************************
#include "DatabaseImportForManager.h"
#include "GroupImportForManager.h"
#include "EmailOptions.h"
#include "LoyaltyManager.h"
#include "PasswordData.h"
#include "PCOptions.h"
//$$******************************************************************
#include "UpgradeDlg.h"
//$$******************************************************************
extern CSysset Sysset;
//$$******************************************************************
#define BUFSIZE MAX_PATH
//$$******************************************************************
#define szSERVERSTARTUP_V1 "Loyx500Server.lnk"
#define szSERVERSTARTUP_V2_OLD "Loyx500v2Server.lnk"
#define szSERVERSTARTUP_V2_NEW "SPosLoyaltyServer.lnk"
//$$******************************************************************
#define	nNOTSET 0
#define nERROR -1
#define nOK 1
//$$******************************************************************

CUpgradeDlg::CUpgradeDlg(const char* szSourcePath, CWnd* pParent)
	: CDialog(CUpgradeDlg::IDD, pParent)
{
	m_strSourcePath = szSourcePath;
	m_strError = "";

	m_bAbort = FALSE;
	m_nV1or2ServerTerminated = nNOTSET;
	m_nV1StartupLinkRemoved = nNOTSET;
	m_nV2OldStartupLinkRemoved = nNOTSET;
	m_nV2NewStartupLinkRemoved = nNOTSET;
	m_strOldVersion = "";
}

//$$******************************************************************

void CUpgradeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_UPGRADEPATH, m_strSourcePath);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CUpgradeDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
END_MESSAGE_MAP()

//$$******************************************************************

void CUpgradeDlg::OnButtonStart()
{
	m_strError = "";

	if ( UpdateData() == TRUE )
	{
		int nSystemType = GetPreviousSystemType();

		switch (nSystemType)
		{
		case SYSSET_LOYALTY4:
			m_bAbort = TRUE;
			m_strError = "The specified folder already contains Loyalty v4 data";
			break;

		case SYSSET_LOYALTY3:
			m_strOldVersion = "v3";
			ConvertV3();
			break;

		case SYSSET_LOYALTY2:
			m_strOldVersion = "v2";
			ConvertV2();
			break;

		case SYSSET_LOYALTY:
			m_strOldVersion = "v1";
			ConvertV1();
			break;

		case 0:
		default:
			m_bAbort = TRUE;
			m_strError = "The specified folder does not contain any Loyalty data";
			break;
		}

		DisplayResult();

		if ( ( m_strError == "" ) && ( FALSE == m_bAbort ) )
		{
			System.Read();										 
			PresentationOptions.Read();
			EmailOptions.Read();
			PCOptionsHost.Read();
			PCOptionsClient.Read();
			Passwords.Read(Filenames.GetPasswordFilename());
			Server.Read(Filenames.GetServerDataFilename());
			EndDialog( IDOK );
		}
	}
}

//$$******************************************************************

void CUpgradeDlg::OnButtonBrowse()
{
	if ( UpdateData() == TRUE )
	{
		CString strPathname;
		if ( BrowseFolder ( strPathname, "Select Previous Loyalty Folder", NULL, this ) == TRUE )
		{
			m_strSourcePath = strPathname;
			UpdateData(FALSE);
		}
	}
}

//$$******************************************************************

int CUpgradeDlg::GetPreviousSystemType()
{
	CArray<int, int> arrayTypesToCheck;
	arrayTypesToCheck.Add(SYSSET_LOYALTY4);
	arrayTypesToCheck.Add(SYSSET_LOYALTY3);
	arrayTypesToCheck.Add(SYSSET_LOYALTY2);
	arrayTypesToCheck.Add(SYSSET_LOYALTY);

	bool bFound = FALSE;
	int nSystemType = 0;

	for (int n = 0; n < arrayTypesToCheck.GetSize(); n++)
	{
		nSystemType = arrayTypesToCheck.GetAt(n);

		CString strSystemFolder = "";
		strSystemFolder.Format("%s\\%s", 
			(const char*) m_strSourcePath, 
			(const char*) Sysset.GetProgramFolder(nSystemType));

		if (ExistSubdirectory(strSystemFolder) == TRUE)
		{
			bFound = TRUE;
			break;
		}
	}

	if (FALSE == bFound)
	{
		nSystemType = 0;
	}

	return nSystemType;
}

//$$******************************************************************

void CUpgradeDlg::ConvertV1()
{
	{
		CWorkingDlg dlgWorking("Converting data from Loyalty v1");
		dlgWorking.Create();

		CString strSystemFolder = "";
		strSystemFolder.Format("%s\\%s",
			(const char*)m_strSourcePath,
			(const char*)Sysset.GetProgramFolder(SYSSET_LOYALTY));

		if (CheckV1Server(strSystemFolder, dlgWorking) == FALSE)
		{
			return;
		}

		int nFileCount = 0;

		//copy barman folder contents
		{
			CString strSourcePathname = strSystemFolder + "\\BarDat";;

			if (::ExistSubdirectory(strSourcePathname) == TRUE)
			{
				CString strDestPathname = "";
				strDestPathname.Format("%s\\BarDat",
					(const char*)Sysset.GetProgramPath());

				CopyFolderRecursive(strSourcePathname, strDestPathname, FALSE, FALSE, &dlgWorking, nFileCount);
			}
		}

		// copy History contents
		{
			CString strSourcePathname = strSystemFolder + "\\History";;

			if (::ExistSubdirectory(strSourcePathname) == TRUE)
			{
				CString strDestPathname = "";
				strDestPathname.Format("%s\\History",
					(const char*)Sysset.GetProgramPath());

				CopyFolderRecursive(strSourcePathname, strDestPathname, FALSE, FALSE, &dlgWorking, nFileCount, "*.dat");
			}
		}

		// copy PhotoID contents
		{
			CString strSourcePathname = strSystemFolder + "\\PhotoID";;

			if (::ExistSubdirectory(strSourcePathname) == TRUE)
			{
				CString strDestPathname = "";
				strDestPathname.Format("%s\\PhotoID",
					(const char*)Sysset.GetProgramPath());

				CopyFolderRecursive(strSourcePathname, strDestPathname, FALSE, FALSE, &dlgWorking, nFileCount);
			}
		}

		// copy sysfiles contents
		{
			CString strSourcePathname = strSystemFolder + "\\SysFiles";;

			if (::ExistSubdirectory(strSourcePathname) == TRUE)
			{
				CString strDestPathname = "";
				strDestPathname.Format("%s\\SysFiles",
					(const char*)Sysset.GetProgramPath());

				CopyFolderRecursive(strSourcePathname, strDestPathname, FALSE, FALSE, &dlgWorking, nFileCount, "*.dat");
			}
		}
	}

	ImportAccountDatabase();
	ImportGroupDatabase();
	ImportPluDatabase();

	CSQLRepositoryCCNo repoCC;
	repoCC.ImportLegacyCCNo(NULL);
	
	CSQLRepositoryAuditPeriod repoPeriod;
	repoPeriod.ImportLegacyAuditPeriod(NULL);
}

//$$******************************************************************

void CUpgradeDlg::ConvertV2()
{
	{
		CWorkingDlg dlgWorking("Converting data from Loyalty v3");
		dlgWorking.Create();

		CString strSystemFolder;
		strSystemFolder.Format("%s\\%s",
			(const char*)m_strSourcePath,
			(const char*)Sysset.GetProgramFolder(SYSSET_LOYALTY2));

		if (CheckV2Server(strSystemFolder, dlgWorking) == FALSE)
		{
			return;
		}

		CString strSourcePath = "";
		strSourcePath = m_strSourcePath;
		strSourcePath += "\\";
		strSourcePath += Sysset.GetProgramFolder(SYSSET_LOYALTY2);

		CString strDestPath = "";
		strDestPath += Sysset.GetProgramPath();

		{
			int nFileCount = 0;

			if (CopyFolderRecursive(strSourcePath, strDestPath, TRUE, FALSE, &dlgWorking, nFileCount) == TRUE)
			{
				strSourcePath = "";
				strSourcePath += m_strSourcePath;
				strSourcePath += "\\Sysset\\";
				strSourcePath += Sysset.GetProgramFolder(SYSSET_LOYALTY2);

				CString strDestPath = "";
				strDestPath += Sysset.GetSyssetProgramPath();

				CopyFolderRecursive(strSourcePath, strDestPath, TRUE, FALSE, &dlgWorking, nFileCount);
			}
		}
	}

	ImportAccountDatabase();
	ImportGroupDatabase();
	ImportPluDatabase();

	CSQLRepositoryCCNo repoCC;
	repoCC.ImportLegacyCCNo(NULL);

	CSQLRepositoryAuditPeriod repoPeriod;
	repoPeriod.ImportLegacyAuditPeriod(NULL);
}

//$$******************************************************************

void CUpgradeDlg::ConvertV3()
{
	CString strSystemFolder;
	strSystemFolder.Format("%s\\%s",
		(const char*)m_strSourcePath,
		(const char*)Sysset.GetProgramFolder(SYSSET_LOYALTY3));

	{
		CSSFile fileStopServer;
		if (fileStopServer.Open(strSystemFolder + "\\SysFiles\\File0030.dat", "wb"))
		{
			fileStopServer.WriteLine("Stop");
		}
		else
		{
			m_strError = "Unable to create v3 Server Stop file";
			return;
		}
	}

	CString strSourcePath = "";
	strSourcePath = m_strSourcePath;
	strSourcePath += "\\";
	strSourcePath += Sysset.GetProgramFolder(SYSSET_LOYALTY3);

	CString strDestPath = "";
	strDestPath += Sysset.GetProgramPath();

	{
		CWorkingDlg dlgWorking("Converting data from Loyalty v3");
		dlgWorking.Create();
		int nFileCount = 0;

		if (CopyFolderRecursive(strSourcePath, strDestPath, TRUE, FALSE, &dlgWorking, nFileCount) == TRUE)
		{
			strSourcePath = "";
			strSourcePath += m_strSourcePath;
			strSourcePath += "\\Sysset\\";
			strSourcePath += Sysset.GetProgramFolder(SYSSET_LOYALTY3);

			CString strDestPath = "";
			strDestPath += Sysset.GetSyssetProgramPath();

			CopyFolderRecursive(strSourcePath, strDestPath, TRUE, FALSE, &dlgWorking, nFileCount);
		}
	}

	ImportAccountDatabase();
	ImportGroupDatabase();
	ImportPluDatabase();

	CSQLRepositoryCCNo repoCC;
	repoCC.ImportLegacyCCNo(NULL);

	CSQLRepositoryAuditPeriod repoPeriod;
	repoPeriod.ImportLegacyAuditPeriod(NULL);
}

//$$******************************************************************

bool CUpgradeDlg::CheckV1Server ( const char* szSystemFolder, CWorkingDlg& WorkingDlg )
{
	if ( IsV1orV2ServerRunning ( szSystemFolder ) == TRUE )
	{
		WorkingDlg.SetCaption2 ( "Terminating Loyalty v1 Server" );

		if ( TerminateV1orV2Server ( szSystemFolder ) == FALSE )
		{
			m_nV1or2ServerTerminated = nERROR;
			m_bAbort = TRUE;
			return FALSE;
		}
		m_nV1or2ServerTerminated = nOK;
	}

// see if shortcut on Program Startup

	CString strStartupLink_v1;
	strStartupLink_v1.Format ( "%s\\%s", 
		(const char*) GetStartupPath(), 
		szSERVERSTARTUP_V1 );

	if ( SolutionGlobalFunctions::FileExists ( strStartupLink_v1 ) == TRUE )
	{
		::SendToRecycleBin ( strStartupLink_v1 );			// remove link

		if (SolutionGlobalFunctions::FileExists(strStartupLink_v1) == TRUE)
		{
			m_nV1StartupLinkRemoved = nERROR;
		}
		else
		{
			m_nV1StartupLinkRemoved = nOK;
		}
	}

	return TRUE;
}

//$$******************************************************************

bool CUpgradeDlg::IsV1orV2ServerRunning ( const char* szSystemFolder )
{
	bool bServerRunning = FALSE;

	CString strServerRunFile;
	strServerRunFile.Format ( "%s\\SysFiles\\loy.run", szSystemFolder );

	CSSFile file;
	int nRetries = 1;
	if ( file.Open ( strServerRunFile, "wb", _SH_DENYRW, nRetries ) == FALSE )
		bServerRunning = TRUE;
	else
	{
		file.Close();
		::SendToRecycleBin ( strServerRunFile );
	}

	return bServerRunning;
}

//$$******************************************************************

bool CUpgradeDlg::TerminateV1orV2Server ( const char* szSystemFolder )
{
	CString strTerminateFilename;
	strTerminateFilename.Format ( "%s\\SysFiles\\stop.req", szSystemFolder );
	
	CSSFile file;
	file.Open ( strTerminateFilename , "wb" );
	file.Close();

	Sleep(500);												// wait a bit

	int nCount;
	for ( nCount = 0 ; nCount < 10 ; nCount++ )
	{
		if ( SolutionGlobalFunctions::FileExists ( strTerminateFilename ) == FALSE )	// see if request been been processed
			break;

		Sleep(1000);										// wait again
	}

	return !IsV1orV2ServerRunning ( szSystemFolder );			// see if server not running
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

bool CUpgradeDlg::CheckV2Server(const char* szSystemFolder, CWorkingDlg& WorkingDlg)
{
	if (IsV1orV2ServerRunning(szSystemFolder) == TRUE)
	{
		WorkingDlg.SetCaption2("Terminating Loyalty v2 Server");

		if (TerminateV1orV2Server(szSystemFolder) == FALSE)
		{
			m_nV1or2ServerTerminated = nERROR;
			m_bAbort = TRUE;
			return FALSE;
		}
		m_nV1or2ServerTerminated = nOK;
	}

	// see if shortcut on Program Startup

	{
		CString strStartupLink_v2;
		strStartupLink_v2.Format("%s\\%s", 
			(const char*) GetStartupPath(), 
			szSERVERSTARTUP_V2_OLD);

		if (SolutionGlobalFunctions::FileExists(strStartupLink_v2) == TRUE)
		{
			::SendToRecycleBin(strStartupLink_v2);			// remove link

			if (SolutionGlobalFunctions::FileExists(strStartupLink_v2) == TRUE)
			{
				m_nV2OldStartupLinkRemoved = nERROR;
			}
			else
			{
				m_nV2OldStartupLinkRemoved = nOK;
			}
		}
	}

	{
		CString strStartupLink_v2;
		strStartupLink_v2.Format("%s\\%s", 
			(const char*) GetStartupPath(), 
			szSERVERSTARTUP_V2_NEW);

		if (SolutionGlobalFunctions::FileExists(strStartupLink_v2) == TRUE)
		{
			::SendToRecycleBin(strStartupLink_v2);			// remove link

			if (SolutionGlobalFunctions::FileExists(strStartupLink_v2) == TRUE)
			{
				m_nV2NewStartupLinkRemoved = nERROR;
			}
			else
			{
				m_nV2NewStartupLinkRemoved = nOK;
			}
		}
	}

	return TRUE;
}

//$$******************************************************************

bool CUpgradeDlg::CopyFolderRecursive(CString strSourceFolder, CString strDestFolder, bool bSubFolders, bool bExcludeRootFiles, CWorkingDlg* pWorking, int& nFileCount, CString strMask)
{
	if (SafelyCreateFolder(strDestFolder) == FALSE)
	{
		m_strError = "Unable to create folder: ";
		m_strError += strDestFolder;
		return FALSE;
	}

	strSourceFolder += "\\";
	strDestFolder += "\\";

	bool bWorking = FALSE;

	CFileFind FileFinder;

	if (FALSE == bExcludeRootFiles)
	{
		bWorking = (FileFinder.FindFile(strSourceFolder + strMask) != 0);

		while (bWorking)
		{
			(bWorking = FileFinder.FindNextFile() != 0);

			//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
			if (FileFinder.IsDots() == TRUE)
				continue;

			//IGNORE DIRECTORIES
			if (FileFinder.IsDirectory() == TRUE)
				continue;

			CString strFilename = FileFinder.GetFileName();

			CString strTest = strFilename;
			strTest.MakeUpper();

			if (strTest.GetLength() == 3)
			{
				if ((strTest.Left(1) == "S") || (strTest.Left(1) == "R"))
				{
					if (::TestNumeric(strTest.Right(2)))
					{
						continue;
					}
				}
			}

			/*
			if (ExcludeList.Find(strTest, nDummy) == TRUE)
			{
				continue;
			}
			*/

			if (CopyFileForUpgrade(strSourceFolder + strFilename, strDestFolder + strFilename) == FALSE)
			{
				return FALSE;
			}

			pWorking->SetCaption2RecordsChecked(++nFileCount, FALSE);
		}
	}

	if (bSubFolders == FALSE)
	{
		return TRUE;
	}

	bWorking = (FileFinder.FindFile(strSourceFolder + "*.*") != 0);
	while (bWorking)
	{
		(bWorking = FileFinder.FindNextFile() != 0);

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if (FileFinder.IsDots() == TRUE)
			continue;

		//IGNORE FILES
		if (FileFinder.IsDirectory() == FALSE)
			continue;

		CString strFilename = FileFinder.GetFileName();

		int nDummy = 0;
		CString strTest = strFilename;
		strTest.MakeUpper();

		/*
		if (ExcludeFolderList.Find(strTest, nDummy) == TRUE)
			continue;
		*/

		if (CopyFolderRecursive(strSourceFolder + strFilename, strDestFolder + strFilename, TRUE, FALSE, pWorking, nFileCount, strMask) == FALSE)
		{
			return FALSE;
		}
	}

	return TRUE;
}

//$$******************************************************************

bool CUpgradeDlg::SafelyCreateFolder(const char* szFolder)
{
	CString strFolder = szFolder;
	if (::ExistSubdirectory(strFolder) == TRUE)
		return TRUE;

	::MakeSubdirectory(strFolder);
	if (::ExistSubdirectory(strFolder) == TRUE)
		return TRUE;

	return FALSE;
}

//$$******************************************************************

bool CUpgradeDlg::CopyFileForUpgrade(const char* szSource, const char* szDest)
{
	CString strSource = szSource;
	CString strDest = szDest;

	if (CopyFile(strSource, strDest, FALSE) == FALSE)
	{
		m_strError = "Unable to copy file: ";
		m_strError += strSource;
		return FALSE;
	}

	return TRUE;
}


//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CUpgradeDlg::DisplayResult()
{
	CString strMsg = "";
	strMsg += "Convert ";
	strMsg += m_bAbort ? "Terminated" : "Successful";

	if (m_strError != "")
	{
		strMsg += "\n\n";
		strMsg += m_strError;
	}

	switch (m_nV1or2ServerTerminated)
	{
	case nOK:
		strMsg += "\n\n";
		strMsg += "The Loyalty ";
		strMsg += m_strOldVersion;
		strMsg += " Server was terminated.";
		break;

	case nERROR:
		strMsg += "\n\n";
		strMsg += "Unable to stop the Loyalty ";
		strMsg += m_strOldVersion;
		strMsg += " Server.";
		break;
	}

	if (FALSE == m_bAbort)
	{
		strMsg += "\n\nAfter closing this message, please check that the Request, Response and other";
		strMsg += "\nfolder paths in Server Options are still vaild for the new v4 system.";
	}

	AppendRemoveStartupLinkMessage(m_nV1StartupLinkRemoved, "Loyalty v1", strMsg );
	AppendRemoveStartupLinkMessage(m_nV2OldStartupLinkRemoved, "Loyalty v2", strMsg);
	AppendRemoveStartupLinkMessage(m_nV2NewStartupLinkRemoved, "SPos Loyalty v2", strMsg);

	Prompter.Info( strMsg, "System Convert" );
}

//$$******************************************************************

void CUpgradeDlg::AppendRemoveStartupLinkMessage(int nResult, CString strLoyaltyType, CString& strMsg)
{
	switch ( nResult )
	{
	case nOK:
		strMsg += "\n\nThe ";
		strMsg += strLoyaltyType;
		strMsg += " Server was removed from system startup.";
		break;

	case nERROR:
		strMsg += "\n\nUnable to remove the ";
		strMsg += strLoyaltyType;
		strMsg += " Server from system startup.";
		break;
	}
}

//$$******************************************************************

void CUpgradeDlg::ImportAccountDatabase()
{
	CString strLegacyDbPath = "";
	strLegacyDbPath += Sysset.GetProgramPath();
	strLegacyDbPath += "\\SysFiles\\File0002.dat";

	CImportDatabaseForManager DatabaseImport(strLegacyDbPath, this);

	if (DatabaseImport.IsValidFileForUpgrade() == TRUE)
	{
		DatabaseImport.DirectImport(IMPORT_METHOD_ALL,TRUE);
	}
}

//$$******************************************************************

void CUpgradeDlg::ImportGroupDatabase()
{
	CString strLegacyDbPath = "";
	strLegacyDbPath += Sysset.GetProgramPath();
	strLegacyDbPath += "\\SysFiles\\File0009.dat";

	CImportGroupForManager GroupImport(strLegacyDbPath, this);

	if (GroupImport.IsValidFileForUpgrade() == TRUE)
	{
		GroupImport.DirectImport(IMPORT_METHOD_ALL, TRUE);
	}
}

//$$******************************************************************

void CUpgradeDlg::ImportPluDatabase()
{
	CString strLegacyDbPath = "";
	strLegacyDbPath += Sysset.GetProgramPath();
	strLegacyDbPath += "\\SysFiles\\File0010.dat";

	CSSFile filePlu;
	if (filePlu.Open(strLegacyDbPath, "rb") == TRUE)
	{
		//discard header
		CString strBuffer = "";
		if (filePlu.ReadString(strBuffer) == TRUE)
		{
			while (filePlu.ReadString(strBuffer) == TRUE)
			{
				CCSV csv(strBuffer);

				CString strPluNo = csv.GetString(0);
				if (::TestNumeric(strPluNo) == FALSE)
				{
					continue;
				}

				__int64 nPluNo = _atoi64(strPluNo);

				if ((nPluNo < PluInfo::PluNo.Min) || ( nPluNo > PluInfo::PluNo.Max))
				{
					continue;
				}

				CSQLRowPluInfo RowPluInfo;
				RowPluInfo.SetPluNo(nPluNo);
				RowPluInfo.SetDescription(csv.GetString(1));
				RowPluInfo.SetPoints(csv.GetInt(2));
				RowPluInfo.SetDeptNo(csv.GetInt(3));
				RowPluInfo.SetTaxCode(csv.GetString(4));
				RowPluInfo.SetModType(csv.GetInt(5));
				RowPluInfo.SetStampOfferID(0);

				CSQLRepositoryPluInfo repoInfo;
				repoInfo.UpsertRow(RowPluInfo, NULL);
			}
		}
	}

}

//$$******************************************************************

