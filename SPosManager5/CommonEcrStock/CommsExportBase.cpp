/**********************************************************************/
#include "SPOSVersions.h"
#include "JobListCSVArray.h"
#include "NetworkCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "CommsExportBase.h"
/**********************************************************************/

CDownloadExclusionItem::CDownloadExclusionItem()
{
	Reset();
}

/**********************************************************************/

void CDownloadExclusionItem::Reset()
{
	m_nDownloadType = 0;
	m_nSPOSVersion = SPOS_V4;
	m_nCount = 0;
}

/**********************************************************************/

void CDownloadExclusionItem::Add (CDownloadExclusionItem& source )
{
	m_nCount += source.m_nCount;
}

/**********************************************************************/

int CDownloadExclusionItem::Compare( CDownloadExclusionItem& source, int nHint )
{
	if ( m_nDownloadType != source.m_nDownloadType )
		return ( ( m_nDownloadType > source.m_nDownloadType ) ? 1 : -1 );

	if ( m_nSPOSVersion != source.m_nSPOSVersion )
		return ( ( m_nSPOSVersion > source.m_nSPOSVersion ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CCommsExportBase::CCommsExportBase( int nNetworkIdx, int nLocIdx, int nTerminalNo, bool bImmediate, bool bFromMemory )
{
	Init( nNetworkIdx, nLocIdx, nTerminalNo, bImmediate, bFromMemory );
}

/**********************************************************************/

CCommsExportBase::CCommsExportBase( CJobListCSVRecord& JobInfo, bool bFromMemory )
{
	Init( JobInfo.GetNetworkIdx(), JobInfo.GetLocIdx(), JobInfo.GetTerminalNo(), JobInfo.GetIsImmediateFlag(), bFromMemory );
}

/**********************************************************************/

void CCommsExportBase::Init( int nNetworkIdx, int nLocIdx, int nTerminalNo, bool bImmediate, bool bFromMemory )
{
	m_nNetworkIdx = nNetworkIdx;
	m_nLocIdx = nLocIdx;
	m_nTerminalNo = nTerminalNo;
	m_bImmediate = bImmediate;
	m_bFromMemory = bFromMemory;
	m_strSourceFolder = "";
	m_strSourceFilename = "";
	m_strDestFolder = "";
}

/**********************************************************************/

bool CCommsExportBase::CheckPluNo(int nSPOSVersion, __int64 nSSPluNo, __int64& nSharpPluNo, bool& bModifiable)
{
	if (SysInfo.IsModifiableSSPluNo(nSSPluNo) == TRUE)
	{
		nSharpPluNo = nSSPluNo * 10;
		bModifiable = TRUE;
	}
	else
	{
		nSharpPluNo = nSSPluNo;
		bModifiable = FALSE;
	}

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4) == TRUE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**********************************************************************/

void CCommsExportBase::TallyDownloadExclusion( int nDownloadType, int nSPOSVersion, int nCount )
{
	CDownloadExclusionItem item;
	item.m_nDownloadType = nDownloadType;
	item.m_nSPOSVersion = nSPOSVersion;
	item.m_nCount = nCount;
	m_arrayDownloadExclusions.Consolidate( item );
}

/**********************************************************************/

void CCommsExportBase::TallyDownloadAccept( int nCount )
{
	CDownloadExclusionItem item;
	item.m_nDownloadType = 9999;
	item.m_nSPOSVersion = 9999;
	item.m_nCount = nCount;
	m_arrayDownloadExclusions.Consolidate( item );
}

/**********************************************************************/

void CCommsExportBase::GetDownloadExclusion( int n, CDownloadExclusionItem& item )
{
	if ( ( n >= 0 ) && ( n < m_arrayDownloadExclusions.GetSize() ) )
		m_arrayDownloadExclusions.GetAt( n, item );
}

/**********************************************************************/
