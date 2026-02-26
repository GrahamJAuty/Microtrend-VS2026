/**********************************************************************/
#include "..\SPosLoyalty4Shared\FileRemover.h"
/**********************************************************************/
#include "LoyReqTidyUpThread.h"
/**********************************************************************/

CLoyReqTidyUpThreadInfo::CLoyReqTidyUpThreadInfo()
{
	m_strREQFolder = "";
	m_pTidyUpFileBuffer = NULL;
}

/**********************************************************************/

UINT CLoyReqTidyUpThread::DoWork(LPVOID pParam)
{
	int nForceQuit = 0;
	CLoyReqTidyUpThreadInfo* pInfo = (CLoyReqTidyUpThreadInfo*)pParam;
	DoWorkInternal(pInfo);
	return 0;
}

/**********************************************************************/

void CLoyReqTidyUpThread::DoWorkInternal(CLoyReqTidyUpThreadInfo* pInfo)
{
	const CString strSourceFileMask = pInfo->m_strREQFolder + "\\*.*";
	
	CFileFind FileFinder;
	BOOL bWorking = FileFinder.FindFile(strSourceFileMask);
	const LONGLONG currentTime = GetTickCount64();

	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();

		if (FileFinder.IsDots() || FileFinder.IsDirectory())
		{
			continue;
		}

		CString strFilename = FileFinder.GetFileName();
		const CString strCheck = strFilename.Right(4).MakeUpper();

		if ((strCheck == ".RSP") || (strCheck == ".$$$") || (strCheck == ".LOG"))
		{
			CFileStatus FileStatus;
			if (CFile::GetStatus(FileFinder.GetFilePath(), FileStatus) == FALSE)
			{
				continue;
			}

			CTidyUpFileInfo infoFile;
			infoFile.m_strFilename = strFilename.MakeUpper();

			int nPos = 0;
			if (!pInfo->m_pTidyUpFileBuffer->Find(infoFile, nPos))
			{
				infoFile.m_strModTime = FileStatus.m_mtime.Format("%Y%m%d%H%M%S");
				infoFile.m_FirstSeenTime = currentTime;
				pInfo->m_pTidyUpFileBuffer->InsertAt(nPos, infoFile);
			}
			else
			{
				const CString strNewModTime = FileStatus.m_mtime.Format("%Y%m%d%H%M%S");
				pInfo->m_pTidyUpFileBuffer->GetAt(nPos, infoFile);

				if (infoFile.m_strModTime != strNewModTime)
				{
					infoFile.m_strModTime = strNewModTime;
					infoFile.m_FirstSeenTime = currentTime;
					pInfo->m_pTidyUpFileBuffer->SetAt(nPos, infoFile);
				}
				else
				{
					const LONGLONG nTimeDiff = currentTime - infoFile.m_FirstSeenTime;

					if (nTimeDiff > (300 * 1000))
					{
						const CString strFile = pInfo->m_strREQFolder + "\\" + strFilename;
						CFileRemover remover;
						remover.RemoveFile(strFile);
						pInfo->m_pTidyUpFileBuffer->RemoveAt(nPos);
					}
				}
			}
		}
	}

	// Remove filenames that have been in the buffer for more than 10 minutes in case system has deleted them
	const LONGLONG nCutOffTime = currentTime - (600 * 1000);
	for (int n = pInfo->m_pTidyUpFileBuffer->GetSize() - 1; n >= 0; n--)
	{
		CTidyUpFileInfo infoFile;
		pInfo->m_pTidyUpFileBuffer->GetAt(n, infoFile);
		if (infoFile.m_FirstSeenTime < nCutOffTime)
		{
			pInfo->m_pTidyUpFileBuffer->RemoveAt(n);
		}
	}
}

/**********************************************************************/