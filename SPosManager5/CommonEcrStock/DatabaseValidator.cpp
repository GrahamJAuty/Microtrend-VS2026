/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "DatabaseValidationDlg.h"
#include "FileRemover.h"
 /**********************************************************************/
#include "DatabaseValidator.h"
/**********************************************************************/

CDatabaseValidator::CDatabaseValidator()
{
	m_bValidAtStartup = TRUE;
	m_strError = "";
	m_bBusy = FALSE;
	m_bError = FALSE;
	m_bDoingUpdate = FALSE;
	m_bCancelAtStartup = FALSE;
}

/**********************************************************************/

bool CDatabaseValidator::Run( bool bFullValidation, CWnd* pParentWnd )
{
	m_bValidAtStartup = TRUE;

	CDatabaseValidationDlg dlg( bFullValidation, pParentWnd );
	dlg.DoModal();
	
	m_bCancelAtStartup = dlg.m_bCancelAtStartup;
	m_bValidAtStartup = dlg.m_bValidAtStartup;
	m_strError = dlg.m_strError;
	
	return m_bValidAtStartup;
}

/**********************************************************************/

void CDatabaseValidator::ShowErrorMessage()
{
	if ( m_strError != "" )
	{
		#ifdef STOCKMAN_SYSTEM
			CString strError = "Stock Manager";
		#else
			CString strError = "Ecr Manager";
		#endif

			strError += " is unable to continue because the product database is invalid.\n\n";
			strError += "Please use the File/Backup option to create a system backup file and\n";
			strError += "send this file to your dealer so that the database can be repaired.\n\n";
			strError += "Please quote the following error code to your dealer:\n\n";
			strError += m_strError;

			Prompter.FatalError( strError );
	}
}

/**********************************************************************/

void CDatabaseValidator::WriteQuitToken()
{
	CFilenameUpdater FnUp(SysFiles::DataValidationQuit);
	CString strFilename = FnUp.GetFilenameToUse();

	if (::FileExists(strFilename) == TRUE)
	{
		return;
	}

	CSSFile fileQuitToken;
	if (fileQuitToken.Open(strFilename, "wb") == TRUE)
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		CString strMsg;
		strMsg.Format("%2.2d/%2.2d/%4.4d, %2.2d:%2.2d:%2.2d",
			timeNow.GetDay(),
			timeNow.GetMonth(),
			timeNow.GetYear(),
			timeNow.GetHour(),
			timeNow.GetMinute(),
			timeNow.GetSecond());

		fileQuitToken.WriteLine(strMsg);
	}
}

/**********************************************************************/

void CDatabaseValidator::DeleteQuitToken()
{
	CFileRemover FileRemover;
	CFilenameUpdater FnUp ( SysFiles::DataValidationQuit );
	FileRemover.RemoveFile( FnUp.GetFilenameToUse() );
}

/**********************************************************************/

	