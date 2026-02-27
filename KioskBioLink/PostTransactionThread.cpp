/***********************************************************************/
#include "GlobalFunctions.h"	
#include "PostTransaction.h"	
/***********************************************************************/
#include "PostTransactionThread.h"
/**********************************************************************/

CPostTransactionThreadInfo::CPostTransactionThreadInfo()
{
}

/**********************************************************************/

UINT CPostTransactionThread::DoWork(LPVOID pParam)
{
	CPostTransactionThreadInfo* pInfo = (CPostTransactionThreadInfo*)pParam;
	DoWorkInternal(pInfo);
	return 0;
}

//****************************************************************************
// Create a pseudo EPOS transaction line in the form if PayLink transaction exists
// $TRAN-BIO,123,1,,001000195,250,,,19/07/2010,11:22

//****************************************************************************
// // send Offline file - called on startup & whilst waiting for user input
//						to send any outstanding transactions to server

//****************************************************************************
// move a partial transaction file to the offline file ready for transmission next time 
// Partial file only created if system crashed before the coinage input was finialised.
// All local files, so no need to use separate thread.

void CPostTransactionThread::DoWorkInternal(CPostTransactionThreadInfo * pInfo)
{
	switch (pInfo -> m_nAction)	
	{
	case nTRANPOST_ACTION_SENDNORMAL:
		pInfo->m_nIntResult = nPOST_ERROR;
		SendNormal(pInfo);
		break;

	case nTRANPOST_ACTION_SENDOFFLINE:
		
		pInfo->m_bBoolResult = ::AppendFile( pInfo -> m_strOfflineFilename,  pInfo -> m_strLoyLogTransFilename);

		if (TRUE == pInfo -> m_bBoolResult)
		{
			::SendToRecycleBin(pInfo -> m_strOfflineFilename);
		}
		
		break;

	default:
		break;
	}
}

/**********************************************************************/

void CPostTransactionThread::SendNormal(CPostTransactionThreadInfo* pInfo)
{
	CSSFile file;
	if (file.Open(pInfo->m_strPayLinkTTLFilename, "rb") == TRUE)		// PayLink Total Transaction file
	{
		CString strTransLine = CPostTransaction::Create$TranLine(&file);	// create pseudo $Tran line to send to Server
		file.Close();

		if (strTransLine != "")
		{
			// see if have any outstanding offline LoyLog file waiting

			bool bOfflineFileExists = ::FileExists(pInfo->m_strOfflineFilename);

			// try to save PayLink data across network

			CString strFileAccess = "wb";
			if (::FileExists(pInfo->m_strLoyLogTransFilename) == TRUE)
			{
				strFileAccess = "ab";
			}

			CSSFile file;
			if (file.Open(pInfo->m_strLoyLogTransFilename, strFileAccess) == TRUE)
			{
				if (bOfflineFileExists == TRUE)						// see if have outstanding offline file to deal with
				{
					CSSFile fileSource;
					if (fileSource.Open(pInfo->m_strOfflineFilename, "rb") == TRUE)
					{
						CString strLine = "";
						while (fileSource.ReadLine(strLine) == TRUE)
						{
							file.WriteString(strLine);				// line already has cr/lf
						}

						fileSource.Close();
						::SendToRecycleBin(pInfo->m_strOfflineFilename);
						bOfflineFileExists = FALSE;
					}
				}

				if (bOfflineFileExists == FALSE)					// check in case still have offline file
				{
					file.WriteLine(strTransLine);				// write current transaction
					file.Close();

					::SendToRecycleBin(pInfo->m_strPayLinkTTLFilename);			// delete current paylink transaction file
					pInfo->m_nIntResult = nPOST_SENT;
					return;
				}

				file.Close();
			}

			// to get here have no network access - so save in offline LoyLog file for next time

			strFileAccess = (bOfflineFileExists == TRUE) ? "ab" : "wb";	// save in internal offline file

			if (file.Open(pInfo->m_strOfflineFilename, strFileAccess) == TRUE)
			{
				file.WriteLine(strTransLine);							// add line to offline file
				file.Close();

				::SendToRecycleBin(pInfo->m_strPayLinkTTLFilename);		// delete current paylink transaction file
				pInfo->m_nIntResult = nPOST_OFFLINE;
				return;
			}
		}
	}

	pInfo->m_nIntResult = nPOST_ERROR;
}

/**********************************************************************/
