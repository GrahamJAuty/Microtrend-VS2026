//$$******************************************************************
#include "..\SmartPay4Shared\EmailOptions.h"
#include "..\SmartPay4Shared\GlobalState.h"
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\PresentationOptions.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_BioRegister\SQLRepositoryBioRegister.h"
#include "..\SmartPay4Shared\SQLTable_CCNo\SQLRepositoryCCNo.h"
#include "..\SmartPay4Shared\SQLTable_ClosingBalance\SQLRepositoryClosingBalance.h"
#include "..\SmartPay4Shared\SQLTable_EposTerminal\SQLRepositoryEposTerminal.h"
#include "..\SmartPay4Shared\SQLTable_PeriodRefresh\SQLRepositoryPeriodRefresh.h"
#include "..\SmartPay4Shared\SQLTable_PluInfo\SQLRepositoryPluInfo.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
#include "..\SmartPay4Shared\SQLTable_WebPaymentOptions\SQLRepositoryWebPaymentOptions.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SmartPay4Shared\WaitDlg.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\DatabaseImport.h"
//$$******************************************************************
#include "GroupImportForManager.h"
#include "PCOptions.h"
#include "SmartPay4Manager.h"
//$$******************************************************************
#include "UpgradeDlg.h"
//$$******************************************************************
extern CSysset Sysset;
//$$******************************************************************
#define BUFSIZE MAX_PATH
//$$******************************************************************
#define szBGNDSTARTUP_V1 "Smp Processor.lnk"
#define szSERVERSTARTUP_V1_OLD "SmartPayServer.lnk"
#define szSERVERSTARTUP_V1_NEW "SPosSmartPayServer.lnk"
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
	m_nV1ServerTerminated = nNOTSET;
	m_nV1OldServerStartupLinkRemoved = nNOTSET;
	m_nV1NewServerStartupLinkRemoved = nNOTSET;
	m_nV1BgndTerminated = nNOTSET;
	m_nV1BgndStartupLinkRemoved = nNOTSET;
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
	m_bAbort = FALSE;

	if ( UpdateData() == TRUE )
	{
		int nSystemType = GetPreviousSystemType();

		switch (nSystemType)
		{
		case SYSSET_SMARTPAY4:
			m_bAbort = TRUE;
			m_strError = "The specified folder already contains SmartPay v4 data";
			break;

		case SYSSET_SMARTPAY2:
			ConvertV2();
			break;

		case SYSSET_SMARTPAY:
			ConvertV1();
			break;

		case 0:
		default:
			m_bAbort = TRUE;
			m_strError = "The specified folder does not contain any SmartPay data";
			break;
		}

		DisplayResult();

		if ( ( m_strError == "" ) && ( FALSE == m_bAbort ) )
		{
			System.Read();										 
			PresentationOptions.Read();
			EmailOptions.Read();
			PCOptionsClient.Read();
			PCOptionsHost.Read();
			Passwords.Read(Filenames.GetPasswordFilename());
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
		if ( BrowseFolder ( strPathname, "Select SmartPay v1 or v2 installaton folder", NULL, this ) == TRUE )
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
	arrayTypesToCheck.Add(SYSSET_SMARTPAY4);
	arrayTypesToCheck.Add(SYSSET_SMARTPAY2);
	arrayTypesToCheck.Add(SYSSET_SMARTPAY);

	bool bFound = FALSE;
	int nSystemType = 0;

	for (int n = 0; n < arrayTypesToCheck.GetSize(); n++)
	{
		nSystemType = arrayTypesToCheck.GetAt(n);

		CString strSystemFolder;
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

bool CUpgradeDlg::CheckV1Server ( const char* szSystemFolder, CWaitDlg* pDlgWait )
{
	if ( IsV1ServerRunning ( szSystemFolder ) == TRUE )
	{
		pDlgWait->SetMessageText ( "Terminating SmartPay v1 Server" );

		if ( TerminateV1Server ( szSystemFolder ) == FALSE )
		{
			m_nV1ServerTerminated = nERROR;
			m_bAbort = TRUE;
			return FALSE;
		}
		m_nV1ServerTerminated = nOK;
	}

// see if shortcut on Program Startup
	// see if shortcut on Program Startup

	{
		CString strStartupLink_v1;
		strStartupLink_v1.Format("%s\\%s", 
			(const char*) GetStartupPath(), 
			szSERVERSTARTUP_V1_OLD);

		if (::FileExists(strStartupLink_v1) == TRUE)
		{
			remove(strStartupLink_v1);			// remove link

			if (::FileExists(strStartupLink_v1) == TRUE)
			{
				m_nV1OldServerStartupLinkRemoved = nERROR;
			}
			else
			{
				m_nV1OldServerStartupLinkRemoved = nOK;
			}
		}
	}

	{
		CString strStartupLink_v1;
		strStartupLink_v1.Format("%s\\%s", 
			(const char*) GetStartupPath(), 
			szSERVERSTARTUP_V1_NEW);

		if (::FileExists(strStartupLink_v1) == TRUE)
		{
			remove(strStartupLink_v1);			// remove link

			if (::FileExists(strStartupLink_v1) == TRUE)
			{
				m_nV1NewServerStartupLinkRemoved = nERROR;
			}
			else
			{
				m_nV1NewServerStartupLinkRemoved = nOK;
			}
		}
	}

	{
		CString strStartupLink_v1;
		strStartupLink_v1.Format("%s\\%s", 
			(const char*) GetStartupPath(), 
			szBGNDSTARTUP_V1);

		if (::FileExists(strStartupLink_v1) == TRUE)
		{
			remove(strStartupLink_v1);			// remove link

			if (::FileExists(strStartupLink_v1) == TRUE)
			{
				m_nV1BgndStartupLinkRemoved = nERROR;
			}
			else
			{
				m_nV1BgndStartupLinkRemoved = nOK;
			}
		}
	}

	return TRUE;
}

//$$******************************************************************

bool CUpgradeDlg::IsV1ServerRunning ( const char* szSystemFolder )
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
		remove( strServerRunFile );
	}

	return bServerRunning;
}

//$$******************************************************************

bool CUpgradeDlg::TerminateV1Server ( const char* szSystemFolder )
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
		if ( ::FileExists ( strTerminateFilename ) == FALSE )	// see if request been been processed
			break;

		Sleep(1000);										// wait again
	}

	return !IsV1ServerRunning ( szSystemFolder );			// see if server not running
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

bool CUpgradeDlg::CheckV1Bgnd(const char* szSystemFolder, CWaitDlg* pDlgWait)
{
	if (IsV1BgndRunning(szSystemFolder) == TRUE)
	{
		pDlgWait->SetMessageText("Terminating SmartPay v1 Background Module");

		if (TerminateV1Bgnd(szSystemFolder) == FALSE)
		{
			m_nV1BgndTerminated = nERROR;
			m_bAbort = TRUE;
			return FALSE;
		}
		m_nV1BgndTerminated = nOK;
	}

	// see if shortcut on Program Startup
	{
		CString strStartupLink_v1;
		strStartupLink_v1.Format("%s\\%s", 
			(const char*) GetStartupPath(), 
			szBGNDSTARTUP_V1);

		if (::FileExists(strStartupLink_v1) == TRUE)
		{
			remove(strStartupLink_v1);			// remove link

			if (::FileExists(strStartupLink_v1) == TRUE)
			{
				m_nV1BgndStartupLinkRemoved = nERROR;
			}
			else
			{
				m_nV1BgndStartupLinkRemoved = nOK;
			}
		}
	}

	return TRUE;
}

//$$******************************************************************

bool CUpgradeDlg::IsV1BgndRunning(const char* szSystemFolder)
{
	bool bBgndRunning = FALSE;

	CString strBgndRunFile;
	strBgndRunFile.Format("%s\\SysFiles\\pfile.run", szSystemFolder);

	CSSFile file;
	int nRetries = 1;
	if (file.Open(strBgndRunFile, "wb", _SH_DENYRW, nRetries) == FALSE)
		bBgndRunning = TRUE;
	else
	{
		file.Close();
		remove(strBgndRunFile);
	}

	return bBgndRunning;
}

//$$******************************************************************

bool CUpgradeDlg::TerminateV1Bgnd(const char* szSystemFolder)
{
	CString strTerminateFilename;
	strTerminateFilename.Format("%s\\SysFiles\\pfile.xxx", szSystemFolder);

	CSSFile file;
	file.Open(strTerminateFilename, "wb");
	file.Close();

	Sleep(500);												// wait a bit

	int nCount;
	for (nCount = 0; nCount < 10; nCount++)
	{
		if (::FileExists(strTerminateFilename) == FALSE)	// see if request been been processed
			break;

		Sleep(1000);										// wait again
	}

	return !IsV1BgndRunning(szSystemFolder);			// see if server not running
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CUpgradeDlg::ConvertV1()
{
	CWaitDlg dlgWait;

	CString strSystemFolder = "";
	strSystemFolder.Format("%s\\%s", 
		(const char*) m_strSourcePath, 
		(const char*) Sysset.GetProgramFolder(SYSSET_SMARTPAY));		
	
	if (CheckV1Server(strSystemFolder, &dlgWait) == FALSE)
	{
		return;
	}

	if (CheckV1Bgnd(strSystemFolder, &dlgWait) == FALSE)
	{
		return;
	}

	dlgWait.SetMessageText("Extracting SmartPay v1 data");

	CString strSourcePath = "";
	strSourcePath = m_strSourcePath;
	strSourcePath += "\\";
	strSourcePath += Sysset.GetProgramFolder(SYSSET_SMARTPAY);

	CString strDestPath = "";
	strDestPath += Sysset.GetProgramPath();

	{
		CWorkingDlg WorkingDlg("Copying files from v1 System");
		WorkingDlg.Create();
		int nFileCount = 0;

		if (CopyFolderRecursive(strSourcePath, strDestPath, TRUE, FALSE, &WorkingDlg, nFileCount) == TRUE)
		{
			strSourcePath = "";
			strSourcePath += m_strSourcePath;
			strSourcePath += "\\Sysset\\";
			strSourcePath += Sysset.GetProgramFolder(SYSSET_SMARTPAY);

			CString strDestPath = "";
			strDestPath += Sysset.GetSyssetProgramPath();

			CopyFolderRecursive(strSourcePath, strDestPath, TRUE, FALSE, &WorkingDlg, nFileCount);
		}
	}

	ImportAccountDatabase();
	ImportLeaverDatabase();
	ImportGroupDatabase();
	ImportPluDatabase();
	ImportAuditPeriods();
	ImportUserTexts();
	ImportPeriodRefresh();
	ImportClosingBalance();
	ImportEposTerminals();
	ImportCCNo();
	ImportWebPaymentOptions();
}

//$$******************************************************************

void CUpgradeDlg::ConvertV2()
{
	CWaitDlg dlgWait;

	CString strSystemFolder = "";
	strSystemFolder.Format("%s\\%s", 
		(const char*) m_strSourcePath, 
		(const char*) Sysset.GetProgramFolder(SYSSET_SMARTPAY2));

	{
		CSSFile fileStopServer;
		if (fileStopServer.Open(strSystemFolder + "\\SysFiles\\File0070.dat", "wb"))
		{
			fileStopServer.WriteLine("Stop");
		}
		else
		{
			m_strError = "Unable to create v2 Server Stop file";
			return;
		}
	}

	{
		CSSFile fileStopBgnd;
		if (fileStopBgnd.Open(strSystemFolder + "\\SysFiles\\File0066.dat", "wb"))
		{
			fileStopBgnd.WriteLine("Stop");
		}
		else
		{
			m_strError = "Unable to create v2 Background Stop file";
			return;
		}
	}

	dlgWait.SetMessageText("Extracting SmartPay v2 data");

	CString strSourcePath = "";
	strSourcePath = m_strSourcePath;
	strSourcePath += "\\";
	strSourcePath += Sysset.GetProgramFolder(SYSSET_SMARTPAY2);

	CString strDestPath = "";
	strDestPath += Sysset.GetProgramPath();

	{
		CWorkingDlg WorkingDlg("Copying files from v2 System");
		WorkingDlg.Create();
		int nFileCount = 0;

		if (CopyFolderRecursive(strSourcePath, strDestPath, TRUE, FALSE, &WorkingDlg, nFileCount) == TRUE)
		{
			strSourcePath = "";
			strSourcePath += m_strSourcePath;
			strSourcePath += "\\Sysset\\";
			strSourcePath += Sysset.GetProgramFolder(SYSSET_SMARTPAY2);

			CString strDestPath = "";
			strDestPath += Sysset.GetSyssetProgramPath();

			CopyFolderRecursive(strSourcePath, strDestPath, TRUE, FALSE, &WorkingDlg, nFileCount);
		}
	}

	ImportAccountDatabase();
	ImportLeaverDatabase();
	ImportGroupDatabase();
	ImportPluDatabase();
	ImportAuditPeriods();
	ImportUserTexts();
	ImportPeriodRefresh();
	ImportClosingBalance();
	ImportEposTerminals();
	ImportCCNo();
	ImportWebPaymentOptions();
}

//$$******************************************************************

bool CUpgradeDlg::CopyFolderRecursive(const char* szSourceFolder, const char* szDestFolder, bool bSubFolders, bool bExcludeRootFiles, CWorkingDlg* pWorking, int& nFileCount)
{
	CString strSourceFolder = szSourceFolder;
	CString strDestFolder = szDestFolder;

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
		bWorking = (FileFinder.FindFile(strSourceFolder + "*.*") != 0);

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

		if (CopyFolderRecursive(strSourceFolder + strFilename, strDestFolder + strFilename, TRUE, FALSE, pWorking, nFileCount) == FALSE)
			return FALSE;
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

	switch (m_nV1ServerTerminated)
	{
	case nOK:
		strMsg += "\n\nThe SmartPay v1 Server was terminated.";
		break;

	case nERROR:
		strMsg += "\n\nUnable to stop the SmartPay v1 Server.";
		break;
	}

	AppendRemoveStartupLinkMessage(m_nV1OldServerStartupLinkRemoved, "SmartPay v1", strMsg);
	AppendRemoveStartupLinkMessage(m_nV1NewServerStartupLinkRemoved, "SmartPay v1 (SPos)", strMsg);

	switch (m_nV1BgndTerminated)
	{
	case nOK:
		strMsg += "\n\nThe SmartPay v1 Background module was terminated.";
		break;

	case nERROR:
		strMsg += "\n\nUnable to stop the SmartPay v1 Background module.";
		break;
	}

	switch (m_nV1BgndStartupLinkRemoved)
	{
	case nOK:
		strMsg += "\n\nThe SmartPay v1 Background Module was removed from system startup.";
		break;

	case nERROR:
		strMsg += "\n\nUnable to remove the SmartPay v1 Background Module from system startup.";
		break;
	}

	if (FALSE == m_bAbort)
	{
		strMsg += "\n\nAfter closing this message, please check that the Request, Response and other";
		strMsg += "\nfolder paths in Server Options are still vaild for the new v4 system.";
	}

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

	CImportDatabase DatabaseImport(this);

	if (DatabaseImport.IsValidFileForUpgrade(strLegacyDbPath) == TRUE)
	{
		DatabaseImport.DirectImport(strLegacyDbPath);
	}
}

//$$******************************************************************

void CUpgradeDlg::ImportLeaverDatabase()
{
	CString strLegacyDbPath = "";
	strLegacyDbPath += Sysset.GetProgramPath();
	strLegacyDbPath += "\\Leavers\\";

	CFileFind FileFinder;
	bool bWorking = (FileFinder.FindFile(strLegacyDbPath + "*.*") != 0);

	while (bWorking)
	{
		(bWorking = FileFinder.FindNextFile() != 0);

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if (FileFinder.IsDots() == TRUE)
		{
			continue;
		}

		//IGNORE INDIVIDUAL FILES
		if (FileFinder.IsDirectory() == FALSE)
		{
			continue;
		}

		CString strFolder = FileFinder.GetFileName();

		if (strFolder.GetLength() != 9)
		{
			continue;
		}

		if (strFolder.Mid(4, 1) != "-")
		{
			continue;
		}

		CString strYear1 = strFolder.Left(4);
		CString strYear2 = strFolder.Right(4);

		if (::TestNumeric(strYear1) == FALSE)
		{
			continue;
		}

		if (::TestNumeric(strYear2) == FALSE)
		{
			continue;
		}

		int nYear1 = atoi(strYear1);
		int nYear2 = atoi(strYear2);
		
		if (nYear2 != nYear1 + 1)
		{
			continue;
		}

		CString strImportPath = FileFinder.GetFilePath();
		strImportPath += "\\SysFiles\\File0002.dat";

		CImportDatabase DatabaseImport(this);

		if (DatabaseImport.IsValidFileForUpgrade(strImportPath) == TRUE)
		{
			g_GlobalState.SetLeaverYear(nYear2);
			DatabaseImport.DirectImport(strImportPath);
			g_GlobalState.ClearLeaverYear();
		}
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
		GroupImport.DirectImport(TRUE);
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

				if ((nPluNo < PluInfo::PluNo.Min) || (nPluNo > PluInfo::PluNo.Max))
				{
					continue;
				}

				CSQLRowPluInfo RowPluInfo;
				RowPluInfo.SetPluNo(nPluNo);
				RowPluInfo.SetDescription(csv.GetString(1));
				RowPluInfo.SetPoints(csv.GetInt(2));
				RowPluInfo.SetDeptNo(csv.GetInt(3));
				RowPluInfo.SetTaxCode(csv.GetString(4));

				CSQLRepositoryPluInfo RepoPlu;
				RepoPlu.UpsertRow(RowPluInfo, NULL);
			}
		}
	}
}

//$$******************************************************************

void CUpgradeDlg::ImportAuditPeriods()
{
	CString strLegacyDbPath = "";
	strLegacyDbPath += Sysset.GetProgramPath();
	strLegacyDbPath += "\\SysFiles\\File0011.dat";

	int nPeriod = 0;

	SQLRowSetAuditPeriod.LoadPeriods();

	CSSFile filePeriods;
	if (filePeriods.Open(strLegacyDbPath, "rb") == TRUE)
	{
		CString strBuffer = "";
		while ( ( nPeriod < nMAXPERIODS) && (filePeriods.ReadString(strBuffer) == TRUE) )
		{
			CCSV csv(strBuffer);
			int nOldTime = csv.GetInt(1);
			int nNewTime = ((nOldTime / 100) * 60) + (nOldTime % 100);
			SQLRowSetAuditPeriod.SetPeriodName(nPeriod,csv.GetString(0));
			SQLRowSetAuditPeriod.SetStartTime(nPeriod, nNewTime);
			nPeriod++;
		}
	}

	SQLRowSetAuditPeriod.SavePeriods();
}

//$$******************************************************************

void CUpgradeDlg::ImportUserTexts()
{
	CString strLegacyDbPath = "";
	strLegacyDbPath += Sysset.GetProgramPath();
	strLegacyDbPath += "\\SysFiles\\File0015.dat";

	CSSFile fileTexts;
	if (fileTexts.Open(strLegacyDbPath, "rb") == TRUE)
	{
		CString strBuffer = "";
		while (fileTexts.ReadString(strBuffer) == TRUE)
		{
			int nLen = strBuffer.GetLength();
			int nPos = strBuffer.Find('=', 0);

			if ( (nPos >= 1) && ( nLen >= nPos + 2) )
			{
				CSQLRepositoryUserText RepoText;
				RepoText.SetUserText(strBuffer.Left(nPos), strBuffer.Mid(nPos + 1),NULL);
			}
		}
	}
}

//$$******************************************************************

void CUpgradeDlg::ImportBioRegister()
{
	CString strLegacyDbPath = "";
	strLegacyDbPath += Sysset.GetProgramPath();
	strLegacyDbPath += "\\SysFiles\\File0048.dat";

	CSSFile fileBioReg;
	if (fileBioReg.Open(strLegacyDbPath, "rb") == TRUE)
	{
		CString strHeader = "";
		if (fileBioReg.ReadString(strHeader) == TRUE)
		{
			bool bGotID = FALSE;
			CCSV csvHeader(strHeader);

			CString strBuffer = "";
			while (fileBioReg.ReadString(strBuffer) == TRUE)
			{
				CCSV csvLine(strBuffer);
				CSQLRowBioRegister RowBio;

				for (int n = 0; n < csvHeader.GetSize(); n++)
				{
					CString strField = csvHeader.GetString(n);
					strField.MakeUpper();

					if (strField == "UID")
					{
						RowBio.SetUserID(csvLine.GetInt64(n));
						bGotID = TRUE;
					}
					else if (strField == "N1")
					{
						RowBio.SetUsername(csvLine.GetString(n));
					}
					else if (strField == "C1")
					{
						RowBio.SetBioCounter(csvLine.GetInt(n));
					}
					else if (strField == "D1")
					{
						RowBio.SetFirstDate(csvLine.GetString(n));
					}
					else if (strField == "T1")
					{
						RowBio.SetFirstTime(csvLine.GetString(n));
					}
					else if (strField == "D2")
					{
						RowBio.SetLastDate(csvLine.GetString(n));
					}
					else if (strField == "T2")
					{
						RowBio.SetLastTime(csvLine.GetString(n));
					}
				}

				if (FALSE == bGotID)
				{
					return;
				}
	
				CSQLRepositoryBioRegister Repo;
				Repo.InsertRow(RowBio,NULL);
			}
		}
	}
}

//$$******************************************************************

void CUpgradeDlg::ImportPeriodRefresh()
{
	CString strLegacyDbPath = "";
	strLegacyDbPath += Sysset.GetProgramPath();
	strLegacyDbPath += "\\SysFiles\\File0046.dat";

	CSSFile fileRefresh;
	if (fileRefresh.Open(strLegacyDbPath, "rb") == TRUE)
	{
		CString strHeader = "";
		if (fileRefresh.ReadString(strHeader) == TRUE)
		{
			bool bGotID = FALSE;
			CCSV csvHeader(strHeader);

			CString strBuffer = "";
			while (fileRefresh.ReadString(strBuffer) == TRUE)
			{
				CCSV csvLine(strBuffer);
				CSQLRowPeriodRefresh RowRefresh;

				for (int n = 0; n < csvHeader.GetSize(); n++)
				{
					CString strField = csvHeader.GetString(n);
					strField.MakeUpper();

					if (strField == "UID")
					{
						RowRefresh.SetUserID(csvLine.GetInt64(n));
						bGotID = TRUE;
					}
					else if (strField == "P1D")
					{
						RowRefresh.SetPeriod1Date(csvLine.GetString(n), TRUE);
					}
					else if (strField == "P1B")
					{
						RowRefresh.SetPeriod1Balance(csvLine.GetDouble(n));
					}
					else if (strField == "P2D")
					{
						RowRefresh.SetPeriod2Date(csvLine.GetString(n), TRUE);
					}
					else if (strField == "P2B")
					{
						RowRefresh.SetPeriod2Balance(csvLine.GetDouble(n));
					}
					else if (strField == "P3D")
					{
						RowRefresh.SetPeriod3Date(csvLine.GetString(n), TRUE);
					}
					else if (strField == "P3B")
					{
						RowRefresh.SetPeriod3Balance(csvLine.GetDouble(n));
					}
					else if (strField == "P4D")
					{
						RowRefresh.SetPeriod4Date(csvLine.GetString(n), TRUE);
					}
					else if (strField == "P4B")
					{
						RowRefresh.SetPeriod4Balance(csvLine.GetDouble(n));
					}
					else if (strField == "P5D")
					{
						RowRefresh.SetPeriod5Date(csvLine.GetString(n), TRUE);
					}
					else if (strField == "P5B")
					{
						RowRefresh.SetPeriod5Balance(csvLine.GetDouble(n));
					}
					else if (strField == "P6D")
					{
						RowRefresh.SetPeriod6Date(csvLine.GetString(n), TRUE);
					}
					else if (strField == "P6B")
					{
						RowRefresh.SetPeriod6Balance(csvLine.GetDouble(n));
					}
				}

				if (FALSE == bGotID)
				{
					return;
				}

				CSQLRepositoryPeriodRefresh RepoPeriod;
				RepoPeriod.InsertRow(RowRefresh,NULL);
			}
		}
	}
}

//$$******************************************************************

void CUpgradeDlg::ImportClosingBalance()
{
	CString strLegacyDbPath = "";
	strLegacyDbPath += Sysset.GetProgramPath();
	strLegacyDbPath += "\\SysFiles\\File0036.dat";

	CSSFile fileClosing;
	if (fileClosing.Open(strLegacyDbPath, "rb") == TRUE)
	{
		CString strHeader = "";
		if (fileClosing.ReadString(strHeader) == TRUE)
		{
			bool bGotKey = FALSE;
			CCSV csvHeader(strHeader);

			CString strBuffer = "";
			while (fileClosing.ReadString(strBuffer) == TRUE)
			{
				CCSV csvLine(strBuffer);
				CSQLRowClosingBalance RowClosing;

				for (int n = 0; n < csvHeader.GetSize(); n++)
				{
					CString strField = csvHeader.GetString(n);
					strField.MakeUpper();

					if (strField == "K1")
					{
						CString strKey = csvLine.GetString(n);
						if ((strKey.GetLength() == 8) && (TestNumeric(strKey) == TRUE))
						{
							RowClosing.SetKeyDate(strKey);
							bGotKey = TRUE;
						}
					}
					else if (strField == "D1")
					{
						CString strInfo = csvLine.GetString(n);
						if ((strInfo.GetLength() == 8) && (TestNumeric(strInfo) == TRUE))
						{
							CString strDate = "";
							strDate.Format("%s/%s/%s",
								(const char*) strInfo.Left(2),
								(const char*) strInfo.Mid(2, 2),
								(const char*) strInfo.Right(4));

							RowClosing.SetDate(strDate);
						}
					}
					else if (strField == "T1")
					{
						CString strInfo = csvLine.GetString(n);
						if ((strInfo.GetLength() == 4) && (TestNumeric(strInfo) == TRUE))
						{
							CString strDate = "";
							strDate.Format("%s:%s",
								(const char*) strInfo.Left(2),
								(const char*) strInfo.Right(2));

							RowClosing.SetTime(strDate);
						}
					}
					else if (strField == "D2")
					{
						CString strInfo = csvLine.GetString(n);
						if ((strInfo.GetLength() == 8) && (TestNumeric(strInfo) == TRUE))
						{
							CString strDate = "";
							strDate.Format("%s/%s/%s",
								(const char*)strInfo.Left(2),
								(const char*)strInfo.Mid(2, 2),
								(const char*)strInfo.Right(4));

							RowClosing.SetArchiveDate(strDate);
						}
					}
					else if (strField == "T2")
					{
						CString strInfo = csvLine.GetString(n);
						if ((strInfo.GetLength() == 4) && (TestNumeric(strInfo) == TRUE))
						{
							CString strDate = "";
							strDate.Format("%s:%s",
								(const char*) strInfo.Left(2),
								(const char*) strInfo.Right(2));

							RowClosing.SetArchiveTime(strDate);
						}
					}
					else if (strField == "A1")
					{
						RowClosing.SetPurse1LiabilityAudit(csvLine.GetDouble(n));
					}
					else if (strField == "A2")
					{
						RowClosing.SetPurse2BalanceAudit(csvLine.GetDouble(n));
					}
					else if (strField == "A3")
					{
						RowClosing.SetPurse3LiabilityAudit(csvLine.GetDouble(n));
					}
					else if (strField == "C1")
					{
						RowClosing.SetPurse1CreditAudit(csvLine.GetDouble(n));
					}
					else if (strField == "C3")
					{
						RowClosing.SetPurse3CreditAudit(csvLine.GetDouble(n));
					}
				}

				if (TRUE == bGotKey)
				{
					CSQLRepositoryClosingBalance RepoClosing;
					RepoClosing.UpsertRow(RowClosing, NULL);
				}
			}
		}
	}
}

//$$******************************************************************

void CUpgradeDlg::ImportEposTerminals()
{
	CSQLRepositoryEposTerminal RepoTerm;
	RepoTerm.UpgradeLegacyData(NULL, FALSE);
}

//$$******************************************************************

void CUpgradeDlg::ImportCCNo()
{
	CSQLRepositoryCCNo RepoCC;
	RepoCC.ImportLegacyCCNo(NULL);
}

//$$******************************************************************

void CUpgradeDlg::ImportWebPaymentOptions()
{
	CSQLRepositoryWebPaymentOptions RepoWeb;
	RepoWeb.ImportLegacyOptions(NULL);
}

//$$******************************************************************
