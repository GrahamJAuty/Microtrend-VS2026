/******************************************************************************/
#include "WorkingDlg.h"
#include "AtcArchiveDlg.h"
/******************************************************************************/
#include "..\SPosLoyalty4Shared\LoyaltyAuditFile.h"
/******************************************************************************/

//******************************************************************************
// archive audit file as specified in the constructor
//******************************************************************************

void CLoyaltyAuditFile::WriteArchive ( CWnd* pParent )
{
	CString strExtension = "atc";
	CString strReportTitle = "Archive Audit file";

	CAtcArchiveDlg dlg ( m_strAuditFile, strReportTitle, pParent );			// archive file specifed in constructor
	while ( dlg.DoModal() == IDOK )
	{
		CSSDate dateStop;										// date to archive upto on including
		dateStop.SetDate ( dlg.GetDate() );
		if (dateStop.IsSet() == FALSE)						// check for valid date
		{
			continue;
		}

		CSSDate dateFirst;
		dateFirst.SetDate ( dlg.GetFirstDate() );				// first date in list
		if (dateFirst.IsSet() == FALSE)
		{
			continue;
		}

		CString strArchiveFile = Filenames.GetAtcArchivePath();
		if (::ExistSubdirectory(strArchiveFile) == FALSE)	// make folder if not found
		{
			::MakeSubdirectory(strArchiveFile);
		}

		CString strName = "";
		strName.Format ( "%4.4d%2.2d%2.2d.%s", 
			dateFirst.GetYear(), 
			dateFirst.GetMonth(),
			dateFirst.GetDay(),
			(const char*) strExtension );

		strArchiveFile = Filenames.GetAtcArchivePath ( strName );

		CString strDateRange = "";
		strDateRange.Format ( "%s - %s", dateFirst.GetDate(), dateStop.GetDate() );
		
		CString strCaption = "";
		strCaption.Format ( "%s ( %s )", 
			(const char*) strReportTitle,
			(const char*) strDateRange );

		if ( SolutionGlobalFunctions::FileExists ( strArchiveFile ) == TRUE )
		{
			MessageBeep ( MB_ICONEXCLAMATION );
			if (Prompter.YesNo("A file already exists for this start date\n\nDo you wish to overwrite it?", strCaption) == IDNO)
			{
				continue;
			}
		}

		CString strMsg = "";
		strMsg.Format ( "Are you sure you wish to archive the %d selected lines?", dlg.m_nLinesOut );
		if (Prompter.YesNo(strMsg, strCaption) == IDNO)
		{
			continue;
		}

// start archiving

		//FIDDLE : IMPLMENT PAUSE
		/*
		CSystemPause sys ( pParent);
		if ( sys.PauseSystem() == FALSE )							// pause server \ background
			break;													// if false will have displayed an error message
		*/

		CWorkingDlg WorkingDlg ( "Archiving lines", FALSE );
		WorkingDlg.Create();

		int nLineCount		= 0;
		bool bReadLine		= FALSE;
		bool bWriteReply	= FALSE;

		OpenReadOnly();											// open audit file

		CSSFile fileOut;
		CAuditRecord_base atc;

		if ( fileOut.Open ( strArchiveFile, "wb" ) == TRUE )
		{
			while ( ( bReadLine = atc.ReadLine ( &m_fileIn1 ) ) == TRUE )
			{
				WorkingDlg.SetCaption2PercentDone ( m_fileIn1.GetPercentPosition(), FALSE );

				CSSDate dateAudit ( atc.GetDateFiled() );
				if (dateAudit > dateStop)
				{
					break;
				}

				++nLineCount;
				bWriteReply = atc.WriteLineToFile( &fileOut );				// write audit line to archive file
			}

			dlg.RemoveEntries ( dateStop );
			fileOut.Close();
		}
		else
		{
			MessageBeep ( MB_ICONSTOP );
			Prompter.FatalError( "Error with create output archive file", "Cannot continue" );
		}

		CString strTmpName = Filenames.GetTempPathFilename ( ::GetUniqueTempFilename("$02") );

		if ( bWriteReply == TRUE && bReadLine == TRUE )					// see if only partial archive
		{																// yes - create temp file
			if ( fileOut.Open ( strTmpName, "wb" )  == TRUE )
			{
				atc.WriteLineToFile ( &fileOut );								// save last line read to archive file

				CString strLine = "";
				while ( m_fileIn1.ReadString ( strLine ) == TRUE )		// just keep reading whole lines
				{
					WorkingDlg.SetCaption2PercentDone ( m_fileIn1.GetPercentPosition(), FALSE );
					bWriteReply = fileOut.WriteLine ( strLine );		// write whole line
				}

				fileOut.Close();
			}
			else
			{
				MessageBeep ( MB_ICONSTOP );
				Prompter.FatalError( "Unable to create a temporary audit output file", "Cannot continue" );
				::SendToRecycleBin ( strArchiveFile );				// delete file just created
			}
		}

		Close();											// close audit file

		WorkingDlg.DestroyWindow();

		if ( bWriteReply == TRUE )							// check all writes ok
		{
			CString strOldFile = Filenames.GetAtcArchivePath ( ::GetUniqueTempFilename("$03", "ATC") );
			::rename ( m_strAuditFile, strOldFile );		// rename rather than delete old audit file

			if (bReadLine == TRUE)
			{
				rename(strTmpName, m_strAuditFile);		// rename temp file if used
			}

			//FIDDLE : IMPLEMENT
			//sys.ResumeSystem();								// restart Server

			CString strMess;
			strMess.Format ( "%d lines have been removed from the audit file", nLineCount );
			Prompter.Info( strMess, strReportTitle );
		}
		else
		{
			//FIDDLE : IMPLEMENT
			//sys.ResumeSystem();								// restart Server

			MessageBeep ( MB_ICONEXCLAMATION );
			Prompter.Error( "No data has been removed from the auditfile!", "Cannot continue" );
		}
	}
}

/******************************************************************************/
