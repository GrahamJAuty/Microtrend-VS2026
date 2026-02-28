/**********************************************************************/
#include "..\SPosLoyalty4Shared\FileRemover.h"
/**********************************************************************/
#include "LoyRspThread.h"
/**********************************************************************/

CLoyRspThreadInfo::CLoyRspThreadInfo()
{
	m_pResponseBuffer = NULL;
}

/**********************************************************************/

UINT CLoyRspThread::DoWork(LPVOID pParam)
{
	int nForceQuit = 0;
	CLoyRspThreadInfo* pInfo = (CLoyRspThreadInfo*)pParam;
	DoWorkInternal(pInfo);
	return 0;
}

/**********************************************************************/

void CLoyRspThread::DoWorkInternal(CLoyRspThreadInfo* pInfo)
{
	if (pInfo->m_pResponseBuffer->GetSize() > 3)
	{
		CString strTempPath = pInfo->m_pResponseBuffer->GetAt(0);
		CString strFilePath = pInfo->m_pResponseBuffer->GetAt(1);
		CString strExternal = pInfo->m_pResponseBuffer->GetAt(2);

		if (strTempPath != strFilePath)
		{
			LogThread(pInfo, "About to create RSP file");

			CSSFile fileTemp;
			if (fileTemp.Open(strTempPath, "wb") == TRUE)
			{
				LogThread(pInfo, "Started to write RSP file");

				for (int n = 3; n < pInfo->m_pResponseBuffer->GetSize(); n++)
				{
					if (strExternal != "1")
					{
						fileTemp.WriteLine(pInfo->m_pResponseBuffer->GetAt(n));
					}
					else
					{
						fileTemp.WriteString(pInfo->m_pResponseBuffer->GetAt(n));
					}
				}

				fileTemp.Close();

				LogThread(pInfo, "Finished writing RSP file");

				CFileRemover FileRemover;
				FileRemover.RemoveFile(strFilePath);
				rename(strTempPath, strFilePath);

				LogThread(pInfo, "Renamed RSP file");
			}
		}
		else
		{
			CSSFile fileAppend;
			if (fileAppend.Open(strFilePath, "ab") == TRUE)
			{
				for (int n = 3; n < pInfo->m_pResponseBuffer->GetSize(); n++)
				{
					if (strExternal != "1")
					{
						fileAppend.WriteLine(pInfo->m_pResponseBuffer->GetAt(n));
					}
					else
					{
						fileAppend.WriteString(pInfo->m_pResponseBuffer->GetAt(n));
					}
				}

				fileAppend.Close();
			}
		}
	}
}

/**********************************************************************/

void CLoyRspThread::LogThread(CLoyRspThreadInfo* pInfo, CString strMsg)
{
	if (FALSE == pInfo->m_bLogFileWrites)
	{
		return;
	}

	CString strDate = "";
	CString strTime = "";
	SolutionGlobalFunctions::GetMessageLogDateTime(strDate, strTime);

	CSSFile fileLog;
	CString strFilename = "";
	strFilename.Format(".\\RSPLOG_%d%d.txt",
		pInfo->m_nDbNo,
		pInfo->m_nSetNo);

	if (fileLog.Open(strFilename, "ab") == TRUE)
	{
		CString strLog = "";
		strLog.Format("%s %s - %s",
			(const char*)strDate,
			(const char*)strTime,
			(const char*)strMsg);

		fileLog.WriteLine(strLog);
		fileLog.Close();
	}
}

/**********************************************************************/