/**********************************************************************/
#include "SPOSCommsConnect.h"
/**********************************************************************/
#include "NetClient.h"
/**********************************************************************/

CNetCommsClient::CNetCommsClient ( const char* szNetworkPath )
{
	m_strNetworkPath = szNetworkPath;
}

/**********************************************************************/

bool CNetCommsClient::TxCommand ( const char* szCommand, CStringArray& strResponseArray )
{
	bool bReply = FALSE;
	DelFile ( szAckFilename );			// delete ack file on server

	CSSFile file;
	if ( file.Open( szEnqFilename, "wb" ) == FALSE )
		m_strError = "Enq error 1";		// cannot create local enq file
	else
	{
		m_nTxEnqNo = __int64(::GetTickCount());	// get unique enquiry number

		CCSV csv;
		csv.Add ( m_nTxEnqNo );			// enqno
		csv.Add ( szCommand );			// command
		file.WriteLine ( csv.GetLine() );
		file.Close();

		if ( PutFile ( "", szEnqFilename, "", szEnqFileTmp, FALSE, 10 ) == FALSE )	// copy enq file to server
			m_strError = "Enq error 2";	// cannot copy enq file to server
		else if ( RenameFile( "", szEnqFileTmp, szEnqFilename ) == FALSE )
		{
			DelFile( szEnqFileTmp );
			m_strError = "Enq error 3";	// cannot rename enq file on server
		}
		else
		{
			bReply = WaitForAck ( strResponseArray );
			DelFile ( szEnqFilename );	// delete enq file on server (this is only reqd for compatiblilty
										// with prev versions using com5000.lib - can be removed in future
										// as the server now deletes this file)
		}
	}

	remove ( szEnqFilename );	// local enq file
	remove ( szAckFilename );	// local ack file

	return bReply;
}

/**********************************************************************/

bool CNetCommsClient::WaitForAck ( CStringArray& strResponseArray )
{
	int i = 0;

	do
	{
		if ( GetFile ( szAckFilename, "", "", FALSE, 10 ) == TRUE )	// copy ack file from server
		{
			DelFile ( szAckFilename );	// delete ack file on server

			CSSFile file;
			if ( file.Open ( szAckFilename, "rb" ) == TRUE )
			{
				CString strBuf;
				file.ReadString ( strBuf );		// read 1st line of ack file
				CCSV csv ( strBuf );

				if ( csv.GetInt64(0) == m_nTxEnqNo )		// matching enquiry numbers
				{
					m_strError = csv.GetString(1);	// server error

					while ( file.ReadString ( strBuf ) == TRUE )
						strResponseArray.Add ( strBuf );

					file.Close();

					if ( m_strError == "" )	// no error from server
					{
						m_strError = "Ok";
						return TRUE;
					}

					return FALSE;			// server error
				}

				file.Close();
			}
			else
			{
				m_strError = "Ack error 1";	// cannot open local ack file
				return FALSE;
			}
		}
		else
		{
			m_strError = "No response";	// no response from server
			return FALSE;
		}

		if ( i < 1 )
			WaitLoop ( 3000 );	// give server time to find enq
	}
	while ( i++ < 1 );			// allow 1 retry if enqno incorrect (may have picked up prev ack)

	m_strError = "Ack error 2";	// enquiry numbers do not match

	return FALSE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

static bool CopyFileWait ( const char* szSource, const char* szDest, BOOL bFailIfExists, int nTimeout )
{
	DWORD dwStartTime = ::GetTickCount();
	DWORD dwMSecs = nTimeout * 1000;

	while ( CopyFile ( szSource, szDest, bFailIfExists ) == 0 )
	{
		if (( ::GetTickCount() - dwStartTime ) >= dwMSecs )
			return FALSE;

		::WaitLoop ( 1000 );
	}

	return TRUE;
}

/**********************************************************************/

bool CNetCommsClient::GetFile ( const char* szFilename, const char* szLocalPath, const char* szRemotePath, bool bFailIfExists, int nTimeout )
{
	CString strLocalPath = szLocalPath;
	CString strRemotePath = m_strNetworkPath+szRemotePath;

	bool bReply = CopyFileWait ( strRemotePath+szFilename, strLocalPath+szFilename, bFailIfExists, nTimeout );

	if ( bReply == TRUE )
		m_strError = "Ok";
	else
		m_strError.Format ( "Error: %s", GetLastRasCommsErrorMessage() );

	return bReply;
}

/**********************************************************************/

bool CNetCommsClient::PutFile ( const char* szLocalPath, const char* szLocalFilename, const char* szRemotePath, const char* szRemoteFilename, bool bFailIfExists, int nTimeout )
{
	CString strLocalPath = szLocalPath;
	CString strRemotePath = m_strNetworkPath+szRemotePath;

	bool bReply = CopyFileWait ( strLocalPath+szLocalFilename, strRemotePath+szRemoteFilename, bFailIfExists, nTimeout );

	if ( bReply == TRUE )
		m_strError = "Ok";
	else
		m_strError.Format ( "Error: %s", GetLastRasCommsErrorMessage() );

	return bReply;
}

/**********************************************************************/

bool CNetCommsClient::RenameFile ( const char* szRemotePath, const char* szOldFilename, const char* szNewFilename )
{
	CString strRemotePath = m_strNetworkPath+szRemotePath;
	return ( rename( strRemotePath + szOldFilename, strRemotePath + szNewFilename ) == 0 );
}

/**********************************************************************/

int CNetCommsClient::DelFile ( const char* szFilename )
{
	return ( remove ( m_strNetworkPath+szFilename ) );
}

/**********************************************************************/
