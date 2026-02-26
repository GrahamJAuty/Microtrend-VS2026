/**********************************************************************/
#include "CashRSPFlusher.h"
/**********************************************************************/
#include "PosTrayCashRSPBuffer.h"
/**********************************************************************/

CPosTrayCashRSPBuffer::CPosTrayCashRSPBuffer() 
{
	m_pPosTrayCashRSPBufferThread = NULL;
	Reset();
}

/**********************************************************************/

void CPosTrayCashRSPBuffer::Reset()
{
	m_nStatus = POSTRAY_CASHRSPBUFFER_INACTIVE;
	m_bGotPendingNetworkInfo = FALSE;
	m_bGotClearedDate = FALSE;
	m_bGotPendingDate = FALSE;
	m_OleDateTimeCleared = COleDateTime( 2010, 1, 1, 0, 0, 0 );
	m_OleDateTimePending = COleDateTime( 2010, 1, 1, 0, 0, 0 );
	m_nPendingNetworks = 0;
	m_nPendingTransactions = 0;
}

/**********************************************************************/

CPosTrayCashRSPBuffer::~CPosTrayCashRSPBuffer(void)
{
	TerminateThread();
}

/**********************************************************************/

void CPosTrayCashRSPBuffer::SetPendingNetworkInfo( CArray<CPosTrayNetworkInfo,CPosTrayNetworkInfo>& arrayNetworkInfo )
{
	m_arrayPendingNetworkInfo.RemoveAll();
	for (int n = 0; n < arrayNetworkInfo.GetSize(); n++)
	{
		m_arrayPendingNetworkInfo.Add(arrayNetworkInfo.GetAt(n));
	}

	m_bGotPendingNetworkInfo = TRUE;
	m_nStatus = POSTRAY_CASHRSPBUFFER_WAITQUIT;
	m_PosTrayCashRSPBufferThreadInfo.SetForceQuitFlag(TRUE);
}

/**********************************************************************/

bool CPosTrayCashRSPBuffer::IsDateTimeCleared( COleDateTime& OleDateTime )
{
	if (FALSE == m_bGotClearedDate)
	{
		return FALSE;
	}

	if (OleDateTime.m_status != COleDateTime::valid)
	{
		return FALSE;
	}
	return ( OleDateTime <= m_OleDateTimeCleared );
}

/**********************************************************************/

bool CPosTrayCashRSPBuffer::SetPendingDateTime( COleDateTime& OleDateTime )
{
	switch( m_nStatus )
	{
	case POSTRAY_CASHRSPBUFFER_INACTIVE:
	case POSTRAY_CASHRSPBUFFER_COMPLETE:
		break;

	default:
		return FALSE;
	}

	if (OleDateTime.m_status != COleDateTime::valid)
	{
		return FALSE;
	}

	if ((OleDateTime <= m_OleDateTimeCleared) && (TRUE == m_bGotPendingDate))
	{
		return FALSE;
	}

	m_nStatus = POSTRAY_CASHRSPBUFFER_NEWTASK;
	m_bGotPendingDate = TRUE;
	m_OleDateTimePending = OleDateTime;

	return TRUE;
}

/**********************************************************************/

void CPosTrayCashRSPBuffer::Pulse()
{
	bool bAlreadyRunning = FALSE;

	if ( m_pPosTrayCashRSPBufferThread != NULL )
	{
		DWORD dwExitCode = 0;
		GetExitCodeThread( m_pPosTrayCashRSPBufferThread -> m_hThread, &dwExitCode );

		if (STILL_ACTIVE == dwExitCode)
		{
			bAlreadyRunning = TRUE;
		}
		else
		{
			delete m_pPosTrayCashRSPBufferThread;
			m_pPosTrayCashRSPBufferThread = NULL;
		}
	}

	if ( FALSE == bAlreadyRunning )
	{
		m_nPendingNetworks = m_PosTrayCashRSPBufferThreadInfo.GetNetworkInfoIdxToClearCount();
		m_nPendingTransactions = m_PosTrayCashRSPBufferThreadInfo.GetTransactionsToClearCount();

		if ( TRUE == m_bGotPendingNetworkInfo )
		{
			m_PosTrayCashRSPBufferThreadInfo.SetNetworkInfo( m_arrayPendingNetworkInfo );
			m_arrayPendingNetworkInfo.RemoveAll();
			m_bGotPendingNetworkInfo = FALSE;
			Reset();
		}
		else 
		{
			switch( m_nStatus )
			{
			case POSTRAY_CASHRSPBUFFER_WORKING:		
				if ( m_PosTrayCashRSPBufferThreadInfo.GetCompletionFlag() == TRUE )
				{
					m_nStatus = POSTRAY_CASHRSPBUFFER_COMPLETE;
					m_bGotClearedDate = TRUE;
					m_OleDateTimeCleared = m_OleDateTimePending;
					m_PosTrayCashRSPBufferThreadInfo.SetCompletionFlag(FALSE);
				}
				else
				{
					m_PosTrayCashRSPBufferThreadInfo.SetForceQuitFlag(FALSE);
					m_PosTrayCashRSPBufferThreadInfo.SetCompletionFlag(FALSE);
					m_PosTrayCashRSPBufferThreadInfo.SetNewTaskFlag(FALSE);
					m_pPosTrayCashRSPBufferThread = AfxBeginThread( CPosTrayCashRSPBufferThread::DoWork, &m_PosTrayCashRSPBufferThreadInfo, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL );
				
					if ( m_pPosTrayCashRSPBufferThread != NULL )
					{
						m_pPosTrayCashRSPBufferThread -> m_bAutoDelete = FALSE;
						m_pPosTrayCashRSPBufferThread -> ResumeThread();
					}
				}
				break;

			case POSTRAY_CASHRSPBUFFER_NEWTASK:
				{
					m_nPendingNetworks = 0;
					m_nPendingTransactions = 0;
					m_PosTrayCashRSPBufferThreadInfo.SetForceQuitFlag(FALSE);
					m_PosTrayCashRSPBufferThreadInfo.SetCompletionFlag(FALSE);
					m_PosTrayCashRSPBufferThreadInfo.SetNewTaskFlag(TRUE);
					m_pPosTrayCashRSPBufferThread = AfxBeginThread( CPosTrayCashRSPBufferThread::DoWork, &m_PosTrayCashRSPBufferThreadInfo, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL );
				
					if ( m_pPosTrayCashRSPBufferThread != NULL )
					{
						m_pPosTrayCashRSPBufferThread -> m_bAutoDelete = FALSE;
						m_pPosTrayCashRSPBufferThread -> ResumeThread();
						m_nStatus = POSTRAY_CASHRSPBUFFER_WORKING;
					}
				}
				break;
			}
		}
	}
}

/**********************************************************************/

void CPosTrayCashRSPBuffer::TerminateThread()
{
	if ( NULL != m_pPosTrayCashRSPBufferThread)
	{
		DWORD dwExitCode = 0;
		m_PosTrayCashRSPBufferThreadInfo.SetForceQuitFlag(TRUE);

		do
		{
			GetExitCodeThread( m_pPosTrayCashRSPBufferThread -> m_hThread, &dwExitCode );
		}
		while ( STILL_ACTIVE == dwExitCode );
		
		delete m_pPosTrayCashRSPBufferThread;
	}
}

/**********************************************************************/

const char* CPosTrayCashRSPBuffer::GetPendingWorkString()
{
	m_strPendingWork = "";

	switch( m_nStatus )
	{
	case POSTRAY_CASHRSPBUFFER_WORKING:
		if ((m_nPendingNetworks != 0) || (m_nPendingTransactions != 0))
		{
			m_strPendingWork.Format(" : Working %d:%d", m_nPendingNetworks, m_nPendingTransactions);
		}
		else
		{
			m_strPendingWork = " : Working";
		}
		break;

	case POSTRAY_CASHRSPBUFFER_NEWTASK:
		m_strPendingWork = " : Starting";
		break;

	case POSTRAY_CASHRSPBUFFER_WAITQUIT:
		m_strPendingWork = " : Stopping";
		break;

	case POSTRAY_CASHRSPBUFFER_COMPLETE:
		m_strPendingWork = " : Done";
		break;

	case POSTRAY_CASHRSPBUFFER_INACTIVE:
	default:
		m_strPendingWork = " : Waiting";
		break;
	}

	return m_strPendingWork;
}

/**********************************************************************/
