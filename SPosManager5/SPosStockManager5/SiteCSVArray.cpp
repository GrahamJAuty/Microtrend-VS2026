/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "EcrmanOptionsIni.h"
#include "FileRemover.h"
//#include "globalfunctions.h"
#include "ReportConsolidationArray.h"
#include "SortedLocation.h"
/**********************************************************************/
#include "SiteCSVArray.h"
/**********************************************************************/

CSiteCSVRecord::CSiteCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CSiteCSVRecord::ClearRecord()
{
	m_nSiteNo = 0;
	m_strName = "";
	m_nDbNo = 0;
	m_strLegacyAddress1 = "";
	m_strLegacyAddress2 = "";
	m_strLegacyAddress3 = "";
	m_strLegacyPostcode = "";
	m_strLegacyPhone = "";
	m_strLegacyFax = "";
	m_strLegacyEmail = "";
}

/**********************************************************************/

void CSiteCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetSiteNo ( csv.GetInt(0) );
	CString strName = csv.GetString(1);
	SetDbNo ( csv.GetInt(2) );

	switch ( csv.GetInt(3) )
	{
	case 1:		V1ConvertFromCSV ( csv );	break;
	}

	SetName( strName );
}

/**********************************************************************/

void CSiteCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetLegacyAddress1 ( csv.GetString(4) );
	SetLegacyAddress2 ( csv.GetString(5) );
	SetLegacyAddress3 ( csv.GetString(6) );
	SetLegacyPostcode ( csv.GetString(7) );
	SetLegacyPhone ( csv.GetString(8) );
	SetLegacyFax ( csv.GetString(9) );
	SetLegacyEmail ( csv.GetString(10) );
}

/**********************************************************************/
	
void CSiteCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nSiteNo );
	csv.Add ( m_strName );
	csv.Add ( m_nDbNo );
	csv.Add ( SITE_VERSION );				
	csv.Add ( m_strLegacyAddress1 );
	csv.Add ( m_strLegacyAddress2 );
	csv.Add ( m_strLegacyAddress3 );
	csv.Add ( m_strLegacyPostcode );
	csv.Add ( m_strLegacyPhone );
	csv.Add ( m_strLegacyFax );
	csv.Add ( m_strLegacyEmail );
}

/**********************************************************************/

void CSiteCSVRecord::SetName ( const char* szName )
{
	CString strName = szName;
	::TrimSpaces ( strName, FALSE );
	if ( strName == "" ) strName = GetSystemName();
	m_strName = strName;	 
}

/**********************************************************************/

const char* CSiteCSVRecord::GetSystemName()
{
	m_strSystemName.Format ( "Site %d.%d", m_nDbNo, m_nSiteNo );
	return m_strSystemName;
}

/**********************************************************************/

CSiteCSVArray::CSiteCSVArray() 
{
}

/**********************************************************************/

CSiteCSVArray::~CSiteCSVArray()
{
	RemoveAll();
}

/**********************************************************************/

int CSiteCSVArray::AddSite ( int nDbNo )
{
	int nStartIdx, nEndIdx;
	FindSiteByNumber ( nDbNo, 0, nStartIdx );
	FindSiteByNumber ( nDbNo + 1, 0, nEndIdx );
	nEndIdx--;
	
	if ( nEndIdx >= nStartIdx + 998 )
		return -1;

	int nSiteIdx = nStartIdx, nSiteNo = 1;
	for ( ; nSiteIdx <= nEndIdx; nSiteIdx++, nSiteNo++ )
		if ( GetSiteNo ( nSiteIdx ) != nSiteNo )
			break;
		
	CSiteCSVRecord* pSite = new CSiteCSVRecord;
	InsertAt ( nSiteIdx, pSite );
	SetSiteNo ( nSiteIdx, nSiteNo );
	SetDbNo ( nSiteIdx, nDbNo );
	SetName ( nSiteIdx, "" );
	
	CString strFolderPath = GetFolderPath ( nSiteIdx );
	::CreateSubdirectory ( strFolderPath );
	
	WriteSite ( nSiteIdx );
	return nSiteIdx;
}

/**********************************************************************/

int CSiteCSVArray::AddSite( int nDbNo, int nSiteNo, const char* szPath )
{
	int nSiteIdx;
	if ( FindSiteByNumber ( nDbNo, nSiteNo, nSiteIdx ) == FALSE )
	{
		CSiteCSVRecord* pSite = new CSiteCSVRecord;
		
		CString strFileInfo;
		strFileInfo.Format ( "%sInfo.dat", szPath );

		CSSFile fileInfo;
		if ( fileInfo.Open( strFileInfo, "rb" ) )
		{
			CString strBuffer;
			fileInfo.ReadString( strBuffer );
			CCSV csv ( strBuffer );
			pSite -> ConvertFromCSV ( csv ); 
			InsertAt ( nSiteIdx, pSite );
			SetSiteNo ( nSiteIdx, nSiteNo );
			SetDbNo ( nSiteIdx, nDbNo );
		}
		else
		{
			pSite -> SetSiteNo ( nSiteNo );
			pSite -> SetDbNo ( nDbNo );
			pSite -> SetName ( "" );
			InsertAt ( nSiteIdx, pSite );
		}
	}
	return nSiteIdx;
}

/**********************************************************************/

bool CSiteCSVArray::FindSiteByNumber ( int nDbNo, int nSiteNo, int& nSiteIdx )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	int nSearchNo = ( 1000 * nDbNo ) + nSiteNo;

	while ( nStart <= nEnd )
	{
		nSiteIdx = ( nStart + nEnd ) / 2;
		int nArrayNo = ( 1000 * GetDbNo ( nSiteIdx ) ) + GetSiteNo ( nSiteIdx );
		
		if ( nArrayNo == nSearchNo )
			return TRUE;
		else if ( nArrayNo < nSearchNo )
			nStart = nSiteIdx + 1;
		else 
			nEnd = nSiteIdx - 1;
	}
	nSiteIdx = nStart;
	return FALSE;
}

/**********************************************************************/

void CSiteCSVArray::GetSiteIdxRange ( int nDbIdx, int& nStartIdx, int& nEndIdx )
{
	nStartIdx = 0;
	nEndIdx = -1;

	if ( nDbIdx < 0 || nDbIdx >= dbDatabase.GetSize() )
		return;

	int nDbNo = dbDatabase.GetDbNo( nDbIdx );

	FindSiteByNumber ( nDbNo, 0, nStartIdx );
	FindSiteByNumber ( nDbNo + 1, 0, nEndIdx );
	nEndIdx--;
}

/**********************************************************************/

bool CSiteCSVArray::RenameForDeletion( int nSiteIdx )
{
	if ( nSiteIdx < 0 || nSiteIdx >= GetSize() )
		return FALSE;

	ClearDeletionFolder( nSiteIdx );	

	if ( rename ( GetFolderPath( nSiteIdx ), GetFolderPathDelete( nSiteIdx ) ) != 0 )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

void CSiteCSVArray::ClearSiteFolder( int nSiteIdx )
{
	CFileRemover FileRemover;
	CString strFolderPath = GetFolderPath( nSiteIdx );
	FileRemover.RemoveFolder( strFolderPath );
}

/**********************************************************************/

void CSiteCSVArray::ClearDeletionFolder( int nSiteIdx )
{
	CFileRemover FileRemover;
	CString strFolderPath = GetFolderPathDelete( nSiteIdx );
	FileRemover.RemoveFolder( strFolderPath );
}

/**********************************************************************/

void CSiteCSVArray::DeleteSiteByIndex ( int nSiteIdx, bool bDeleteFiles )
{
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		if ( bDeleteFiles == TRUE )
		{
			ClearSiteFolder( nSiteIdx );
			ClearDeletionFolder( nSiteIdx );
		}
		
		delete GetSite ( nSiteIdx );
		RemoveAt ( nSiteIdx );
	}
}

/**********************************************************************/

void CSiteCSVArray::DeleteSiteByNumber ( int nDbNo, int nSiteNo )
{
	int nSiteIdx;
	if ( FindSiteByNumber ( nDbNo, nSiteNo, nSiteIdx ) == TRUE )
		DeleteSiteByIndex ( nSiteIdx, TRUE );
}

/**********************************************************************/

void CSiteCSVArray::PurgeDatabase( int nDbIdx )
{
	if ( nDbIdx < 0 || nDbIdx >= dbDatabase.GetSize() )
		return;

	int nStartIdx, nEndIdx;
	GetSiteIdxRange ( nDbIdx, nStartIdx, nEndIdx );

	for ( int nSiteIdx = nEndIdx; nSiteIdx >= nStartIdx; nSiteIdx-- )
		DeleteSiteByIndex ( nSiteIdx, FALSE );
}

/**********************************************************************/

void CSiteCSVArray::RemoveAll()
{
	while ( GetSize() > 0 )
	{
		int nSiteIdx = GetSize() - 1;
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		delete pSite;
		RemoveAt ( nSiteIdx );
	}
}

/**********************************************************************/

const char* CSiteCSVArray::GetCSVLine ( int nSiteIdx )
{
	m_strCSVLine = "";
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		
		CCSV csv;
		pSite ->ConvertToCSV(csv);

		m_strCSVLine = csv.GetLine();
	}
	return m_strCSVLine;
}

/**********************************************************************/

int CSiteCSVArray::GetSiteNo ( int nSiteIdx )
{
	int nSiteNo = 0;
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		nSiteNo = pSite -> m_nSiteNo;
	}
	return nSiteNo;
}

/**********************************************************************/

int CSiteCSVArray::GetDbNo ( int nSiteIdx )
{
	int nDbNo = 0;
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		nDbNo = pSite -> m_nDbNo;
	}
	return nDbNo;
}

/**********************************************************************/

const char* CSiteCSVArray::GetName ( int nSiteIdx )
{
	m_strName = "";
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		m_strName = pSite -> m_strName;
	}
	return m_strName;
}

/**********************************************************************/

const char* CSiteCSVArray::GetSystemName ( int nSiteIdx )
{
	m_strSystemName = "";
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		m_strSystemName = pSite -> GetSystemName();
	}
	return m_strSystemName;
}

/**********************************************************************/

const char* CSiteCSVArray::GetLegacyAddress1 ( int nSiteIdx )
{
	m_strLegacyAddress1 = "";
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		m_strLegacyAddress1 = pSite -> m_strLegacyAddress1;
	}
	return m_strLegacyAddress1;
}

/**********************************************************************/

const char* CSiteCSVArray::GetLegacyAddress2 ( int nSiteIdx )
{
	m_strLegacyAddress2 = "";
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		m_strLegacyAddress2 = pSite -> m_strLegacyAddress2;
	}
	return m_strLegacyAddress2;
}

/**********************************************************************/

const char* CSiteCSVArray::GetLegacyAddress3 ( int nSiteIdx )
{
	m_strLegacyAddress3 = "";
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		m_strLegacyAddress3 = pSite -> m_strLegacyAddress3;
	}
	return m_strLegacyAddress3;
}

/**********************************************************************/

const char* CSiteCSVArray::GetLegacyPhone ( int nSiteIdx )
{
	m_strLegacyPhone = "";
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		m_strLegacyPhone = pSite -> m_strLegacyPhone;
	}
	return m_strLegacyPhone;
}

/**********************************************************************/

const char* CSiteCSVArray::GetLegacyPostcode ( int nSiteIdx )
{
	m_strLegacyPostcode = "";
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		m_strLegacyPostcode = pSite -> m_strLegacyPostcode;
	}
	return m_strLegacyPostcode;
}

/**********************************************************************/

const char* CSiteCSVArray::GetLegacyFax ( int nSiteIdx )
{
	m_strLegacyFax = "";
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		m_strLegacyFax = pSite -> m_strLegacyFax;
	}
	return m_strLegacyFax;
}

/**********************************************************************/

const char* CSiteCSVArray::GetLegacyEmail ( int nSiteIdx )
{
	m_strLegacyEmail = "";
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		m_strLegacyEmail = pSite -> m_strLegacyEmail;
	}
	return m_strLegacyEmail;
}

/**********************************************************************/

void CSiteCSVArray::SetDbNo ( int nSiteIdx, int nDbNo )
{
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		pSite -> SetDbNo ( nDbNo );
	}
}

/**********************************************************************/

void CSiteCSVArray::SetSiteNo ( int nSiteIdx, int nSiteNo )
{
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		pSite -> SetSiteNo ( nSiteNo );
	}
}

/**********************************************************************/

void CSiteCSVArray::SetName ( int nSiteIdx, const char* szName )
{
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		pSite -> SetName ( szName );
	}
}

/**********************************************************************/

void CSiteCSVArray::SetLegacyAddress1 ( int nSiteIdx, const char* szAddress1 )
{
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		pSite -> SetLegacyAddress1 ( szAddress1 );
	}
}

/**********************************************************************/

void CSiteCSVArray::SetLegacyAddress2 ( int nSiteIdx, const char* szAddress2 )
{
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		pSite -> SetLegacyAddress2 ( szAddress2 );
	}
}

/**********************************************************************/

void CSiteCSVArray::SetLegacyAddress3 ( int nSiteIdx, const char* szAddress3 )
{
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		pSite -> SetLegacyAddress3 ( szAddress3 );
	}
}

/**********************************************************************/

void CSiteCSVArray::SetLegacyPostcode ( int nSiteIdx, const char* szPostcode )
{
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		pSite -> SetLegacyPostcode ( szPostcode );
	}
}

/**********************************************************************/

void CSiteCSVArray::SetLegacyPhone ( int nSiteIdx, const char* szPhone )
{
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		pSite -> SetLegacyPhone ( szPhone );
	}
}

/**********************************************************************/

void CSiteCSVArray::SetLegacyFax ( int nSiteIdx, const char* szFax )
{
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		pSite -> SetLegacyFax ( szFax );
	}
}

/**********************************************************************/

void CSiteCSVArray::SetLegacyEmail ( int nSiteIdx, const char* szEmail )
{
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		pSite -> SetLegacyEmail ( szEmail );
	}
}

/**********************************************************************/

bool CSiteCSVArray::WriteSite ( int nSiteIdx )
{
	if ( nSiteIdx < 0 || nSiteIdx >= GetSize() )
		return FALSE;

	CSSFile fileSite;
	if ( fileSite.Open ( GetFilePathInfo( nSiteIdx ), "wb" ) == FALSE )
		return FALSE;

	fileSite.WriteLine ( GetCSVLine ( nSiteIdx ) );
	return TRUE;
}

/**********************************************************************/

const char* CSiteCSVArray::GetFolderPath ( int nSiteIdx )
{
	m_strFolderPath = "";
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		CSiteCSVRecord* pSite = GetSite ( nSiteIdx );
		m_strFolderPath.Format ( "DB%3.3d\\SITE%3.3d", pSite -> m_nDbNo, pSite -> m_nSiteNo );
		GetDataProgramPath( m_strFolderPath );
	}
	return m_strFolderPath;
}

/**********************************************************************/

const char* CSiteCSVArray::GetFolderPathDelete ( int nSiteIdx )
{
	m_strFolderPathDelete = GetFolderPath( nSiteIdx );
	m_strFolderPathDelete += "XXX";
	return m_strFolderPathDelete;
}

/**********************************************************************/

const char* CSiteCSVArray::GetFilePathInfo( int nSiteIdx )
{
	m_strFilePathInfo = "";
	if ( nSiteIdx >= 0 && nSiteIdx < GetSize() )
	{
		m_strFilePathInfo = GetFolderPath ( nSiteIdx );
		m_strFilePathInfo += "\\info.dat";
	}
	return m_strFilePathInfo;
}

/**********************************************************************/

void CSiteCSVArray::SortByName( CWordArray& arraySiteIdx, int nDbIdx )
{
	arraySiteIdx.RemoveAll();

	CReportConsolidationArray<CSortedLocation> arrayTemp;

	int nStart = 0;
	int nEnd = GetSize() - 1;

	if ( nDbIdx >= 0 && nDbIdx < dbDatabase.GetSize() )
		GetSiteIdxRange( nDbIdx, nStart, nEnd );

	for ( int nSiteIdx = nStart; nSiteIdx <= nEnd; nSiteIdx++ )
	{
		if ( EcrmanOptions.GetReportsLocationSortType() == 2 )
		{
			CSortedLocation item;
			item.m_strName = GetName( nSiteIdx );
			item.m_nIdx = nSiteIdx;
			arrayTemp.Consolidate( item );
		}
		else
		{
			CSortedLocation item;
			item.m_nDbNo = GetDbNo( nSiteIdx );
			item.m_strName = GetName( nSiteIdx );
			item.m_nIdx = nSiteIdx;
			arrayTemp.Consolidate( item );
		}
	}
	
	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CSortedLocation item;
		arrayTemp.GetAt( n, item );
		arraySiteIdx.Add( item.m_nIdx );
	}
}

/**********************************************************************/

void CSiteCSVArray::BuildSortIdxTables()
{
	m_arraySortIdxToSiteIdx.RemoveAll();
	m_arraySiteIdxToSortIdx.RemoveAll();

	SortByName( m_arraySortIdxToSiteIdx );

	for ( int n = 0; n < GetSize(); n++ )
		m_arraySiteIdxToSortIdx.Add(0);

	for ( int n = 0; n < m_arraySortIdxToSiteIdx.GetSize(); n++ )
	{
		int nSiteIdx = m_arraySortIdxToSiteIdx.GetAt(n);
		m_arraySiteIdxToSortIdx.SetAt( nSiteIdx, n );
	}
}

/**********************************************************************/

int CSiteCSVArray::GetSiteIdxFromSortIdx( int nSortIdx )
{
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
		return nSortIdx;

	if ( nSortIdx < 0 || nSortIdx >= m_arraySortIdxToSiteIdx.GetSize() )
		return nSortIdx;
	else
		return m_arraySortIdxToSiteIdx.GetAt( nSortIdx );
}

/**********************************************************************/

int CSiteCSVArray::GetSortIdxFromSiteIdx( int nSiteIdx )
{
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
		return nSiteIdx;

	if ( nSiteIdx < 0 || nSiteIdx >= m_arraySiteIdxToSortIdx.GetSize() )
		return nSiteIdx;
	else
		return m_arraySiteIdxToSortIdx.GetAt( nSiteIdx );
}

/**********************************************************************/

void CSiteCSVArray::GetAddressInfo( int nSiteIdx, CAddressInfo& info )
{
	info.m_strAddress1 = "";
	info.m_strAddress2 = "";
	info.m_strAddress3 = "";
	info.m_strPostcode = "";
	info.m_strPhone = "";
	info.m_strFax = "";
	info.m_strEmail = "";

	CFilenameUpdater FnUp( SysFiles::SiteAddress, nSiteIdx );
	
	CString strFilename = FnUp.GetFilenameToUse();
	if ( ::FileExists( strFilename ) == TRUE )
	{
		CSSFile file;
		if ( file.Open( strFilename, "rb" ) == TRUE )
		{
			CString strBuffer;
			file.ReadString( strBuffer );

			CCSV csv( strBuffer );

			if ( csv.GetInt(0) == 1 )
			{
				info.m_strAddress1 = csv.GetString(1);
				info.m_strAddress2 = csv.GetString(2);
				info.m_strAddress3 = csv.GetString(3);
				info.m_strPostcode = csv.GetString(4);
				info.m_strPhone = csv.GetString(5);
				info.m_strFax = csv.GetString(6);
				info.m_strEmail = csv.GetString(7);
			}
		}
	}
	else
	{
		info.m_strAddress1 = GetLegacyAddress1( nSiteIdx );
		info.m_strAddress2 = GetLegacyAddress2( nSiteIdx );
		info.m_strAddress3 = GetLegacyAddress3( nSiteIdx );
		info.m_strPostcode = GetLegacyPostcode( nSiteIdx );
		info.m_strPhone = GetLegacyPhone( nSiteIdx );
		info.m_strFax = GetLegacyFax( nSiteIdx );
		info.m_strEmail = GetLegacyEmail( nSiteIdx );
	}
}

/**********************************************************************/

void CSiteCSVArray::SetAddressInfo( int nSiteIdx, CAddressInfo& info )
{
	CFilenameUpdater FnUp( SysFiles::SiteAddress, nSiteIdx );
		
	CSSFile file;
	CString strFilename = FnUp.GetFilenameToUse();
	if ( file.Open( strFilename, "wb" ) == TRUE )
	{
		CCSV csv;
		csv.Add( 1 );
		csv.Add( info.m_strAddress1 );
		csv.Add( info.m_strAddress2 );
		csv.Add( info.m_strAddress3 );
		csv.Add( info.m_strPostcode );
		csv.Add( info.m_strPhone );
		csv.Add( info.m_strFax );
		csv.Add( info.m_strEmail );
		file.WriteLine( csv.GetLine() );
	}
}

/**********************************************************************/
