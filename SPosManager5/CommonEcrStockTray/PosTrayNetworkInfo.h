#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/

struct CPosTrayNetworkInfo
{
public:
	CPosTrayNetworkInfo()
	{
		m_nNetworkIdx = 0;
		m_strNetworkName = "";
		m_strSPOSVersion = "";
		m_strNetworkPath = "";
	}

public:
	int m_nNetworkIdx;
	CString m_strNetworkName;
	CString m_strSPOSVersion;
	CString m_strNetworkPath;
};

/**********************************************************************/
#endif
/**********************************************************************/
