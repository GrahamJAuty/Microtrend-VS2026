/**********************************************************************/

#include "Defines.h"
#include "Crypto.h"
#include "Register.h"
#include "MyServer.h"

/**********************************************************************/
// careful calling static functions from multi-threaded OnConnect() function
/**********************************************************************/
/*
	[WebServer]
		[AspDLL]
			gps.dll
		[private]
			[custlist]
				custlist.csv
			[logs]
				[2013]
					[201310]
						20131001.dat
						...
						20131031.dat
			[register]
				register.dbs
				register.log
				ecrset.dbs

			passwords.dat

		[wwwroot]
			[custlist]			// password protected
				Default.aspx
			[register]
				Default.aspx

		[VoiceModem]
			[Deleted]
			[Logs]
				[2013]
					VoiceModem.log
			[WaveAuto8]
			[WaveAuto16]
			VoiceModem.ini

		webserver.exe
		netenable.exe
		voicemodem.exe
		register.ini
		webserver.dat (optional)
*/
/**********************************************************************/

CMyServer::CMyServer()
	: CSSServer ( 50, 2000 )		// max 50 threads, exit timeout
{
	m_pWnd = NULL;
	m_nConnectionCount = 0;

	CSSFile file;
	if ( file.Open ( WEBSERVER_MIME_FILENAME, "rb" ) == TRUE )
	{
		CString strBuf;
		while ( file.ReadString ( strBuf ) == TRUE )
			m_arMime.Add ( strBuf );		// extension,content type

		file.Close();
	}

	if ( m_arMime.GetSize() == 0 )	// allow following file types
	{
		m_arMime.Add ( "aspx,text/html" );
		m_arMime.Add ( "htm,text/html" );
		m_arMime.Add ( "html,text/html" );
		m_arMime.Add ( "jpg,image/jpeg" );
		m_arMime.Add ( "gif,image/gif" );
		m_arMime.Add ( "png,image/png" );
		m_arMime.Add ( "css,text/css" );
		m_arMime.Add ( "js,application/javascript" );
		m_arMime.Add ( "tif,image/tiff" );
		m_arMime.Add ( "ico,image/x-icon" );
		m_arMime.Add ( "zip,application/zip" );
		m_arMime.Add ( "pdf,application/pdf" );
		m_arMime.Add ( "mp4,video/mp4" );
		m_arMime.Add ( "cgi,text/html" );
		m_arMime.Add ( "txt,text/plain" );
		m_arMime.Add ( "dat,text/plain" );
	}

	m_arDefaultFilenames.Add ( "default.aspx" );
	m_arDefaultFilenames.Add ( "default.htm" );
	m_arDefaultFilenames.Add ( "default.html" );
	m_arDefaultFilenames.Add ( "index.htm" );
	m_arDefaultFilenames.Add ( "index.html" );

	CreatePasswordArray ( m_arPasswords );		// create password array from password database
}

/**********************************************************************/

CMyServer::~CMyServer()
{
	UnloadDlls();	// free all loaded dlls
}

/**********************************************************************/

void CMyServer::OnConnect ( CSSSocket& sock, int nThreadNo, bool& bTerminate )
{
	CString strIpAddress = GetIpAddress ( sock );
	Log ( strIpAddress, "", "OnConnect (Start)", nThreadNo );

	DisplayWindowText ( ++m_nConnectionCount );

	int nInactivityTimer = 0;
	sock.SetRxTimeout ( 1000 );	// dont get blocked in ReadString() routine

	CString strError;
	CStringArray arRequest;
	CStringArray arCookies;

	while ( bTerminate == FALSE )
	{
		CString strBuf;
		if ( sock.ReadString ( strBuf ) == TRUE ) // blocks unless rxtimeout set
		{
			nInactivityTimer = 0;		// got data - reset inactivity timer

			Log ( strIpAddress, strBuf, "", nThreadNo );

			if ( strBuf != "" )
			{
				arRequest.Add ( strBuf );
			}
			else		// end of request
			{
				if ( arRequest.GetSize() > 0 )
				{
					CString str = arRequest.GetAt(0);
					CCSV csv ( str, ' ' );
					if ( csv.GetSize() >= 3 )
					{
						CString strVerb = csv.GetString(0);
						strVerb.MakeUpper();
						CString strData = csv.GetString(1);
						CString strVersion = csv.GetString(2);

						CString strURL;
						CString strFile;
						CString strParams;

						bool bGotVerb = FALSE;

						if ( strVerb == "GET" || strVerb == "HEAD" )		// e.g. GET /fred.html?x=1&y=2&z=3 HTTP/1.1
						{
							CCSV csvData ( strData, '?' );
							strURL = csvData.GetString(0);
							strFile = strURL;
							strFile.MakeLower();

							if ( csvData.GetSize() >= 2 )
								strParams = HtmlDecode ( csvData.GetString(1) );	// got params

							bGotVerb = TRUE;
						}
						else if ( strVerb == "POST" )		// e.g. POST /fred.html (cant see params)
						{
							strURL = strData;
							strFile = strURL;
							strFile.MakeLower();

							int nLength = atoi ( FindHeader ( "Content-Length: ", arRequest ) );
							if ( nLength > 0 )
							{
								if ( sock.ReadData ( strParams, nLength ) == TRUE )	// got params
								{
									strParams = HtmlDecode ( strParams );
									Log ( strIpAddress, strParams, "", nThreadNo );	// log params because they cant be seen on POST line
								}
							}

							bGotVerb = TRUE;
						}

						if ( bGotVerb == TRUE )
						{
							CCSV csvSessionCookies;
							GetSessionCookies ( arRequest, csvSessionCookies );

							CCSV csvUserParams ( strParams, '&' );		// eg. x=1&y=2&z=3
							void (*AspFunction)(bool,CCSV&,CAspArray&,CCSV&) = NULL;
							int (*ExecuteFunction)(const char*,CString&,CCSV&,CCSV&) = NULL;

							bool bCompleted = FALSE;

							if ( strFile == "/custlist/" )		// handled automatically, username and password required
							{
							}
							else if ( strFile == "/register/" )	// from microtrend.co.uk website, no username or password required
							{
								AspFunction = Register;
							}
							else if ( strFile == "/webreg.asp" )	// hard coded into sysset3 library, no username or password required
							{
								CString strDest;
								WebReg ( csvUserParams, strDest );
								SendString ( sock, arCookies, strDest, strVerb, nThreadNo );
								bCompleted = TRUE;
							}

							if ( bCompleted == FALSE )
							{
								CString strExtension = GetExtension ( strFile );
								if ( strExtension == "" )							// no extension
								{
									if ( strFile.Right(1) != "/" )					// there is no trailing slash
									{
										strFile += '/';							// need trailing slash

										CString strTemp = strFile + GetDefaultFilename ( strFile );
										if ( _access ( GetRootFilename ( strTemp ), 0 ) == 0 )	// file exists - redirect with trailing slash
										{
											HTML_Redirect ( sock, arRequest, strFile, nThreadNo );
											bCompleted = TRUE;
										}
										else										// file does not exist, no point redirecting
											strFile += GetDefaultFilename ( strFile );	// will return 404 not found
									}
									else
									{
										strFile += GetDefaultFilename ( strFile );
									}
								}
							}

							if ( bCompleted == FALSE )
							{
								if ( CheckUsernamePassword ( arRequest, strFile ) == TRUE )		// good password or password not required
								{
									CString strSourceFilename = GetRootFilename ( strFile );	// add wwwroot prefix

									CString strExtension = GetExtension ( strFile );
									if ( strExtension == "aspx" || strExtension == "ws" )
									{
										CCSV csv ( strFile, '/' );		// eg. /gps/default.aspx or /gps/gpswebservice.ws
										if ( csv.GetSize() == 3 )
										{
											CString strDll = csv.GetString(1);		// eg. gps

											ExecuteFunction = (int(*)(const char*,CString&,CCSV&,CCSV&)) LoadDll ( strDll );	// try and load dll
											if ( ExecuteFunction != NULL )		// got dll
											{
												if ( strExtension == "ws" )
													strSourceFilename = csv.GetString(2);		// eg. gpswebservice.ws

												CString strDest;
												int nReply = ExecuteFunction ( strSourceFilename, strDest, csvUserParams, csvSessionCookies );
												if ( nReply == EXECUTE_SEND_STRING )
												{
													SendString ( sock, arCookies, strDest, strVerb, nThreadNo );
												}
												else if ( nReply == EXECUTE_SEND_MIME_OK )
												{
													HTML_Mime_Ok ( sock, nThreadNo );
												}
												else
												{
													SendFile ( sock, arCookies, strSourceFilename, strVerb, nThreadNo );
												}
											}
											else		// no dll
											{
												CAsp asp;
												CString strDest;
												asp.Execute ( AspFunction, strSourceFilename, strDest, csvUserParams, csvSessionCookies );
												SendString ( sock, arCookies, strDest, strVerb, nThreadNo );
											}
										}
										else
										{
											SendFile ( sock, arCookies, strSourceFilename, strVerb, nThreadNo );
										}
									}
									else		// all other extensions
									{
										SendFile ( sock, arCookies, strSourceFilename, strVerb, nThreadNo );
									}
								}
								else
								{
									HTML_Unauthorized ( sock, nThreadNo );		// password required
								}
							}
						}
						else		// unhandled verb
						{
							HTML_Forbidden ( sock, nThreadNo );
						}
					}
				}

				arRequest.RemoveAll();
			}
		}
		else
		{
			strError = sock.GetError();
			if ( strError == "Receive timeout" )
			{
				if ( ++nInactivityTimer >= INACTIVITY_TIMEOUT )
				{
					strError = "Timeout";
					break;
				}
			}
			else		// break out if anything other than receive timeout
			{
				break;
			}
		}
	}

	if ( bTerminate == TRUE )
		strError = "Terminated";

	CString strMessage;
	strMessage.Format ( "OnConnect (%s)", strError );
	Log ( strIpAddress, "", strMessage, nThreadNo );

	DisplayWindowText ( --m_nConnectionCount );
}

/**********************************************************************/

void CMyServer::SendFile ( CSSSocket& sock, CStringArray& arCookies, const char* szFilename, CString strVerb, int nThreadNo )
{
	bool bReply = FALSE;

	CString strContentType = GetMimeContentType ( szFilename );
	if ( strContentType != "" )
	{
		CSSFile file;
		if ( file.Open ( szFilename, "rb" ) == TRUE )
		{
			int nLength = file.GetLength();
			if ( nLength > 0 )
			{
				BYTE* buf = new BYTE[nLength];
				if ( buf != NULL )
				{
					if ( file.Read ( buf, 1, nLength ) == nLength )
					{
						HTML_Ok ( sock, arCookies, strContentType, nLength, nThreadNo );

						if ( strVerb == "GET" || strVerb == "POST" )		// "HEAD" does not return data
							sock.TxData ( buf, nLength );

						bReply = TRUE;
					}

					delete [] buf;
				}
			}
			else if ( nLength == 0 )
			{
				HTML_Ok ( sock, arCookies, strContentType, nLength, nThreadNo );
				bReply = TRUE;
			}

			file.Close();
		}
	}

	if ( bReply == FALSE )
		HTML_NotFound ( sock, nThreadNo );
}

/**********************************************************************/

void CMyServer::SendString ( CSSSocket& sock, CStringArray& arCookies, CString& strSource, CString strVerb, int nThreadNo )
{
	int nLength = strSource.GetLength();
	if ( nLength >= 0 )
	{
		HTML_Ok ( sock, arCookies, "text/html", nLength, nThreadNo );

		if ( nLength > 0 )
		{
			if ( strVerb == "GET" || strVerb == "POST" )		// "HEAD" does not return data
				sock.WriteString ( strSource );
		}
	}
}

/**********************************************************************/

void CMyServer::HTML_Ok ( CSSSocket& sock, CStringArray& arCookies, const char* szContentType, int nContentLength, int nThreadNo )
{
	Log ( "", "", "200 OK", nThreadNo );

	CString strContentType;
	strContentType.Format ( "Content-Type: %s", szContentType );

	CString strLength;
	strLength.Format ( "Content-Length: %d", nContentLength );

	sock.WriteLine ( "HTTP/1.1 200 OK" );
	sock.WriteLine ( SERVER_NAME );
	sock.WriteLine ( strContentType );

	for ( int i = 0 ; i < arCookies.GetSize() ; i++ )
		sock.WriteLine ( "Set-Cookie: " + arCookies.GetAt(i) );

	arCookies.RemoveAll();

	sock.WriteLine ( strLength );
	sock.WriteLine ( "" );		// end of header
}

/**********************************************************************/

void CMyServer::HTML_Mime_Ok ( CSSSocket& sock, int nThreadNo )
{
	Log ( "", "", "200 Mime OK", nThreadNo );

	sock.WriteLine ( "HTTP/1.1 200 OK" );
	sock.WriteLine ( "Content-Type: multipart/x-mixed-replace;boundary=myboundary" );
	sock.WriteLine ( "" );		// end of header
}

/**********************************************************************/

void CMyServer::HTML_Unauthorized ( CSSSocket& sock, int nThreadNo )
{
	Log ( "", "", "401 Unauthorized", nThreadNo );

	sock.WriteLine ( "HTTP/1.1 401 Unauthorized" ); 
	sock.WriteLine ( SERVER_NAME ); 
	sock.WriteLine ( "Content-Type: text/html" );
	sock.WriteLine ( "Connection: keep-alive" );
	sock.WriteLine ( "WWW-Authenticate: Basic realm=\"MyRealm\"" );
	sock.WriteLine ( "Content-Length: 17" );
	sock.WriteLine ( "" );		// end of header

	sock.WriteString ( "401 Unauthorized\n" );	// data
}

/**********************************************************************/

void CMyServer::HTML_Forbidden ( CSSSocket& sock, int nThreadNo )
{
	Log ( "", "", "403 Forbidden", nThreadNo );

	sock.WriteLine ( "HTTP/1.1 403 Forbidden" );
	sock.WriteLine ( SERVER_NAME ); 
	sock.WriteLine ( "Content-Type: text/html" );
	sock.WriteLine ( "Connection: keep-alive" );
	sock.WriteLine ( "WWW-Authenticate: Basic realm=\"MyRealm\"" );
	sock.WriteLine ( "Content-Length: 14" );
	sock.WriteLine ( "" );		// end of header

	sock.WriteString ( "403 Forbidden\n" );		// data
}

/**********************************************************************/

void CMyServer::HTML_NotFound ( CSSSocket& sock, int nThreadNo )
{
	Log ( "", "", "404 Not Found", nThreadNo );

	sock.WriteLine ( "HTTP/1.1 404 Not Found" ); 
	sock.WriteLine ( SERVER_NAME ); 
	sock.WriteLine ( "Content-Type: text/html" );
	sock.WriteLine ( "Connection: close" );
	sock.WriteLine ( "Content-Length: 14" );
	sock.WriteLine ( "" );		// end of header

	sock.WriteString ( "404 Not Found\n" );		// data
}

/**********************************************************************/

void CMyServer::HTML_Redirect ( CSSSocket& sock, CStringArray& arRequest, const char* szURL, int nThreadNo )
{
	CString str;
	str.Format ( "301 Redirect (%s)", szURL );
	Log ( "", "", str, nThreadNo );

	sock.WriteLine ( "HTTP/1.1 301 Moved Permanently" );
	sock.WriteLine ( SERVER_NAME ); 
	str.Format ( "Location: %s", szURL );		// eg. Location: http://www.example.org/index.asp
	sock.WriteLine ( str );
	sock.WriteLine ( "" );		// end of header
}

/**********************************************************************/

void CMyServer::DisplayWindowText ( int nConnectionCount )
{
	if ( m_pWnd != NULL )
	{
		CString str;
		str.Format ( "%d Connected", nConnectionCount );
		m_pWnd->SetWindowText ( str );
	}
}

/**********************************************************************/

CString CMyServer::FindHeader ( const char* szLabel, CStringArray& arRequest )
{
	int nLen = strlen(szLabel);						// eg. szLabel = "Content-Length: "

	CString strValue;

	for ( int i = 0 ; i < arRequest.GetSize() ; i++ )		// scan request headers for label
	{
		CString strBuf = arRequest.GetAt(i);			// eg. Content-Length: 234
		if ( strBuf.Left(nLen) == szLabel )
		{
			strValue = strBuf.Mid(nLen);
			break;
		}
	}

	return strValue;
}

/**********************************************************************/

CString CMyServer::GetDefaultFilename ( const char* szPath )
{
	CString strFile;
	CString strPath;

	for ( int i = 0 ; i < m_arDefaultFilenames.GetSize() ; i++ )
	{
		strPath = szPath;
		strFile = m_arDefaultFilenames.GetAt(i);
		strPath += strFile;

		if ( _access ( GetRootFilename(strPath), 0 ) == 0 )		// file exists
			return strFile;
	}

	return m_arDefaultFilenames.GetAt(0);		// 1st in array is default
}

/**********************************************************************/

CString CMyServer::GetExtension ( const char* szFilename )
{
	CString strExtension;

	CCSV csv ( szFilename, '.' );
	if ( csv.GetSize() > 1 )
	{
		strExtension = csv.GetString(csv.GetSize()-1);
	}
	return strExtension;
}

/**********************************************************************/

CString CMyServer::GetMimeContentType ( const char* szFilename )
{
	CString strExtension = GetExtension ( szFilename );
	CString strContentType;

	for ( int i = 0 ; i < m_arMime.GetSize() ; i++ )
	{
		CCSV csv ( m_arMime.GetAt(i) );
		if ( strExtension == csv.GetString(0) )		// known file type
		{
			strContentType = csv.GetString(1);		// content type
			break;
		}
	}

	return strContentType;
}

/**********************************************************************/

CString CMyServer::GetIpAddress ( CSSSocket& sock )
{
	CString strIpAddress;

	sockaddr addr;
	int addr_len = sizeof addr;
	if ( getpeername ( sock.GetHandle(), &addr, &addr_len ) == 0 )
	{
		//	addr.sa_data[0];	// port
		//	addr.sa_data[1];	// port

		strIpAddress.Format ( "%d.%d.%d.%d", 
			(unsigned char)addr.sa_data[2], 
			(unsigned char)addr.sa_data[3], 
			(unsigned char)addr.sa_data[4], 
			(unsigned char)addr.sa_data[5] );
	}

	return strIpAddress;
}

/**********************************************************************/

CString CMyServer::GetRootFilename ( const char* szFilename )
{
	CString strFilename = ROOT_PATH;	// szFilename must have leading slash character
	strFilename += szFilename;		// eg. /register/default.aspx
	return strFilename;
}

/**********************************************************************/

CString CMyServer::HtmlEncode ( const char* szBuf )
{
	CString strBuf;

	unsigned char c;
	while (( c = *szBuf++) != 0 )
	{
		if ( c >= '0' && c <= '9' )		strBuf += c;
		else if ( c >= 'A' && c <= 'Z' )	strBuf += c;
		else if ( c >= 'a' && c <= 'z' )	strBuf += c;
		else if ( c == '-' )			strBuf += c;
		else if ( c == '_' )			strBuf += c;
		else if ( c == '.' )			strBuf += c;
		else if ( c == '~' )			strBuf += c;
		else if ( c == ' ' )			strBuf += '+';		// convert space to plus
		else
		{
			CString str;
			str.Format ( "%%%2.2X", c );
			strBuf += str;
		}
	}

	return strBuf;
}

/**********************************************************************/

static BYTE HexToNibble ( char c )		// c = '0'-'9', 'a'-'f', 'A'-'F'	returns 0x00 to 0x0f
{
	return ((( c & 0x40 ) != 0 ) ? c + 9 : c ) & 0x0f;
}

CString CMyServer::HtmlDecode ( const char* szBuf )
{
	CString strBuf;

	int nLen = strlen ( szBuf );
	for ( int i = 0 ; i < nLen ; i++ )
	{
		char c = szBuf[i];

		if ( c == '%' )	// %HH (2 character hex code)
		{
			if ( i+2 < nLen )
			{
				c = ( HexToNibble(szBuf[i+1]) << 4 ) | HexToNibble(szBuf[i+2] );
				i+= 2;
			}
			else		// not got 2 digits following percent
			{
				break;
			}
		}
		else if ( c == '+' )	// convert plus to space
			c = ' ';

		strBuf += c;
	}

	return strBuf;
}

/**********************************************************************/

void CMyServer::Log (  const char* szIpAddress, const char * szClient, const char* szServer, int nThreadNo )
{
	SYSTEMTIME t;
	GetLocalTime ( &t );

	CString strYearFolder;
	strYearFolder.Format ( "%s/%4.4d", LOG_PATH, t.wYear );					// "private/logs/yyyy"
	MakeSubdirectory ( strYearFolder );

	CString strMonthFolder;
	strMonthFolder.Format ( "%s/%4.4d%2.2d", strYearFolder, t.wYear, t.wMonth );	// "private/logs/yyyy/yyyymm"
	MakeSubdirectory ( strMonthFolder );

	CString strFilename;
	strFilename.Format ( "%s/%4.4d%2.2d%2.2d.dat", strMonthFolder, t.wYear, t.wMonth, t.wDay );	// "private/logs/yyyy/yyyymm/yyyymmdd.log"

	CString strDate;
	strDate.Format ( "%2.2d-%2.2d-%4.4d", t.wDay, t.wMonth, t.wYear );

	CString strTime;
	strTime.Format ( "%2.2d:%2.2d:%2.2d.%3.3d", t.wHour, t.wMinute, t.wSecond, t.wMilliseconds );

	CCSV csv;
	csv.Add ( strDate );
	csv.Add ( strTime );
	csv.Add ( szIpAddress );

	if ( nThreadNo >= 0 )
		csv.Add ( nThreadNo );
	else
		csv.Add ( "" );

	csv.Add ( szClient );
	csv.Add ( szServer );

	CSSFile file;
	if ( _access ( strFilename, 0 ) == -1 )		// log file does not exist 
	{
		if ( file.Open ( strFilename, "wb", _SH_DENYWR ) == TRUE )
		{
			file.WriteLine ( "Date,Time,Address,ThreadNo,Client,Server" );		// header line
			file.WriteLine ( csv.GetLine() );								// data
			file.Close();
		}
	}
	else 
	{
		if ( file.Open ( strFilename, "ab", _SH_DENYWR ) == TRUE )
		{
			file.WriteLine ( csv.GetLine() );		// data
			file.Close();
		}
	}
}

/**********************************************************************/

void CMyServer::GetSessionCookies ( CStringArray& arRequest, CCSV& csvSessionCookies )
{
	for ( int i = 0 ; i < arRequest.GetSize() ; i++ )
	{
		CString strBuf = arRequest.GetAt(i);					// eg. Cookie: name=value; name2=value2
		if ( strBuf.Left(8) == "Cookie: " )
		{
			GetTokens ( csvSessionCookies, strBuf.Mid(8), "; \r\n", "" );
			return;
		}
	}
}

/**********************************************************************/
/*
	/folder,base64,base64,base64,base64,base64
	/folder,base64,base64
*/
/**********************************************************************/

bool CMyServer::CheckUsernamePassword ( CStringArray& arRequest, CString& strFile )
{
	bool bReply = TRUE;

	for ( int i = 0 ; i < m_arPasswords.GetSize() ; i++ )	// /folder,base64,base64,base64,base64,base64 ...
	{
		CCSV csvPasswords ( m_arPasswords.GetAt(i) );
		CString strFolder = csvPasswords.GetString(0);

		if ( strFile.Left(strFolder.GetLength()) == strFolder )	// folder is protected
		{
			bReply = FALSE;		// need to check password

			for ( int i = 0 ; i < arRequest.GetSize() ; i++ )
			{
				CString strBuf = arRequest.GetAt(i);					// eg. Authorization: Basic YWRtaW46d2lsc29u
				if ( strBuf.Left(21) == "Authorization: Basic " )
				{
					CString strBase64 = strBuf.Mid(21);				// eg. YWRtaW46d2lsc29u
					for ( int n = 1 ; n < csvPasswords.GetSize() ; n++ )	// check against all stored passwords for this folder
					{
						if ( strBase64 == csvPasswords.GetString(n) )
							return TRUE;
					}
				}
			}
		}
	}

	return bReply;
}

/**********************************************************************/
/*
	private/passwords.dat

	Folder,Username,Password
	/custlist,bob,plastic
	/custlist,graeme,plastic
*/
/**********************************************************************/

void CMyServer::CreatePasswordArray ( CStringArray& ar )
{
	CSSUnsortedDatabase db;
	if ( db.OpenReadOnly ( PASSWORD_FILENAME, "", 0 ) == DB_ERR_NONE )		// Folder,Username,Password
	{
		for ( int nRow = 0 ; nRow < db.GetRecordCount() ; nRow++ )
		{
			db.MoveTo ( nRow );
			AddPassword ( ar, db.GetString("Folder"), db.GetString("Username"), db.GetString("Password") );
		}

		db.Close();
	}
}

/**********************************************************************/

void CMyServer::AddPassword ( CStringArray& ar, const char* szFolder, const char* szUsername, const char* szPassword )
{
	CString str;
	str.Format ( "%s:%s", szUsername, szPassword );
	bool bAdmin = ( str == ":" ) ? TRUE : FALSE;		// no username or password (admin)
	CString strBase64 = AsciiToBase64 ( str );

	for ( int i = 0 ; i < ar.GetSize() ; i++ )
	{
		CCSV csv ( ar.GetAt(i) );			// /folder,base64,base64,base64,base64,base64 ...
		CString strFolder = csv.GetString(0);
		if ( strFolder == szFolder )			// folder already exists
		{
			if ( bAdmin == FALSE )			// admin will already have been added
			{
				csv.Add ( strBase64 );
				ar.SetAt ( i, csv.GetLine() );
			}
			return;
		}
	}

	// folder not found, add new folder with admin as first option

	CCSV csv;
	csv.Add ( szFolder );
	csv.Add ( AsciiToBase64 ( ADMIN_PASSWORD ) );	// "admin:F1rew0rk" admin is allowed to access any protected folder

	if ( bAdmin == FALSE )
		csv.Add ( strBase64 );

	ar.Add ( csv.GetLine() );
}

/**********************************************************************/
