/**********************************************************************/
#include "..\\SmartPay4Shared\FileRemover.h"
/**********************************************************************/
#include "..\\SmartPay4Shared\EmailOptions.h"
/**********************************************************************/
#include "SMTPEmailHelpers.h"
/**********************************************************************/

CSMTPSendEmailErrorInfo::CSMTPSendEmailErrorInfo()
{
	m_nNameTableIdx = 0;
	m_nStatementError = 0;
	m_nSMTPError = 0;
	m_strErrorText = "";
}

/**********************************************************************/

void CSMTPSendEmailErrorInfo::GetErrorText(CString& strText)
{
	strText = "";

	switch (m_nStatementError)
	{
	case SMTP_SEND_ERROR_EMAIL:
		strText = m_strErrorText;
		break;

	case SMTP_SEND_ERROR_NOADDRESS:
		strText = "No email address specified";
		break;

	case SMTP_SEND_ERROR_REDIRECT:
		strText = "Redirected to printer";
		break;

	case SMTP_SEND_ERROR_NICKNAME_FIND:
		strText = "Unknown email nickname";
		break;

	case SMTP_SEND_ERROR_NICKNAME_EMAIL:
		strText = "No email addresses for nickname";
		break;

	case SMTP_SEND_ERROR_NONE:
		strText = "OK";
		break;

	case SMTP_SEND_ERROR_SMTP:
		CSMTPEmailHelpers::GetErrorText(m_nSMTPError, strText);
		break;

	default:
		strText = "Unknown error";
		break;
	}
}

/**********************************************************************/

CSMTPPendingEmailInfo::CSMTPPendingEmailInfo()
{
	m_strFilename = "";
	m_strFriendlyName = "";
	m_nStatus = SMTP_PENDING_WAIT_SEND;
	m_nContextError = 0;
	m_nSMTPError = SMTP_ERROR_NONE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSMTPEmailHelpers::CSMTPEmailHelpers()
{
	m_strCurrentSession = "";
	m_nErrorCount = 0;
	m_nRedirectCount = 0;
	m_nSessionType = 0;
}

/**********************************************************************/

void CSMTPEmailHelpers::OpenSession(int nType)
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	m_strCurrentSession.Format("%2.2d_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d",
		nType,
		timeNow.GetYear(),
		timeNow.GetMonth(),
		timeNow.GetDay(),
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond());

	m_arraySession.RemoveAll();
	m_nErrorCount = 0;
	m_nRedirectCount = 0;
	m_nSessionType = nType;
}

/**********************************************************************/

const char* CSMTPEmailHelpers::GetIndexFileName(bool bCreateFolders)
{
	CString strEmailFolder = Filenames.GetSyssetFilename("EmailSender", "");

	if (TRUE == bCreateFolders)
	{
		if (::ExistSubdirectory(strEmailFolder) == FALSE)
		{
			::CreateSubdirectory(strEmailFolder);
		}
	}

	m_strIndexFileName = "";
	m_strIndexFileName += strEmailFolder;
	m_strIndexFileName += "\\";
	m_strIndexFileName += m_strCurrentSession;
	m_strIndexFileName += ".dat";

	return m_strIndexFileName;
}

/**********************************************************************/

const char* CSMTPEmailHelpers::GetSessionFolderName(bool bCreateFolders)
{
	CString strEmailFolder = Filenames.GetSyssetFilename("EmailSender", "");

	if (TRUE == bCreateFolders)
	{
		if (::ExistSubdirectory(strEmailFolder) == FALSE)
		{
			::CreateSubdirectory(strEmailFolder);
		}
	}

	m_strSessionFolderName = strEmailFolder;
	m_strSessionFolderName += "\\";
	m_strSessionFolderName += m_strCurrentSession;

	if (TRUE == bCreateFolders)
	{
		if (::ExistSubdirectory(m_strSessionFolderName) == FALSE)
		{
			::CreateSubdirectory(m_strSessionFolderName);
		}
	}

	return m_strSessionFolderName;
}

/**********************************************************************/

const char* CSMTPEmailHelpers::GetEmailFileName(int nIdx)
{
	m_strEmailFileName = "";
	if ((nIdx >= 0) && (nIdx < m_arraySession.GetSize()))
	{
		CSMTPPendingEmailInfo info;
		info = m_arraySession.GetAt(nIdx);

		m_strEmailFileName += GetSessionFolderName();
		m_strEmailFileName += "\\";
		m_strEmailFileName += info.m_strFilename;
	}
	return m_strEmailFileName;
}

/**********************************************************************/

void CSMTPEmailHelpers::DeleteSession()
{
	if (m_strCurrentSession != "")
	{
		CFileRemover FileRemover;
		FileRemover.RemoveFile(GetIndexFileName());
		FileRemover.RemoveFolder(GetSessionFolderName());
		m_strCurrentSession = "";
	}
}

/**********************************************************************/

void CSMTPEmailHelpers::CreatePendingEmail(const char* szAddress, const char* szSubject, const char* szFriendlyName, const char* szFilename)
{
	if (m_strCurrentSession != "")
	{
		CString strEmailFilePath = "";
		strEmailFilePath += GetSessionFolderName(TRUE);
		strEmailFilePath += "\\";
		strEmailFilePath += szFilename;

		CSSFile fileNKN;
		if (fileNKN.Open(strEmailFilePath + ".NKN", "wb") == TRUE)
		{
			fileNKN.WriteLine(szAddress);
			fileNKN.WriteLine(szSubject);
		}

		CSMTPPendingEmailInfo info;
		info.m_strFilename = szFilename;
		info.m_strFriendlyName = szFriendlyName;
		info.m_nStatus = SMTP_PENDING_WAIT_SEND;
		info.m_nContextError = 0;
		info.m_nSMTPError = SMTP_ERROR_NONE;
		m_arraySession.Add(info);
	}
}

/**********************************************************************/

void CSMTPEmailHelpers::CreatePendingEmail(CStringArray& arrayEmailAddresses, const char* szSubject, const char* szFriendlyName, CReportFileDoc& ReportFileDoc, const char* szFilename)
{
	if (m_strCurrentSession != "")
	{
		CString strEmailFilePath = GetSessionFolderName(TRUE);
		strEmailFilePath += "\\";
		strEmailFilePath += szFilename;
		ReportFileDoc.SaveAsHTML(strEmailFilePath + ".HTM");

		CCSV csvAddresses;
		for (int n = 0; n < arrayEmailAddresses.GetSize(); n++)
		{
			csvAddresses.Add(arrayEmailAddresses.GetAt(n));
		}

		CString strAddresses = csvAddresses.GetLine();

		CSSFile fileNKN;
		if (fileNKN.Open(strEmailFilePath + ".NKN", "wb") == TRUE)
		{
			fileNKN.WriteLine(strAddresses);
			fileNKN.WriteLine(szSubject);
		}

		CSMTPPendingEmailInfo info;
		info.m_strFilename = szFilename;
		info.m_strFriendlyName = szFriendlyName;
		info.m_nStatus = SMTP_PENDING_WAIT_SEND;
		info.m_nContextError = 0;
		info.m_nSMTPError = SMTP_ERROR_NONE;
		m_arraySession.Add(info);
	}
}

/**********************************************************************/

void CSMTPEmailHelpers::FlagUnsendableEmail(const char* szFriendlyName, int nContextError)
{
	CSMTPPendingEmailInfo info;
	info.m_strFilename = "";
	info.m_strFriendlyName = szFriendlyName;
	info.m_nStatus = SMTP_PENDING_INVALID;
	info.m_nContextError = nContextError;
	info.m_nSMTPError = SMTP_ERROR_NONE;
	m_arraySession.InsertAt(m_nErrorCount++, info);
}

/**********************************************************************/

void CSMTPEmailHelpers::FlagRedirectedEmail(const char* szFriendlyName)
{
	CSMTPPendingEmailInfo info;
	info.m_strFilename = "";
	info.m_strFriendlyName = szFriendlyName;
	info.m_nStatus = SMTP_PENDING_REDIRECTED;
	info.m_nContextError = 0;
	info.m_nSMTPError = SMTP_ERROR_NONE;
	m_arraySession.InsertAt(m_nErrorCount + m_nRedirectCount++, info);
}

/**********************************************************************/

void CSMTPEmailHelpers::GetSessionEmailInfo(int nIdx, CSMTPPendingEmailInfo& info)
{
	if ((nIdx >= 0) && (nIdx < m_arraySession.GetSize()))
	{
		info = m_arraySession.GetAt(nIdx);
	}
}

/**********************************************************************/

void CSMTPEmailHelpers::SetSessionEmailInfo(int nIdx, CSMTPPendingEmailInfo& info)
{
	if ((nIdx >= 0) && (nIdx < m_arraySession.GetSize()))
	{
		m_arraySession.SetAt(nIdx, info);
	}
}

/**********************************************************************/

int CSMTPEmailHelpers::SendSessionEmail(CWnd* pParent, int nIdx)
{
	int nResult = SMTP_ERROR_UNKNOWN;

	CString strEmailPath = GetEmailFileName(nIdx);

	if (strEmailPath != "")
	{
		CString strToAddress = "";
		CString strSubject = "";

		{
			CSSFile fileNKN;
			if (fileNKN.Open(strEmailPath + ".NKN", "rb") == TRUE)
			{
				fileNKN.ReadString(strToAddress);
				fileNKN.ReadString(strSubject);
			}
		}

		CFileRemover FileRemover;
		FileRemover.RemoveFile(strEmailPath + ".ERR");
		FileRemover.RemoveFile(strEmailPath + ".OK");

		nResult = SendEmail(pParent, strToAddress, strSubject, strEmailPath);
	}

	return nResult;
}

/**********************************************************************/

int CSMTPEmailHelpers::SendEmail(CWnd* pParent, const char* szToAddress, const char* szSubject, const char* szEmailFilename)
{
	int nReturnCode = SMTP_ERROR_NONE;
	CString strEmailFilename = szEmailFilename;

	CString strServer = EmailOptions.GetEmailSMTPServerToUse();
	CString strUsername = EmailOptions.GetEmailSMTPUsernameToUse();
	CString strPassword = EmailOptions.GetEmailSMTPPasswordToUse();

	CString strFrom = EmailOptions.GetEmailSMTPFromAddressToUse();
	CString strTo = szToAddress;

	if ((strServer == "") || (strUsername == "") || (strPassword == "") || (strFrom == ""))
	{
		nReturnCode = SMTP_ERROR_CONFIG;
	}
	else
	{
		CChilkatSendMail CKSendMail;

		bool bTest = (SMTP_SESSIONTYPE_TEST == m_nSessionType);

		CString strBodyText = "";
		if (TRUE == bTest)
		{
			strBodyText += "This is a test email from ";
			strBodyText += AfxGetAppName();
			strBodyText += " to check the current SMTP settings.";
		}

		nReturnCode = CKSendMail.SendMail(
			strServer,
			strUsername,
			strPassword,
			EmailOptions.GetEmailSMTPPortToUse(),
			EmailOptions.GetEmailSMTPUseSSLFlagToUse(),
			szSubject,
			strBodyText,
			szEmailFilename,
			".HTM",
			FALSE,
			strFrom,
			EmailOptions.GetEmailSMTPReplyToToUse(),
			strTo,
			bTest);
	}

	return nReturnCode;
}

/**********************************************************************/

void CSMTPEmailHelpers::CreateIndexFile()
{
	if ( m_strCurrentSession != "" )
	{
		CString strIndexFilename = GetIndexFileName( TRUE );

		CSSFile fileIndex;
		if ( fileIndex.Open( strIndexFilename, "wb" ) == TRUE )
		{
			for ( int n = 0; n < m_arraySession.GetSize(); n++ )
			{
				CSMTPPendingEmailInfo info = m_arraySession.GetAt( n );

				CCSV csv;
				csv.Add( 1 );
				csv.Add( info.m_strFilename );
				csv.Add( info.m_strFriendlyName );
				csv.Add( info.m_nStatus );
				csv.Add( info.m_nContextError );
				csv.Add( info.m_nSMTPError );

				fileIndex.WriteLine( csv.GetLine() );
			}
		}
	}
}

/**********************************************************************/

void CSMTPEmailHelpers::GetErrorText(int nErrorCode, CString& strError)
{
	switch (nErrorCode)
	{
	case SMTP_ERROR_CONFIG:
		strError = "SMTP configuration error";
		break;

	case SMTP_ERROR_EXCEPTION:
		strError = "SMTP exception";
		break;

	case SMTP_ERROR_UNKNOWN:
	default:
		strError = "Unknown SMTP error";
		break;
	}
}

/**********************************************************************/

bool CSMTPEmailHelpers::CheckAllSent()
{
	bool bResult = TRUE;

	for (int n = 0; n < GetSessionSize(); n++)
	{
		CSMTPPendingEmailInfo info = m_arraySession.GetAt(n);

		if (info.m_nStatus != SMTP_PENDING_SENT)
		{
			bResult = FALSE;
			break;
		}
	}

	return bResult;
}

/**********************************************************************/
