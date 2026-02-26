/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "FileRemover.h"
#include "MessageLogger.h"
#include "PMSOptions.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "PMSCustomerCSVArray.h"
/**********************************************************************/

CPMSCustomerCSVRecord::CPMSCustomerCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CPMSCustomerCSVRecord::ClearRecord()
{
	m_nCustomerId = 1;
	m_strGuestName = "";
	m_strBillingName = "";
	m_strAddress1 = "";
	m_strAddress2 = "";
	m_strAddress3 = "";
	m_strPostcode = "";
	m_strPhone = "";
	m_strFax = "";
	m_strEmail = "";
}

/**********************************************************************/

void CPMSCustomerCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetCustomerId( csv.GetInt(0) );
	
	switch ( csv.GetInt(1) )
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	case 2:	V2ConvertFromCSV ( csv );	break;
	}
}

/**********************************************************************/

void CPMSCustomerCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetGuestName	( csv.GetString(2) );
	SetAddress1		( csv.GetString(3) );
	SetAddress2		( csv.GetString(4) );
	SetAddress3		( csv.GetString(5) );
	SetPostcode		( csv.GetString(6) );
	SetPhone		( csv.GetString(7) );
	SetFax			( csv.GetString(8) );
	SetEmail		( csv.GetString(9) );
}

/**********************************************************************/

void CPMSCustomerCSVRecord::V2ConvertFromCSV ( CCSV& csv )
{
	SetGuestName	( csv.GetString(2) );
	SetBillingName	( csv.GetString(3) );
	SetAddress1		( csv.GetString(4) );
	SetAddress2		( csv.GetString(5) );
	SetAddress3		( csv.GetString(6) );
	SetPostcode		( csv.GetString(7) );
	SetPhone		( csv.GetString(8) );
	SetFax			( csv.GetString(9) );
	SetEmail		( csv.GetString(10));
}

/**********************************************************************/

void CPMSCustomerCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nCustomerId );
	csv.Add ( PMS_CUSTOMER_VERSION );
	csv.Add ( m_strGuestName );
	csv.Add ( m_strBillingName );
	csv.Add ( m_strAddress1 );
	csv.Add ( m_strAddress2 );
	csv.Add ( m_strAddress3 );
	csv.Add ( m_strPostcode );
	csv.Add ( m_strPhone );
	csv.Add ( m_strFax );
	csv.Add ( m_strEmail );
}

/**********************************************************************/

void CPMSCustomerCSVRecord::SetCustomerId( int n )
{
	Set ( m_nCustomerId, n, PMSCustomer::CustomerId );
}

/**********************************************************************/

void CPMSCustomerCSVRecord::SetGuestName ( const char* sz )
{
	CString strName = sz;
	::TrimSpaces ( strName, FALSE );
	Set( m_strGuestName, strName, PMSCustomer::GuestName );
}

/**********************************************************************/

void CPMSCustomerCSVRecord::SetBillingName ( const char* sz )
{
	CString strName = sz;
	::TrimSpaces ( strName, FALSE );
	Set( m_strBillingName, strName, PMSCustomer::BillingName );
}

/**********************************************************************/

void CPMSCustomerCSVRecord::SetAddress1 ( const char* sz )
{
	Set( m_strAddress1, sz, PMSCustomer::Address );
}

/**********************************************************************/

void CPMSCustomerCSVRecord::SetAddress2 ( const char* sz )
{
	Set( m_strAddress2, sz, PMSCustomer::Address );
}

/**********************************************************************/

void CPMSCustomerCSVRecord::SetAddress3 ( const char* sz )
{
	Set( m_strAddress3, sz, PMSCustomer::Address );
}

/**********************************************************************/

void CPMSCustomerCSVRecord::SetPostcode ( const char* sz )
{
	Set( m_strPostcode, sz, PMSCustomer::Postcode );
}

/**********************************************************************/

void CPMSCustomerCSVRecord::SetPhone ( const char* sz )
{
	Set( m_strPhone, sz, PMSCustomer::Phone );
}

/**********************************************************************/

void CPMSCustomerCSVRecord::SetFax ( const char* sz )
{
	Set( m_strFax, sz, PMSCustomer::Fax );
}

/**********************************************************************/

void CPMSCustomerCSVRecord::SetEmail ( const char* sz )
{
	Set( m_strEmail, sz, PMSCustomer::Email );
}

/**********************************************************************/

CPMSCustomerCSVArray::CPMSCustomerCSVArray() 
{
}

/**********************************************************************/

CPMSCustomerCSVArray::~CPMSCustomerCSVArray() 
{
}

/**********************************************************************/

int CPMSCustomerCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}
	
	return nReply;
}

/**********************************************************************/

int CPMSCustomerCSVArray::GetCustomerId ( int nIdx )
{
	CString strTemp;
	if ( ( strTemp = GetAt ( nIdx ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		return csv.GetInt ( 0 );
	}
	return 0;
}

/**********************************************************************/

bool CPMSCustomerCSVArray::FindCustomerById( int nId, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;
		int nArrayId = GetCustomerId( nIndex );
			
		if ( nId == nArrayId )
			return TRUE;

		if ( nId < nArrayId )
			nEnd = nIndex - 1;
		else
			nStart = nIndex + 1;
	}

	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

int CPMSCustomerCSVArray::FindFirstFreeId()
{
	int nResult = 0;

	int nSize = GetSize();
	if ( 0 == nSize )
		nResult = 10;
	else
		nResult = GetCustomerId( nSize - 1 ) + 1;

	if ( nResult < PMSCustomer::CustomerId.Min || nResult > PMSCustomer::CustomerId.Max )
		nResult = 0;

	if ( ( nResult % 10 ) != 0 )
	{
		nResult -= nResult % 10;
		nResult += 10;
	}

	return nResult;
}

/**********************************************************************/

void CPMSCustomerCSVArray::InsertAndSaveCustomer( CPMSCustomerCSVRecord& CustomerRecord )
{
	int nPos;
	if ( DataManagerNonDb.PMSCustomer.FindCustomerById( CustomerRecord.GetCustomerId(), nPos ) == TRUE )
		DataManagerNonDb.PMSCustomer.SetAt( nPos, CustomerRecord );
	else	
		DataManagerNonDb.PMSCustomer.InsertAt( nPos, CustomerRecord );

	CFilenameUpdater FnUp( SysFiles::PMSCustomerNew );
	DataManagerNonDb.PMSCustomer.Write( FnUp.GetFilenameToUse() );
	
	CString strFilename;
	strFilename.Format( "%s\\%d.dat",
		PMSOptions.GetPMSFolderCustomer(),
		CustomerRecord.GetCustomerId() );

	CCSV csv;
	CustomerRecord.ConvertToCSV( csv );

	CSSFile fileCustomer;
	if ( fileCustomer.Open( strFilename, "wb" ) == TRUE )
		fileCustomer.WriteLine( csv.GetLine() );
}

/**********************************************************************/

void CPMSCustomerCSVArray::RemoveAndSaveCustomer( int nCustomerId )
{
	int nCustomerIdx;
	if ( DataManagerNonDb.PMSCustomer.FindCustomerById( nCustomerId, nCustomerIdx ) == TRUE )
	{
		DataManagerNonDb.PMSCustomer.RemoveAt( nCustomerIdx );

		CFilenameUpdater FnUp( SysFiles::PMSCustomerNew );
		DataManagerNonDb.PMSCustomer.Write( FnUp.GetFilenameToUse() );
	
		CString strFilename;
		strFilename.Format( "%s\\%d.dat",
			PMSOptions.GetPMSFolderCustomer(),
			nCustomerId );

		CFileRemover FileRemover( strFilename );
	}
}

/**********************************************************************/

void CPMSCustomerCSVArray::CreateCustomerBackup()
{
	StatusProgress.Lock( TRUE, "Creating customer backup files" );

	int nSize = GetSize();
	for ( int nCustomerIdx = 0; nCustomerIdx < nSize; nCustomerIdx++ )
	{
		StatusProgress.SetPos( nCustomerIdx, nSize );

		CPMSCustomerCSVRecord CustomerRecord;
		DataManagerNonDb.PMSCustomer.GetAt( nCustomerIdx, CustomerRecord );

		CString strFilename;
		strFilename.Format( "%s\\%d.dat",
			PMSOptions.GetPMSFolderCustomer(),
			CustomerRecord.GetCustomerId() );

		CCSV csv;
		CustomerRecord.ConvertToCSV( csv );

		CSSFile fileCustomer;
		if ( fileCustomer.Open( strFilename, "wb" ) == TRUE )
			fileCustomer.WriteLine( csv.GetLine() );
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CPMSCustomerCSVArray::RebuildFromCustomerFiles()
{
	RemoveAt( 0, GetSize() );

	CString strFilePath = "";
	strFilePath += PMSOptions.GetPMSFolderCustomer();
	strFilePath += "\\";
	
	CFileFind finder;
	BOOL bWorking = finder.FindFile ( strFilePath + "*.*" );

	while ( bWorking == TRUE )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDirectory() == TRUE )
			continue;

		if ( finder.IsDots() == TRUE )
			continue;

		CString strFileName = finder.GetFileName();
		strFileName.MakeUpper();

		int nLength = strFileName.GetLength();
		if ( nLength < 5 )
			continue;

		if ( strFileName.Right(4) != ".DAT" )
			continue;

		if ( ::TestNumeric( strFileName.Left( nLength - 4 ) ) == FALSE )
			continue;

		CSSFile fileCustomer;
		if ( fileCustomer.Open( strFilePath + strFileName, "rb" ) == FALSE )
			continue;

		CString strBuffer;
		fileCustomer.ReadString( strBuffer );
		fileCustomer.Close();

		CCSV csv( strBuffer );

		CPMSCustomerCSVRecord CustomerRecord;
		CustomerRecord.ConvertFromCSV( csv );

		int nCustomerIdx;
		int nCustomerId = CustomerRecord.GetCustomerId();
		if ( FindCustomerById( nCustomerId, nCustomerIdx ) == FALSE )
			InsertAt( nCustomerIdx, CustomerRecord );
	}
}

/**********************************************************************/

void CPMSCustomerCSVArray::ValidateCustomerDatabase()
{
	bool bRepairCustomerDatabase = FALSE;
	int nLastCustomerId = 0;
	int nCustomerCount = DataManagerNonDb.PMSCustomer.GetSize();
	for ( int nCustomerIdx = 0; nCustomerIdx < nCustomerCount; nCustomerIdx++ )
	{
		int nCustomerId = DataManagerNonDb.PMSCustomer.GetCustomerId( nCustomerIdx );

		if ( ( nCustomerId <= nLastCustomerId ) || ( nCustomerId < PMSCustomer::CustomerId.Min ) ||( nCustomerIdx > PMSCustomer::CustomerId.Max ) )
		{
			bRepairCustomerDatabase = TRUE;
			break;
		}

		nLastCustomerId = nCustomerId;
	}

	int nError = 0;
	if ( TRUE == bRepairCustomerDatabase )
		nError = 1;
	else if ( DataManagerNonDb.PMSCustomer.GetSize() == 0 )
	{
		CString strTestFile;
		strTestFile.Format( "%s\\10.dat", PMSOptions.GetPMSFolderCustomer() );
		
		if ( ::FileExists( strTestFile ) == TRUE )
			nError = 2;
		else
			nError = 3;
	}

	if ( 0 != nError ) 
	{
		int nSizeOld = DataManagerNonDb.PMSCustomer.GetSize();

		switch( nError )
		{
		case 1:
		case 2:
			{
				CString strMsg = "";

#ifndef STOCKMAN_SYSTEM
				strMsg += "Ecr Manager has detected a problem in the PMS customer index file.\n\n";
#else
				strMsg += "Stock Manager has detected a problem in the PMS customer index file.\n\n";
#endif

				strMsg += "The index file will be rebuilt from individual customer records.";
				Prompter.Error( strMsg );
			}
			break;
		}

		DataManagerNonDb.PMSCustomer.RebuildFromCustomerFiles();

		int nSizeNew = DataManagerNonDb.PMSCustomer.GetSize();
		
		CFilenameUpdater FnUp( SysFiles::PMSCustomerNew );
		DataManagerNonDb.PMSCustomer.Write( FnUp.GetFilenameToUse() );
	
		if ( ( 3 != nError ) || ( DataManagerNonDb.PMSCustomer.GetSize() > 0 ) )
		{
			CMessageLoggerInfo infoMsg;
			infoMsg.m_strMessage.Format ( "Rebuilt PMS Customer File [%d:%d:%d]", nError, nSizeOld, nSizeNew );
			infoMsg.m_strStatus = "";
			infoMsg.m_nHideLevel = 2;
			infoMsg.m_nNetworkIdx = 0;
			infoMsg.m_bAddDate = TRUE;
			infoMsg.m_bAddTimeOfDay = TRUE;
			infoMsg.m_bAddNetworkName = FALSE;
			MessageLogger.LogSchedulerMessage( infoMsg, FALSE, TRUE, FALSE );
		}
	}
}

/**********************************************************************/

void CPMSCustomerCSVArray::ReadAndUpdateCustomerDatabase()
{
	bool bCreateCustomerBackup = FALSE;

	CFilenameUpdater FnUpOld( SysFiles::PMSCustomerOld );
	CFilenameUpdater FnUpNew( SysFiles::PMSCustomerNew );

	if ( ::FileExists( FnUpNew.GetFilenameToUse() ) == FALSE )
	{
		if ( ::FileExists( FnUpOld.GetFilenameToUse() ) == TRUE )
		{
			if ( CopyFile( FnUpOld.GetFilenameToUse(), FnUpNew.GetFilenameToUse(), TRUE ) != 0 )
			{
				bCreateCustomerBackup = TRUE;
				CFileRemover FileRemover( FnUpOld.GetFilenameToUse() );
			}
		}
	}

	if ( DataManagerNonDb.PMSCustomer.Read( FnUpNew.GetFilenameToUse() ) == TRUE )
		if ( TRUE == bCreateCustomerBackup )
			DataManagerNonDb.PMSCustomer.CreateCustomerBackup();
}

/**********************************************************************/
#endif
/**********************************************************************/
