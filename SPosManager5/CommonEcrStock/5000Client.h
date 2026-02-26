#pragma once
/**********************************************************************/
#include "CommsExportBase.h"
#include "JobListCSVArray.h"
/**********************************************************************/
#include "NetClient.h"
/**********************************************************************/

struct C5000ClientPutFileInfo
{
public:
	C5000ClientPutFileInfo();
	C5000ClientPutFileInfo( CCommsExportBase& commsExport, CJobListCSVArray& jobArray, CJobListCSVRecord& jobRecord );
	void GetDownloadTexts( CString& strSource, CString& strTarget );

public:
	CString m_strSourceFolder;
	CString m_strSourceFilename;
	CString m_strDestFolder;
	CString m_strDestFilename;
	CString m_strDestSubFolder;
	bool m_bCreateSubFolders;
};

/**********************************************************************/

class C5000Client : public CNetCommsClient
{
public:
	C5000Client ( int nNetworkIdx, const char* szNetworkPath, CWnd* pStatusWnd );
	bool Receive ( const char* szRemotePath, const char* szLocalPath );
	bool ReceiveReasonFiles ( const char* szRemotePath, int nNetworkIdx, int nParam1 );
	bool ReceiveEODPluFile ( const char* szRemotePath, int nLocIdx );
	bool ReceiveEODCustomerFile ( const char* szRemotePath, int nLocIdx );
	bool PutFile ( C5000ClientPutFileInfo& infoPutFile );
	bool DeleteFile ( const char* szFilename, const char* szRemotePath );
	const char* GetError() { return m_strError; }

private:
	void DisplayStatus ( const char* szText );
	void LogMessage ( const char* szText, const char* szError, bool bHide=FALSE );
	void LogMessage ( const char* szType, int nCount, const char* szError, bool bHide=FALSE );

private:
	CWnd* m_pStatusWnd;
	int m_nNetworkIdx;
	CString m_strError;
};

/**********************************************************************/
