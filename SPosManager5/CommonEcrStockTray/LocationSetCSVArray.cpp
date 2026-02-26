/**********************************************************************/
 
/**********************************************************************/
#include "Consolidation.h"
#include "DatabaseCSVArray.h"
//#include "EcrmanOptionsIni.h"
#include "FileRemover.h"
//#include "globalfunctions.h"
#include "LocationCSVArray.h"
#include "ReportConsolidationArray.h"
#include "SortedLocation.h"
#include "SysInfo.h"
/**********************************************************************/
#include "LocationSetCSVArray.h"
/**********************************************************************/

CLocationSetCSVRecord::CLocationSetCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CLocationSetCSVRecord::ClearRecord()
{
	m_nLsNo = 0;
	m_strName = "";
	m_nDbNo = 0;
	m_arrayNwkLocNo.RemoveAll();
}

/**********************************************************************/

void CLocationSetCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetLsNo ( csv.GetInt(0) );
	CString strName = csv.GetString(1);
	SetDbNo ( csv.GetInt(2) );

	switch ( csv.GetInt(3) )
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	}

	SetName( strName );
}

/**********************************************************************/

void CLocationSetCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	CReportConsolidationArray<CSortedStringItem> arrayTemp;

	int nSize = csv.GetInt(4);

	for ( int n = 0; n < nSize; n++ )
	{
		int nNwkLocNo = csv.GetInt( n + 5 );

		if ( nNwkLocNo >= 1 && nNwkLocNo <= 999999 )
		{
			CString strNwkLocNo;
			strNwkLocNo.Format( "%6.6d", nNwkLocNo );

			CSortedStringItem item;
			item.m_strItem = strNwkLocNo;
			arrayTemp.Consolidate( item );
		}
	}

	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CSortedStringItem item;
		arrayTemp.GetAt( n, item );
		m_arrayNwkLocNo.Add( atoi( item.m_strItem ) );
	}
}

/**********************************************************************/
	
void CLocationSetCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nLsNo );
	csv.Add ( m_strName );
	csv.Add ( m_nDbNo );
	csv.Add ( LOCATIONSET_VERSION );

	int nSize = m_arrayNwkLocNo.GetSize();
	
	csv.Add( nSize );
	for ( int n = 0; n < nSize; n++ )
		csv.Add( (int) m_arrayNwkLocNo.GetAt(n) );
}

/**********************************************************************/

void CLocationSetCSVRecord::SetName ( const char* szName )
{
	CString strName = szName;
	::TrimSpaces ( strName, FALSE );
	if ( strName == "" ) strName = GetSystemName();
	m_strName = strName;	 
}

/**********************************************************************/

const char* CLocationSetCSVRecord::GetSystemName()
{
	m_strSystemName.Format ( "Location Set %d.%d", m_nDbNo, m_nLsNo );
	return m_strSystemName;
}

/**********************************************************************/

void CLocationSetCSVRecord::TidyNwkLocNoArray( CReportConsolidationArray<CSortedIntItem>& LocIdxArray )
{
	CReportConsolidationArray<CSortedIntItem> arrayTemp;

	for ( int n = 0; n < m_arrayNwkLocNo.GetSize(); n++ )
	{
		int nNwkLocNo = m_arrayNwkLocNo.GetAt(n);

		if ( ( nNwkLocNo >= 1 ) && ( nNwkLocNo <= 999999 ) )
		{
			CSortedIntItem item;
			item.m_nItem = nNwkLocNo;
			arrayTemp.Consolidate( item );
		}
	}
	
	LocIdxArray.RemoveAll();
	m_arrayNwkLocNo.RemoveAll();

	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CSortedIntItem item;
		arrayTemp.GetAt( n, item );
		
		int nNwkNo = item.m_nItem / 1000;
		int nLocNo = item.m_nItem % 1000;

		int nLocIdx;
		if ( dbLocation.FindLocationByNumber( nNwkNo, nLocNo, nLocIdx ) == TRUE )
		{
			m_arrayNwkLocNo.Add( item.m_nItem );
		
			item.m_nItem = nLocIdx;
			LocIdxArray.Consolidate( item );
		}
	}
}

/**********************************************************************/

void CLocationSetCSVRecord::SetNwkLocNoArray( CDWordArray& NwkLocNoArray )
{
	m_arrayNwkLocNo.RemoveAll();

	for ( int n = 0; n < NwkLocNoArray.GetSize(); n++ )
		m_arrayNwkLocNo.Add( NwkLocNoArray.GetAt(n) );

	CReportConsolidationArray<CSortedIntItem> arrayDummy;
	TidyNwkLocNoArray( arrayDummy );
}

/**********************************************************************/

void CLocationSetCSVRecord::RemoveLocation( int nNwkNo, int nLocNo )
{
	DWORD dwNwkLocNo = ( nNwkNo * 1000 ) + nLocNo;

	for ( int n = m_arrayNwkLocNo.GetSize() - 1; n >= 0; n-- )
		if ( m_arrayNwkLocNo.GetAt(n) == dwNwkLocNo )
			m_arrayNwkLocNo.RemoveAt(n);
}

/**********************************************************************/

CLocationSetCSVArray::CLocationSetCSVArray() 
{
}

/**********************************************************************/

CLocationSetCSVArray::~CLocationSetCSVArray()
{
	RemoveAll();
}

/**********************************************************************/

void CLocationSetCSVArray::RemoveAll()
{
	while ( GetSize() > 0 )
	{
		int nLsIdx = GetSize() - 1;
		CLocationSetCSVRecord* pLocationSet = GetLocationSet ( nLsIdx );
		delete pLocationSet;
		RemoveAt ( nLsIdx );
	}
}

/**********************************************************************/

int CLocationSetCSVArray::AddLocationSet ( int nDbNo )
{
	int nStartIdx, nEndIdx;
	FindLocationSetByNumber ( nDbNo, 0, nStartIdx );
	FindLocationSetByNumber ( nDbNo + 1, 0, nEndIdx );
	nEndIdx--;
	
	if ( nEndIdx >= nStartIdx + 998 )
		return -1;

	int nLsIdx = nStartIdx, nLsNo = 1;
	for ( ; nLsIdx <= nEndIdx; nLsIdx++, nLsNo++ )
		if ( GetLsNo ( nLsIdx ) != nLsNo )
			break;
		
	CLocationSetCSVRecord* pLocationSet = new CLocationSetCSVRecord;
	InsertAt ( nLsIdx, pLocationSet );
	SetLsNo ( nLsIdx, nLsNo );
	SetDbNo ( nLsIdx, nDbNo );
	SetName ( nLsIdx, "" );
	
	CString strFolderPath = GetFolderPath ( nLsIdx );
	::CreateSubdirectory ( strFolderPath );
	
	WriteLocationSet ( nLsIdx );
	return nLsIdx;
}

/**********************************************************************/

int CLocationSetCSVArray::AddLocationSet( int nDbNo, int nLsNo, const char* szPath )
{
	int nLsIdx;
	if ( FindLocationSetByNumber ( nDbNo, nLsNo, nLsIdx ) == FALSE )
	{
		CLocationSetCSVRecord* pLocationSet = new CLocationSetCSVRecord;
		
		CString strFileInfo;
		strFileInfo.Format ( "%sInfo.dat", szPath );

		CSSFile fileInfo;
		if ( fileInfo.Open( strFileInfo, "rb" ) )
		{
			CString strBuffer;
			fileInfo.ReadString( strBuffer );
			CCSV csv ( strBuffer );
			pLocationSet -> ConvertFromCSV ( csv ); 
			InsertAt ( nLsIdx, pLocationSet );
			SetLsNo ( nLsIdx, nLsNo );
			SetDbNo ( nLsIdx, nDbNo );
		}
		else
		{
			pLocationSet -> SetLsNo ( nLsNo );
			pLocationSet -> SetDbNo ( nDbNo );
			pLocationSet -> SetName ( "" );
			InsertAt ( nLsIdx, pLocationSet );
		}
	}
	return nLsIdx;
}

/**********************************************************************/

bool CLocationSetCSVArray::RenameForDeletion( int nLsIdx )
{
	if ( nLsIdx < 0 || nLsIdx >= GetSize() )
		return FALSE;

	ClearDeletionFolder( nLsIdx );	

	if ( rename ( GetFolderPath( nLsIdx ), GetFolderPathDelete( nLsIdx ) ) != 0 )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

void CLocationSetCSVArray::ClearLocationSetFolder( int nLsIdx )
{
	CFileRemover FileRemover;
	CString strFolderPath = GetFolderPath( nLsIdx );
	FileRemover.RemoveFolder( strFolderPath );
}

/**********************************************************************/

void CLocationSetCSVArray::ClearDeletionFolder( int nLsIdx )
{
	CFileRemover FileRemover;
	CString strFolderPath = GetFolderPathDelete( nLsIdx );
	FileRemover.RemoveFolder( strFolderPath );
}

/**********************************************************************/

void CLocationSetCSVArray::DeleteLocationSetByIndex ( int nLsIdx, bool bDeleteFiles )
{
	if ( nLsIdx >= 0 && nLsIdx < GetSize() )
	{
		if ( bDeleteFiles == TRUE )
		{
			ClearLocationSetFolder( nLsIdx );
			ClearDeletionFolder( nLsIdx );
		}
		
		delete GetLocationSet ( nLsIdx );
		RemoveAt ( nLsIdx );
	}
}

/**********************************************************************/

void CLocationSetCSVArray::DeleteLocationSetByNumber ( int nDbNo, int nLsNo )
{
	int nLsIdx;
	if ( FindLocationSetByNumber ( nDbNo, nLsNo, nLsIdx ) == TRUE )
		DeleteLocationSetByIndex ( nLsIdx, TRUE );
}

/**********************************************************************/

bool CLocationSetCSVArray::FindLocationSetByNumber ( int nDbNo, int nLsNo, int& nLsIdx )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	int nCheckNo = ( nDbNo * 1000 ) + nLsNo;

	while ( nStart <= nEnd )
	{
		nLsIdx = ( nStart + nEnd ) / 2;
		int nArrayNo = ( GetDbNo ( nLsIdx ) * 1000 ) + GetLsNo ( nLsIdx );
		
		if ( nArrayNo == nCheckNo )
			return TRUE;
		else if ( nArrayNo < nCheckNo )
			nStart = nLsIdx + 1;
		else 
			nEnd = nLsIdx - 1;
	}
	nLsIdx = nStart;
	return FALSE;
}

/**********************************************************************/

int CLocationSetCSVArray::GetLsNo ( int nLsIdx )
{
	int nLsNo = 0;
	if ( nLsIdx >= 0 && nLsIdx < GetSize() )
	{
		CLocationSetCSVRecord* pLocationSet = GetLocationSet ( nLsIdx );
		nLsNo = pLocationSet -> m_nLsNo;
	}
	return nLsNo;
}

/**********************************************************************/

int CLocationSetCSVArray::GetDbNo ( int nLsIdx )
{
	int nDbNo = 0;
	if ( nLsIdx >= 0 && nLsIdx < GetSize() )
	{
		CLocationSetCSVRecord* pLocationSet = GetLocationSet ( nLsIdx );
		nDbNo = pLocationSet -> m_nDbNo;
	}
	return nDbNo;
}

/**********************************************************************/

const char* CLocationSetCSVArray::GetName ( int nLsIdx )
{
	m_strName = "";
	if ( nLsIdx >= 0 && nLsIdx < GetSize() )
	{
		CLocationSetCSVRecord* pLocationSet = GetLocationSet ( nLsIdx );
		m_strName = pLocationSet -> m_strName;
	}
	return m_strName;
}

/**********************************************************************/

const char* CLocationSetCSVArray::GetSystemName( int nLsIdx )
{
	m_strSystemName = "";
	if ( nLsIdx >= 0 && nLsIdx < GetSize() )
	{
		CLocationSetCSVRecord* pLocationSet = GetLocationSet ( nLsIdx );
		m_strSystemName = pLocationSet -> GetSystemName();
	}
	return m_strSystemName;
}

/**********************************************************************/

int CLocationSetCSVArray::GetLocCount( int nLsIdx )
{
	int nCount = 0;
	if ( nLsIdx >= 0 && nLsIdx < GetSize() )
	{
		CLocationSetCSVRecord* pLocationSet = GetLocationSet ( nLsIdx );
		nCount = pLocationSet -> m_arrayNwkLocNo.GetSize();
	}
	return nCount;
}

/**********************************************************************/

int CLocationSetCSVArray::GetNwkLocNo( int nLsIdx, int n )
{
	int nNwkLocNo = 0;
	if ( nLsIdx >= 0 && nLsIdx < GetSize() )
	{
		CLocationSetCSVRecord* pLocationSet = GetLocationSet ( nLsIdx );
		if ( n >= 0 && n < pLocationSet -> m_arrayNwkLocNo.GetSize() )
			nNwkLocNo = pLocationSet -> m_arrayNwkLocNo.GetAt(n);
	}
	return nNwkLocNo;
}

/**********************************************************************/

void CLocationSetCSVArray::SetDbNo ( int nLsIdx, int nDbNo )
{
	if ( nLsIdx >= 0 && nLsIdx < GetSize() )
	{
		CLocationSetCSVRecord* pLocationSet = GetLocationSet ( nLsIdx );
		pLocationSet -> SetDbNo ( nDbNo );
	}
}

/**********************************************************************/

void CLocationSetCSVArray::SetLsNo ( int nLsIdx, int nLsNo )
{
	if ( nLsIdx >= 0 && nLsIdx < GetSize() )
	{
		CLocationSetCSVRecord* pLocationSet = GetLocationSet ( nLsIdx );
		pLocationSet -> SetLsNo ( nLsNo );
	}
}

/**********************************************************************/

void CLocationSetCSVArray::SetName ( int nLsIdx, const char* szName )
{
	if ( nLsIdx >= 0 && nLsIdx < GetSize() )
	{
		CLocationSetCSVRecord* pLocationSet = GetLocationSet ( nLsIdx );
		pLocationSet -> SetName ( szName );
	}
}

/**********************************************************************/

void CLocationSetCSVArray::TidyNwkLocNoArray( int nLsIdx, CReportConsolidationArray<CSortedIntItem>& LocIdxArray )
{
	LocIdxArray.RemoveAll();

	if ( nLsIdx >= 0 && nLsIdx < GetSize() )
	{
		CLocationSetCSVRecord* pLocationSet = GetLocationSet ( nLsIdx );
		pLocationSet -> TidyNwkLocNoArray( LocIdxArray );
	}
}

/**********************************************************************/

void CLocationSetCSVArray::SetNwkLocNoArray( int nLsIdx, CDWordArray& NwkLocNoArray )
{
	if ( nLsIdx >= 0 && nLsIdx < GetSize() )
	{
		CLocationSetCSVRecord* pLocationSet = GetLocationSet ( nLsIdx );
		pLocationSet -> SetNwkLocNoArray( NwkLocNoArray );
	}
}

/**********************************************************************/

void CLocationSetCSVArray::RemoveLocation( int nLsIdx, int nNwkNo, int nLocNo )
{
	if ( nLsIdx >= 0 && nLsIdx < GetSize() )
	{
		CLocationSetCSVRecord* pLocationSet = GetLocationSet ( nLsIdx );
		pLocationSet -> RemoveLocation( nNwkNo, nLocNo );
	}
}

/**********************************************************************/

const char* CLocationSetCSVArray::GetFolderPath( int nLsIdx )
{
	m_strFolderPath = "";
	if ( nLsIdx >= 0 && nLsIdx < GetSize() )
	{
		CLocationSetCSVRecord* pLocationSet = GetLocationSet ( nLsIdx );
		m_strFolderPath.Format ( "DB%3.3d\\LS%3.3d", 
			pLocationSet -> m_nDbNo, 
			pLocationSet -> m_nLsNo );

		GetDataProgramPath( m_strFolderPath );
	}
	return m_strFolderPath;	
}

/**********************************************************************/

const char* CLocationSetCSVArray::GetFolderPathDelete( int nLsIdx )
{
	m_strFolderPathDelete = GetFolderPath( nLsIdx );
	m_strFolderPathDelete += "XXX";
	return m_strFolderPathDelete;
}

/**********************************************************************/

const char* CLocationSetCSVArray::GetFilePathInfo( int nLsIdx )
{
	m_strFilePathInfo = "";
	if ( nLsIdx >= 0 && nLsIdx < GetSize() )
	{
		m_strFilePathInfo = GetFolderPath ( nLsIdx );
		m_strFilePathInfo += "\\info.dat";
	}
	return m_strFilePathInfo;
}

/**********************************************************************/

const char* CLocationSetCSVArray::GetCSVLine ( int nLsIdx )
{
	m_strCSVLine = "";
	if ( nLsIdx >= 0 && nLsIdx < GetSize() )
	{
		CLocationSetCSVRecord* pLocationSet = GetLocationSet ( nLsIdx );
		
		CCSV csv;
		pLocationSet ->ConvertToCSV(csv);

		m_strCSVLine = csv.GetLine();
	}
	return m_strCSVLine;
}

/**********************************************************************/

void CLocationSetCSVArray::PurgeDatabase( int nDbIdx )
{
	if ( nDbIdx < 0 || nDbIdx >= dbDatabase.GetSize() )
		return;

	int nStartIdx, nEndIdx;
	GetLsIdxRange ( nDbIdx, nStartIdx, nEndIdx );

	for ( int nLsIdx = nEndIdx; nLsIdx >= nStartIdx; nLsIdx-- )
		DeleteLocationSetByIndex ( nLsIdx, FALSE );
}

/**********************************************************************/

void CLocationSetCSVArray::GetLsIdxRange ( int nDbIdx, int& nStartIdx, int& nEndIdx )
{
	nStartIdx = 0;
	nEndIdx = -1;

	if ( nDbIdx < 0 || nDbIdx >= dbDatabase.GetSize() )
		return;

	int nDbNo = dbDatabase.GetDbNo( nDbIdx );

	FindLocationSetByNumber ( nDbNo, 0, nStartIdx );
	FindLocationSetByNumber ( nDbNo + 1, 0, nEndIdx );
	nEndIdx--;
}

/**********************************************************************/

bool CLocationSetCSVArray::WriteLocationSet ( int nLsIdx )
{
	if ( nLsIdx < 0 || nLsIdx >= GetSize() )
		return FALSE;

	CSSFile fileLocationSet;
	if ( fileLocationSet.Open ( GetFilePathInfo( nLsIdx ), "wb" ) == FALSE )
		return FALSE;

	fileLocationSet.WriteLine ( GetCSVLine ( nLsIdx ) );
	return TRUE;
}

/**********************************************************************/

void CLocationSetCSVArray::SortByName( CWordArray& arrayLsIdx, int nDbIdx )
{
	arrayLsIdx.RemoveAll();

	CReportConsolidationArray<CSortedLocation> arrayTemp;

	int nStart = 0;
	int nEnd = GetSize() - 1;

	if ( nDbIdx >= 0 && nDbIdx < dbDatabase.GetSize() )
		GetLsIdxRange( nDbIdx, nStart, nEnd );

	for ( int nLsIdx = nStart; nLsIdx <= nEnd; nLsIdx++ )
	{
		if ( EcrmanOptions.GetReportsLocationSortType() == 2 )
		{
			CSortedLocation item;
			item.m_strName = GetName( nLsIdx );
			item.m_nIdx = nLsIdx;
			arrayTemp.Consolidate( item );
		}
		else
		{
			CSortedLocation item;
			item.m_nDbNo = GetDbNo( nLsIdx );
			item.m_strName = GetName( nLsIdx );
			item.m_nIdx = nLsIdx;
			arrayTemp.Consolidate( item );
		}
	}

	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CSortedLocation item;
		arrayTemp.GetAt( n, item );
		arrayLsIdx.Add( item.m_nIdx );
	}
}

/**********************************************************************/

