//*******************************************************************
#include "..\SmartPay4Shared\TLogDatabase.h"
//*******************************************************************
#include "EmailPurchasesReport.h"
//*******************************************************************

CEmailPurchasesReport::CEmailPurchasesReport ()
{
	m_nLineCount = 0;
	m_strLastDate = "";
	m_bExcludeZeroPriced = FALSE;

	m_dateFrom.SetDate ( "" );					// MUST be set externally before create used
	m_timeFrom.SetTime ( "" );					// MUST be set externally before create used

	m_dateTo.SetCurrentDate();
	m_dateHighest.SetDate("");					// used to stop sent emails more than once
	m_timeHighest.SetTime("");

	m_dTotalCardSpend = 0.0;
	m_dTotalQty = 0.0;
	m_dTotalValue = 0.0;
}

//*******************************************************************

void CEmailPurchasesReport::Reset()
{
	m_dTotalQty			= 0.0;
	m_dTotalValue		= 0.0;
	m_dTotalCardSpend	= 0.0;
}

//*********************************************************************

bool CEmailPurchasesReport::Create ( CSQLRowAccountFull& RowAccount, const char* szReportFile )
{
	bool bReply;
	if ( ( bReply = m_Report.Open ( szReportFile ) ) == TRUE )
	{
		m_Report.SetStyle1 ( "Itemised Purchases" );
		AddColumnText();

		CTLogFile tlogFile ( RowAccount.GetUserIDString() );
		if ( tlogFile.Open() == TRUE )
		{
			AccountItemisedReport ( RowAccount, &tlogFile );

			tlogFile.Close();
		}
		m_Report.Close();
	}

	return ( m_nLineCount == 0 ) ? FALSE : bReply;
}

//******************************************************************************

void CEmailPurchasesReport::AccountItemisedReport (CSQLRowAccountFull& RowAccount, CTLogFile* pTlogFile )
{
	int nLineCount = 0;

	Reset();

	pTlogFile->JumpIn ( m_dateFrom.GetDate() );

	CTLogRecord tlogRecord;
	while ( pTlogFile->ReadLine ( &tlogRecord ) == TRUE )
	{
		if ( tlogRecord.m_date < m_dateFrom )
			continue;										// not reach date yet
	
		if ( tlogRecord.m_date == m_dateFrom )				// date matches
		{
			if ( m_timeFrom.IsSet() == TRUE )
			{
				if ( tlogRecord.m_time <= m_timeFrom )		// has already been sent
					continue;
			}
		}

		if ( tlogRecord.m_date > m_dateTo )					// passed reqd date
			break;

		if ( nLineCount == 0 )								// need header before first line
		{
			CString strHeader;
			strHeader.Format ( "<..>Account %s : %I64d  -  %s", 
				(const char*) ::FormatGroupNo(RowAccount.GetGroupNo()),
				RowAccount.GetUserID(), 
				(const char*) RowAccount.GetUsername() );

			m_Report.WriteLine ( strHeader );
			m_Report.WriteLine ( "<LI>" );
		}

		ShowItemisedSales ( &tlogRecord );
		++nLineCount;
	}

	if ( nLineCount > 0 )
	{
		ShowTotals();
		m_nLineCount += nLineCount;
	}
}

//*****************************************************************************

void CEmailPurchasesReport::ShowItemisedSales ( CTLogRecord* pTLogRecord )
{
	for ( int nIndex = 0; nIndex < pTLogRecord->GetItemCounter(); nIndex++ )
	{
		CCSV csv ( '\t' );

		if ( m_strLastDate == pTLogRecord->GetDate() )				// see if date same as last purchase
		{
			csv.Add ( "" );											// save date as last transaction

			if ( m_dateHighest == pTLogRecord->GetDate() )			// see if date is highest date so far
				m_timeHighest = pTLogRecord->m_time;				// yes - save new highest time
		}
		else
		{
			m_strLastDate = pTLogRecord->GetDate();
			csv.Add ( m_strLastDate );								// new transaction date

			if ( pTLogRecord->m_date > m_dateHighest )
			{
				m_dateHighest = pTLogRecord->m_date;
				m_timeHighest = pTLogRecord->m_time;
			}
		}

		if ( nIndex == 0 )
		{
			csv.Add ( pTLogRecord->GetTime() );
			csv.Add ( pTLogRecord->GetTotalSpend(),System.GetDPValue() );
			m_dTotalCardSpend += pTLogRecord->GetTotalSpend();
		}
		else
		{
			csv.Add ( "" );
			csv.Add ( "" );
		}

		pTLogRecord->ExtractItemData ( nIndex );

		double dItemValue = pTLogRecord->GetItemLineVal();

		if ( dItemValue == 0 && m_bExcludeZeroPriced == TRUE )
			continue;

		csv.Add ( ::FormatQty ( pTLogRecord->GetItemLineQty()) );
		csv.Add ( pTLogRecord->GetItemText() );
		csv.Add ( dItemValue, System.GetDPValue() );

		m_Report.WriteLine ( csv );

		m_dTotalValue += dItemValue;
	}
}

//**********************************************************************

void CEmailPurchasesReport::AddColumnText()
{
	m_Report.AddColumn ( "Date", TA_LEFT, 219 );
	m_Report.AddColumn ( "Time", TA_LEFT, 120 );
	m_Report.AddColumn ( "Card Spend", TA_RIGHT, 249 );
	m_Report.AddColumn ( "Qty", TA_RIGHT, 120);
	m_Report.AddColumn ( "Description", TA_LEFT, 500 );
	m_Report.AddColumn ( "Value", TA_RIGHT, 249 );
}

//**********************************************************************

void CEmailPurchasesReport::ShowTotals()
{
	m_Report.WriteLine ( "\t\t<LI>" );

	CCSV csv ( '\t' );

	csv.Add ( "<..>Total Card Spend" );					// date
	csv.Add ( "" );										// time
	csv.Add ( m_dTotalCardSpend, System.GetDPValue() );	// card spend
	m_Report.WriteLine ( csv );

	double dDiffer = m_dTotalValue - m_dTotalCardSpend;
	dDiffer = ::RoundDoubleValue ( dDiffer );			// lose rounding errors on value

	if ( dDiffer != 0  )
	{
		CCSV csv ( '\t' );
		csv.Add ( "<..>Other Cash Spend" );				// date
		csv.Add ( "" );									// time
		csv.Add ( dDiffer, System.GetDPValue() );			// cash differ
		m_Report.WriteLine ( csv );
	}

	m_Report.WriteLine("");
}

//*****************************************************************************
