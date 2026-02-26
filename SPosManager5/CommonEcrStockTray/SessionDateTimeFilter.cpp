/**********************************************************************/
#include "PMSDefines.h"
#include "PMSOptions.h"
/**********************************************************************/
#include "SessionDateTimeFilter.h"
/**********************************************************************/

CSessionDateTimeFilter::CSessionDateTimeFilter(void)
{
	Reset();
}

/**********************************************************************/

void CSessionDateTimeFilter::Reset( bool bApplySession )
{
	m_bGotFilter = FALSE;
	m_bIncludesPMS = FALSE;
	m_nPMSAllowMode = PMS_ALLOWMODE_ALL;
	m_strCheckDateTimeStart = "";
	m_strCheckDateTimeEnd = "";
	m_DateFrom.Reset();
	m_DateTo.Reset();
	m_CheckDateFrom.Reset();
	m_CheckDateTo.Reset();
	m_bSpecialDepositMode = FALSE;
	
	if ( TRUE == bApplySession )
	{
		m_bCalendarDateOnly = FALSE;
		SetSimpleSessionArray();
		m_strSessionID = "";
		m_strSessionName = "";
	}
}

/**********************************************************************/

void CSessionDateTimeFilter::SetSimpleSessionArray()
{
	m_arraySessionInfo.RemoveAll();
	
	CReportSessionInfo infoSession;
	infoSession.m_SessionDateFrom = m_CheckDateFrom;
	infoSession.m_SessionDateTo = m_CheckDateTo;
	
	m_arraySessionInfo.Add( infoSession );
}

/**********************************************************************/

void CSessionDateTimeFilter::SetCustomerBalanceCheckDates()
{	
	m_CheckDateFrom.SetDate( COleDateTime( 2011, 1, 1, 0, 0, 0 ) );
	m_CheckDateTo.SetDate( COleDateTime::GetCurrentTime() );
	SetSimpleSessionArray();
}

/**********************************************************************/

void CSessionDateTimeFilter::SetCustomerStatementCheckDates( CCustomerSelectionListAccountInfo infoCustomer )
{
	m_CheckDateFrom.SetDate( infoCustomer.m_startTime );
	m_CheckDateTo = m_DateTo;
	SetSimpleSessionArray();
}

/**********************************************************************/

void CSessionDateTimeFilter::SetDepositUnredeemedCheckDateTo()
{
	m_CheckDateTo.SetDate( COleDateTime::GetCurrentTime() );
}

/**********************************************************************/

void CSessionDateTimeFilter::SetDepositDateRangeEvent()
{
	if ( FALSE == m_bSpecialDepositMode )
	{
		m_SavedCheckDateFrom = m_CheckDateFrom;
		m_SavedCheckDateTo = m_CheckDateTo;
		
		m_CheckDateFrom.SetDate( COleDateTime( 2015, 1, 1, 0, 0, 0 ) );

		CSortedDateTimeItem CheckDateNow;
		CheckDateNow.SetDate( COleDateTime::GetCurrentTime() );
		
		if ( CheckDateNow.Compare( m_CheckDateTo ) == 1 )
			m_CheckDateTo = CheckDateNow;
		
		m_bSpecialDepositMode = TRUE;
	}
}

/**********************************************************************/

void CSessionDateTimeFilter::SetDepositDateRangeRedeem()
{
	if ( FALSE == m_bSpecialDepositMode )
	{
		m_SavedCheckDateFrom = m_CheckDateFrom;
		m_SavedCheckDateTo = m_CheckDateTo;
		
		CSortedDateTimeItem CheckDateNow;
		CheckDateNow.SetDate( COleDateTime::GetCurrentTime() );
		
		if ( CheckDateNow.Compare( m_CheckDateTo ) == 1 )
			m_CheckDateTo = CheckDateNow;
		
		m_bSpecialDepositMode = TRUE;
	}
}

/**********************************************************************/

void CSessionDateTimeFilter::SetDepositDateRangeNormal()
{
	if ( TRUE == m_bSpecialDepositMode )
	{
		m_CheckDateFrom = m_SavedCheckDateFrom;
		m_CheckDateTo = m_SavedCheckDateTo;
		m_bSpecialDepositMode = FALSE;
	}
}

/**********************************************************************/

COleDateTime CSessionDateTimeFilter::GetLatestAcceptedDateTime()
{
	COleDateTime result = COleDateTime(1900, 1, 1, 0, 0, 0);

	for (int n = 0; n < m_arraySessionInfo.GetSize(); n++)
	{
		CReportSessionInfo infoSession = m_arraySessionInfo.GetAt(n);

		COleDateTime dateSession = COleDateTime(
			infoSession.m_SessionDateTo.m_oleDateTime.GetYear(),
			infoSession.m_SessionDateTo.m_oleDateTime.GetMonth(),
			infoSession.m_SessionDateTo.m_oleDateTime.GetDay(),
			infoSession.m_nSessionEndHour,
			infoSession.m_nSessionEndMinute,
			0);

		if (dateSession > result)
		{
			result = dateSession;
		}
	}

	return result;
}

/**********************************************************************/

void CSessionDateTimeFilter::SetDates ( COleDateTime& oleDateFrom, COleDateTime& oleDateTo, CSessionCSVRecord& ReportSession, bool bApplySession )
{
	Reset( bApplySession );
	
	if ( ( oleDateFrom.GetStatus() == COleDateTime::valid ) && ( oleDateTo.GetStatus() == COleDateTime::valid ) )
	{
		if ( TRUE == bApplySession )
		{
			m_arraySessionInfo.RemoveAll();

			m_strSessionID = ReportSession.GetEntityID();
			m_strSessionName = ReportSession.GetName();

			m_bCalendarDateOnly = ( ReportSession.GetHistoryDateType() == 0 ) && ( ReportSession.GetEODFlag() == FALSE );
			
			if ( ReportSession.GetCombinedSessionFlag() == FALSE )
			{
				CReportSessionInfo info;
				ReportSession.GetSessionInfo( info );
				info.m_bSpansMidnight = info.SpansMidnight();
				m_arraySessionInfo.Add( info );
			}
			else
			{
				CReportConsolidationArray<CSortedStringItem> arraySortedIDs;
				
				{
					CStringArray arrayIDs;
					ReportSession.GetCombinedSessionIDs( arrayIDs );

					for ( int n = 0; n < arrayIDs.GetSize(); n++ )
					{
						CSortedStringItem item;
						item.m_strItem = arrayIDs.GetAt(n);
						arraySortedIDs.Consolidate( item );
					}
				}

				for ( int n = 0; n < DataManagerNonDb.ReportSession.GetSize(); n++ )
				{
					CSessionCSVRecord SessionToCheck;
					DataManagerNonDb.ReportSession.GetAt( n, SessionToCheck );
					
					if ( SessionToCheck.GetCombinedSessionFlag() == FALSE )
					{
						CSortedStringItem item;
						item.m_strItem = SessionToCheck.GetEntityID();

						int nPos;
						if ( arraySortedIDs.Find( item, nPos ) == TRUE )
						{
							CReportSessionInfo info;
							SessionToCheck.GetSessionInfo( info );
							info.m_nSessionStartType = ReportSession.GetSessionStartType();
							info.m_nHistoryDateType = ReportSession.GetHistoryDateType();
							info.m_bSpansMidnight = info.SpansMidnight();
							info.m_bContinuous = FALSE;
							m_arraySessionInfo.Add( info );
						}
					}
				}
			}
		}
		
		{
			COleDateTime oleCheckFrom = oleDateFrom;
			COleDateTime oleCheckTo = oleDateTo;

			for ( int n = 0; n < m_arraySessionInfo.GetSize(); n++ )
			{
				COleDateTime oleSessionFrom = oleDateFrom;
				COleDateTime oleSessionTo = oleDateTo;

				CReportSessionInfo infoSession = m_arraySessionInfo.GetAt(n);

				if ( TRUE == infoSession.m_bSpansMidnight )
					oleSessionTo += COleDateTimeSpan ( 1, 0, 0, 0 );

				if ( 1 == infoSession.m_nSessionStartType )
				{
					if ( infoSession.StartsBeforeSOD() )
					{
						oleSessionFrom += COleDateTimeSpan( 1, 0, 0, 0 );
						oleSessionTo += COleDateTimeSpan( 1, 0, 0, 0 );
					}
				}

				infoSession.m_SessionDateFrom.SetDate( oleSessionFrom );
				infoSession.m_SessionDateTo.SetDate( oleSessionTo );
				m_arraySessionInfo.SetAt( n, infoSession );

				if ( ( 0 == n ) || ( oleSessionFrom < oleCheckFrom ) )
					oleCheckFrom = oleSessionFrom;

				if ( ( 0 == n ) || ( oleSessionTo > oleCheckTo ) )
					oleCheckTo = oleSessionTo;
			}

			m_CheckDateFrom.SetDate( oleCheckFrom );
			m_CheckDateTo.SetDate( oleCheckTo );
		}

		m_DateFrom.SetDate( oleDateFrom );
		m_DateTo.SetDate( oleDateTo );

		m_bIncludesPMS = FALSE;

#ifdef COMPILE_PMS
		COleDateTime timePMS = COleDateTime( 2010, 1, 1, PMSOptions.GetEposReportHour(), PMSOptions.GetEposReportMinute(), 0 );

		if ( COleDateTime::valid == timePMS.m_status )
		{
			for ( int n = 0; n < m_arraySessionInfo.GetSize(); n++ )
			{
				if ( FALSE == m_arraySessionInfo[n].SpansMidnight() )
					m_bIncludesPMS |= m_arraySessionInfo[n].IsInSession( timePMS );
				else
				{
					m_bIncludesPMS |= m_arraySessionInfo[n].IsPreMidnight( timePMS );
					m_bIncludesPMS |= m_arraySessionInfo[n].IsPostMidnight( timePMS );
				}

				if ( TRUE == m_bIncludesPMS )
					break;
			}
		}
#endif

		m_bGotFilter = TRUE;
	}

	if ( m_arraySessionInfo.GetSize() > 0 )
	{
		m_strCheckDateTimeStart.Format ( "%s%2.2d%2.2d",
			(const char*) m_CheckDateFrom.m_strDateTime,
			m_arraySessionInfo[0].m_nSessionStartHour,
			m_arraySessionInfo[0].m_nSessionStartMinute );

		m_strCheckDateTimeEnd.Format ( "%s%2.2d%2.2d",
			(const char*) m_CheckDateTo.m_strDateTime,
			m_arraySessionInfo[0].m_nSessionEndHour,
			m_arraySessionInfo[0].m_nSessionEndMinute );
	}
}

/**********************************************************************/
//RETURN 0 FOR SALE OUT OF RANGE
//RETURN 1 FOR SALE IN RANGE AND CORRECT DAY
//RETURN 2 FOR SALE IN RANGE BUT APPLIES TO PREVIOUS DAY
//RETURN 3 FOR SALE IN RANGE BUT APPLIES TO SECOND PREVIOUS DAY
/**********************************************************************/

int CSessionDateTimeFilter::CheckTime( const CString& strDate, const CString& strTime )
{
	if ( FALSE == m_bGotFilter )
		return 0;

	if ( ( strDate < m_CheckDateFrom.m_strDateTime ) || ( strDate > m_CheckDateTo.m_strDateTime ) )
		return 0;

	if ( strTime.GetLength() != 6 )	
		return 0;

	COleDateTime oleTime = COleDateTime( 2005, 1, 1, atoi( strTime.Left(2)), atoi( strTime.Mid(2,2)), 0 );
	if ( oleTime.GetStatus() != COleDateTime::valid )
		return 0;

	if ( m_arraySessionInfo.GetSize() == 0 )
		return 0;

	if ( TRUE == m_arraySessionInfo[0].m_bContinuous )
	{
		CString strDateTime = strDate + strTime.Left(4);

		if ( ( strDateTime >= m_strCheckDateTimeStart ) &&( strDateTime <= m_strCheckDateTimeEnd ) )
		{
			CReportSessionInfo infoSession = m_arraySessionInfo[0];

			switch( infoSession.m_nHistoryDateType )
			{
			//CALENDAR DATE OF SALE	
			case 0:	
				return 1;
			
			//BUSINESS DATE OF SALE	
			case 1:
				return CSessionCSVRecord::IsPreSOD( oleTime ) ? 2 : 1;
				
			//CALENDAR DATE OF SESSION
			case 2:
				
				if ( ( TRUE == infoSession.m_bSpansMidnight ) && ( infoSession.IsPostMidnight( oleTime ) ) )
					return 2;
				else
					return 1;
			
			//BUSINESS DATE OF SESSION
			case 3:
				
				if ( FALSE == infoSession.m_bSpansMidnight )
					return infoSession.StartsBeforeSOD() ? 2 : 1;
				else if ( infoSession.IsPostMidnight( oleTime ) )
					return infoSession.StartsBeforeSOD() ? 3 : 2;
				else
					return infoSession.StartsBeforeSOD() ? 2 : 1;
			}
		}

		return 0;
	}
	else
	{
		for ( int n = 0; ( n < m_arraySessionInfo.GetSize() ); n++ )
		{
			CReportSessionInfo infoSession = m_arraySessionInfo[n];

			if ( ( strDate < infoSession.m_SessionDateFrom.m_strDateTime ) || ( strDate > infoSession.m_SessionDateTo.m_strDateTime ) )
				continue;

			switch( infoSession.m_nHistoryDateType )
			{
			//CALENDAR DATE OF SALE
			case 0:
				if ( FALSE == infoSession.m_bSpansMidnight )
				{
					if ( infoSession.IsInSession( oleTime ) )
						return 1;
				}
				else
				{
					if ( strDate > infoSession.m_SessionDateFrom.m_strDateTime )
						if ( infoSession.IsPostMidnight( oleTime ) )
							return 1;
					
					if ( strDate < infoSession.m_SessionDateTo.m_strDateTime ) 
						if ( infoSession.IsPreMidnight( oleTime ) )
							return 1;
				}
				break;
				
			//BUSINESS DATE OF SALE
			case 1:
				if ( FALSE == infoSession.m_bSpansMidnight )
				{
					if ( infoSession.IsInSession( oleTime ) == TRUE )
						return CSessionCSVRecord::IsPreSOD( oleTime ) ? 2 : 1;
				}
				else
				{
					if ( strDate > infoSession.m_SessionDateFrom.m_strDateTime ) 
					{
						if ( infoSession.IsPostMidnight( oleTime ) )
							return CSessionCSVRecord::IsPreSOD( oleTime ) ? 2 : 1;
					}

					if ( strDate < infoSession.m_SessionDateTo.m_strDateTime ) 
					{
						if ( infoSession.IsPreMidnight( oleTime ) )
							return CSessionCSVRecord::IsPreSOD( oleTime ) ? 2 : 1;
					}
				}
				
				break;

			//CALENDAR DATE OF SESSION
			case 2:
				if ( FALSE == infoSession.m_bSpansMidnight )
				{
					if ( infoSession.IsInSession( oleTime ) == TRUE )
						return 1;
				}
				else
				{
					if ( strDate > infoSession.m_SessionDateFrom.m_strDateTime ) 
					{
						if ( infoSession.IsPostMidnight( oleTime ) )
							return 2;
					}

					if ( strDate < infoSession.m_SessionDateTo.m_strDateTime ) 
					{
						if ( infoSession.IsPreMidnight( oleTime ) )
							return 1;
					}
				}
				
				break;
						
			//BUSINESS DATE OF SESSION
			case 3:
				if ( FALSE == infoSession.m_bSpansMidnight )
				{
					if ( infoSession.IsInSession( oleTime ) == TRUE )
						return infoSession.StartsBeforeSOD() ? 2 : 1;
				}
				else
				{
					if ( strDate > infoSession.m_SessionDateFrom.m_strDateTime ) 
					{
						if ( infoSession.IsPostMidnight( oleTime ) )
							return infoSession.StartsBeforeSOD() ? 3 : 2;
					}

					if ( strDate < infoSession.m_SessionDateTo.m_strDateTime ) 
					{
						if ( infoSession.IsPreMidnight( oleTime ) )
							return infoSession.StartsBeforeSOD() ? 2 : 1;
					}
				}
				break;
			}
		}
		return 0;
	}
}

/**********************************************************************/

bool CSessionDateTimeFilter::CheckDate( CString& strDate )
{
	bool bResult = FALSE;
	
	if ( TRUE == m_bGotFilter )
		if ( strDate.GetLength() == 8 )
			bResult = ( ( strDate >= m_CheckDateFrom.m_strDateTime ) && ( strDate <= m_CheckDateTo.m_strDateTime ) );

	return bResult;
}

/**********************************************************************/

bool CSessionDateTimeFilter::ValidateBusinessDate( CString& strDate, CString& strTime, int& nDateType, COleDateTime& dateTransaction )
{
	nDateType = 0;
	bool bResult = FALSE;

	if ( ( strDate.GetLength() == 8 ) && ( strTime.GetLength() == 6 ) )
	{	
		if ( ( strDate >= m_CheckDateFrom.m_strDateTime ) && ( strDate <= m_CheckDateTo.m_strDateTime ) )
		{
			CString strDateTime = strDate + strTime;
				
			if ( GetOleDateFromString( strDate, dateTransaction ) == TRUE )
			{
				nDateType = CheckTime( strDate, strTime );
				switch( nDateType )
				{
				case 1:
					bResult = TRUE;
					break;

				case 2:
					dateTransaction -= COleDateTimeSpan( 1, 0, 0, 0 );
					bResult = TRUE;
					break;

				case 3:
					dateTransaction -= COleDateTimeSpan( 2, 0, 0, 0 );
					bResult = TRUE;
					break;
				}
			}
		}
	}

	return bResult;
}

/**********************************************************************/
