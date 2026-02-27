/**********************************************************************/
#include "GlobalFunctions.h"
#include "GTData.h"
/**********************************************************************/
#include "StatusCheckThread.h"
/**********************************************************************/

CStatusCheckThreadInfo::CStatusCheckThreadInfo()
{
}

/**********************************************************************/

UINT CStatusCheckThread::DoWork(LPVOID pParam)
{
	CStatusCheckThreadInfo* pInfo = (CStatusCheckThreadInfo*)pParam;
	DoWorkInternal(pInfo);
	return 0;
}

/**********************************************************************/

void CStatusCheckThread::DoWorkInternal(CStatusCheckThreadInfo* pInfo)
{
	switch (pInfo->m_nAction)
	{
	case nWAITSTATUS_ACTION_CHECKSERVERONLINE:
		CheckServerOnline(pInfo);
		break;

	case nWAITSTATUS_ACTION_SETSTATUSOFF:
		pInfo->m_bBoolResult = Send(pInfo, 0);
		break;

	case nWAITSTATUS_ACTION_SETSTATUSON:
		pInfo->m_bBoolResult = Send(pInfo, 1);
		break;

	case nWAITSTATUS_ACTION_SETSTATUSOUTSERVICE:
		Send(pInfo, 3);
		break;

	case nWAITSTATUS_ACTION_SETSTATUSINSERVICE:
		Send(pInfo, 4);
		break;

	case nWAITSTATUS_ACTION_EMPTYCASH:
		pInfo->m_bBoolResult = Send(pInfo, 5);
		break;

	case nWAITSTATUS_ACTION_SETSTATUSON_CHECKSERVERONLINE:
		pInfo->m_bBoolResult = Send(pInfo, 1);
		CheckServerOnline(pInfo);
		break;

	case nWAITSTATUS_ACTION_SETSTATUSINSERVICE_CHECKSERVERONLINE:
		Send(pInfo, 4);
		CheckServerOnline(pInfo);
		break;

	default:
		break;
	}
}

/**********************************************************************/

void CStatusCheckThread::CheckServerOnline(CStatusCheckThreadInfo* pInfo)
{
	pInfo->m_nIntResult = nSTATUS_OFFLINE;
	if (Send(pInfo, 2) == TRUE)
	{
		pInfo->m_nIntResult = (0 == pInfo->m_nStatusErrorNo) ? nSTATUS_ONLINE : nSTATUS_DISABLED;
	}
}

/**********************************************************************/

bool CStatusCheckThread::Send(CStatusCheckThreadInfo* pInfo, int nStatus)
{
	bool bReply = FALSE;

	CGTData gt;
	CCSV csvData;

	csvData.Add("CMD=STATUS");
	csvData.Add(pInfo -> m_nTerminalNo);
	csvData.Add(nStatus);
	csvData.Add(gt.GetCCNo());					// current ccno
	csvData.Add(gt.GetGTValue(), 2);			// GT

	CSSFile file;
	if (file.Open( pInfo -> m_strLoyReqTempFilename, "wb") == TRUE)
	{
		file.WriteLine(csvData.GetLine());
		file.Close();

		int x = ::rename(pInfo->m_strLoyReqTempFilename, pInfo->m_strLoyReqFilename);	// enable Server to see req file

		for (int nCount = 0; nCount < 8; nCount++)
		{
			Sleep(500);														// wait a bit

			if (::FileExists(pInfo->m_strLoyReqReplyFilename) == TRUE)	// see if request been been processed
			{
				CSSFile file;
				file.Open(pInfo->m_strLoyReqReplyFilename, "rb");

				CCSV csv;
				file.Read(csv);
				pInfo -> m_strStatusReply = csv.GetString(0);			// OK \ DISABLED
				pInfo -> m_nStatusErrorNo = csv.GetInt(1);			//  0  \  1

				bReply = TRUE;									// Server is on but may be disabled

				file.Close();
				::SendToRecycleBin(pInfo->m_strLoyReqReplyFilename);
				break;
			}

			if (pInfo -> m_strImportFile != "")							// see if biometric import file has turned up
			{														// acts like a Cancel
				if (::FileExists(pInfo -> m_strImportFile) == TRUE)
				{
					break;											// allow exit
				}
			}
		}
	}

	::SendToRecycleBin(pInfo->m_strLoyReqFilename);
	return bReply;
}

//*******************************************************************



