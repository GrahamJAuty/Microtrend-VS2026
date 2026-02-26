/**********************************************************************/
#include <CkSFtp.h>
#include <CkSFtpDir.h>
#include <CkSFtpFile.h>
/**********************************************************************/
#include "..\CommonEcrStockTray\AESHelpers.h"
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "FNBDatabaseOptions.h"
#include "FTPConnect.h"
#include "MessageLogger.h"
/**********************************************************************/
#include "PosTrayChartwellsSender.h"
/**********************************************************************/

CPosTrayChartwellsSender::CPosTrayChartwellsSender(void)
{
}

/**********************************************************************/

CPosTrayChartwellsSender::~CPosTrayChartwellsSender(void)
{
}

/**********************************************************************/

void CPosTrayChartwellsSender::SendExports()
{
	m_strSentFolder = "";
	GetSyssetProgramPath(m_strSentFolder);
	m_strSentFolder += "\\Chartwells";
	::CreateSubdirectory(m_strSentFolder);
	m_strSentFolder += "\\SENT";
	::CreateSubdirectory(m_strSentFolder);

	CStringArray arrayFilenames;
	GetFileList(ChartwellsOptions, arrayFilenames);

	m_strExtraErrorInfo = "";
	int nResult = FTPTransfer(ChartwellsOptions, arrayFilenames);

	if (CHARTWELLS_EXPORT_ERROR_NONE != nResult)
	{
		CString strExtra = "";
		if (m_strExtraErrorInfo != "")
		{
			strExtra += " (";
			strExtra += m_strExtraErrorInfo;
			strExtra += ")";
		}

		CString strMessage = "";
		bool bLeadingLineNetwork = FALSE;

		switch (nResult)
		{
		case CHARTWELLS_EXPORT_ERROR_FTP_CONNECT:
			bLeadingLineNetwork = TRUE;
			strMessage = "FTP Connection Error : ";
			break;

		case CHARTWELLS_EXPORT_ERROR_FTP_AUTHENTICATE:
			strMessage = "FTP Authentication Error : ";
			break;

		case CHARTWELLS_EXPORT_ERROR_FTP_INITIALISE:
			strMessage = "FTP Initialisation Error : ";
			break;

		default:
			strMessage = "Unknown Error : ";
			break;
		}

		strMessage += strExtra;
		LogMessage(strMessage, "ERROR", bLeadingLineNetwork);
	}
}

/**********************************************************************/

void CPosTrayChartwellsSender::GetFileList(CChartwellsOptions& ChartwellsOptions, CStringArray& arrayFilenames)
{
	CString strFolder = "";
	GetSyssetProgramPath(strFolder);
	strFolder += "CHARTWELLS\\EXPORTS\\";

	CString strFilenameOptions = ChartwellsOptions.GetFilename(FALSE);

	if (::FileExists(strFilenameOptions) == FALSE)
	{
		return;
	}

	if (ChartwellsOptions.Read(FALSE) == FALSE)
	{
		return;
	}

	if (ChartwellsOptions.GetEnable() == FALSE)
	{
		return;
	}

	CString strSiteNo = ChartwellsOptions.GetChartwellsSiteNo();
	if ( strSiteNo.IsEmpty() == TRUE )
	{
		return;
	}
	int nSiteNoLen = strSiteNo.GetLength();	

	CStringArray arrayAlreadySent;

	CFileFind finder;
	BOOL bWorking = finder.FindFile(strFolder + "*.csv");

	while (TRUE == bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots() == TRUE)
		{
			continue;
		}

		if (finder.IsDirectory() == TRUE)
		{
			continue;
		}

		CString strFilename = finder.GetFileName();
		int nFilenameLen = strFilename.GetLength();	

		if (nFilenameLen < nSiteNoLen + 11 )
		{
			continue;
		}

		if (::TestNumeric(strFilename.Left(8)) == FALSE)
		{
			continue;
		}

		if (strFilename.Mid(8, 1) != "_")
		{
			continue;
		}

		if (strFilename.Mid(9, nSiteNoLen) != strSiteNo)
		{
			continue;
		}

		if (strFilename.Mid(nSiteNoLen + 9, 1) != "_")
		{
			continue;
		}

		CString strType = strFilename.Right(nFilenameLen - ( nSiteNoLen + 10));
		strType.MakeUpper();

		if ((strType != "PAYMENT.CSV") && (strType != "HEADER.CSV") && ( strType != "SALES.CSV"))
		{
			continue;
		}

		CString strSentFileLog = m_strSentFolder;
		strSentFileLog += "\\";
		strSentFileLog += strFilename.Left(6);
		strSentFileLog += ".DAT";
		
		bool bAlreadySent = FALSE;

		CSSFile fileSent;
		if (fileSent.Open(strSentFileLog, "rb") == TRUE)
		{
			CString strBuffer;
			while (fileSent.ReadString(strBuffer) == TRUE)
			{
				CString strTest1 = strFilename;
				CString strTest2 = strBuffer;
				strTest1.MakeUpper();
				strTest2.MakeUpper();

				if (strTest1 == strTest2)
				{
					bAlreadySent = TRUE;
					break;
				}
			}
		}

		if (FALSE == bAlreadySent)
		{
			arrayFilenames.Add(strFilename);
		}
		else
		{
			CMessageLoggerInfo info;
			info.m_bAddDate = TRUE;
			info.m_bAddTimeOfDay = TRUE;
			info.m_bFollowingLineNetwork = FALSE;
			info.m_bFollowingLineSystem = FALSE;
			info.m_bLeadingLineNetwork = (arrayAlreadySent.GetSize() == 0);
			info.m_bLeadingLineSystem = FALSE;
			info.m_bLogSystemStartStop = FALSE;
			info.m_nHideLevel = 0;
			info.m_nNetworkIdx = 0;
			info.m_bAddNetworkName = FALSE;

			info.m_strMessage = "";
			info.m_strMessage += "File Already Sent (";
			info.m_strMessage += strFilename;
			info.m_strMessage += ")";

			MessageLogger.LogChartwellsSystem(info);

			arrayAlreadySent.Add(strFilename);
		}
	}

	for (int n = 0; n < arrayAlreadySent.GetSize(); n++)
	{
		CFileRemover FileRemover(strFolder + arrayAlreadySent.GetAt(n));
	}
}

/**********************************************************************/

int CPosTrayChartwellsSender::FTPTransfer( CChartwellsOptions& ChartwellsOptions, CStringArray& arrayFilenames )
{
	int nResult = CHARTWELLS_EXPORT_ERROR_NONE;

	if (arrayFilenames.GetSize() == 0)
	{
		return nResult;
	}

	CkSFtp SFTPConnect;

	SFTPConnect.put_ConnectTimeoutMs(5000);
	SFTPConnect.put_IdleTimeoutMs(10000);

	// Connect to the SSH server.  
	if ( SFTPConnect.Connect(ChartwellsOptions.GetFTPServer(), 22) == TRUE )
	{ 
		{
			CString strMessge = "Connect : ";
			strMessge += ChartwellsOptions.GetFTPServer();
			LogMessage(strMessge, "OK", TRUE);
		}

		CString strPassword = "";
		CAESHelpers::AESHexToPlainText(ChartwellsOptions.GetFTPPassword(), strPassword);
	
		if (SFTPConnect.AuthenticatePw(ChartwellsOptions.GetFTPUserName(), strPassword) == TRUE)
		{
			{
				CString strMessage = "Authenticate : ";
				strMessage += ChartwellsOptions.GetFTPServer();
				LogMessage(strMessage, "OK");
			}

			if (SFTPConnect.InitializeSftp() == TRUE)
			{
				{
					CString strMessage = "Initialise : ";
					strMessage += ChartwellsOptions.GetFTPServer();
					LogMessage(strMessage, "OK");
				}

				SendFiles(SFTPConnect, arrayFilenames, nResult);
			}
			else
			{
				m_strExtraErrorInfo = ChartwellsOptions.GetFTPServer();
				nResult = CHARTWELLS_EXPORT_ERROR_FTP_INITIALISE;
			}
		}
		else
		{
			m_strExtraErrorInfo = ChartwellsOptions.GetFTPServer();
			nResult = CHARTWELLS_EXPORT_ERROR_FTP_AUTHENTICATE;
		}

		SFTPConnect.Disconnect();
	}
	else
	{
		m_strExtraErrorInfo = ChartwellsOptions.GetFTPServer();
		nResult = CHARTWELLS_EXPORT_ERROR_FTP_CONNECT;
	}

	return nResult;
}

/**********************************************************************/

int CPosTrayChartwellsSender::UploadSiteList(CChartwellsOptions& ChartwellsOptions)
{
	int nResult = CHARTWELLS_EXPORT_ERROR_NONE;

	CkSFtp SFTPConnect;

	SFTPConnect.put_ConnectTimeoutMs(5000);
	SFTPConnect.put_IdleTimeoutMs(10000);

	// Connect to the SSH server.  
	if (SFTPConnect.Connect(ChartwellsOptions.GetFTPServer(), 22) == TRUE)
	{
		{
			CString strMessge = "Connect : ";
			strMessge += ChartwellsOptions.GetFTPServer();
			LogMessage(strMessge, "OK", TRUE);
		}

		CString strPassword = "";
		CAESHelpers::AESHexToPlainText(ChartwellsOptions.GetFTPPassword(), strPassword);

		if (SFTPConnect.AuthenticatePw(ChartwellsOptions.GetFTPUserName(), strPassword) == TRUE)
		{
			{
				CString strMessage = "Authenticate : ";
				strMessage += ChartwellsOptions.GetFTPServer();
				LogMessage(strMessage, "OK");
			}

			if (SFTPConnect.InitializeSftp() == TRUE)
			{
				{
					CString strMessage = "Initialise : ";
					strMessage += ChartwellsOptions.GetFTPServer();
					LogMessage(strMessage, "OK");
				}

				// Open a file on the server:
				const char* handle = SFTPConnect.openFile("SITELIST.DAT", "readOnly", "openExisting");
				if (SFTPConnect.get_LastMethodSuccess() == true)
				{
					CFilenameUpdater FnUp(SysFiles::ChartwellsSiteListIn);
					bool success = SFTPConnect.DownloadFile(handle, FnUp.GetFilenameToUse());
			
					SFTPConnect.CloseHandle(handle);
				}
			}
			else
			{
				m_strExtraErrorInfo = ChartwellsOptions.GetFTPServer();
				nResult = CHARTWELLS_EXPORT_ERROR_FTP_INITIALISE;
			}
		}
		else
		{
			m_strExtraErrorInfo = ChartwellsOptions.GetFTPServer();
			nResult = CHARTWELLS_EXPORT_ERROR_FTP_AUTHENTICATE;
		}

		SFTPConnect.Disconnect();
	}
	else
	{
		m_strExtraErrorInfo = ChartwellsOptions.GetFTPServer();
		nResult = CHARTWELLS_EXPORT_ERROR_FTP_CONNECT;
	}

	return nResult;
}

/**********************************************************************/

void CPosTrayChartwellsSender::SendFiles(CkSFtp& SFTPConnect, CStringArray& arrayFilenames, int& nResult)
{
	CString strFolder = "";
	GetSyssetProgramPath(strFolder);
	strFolder += "Chartwells\\EXPORTS\\";

	int nSiteNoLen = ChartwellsOptions.GetChartwellsSiteNo().GetLength();

	for (int n = 0; n < arrayFilenames.GetSize(); n++)
	{
		CString strFilename = arrayFilenames.GetAt(n);

		CString strSource = "";
		strSource += strFolder;
		strSource += strFilename;

		CString strDest = "test/";
		strDest += strFilename;

		// Open a file for writing on the SSH server.
		// If the file already exists, it is overwritten.
		// (Specify "createNew" instead of "createTruncate" to
		// prevent overwriting existing files.)
		const char* handle = SFTPConnect.openFile(strDest, "writeOnly", "createTruncate");
		if (SFTPConnect.get_LastMethodSuccess() != true)
		{
			m_strExtraErrorInfo = strFilename;
			nResult = CHARTWELLS_EXPORT_ERROR_FTP_OPENFILE;
			break;
		}
		// Upload from the local file to the SSH server.
		if (SFTPConnect.UploadFile(handle, strSource) != TRUE)
		{
			m_strExtraErrorInfo = strFilename;
			nResult = CHARTWELLS_EXPORT_ERROR_FTP_UPLOADFILE;
			break;
		}

		// -----------------------------------------------------------------------------
		// This is important.  You must close the handle on the server.
		// Otherwise open handles will accumulate on the server until eventually a limit
		// is reached and the server will fail on a call to OpenFile.
		// -----------------------------------------------------------------------------

		// Close the file.
		if (SFTPConnect.CloseHandle(handle) != TRUE)
		{
			LogMessage("Close Handle", "ERROR");
		}


		CString strMessage = "Sent File : ";
		strMessage += strFilename;
		LogMessage(strMessage, "OK");

		CString strSentFileLog = m_strSentFolder;
		strSentFileLog += "\\";
		strSentFileLog += strFilename.Left(6);
		strSentFileLog += ".DAT";

		CSSFile fileSent;
		if (fileSent.Open(strSentFileLog, "ab") == TRUE)
		{
			fileSent.WriteLine(strFilename);
			fileSent.Close();
		}

		CFileRemover FileRemover(strSource);
	}
}

/**********************************************************************/

void CPosTrayChartwellsSender::LogMessage(CString strMessage, CString strStatus, bool bLeadingLineNetwork)
{
	CMessageLoggerInfo info;
	info.m_bAddDate = TRUE;
	info.m_bAddNetworkName = FALSE;
	info.m_bAddTimeOfDay = TRUE;
	info.m_bFollowingLineNetwork = FALSE;
	info.m_bFollowingLineSystem = FALSE;
	info.m_bLeadingLineNetwork = bLeadingLineNetwork;
	info.m_bLeadingLineSystem = FALSE;
	info.m_bLogSystemStartStop = FALSE;
	info.m_nHideLevel = 0;
	info.m_strMessage = strMessage;
	info.m_strStatus = strStatus;
	MessageLogger.LogChartwellsSystem(info);
}

/**********************************************************************/

