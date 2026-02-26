/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportVoid.h"
/**********************************************************************/

CEposReportVoidTotals::CEposReportVoidTotals()
{
	Reset();
}

/**********************************************************************/

void CEposReportVoidTotals::Reset()
{
	m_bGotTotal = FALSE;
	m_dModQty = 0.0;
	m_dPluQty = 0.0;
	m_dValue = 0.0;
}

/**********************************************************************/

void CEposReportVoidTotals::Consolidate( double dModQty, double dPluQty, double dValue )
{
	m_bGotTotal = TRUE;
	m_dModQty += dModQty;
	m_dPluQty += dPluQty;
	m_dValue += dValue;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportVoid::CEposReportVoid( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_TRANSERVER_VOID;
	SetPMSAllowMode( PMS_ALLOWMODE_NONE );
}

/**********************************************************************/

void CEposReportVoid::Consolidate()
{
	m_nReportType = ( m_bItemServer ) ? REPORT_TYPE_ITEMSERVER_VOID : REPORT_TYPE_TRANSERVER_VOID;

	int nLineNo = 0;

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetTermFileList ( arrayFileInfo, m_SelectArray, FALSE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );
	
	int nTransactionServerNo = 0;
	bool bInclude = FALSE;
	bool bIsVoidTran = FALSE;
	COleDateTime oleTranDate;
	CString strTranDate = "";
	CString strTranTime = "";

	m_ReportFilters.ResetDbIdx();
	for ( int nFileIdx = 0; ( nFileIdx < arrayFileInfo.GetSize() ) && ( m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() == FALSE ); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );
		
		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		m_ReportFilters.SetDatabase( infoFile.m_nDbIdx );

		m_CashRSPVersionChecker.ResetTransactionVersions();
		
		CString strBuffer;
		while ( ( m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() == FALSE ) && ( fileSales.ReadString ( strBuffer ) ) )
		{
			StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

			CCSVEposTermLine csvIn ( strBuffer );
			
			int nCashRSPLineType = csvIn.GetLineTypeInt();
			switch( nCashRSPLineType )
			{
			case CASHRSP_LINETYPE_TRAN_NORMAL:
			case CASHRSP_LINETYPE_TRAN_VOID:
				{
					m_CashRSPVersionChecker.ApplyPendingTransactionInfo();

					bInclude = FALSE;

					CString strDate = csvIn.GetTransactionDate();
					CString strTime = csvIn.GetTransactionTime();

					nTransactionServerNo = csvIn.GetTransactionServer();
					
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

					if ( SimpleTimeCheck( infoFile.m_nLocIdx , strTranDate, strTranTime ) == TRUE )
					{
						bInclude = TRUE;
						bIsVoidTran = ( csvIn.GetLineTypeInt() == CASHRSP_LINETYPE_TRAN_VOID );

						oleTranDate = COleDateTime (
							atoi(strTranDate.Left(4)),
							atoi(strTranDate.Mid(4,2)),
							atoi(strTranDate.Right(2)),
							0, 0, 0 );
					}
				}
				break;

			case CASHRSP_LINETYPE_WASTAGE:
			case CASHRSP_LINETYPE_NOSALE:
			case CASHRSP_LINETYPE_AGEVERIFY:
				m_CashRSPVersionChecker.ApplyPendingTransactionInfo();
				bInclude = FALSE;
				break;

			case CASHRSP_LINETYPE_ITEM:
			case CASHRSP_LINETYPE_SSFT_DELIVERY:
				if ( TRUE == bInclude )
				{
					int nDeptNo = csvIn.GetItemDeptNo();
					if ( m_ReportFilters.CheckDepartment( nDeptNo ) == FALSE )
						continue;

					int nServerNo = nTransactionServerNo;
					if ( TRUE == m_bItemServer )
					{
						int nCashRSPVersion = m_CashRSPVersionChecker.GetCurrentTransactionVersionNo();

						nServerNo = 0;
						if ( EcrmanOptions.GetReportsAuthServerFlag() == TRUE )
							nServerNo = csvIn.GetItemAuthServerNo( nCashRSPVersion ); 

						if ( 0 == nServerNo )
							nServerNo = csvIn.GetItemServerNo( nCashRSPVersion );
					}

					int nLineVoidBAType = EPOSREPORT_VOIDTYPE_NONE;

					{
						CString strTest = csvIn.GetItemVoidFlag();
						if ( ( strTest == "V" ) || ( TRUE == bIsVoidTran ) )
						{
							bool bRefund = FALSE;
							{
								CString strRefund = csvIn.GetItemRefundFlag();
								bRefund = ( strRefund == "R" );
							}

							if ( CASHRSP_LINETYPE_SSFT_DELIVERY == nCashRSPLineType )
								nLineVoidBAType = (bRefund) ? EPOSREPORT_VOIDTYPE_DELIVERY_REFUND : EPOSREPORT_VOIDTYPE_DELIVERY_SALE;
							else if ( csvIn.GetItemWastageVoidFlag( m_CashRSPVersionChecker.GetCurrentTransactionVersionNo() ) == FALSE )
								nLineVoidBAType = (bRefund) ? EPOSREPORT_VOIDTYPE_ITEM_REFUND : EPOSREPORT_VOIDTYPE_ITEM_SALE;
							else
								nLineVoidBAType = (bRefund) ? EPOSREPORT_VOIDTYPE_WASTE_REFUND : EPOSREPORT_VOIDTYPE_WASTE_SALE;
						
							if ( TRUE == bIsVoidTran )
								nLineVoidBAType += 10000;
						}
					}

					if ( nLineVoidBAType != 0 )
					{
						CSQLPluNoInfo infoPluNo;
						infoPluNo.m_nEposPluNo = csvIn.GetItemPluNo();
						::ProcessPluNo( infoPluNo, FALSE, TRUE );
						if ( FALSE == infoPluNo.m_bValid )
							continue;

						double dMultiplier = 1.0;
						CString strText = "**Unknown";

						if ( infoPluNo.m_nModifier != 0 )
							dMultiplier = DataManager.Modifier.GetMultiplierForQty( infoPluNo.m_nModifier );
					
						int nPluIdx;
						if ( DataManager.Plu.FindPluByNumber( infoPluNo.m_nBasePluNo, nPluIdx ) == TRUE )
						{
							CPluCSVRecord PluRecord;
							DataManager.Plu.GetAt( nPluIdx, PluRecord );
							strText = PluRecord.GetModifierReportText( infoPluNo.m_nModifier );
							if ( "" == strText ) strText = PluRecord.GetEposText();
						}

						double dQty = csvIn.GetItemQty();
						double dLineValue = csvIn.GetItemLineValueReport();				
						
						switch( nLineVoidBAType % 10000 )
						{
						case EPOSREPORT_VOIDTYPE_ITEM_REFUND:
						case EPOSREPORT_VOIDTYPE_DELIVERY_REFUND:
						case EPOSREPORT_VOIDTYPE_WASTE_REFUND:
							dLineValue = -dLineValue;
							break;
						}

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
						infoConsol.m_nBlock = 0;
						infoConsol.m_strDate = strTranDate;
						infoConsol.m_strTime = strTranTime;
						infoConsol.m_nLineVoidBAType = nLineVoidBAType;
						infoConsol.m_nLineNo = nLineNo++;
						infoConsol.m_nPluNo = infoPluNo.m_nBasePluNo;
						infoConsol.m_nModifier = infoPluNo.m_nModifier;
						infoConsol.m_nDeptNo = nDeptNo;
						infoConsol.m_strText = strText;
						infoConsol.m_dModQty = dQty;
						infoConsol.m_dPluQty = dQty * dMultiplier;
						infoConsol.m_dValue = dLineValue;
						infoConsol.m_nAuthServerNo = 0;
						m_ConsolArray.Consolidate( infoConsol );
			
						m_SelectArray.ProcessStartDate ( infoFile.m_nSelectArrayIdx, oleTranDate );
						m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, oleTranDate );
					}
				}
				break;

			case CASHRSP_LINETYPE_DISCOUNT_VOID:
				if ( TRUE == bInclude )
				{					
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
						infoConsol.m_nServerLocOrDbIdx = DataManagerNonDb.ServerNameTable.GetServerNameIdx( infoFile.m_nLocIdx, nTransactionServerNo );
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

					infoConsol.m_nServerNo = nTransactionServerNo;
					infoConsol.m_nBlock = 1;
					infoConsol.m_strDate = strTranDate;
					infoConsol.m_strTime = strTranTime;
					infoConsol.m_nLineVoidBAType = EPOSREPORT_VOIDTYPE_DISCOUNT;
					infoConsol.m_nLineNo = nLineNo++;
					infoConsol.m_nPluNo = 0;
					infoConsol.m_nModifier = 0;
					infoConsol.m_nDeptNo = 0;
					infoConsol.m_strText = csvIn.GetLine();
					infoConsol.m_dModQty = 0.0;
					infoConsol.m_dPluQty = 0.0;
					infoConsol.m_dValue = 0.0;
					infoConsol.m_nAuthServerNo = csvIn.GetVoidDiscountAuthServerNo( m_CashRSPVersionChecker.GetCurrentTransactionVersionNo() );
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

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportVoid::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Date", TA_LEFT, 250 );
	m_ReportFile.AddColumn ( "Time", TA_LEFT, 200 );
	m_ReportFile.AddColumn ( "Level", TA_LEFT, 300 );
	m_ReportFile.AddColumn ( "Type", TA_LEFT, 250 );

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
	m_ReportFile.AddColumn ( "Authorised By", TA_LEFT, 600 );

	CEposReportVoidTotals ServerTotalSale;
	CEposReportVoidTotals ServerTotalRefund;
	CEposReportVoidTotals ServerTotalDiscount;

	CEposReportVoidTotals GrandTotalSale;
	CEposReportVoidTotals GrandTotalRefund;
	CEposReportVoidTotals GrandTotalDiscount;
	
	CEposReportConsolRefundVoid CurrentServer;
	CurrentServer.m_nServerLocOrDbSort = -1;
	CurrentServer.m_nServerLocOrDbIdx = -1;
	
	int nSize = m_ConsolArray.GetSize();
	StatusProgress.Lock( TRUE, "Creating Report" );

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
			if ( CurrentServer.m_nServerLocOrDbIdx != -1 )
				WriteServerTotals( CurrentServer.m_nBlock, ServerTotalSale, ServerTotalRefund, ServerTotalDiscount );

			m_ReportFile.RequestBlankLines(2);
			m_ReportFile.WriteReportMiscLine( GetServerHeader( infoConsol.m_nServerLocOrDbIdx, infoConsol.m_nServerNo ) );
			m_ReportFile.WriteReportMiscLine ( "<LI>" );
			
			CurrentServer = infoConsol;

			ServerTotalSale.Reset();
			ServerTotalRefund.Reset();
			ServerTotalDiscount.Reset();
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

		CCSV csv ( '\t' );
		csv.Add( strLineDate );
		csv.Add( strLineTime );

		switch( infoConsol.m_nBlock )
		{
		//ITEM AND TRANSACTION VOIDS
		case 0:
			{
				CString strPluNo = ReportHelpers.GetDisplayPluNo( infoConsol.m_nPluNo, TRUE );

				if ( infoConsol.m_nModifier != 0 )
				{
					CString strExtra;
					strExtra.Format( " + %d", infoConsol.m_nModifier );
					strPluNo += strExtra;
				}

				if ( infoConsol.m_nLineVoidBAType >= 10000 )
					csv.Add( "Transaction" );
				else
					csv.Add( "Item" );
				
				switch( infoConsol.m_nLineVoidBAType % 10000 )
				{
				case EPOSREPORT_VOIDTYPE_ITEM_SALE:			csv.Add( "Sale" );				break;
				case EPOSREPORT_VOIDTYPE_ITEM_REFUND:		csv.Add( "Refund" );			break;
				case EPOSREPORT_VOIDTYPE_DELIVERY_SALE:		csv.Add( "Delivery" );			break;
				case EPOSREPORT_VOIDTYPE_DELIVERY_REFUND:	csv.Add( "Delivery Refund" );	break;
				case EPOSREPORT_VOIDTYPE_WASTE_SALE:		csv.Add( "Waste" );				break;
				case EPOSREPORT_VOIDTYPE_WASTE_REFUND:		csv.Add( "Waste Refund" );		break;
				default:									csv.Add( "Unknown" );			break;
				}

				csv.Add ( strPluNo );
				csv.Add ( infoConsol.m_strText );
				csv.Add ( infoConsol.m_nDeptNo );
				
				if ( SysInfo.GetMaxBasePluLen() != 0 )
					csv.Add ( dLineModQty, SysInfo.GetDPQty(), TRUE );
				
				csv.Add ( dLinePluQty, SysInfo.GetDPQty() );
				csv.Add ( dLineValue, SysInfo.GetDPValue() );

				switch ( infoConsol.m_nLineVoidBAType % 10000 )
				{
				case EPOSREPORT_VOIDTYPE_ITEM_REFUND:		
				case EPOSREPORT_VOIDTYPE_DELIVERY_REFUND:
				case EPOSREPORT_VOIDTYPE_WASTE_REFUND:
					ServerTotalRefund.Consolidate( dLineModQty, dLinePluQty, dLineValue );
					GrandTotalRefund.Consolidate( dLineModQty, dLinePluQty, dLineValue );
					break;

				case EPOSREPORT_VOIDTYPE_ITEM_SALE:		
				case EPOSREPORT_VOIDTYPE_DELIVERY_SALE:	
				case EPOSREPORT_VOIDTYPE_WASTE_SALE:	
				default:
					ServerTotalSale.Consolidate( dLineModQty, dLinePluQty, dLineValue );
					GrandTotalSale.Consolidate( dLineModQty, dLinePluQty, dLineValue );
					break;
				}
			}
			break;

		//DISCOUNT VOIDS 
		case 1:
			{
				csv.Add( "Discount" );

				CCSVEposTermLine csvIn( infoConsol.m_strText );

				CString strDiscountType = "";
				switch( csvIn.GetVoidDiscountUsage() )
				{
				case 1:		strDiscountType += "Subtotal";		break;
				case 2:		strDiscountType += "Loyalty";		break;
				case 3:		strDiscountType += "Differ";		break;
				default:	strDiscountType += "??Usage";		break;
				}
	
				CString strType = csvIn.GetVoidDiscountType();
					
				if		( strType == "M" )	strDiscountType += " Markdown";
				else if	( strType == "D" )	strDiscountType += " Discount";
				else if	( strType == "P" )	strDiscountType += " Premium";
				else						strDiscountType += " ??Discount";

				double dDiscountAmount = 0.0;
				{
					int nDiscountAmount = csvIn.GetVoidDiscountAmount();
					nDiscountAmount *= -1;
					dDiscountAmount = ( (double) nDiscountAmount ) / 100.0;
				}

				CString strRate = "";
				if ( strType != "M" )
					strRate.Format ( "%.2f%%", ( (double) csvIn.GetVoidDiscountRate() ) / 100.0 );

				CString strPromo;
				
				int nPromoIdx = 0;
				int nPromoNo = csvIn.GetVoidDiscountPromo();
				if ( DataManager.Promotion.FindTableByNumber( nPromoNo, nPromoIdx ) == TRUE )
				{
					CPromotionsCSVRecord Promo;
					DataManager.Promotion.GetAt( nPromoIdx, Promo );
					strPromo = Promo.GetDisplayName();
				}
				else
				{
					strPromo.Format("Promo %d", nPromoNo);
				}

				csv.Add( strDiscountType );
				csv.Add( strPromo );
				
				if (SysInfo.GetMaxBasePluLen() != 0)
				{
					csv.Add("");
				}

				csv.Add( "" );
				csv.Add( "" );
				csv.Add( strRate );
				csv.Add( dDiscountAmount, 2 );

				if (infoConsol.m_nAuthServerNo != 0)
				{
					csv.Add(GetServerColumnName(infoConsol.m_nServerLocOrDbIdx, infoConsol.m_nAuthServerNo));
				}

				ServerTotalDiscount.Consolidate( 0.0, 0.0, dDiscountAmount );
				GrandTotalDiscount.Consolidate( 0.0, 0.0, dDiscountAmount );
			}
			break;
		}

		m_ReportFile.WriteReportDataLine ( csv.GetLine() );
	}
	StatusProgress.Unlock();

	if ( CurrentServer.m_nServerLocOrDbIdx != -1 )
		WriteServerTotals( CurrentServer.m_nBlock, ServerTotalSale, ServerTotalRefund, ServerTotalDiscount );

	if ( TRUE == ( GrandTotalSale.m_bGotTotal | GrandTotalRefund.m_bGotTotal ) )
	{
		m_ReportFile.RequestBlankLines(1);
		
		if ( TRUE == GrandTotalSale.m_bGotTotal )
			WriteVoidTotals( "Grand Total Voids (Sales)", GrandTotalSale, FALSE, TRUE );

		if ( TRUE == GrandTotalRefund.m_bGotTotal )
			WriteVoidTotals( "Grand Total Voids (Refunds)", GrandTotalRefund, FALSE, ( FALSE == GrandTotalSale.m_bGotTotal ) );
	}

	if ( TRUE == GrandTotalDiscount.m_bGotTotal )
	{
		m_ReportFile.RequestBlankLines(1);
		WriteVoidTotals( "Grand Total Voids (Discounts)", GrandTotalDiscount, TRUE, TRUE );
	}

	if ( FALSE == ( GrandTotalSale.m_bGotTotal | GrandTotalRefund.m_bGotTotal | GrandTotalDiscount.m_bGotTotal ) )
	{
		m_ReportFile.RequestBlankLines(1);
		CEposReportVoidTotals Dummy;
		WriteVoidTotals( "Grand Total", Dummy, FALSE, TRUE );
	}
		
	ShowEposDetails();
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/

void CEposReportVoid::WriteServerTotals( int nBlock, CEposReportVoidTotals& Sales, CEposReportVoidTotals& Refunds, CEposReportVoidTotals& Discounts  )
{
	if ( 0 == nBlock )
	{
		if ( TRUE == Sales.m_bGotTotal )
			WriteVoidTotals( "Server Total Voids (Sales)", Sales, FALSE, TRUE );

		if ( TRUE == Refunds.m_bGotTotal )
			WriteVoidTotals( "Server Total Voids (Refunds)", Refunds, FALSE, ( FALSE == Sales.m_bGotTotal ) );
	}
	else
	{
		if ( TRUE == Discounts.m_bGotTotal )
			WriteVoidTotals( "Server Total Voids (Discounts)", Discounts, TRUE, TRUE );
	}
}

/**********************************************************************/

void CEposReportVoid::WriteVoidTotals( const char* szType, CEposReportVoidTotals& Totals, bool bDiscount, bool bUnderline )
{
	CString strUnderline = "";

	CString strType = "<..>";
	strType += szType;

	CCSV csv ( '\t' );
	csv.Add ( "" );
	csv.Add ( strType );
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( "" );
		
	if ( FALSE == bDiscount )
	{
		if (SysInfo.GetMaxBasePluLen() != 0)
		{
			strUnderline = "\t\t\t\t\t\t\t<LI>\t<LI>\t<LI>";
		}
		else
		{
			strUnderline = "\t\t\t\t\t\t\t<LI>\t<LI>";
		}

		if (SysInfo.GetMaxBasePluLen() != 0)
		{
			csv.Add(Totals.m_dModQty, SysInfo.GetDPQty(), TRUE);
		}

		csv.Add ( Totals.m_dPluQty, SysInfo.GetDPQty() );
		csv.Add ( Totals.m_dValue, SysInfo.GetDPValue() );
	}
	else
	{
		if (SysInfo.GetMaxBasePluLen() != 0)
		{
			strUnderline = "\t\t\t\t\t\t\t\t\t<LI>";
		}
		else
		{
			strUnderline = "\t\t\t\t\t\t\t\t<LI>";
		}

		if (SysInfo.GetMaxBasePluLen() != 0)
		{
			csv.Add("");
		}

		csv.Add ( "" );
		csv.Add ( Totals.m_dValue, SysInfo.GetDPValue() );
	}

	if (TRUE == bUnderline)
	{
		m_ReportFile.WriteReportMiscLine(strUnderline);
	}

	m_ReportFile.WriteReportMiscLine ( csv.GetLine() );
}

/**********************************************************************/
