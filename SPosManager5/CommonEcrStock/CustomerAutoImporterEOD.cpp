/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "LocationCSVArray.h"
#include "MessageLogger.h"
#include "SQLImporter.h"
#include "SysInfo.h"
/**********************************************************************/
#include "CustomerAutoImporterEOD.h"
/**********************************************************************/

CCustomerAutoImporterEOD::CCustomerAutoImporterEOD()
{
	m_nLocIdx = 0;

	m_bReadyToGo = FALSE;
}

/**********************************************************************/

void CCustomerAutoImporterEOD::CheckLocIdx()
{
	if ( m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize() )
		m_nLocIdx = 0;
}

/**********************************************************************/

void CCustomerAutoImporterEOD::GotoNextLoc()
{
	m_nLocIdx++;
	if ( m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize() )
		m_nLocIdx = 0;
}

/**********************************************************************/

void CCustomerAutoImporterEOD::ImportCustomerIdleTime()
{
	if ( m_bReadyToGo == FALSE )
		return;

	CheckLocIdx();

	if ( dbLocation.GetSize() == 0 )
		return;

	CString strPath = "";
	strPath += dbLocation.GetFolderPathSyssetPendingCustomerLocal( m_nLocIdx );
	strPath += "\\";

	CFileFind finder;
	bool bWorking = ( finder.FindFile ( strPath + "CUSTOMER*.SQL" ) != 0 );
	while (bWorking)   		
	{
		( bWorking = finder.FindNextFile() != 0 );

		if ( finder.IsDots() == TRUE )
			continue;
			
		if ( finder.IsDirectory() == TRUE )
			continue;

		CCustomerAutoImporterEODInfo infoFile;
		infoFile.m_nLocIdx = m_nLocIdx;	
		infoFile.m_strFilenameOriginal = finder.GetFileName();
		
		CDataManagerInfo info;
		if ( DataManagerNonDb.OpenCustomerAll( DB_READWRITE, m_nLocIdx, info ) == TRUE )
		{
			DataManagerNonDb.CustomerAccount.LoadHeaderStartDateRecord();
			DataManagerNonDb.CustomerAccount.MoveDeletionsToHelperArray();
			DataManagerNonDb.CustomerOneOff.LoadHeaderStartDateRecord();
			DataManagerNonDb.CustomerOneOff.MoveDeletionsToHelperArray();
			ImportCustomerInternal( infoFile );	
			DataManagerNonDb.CloseCustomerAll( info );
		}
	}
}

/**********************************************************************/

void CCustomerAutoImporterEOD::ImportCustomerExternal( int nLocIdx )
{
	CString strPath = "";
	strPath += dbLocation.GetFolderPathSyssetPendingCustomerLocal( nLocIdx );
	strPath += "\\";

	CFileFind finder;
	bool bWorking = ( finder.FindFile ( strPath + "CUSTOMER*.SQL" ) != 0 );
	while (bWorking)   		
	{
		( bWorking = finder.FindNextFile() != 0 );

		if ( finder.IsDots() == TRUE )
			continue;
			
		if ( finder.IsDirectory() == TRUE )
			continue;

		CCustomerAutoImporterEODInfo infoFile;
		infoFile.m_nLocIdx = nLocIdx;
		infoFile.m_strFilenameOriginal = finder.GetFileName();	
		ImportCustomerInternal( infoFile );
	}
}

/**********************************************************************/

void CCustomerAutoImporterEOD::ImportCustomerInternal( CCustomerAutoImporterEODInfo& infoFile )
{
	int nLocIdx = infoFile.m_nLocIdx;
	CString strPath = dbLocation.GetFolderPathSyssetPendingCustomerLocal( nLocIdx );

	CString strImportFileOriginal = "";
	strImportFileOriginal += strPath;
	strImportFileOriginal += "\\";
	strImportFileOriginal += infoFile.m_strFilenameOriginal;

	DataManagerNonDb.CustomerAccount.SetDeletionStatus( CUSTOMER_DELETIONSTATUS_CANDELETE );

	CSQLImporter SQLImporter( strImportFileOriginal );
	SQLImporter.ImportCustomersOnly( TRUE );
	int nAdded = SQLImporter.GetCustomersAdded();
	int nModified = SQLImporter.GetCustomersModified();
	
	CDataManagerInfo info;
	DataManagerNonDb.CustomerAccount.SaveHeaderStartDateRecord();
	DataManagerNonDb.CustomerAccount.MoveDeletionsToMainArray();
	DataManagerNonDb.CustomerOneOff.SaveHeaderStartDateRecord();
	DataManagerNonDb.CustomerOneOff.MoveDeletionsToMainArray();
	DataManagerNonDb.WriteCustomerAll( info );
	DataManagerNonDb.CustomerAccount.LoadHeaderStartDateRecord();
	DataManagerNonDb.CustomerAccount.MoveDeletionsToHelperArray();
	DataManagerNonDb.CustomerOneOff.LoadHeaderStartDateRecord();
	DataManagerNonDb.CustomerOneOff.MoveDeletionsToHelperArray();

	CFileRemover FileRemover ( strImportFileOriginal );
	
	CMessageLoggerInfo infoMsg;
	infoMsg.m_nHideLevel = 2;
	infoMsg.m_bAddNetworkName = FALSE;
	infoMsg.m_strMessage = "CUSTOMER.SQL Import";
	MessageLogger.LogSchedulerMessage ( infoMsg, FALSE, TRUE, FALSE );

	infoMsg.m_strMessage.Format( "LOCATION: Nwk%3.3d Loc%3.3d (%s)",
		dbLocation.GetNetworkNo( m_nLocIdx ),
		dbLocation.GetLocNo( m_nLocIdx ),
		dbLocation.GetName( m_nLocIdx ) );
	MessageLogger.LogSchedulerMessage ( infoMsg, FALSE, TRUE, FALSE );

	infoMsg.m_strMessage.Format( "RESULT: (%d:%d)",
		nAdded,
		nModified );
	MessageLogger.LogSchedulerMessage ( infoMsg, FALSE, TRUE, FALSE );
}

/**********************************************************************/

