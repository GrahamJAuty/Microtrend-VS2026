/**********************************************************************/
#include "EmailHandler.h"
#include "EmailLogDatabase.h"
/**********************************************************************/
#include "EmailPointsAchievement.h"
/**********************************************************************/

void CEmailPointsAchievement::Init()
{
	m_nExportCount = 0;
	m_strTitle = "Points Achievement";
	m_nPointsThreshold = 0;

	m_data.Read();
	m_strEmailLogDatabase = Filenames.GetEmailLogDatabasePathname(3);		// points achievement log database
}

//*******************************************************************
//account points level >= threshold

bool CEmailPointsAchievement::CheckNewEmailRequired(CSQLRowAccountFull& RowAccount)
{
	bool bEntryFound = FALSE;
	int nLastPoints = 0;

	CEmailLogDatabase db;
	if ( db.Open ( m_strEmailLogDatabase, DB_READONLY ) == DB_ERR_NONE )
	{
		if ((bEntryFound = db.FindRecord(RowAccount.GetUserIDString())) == TRUE)	// see if any email has been sent
		{
			nLastPoints = db.GetPoints();
		}

		db.Close();
	}

	bool bSendEmail = ( RowAccount.GetPoints() >= m_nPointsThreshold ) ? TRUE : FALSE;			// see if points level reached

	if ( bEntryFound == TRUE )													// see if have old entry
	{
		bSendEmail = FALSE;														// email alreaay sent

		if ( RowAccount.GetPoints() < m_nPointsThreshold  )					// points must have been redeemed so remove entry
		{
			if ( db.Open ( m_strEmailLogDatabase, DB_READWRITE ) == DB_ERR_NONE )
			{
				if ( db.FindRecord ( RowAccount.GetUserIDString() ) == TRUE )
					db.DeleteRecord();

				db.Close();
			}
		}
	}
	
	return bSendEmail;
}

//*******************************************************************
// email address field already checked not blank

CString CEmailPointsAchievement::SendEmail(CSQLRowAccountFull& RowAccount)
{
	CString strError = "";

	CString strLine = "";
	strLine.Format("Congratulations %s !!!\n\nYou have achieved the target of %d points which can now be redeemed.", RowAccount.GetUsername(), m_nPointsThreshold);

	CString strReportFile = Filenames.GetTempReportPath(RowAccount.GetUserIDString());
	CString strEmailFile = Filenames.GetTempReportPath(RowAccount.GetUserIDString(), "htm");

	CReportFileWithEndText reportFile;
	if (reportFile.Open(strReportFile) == TRUE)
	{
		reportFile.SetStyle1("Points Achievement");
		reportFile.AddColumn("", TA_LEFT, 3000);

		reportFile.WriteLine(strLine);

		reportFile.WriteLine("");
		reportFile.WriteLine(m_data.m_strSignatureText1);					// "End of message."
		reportFile.Close();

		CReportFileDoc report(strReportFile);
		report.SaveAsHTML(strEmailFile);

		CEmailHandler email;
		if (email.SendEmail(m_data.GetEmailAddress(RowAccount), m_data.m_strPointsAchievementEmailText, strEmailFile) == FALSE)
		{
			strError = email.m_strError;
		}
		else
		{
			LogEmailSent(RowAccount);
			++m_nExportCount;
		}

		CFileRecycler::SendToRecycleBin(strEmailFile);
		CFileRecycler::SendToRecycleBin(strReportFile);
	}
	else
	{
		strError.Format("Unable to create report file ' %s ' ", strReportFile);
	}

	return strError;
}

//*******************************************************************

void CEmailPointsAchievement::LogEmailSent(CSQLRowAccountFull& RowAccount)
{
	bool bSendEmail = TRUE;

	CEmailLogDatabase db;
	if (db.Open(m_strEmailLogDatabase, DB_READWRITE) == DB_ERR_NONE)
	{
		if (db.FindRecord(RowAccount.GetUserIDString()) == FALSE)		// see if any email has been sent before
		{
			db.AddBlankRecord(RowAccount.GetUserIDString());						// no add record
			db.SetCardName(RowAccount.GetUsername());
		}

		db.SetDateLastSent();
		db.SetTimeLastSent();
		db.SetEmailAddress(m_data.GetEmailAddress(RowAccount));
		db.SetPurse1DateLastUsed(RowAccount.GetPurse1LastSpendDate());
		db.SetPurse1TimeLastUsed(RowAccount.GetPurse1LastSpendTime());
		db.SetPurse2DateLastUsed(RowAccount.GetPurse2LastSpendDate());
		db.SetPurse2TimeLastUsed(RowAccount.GetPurse2LastSpendTime());
		db.SetPurse3DateLastUsed(RowAccount.GetPurse3LastSpendDate());
		db.SetPurse3TimeLastUsed(RowAccount.GetPurse3LastSpendTime());
		db.SetPurse1(RowAccount.GetPurse1Balance());
		db.SetPurse2(RowAccount.GetPurse2Balance());
		db.SetPurse3(RowAccount.GetPurse3Balance());
		db.SetPoints(RowAccount.GetPoints());
		db.Close();
	}
}

//*******************************************************************

