/**********************************************************************/
#include "FileRemover.h"
#include "..\CommonEcrStock\IDraughtExportCreator.h"
#include "..\CommonEcrStock\IDraughtExportManager.h"
#include "PosTrayOptions.h"
#include "..\CommonEcrStock\SageExportCreator.h"
#include "..\CommonEcrStock\SageExportManager.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "CashRSPFlusher.h"
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

long CCashRSPFlusher::DoComms( LPVOID pParam, int nType )
{
	CRealTimeCommsThreadInfo* pInfo = (CRealTimeCommsThreadInfo*) pParam;

	long lGoodResult = GetErrorCode( nType, CASHRSP_FLUSH_ERROR_NONE );
	long lResult = lGoodResult;
	
	for ( int nStage = 1; ( nStage <= 3 ) && ( lResult == lGoodResult ); nStage++ )
	{
		CArray<CRealTimeFileBufferInfo,CRealTimeFileBufferInfo> arrayBuffer;
		lResult = BuildCASHRSPBuffer( pInfo, arrayBuffer, nType, nStage );

		if ( ( lResult == lGoodResult ) && ( arrayBuffer.GetSize() != 0 ) )
			lResult = ProcessCASHRSPBuffer( pInfo, arrayBuffer, nType );
	}

	return lResult;
}

/**********************************************************************/

long CCashRSPFlusher::BuildCASHRSPBuffer ( CRealTimeCommsThreadInfo* pInfo, CArray<CRealTimeFileBufferInfo,CRealTimeFileBufferInfo>& arrayBuffer, int nType, int nStage )
{
	CFileFind finder;

	CString strPath = pInfo -> m_siteInfoSales.m_strEcrDataPath;

	BOOL bWorking = finder.FindFile ( strPath + "*.*" );

	if ( FALSE == bWorking )
	{
		switch( nType )
		{
		case CASHRSP_FLUSH_IDRAUGHT:
			IDraughtExportManager.SetExtraErrorInfo( strPath );
			break;

		case CASHRSP_FLUSH_SAGE:
			SageExportManager.SetExtraErrorInfo( strPath );
			break;
		}

		return GetErrorCode( nType, CASHRSP_FLUSH_ERROR_READ_SALES );
	}


	CString strMsg;
	strMsg.Format( "Finding CashRSP files (stage %d/3)", nStage );
	StatusProgress.Lock( TRUE, strMsg );

	while ( bWorking == TRUE )
	{
		bWorking = finder.FindNextFile();

		CRealTimeFileBufferInfo FileBufferInfo;
		if ( CCashRSPFlusher::CheckCashRSPFile( finder, FileBufferInfo ) == TRUE )
		{
			bool bFailed = FALSE;

			if ( 0 == FileBufferInfo.m_nRenameType )
			{
				CFileStatus FileStatus1;
				CString strOriginalPath = strPath + FileBufferInfo.GetOriginalFilename();
			
				if ( CFile::GetStatus( strOriginalPath, FileStatus1 ) == FALSE ) 
					bFailed = TRUE;
				else if ( 0 == FileStatus1.m_size )
					bFailed = TRUE;
			}
			
			if ( FALSE == bFailed )
			{
				arrayBuffer.Add ( FileBufferInfo );
				
				CString str;
				str.Format( "Files : %d", arrayBuffer.GetSize() );
				StatusProgress.SetDialogText( str );
			}
		}
	}

	StatusProgress.Unlock();

	return GetErrorCode( nType, CASHRSP_FLUSH_ERROR_NONE );
}

/**********************************************************************/

long CCashRSPFlusher::ProcessCASHRSPBuffer( CRealTimeCommsThreadInfo* pInfo, CArray<CRealTimeFileBufferInfo,CRealTimeFileBufferInfo>& arrayBuffer, int nType )
{
	CSSFile fileLock;
	if ( fileLock.Open( pInfo -> m_siteInfoSales.m_strNetworkLockFilename, "wb" ) == FALSE )
	{
		switch( nType )
		{
		case CASHRSP_FLUSH_IDRAUGHT:
			IDraughtExportManager.SetExtraErrorInfo( pInfo -> m_siteInfoSales.m_strNetworkLockFilename );
			break;

		case CASHRSP_FLUSH_SAGE:
			SageExportManager.SetExtraErrorInfo( pInfo -> m_siteInfoSales.m_strNetworkLockFilename );
			break;
		}
	
		return GetErrorCode( nType, CASHRSP_FLUSH_ERROR_NETWORK_LOCK );
	}
		
	CString strEcrDataPath = pInfo -> m_siteInfoSales.m_strEcrDataPath;
	
	bool bFail = FALSE;

	StatusProgress.Lock( TRUE, "Processing CashRSP Buffer" );
	int nSizeMax = arrayBuffer.GetSize();

	int nSizeNow = arrayBuffer.GetSize();
	while( nSizeNow != 0 )
	{
		StatusProgress.SetPos( nSizeMax - nSizeNow, nSizeMax );

		CString strTrans = arrayBuffer[0].GetDestinationFilename();
		int nRenameType = arrayBuffer[0].m_nRenameType;
		int nFileType = arrayBuffer[0].m_nFileType;

		CString strOriginalFilename = "";
		strOriginalFilename = strEcrDataPath;
		strOriginalFilename += arrayBuffer[0].GetOriginalFilename();

		CString strRenamedFilename = "";
		strRenamedFilename = strEcrDataPath;
		strRenamedFilename += arrayBuffer[0].GetRenamedFilename();

		CString strLocalTempFile = "";
		strLocalTempFile += pInfo -> m_siteInfoSales.m_strLocalDestPath;
		strLocalTempFile += strTrans;
	
		if ( ProcessRealTimeFile ( pInfo, nRenameType, strOriginalFilename, strRenamedFilename, strLocalTempFile, nFileType ) == TRUE )
		{
			if ( nRenameType != 99 )
			{
				CFileRemover FileRemover;
				FileRemover.RemoveFile( strLocalTempFile );
			}

			pInfo -> m_nTransactionCount++;
			arrayBuffer.RemoveAt(0);
			nSizeNow = arrayBuffer.GetSize();
		}
		else
		{
			switch( nType )
			{
			case CASHRSP_FLUSH_IDRAUGHT:
				IDraughtExportManager.SetExtraErrorInfo( strTrans );
				break;

			case CASHRSP_FLUSH_SAGE:
				SageExportManager.SetExtraErrorInfo( strTrans );
				break;
			}

			CFileRemover FileRemover( strLocalTempFile );
			bFail = TRUE;
			break;
		}
	}

	StatusProgress.Unlock();

	fileLock.Close();

	if ( FALSE == bFail )
		return GetErrorCode( nType, CASHRSP_FLUSH_ERROR_NONE );
	else
		return GetErrorCode( nType, CASHRSP_FLUSH_ERROR_COPY_SALES );
	
	return 0;
}

/**********************************************************************/

long CCashRSPFlusher::GetErrorCode( int nType, int nError )
{
	long lResult = 0l;

	switch( nType )
	{
	case CASHRSP_FLUSH_IDRAUGHT:
		switch( nError )
		{
		case CASHRSP_FLUSH_ERROR_NONE:			lResult = IDRAUGHT_EXPORT_ERROR_NONE;			break;
		case CASHRSP_FLUSH_ERROR_READ_SALES:	lResult = IDRAUGHT_EXPORT_ERROR_READ_SALES;		break;
		case CASHRSP_FLUSH_ERROR_COPY_SALES:	lResult = IDRAUGHT_EXPORT_ERROR_COPY_SALES;		break;
		case CASHRSP_FLUSH_ERROR_NETWORK_LOCK:	lResult = IDRAUGHT_EXPORT_ERROR_NETWORK_LOCK;	break;
		case CASHRSP_FLUSH_ERROR_NETWORK_NOPATH:lResult = IDRAUGHT_EXPORT_ERROR_NETWORK_NOPATH;	break;
		}
		break;

	case CASHRSP_FLUSH_SAGE:
		switch( nError )
		{
		case CASHRSP_FLUSH_ERROR_NONE:			lResult = SAGE_EXPORT_ERROR_NONE;				break;
		case CASHRSP_FLUSH_ERROR_READ_SALES:	lResult = SAGE_EXPORT_ERROR_READ_SALES;			break;
		case CASHRSP_FLUSH_ERROR_COPY_SALES:	lResult = SAGE_EXPORT_ERROR_COPY_SALES;			break;
		case CASHRSP_FLUSH_ERROR_NETWORK_LOCK:	lResult = SAGE_EXPORT_ERROR_NETWORK_LOCK;		break;
		case CASHRSP_FLUSH_ERROR_NETWORK_NOPATH:lResult = SAGE_EXPORT_ERROR_NETWORK_NOPATH;		break;
		}
		break;

	case CASHRSP_FLUSH_SYSTEM:
		switch( nError )
		{
		case CASHRSP_FLUSH_ERROR_NONE:			lResult = 0l;									break;
		default:								lResult = 1l;									break;
		}
		break;
	}

	return lResult;
}

/**********************************************************************/
#endif
/**********************************************************************/

bool CCashRSPFlusher::ProcessRealTimeFile ( CRealTimeCommsThreadInfo* pInfo, int nRenameType, CString& strOriginalFilename, CString& strRenamedFilename, CString& strLocalTempFile, int nFileType )
{	
	bool bResult = FALSE;

	switch( nRenameType )
	{
	case 0:
		{
			if ( rename( strOriginalFilename, strRenamedFilename ) != 0 )
				CRealTimeCommsThread::LogRealTimeCommsResult( pInfo, "Unable to rename file", "Old name", strOriginalFilename, "New name", strRenamedFilename );	
			else
				bResult = TRUE;
		}
		break;

	case 1:
		{
			bool bFailed = FALSE;

			CString strTerminal = strOriginalFilename.Right(3);
	
			//COPY UPAUDIT FILE TO LOCAL SPACE
			if ( CopyFile( strOriginalFilename, strLocalTempFile, FALSE ) == 0 )
			{
				CRealTimeCommsThread::LogRealTimeCommsResult( pInfo, "Unable to copy file", "Source", strOriginalFilename, "Destination", strLocalTempFile );
				bFailed = TRUE;
			}

			CFileStatus FileStatus1, FileStatus2;
				
			//CHECK THAT COPY IS SAME SIZE AS ORIGINAL
			if ( FALSE == bFailed )
			{
				if ( CFile::GetStatus( strOriginalFilename, FileStatus1 ) == FALSE )
				{
					CRealTimeCommsThread::LogRealTimeCommsResult( pInfo, "Unable to check file status", "File", strOriginalFilename, "", "" );
					bFailed = TRUE;
				}
				else if ( CFile::GetStatus( strLocalTempFile, FileStatus2 ) == FALSE )
				{
					CRealTimeCommsThread::LogRealTimeCommsResult( pInfo, "Unable to check file status", "File", strLocalTempFile, "", "" );
					bFailed = TRUE;
				}
				else if ( FileStatus1.m_size != FileStatus2.m_size )
				{
					CRealTimeCommsThread::LogRealTimeCommsResult( pInfo, "Error in copied file", "Source", strOriginalFilename, "Destination", strLocalTempFile );
					bFailed = TRUE;
				}
			}

			//ACTUALLY PROCESS THE FILE
			if ( FALSE == bFailed )
			{
				CString strLocalSalesFilename = pInfo -> m_siteInfoSales.m_strLocalDestPath;
				
				switch( nFileType )
				{
				case REALTIME_FILEBUFFER_DEPOSIT:
					strLocalSalesFilename += sz5000DepoFilename;
					break;

				case REALTIME_FILEBUFFER_EOD:
					strLocalSalesFilename += sz5000EODFilename;
					break;

				case REALTIME_FILEBUFFER_SALE:
				default:
					strLocalSalesFilename += sz5000TermFilename;
					break;
				}
					
				strLocalSalesFilename += ".";
				strLocalSalesFilename += strTerminal;
					
				//SCOPE HERE TO CLOSE FILES AT END OF THIS SECTION
				{
					CSSFile fileSource, fileLocalSales, fileRemoteSales;
					if ( fileSource.Open( strLocalTempFile, "rb" ) == FALSE )
					{
						CRealTimeCommsThread::LogRealTimeCommsResult( pInfo, "Unable to read file", "Filename", strLocalTempFile, "", "" );
						bFailed = TRUE;
					}

					if ( FALSE == bFailed )
					{
						if ( TRUE == pInfo -> m_bEnableLocal )
						{
							if ( fileLocalSales.Open( strLocalSalesFilename, "ab" ) == FALSE )
							{
								CRealTimeCommsThread::LogRealTimeCommsResult( pInfo, "Unable to append to file", "Source", strLocalTempFile, "Destination", strLocalSalesFilename );
								bFailed = TRUE;
							}
						}
					}
	
					if ( FALSE == bFailed )
					{
						if ( rename( strOriginalFilename, strRenamedFilename ) != 0 )
						{
							CRealTimeCommsThread::LogRealTimeCommsResult( pInfo, "Unable to rename file", "Old name", strOriginalFilename, "New name", strRenamedFilename );
							bFailed = TRUE;
						}
					}

					if ( FALSE == bFailed )
					{
						CString strBegin = "$,BEGIN,";
						strBegin += strLocalTempFile;

						int nLineCount = 0;

						CString strBuffer;
						while( fileSource.ReadString( strBuffer ) == TRUE )
						{
							if  ( ( 0 == nLineCount ) && ( TRUE == pInfo -> m_bLogAllCASHRSPFiles ) )
							{
								if ( TRUE == pInfo -> m_bEnableLocal )
									fileLocalSales.WriteLine( strBegin );
							}

							if ( TRUE == pInfo -> m_bEnableLocal )
								fileLocalSales.WriteLine( strBuffer );
						
							nLineCount++;
						}

						if ( TRUE == pInfo -> m_bLogAllCASHRSPFiles )
						{
							CString strEnd;
							strEnd.Format ( "$,END,%s,%d", (const char*) strLocalTempFile, nLineCount );
							
							if ( TRUE == pInfo -> m_bEnableLocal )
								fileLocalSales.WriteLine( strEnd );
							
							CString strMsg;
							strMsg.Format( "Appended %d lines", nLineCount );
							CRealTimeCommsThread::LogRealTimeCommsResult( pInfo, strMsg, "Source", strOriginalFilename, "", "" );

							if ( TRUE == pInfo -> m_bEnableLocal )
							{
								CString strMsg = "Local Dest :";
								strMsg += strLocalSalesFilename;
								CRealTimeCommsThread::LogRealTimeCommsMessage( pInfo, strMsg );
							}
						}
					}
				}
			}
			bResult = ( FALSE == bFailed );
		}
		break;

	case 99:
		{
			CFileRemover FileRemover;
			bResult = FileRemover.RemoveFile( strOriginalFilename );
		}
		break;
	}

	if ( nRenameType != 99 )
		CFileRemover FileRemover( strLocalTempFile );

	return bResult;
}

/**********************************************************************/

bool CCashRSPFlusher::CheckCashRSPFile( CFileFind& finder, CRealTimeFileBufferInfo& FileBufferInfo )
{
	if ( finder.IsDirectory() == TRUE )
		return FALSE;

	if ( finder.IsDots() == TRUE )
		return FALSE;

	CString strFileName = finder.GetFileName();
		
	int nFileNameLen = strFileName.GetLength();
	if ( nFileNameLen < 7 )
		return FALSE;

	CString strTerminal = strFileName.Right(3);
	if ( ::TestNumeric( strTerminal ) == FALSE )
		return FALSE;

	int nEcr = atoi( strTerminal );
	if ( ( nEcr < 1 ) || ( nEcr > 999 ) )
		return FALSE;

	if ( strFileName.Mid( nFileNameLen - 4, 1 ) != "." )
		return FALSE;

	strFileName.MakeUpper();
		
	CString strFileTitle = strFileName.Left( nFileNameLen - 4 );

	int nRenameType = 0;
	strFileName.MakeUpper();

	if ( strFileName.Left(6) == "SSTEMP" )
	{
		if ( nFileNameLen >= 16 )
		{
			if ( strFileName.Mid(8,1) == "_" )
			{
				CString strNum = strFileName.Mid(6,2);
				if ( ::TestNumeric( strNum ) == TRUE )
				{
					nRenameType = atoi( strNum );

					if ( nRenameType != 0 )
					{
						CString strTemp = strFileName;
						strFileName =	strTemp.Mid( 9, nFileNameLen - 9 );
						strFileTitle =	strTemp.Mid( 9, nFileNameLen - 13 );
						nFileNameLen -= 9;
					}
				}
			}
		}
	}

	CString strTransaction = "";
	int nFileType = REALTIME_FILEBUFFER_SALE;

	if ( strFileTitle.Left(2) == "TA" )
	{
		int nEventEditLen = nFileNameLen - 6;
		CString strEventEdit = strFileTitle.Right( nEventEditLen );
		
		CString strEventID = strEventEdit.SpanExcluding( "_" );
		int nEventIDLen = strEventID.GetLength();

		if ( nEventIDLen <= nEventEditLen - 2 )
		{
			CString strEditCount = strFileTitle.Right( nEventEditLen - ( nEventIDLen + 1 ) );

			if ( ::TestNumeric( strEventID ) == TRUE )
				if ( ::TestNumeric( strEditCount ) == TRUE )
					strTransaction = strFileTitle;
		}
	}
	else if ( strFileTitle.Left(2) == "DP" )
	{
		//EG. TEST NUMERIC ON 123 FROM DP123.001
		if ( ::TestNumeric( strFileTitle.Right( nFileNameLen - 6 ) ) == TRUE )
		{
			strTransaction = strFileTitle;
			nFileType = REALTIME_FILEBUFFER_DEPOSIT;
		}
	}
	else if ( 12 == nFileNameLen )
	{
		if ( ::TestNumericHex( strFileTitle ) == TRUE )
			strTransaction = strFileTitle;

		if ( strFileTitle.Left(1) == "E" )
			nFileType = REALTIME_FILEBUFFER_EOD;
	}
	else if ( 30 == nFileNameLen )
	{
		if ( ::TestNumericHex( strFileTitle.Left(8) ) == TRUE )
			if ( ::TestNumericHex( strFileTitle.Right(17) ) == TRUE )
				strTransaction = strFileTitle;
	}

	if ( strTransaction == "" )
		return FALSE;

	FileBufferInfo.m_strFileTitle = strTransaction;
	FileBufferInfo.m_strFileExt = strTerminal;
	FileBufferInfo.m_nFileType = nFileType;
	FileBufferInfo.m_nRenameType = nRenameType;
	return TRUE;
}

/**********************************************************************/
