#pragma once
/**********************************************************************/
static const char* sz5000TermFilename		= "term";
static const char* sz5000DepoFilename		= "termdpst";
static const char* sz5000EODFilename		= "termeod";
static const char* sz5000SendFilename		= "send";
static const char* sz5000CmdReceive			= "Receive";
static const char* sz5000CmdReceiveReasons	= "ReceiveReasons";
static const char* sz5000CmdReceiveEODPlu	= "ReceiveEODPlu";
static const char* sz5000CmdReceiveEODCustomer	= "ReceiveEODCustomer";
/**********************************************************************/

struct C5000SiteInfo
{
public:
	C5000SiteInfo();

public:
	CString m_strEcrDataPath;
	CString m_strLocalDestPath;
	CString m_strNetworkLockFilename;
	CString m_strExportControlLocs;
	unsigned int m_nTransactionCount;
	unsigned int m_nZeroFileCount;
};

/**********************************************************************/

class C5000SiteArray
{
public:
	C5000SiteArray();
	
	int AddSite ( C5000SiteInfo& info );
	int GetSize() { return m_arraySiteInfo.GetSize(); }
	void RemoveAll();
	void RemoveFirst();
	
	void SelectFirstSite();
	void SelectNextSite();

	void GetSiteInfo( C5000SiteInfo& info );

	void IncTransactionCount( int nAmount = 1 );
	void SyncZeroFileCount( unsigned int nCount );
	unsigned int GetTransactionCount ( int nIndex );

private:
	void GetSiteInfo( int nSiteIndex, C5000SiteInfo& info );
	

private:
	int m_nSiteIndex;
	CArray<C5000SiteInfo,C5000SiteInfo> m_arraySiteInfo;
};

/**********************************************************************/

