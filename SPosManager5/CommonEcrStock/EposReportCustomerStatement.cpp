/**********************************************************************/
#include "CustomerStatementErrorInfo.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "PriceHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportCustomerStatement.h"
/**********************************************************************/

CEposReportCustomerStatement::CEposReportCustomerStatement( CEposSelectArray& SelectArray, int nNameTableIdx ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_CUSTOMER_STATEMENT;
	SetPMSAllowMode( PMS_ALLOWMODE_NONE );
	m_nNameTableIdx = nNameTableIdx;
	m_nConsolidationError = CUSTOMER_STATEMENT_ERROR_NONE;
	m_strHeaderLine1 = "";
	m_strHeaderLine2 = "";
	m_bShowRollingBalance = FALSE;
}

/**********************************************************************/

const char* CEposReportCustomerStatement::GetReportTitle( bool bDummy )
{
	CCustomerSelectionListAccountInfo infoCustomer;
	DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( m_nNameTableIdx, infoCustomer );

	m_strTitle.Format ( "Customer Statement for Account : %8.8d (%s)",
		infoCustomer.m_nCustomerID,
		infoCustomer.GetReportName() );
		
	return m_strTitle;
}

/**********************************************************************/

void CEposReportCustomerStatement::Consolidate()
{
	m_nConsolidationError = CUSTOMER_STATEMENT_ERROR_NONE;

	CCustomerSelectionListAccountInfo infoCustomer;
	DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( m_nNameTableIdx, infoCustomer );
	DataManagerNonDb.SessionDateTimeFilter.SetCustomerStatementCheckDates( infoCustomer );

	CSortedDateTimeItem DateFrom = DataManagerNonDb.SessionDateTimeFilter.GetDateFrom();
	CSortedDateTimeItem DateTo = DataManagerNonDb.SessionDateTimeFilter.GetDateTo();
	CSortedDateTimeItem CheckDateFrom = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateFrom();
	CSortedDateTimeItem CheckDateTo = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateTo();
		
	if ( ( DateFrom.m_strDateTime.GetLength() != 8 ) || ( ::TestNumeric( DateFrom.m_strDateTime ) == FALSE  ) )
	{
		m_nConsolidationError = CUSTOMER_STATEMENT_ERROR_DATERANGE;
		return;
	}

	if ( ( DateTo.m_strDateTime.GetLength() != 8 ) || ( ::TestNumeric( DateTo.m_strDateTime ) == FALSE ) )
	{
		m_nConsolidationError = CUSTOMER_STATEMENT_ERROR_DATERANGE;
		return;
	}

	if ( DateTo.m_strDateTime < DateFrom.m_strDateTime )
	{
		m_nConsolidationError = CUSTOMER_STATEMENT_ERROR_DATERANGE;
		return;
	}

	if ( DateFrom.m_strDateTime < CheckDateFrom.m_strDateTime )
	{
		m_nConsolidationError = CUSTOMER_STATEMENT_ERROR_DATEBALANCE;
		return;
	}
	
	{
		CEposReportConsolCustomerStatement infoConsol;
		infoConsol.m_strDate = DateFrom.m_strDateTime;
		infoConsol.m_strTime = "";
		infoConsol.m_nBlock = 1;
		infoConsol.m_nType = 0;
		infoConsol.m_dBalance = infoCustomer.m_dStartBalance;
		m_ConsolArray.Consolidate( infoConsol );
	}

	{
		CEposReportConsolCustomerStatement infoConsol;
		infoConsol.m_strDate = DateTo.m_strDateTime;
		infoConsol.m_strTime = "";
		infoConsol.m_nBlock = 3;
		infoConsol.m_nType = 0;
		infoConsol.m_dBalance = infoCustomer.m_dStartBalance;
		m_ConsolArray.Consolidate( infoConsol );
	}

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_ACCOUNT, m_SelectArray, FALSE, nTotalFileSize );

	int nLineNo = 1;
	bool bValidTime = FALSE;
	COleDateTime timeTransaction;
	COleDateTime dateTransaction;

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Checking customer transactions" );

	for ( int nFileIdx = 0; ( nFileIdx < arrayFileInfo.GetSize() ) && ( m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() == FALSE ); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		CEposReportConsolCustomerStatement infoConsol;
		
		CString strBuffer;
		while( fileSales.ReadString( strBuffer ) == TRUE )
		{
			CCSVAccountHistory csvIn( strBuffer );

			int nID = 0;
			
			switch( csvIn.GetAccountLineType() )
			{
			case ACCOUNT_LINETYPE_HEADER:
				{
					infoConsol.m_strDate = infoFile.m_strDateTran;
					infoConsol.m_strTime = csvIn.GetHeaderLineTime();
					bValidTime = ValidateAccountDate( infoFile.m_nLocIdx, infoFile.m_strDateTran, infoConsol.m_strTime, timeTransaction, dateTransaction );
				}
				break;

			case ACCOUNT_LINETYPE_RECEIVE:
				if ( csvIn.GetReceiveType() == CASHRSP_ITEMPAYTYPE_CUSTOMER )
				{
					nID = csvIn.GetReceiveID();
					infoConsol.m_nType = 2;
					infoConsol.m_dBalance = -csvIn.GetReceiveValue();
				}
				break;

			case ACCOUNT_LINETYPE_CHARGE:
				if ( csvIn.GetChargeType() == CASHRSP_ITEMPAYTYPE_CUSTOMER )
				{
					nID = csvIn.GetChargeID();
					infoConsol.m_nType = 1;
					infoConsol.m_dBalance = csvIn.GetChargeTender();
				}
				break;
			}

			if ( ( nID == infoCustomer.m_nCustomerID ) && ( TRUE == bValidTime ) )
			{
				if ( timeTransaction >= infoCustomer.m_startTime )
				{
					if ( ( infoConsol.m_strDate >= CheckDateFrom.m_strDateTime ) && ( infoConsol.m_strDate < DateFrom.m_strDateTime ) )
					{
						infoConsol.m_nBlock = 1;
						m_ConsolArray.Consolidate( infoConsol );
						infoConsol.m_nBlock = 3;
						m_ConsolArray.Consolidate( infoConsol );
					}
					else if ( ( infoConsol.m_strDate >= DateFrom.m_strDateTime ) && ( infoConsol.m_strDate <= DateTo.m_strDateTime ) )
					{
						infoConsol.m_nBlock = 2;
						infoConsol.m_nLineNo = nLineNo++;
						m_ConsolArray.Consolidate( infoConsol );
						infoConsol.m_nBlock = 3;
						m_ConsolArray.Consolidate( infoConsol );
					}
				}
			}
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportCustomerStatement::CreateReport()
{
	if (m_ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return FALSE;
	}

	CString strTitle = GetReportTitle( FALSE );
	m_ReportFile.SetStyle1 ( strTitle );
	m_ReportFile.AddColumn ( "", TA_LEFT, 300 );
	m_ReportFile.AddColumn ( "", TA_LEFT, 300 );
	m_ReportFile.AddColumn ( "", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "", TA_RIGHT, 250 );
	m_ReportFile.AddColumn ( "", TA_LEFT, 100 );

	if ( TRUE == m_bShowRollingBalance )
	{
		m_ReportFile.AddColumn ( "", TA_RIGHT, 200 );
		m_ReportFile.AddColumn ( "", TA_LEFT, 100 );
	}

	CString strFullLine = "<..>";
	
	if (m_strHeaderLine1 != "")
	{
		m_ReportFile.WriteReportDataLine(strFullLine + m_strHeaderLine1);
	}

	if (m_strHeaderLine2 != "")
	{
		m_ReportFile.WriteReportDataLine(strFullLine + m_strHeaderLine2);
	}

	if ( ( m_strHeaderLine1 != "" ) || ( m_strHeaderLine2 != "" ) )
	{
		m_ReportFile.WriteReportMiscLine( "<LI>" );
		m_ReportFile.RequestBlankLines(1);
	}

	m_ReportFile.WriteReportMiscLine( strFullLine + strTitle );
	m_ReportFile.WriteReportMiscLine( "<LI>" );
	m_ReportFile.RequestBlankLines(1);

	int nSize = m_ConsolArray.GetSize();
	StatusProgress.Lock( TRUE, "Creating report" );

	double dRollingBalance = 0.0;

	for ( int nPos = 0; nPos < m_ConsolArray.GetSize(); nPos++ )
	{
		CEposReportConsolCustomerStatement info;
		m_ConsolArray.GetAt( nPos, info );

		CString strDate;
		strDate.Format( "%s/%s/%s",
			info.m_strDate.Right(2),
			info.m_strDate.Mid(4,2),
			info.m_strDate.Left(4) );

		CCSV csv( '\t' );

		switch( info.m_nBlock )
		{
		case 1:
			csv.Add( strDate );
			csv.Add( "" );
			csv.Add( "" );
			csv.Add( "Balance CF" );
			AppendBalance( csv, info.m_dBalance );
			dRollingBalance = info.m_dBalance;
			m_ReportFile.WriteReportDataLine( csv.GetLine() );
			m_ReportFile.RequestBlankLines(1);
			break;

		case 2:
			{				
				CString strColumn1 = "";
				CString strColumn2 = "";

				switch( info.m_nType )
				{
				case 1:		
					strColumn1 = "Sale";	
					dRollingBalance += info.m_dBalance; 
					break;

				case 2:		
					strColumn2 = "Payment";	
					dRollingBalance += info.m_dBalance; 
					break;

				default:	
					strColumn1 = "Unknown";	
					break;
				}
		
				csv.Add( strDate );
				csv.Add( strColumn1 );
				csv.Add( strColumn2 );
				csv.Add( "" );
				AppendBalance( csv, info.m_dBalance );
				AppendRollingBalance( csv, dRollingBalance );
				m_ReportFile.WriteReportDataLine( csv.GetLine() );
			}
			break;
			

		case 3:
			csv.Add( strDate );
			csv.Add( "" );
			csv.Add( "" );
			csv.Add( "Balance" );
			AppendBalance( csv, info.m_dBalance );
			m_ReportFile.RequestBlankLines(1);
			m_ReportFile.WriteReportDataLine( csv.GetLine() );
			break;
		}
	}

	StatusProgress.Unlock();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/

void CEposReportCustomerStatement::AppendBalance( CCSV& csv, double dBalance )
{
	switch( CPriceHelpers::CompareDoubles( dBalance, 0.0, SysInfo.GetDPValue() ) )
	{
	case -1:
		csv.Add( -dBalance, SysInfo.GetDPValue() );
		csv.Add ( "CR" );
		break;

	case 1:
		csv.Add( dBalance, SysInfo.GetDPValue() );
		csv.Add( "" );
		break;

	default:
		csv.Add( 0.0, SysInfo.GetDPValue() );
		csv.Add( "" );
		break;
	}
}

/**********************************************************************/

void CEposReportCustomerStatement::AppendRollingBalance( CCSV& csv, double dBalance )
{
	if ( TRUE == m_bShowRollingBalance )
	{
		switch( CPriceHelpers::CompareDoubles( dBalance, 0.0, SysInfo.GetDPValue() ) )
		{
		case -1:
			{
				CString strBalance;
				strBalance.Format( "(%.*f)", SysInfo.GetDPValue(), -dBalance );
				csv.Add( strBalance );
				csv.Add( "CR" );
			}
			break;

		case 1:
			{
				CString strBalance;
				strBalance.Format( "(%.*f)", SysInfo.GetDPValue(), dBalance );
				csv.Add( strBalance );
				csv.Add( "" );
			}
			break;

		default:
			{
				CString strBalance;
				strBalance.Format( "(%.*f)", SysInfo.GetDPValue(), 0 );
				csv.Add( strBalance );
				csv.Add( "" );
			}
			break;
		}
	}
}

/**********************************************************************/
