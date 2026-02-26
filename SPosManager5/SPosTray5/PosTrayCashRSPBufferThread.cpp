/**********************************************************************/
#include "CashRSPFlusher.h"
#include "PosTrayCashRSPBufferThread.h"
/**********************************************************************/

CPosTrayCashRSPBufferThreadInfo::CPosTrayCashRSPBufferThreadInfo()
{
	m_bForceQuit = FALSE;
	m_bNewTask = TRUE;
	m_bCompletion = FALSE;
}

/**********************************************************************/

void CPosTrayCashRSPBufferThreadInfo::SetNetworkInfo( CArray<CPosTrayNetworkInfo,CPosTrayNetworkInfo>& arrayPendingNetworkInfo )
{
	m_arrayNetworkInfo.RemoveAll();
	for (int n = 0; n < arrayPendingNetworkInfo.GetSize(); n++)
	{
		m_arrayNetworkInfo.Add(arrayPendingNetworkInfo.GetAt(n));
	}
}

/**********************************************************************/

void CPosTrayCashRSPBufferThreadInfo::GetNetworkInfo( int n, CPosTrayNetworkInfo& infoNetwork )
{
	if ((n >= 0) && (n < m_arrayNetworkInfo.GetSize()))
	{
		infoNetwork = m_arrayNetworkInfo.GetAt(n);
	}
}

/**********************************************************************/

void CPosTrayCashRSPBufferThreadInfo::SetNetworkInfoIdxToClear()
{
	m_arrayNetworkInfoIdxToClear.RemoveAll();
	for (int n = 0; n < m_arrayNetworkInfo.GetSize(); n++)
	{
		m_arrayNetworkInfoIdxToClear.Add(n);
	}
}

/**********************************************************************/

int CPosTrayCashRSPBufferThreadInfo::GetNetworkInfoIdxToClear(int n)
{
	if ((n >= 0) && (n < m_arrayNetworkInfoIdxToClear.GetSize()))
	{
		return m_arrayNetworkInfoIdxToClear.GetAt(n);
	}
	else
	{
		return -1;
	}
}

/**********************************************************************/

void CPosTrayCashRSPBufferThreadInfo::RemoveNetworkInfoIdxToClear(int n)
{
	if ((n >= 0) && (n < m_arrayNetworkInfoIdxToClear.GetSize()))
	{
		m_arrayNetworkInfoIdxToClear.RemoveAt(n);
	}
}

/**********************************************************************/

void CPosTrayCashRSPBufferThreadInfo::AddTransactionsToClear( CArray<CPosTrayCashRSPBufferFileInfo>& arrayFiles )
{
	for (int x = 0; x < arrayFiles.GetSize(); x++)
	{
		m_arrayTransactionsToClear.Add(arrayFiles.GetAt(x));
	}
}

/**********************************************************************/

void CPosTrayCashRSPBufferThreadInfo::GetTransactionToClear( int n, CPosTrayCashRSPBufferFileInfo& infoTran )
{
	if ((n >= 0) && (n < m_arrayTransactionsToClear.GetSize()))
	{
		infoTran = m_arrayTransactionsToClear.GetAt(n);
	}
}

/**********************************************************************/

void CPosTrayCashRSPBufferThreadInfo::RemoveTransactionToClear(int n)
{
	if ((n >= 0) && (n < m_arrayTransactionsToClear.GetSize()))
	{
		m_arrayTransactionsToClear.RemoveAt(n);
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

UINT CPosTrayCashRSPBufferThread::DoWork( LPVOID pParam )
{
	CPosTrayCashRSPBufferThreadInfo* pInfo = (CPosTrayCashRSPBufferThreadInfo*) pParam;
	DoWorkInternal( pInfo );
	return 0;
}

/**********************************************************************/

void CPosTrayCashRSPBufferThread::DoWorkInternal( CPosTrayCashRSPBufferThreadInfo* pInfo )
{
	{
		CString str;
		str.Format( "POSTRAYBUFFERTASK IN %d %d %d\n",
			pInfo -> GetNewTaskFlag(),
			pInfo -> GetNetworkInfoIdxToClearCount(),
			pInfo -> GetTransactionsToClearCount() );
		TRACE(str);
	}

	if ( pInfo -> GetNewTaskFlag() == TRUE )
	{
		pInfo -> SetNetworkInfoIdxToClear();
		pInfo -> ResetTransactionsToClear();
	}

	for ( int n = pInfo -> GetNetworkInfoIdxToClearCount() - 1; n >= 0; n-- )
	{
		int nIdx = pInfo -> GetNetworkInfoIdxToClear(n);
		
		bool bForceQuit = FALSE;
		CArray<CPosTrayCashRSPBufferFileInfo> arrayFiles;
		if ( GetCashRSPFileList( pInfo, nIdx, bForceQuit, arrayFiles ) == TRUE )
		{
			pInfo -> RemoveNetworkInfoIdxToClear(n);
			pInfo -> AddTransactionsToClear( arrayFiles );
		}

		if ( TRUE == bForceQuit  )
		{
			TRACE( "POSTRAYBUFFERTASK FORCE QUIT 1\n" );
			return;
		}
	}

	for ( int n = pInfo -> GetTransactionsToClearCount() - 1; n >= 0; n-- )
	{
		CPosTrayCashRSPBufferFileInfo infoPosTrayFile;
		pInfo -> GetTransactionToClear( n, infoPosTrayFile );

		if ((infoPosTrayFile.m_nNetworkInfoIdx < 0) || (infoPosTrayFile.m_nNetworkInfoIdx >= pInfo->GetNetworkInfoCount()))
		{
			pInfo->RemoveTransactionToClear(n);
		}
		else
		{
			CPosTrayNetworkInfo infoNetwork;
			pInfo -> GetNetworkInfo( infoPosTrayFile.m_nNetworkInfoIdx, infoNetwork );

			bool bStillExists = FALSE;
			CString strPath = infoNetwork.m_strNetworkPath;
			CString strFilename = infoPosTrayFile.m_infoFile.GetOriginalFilename();
			
			CRealTimeFileBufferInfo infoFile = infoPosTrayFile.m_infoFile;
			int nRenameTypeWhenDetected = infoFile.m_nRenameType;
			
			if ( 0 == nRenameTypeWhenDetected )
			{
				infoFile.m_nRenameType = 0;
				CString strFilename = infoFile.GetOriginalFilename();
				bStillExists = ::FileExists( strPath + strFilename  );
			}

			if ( TRUE == pInfo -> GetForceQuitFlag()   )
			{
				TRACE( "POSTRAYBUFFERTASK FORCE QUIT 2\n" );
				return;
			}
			
			if ( ( 99 != nRenameTypeWhenDetected ) && ( FALSE == bStillExists ) )
			{
				infoFile.m_nRenameType = 1;
				CString strFilename = infoFile.GetOriginalFilename();
				bStillExists = ::FileExists( strPath + strFilename  );
			}

			if ( TRUE == pInfo ->GetForceQuitFlag()   )
			{
				TRACE( "POSTRAYBUFFERTASK FORCE QUIT 3\n" );
				return;
			}

			if ( FALSE == bStillExists )
			{
				infoFile.m_nRenameType = 99;
				CString strFilename = infoFile.GetOriginalFilename();
				bStillExists = ::FileExists( strPath + strFilename  );
			}

			if ( TRUE == pInfo ->GetForceQuitFlag()   )
			{
				TRACE( "POSTRAYBUFFERTASK FORCE QUIT 4\n" );
				return;
			}

			if (FALSE == bStillExists)
			{
				pInfo->RemoveTransactionToClear(n);
			}
		}
	}

	if (pInfo->GetNetworkInfoIdxToClearCount() == 0)
	{
		if (pInfo->GetTransactionsToClearCount() == 0)
		{
			pInfo->SetCompletionFlag(TRUE);
		}
	}

	{
		CString str;
		str.Format( "POSTRAYBUFFERTASK OUT %d %d %d\n",
			pInfo -> GetNewTaskFlag(),
			pInfo -> GetNetworkInfoIdxToClearCount(),
			pInfo -> GetTransactionsToClearCount() );
		TRACE(str);
	}
}

/**********************************************************************/

bool CPosTrayCashRSPBufferThread::GetCashRSPFileList( CPosTrayCashRSPBufferThreadInfo* pInfo, int nIdx, bool& bForceQuit, CArray<CPosTrayCashRSPBufferFileInfo>& arrayFiles )
{
	arrayFiles.RemoveAll();

	if ((nIdx < 0) || (nIdx >= pInfo->GetNetworkInfoCount()))
	{
		return TRUE;
	}

	CPosTrayNetworkInfo infoNetwork;
	pInfo -> GetNetworkInfo( nIdx, infoNetwork );

	CFileFind finder;
	CString strPath = infoNetwork.m_strNetworkPath;
	BOOL bWorking = finder.FindFile ( strPath + "*.*" );

	if ( TRUE == pInfo -> GetForceQuitFlag() )
	{
		TRACE( "POSTRAYBUFFERTASK FORCE QUIT 1.1\n" );
		bForceQuit = TRUE;
		return FALSE;
	}
	
	while ( ( bWorking == TRUE ) && ( ( arrayFiles.GetSize() <= 100 ) ) )
	{
		bWorking = finder.FindNextFile();

		if ( TRUE == pInfo -> GetForceQuitFlag() )
		{
			TRACE( "POSTRAYBUFFERTASK FORCE QUIT 1.2\n" );
			bForceQuit = TRUE;
			return FALSE;
		}

		CPosTrayCashRSPBufferFileInfo infoPosTrayFile;
		infoPosTrayFile.m_nNetworkInfoIdx = nIdx;
		
		if ( CCashRSPFlusher::CheckCashRSPFile( finder, infoPosTrayFile.m_infoFile ) == TRUE )
		{
			CFileStatus FileStatus;

			if (CFile::GetStatus(finder.GetFilePath(), FileStatus) == TRUE)
			{
				if (FileStatus.m_size != 0)
				{
					if (REALTIME_FILEBUFFER_SALE == infoPosTrayFile.m_infoFile.m_nFileType)
					{
						switch (infoPosTrayFile.m_infoFile.m_nRenameType)
						{
						case 0:
						case 1:
						case 99:
							arrayFiles.Add(infoPosTrayFile);
							break;
						}
					}
				}
			}
		}

		if ( TRUE == pInfo -> GetForceQuitFlag() )
		{
			TRACE( "POSTRAYBUFFERTASK FORCE QUIT 1.3\n" );
			bForceQuit = TRUE;
			return FALSE;
		}
	}

	return ( arrayFiles.GetSize() <= 100 );
}

/**********************************************************************/
