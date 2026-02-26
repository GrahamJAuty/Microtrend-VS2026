 /**********************************************************************/
#include "FileRemover.h"
#include "KeyboardSetCSVArray.h"
#include "LocationCSVArray.h"
#include "LocationSetCSVArray.h"
#include "MessageLogger.h"
#include "..\CommonEcrStock\PriceSetItemManager.h"
#include "ReportpointCSVArray.h"
#include "ReportConsolidationArray.h"
#include "..\SPosStockManager5\SiteCSVArray.h"
#include "SortedLocation.h"
#include "..\SPosStockManager5\StockpointCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DatabaseCSVArray.h"
/**********************************************************************/

CDatabaseCSVRecord::CDatabaseCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CDatabaseCSVRecord::ClearRecord()
{
	m_nDbNo = 0;
	m_strName = "";
	m_strLegacyAddress1 = "";
	m_strLegacyAddress2 = "";
	m_strLegacyAddress3 = "";
	m_strLegacyPostcode = "";
	m_strLegacyPhone = "";
	m_strLegacyFax = "";
	m_strLegacyEmail = "";
	m_strTaxNo = "";
}

/**********************************************************************/

void CDatabaseCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nDbNo );
	csv.Add ( m_strName );
	csv.Add ( DATABASE_VERSION );				
	csv.Add ( m_strLegacyAddress1 );
	csv.Add ( m_strLegacyAddress2 );
	csv.Add ( m_strLegacyAddress3 );
	csv.Add ( m_strLegacyPostcode );
	csv.Add ( m_strLegacyPhone );
	csv.Add ( m_strLegacyFax );
	csv.Add ( m_strLegacyEmail );
	csv.Add ( m_strTaxNo );
}

/**********************************************************************/

void CDatabaseCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord(); 

	SetDbNo ( csv.GetInt(0) );
	SetName ( csv.GetString(1) );

	switch ( csv.GetInt(2) )
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	case 2:	V2ConvertFromCSV ( csv );	break;
	}
}

/**********************************************************************/

void CDatabaseCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetLegacyAddress1 ( csv.GetString(3) );
	SetLegacyAddress2 ( csv.GetString(4) );
	SetLegacyAddress3 ( csv.GetString(5) );
	SetLegacyPostcode ( csv.GetString(6) );
	SetLegacyPhone ( csv.GetString(7) );
	SetLegacyFax ( csv.GetString(8) );
	SetLegacyEmail ( csv.GetString(9) );
	SetTaxNo ( csv.GetString(10) );
	//IGNORE LEGACY WAREHOUSE FIELDS IN V1
}

/**********************************************************************/

void CDatabaseCSVRecord::V2ConvertFromCSV ( CCSV& csv )
{
	SetLegacyAddress1 ( csv.GetString(3) );
	SetLegacyAddress2 ( csv.GetString(4) );
	SetLegacyAddress3 ( csv.GetString(5) );
	SetLegacyPostcode ( csv.GetString(6) );
	SetLegacyPhone ( csv.GetString(7) );
	SetLegacyFax ( csv.GetString(8) );
	SetLegacyEmail ( csv.GetString(9) );
	SetTaxNo ( csv.GetString(10) );
	//ANY NEW FIELDS WILL BE ADDED TO V2
}

/**********************************************************************/

void CDatabaseCSVRecord::SetName ( const char* szName )
{
	CString strName = szName;
	::TrimSpaces ( strName, TRUE );
	if ( strName == "" ) strName = GetSystemName();
	m_strName = strName; 
}

/**********************************************************************/

const char* CDatabaseCSVRecord::GetSystemName()
{
	m_strSystemName.Format ( "Database %d", m_nDbNo );
	return m_strSystemName;
}

/**********************************************************************/

CDatabaseCSVArray::CDatabaseCSVArray()
{
}

/**********************************************************************/

CDatabaseCSVArray::~CDatabaseCSVArray()
{
	RemoveAll();
}

/**********************************************************************/

void CDatabaseCSVArray::RemoveAll()
{
	while ( GetSize() > 0 )
	{
		int nDbIdx = GetSize() - 1;
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		delete pDatabase;
		RemoveAt ( nDbIdx );
	}
}

/**********************************************************************/

int CDatabaseCSVArray::AddDatabase( int nDbNo, const char* szPath )
{
	int nDbIdx;
	if ( FindDatabaseByNumber ( nDbNo, nDbIdx ) == FALSE )
	{
		CDatabaseCSVRecord*  pDatabase = new CDatabaseCSVRecord;
		
		CString strFileInfo;
		strFileInfo.Format ( "%sInfo.dat", szPath );

		CSSFile fileInfo;
		if ( fileInfo.Open( strFileInfo, "rb" ) )
		{
			CString strBuffer;
			fileInfo.ReadString( strBuffer );
			CCSV csv ( strBuffer );
			pDatabase -> ConvertFromCSV ( csv ); 
			InsertAt ( nDbIdx, pDatabase );
			SetDbNo ( nDbIdx, nDbNo );
		}
		else
		{
			pDatabase -> SetDbNo ( nDbNo );
			pDatabase -> SetName ( "" );
			InsertAt ( nDbIdx, pDatabase );
		}
	}
	return nDbIdx;
}

/**********************************************************************/

int CDatabaseCSVArray::AddDatabase()
{
	if ( GetSize() >= SysInfo.GetMaxDatabases() )
		return -1;

	int nDbIdx = 0, nDbNo = 1;
	for ( ; nDbIdx < GetSize(); nDbIdx++, nDbNo++ )
		if ( GetDbNo ( nDbIdx ) != nDbNo )
			break;

	CDatabaseCSVRecord*  pDatabase = new CDatabaseCSVRecord;
	InsertAt ( nDbIdx, pDatabase );
	SetDbNo ( nDbIdx, nDbIdx + 1 );
	SetName ( nDbIdx, "" );

	::CreateSubdirectory( dbDatabase.GetFolderPathData( nDbIdx ) );
	::CreateSubdirectory( dbDatabase.GetFolderPathSysset( nDbIdx ) );

	WriteDatabase ( nDbIdx );

	return nDbIdx;
}

/**********************************************************************/

bool CDatabaseCSVArray::RenameForDeletion( int nDbIdx )
{
	if ( nDbIdx < 0 || nDbIdx >= GetSize() )
		return FALSE;

	ClearDeletionFolder( nDbIdx );	

	if ( rename ( GetFolderPathData( nDbIdx ), GetFolderPathDataDelete( nDbIdx ) ) != 0 )
		return FALSE;

	rename ( GetFolderPathSysset( nDbIdx ), GetFolderPathSyssetDelete( nDbIdx ) );
		
	return TRUE;
}

/**********************************************************************/

void CDatabaseCSVArray::DeleteDatabaseByIndex ( int nDbIdx )
{
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		int nDbNo = GetDbNo( nDbIdx );

		dbLocation.PurgeDatabase( nDbIdx );
		dbReportpoint.PurgeDatabase ( nDbIdx );
		dbLocationSet.PurgeDatabase ( nDbIdx );
		dbKeyboardSet.PurgeDatabase ( nDbIdx );

#ifdef STOCKMAN_SYSTEM
		dbStockpoint.PurgeDatabase( nDbIdx );
		dbSite.PurgeDatabase ( nDbIdx );
#endif

		ClearDatabaseFolder( nDbIdx );
		ClearDeletionFolder( nDbIdx );
		
		delete GetDatabase ( nDbIdx );
		RemoveAt ( nDbIdx );
	}
}

/**********************************************************************/

void CDatabaseCSVArray::ClearDatabaseFolder( int nDbIdx )
{
	CFileRemover FileRemover;
	FileRemover.RemoveFolder( dbDatabase.GetFolderPathData( nDbIdx ) );
	FileRemover.RemoveFolder( dbDatabase.GetFolderPathSysset( nDbIdx ) );
}

/**********************************************************************/

void CDatabaseCSVArray::ClearDeletionFolder( int nDbIdx )
{
	CFileRemover FileRemover;
	FileRemover.RemoveFolder( dbDatabase.GetFolderPathDataDelete( nDbIdx ) );
	FileRemover.RemoveFolder( dbDatabase.GetFolderPathSyssetDelete( nDbIdx ) );
}

/**********************************************************************/

void CDatabaseCSVArray::DeleteDatabaseByNumber ( int nDbNo )
{
	int nDbIdx;

	if ( FindDatabaseByNumber ( nDbNo, nDbIdx ) == TRUE )
		DeleteDatabaseByIndex ( nDbIdx );
}

/**********************************************************************/

bool CDatabaseCSVArray::FindDatabaseByNumber( int nDbNo, int& nDbIdx )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nDbIdx = ( nStart + nEnd ) / 2;
		int nArrayDbNo = GetDbNo ( nDbIdx );
		
		if ( nArrayDbNo == nDbNo )
			return TRUE;
		else if ( nArrayDbNo < nDbNo )
			nStart = nDbIdx + 1;
		else 
			nEnd = nDbIdx - 1;
	}
	nDbIdx = nStart;
	return FALSE;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetCSVLine ( int nDbIdx )
{
	m_strCSVLine = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		
		CCSV csv;
		pDatabase ->ConvertToCSV(csv);

		m_strCSVLine = csv.GetLine();
	}
	return m_strCSVLine;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetName ( int nDbIdx )
{
	m_strName = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		m_strName = pDatabase -> m_strName;
	}
	return m_strName;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetSystemName ( int nDbIdx )
{
	m_strSystemName = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		m_strSystemName = pDatabase -> GetSystemName();
	}
	return m_strSystemName;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetExtendedSystemName ( int nDbIdx )
{
	GetSystemName( nDbIdx );
	GetName( nDbIdx );

	if ( m_strSystemName != m_strName )
	{
		m_strSystemName += " (";
		m_strSystemName += m_strName;
		m_strSystemName += ")";
	}

	return m_strSystemName;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetLegacyAddress1 ( int nDbIdx )
{
	m_strLegacyAddress1 = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		m_strLegacyAddress1 = pDatabase -> m_strLegacyAddress1;
	}
	return m_strLegacyAddress1;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetLegacyAddress2 ( int nDbIdx )
{
	m_strLegacyAddress2 = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		m_strLegacyAddress2 = pDatabase -> m_strLegacyAddress2;
	}
	return m_strLegacyAddress2;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetLegacyAddress3 ( int nDbIdx )
{
	m_strLegacyAddress3 = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		m_strLegacyAddress3 = pDatabase -> m_strLegacyAddress3;
	}
	return m_strLegacyAddress3;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetLegacyPhone ( int nDbIdx )
{
	m_strLegacyPhone = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		m_strLegacyPhone = pDatabase -> m_strLegacyPhone;
	}
	return m_strLegacyPhone;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetLegacyPostcode ( int nDbIdx )
{
	m_strLegacyPostcode = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		m_strLegacyPostcode = pDatabase -> m_strLegacyPostcode;
	}
	return m_strLegacyPostcode;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetLegacyFax ( int nDbIdx )
{
	m_strLegacyFax = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		m_strLegacyFax = pDatabase -> m_strLegacyFax;
	}
	return m_strLegacyFax;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetLegacyEmail ( int nDbIdx )
{
	m_strLegacyEmail = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		m_strLegacyEmail = pDatabase -> m_strLegacyEmail;
	}
	return m_strLegacyEmail;
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetTaxNo ( int nDbIdx )
{
	m_strTaxNo = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		m_strTaxNo = pDatabase -> m_strTaxNo;
	}
	return m_strTaxNo;
}

/**********************************************************************/

int CDatabaseCSVArray::GetDbNo ( int nDbIdx )
{
	int nDbNo = 0;
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		nDbNo = pDatabase -> m_nDbNo;
	}
	return nDbNo;
}

/**********************************************************************/

void CDatabaseCSVArray::SetDbNo ( int nDbIdx, int nDbNo )
{
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		pDatabase -> SetDbNo ( nDbNo );
	}
}

/**********************************************************************/

void CDatabaseCSVArray::SetName ( int nDbIdx, const char* szName )
{
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		pDatabase -> SetName ( szName );
	}
}

/**********************************************************************/

void CDatabaseCSVArray::SetLegacyAddress1 ( int nDbIdx, const char* szAddress1 )
{
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		pDatabase -> SetLegacyAddress1 ( szAddress1 );
	}
}

/**********************************************************************/

void CDatabaseCSVArray::SetLegacyAddress2 ( int nDbIdx, const char* szAddress2 )
{
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		pDatabase -> SetLegacyAddress2 ( szAddress2 );
	}
}

/**********************************************************************/

void CDatabaseCSVArray::SetLegacyAddress3 ( int nDbIdx, const char* szAddress3 )
{
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		pDatabase -> SetLegacyAddress3 ( szAddress3 );
	}
}

/**********************************************************************/

void CDatabaseCSVArray::SetLegacyPostcode ( int nDbIdx, const char* szPostcode )
{
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		pDatabase -> SetLegacyPostcode ( szPostcode );
	}
}

/**********************************************************************/

void CDatabaseCSVArray::SetLegacyPhone ( int nDbIdx, const char* szPhone )
{
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		pDatabase -> SetLegacyPhone ( szPhone );
	}
}

/**********************************************************************/

void CDatabaseCSVArray::SetLegacyFax ( int nDbIdx, const char* szFax )
{
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		pDatabase -> SetLegacyFax ( szFax );
	}
}

/**********************************************************************/

void CDatabaseCSVArray::SetLegacyEmail ( int nDbIdx, const char* szEmail )
{
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		pDatabase -> SetLegacyEmail ( szEmail );
	}
}

/**********************************************************************/

void CDatabaseCSVArray::SetTaxNo ( int nDbIdx, const char* szTaxNo )
{
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		CDatabaseCSVRecord* pDatabase = GetDatabase ( nDbIdx );
		pDatabase -> SetTaxNo ( szTaxNo );
	}
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetFilePathInfo( int nDbIdx )
{
	m_strFilePathInfo = "";
	if ( nDbIdx >= 0 && nDbIdx < GetSize() )
	{
		m_strFilePathInfo = GetFolderPathData ( nDbIdx );
		m_strFilePathInfo += "\\info.dat";
	}
	return m_strFilePathInfo;
}

/**********************************************************************/

void CDatabaseCSVArray::LogMessage ( const char* szMessage )
{
	CMessageLoggerInfo info;
	info.m_strMessage = szMessage;
	info.m_nHideLevel = 2;

	MessageLogger.LogSchedulerMessage ( info, FALSE, TRUE, FALSE );	
}

/**********************************************************************/

const char* CDatabaseCSVArray::GetDatabaseTitle ( int nDbIdx, bool bGlobal, bool bBrackets )
{
	m_strDatabaseTitle = "";
	if (SysInfo.GetMaxDatabases() > 1)
	{
		if (bGlobal == TRUE)
		{
			m_strDatabaseTitle = "Global";
		}
		else if ((nDbIdx >= 0) && (nDbIdx < GetSize()))
		{
			m_strDatabaseTitle.Format("%s", GetName(nDbIdx));
		}
	}
	else if ((nDbIdx >= 0) && (nDbIdx < GetSize()))
	{
		m_strDatabaseTitle.Format("%s", GetName(nDbIdx));
	}

	if ( bBrackets == TRUE )
	{
		CString strTemp = "(";
		strTemp += m_strDatabaseTitle;
		strTemp += ")";
		m_strDatabaseTitle = strTemp;
	}

	return m_strDatabaseTitle;
}

/**********************************************************************/

bool CDatabaseCSVArray::WriteDatabase ( int nDbIdx )
{
	if (nDbIdx < 0 || nDbIdx >= GetSize())
	{
		return FALSE;
	}

	CSSFile fileDatabase;
	if (fileDatabase.Open(GetFilePathInfo(nDbIdx), "wb") == FALSE)
	{
		return FALSE;
	}

	fileDatabase.WriteLine ( GetCSVLine ( nDbIdx ) );
	return TRUE;
}

/**********************************************************************/

void CDatabaseCSVArray::LoadDatabaseBuffer( int nDbIdx, CDatabaseCSVArray& buffer )
{
	if (nDbIdx < 0 || nDbIdx >= GetSize())
	{
		return;
	}

	if (buffer.GetSize() != 1)
	{
		return;
	}

	CDatabaseCSVRecord* pDatabase;
	CDatabaseCSVRecord* pBuffer;

	pDatabase = GetDatabase( nDbIdx );
	pBuffer = buffer.GetDatabase( 0 );

	CCSV csv;
	pDatabase -> ConvertToCSV( csv );
	pBuffer -> ConvertFromCSV( csv );
}

/**********************************************************************/

void CDatabaseCSVArray::SaveDatabaseBuffer(int nDbIdx, CDatabaseCSVArray& buffer)
{
	if (nDbIdx < 0 || nDbIdx >= GetSize())
		return;

	if (buffer.GetSize() != 1)
		return;

	CDatabaseCSVRecord* pDatabase;
	CDatabaseCSVRecord* pBuffer;

	pDatabase = GetDatabase(nDbIdx);
	pBuffer = buffer.GetDatabase(0);

	CCSV csv;
	pBuffer->ConvertToCSV(csv);
	pDatabase->ConvertFromCSV(csv);

	CAddressInfo infoAddress;
	infoAddress.m_strAddress1 = buffer.GetLegacyAddress1(0);
	infoAddress.m_strAddress2 = buffer.GetLegacyAddress2(0);
	infoAddress.m_strAddress3 = buffer.GetLegacyAddress3(0);
	infoAddress.m_strPostcode = buffer.GetLegacyPostcode(0);
	infoAddress.m_strPhone = buffer.GetLegacyPhone(0);
	infoAddress.m_strFax = buffer.GetLegacyFax(0);
	infoAddress.m_strEmail = buffer.GetLegacyEmail(0);

	dbDatabase.SetAddressInfo(nDbIdx, infoAddress);
}

/**********************************************************************/

void CDatabaseCSVArray::SortByName( CWordArray& arrayDbIdx )
{
	arrayDbIdx.RemoveAll();

	CReportConsolidationArray<CSortedLocation> arrayTemp;

	for ( int nDbIdx = 0; nDbIdx < GetSize(); nDbIdx++ )
	{
		CSortedLocation item;
		item.m_strName = GetName( nDbIdx );
		item.m_nIdx = nDbIdx;
		arrayTemp.Consolidate( item );
	}

	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CSortedLocation item;
		arrayTemp.GetAt( n, item );
		arrayDbIdx.Add( item.m_nIdx );
	}
}

/**********************************************************************/

void CDatabaseCSVArray::BuildSortIdxTables()
{
	m_arraySortIdxToDbIdx.RemoveAll();
	m_arrayDbIdxToSortIdx.RemoveAll();

	SortByName( m_arraySortIdxToDbIdx );

	for ( int n = 0; n < GetSize(); n++ )
		m_arrayDbIdxToSortIdx.Add(0);

	for ( int n = 0; n < m_arraySortIdxToDbIdx.GetSize(); n++ )
	{
		int nDbIdx = m_arraySortIdxToDbIdx.GetAt(n);
		m_arrayDbIdxToSortIdx.SetAt( nDbIdx, n );
	}
}

/**********************************************************************/

int CDatabaseCSVArray::GetDbIdxFromSortIdx( int nSortIdx )
{
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
		return nSortIdx;

	if ( nSortIdx < 0 || nSortIdx >= m_arraySortIdxToDbIdx.GetSize() )
		return nSortIdx;
	else
		return m_arraySortIdxToDbIdx.GetAt( nSortIdx );
}

/**********************************************************************/

int CDatabaseCSVArray::GetSortIdxFromDbIdx( int nDbIdx )
{
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
		return nDbIdx;

	if ( nDbIdx < 0 || nDbIdx >= m_arrayDbIdxToSortIdx.GetSize() )
		return nDbIdx;
	else
		return m_arrayDbIdxToSortIdx.GetAt( nDbIdx );
}

/**********************************************************************/

void CDatabaseCSVArray::SortDbListByName( CArray<int,int>& arrayDbIdx )
{
	CReportConsolidationArray<CSortedLocation> arrayTemp;

	switch( EcrmanOptions.GetReportsLocationSortType() )
	{
	case 1:
	case 2:
		{	
			for ( int n = 0; n < arrayDbIdx.GetSize(); n++ )
			{
				int nDbIdx = arrayDbIdx.GetAt( n );

				if ( ( nDbIdx >= 0 ) && ( nDbIdx < GetSize() ) )
				{
					CSortedLocation item;
					item.m_nDbNo = 0;
					item.m_nBlock = 0;
					item.m_strName = GetName( nDbIdx );
					item.m_nIdx = nDbIdx;
					arrayTemp.Consolidate( item );
				}
			}
		}
		break;

	case 0:
	default:
		for ( int n = 0; n < arrayDbIdx.GetSize(); n++ )
		{
			int nDbIdx = arrayDbIdx.GetAt( n );

			if ( ( nDbIdx >= 0 ) && ( nDbIdx < GetSize() ) )
			{
				CSortedLocation item;
				item.m_nDbNo = 0;
				item.m_nBlock = 0;
				item.m_nIdx = nDbIdx;
				arrayTemp.Consolidate( item );
			}
		}
		break;
	}

	arrayDbIdx.RemoveAll();
	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CSortedLocation item;
		arrayTemp.GetAt( n, item );
		arrayDbIdx.Add( item.m_nIdx );
	}
}

/**********************************************************************/

void CDatabaseCSVArray::GetAddressInfo( int nDbIdx, CAddressInfo& info )
{
	info.m_strAddress1 = "";
	info.m_strAddress2 = "";
	info.m_strAddress3 = "";
	info.m_strPostcode = "";
	info.m_strPhone = "";
	info.m_strFax = "";
	info.m_strEmail = "";

	CFilenameUpdater FnUp( SysFiles::DbAddress, nDbIdx );
	
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
		info.m_strAddress1 = GetLegacyAddress1( nDbIdx );
		info.m_strAddress2 = GetLegacyAddress2( nDbIdx );
		info.m_strAddress3 = GetLegacyAddress3( nDbIdx );
		info.m_strPostcode = GetLegacyPostcode( nDbIdx );
		info.m_strPhone = GetLegacyPhone( nDbIdx );
		info.m_strFax = GetLegacyFax( nDbIdx );
		info.m_strEmail = GetLegacyEmail( nDbIdx );
	}
}

/**********************************************************************/

void CDatabaseCSVArray::SetAddressInfo( int nDbIdx, CAddressInfo& info )
{
	CFilenameUpdater FnUp( SysFiles::DbAddress, nDbIdx );
		
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
