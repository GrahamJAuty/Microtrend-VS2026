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
#include "KeyboardSetCSVArray.h"
/**********************************************************************/

CKeyboardSetCSVRecord::CKeyboardSetCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CKeyboardSetCSVRecord::ClearRecord()
{
	m_nKbsNo = 0;
	m_strName = "";
	m_nDbNo = 0;
}

/**********************************************************************/

void CKeyboardSetCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetKbsNo ( csv.GetInt(0) );
	CString strName = csv.GetString(1);
	SetDbNo ( csv.GetInt(2) );

	switch ( csv.GetInt(3) )
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	}

	SetName( strName );
}

/**********************************************************************/

void CKeyboardSetCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
}

/**********************************************************************/
	
void CKeyboardSetCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nKbsNo );
	csv.Add ( m_strName );
	csv.Add ( m_nDbNo );
	csv.Add ( KEYBOARDSET_VERSION );				
}

/**********************************************************************/

void CKeyboardSetCSVRecord::SetName ( const char* szName )
{
	CString strName = szName;
	::TrimSpaces ( strName, FALSE );
	if ( strName == "" ) strName = GetSystemName();
	m_strName = strName;	 
}

/**********************************************************************/

const char* CKeyboardSetCSVRecord::GetSystemName()
{
	m_strSystemName.Format ( "Keyboard Set %d.%d", m_nDbNo, m_nKbsNo );
	return m_strSystemName;
}

/**********************************************************************/

CKeyboardSetCSVArray::CKeyboardSetCSVArray() 
{
}

/**********************************************************************/

CKeyboardSetCSVArray::~CKeyboardSetCSVArray()
{
	RemoveAll();
}

/**********************************************************************/

void CKeyboardSetCSVArray::RemoveAll()
{
	while ( GetSize() > 0 )
	{
		int nKbsIdx = GetSize() - 1;
		CKeyboardSetCSVRecord* pKeyboardSet = GetKeyboardSet ( nKbsIdx );
		delete pKeyboardSet;
		RemoveAt ( nKbsIdx );
	}
}

/**********************************************************************/

int CKeyboardSetCSVArray::AddKeyboardSet ( int nDbNo )
{
	int nStartIdx, nEndIdx;
	FindKeyboardSetByNumber ( nDbNo, 0, nStartIdx );
	FindKeyboardSetByNumber ( nDbNo + 1, 0, nEndIdx );
	nEndIdx--;
	
	if ( nEndIdx >= nStartIdx + MAX_KEYBOARD_SETS_PER_DATABASE )
		return -1;

	int nKbsIdx = nStartIdx, nKbsNo = 1;
	for ( ; nKbsIdx <= nEndIdx; nKbsIdx++, nKbsNo++ )
		if ( GetKbsNo ( nKbsIdx ) != nKbsNo )
			break;

	if ( nKbsNo > MAX_KEYBOARD_SETS_PER_DATABASE )
		return -1;
		
	CKeyboardSetCSVRecord* pKeyboardSet = new CKeyboardSetCSVRecord;
	InsertAt ( nKbsIdx, pKeyboardSet );
	SetKbsNo ( nKbsIdx, nKbsNo );
	SetDbNo ( nKbsIdx, nDbNo );
	SetName ( nKbsIdx, "" );
	
	CString strFolderPath = GetFolderPath ( nKbsIdx );
	::CreateSubdirectory ( strFolderPath );
	
	WriteKeyboardSet ( nKbsIdx );
	return nKbsIdx;
}

/**********************************************************************/

int CKeyboardSetCSVArray::AddKeyboardSet( int nDbNo, int nKbsNo, const char* szPath )
{
	int nKbsIdx;
	if ( FindKeyboardSetByNumber ( nDbNo, nKbsNo, nKbsIdx ) == FALSE )
	{
		CKeyboardSetCSVRecord* pKeyboardSet = new CKeyboardSetCSVRecord;
		
		CString strFileInfo;
		strFileInfo.Format ( "%sInfo.dat", szPath );

		CSSFile fileInfo;
		if ( fileInfo.Open( strFileInfo, "rb" ) )
		{
			CString strBuffer;
			fileInfo.ReadString( strBuffer );
			CCSV csv ( strBuffer );
			pKeyboardSet -> ConvertFromCSV ( csv ); 
			InsertAt ( nKbsIdx, pKeyboardSet );
			SetKbsNo ( nKbsIdx, nKbsNo );
			SetDbNo ( nKbsIdx, nDbNo );
		}
		else
		{
			pKeyboardSet -> SetKbsNo ( nKbsNo );
			pKeyboardSet -> SetDbNo ( nDbNo );
			pKeyboardSet -> SetName ( "" );
			InsertAt ( nKbsIdx, pKeyboardSet );
		}
	}
	return nKbsIdx;
}

/**********************************************************************/

bool CKeyboardSetCSVArray::RenameForDeletion( int nKbsIdx )
{
	if ( nKbsIdx < 0 || nKbsIdx >= GetSize() )
		return FALSE;

	ClearDeletionFolder( nKbsIdx );	

	if ( rename ( GetFolderPath( nKbsIdx ), GetFolderPathDelete( nKbsIdx ) ) != 0 )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

void CKeyboardSetCSVArray::ClearKeyboardSetFolder( int nKbsIdx )
{
	CFileRemover FileRemover;
	CString strFolderPath = GetFolderPath( nKbsIdx );
	FileRemover.RemoveFolder( strFolderPath );
}

/**********************************************************************/

void CKeyboardSetCSVArray::ClearDeletionFolder( int nKbsIdx )
{
	CFileRemover FileRemover;
	CString strFolderPath = GetFolderPathDelete( nKbsIdx );
	FileRemover.RemoveFolder( strFolderPath );
}

/**********************************************************************/

void CKeyboardSetCSVArray::DeleteKeyboardSetByIndex ( int nKbsIdx, bool bDeleteFiles )
{
	if ( nKbsIdx >= 0 && nKbsIdx < GetSize() )
	{
		if ( bDeleteFiles == TRUE )
		{
			ClearKeyboardSetFolder( nKbsIdx );
			ClearDeletionFolder( nKbsIdx );
		}
		
		delete GetKeyboardSet ( nKbsIdx );
		RemoveAt ( nKbsIdx );
	}
}

/**********************************************************************/

void CKeyboardSetCSVArray::DeleteKeyboardSetByNumber ( int nDbNo, int nKbsNo )
{
	int nKbsIdx;
	if ( FindKeyboardSetByNumber ( nDbNo, nKbsNo, nKbsIdx ) == TRUE )
		DeleteKeyboardSetByIndex ( nKbsIdx, TRUE );
}

/**********************************************************************/

bool CKeyboardSetCSVArray::FindKeyboardSetByNumber ( int nDbNo, int nKbsNo, int& nKbsIdx )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	int nCheckNo = ( nDbNo * 1000 ) + nKbsNo;

	while ( nStart <= nEnd )
	{
		nKbsIdx = ( nStart + nEnd ) / 2;
		int nArrayNo = ( GetDbNo ( nKbsIdx ) * 1000 ) + GetKbsNo ( nKbsIdx );
		
		if ( nArrayNo == nCheckNo )
			return TRUE;
		else if ( nArrayNo < nCheckNo )
			nStart = nKbsIdx + 1;
		else 
			nEnd = nKbsIdx - 1;
	}
	nKbsIdx = nStart;
	return FALSE;
}

/**********************************************************************/

int CKeyboardSetCSVArray::GetKbsNo ( int nKbsIdx )
{
	int nKbsNo = 0;
	if ( nKbsIdx >= 0 && nKbsIdx < GetSize() )
	{
		CKeyboardSetCSVRecord* pKeyboardSet = GetKeyboardSet ( nKbsIdx );
		nKbsNo = pKeyboardSet -> m_nKbsNo;
	}
	return nKbsNo;
}

/**********************************************************************/

int CKeyboardSetCSVArray::GetDbNo ( int nKbsIdx )
{
	int nDbNo = 0;
	if ( nKbsIdx >= 0 && nKbsIdx < GetSize() )
	{
		CKeyboardSetCSVRecord* pKeyboardSet = GetKeyboardSet ( nKbsIdx );
		nDbNo = pKeyboardSet -> m_nDbNo;
	}
	return nDbNo;
}

/**********************************************************************/

const char* CKeyboardSetCSVArray::GetName ( int nKbsIdx )
{
	m_strName = "";
	if ( nKbsIdx >= 0 && nKbsIdx < GetSize() )
	{
		CKeyboardSetCSVRecord* pKeyboardSet = GetKeyboardSet ( nKbsIdx );
		m_strName = pKeyboardSet -> m_strName;
	}
	return m_strName;
}

/**********************************************************************/

const char* CKeyboardSetCSVArray::GetSystemName( int nKbsIdx )
{
	m_strSystemName = "";
	if ( nKbsIdx >= 0 && nKbsIdx < GetSize() )
	{
		CKeyboardSetCSVRecord* pKeyboardSet = GetKeyboardSet ( nKbsIdx );
		m_strSystemName = pKeyboardSet -> GetSystemName();
	}
	return m_strSystemName;
}

/**********************************************************************/

void CKeyboardSetCSVArray::SetDbNo ( int nKbsIdx, int nDbNo )
{
	if ( nKbsIdx >= 0 && nKbsIdx < GetSize() )
	{
		CKeyboardSetCSVRecord* pKeyboardSet = GetKeyboardSet ( nKbsIdx );
		pKeyboardSet -> SetDbNo ( nDbNo );
	}
}

/**********************************************************************/

void CKeyboardSetCSVArray::SetKbsNo ( int nKbsIdx, int nKbsNo )
{
	if ( nKbsIdx >= 0 && nKbsIdx < GetSize() )
	{
		CKeyboardSetCSVRecord* pKeyboardSet = GetKeyboardSet ( nKbsIdx );
		pKeyboardSet -> SetKbsNo ( nKbsNo );
	}
}

/**********************************************************************/

void CKeyboardSetCSVArray::SetName ( int nKbsIdx, const char* szName )
{
	if ( nKbsIdx >= 0 && nKbsIdx < GetSize() )
	{
		CKeyboardSetCSVRecord* pKeyboardSet = GetKeyboardSet ( nKbsIdx );
		pKeyboardSet -> SetName ( szName );
	}
}

/**********************************************************************/

const char* CKeyboardSetCSVArray::GetFolderPath( int nKbsIdx )
{
	m_strFolderPath = "";
	if ( nKbsIdx >= 0 && nKbsIdx < GetSize() )
	{
		CKeyboardSetCSVRecord* pKeyboardSet = GetKeyboardSet ( nKbsIdx );
		m_strFolderPath.Format ( "DB%3.3d\\KBS%3.3d", pKeyboardSet -> m_nDbNo, pKeyboardSet -> m_nKbsNo );
		GetDataProgramPath( m_strFolderPath );
	}
	return m_strFolderPath;	
}

/**********************************************************************/

const char* CKeyboardSetCSVArray::GetFolderPathKeyboard( int nKbsIdx, int nKbNo )
{
	m_strFolderPath = "";
	if ( nKbsIdx >= 0 && nKbsIdx < GetSize() )
	{
		CKeyboardSetCSVRecord* pKeyboardSet = GetKeyboardSet ( nKbsIdx );
		m_strFolderPath = GetFolderPath( nKbsIdx );

		CString strExtra;
		strExtra.Format ( "\\V3Kbd%3.3d", nKbNo );
		m_strFolderPath += strExtra;
	}
	return m_strFolderPath;	
}

/**********************************************************************/

const char* CKeyboardSetCSVArray::GetFolderPathDelete( int nKbsIdx )
{
	m_strFolderPathDelete = GetFolderPath( nKbsIdx );
	m_strFolderPathDelete += "XXX";
	return m_strFolderPathDelete;	
}

/**********************************************************************/

const char* CKeyboardSetCSVArray::GetFilePathInfo( int nKbsIdx )
{
	m_strFilePathInfo = "";
	if ( nKbsIdx >= 0 && nKbsIdx < GetSize() )
	{
		m_strFilePathInfo = GetFolderPath ( nKbsIdx );
		m_strFilePathInfo += "\\info.dat";
	}
	return m_strFilePathInfo;
}

/**********************************************************************/

const char* CKeyboardSetCSVArray::GetCSVLine ( int nKbsIdx )
{
	m_strCSVLine = "";
	if ( nKbsIdx >= 0 && nKbsIdx < GetSize() )
	{
		CKeyboardSetCSVRecord* pKeyboardSet = GetKeyboardSet ( nKbsIdx );
		
		CCSV csv;
		pKeyboardSet ->ConvertToCSV(csv);

		m_strCSVLine = csv.GetLine();
	}
	return m_strCSVLine;
}

/**********************************************************************/

void CKeyboardSetCSVArray::PurgeDatabase( int nDbIdx )
{
	if ( nDbIdx < 0 || nDbIdx >= dbDatabase.GetSize() )
		return;

	int nStartIdx, nEndIdx;
	GetKbsIdxRange ( nDbIdx, nStartIdx, nEndIdx );

	for ( int nKbsIdx = nEndIdx; nKbsIdx >= nStartIdx; nKbsIdx-- )
		DeleteKeyboardSetByIndex ( nKbsIdx, FALSE );
}

/**********************************************************************/

void CKeyboardSetCSVArray::GetKbsIdxRange ( int nDbIdx, int& nStartIdx, int& nEndIdx )
{
	nStartIdx = 0;
	nEndIdx = -1;

	if ( nDbIdx < 0 || nDbIdx >= dbDatabase.GetSize() )
		return;

	int nDbNo = dbDatabase.GetDbNo( nDbIdx );

	FindKeyboardSetByNumber ( nDbNo, 0, nStartIdx );
	FindKeyboardSetByNumber ( nDbNo + 1, 0, nEndIdx );
	nEndIdx--;
}

/**********************************************************************/

bool CKeyboardSetCSVArray::WriteKeyboardSet ( int nKbsIdx )
{
	if ( nKbsIdx < 0 || nKbsIdx >= GetSize() )
		return FALSE;

	CSSFile fileKeyboardSet;
	if ( fileKeyboardSet.Open ( GetFilePathInfo( nKbsIdx ), "wb" ) == FALSE )
		return FALSE;

	fileKeyboardSet.WriteLine ( GetCSVLine ( nKbsIdx ) );
	return TRUE;
}

/**********************************************************************/

void CKeyboardSetCSVArray::SortByName( CWordArray& arrayKbsIdx, int nDbIdx )
{
	arrayKbsIdx.RemoveAll();

	CReportConsolidationArray<CSortedLocation> arrayTemp;

	int nStartIdx = 0;
	int nEndIdx = GetSize() - 1;

	if ( nDbIdx >= 0 && nDbIdx < dbDatabase.GetSize() )
		GetKbsIdxRange( nDbIdx, nStartIdx, nEndIdx );

	for ( int nKbsIdx = nStartIdx; nKbsIdx <= nEndIdx; nKbsIdx++ )
	{
		if ( EcrmanOptions.GetReportsLocationSortType() == 2 )
		{
			CSortedLocation item;
			item.m_strName = GetName( nKbsIdx );
			item.m_nIdx = nKbsIdx;
			arrayTemp.Consolidate( item );
		}
		else
		{
			CSortedLocation item;
			item.m_nDbNo = GetDbNo( nKbsIdx );
			item.m_strName = GetName( nKbsIdx );
			item.m_nIdx = nKbsIdx;
			arrayTemp.Consolidate( item );
		}
	}

	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CSortedLocation item;
		arrayTemp.GetAt( n, item );
		arrayKbsIdx.Add( item.m_nIdx );
	}
}

/**********************************************************************/

void CKeyboardSetCSVArray::BuildSortIdxTables()
{
	m_arraySortIdxToKbsIdx.RemoveAll();
	m_arrayKbsIdxToSortIdx.RemoveAll();

	SortByName( m_arraySortIdxToKbsIdx );

	for ( int n = 0; n < GetSize(); n++ )
		m_arrayKbsIdxToSortIdx.Add(0);

	for ( int n = 0; n < m_arraySortIdxToKbsIdx.GetSize(); n++ )
	{
		int nKbsIdx = m_arraySortIdxToKbsIdx.GetAt(n);
		m_arrayKbsIdxToSortIdx.SetAt( nKbsIdx, n );
	}
}

/**********************************************************************/

int CKeyboardSetCSVArray::GetKbsIdxFromSortIdx( int nSortIdx )
{
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
		return nSortIdx;

	if ( nSortIdx < 0 || nSortIdx >= m_arraySortIdxToKbsIdx.GetSize() )
		return nSortIdx;
	else
		return m_arraySortIdxToKbsIdx.GetAt( nSortIdx );
}

/**********************************************************************/

int CKeyboardSetCSVArray::GetSortIdxFromKbsIdx( int nKbsIdx )
{
	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
		return nKbsIdx;

	if ( nKbsIdx < 0 || nKbsIdx >= m_arrayKbsIdxToSortIdx.GetSize() )
		return nKbsIdx;
	else
		return m_arrayKbsIdxToSortIdx.GetAt( nKbsIdx );
}

/**********************************************************************/

