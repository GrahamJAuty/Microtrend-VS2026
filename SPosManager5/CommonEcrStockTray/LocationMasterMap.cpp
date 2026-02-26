/**********************************************************************/
 
/**********************************************************************/
//#include "globalfunctions.h"
#include "LocationCSVArray.h"
#include "MaxLengths.h"
/**********************************************************************/
#include "LocationMasterMap.h"
/**********************************************************************/

CLocationMasterInfo::CLocationMasterInfo()
{
	Reset();
}

/**********************************************************************/

void CLocationMasterInfo::Reset()
{
	m_nNwkNo = 0;
	m_nLocNo = 0;
	m_nTerminalNo = 0;
	m_strTerminalName = "";
	m_bMaster = FALSE;
	m_strMasterName = "";
	m_strImmediateFolder = "";
	m_strDeferredFolder = "";
	m_bCashRSP = FALSE;
	m_strCashRSPFolder = "";
}

/**********************************************************************/

void CLocationMasterInfo::Add ( CLocationMasterInfo& source )
{
}

/**********************************************************************/

int CLocationMasterInfo::Compare( CLocationMasterInfo& source, int nHint )
{
	if ( m_nNwkNo != source.m_nNwkNo )
		return ( ( m_nNwkNo > source.m_nNwkNo ) ? 1 : -1 );

	if ( m_nLocNo != source.m_nLocNo )
		return ( ( m_nLocNo > source.m_nLocNo ) ? 1 : -1 );

	if ( m_nTerminalNo != source.m_nTerminalNo )
		return ( ( m_nTerminalNo > source.m_nTerminalNo ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

void CLocationMasterInfo::ValidateSettings()
{
	if ( TRUE == m_bMaster )
	{
		::TrimSpaces( m_strMasterName, FALSE );
		if ( m_strMasterName == "" )
			m_bMaster = FALSE;

		::TrimSpaces( m_strDeferredFolder, FALSE );
		if ( m_strDeferredFolder == "" )
			m_bMaster = FALSE;

		::TrimSpaces( m_strImmediateFolder, FALSE );
		if ( m_strImmediateFolder == "" )
			m_bMaster = FALSE;
	}

	if ( TRUE == m_bMaster )
	{
		m_strMasterName = m_strMasterName.Left( MAX_LENGTH_GENERAL_NAME );

		if ( ( m_strDeferredFolder != "" ) && ( m_strDeferredFolder.Right(1) != "\\" ) )
			m_strDeferredFolder += "\\";

		if ( ( m_strImmediateFolder != "" ) && ( m_strImmediateFolder.Right(1) != "\\" ) )
			m_strImmediateFolder += "\\";
	}
	else
	{
		m_strDeferredFolder = "";
		m_strImmediateFolder = "";
	}

	if ( TRUE == m_bCashRSP )
	{
		::TrimSpaces( m_strCashRSPFolder, FALSE );
		if ( m_strCashRSPFolder == "" )
			m_bCashRSP = FALSE;
	}
	if ( TRUE == m_bCashRSP )
	{
		if ( ( m_strCashRSPFolder != "" ) && ( m_strCashRSPFolder.Right(1) != "\\" ) )
			m_strCashRSPFolder += "\\";
	}
	else
	{
		m_strCashRSPFolder = "";
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CLocationMasterMap::CLocationMasterMap(void)
{
}

/**********************************************************************/

void CLocationMasterMap::Reset()
{
	m_arrayLocFlags.RemoveAll();
	m_arrayMasters.RemoveAll();

}

/**********************************************************************/

bool CLocationMasterMap::IsLocationLoaded( int nLocIdx )
{
	bool bResult = TRUE;
	if ( ( nLocIdx >= 0 ) && ( nLocIdx < dbLocation.GetSize() ) )
	{
		CSortedIntAndInt Test;
		Test.m_nInt1 = dbLocation.GetNetworkNo( nLocIdx );
		Test.m_nInt2 = dbLocation.GetLocNo( nLocIdx );

		int nPos;
		bResult = m_arrayLocFlags.Find( Test, nPos );
	}
	return bResult;
}

/**********************************************************************/

void CLocationMasterMap::LoadLocation( int nLocIdx )
{
	if ( ( nLocIdx >= 0 ) && ( nLocIdx < dbLocation.GetSize() ) )
	{
		CSortedIntAndInt Test;
		Test.m_nInt1 = dbLocation.GetNetworkNo( nLocIdx );
		Test.m_nInt2 = dbLocation.GetLocNo( nLocIdx );
		
		int nPos;
		if ( m_arrayLocFlags.Find( Test, nPos ) == FALSE )
			m_arrayLocFlags.InsertAt( nPos, Test );

		CSSFile fileInfo;
		CFilenameUpdater FnUp( SysFiles::ExtraCommsInFolders, nLocIdx );
		if ( fileInfo.Open( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
		{
			CString strBuffer;
			while ( fileInfo.ReadString( strBuffer ) == TRUE )
			{
				CCSV csv( strBuffer );
				
				CLocationMasterInfo infoMaster;
				infoMaster.SetNwkNo( dbLocation.GetNetworkNo( nLocIdx ) );
				infoMaster.SetLocNo( dbLocation.GetLocNo( nLocIdx ) );

				bool bGotInfo = FALSE;

				CString strTest = csv.GetString(0);
				if ( strTest != "#" )
				{
					infoMaster.SetTerminalNo( csv.GetInt(0) );
					infoMaster.SetTerminalName( "" );
					infoMaster.SetMasterFlag( TRUE );
					infoMaster.SetMasterName( csv.GetString(1) );
					infoMaster.SetDeferredFolder( csv.GetString(2) );
					infoMaster.SetImmediateFolder( csv.GetString(3) );
					infoMaster.SetCashRSPFlag( FALSE );
					infoMaster.SetCashRSPFolder( "" );
					bGotInfo = TRUE;
				}
				else
				{
					switch( csv.GetInt(1) )
					{
					case 1:
						infoMaster.SetTerminalNo( csv.GetInt(2) );
						infoMaster.SetTerminalName( csv.GetString(3) );
						infoMaster.SetMasterFlag( csv.GetBool(4) );
						infoMaster.SetMasterName( csv.GetString(5) );
						infoMaster.SetDeferredFolder( csv.GetString(6) );
						infoMaster.SetImmediateFolder( csv.GetString(7) );
						infoMaster.SetCashRSPFlag( csv.GetBool(8) );
						infoMaster.SetCashRSPFolder( csv.GetString(9) );
						bGotInfo = TRUE;
						break;
					}
				}

				if ( TRUE == bGotInfo )
				{
					infoMaster.ValidateSettings();
					AddMaster( nLocIdx, infoMaster, TRUE );
				}
			}
		}
	}
}

/**********************************************************************/

bool CLocationMasterMap::GetMasterInfo( int nLocIdx, int nTerminalNo, CLocationMasterInfo& infoMaster )
{
	bool bResult = FALSE;

	if ( IsLocationLoaded( nLocIdx ) == FALSE )
		LoadLocation( nLocIdx );

	infoMaster.Reset();
	
	if ( ( nLocIdx >= 0 ) && ( nLocIdx < dbLocation.GetSize() ) )
	{
		infoMaster.SetNwkNo( dbLocation.GetNetworkNo( nLocIdx ) );
		infoMaster.SetLocNo( dbLocation.GetLocNo( nLocIdx ) );
		infoMaster.SetTerminalNo( nTerminalNo );

		if ( 0 == nTerminalNo )
		{
			infoMaster.SetMasterFlag( TRUE );
			infoMaster.SetMasterName( dbLocation.GetName( nLocIdx ) );
			
			CString strPath;
			dbLocation.GetSafeDownloadPath( nLocIdx, strPath );
			infoMaster.SetDeferredFolder( strPath );

			dbLocation.GetSafeDownloadNowPath( nLocIdx, strPath );
			infoMaster.SetImmediateFolder( strPath );

			bResult = TRUE;
		}
		else
		{
			int nPos;
			if ( m_arrayMasters.Find( infoMaster, nPos ) == TRUE )
			{
				m_arrayMasters.GetAt( nPos, infoMaster );
				infoMaster.ValidateSettings();
				bResult = TRUE;
			}
		}
	}

	return bResult;
}

/**********************************************************************/

const char* CLocationMasterMap::GetMasterName( int nLocIdx, int nTerminalNo )
{
	CLocationMasterInfo infoMaster;
	GetMasterInfo( nLocIdx, nTerminalNo, infoMaster );
	m_strMasterName = infoMaster.GetMasterName();
	return m_strMasterName;
}

/**********************************************************************/

const char* CLocationMasterMap::GetDownloadFolder( int nLocIdx, int nTerminalNo, bool bImmediate )
{
	CLocationMasterInfo infoMaster;
	GetMasterInfo( nLocIdx, nTerminalNo, infoMaster );
	m_strDownloadFolder = bImmediate ? infoMaster.GetImmediateFolder() : infoMaster.GetDeferredFolder();
	return m_strDownloadFolder;
}

/**********************************************************************/

const char* CLocationMasterMap::GetTerminalName( int nLocIdx, int nTerminalNo )
{
	CLocationMasterInfo infoMaster;
	GetMasterInfo( nLocIdx, nTerminalNo, infoMaster );
	m_strTerminalName = infoMaster.GetTerminalName();

	::TrimSpaces( m_strTerminalName, FALSE );
	if ( m_strTerminalName == "" )
		m_strTerminalName.Format( "Terminal %d", infoMaster.GetTerminalNo() );

	return m_strTerminalName;
}

/**********************************************************************/

const char* CLocationMasterMap::GetCashRSPFolder( int nLocIdx, int nTerminalNo )
{
	CLocationMasterInfo infoMaster;
	GetMasterInfo( nLocIdx, nTerminalNo, infoMaster );
	m_strCashRSPFolder = infoMaster.GetCashRSPFolder();
	return m_strCashRSPFolder;
}

/**********************************************************************/

void CLocationMasterMap::GetLocationIdxRange( int nLocIdx, int& nStartIdx, int& nEndIdx )
{
	CLocationMasterInfo infoMaster;
	infoMaster.SetNwkNo( dbLocation.GetNetworkNo( nLocIdx ) );
	infoMaster.SetLocNo( dbLocation.GetLocNo( nLocIdx ) );
	infoMaster.SetTerminalNo( 0 );
		
	m_arrayMasters.Find( infoMaster, nStartIdx );

	infoMaster.SetLocNo( infoMaster.GetLocNo() + 1 );
	m_arrayMasters.Find( infoMaster, nEndIdx );
	nEndIdx--;
}

/**********************************************************************/

void CLocationMasterMap::GetLocationMasterList( int nLocIdx, CArray<CLocationMasterInfo,CLocationMasterInfo>& arrayMasters, bool bMasterOnly, bool bCashRSPOnly )
{
	arrayMasters.RemoveAll();

	if ( IsLocationLoaded( nLocIdx ) == FALSE )
		LoadLocation( nLocIdx );
	
	if ( ( nLocIdx >= 0 ) && ( nLocIdx < dbLocation.GetSize() ) )
	{
		if ( FALSE == bCashRSPOnly )
		{
			CLocationMasterInfo infoMaster;
			infoMaster.SetNwkNo( dbLocation.GetNetworkNo( nLocIdx ) );
			infoMaster.SetLocNo( dbLocation.GetLocNo( nLocIdx ) );
			infoMaster.SetTerminalNo( 0 );
			infoMaster.SetMasterName( dbLocation.GetName( nLocIdx ) );
			infoMaster.SetMasterFlag( TRUE );

			CString strPath;
			dbLocation.GetSafeDownloadPath( nLocIdx, strPath );
			infoMaster.SetDeferredFolder( strPath );

			dbLocation.GetSafeDownloadNowPath( nLocIdx, strPath );
			infoMaster.SetImmediateFolder( strPath );

			arrayMasters.Add( infoMaster );
		}

		int nStartIdx, nEndIdx;
		GetLocationIdxRange( nLocIdx, nStartIdx, nEndIdx );

		for ( int nIdx = nStartIdx; nIdx <= nEndIdx; nIdx++ )
		{
			CLocationMasterInfo infoMaster;
			m_arrayMasters.GetAt( nIdx, infoMaster );

			infoMaster.ValidateSettings();

			if ( ( TRUE == bMasterOnly ) && ( FALSE == infoMaster.GetMasterFlag() ) )
				continue;

			if ( ( TRUE == bCashRSPOnly ) && ( FALSE == infoMaster.GetCashRSPFlag() ) )
				continue;
	
			arrayMasters.Add( infoMaster );
		}
	}
}

/**********************************************************************/

void CLocationMasterMap::SetLocationMasterList( int nLocIdx, CArray<CLocationMasterInfo,CLocationMasterInfo>& arrayMasters )
{
	if ( ( nLocIdx >= 0 ) && ( nLocIdx < dbLocation.GetSize() ) )
	{
		int nStartIdx, nEndIdx;
		GetLocationIdxRange( nLocIdx, nStartIdx, nEndIdx );

		for ( int nIdx = nEndIdx; nIdx >= nStartIdx; nIdx-- )
			m_arrayMasters.RemoveAt( nIdx );

		for ( int nIdx = 0; nIdx < arrayMasters.GetSize(); nIdx++ )
		{
			CLocationMasterInfo infoMaster = arrayMasters.GetAt( nIdx );
			AddMaster( nLocIdx, infoMaster, TRUE );
		}

		CSortedIntAndInt Test;
		Test.m_nInt1 = dbLocation.GetNetworkNo( nLocIdx );
		Test.m_nInt2 = dbLocation.GetLocNo( nLocIdx );
		
		int nPos;
		if ( m_arrayLocFlags.Find( Test, nPos ) == FALSE )
			m_arrayLocFlags.InsertAt( nPos, Test );
	}
}

/**********************************************************************/

void CLocationMasterMap::DeleteLocation( int nLocIdx )
{
	if ( ( nLocIdx >= 0 ) && ( nLocIdx < dbLocation.GetSize() ) )
	{
		int nStartIdx, nEndIdx;
		GetLocationIdxRange( nLocIdx, nStartIdx, nEndIdx );

		for ( int nIdx = nEndIdx; nIdx >= nStartIdx; nIdx-- )
			m_arrayMasters.RemoveAt( nIdx );

		CSortedIntAndInt Test;
		Test.m_nInt1 = dbLocation.GetNetworkNo( nLocIdx );
		Test.m_nInt2 = dbLocation.GetLocNo( nLocIdx );
		
		int nPos;
		if ( m_arrayLocFlags.Find( Test, nPos ) == TRUE )
			m_arrayLocFlags.RemoveAt( nPos );
	}
}

/**********************************************************************/

void CLocationMasterMap::AddMaster( int nLocIdx, CLocationMasterInfo& infoMaster, bool bCheckTNo )
{
	if ( infoMaster.GetTerminalNo() <= 0 )
		return;

	if ( TRUE == bCheckTNo )
	{
		int nTerminalIdx;
		if ( dbLocation.FindTerminalByNumber( nLocIdx, infoMaster.GetTerminalNo(), nTerminalIdx ) == FALSE )
			return;
	}

	if ( infoMaster.GetNwkNo() != dbLocation.GetNetworkNo( nLocIdx ) )
		return;

	if ( infoMaster.GetLocNo() != dbLocation.GetLocNo( nLocIdx ) )
		return;

	int nInsertPos;
	if ( m_arrayMasters.Find ( infoMaster, nInsertPos ) == TRUE )
		return;

	infoMaster.ValidateSettings();
	m_arrayMasters.InsertAt( nInsertPos, infoMaster );
}

/**********************************************************************/

void CLocationMasterMap::RemoveMaster( CLocationMasterInfo& infoMaster )
{
	int nMasterIdx;
	if ( m_arrayMasters.Find( infoMaster, nMasterIdx ) == TRUE )
		m_arrayMasters.RemoveAt( nMasterIdx );
}

/**********************************************************************/

void CLocationMasterMap::WriteLocationMasterFile( int nLocIdx )
{
	if ( ( nLocIdx >= 0 ) && ( nLocIdx < dbLocation.GetSize() ) )
	{
		int nStartIdx, nEndIdx;
		GetLocationIdxRange( nLocIdx, nStartIdx, nEndIdx );

		CSSFile fileMasters;
		CFilenameUpdater FnUp( SysFiles::ExtraCommsInFolders, nLocIdx );
		if ( fileMasters.Open( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
		{
			for( int nIdx = nStartIdx; nIdx <= nEndIdx; nIdx++ )
			{
				CLocationMasterInfo infoMaster;
				m_arrayMasters.GetAt( nIdx, infoMaster );
				infoMaster.ValidateSettings();

				CCSV csv;
				csv.Add( "#" );
				csv.Add( 1 );
				csv.Add( infoMaster.GetTerminalNo() );
				csv.Add( infoMaster.GetTerminalName() );
				csv.Add( infoMaster.GetMasterFlag() );
				csv.Add( infoMaster.GetMasterName() );
				csv.Add( infoMaster.GetDeferredFolder() );
				csv.Add( infoMaster.GetImmediateFolder() );
				csv.Add( infoMaster.GetCashRSPFlag() );
				csv.Add( infoMaster.GetCashRSPFolder() );
				fileMasters.WriteLine( csv.GetLine() );
			}
		}
	}
}

/**********************************************************************/
