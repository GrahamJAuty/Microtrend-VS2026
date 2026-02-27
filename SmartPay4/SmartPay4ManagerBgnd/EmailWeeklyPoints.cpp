/**********************************************************************/
#include "EmailHandler.h"
#include "EmailLogDatabase.h"
/**********************************************************************/
#include "EmailWeeklyPoints.h"
/**********************************************************************/

void CEmailWeeklyPoints::Init()
{
	m_nExportCount  = 0;
	m_strTitle = "Email Weekly Points Update";

	m_strEmailLogDatabase = Filenames.GetEmailLogDatabasePathname(4);		// weekly points update log database
	m_data.Read();

	m_dateToday.SetCurrentDate();

	m_nLastPoints = 0;
	m_dateLastSent.SetDate("");
}

//*******************************************************************
// see if points have changed since last time

bool CEmailWeeklyPoints::CheckNewEmailRequired(CSQLRowAccountFull& RowAccount)
{
	bool bSendEmail = TRUE;

	CEmailLogDatabase db;
	if ( db.Open ( m_strEmailLogDatabase, DB_READONLY ) == DB_ERR_NONE )
	{
		if ( db.FindRecord ( RowAccount.GetUserIDString() ) == TRUE )		// see if any email has been sent
		{
			m_nLastPoints = db.GetPoints();
			if (RowAccount.GetPoints() == m_nLastPoints)
			{
				if (m_data.m_bWeeklyPointsExcludeNoChange == TRUE)
				{
					bSendEmail = FALSE;									// no change since last email
				}
			}

			if ( bSendEmail == TRUE )
			{
				m_dateLastSent.SetDate ( db.GetDateLastSent() );

				if (m_dateLastSent >= m_dateToday)
				{
					bSendEmail = FALSE;									// not required
				}
				else
				{
					m_timeLastSent.SetTime(db.GetTimeLastSent());	// otherwise - get time lat sent
				}
			}	
		}
		db.Close();
	}

	return bSendEmail;
}


//*******************************************************************
// email address field already checked not blank

CString CEmailWeeklyPoints::SendEmail(CSQLRowAccountFull& RowAccount)
{
	CString strError = "";

	CString strReportFile	= Filenames.GetTempReportPath( RowAccount.GetUserIDString() );
	CString strEmailFile	= Filenames.GetTempReportPath( RowAccount.GetUserIDString(), "htm" );

	CReportFileWithEndText reportFile;
	if ( reportFile.Open ( strReportFile ) == TRUE )
	{
		reportFile.SetStyle1 ( "Please find below your points allocation" );
		reportFile.AddColumn ( "", TA_LEFT, 300 );
		reportFile.AddColumn ( "", TA_RIGHT, 120 );
		reportFile.AddColumn ( "", TA_RIGHT, 400 );

		CString strLine;
		CCSV csvLine ( '\t' );

		if ( m_dateLastSent.IsSet() == FALSE )
		{
			csvLine.Add ( "<..>Opening balance" );
			strLine = "";
		}
		else
		{
			csvLine.Add ( "<..>Previous balance" );
			strLine = FormatDateTime ( m_dateLastSent.GetDate(), m_timeLastSent.GetTime() );
		}
		csvLine.Add ( m_nLastPoints );
		csvLine.Add ( strLine );
		reportFile.WriteLine ( csvLine );
		reportFile.WriteLine ( "" );

		csvLine.RemoveAll();
		
		int nPointsAdded = ( RowAccount.GetPoints() - m_nLastPoints );
		csvLine.Add ( "<..>Points adjustment" );
		csvLine.Add ( ::FormatIntValue(nPointsAdded) );
		reportFile.WriteLine ( csvLine );
		reportFile.WriteLine ( "" );

		csvLine.RemoveAll();

		csvLine.Add ( "<..>New balance" );
		csvLine.Add (RowAccount.GetPoints() );
		csvLine.Add ( FormatDateTime ( m_dateToday.GetDate(), CSSTime::GetCurrentTime().GetTime() ) );
		reportFile.WriteLine ( csvLine );
		reportFile.WriteLine ( "" );

		reportFile.WriteLine ( "" );
		strLine.Format ( "<..>%s", m_data.m_strSignatureText1 );
		reportFile.WriteLine ( strLine );					// "End of message."

		reportFile.Close();

//
		CReportFileDoc report ( strReportFile );
		report.SaveAsHTML( strEmailFile );

		CEmailHandler email;
		if (email.SendEmail(m_data.GetEmailAddress(RowAccount), m_data.m_strWeeklyPointsEmailText, strEmailFile) == FALSE)
		{
			strError = email.m_strError;
		}
		else
		{
			LogEmailSent(RowAccount);
			++m_nExportCount;
		}

		CFileRecycler::SendToRecycleBin ( strEmailFile );
		CFileRecycler::SendToRecycleBin ( strReportFile );
	}
	else
	{
		strError.Format("Unable to create report file ' %s ' ", strReportFile);
	}

	return strError;
}

//*******************************************************************

CString CEmailWeeklyPoints::FormatDateTime ( const char* szDate, const char* szTime )
{
	CString strLine;
	strLine.Format ( "%s @ %s", szDate, szTime );
	return strLine;
}

//*******************************************************************

void CEmailWeeklyPoints::LogEmailSent(CSQLRowAccountFull& RowAccount)
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