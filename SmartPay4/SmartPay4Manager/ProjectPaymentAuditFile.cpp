//******************************************************************************
#include "..\SmartPay4Shared\PaymentAuditFile.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//******************************************************************************
// archive lines from the Epos Payment audit file

int CEposPaymentAuditFile::WriteArchive ( const char* szArchiveFile, const char* szDateStop, CWnd* pParent )
{
	int nLineCount = 0;
	CString strTitle = "Archive Epos Payments";

	CSSDate dateStop;
	dateStop.SetDate ( szDateStop );

	CSSFile fileOut;
	if (fileOut.Open(szArchiveFile, "wb") == TRUE)
	{
		CWorkingDlg dlgWorking("Archiving Epos Payment lines", FALSE);
		dlgWorking.Create();

		bool bReadLine = FALSE;
		bool bWriteReply = FALSE;

		Open();															// open audit file

		CPaymentAuditRecord atc;
		while ((bReadLine = ReadLine(&atc)) == TRUE)
		{
			dlgWorking.SetCaption2PercentDone(m_fileIn1.GetPercentPosition(),FALSE);

			CSSDate dateAudit(atc.GetDateFiled());
			if (dateAudit > dateStop)
			{
				break;
			}

			if ((bWriteReply = atc.WriteLine(&fileOut)) == TRUE)	// write audit line to archive file
			{
				++nLineCount;
			}
		}
		fileOut.Close();

		CString strTmpName = Filenames.GetTempPathFilename(::GetUniqueTempFilename("$04"));

		if (bWriteReply == TRUE && bReadLine == TRUE)					// see if only partial archive
		{																// yes - create temp file
			if (fileOut.Open(strTmpName, "wb") == TRUE)
			{
				atc.WriteLine(&fileOut);								// save last line read to archive file
				CString strLine;

				while (m_fileIn1.ReadString(strLine) == TRUE)		// just keep reading whole lines
				{
					dlgWorking.SetCaption2PercentDone(m_fileIn1.GetPercentPosition(),FALSE);
					bWriteReply = fileOut.WriteLine(strLine);		// write whole line
				}
				fileOut.Close();
			}
			else
			{
				MessageBeep(MB_ICONSTOP);
				Prompter.FatalError("Cannot open temporary payment audit output file for write!", strTitle);
				CFileRecycler::SendToRecycleBin(szArchiveFile);				// delete file just created
			}
		}
		Close();													// close audit file

		dlgWorking.DestroyWindow();

		if (TRUE == bWriteReply)									// check all writes ok
		{
			CString strOldFile = Filenames.GetAtcArchivePath(::GetUniqueTempFilename("$03", "EPA"));
			::rename(m_strAuditFile, strOldFile);				// rename rather than delete old audit file
			if (bReadLine == TRUE)
			{
				rename(strTmpName, m_strAuditFile);				// rename temp file if used
			}
		}

		if (0 == nLineCount)										// see if any line archived
		{
			CFileRecycler::SendToRecycleBin(szArchiveFile);					// delete empty file
		}
	}
	else
	{
		MessageBeep ( MB_ICONSTOP );
		Prompter.FatalError( "Cannot open payment audit output file for write!", strTitle );
	}

	return nLineCount;
}

//******************************************************************************
