/**********************************************************************/
#include "..\SPosLoyalty4Shared\FileRemover.h"
/**********************************************************************/
#include "LoyReqThread.h"
/**********************************************************************/

CLoyReqThreadInfo::CLoyReqThreadInfo()
{
}

/**********************************************************************/

UINT CLoyReqThread::DoWork(LPVOID pParam)
{
	int nForceQuit = 0;
	CLoyReqThreadInfo* pInfo = (CLoyReqThreadInfo*)pParam;
	DoWorkInternal(pInfo);
	return 0;
}

/**********************************************************************/

void CLoyReqThread::DoWorkInternal(CLoyReqThreadInfo* pInfo)
{
	pInfo->m_StartTime = GetTickCount64();

	CString strSourceFileMask = pInfo->m_strREQFolder + "\\*.REQ";
	
	CFileFind FileFinder;
	BOOL bWorking = FileFinder.FindFile(strSourceFileMask);

	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();
	
		if (FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE)
		{
			continue;
		}

		CString strFilename = FileFinder.GetFileName();
		strFilename.MakeUpper();

		bool bAcceptFilename = FALSE;
		if (strFilename.Right(4) == ".REQ")
		{
			if (strFilename.Left(5) == "RECON")
			{
				pInfo -> m_bIsReconRequest = TRUE;
				bAcceptFilename = TRUE;
			}
			else if (strFilename.Left(7) == "LOYJSON")
			{
				pInfo->m_bIsJSONRequest = TRUE;
				bAcceptFilename = TRUE;
			}
			else
			{
				pInfo->m_bIsReconRequest = FALSE;

				if (Server.GetRealTimePlusFlag() == TRUE)
				{
					bAcceptFilename = (strFilename.Left(3) == "LID");
				}
				else
				{
					bAcceptFilename = (strFilename.Left(3) == "LOY");
				}
			}
		}

		if (TRUE == bAcceptFilename)
		{
			CString strFilepath = FileFinder.GetFilePath();

			{
				LogThread(pInfo, "About to open REQ file");

				CSSFile fileReq;
				if (fileReq.Open(strFilepath, "rb") == FALSE)
				{
					pInfo->m_EndTime = GetTickCount64();
					return;
				}

				LogThread(pInfo, "Opened REQ file");

				pInfo->m_strREQFilepath = strFilepath;
				pInfo->m_strREQFilename = strFilename;
				pInfo->m_strRSPFilepath = strFilepath.Left(strFilepath.GetLength() - 3) + "RSP";
				pInfo->m_strRSPFilepathTemp = strFilepath.Left(strFilepath.GetLength() - 3) + "$$$";

				CString strBuffer = "";

				while (fileReq.ReadString(strBuffer) == TRUE)
				{
					pInfo->m_pRequestBuffer->Add(strBuffer);
				}
			}

			LogThread(pInfo, "Closed REQ file ");

			CFileRemover FileRemover;
			FileRemover.RemoveFile(strFilepath);

			LogThread(pInfo, "Deleted REQ file ");

			break;
		}
	}

	pInfo->m_EndTime = GetTickCount64();
}

/**********************************************************************/

void CLoyReqThread::LogThread(CLoyReqThreadInfo* pInfo, CString strMsg)
{
	if (FALSE == pInfo->m_bLogFileReads)
	{
		return;
	}

	CString strDate = "";
	CString strTime = "";
	GetMessageLogDateTime(strDate, strTime);

	CSSFile fileLog;
	CString strFilename = "";
	strFilename.Format(".\\REQLOG_%d%d.txt", 
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