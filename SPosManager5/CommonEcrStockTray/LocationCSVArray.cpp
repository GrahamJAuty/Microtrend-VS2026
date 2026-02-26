/**********************************************************************/
#include "..\CommonEcrStock\JobNumbers.h"
/**********************************************************************/
#include "Consolidation.h"
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "LocationMasterMap.h"
#include "MaxLengths.h"
#include "NetworkCSVArray.h"
#include "NetworkTerminalMap.h"
#include "NodeTypes.h"
#include "ReportConsolidationArray.h"
#include "ReportPointCSVArray.h"
#include "SortedLocation.h"
#include "SPOSVersions.h"
#include "SysInfo.h"
/**********************************************************************/
#include "LocationCSVArray.h"
/**********************************************************************/

CTerminalLinks::CTerminalLinks()
{
	m_nTNo = 0;
	m_nSpNo = 0;
	m_nRpNoLegacy = 0;
	m_nRpNoReport = -1;
	m_nRpFlags = 0;
}

/**********************************************************************/

CLocationCSVRecord::CLocationCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CLocationCSVRecord::ClearRecord()
{
	m_nObsoleteSiteNo = 0;
	m_bObsoleteUploadPath = FALSE;
	m_strObsoleteUploadPath = "";
	m_bObsoleteRecord = FALSE;

	m_nLocNo = 0;
	m_nNetworkNo = 0;
	m_nDbNo = 0;
	m_strName = "";
	m_strIDraughtID = "";
	m_nDefaultPriceSet = 1;
	m_nPluPriceSetCount = 1;
	m_nDbPriceSet1 = 0;
	m_nDbPriceSet2 = 0;
	m_nDbPriceSet3 = 0;
	m_nPriceSetKeyboard1 = 0;
	m_nPriceSetKeyboard2 = 0;
	m_nPriceSetKeyboard3 = 0;
	m_nDbKeyboardSet = 0;
	m_strTerminalExportPath = "";
	m_strDownloadPath = "";
	m_strDownloadNowPath = "";
	m_bExportControl = FALSE;
	m_strExportControlPath = "";
	m_nExportControlDelay = 1;
	m_nProdCSVImportFolderType = EODIMPORT_FOLDERTYPE_ROOT;
	m_nProdCSVImportExistingMode = 0;
	m_nProdCSVImportTypeMask = 0x7FFFFFFF;
	m_nProdCSVProtectFieldMask = 0;
	m_nProdCSVImportModifierMode = 0;
	m_nCustomerImportFolderType = EODIMPORT_FOLDERTYPE_ROOT;
	m_nLoyaltyFolderSet = 0;
	
	m_nPluDownloadRangeType = PLUDOWNLOADRANGE_NONE;
	for ( int n = 0; n < PLUDOWNLOADRANGE_COUNT; n++ )
	{
		m_nPluDownloadRangeStart[n] = 0;
		m_nPluDownloadRangeEnd[n] = 0;
	}

	m_nBritanniaDefaultPriceBand = 1;
	m_strRemoteID = "";
	m_strAgressoLocationGroup = "";
	
	m_arrayTerminals.RemoveAll();
}

/**********************************************************************/

void CLocationCSVRecord::SetName( const char* szName )
{
	CString strName = szName;
	::TrimSpaces ( strName, FALSE );
	if ( strName == "" ) strName = GetSystemName();
	m_strName = strName;
}

/**********************************************************************/

void CLocationCSVRecord::SetIDraughtID( const char* szID )
{
	CString strID = szID;
	::TrimSpaces ( strID, FALSE );
	
	if ( strID.GetLength() <= MAX_LENGTH_IDRAUGHT_ID )
		m_strIDraughtID = strID;
}

/**********************************************************************/

void CLocationCSVRecord::SetRemoteID( const char* szID )
{
	CString strID = szID;
	::TrimSpaces ( strID, FALSE );
	
	if ( strID.GetLength() <= MAX_LENGTH_REMOTE_ID )
		m_strRemoteID = strID;
}

/**********************************************************************/

void CLocationCSVRecord::SetAgressoLocationGroup( const char* sz )
{
	CString str = sz;
	::TrimSpaces ( str, FALSE );
	
	if ( str.GetLength() <= MAX_LENGTH_AGRESSO_LOCGROUP )
		m_strAgressoLocationGroup = str;
}

/**********************************************************************/

void CLocationCSVRecord::SetPluDownloadRangeStart( int x, __int64 n )
{
	if ( ( x >= 0 ) && ( x< PLUDOWNLOADRANGE_COUNT ) )
		if ( ( n >= 0 ) && ( n <= Plu::PluNo.Max ) )
			m_nPluDownloadRangeStart[x] = n;
}

/**********************************************************************/

void CLocationCSVRecord::SetPluDownloadRangeEnd( int x, __int64 n )
{
	if ( ( x >= 0 ) && ( x< PLUDOWNLOADRANGE_COUNT ) )
		if ( ( n >= 0 ) && ( n <= Plu::PluNo.Max ) )
			m_nPluDownloadRangeEnd[x] = n;
}

/**********************************************************************/

__int64 CLocationCSVRecord::GetPluDownloadRangeStart( int x )
{
	if ( ( x >= 0 ) && ( x< PLUDOWNLOADRANGE_COUNT ) )
		return m_nPluDownloadRangeStart[x];
	else
		return 0;
}

/**********************************************************************/

__int64 CLocationCSVRecord::GetPluDownloadRangeEnd( int x )
{
	if ( ( x >= 0 ) && ( x< PLUDOWNLOADRANGE_COUNT ) )
		return m_nPluDownloadRangeEnd[x];
	else
		return 0;
}

/**********************************************************************/

const char* CLocationCSVRecord::GetSystemName()
{
	m_strSystemName.Format ( "Location %d.%d", m_nNetworkNo, m_nLocNo );
	return m_strSystemName;
}

/**********************************************************************/

void CLocationCSVRecord::SetProdCSVImportModifierMode( int n )
{
	switch(n)
	{
	case 0:
	case 1:
	case 2:
		m_nProdCSVImportModifierMode = n;
		break;
	}
}

/**********************************************************************/

bool CLocationCSVRecord::FindTerminalByNumber( int nTNo, int& nTerminalIdx )
{
	int nStart = 0;
	int nEnd = GetTerminalCount() - 1;

	while ( nStart <= nEnd )
	{
		nTerminalIdx = ( nStart + nEnd ) / 2;

		int nArrayTNo = GetTNo ( nTerminalIdx );
		
		if ( nArrayTNo < nTNo )
			nStart = nTerminalIdx + 1;

		if ( nArrayTNo > nTNo )
			nEnd = nTerminalIdx - 1;

		if ( nArrayTNo == nTNo )
			return TRUE;
	}
		
	nTerminalIdx = nStart;
	return FALSE;
}

/**********************************************************************/

bool CLocationCSVRecord::AddTerminal( int nTNo, int nRpNoLegacy, __int64 nRpFlags, int nSpNo, int& nTerminalIdx )
{
	nTerminalIdx = -1;

	if ( ( nTNo < 1 ) || ( nTNo > 999 ) )
		return FALSE;

	if ( FindTerminalByNumber( nTNo, nTerminalIdx ) == TRUE )
		return FALSE;

	CTerminalLinks Links;
	Links.m_nTNo = nTNo;
	Links.m_nRpNoLegacy = nRpNoLegacy;
	Links.m_nRpFlags = nRpFlags;
	Links.m_nSpNo = nSpNo;
	m_arrayTerminals.InsertAt( nTerminalIdx, Links );
	return TRUE;
}

/**********************************************************************/

void CLocationCSVRecord::RemoveTerminal( int nTNo )
{
	int nTerminalIdx;
	if ( FindTerminalByNumber( nTNo, nTerminalIdx ) == TRUE )
		m_arrayTerminals.RemoveAt( nTerminalIdx );
}

/**********************************************************************/

void CLocationCSVRecord::FixWebSalesTerminals()
{
	for ( int nTermIdx = m_arrayTerminals.GetSize() - 1; nTermIdx >= 0; nTermIdx-- )
	{
		CTerminalLinks Links = m_arrayTerminals.GetAt( nTermIdx );

		if ( Links.m_nTNo != 1 )
			m_arrayTerminals.RemoveAt( nTermIdx );
		else
		{
			Links.m_nSpNo = 0;
			m_arrayTerminals.SetAt( nTermIdx, Links );
		}
	}
}

/**********************************************************************/

int CLocationCSVRecord::GetTNo( int nTerminalIdx )
{
	if ( nTerminalIdx >= 0 && nTerminalIdx < m_arrayTerminals.GetSize() )
		return m_arrayTerminals[ nTerminalIdx ].m_nTNo;
	else
		return 0;
}

/**********************************************************************/

int CLocationCSVRecord::GetRpNoLegacy( int nTerminalIdx )
{
	if ( nTerminalIdx >= 0 && nTerminalIdx < m_arrayTerminals.GetSize() )
		return m_arrayTerminals[ nTerminalIdx ].m_nRpNoLegacy;
	else
		return 0;
}

/**********************************************************************/

int CLocationCSVRecord::GetRpNoReport( int nTerminalIdx )
{
	if ( nTerminalIdx >= 0 && nTerminalIdx < m_arrayTerminals.GetSize() )
		return m_arrayTerminals[ nTerminalIdx ].m_nRpNoReport;
	else
		return -1;
}

/**********************************************************************/

__int64 CLocationCSVRecord::GetRpFlags( int nTerminalIdx )
{
	if ( nTerminalIdx >= 0 && nTerminalIdx < m_arrayTerminals.GetSize() )
		return m_arrayTerminals[ nTerminalIdx ].m_nRpFlags;
	else
		return 0;
}

/**********************************************************************/

int CLocationCSVRecord::GetSpNo( int nTerminalIdx )
{
	if ( nTerminalIdx >= 0 && nTerminalIdx < m_arrayTerminals.GetSize() )
		return m_arrayTerminals[ nTerminalIdx ].m_nSpNo;
	else
		return 0;
}

/**********************************************************************/

void CLocationCSVRecord::SetRpNoLegacy( int nTerminalIdx, int nRpNo )
{
	if ( nTerminalIdx >= 0 && nTerminalIdx < m_arrayTerminals.GetSize() )
		m_arrayTerminals[ nTerminalIdx ].m_nRpNoLegacy = nRpNo;
}

/**********************************************************************/

void CLocationCSVRecord::SetRpNoReport( int nTerminalIdx, int nRpNo )
{
	if ( nTerminalIdx >= 0 && nTerminalIdx < m_arrayTerminals.GetSize() )
		m_arrayTerminals[ nTerminalIdx ].m_nRpNoReport = nRpNo;
}

/**********************************************************************/

void CLocationCSVRecord::SetRpFlags( int nTerminalIdx, __int64 nRpFlags )
{
	if ( nTerminalIdx >= 0 && nTerminalIdx < m_arrayTerminals.GetSize() )
		m_arrayTerminals[ nTerminalIdx ].m_nRpFlags = nRpFlags;
}

/**********************************************************************/

bool CLocationCSVRecord::GetRpFlag( int nTerminalIdx, int nRpNo )
{
	bool bResult = FALSE;
	if ( nTerminalIdx >= 0 && nTerminalIdx < m_arrayTerminals.GetSize() )
	{
		if ( ( nRpNo >= 1 ) && ( nRpNo <= MAX_SHARED_RPNO ) )
		{
			__int64 nMask = 1;
			nMask <<= ( nRpNo - 1 );
			bResult = ( ( GetRpFlags( nTerminalIdx ) & nMask ) != 0 );
		}
	}
	return bResult;
}

/**********************************************************************/

void CLocationCSVRecord::SetRpFlag( int nTerminalIdx, int nRpNo )
{
	if ( nTerminalIdx >= 0 && nTerminalIdx < m_arrayTerminals.GetSize() )
	{
		if ( ( nRpNo >= 1 ) && ( nRpNo <= MAX_SHARED_RPNO ) )
		{
			__int64 nMask = 1;
			nMask <<= ( nRpNo - 1 );
			__int64 nCurrent = GetRpFlags( nTerminalIdx );

			if ( ( nCurrent & nMask ) == 0 )
				SetRpFlags( nTerminalIdx, nCurrent + nMask );
		}
	}
}

/**********************************************************************/

void CLocationCSVRecord::ClearRpFlag( int nTerminalIdx, int nRpNo )
{
	if ( nTerminalIdx >= 0 && nTerminalIdx < m_arrayTerminals.GetSize() )
	{
		if ( ( nRpNo >= 1 ) && ( nRpNo <= MAX_SHARED_RPNO ) )
		{
			__int64 nMask = 1;
			nMask <<= ( nRpNo - 1 );
			__int64 nCurrent = GetRpFlags( nTerminalIdx );
			
			if ( ( nCurrent & nMask ) != 0 )
				SetRpFlags( nTerminalIdx, nCurrent - nMask );
		}
	}
}

/**********************************************************************/

void CLocationCSVRecord::SetSpNo( int nTerminalIdx, int nSpNo )
{
	if ( nTerminalIdx >= 0 && nTerminalIdx < m_arrayTerminals.GetSize() )
		m_arrayTerminals[ nTerminalIdx ].m_nSpNo = nSpNo;
}


/**********************************************************************/

void CLocationCSVRecord::TidyPluDownloadRanges()
{
	for ( int n = 0; n < PLUDOWNLOADRANGE_COUNT; n++ )
	{
		__int64 nStart = GetPluDownloadRangeStart(n);
		__int64 nEnd = GetPluDownloadRangeEnd(n);

		if ( nEnd < nStart )
		{
			__int64 nTemp = nStart;
			nStart = nEnd;
			nEnd = nTemp;
		}

		if ( ( nStart <= 0 ) || ( nStart > Plu::PluNo.Max ) || ( nEnd <= 0 ) || ( nEnd > Plu::PluNo.Max ) )
		{
			nStart = 0;
			nEnd = 0;
		}

		SetPluDownloadRangeStart( n, nStart );
		SetPluDownloadRangeEnd( n, nEnd );
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CLocationCSVArray::CLocationCSVArray()
{
	m_bGotPMSPseudoLocation = FALSE;
}

/**********************************************************************/

CLocationCSVArray::~CLocationCSVArray()
{
	RemoveAll();
	m_nIDraughtStatus = IDRAUGHT_STATUS_UNKNOWN;
}

/**********************************************************************/

void CLocationCSVArray::RemoveAll()
{
	while ( GetSize() > 0 )
	{
		int nLocIdx = GetSize() - 1;
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		delete pLocation;
		RemoveAt ( nLocIdx );
	}
}

/**********************************************************************/

int CLocationCSVArray::AddLocationByUser ( int nNetworkNo, int nConnectionType )
{
	if ( CONNECTION_TYPE_STANDARD_NONE == nConnectionType )
		if ( SysInfo.GetMaxLocations() <= GetSharpPosLocationCount() )
			return -1;

	int nStartIdx, nEndIdx;
	FindLocationByNumber ( nNetworkNo, 0, nStartIdx );
	FindLocationByNumber ( nNetworkNo + 1, 0, nEndIdx );
	nEndIdx--;
	
	if ( nEndIdx >= nStartIdx + 998 )
		return -1;

	int nLocIdx = nStartIdx, nLocNo = 1;
	for ( ; nLocIdx <= nEndIdx; nLocIdx++, nLocNo++ )
		if ( GetLocNo ( nLocIdx ) != nLocNo )
			break;
		
	CLocationCSVRecord* pLocation = new CLocationCSVRecord;
	InsertAt ( nLocIdx, pLocation );
	SetLocNo ( nLocIdx, nLocNo );
	SetNetworkNo ( nLocIdx, nNetworkNo );
	SetDbNo ( nLocIdx, 0 );
	SetName ( nLocIdx, "" );

	int nTNo = NetworkTerminalMap.FindFreeTerminalNumber( nNetworkNo );
	if ( nTNo != 0 )
	{
		int nIdx;
		dbLocation.AddTerminal( nLocIdx, nTNo, 0, 0, 0, nIdx );
		NetworkTerminalMap.AddTerminal( nNetworkNo, nTNo, 0 );
	}
	
	::CreateSubdirectory( GetFolderPathData( nLocIdx ) );
	::CreateSubdirectory( GetFolderPathSysset( nLocIdx ) );

	ResetPluPresetDate( nLocIdx, 0 );
	ResetPluInfoDate( nLocIdx, 0 );

	WriteLocation ( nLocIdx );

	return nLocIdx;
}

/**********************************************************************/

int CLocationCSVArray::AddLocation( int nNetworkNo, int nLocNo, const char* szPath )
{
	int nLocIdx;
	if ( FindLocationByNumber ( nNetworkNo, nLocNo, nLocIdx ) == FALSE )
	{
		CLocationCSVRecord* pLocation = new CLocationCSVRecord;
		
		CString strFileInfo;
		strFileInfo.Format ( "%sInfo.dat", szPath );

		CSSFile fileInfo;
		if ( fileInfo.Open( strFileInfo, "rb" ) )
		{
			CString strBuffer;
			fileInfo.ReadString( strBuffer );
			CCSV csv ( strBuffer );
			pLocation -> ConvertFromCSV ( csv ); 
			InsertAt ( nLocIdx, pLocation );
			SetLocNo ( nLocIdx, nLocNo );
			SetNetworkNo ( nLocIdx, nNetworkNo );
		}
		else
		{
			pLocation -> SetLocNo ( nLocNo );
			pLocation -> SetNetworkNo ( nNetworkNo );
			pLocation -> SetName ( "" );

			int nTerminalIdx;
			pLocation -> AddTerminal( 1, 0, 0, 0, nTerminalIdx );
			InsertAt ( nLocIdx, pLocation );
		}
	}
	return nLocIdx;
}

/**********************************************************************/

bool CLocationCSVArray::DeleteLocationByNumber ( int nNetworkNo, int nLocNo )
{
	int nLocIdx;
	
	if ( FindLocationByNumber ( nNetworkNo, nLocNo, nLocIdx ) == TRUE )
		return DeleteLocationByIndex ( nLocIdx, TRUE );
	
	return FALSE;
}

/**********************************************************************/

bool CLocationCSVArray::DeleteLocationByIndex ( int nLocIdx, bool bDeleteFiles )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		if ( bDeleteFiles == TRUE )
		{
			ClearLocationFolder( nLocIdx );
			ClearDeletionFolder( nLocIdx );
		}

#ifndef POSTRAY_UTILITY
		LocationMasterMap.DeleteLocation( nLocIdx );
#endif

		delete GetLocation ( nLocIdx );
		RemoveAt ( nLocIdx );
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CLocationCSVArray::ClearLocationFolder ( int nLocIdx )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CFileRemover FileRemover;
		FileRemover.RemoveFolder( dbLocation.GetFolderPathData( nLocIdx ) );
		FileRemover.RemoveFolder( dbLocation.GetFolderPathSysset( nLocIdx ) );
	}
}
	
/**********************************************************************/

void CLocationCSVArray::ClearDeletionFolder ( int nLocIdx )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CFileRemover FileRemover;
		FileRemover.RemoveFolder( dbLocation.GetFolderPathDataDelete( nLocIdx ) );
		FileRemover.RemoveFolder( dbLocation.GetFolderPathSyssetDelete( nLocIdx ) );
	}
}

/**********************************************************************/

bool CLocationCSVArray::RenameForDeletion( int nLocIdx )
{
	if ( nLocIdx < 0 || nLocIdx >= GetSize() )
		return FALSE;

	ClearDeletionFolder( nLocIdx );	

	if ( rename ( GetFolderPathData( nLocIdx ), GetFolderPathDataDelete( nLocIdx ) ) != 0 )
		return FALSE;

	rename ( GetFolderPathSysset( nLocIdx ), GetFolderPathSyssetDelete( nLocIdx ) );
	
	return TRUE;
}

/**********************************************************************/

void CLocationCSVArray::PurgeNetwork( int nNetworkIdx )
{
	if ( nNetworkIdx < 0 || nNetworkIdx >= dbNetwork.GetSize() )
		return;

	int nStartIdx, nEndIdx;
	GetNetworkLocIdxRange ( nNetworkIdx, nStartIdx, nEndIdx );

	for ( int nLocIdx = nEndIdx; nLocIdx >= nStartIdx; nLocIdx-- )
		DeleteLocationByIndex ( nLocIdx, TRUE );
}

/**********************************************************************/

void CLocationCSVArray::PurgeDatabase( int nDbIdx )
{
	if ( nDbIdx < 0 || nDbIdx >= dbDatabase.GetSize() )
		return;

	int nDbNo = dbDatabase.GetDbNo( nDbIdx );

	for ( int nLocIdx = 0; nLocIdx < GetSize(); nLocIdx++ )
	{
		if ( GetDbNo ( nLocIdx ) == nDbNo )
		{
			SetDbNo( nLocIdx, 0 );

			for ( int nTerminalIdx = 0; nTerminalIdx < GetTerminalCount( nLocIdx ); nTerminalIdx++ )
			{
				SetRpNoLegacy ( nLocIdx, nTerminalIdx, 0 );
				SetRpFlags ( nLocIdx, nTerminalIdx, 0 );
#ifdef STOCKMAN_SYSTEM
				SetSpNo ( nLocIdx, nTerminalIdx, 0 );
#endif
			}
		}
	}
}

/**********************************************************************/

bool CLocationCSVArray::FindLocationByNumber( int nNetworkNo, int nLocNo, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	int nSearchNo = ( 1000 * nNetworkNo ) + nLocNo;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;
		int nArrayNo = ( 1000 * GetNetworkNo ( nIndex ) ) + GetLocNo ( nIndex );
		
		if ( nArrayNo == nSearchNo )
			return TRUE;
		else if ( nArrayNo < nSearchNo )
			nStart = nIndex + 1;
		else 
			nEnd = nIndex - 1;
	}
	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

void CLocationCSVArray::GetNetworkLocIdxRange ( int nNetworkIdx, int& nStartIdx, int& nEndIdx )
{
	nStartIdx = 0;
	nEndIdx = -1;

	if ( nNetworkIdx < 0 || nNetworkIdx >= dbNetwork.GetSize() )
		return;

	int nNetworkNo = dbNetwork.GetNetworkNo( nNetworkIdx );

	FindLocationByNumber ( nNetworkNo, 0, nStartIdx );
	FindLocationByNumber ( nNetworkNo + 1, 0, nEndIdx );
	nEndIdx--;
}

/**********************************************************************/

void CLocationCSVArray::GetDatabaseLocIdxSet( int nDbIdx, CArray<int,int>& Set )
{
	Set.RemoveAll();

	if ( nDbIdx < 0 || nDbIdx >= dbDatabase.GetSize() )
		return;

	int nDbNo = dbDatabase.GetDbNo ( nDbIdx );

	for ( int nLocIdx = 0; nLocIdx < GetSize(); nLocIdx++ )
		if ( GetDbNo ( nLocIdx ) == nDbNo )
			Set.Add ( nLocIdx );
}

/**********************************************************************/

void CLocationCSVArray::FixWebSalesTerminals( int nLocIdx )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> FixWebSalesTerminals();
	}
}

/**********************************************************************/
	
int CLocationCSVArray::GetLocNo ( int nLocIdx )
{
	int nLocNo = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nLocNo = pLocation -> m_nLocNo;
	}
	return nLocNo;
}

/**********************************************************************/

int CLocationCSVArray::GetNetworkNo ( int nLocIdx )
{
	int nNetworkNo = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nNetworkNo = pLocation -> m_nNetworkNo;
	}
	return nNetworkNo;
}

/**********************************************************************/

int CLocationCSVArray::GetNetworkLocNo ( int nLocIdx )
{
	int nNetworkLocNo = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nNetworkLocNo = ( pLocation -> m_nNetworkNo * 1000 ) + pLocation -> m_nLocNo;
	}
	return nNetworkLocNo;
}

/**********************************************************************/

int CLocationCSVArray::GetDbNo ( int nLocIdx )
{
	int nDbNo = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nDbNo = pLocation -> m_nDbNo;
	}
	return nDbNo;
}

/**********************************************************************/

int CLocationCSVArray::GetObsoleteSiteNo ( int nLocIdx )
{
	int nSiteNo = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nSiteNo = pLocation -> m_nObsoleteSiteNo;
	}
	return nSiteNo;
}

/**********************************************************************/

const char* CLocationCSVArray::GetName ( int nLocIdx )
{
	m_strName = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		m_strName = pLocation -> m_strName;
	}
	return m_strName;
}

/**********************************************************************/

const char* CLocationCSVArray::GetTreeName ( int nLocIdx )
{
	m_strTreeName = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		m_strTreeName = pLocation -> m_strName;
		m_strTreeName += " ";

		int nSPOSVersion;
		int nConnectionType;
		GetLocSPOSVersion( nLocIdx, nSPOSVersion, nConnectionType );

		CString strExtra = "";
		CNetworkCSVRecord::GetTreeNameExternal( nConnectionType, nSPOSVersion, strExtra, TRUE );
		m_strTreeName += strExtra;
	}
	return m_strTreeName;
}

/**********************************************************************/

const char* CLocationCSVArray::GetSystemName ( int nLocIdx )
{
	m_strSystemName = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		m_strSystemName = pLocation -> GetSystemName();
	}
	return m_strSystemName;
}

/**********************************************************************/

const char* CLocationCSVArray::GetExtendedSystemName ( int nLocIdx )
{
	GetSystemName( nLocIdx );
	GetName( nLocIdx );

	if ( m_strSystemName != m_strName )
	{
		m_strSystemName += " (";
		m_strSystemName += m_strName;
		m_strSystemName += ")";
	}

	return m_strSystemName;
}

/**********************************************************************/

const char* CLocationCSVArray::GetIDraughtID ( int nLocIdx )
{
	m_strIDraughtID = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		m_strIDraughtID = pLocation -> m_strIDraughtID;
	}
	return m_strIDraughtID;
}

/**********************************************************************/

const char* CLocationCSVArray::GetRemoteID ( int nLocIdx )
{
	m_strRemoteID = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		m_strRemoteID = pLocation -> m_strRemoteID;
	}
	return m_strRemoteID;
}

/**********************************************************************/

int CLocationCSVArray::GetLoyaltyFolderSet(int nLocIdx)
{
	int nResult = 0;
	if (nLocIdx >= 0 && nLocIdx < GetSize())
	{
		CLocationCSVRecord* pLocation = GetLocation(nLocIdx);
		nResult = pLocation->m_nLoyaltyFolderSet;
	}
	return nResult;
}

/**********************************************************************/

int CLocationCSVArray::GetDefaultPriceSet ( int nLocIdx )
{
	int nSet = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nSet = pLocation -> m_nDefaultPriceSet;
	}
	return nSet;
}

/**********************************************************************/

int CLocationCSVArray::GetPluPriceSetCount ( int nLocIdx )
{
	int nCount = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nCount = pLocation -> m_nPluPriceSetCount;
	}
	return nCount;
}

/**********************************************************************/

int CLocationCSVArray::GetDbPriceSet1 ( int nLocIdx )
{
	int nSet = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nSet = pLocation -> m_nDbPriceSet1;
	}
	return nSet;
}

/**********************************************************************/

int CLocationCSVArray::GetDbPriceSet2 ( int nLocIdx )
{
	int nSet = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nSet = pLocation -> m_nDbPriceSet2;
	}
	return nSet;
}

/**********************************************************************/

int CLocationCSVArray::GetDbPriceSet3 ( int nLocIdx )
{
	int nSet = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nSet = pLocation -> m_nDbPriceSet3;
	}
	return nSet;
}

/**********************************************************************/

int CLocationCSVArray::GetPriceSetKeyboard1 ( int nLocIdx )
{
	int nKbd = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nKbd = pLocation -> m_nPriceSetKeyboard1;
	}
	return nKbd;
}

/**********************************************************************/

int CLocationCSVArray::GetPriceSetKeyboard2 ( int nLocIdx )
{
	int nKbd = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nKbd = pLocation -> m_nPriceSetKeyboard2;
	}
	return nKbd;
}

/**********************************************************************/

int CLocationCSVArray::GetPriceSetKeyboard3 ( int nLocIdx )
{
	int nKbd = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nKbd = pLocation -> m_nPriceSetKeyboard3;
	}
	return nKbd;
}

/**********************************************************************/

int CLocationCSVArray::GetPriceSetKeyboardJob( int nLocIdx, int nPriceSet )
{
	int nJob = 0;

	if ( ( nLocIdx >= 0 ) && ( nLocIdx < GetSize() ) )
	{
		if ( 0 == nPriceSet )
			nPriceSet = GetDefaultPriceSet( nLocIdx );

		if ( ( nPriceSet >= 1 ) && ( nPriceSet <= GetPluPriceSetCount( nLocIdx ) ) )
		{
			int nDbPriceSet = -1;
			int nKeyboard = 0;

			switch( nPriceSet )
			{
			case 1: 
				nDbPriceSet = GetDbPriceSet1( nLocIdx );	
				nKeyboard = GetPriceSetKeyboard1( nLocIdx );
				break;

			case 2: 
				nDbPriceSet = GetDbPriceSet2( nLocIdx );	
				nKeyboard = GetPriceSetKeyboard2( nLocIdx );
				break;

			case 3: 
				nDbPriceSet = GetDbPriceSet3( nLocIdx );	
				nKeyboard = GetPriceSetKeyboard3( nLocIdx );
				break;
			}

			if ( 0 == nDbPriceSet )
			{
				switch( nKeyboard )
				{
				case 1:		nJob = DN_KBD1;		break;
				case 2:		nJob = DN_KBD2;		break;
				case 3:		nJob = DN_KBD3;		break;
				case 4:		nJob = DN_KBD4;		break;
				case 5:		nJob = DN_KBD5;		break;
				case 6:		nJob = DN_KBD6;		break;
				case 7:		nJob = DN_KBD7;		break;
				case 8:		nJob = DN_KBD8;		break;
				case 9:		nJob = DN_KBD9;		break;
				case 10:	nJob = DN_KBD10;	break;
				case 11:	nJob = DN_KBD11;	break;
				case 12:	nJob = DN_KBD12;	break;
				case 13:	nJob = DN_KBD13;	break;
				case 14:	nJob = DN_KBD14;	break;
				default:	nJob = -2;			break;
				}
			}
			else
				nJob = -1;
		}
	}

	return nJob;
}

/**********************************************************************/

int CLocationCSVArray::GetDbKeyboardSet ( int nLocIdx )
{
	int nSet = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nSet = pLocation -> m_nDbKeyboardSet;
	}
	return nSet;
}

/**********************************************************************/

bool CLocationCSVArray::GetObsoleteUploadPathFlag( int nLocIdx )
{
	bool bFlag = FALSE;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		bFlag = pLocation -> m_bObsoleteUploadPath;
	}
	return bFlag;
}

/**********************************************************************/

bool CLocationCSVArray::GetObsoleteRecordFlag( int nLocIdx )
{
	bool bFlag = FALSE;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		bFlag = pLocation -> m_bObsoleteRecord;
	}
	return bFlag;
}

/**********************************************************************/

const char* CLocationCSVArray::GetTerminalExportPath( int nLocIdx )
{
	m_strTerminalExportPath = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		m_strTerminalExportPath = pLocation -> m_strTerminalExportPath;
	}
	return m_strTerminalExportPath;
}

/**********************************************************************/

const char* CLocationCSVArray::GetObsoleteUploadPath( int nLocIdx )
{
	m_strObsoleteUploadPath = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		m_strObsoleteUploadPath = pLocation -> m_strObsoleteUploadPath;
	}
	return m_strObsoleteUploadPath;
}

/**********************************************************************/

const char* CLocationCSVArray::GetDownloadPath( int nLocIdx )
{
	m_strDownloadPath = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		m_strDownloadPath = pLocation -> m_strDownloadPath;
	}
	return m_strDownloadPath;
}

/**********************************************************************/

const char* CLocationCSVArray::GetDownloadNowPath( int nLocIdx )
{
	m_strDownloadNowPath = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		m_strDownloadNowPath = pLocation -> m_strDownloadNowPath;
	}
	return m_strDownloadNowPath;
}

/**********************************************************************/

const char* CLocationCSVArray::GetExportControlPath( int nLocIdx )
{
	m_strExportControlPath = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		m_strExportControlPath = pLocation -> m_strExportControlPath;
	}
	return m_strExportControlPath;
}

/**********************************************************************/

void CLocationCSVArray::GetSafeTerminalExportPath ( int nLocIdx, CString& strPath )
{
	strPath = "";
	if ( nLocIdx < 0 || nLocIdx >= GetSize() )
		return;

	strPath = GetTerminalExportPath( nLocIdx );
	::TrimSpaces ( strPath, FALSE );
	if ( strPath == "" )
		return;

	if ( strPath.Right(1) != "\\" )
		strPath += "\\";
}

/**********************************************************************/

void CLocationCSVArray::GetSafeDownloadPath ( int nLocIdx, CString& strPath )
{
	strPath = "";
	if ( nLocIdx < 0 || nLocIdx >= GetSize() )
		return;

	strPath = GetDownloadPath( nLocIdx );
	::TrimSpaces ( strPath, FALSE );
	if ( strPath == "" )
		return;

	if ( strPath.Right(1) != "\\" )
		strPath += "\\";
}

/**********************************************************************/

void CLocationCSVArray::GetSafeDownloadNowPath ( int nLocIdx, CString& strPath )
{
	strPath = "";
	if ( nLocIdx < 0 || nLocIdx >= GetSize() )
		return;

	strPath = GetDownloadNowPath( nLocIdx );
	::TrimSpaces ( strPath, FALSE );
	if ( strPath == "" )
		return;

	if ( strPath.Right(1) != "\\" )
		strPath += "\\";
}

/**********************************************************************/

void CLocationCSVArray::GetSafeExportControlPath ( int nLocIdx, CString& strPath )
{
	strPath = "";
	if ( nLocIdx < 0 || nLocIdx >= GetSize() )
		return;

	strPath = GetExportControlPath( nLocIdx );
	::TrimSpaces ( strPath, FALSE );
	if ( strPath == "" )
		return;

	if ( strPath.Right(1) != "\\" )
		strPath += "\\";
}

/**********************************************************************/

bool CLocationCSVArray::GetExportControlFlag( int nLocIdx )
{
	bool bResult = FALSE;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		bResult = pLocation -> m_bExportControl;
	}
	return bResult;
}

/**********************************************************************/

int CLocationCSVArray::GetExportControlDelay( int nLocIdx )
{
	int nResult = 1;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nResult = pLocation -> m_nExportControlDelay;
	}
	return nResult;
}

/**********************************************************************/

int CLocationCSVArray::GetProdCSVImportFolderType( int nLocIdx )
{
	int nResult = EODIMPORT_FOLDERTYPE_NONE;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nResult = pLocation -> m_nProdCSVImportFolderType;
	}
	return nResult;
}

/**********************************************************************/

int CLocationCSVArray::GetProdCSVImportExistingMode( int nLocIdx )
{
	int nResult = 0;
	
	int nFixedMode = DealerFlags.GetProdCSVImportExistingMode();
	if ( ( nFixedMode >= 1 ) && ( nFixedMode <= 3 ) )
		nResult = nFixedMode - 1;
	else
	{
		if ( nLocIdx >= 0 && nLocIdx < GetSize() )
		{
			CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
			nResult = pLocation -> m_nProdCSVImportExistingMode;
		}
	}

	if ( ( 2 == nResult ) && ( EcrmanOptions.GetPluPriceSetsFlag() == FALSE ) )
		nResult = 1;

	return nResult;
}

/**********************************************************************/

int CLocationCSVArray::GetProdCSVImportTypeMask( int nLocIdx )
{
	int nResult = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nResult = pLocation -> m_nProdCSVImportTypeMask;
	}
	return nResult;
}

/**********************************************************************/

int CLocationCSVArray::GetProdCSVImportModifierMode( int nLocIdx )
{
	int nResult = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nResult = pLocation -> m_nProdCSVImportModifierMode;
	}
	return nResult;
}

/**********************************************************************/

int CLocationCSVArray::GetProdCSVProtectFieldMask( int nLocIdx )
{
	int nResult = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nResult = pLocation -> m_nProdCSVProtectFieldMask;
	}
	return nResult;
}

/**********************************************************************/

int CLocationCSVArray::GetCustomerImportFolderType( int nLocIdx )
{
	int nResult = EODIMPORT_FOLDERTYPE_ROOT;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nResult = pLocation -> m_nCustomerImportFolderType;
	}
	return nResult;
}

/**********************************************************************/

int CLocationCSVArray::GetPluDownloadRangeType( int nLocIdx )
{
	int nResult = PLUDOWNLOADRANGE_NONE;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nResult = pLocation -> m_nPluDownloadRangeType;
	}
	return nResult;
}

/**********************************************************************/

__int64 CLocationCSVArray::GetPluDownloadRangeStart( int nLocIdx, int x )
{
	__int64 nResult = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nResult = pLocation -> GetPluDownloadRangeStart(x);
	}
	return nResult;
}

/**********************************************************************/

__int64 CLocationCSVArray::GetPluDownloadRangeEnd( int nLocIdx, int x )
{
	__int64 nResult = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nResult = pLocation -> GetPluDownloadRangeEnd(x);
	}
	return nResult;
}

/**********************************************************************/

int CLocationCSVArray::GetBritanniaDefaultPriceBand( int nLocIdx )
{
	int nResult = 1;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nResult = pLocation -> m_nBritanniaDefaultPriceBand;
	}
	return nResult;
}

/**********************************************************************/

const char* CLocationCSVArray::GetAgressoLocationGroup( int nLocIdx )
{
	m_strAgressoLocationGroup = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		m_strAgressoLocationGroup = pLocation -> m_strAgressoLocationGroup;
	}
	return m_strAgressoLocationGroup;
}

/**********************************************************************/

void CLocationCSVArray::SetLocNo ( int nLocIdx, int nLocNo )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetLocNo ( nLocNo );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetNetworkNo ( int nLocIdx, int nNetworkNo )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetNetworkNo ( nNetworkNo );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetDbNo ( int nLocIdx, int nDbNo )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetDbNo ( nDbNo );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetObsoleteSiteNo ( int nLocIdx, int nSiteNo )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetObsoleteSiteNo ( nSiteNo );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetName ( int nLocIdx, const char* szName )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetName ( szName );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetIDraughtID ( int nLocIdx, const char* szID )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetIDraughtID ( szID );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetRemoteID ( int nLocIdx, const char* szID )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetRemoteID ( szID );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetLoyaltyFolderSet(int nLocIdx, int nSet)
{
	if (nLocIdx >= 0 && nLocIdx < GetSize())
	{
		CLocationCSVRecord* pLocation = GetLocation(nLocIdx);
		
		if ((nSet >= 0) && (nSet <= 10))
		{
			pLocation->SetLoyaltyFolderSet(nSet);
		}
	}
}

/**********************************************************************/

void CLocationCSVArray::SetDefaultPriceSet ( int nLocIdx, int nSet )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetDefaultPriceSet ( nSet );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetPluPriceSetCount ( int nLocIdx, int nCount )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetPluPriceSetCount ( nCount );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetDbPriceSet1 ( int nLocIdx, int nSet )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetDbPriceSet1 ( nSet );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetDbPriceSet2 ( int nLocIdx, int nSet )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetDbPriceSet2 ( nSet );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetDbPriceSet3 ( int nLocIdx, int nSet )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetDbPriceSet3 ( nSet );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetPriceSetKeyboard1 ( int nLocIdx, int nKbd )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetPriceSetKeyboard1 ( nKbd );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetPriceSetKeyboard2 ( int nLocIdx, int nKbd )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetPriceSetKeyboard2 ( nKbd );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetPriceSetKeyboard3 ( int nLocIdx, int nKbd )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetPriceSetKeyboard3 ( nKbd );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetDbKeyboardSet ( int nLocIdx, int nSet )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetDbKeyboardSet ( nSet );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetObsoleteUploadPathFlag ( int nLocIdx, bool bFlag )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetObsoleteUploadPathFlag ( bFlag );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetTerminalExportPath ( int nLocIdx, const char* szPath )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetTerminalExportPath ( szPath );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetObsoleteUploadPath ( int nLocIdx, const char* szPath )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetObsoleteUploadPath ( szPath );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetDownloadPath ( int nLocIdx, const char* szPath )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetDownloadPath ( szPath );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetDownloadNowPath ( int nLocIdx, const char* szPath )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetDownloadNowPath ( szPath );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetExportControlPath ( int nLocIdx, const char* szPath )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetExportControlPath ( szPath );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetExportControlFlag ( int nLocIdx, bool bFlag )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetExportControlFlag ( bFlag );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetExportControlDelay( int nLocIdx, int nDelay )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetExportControlDelay( nDelay );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetProdCSVImportFolderType ( int nLocIdx, int nType )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetProdCSVImportFolderType ( nType );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetProdCSVImportExistingMode ( int nLocIdx, int nMode )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetProdCSVImportExistingMode ( nMode );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetProdCSVImportTypeMask ( int nLocIdx, int nMask )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetProdCSVImportTypeMask ( nMask );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetProdCSVProtectFieldMask ( int nLocIdx, int nMask )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetProdCSVProtectFieldMask ( nMask );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetProdCSVImportModifierMode ( int nLocIdx, int nMode )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetProdCSVImportModifierMode ( nMode );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetCustomerImportFolderType ( int nLocIdx, int nType )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetCustomerImportFolderType ( nType );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetPluDownloadRangeType( int nLocIdx, int n )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation( nLocIdx );
		pLocation -> SetPluDownloadRangeType(n);
	}
}

/**********************************************************************/

void CLocationCSVArray::SetPluDownloadRangeStart( int nLocIdx, int x, __int64 n )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetPluDownloadRangeStart(x,n);
	}
}

/**********************************************************************/

void CLocationCSVArray::SetPluDownloadRangeEnd( int nLocIdx, int x, __int64 n )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetPluDownloadRangeEnd(x,n);
	}
}

/**********************************************************************/

void CLocationCSVArray::SetBritanniaDefaultPriceBand ( int nLocIdx, int nBand )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetBritanniaDefaultPriceBand ( nBand );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetAgressoLocationGroup( int nLocIdx, const char* sz )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetAgressoLocationGroup( sz );
	}
}

/**********************************************************************/

void CLocationCSVArray::GetPluPresetFilename( int nLocIdx, int nTerminalNo, CString& strFilename )
{
	CFilenameUpdater FnUp( SysFiles::LocPresetDate, nLocIdx );
	strFilename = FnUp.GetFilenameToUse();

	if ( ( nTerminalNo >= 1 ) && ( nTerminalNo <= 999 ) )
	{
		strFilename.MakeUpper();

		int nLength = strFilename.GetLength();
		if ( nLength > 12 )
		{
			if ( strFilename.Right(12) == "FILE3033.DAT" )
			{
				CString strMaster;
				strMaster.Format( "MASTER%3.3d\\", nTerminalNo );
				
				strFilename = strFilename.Left( nLength - 12 );
				strFilename += strMaster;
				CreateSubdirectory( strFilename );

				strFilename += "FILE3033.DAT";
			}
		}
	}
}

/**********************************************************************/

const char* CLocationCSVArray::GetPluPresetDate ( int nLocIdx, int nTerminalNo )
{
	m_strPluPresetDate = "";
	if ( ( nLocIdx >= 0 ) && ( nLocIdx < GetSize() ) )
	{
		CString strFilename;
		GetPluPresetFilename( nLocIdx, nTerminalNo, strFilename );

		CSSFile fileDate;
		if ( fileDate.Open( strFilename, "rb" ) == TRUE )
			fileDate.ReadString( m_strPluPresetDate );
	}
	return m_strPluPresetDate;
}

/**********************************************************************/

void CLocationCSVArray::SetPluPresetDate ( int nLocIdx, int nTerminalNo )
{
	if ( ( nLocIdx >= 0 ) && ( nLocIdx < GetSize() ) )
	{
		COleDateTime tm = COleDateTime::GetCurrentTime();
		CString strTime = tm.Format ( "%Y%m%d%H%M%S" );

		CString strFilename;
		GetPluPresetFilename( nLocIdx, nTerminalNo, strFilename );
		
		CSSFile fileDate;
		if ( fileDate.Open( strFilename, "wb" ) == TRUE )
			fileDate.WriteLine( strTime );
	}
}

/**********************************************************************/

void CLocationCSVArray::ResetPluPresetDate ( int nLocIdx, int nTerminalNo )
{
	if ( ( nLocIdx >= 0 ) && ( nLocIdx < GetSize() ) )
	{
		CString strFilename;
		GetPluPresetFilename( nLocIdx, nTerminalNo, strFilename );
		CFileRemover FileRemover( strFilename );
	}
}

/**********************************************************************/

void CLocationCSVArray::GetPluInfoFilename( int nLocIdx, int nTerminalNo, CString& strFilename )
{
	CFilenameUpdater FnUp( SysFiles::LocInfoDate, nLocIdx );
	strFilename = FnUp.GetFilenameToUse();

	if ( ( nTerminalNo >= 1 ) && ( nTerminalNo <= 999 ) )
	{
		strFilename.MakeUpper();

		int nLength = strFilename.GetLength();
		if ( nLength > 12 )
		{
			if ( strFilename.Right(12) == "FILE3038.DAT" )
			{
				CString strMaster;
				strMaster.Format( "MASTER%3.3d\\", nTerminalNo );
				
				strFilename = strFilename.Left( nLength - 12 );
				strFilename += strMaster;
				CreateSubdirectory( strFilename );

				strFilename += "FILE3038.DAT";
			}
		}
	}
}

/**********************************************************************/

const char* CLocationCSVArray::GetPluInfoDate ( int nLocIdx, int nTerminalNo )
{
	m_strPluInfoDate = "";
	if ( ( nLocIdx >= 0 ) && ( nLocIdx < GetSize() ) )
	{
		CString strFilename;
		GetPluInfoFilename( nLocIdx, nTerminalNo, strFilename );

		CSSFile fileDate;
		if ( fileDate.Open( strFilename, "rb" ) == TRUE )
			fileDate.ReadString( m_strPluInfoDate );
	}
	return m_strPluInfoDate;
}

/**********************************************************************/

void CLocationCSVArray::SetPluInfoDate ( int nLocIdx, int nTerminalNo )
{
	if ( ( nLocIdx >= 0 ) && ( nLocIdx < GetSize() ) )
	{
		COleDateTime tm = COleDateTime::GetCurrentTime();
		CString strTime = tm.Format ( "%Y%m%d%H%M%S" );

		CString strFilename;
		GetPluInfoFilename( nLocIdx, nTerminalNo, strFilename );
		
		CSSFile fileDate;
		if ( fileDate.Open( strFilename, "wb" ) == TRUE )
			fileDate.WriteLine( strTime );
	}
}

/**********************************************************************/

void CLocationCSVArray::ResetPluInfoDate ( int nLocIdx, int nTerminalNo )
{
	if ( ( nLocIdx >= 0 ) && ( nLocIdx < GetSize() ) )
	{
		CString strFilename;
		GetPluInfoFilename( nLocIdx, nTerminalNo, strFilename );
		CFileRemover FileRemover( strFilename );
	}
}

/**********************************************************************/

int CLocationCSVArray::GetTerminalCount ( int nLocIdx )
{
	int nCount = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nCount = pLocation -> m_arrayTerminals.GetSize();
	}
	return nCount;
}

/**********************************************************************/

int CLocationCSVArray::GetTNo ( int nLocIdx, int nTerminalIdx )
{
	int nTerminalNo = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		
		if ( nTerminalIdx >= 0 && nTerminalIdx < pLocation -> GetTerminalCount() )
			nTerminalNo = pLocation -> GetTNo( nTerminalIdx );
	}
	return nTerminalNo;
}

/**********************************************************************/

const char* CLocationCSVArray::GetTerminalName ( int nLocIdx, int nTerminalIdx )
{
	m_strTerminalName = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		
		if ( nTerminalIdx >= 0 && nTerminalIdx < pLocation -> GetTerminalCount() )
			m_strTerminalName = LocationMasterMap.GetTerminalName( nLocIdx, pLocation -> GetTNo( nTerminalIdx ) );
			//m_strTerminalName.Format ( "TNo %d", pLocation -> GetTNo( nTerminalIdx ) );
	}
	return m_strTerminalName;
}

/**********************************************************************/

int CLocationCSVArray::GetSpNo ( int nLocIdx, int nTerminalIdx )
{
	int nSpNo = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nSpNo = pLocation -> GetSpNo( nTerminalIdx );
	}
	return nSpNo;
}

/**********************************************************************/

int CLocationCSVArray::GetRpNoLegacy ( int nLocIdx, int nTerminalIdx )
{
	int nRpNo = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nRpNo = pLocation -> GetRpNoLegacy( nTerminalIdx );
	}
	return nRpNo;
}

/**********************************************************************/

__int64 CLocationCSVArray::GetRpFlags ( int nLocIdx, int nTerminalIdx )
{
	__int64 nRpFlags = 0;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nRpFlags = pLocation -> GetRpFlags( nTerminalIdx );
	}
	return nRpFlags;
}

/**********************************************************************/

bool CLocationCSVArray::GetRpFlag ( int nLocIdx, int nTerminalIdx, int nRpNo )
{
	bool bResult = FALSE;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		bResult = pLocation -> GetRpFlag( nTerminalIdx, nRpNo );
	}
	return bResult;
}

/**********************************************************************/

bool CLocationCSVArray::HasRpNoLink( int nLocIdx, int nTerminalIdx, int nRpNo )
{
	bool bResult = FALSE;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		if ( ( nRpNo >= 1 ) && ( nRpNo <= MAX_SHARED_RPNO ) )
			bResult = GetRpFlag( nLocIdx, nTerminalIdx, nRpNo );
		else if ( ( nRpNo > MAX_SHARED_RPNO ) && ( nRpNo <= 999 ) )
			bResult = ( GetRpNoLegacy( nLocIdx, nTerminalIdx ) == nRpNo );
	}
	return bResult;
}

/**********************************************************************/

bool CLocationCSVArray::CanAddRpNoLink( int nLocIdx, int nTerminalIdx, int nRpNo )
{
	bool bResult = FALSE;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		if ( ( nRpNo >= 1 ) && ( nRpNo <= MAX_SHARED_RPNO ) )
			bResult = ( GetRpFlag( nLocIdx, nTerminalIdx, nRpNo ) == FALSE );
		else if ( ( nRpNo > MAX_SHARED_RPNO ) && ( nRpNo <= 999 ) )
			bResult = ( GetRpNoLegacy( nLocIdx, nTerminalIdx ) == 0 );
	}
	return bResult;
}

/**********************************************************************/

void CLocationCSVArray::SetRpNoLink( int nLocIdx, int nTerminalIdx, int nRpNo )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		if ( ( nRpNo >= 1 ) && ( nRpNo <= MAX_SHARED_RPNO ) )
			SetRpFlag( nLocIdx, nTerminalIdx, nRpNo );
		else if ( ( nRpNo > MAX_SHARED_RPNO ) && ( nRpNo <= 999 ) )
			SetRpNoLegacy( nLocIdx, nTerminalIdx, nRpNo );
	}
}

/**********************************************************************/

void CLocationCSVArray::ClearRpNoLink( int nLocIdx, int nTerminalIdx, int nRpNo )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		if ( ( nRpNo >= 1 ) && ( nRpNo <= MAX_SHARED_RPNO ) )
			ClearRpFlag( nLocIdx, nTerminalIdx, nRpNo );
		else if ( ( nRpNo > MAX_SHARED_RPNO ) && ( nRpNo <= 999 ) )
			if ( GetRpNoLegacy( nLocIdx, nTerminalIdx ) == nRpNo )
				SetRpNoLegacy( nLocIdx, nTerminalIdx, 0 );
	}
}

/**********************************************************************/

void CLocationCSVArray::ClearAllRpNoLinks( int nLocIdx, int nTerminalIdx )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		SetRpFlags( nLocIdx, nTerminalIdx, 0 );
		SetRpNoLegacy( nLocIdx, nTerminalIdx, 0 );
	}
}

/**********************************************************************/

int CLocationCSVArray::GetRpNoReport ( int nLocIdx, int nTerminalIdx )
{
	int nRpNo = -1;
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		nRpNo = pLocation -> GetRpNoReport( nTerminalIdx );

		if ( nRpNo == -1 )
		{
			nRpNo = 0;
			for ( int n = 1; n <= MAX_SHARED_RPNO; n++ )
			{
				if ( HasRpNoLink( nLocIdx, nTerminalIdx, n ) == TRUE )
				{
					nRpNo = n;
					break;
				}
			}

			if ( 0 == nRpNo )
				nRpNo = GetRpNoLegacy( nLocIdx, nTerminalIdx );
	
			pLocation -> SetRpNoReport( nTerminalIdx, nRpNo );
		}
	}
	return nRpNo;
}

/**********************************************************************/

void CLocationCSVArray::ResetRpNoReport()
{
	for ( int nLocIdx = 0; nLocIdx < GetSize(); nLocIdx++ )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		for ( int nTermIdx = 0; nTermIdx < pLocation -> GetTerminalCount(); nTermIdx++ )
			pLocation -> SetRpNoReport( nTermIdx, -1 );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetSpNo ( int nLocIdx, int nTerminalIdx, int nSpNo )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetSpNo ( nTerminalIdx, nSpNo );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetRpNoLegacy ( int nLocIdx, int nTerminalIdx, int nRpNo )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetRpNoLegacy ( nTerminalIdx, nRpNo );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetRpFlags ( int nLocIdx, int nTerminalIdx, __int64 nRpFlags )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetRpFlags ( nTerminalIdx, nRpFlags );
	}
}

/**********************************************************************/

void CLocationCSVArray::SetRpFlag ( int nLocIdx, int nTerminalIdx, int nRpNo )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> SetRpFlag ( nTerminalIdx, nRpNo );
	}
}

/**********************************************************************/

void CLocationCSVArray::ClearRpFlag ( int nLocIdx, int nTerminalIdx, int nRpNo )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> ClearRpFlag ( nTerminalIdx, nRpNo );
	}
}

/**********************************************************************/

const char* CLocationCSVArray::GetFilePathInfo( int nLocIdx )
{
	m_strFilePathInfo = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		m_strFilePathInfo = GetFolderPathData ( nLocIdx );
		m_strFilePathInfo += "\\info.dat";
	}
	return m_strFilePathInfo;
}

/**********************************************************************/

bool CLocationCSVArray::WriteLocation ( int nLocIdx )
{
	if ( nLocIdx < 0 || nLocIdx >= GetSize() )
		return FALSE;

	CSSFile fileLocation;
	if ( fileLocation.Open ( GetFilePathInfo ( nLocIdx ), "wb" ) == FALSE )
		return FALSE;

	fileLocation.WriteLine ( GetCSVLine ( nLocIdx ) );
	return TRUE;
}

/**********************************************************************/

const char* CLocationCSVArray::GetCSVLine ( int nLocIdx )
{
	m_strCSVLine = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		
		CCSV csv;
		pLocation ->ConvertToCSV(csv);

		m_strCSVLine = csv.GetLine();
	}
	return m_strCSVLine;
}

/**********************************************************************/

bool CLocationCSVArray::FindTerminalByNumber( int nLocIdx, int nTNo, int &nTerminalIdx )
{
	bool bResult = FALSE;
	nTerminalIdx = -1;

	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		bResult = pLocation -> FindTerminalByNumber( nTNo, nTerminalIdx );
	}

	return bResult;
}

/**********************************************************************/

bool CLocationCSVArray::AddTerminal( int nLocIdx, int nTNo, int nRpNoLegacy, __int64 nRpFlags, int nSpNo, int &nTerminalIdx )
{
	bool bResult = FALSE;
	nTerminalIdx = -1;

	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		bResult = pLocation -> AddTerminal( nTNo, nRpNoLegacy, nRpFlags, nSpNo, nTerminalIdx );
	}

	return bResult;
}

/**********************************************************************/

void CLocationCSVArray::RemoveTerminal( int nLocIdx, int nTNo )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> RemoveTerminal( nTNo );
	}
}

/**********************************************************************/

bool CLocationCSVArray::CanDoReports()
{
	for ( int nLocIdx = 0; nLocIdx < GetSize(); nLocIdx++ )
	{
		int nDbIdx;
		int nDbNo = dbLocation.GetDbNo( nLocIdx );
		if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) )
			return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CLocationCSVArray::GetLocSPOSVersion( int nLocIdx, int& nSPOSVersion, int& nConnectionType )
{
	nSPOSVersion = SPOS_V4;
	nConnectionType = CONNECTION_TYPE_STANDARD_NONE;

	if ( ( nLocIdx >= 0 ) && ( nLocIdx < GetSize() ) )
	{
		int nNetworkIdx;
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		if ( dbNetwork.FindNetworkByNumber( pLocation -> m_nNetworkNo, nNetworkIdx ) == TRUE )
		{
			nSPOSVersion = dbNetwork.GetSPOSVersion( nNetworkIdx );

			if ( SysInfo.IsSportsBookerSystem() == TRUE )
			{
				switch( dbNetwork.GetConnectionType( nNetworkIdx ) )
				{
				case CONNECTION_TYPE_SPTBOOK_NONE:
				case CONNECTION_TYPE_SPTBOOK_WEB:
					nConnectionType = CONNECTION_TYPE_SPTBOOK_NONE;
					break;
				}
			}

			if ( SysInfo.IsSmartEntertainmentSystem() == TRUE )
			{
				switch( dbNetwork.GetConnectionType( nNetworkIdx ) )
				{
				case CONNECTION_TYPE_SMARTENT_NONE:
				case CONNECTION_TYPE_SMARTENT_WEB:
					nConnectionType = CONNECTION_TYPE_SMARTENT_NONE;
					break;
				}
			}
		}
	}
}

/**********************************************************************/

bool CLocationCSVArray::IsPMSPseudoLocation( int nLocIdx )
{
#ifdef COMPILE_PMS

	if ( SysInfo.IsPMSSystem() == FALSE )
		return FALSE;

	if ( FALSE == m_bGotPMSPseudoLocation )
		return FALSE;

	if ( ( nLocIdx < 0 ) || ( nLocIdx >= GetSize() ) )
		return FALSE;

	CLocationCSVRecord* pLocation = GetLocation( nLocIdx );

	if ( pLocation -> m_nNetworkNo != 1 )
		return FALSE;

	if ( pLocation -> m_nLocNo != 999 )
		return FALSE;

	return TRUE;

#else

	return FALSE;

#endif
}

/**********************************************************************/

void CLocationCSVArray::CreatePMSPseudoLocation()
{
	if ( SysInfo.IsPMSSystem() == FALSE )
		return;
	
	if ( TRUE == m_bGotPMSPseudoLocation )
		return;

	if ( dbDatabase.GetSize() == 0 )
		return;

	int nNetworkNo = 1;
	int nLocNo = 999;

	int nLocIdx;
	if ( FindLocationByNumber ( nNetworkNo, nLocNo, nLocIdx ) == TRUE )
		return;

	CLocationCSVRecord* pLocation = new CLocationCSVRecord;	
	pLocation -> SetNetworkNo ( nNetworkNo );
	pLocation -> SetLocNo ( nLocNo );
	pLocation -> SetDbNo ( dbDatabase.GetDbNo(0) );
	pLocation -> SetName ( "Guest Accounts" );

	int nTerminalIdx;
	pLocation -> AddTerminal( 1, 0, 0, 0, nTerminalIdx );

	InsertAt ( nLocIdx, pLocation );
	NetworkTerminalMap.IncrementLocIdx( nLocIdx );
	NetworkTerminalMap.AddTerminal( nNetworkNo, 1, nLocIdx );
	
	m_bGotPMSPseudoLocation = TRUE;
}

/**********************************************************************/

void CLocationCSVArray::RemovePMSPseudoLocation()
{
	if ( SysInfo.IsPMSSystem() == FALSE )
		return;
	
	if ( FALSE == m_bGotPMSPseudoLocation )
		return;

	int nNetworkNo = 1;
	int nLocNo = 999;

	int nLocIdx;
	if ( FindLocationByNumber ( nNetworkNo, nLocNo, nLocIdx ) == FALSE )
		return;

	DeleteLocationByIndex( nLocIdx, FALSE );
	NetworkTerminalMap.DecrementLocIdx( nLocIdx + 1 );
	NetworkTerminalMap.RemoveTerminal( nNetworkNo, 1 );
	
	m_bGotPMSPseudoLocation = FALSE;
}

/**********************************************************************/

bool CLocationCSVArray::IsWebSalesLocation( int nLocIdx )
{
	bool bResult = FALSE;
	switch( GetConnectionType( nLocIdx ) )
	{
	case CONNECTION_TYPE_SPTBOOK_NONE:
	case CONNECTION_TYPE_SPTBOOK_WEB:
	case CONNECTION_TYPE_SMARTENT_NONE:
	case CONNECTION_TYPE_SMARTENT_WEB:
		bResult = TRUE;
		break;
	}
	return bResult;
}

/**********************************************************************/

int CLocationCSVArray::GetConnectionType( int nLocIdx )
{
	int nConnectionType = CONNECTION_TYPE_STANDARD_NONE;

	if ( ( nLocIdx >= 0 ) && ( nLocIdx < GetSize() ) )
	{
		CLocationCSVRecord* pLocation = GetLocation( nLocIdx );

		int nNetworkIdx;
		if ( dbNetwork.FindNetworkByNumber( pLocation -> m_nNetworkNo, nNetworkIdx ) == TRUE )
			nConnectionType = dbNetwork.GetConnectionType( nNetworkIdx );
	}

	return nConnectionType;
}

/**********************************************************************/

int CLocationCSVArray::GetWebSalesLocationCount()
{
	int nCount = 0;
	int nLastNetworkNo = -1;
	bool bWebSalesNetwork = FALSE;
	
	for ( int nLocIdx = 0; nLocIdx < GetSize(); nLocIdx++ )
	{
		CLocationCSVRecord* pLocation = GetLocation( nLocIdx );
		
		if ( pLocation -> m_nNetworkNo != nLastNetworkNo )
		{
			switch( GetConnectionType( nLocIdx ) )
			{
			case CONNECTION_TYPE_SPTBOOK_NONE:
			case CONNECTION_TYPE_SPTBOOK_WEB:
			case CONNECTION_TYPE_SMARTENT_NONE:
			case CONNECTION_TYPE_SMARTENT_WEB:
				bWebSalesNetwork = TRUE;
				break;

			default:
				bWebSalesNetwork = FALSE;
				break;
			}

			nLastNetworkNo = pLocation -> m_nNetworkNo;
		}

		if ( TRUE == bWebSalesNetwork )
			nCount++;
	}

	return nCount;
}

/**********************************************************************/

int CLocationCSVArray::GetSharpPosLocationCount()
{
	return GetSize() - GetWebSalesLocationCount();
}

/**********************************************************************/

void CLocationCSVArray::LoadLocationBuffer( int nLocIdx, CLocationCSVArray& buffer )
{
	if ( nLocIdx < 0 || nLocIdx >= GetSize() )
		return;

	if ( buffer.GetSize() != 1 )
		return;

	CLocationCSVRecord* pLocation;
	CLocationCSVRecord* pBuffer;

	pLocation = GetLocation( nLocIdx );
	pBuffer = buffer.GetLocation( 0 );

	CCSV csv;
	pLocation -> ConvertToCSV( csv );
	pBuffer -> ConvertFromCSV( csv );
}

/**********************************************************************/

void CLocationCSVArray::SaveLocationBuffer( int nLocIdx, CLocationCSVArray& buffer )
{
	if ( nLocIdx < 0 || nLocIdx >= GetSize() )
		return;

	if ( buffer.GetSize() != 1 )
		return;

	CLocationCSVRecord* pLocation;
	CLocationCSVRecord* pBuffer;

	pLocation = GetLocation( nLocIdx );
	pBuffer = buffer.GetLocation( 0 );

	CCSV csv;
	pBuffer -> ConvertToCSV( csv );
	pLocation -> ConvertFromCSV( csv );
}

/**********************************************************************/

int CLocationCSVArray::GetPriceSetUsageCount( int nLocIdx, int nPriceSet )
{
	if ( nLocIdx < 0 || nLocIdx >= GetSize() )
		return 0;

	if ( nPriceSet < 1 || nPriceSet > 3 )
		return 0;

	int nDbPriceSet;
	switch( nPriceSet )
	{
	case 1:	nDbPriceSet = GetDbPriceSet1( nLocIdx );	break;
	case 2:	nDbPriceSet = GetDbPriceSet2( nLocIdx );	break;
	case 3:	nDbPriceSet = GetDbPriceSet3( nLocIdx );	break;
	}

	if ( nDbPriceSet == 0 )
		return 1;

	int nDbIdx;
	int nDbNo = GetDbNo( nLocIdx );
	if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
		return 1;

	CArray<int,int> arrayLocIdx;
	GetDatabaseLocIdxSet( nDbIdx, arrayLocIdx );

	int nUseCount = 0;
	for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
	{
		int nLocIdxToCheck = arrayLocIdx.GetAt(n);
		int nPriceSets = GetPluPriceSetCount( nLocIdxToCheck );

		if ( ( nPriceSets >= 1 ) && GetDbPriceSet1( nLocIdxToCheck ) == nDbPriceSet ) nUseCount++;
		if ( ( nPriceSets >= 2 ) && GetDbPriceSet2( nLocIdxToCheck ) == nDbPriceSet ) nUseCount++;
		if ( ( nPriceSets >= 3 ) && GetDbPriceSet3( nLocIdxToCheck ) == nDbPriceSet ) nUseCount++;
	}

	return nUseCount;
}

/**********************************************************************/

void CLocationCSVArray::SortByName( CArray<int,int>& arrayLocIdx, int nDbIdx, int nNwkIdx, bool bForceGroupByDb )
{
	if ( ( nDbIdx >= 0 ) && ( nDbIdx < dbDatabase.GetSize() ) )
	{
		arrayLocIdx;
		GetDatabaseLocIdxSet( nDbIdx, arrayLocIdx );
		SortLocListByName( arrayLocIdx );
		return;
	}

	arrayLocIdx.RemoveAll();

	CReportConsolidationArray<CSortedLocation> arrayTemp;

	int nStart = 0;
	int nEnd = GetSize() - 1;

	if ( ( nNwkIdx >= 0 ) && ( nNwkIdx < dbNetwork.GetSize() ) )
		GetNetworkLocIdxRange( nNwkIdx, nStart, nEnd );

	for ( int nLocIdx = nStart; nLocIdx <= nEnd; nLocIdx++ )
	{
		if ( ( EcrmanOptions.GetReportsLocationSortType() == 2 ) && ( FALSE == bForceGroupByDb ) )
		{
			CSortedLocation item;
			item.m_strName = GetName( nLocIdx );
			item.m_nIdx = nLocIdx;
			arrayTemp.Consolidate( item );
		}
		else
		{
			CSortedLocation item;
			item.m_nDbNo = GetDbNo( nLocIdx );
			item.m_strName = GetName( nLocIdx );
			item.m_nIdx = nLocIdx;
			arrayTemp.Consolidate( item );
		}
	}

	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CSortedLocation item;
		arrayTemp.GetAt( n, item );
		arrayLocIdx.Add( item.m_nIdx );
	}
}

/**********************************************************************/

void CLocationCSVArray::SortLocListByName( CArray<int,int>& arrayLocIdx )
{
	CReportConsolidationArray<CSortedLocation> arrayTemp;

	for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
	{
		int nLocIdx = arrayLocIdx.GetAt(n);
		if ( nLocIdx >= 0 && nLocIdx < GetSize() )
		{
			if ( EcrmanOptions.GetReportsLocationSortType() == 2 )
			{
				CSortedLocation item;
				item.m_strName = GetName( nLocIdx );
				item.m_nIdx = nLocIdx;
				arrayTemp.Consolidate( item );
			}
			else
			{
				CSortedLocation item;
				item.m_nDbNo = GetDbNo( nLocIdx );
				item.m_strName = GetName( nLocIdx );
				item.m_nIdx = nLocIdx;
				arrayTemp.Consolidate( item );
			}
		}
	}

	arrayLocIdx.RemoveAll();
	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CSortedLocation item;
		arrayTemp.GetAt( n, item );
		arrayLocIdx.Add( item.m_nIdx );
	}
}

/**********************************************************************/

void CLocationCSVArray::BuildSortIdxTables()
{
	m_arraySortIdxToLocIdx.RemoveAll();
	m_arrayLocIdxToSortIdx.RemoveAll();

	SortByName( m_arraySortIdxToLocIdx );

	for ( int n = 0; n < GetSize(); n++ )
		m_arrayLocIdxToSortIdx.Add(0);

	for ( int n = 0; n < m_arraySortIdxToLocIdx.GetSize(); n++ )
	{
		int nLocIdx = m_arraySortIdxToLocIdx.GetAt(n);
		m_arrayLocIdxToSortIdx.SetAt( nLocIdx, n );
	}
}

/**********************************************************************/

int CLocationCSVArray::GetLocIdxFromSortIdx( int nSortIdx )
{
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
		return nSortIdx;

	if ( nSortIdx < 0 || nSortIdx >= m_arraySortIdxToLocIdx.GetSize() )
		return nSortIdx;
	else
		return m_arraySortIdxToLocIdx.GetAt( nSortIdx );
}

/**********************************************************************/

int CLocationCSVArray::GetSortIdxFromLocIdx( int nLocIdx )
{
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
		return nLocIdx;

	if ( nLocIdx < 0 || nLocIdx >= m_arrayLocIdxToSortIdx.GetSize() )
		return nLocIdx;
	else
		return m_arrayLocIdxToSortIdx.GetAt( nLocIdx );
}

/**********************************************************************/

void CLocationCSVArray::SetIDraughtStatus( int nStatus)
{
	switch( nStatus )
	{
	case IDRAUGHT_STATUS_UNKNOWN:
	case IDRAUGHT_STATUS_VALID:
	case IDRAUGHT_STATUS_INVALID_SITE:
	case IDRAUGHT_STATUS_INVALID_TERMINAL:
		m_nIDraughtStatus = nStatus;
		break;
	}
}

/**********************************************************************/

void CLocationCSVArray::UpdateIDraughtStatus()
{
	//CHECK FOR DUPLICATED OR MISSING SITE NAMES
	CReportConsolidationArray<CSortedStringItem> arrayNames;
	for ( int nLocIdx = 0; nLocIdx < GetSize(); nLocIdx++ )
	{
		CString strLabel = GetIDraughtID( nLocIdx );

		if ( strLabel == "" )
		{
			m_nIDraughtStatus = IDRAUGHT_STATUS_INVALID_SITE;
			return;
		}
		else 
		{
			CSortedStringItem item;
			item.m_strItem = strLabel;

			int nIndex;
			if ( arrayNames.Find( item, nIndex ) == FALSE )
				arrayNames.Consolidate( item );
			else
			{
				m_nIDraughtStatus = IDRAUGHT_STATUS_INVALID_SITE;
				return;
			}
		}
	}

	//CHECK FOR DUPLICATED TERMINAL NUMBERS
	CReportConsolidationArray<CSortedStringItem> arrayTerminals;
	for ( int nLocIdx = 0; nLocIdx < GetSize(); nLocIdx++ )
	{
		int nNetworkNo = GetNetworkNo( nLocIdx );

		for ( int x = 0; x < GetTerminalCount( nLocIdx ); x++ )
		{
			CString strLabel;
			strLabel.Format( "%5.5d%5.5d",
				nNetworkNo,
				GetTNo( nLocIdx, x ) );

			CSortedStringItem item;
			item.m_strItem = strLabel;

			int nIndex;
			if ( arrayTerminals.Find( item, nIndex ) == FALSE )
				arrayTerminals.Consolidate( item );
			else
			{
				m_nIDraughtStatus = IDRAUGHT_STATUS_INVALID_TERMINAL;
				return;
			}
		}
	}

	//CHECK FOR MISSING DATABASE LINKS
	for ( int nLocIdx = 0; nLocIdx < GetSize(); nLocIdx++ )
	{
		int nDbNo = dbLocation.GetDbNo( nLocIdx );

		if ( 0 == nDbNo )
		{
			m_nIDraughtStatus = IDRAUGHT_STATUS_INVALID_DBLINK;
			return;
		}

		int nDbIdx;
		if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
		{
			m_nIDraughtStatus = IDRAUGHT_STATUS_INVALID_DBLINK;
			return;
		}
	}

	m_nIDraughtStatus = IDRAUGHT_STATUS_VALID;
}

/**********************************************************************/

bool CLocationCSVArray::CheckPluDownloadRange( int nLocIdx, __int64 nPluNo )
{
	TidyPluDownloadRanges( nLocIdx );

	int nRangeType = GetPluDownloadRangeType( nLocIdx );

	switch( nRangeType )
	{
	case PLUDOWNLOADRANGE_INCLUDE:
	case PLUDOWNLOADRANGE_EXCLUDE:
		break;

	case PLUDOWNLOADRANGE_NONE:
	default:
		return TRUE;
	}

	bool bInRange = FALSE;
	for ( int n = 0; n < PLUDOWNLOADRANGE_COUNT; n++ )
	{
		__int64 nStart = GetPluDownloadRangeStart(nLocIdx,n);
		__int64 nEnd = GetPluDownloadRangeEnd(nLocIdx,n);

		if ( ( nPluNo >= nStart ) && ( nPluNo <= nEnd ) )
		{
			bInRange = TRUE;
			break;
		}
	}
		
	if ( PLUDOWNLOADRANGE_INCLUDE == nRangeType )
		return bInRange;
	else
		return ( FALSE == bInRange );
}

/**********************************************************************/

void CLocationCSVArray::TidyPluDownloadRanges( int nLocIdx )
{
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		pLocation -> TidyPluDownloadRanges();
	}
}

/**********************************************************************/
