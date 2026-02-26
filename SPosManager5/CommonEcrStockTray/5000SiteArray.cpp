/**********************************************************************/
#include "5000SiteArray.h"
/**********************************************************************/

C5000SiteInfo::C5000SiteInfo()
{
	m_strEcrDataPath = "";
	m_strLocalDestPath = "";
	m_strNetworkLockFilename = "";
	m_strExportControlLocs = "";
	m_nTransactionCount = 0;
	m_nZeroFileCount = 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

C5000SiteArray::C5000SiteArray()
{
	m_nSiteIndex = 0;
}

/**********************************************************************/

int C5000SiteArray::AddSite ( C5000SiteInfo& info )
{
	info.m_nTransactionCount = 0;
	info.m_nZeroFileCount = 0;
	return m_arraySiteInfo.Add ( info );
}

/**********************************************************************/

void C5000SiteArray::SelectFirstSite()
{
	m_nSiteIndex = 0;
}

/**********************************************************************/

void C5000SiteArray::SelectNextSite()
{
	if ( ++m_nSiteIndex >= m_arraySiteInfo.GetSize() )
		m_nSiteIndex = 0;
}

/**********************************************************************/

void C5000SiteArray::GetSiteInfo( C5000SiteInfo& info )
{
	GetSiteInfo( m_nSiteIndex, info );
}

/**********************************************************************/

void C5000SiteArray::GetSiteInfo( int nSiteIndex, C5000SiteInfo& info )
{
	if ( ( nSiteIndex >= 0 ) && ( nSiteIndex < m_arraySiteInfo.GetSize() )  )
	{
		info.m_strEcrDataPath = m_arraySiteInfo[ nSiteIndex ].m_strEcrDataPath;
		info.m_strLocalDestPath = m_arraySiteInfo[ nSiteIndex ].m_strLocalDestPath;
		info.m_strNetworkLockFilename = m_arraySiteInfo[ nSiteIndex ].m_strNetworkLockFilename;
		info.m_nTransactionCount = m_arraySiteInfo[ nSiteIndex ].m_nTransactionCount;
		info.m_nZeroFileCount = m_arraySiteInfo[ nSiteIndex ].m_nZeroFileCount;
		info.m_strExportControlLocs = m_arraySiteInfo[ nSiteIndex ].m_strExportControlLocs;
	}
	else
	{
		info.m_strEcrDataPath = "";
		info.m_strLocalDestPath = "";
		info.m_strNetworkLockFilename = "";
		info.m_nTransactionCount = 0;
		info.m_nZeroFileCount = 0;
		info.m_strExportControlLocs = "";
	}
}

/**********************************************************************/

void C5000SiteArray::IncTransactionCount( int nAmount )
{
	if ( ( m_nSiteIndex >= 0 ) && ( m_nSiteIndex < m_arraySiteInfo.GetSize() )  )
		m_arraySiteInfo[ m_nSiteIndex ].m_nTransactionCount += nAmount;
}

/**********************************************************************/

void C5000SiteArray::SyncZeroFileCount( unsigned int nCount )
{
	if ( ( m_nSiteIndex >= 0 ) && ( m_nSiteIndex < m_arraySiteInfo.GetSize() )  )
		if ( nCount > m_arraySiteInfo[ m_nSiteIndex ].m_nZeroFileCount )
			m_arraySiteInfo[ m_nSiteIndex ].m_nZeroFileCount = nCount;
}

/**********************************************************************/

unsigned int C5000SiteArray::GetTransactionCount ( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_arraySiteInfo.GetSize() )  )
		return m_arraySiteInfo[nIndex].m_nTransactionCount;
	else
		return 0;
}

/**********************************************************************/

void C5000SiteArray::RemoveAll()
{
	m_arraySiteInfo.RemoveAll();
}

/**********************************************************************/

void C5000SiteArray::RemoveFirst()
{
	if ( m_arraySiteInfo.GetSize() > 0 )
		m_arraySiteInfo.RemoveAt(0);
}

/**********************************************************************/