#pragma once
/**********************************************************************/

struct CNetworkSPOSVersionInfo
{
public:
	CNetworkSPOSVersionInfo();
	void Reset();

public:
	int m_nSPOSVersion;
	CString m_strVersion;
	CString m_strFeatures;
	CString m_strCashRSP;
};

/**********************************************************************/

class CNetworkSPOSVersionArray
{
public:
	CNetworkSPOSVersionArray();

public:
	int GetSize(){ return m_arrayVersions.GetSize(); }
	void GetAt( int n, CNetworkSPOSVersionInfo& info );
	const char* GetSPOSVersionText( int nSPOSVersion );

private:
	void AddSPOSVersion( int nVersion, const char* szVersion, const char* szCashRSP, const char* szFeatures );

private:
	CArray<CNetworkSPOSVersionInfo,CNetworkSPOSVersionInfo> m_arrayVersions;
	CString m_strVersionText;
};

/**********************************************************************/