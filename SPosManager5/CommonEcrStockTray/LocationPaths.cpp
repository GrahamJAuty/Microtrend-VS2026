/**********************************************************************/
#include "DatabaseCSVArray.h"
/**********************************************************************/
#include "LocationCSVArray.h"
/**********************************************************************/

const char* CLocationCSVArray::GetFolderPathData( int nLocIdx )
{
	m_strFolderPath = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CLocationCSVRecord* pLocation = GetLocation ( nLocIdx );
		m_strFolderPath.Format ( "NetWk%3.3d\\Loc%3.3d", 
			pLocation -> m_nNetworkNo, 
			pLocation -> m_nLocNo );

		GetDataProgramPath( m_strFolderPath );
	}
	return m_strFolderPath;	
}

/**********************************************************************/

const char* CLocationCSVArray::GetFolderPathDataDelete( int nLocIdx )
{
	m_strFolderPathDelete = GetFolderPathData( nLocIdx );
	m_strFolderPathDelete += "XXX";
	return m_strFolderPathDelete;	
}

/**********************************************************************/

const char* CLocationCSVArray::GetFolderPathDataKeyboard( int nLocIdx, int nKbNo )
{
	m_strFolderPath = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		m_strFolderPath = GetFolderPathData( nLocIdx );

		CString strExtra;
		strExtra.Format ( "\\V3Kbd%3.3d", nKbNo );
		m_strFolderPath += strExtra;
	}

	return m_strFolderPath;
}

/**********************************************************************/

const char* CLocationCSVArray::GetFolderPathSyssetPendingProductsLocal( int nLocIdx )
{
	m_strFolderPath = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		m_strFolderPath = GetFolderPathSysset( nLocIdx );
		m_strFolderPath += "\\Pending1";
	}

	return m_strFolderPath;
}

/**********************************************************************/

const char* CLocationCSVArray::GetFolderPathSyssetPendingProductsRemote( int nLocIdx )
{
	m_strFolderPath = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		m_strFolderPath = GetFolderPathSysset( nLocIdx );
		m_strFolderPath += "\\Pending2";
	}

	return m_strFolderPath;
}

/**********************************************************************/

const char* CLocationCSVArray::GetFolderPathSyssetPendingCustomerLocal( int nLocIdx )
{
	m_strFolderPath = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		m_strFolderPath = GetFolderPathSysset( nLocIdx );
		m_strFolderPath += "\\Pending3";
	}

	return m_strFolderPath;
}

/**********************************************************************/

const char* CLocationCSVArray::GetFolderPathSyssetPendingCustomerRemote( int nLocIdx )
{
	m_strFolderPath = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		m_strFolderPath = GetFolderPathSysset( nLocIdx );
		m_strFolderPath += "\\Pending4";
	}

	return m_strFolderPath;
}

/**********************************************************************/

const char* CLocationCSVArray::GetFilePathDataLocalPluExclude( int nLocIdx )
{
	m_strFilePathLocalPluExclude = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CFilenameUpdater FnUp( SysFiles::PriceSetExclude, nLocIdx );
		m_strFilePathLocalPluExclude = FnUp.GetFilenameToUse();
	}
	return m_strFilePathLocalPluExclude;
}

/**********************************************************************/

const char* CLocationCSVArray::GetFilePathDataLocalPluExcludeTemp( int nLocIdx )
{
	m_strFilePathLocalPluExcludeTemp = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CFilenameUpdater FnUp( SysFiles::PriceSetExcludeTemp, nLocIdx );
		m_strFilePathLocalPluExcludeTemp = FnUp.GetFilenameToUse();
	}
	return m_strFilePathLocalPluExcludeTemp;
}

/**********************************************************************/

const char* CLocationCSVArray::GetFilePathDataLocalPluInclude( int nLocIdx )
{
	m_strFilePathLocalPluInclude = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CFilenameUpdater FnUp( SysFiles::PriceSetInclude, nLocIdx );
		m_strFilePathLocalPluInclude = FnUp.GetFilenameToUse();
	}
	return m_strFilePathLocalPluInclude;
}

/**********************************************************************/

const char* CLocationCSVArray::GetFilePathDataLocalPluIncludeTemp( int nLocIdx )
{
	m_strFilePathLocalPluIncludeTemp = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CFilenameUpdater FnUp( SysFiles::PriceSetIncludeTemp, nLocIdx );
		m_strFilePathLocalPluIncludeTemp = FnUp.GetFilenameToUse();
	}
	return m_strFilePathLocalPluIncludeTemp;
}

/**********************************************************************/

const char* CLocationCSVArray::GetFilePathDataLocalPluField( int nLocIdx, int nSet, bool bForceLocSpecific )
{
	m_strFilePathLocalPluField = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		int nDbPriceSet = 0;
		switch( nSet )
		{
		case 1:	nDbPriceSet = GetDbPriceSet1( nLocIdx );	break;
		case 2:	nDbPriceSet = GetDbPriceSet2( nLocIdx );	break;
		case 3:	nDbPriceSet = GetDbPriceSet3( nLocIdx );	break;
		}
			
		if ( ( nDbPriceSet == 0 ) || ( bForceLocSpecific == TRUE ) )
		{
			CFilenameUpdater FnUp( SysFiles::LocPriceSetFields, nLocIdx, nSet );
			m_strFilePathLocalPluField = FnUp.GetFilenameToUse();
		}
		else
		{
			int nDbIdx;
			int nDbNo = GetDbNo( nLocIdx );
			if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == TRUE )
			{
				CFilenameUpdater FnUp( SysFiles::DbPriceSetFields, nDbIdx, nDbPriceSet );
				m_strFilePathLocalPluField = FnUp.GetFilenameToUse();
			}
		}
	}
	return m_strFilePathLocalPluField;
}

/**********************************************************************/

const char* CLocationCSVArray::GetFilePathDataLocalPluFieldTemp( int nLocIdx, int nSet, bool bForceLocSpecific )
{
	m_strFilePathLocalPluFieldTemp = "";
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		int nDbPriceSet = 0;
		switch( nSet )
		{
		case 1:	nDbPriceSet = GetDbPriceSet1( nLocIdx );	break;
		case 2:	nDbPriceSet = GetDbPriceSet2( nLocIdx );	break;
		case 3:	nDbPriceSet = GetDbPriceSet3( nLocIdx );	break;
		}
			
		if ( ( nDbPriceSet == 0 ) || ( bForceLocSpecific == TRUE ) )
		{
			CFilenameUpdater FnUp( SysFiles::LocPriceSetTemp, nLocIdx, nSet );
			m_strFilePathLocalPluFieldTemp = FnUp.GetFilenameToUse();
		}	
		else
		{
			int nDbIdx;
			int nDbNo = GetDbNo( nLocIdx );
			if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == TRUE )
			{
				CFilenameUpdater FnUp( SysFiles::DbPriceSetTemp, nDbIdx, nSet );
				m_strFilePathLocalPluFieldTemp = FnUp.GetFilenameToUse();
			}
		}
	}
	return m_strFilePathLocalPluFieldTemp;
}

/**********************************************************************/

const char* CLocationCSVArray::GetFilePathSyssetPluEcr( int nLocIdx )
{
	m_strFilePathPluEcr = "";
	
	if ( nLocIdx >= 0 && nLocIdx < GetSize() )
	{
		CFilenameUpdater FnUp( SysFiles::LocPluEcr, nLocIdx );
		m_strFilePathPluEcr = FnUp.GetFilenameToUse();
	}

	return m_strFilePathPluEcr;
}

/**********************************************************************/