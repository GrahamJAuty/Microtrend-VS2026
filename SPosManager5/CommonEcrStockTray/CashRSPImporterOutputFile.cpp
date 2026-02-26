/**********************************************************************/
 
/**********************************************************************/
#include "CashRSPImporterDefines.h"
//#include "globalfunctions.h"
#include "NetworkCSVArray.h"
/**********************************************************************/
#include "CashRSPImporterOutputFile.h"
/**********************************************************************/

CCashRSPImporterOutputFile::CCashRSPImporterOutputFile( int nFileType, CCashRSPImporterBatchInfo& BatchInfo, CCashRSPImporterTransaction& TransactionInfo ) : m_BatchInfo( BatchInfo ), m_TransactionInfo( TransactionInfo )
{
	m_nFileType = nFileType;
	m_strCurrentFile = "";
	m_bIsOpen = FALSE;
	m_bLastOpenNewFile = FALSE;

	switch( m_nFileType )
	{
	case CASHRSP_DAILYFILE_SALES_TRAN:		
	case CASHRSP_DAILYFILE_SALES_ITEM:			m_strFilename = "sales";			break;
	case CASHRSP_DAILYFILE_REASONS_TRAN:		
	case CASHRSP_DAILYFILE_REASONS_ITEM:		m_strFilename = "reasons";			break;
	case CASHRSP_DAILYFILE_ITEMPAY:				m_strFilename = "itempay";			break;
	case CASHRSP_DAILYFILE_ACCOUNT:				m_strFilename = "accnt";			break;
	case CASHRSP_STOCKFILE_SALES:				m_strFilename = "stock";			break;
	case CASHRSP_STOCKFILE_DELIVERY:			m_strFilename = "stock_delivery";	break;
	}
}

/**********************************************************************/

bool CCashRSPImporterOutputFile::OpenFile()
{
	m_bLastOpenNewFile = FALSE;

	CString strOutputFolder;

	switch( m_nFileType )
	{
	case CASHRSP_TERMFILE_NONDUPLICATE:
		strOutputFolder = m_BatchInfo.m_strLocalPath;
		break;

	case CASHRSP_DAILYFILE_SALES_TRAN:
	case CASHRSP_DAILYFILE_REASONS_TRAN:
	case CASHRSP_DAILYFILE_ACCOUNT:
	case CASHRSP_DAILYFILE_ITEMPAY:
	case CASHRSP_DAILYFILE_TERM:
		strOutputFolder.Format ( "%s%s_%s",
			(const char*) m_BatchInfo.m_strLocalPath,
			(const char*) m_TransactionInfo.m_strDateTran,
			(const char*) m_TransactionInfo.m_strDateTran);
		break;

	case CASHRSP_DAILYFILE_SALES_ITEM:
	case CASHRSP_DAILYFILE_REASONS_ITEM:
		strOutputFolder.Format ( "%s%s_%s",
			(const char*) m_BatchInfo.m_strLocalPath,
			(const char*) m_TransactionInfo.m_strDateTran,
			(const char*) m_TransactionInfo.m_strDateItem );
		break;

	case CASHRSP_STOCKFILE_SALES:		
	case CASHRSP_STOCKFILE_DELIVERY:
		strOutputFolder = dbNetwork.GetFolderPathSyssetStock( m_BatchInfo.m_nNetworkIdx );
		break;
	}

	if ( ::ExistSubdirectory( strOutputFolder ) == FALSE )
		::CreateSubdirectory( strOutputFolder );

	CString strOutputFile = "";

	switch( m_nFileType )
	{
	case CASHRSP_DAILYFILE_TERM:
		GetDailyTermFilename();
	
		strOutputFile.Format ( "%s\\%s",
			(const char*) strOutputFolder,
			(const char*) m_strFilename );

		break;

	case CASHRSP_TERMFILE_NONDUPLICATE:
		{
			CString strFilename = "";
			::GetTermSaleFilename( CASHRSP_SALES_NONDUPLICATE, atoi( m_BatchInfo.m_strTNoOriginal ), strFilename );

			strOutputFile.Format( "%s\\%s",
				(const char*) strOutputFolder,
				(const char*) strFilename );
		}	
		break;

	default:
		strOutputFile.Format ( "%s\\%s.%s",
			(const char*) strOutputFolder,
			(const char*) m_strFilename,
			(const char*) m_TransactionInfo.m_strTNoSalesHistory );

		break;
	}

	if ( TRUE == m_bIsOpen )
	{
		if ( strOutputFile == m_strCurrentFile )
			return TRUE;
		else
			m_File.Close();
	}

	m_bIsOpen = m_File.Open( strOutputFile, "ab" );

	if ( TRUE == m_bIsOpen )
	{
		m_strCurrentFile = strOutputFile;
	}
	else
	{
		m_strCurrentFile = "";
	}

	m_bLastOpenNewFile = m_bIsOpen;

	return m_bIsOpen;
}

/**********************************************************************/

void CCashRSPImporterOutputFile::GetDailyTermFilename()
{	
	m_strFilename = "";

	int nTNo = atoi( m_BatchInfo.m_strTNoOriginal );

	switch( m_BatchInfo.m_nProcessType )
	{
	case CASHRSP_PROCESSTYPE_REBUILD:
		switch( m_BatchInfo.m_nFileType )
		{
		case CASHRSP_OBSOLETE_TERMSAVE:
		case CASHRSP_OBSOLETE_TRMSAVE2:
		case CASHRSP_SALES_TRADING:
			::GetTermSaleFilename( CASHRSP_SALES_TRADING, nTNo, m_strFilename );	
			break;
		}
		break;

	case CASHRSP_PROCESSTYPE_NEWSALE:
	case CASHRSP_PROCESSTYPE_RETRY:
	default:
		::GetTermSaleFilename( CASHRSP_SALES_TRADING, nTNo, m_strFilename );
		break;
	}
}

/**********************************************************************/

void CCashRSPImporterOutputFile::WriteLine( const char* szLine )
{
	if ( TRUE == m_bIsOpen )
		m_File.WriteLine( szLine );
}

/**********************************************************************/

void CCashRSPImporterOutputFile::CloseFile()
{
	if ( TRUE == m_bIsOpen )
		m_File.Close();

	m_bIsOpen = "";
	m_strFilename = "";
}

/**********************************************************************/
