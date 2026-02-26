#pragma once
/**********************************************************************/

class CNetCommsClient
{
public:
	CNetCommsClient ( const char* szNetworkPath );
	bool TxCommand ( const char* szCommand, CStringArray& strResponseArray );
	bool GetFile ( const char* szFilename, const char* szLocalPath, const char* szRemotePath, bool bFailIfExists=FALSE, int nTimeout=0 );	// copy file from server
	bool PutFile ( const char* szLocalPath, const char* szLocalFilename, const char* szRemotePath, const char* szRemoteFilename, bool bFailIfExists=FALSE, int nTimeout=0 );	// copy file to server
	int DelFile ( const char* szFilename );																				// delete file on server
	bool RenameFile ( const char* szRemotePath, const char* szOldFilename, const char* szNewFilename ); //rename file
	const char* GetError() { return m_strError; }

private:
	bool WaitForAck ( CStringArray& strResponseArray );

private:
	CString m_strNetworkPath;
	CString m_strError;
	__int64 m_nTxEnqNo;
};

/**********************************************************************/

