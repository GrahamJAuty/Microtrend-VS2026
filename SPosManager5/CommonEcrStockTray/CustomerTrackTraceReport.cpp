/**********************************************************************/
#include "HistoryFileTypes.h"
/**********************************************************************/
#include "CustomerTrackTraceReport.h"
/**********************************************************************/

CCustomerTrackTraceReport::CCustomerTrackTraceReport( int nLocIdx, int nCustIdx, int nBeforeThis, int nAfterThis, int nBeforeOther, int nAfterOther, int nDefaultVisit )
{
	m_nLocIdx = nLocIdx;
	m_nCustIdx = nCustIdx;
	m_nBeforeThis = nBeforeThis;
	m_nAfterThis = nAfterThis;
	m_nBeforeOther = nBeforeOther;
	m_nAfterOther = nAfterOther;
	m_nDefaultVisit = nDefaultVisit;
	m_strNoCheckInTime = "<..>No check in time recorded";
	m_strBadCheckOutTime = "<..>Check out time is before check in time";
	m_strDefaultVisitMarker = " **";
}

/**********************************************************************/

bool CCustomerTrackTraceReport::CreateReport()
{
	bool bResult = FALSE;

	if ( m_ReportFile.Open( Super.ReportFilename() ) == TRUE )
	{
		m_ReportFile.SetStyle1( "Customer Contact Report" );
		m_ReportFile.AddColumn ( "Customer Name", TA_LEFT, 500 );
		m_ReportFile.AddColumn ( "Est. Time In", TA_LEFT, 400 );
		m_ReportFile.AddColumn ( "Est. Time Out", TA_LEFT, 400 );
		m_ReportFile.AddColumn ( "Phone1", TA_LEFT, 500 );
		m_ReportFile.AddColumn ( "Phone2", TA_LEFT, 500 );
		m_ReportFile.AddColumn ( "Phone3", TA_LEFT, 500 );
		
		CreateReportInternal();
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CCustomerTrackTraceReport::CreateReportInternal()
{
	CCustomerCSVRecord SubjectCustomer;
	DataManagerNonDb.CustomerOneOff.GetAt( m_nCustIdx, SubjectCustomer );
			
	//CHECK THAT WE KNOW WHEN THE SUBJECT CUSTOMER ARRIVED
	int nSCFirstDay = SubjectCustomer.GetPosCreationDay() - 1;
	if ( nSCFirstDay <= Customer::PosCreationDay.Min )
	{
		CCSV csvOut( '\t' );
		csvOut.Add( SubjectCustomer.GetDisplayNameFull() );
		csvOut.Add( "" );
		csvOut.Add( "" );
		csvOut.Add( m_strNoCheckInTime );
		
		m_ReportFile.WriteReportDataLine( csvOut.GetLine() );
		return;
	}

	//ADJUST CHECK IN TIME IF NECESSARY
	if ( ( m_nBeforeThis > 0 ) && ( m_nBeforeThis < 100 ) )
	{
		int nDay = SubjectCustomer.GetPosCreationDay();
		int nTime = SubjectCustomer.GetPosCreationTime();

		nTime -= m_nBeforeThis;
		if ( nTime < 0 )
		{
			nTime += MINUTES_PER_DAY;
			nDay -= 1;
		}

		SubjectCustomer.SetPosCreationDay( nDay );
		SubjectCustomer.SetPosCreationTime( nTime );
	}
	else
	{
		m_nBeforeThis = 0;
	}

	//MAKE A LIST OF CUSTOMERS WHO ARRIVED WITH ONE DAY OF THE SUBJECT (BEFORE OR AFTER)
	CReportConsolidationArray<CCustomerTrackTraceInfo> arrayCustomerInfo;
	for ( int nCustIdx = 0; nCustIdx < DataManagerNonDb.CustomerOneOff.GetSize(); nCustIdx++ )
	{
		CCustomerCSVRecord Customer;
		DataManagerNonDb.CustomerOneOff.GetAt( nCustIdx, Customer );

		int nCreationDay = Customer.GetPosCreationDay();

		if ( ( nCreationDay >= nSCFirstDay ) && ( nCreationDay <= nSCFirstDay + 2 ) )
		{
			CCustomerTrackTraceInfo info;
			info.m_nCustID = Customer.GetCustomerID();
			info.m_nCustIndex = nCustIdx;
			info.m_nStartMinute = ( ( nCreationDay - nSCFirstDay ) * MINUTES_PER_DAY ) + Customer.GetPosCreationTime();
			info.m_bGotEndMinute = FALSE;
			info.m_nEndMinute = info.m_nStartMinute + m_nDefaultVisit;

			if ( nCustIdx != m_nCustIdx )
			{
				info.m_nStartMinute = info.m_nStartMinute - m_nBeforeOther;
				info.m_nEndMinute = info.m_nEndMinute + m_nAfterOther;
			}

			arrayCustomerInfo.Consolidate( info );
		}
	}

	//CALCULATE START AND END DATES TO SEARCH
	CStringArray arrayDates;
	COleDateTime dateStart, dateEnd;

	GetOleDateSince2011( nSCFirstDay, dateStart );
	dateEnd = dateStart - COleDateTimeSpan( 1, 0, 0, 0 );

	for ( int n = 0; n <= 2; n++ )
	{
		dateEnd += COleDateTimeSpan( 1, 0, 0, 0 );

		CString strDate;
		strDate.Format( "%4.4d%2.2d%2.2d",
			dateEnd.GetYear(),
			dateEnd.GetMonth(),
			dateEnd.GetDay() );

		arrayDates.Add( strDate );
	}
	
	//CREATE LIST OF FILES TO SEARCH FOR CUSTOMER TRANSACTIONS
	CSessionCSVRecord SessionRecord;
	DataManagerNonDb.SessionDateTimeFilter.SetDates( dateStart, dateEnd, SessionRecord, FALSE );

	CEposSelectArray SelectArray;
	SelectArray.SelectLocation( m_nLocIdx );

	int nTotalFileSize = 0;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	
	CEposReportPMSModes PMSModes;
	PMSModes.SetAllowMode( PMS_ALLOWMODE_NONE );
	CEposReportFileListMaker FileListMaker( FALSE, FALSE, PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_ACCOUNT, SelectArray, FALSE, nTotalFileSize );

	int nSCArriveTime = MINUTES_PER_DAY + SubjectCustomer.GetPosCreationTime();
	int nSCLeaveTime = nSCArriveTime + m_nDefaultVisit + m_nBeforeThis + m_nAfterThis;
	
	//SEARCH FILES FOR CUSTOMER TRANSACTIONS
	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
		{
			continue;
		}

		int nTranDay = -1;
		
		for ( int n = 0; n <= 2; n++ )
		{
			if ( infoFile.m_strDateTran == arrayDates.GetAt(n) )
			{
				nTranDay = n;
				break;
			}
		}

		if ( -1 == nTranDay )
		{
			continue;
		}

		int nTranTime = 0;

		bool bGotHeader = FALSE;
		bool bGotCustomer = FALSE;

		CString strBuffer;
		while( fileSales.ReadString( strBuffer ) )
		{
			CCSVAccountHistory csvIn( strBuffer );

			int nCustomerID = 0;

			switch( csvIn.GetAccountLineType() )
			{
			case ACCOUNT_LINETYPE_HEADER:
				{
					bGotHeader = FALSE;
					bGotCustomer = FALSE;

					CString strTime = csvIn.GetHeaderLineTime();

					if ( strTime.GetLength() == 6 )
					{
						int nHour = atoi( strTime.Left(2) );
						int nMinute = atoi( strTime.Mid(2,2) );

						if ( ( nHour >= 0 ) && ( nHour <= 23 ) && ( nMinute >= 0 ) && ( nMinute <= 59 ) )
						{
							bGotHeader = TRUE;
							nTranTime = ( nHour * 60 ) + nMinute;
						}

					}
				}
				break;

			case ACCOUNT_LINETYPE_MARKER:
				if ( csvIn.GetMarkerType() == CASHRSP_ITEMPAYTYPE_CUSTOMER )
				{
					if ( ( TRUE == bGotHeader ) && ( FALSE == bGotCustomer ) )
					{
						bGotCustomer = TRUE;

						CCustomerTrackTraceInfo info;
						info.m_nCustID = csvIn.GetMarkerID();
						
						int nPos;
						if ( arrayCustomerInfo.Find( info, nPos ) == TRUE )
						{
							arrayCustomerInfo.GetAt( nPos, info );
							info.m_bGotEndMinute = TRUE;
							info.m_nEndMinute = ( MINUTES_PER_DAY * nTranDay ) + nTranTime;

							if ( info.m_nCustIndex == m_nCustIdx )
							{
								info.m_nEndMinute += m_nAfterThis;
							}
							else
							{
								info.m_nEndMinute += m_nAfterOther;
							}

							arrayCustomerInfo.SetAt( nPos, info );
						}
					}
				}
				break;
			}	
		}
	}

	//FIND THE LEAVING TIME FOR THE SUBJECT CUSTOMER
	bool bUsedDefaultVisit = TRUE;
	
	{
		int nPos;
		CCustomerTrackTraceInfo info;
		info.m_nCustID = SubjectCustomer.GetCustomerID();

		if ( arrayCustomerInfo.Find( info, nPos ) == TRUE )
		{
			arrayCustomerInfo.GetAt( nPos, info );

			if ( TRUE == info.m_bGotEndMinute )
			{
				bUsedDefaultVisit = FALSE;
				nSCLeaveTime = info.m_nEndMinute;
			}
		}
	}

	if ( TRUE == bUsedDefaultVisit )
	{
		nSCLeaveTime = nSCArriveTime + m_nDefaultVisit + m_nBeforeThis + m_nAfterThis;
	}

	//FIDDLE : STORE LEAVING DATE/TIME IN THE RECORD CREATION DATE/TIME FIELDS 
	SubjectCustomer.SetRecordCreationDay( nSCFirstDay + ( nSCLeaveTime / MINUTES_PER_DAY ) );
	SubjectCustomer.SetRecordCreationTime( nSCLeaveTime % MINUTES_PER_DAY );

	//MAKE SURE THAT CHECK OUT TIME IS NOT BEFORE CHECK IN TIME
	if ( nSCLeaveTime < nSCArriveTime )
	{
		CCSV csvOut( '\t' );		
		csvOut.Add( SubjectCustomer.GetDisplayNameFull() );				
		csvOut.Add( SubjectCustomer.GetDisplayPosDateCreated() );
		csvOut.Add( SubjectCustomer.GetDisplayRecordDateCreated() );
		csvOut.Add( m_strBadCheckOutTime );
		
		m_ReportFile.WriteReportDataLine( csvOut.GetLine() );
		return;
	}

	//WRITE BASIC DETAILS OF SUBJECT CUSTOMER
	{
		CString strLeaveTime = SubjectCustomer.GetDisplayRecordDateCreated();
		
		if ( TRUE == bUsedDefaultVisit )
		{
			strLeaveTime += m_strDefaultVisitMarker;
		}

		CCSV csvOut( '\t' );
		csvOut.Add( SubjectCustomer.GetDisplayNameFull() );
		csvOut.Add( SubjectCustomer.GetDisplayPosDateCreated() );
		csvOut.Add( strLeaveTime );
		m_ReportFile.WriteReportDataLine( csvOut.GetLine() );
	}

	//CHECK FOR OTHER CUSTOMERS WHO WERE PRESENT AT SAME TIME
	CStringArray arrayBuffer;
	for ( int n = 0; n < arrayCustomerInfo.GetSize(); n++ )
	{
		CCustomerTrackTraceInfo info;
		arrayCustomerInfo.GetAt( n, info );

		if ( info.m_nCustIndex != m_nCustIdx )
		{
			int nTCArriveTime = info.m_nStartMinute;
			int nTCLeaveTime = info.m_nEndMinute;

			if ( nTCLeaveTime >= nTCArriveTime )
			{
				if ( ( nSCLeaveTime >= nTCArriveTime ) && ( nTCLeaveTime >= nSCArriveTime ) )
				{
					CCustomerCSVRecord ThisCustomer;
					DataManagerNonDb.CustomerOneOff.GetAt( info.m_nCustIndex, ThisCustomer );

					//FIDDLE : STORE LEAVING DATE/TIME IN THE RECORD CREATION DATE/TIME FIELDS 
					ThisCustomer.SetRecordCreationDay( nSCFirstDay + ( nTCLeaveTime / MINUTES_PER_DAY ) );
					ThisCustomer.SetRecordCreationTime( nTCLeaveTime % MINUTES_PER_DAY );

					CString strLeaveTime = ThisCustomer.GetDisplayRecordDateCreated();

					if ( FALSE == info.m_bGotEndMinute )
					{
						strLeaveTime += m_strDefaultVisitMarker;
						bUsedDefaultVisit = TRUE;
					}
		
					CCSV csvOut( '\t' );
					csvOut.Add( ThisCustomer.GetDisplayNameFull() );
					csvOut.Add( ThisCustomer.GetDisplayPosDateCreated() );
					csvOut.Add( strLeaveTime );
					csvOut.Add( ThisCustomer.GetPhone1() );
					csvOut.Add( ThisCustomer.GetPhone2() );
					csvOut.Add( ThisCustomer.GetPhone3() );
					arrayBuffer.Add( csvOut.GetLine() );
				}
			}
		}			
	}
	
	m_ReportFile.RequestBlankLines(2);
	m_ReportFile.WriteReportMiscLine( "<..>Other customers present at same time" );
	m_ReportFile.WriteReportMiscLine( "<LI>" );

	if ( arrayBuffer.GetSize() == 0 )
	{
		m_ReportFile.WriteReportDataLine( "<..>None." );
	}
	else
	{
		for ( int n = 0; n < arrayBuffer.GetSize(); n++ )
		{
			m_ReportFile.WriteReportDataLine( arrayBuffer.GetAt(n) );
		}
	}

	if ( TRUE == bUsedDefaultVisit )
	{
		m_ReportFile.RequestBlankLines(1);
		m_ReportFile.WriteReportMiscLine( "<..>(** = default visit length assumed for customers with no check out date)" );
	}
}

/**********************************************************************/
