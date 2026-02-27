//******************************************************************************
#include "..\SmartPay4Shared\PaymentAuditFile.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SmartPayAuditFile.h"
#include "..\SmartPay4Shared\SmartPayLockManager.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_ClosingBalance\SQLRepositoryClosingBalance.h"
//******************************************************************************
#include "AtcArchiveDlg.h"
//******************************************************************************

void CSmartPayAuditFile::WriteArchive(CWnd* pParent)
{
	CString strReportTitle = "Archive Audit file";

	CAtcArchiveDlg dlg(m_strAuditFile, strReportTitle, pParent);			// archive file specifed in constructor
	while (dlg.DoModal() == IDOK)
	{
		CSSDate dateStop;										// date to archive upto on including
		dateStop.SetDate(dlg.GetDate());
		if (dateStop.IsSet() == FALSE)						// check for valid date
		{
			continue;
		}

		CSSDate dateFirst;
		dateFirst.SetDate(dlg.GetFirstDate());				// first date in list
		if (dateFirst.IsSet() == FALSE)
		{
			continue;
		}

		CString strArchiveFile = Filenames.GetAtcArchivePath();
		if (::ExistSubdirectory(strArchiveFile) == FALSE)	// make folder if not found
		{
			::MakeSubdirectory(strArchiveFile);
		}

		CString strFilename = "";
		strFilename.Format("%4.4d%2.2d%2.2d.atc",
			dateFirst.GetYear(),
			dateFirst.GetMonth(),
			dateFirst.GetDay());

		strArchiveFile = Filenames.GetAtcArchivePath(strFilename);

		CString strDateRange = "";
		strDateRange.Format("%s - %s",
			dateFirst.GetDate(),
			dateStop.GetDate());

		CString strCaption = "";
		strCaption.Format("%s ( %s )",
			(const char*)strReportTitle,
			(const char*)strDateRange);

		if (::FileExists(strArchiveFile) == TRUE)
		{
			MessageBeep(MB_ICONEXCLAMATION);
			if (Prompter.YesNo("A file already exists for this start date\n\nDo you wish to overwrite it?", strCaption) == IDNO)
			{
				continue;
			}
		}

		CString strMsg = "";
		strMsg.Format("Are you sure you wish to archive the %d selected lines?",
			dlg.m_nLinesOut);

		if (Prompter.YesNo(strMsg, strCaption) == IDNO)
		{
			continue;
		}

		// start archiving
		strMsg = "";
		CString strMsgTitle = strReportTitle;
		UINT nMessageType = MB_ICONINFORMATION;

		{
			CSmartPayLockRequest Request;
			Request.m_bWantLockBgnd = TRUE;
			Request.m_bWantLockServerInner = TRUE;

			CSmartPayLockManager LockManager;
			int nLockError = LockManager.AcquireLocks(Request, pParent);

			if (LOCKMANAGER_OK != nLockError)
			{
				break;
			}

			CSSFile fileOut;
			if (fileOut.Open(strArchiveFile, "wb") == FALSE)
			{
				strMsg.Format("Cannot open archive file ' %s ' for write!",
					(const char*)strArchiveFile);

				strMsgTitle = "Cannot continue";
				nMessageType = MB_ICONSTOP;
			}
			else
			{
				CWorkingDlg WorkingDlg("Archiving Audit lines", FALSE);
				WorkingDlg.Create();

				Open();														// open audit file
				CSQLAuditRecord atc;
				CStringArray arrayEOD;										// list of EOD lines removed from Closing Balance database

				int nLineCount = 0;
				bool bReadLine = FALSE;
				bool bWriteReply = FALSE;

				while ((bReadLine = atc.ReadLine(&m_fileIn1)) == TRUE)
				{
					WorkingDlg.SetCaption2PercentDone(m_fileIn1.GetPercentPosition());

					CSSDate dateAudit(atc.GetDateFiled());
					if (dateAudit > dateStop)
					{
						break;
					}

					if (atc.GetSourceType() == AUDIT_EODBALANCE)
					{
						if (atc.GetApplicationNo() == APPNO_BALANCE)
						{
							arrayEOD.Add(atc.GetDateFiled());
						}
					}

					++nLineCount;
					bWriteReply = atc.WriteLineToFile(&fileOut);					// write audit line to archive file
				}

				dlg.RemoveEntries(dateStop);
				fileOut.Close();

				CString strTmpName = Filenames.GetTempPathFilename(::GetUniqueTempFilename("$05"));

				if (bWriteReply == TRUE && bReadLine == TRUE)					// see if only partial archive
				{																// yes - create temp file
					if (fileOut.Open(strTmpName, "wb") == TRUE)
					{
						atc.WriteLineToFile(&fileOut);								// save last line read to archive file

						CString strLine;
						while (m_fileIn1.ReadString(strLine) == TRUE)		// just keep reading whole lines
						{
							WorkingDlg.SetCaption2PercentDone(m_fileIn1.GetPercentPosition());
							bWriteReply = fileOut.WriteLine(strLine);		// write whole line
						}
						fileOut.Close();
					}
					else
					{
						strMsg.Format("Cannot open temporary output file ' %s ' for write!",
							(const char*)strTmpName);

						strMsgTitle = "Cannot continue";
						nMessageType = MB_ICONSTOP;

						CFileRecycler::SendToRecycleBin(strArchiveFile);				// delete file just created
					}
				}

				Close();													// close audit file

				WorkingDlg.DestroyWindow();

				if (bWriteReply == TRUE)									// check all writes ok
				{
					strMsg.Format("%d lines have been removed from the audit file", nLineCount);

					CString strOldFile = Filenames.GetAtcArchivePath(::GetUniqueTempFilename("$03", "ATC"));
					::rename(m_strAuditFile, strOldFile);		// rename rather than delete old audit file

					if (bReadLine == TRUE)
					{
						rename(strTmpName, m_strAuditFile);		// rename temp file if used
					}

					RemoveClosingBalances(&arrayEOD);

					// archive EposPaymentAudit to match archived audit lines

					strFilename.Format("%4.4d%2.2d%2.2d.epa", dateFirst.GetYear(), dateFirst.GetMonth(), dateFirst.GetDay());
					strArchiveFile = Filenames.GetAtcArchivePath(strFilename);

					CEposPaymentAuditFile eposAudit;
					eposAudit.WriteArchive(strArchiveFile, dateStop.GetDate(), pParent);
				}
				else
				{
					strMsg = "No data has been removed from the audit file";
				}
			}
		}

		if (nMessageType != MB_ICONINFORMATION)
		{
			MessageBeep(nMessageType);
		}

		Prompter.DisplayMessage(strMsg, strMsgTitle, nMessageType);
	}
}

//******************************************************************************
/******************************************************************************/
/*** append one audit file to Audit file set when contructed ******************/
/*** called to add 'Added to card info' when editting account *****************/
/******************************************************************************/

bool CSmartPayAuditFile::AppendFile(const char* szFilename)
{
	bool bReply = FALSE;

	CSSFile fileOut;
	if ((bReply = fileOut.Open(m_strAuditFile, "ab")) == TRUE)		// main audit file
	{
		CSSFile fileIn;
		bReply = FALSE;

		if (fileIn.Open(szFilename, "rb") == TRUE)	// new file
		{
			CSQLAuditRecord atc;
			while (atc.ReadLine(&fileIn) == TRUE)			// read line from new file
			{
				bReply = atc.SaveNewLine(&fileOut);		// will increment ccno on saving if NOT temp file
			}
			fileIn.Close();
		}
		fileOut.Close();
	}

	return bReply;
}

//******************************************************************************
// archive audit file as specified in the constructor
// also archive EposPaymentAuidt files

void CSmartPayAuditFile::RemoveClosingBalances(CStringArray* pArray)
{
	for (int n = 0; n < pArray->GetSize(); n++)
	{
		CSQLRowClosingBalance RowClosing;
		RowClosing.SetKeyDate(RowClosing.MakeKeyDate(pArray->GetAt(n)));

		CSQLRepositoryClosingBalance RepoClosing;
		if (RepoClosing.SelectRow(RowClosing, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			RowClosing.SetArchived();
			RepoClosing.UpdateRow(RowClosing, NULL);
		}
	}
}

//******************************************************************************

bool CSmartPayAuditFile::AppendArray(CStringArray& array, bool bNoWebUpdateForPebble)
{
	bool bReply = FALSE;

	if (m_strAuditFile == "")
	{
		m_strAuditFile = Filenames.GetAuditFilename();
	}

	CSSFile fileOut;
	if ((bReply = fileOut.Open(m_strAuditFile, "ab")) == TRUE)		// main audit file
	{
		CSQLAuditRecord atc;
		for (int n = 0; n < array.GetSize(); n++)
		{
			CString str = array.GetAt(n);

			if (str != "")
			{
				atc.ReadFromString(str);
				bReply = atc.SaveLineToFile(&fileOut, FALSE, bNoWebUpdateForPebble);		// line being saved is not new
			}
		}

		fileOut.Close();
	}

	return bReply;
}

//******************************************************************************
