/**********************************************************************/
#include "..\SmartPay4Shared\GlobalFunctions.h"
/**********************************************************************/
#include "BusyFileUpdateThread.h"
/**********************************************************************/

CBusyFileUpdateThreadInfo::CBusyFileUpdateThreadInfo()
{
	m_strBusyFilename = "";
	m_bWantQuit = FALSE;
}

/**********************************************************************/

UINT CBusyFileUpdateThread::DoWork(LPVOID pParam)
{
	CBusyFileUpdateThreadInfo* pInfo = (CBusyFileUpdateThreadInfo*)pParam;
	DoWorkInternal(pInfo);
	return 0;
}

/**********************************************************************/

void CBusyFileUpdateThread::DoWorkInternal(CBusyFileUpdateThreadInfo* pInfo)
{
	LONGLONG nLastUpdateTime = GetTickCount64();

	while (pInfo->m_bWantQuit == FALSE)
	{
		LONGLONG nCurrentTime = GetTickCount64();
		if (((nCurrentTime - nLastUpdateTime) >= 5000) && (pInfo->m_bCanBeBusy == TRUE))
		{
			nLastUpdateTime = nCurrentTime;
			
			CSSFile fileInfo;
			if (fileInfo.Open(pInfo->m_strBusyFilename, "wb") == TRUE)
			{
				CString strInfo = "";
				strInfo.Format("PCId = %10.10d",
					Sysset.GetPCIdHost());

				CCSV csv;
				csv.Add(strInfo);
				csv.Add(GetSimpleDateTimeNowString());
				fileInfo.WriteLine(csv.GetLine());
			}
		}
		Sleep(200);
	}
}

/**********************************************************************/