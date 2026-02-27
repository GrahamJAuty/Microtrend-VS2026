/**********************************************************************/
#include "EmailHandler.h"
#include "EmailLogDatabase.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
/**********************************************************************/
#include "EmailLowBalances.h"
/**********************************************************************/

void CEmailLowBalances::Init()
{
	m_nExportCount = 0;
	m_strTitle = "Low Balance Alerts";

	m_data.Read();
	m_strEmailLogDatabase = Filenames.GetEmailLogDatabasePathname(1);		// low balance alert log database
}

/**********************************************************************/

bool CEmailLowBalances::CheckLowBalances(CSQLRowAccountFull& RowAccount)
{
	bool bReply = FALSE;							// assume nothing to do

	CSQLRowGroupFull RowGroup;
	RowGroup.SetGroupNo(RowAccount.GetGroupNo());

	CSQLRepositoryGroup RepoGroup;
	if ( RepoGroup.SelectRow(RowGroup,NULL).GetSQLError() == SQLCRUD_ERR_NONE )
	{
		if (m_data.m_bLowBalancePurse1Reqd == TRUE)
		{
			if (RowAccount.GetPurse1Balance() < RowGroup.GetLowBalancePurse1())
			{
				bReply = TRUE;
			}
		}

		if (m_data.m_bLowBalancePurse2Reqd == TRUE)
		{
			if (RowAccount.GetPurse2Balance() < RowGroup.GetLowBalancePurse2())
			{
				bReply = TRUE;
			}
		}

		if ( m_data.m_bLowBalancePurse3Reqd == TRUE )
		{
			if (RowAccount.GetPurse3Balance() < RowGroup.GetLowBalancePurse3())
			{
				bReply = TRUE;
			}
		}
	}
	return bReply;
}

//*******************************************************************

bool CEmailLowBalances::CheckNewEmailRequired(CSQLRowAccountFull& RowAccount)
{
	bool bSendEmail = TRUE;

	CEmailLogDatabase db;
	if ( db.Open ( m_strEmailLogDatabase, DB_READONLY ) == DB_ERR_NONE )
	{
		if ( db.FindRecord ( RowAccount.GetUserIDString() ) == TRUE )	// see if any email has been sent
		{
			bSendEmail = FALSE;										// assume already sent today

			if ( m_data.m_bLowBalancePurse1Reqd == TRUE )
			{
				if ( /*db.GetPurse1DateLastUsed() != "" && */ ::CheckDoubleMatch(db.GetPurse1(), RowAccount.GetPurse1Balance()) == FALSE)	// send email if different values from last time
				{
					bSendEmail = TRUE;
				}
			}

			if ( m_data.m_bLowBalancePurse2Reqd == TRUE )
			{
				if ( /*db.GetPurse2DateLastUsed() != "" && */ ::CheckDoubleMatch ( db.GetPurse2(), RowAccount.GetPurse2Balance() ) == FALSE )	// send email if different values from last time
					bSendEmail = TRUE;
			}

			if ( m_data.m_bLowBalancePurse3Reqd == TRUE )
			{
				if ( /* db.GetPurse3DateLastUsed() != "" && */ ::CheckDoubleMatch ( db.GetPurse3(), RowAccount.GetPurse3Balance() ) == FALSE ) // send email if different values from last time
					bSendEmail= TRUE;
			}
		}
	}

	return bSendEmail;
}

//*******************************************************************

CString CEmailLowBalances::GetBalanceText( int nPurse )
{
	CString strText = "";
	
	switch( nPurse )
	{
	case 2:		
		strText = m_data.m_strLowBalancePurse2Text;
		break;

	case 3:
		strText = m_data.m_strLowBalancePurse3Text;
		break;

	case 1:
	default:
		nPurse = 1;
		strText = m_data.m_strLowBalancePurse1Text;
		break;
	}

	::TrimSpacesFromString( strText, FALSE );
	
	if ( strText == "" )
	{
		strText.Format( "Purse %d", nPurse );
	}

	if ( strText.Right(1) != " " )
	{
		strText += " ";
	}

	CString strFullLine = "<..>";
	strFullLine += strText;
	return strFullLine;
}

//*******************************************************************

CString CEmailLowBalances::GetValueText( int nPurse, double dPurse )
{
	CString strSymbol = "";

	{
		bool bSymbol = FALSE;

		switch( nPurse )
		{
		case 2:		bSymbol = m_data.m_bLowBalancePurse2Symbol;		break;
		case 3:		bSymbol = m_data.m_bLowBalancePurse3Symbol;		break;
		case 1:
		default:	bSymbol = m_data.m_bLowBalancePurse1Symbol;		break;
		}

		if ( TRUE == bSymbol )
		{
			strSymbol = "&pound;";
		}
	}

	CString strResult;
	strResult.Format( "%s%s",
		(const char*) strSymbol,
		::FormatDoubleValue( dPurse ) );

	return strResult;
}

//*******************************************************************
// email address field already checked not blank
//	strLine.Format ( "<..>This is to inform you that the balance for %s ( account %s ) is currently £%.2lf", m_pDbSmartPay->GetUserName(), m_pDbSmartPay->GetUserID(),  m_data.GetCardBalance(pDbSmartPay) );

CString CEmailLowBalances::SendEmail(CSQLRowAccountFull& RowAccount)
{
	CSQLRowGroupFull RowGroup;
	RowGroup.SetGroupNo(RowAccount.GetGroupNo());
	
	CSQLRepositoryGroup RepoGroup;
	RepoGroup.SelectRow(RowGroup, NULL);
	
	CString strError = "";

	CString strReportFile	= Filenames.GetTempReportPath (RowAccount.GetUserIDString() );
	CString strEmailFile	= Filenames.GetTempReportPath (RowAccount.GetUserIDString(), "htm" );

	CReportFileWithEndText reportFile;
	if ( reportFile.Open ( strReportFile ) == TRUE )
	{
		reportFile.SetStyle1 ( "Low Balance Alert" );
		reportFile.AddColumn ( "", TA_LEFT, 200 );			// purse text
		reportFile.AddColumn ( "", TA_RIGHT, 200 );			// balance
		reportFile.AddColumn ( "", TA_CENTER, 100 );		// loe indicator
		reportFile.AddColumn ( "", TA_LEFT, 1000 );			// last used

		CString strLine;
		strLine.Format ( "<..>This is to inform you that the balance for %s ( account %s ) is currently", RowAccount.GetUsername(), RowAccount.GetUserIDString() );

		reportFile.WriteLine ( strLine );
		reportFile.WriteLine ( "" );
	
		double dPurse;
		CString strDate;

		if ( m_data.m_bLowBalanceShowPurse1 == TRUE )
		{
			dPurse	= RowAccount.GetPurse1Balance();
			strDate	= RowAccount.GetPurse1LastSpendDate();

			CCSV csv ( '\t' );
			csv.Add ( GetBalanceText(1) );
			csv.Add ( GetValueText(1, dPurse) );

			if ( strDate != "" )
			{
				if (dPurse < RowGroup.GetLowBalancePurse1() && m_data.m_bLowBalancePurse1Reqd == TRUE)
				{
					csv.Add("LOW");
				}
				else
				{
					csv.Add("");
				}

				strLine.Format ( "last used %s @ %s", strDate, RowAccount.GetPurse1LastSpendTime() );
				csv.Add ( strLine );
			}

			reportFile.WriteLine ( csv );
		}

		if ( m_data.m_bLowBalanceShowPurse2 == TRUE )
		{
			dPurse	= RowAccount.GetPurse2Balance();
			strDate	= RowAccount.GetPurse2LastSpendDate();

			CCSV csv ( '\t' );
			csv.Add ( GetBalanceText(2) );
			csv.Add ( GetValueText(2, dPurse) );

			if (strDate != "")
			{
				if (dPurse < RowGroup.GetLowBalancePurse2() && m_data.m_bLowBalancePurse2Reqd == TRUE)
				{
					csv.Add("LOW");
				}
				else
				{
					csv.Add("");
				}

				strLine.Format ( "last used %s @ %s", strDate, RowAccount.GetPurse2LastSpendTime() );
				csv.Add ( strLine );
			}

			reportFile.WriteLine ( csv );
		}

		if ( m_data.m_bLowBalanceShowPurse3 == TRUE )
		{
			dPurse	= RowAccount.GetPurse3Balance();
			strDate	= RowAccount.GetPurse3LastSpendDate();

			CCSV csv ( '\t' );
			csv.Add ( GetBalanceText(3) );
			csv.Add ( GetValueText(3, dPurse) );

			if ( strDate != "" )
			{
				if (dPurse < RowGroup.GetLowBalancePurse3() && m_data.m_bLowBalancePurse3Reqd == TRUE)
				{
					csv.Add("LOW");
				}
				else
				{
					csv.Add("");
				}

				strLine.Format ( "last used %s @ %s", strDate, RowAccount.GetPurse3LastSpendTime() );
				csv.Add ( strLine );
			}

			reportFile.WriteLine ( csv );
		}

		if ( m_data.m_bLowBalanceShowPoints == TRUE )
		{
			CCSV csv ( '\t' );
			csv.Add ( "<..>Points " );
			csv.Add ( ::FormatIntValue (RowAccount.GetPoints() ) );
			reportFile.WriteLine ( csv );
		}

		reportFile.WriteLine ( "" );
		strLine.Format ( "<..>%s", m_data.m_strSignatureText1 );			// "End of message"
		reportFile.WriteLine ( strLine );
		reportFile.Close();


		CReportFileDoc report ( strReportFile );
		report.SaveAsHTML( strEmailFile );

		CEmailHandler email;
		if (email.SendEmail(m_data.GetEmailAddress(RowAccount), m_data.m_strLowBalanceEmailText, strEmailFile) == FALSE)
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

void CEmailLowBalances::LogEmailSent(CSQLRowAccountFull& RowAccount)
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

