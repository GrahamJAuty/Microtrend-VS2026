//*******************************************************************
#include "SQLDefines.h"
#include "SQLTable_Account\SQLRowAccountFull.h"
#include "SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "SQLTable_Group\SQLRowGroupFull.h"
#include "SQLTable_PeriodRefresh\SQLRepositoryPeriodRefresh.h"
//*******************************************************************
#ifdef SYSTEMTYPE_SERVER
//*******************************************************************
#include "..\SmartPay4Shared\MessageLogger.h"
//*******************************************************************
#endif
//*******************************************************************
#include "SQLPurse2Handler.h"
//*******************************************************************

CSQLPurse2Handler::CSQLPurse2Handler (CSQLRowAccountFull& SQLRowAccount, CSQLRowGroupFull& SQLRowGroup) :
	m_SQLRowAccount(SQLRowAccount), m_SQLRowGroup(SQLRowGroup)
{
	m_bPurse2Allowed = FALSE;
	m_bRefreshReqd = FALSE;
	m_nPeriodIndex = -1;
	m_dPeriodBalance = 0;

	SQLRowSetAuditPeriod.LoadPeriods();

	m_dateToday.SetDate("");
	m_timeToday.SetTime("");

	m_strUserID = m_SQLRowAccount.GetUserIDString();

	m_strEODRefreshDate = "";
}

//*****************************************************************

int CSQLPurse2Handler::GetPeriodIndex()
{
	// see if been set yet
	if ( m_nPeriodIndex == -1 )						
	{
		if (m_timeToday.IsSet() == TRUE)
		{
			m_nPeriodIndex = SQLRowSetAuditPeriod.GetPeriodIndex(m_timeToday.GetTime());
		}
		else
		{
			// get current period index ( 0-5 )
			m_nPeriodIndex = SQLRowSetAuditPeriod.GetPeriodIndex();
		}
	}

	return m_nPeriodIndex;
}

//*****************************************************************
// get current refresh value for current time

double CSQLPurse2Handler::GetRefreshValue()
{
	double dRefreshValue = 0;

	switch (m_SQLRowGroup.GetRefreshType())
	{
	case nREFRESHTYPE_DISABLED:
	case nREFRESHTYPE_MANUAL:
		break;

	case nREFRESHTYPE_PERIOD:
		// see if Purse2 allowed in current period
		if (m_SQLRowGroup.IsPurseAllowed(2, GetPeriodIndex()) == TRUE)
		{
			// get refresh value for current period
			dRefreshValue = m_SQLRowGroup.GetRefreshValueByPeriod(GetPeriodIndex());
		}
		break;

	default:
		dRefreshValue = m_SQLRowGroup.GetRefreshValue();
		break;
	}

	return dRefreshValue;
}

//*****************************************************************
// Get usuable purse2 balance / will refresh Purse2 if required

double CSQLPurse2Handler::GetBalance ( bool bSaveRefresh,  const char* szDate, const char* szTime )
{
	double dPurse2 = 0;
	int nRefreshType = m_SQLRowGroup.GetRefreshType();

	m_bRefreshReqd = FALSE;

	m_dateToday.SetDate ( szDate );
	if (m_dateToday.IsSet() == FALSE)								// see if date has been set
	{
		m_dateToday.SetCurrentDate();
	}

	m_timeToday.SetTime ( szTime );
	if (m_timeToday.IsSet() == FALSE)
	{
		m_timeToday.SetCurrentTime();
	}

	m_bPurse2Allowed = m_SQLRowGroup.IsPurse2Refreshable();
	
	if ( TRUE == m_bPurse2Allowed )
	{
		switch (nRefreshType)
		{
			case nREFRESHTYPE_DAILY:
			case nREFRESHTYPE_PERIOD:
			{
				// see if purse2 allowed on given day
				int nDay = m_dateToday.GetDayOfWeek();															// 1=sun, 2=mon, 0=not set
			
				if (m_SQLRowAccount.IsRefreshSet() == TRUE)
				{
					// record specific 1=sun, 2=mon etc..
					m_bPurse2Allowed = m_SQLRowAccount.IsRefreshAllowed(nDay);
				}
				else
				{
					// 0=sun, 1=mon etc..
					m_bPurse2Allowed = m_SQLRowGroup.IsRefreshAllowed(nDay - 1);
				}

				if (TRUE == m_bPurse2Allowed )									
				{
					if (nRefreshType != nREFRESHTYPE_PERIOD)
					{
						switch (m_SQLRowGroup.GetMaxSpendType())
						{
						case nGRPSPENDTYPE_PERIOD_LOCKED:
							m_bPurse2Allowed = m_SQLRowGroup.IsPurseAllowed(2, GetPeriodIndex());
							break;

						case nGRPSPENDTYPE_PERIOD_RECORD1:
							m_bPurse2Allowed = m_SQLRowAccount.IsPurseAllowed(2, GetPeriodIndex());
							break;

						case nGRPSPENDTYPE_PERIOD_RECORD2:
							if (m_SQLRowAccount.GetOwnMaxSpend() == TRUE)
							{
								m_bPurse2Allowed = m_SQLRowAccount.IsPurseAllowed(2, GetPeriodIndex());
							}
							else
							{
								m_bPurse2Allowed = m_SQLRowGroup.IsPurseAllowed(2, GetPeriodIndex());
							}
							break;
						}
					}
				}
			}
		}

		if ( m_bPurse2Allowed == TRUE )
		{
			if ( nRefreshType == nREFRESHTYPE_PERIOD )
			{
				m_bRefreshReqd = CheckPeriodRefreshReqd();
				dPurse2 = m_dPeriodBalance;										// get current purse2 balance
			}
			else
			{
				m_bRefreshReqd = CheckRefreshReqd ( nRefreshType, m_SQLRowAccount.GetPurse2RefreshedDate(), FALSE );
				dPurse2 = m_SQLRowAccount.GetPurse2Balance();							// get current purse2 balance
			}

			if ( m_bRefreshReqd == TRUE )
			{
				if (m_SQLRowGroup.GetRefreshAppendFlag() != 0)
				{
					dPurse2 += GetRefreshValue();	// append to purse2
				}
				else
				{
					dPurse2 = GetRefreshValue();	// overwrite purse2
				}

				if ( bSaveRefresh == TRUE )
				{
					//SELECT EXISTING ROW IF IT EXISTS (IT MAY NOT)
					CSQLRowPeriodRefresh RowRefresh;
					RowRefresh.SetUserID(m_SQLRowAccount.GetUserID());
					
					CSQLRepositoryPeriodRefresh RepoPeriod;
					RepoPeriod.SelectRow(RowRefresh, NULL);

					RowRefresh.SetUserID(m_SQLRowAccount.GetUserID());
					RowRefresh.SetDate(GetPeriodIndex(), m_dateToday.GetDate(), FALSE);		// set date last period refreshed
					RowRefresh.SetBalance(GetPeriodIndex(), dPurse2);						// set new purse2 balance

					RepoPeriod.UpsertRow(RowRefresh, NULL);
				}
			}
		}
	}
	else
	{
		if ( nRefreshType == nREFRESHTYPE_MANUAL )														// SmartPayServer v1.07a 16/12/2015
		{
			m_bPurse2Allowed = TRUE;
			
			switch(m_SQLRowGroup.GetMaxSpendType() )
			{
			case nGRPSPENDTYPE_PERIOD_LOCKED:
				m_bPurse2Allowed = m_SQLRowGroup.IsPurseAllowed ( 2, GetPeriodIndex() );
				break;

			case nGRPSPENDTYPE_PERIOD_RECORD1:
				m_bPurse2Allowed = m_SQLRowAccount.IsPurseAllowed ( 2, GetPeriodIndex() );
				break;

			case nGRPSPENDTYPE_PERIOD_RECORD2:
				if (m_SQLRowAccount.GetOwnMaxSpend() == TRUE)
				{
					m_bPurse2Allowed = m_SQLRowAccount.IsPurseAllowed(2, GetPeriodIndex());
				}
				else
				{
					m_bPurse2Allowed = m_SQLRowGroup.IsPurseAllowed(2, GetPeriodIndex());
				}
				break;
			}
	
			if (m_bPurse2Allowed == TRUE)
			{
				dPurse2 = m_SQLRowAccount.GetPurse2Balance();
			}
		}
	}
	
	return dPurse2;
}

//*******************************************************************************
// check for standard ( none Period ) refresh required
// nRefreshType 9=Inhibit, 1=Daily,2=weekly,3=Monthly, 4=Yearly

bool CSQLPurse2Handler::CheckRefreshReqd ( int nRefreshType, const char* szLastRefreshed, bool bPeriodRefreshAdjust )
{
	int nDay, nMonth;
	CSSDate dateTo, dateFrom;
	bool bRefreshReqd = FALSE;
	m_strEODRefreshDate = "";

	CSSDate dateRefreshed ( szLastRefreshed );							// date last refreshed date

	if (bPeriodRefreshAdjust == TRUE)									// 0=daily, 1=Weekly ..etc
	{
		nRefreshType += 1;												// 0=Inhibit, 1=daily, 2=Weekly, 3=..etc
	}

	switch ( nRefreshType )
	{
	case nREFRESHTYPE_DAILY:	
		if (dateRefreshed != m_dateToday)
		{
			nDay = m_dateToday.GetDayOfWeek();															// 1=sun, 2=mon, 0=not set
			if (m_SQLRowAccount.IsRefreshSet() == TRUE)
			{
				// record specific 1=sun, 2=mon etc..
				bRefreshReqd = m_SQLRowAccount.IsRefreshAllowed(nDay);
			}
			else
			{
				// 0=sun, 1=mon etc..
				bRefreshReqd = m_SQLRowGroup.IsRefreshAllowed(nDay - 1);
			}
		}
		break;

	case nREFRESHTYPE_WEEKLY:	
		{
			if (bPeriodRefreshAdjust == TRUE)
			{
				// 1=sun, 2=mon
				nDay = System.GetStartOfWeek();
			}
			else
			{
				// 0=Sun, 1=Mon
				nDay = m_SQLRowGroup.GetRefreshWeeklyDayNo() + 1;
			}
		
			CSSDateRange date(nDay, 1, 1, 1);
			date.GetRange(szRANGE_THIS_WEEK, dateFrom, dateTo);
			if (dateRefreshed < dateFrom)
			{
				bRefreshReqd = TRUE;
			}
		}							
		break;

	case nREFRESHTYPE_MONTHLY:	
		{
			bRefreshReqd = TRUE;

			if (dateRefreshed.GetYear() == m_dateToday.GetYear())		// see if already done this year
			{
				if (dateRefreshed.GetMonth() == m_dateToday.GetMonth())	// see if already done this month
				{
					bRefreshReqd = FALSE;								// already done this month & year - no need to do.
				}
			}
		}	
		break;

	case nREFRESHTYPE_YEARLY:	
		{
		m_SQLRowGroup.GetRefreshYearDate(nDay, nMonth);
			int nYear = m_dateToday.GetYear();

			CString strDate;
			strDate.Format("%2.2d%2.2d%d", nDay, nMonth, nYear);
			CSSDate dateRenewal(strDate);							// This years renewal date

			if (dateRenewal > m_dateToday)								// see if this years renewal date has happened yet.
			{
				--nYear;
			}

			strDate.Format("%2.2d%2.2d%d", nDay, nMonth, nYear);
			dateFrom.SetDate(strDate);

			if (dateRefreshed < dateFrom)
			{
				bRefreshReqd = TRUE;
			}
		}
		break;

	case nREFRESHTYPE_EOD:
		{
			bRefreshReqd = FALSE;

			CString strLastRefreshedDate = "";
			strLastRefreshedDate.Format("%4.4d%2.2d%2.2d",
				dateRefreshed.GetYear(),
				dateRefreshed.GetMonth(),
				dateRefreshed.GetDay());

			CString strEODRefreshDate = "";

			{
				CSSFile fileEODDate;
				if (fileEODDate.Open(Filenames.GetEODRefreshDateFilename(), "rb") == TRUE)
				{
					fileEODDate.ReadString(strEODRefreshDate);
				}
			}

			if ((strEODRefreshDate.GetLength() == 8) && (::TestNumeric(strEODRefreshDate) == TRUE))
			{
				bRefreshReqd = strEODRefreshDate > strLastRefreshedDate;
				
				if (TRUE == bRefreshReqd)
				{
					m_strEODRefreshDate.Format("%s/%s/%s",
						(const char*)strEODRefreshDate.Right(2),
						(const char*)strEODRefreshDate.Mid(4, 2),
						(const char*)strEODRefreshDate.Left(4));
				}
			}
		}	
		break;
	}

	return bRefreshReqd;
}

//*******************************************************************************
// check for daily period refresh required

bool CSQLPurse2Handler::CheckPeriodRefreshReqd()
{
	CString strDateRefreshed = "";									// date last period refreshed
	m_dPeriodBalance = 0;											// remaining balance for this period
	m_strEODRefreshDate = "";

	CSQLRowPeriodRefresh RowRefresh;
	RowRefresh.SetUserID(m_SQLRowAccount.GetUserID());

	CSQLRepositoryPeriodRefresh RepoPeriod;
	if ( RepoPeriod.SelectRow(RowRefresh, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
	{
		strDateRefreshed = RowRefresh.GetDate ( GetPeriodIndex() );			// date last period refreshed
		m_dPeriodBalance = RowRefresh.GetBalance ( GetPeriodIndex() );		// remaining period balance ( incase refresh not reqd )
	}
	
	int nRefreshType = m_SQLRowGroup.GetRefreshTypeByPeriod ( GetPeriodIndex() ); // 0=daily, 1=Weekly ..etc
	return CheckRefreshReqd ( nRefreshType, strDateRefreshed, TRUE );
}

//*******************************************************************************
