/**********************************************************************/
#include "..\SPOSLoyalty4Shared\FileRemover.h"
/**********************************************************************/
#include "LoyLogThread.h"
/**********************************************************************/

CLoyLogThreadInfo::CLoyLogThreadInfo()
{
	m_strLOGFilename = "";
	m_strLOGFilepath = "";
	m_strRSPFilepath = "";
	m_strRSPFilepathTemp = "";
	m_pLOGBuffer = NULL;
	m_pEmptyFileBuffer = NULL;
	m_bCopyLogFile = FALSE;
	m_StartTime = 0;
	m_EndTime = 0;
}

/**********************************************************************/

UINT CLoyLogThread::DoWork(LPVOID pParam)
{
	int nForceQuit = 0;
	CLoyLogThreadInfo* pInfo = (CLoyLogThreadInfo*)pParam;
	DoWorkInternal(pInfo);
	return 0;
}

/**********************************************************************/

void CLoyLogThread::DoWorkInternal(CLoyLogThreadInfo* pInfo)
{
	pInfo->m_StartTime = GetTickCount64();

	CFileFind FileFinder;
	BOOL bWorking = FileFinder.FindFile(pInfo->m_strLOGFolder + "\\LOY*.LOG");

	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();
	
		if (FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE)
		{
			continue;
		}

		CString strFilename = FileFinder.GetFileName();
		strFilename.MakeUpper();

		if (strFilename.Right(4) == ".LOG")
		{
			CString strFilepath = FileFinder.GetFilePath();

			{
				CSSFile fileLOG;
				if (fileLOG.Open(strFilepath, "rb") == FALSE)
				{
					pInfo->m_EndTime = GetTickCount64();
					return;
				}

				CString strBuffer = "";

				while (fileLOG.ReadString(strBuffer) == TRUE)
				{
					pInfo->m_pLOGBuffer->Add(strBuffer);
				}
			}

			if (pInfo->m_pLOGBuffer->GetSize() != 0)
			{
				pInfo->m_strLOGFilepath = strFilepath;
				pInfo->m_strLOGFilename = strFilename;
				pInfo->m_strRSPFilepath = strFilepath.Left(strFilepath.GetLength() - 3) + "RSP";
				pInfo->m_strRSPFilepathTemp = strFilepath.Left(strFilepath.GetLength() - 3) + "$$$";

				if (TRUE == pInfo->m_bCopyLogFile)
				{
					CString strCopy = "";
					COleDateTime timeNow = COleDateTime::GetCurrentTime();
					strCopy.Format("%s\\%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d_%s",
						(const char*)Filenames.GetLogCopyFolder(),
						timeNow.GetYear(),
						timeNow.GetMonth(),
						timeNow.GetDay(),
						timeNow.GetHour(),
						timeNow.GetMinute(),
						timeNow.GetSecond(),
						(const char*)strFilename);

					CopyFile(strFilepath, strCopy, FALSE);
				}

				CFileRemover FileRemover;
				FileRemover.RemoveFile(strFilepath);

				break;
			}
			else
			{
				pInfo->m_pEmptyFileBuffer->Add(strFilepath);
			}
		}
	}

	pInfo->m_EndTime = GetTickCount64();
}

/**********************************************************************/