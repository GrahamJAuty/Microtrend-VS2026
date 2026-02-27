/**********************************************************************/
#include "EmailHandler.h"
#include "EmailPurchasesReport.h"
/**********************************************************************/
#include "EmailPurchases.h"
/**********************************************************************/

void CEmailPurchases::Init()
{
	m_nExportCount = 0;
	m_strTitle = "Email Itemised Purchases";
	m_data.Read();
}

//*******************************************************************

void CEmailPurchases::CreateEmail(CSQLRowAccountFull& RowAccount, CEmailLogDatabase& db, bool& bCancelButton, CString& strError)
{
	strError = "";

	if (m_data.IsValidPurchaseRecord(RowAccount) == TRUE)
	{
		CString strError = "";
		CString strReportFile = Filenames.GetTempReportPath(RowAccount.GetUserIDString());

		CEmailPurchasesReport report{};
		report.m_bExcludeZeroPriced = m_data.m_bPurchaseExcludeZeroPriced;

		bool bEntryFound = db.FindRecord(RowAccount.GetUserIDString());		// see if have previous email log entry
		if (bEntryFound == TRUE)
		{
			report.m_dateFrom.SetDate(db.GetDateLastSent());				// yes - start from last email date / time
			report.m_timeFrom.SetTime(db.GetTimeLastSent());
		}
		else
		{
			report.m_dateFrom.SetDate(m_data.m_strPurchaseStartFrom);		// no - start from default start date
		}

		if (report.Create(RowAccount,strReportFile) == TRUE)
		{
			strError = SendEmail(RowAccount, strReportFile);				// compile email message & send to outbox

			if (strError == "")
			{
				if (bEntryFound == FALSE)									// see if have email log entry
				{
					db.AddBlankRecord(RowAccount.GetUserIDString());				// no - add new record
					db.SetCardName(RowAccount.GetUsername());
				}

				db.SetEmailAddress(m_data.GetEmailAddress(RowAccount));
				db.SetDateLastSent(report.m_dateHighest.GetDate());		// highest last purchase date
				db.SetTimeLastSent(report.m_timeHighest.GetTime());		// highest last purchase time
			}
			else
			{
				bCancelButton = TRUE;
			}
		}

		CFileRecycler::SendToRecycleBin(strReportFile);
	}
}

//*******************************************************************
// email address field already checked not blank

CString CEmailPurchases::SendEmail (CSQLRowAccountFull& RowAccount, const char* szReportFile  )
{
	CString strError = "";

	CString strEmailFile = Filenames.GetTempReportPath( RowAccount.GetUserIDString(), "htm" );

	CReportFileDoc report ( szReportFile );
	report.SaveAsHTML( strEmailFile );

	CEmailHandler email;
	if (email.SendEmail(m_data.GetEmailAddress(RowAccount), m_data.m_strPurchaseEmailText, strEmailFile) == FALSE)
	{
		strError = email.m_strError;
	}
	else
	{
		++m_nExportCount;
	}

	CFileRecycler::SendToRecycleBin ( strEmailFile );

	return strError;
}

//*******************************************************************
