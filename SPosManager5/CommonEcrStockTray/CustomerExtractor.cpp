/**********************************************************************/
 
/**********************************************************************/
#include "CashRSPImporter.h"
#include "DatabaseCSVArray.h"
//#include "DataManagerNonDb.h"
#include "HistoryFileTypes.h"
//#include "globalfunctions.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "PMSBookingCSVArray.h"
#include "StatusProgress.h"
#include "StringDateChecker.h"
/**********************************************************************/
#include "CustomerExtractor.h"
/**********************************************************************/

CCustomerExtractor::CCustomerExtractor( int nLocIdx, const char* szDateFrom, const char* szDateTo, CCustomerCSVArray& arrayCustomers ) : m_arrayCustomers ( arrayCustomers )
{
	m_nLocIdx = nLocIdx;
	m_strCheckDateFrom = szDateFrom;
	m_strCheckDateTo = szDateTo;
	m_nTotalFileSize = 0;
	m_arrayCustomers.RemoveAt( 0, m_arrayCustomers.GetSize() );

	GetTermFileList();
	ExtractCustomers();
}

/**********************************************************************/

void CCustomerExtractor::GetTermFileList()
{
	CWaitCursor wait;

	m_arrayFileInfo.RemoveAll();
	m_nTotalFileSize = 0;

	int nNetworkIdx;
	int nNetworkNo = dbLocation.GetNetworkNo( m_nLocIdx );
	if ( dbNetwork.FindNetworkByNumber ( nNetworkNo, nNetworkIdx ) == FALSE )
		return;

	int nDbIdx;
	int nDbNo = dbLocation.GetDbNo( m_nLocIdx );
	if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
		return;

	CString strDataPath = dbNetwork.GetFolderPathData ( nNetworkIdx );
	strDataPath += "\\";

	CStringDateChecker StringDateChecker( m_strCheckDateFrom, m_strCheckDateTo );

	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strDataPath + "*.*" ) != 0 );

	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
			continue;

		//WE ONLY NEED TO CHECK DIRECTORIES
		if ( FileFinder.IsDirectory() == FALSE )
			continue;

		CString strDateFolder = FileFinder.GetFileName();

		CString strDateTran = "";
		CString strDateSale = "";
		if ( StringDateChecker.CheckDate( FALSE, HISTORYFILE_TYPE_ACCOUNT, strDateFolder, strDateTran, strDateSale ) == FALSE )
			continue;

		CString strPath;
		strPath.Format ( "%s%s\\",
			(const char*) strDataPath,
			(const char*) strDateFolder );

		for ( int nTermIdx = 0; nTermIdx < dbLocation.GetTerminalCount( m_nLocIdx ); nTermIdx++ )
		{
			CString strFilename;
			strFilename.Format( "Accnt.%3.3d", dbLocation.GetTNo ( m_nLocIdx, nTermIdx ) );
			
			CSSFile fileTest;
			if ( fileTest.Open ( strPath + strFilename, "rb", _SH_DENYNO ) == TRUE )
			{
				m_nTotalFileSize += fileTest.GetLength();
				fileTest.Close();

				CTermFileListInfo info;
				info.m_strFilename = strPath + strFilename;
				info.m_strDateFolder = strDateFolder;
				info.m_nCashRSPMode = 0;
				info.m_nDbIdx = nDbIdx;
				info.m_nLocIdx = m_nLocIdx;
				info.m_nTermIdx = nTermIdx;
				m_arrayFileInfo.Add( info );
			}
		}
	}	
}

/**********************************************************************/

void CCustomerExtractor::ExtractCustomers()
{
	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Finding Customer IDs" );
	
	for ( int nFileIdx = 0; nFileIdx < m_arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile = m_arrayFileInfo.GetAt( nFileIdx );

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		CString strBuffer;
		while( fileSales.ReadString( strBuffer ) )
		{
			CCSVAccountHistory csvIn( strBuffer );

			int nNewCustomerID = 0;

			switch( csvIn.GetAccountLineType() )
			{
			case ACCOUNT_LINETYPE_RECEIVE:
				if ( csvIn.GetReceiveType() == CASHRSP_ITEMPAYTYPE_CUSTOMER )
					nNewCustomerID = csvIn.GetReceiveID();
				break;

			case ACCOUNT_LINETYPE_CHARGE:
				if ( csvIn.GetChargeType() == CASHRSP_ITEMPAYTYPE_CUSTOMER )
					nNewCustomerID = csvIn.GetChargeID();
				break;

			case ACCOUNT_LINETYPE_MARKER:
				if ( csvIn.GetMarkerType() == CASHRSP_ITEMPAYTYPE_CUSTOMER )
					nNewCustomerID = csvIn.GetMarkerID();
				break;
			}	
			
			if ( nNewCustomerID != 0 )
			{
				int nCustomerIdx;
				if ( m_arrayCustomers.FindCustomerByID( nNewCustomerID, nCustomerIdx ) == FALSE )
				{
					CCustomerCSVRecord Customer;
					Customer.SetCustomerID( nNewCustomerID );
					Customer.SetSpecifyStartDayFlag( FALSE );
					m_arrayCustomers.InsertAt( nCustomerIdx, Customer );
				}
			}
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

CCustomerExtractor::~CCustomerExtractor(void)
{
}

/**********************************************************************/