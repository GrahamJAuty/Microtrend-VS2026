/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "EcrmanOptionsIni.h"
#include "FileRemover.h"
//#include "globalfunctions.h"
#include "ReportConsolidationArray.h"
#include "SortedLocation.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ReportpointCSVArray.h"
/**********************************************************************/

CReportpointCSVRecord::CReportpointCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CReportpointCSVRecord::ClearRecord()
{
	m_nRpNo = 0;
	m_strName = "";
	m_nDbNo = 0;
}

/**********************************************************************/

void CReportpointCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetRpNo ( csv.GetInt(0) );
	CString strName = csv.GetString(1);
	SetDbNo ( csv.GetInt(2) );

	switch ( csv.GetInt(3) )
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	}

	SetName( strName );
}

/**********************************************************************/

void CReportpointCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
}

/**********************************************************************/
	
void CReportpointCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nRpNo );
	csv.Add ( m_strName );
	csv.Add ( m_nDbNo );
	csv.Add ( REPORTPOINT_VERSION );				
}

/**********************************************************************/

void CReportpointCSVRecord::SetName ( const char* szName )
{
	CString strName = szName;
	::TrimSpaces ( strName, FALSE );
	if ( strName == "" ) strName = GetSystemName();
	m_strName = strName;	 
}

/**********************************************************************/

const char* CReportpointCSVRecord::GetSystemName()
{
	m_strSystemName.Format ( "Report Point %d.%d", m_nDbNo, m_nRpNo );
	return m_strSystemName;
}

/**********************************************************************/

CReportpointCSVArray::CReportpointCSVArray() 
{
}

/**********************************************************************/

CReportpointCSVArray::~CReportpointCSVArray()
{
	RemoveAll();
}

/**********************************************************************/

void CReportpointCSVArray::RemoveAll()
{
	while ( GetSize() > 0 )
	{
		int nRpIdx = GetSize() - 1;
		CReportpointCSVRecord* pReportpoint = GetReportpoint ( nRpIdx );
		delete pReportpoint;
		RemoveAt ( nRpIdx );
	}
}

/**********************************************************************/

int CReportpointCSVArray::AddReportpoint ( int nDbNo )
{
	int nStartIdx, nEndIdx;
	FindReportpointByNumber ( nDbNo, 0, nStartIdx );
	FindReportpointByNumber ( nDbNo + 1, 0, nEndIdx );
	nEndIdx--;
	
	if ( nEndIdx >= nStartIdx + 998 )
		return -1;

	int nRpIdx = nStartIdx, nRpNo = 1;
	for ( ; nRpIdx <= nEndIdx; nRpIdx++, nRpNo++ )
		if ( GetRpNo ( nRpIdx ) != nRpNo )
			break;
		
	CReportpointCSVRecord* pReportpoint = new CReportpointCSVRecord;
	InsertAt ( nRpIdx, pReportpoint );
	SetRpNo ( nRpIdx, nRpNo );
	SetDbNo ( nRpIdx, nDbNo );
	SetName ( nRpIdx, "" );
	
	CString strFolderPath = GetFolderPath ( nRpIdx );
	::CreateSubdirectory ( strFolderPath );
	
	WriteReportpoint ( nRpIdx );
	return nRpIdx;
}

/**********************************************************************/

int CReportpointCSVArray::AddReportpoint( int nDbNo, int nRpNo, const char* szPath )
{
	int nRpIdx;
	if ( FindReportpointByNumber ( nDbNo, nRpNo, nRpIdx ) == FALSE )
	{
		CReportpointCSVRecord* pReportpoint = new CReportpointCSVRecord;
		
		CString strFileInfo;
		strFileInfo.Format ( "%sInfo.dat", szPath );

		CSSFile fileInfo;
		if ( fileInfo.Open( strFileInfo, "rb" ) )
		{
			CString strBuffer;
			fileInfo.ReadString( strBuffer );
			CCSV csv ( strBuffer );
			pReportpoint -> ConvertFromCSV ( csv ); 
			InsertAt ( nRpIdx, pReportpoint );
			SetRpNo ( nRpIdx, nRpNo );
			SetDbNo ( nRpIdx, nDbNo );
		}
		else
		{
			pReportpoint -> SetRpNo ( nRpNo );
			pReportpoint -> SetDbNo ( nDbNo );
			pReportpoint -> SetName ( "" );
			InsertAt ( nRpIdx, pReportpoint );
		}
	}
	return nRpIdx;
}

/**********************************************************************/

bool CReportpointCSVArray::RenameForDeletion( int nRpIdx )
{
	if ( nRpIdx < 0 || nRpIdx >= GetSize() )
		return FALSE;

	ClearDeletionFolder( nRpIdx );	

	if ( rename ( GetFolderPath( nRpIdx ), GetFolderPathDelete( nRpIdx ) ) != 0 )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

void CReportpointCSVArray::ClearReportpointFolder( int nRpIdx )
{
	CFileRemover FileRemover;
	CString strFolderPath = GetFolderPath( nRpIdx );
	FileRemover.RemoveFolder( strFolderPath );
}

/**********************************************************************/

void CReportpointCSVArray::ClearDeletionFolder( int nRpIdx )
{
	CFileRemover FileRemover;
	CString strFolderPath = GetFolderPathDelete( nRpIdx );
	FileRemover.RemoveFolder( strFolderPath );
}

/**********************************************************************/

void CReportpointCSVArray::DeleteReportpointByIndex ( int nRpIdx, bool bDeleteFiles )
{
	if ( nRpIdx >= 0 && nRpIdx < GetSize() )
	{
		if ( bDeleteFiles == TRUE )
		{
			ClearReportpointFolder( nRpIdx );
			ClearDeletionFolder( nRpIdx );
		}
		
		delete GetReportpoint ( nRpIdx );
		RemoveAt ( nRpIdx );
	}
}

/**********************************************************************/

void CReportpointCSVArray::DeleteReportpointByNumber ( int nDbNo, int nRpNo )
{
	int nRpIdx;
	if ( FindReportpointByNumber ( nDbNo, nRpNo, nRpIdx ) == TRUE )
		DeleteReportpointByIndex ( nRpIdx, TRUE );
}

/**********************************************************************/

bool CReportpointCSVArray::FindReportpointByNumber ( int nDbNo, int nRpNo, int& nRpIdx )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	int nCheckNo = ( nDbNo * 1000 ) + nRpNo;

	while ( nStart <= nEnd )
	{
		nRpIdx = ( nStart + nEnd ) / 2;
		int nArrayNo = ( GetDbNo ( nRpIdx ) * 1000 ) + GetRpNo ( nRpIdx );
		
		if ( nArrayNo == nCheckNo )
			return TRUE;
		else if ( nArrayNo < nCheckNo )
			nStart = nRpIdx + 1;
		else 
			nEnd = nRpIdx - 1;
	}
	nRpIdx = nStart;
	return FALSE;
}

/**********************************************************************/

int CReportpointCSVArray::GetRpNo ( int nRpIdx )
{
	int nRpNo = 0;
	if ( nRpIdx >= 0 && nRpIdx < GetSize() )
	{
		CReportpointCSVRecord* pReportpoint = GetReportpoint ( nRpIdx );
		nRpNo = pReportpoint -> m_nRpNo;
	}
	return nRpNo;
}

/**********************************************************************/

int CReportpointCSVArray::GetDbNo ( int nRpIdx )
{
	int nDbNo = 0;
	if ( nRpIdx >= 0 && nRpIdx < GetSize() )
	{
		CReportpointCSVRecord* pReportpoint = GetReportpoint ( nRpIdx );
		nDbNo = pReportpoint -> m_nDbNo;
	}
	return nDbNo;
}

/**********************************************************************/

const char* CReportpointCSVArray::GetName ( int nRpIdx )
{
	m_strName = "";
	if ( nRpIdx >= 0 && nRpIdx < GetSize() )
	{
		CReportpointCSVRecord* pReportpoint = GetReportpoint ( nRpIdx );
		m_strName = pReportpoint -> m_strName;
	}
	return m_strName;
}

/**********************************************************************/

const char* CReportpointCSVArray::GetSystemName( int nRpIdx )
{
	m_strSystemName = "";
	if ( nRpIdx >= 0 && nRpIdx < GetSize() )
	{
		CReportpointCSVRecord* pReportpoint = GetReportpoint ( nRpIdx );
		m_strSystemName = pReportpoint -> GetSystemName();
	}
	return m_strSystemName;
}

/**********************************************************************/

void CReportpointCSVArray::SetDbNo ( int nRpIdx, int nDbNo )
{
	if ( nRpIdx >= 0 && nRpIdx < GetSize() )
	{
		CReportpointCSVRecord* pReportpoint = GetReportpoint ( nRpIdx );
		pReportpoint -> SetDbNo ( nDbNo );
	}
}

/**********************************************************************/

void CReportpointCSVArray::SetRpNo ( int nRpIdx, int nRpNo )
{
	if ( nRpIdx >= 0 && nRpIdx < GetSize() )
	{
		CReportpointCSVRecord* pReportpoint = GetReportpoint ( nRpIdx );
		pReportpoint -> SetRpNo ( nRpNo );
	}
}

/**********************************************************************/

void CReportpointCSVArray::SetName ( int nRpIdx, const char* szName )
{
	if ( nRpIdx >= 0 && nRpIdx < GetSize() )
	{
		CReportpointCSVRecord* pReportpoint = GetReportpoint ( nRpIdx );
		pReportpoint -> SetName ( szName );
	}
}

/**********************************************************************/

const char* CReportpointCSVArray::GetFolderPath( int nRpIdx )
{
	m_strFolderPath = "";
	if ( nRpIdx >= 0 && nRpIdx < GetSize() )
	{
		CReportpointCSVRecord* pReportpoint = GetReportpoint ( nRpIdx );
		m_strFolderPath.Format ( "DB%3.3d\\RP%3.3d", pReportpoint -> m_nDbNo, pReportpoint -> m_nRpNo );
		GetDataProgramPath( m_strFolderPath );
	}
	return m_strFolderPath;	
}

/**********************************************************************/

const char* CReportpointCSVArray::GetFolderPathDelete( int nRpIdx )
{
	m_strFolderPathDelete = GetFolderPath( nRpIdx );
	m_strFolderPathDelete += "XXX";
	return m_strFolderPathDelete;	
}

/**********************************************************************/

const char* CReportpointCSVArray::GetFilePathInfo( int nRpIdx )
{
	m_strFilePathInfo = "";
	if ( nRpIdx >= 0 && nRpIdx < GetSize() )
	{
		m_strFilePathInfo = GetFolderPath ( nRpIdx );
		m_strFilePathInfo += "\\info.dat";
	}
	return m_strFilePathInfo;
}

/**********************************************************************/

const char* CReportpointCSVArray::GetCSVLine ( int nRpIdx )
{
	m_strCSVLine = "";
	if ( nRpIdx >= 0 && nRpIdx < GetSize() )
	{
		CReportpointCSVRecord* pReportpoint = GetReportpoint ( nRpIdx );
		
		CCSV csv;
		pReportpoint ->ConvertToCSV(csv);

		m_strCSVLine = csv.GetLine();
	}
	return m_strCSVLine;
}

/**********************************************************************/

void CReportpointCSVArray::PurgeDatabase( int nDbIdx )
{
	if ( nDbIdx < 0 || nDbIdx >= dbDatabase.GetSize() )
		return;

	int nStartIdx, nEndIdx;
	GetRpIdxRange ( nDbIdx, nStartIdx, nEndIdx );

	for ( int nRpIdx = nEndIdx; nRpIdx >= nStartIdx; nRpIdx-- )
		DeleteReportpointByIndex ( nRpIdx, FALSE );
}

/**********************************************************************/

void CReportpointCSVArray::GetRpIdxRange ( int nDbIdx, int& nStartIdx, int& nEndIdx )
{
	nStartIdx = 0;
	nEndIdx = -1;

	if ( nDbIdx < 0 || nDbIdx >= dbDatabase.GetSize() )
		return;

	int nDbNo = dbDatabase.GetDbNo( nDbIdx );

	FindReportpointByNumber ( nDbNo, 0, nStartIdx );
	FindReportpointByNumber ( nDbNo + 1, 0, nEndIdx );
	nEndIdx--;
}

/**********************************************************************/

bool CReportpointCSVArray::WriteReportpoint ( int nRpIdx )
{
	if ( nRpIdx < 0 || nRpIdx >= GetSize() )
		return FALSE;

	CSSFile fileReportpoint;
	if ( fileReportpoint.Open ( GetFilePathInfo( nRpIdx ), "wb" ) == FALSE )
		return FALSE;

	fileReportpoint.WriteLine ( GetCSVLine ( nRpIdx ) );
	return TRUE;
}

/**********************************************************************/

void CReportpointCSVArray::SortByName( CWordArray& arrayRpIdx, int nDbIdx )
{
	arrayRpIdx.RemoveAll();

	CReportConsolidationArray<CSortedLocation> arrayTemp;

	int nStartIdx = 0;
	int nEndIdx = GetSize() - 1;

	if ( nDbIdx >= 0 && nDbIdx < dbDatabase.GetSize() )
		GetRpIdxRange( nDbIdx, nStartIdx, nEndIdx );

	for ( int nRpIdx = nStartIdx; nRpIdx <= nEndIdx; nRpIdx++ )
	{
		if ( EcrmanOptions.GetReportsLocationSortType() == 2 )
		{
			CSortedLocation item;
			item.m_strName = GetName( nRpIdx );
			item.m_nIdx = nRpIdx;
			arrayTemp.Consolidate( item );
		}
		else
		{
			CSortedLocation item;
			item.m_nDbNo = GetDbNo( nRpIdx );
			item.m_strName = GetName( nRpIdx );
			item.m_nIdx = nRpIdx;
			arrayTemp.Consolidate( item );
		}
	}

	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CSortedLocation item;
		arrayTemp.GetAt( n, item );
		arrayRpIdx.Add( item.m_nIdx );
	}
}

/**********************************************************************/

void CReportpointCSVArray::BuildSortIdxTables()
{
	m_arraySortIdxToRpIdx.RemoveAll();
	m_arrayRpIdxToSortIdx.RemoveAll();

	SortByName( m_arraySortIdxToRpIdx );

	for ( int n = 0; n < GetSize(); n++ )
		m_arrayRpIdxToSortIdx.Add(0);

	for ( int n = 0; n < m_arraySortIdxToRpIdx.GetSize(); n++ )
	{
		int nRpIdx = m_arraySortIdxToRpIdx.GetAt(n);
		m_arrayRpIdxToSortIdx.SetAt( nRpIdx, n );
	}
}

/**********************************************************************/

int CReportpointCSVArray::GetRpIdxFromSortIdx( int nSortIdx )
{
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
		return nSortIdx;

	if ( nSortIdx < 0 || nSortIdx >= m_arraySortIdxToRpIdx.GetSize() )
		return nSortIdx;
	else
		return m_arraySortIdxToRpIdx.GetAt( nSortIdx );
}

/**********************************************************************/

int CReportpointCSVArray::GetSortIdxFromRpIdx( int nRpIdx )
{
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
		return nRpIdx;

	if ( nRpIdx < 0 || nRpIdx >= m_arrayRpIdxToSortIdx.GetSize() )
		return nRpIdx;
	else
		return m_arrayRpIdxToSortIdx.GetAt( nRpIdx );
}

/**********************************************************************/
