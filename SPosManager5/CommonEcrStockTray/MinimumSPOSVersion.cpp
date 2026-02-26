/**********************************************************************/
#include "NetworkCSVArray.h"
#include "SPOSVersions.h"
/**********************************************************************/
#include "MinimumSPOSVersion.h"
/**********************************************************************/

CMinimumSPOSVersion::CMinimumSPOSVersion()
{
	m_strReason = "";
	m_nMinVer = -1;
	m_bInvalidData = FALSE;
}

/**********************************************************************/

void CMinimumSPOSVersion::CheckSPOSVersion( int nSPOSVersion, int& nErrorType )
{
	nErrorType = -1;
	if ( m_nMinVer != -1 )
	{
		if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, m_nMinVer ) == FALSE )
			nErrorType = m_nMinVer;
	}
}

/**********************************************************************/

const char* CMinimumSPOSVersion::GetMinVerName()
{
	m_strMinVerName = "";
	CNetworkCSVRecord::GetTreeNameExternal( CONNECTION_TYPE_STANDARD_NONE, m_nMinVer, m_strMinVerName, FALSE );
	return m_strMinVerName;
}

/**********************************************************************/

void CMinimumSPOSVersion::Merge(int nMinVer, CString strReason)
{
	if (nMinVer > m_nMinVer)
	{
		m_nMinVer = nMinVer;
		m_strReason = strReason;
	}
}

/**********************************************************************/
