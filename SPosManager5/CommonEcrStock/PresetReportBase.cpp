/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
#include "LocationSelectorEntity.h"
#include "LocationSetCSVArray.h"
#include "Password.h"
#include "ReportpointCSVArray.h"
#include "..\SPosStockManager5\SiteCSVArray.h"
#include "..\SPosStockManager5\StockpointCSVArray.h"
/**********************************************************************/
#include "..\CommonEcrStock\PresetReportBase.h"
/**********************************************************************/

CPresetReportInfo::CPresetReportInfo()
{
	Reset();
}

/**********************************************************************/

void CPresetReportInfo::Reset()
{
	m_strName = "";
	m_nEntityNo = 0;
	m_nPresetConLevel = 0;
	m_nPresetEntityNo1 = 0;
	m_nPresetEntityNo2 = 0;
	m_nTerminalIdx = 0;
	m_bPasswordEnable = TRUE;
	m_nPasswordConLevel = 0;
	m_nPasswordEntityNo1 = 0;
	m_nPasswordEntityNo2 = 0;
	m_strOtherFields = "";
}

/**********************************************************************/

const char* CPresetReportInfo::GetListName()
{
	m_strListName = m_strName;
	::TrimSpaces( m_strListName, FALSE );
	if ( m_strListName == "" ) m_strListName.Format( "Preset Report #%d", m_nEntityNo );
	return m_strListName;
}

/**********************************************************************/

void CPresetReportInfo::SetEntityNo( int n )
{
	if ( ( n >= 0 ) && ( n <= 999 ) )
		m_nEntityNo = n;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPresetReportArray::CPresetReportArray(void)
{
	m_strFilename = "";
}

/**********************************************************************/

CPresetReportArray::~CPresetReportArray(void)
{
}

/**********************************************************************/

void CPresetReportArray::PrepareForEditing( CPresetReportArray& Source )
{
	CopyStrings( Source );
	PrepareForEditing();
}

/**********************************************************************/

void CPresetReportArray::PrepareForEditing()
{
	PrepareForEditingInternal();
	ClearStrings();
}

/**********************************************************************/

void CPresetReportArray::CopyStrings( CPresetReportArray& Source )
{
	m_arrayFileStrings.RemoveAt( 0, m_arrayFileStrings.GetSize() );
	for ( int n = 0; n < Source.m_arrayFileStrings.GetSize(); n++ )
		m_arrayFileStrings.Add( Source.m_arrayFileStrings.GetAt(n) );
}

/**********************************************************************/

int CPresetReportArray::GetEntityNo( int nIdx )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayPresets.GetSize() ) )
		return m_arrayPresets[nIdx].GetEntityNo();
	else
		return 0;
}

/**********************************************************************/

const char* CPresetReportArray::GetName( int nIdx )
{
	m_strName = "";
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayPresets.GetSize() ) )
		m_strName = m_arrayPresets[nIdx].GetName();
	
	return m_strName;
}

/**********************************************************************/

void CPresetReportArray::GetEntityNoAndListName( int nIdx, int& nEntityNo, CString& strName )
{
	nEntityNo = GetEntityNo(nIdx);
	strName = GetName(nIdx);
	::TrimSpaces( strName, FALSE );
	if ( strName == "" ) strName.Format( "Preset #%d", nEntityNo );
}

/**********************************************************************/

void CPresetReportArray::SetEntityNo( int nIdx, int nEntityNo )
{
	if ( ( nIdx >= 0 ) && ( nIdx < GetSize() ) )
		m_arrayPresets[nIdx].SetEntityNo( nEntityNo );
}

/**********************************************************************/

void CPresetReportArray::SetName( int nIdx, const char* sz )
{
	if ( ( nIdx >= 0 ) && ( nIdx < GetSize() ) )
		m_arrayPresets[nIdx].SetName( sz );
}

/**********************************************************************/

void CPresetReportArray::SwapItems( int nFirst )
{
	if ( ( nFirst >= 0 ) && ( nFirst < GetSize() - 1 ) )
	{
		CPresetReportInfo TempRecord1 = m_arrayPresets.GetAt( nFirst ); 
		CPresetReportInfo TempRecord2 = m_arrayPresets.GetAt( nFirst + 1 );
		m_arrayPresets.SetAt( nFirst, TempRecord2 );
		m_arrayPresets.SetAt( nFirst + 1, TempRecord1 );
	}
}

/**********************************************************************/

void CPresetReportArray::GetAt( int nIdx, CPresetReportInfo& info )
{
	if ( ( nIdx >= 0 ) && ( nIdx < GetSize() ) )
		info = m_arrayPresets.GetAt( nIdx );
}

/**********************************************************************/

void CPresetReportArray::SetAt( int nIdx, CPresetReportInfo& info )
{
	if ( ( nIdx >= 0 ) && ( nIdx < GetSize() ) )
		m_arrayPresets.SetAt( nIdx, info );
}

/**********************************************************************/

void CPresetReportArray::InsertAt( int nIdx, CPresetReportInfo& info )
{
	if ( ( nIdx >= 0 ) && ( nIdx <= GetSize() ) )
		m_arrayPresets.InsertAt( nIdx, info );
}

/**********************************************************************/

void CPresetReportArray::RemoveAt( int nIdx )
{
	if ( ( nIdx >= 0 ) && ( nIdx < GetSize() ) )
		m_arrayPresets.RemoveAt( nIdx );
}

/**********************************************************************/

void CPresetReportArray::Add( CPresetReportInfo& info )
{
	m_arrayPresets.Add( info );
}

/**********************************************************************/

void CPresetReportArray::GetPresetConLevelInfo( int nPresetIdx, int& nConLevel, int& nEntityNo1, int& nEntityNo2 )
{
	nConLevel = NODE_SYSTEM;
	nEntityNo1 = 0;
	nEntityNo2 = 0;

	if ( ( nPresetIdx >= 0 ) && ( nPresetIdx < GetSize() ) )
	{
		CPresetReportInfo Preset = m_arrayPresets.GetAt( nPresetIdx );
		nConLevel = Preset.GetPresetConLevel();
		nEntityNo1 = Preset.GetPresetEntityNo1();
		nEntityNo2 = Preset.GetPresetEntityNo2();
	}
}

/**********************************************************************/

void CPresetReportArray::SetPasswordConLevelInfo( int nPresetIdx, bool bAllow, int nConLevel, int nEntityNo1, int nEntityNo2 )
{
	if ( ( nPresetIdx >= 0 ) && ( nPresetIdx < GetSize() ) )
	{
		CPresetReportInfo Preset = m_arrayPresets.GetAt( nPresetIdx );
		Preset.SetPasswordEnableFlag( bAllow );
		Preset.SetPasswordConLevel( nConLevel );
		Preset.SetPasswordEntityNo1( nEntityNo1 );
		Preset.SetPasswordEntityNo2( nEntityNo2 );
		m_arrayPresets.SetAt( nPresetIdx, Preset );
	}
}

/**********************************************************************/

void CPresetReportArray::GetLegacyConLevelInfo( int nPresetConLevel, int nLegacyIdx, int& nEntityNo1, int& nEntityNo2 )
{
	switch( nPresetConLevel )
	{
	case NODE_DATABASE:
		nEntityNo1 = dbDatabase.GetDbNo( nLegacyIdx );
		nEntityNo2 = 0;
		break;

	case NODE_LOCATION:
	case NODE_LOCATION_TERMINAL:
		nEntityNo1 = dbLocation.GetNetworkNo( nLegacyIdx );
		nEntityNo2 = dbLocation.GetLocNo( nLegacyIdx );
		break;

	case NODE_REPORTPOINT:
		nEntityNo1 = dbReportpoint.GetDbNo( nLegacyIdx );
		nEntityNo2 = dbReportpoint.GetRpNo( nLegacyIdx );
		break;

	case NODE_LOCATIONSET:
		nEntityNo1 = dbLocationSet.GetDbNo( nLegacyIdx );
		nEntityNo2 = dbLocationSet.GetLsNo( nLegacyIdx );
		break;

#ifdef STOCKMAN_SYSTEM

	case NODE_STOCKPOINT:
		nEntityNo1 = dbStockpoint.GetDbNo( nLegacyIdx );
		nEntityNo2 = dbStockpoint.GetSpNo( nLegacyIdx );
		break;

	case NODE_SITE:
		nEntityNo1 = dbSite.GetDbNo( nLegacyIdx );
		nEntityNo2 = dbSite.GetSiteNo( nLegacyIdx );
		break;

#endif

	case NODE_SYSTEM:
	default:
		nEntityNo1 = 0;
		nEntityNo2 = 0;
		break;
	}
}

/***********************************************************************/

void CPresetReportArray::SetPasswordAccess()
{
	int nPasswordConLevel, nPasswordEntityNo1, nPasswordEntityNo2;

	switch( PasswordArray.GetLocationAccessType() )
	{
	case LOCATION_ACCESS_DB:
		{
			int nDbIdx = PasswordArray.GetLocationAccessDbIdx();
			nPasswordConLevel = NODE_DATABASE;
			nPasswordEntityNo1 = dbDatabase.GetDbNo( nDbIdx );
			nPasswordEntityNo2 = 0;
		}
		break;

	case LOCATION_ACCESS_LOC:
#ifdef STOCKMAN_SYSTEM
		if ( IsStockPreset() == TRUE )
		{
			int nSpIdx = PasswordArray.GetLocationAccessSpIdx();
			nPasswordConLevel = NODE_STOCKPOINT;
			nPasswordEntityNo1 = dbStockpoint.GetDbNo( nSpIdx );
			nPasswordEntityNo2 = dbStockpoint.GetSpNo( nSpIdx );
		}
		else
#endif
		{
			int nLocIdx = PasswordArray.GetLocationAccessLocIdx();
			nPasswordConLevel = NODE_LOCATION;
			nPasswordEntityNo1 = dbLocation.GetDbNo( nLocIdx );
			nPasswordEntityNo2 = dbLocation.GetLocNo( nLocIdx );
		}
		break;

	case LOCATION_ACCESS_NONE:
		nPasswordConLevel = NODE_PLACEHOLDER;
		nPasswordEntityNo1 = 0;
		nPasswordEntityNo2 = 0;
		break;

	case LOCATION_ACCESS_SYSTEM:
	default:
		nPasswordConLevel = NODE_SYSTEM;
		nPasswordEntityNo1 = 0;
		nPasswordEntityNo2 = 0;
		break;
	}

	for ( int nPresetIdx = 0; nPresetIdx < GetSize(); nPresetIdx++ )
	{
		int nPresetConLevel, nPresetEntityNo1, nPresetEntityNo2;
		GetPresetConLevelInfo( nPresetIdx, nPresetConLevel, nPresetEntityNo1, nPresetEntityNo2 );

		if ( ( nPresetConLevel == NODE_SYSTEM ) && ( nPasswordConLevel == NODE_SYSTEM ) )
		{
			SetPasswordConLevelInfo( nPresetIdx, TRUE, NODE_SYSTEM, 0, 0 );
		}
		else if ( ( nPresetConLevel == nPasswordConLevel ) && ( nPresetEntityNo1 == nPasswordEntityNo1 ) && ( nPresetEntityNo2 == nPasswordEntityNo2 ) )
		{
			SetPasswordConLevelInfo( nPresetIdx, TRUE, nPresetConLevel, nPresetEntityNo1, nPresetEntityNo2 );
		}
		else if ( IsContainedEntity( nPresetConLevel, nPresetEntityNo1, nPresetEntityNo2, nPasswordConLevel, nPasswordEntityNo1, nPasswordEntityNo2 ) )
		{
			SetPasswordConLevelInfo( nPresetIdx, TRUE, nPasswordConLevel, nPasswordEntityNo1, nPasswordEntityNo2 );
		}
		else if ( IsContainedEntity( nPasswordConLevel, nPasswordEntityNo1, nPasswordEntityNo2, nPresetConLevel, nPresetEntityNo1, nPresetEntityNo2 ) )
		{
			SetPasswordConLevelInfo( nPresetIdx, TRUE, nPresetConLevel, nPresetEntityNo1, nPresetEntityNo2 );
		}
		else
		{
			SetPasswordConLevelInfo( nPresetIdx, FALSE, NODE_PLACEHOLDER, 0, 0 );
		}
	}
}

/***********************************************************************/

bool CPresetReportArray::IsContainedEntity( int nOuterConLevel, int nOuterEntityNo1, int nOuterEntityNo2, int nInnerConLevel, int nInnerEntityNo1, int nInnerEntityNo2 )
{
	bool bResult = FALSE;

	switch( nOuterConLevel )
	{
	case NODE_SYSTEM:
		switch( nInnerConLevel )
		{
		case NODE_DATABASE:
		case NODE_LOCATIONSET:
		case NODE_REPORTPOINT:
		case NODE_LOCATION:
		case NODE_LOCATION_TERMINAL:
		case NODE_SITE:
		case NODE_STOCKPOINT:
			bResult = TRUE;
			break;
		}
		break;

	case NODE_DATABASE:
		switch( nInnerConLevel )
		{
		case NODE_LOCATION:
		case NODE_LOCATION_TERMINAL:
			{
				int nLocIdx;
				if ( dbLocation.FindLocationByNumber( nInnerEntityNo1, nInnerEntityNo2, nLocIdx ) == TRUE )
					bResult = ( dbLocation.GetDbNo( nLocIdx ) == nOuterEntityNo1 );
			}
			break;

		case NODE_LOCATIONSET:
		case NODE_REPORTPOINT:
		case NODE_SITE:
		case NODE_STOCKPOINT:
			bResult = ( nInnerEntityNo1 == nOuterEntityNo1 );
			break;
		}
		break;

	case NODE_LOCATION:
		switch( nInnerConLevel )
		{
		case NODE_LOCATION_TERMINAL:
			bResult = ( ( nInnerEntityNo1 == nOuterEntityNo1 ) && ( nInnerEntityNo2 == nOuterEntityNo2 ) );
			break;
		}
	}

	return bResult;
}

/***********************************************************************/

int CPresetReportArray::GetNextFreeEntityNo()
{
	bool arrayEntitiesInUse[1000];
	for ( int n = 0; n < 1000; n++ )
		arrayEntitiesInUse[n] = FALSE;
	
	CArray<int,int> arrayPresetsToFix;

	for ( int nIdx = 0; nIdx < GetSize(); nIdx++ )
	{
		int nEntityNo = GetEntityNo( nIdx );
		if ( ( nEntityNo >= 1 ) && ( nEntityNo <= 999 ) )
			arrayEntitiesInUse[nEntityNo] = TRUE;
		else
			arrayPresetsToFix.Add(nIdx);
	}

	int nNextEntityNo = 0;
	
	int nEntityNoToTry = 1;
	for ( int nIdx = 0; nIdx <= arrayPresetsToFix.GetSize(); nIdx++ )
	{
		bool bFound = FALSE;
		while ( ( nEntityNoToTry < 1000 ) && ( FALSE == bFound ) )
		{
			if ( arrayEntitiesInUse[ nEntityNoToTry ] == FALSE )
			{
				if ( nIdx < arrayPresetsToFix.GetSize() )
				{
					int nPresetIdx = arrayPresetsToFix.GetAt( nIdx );
					SetEntityNo( nPresetIdx, nEntityNoToTry++ );
					bFound = TRUE;
				}
				else
				{
					nNextEntityNo = nEntityNoToTry;
					bFound = TRUE;
				}
			}
			else
			{ 
				nEntityNoToTry++;
			}
		}
	}
		
	return nNextEntityNo;
}

/***********************************************************************/

void CPresetReportArray::GetLocSelEntityNo( CLocationSelectorEntity& LocSelEntity, int& nEntityNo1, int& nEntityNo2 )
{
	switch( LocSelEntity.GetConType() )
	{
	case NODE_DATABASE:
		nEntityNo1 = LocSelEntity.GetDbNo();
		nEntityNo2 = 0;
		break;

	case NODE_LOCATION:
	case NODE_LOCATION_TERMINAL:
		nEntityNo1 = LocSelEntity.GetNwkNo();
		nEntityNo2 = LocSelEntity.GetEntityNo();
		break;

	case NODE_REPORTPOINT:
	case NODE_LOCATIONSET:
	case NODE_STOCKPOINT:
	case NODE_SITE:
		nEntityNo1 = LocSelEntity.GetDbNo();
		nEntityNo2 = LocSelEntity.GetEntityNo();
		break;

	case NODE_SYSTEM:
	default:
		nEntityNo1 = 0;
		nEntityNo1 = 0;
		break;
	}
}
	
/***********************************************************************/

void CPresetReportArray::GetEntityIdx( int nConLevel, int nEntityNo1, int nEntityNo2, int& nEntityIdx )
{
	switch( nConLevel )
	{
	case NODE_LOCATION:
	case NODE_LOCATION_TERMINAL:
		dbLocation.FindLocationByNumber( nEntityNo1, nEntityNo2, nEntityIdx );
		break;

	case NODE_REPORTPOINT:
		dbReportpoint.FindReportpointByNumber( nEntityNo1, nEntityNo2, nEntityIdx );
		break;

	case NODE_LOCATIONSET:
		dbLocationSet.FindLocationSetByNumber( nEntityNo1, nEntityNo2, nEntityIdx );
		break;

	case NODE_DATABASE:
		dbDatabase.FindDatabaseByNumber( nEntityNo1, nEntityIdx );
		break;

#ifdef STOCKMAN_SYSTEM

	case NODE_STOCKPOINT:
		dbStockpoint.FindStockpointByNumber( nEntityNo1, nEntityNo2, nEntityIdx );
		break;

	case NODE_SITE:
		dbSite.FindSiteByNumber( nEntityNo1, nEntityNo2, nEntityIdx );
		break;

#endif

	}
}

/***********************************************************************/
