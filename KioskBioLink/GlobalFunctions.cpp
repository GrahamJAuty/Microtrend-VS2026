/**********************************************************************/
#include <io.h>
#include <mmsystem.h>
/**********************************************************************/
#include "SystemData.h"
#include "PasswordData.h"
#include "ErrorDlg.h"
#include "KeypadDlg.h"
/**********************************************************************/
#include "GlobalFunctions.h"
/**********************************************************************/

bool FileExists(const char* szFile)
{
	return (_access(szFile, 0) == 0);
}

/******************************************************************************/

void SendToRecycleBin(CStringArray* pArray)
{
	for (int i = 0; i < pArray->GetSize(); i++)
	{
		remove(pArray->GetAt(i));
	}
}

/******************************************************************************/

void SendToRecycleBin(const char* szFileMask)
{
	CFileFind FileFinder;
	BOOL bWorking = FileFinder.FindFile(szFileMask);

	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();
		if (FileFinder.IsDots() == TRUE)
		{
			continue;
		}

		if (FileFinder.IsDirectory() == TRUE)
		{
			continue;
		}

		remove(FileFinder.GetFilePath());
	}
}

//*******************************************************************

void PlayMessage(int nMessageNo, bool bWaitFinshed)
{
	if (nMESSAGE_NONE == nMessageNo)
	{
		return;
	}

	CString strSoundFile = "";

	switch (nMessageNo)
	{
	case nMESSAGE_WAITING:			strSoundFile = "sound1.wav";	break;
	case nMESSAGE_FINISHED:			strSoundFile = "sound2.wav";	break;
	case nMESSAGE_TOPUP_OK:			strSoundFile = "sound3.wav";	break;
	case nMESSAGE_TOPUP_FAILED:		strSoundFile = "sound4.wav";	break;
	case nMESSAGE_WAITINGFORSERVER:	strSoundFile = "sound5.wav";	break;
	case nMESSAGE_NOSERVERRESPONSE:	strSoundFile = "sound6.wav";	break;
	case nMESSAGE_SERVERERROR:		strSoundFile = "sound7.wav";	break;
	case nMESSAGE_EXPIRED:			strSoundFile = "sound8.wav";	break;
	case nMESSAGE_FILECREATEERROR:	strSoundFile = "sound9.wav";	break;
	case nMESSAGE_FILEREADERROR:	strSoundFile = "sound10.wav";	break;
	case nMESSAGE_FILEWRITEERROR:	strSoundFile = "sound11.wav";	break;
	}

	if (strSoundFile != "")
	{
		CString strFilename = System.AddSysFilePath(strSoundFile);
		if (::FileExists(strFilename) == TRUE)
		{
			PlaySound(strFilename, NULL, SND_FILENAME | SND_ASYNC);
		}
	}
}

//*******************************************************************

bool AppendFile(const char* szSourceFilename, const char* szDestFilename)
{
	bool bReply = FALSE;

	CSSFile fileDest;
	if ((bReply = fileDest.Open(szDestFilename, "ab")) == TRUE)
	{
		CSSFile fileSource;
		if ((bReply = fileSource.Open(szSourceFilename, "rb")) == TRUE)
		{
			CString strBuf = "";
			bool bReply = TRUE;

			while (fileSource.ReadLine(strBuf) == TRUE)
			{
				bReply = fileDest.WriteString(strBuf);			// line already has cr/lf
			}

			fileSource.Close();
		}
		fileDest.Close();
	}

	return bReply;
}

//*******************************************************************

void DisplayMessage(CWnd* pParent, const char* szMsg, const char* szTitle, UINT uType)
{
	if (uType != 0)
	{
		MessageBeep(uType);
	}

	CErrorDlg dlg(szTitle, szMsg, pParent);
	dlg.DoModal();
}

//*******************************************************************

bool ValidateFolderAccess(const char* szPath)
{
	bool bReply = FALSE;
	CString strFilename;	strFilename.Format("%s\\KioskTest.$$$", szPath);			// create a temp file

	CSSFile file;
	if ((bReply = file.Open(strFilename, "wb")) == TRUE)
	{
		file.Close();
		SendToRecycleBin(strFilename);
	}
	return bReply;
}

//*******************************************************************

bool GetAdminPassword(CWnd* pParent)
{
	bool bOk = FALSE;
	int nFailures = 0;

	do
	{
		if (System.GetSystemPasswordReqdFlag() == TRUE)
		{
			CKeypadDlg dlg(TRUE, pParent);
			dlg.m_strPrompt = "Enter Admin access PIN";

			if (dlg.DoModal() == IDCANCEL)
			{
				break;
			}

			CPasswordData password;
			if ((bOk = password.CheckPassword(dlg.m_strInput)) == FALSE)
			{
				DisplayMessage(pParent, password.GetErrorMessage(), "Access Denied", MB_ICONSTOP);

				if (++nFailures > 2)
				{
					break;
				}
			}
		}
		else
		{
			bOk = TRUE;
		}
	} while (FALSE == bOk);

	return bOk;
}

//*******************************************************************

bool CheckEventDue(LONGLONG& nLastTime, int nInterval)
{
	bool bResult = FALSE;

	LONGLONG nCurrentTime = GetTickCount64();
	if (nCurrentTime >= nLastTime + (nInterval * 1000))
	{
		bResult = TRUE;
		nLastTime = 0;
	}

	return bResult;
}

//**********************************************************************

void ResetEventIfFired(LONGLONG& nLastTime)
{
	if (0 == nLastTime)
	{
		ResetEvent(nLastTime);
	}
}

//**********************************************************************

void ResetEvent(LONGLONG& nLastTime)
{
	nLastTime = GetTickCount64();
}

//**********************************************************************