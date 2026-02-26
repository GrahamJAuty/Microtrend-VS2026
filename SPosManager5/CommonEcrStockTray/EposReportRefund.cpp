/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "PMSOptions.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportRefund.h"
/**********************************************************************/

CEposReportRefund::CEposReportRefund( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_TRANSERVER_REFUND;
	SetPMSDepositsFlag( TRUE );
}

/**********************************************************************/

void CEposReportRefund::Consolidate()
{
	m_nReportType = ( m_bItemServer ) ? REPORT_TYPE_ITEMSERVER_REFUND : REPORT_TYPE_TRANSERVER_REFUND;

	int nLineNo = 0;

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetTermFileList ( arrayFileInfo, m_SelectArray, FALSE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );
	
	int nServerNo = 0;
	bool bInclude = FALSE;
	COleDateTime oleTranDate;
	CString strTranDate = "";
	CString strTranTime = "";

	m_ReportFilters.ResetDbIdx();
	for ( int nFileIdx = 0; ( nFileIdx < arrayFileInfo.GetSize() ) && ( m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() == FALSE ); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		int nPMSStatus = m_PMSModes.GetPMSLocationStatus( infoFile );
		
		if ( 0 == nPMSStatus)
			continue;

		bool bIsPMSLocation = ( 1 == nPMSStatus );

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		m_ReportFilters.SetDatabase( infoFile.m_nDbIdx );

		m_PMSModes.GetPMSTransactionDate( bIsPMSLocation, infoFile, strTranDate, strTranTime );

		int nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
		{
			int nDummy = 0;
			dbLocation.GetLocSPOSVersion( infoFile.m_nLocIdx, nDummy, nConnectionType );
		}

		int nLinesToRead;
		CString strBuffer;
		if ( ::FindFirstTermLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			m_CashRSPVersionChecker.ResetTransactionVersions();

			do
			{
				StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

				CCSVEposTermLine csvIn ( strBuffer );

				int nCashRSPLineType = csvIn.GetLineTypeInt();

				if ( m_PMSModes.CheckPMSTransactionType( bIsPMSLocation, nCashRSPLineType ) == FALSE )
					continue;

				switch( nCashRSPLineType )
				{
				case CASHRSP_LINETYPE_TRAN_NORMAL:
					{
						m_CashRSPVersionChecker.ApplyPendingTransactionInfo();

						bInclude = FALSE;

						if ( FALSE == m_bItemServer )
							nServerNo = csvIn.GetTransactionServer();
							
						bool bAcceptDateTime = FALSE;

						if ( ( TRUE == bIsPMSLocation ) && ( m_PMSModes.GetDateMode() == PMS_DATEMODE_ACCOUNT ) )
							bAcceptDateTime = TRUE;
						else
						{
							CString strDate = csvIn.GetTransactionDate();
							CString strTime = csvIn.GetTransactionTime();			
				
							//FILTER FOR DATE AND TIME
							if ( strDate.GetLength() != 8 || strTime.GetLength() != 8 )
								continue;
						
							strTranDate.Format ( "20%s%s%s",
								(const char*) strDate.Right(2),
								(const char*) strDate.Mid(3,2),
								(const char*) strDate.Left(2) );

							strTranTime.Format ( "%s%s%s",
								(const char*) strTime.Left(2),
								(const char*) strTime.Mid(3,2),
								(const char*) strTime.Right(2) );

							bAcceptDateTime = SimpleTimeCheck( infoFile.m_nLocIdx, strTranDate, strTranTime );
						}

						if ( TRUE == bAcceptDateTime )
						{
							bInclude = TRUE;
							
							oleTranDate = COleDateTime (
								atoi(strTranDate.Left(4)),
								atoi(strTranDate.Mid(4,2)),
								atoi(strTranDate.Right(2)),
								0, 0, 0 );
						}
					}
					break;

				case CASHRSP_LINETYPE_TRAN_VOID:
				case CASHRSP_LINETYPE_WASTAGE:
				case CASHRSP_LINETYPE_NOSALE:
				case CASHRSP_LINETYPE_AGEVERIFY:
					m_CashRSPVersionChecker.ApplyPendingTransactionInfo();
					bInclude = FALSE;
					break;

				case CASHRSP_LINETYPE_PAYMENT:
					if ( TRUE == bInclude )
					{
						if ( CONNECTION_TYPE_STANDARD_NONE == nConnectionType )
							if ( SysInfo.IsPMSSystem() == TRUE )
								if ( ( m_PMSModes.GetEposFlag() == FALSE ) && ( FALSE == bIsPMSLocation ) )
									if ( DataManager.Payment.IsPMSPaymentType( csvIn.GetPaymentNumber() ) == TRUE )
										bInclude = FALSE;
					}
					break;

				case CASHRSP_LINETYPE_DEPOSIT_PMS:
					if ( TRUE == bInclude )
					{
						int nAmount = csvIn.GetPaymentAmount();

						if ( nAmount < 0 )
						{
							double dAmount = double ( nAmount ) / 100;
							dAmount = -dAmount;

							CEposReportConsolRefundVoid infoConsol;
	
							switch( m_nServerReportType )
							{
							case SERVER_REPORT_CODE:
							case SERVER_REPORT_SYS:
							case SERVER_REPORT_DB:
							case SERVER_REPORT_LOC:
								infoConsol.m_nServerLocOrDbSort = 0;
								infoConsol.m_nServerLocOrDbIdx = 0;
								break;

							case SERVER_REPORT_NAME:
								infoConsol.m_nServerLocOrDbSort = 999999;
								infoConsol.m_nServerLocOrDbIdx = 999999;
								break;
							}

							infoConsol.m_nServerNo = 0;
							infoConsol.m_strDate = strTranDate;
							infoConsol.m_strTime = strTranTime;
							infoConsol.m_nLineVoidBAType = EPOSREPORT_VOIDTYPE_NONE;
							infoConsol.m_nLineNo = nLineNo++;
							infoConsol.m_nPluNo = 0;
							infoConsol.m_nModifier = 0;
							infoConsol.m_nDeptNo = 0;
							infoConsol.m_strText = "PMS Deposit";
							infoConsol.m_dModQty = 1;
							infoConsol.m_dPluQty = 1;
							infoConsol.m_dValue = dAmount;
							m_ConsolArray.Consolidate( infoConsol );

							m_SelectArray.ProcessStartDate ( infoFile.m_nSelectArrayIdx, oleTranDate );
							m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, oleTranDate );
						}
					}
					break;

				case CASHRSP_LINETYPE_ITEM:
					if ( TRUE == bInclude )
					{
						if ( TRUE == m_bItemServer )
						{
							int nCashRSPVersion = m_CashRSPVersionChecker.GetCurrentTransactionVersionNo();

							nServerNo = 0;
							if ( EcrmanOptions.GetReportsAuthServerFlag() == TRUE )
								nServerNo = csvIn.GetItemAuthServerNo( nCashRSPVersion ); 

							if ( 0 == nServerNo )
								nServerNo = csvIn.GetItemServerNo( nCashRSPVersion );
						}
							
						int nDeptNo = csvIn.GetItemDeptNo();
						if ( m_ReportFilters.CheckDepartment( nDeptNo ) == FALSE )
							continue;

						CString strVoid = csvIn.GetItemVoidFlag();
						if ( strVoid == "V" )
							continue;

						CString strTest = csvIn.GetItemRefundFlag();
						if ( strTest != "R" )
							continue;

						double dMultiplier = 1.0;

						CSQLPluNoInfo infoPluNo;
						infoPluNo.m_nEposPluNo = csvIn.GetItemPluNo();
						::ProcessPluNo( infoPluNo, FALSE, TRUE );

						if ( FALSE == infoPluNo.m_bValid )
							continue;
						
						if ( infoPluNo.m_nModifier != 0 )
							dMultiplier = DataManager.Modifier.GetMultiplierForQty( infoPluNo.m_nModifier );
						
						m_PluInfoFinder.SetPluNo( infoPluNo.m_nBasePluNo );

						//REFUNDS COME FROM EPOS WITH +VE QTY AND -VE VALUE
						double dQty = csvIn.GetItemQty();
						double dLineValue = csvIn.GetItemLineValueReport();				
						dLineValue = -dLineValue;

						CEposReportConsolRefundVoid infoConsol;
	
						switch( m_nServerReportType )
						{
						case SERVER_REPORT_CODE:
						case SERVER_REPORT_SYS:
							infoConsol.m_nServerLocOrDbSort = 0;
							infoConsol.m_nServerLocOrDbIdx = 0;
							break;

						case SERVER_REPORT_NAME:
							infoConsol.m_nServerLocOrDbSort = 0;
							infoConsol.m_nServerLocOrDbIdx = DataManagerNonDb.ServerNameTable.GetServerNameIdx( infoFile.m_nLocIdx, nServerNo );
							break;

						case SERVER_REPORT_DB:
							infoConsol.m_nServerLocOrDbSort = dbDatabase.GetSortIdxFromDbIdx( m_ReportFilters.GetDbIdx() );
							infoConsol.m_nServerLocOrDbIdx = m_ReportFilters.GetDbIdx();
							break;
				
						case SERVER_REPORT_LOC:
							infoConsol.m_nServerLocOrDbSort = dbLocation.GetSortIdxFromLocIdx( infoFile.m_nLocIdx );
							infoConsol.m_nServerLocOrDbIdx = infoFile.m_nLocIdx;
							break;
						}

						infoConsol.m_nServerNo = nServerNo;
						infoConsol.m_strDate = strTranDate;
						infoConsol.m_strTime = strTranTime;
						infoConsol.m_nLineVoidBAType = EPOSREPORT_VOIDTYPE_NONE;
						infoConsol.m_nLineNo = nLineNo++;
						infoConsol.m_nPluNo = infoPluNo.m_nBasePluNo;
						infoConsol.m_nModifier = infoPluNo.m_nModifier;
						infoConsol.m_nDeptNo = nDeptNo;
						infoConsol.m_strText = m_PluInfoFinder.GetPluInfoRepText( TRUE, infoPluNo.m_nModifier, FALSE );
						infoConsol.m_dModQty = dQty;
						infoConsol.m_dPluQty = dQty * dMultiplier;
						infoConsol.m_dValue = dLineValue;
						m_ConsolArray.Consolidate( infoConsol );
				
						m_SelectArray.ProcessStartDate ( infoFile.m_nSelectArrayIdx, oleTranDate );
						m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, oleTranDate );
					}
					break;
				}

				switch( nCashRSPLineType )
				{
				case CASHRSP_LINETYPE_VERSION:
					m_CashRSPVersionChecker.ProcessCashRSPVersionLine( csvIn );
					break;

				case CASHRSP_LINETYPE_APPLICATION:
					m_CashRSPVersionChecker.ProcessAppTypeLine( csvIn );
					break;

				default:
					m_CashRSPVersionChecker.ResetPendingTransactionInfo();
					break;
				}
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) && ( m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() == FALSE ) );
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportRefund::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Date", TA_LEFT, 250 );
	m_ReportFile.AddColumn ( "Time", TA_LEFT, 200 );

	if ( SysInfo.GetMaxBasePluLen() != 0 )
		m_ReportFile.AddColumn ( "Plu No + Mod", TA_LEFT, 350 );
	else
		m_ReportFile.AddColumn ( "Plu No", TA_LEFT, 350 );

	m_ReportFile.AddColumn ( "Description", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Dept", TA_RIGHT, 200 );

	if ( SysInfo.GetMaxBasePluLen() != 0 )
		m_ReportFile.AddColumn ( "Mod Qty", TA_RIGHT, 250 );
	
	m_ReportFile.AddColumn ( "Plu Qty", TA_RIGHT, 250 );
	m_ReportFile.AddColumn ( "Value", TA_RIGHT, 250 );
	
	double dServerModQty = 0.0;
	double dServerPluQty = 0.0;
	double dServerValue = 0.0;
	
	double dTotalModQty = 0.0;
	double dTotalPluQty = 0.0;
	double dTotalValue = 0.0;
	
	CEposReportConsolRefundVoid CurrentServer;
	CurrentServer.m_nServerLocOrDbSort = -1;
	CurrentServer.m_nServerLocOrDbIdx = -1;
	
	int nSize = m_ConsolArray.GetSize();
	StatusProgress.Lock( TRUE, "Creating report" );

	for ( int i = 0 ; i < nSize ; i++ )
	{
		StatusProgress.SetPos( i, nSize );

		CEposReportConsolRefundVoid infoConsol;
		m_ConsolArray.GetAt( i, infoConsol );

		double dLineModQty = infoConsol.m_dModQty;
		double dLinePluQty = infoConsol.m_dPluQty;
		double dLineValue = infoConsol.m_dValue;
	
		if ( infoConsol.CompareServer( CurrentServer ) == FALSE )
		{
			if (CurrentServer.m_nServerLocOrDbIdx != -1)
			{
				WriteTotals("Server Total", dServerModQty, dServerPluQty, dServerValue);
			}

			m_ReportFile.RequestBlankLines(2);
			m_ReportFile.WriteReportMiscLine( GetServerHeader( infoConsol.m_nServerLocOrDbIdx, infoConsol.m_nServerNo ) );
			m_ReportFile.WriteReportMiscLine ( "<LI>" );
			
			CurrentServer = infoConsol;

			dServerModQty = 0.0;
			dServerPluQty = 0.0;
			dServerValue = 0.0;
		}
			
		CString strLineDate;
		strLineDate.Format ( "%s/%s/%s",
			(const char*) infoConsol.m_strDate.Right(2),
			(const char*) infoConsol.m_strDate.Mid(4,2),
			(const char*) infoConsol.m_strDate.Left(4) );

		CString strLineTime;
		strLineTime.Format ( "%s:%s:%s",
			(const char*) infoConsol.m_strTime.Left(2),
			(const char*) infoConsol.m_strTime.Mid(2,2),
			(const char*) infoConsol.m_strTime.Right(2) );

		CString strPluNo = ReportHelpers.GetDisplayPluNo( infoConsol.m_nPluNo, TRUE );

		if ( infoConsol.m_nModifier != 0 )
		{
			CString strExtra;
			strExtra.Format( " + %d", infoConsol.m_nModifier );
			strPluNo += strExtra;
		}
	
		CCSV csv ( '\t' );
		csv.Add( strLineDate );
		csv.Add( strLineTime );
		csv.Add ( strPluNo );
		csv.Add ( infoConsol.m_strText );
		csv.Add ( infoConsol.m_nDeptNo );

		if (SysInfo.GetMaxBasePluLen() != 0)
		{
			csv.Add(dLineModQty, SysInfo.GetDPQty(), TRUE);
		}

		csv.Add ( dLinePluQty, SysInfo.GetDPQty() );
		csv.Add ( dLineValue, SysInfo.GetDPValue() );
		m_ReportFile.WriteReportDataLine ( csv.GetLine() );

		dServerModQty += dLineModQty;
		dServerPluQty += dLinePluQty;
		dServerValue += dLineValue;

		dTotalModQty += dLineModQty;
		dTotalPluQty += dLinePluQty;
		dTotalValue += dLineValue;
	}
	StatusProgress.Unlock();

	if (CurrentServer.m_nServerLocOrDbIdx != -1)
	{
		WriteTotals("Server Total", dServerModQty, dServerPluQty, dServerValue);
	}

	m_ReportFile.RequestBlankLines(1);
	WriteTotals( "Grand Total", dTotalModQty, dTotalPluQty, dTotalValue );
	
	ShowEposDetails();
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/

void CEposReportRefund::WriteTotals( const char* szType, double dModQty, double dPluQty, double dValue )
{
	CString strType = "<..>";
	strType += szType;

	if (SysInfo.GetMaxBasePluLen() != 0)
	{
		m_ReportFile.WriteReportMiscLine("\t\t\t\t\t<LI>\t<LI>\t<LI>");
	}
	else
	{
		m_ReportFile.WriteReportMiscLine("\t\t\t\t\t<LI>\t<LI>");
	}

	CCSV csv ( '\t' );
	csv.Add ( "" );
	csv.Add ( strType );
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( "" );

	if (SysInfo.GetMaxBasePluLen() != 0)
	{
		csv.Add(dModQty, SysInfo.GetDPQty(), TRUE);
	}

	csv.Add ( dPluQty, SysInfo.GetDPQty() );
	csv.Add ( dValue, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportMiscLine ( csv.GetLine() );
}

/**********************************************************************/
