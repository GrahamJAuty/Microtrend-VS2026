/**********************************************************************/
#include "FileRemover.h"
#include "LocationCSVArray.h"
#include "MaxLengths.h"
#include "MessageLogger.h"
#include "Password.h"
#include "SortedLocation.h"
#include "SPOSVersions.h"
#include "SysInfo.h"
/**********************************************************************/
#include "NetworkCSVArray.h"
/**********************************************************************/

CNetworkCSVRecord::CNetworkCSVRecord()
{
	ClearRecord();
	m_bGotIDraughtDate = FALSE;
	m_oleIDraughtDate = COleDateTime( 2011, 1, 1, 0, 0, 0 );
	m_strIDraughtLocationList = "";
}

/**********************************************************************/

void CNetworkCSVRecord::ClearRecord()
{
	m_nNetworkNo = 0;
	m_nSPOSVersion = SPOS_V4;
	m_strName = "";
	m_nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
	m_strFileNetworkPath = "";
	m_strSportsBookerSite = "";
	m_strSportsBookerUserName = "";
	m_strSportsBookerPassword = "";
	m_nSportsBookerTimeOut = 30;
}

/**********************************************************************/

void CNetworkCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetNetworkNo ( csv.GetInt ( 0 ) );
	int nVersion = csv.GetInt(1);

	//LOCATION DATABASE VERSION NUMBER
	switch ( nVersion )					
	{
	case 1:		V1ConvertFromCSV ( csv );		break;
	case 2:		V2ConvertFromCSV ( csv );		break;
	case 3:		V3ConvertFromCSV ( csv );		break;
	case 4:		V4ConvertFromCSV ( csv );		break;
	case 5:		V5ConvertFromCSV ( csv );		break;
	case 6:		V6to7ConvertFromCSV ( csv, 6 );	break;
	case 7:		V6to7ConvertFromCSV ( csv, 7 );	break;
	case 8:		V8ConvertFromCSV ( csv );		break;
	}
	ValidateSPOSVersion();
	ValidateConnectionType();
}

/**********************************************************************/

void CNetworkCSVRecord::V1ConvertFromCSV( CCSV& csv )
{
	SetName ( csv.GetString(2) );
	SetConnectionType ( csv.GetInt(3) );
	//4 = Phone
	//5 = Obsolete Network Path
	//6 = File User Name
	//7 = File Password
}

/**********************************************************************/

void CNetworkCSVRecord::V2ConvertFromCSV( CCSV& csv )
{
	SetName ( csv.GetString(2) );
	SetConnectionType ( csv.GetInt(3) );
	//4 = Phone
	SetFileNetworkPath ( csv.GetString(5) );
	//6 = File User Name
	//7 = File Password
}

/**********************************************************************/

void CNetworkCSVRecord::V3ConvertFromCSV( CCSV& csv )
{
	SetName ( csv.GetString(2) );
	SetConnectionType ( csv.GetInt(3) );
	//4 = Entry Name Flag
	//5 = Phone
	//6 = Entry Name
	SetFileNetworkPath ( csv.GetString(7) );
	//8 = User Name
	//9 = Password
}

/**********************************************************************/

void CNetworkCSVRecord::V4ConvertFromCSV( CCSV& csv )
{
	SetSPOSVersion( csv.GetInt(2) );
	SetName ( csv.GetString(3) );
	SetConnectionType ( csv.GetInt(4) );
	//5 = Entry Name Flag
	//6 = Phone
	//7 = Entry Name
	SetFileNetworkPath ( csv.GetString(8) );
	//9 = User Name
	//10 = Password
}

/**********************************************************************/

void CNetworkCSVRecord::V5ConvertFromCSV( CCSV& csv )
{
	SetSPOSVersion( csv.GetInt(2) );
	SetName ( csv.GetString(3) );
	SetConnectionType ( csv.GetInt(4) );
	//5 = Entry Name Flag
	//6 = Phone
	//7 = Entry Name
	SetFileNetworkPath ( csv.GetString(8) );
	//9 = User Name
	//10 = Password
	//11 = FTP IP Address 1
	//12 = FTP IP Address 2
	//13 = FTP IP Address 3
	//14 = FTP IP Address 4
	//15 = FTP User Name
	//16 = FTP Password
	//17 = FTP Port Number
	//18 = FTP Path
	//19 = FTP Timeout
	//20 = FTP Auto Quit
}

/**********************************************************************/

void CNetworkCSVRecord::V6to7ConvertFromCSV( CCSV& csv, int nVer )
{
	SetSPOSVersion( csv.GetInt(2) );
	SetName ( csv.GetString(3) );
	SetConnectionType ( csv.GetInt(4) );
	//5 = Entry Name Flag
	//6 = Phone
	//7 = Entry Name
	SetFileNetworkPath ( csv.GetString(8) );
	//9 = User Name
	//10 = Password
	//11 = FTP Server
	//12 = FTP User Name
	//13 = FTP Password
	//14 = FTP Port Number
	//15 = FTP Path
	//16 = FTP Timeout
	//17 = FTP Auto Quit
	//18 = FTP Sys5000
	SetSportsBookerSite( csv.GetString(19) );
	SetSportsBookerUserName( csv.GetString(20) );
	SetSportsBookerPassword( csv.GetString(21) );

	int nPos = 22;
	if ( nVer >= 7 )
		SetSportsBookerTimeOut( csv.GetInt(nPos++) );
}

/**********************************************************************/

void CNetworkCSVRecord::V8ConvertFromCSV( CCSV& csv )
{
	SetSPOSVersion( csv.GetInt(2) );
	SetName ( csv.GetString(3) );
	SetConnectionType ( csv.GetInt(4) );
	SetFileNetworkPath ( csv.GetString(5) );
	SetSportsBookerSite( csv.GetString(6) );
	SetSportsBookerUserName( csv.GetString(7) );
	SetSportsBookerPassword( csv.GetString(8) );
	SetSportsBookerTimeOut( csv.GetInt(9) );
}

/**********************************************************************/

void CNetworkCSVRecord::ValidateConnectionType()
{
	switch ( m_nConnectionType )
	{
	case CONNECTION_TYPE_STANDARD_NONE:
	case CONNECTION_TYPE_STANDARD_REALTIME:
	case CONNECTION_TYPE_STANDARD_BATCH:
	case CONNECTION_TYPE_SPTBOOK_NONE:
	case CONNECTION_TYPE_SPTBOOK_WEB:
	case CONNECTION_TYPE_SMARTENT_NONE:
	case CONNECTION_TYPE_SMARTENT_WEB:
		break;

	default:						
		SetConnectionType ( CONNECTION_TYPE_STANDARD_NONE );			
		break;
	}	
}

/**********************************************************************/

void CNetworkCSVRecord::ValidateSPOSVersion()
{
	if ( CSPOSVersions::CheckSPOSVersion( m_nSPOSVersion, SPOS_V4 ) == FALSE )
		SetSPOSVersion( SPOS_V4 );
}

/**********************************************************************/

void CNetworkCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nNetworkNo );
	csv.Add ( NETWORK_VERSION );
	csv.Add ( m_nSPOSVersion );
	csv.Add ( m_strName );
	csv.Add ( m_nConnectionType );
	csv.Add ( m_strFileNetworkPath );
	csv.Add ( m_strSportsBookerSite );
	csv.Add ( m_strSportsBookerUserName );
	csv.Add ( m_strSportsBookerPassword );
	csv.Add ( m_nSportsBookerTimeOut );
}

/**********************************************************************/

void CNetworkCSVRecord::SetName ( const char* szName )
{
	CString strName = szName;
	::TrimSpaces ( strName, FALSE );
	if ( strName == "" ) strName = GetSystemName();
	m_strName = strName;	 
}

/**********************************************************************/

void CNetworkCSVRecord::SetConnectionType( int nType )
{
	switch( nType )
	{
	case CONNECTION_TYPE_STANDARD_NONE:
	case CONNECTION_TYPE_STANDARD_REALTIME:	
	case CONNECTION_TYPE_STANDARD_BATCH:			
	case CONNECTION_TYPE_SPTBOOK_NONE:
	case CONNECTION_TYPE_SPTBOOK_WEB:
	case CONNECTION_TYPE_SMARTENT_NONE:
	case CONNECTION_TYPE_SMARTENT_WEB:
		m_nConnectionType = nType;
		break;
	}
}

/**********************************************************************/

void CNetworkCSVRecord::SetFileNetworkPath( const char* szPath )
{
	CString strPath = szPath;
	::TrimSpaces ( strPath, FALSE );
	m_strFileNetworkPath = strPath;
}

/**********************************************************************/

void CNetworkCSVRecord::SetSportsBookerSite( const char* szSite )
{
	CString strSite = szSite;
	::TrimSpaces ( strSite, FALSE );

	if ( strSite.GetLength() <= MAX_LENGTH_SPTBOOK_SITE )
		m_strSportsBookerSite = strSite;
}

/**********************************************************************/

void CNetworkCSVRecord::SetSportsBookerUserName( const char* szName )
{
	CString strName = szName;
	::TrimSpaces ( strName, FALSE );

	if ( strName.GetLength() <= MAX_LENGTH_SPTBOOK_USERNAME )
		m_strSportsBookerUserName = strName;
}

/**********************************************************************/

void CNetworkCSVRecord::SetSportsBookerPassword( const char* szPassword )
{
	CString strPassword = szPassword;
	::TrimSpaces ( strPassword, FALSE );

	if ( strPassword.GetLength() <= MAX_LENGTH_SPTBOOK_PASSWORD )
		m_strSportsBookerPassword = strPassword;
}

/**********************************************************************/

void CNetworkCSVRecord::SetSportsBookerTimeOut( int nTimeOut )
{
	if ( ( nTimeOut >= MIN_SB_TIMEOUT ) && ( nTimeOut <= MAX_SB_TIMEOUT ) )
		m_nSportsBookerTimeOut = nTimeOut;
}

/**********************************************************************/

const char* CNetworkCSVRecord::GetSystemName()
{
	m_strSystemName.Format ( "Network %d", m_nNetworkNo );
	return m_strSystemName;
}

/**********************************************************************/

void CNetworkCSVRecord::GetVersionNumberExternal(int nConnectionType, int nSPOSVersion, CString& strVersion)
{
	switch (nConnectionType)
	{
	case CONNECTION_TYPE_SPTBOOK_NONE:
	case CONNECTION_TYPE_SMARTENT_NONE:
		strVersion = "";
		break;

	default:
		switch (nSPOSVersion)
		{
		case SPOS_V4_3_104:	strVersion = "V4.3.104";	break;
		case SPOS_V4_3_93:	strVersion = "V4.3.93";	break;
		case SPOS_V4_3_18:	strVersion = "V4.3.18";	break;
		case SPOS_V4_2_1622: strVersion = "V4.2.1622";	break;
		case SPOS_V4_2_1618: strVersion = "V4.2.1618";	break;
		case SPOS_V4_2_1605: strVersion = "V4.2.1605";	break;
		case SPOS_V4_2_1579: strVersion = "V4.2.1579";	break;
		case SPOS_V4_2_1530: strVersion = "V4.2.1530";	break;
		case SPOS_V4_2_1456: strVersion = "V4.2.1456";	break;
		case SPOS_V4_2_1384: strVersion = "V4.2.1384";	break;
		case SPOS_V4_2_1087: strVersion = "V4.2.1087";	break;
		case SPOS_V4_2_1081: strVersion = "V4.2.1081";	break;
		case SPOS_V4_2_959:	strVersion = "V4.2.959";	break;
		case SPOS_V4_2_919:	strVersion = "V4.2.919";	break;
		case SPOS_V4_2_714:	strVersion = "V4.2.714";	break;
		case SPOS_V4_2_631:	strVersion = "V4.2.631";	break;
		case SPOS_V4_2_369:	strVersion = "V4.2.369";	break;
		case SPOS_V4_2606:		strVersion = "V4.1.2606";	break;
		case SPOS_V4_2455:		strVersion = "V4.1.2455";	break;
		case SPOS_V4_2161:		strVersion = "V4.1.2161";	break;
		case SPOS_V4_2133:		strVersion = "V4.1.2133";	break;
		case SPOS_V4_2067:		strVersion = "V4.1.2067";	break;
		case SPOS_V4_2012:		strVersion = "V4.1.2012";	break;
		case SPOS_V4_1977:		strVersion = "V4.1.1977";	break;
		case SPOS_V4_1901:		strVersion = "V4.1.1901";	break;
		case SPOS_V4_1734:		strVersion = "V4.1.1734";	break;
		case SPOS_V4_1525:		strVersion = "V4.1.1525";	break;
		case SPOS_V4_1522:		strVersion = "V4.1.1522";	break;
		case SPOS_V4_1519:		strVersion = "V4.1.1519";	break;
		case SPOS_V4_1510:		strVersion = "V4.1.1510";	break;
		case SPOS_V4_1392:		strVersion = "V4.1.1392";	break;
		case SPOS_V4_1300:		strVersion = "V4.1.1300";	break;
		case SPOS_V4_1216:		strVersion = "V4.1.1216";	break;
		case SPOS_V4_1124:		strVersion = "V4.1.1124";	break;
		case SPOS_V4_1096:		strVersion = "V4.1.1096";	break;
		case SPOS_V4_1092:		strVersion = "V4.1.1092";	break;
		case SPOS_V4_1071:		strVersion = "V4.1.1071";	break;
		case SPOS_V4_999:		strVersion = "V4.1.999";	break;
		case SPOS_V4_992:		strVersion = "V4.1.992";	break;
		case SPOS_V4_958:		strVersion = "V4.1.958";	break;
		case SPOS_V4_861:		strVersion = "V4.1.861";	break;
		case SPOS_V4_837:		strVersion = "V4.1.837";	break;
		case SPOS_V4_827:		strVersion = "V4.1.827";	break;
		case SPOS_V4_780:		strVersion = "V4.1.780";	break;
		case SPOS_V4_753:		strVersion = "V4.1.753";	break;
		case SPOS_V4_719:		strVersion = "V4.1.719";	break;
		case SPOS_V4_682:		strVersion = "V4.1.682";	break;
		case SPOS_V4_634:		strVersion = "V4.1.634";	break;
		case SPOS_V4_494:		strVersion = "V4.1.494";	break;
		case SPOS_V4_425:		strVersion = "V4.1.425";	break;
		case SPOS_V4_423:		strVersion = "V4.1.423";	break;
		case SPOS_V4_389:		strVersion = "V4.1.389";	break;
		case SPOS_V4_321:		strVersion = "V4.1.321";	break;
		case SPOS_V4_210:		strVersion = "V4.1.210";	break;
		case SPOS_V4:			strVersion = "V4.1.183";	break;
		}
	}
}

/**********************************************************************/

void CNetworkCSVRecord::GetTreeNameExternal( int nConnectionType, int nSPOSVersion, CString& strName, bool bBrackets )
{
	CNetworkCSVRecord TempRecord;
	TempRecord.SetSPOSVersion( nSPOSVersion );

	switch( nConnectionType )
	{
	case CONNECTION_TYPE_STANDARD_NONE:
	case CONNECTION_TYPE_SPTBOOK_NONE:
	case CONNECTION_TYPE_SMARTENT_NONE:
		TempRecord.SetConnectionType( nConnectionType );
		break;

	default:
		TempRecord.SetConnectionType( CONNECTION_TYPE_STANDARD_NONE );
		break;
	}

	TempRecord.GetSPOSVersionTreeName( strName, bBrackets );
}

/**********************************************************************/

void CNetworkCSVRecord::GetSPOSVersionTreeName( CString& strName, bool bBrackets )
{
	CString strFamily = "";
	CString strVersion = "";

	switch( m_nConnectionType )
	{
	case CONNECTION_TYPE_SPTBOOK_NONE:
	case CONNECTION_TYPE_SPTBOOK_WEB:
		strFamily = "Sports Booker";
		break;

	case CONNECTION_TYPE_SMARTENT_NONE:
	case CONNECTION_TYPE_SMARTENT_WEB:
		strFamily = "Smart Entertainment";
		break;

	case CONNECTION_TYPE_STANDARD_NONE:
	case CONNECTION_TYPE_STANDARD_REALTIME:
	case CONNECTION_TYPE_STANDARD_BATCH:
	default:

		if ( CSPOSVersions::CheckSPOSVersion( m_nSPOSVersion, SPOS_V4 ) == TRUE )
			strFamily = "SPOS ";
		else
			strFamily = "";

		GetVersionNumberExternal( m_nConnectionType, m_nSPOSVersion, strVersion  );
	}

	if ( TRUE == bBrackets )
	{
		strName = "(";
		strName += strFamily + strVersion;
		strName += ")";
	}
	else
		strName = strFamily + strVersion;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CNetworkCSVArray::CNetworkCSVArray() 
{
}

/**********************************************************************/

CNetworkCSVArray::~CNetworkCSVArray()
{
	RemoveAll();
}

/**********************************************************************/

void CNetworkCSVArray::RemoveAll()
{
	while ( GetSize() > 0 )
	{
		int nLocIdx = GetSize() - 1;
		CNetworkCSVRecord* pNetwork = GetNetwork ( nLocIdx );
		delete pNetwork;
		RemoveAt ( nLocIdx );
	}
}

/**********************************************************************/

int CNetworkCSVArray::AddNetwork( int nConnectionType )
{
	int nNetworkIdx = 0, nNetworkNo = 1;
	for ( ; nNetworkIdx < GetSize(); nNetworkIdx++, nNetworkNo++ )
		if ( GetNetworkNo ( nNetworkIdx ) != nNetworkNo )
			break;

	CNetworkCSVRecord*  pNetwork = new CNetworkCSVRecord;
	InsertAt ( nNetworkIdx, pNetwork );
	SetNetworkNo ( nNetworkIdx, nNetworkIdx + 1 );
	SetName ( nNetworkIdx, "" );

	switch( nConnectionType )
	{
	case CONNECTION_TYPE_SPTBOOK_NONE:
		SetConnectionType( nNetworkIdx, CONNECTION_TYPE_SPTBOOK_WEB );
		break;

	case CONNECTION_TYPE_SMARTENT_NONE:
		SetConnectionType( nNetworkIdx, CONNECTION_TYPE_SMARTENT_WEB );
		break;

	case CONNECTION_TYPE_STANDARD_NONE:
	default:
		SetConnectionType( nNetworkIdx, CONNECTION_TYPE_STANDARD_NONE );
		break;
	}

	::CreateSubdirectory( GetFolderPathData( nNetworkIdx ) );
	::CreateSubdirectory( GetFolderPathSysset( nNetworkIdx ) );

	WriteNetwork ( nNetworkIdx );

	return nNetworkIdx;
}

/**********************************************************************/

int CNetworkCSVArray::AddNetwork( int nNetworkNo, const char* szPath )
{
	int nNetworkIdx;
	if ( FindNetworkByNumber ( nNetworkNo, nNetworkIdx ) == FALSE )
	{
		CNetworkCSVRecord*  pNetwork = new CNetworkCSVRecord;
		
		CString strFileInfo;
		strFileInfo.Format ( "%sInfo.dat", szPath );

		CSSFile fileInfo;
		if ( fileInfo.Open( strFileInfo, "rb" ) )
		{
			CString strBuffer;
			fileInfo.ReadString( strBuffer );
			CCSV csv ( strBuffer );
			pNetwork -> ConvertFromCSV ( csv ); 
			InsertAt ( nNetworkIdx, pNetwork );
			SetNetworkNo ( nNetworkIdx, nNetworkNo );
		}
		else
		{
			pNetwork -> SetNetworkNo ( nNetworkNo );
			pNetwork -> SetName ( "" );
			InsertAt ( nNetworkIdx, pNetwork );
		}
	}
	return nNetworkIdx;
}

/**********************************************************************/

bool CNetworkCSVArray::RenameForDeletion( int nNwkIdx )
{
	if ( nNwkIdx < 0 || nNwkIdx >= GetSize() )
		return FALSE;

	ClearDeletionFolder( nNwkIdx );	

	if ( rename ( GetFolderPathData( nNwkIdx ), GetFolderPathDataDelete( nNwkIdx ) ) != 0 )
		return FALSE;

	rename ( GetFolderPathSysset( nNwkIdx ), GetFolderPathSyssetDelete( nNwkIdx ) );
	
	return TRUE;
}

/**********************************************************************/

void CNetworkCSVArray::ClearNetworkFolder( int nNwkIdx )
{
	CFileRemover FileRemover;
	FileRemover.RemoveFolder( dbNetwork.GetFolderPathData( nNwkIdx ) );
	FileRemover.RemoveFolder( dbNetwork.GetFolderPathSysset( nNwkIdx ) );
}

/**********************************************************************/

void CNetworkCSVArray::ClearDeletionFolder( int nNwkIdx )
{
	CFileRemover FileRemover;
	FileRemover.RemoveFolder( dbNetwork.GetFolderPathDataDelete( nNwkIdx ) );
	FileRemover.RemoveFolder( dbNetwork.GetFolderPathSyssetDelete( nNwkIdx ) );
}

/**********************************************************************/

void CNetworkCSVArray::DeleteNetworkByIndex ( int nNwkIdx, bool bDeleteFiles )
{
	if ( nNwkIdx >= 0 && nNwkIdx < GetSize() )
	{
		if ( bDeleteFiles == TRUE )
		{
			ClearNetworkFolder( nNwkIdx );
			ClearDeletionFolder( nNwkIdx );
		}

		dbLocation.PurgeNetwork ( nNwkIdx );
		
		delete GetNetwork ( nNwkIdx );
		RemoveAt ( nNwkIdx );
	}
}

/**********************************************************************/

void CNetworkCSVArray::DeleteNetworkByNumber ( int nNwkNo )
{
	int nNwkIdx;
	if ( FindNetworkByNumber ( nNwkNo, nNwkIdx ) == TRUE )
		DeleteNetworkByIndex ( nNwkIdx, TRUE );
}

/**********************************************************************/

bool CNetworkCSVArray::FindNetworkByNumber ( int nNetworkNo, int& nNwkIdx )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nNwkIdx = ( nStart + nEnd ) / 2;
		int nArrayNo = GetNetworkNo ( nNwkIdx );
		
		if ( nArrayNo == nNetworkNo )
			return TRUE;
		else if ( nArrayNo < nNetworkNo )
			nStart = nNwkIdx + 1;
		else 
			nEnd = nNwkIdx - 1;
	}
	nNwkIdx = nStart;
	return FALSE;
}

/**********************************************************************/

bool CNetworkCSVArray::CanDoWebAPIComms( int nNetworkIdx, int& nDbIdx )
{
	nDbIdx = 0;
	bool bResult = FALSE;
	if ( ( nNetworkIdx >= 0 ) && ( nNetworkIdx < GetSize() ) )
	{
		int nStartIdx, nEndIdx;
		dbLocation.GetNetworkLocIdxRange( nNetworkIdx, nStartIdx, nEndIdx );

		if ( nStartIdx == nEndIdx )
		{	
			if ( dbLocation.GetTerminalCount( nStartIdx ) == 1 )
			{
				if ( dbLocation.GetTNo( nStartIdx, 0 ) == 1 )
				{
					int nDbNo = dbLocation.GetDbNo( nStartIdx );
					bResult = dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx );
				}
			}
		}
	}
	return bResult;
}

/**********************************************************************/

const char* CNetworkCSVArray::GetTreeName ( int nNetworkIdx )
{
	m_strTreeName = "";
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		m_strTreeName = pNetwork -> m_strName;
		
		CString strExtra = "";
		pNetwork -> GetSPOSVersionTreeName( strExtra, TRUE );
			
		m_strTreeName += " ";
		m_strTreeName += strExtra;
	}
	return m_strTreeName;
}

/**********************************************************************/

void CNetworkCSVArray::GetSPOSVersionTreeName ( int nNetworkIdx, CString& strName, bool bBrackets )
{
	strName = "";
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		pNetwork -> GetSPOSVersionTreeName( strName, bBrackets );
	}
}

/**********************************************************************/

const char* CNetworkCSVArray::GetFileNetworkPath ( int nNetworkIdx )
{
	m_strNetworkPath = "";
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		m_strNetworkPath = pNetwork -> m_strFileNetworkPath;
	}
	return m_strNetworkPath;
}

/**********************************************************************/

void CNetworkCSVArray::GetSafeFileNetworkPath ( int nNetworkIdx, CString& strPath )
{
	strPath = "";
	if ( nNetworkIdx < 0 || nNetworkIdx >= GetSize() )
		return;

	strPath = GetFileNetworkPath( nNetworkIdx );
	::TrimSpaces ( strPath, FALSE );
	if ( strPath == "" )
		return;

	if ( strPath.Right(1) != "\\" )
		strPath += "\\";
}

/**********************************************************************/

int CNetworkCSVArray::GetNetworkNo ( int nNetworkIdx )
{
	int nNetworkNo = 0;
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		nNetworkNo = pNetwork -> m_nNetworkNo;
	}
	return nNetworkNo;
}

/**********************************************************************/

const char* CNetworkCSVArray::GetName ( int nNetworkIdx )
{
	m_strName = "";
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		m_strName = pNetwork -> m_strName;
	}
	return m_strName;
}

/**********************************************************************/

const char* CNetworkCSVArray::GetSystemName( int nRpIdx )
{
	m_strSystemName = "";
	if ( nRpIdx >= 0 && nRpIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nRpIdx );
		m_strSystemName = pNetwork -> GetSystemName();
	}
	return m_strSystemName;
}

/**********************************************************************/

int CNetworkCSVArray::GetSPOSVersion ( int nNetworkIdx )
{
	int nSPOSVersion = SPOS_V4;
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		nSPOSVersion = pNetwork -> m_nSPOSVersion;
	}
	return nSPOSVersion;
}

/**********************************************************************/

int CNetworkCSVArray::GetConnectionType ( int nNetworkIdx )
{
	int nConnectionType = 0;
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		nConnectionType = pNetwork -> m_nConnectionType;
	}
	return nConnectionType;
}

/**********************************************************************/

const char* CNetworkCSVArray::GetExtendedSystemName ( int nNetworkIdx )
{
	GetSystemName( nNetworkIdx );
	GetName( nNetworkIdx );

	if ( m_strSystemName != m_strName )
	{
		m_strSystemName += " (";
		m_strSystemName += m_strName;
		m_strSystemName += ")";
	}

	return m_strSystemName;
}

/**********************************************************************/

const char* CNetworkCSVArray::GetSportsBookerSite( int nNetworkIdx )
{
	m_strSportsBookerSite = "";
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		m_strSportsBookerSite = pNetwork -> m_strSportsBookerSite;
	}
	return m_strSportsBookerSite;
}

/**********************************************************************/

const char* CNetworkCSVArray::GetSportsBookerUserName( int nNetworkIdx )
{
	m_strSportsBookerUserName = "";
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		m_strSportsBookerUserName = pNetwork -> m_strSportsBookerUserName;
	}
	return m_strSportsBookerUserName;
}

/**********************************************************************/

const char* CNetworkCSVArray::GetSportsBookerPassword ( int nNetworkIdx )
{
	m_strSportsBookerPassword = "";
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		m_strSportsBookerPassword = pNetwork -> m_strSportsBookerPassword;
	}
	return m_strSportsBookerPassword;
}

/**********************************************************************/

int CNetworkCSVArray::GetSportsBookerTimeOut ( int nNetworkIdx )
{
	int nResult = 30;
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		nResult = pNetwork -> m_nSportsBookerTimeOut;
	}
	return nResult;
}

/**********************************************************************/

void CNetworkCSVArray::SetNetworkNo ( int nNetworkIdx, int nNetworkNo )
{
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		pNetwork -> SetNetworkNo ( nNetworkNo );
	}
}

/**********************************************************************/

void CNetworkCSVArray::SetName ( int nNetworkIdx, const char* szName )
{
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		pNetwork -> SetName ( szName );
	}
}

/**********************************************************************/

void CNetworkCSVArray::SetSPOSVersion( int nNetworkIdx, int nSPOSVersion )
{
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		pNetwork -> SetSPOSVersion ( nSPOSVersion );
		pNetwork -> ValidateSPOSVersion();
	}
}

/**********************************************************************/

void CNetworkCSVArray::SetConnectionType ( int nNetworkIdx, int nType )
{
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		pNetwork -> SetConnectionType ( nType );
	}
}

/**********************************************************************/

void CNetworkCSVArray::SetFileNetworkPath ( int nNetworkIdx, const char* szName )
{
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		pNetwork -> SetFileNetworkPath ( szName );
	}
}

/**********************************************************************/

void CNetworkCSVArray::SetSportsBookerSite ( int nNetworkIdx, const char* szSite )
{
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		pNetwork -> SetSportsBookerSite ( szSite );
	}
}

/**********************************************************************/

void CNetworkCSVArray::SetSportsBookerUserName ( int nNetworkIdx, const char* szName )
{
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		pNetwork -> SetSportsBookerUserName ( szName );
	}
}

/**********************************************************************/

void CNetworkCSVArray::SetSportsBookerPassword ( int nNetworkIdx, const char* szName )
{
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		pNetwork -> SetSportsBookerPassword ( szName );
	}
}

/**********************************************************************/

void CNetworkCSVArray::SetSportsBookerTimeOut ( int nNetworkIdx, int nTimeOut )
{
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		pNetwork -> SetSportsBookerTimeOut ( nTimeOut );
	}
}

/**********************************************************************/

const char* CNetworkCSVArray::GetFolderPathData( int nNetworkIdx )
{
	m_strFolderPath = "";
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		m_strFolderPath.Format ( "NetWk%3.3d", pNetwork -> m_nNetworkNo );
		GetDataProgramPath( m_strFolderPath );
	}
	return m_strFolderPath;	
}

/**********************************************************************/

const char* CNetworkCSVArray::GetFolderPathDataHistory( int nNetworkIdx )
{
	m_strFolderPath = "";
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		m_strFolderPath.Format ( "NetWk%3.3d\\History", 
			pNetwork -> m_nNetworkNo );

		GetDataProgramPath( m_strFolderPath );
	}
	return m_strFolderPath;	
}

/**********************************************************************/

const char* CNetworkCSVArray::GetFolderPathDataEODFiles( int nNetworkIdx, const char* szFolder )
{
	m_strFolderPath = "";
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		m_strFolderPath.Format ( "NetWk%3.3d\\History\\%s", 
			pNetwork -> m_nNetworkNo,
			szFolder );

		GetDataProgramPath( m_strFolderPath );
	}
	return m_strFolderPath;	
}

/**********************************************************************/

const char* CNetworkCSVArray::GetFolderPathDataDelete( int nNetworkIdx )
{
	m_strFolderPathDelete = GetFolderPathData( nNetworkIdx );
	m_strFolderPathDelete += "XXX";
	return m_strFolderPathDelete;	
}

/**********************************************************************/

const char* CNetworkCSVArray::GetFolderPathSysset( int nNetworkIdx )
{
	m_strFolderPath = "";
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		m_strFolderPath.Format ( "NetWk%3.3d", pNetwork -> m_nNetworkNo );
		GetSyssetProgramPath( m_strFolderPath );
	}
	else if ( -1 == nNetworkIdx )
	{
		m_strFolderPath = "NetWkPMS";
		GetSyssetProgramPath( m_strFolderPath );
	}

	return m_strFolderPath;	
}

/**********************************************************************/

const char* CNetworkCSVArray::GetFolderPathSyssetDelete( int nNetworkIdx )
{
	m_strFolderPathDelete = GetFolderPathSysset( nNetworkIdx );
	m_strFolderPathDelete += "XXX";
	return m_strFolderPathDelete;	
}

/**********************************************************************/

const char* CNetworkCSVArray::GetFilePathInfo( int nNetworkIdx )
{
	m_strFilePathInfo = "";
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		m_strFilePathInfo = GetFolderPathData ( nNetworkIdx );
		m_strFilePathInfo += "\\info.dat";
	}
	return m_strFilePathInfo;
}

/**********************************************************************/

const char* CNetworkCSVArray::GetFilePathDataReasonToken( int nNetworkIdx )
{
	m_strFilePath = "";
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		m_strFilePath = GetFolderPathDataHistory ( nNetworkIdx );
		m_strFilePath += "\\reasons.dat";
	}
	return m_strFilePath;
}

/**********************************************************************/

void CNetworkCSVArray::WriteReasonTokenFile( int nNetworkIdx )
{
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CSSFile fileToken;
		if ( fileToken.Open( GetFilePathDataReasonToken( nNetworkIdx ), "wb" ) == TRUE )
			fileToken.WriteLine( "MICROTREND" );
	}
}

/**********************************************************************/

void CNetworkCSVArray::DeleteReasonTokenFile( int nNetworkIdx )
{
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
		CFileRemover FileRemover ( GetFilePathDataReasonToken( nNetworkIdx ) );
}

/**********************************************************************/

bool CNetworkCSVArray::CheckReasonTokenFile( int nNetworkIdx )
{
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
		return ::FileExists ( GetFilePathDataReasonToken( nNetworkIdx ) );
	else
		return FALSE;
}

/**********************************************************************/

const char* CNetworkCSVArray::GetCSVLine ( int nNetworkIdx )
{
	m_strCSVLine = "";
	if ( nNetworkIdx >= 0 && nNetworkIdx < GetSize() )
	{
		CNetworkCSVRecord* pNetwork = GetNetwork ( nNetworkIdx );
		
		CCSV csv;
		pNetwork ->ConvertToCSV(csv);

		m_strCSVLine = csv.GetLine();
	}
	return m_strCSVLine;
}

/**********************************************************************/

bool CNetworkCSVArray::WriteNetwork ( int nNetworkIdx )
{
	if ( nNetworkIdx < 0 || nNetworkIdx >= GetSize() )
		return FALSE;

	CSSFile fileNetwork;
	if ( fileNetwork.Open ( GetFilePathInfo ( nNetworkIdx ), "wb" ) == FALSE )
		return FALSE;

	fileNetwork.WriteLine ( GetCSVLine ( nNetworkIdx ) );
	return TRUE;
}

/**********************************************************************/

void CNetworkCSVArray::SortByName( CWordArray& arrayNwkIdx )
{
	arrayNwkIdx.RemoveAll();

	CReportConsolidationArray<CSortedLocation> arrayTemp;

	for ( int nNwkIdx = 0; nNwkIdx < GetSize(); nNwkIdx++ )
	{
		CSortedLocation item;
		item.m_strName = GetName( nNwkIdx );
		item.m_nIdx = nNwkIdx;
		arrayTemp.Consolidate( item );
	}
	
	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CSortedLocation item;
		arrayTemp.GetAt( n, item );
		arrayNwkIdx.Add( item.m_nIdx );
	}
}

/**********************************************************************/

void CNetworkCSVArray::SortNetworkListByName( CArray<CSelectNetworkInfo,CSelectNetworkInfo>& arraySelected )
{
	CReportConsolidationArray<CSortedLocation> arrayTemp;

	for ( int n = 0; n < arraySelected.GetSize(); n++ )
	{
		int nNwkIdx = arraySelected[n].m_nNwkIdx;
		bool bSelect = arraySelected[n].m_bSelected;
		
		if ( nNwkIdx >= 0 && nNwkIdx < GetSize() )
		{
			CSortedLocation item;
			item.m_strName = GetName( nNwkIdx );
			item.m_nIdx = nNwkIdx;
			item.m_bSelected = bSelect;
			arrayTemp.Consolidate( item );
		}
	}
	
	arraySelected.RemoveAll();
	
	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CSortedLocation item;
		arrayTemp.GetAt( n, item );
		
		CSelectNetworkInfo info;
		info.m_nNwkIdx = item.m_nIdx;
		info.m_bSelected = item.m_bSelected;
		arraySelected.Add( info );
	}
}

/**********************************************************************/

void CNetworkCSVArray::GetDatabaseCommsLocArray( int nDbIdx, CArray<int,int>& array, bool& bBlocked )
{
	array.RemoveAll();
	bBlocked = FALSE;

	int nStartDbIdx, nEndDbIdx;

	if ( nDbIdx < -1 || nDbIdx >= dbDatabase.GetSize() )
		return;

	else if ( nDbIdx == -1 )
	{
		nStartDbIdx = 0;
		nEndDbIdx = dbDatabase.GetSize() - 1;
	}
	else
	{
		nStartDbIdx = nDbIdx;
		nEndDbIdx = nDbIdx;
	}

	int nCount = 0;
	for ( nDbIdx = nStartDbIdx; nDbIdx <= nEndDbIdx; nDbIdx++ )
	{
		CArray<int,int> Set;
		dbLocation.GetDatabaseLocIdxSet( nDbIdx, Set );

		for ( int nIndex = 0; nIndex < Set.GetSize(); nIndex++ )
		{
			int nLocIdx = Set.GetAt ( nIndex );
			int nNetworkNo = dbLocation.GetNetworkNo( nLocIdx );

			int nNetworkIdx;
			if ( dbNetwork.FindNetworkByNumber( nNetworkNo, nNetworkIdx ) == FALSE )
				continue;

			switch( dbNetwork.GetConnectionType( nNetworkIdx ) )
			{
			case CONNECTION_TYPE_STANDARD_NONE:
			case CONNECTION_TYPE_SPTBOOK_NONE:
			case CONNECTION_TYPE_SMARTENT_NONE:
				continue;
			}

			if ( PasswordArray.CheckLocationAccessLocIdx( nLocIdx ) == FALSE )
			{
				bBlocked = TRUE;
				continue;
			}
			
			array.Add( nLocIdx );
		}
	}
}

/**********************************************************************/

bool CNetworkCSVArray::ValidateNetworks( bool bDisableComms, bool bStandard, bool bSportsBooker, bool bSmartEnt )
{
	bool bResult = TRUE;

	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return TRUE;

	ReportFile.SetStyle1 ( "Duplicated Terminal Numbers" );
	ReportFile.AddColumn ( "Loc No", TA_LEFT, 150 );
	ReportFile.AddColumn ( "Location Name", TA_LEFT, 500 );
	ReportFile.WriteLine ( "" );

	for ( int nNetworkIdx = 0; nNetworkIdx < GetSize(); nNetworkIdx++ )
	{
		int nConnectionType = dbNetwork.GetConnectionType( nNetworkIdx );
		
		bool bAccept = TRUE;

		switch( nConnectionType )
		{
		case CONNECTION_TYPE_STANDARD_NONE:			bAccept = bStandard;		break;
		case CONNECTION_TYPE_STANDARD_REALTIME:		bAccept = bStandard;		break;
		case CONNECTION_TYPE_STANDARD_BATCH:		bAccept = bStandard;		break;
		case CONNECTION_TYPE_SPTBOOK_NONE:			bAccept = bSportsBooker;	break;
		case CONNECTION_TYPE_SPTBOOK_WEB:			bAccept = bSportsBooker;	break;
		case CONNECTION_TYPE_SMARTENT_NONE:			bAccept = bSmartEnt;		break;
		case CONNECTION_TYPE_SMARTENT_WEB:			bAccept = bSmartEnt;		break;
		default:									bAccept = FALSE;			break;
		}

		if ( FALSE == bAccept )
			continue;
	
		bool bBadNetwork = FALSE;

		int nStartIdx, nEndIdx;
		dbLocation.GetNetworkLocIdxRange( nNetworkIdx, nStartIdx, nEndIdx );

		CReportConsolidationArray<CSortedStringItem> arrayTerminals;

		//CREATE A SORTED LIST OF TERMINALS AND LOCATIONS
		for ( int nLocIdx = nStartIdx; nLocIdx <= nEndIdx; nLocIdx++ )
		{
			for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )
			{
				CString strTNo;
				strTNo.Format ( "%3.3d%6.6d", dbLocation.GetTNo( nLocIdx, nTerminalIdx ), nLocIdx );
				
				CSortedStringItem item;
				item.m_strItem = strTNo;
				arrayTerminals.Consolidate ( item );
			}
		}

		//CLUMP LOCATIONS BY TERMINAL
		CStringArray arrayLocMap;
		CString strLoc = "";

		for ( int n = 0; n < arrayTerminals.GetSize(); n++ )
		{
			CSortedStringItem item;
			arrayTerminals.GetAt( n, item );
			CString strLabel = item.m_strItem;

			if ( "" == strLoc )
				strLoc = strLabel;
			else if ( strLoc.Left(3) == strLabel.Left(3) )
				strLoc += strLabel.Right(6);
			else
			{
				if ( strLoc.GetLength() > 9 )
					arrayLocMap.Add( strLoc );

				strLoc = strLabel;
			}
		}

		if ( strLoc.GetLength() > 9 )
			arrayLocMap.Add( strLoc );

		arrayTerminals.RemoveAll();

		CString strCurrentClump = "";
		CStringArray arrayTNo;
		int nPreviousTNo = 999;

		for ( int n = 0; n < arrayLocMap.GetSize(); n++ )
		{
			CString strClump = arrayLocMap.GetAt(n);

			bool bSame = FALSE;
			
			int nLen = strClump.GetLength();
			if ( nLen == strCurrentClump.GetLength() )
				if ( strClump.Right( nLen - 3 ) == strCurrentClump.Right( nLen - 3 ) )
					bSame = TRUE;

			int nTNo = atoi( strClump.Left(3) );
			if ( TRUE == bSame )
			{
				if ( nTNo != nPreviousTNo + 1 )
					bSame = FALSE;
			}
			nPreviousTNo = nTNo;
			
			if ( FALSE == bSame )
			{
				if ( arrayTNo.GetSize() != 0 )
				{
					WriteBadTerminals( ReportFile, arrayTNo, strCurrentClump, nNetworkIdx );
					bBadNetwork = TRUE;
				}
				
				arrayTNo.RemoveAll();
				strCurrentClump = strClump;
			}

			if ( arrayTNo.GetSize() < 2 )
				arrayTNo.Add( strClump.Left(3) );
			else
				arrayTNo.SetAt( 1, strClump.Left(3) );
		}

		if ( arrayTNo.GetSize() != 0 )
		{
			WriteBadTerminals( ReportFile, arrayTNo, strCurrentClump, nNetworkIdx );
			bBadNetwork = TRUE;
		}
			
		if ( bBadNetwork )
		{
			if ( bDisableComms )
			{
				switch( GetConnectionType( nNetworkIdx ) )
				{
				case CONNECTION_TYPE_SPTBOOK_NONE:
				case CONNECTION_TYPE_SPTBOOK_WEB:
					SetConnectionType( nNetworkIdx, CONNECTION_TYPE_SPTBOOK_NONE );
					break;

				case CONNECTION_TYPE_SMARTENT_NONE:
				case CONNECTION_TYPE_SMARTENT_WEB:
					SetConnectionType( nNetworkIdx, CONNECTION_TYPE_SMARTENT_NONE );
					break;

				case CONNECTION_TYPE_STANDARD_NONE:
				case CONNECTION_TYPE_STANDARD_REALTIME:
				case CONNECTION_TYPE_STANDARD_BATCH:
				default:
					SetConnectionType( nNetworkIdx, CONNECTION_TYPE_STANDARD_NONE );
					break;
				}

				WriteNetwork( nNetworkIdx );

				CString strMsg;
				strMsg.Format ( "DISABLE COMMS (%s)", GetName( nNetworkIdx ) );
				
				CMessageLoggerInfo info;
				info.m_nHideLevel = 2;
				info.m_strMessage = strMsg;
				info.m_nNetworkIdx = nNetworkIdx;
				info.m_bAddNetworkName = TRUE;
				info.m_bAddDate = TRUE;
				info.m_bAddTimeOfDay = TRUE;

				MessageLogger.LogSchedulerMessage( info, TRUE, TRUE, FALSE );
			}
			bResult = FALSE;
		}
	}
	return bResult;
}

/**********************************************************************/

void CNetworkCSVArray::WriteBadTerminals( CReportFile& ReportFile, CStringArray& arrayTNo, CString& strCurrentClump, int nNetworkIdx )
{
	CString str;
	switch( arrayTNo.GetSize() )
	{
	case 1:
		str.Format( "<..>%s, TNo %s",
			dbNetwork.GetName( nNetworkIdx ),
			(const char*) arrayTNo.GetAt(0) );
		break;

	case 2:
		str.Format( "<..>%s, TNo %s - %s",
			dbNetwork.GetName( nNetworkIdx ),
			(const char*) arrayTNo.GetAt(0),
			(const char*) arrayTNo.GetAt(1) );
		break;

	default:
		return;
	}

	ReportFile.WriteLine( str );
	ReportFile.WriteLine( "<LI>" );

	for ( int x = 3; x <= strCurrentClump.GetLength() - 6; x += 6 )
	{	
		int nLocIdx = atoi( strCurrentClump.Mid( x, 6 ) );

		CString strLocNo;
		strLocNo.Format( "%3.3d", dbLocation.GetLocNo( nLocIdx ) );

		CCSV csv( '\t' );
		csv.Add( strLocNo );
		csv.Add( dbLocation.GetName( nLocIdx ) );
		ReportFile.WriteLine( csv.GetLine() );
	}

	ReportFile.WriteLine( "" );
	ReportFile.WriteLine( "" );
}

/**********************************************************************/

void CNetworkCSVArray::GetDownloadPermissions( bool& bCanDoDownloads )
{
	bCanDoDownloads = FALSE;
	
	if ( PasswordArray.GetEnable( PasswordTicks::CommsDownloads ) == FALSE )
		return;

	for ( int nNetworkIdx = 0; nNetworkIdx < GetSize(); nNetworkIdx++ )
	{
		switch( dbNetwork.GetConnectionType( nNetworkIdx ) )
		{
		case CONNECTION_TYPE_STANDARD_NONE:
		case CONNECTION_TYPE_SPTBOOK_NONE:
		case CONNECTION_TYPE_SPTBOOK_WEB:
		case CONNECTION_TYPE_SMARTENT_NONE:
		case CONNECTION_TYPE_SMARTENT_WEB:
			continue;
		}

		int nStartIdx, nEndIdx;
		dbLocation.GetNetworkLocIdxRange( nNetworkIdx, nStartIdx, nEndIdx );
		for ( int nLocIdx = nStartIdx; nLocIdx <= nEndIdx; nLocIdx++ )
		{
			int nDbIdx;
			int nDbNo = dbLocation.GetDbNo( nLocIdx );
			if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) )
			{
				bCanDoDownloads = TRUE;
				break;
			}
		}

		if ( bCanDoDownloads == TRUE )
			break;
	}
}

/**********************************************************************/

const char* CNetworkCSVArray::GetFilePathSyssetSchedLogAll( int nNwkIdx )
{
	m_strFilePathSchedLogAll = "";

	if ( nNwkIdx >= 0 && nNwkIdx < GetSize() )
	{
		m_strFilePathSchedLogAll.Format ( "SchedLog\\N%3.3d.rep",
			GetNetworkNo( nNwkIdx ) );
		
		GetSyssetProgramPath( m_strFilePathSchedLogAll );
	}

	return m_strFilePathSchedLogAll;
}

/**********************************************************************/

const char* CNetworkCSVArray::GetFilePathSyssetSchedLogOld( int nNwkIdx )
{
	m_strFilePathSchedLogOld = "";

	if ( nNwkIdx >= 0 && nNwkIdx < GetSize() )
	{
		m_strFilePathSchedLogOld.Format ( "SchedOld\\N%3.3d.rep",
			GetNetworkNo( nNwkIdx ) );
		
		GetSyssetProgramPath( m_strFilePathSchedLogOld );
	}

	return m_strFilePathSchedLogOld;
}

/**********************************************************************/


const char* CNetworkCSVArray::GetFolderPathSyssetPending( int nNetworkIdx, int n )
{
	m_strFolderPathPending = "";
	if ( nNetworkIdx >= -1 && nNetworkIdx < GetSize() )
	{
		CString strFolder;
		strFolder.Format( "\\Pending%d", n );

		m_strFolderPathPending = GetFolderPathSysset( nNetworkIdx );
		m_strFolderPathPending += strFolder;
	}
	return m_strFolderPathPending;
}

/**********************************************************************/

const char* CNetworkCSVArray::GetFolderPathSyssetCashRSP( int nNetworkIdx )
{
	m_strFolderPathPending = GetFolderPathSyssetPending( nNetworkIdx, 1 );
	return m_strFolderPathPending;
}

/**********************************************************************/

const char* CNetworkCSVArray::GetFolderPathSyssetStock( int nNetworkIdx )
{
	m_strFolderPathPending = GetFolderPathSyssetPending( nNetworkIdx, 2 );
	return m_strFolderPathPending;
}

/**********************************************************************/

const char* CNetworkCSVArray::GetFolderPathSyssetCashRSPFailed( int nNetworkIdx )
{
	m_strFolderPathPending = GetFolderPathSyssetPending( nNetworkIdx, 3 );
	return m_strFolderPathPending;
}

/**********************************************************************/

const char* CNetworkCSVArray::GetFolderPathSyssetBatchExport( int nNetworkIdx )
{
	m_strFolderPathPending = GetFolderPathSyssetPending( nNetworkIdx, 4 );
	return m_strFolderPathPending;
}

/**********************************************************************/
