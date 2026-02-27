//*********************************************************************
#include "..\SmartPay4Shared\WaitDlg.h"
//*********************************************************************
#include "FileRecycler.h"
//*********************************************************************

int CFileRecycler::SendToRecycleBin ( const char* szFileMask, bool bReportError )
{
	int nFailureCount = 0;
	bool bConfirmContinue = FALSE;

	CFileFind FileFinder;
	BOOL bWorking = FileFinder.FindFile ( szFileMask );

	while (bWorking)   
	{
		bWorking = FileFinder.FindNextFile();
		if ( FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE )
			continue;

		if ( bConfirmContinue == TRUE )
		{
			if ( Prompter.YesNo( "Do you wish to carry on with the delete process (y/n)?", "", TRUE ) == IDNO )
			{
				++nFailureCount;							// inc counter as the next file was not deleted
				break;										// give up altogther & exit
			}
			bConfirmContinue = FALSE;
		}


		CString strFilename = FileFinder.GetFilePath();
		::remove ( strFilename );

		int nLoopCounter = 0;
		while ( ::FileExists ( strFilename ) == TRUE )		// see if file still here!
		{
			if ( bReportError == FALSE )					// see if should wait
				break;

			CString strMsg;		strMsg.Format ( "Delete file retry in progress : %d ", ++nLoopCounter );

			CWaitDlg dlgWait;
			dlgWait.SetDlgItemText ( IDC_STATIC_MESSAGE, strMsg );

			WaitLoop(20);									// pump messages (milli seconds)
			Sleep(1980);									// free up processor time (milli seconds)

			if ( nLoopCounter < 10 )						// 10 x 2 seconds
				::remove ( strFilename );					// try & remove after waiting 1/2 second
			else
			{
				CString strMsg;
				strMsg.Format ( "SYSTEM HALTED:\n\nCannot delete file : %s\n\nDo you wish to retry the delete (y/n)", strFilename );
				if ( Prompter.YesNo( strMsg, "", TRUE ) == IDNO )
				{
					++nFailureCount;
					bConfirmContinue = TRUE;				// confirm continue  if another file found
					break;									// give up on this file & carry on
				}
				else
					nLoopCounter = 0;						// go round again
			}
		}
	}

	return nFailureCount;
}

//*******************************************************************




/*
int SendToRecycleBin ( const char* szFileMask, bool bReportError )
{
	int nFailureCount = 0;

	CFileFind FileFinder;	
	BOOL bWorking = FileFinder.FindFile ( szFileMask );

	while (bWorking)   
	{
		bWorking = FileFinder.FindNextFile();
		if ( FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE )
			continue;

		CString strFilename = FileFinder.GetFilePath();
		::remove ( strFilename );

		int nLoopCounter = 0;
		while ( ::FileExists ( strFilename ) == TRUE )		// see if file still here!
		{
			if ( bReportError == FALSE )					// see if should wait
				break;

			CString strMsg;		strMsg.Format ( "Delete file retry in progress : %d ", ++nLoopCounter );

			CWaitDlg dlgWait;
			dlgWait.SetDlgItemText ( IDC_STATIC_MESSAGE, strMsg );

			WaitLoop(20);									// pump messages (milli seconds)
			Sleep(1980);									// free up processor time (milli seconds)

			if ( nLoopCounter < 10 )						// 10 x 2 seconds
				::remove ( strFilename );					// try & remove after waiting 1/2 second
			else
			{
				CString strMsg;
				strMsg.Format ( "SYSTEM HALTED:\n\nCannot delete file : %s\n\nDo you wish to retry the delete (y/n)", strFilename );
				if ( Prompter.YesNo ( strMsg, "", TRUE ) == IDNO )
				{
					++nFailureCount;
					break;									// give up on this file & carry on
				}
				else
					nLoopCounter = 0;						// go round again
			}
		}
	}

	return nFailureCount;
}
*/


//*********************************************************************
