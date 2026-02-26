/**********************************************************************/
#include <CkEmail.h>
#include <CkJsonObject.h>
#include <CkMailMan.h>
/**********************************************************************/

CSSMapi32::CSSMapi32()
{
	m_hInst = NULL;
	m_pMAPILogon = NULL;
	m_pMAPILogoff = NULL;
	m_pMAPIFindNext = NULL;
	m_pMAPIReadMail = NULL;
	m_pMAPIFreeBuffer = NULL;
	m_pMAPISendMail = NULL;
	m_pMAPIDeleteMail = NULL;
	m_strError = "";
}

/**********************************************************************/

CSSMapi32::~CSSMapi32()
{
	FreeLibrary();
}

/**********************************************************************/

bool CSSMapi32::SendMessage (  const char* szSubject, const char* szFilename )
{
	CStringArray recipArray;

	return SendMail ( recipArray, szSubject, "", szFilename, FALSE );
}

bool CSSMapi32::SendMessage (  const char* szEmail, const char* szSubject, const char* szFilename, bool bImmediate )
{
	CStringArray recipArray;
	recipArray.Add ( szEmail );

	return SendMail ( recipArray, szSubject, "", szFilename, bImmediate );
}

bool CSSMapi32::SendMessage (  CStringArray& recipArray, const char* szSubject, const char* szFilename, bool bImmediate )
{
	return SendMail ( recipArray, szSubject, "", szFilename, bImmediate );
}

/**********************************************************************/

bool CSSMapi32::SendAttachment ( const char* szSubject, const char* szFilename )
{
	CStringArray recipArray;

	return SendMail ( recipArray, szSubject, szSubject, szFilename, FALSE );
}

bool CSSMapi32::SendAttachment (  const char* szEmail, const char* szSubject, const char* szFilename, bool bImmediate )
{
	CStringArray recipArray;
	recipArray.Add ( szEmail );

	return SendMail ( recipArray, szSubject, szSubject, szFilename, bImmediate );
}

bool CSSMapi32::SendAttachment (  CStringArray& recipArray, const char* szSubject, const char* szFilename, bool bImmediate )
{
	return SendMail ( recipArray, szSubject, szSubject, szFilename, bImmediate );
}

/**********************************************************************/

bool CSSMapi32::ExtractAttachments ( const char* szSubject, const char* szDestPath, bool bDeleteAfterCopy, int& nTotalFilesCopied )
{
	CString strSubject = szSubject;	// only extract attachments if the subject matches this
	CString strDestPath = szDestPath;	// needs to be full path from drive including trailing backslash else saved in outlook express folder
	m_strError = "";
	nTotalFilesCopied = 0;			// total number of files copied

	int nCopyFileError = 0;			// number of copy file errors detected
	int nDeleteFileError = 0;		// number of delete file errors detected
	int nDeleteMailError = 0;		// number of delete mail errors detected
	bool bReply = FALSE;

	char szCurrentDirectory[MAX_PATH];	// save and restore the current directory since mapi32.dll changes it
	if ( GetCurrentDirectory ( MAX_PATH, szCurrentDirectory ) == 0 )		// save current directory
	{
		m_strError = "Cannot get current directory";
		return FALSE;
	}

	if (( bReply = LoadLibrary()) == TRUE )
	{
		char cProfile[] = "";
		char cPassword[] = "";

		ULONG ulUIParam = 0;
		LPTSTR lpszProfileName = cProfile;
		LPTSTR lpszPassword = cPassword;
		FLAGS flFlags = 0;
		ULONG ulReserved = 0;
		LHANDLE lhSession;

		if (( bReply = Logon ( ulUIParam, lpszProfileName, lpszPassword, flFlags, ulReserved, &lhSession )) == TRUE )
		{
			CStringArray strMessageIdArray;
			ulUIParam = 0;
			LPTSTR lpszMessageType = NULL;
			char szSeedMessageID[512] = "";
			flFlags = MAPI_GUARANTEE_FIFO | MAPI_LONG_MSGID;
			ulReserved = 0;
			char lpszMessageID[512+1];
			ULONG ulReply;

			while (( ulReply = FindNext ( lhSession, ulUIParam, lpszMessageType, szSeedMessageID, flFlags, ulReserved, lpszMessageID )) == SUCCESS_SUCCESS )
			{
				ulUIParam = 0;
				flFlags = MAPI_PEEK | MAPI_ENVELOPE_ONLY;
				ulReserved = 0;
				lpMapiMessage lpMessage;

				if (( bReply = ReadMail ( lhSession, ulUIParam, lpszMessageID, flFlags, ulReserved, &lpMessage )) == TRUE )
				{
					CString strMailSubject = lpMessage->lpszSubject;
					FreeBuffer ( lpMessage );	// allocated by readmail

					if ( strMailSubject.Left(strSubject.GetLength()) == strSubject )	// is it the subject we are looking for
					{
						CString strFilenamePrefix;
						if ( strMailSubject.GetLength() > strSubject.GetLength() )			// anything past the end of the subject we
							strFilenamePrefix = strMailSubject.Mid(strSubject.GetLength());	// are looking for is the filename prefix

						ulUIParam = 0;
						flFlags = MAPI_PEEK | MAPI_SUPPRESS_ATTACH;
						ulReserved = 0;

						if (( bReply = ReadMail ( lhSession, ulUIParam, lpszMessageID, flFlags, ulReserved, &lpMessage )) == TRUE )
						{
							CStringArray strFilenameArray;
							int nFileCount = lpMessage->nFileCount;
							for ( int i = 0 ; i < nFileCount ; i++ )
								strFilenameArray.Add ( lpMessage->lpFiles[i].lpszFileName );

							FreeBuffer ( lpMessage );	// allocated by readmail

							if ( nFileCount > 0 )
							{
								ulUIParam = 0;
								flFlags = MAPI_PEEK;
								ulReserved = 0;

								if (( bReply = ReadMail ( lhSession, ulUIParam, lpszMessageID, flFlags, ulReserved, &lpMessage )) == TRUE )
								{
									int nFilesCopied = 0;

									for ( int i = 0 ; i < nFileCount ; i++ )
									{
										CString strSource = lpMessage->lpFiles[i].lpszPathName;
										CString strDest = strDestPath + strFilenamePrefix + strFilenameArray.GetAt(i);

										if ( CopyFile ( strSource, strDest, FALSE ) != 0 )	// ok
										{
											if ( DeleteFile ( strSource ) != 0 )	// delete temp file
											{
												nFilesCopied++;		// file copied and deleted ok
												nTotalFilesCopied++;	// total number of files copied ok
											}
											else
												nDeleteFileError++;
										}
										else						// copy error
											nCopyFileError++;
									}

									if ( nFilesCopied == nFileCount )				// copy and delete ok
										strMessageIdArray.Add ( lpszMessageID );	// save id for deletion later

									FreeBuffer ( lpMessage );	// allocated by readmail
								}
								else
									break;
							}
						}
						else
							break;
					}
				}
				else
					break;

				ulUIParam = 0;
				lpszMessageType = NULL;
				strcpy_s ( szSeedMessageID, sizeof(szSeedMessageID), lpszMessageID );
				flFlags = MAPI_GUARANTEE_FIFO | MAPI_LONG_MSGID;
				ulReserved = 0;
			}
			if ( ulReply != MAPI_E_NO_MESSAGES )
				bReply = FALSE;

			if ( bDeleteAfterCopy == TRUE )
			{
				for ( int i = 0 ; i < strMessageIdArray.GetSize() ; i++ )
				{
					if ( DeleteMail ( lhSession, ulUIParam, (char*)(const char*)strMessageIdArray.GetAt(i), 0, 0 ) == FALSE )
						nDeleteMailError++;
				}
			}

			ulUIParam = 0;
			flFlags = 0;
			ulReserved = 0;
			Logoff ( lhSession, ulUIParam, flFlags, ulReserved );
		}
		FreeLibrary();
	}

	SetCurrentDirectory ( szCurrentDirectory );		// restore original directory

	if ( bReply == TRUE )
	{
		if ( nCopyFileError > 0 || nDeleteFileError > 0 || nDeleteMailError > 0 )
		{
			m_strError.Format ( "Errors detected: CopyFile(%d) DeleteFile(%d) DeleteMail(%d)", nCopyFileError, nDeleteFileError, nDeleteMailError );
			bReply = FALSE;
		}
	}

	return bReply;
}

/**********************************************************************/
/*	Low level functions										*/
/**********************************************************************/

bool CSSMapi32::LoadLibrary()
{
	if (( m_hInst = ::LoadLibrary ( "mapi32.dll" )) != NULL )
	{
		m_pMAPILogon = (ULONG (FAR PASCAL*) ( ULONG, LPTSTR, LPTSTR, FLAGS, ULONG, LPLHANDLE )) ::GetProcAddress ( m_hInst, "MAPILogon" );
		if ( m_pMAPILogon == NULL )
		{
			m_strError = "MAPILogon address error";
			return FALSE;
		}

		m_pMAPILogoff = (ULONG (FAR PASCAL*) ( LHANDLE, ULONG, FLAGS, ULONG )) ::GetProcAddress ( m_hInst, "MAPILogoff" );
		if ( m_pMAPILogoff == NULL )
		{
			m_strError = "MAPILogoff address error";
			return FALSE;
		}

		m_pMAPIFindNext = (ULONG (FAR PASCAL*) ( LHANDLE, ULONG, LPTSTR, LPTSTR, FLAGS, ULONG, LPTSTR )) ::GetProcAddress ( m_hInst, "MAPIFindNext" );
		if ( m_pMAPIFindNext == NULL )
		{
			m_strError = "MAPIFindNext address error";
			return FALSE;
		}

		m_pMAPIReadMail = (ULONG (FAR PASCAL*) ( LHANDLE, ULONG, LPTSTR, FLAGS, ULONG, lpMapiMessage FAR * )) ::GetProcAddress ( m_hInst, "MAPIReadMail" );
		if ( m_pMAPIReadMail == NULL )
		{
			m_strError = "MAPIReadMail address error";
			return FALSE;
		}

		m_pMAPIFreeBuffer = (ULONG (FAR PASCAL*) ( LPVOID )) ::GetProcAddress ( m_hInst, "MAPIFreeBuffer" );
		if ( m_pMAPIFreeBuffer == NULL )
		{
			m_strError = "MAPIFreeBuffer address error";
			return FALSE;
		}

		m_pMAPISendMail = (ULONG (FAR PASCAL*) ( LHANDLE, ULONG, lpMapiMessage, FLAGS, ULONG )) ::GetProcAddress ( m_hInst, "MAPISendMail" );
		if ( m_pMAPISendMail == NULL )
		{
			m_strError = "MAPISendMail address error";
			return FALSE;
		}

		m_pMAPIDeleteMail = (ULONG (FAR PASCAL*) ( LHANDLE, ULONG, LPTSTR, FLAGS, ULONG )) ::GetProcAddress ( m_hInst, "MAPIDeleteMail" );
		if ( m_pMAPIDeleteMail == NULL )
		{
			m_strError = "MAPIDeleteMail address error";
			return FALSE;
		}

		return TRUE;
	}
	else
		m_strError = "Cannot load mapi32.dll";

	return FALSE;
}

/**********************************************************************/

void CSSMapi32::FreeLibrary()
{
	if ( m_hInst != NULL )
		::FreeLibrary ( m_hInst );

	m_hInst = NULL;
}

/**********************************************************************/

bool CSSMapi32::Logon ( ULONG ulUIParam, LPTSTR lpszProfileName, LPTSTR lpszPassword, FLAGS flFlags, ULONG ulReserved, LPLHANDLE lhSession )
{
	ULONG ulError = m_pMAPILogon ( ulUIParam, lpszProfileName, lpszPassword, flFlags, ulReserved, lhSession );

	if ( ulError == SUCCESS_SUCCESS )
		return TRUE;

	m_strError = GetMapiError ( ulError );
	return FALSE;
}

bool CSSMapi32::Logoff ( LHANDLE lhSession, ULONG ulUIParam, FLAGS flFlags, ULONG ulReserved )
{
	ULONG ulError = m_pMAPILogoff ( lhSession, ulUIParam, flFlags, ulReserved );

	if ( ulError == SUCCESS_SUCCESS )
		return TRUE;

	m_strError = GetMapiError ( ulError );
	return FALSE;
}

ULONG CSSMapi32::FindNext ( LHANDLE lhSession, ULONG ulUIParam, LPTSTR lpszMessageType, LPTSTR szSeedMessageID, FLAGS flFlags, ULONG ulReserved, LPTSTR lpszMessageID )
{
	ULONG ulError = m_pMAPIFindNext ( lhSession, ulUIParam, lpszMessageType, szSeedMessageID, flFlags, ulReserved, lpszMessageID );

	if ( ulError != SUCCESS_SUCCESS && ulError != MAPI_E_NO_MESSAGES )
		m_strError = GetMapiError ( ulError );

	return ulError;
}

bool CSSMapi32::ReadMail ( LHANDLE lhSession, ULONG ulUIParam, LPTSTR lpszMessageID, FLAGS flFlags, ULONG ulReserved, lpMapiMessage FAR* lpMessage )
{
	ULONG ulError = m_pMAPIReadMail ( lhSession, ulUIParam, lpszMessageID, flFlags, ulReserved, lpMessage );

	if ( ulError == SUCCESS_SUCCESS )
		return TRUE;

	m_strError = GetMapiError ( ulError );
	return FALSE;
}

bool CSSMapi32::FreeBuffer ( LPVOID lpMessage )
{
	ULONG ulError = m_pMAPIFreeBuffer ( lpMessage );

	if ( ulError == SUCCESS_SUCCESS )
		return TRUE;

	m_strError = GetMapiError ( ulError );
	return FALSE;
}

bool CSSMapi32::SendMail ( LHANDLE lhSession, ULONG ulUIParam, lpMapiMessage lpMessage, FLAGS flFlags, ULONG ulReserved )
{
	ULONG ulError = m_pMAPISendMail ( lhSession, ulUIParam, lpMessage, flFlags, ulReserved );

	if ( ulError == SUCCESS_SUCCESS )
		return TRUE;

	m_strError = GetMapiError ( ulError );
	return FALSE;
}

bool CSSMapi32::DeleteMail ( LHANDLE lhSession, ULONG ulUIParam, LPTSTR lpszMessageID, FLAGS flFlags, ULONG ulReserved )
{
	ULONG ulError = m_pMAPIDeleteMail ( lhSession, ulUIParam, lpszMessageID, flFlags, ulReserved );

	if ( ulError == SUCCESS_SUCCESS )
		return TRUE;

	m_strError = GetMapiError ( ulError );
	return FALSE;
}

/**********************************************************************/
/*	Private functions										*/
/**********************************************************************/

bool CSSMapi32::SendMail ( CStringArray& recipArray, const char* szSubject, const char* szMessage, const char* szFilename, bool bImmediate )
{
	m_strError = "";		// clear error message
	bool bReply = FALSE;

	if ( _access ( szFilename, 0 ) == -1 )
	{
		m_strError = "Invalid path or filename";
		return FALSE;
	}

	char szCurrentDirectory[MAX_PATH];	// save and restore the current directory since mapi32.dll changes it

	if ( GetCurrentDirectory ( MAX_PATH, szCurrentDirectory ) == 0 )		// save current directory
	{
		m_strError = "Cannot get current directory";
		return FALSE;
	}

	HINSTANCE hInst;

	ULONG (FAR PASCAL *pMAPISendMail) ( LHANDLE, ULONG, lpMapiMessage, FLAGS, ULONG );

	if (( hInst = ::LoadLibrary ( "mapi32.dll" )) != NULL )
	{
		pMAPISendMail = (ULONG (FAR PASCAL*) ( LHANDLE, ULONG, lpMapiMessage, FLAGS, ULONG )) ::GetProcAddress ( hInst, "MAPISendMail" );
		if ( pMAPISendMail != NULL )
		{
			CStringArray smtpArray;

			int nRecipCount = recipArray.GetSize();		// number of recipients

			if ( nRecipCount == 0 )
				recipArray.Add ( "" );

			int nRecipArraySize = recipArray.GetSize();
			MapiRecipDesc* lpRecips = new MapiRecipDesc[nRecipArraySize];
			
			if ( lpRecips != NULL )
			{
				for ( int i = 0 ; i < nRecipArraySize ; i++ )
				{
					lpRecips[i].ulRecipClass = (0 == i) ? MAPI_TO : MAPI_CC;

					CString strSMTP = "SMTP:";	// prefix email address with SMTP:
					strSMTP += recipArray.GetAt(i);
					smtpArray.Add ( strSMTP );	// add to smtp array

					lpRecips[i].ulReserved = 0;
					lpRecips[i].lpszName = (char*)(const char*) recipArray.GetAt(i);	// use email address as name
					lpRecips[i].lpszAddress = (char*)(const char*) smtpArray.GetAt(i);	// email address prefixed with SMTP:
					lpRecips[i].ulEIDSize = 0;
					lpRecips[i].lpEntryID = NULL;
				}

				char cFilename[] = "";

				MapiFileDesc file{};
				file.ulReserved = 0;
				file.flFlags = 0;
				file.nPosition = -1;
				file.lpszPathName = (char*) szFilename;		// must be full path and filename
				file.lpszFileName = cFilename;
				file.lpFileType = NULL;

				char cRecieved[] = "";
				char cID[] = "";

				MapiMessage msg{};
				msg.ulReserved = 0;
				msg.lpszSubject = (char*) szSubject;
				msg.lpszNoteText = (char*) szMessage;
				msg.lpszMessageType = NULL;	// outlook needs NULL outlook express was ok with ""
				msg.lpszDateReceived = cRecieved;
				msg.lpszConversationID = cID;
				msg.flFlags = 0;
				msg.lpOriginator = NULL;				// sender;
				msg.nRecipCount = nRecipCount;		// number of recipients
				msg.lpRecips = lpRecips;				// array of recipient structures
				msg.nFileCount = (strlen(szFilename) == 0) ? 0 : 1;
				msg.lpFiles = (strlen(szFilename) == 0) ? NULL : &file;

				LHANDLE lhSession = 0;
				ULONG ulUIParam = 0;
				FLAGS flFlags = ( bImmediate == TRUE ) ? 0 : MAPI_DIALOG;
				ULONG ulReserved = 0;

				ULONG ulReply = pMAPISendMail ( lhSession, ulUIParam, &msg, flFlags, ulReserved );

				if ( ulReply == SUCCESS_SUCCESS )
					bReply = TRUE;
				else
					m_strError = GetMapiError ( ulReply );
			}

			::FreeLibrary ( hInst );

			delete [] lpRecips;
		}
		else
			m_strError = "MAPISendMail address error";
	}
	else
		m_strError = "Cannot load mapi32.dll";

	SetCurrentDirectory ( szCurrentDirectory );		// restore original directory

	return bReply;
}

/**********************************************************************/

CString CSSMapi32::GetMapiError ( ULONG ulError )
{
	CString strError;

	switch ( ulError )
	{
	case MAPI_E_AMBIGUOUS_RECIPIENT:
		strError = "Ambiguous email recipient.";
		break;

	case MAPI_E_ATTACHMENT_NOT_FOUND:
		strError = "Email attachment not found.";
		break;

	case MAPI_E_ATTACHMENT_OPEN_FAILURE:
		strError = "Email attachment could not be opened.";
		break;

	case MAPI_E_ATTACHMENT_WRITE_FAILURE:
		strError = "Email attachment could not be written to a temporary file.";
		break;

	case MAPI_E_BAD_RECIPTYPE:
		strError = "Bad email recipient type.";
		break;

	case MAPI_E_DISK_FULL:
		strError = "Disk full.";
		break;

	case MAPI_E_FAILURE:
		strError = "Unspecified email error.";
		break;

	case MAPI_E_INSUFFICIENT_MEMORY:
		strError = "Insufficient memory to proceed with email.";
		break;

	case MAPI_E_INVALID_MESSAGE:
		strError = "Invalid message identifier.";
		break;

	case MAPI_E_INVALID_RECIPS:
		strError = "Invalid or unspecified email address.";
		break;

	case MAPI_E_INVALID_SESSION:
		strError = "Invalid session handle.";
		break;

	case MAPI_E_LOGIN_FAILURE:
		strError = "Email logon failure";
		break;

	case MAPI_E_TEXT_TOO_LARGE:
		strError = "Email message too large";
		break;

	case MAPI_E_TOO_MANY_FILES:
		strError = "Too many email attachments.";
		break;

	case MAPI_E_TOO_MANY_RECIPIENTS:
		strError = "Too many email recipients.";
		break;

	case MAPI_E_TOO_MANY_SESSIONS:
		strError = "Too many sessions open simultaneously.";
		break;

	case MAPI_E_UNKNOWN_RECIPIENT:
		strError = "Email recipient not found in address list.";
		break;

	case MAPI_E_USER_ABORT:
		strError = "Email aborted by user.";
		break;

	case SUCCESS_SUCCESS:
		strError = "";
		break;

	default:
		strError.Format ( "MAPI error: %lu", ulError );
		break;
	}

	return strError;
}

/**********************************************************************/

int CChilkatSendMail::SendMail(
	CString strServer,
	CString strUsername,
	CString strPassword,
	int nPort,
	bool bSSL,
	CString strSubject,
	CString strBodyTextFilename,
	CString strReportFilename,
	CString strExt,
	bool bAttachReport,
	CString strFrom,
	CString strReplyTo,
	CString strTo,
	bool bTest)
{
	int nResult = 0;

	CkMailMan mailman;

	// Set the SMTP server.
	mailman.put_SmtpHost(strServer);

	// Set the SMTP login/password (if required)
	mailman.put_SmtpUsername(strUsername);
	mailman.put_SmtpPassword(strPassword);
	mailman.put_SmtpPort(nPort);
	mailman.put_SmtpSsl(bSSL);
	mailman.put_StartTLS(bSSL);

	// Create a new email object
	CkEmail email;

	if (TRUE == bTest)
	{
		email.put_Body(strBodyTextFilename);
	}
	else if (FALSE == bAttachReport)
	{
		if (strReportFilename != "")
		{
			CkString bodyText;
			bodyText.loadFile(strReportFilename + strExt, "utf-8");
			email.put_Body(bodyText);
		}
	}
	else
	{
		email.addFileAttachment(strReportFilename + strExt);

		if (strBodyTextFilename != "")
		{
			CkString bodyText;
			bodyText.loadFile(strBodyTextFilename, "utf-8");
			email.put_Body(bodyText);
		}
	}

	email.put_Subject(strSubject);
	email.put_From(strFrom);

	if (strReplyTo != "")
	{
		email.put_ReplyTo(strReplyTo);
	}

	CCSV csvTo(strTo);
	for (int n = 0; n < csvTo.GetSize(); n++)
	{
		CString strAddress = csvTo.GetString(n);
		strAddress.TrimLeft();
		strAddress.TrimRight();

		if (strAddress != "")
		{
			bool success = email.AddTo("", strAddress);
		}
	}

	// To add more recipients, call AddTo, AddCC, or AddBcc once per recipient.

	// Call SendEmail to connect to the SMTP server and send.
	// The connection (i.e. session) to the SMTP server remains
	// open so that subsequent SendEmail calls may use the
	// same connection.  
	// 	
	if (mailman.SendEmail(email) != true)
	{
		CSSFile fileError;
		fileError.Open(strReportFilename + ".ERR", "wb");

		switch (mailman.get_ConnectFailReason())
		{
		case 0:
			break;

		default:
			fileError.WriteLine("Connection Error");
			break;
		}

		CkString reason;
		mailman.get_SmtpFailReason(reason);
		fileError.WriteLine(reason);

		nResult = 4;
	}
	else
	{
		CSSFile fileOK;
		fileOK.Open(strReportFilename + ".OK", "wb");
		fileOK.WriteLine("OK");
	}

	// Some SMTP servers do not actually send the email until 
	// the connection is closed.  In these cases, it is necessary to
	// call CloseSmtpConnection for the mail to be  sent.  
	// Most SMTP servers send the email immediately, and it is 
	// not required to close the connection.  We'll close it here
	// for the example:
	bool success = mailman.CloseSmtpConnection();

	return nResult;
}

/**********************************************************************/
