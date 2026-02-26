/**********************************************************************/
 
/**********************************************************************/
//#include "EcrmanOptionsIni.h"
#include "SPOSVersions.h"
#include "KeyboardSetCSVArray.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "Password.h"
/**********************************************************************/
#include "KeyboardMap.h"
/**********************************************************************/

CKeyboardMapInfo::CKeyboardMapInfo()
{
	m_nEntryType = KBD_MAP_LOC;
	m_nSPOSVersion = 0;
	m_nDbIdx = 0;
	m_nLocIdx = 0;
	m_nKbsIdx = 0;
}

/**********************************************************************/

/*
bool CKeyboardMapInfo::Compare( CKeyboardMapInfo& test )
{
	if ( m_nEntryType != test.m_nEntryType ) return FALSE;
	if ( m_nSPOSVersion != test.m_nSPOSVersion ) return FALSE;
	if ( m_nDbIdx != test.m_nDbIdx ) return FALSE;
	if ( m_nLocIdx != test.m_nLocIdx ) return FALSE;
	if ( m_nKbsIdx != test.m_nKbsIdx ) return FALSE;
	return TRUE;
}
*/

/**********************************************************************/

CKeyboardMap::CKeyboardMap()
{
	ClearMap();
}

/**********************************************************************/

void CKeyboardMap::ClearMap()
{
	m_arrayLocInfo.RemoveAll();
	m_arrayMapInfo.RemoveAll();
	m_arrayKbSetInfo.RemoveAll();
	m_bIsBuilt = FALSE;
}

/**********************************************************************/

void CKeyboardMap::BuildMap()
{
	if ( FALSE == m_bIsBuilt )
	{
		//INITIALISE MAP OF LOCATIONS
		int nDummy = FALSE;
		for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
		{
			CKeyboardMapInfo info;
			info.m_nEntryType = KBD_MAP_NULL;
			info.m_nDbIdx = 0;
			info.m_nLocIdx = nLocIdx;
			info.m_nKbsIdx = 0;
			dbLocation.GetLocSPOSVersion( nLocIdx, info.m_nSPOSVersion, nDummy );
			m_arrayLocInfo.Add( info );
		}

		//INITIALISE MAP OF KEYBOARD SETS
		for ( int nKbsIdx = 0; nKbsIdx < dbKeyboardSet.GetSize(); nKbsIdx++ )
			m_arrayKbSetInfo.Add( -1 );

		if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
		{
			for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
				BuildMapDatabase( nDbIdx );
		}
		else
		{
			CWordArray arrayDbIdx;
			dbDatabase.SortByName( arrayDbIdx );

			for ( int n = 0; n < arrayDbIdx.GetSize(); n++ )
			{
				int nDbIdx = arrayDbIdx.GetAt(n);
				BuildMapDatabase( nDbIdx );
			}
		}

		m_bIsBuilt = TRUE;
	}
}

/**********************************************************************/

void CKeyboardMap::BuildMapDatabase( int nDbIdx )
{
	if ( PasswordArray.CheckLocationAccessDbIdx( nDbIdx ) == FALSE )
		return;
	
	CArray<int,int> Set;
	dbLocation.GetDatabaseLocIdxSet ( nDbIdx, Set );

	if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
		dbLocation.SortLocListByName( Set );

	//IGNORE LOCATIONS THAT ARE PASSWORD PROTECTED OR SPORTS BOOKER / SMART ENTERTAINMENT
	for ( int nIndex = Set.GetSize() - 1; nIndex >= 0; nIndex-- )
	{
		int nLocIdx = Set.GetAt( nIndex );
		
		if ( PasswordArray.CheckLocationAccessLocIdx( nLocIdx ) == FALSE )
			Set.RemoveAt( nIndex );
		else if ( dbLocation.IsWebSalesLocation( nLocIdx ) == TRUE )
			Set.RemoveAt( nIndex );
	}

	//IDENTIFY LOCATIONS THAT ARE IN A KEYBOARD SET
	for ( int nIndex = 0; nIndex < Set.GetSize(); nIndex++ )
	{
		int nLocIdx = Set.GetAt( nIndex );

		int nKbsIdx;
		int nDbNo = dbDatabase.GetDbNo( nDbIdx );
		int nKbSet = dbLocation.GetDbKeyboardSet( nLocIdx );
	
		if ( ( nKbSet < 1 ) || ( nKbSet > MAX_KEYBOARD_SETS_PER_DATABASE ) )
			continue;

		//FIND THE KEYBOARD SET FOR THIS LOCATION
		if ( dbKeyboardSet.FindKeyboardSetByNumber( nDbNo, nKbSet, nKbsIdx ) == FALSE )
			continue;
		
		int nSPOSVersionForLoc;
		int nDummy;
		dbLocation.GetLocSPOSVersion( nLocIdx, nSPOSVersionForLoc, nDummy );

		int nSPOSVersionForSet = m_arrayKbSetInfo[nKbsIdx];

		if ( nSPOSVersionForSet == -1 )
			nSPOSVersionForSet = nSPOSVersionForLoc;
		else
		{
			int nBaseSPOSVersionSet = CSPOSVersions::GetSPOSBaseVersion( nSPOSVersionForSet );
			int nBaseSPOSVersionLoc = CSPOSVersions::GetSPOSBaseVersion( nSPOSVersionForLoc );

			if ( ( nBaseSPOSVersionSet == nBaseSPOSVersionLoc ) && ( nBaseSPOSVersionSet != -1 ) )
			{
				if ( CSPOSVersions::GetSPOSFeatureNumber( nSPOSVersionForLoc ) < CSPOSVersions::GetSPOSFeatureNumber( nSPOSVersionForSet ) )
					nSPOSVersionForSet = nSPOSVersionForLoc;
			}
			else
				nSPOSVersionForSet = -1;
		}

		//ALLOW LOCATION INTO KEYBOARD SET IF FROM SAME HARDWARE FAMILY
		if ( nSPOSVersionForSet != -1 )
		{
			//SET SPOS VERSION FOR THIS KEYBOARD SET TO LOWEST SO FAR
			m_arrayKbSetInfo[nKbsIdx] = nSPOSVersionForSet;
			
			//REMOVE LOCATION FROM DATABASE SET
			Set.RemoveAt( nIndex-- );

			//MAP LOCATION AS LINKED TO KEYBOARD SET
			CKeyboardMapInfo info;
			info.m_nEntryType = KBD_MAP_SET;
			info.m_nSPOSVersion = nSPOSVersionForLoc;
			info.m_nDbIdx = nDbIdx;
			info.m_nLocIdx = nLocIdx;
			info.m_nKbsIdx = nKbsIdx;
			m_arrayLocInfo.SetAt( nLocIdx, info );
		}
	}

	int nStartKbsIdx, nEndKbsIdx;
	dbKeyboardSet.GetKbsIdxRange( nDbIdx, nStartKbsIdx, nEndKbsIdx );
	
	//ADD KEYBOARD SETS TO MAP
	for ( int nKbsIdx = nStartKbsIdx; nKbsIdx <= nEndKbsIdx; nKbsIdx++ )
	{
		if ( m_arrayKbSetInfo[ nKbsIdx ] != -1 )
		{
			CKeyboardMapInfo info;
			info.m_nEntryType = KBD_MAP_SET;
			info.m_nDbIdx = nDbIdx;
			info.m_nLocIdx = 0;
			info.m_nKbsIdx = nKbsIdx;
			info.m_nSPOSVersion = m_arrayKbSetInfo[ nKbsIdx ];
			m_arrayMapInfo.Add( info );
		}
	}

	//ADD LOCATION KEYBOARDS TO MAP
	int nDummy = FALSE;
	for ( int nIndex = 0; nIndex < Set.GetSize(); nIndex++ )
	{
		int nLocIdx = Set.GetAt( nIndex );

		CKeyboardMapInfo info;
		info.m_nEntryType = KBD_MAP_LOC;
		info.m_nDbIdx = nDbIdx;
		info.m_nLocIdx = nLocIdx;
		info.m_nKbsIdx = 0;
		dbLocation.GetLocSPOSVersion( nLocIdx, info.m_nSPOSVersion, nDummy );
		m_arrayMapInfo.Add( info );

		//MARK LOCATION AS NOT LINKED TO KEYBOARD SET
		m_arrayLocInfo.SetAt( nLocIdx, info );
	}
}

/**********************************************************************/

void CKeyboardMap::GetMapInfo( int nIdx, CKeyboardMapInfo& info )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayMapInfo.GetSize() ) )
		info = m_arrayMapInfo[ nIdx ];
}

/**********************************************************************/

void CKeyboardMap::GetLocInfo( int nIdx, CKeyboardMapInfo& info )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayLocInfo.GetSize() ) )
		info = m_arrayLocInfo[ nIdx ];
}

/**********************************************************************/
