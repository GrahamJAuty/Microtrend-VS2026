/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "EcrmanOptionsIni.h"
#include "FileRemover.h"
//#include "globalfunctions.h"
#include "NodeTypes.h"
#include "ProcessedOrder.h"
#include "ReportConsolidationArray.h"
#include "SiteCSVArray.h"
#include "SortedLocation.h"
#include "StockAuditBaseReport.h"
/**********************************************************************/
#include "StockpointCSVArray.h"
/**********************************************************************/

CStockpointCSVRecord::CStockpointCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CStockpointCSVRecord::ClearRecord()
{
	m_nSpNo = 0;
	m_strName = "";
	m_nDbNo = 0;
	m_nSiteNo = 0;
	SetPeriodStart();
	SetYearStart();
	m_bWarehouse = FALSE;
	m_bTransferIn = TRUE;
	m_bTransferOut = TRUE;
	m_nImportSiteNo = 0;
}

/**********************************************************************/

void CStockpointCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetSpNo ( csv.GetInt(0) );
	CString strName = csv.GetString(1);
	SetDbNo ( csv.GetInt(2) );

	switch ( csv.GetInt(3) )
	{
	case 1:	V1To3ConvertFromCSV ( csv, 1 );	break;
	case 2:	V1To3ConvertFromCSV ( csv, 2 );	break;
	case 3:	V1To3ConvertFromCSV ( csv, 3 );	break;
	}

	SetName( strName );
}

/**********************************************************************/

void CStockpointCSVRecord::V1To3ConvertFromCSV ( CCSV& csv, int nVer )
{
	int nPos = 4;

	SetSiteNo ( csv.GetInt(nPos++) );
	SetPeriodStart ( csv.GetString(nPos++) );
	SetYearStart ( csv.GetString(nPos++) );

	if ( nVer >= 2 )
	{
		SetWarehouseFlag( csv.GetBool(nPos++) );
		SetTransferOutFlag( csv.GetBool(nPos++) );
		SetTransferInFlag( csv.GetBool(nPos++) );
	}

	if ( nVer >= 3 )
		SetImportSiteNo( csv.GetInt(nPos++) );
}

/**********************************************************************/

void CStockpointCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nSpNo );
	csv.Add ( m_strName );
	csv.Add ( m_nDbNo );
	csv.Add ( STOCKPOINT_VERSION );				
	csv.Add ( m_nSiteNo );
	csv.Add ( m_strPeriodStart );
	csv.Add ( m_strYearStart );
	csv.Add ( m_bWarehouse );
	csv.Add ( m_bTransferOut );
	csv.Add ( m_bTransferIn );
	csv.Add ( m_nImportSiteNo );
}

/**********************************************************************/

void CStockpointCSVRecord::SetName ( const char* szName )
{
	CString strName = szName;
	::TrimSpaces ( strName, FALSE );
	if ( strName == "" ) strName = GetSystemName();
	m_strName = strName;	 
}

/**********************************************************************/

void CStockpointCSVRecord::SetPeriodStart( const char* szDate )
{
	CString strDate = szDate;
	CSSDate Date ( szDate );
	if ( Date.IsSet() == TRUE ) m_strPeriodStart = Date.GetCSVDate();
}

/**********************************************************************/

void CStockpointCSVRecord::SetYearStart( const char* szDate )
{
	CString strDate = szDate;
	CSSDate Date ( szDate );
	if ( Date.IsSet() == TRUE ) m_strYearStart = Date.GetCSVDate();
}

/**********************************************************************/

void CStockpointCSVRecord::SetPeriodStart()
{
	CSSDate dateNow;
	dateNow.SetCurrentDate();
	m_strPeriodStart = dateNow.GetCSVDate();
}

/**********************************************************************/

void CStockpointCSVRecord::SetYearStart()
{
	CSSDate dateNow;
	dateNow.SetCurrentDate();
	m_strYearStart = dateNow.GetCSVDate();
	SetPeriodStart();
}

/**********************************************************************/

const char* CStockpointCSVRecord::GetSystemName()
{
	m_strSystemName.Format ( "Stockpoint %d.%d", m_nDbNo, m_nSpNo );
	return m_strSystemName;
}

/**********************************************************************/

void CStockpointCSVRecord::SetImportSiteNo( int n )
{
	if ( ( n >= 0 ) && ( n <= 999 ) )
		m_nImportSiteNo = n;
}

/**********************************************************************/

CStockpointCSVArray::CStockpointCSVArray() 
{
}

/**********************************************************************/

CStockpointCSVArray::~CStockpointCSVArray() 
{
	RemoveAll();
}

/**********************************************************************/

int CStockpointCSVArray::AddStockpoint ( int nDbNo )
{
	if ( Sysset.GetMaxStockPoints() <= GetSize() )
		return -1;

	int nStartIdx, nEndIdx;
	FindStockpointByNumber ( nDbNo, 0, nStartIdx );
	FindStockpointByNumber ( nDbNo + 1, 0, nEndIdx );
	nEndIdx--;
	
	if ( nEndIdx >= nStartIdx + 998 )
		return -1;

	int nSpIdx = nStartIdx, nSpNo = 1;
	for ( ; nSpIdx <= nEndIdx; nSpIdx++, nSpNo++ )
		if ( GetSpNo ( nSpIdx ) != nSpNo )
			break;
		
	CStockpointCSVRecord* pStockpoint = new CStockpointCSVRecord;
	InsertAt ( nSpIdx, pStockpoint );
	SetSpNo ( nSpIdx, nSpNo );
	SetDbNo ( nSpIdx, nDbNo );
	SetName ( nSpIdx, "" );
	
	CString strFolderPath = GetFolderPath ( nSpIdx );
	::CreateSubdirectory ( strFolderPath );
	
	WriteStockpoint ( nSpIdx );
	return nSpIdx;
}

/**********************************************************************/

int CStockpointCSVArray::AddStockpoint( int nDbNo, int nSpNo, const char* szPath )
{
	int nSpIdx;
	if ( FindStockpointByNumber ( nDbNo, nSpNo, nSpIdx ) == FALSE )
	{
		CStockpointCSVRecord* pStockpoint = new CStockpointCSVRecord;
		
		CString strFileInfo;
		strFileInfo.Format ( "%sInfo.dat", szPath );

		CSSFile fileInfo;
		if ( fileInfo.Open( strFileInfo, "rb" ) )
		{
			CString strBuffer;
			fileInfo.ReadString( strBuffer );
			CCSV csv ( strBuffer );
			pStockpoint -> ConvertFromCSV ( csv ); 
			InsertAt ( nSpIdx, pStockpoint );
			SetSpNo ( nSpIdx, nSpNo );
			SetDbNo ( nSpIdx, nDbNo );
		}
		else
		{
			pStockpoint -> SetSpNo ( nSpNo );
			pStockpoint -> SetDbNo ( nDbNo );
			pStockpoint -> SetName ( "" );
			InsertAt ( nSpIdx, pStockpoint );
		}
	}
	return nSpIdx;
}

/**********************************************************************/

void CStockpointCSVArray::GetSpIdxRange ( int nDbIdx, int& nStartIdx, int& nEndIdx )
{
	nStartIdx = 0;
	nEndIdx = -1;

	if ( nDbIdx < 0 || nDbIdx >= dbDatabase.GetSize() )
		return;

	int nDbNo = dbDatabase.GetDbNo( nDbIdx );

	FindStockpointByNumber ( nDbNo, 0, nStartIdx );
	FindStockpointByNumber ( nDbNo + 1, 0, nEndIdx );
	nEndIdx--;
}

/**********************************************************************/

bool CStockpointCSVArray::FindStockpointByNumber ( int nDbNo, int nSpNo, int& nSpIdx )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	int nCheckNo = ( nDbNo * 1000 ) + nSpNo;

	while ( nStart <= nEnd )
	{
		nSpIdx = ( nStart + nEnd ) / 2;
		int nArrayNo = ( GetDbNo ( nSpIdx ) * 1000 ) + GetSpNo ( nSpIdx );
		
		if ( nArrayNo == nCheckNo )
			return TRUE;
		else if ( nArrayNo < nCheckNo )
			nStart = nSpIdx + 1;
		else 
			nEnd = nSpIdx - 1;
	}
	nSpIdx = nStart;
	return FALSE;
}

/**********************************************************************/

bool CStockpointCSVArray::RenameForDeletion( int nSpIdx )
{
	if ( nSpIdx < 0 || nSpIdx >= GetSize() )
		return FALSE;

	ClearDeletionFolder( nSpIdx );	

	if ( rename ( GetFolderPath( nSpIdx ), GetFolderPathDelete( nSpIdx ) ) != 0 )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

void CStockpointCSVArray::ClearStockpointFolder( int nSpIdx )
{
	CFileRemover FileRemover;
	CString strFolderPath = GetFolderPath( nSpIdx );
	FileRemover.RemoveFolder( strFolderPath );
}

/**********************************************************************/

void CStockpointCSVArray::ClearDeletionFolder( int nSpIdx )
{
	CFileRemover FileRemover;
	CString strFolderPath = GetFolderPathDelete( nSpIdx );
	FileRemover.RemoveFolder( strFolderPath );
}

/**********************************************************************/

void CStockpointCSVArray::DeleteStockpointByIndex ( int nSpIdx, bool bDeleteFiles )
{
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		if( bDeleteFiles == TRUE )
		{
			ClearStockpointFolder( nSpIdx );
			ClearDeletionFolder( nSpIdx );
		}

		delete GetStockpoint ( nSpIdx );
		RemoveAt ( nSpIdx );
	}
}

/**********************************************************************/

void CStockpointCSVArray::DeleteStockpointByNumber ( int nDbNo, int nSpNo )
{
	int nSpIdx;
	if ( FindStockpointByNumber ( nDbNo, nSpNo, nSpIdx ) == TRUE )
		DeleteStockpointByIndex ( nSpIdx, TRUE );
}

/**********************************************************************/

void CStockpointCSVArray::PurgeDatabase( int nDbIdx )
{
	if ( nDbIdx < 0 || nDbIdx >= dbDatabase.GetSize() )
		return;

	int nStartIdx, nEndIdx;
	GetSpIdxRange ( nDbIdx, nStartIdx, nEndIdx );

	for ( int nSpIdx = nEndIdx; nSpIdx >= nStartIdx; nSpIdx-- )
		DeleteStockpointByIndex ( nSpIdx, FALSE );
}

/**********************************************************************/

void CStockpointCSVArray::RemoveAll()
{
	while ( GetSize() > 0 )
	{
		int nSpIdx = GetSize() - 1;
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		delete pStockpoint;
		RemoveAt ( nSpIdx );
	}
}

/**********************************************************************/

const char* CStockpointCSVArray::GetCSVLine ( int nSpIdx )
{
	m_strCSVLine = "";
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		
		CCSV csv;
		pStockpoint ->ConvertToCSV(csv);

		m_strCSVLine = csv.GetLine();
	}
	return m_strCSVLine;
}

/**********************************************************************/

int CStockpointCSVArray::GetSpNo ( int nSpIdx )
{
	int nSpNo = 0;
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		nSpNo = pStockpoint -> m_nSpNo;
	}
	return nSpNo;
}

/**********************************************************************/

int CStockpointCSVArray::GetSiteNo ( int nSpIdx )
{
	int nSiteNo = 0;
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		nSiteNo = pStockpoint -> m_nSiteNo;
	}
	return nSiteNo;
}

/**********************************************************************/

int CStockpointCSVArray::GetDbNo ( int nSpIdx )
{
	int nDbNo = 0;
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		nDbNo = pStockpoint -> m_nDbNo;
	}
	return nDbNo;
}

/**********************************************************************/

int CStockpointCSVArray::GetImportSiteNo ( int nSpIdx )
{
	int nSiteNo = 0;
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		nSiteNo = pStockpoint -> m_nImportSiteNo;
	}
	return nSiteNo;
}

/**********************************************************************/

const char* CStockpointCSVArray::GetName ( int nSpIdx )
{
	m_strName = "";
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		m_strName = pStockpoint -> m_strName;
	}
	return m_strName;
}

/**********************************************************************/

const char* CStockpointCSVArray::GetSystemName ( int nSpIdx )
{
	m_strSystemName = "";
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		m_strSystemName = pStockpoint -> GetSystemName();
	}
	return m_strSystemName;
}

/**********************************************************************/

const char* CStockpointCSVArray::GetExtendedSystemName ( int nSpIdx )
{
	GetSystemName( nSpIdx );
	GetName( nSpIdx );

	if ( m_strSystemName != m_strName )
	{
		m_strSystemName += " (";
		m_strSystemName += m_strName;
		m_strSystemName += ")";
	}

	return m_strSystemName;
}

/**********************************************************************/

const char* CStockpointCSVArray::GetFolderPath ( int nSpIdx )
{
	m_strFolderPath = "";
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		m_strFolderPath.Format ( "Db%3.3d\\Sp%3.3d", pStockpoint -> m_nDbNo, pStockpoint -> m_nSpNo );
		GetDataProgramPath( m_strFolderPath );
	}
	return m_strFolderPath;
}

/**********************************************************************/

const char* CStockpointCSVArray::GetFolderPathDelete ( int nSpIdx )
{
	m_strFolderPathDelete = GetFolderPath( nSpIdx );
	m_strFolderPathDelete += "XXX";
	return m_strFolderPathDelete;
}

/**********************************************************************/

const char* CStockpointCSVArray::GetFolderPathHistory ( int nSpIdx )
{
	m_strFolderPathHistory = "";
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		m_strFolderPathHistory = GetFolderPath( nSpIdx );
		m_strFolderPathHistory += "\\History";
	}
	return m_strFolderPathHistory;
}

/**********************************************************************/

const char* CStockpointCSVArray::GetFolderPathAudit ( int nSpIdx )
{
	m_strFolderPathAudit = "";
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		m_strFolderPathAudit = GetFolderPath( nSpIdx );
		m_strFolderPathAudit += "\\Audit";
	}
	return m_strFolderPathAudit;
}

/**********************************************************************/

const char* CStockpointCSVArray::GetFolderPathSalesXXX ( int nSpIdx )
{
	m_strFolderPathSalesXXX = "";
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		m_strFolderPathSalesXXX = GetFolderPath( nSpIdx );
		m_strFolderPathSalesXXX += "\\SalesXXX_V2";
	}
	return m_strFolderPathSalesXXX;
}

/**********************************************************************/

const char* CStockpointCSVArray::GetFolderPathSession( int nSpIdx, int nType )
{
	m_strFolderPathSession = "";
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		m_strFolderPathSession = GetFolderPath( nSpIdx );
		m_strFolderPathSession += "\\Session";

		switch( nType )
		{
		case NODE_DELIVERY:
			m_strFolderPathSession += "\\Delivery";
			break;

		case NODE_RETURN:
			m_strFolderPathSession += "\\Return";
			break;

		case NODE_ADJUSTMENT_STOCK:
			m_strFolderPathSession += "\\Adjust";
			break;

		case NODE_ADJUSTMENT_PLU:
			m_strFolderPathSession += "\\PluAdj";
			break;

		case NODE_MANUALSALE_STOCK:
			m_strFolderPathSession += "\\StkSale";
			break;

		case NODE_MANUALSALE_PLU:
			m_strFolderPathSession += "\\PluSale";
			break;
		}
	}

	return m_strFolderPathSession;
}

/**********************************************************************/

const char* CStockpointCSVArray::GetFilePathInfo( int nSpIdx )
{
	m_strFilePathInfo = "";
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		m_strFilePathInfo = GetFolderPath ( nSpIdx );
		m_strFilePathInfo += "\\info.dat";
	}
	return m_strFilePathInfo;
}

/**********************************************************************/

const char* CStockpointCSVArray::GetPeriodStart ( int nSpIdx )
{
	m_strPeriodStart = "";
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		m_strPeriodStart = pStockpoint -> m_strPeriodStart;
	}
	return m_strPeriodStart;
}

/**********************************************************************/

const char* CStockpointCSVArray::GetYearStart ( int nSpIdx )
{
	m_strYearStart = "";
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		m_strYearStart = pStockpoint -> m_strYearStart;
	}
	return m_strYearStart;
}

/**********************************************************************/

bool CStockpointCSVArray::GetWarehouseFlag ( int nSpIdx )
{
	bool bFlag = FALSE;
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		bFlag = pStockpoint -> m_bWarehouse;
	}
	return bFlag;
}

/**********************************************************************/

bool CStockpointCSVArray::GetTransferInFlag ( int nSpIdx )
{
	bool b = FALSE;
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		b = pStockpoint -> m_bTransferIn;
	}
	return b;
}

/**********************************************************************/

bool CStockpointCSVArray::GetTransferOutFlag ( int nSpIdx )
{
	bool b = FALSE;
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		b = pStockpoint -> m_bTransferOut;
	}
	return b;
}

/**********************************************************************/

void CStockpointCSVArray::SetSpNo ( int nSpIdx, int nSpNo )
{
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		pStockpoint -> SetSpNo ( nSpNo );
	}
}

/**********************************************************************/

void CStockpointCSVArray::SetSiteNo ( int nSpIdx, int nSiteNo )
{
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		pStockpoint -> SetSiteNo ( nSiteNo );
	}
}

/**********************************************************************/

void CStockpointCSVArray::SetDbNo ( int nSpIdx, int nDbNo )
{
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		pStockpoint -> SetDbNo ( nDbNo );
	}
}

/**********************************************************************/

void CStockpointCSVArray::SetImportSiteNo ( int nSpIdx, int nSiteNo )
{
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		pStockpoint -> SetImportSiteNo ( nSiteNo );
	}
}

/**********************************************************************/

void CStockpointCSVArray::SetName ( int nSpIdx, const char* szName )
{
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		pStockpoint -> SetName ( szName );
	}
}

/**********************************************************************/

void CStockpointCSVArray::SetPeriodStart ( int nSpIdx, const char* szDate )
{
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		pStockpoint -> SetPeriodStart ( szDate );
	}
}

/**********************************************************************/

void CStockpointCSVArray::SetYearStart ( int nSpIdx, const char* szDate )
{
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		pStockpoint -> SetYearStart ( szDate );
	}
}

/**********************************************************************/

void CStockpointCSVArray::SetPeriodStart ( int nSpIdx )
{
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		pStockpoint -> SetPeriodStart();
	}
}

/**********************************************************************/

void CStockpointCSVArray::SetYearStart ( int nSpIdx )
{
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		pStockpoint -> SetYearStart();
	}
}

/**********************************************************************/

void CStockpointCSVArray::SetWarehouseFlag ( int nSpIdx, bool bFlag )
{
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		pStockpoint -> SetWarehouseFlag( bFlag );
	}
}

/**********************************************************************/

void CStockpointCSVArray::SetTransferOutFlag ( int nSpIdx, bool b )
{
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		pStockpoint -> m_bTransferOut = b;
	}
}

/**********************************************************************/

void CStockpointCSVArray::SetTransferInFlag ( int nSpIdx, bool b )
{
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		CStockpointCSVRecord* pStockpoint = GetStockpoint ( nSpIdx );
		pStockpoint -> m_bTransferIn = b;
	}
}

/**********************************************************************/

bool CStockpointCSVArray::WriteStockpoint ( int nSpIdx )
{
	if ( nSpIdx < 0 || nSpIdx >= GetSize() )
		return FALSE;

	CSSFile fileStockpoint;
	if ( fileStockpoint.Open ( GetFilePathInfo( nSpIdx ), "wb" ) == FALSE )
		return FALSE;

	fileStockpoint.WriteLine ( GetCSVLine ( nSpIdx ) );
	return TRUE;
}

/**********************************************************************/

void CStockpointCSVArray::GetHistoryFileList( int nSpIdx, const char* szMonthFrom, const char* szMonthTo, CStringArray& FileArray, CStringArray& MonthArray, __int64& nTotalLength, bool bAppend, int nVersion )
{
	CString strExt;
	strExt.Format ( ".%3.3d", nVersion );

	if ( bAppend == FALSE )
	{
		nTotalLength = 0;
		FileArray.RemoveAll();
		MonthArray.RemoveAll();
	}

	CString strMonthFrom = szMonthFrom;
	CString strMonthTo = szMonthTo;

	CString strPath = dbStockpoint.GetFolderPathHistory( nSpIdx );
	strPath += "\\";

	CFileFind FileFinder;
	bool bWorking = ( FileFinder.FindFile ( strPath + "*.*" ) != 0 );
	
	while (bWorking)   		
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
			continue;

		//WE ONLY NEED TO CHECK FILES
		if ( FileFinder.IsDirectory() == TRUE )
			continue;

		//MUST BE 8 CHARS PLUS DOT AND EXTENSION
		CString strFilename = FileFinder.GetFileName();
		if ( strFilename.GetLength() != 12 )
			continue;

		//MUST BEGIN WITH ST
		strFilename.MakeUpper();
		if ( strFilename.Left(2) != "ST" )
			continue;

		//MUST BE A STOCK SALES HISTORY FILE
		if ( strFilename.Right(4) != strExt )
			continue;

		//MUST CONTAIN A VALID MONTH
		CString strMonth = strFilename.Mid(2,6);
		if ( ::TestNumeric( strMonth ) == FALSE )
			continue;

		//MONTH MUST BE IN RANGE
		if ( strMonth < strMonthFrom || strMonth > strMonthTo )
			continue;
	
		FileArray.Add ( strPath + strFilename );
		MonthArray.Add ( strMonth );

		CSSFile fileTest;
		if ( fileTest.Open ( strPath + strFilename, "rb" ) )
			nTotalLength += fileTest.GetLength();
	}
}

/**********************************************************************/

const char* CStockpointCSVArray::GetFilePathAudit( int nSpIdx, COleDateTime& date, int nType, bool bPMS )
{
	if ( nType != AUDIT_FAMILY_RETRY )
	{
		m_strFilePathAudit = GetFolderPathAudit( nSpIdx );
		::CreateSubdirectory( m_strFilePathAudit );
	}
	else
	{
		m_strFilePathAudit = GetFolderPathSalesXXX( nSpIdx );
		::CreateSubdirectory( m_strFilePathAudit );
	}

	m_strFilePathAudit += "\\";
	m_strFilePathAudit += GetFileNameAudit( nSpIdx, date, nType, bPMS );
	return m_strFilePathAudit;
}

/**********************************************************************/

const char* CStockpointCSVArray::GetFolderPathDataOrdersRoot( int nSpIdx )
{
	m_strFolderPathOrder = "";
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		m_strFolderPathOrder = GetFolderPath( nSpIdx );
		m_strFolderPathOrder += "\\Orders";
	}
	return m_strFolderPathOrder;
}

/**********************************************************************/

const char* CStockpointCSVArray::GetFolderPathDataOrdersSupplier( int nSpIdx, int nSuppNo )
{
	m_strFolderPathOrder = "";
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		m_strFolderPathOrder.Format( "%s\\Orders\\Supp%3.3d",
			GetFolderPath( nSpIdx ),
			nSuppNo );
	}
	return m_strFolderPathOrder;
}

/**********************************************************************/

const char* CStockpointCSVArray::GetFolderPathDataOrdersType( int nSpIdx, int nSuppNo, int nType )
{
	m_strFolderPathOrder = "";
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		switch( nType )
		{
		case ORDER_TYPE_OPEN:
			m_strFolderPathOrder.Format( "%s\\Orders\\Supp%3.3d\\Open",
				GetFolderPath( nSpIdx ),
				nSuppNo );
			break;

		case ORDER_TYPE_MIXED:
			m_strFolderPathOrder.Format( "%s\\Orders\\Supp%3.3d\\Mixed",
				GetFolderPath( nSpIdx ),
				nSuppNo );
			break;

		case ORDER_TYPE_CLOSED:
		default:
			m_strFolderPathOrder.Format( "%s\\Orders\\Supp%3.3d\\Closed",
				GetFolderPath( nSpIdx ),
				nSuppNo );
			break;
		}
	}
	return m_strFolderPathOrder;
}

/**********************************************************************/

const char* CStockpointCSVArray::GetFilePathDataOrder( int nSpIdx, int nSuppNo, int nType, int nOrderNum, CString& strDate, CString& strTime )
{
	if ( strDate.GetLength() != 8 )
		strDate = "00000000";

	if ( strTime.GetLength() != 6 )
		strTime = "000000";

	m_strFilePath = "";
	if ( nSpIdx >= 0 && nSpIdx < GetSize() )
	{
		switch( nType )
		{
		case ORDER_TYPE_OPEN:
			m_strFilePath.Format( "%s\\Orders\\Supp%3.3d\\Open\\%6.6d",
				GetFolderPath( nSpIdx ),
				nSuppNo,
				nOrderNum );
			break;

		case ORDER_TYPE_MIXED:
			m_strFilePath.Format( "%s\\Orders\\Supp%3.3d\\Mixed\\%6.6d",
				GetFolderPath( nSpIdx ),
				nSuppNo,
				nOrderNum );
			break;

		case ORDER_TYPE_CLOSED:
		default:
			m_strFilePath.Format( "%s\\Orders\\Supp%3.3d\\Closed\\%6.6d",
				GetFolderPath( nSpIdx ),
				nSuppNo,
				nOrderNum );
			break;
		}

		CString strHyphen = "-";
		m_strFilePath += strHyphen + strDate + strHyphen + strTime + ".dat";
	}
	return m_strFilePath;
}

/**********************************************************************/

const char* CStockpointCSVArray::GetFileNameAudit( int nSpIdx, COleDateTime& date, int nType, bool bPMS )
{
	m_strFileNameAudit = "";

	switch ( nType )
	{
	case AUDIT_FAMILY_TEMP:		m_strFileNameAudit = "audit.tmp";	break;
	case AUDIT_FAMILY_TEMP_EX:	m_strFileNameAudit = "except.tmp";	break;
	case AUDIT_FAMILY_TEMP2:		m_strFileNameAudit = "audit2.tmp";	break;
	case AUDIT_FAMILY_TEMP2_EX:	m_strFileNameAudit = "except2.tmp";break;
	case AUDIT_FAMILY_PREV:		m_strFileNameAudit = "audit.pre";	break;
	case AUDIT_FAMILY_PREV_EX:	m_strFileNameAudit = "except.pre";	break;
	case AUDIT_FAMILY_IMPORT:	m_strFileNameAudit = "import.pre";	break;
	
	case AUDIT_FAMILY_RETRY:
		{
			CString strPMS = ( bPMS ) ? "_PMS" : "";

			CString strDate;
			strDate.Format ( "SALESXXX%s_%4.4d%2.2d%2.2d.001", 
				(const char*) strPMS,
				date.GetYear(), 
				date.GetMonth(), 
				date.GetDay() );

			m_strFileNameAudit = strDate;
		}
		break;
	
	case AUDIT_FAMILY_NORMAL:
	default:
		{
			CString strDate;
			strDate.Format ( "%4.4d%2.2d%2.2d.001", date.GetYear(), date.GetMonth(), date.GetDay() );
			m_strFileNameAudit = strDate;
		}
		break;
	}
	
	return m_strFileNameAudit;
}

/**********************************************************************/

void CStockpointCSVArray::RemoveTemporaryAudits( int nSpIdx )
{
	COleDateTime date;
	CFileRemover FileRemover;
	FileRemover.RemoveFile ( GetFilePathAudit( nSpIdx, date, AUDIT_FAMILY_TEMP ) );
	FileRemover.RemoveFile ( GetFilePathAudit( nSpIdx, date, AUDIT_FAMILY_TEMP2 ) );
	FileRemover.RemoveFile ( GetFilePathAudit( nSpIdx, date, AUDIT_FAMILY_PREV ) );
	FileRemover.RemoveFile ( GetFilePathAudit( nSpIdx, date, AUDIT_FAMILY_TEMP_EX ) );
	FileRemover.RemoveFile ( GetFilePathAudit( nSpIdx, date, AUDIT_FAMILY_TEMP2_EX ) );
	FileRemover.RemoveFile ( GetFilePathAudit( nSpIdx, date, AUDIT_FAMILY_PREV_EX ) );
	FileRemover.RemoveFile ( GetFilePathAudit( nSpIdx, date, AUDIT_FAMILY_IMPORT ) );
}

/**********************************************************************/

void CStockpointCSVArray::LoadStockpointBuffer( int nSpIdx, CStockpointCSVArray& buffer )
{
	if ( nSpIdx < 0 || nSpIdx >= GetSize() )
		return;

	if ( buffer.GetSize() != 1 )
		return;

	CStockpointCSVRecord* pStockpoint;
	CStockpointCSVRecord* pBuffer;

	pStockpoint = GetStockpoint( nSpIdx );
	pBuffer = buffer.GetStockpoint( 0 );

	CCSV csv;
	pStockpoint -> ConvertToCSV( csv );
	pBuffer -> ConvertFromCSV( csv );
}

/**********************************************************************/

void CStockpointCSVArray::SaveStockpointBuffer( int nSpIdx, CStockpointCSVArray& buffer )
{
	if ( nSpIdx < 0 || nSpIdx >= GetSize() )
		return;

	if ( buffer.GetSize() != 1 )
		return;

	CStockpointCSVRecord* pStockpoint;
	CStockpointCSVRecord* pBuffer;

	pStockpoint = GetStockpoint( nSpIdx );
	pBuffer = buffer.GetStockpoint( 0 );

	CCSV csv;
	pBuffer -> ConvertToCSV( csv );
	pStockpoint -> ConvertFromCSV( csv );
}

/**********************************************************************/

void CStockpointCSVArray::SortByName( CWordArray& arraySpIdx, int nDbIdx )
{
	CReportConsolidationArray<CSortedLocation> arrayTemp;

	int nStart = 0;
	int nEnd = GetSize() - 1;

	if ( nDbIdx >= 0 && nDbIdx < dbDatabase.GetSize() )
		GetSpIdxRange( nDbIdx, nStart, nEnd );

	switch( EcrmanOptions.GetReportsLocationSortType() )
	{
	case 1:
		{	
			for ( int nSpIdx = nStart; nSpIdx <= nEnd; nSpIdx++ )
			{
				CSortedLocation item;
				item.m_nDbNo = GetDbNo( nSpIdx );
				item.m_nBlock = GetWarehouseFlag( nSpIdx ) ? 0 : 1;
				item.m_strName = GetName( nSpIdx );
				item.m_nIdx = nSpIdx;
				arrayTemp.Consolidate( item );
			}
		}
		break;

	case 2:
		for ( int nSpIdx = nStart; nSpIdx <= nEnd; nSpIdx++ )
		{
			CSortedLocation item;
			item.m_nBlock = GetWarehouseFlag( nSpIdx ) ? 0 : 1;
			item.m_strName = GetName( nSpIdx );
			item.m_nIdx = nSpIdx;
			arrayTemp.Consolidate( item );
		}
		break;

	case 0:
	default:
		for ( int nSpIdx = nStart; nSpIdx <= nEnd; nSpIdx++ )
		{
			CSortedLocation item;
			item.m_nDbNo = GetDbNo( nSpIdx );
			item.m_nBlock = GetWarehouseFlag( nSpIdx ) ? 0 : 1;
			item.m_nIdx = nSpIdx;
			arrayTemp.Consolidate( item );
		}
	}

	arraySpIdx.RemoveAll();
	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CSortedLocation item;
		arrayTemp.GetAt( n, item );
		arraySpIdx.Add( item.m_nIdx );
	}
}

/**********************************************************************/

void CStockpointCSVArray::SortSpListByName( CArray<int,int>& arraySpIdx )
{
	CReportConsolidationArray<CSortedLocation> arrayTemp;

	switch( EcrmanOptions.GetReportsLocationSortType() )
	{
	case 1:
		{	
			for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
			{
				int nSpIdx = arraySpIdx.GetAt( n );

				if ( ( nSpIdx >= 0 ) && ( nSpIdx < GetSize() ) )
				{
					CSortedLocation item;
					item.m_nDbNo = GetDbNo( nSpIdx );
					item.m_nBlock = GetWarehouseFlag( nSpIdx ) ? 0 : 1;
					item.m_strName = GetName( nSpIdx );
					item.m_nIdx = nSpIdx;
					arrayTemp.Consolidate( item );
				}
			}
		}
		break;

	case 2:
		{
			for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
			{
				int nSpIdx = arraySpIdx.GetAt( n );

				if ( ( nSpIdx >= 0 ) && ( nSpIdx < GetSize() ) )
				{
					CSortedLocation item;
					item.m_nBlock = GetWarehouseFlag( nSpIdx ) ? 0 : 1;
					item.m_strName = GetName( nSpIdx );
					item.m_nIdx = nSpIdx;
					arrayTemp.Consolidate( item );
				}
			}
		}
		break;

	case 0:
	default:
		for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
		{
			int nSpIdx = arraySpIdx.GetAt( n );

			if ( ( nSpIdx >= 0 ) && ( nSpIdx < GetSize() ) )
			{
				CSortedLocation item;
				item.m_nDbNo = GetDbNo( nSpIdx );
				item.m_nBlock = GetWarehouseFlag( nSpIdx ) ? 0 : 1;
				item.m_nIdx = nSpIdx;
				arrayTemp.Consolidate( item );
			}
		}
		break;
	}

	arraySpIdx.RemoveAll();
	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CSortedLocation item;
		arrayTemp.GetAt( n, item );
		arraySpIdx.Add( item.m_nIdx );
	}
}

/**********************************************************************/

void CStockpointCSVArray::BuildSortIdxTables()
{
	m_arraySortIdxToSpIdx.RemoveAll();
	m_arraySpIdxToSortIdx.RemoveAll();

	SortByName( m_arraySortIdxToSpIdx );

	for ( int n = 0; n < GetSize(); n++ )
		m_arraySpIdxToSortIdx.Add(0);

	for ( int n = 0; n < m_arraySortIdxToSpIdx.GetSize(); n++ )
	{
		int nSpIdx = m_arraySortIdxToSpIdx.GetAt(n);
		m_arraySpIdxToSortIdx.SetAt( nSpIdx, n );
	}
}

/**********************************************************************/

int CStockpointCSVArray::GetSpIdxFromSortIdx( int nSortIdx )
{
	if ( nSortIdx < 0 || nSortIdx >= m_arraySortIdxToSpIdx.GetSize() )
		return nSortIdx;
	else
		return m_arraySortIdxToSpIdx.GetAt( nSortIdx );
}

/**********************************************************************/

int CStockpointCSVArray::GetSortIdxFromSpIdx( int nSpIdx )
{
	if ( nSpIdx < 0 || nSpIdx >= m_arraySpIdxToSortIdx.GetSize() )
		return nSpIdx;
	else
		return m_arraySpIdxToSortIdx.GetAt( nSpIdx );
}

/**********************************************************************/

void CStockpointCSVArray::AddSyncRequestStockLevels(int nDbNo, int nSpNo)
{
	int nIdx;
	if ( FindSyncRequestStockLevels( nDbNo, nSpNo, nIdx ) == FALSE )
	{
		CStockSyncInfo info;
		info.m_nDbNo = nDbNo;
		info.m_nSpNo = nSpNo;
		m_arraySyncStockLevels.InsertAt( nIdx, info );
	}
}

/**********************************************************************/

bool CStockpointCSVArray::GetSyncRequestStockLevels(int& nDbNo, int& nSpNo)
{
	if ( m_arraySyncStockLevels.GetSize() == 0 )
		return FALSE;

	nDbNo = m_arraySyncStockLevels[0].m_nDbNo;
	nSpNo = m_arraySyncStockLevels[0].m_nSpNo;

	return TRUE;
}

/**********************************************************************/

void CStockpointCSVArray::RemoveSyncRequestStockLevels()
{
	if ( m_arraySyncStockLevels.GetSize() > 0 )
		m_arraySyncStockLevels.RemoveAt(0);
}

/**********************************************************************/

void CStockpointCSVArray::RemoveSyncRequestStockLevels( int nSpIdx )
{
	if ( nSpIdx < 0 || nSpIdx >= GetSize() )
		return;

	int nIdx;
	if ( FindSyncRequestStockLevels( GetDbNo(nSpIdx), GetSpNo(nSpIdx), nIdx ) == TRUE )
		m_arraySyncStockLevels.RemoveAt(nIdx);
}

/**********************************************************************/

bool CStockpointCSVArray::FindSyncRequestStockLevels(int nDbNo, int nSpNo, int &nIdx)
{
	int nStart = 0;
	int nEnd = m_arraySyncStockLevels.GetSize() - 1;

	while ( nStart <= nEnd )
	{
		int nArrayIdx = ( nStart + nEnd ) / 2;

		if ( m_arraySyncStockLevels[nArrayIdx].m_nDbNo < nDbNo )
		{
			nStart = nArrayIdx + 1;
			continue;
		}
		else if ( m_arraySyncStockLevels[nArrayIdx].m_nDbNo > nDbNo )
		{
			nEnd = nArrayIdx - 1;
			continue;
		}
		else if ( m_arraySyncStockLevels[nArrayIdx].m_nSpNo < nSpNo )
		{
			nStart = nArrayIdx + 1;
			continue;
		}
		else if ( m_arraySyncStockLevels[nArrayIdx].m_nSpNo > nSpNo )
		{
			nEnd = nArrayIdx - 1;
			continue;
		}
		else
		{
			nIdx = nArrayIdx;
			return TRUE;
		}
	}

	nIdx = nStart;
	return FALSE;
}

/**********************************************************************/

void CStockpointCSVArray::GetAddressInfo( int nSpIdx, CAddressInfo& info )
{
	info.m_strAddress1 = "";
	info.m_strAddress2 = "";
	info.m_strAddress3 = "";
	info.m_strPostcode = "";
	info.m_strPhone = "";
	info.m_strFax = "";
	info.m_strEmail = "";

	CFilenameUpdater FnUp( SysFiles::SpAddress, nSpIdx );
	
	CSSFile file;
	if ( file.Open( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
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

/**********************************************************************/

void CStockpointCSVArray::SetAddressInfo( int nSpIdx, CAddressInfo& info )
{
	CFilenameUpdater FnUp( SysFiles::SpAddress, nSpIdx );
		
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
