/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportVoidReason.h"
/**********************************************************************/

CEposReportVoidReason::CEposReportVoidReason( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_TRANSERVER_VOID_REASON;
	SetPMSAllowMode( PMS_ALLOWMODE_NONE );
}

/**********************************************************************/

void CEposReportVoidReason::Consolidate()
{
	m_nReportType = ( m_bItemServer ) ? REPORT_TYPE_ITEMSERVER_VOID_REASON : REPORT_TYPE_TRANSERVER_VOID_REASON;

	int nLineNo = 0;

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_REASONS_ALL, m_SelectArray, FALSE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );

	m_ReportFilters.ResetDbIdx();
	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		//CHECK PMS STATUS
		if ( dbLocation.IsPMSPseudoLocation( infoFile.m_nLocIdx ) == TRUE )
			continue;

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		CString strThisSaleDate = "";
		CString strThisSaleTime = "";
		m_PMSModes.GetInitialSaleDateTime( FALSE, infoFile, m_bSaleTime, strThisSaleDate, strThisSaleTime );

		m_ReportFilters.SetDatabase( infoFile.m_nDbIdx );

		CCSV csvVoid( "" );

		bool bLegacy = FALSE;
		{
			CString strTest = infoFile.m_strFilename;
			strTest = strTest.Right(12);
			strTest = strTest.Left(8);
			strTest.MakeUpper();
			bLegacy = ( strTest == "RFNDVOID" );
		}

		CString strBuffer = "";
		while ( fileSales.ReadString( strBuffer ) == TRUE )
		{
			StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

			CReasonInfo ReasonInfo;
			csvVoid.ParseLine( strBuffer );

			if ( TRUE == bLegacy )
				ReasonInfo.LoadLegacyLine( csvVoid );
			else
				ReasonInfo.LoadCSVLine( csvVoid );

			int nLineType = ReasonInfo.GetReasonType();

			switch( nLineType )
			{
			case SS_REASON_REFUNDVOID:
				{
					//EXCLUDE NON VOID LINE
					if ( ReasonInfo.GetItemVoidFlag() == FALSE )
						continue;

					//CHECK VOID DATE AND TIME
					if ( CheckReasonInfoDateTime( infoFile, ReasonInfo, strThisSaleDate, strThisSaleTime ) == FALSE )
						continue;

					//DEPARTMENT FILTER
					int nDeptNo = ReasonInfo.GetItemDeptNo();
					if ( m_ReportFilters.CheckDepartment( nDeptNo ) == FALSE )
						continue;

					//SERVER NO
					int nServerNo;
					if ( TRUE == m_bItemServer )
					{
						nServerNo = 0;
						if ( EcrmanOptions.GetReportsAuthServerFlag() == TRUE )
							nServerNo = ReasonInfo.GetItemAuthServerNo();

						if ( 0 == nServerNo )
							nServerNo = ReasonInfo.GetItemServerNo();
					}
					else
						nServerNo = ReasonInfo.GetTranServerNo();

					//PLU NO AND MODIFIER
					CSQLPluNoInfo infoPluNo;
					infoPluNo.m_nEposPluNo = ReasonInfo.GetItemPluNo();
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

					//QTY AND VALUE (MAKE POSITIVE)
					double dQty = ReasonInfo.GetItemQty();
					double dLineValue = ReasonInfo.GetItemVal();	
					
					if ( TRUE == ReasonInfo.GetItemRefundFlag() )
					{
						dQty *= -1;
						dLineValue *= -1;
					}

					//dLineValue *= -1;

					//SERVER NO
					int nServerLocOrDbIdx = 0;
					int nServerLocOrDbSort = 0;
					switch( m_nServerReportType )
					{
					case SERVER_REPORT_CODE:
					case SERVER_REPORT_SYS:
						nServerLocOrDbSort = 0;
						nServerLocOrDbIdx = 0;
						break;

					case SERVER_REPORT_NAME:
						nServerLocOrDbSort = 0;
						nServerLocOrDbIdx = DataManagerNonDb.ServerNameTable.GetServerNameIdx( infoFile.m_nLocIdx, nServerNo );
						break;

					case SERVER_REPORT_DB:
						nServerLocOrDbSort = dbDatabase.GetSortIdxFromDbIdx( infoFile.m_nDbIdx );
						nServerLocOrDbIdx = infoFile.m_nDbIdx;
						break;
			
					case SERVER_REPORT_LOC:
						nServerLocOrDbSort = dbLocation.GetSortIdxFromLocIdx( infoFile.m_nLocIdx );
						nServerLocOrDbIdx = infoFile.m_nLocIdx;
						break;
					}

					CString strReasonText = ReasonInfo.GetReasonText();
					CCSV csvReason( strReasonText, ',', '\'' );

					CEposReportConsolVoidReason infoConsol;
					
					if ( FALSE == m_bSaleTime )
					{
						infoConsol.m_strReportDate = infoFile.m_strDateTran;
						infoConsol.m_strReportTime = ReasonInfo.GetTimeTran();
						infoConsol.m_strOtherTime = ReasonInfo.GetTimeAction();
					}
					else
					{
						infoConsol.m_strReportDate = infoFile.m_strDateSale;
						infoConsol.m_strReportTime = ReasonInfo.GetTimeAction();
						infoConsol.m_strOtherTime = ReasonInfo.GetTimeTran();
					}

					infoConsol.m_bGrandTotal = FALSE;
					infoConsol.m_nServerLocOrDbSort = nServerLocOrDbSort;
					infoConsol.m_nServerLocOrDbIdx = nServerLocOrDbIdx;
					infoConsol.m_nServerNo = nServerNo;
					infoConsol.m_nTableNo = ReasonInfo.GetTranTableNo();
					infoConsol.m_nSeqNo = ReasonInfo.GetTranSeqNo();
					infoConsol.m_nLineNo = nLineNo++;
					infoConsol.m_nPluNo = infoPluNo.m_nBasePluNo;
					infoConsol.m_nModifier = infoPluNo.m_nModifier;
					infoConsol.m_nDeptNo = nDeptNo;
					infoConsol.m_strPluText = strText;
					infoConsol.m_dModQty = dQty;
					infoConsol.m_dPluQty = dQty * dMultiplier;
					infoConsol.m_dValue = dLineValue;
					infoConsol.m_strReason = csvReason.GetString(0);
					infoConsol.m_bWaste = FALSE;

					CString strBeforeAfter = ReasonInfo.GetItemVoidBAType();
					if ( strBeforeAfter != "A" )
					{
						if ( ReasonInfo.GetItemRefundFlag() == FALSE )
						{
							infoConsol.m_nBlock = VOID_SERVER_SALES_BEFORE_LINE;
							m_ConsolArray.Consolidate( infoConsol );
							infoConsol.m_nBlock = VOID_SERVER_SALES_BEFORE_TOTAL;
							m_ConsolArray.Consolidate( infoConsol );
							infoConsol.m_nBlock = VOID_SERVER_SALES_COMBINED_TOTAL;
							m_ConsolArray.Consolidate( infoConsol );

							infoConsol.m_bGrandTotal = TRUE;
							infoConsol.m_nBlock = VOID_TOTAL_SALES_BEFORE;
							m_ConsolArray.Consolidate( infoConsol );
							infoConsol.m_nBlock = VOID_TOTAL_SALES_COMBINED;
							m_ConsolArray.Consolidate( infoConsol );
						}
						else
						{
							infoConsol.m_nBlock = VOID_SERVER_REFUND_BEFORE_LINE;
							m_ConsolArray.Consolidate( infoConsol );
							infoConsol.m_nBlock = VOID_SERVER_REFUND_BEFORE_TOTAL;
							m_ConsolArray.Consolidate( infoConsol );
							infoConsol.m_nBlock = VOID_SERVER_REFUND_COMBINED_TOTAL;
							m_ConsolArray.Consolidate( infoConsol );

							infoConsol.m_bGrandTotal = TRUE;
							infoConsol.m_nBlock = VOID_TOTAL_REFUND_BEFORE;
							m_ConsolArray.Consolidate( infoConsol );
							infoConsol.m_nBlock = VOID_TOTAL_REFUND_COMBINED;
							m_ConsolArray.Consolidate( infoConsol );
						}
					}
					else
					{
						infoConsol.m_bWaste = ReasonInfo.GetItemWastageVoidFlag();

						if ( ReasonInfo.GetItemRefundFlag() == FALSE )
						{
							infoConsol.m_nBlock = VOID_SERVER_SALES_AFTER_LINE;
							m_ConsolArray.Consolidate( infoConsol );
							infoConsol.m_nBlock = VOID_SERVER_SALES_AFTER_TOTAL;
							m_ConsolArray.Consolidate( infoConsol );
							infoConsol.m_nBlock = VOID_SERVER_SALES_COMBINED_TOTAL;
							m_ConsolArray.Consolidate( infoConsol );

							infoConsol.m_bGrandTotal = TRUE;
							infoConsol.m_nBlock = VOID_TOTAL_SALES_AFTER;
							m_ConsolArray.Consolidate( infoConsol );
							infoConsol.m_nBlock = VOID_TOTAL_SALES_COMBINED;
							m_ConsolArray.Consolidate( infoConsol );
						}
						else
						{
							infoConsol.m_nBlock = VOID_SERVER_REFUND_AFTER_LINE;
							m_ConsolArray.Consolidate( infoConsol );
							infoConsol.m_nBlock = VOID_SERVER_REFUND_AFTER_TOTAL;
							m_ConsolArray.Consolidate( infoConsol );
							infoConsol.m_nBlock = VOID_SERVER_REFUND_COMBINED_TOTAL;
							m_ConsolArray.Consolidate( infoConsol );

							infoConsol.m_bGrandTotal = TRUE;
							infoConsol.m_nBlock = VOID_TOTAL_REFUND_AFTER;
							m_ConsolArray.Consolidate( infoConsol );
							infoConsol.m_nBlock = VOID_TOTAL_REFUND_COMBINED;
							m_ConsolArray.Consolidate( infoConsol );
						}
					}	
				}
				break;
			
			case SS_REASON_DISCOUNT_VOID:
				{
					//CHECK VOID DATE AND TIME
					if ( CheckReasonInfoDateTime( infoFile, ReasonInfo, strThisSaleDate, strThisSaleTime ) == FALSE )
						continue;

					int nServerLocOrDbSort = 0;
					int nServerLocOrDbIdx = 0;
					int nServerNo = ReasonInfo.GetTranServerNo();
					switch( m_nServerReportType )
					{
					case SERVER_REPORT_CODE:
					case SERVER_REPORT_SYS:
						nServerLocOrDbSort = 0;
						nServerLocOrDbIdx = 0;
						break;

					case SERVER_REPORT_NAME:
						nServerLocOrDbSort = 0;
						nServerLocOrDbIdx = DataManagerNonDb.ServerNameTable.GetServerNameIdx( infoFile.m_nLocIdx, nServerNo );
						break;

					case SERVER_REPORT_DB:
						nServerLocOrDbSort = dbDatabase.GetSortIdxFromDbIdx( infoFile.m_nDbIdx );
						nServerLocOrDbIdx = infoFile.m_nDbIdx;
						break;
			
					case SERVER_REPORT_LOC:
						nServerLocOrDbSort = dbLocation.GetSortIdxFromLocIdx( infoFile.m_nLocIdx );
						nServerLocOrDbIdx = infoFile.m_nLocIdx;
						break;
					}

					CString strBeforeAfter = ReasonInfo.GetVoidDiscountBAType();

					CString strReasonText = ReasonInfo.GetReasonText();
					CCSV csvReason( strReasonText, ',', '\'' );

					CString strDiscountType = "";
					switch( ReasonInfo.GetVoidDiscountUsage() )
					{
					case 1:		strDiscountType += "Subtotal";		break;
					case 2:		strDiscountType += "Loyalty";		break;
					case 3:		strDiscountType += "Differ";		break;
					default:	strDiscountType += "??Usage";		break;
					}
	
					CString strType = ReasonInfo.GetVoidDiscountType();
						
					if		( strType == "M" )	strDiscountType += " Markdown";
					else if	( strType == "D" )	strDiscountType += " Discount";
					else if	( strType == "P" )	strDiscountType += " Premium";
					else						strDiscountType += " ??Discount";

					double dDiscountAmount = 0.0;
					{
						int nDiscountAmount = ReasonInfo.GetVoidDiscountAmount();
						nDiscountAmount *= -1;
						dDiscountAmount = ( (double) nDiscountAmount ) / 100.0;
					}

					CString strRate = "";
					if ( strType != "M" )
						strRate.Format ( "%.2f%%", ( (double) ReasonInfo.GetVoidDiscountRate() ) / 100.0 );

					CString strPromo;
					
					int nPromoIdx = 0;
					int nPromoNo = ReasonInfo.GetVoidDiscountPromoNo();
					if ( DataManager.Promotion.FindTableByNumber( nPromoNo, nPromoIdx ) == TRUE )
					{
						CPromotionsCSVRecord Promo;
						DataManager.Promotion.GetAt( nPromoIdx, Promo );
						strPromo = Promo.GetDisplayName();
					}
					else
						strPromo.Format ( "Promo %d", nPromoNo );

					CEposReportConsolVoidReason infoConsol;

					if ( FALSE == m_bSaleTime )
					{
						infoConsol.m_strReportDate = infoFile.m_strDateTran;
						infoConsol.m_strReportTime = ReasonInfo.GetTimeTran();
						infoConsol.m_strOtherTime = ReasonInfo.GetTimeAction();
					}
					else
					{
						infoConsol.m_strReportDate = infoFile.m_strDateSale;
						infoConsol.m_strReportTime = ReasonInfo.GetTimeAction();
						infoConsol.m_strOtherTime = ReasonInfo.GetTimeTran();
					}

					infoConsol.m_bWaste = FALSE;
					infoConsol.m_bGrandTotal = FALSE;
					infoConsol.m_nServerLocOrDbSort = nServerLocOrDbSort;
					infoConsol.m_nServerLocOrDbIdx = nServerLocOrDbIdx;
					infoConsol.m_nServerNo = nServerNo;
					infoConsol.m_nSeqNo = ReasonInfo.GetTranSeqNo();
					infoConsol.m_nTableNo = ReasonInfo.GetTranTableNo();
					infoConsol.m_nLineNo = nLineNo++;
					infoConsol.m_strDiscountType = strDiscountType;
					infoConsol.m_strPromo = strPromo;
					infoConsol.m_strRate = strRate;
					infoConsol.m_dValue = dDiscountAmount;
					infoConsol.m_strReason = csvReason.GetString(0);

					if ( strBeforeAfter != "A" )
					{
						infoConsol.m_nBlock = VOID_SERVER_DISCOUNT_BEFORE_LINE;
						m_ConsolArray.Consolidate( infoConsol );
						infoConsol.m_nBlock = VOID_SERVER_DISCOUNT_BEFORE_TOTAL;
						m_ConsolArray.Consolidate( infoConsol );
						infoConsol.m_nBlock = VOID_SERVER_DISCOUNT_COMBINED_TOTAL;
						m_ConsolArray.Consolidate( infoConsol );

						infoConsol.m_bGrandTotal = TRUE;
						infoConsol.m_nBlock = VOID_TOTAL_DISCOUNT_BEFORE;
						m_ConsolArray.Consolidate( infoConsol );
						infoConsol.m_nBlock = VOID_TOTAL_DISCOUNT_COMBINED;
						m_ConsolArray.Consolidate( infoConsol );
					}
					else
					{
						infoConsol.m_nBlock = VOID_SERVER_DISCOUNT_AFTER_LINE;
						m_ConsolArray.Consolidate( infoConsol );
						infoConsol.m_nBlock = VOID_SERVER_DISCOUNT_AFTER_TOTAL;
						m_ConsolArray.Consolidate( infoConsol );
						infoConsol.m_nBlock = VOID_SERVER_DISCOUNT_COMBINED_TOTAL;
						m_ConsolArray.Consolidate( infoConsol );

						infoConsol.m_bGrandTotal = TRUE;
						infoConsol.m_nBlock = VOID_TOTAL_DISCOUNT_AFTER;
						m_ConsolArray.Consolidate( infoConsol );
						infoConsol.m_nBlock = VOID_TOTAL_DISCOUNT_COMBINED;
						m_ConsolArray.Consolidate( infoConsol );
					}
				}
				break;
			}
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportVoidReason::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );

	if ( TRUE == m_bSaleTime )
	{
		m_ReportFile.AddColumn ( "Date Voided", TA_LEFT, 280 );
		m_ReportFile.AddColumn ( "Seq No", TA_LEFT, 150 );
		m_ReportFile.AddColumn ( "Table", TA_LEFT, 150 );
		m_ReportFile.AddColumn ( "Time Voided", TA_LEFT, 280 );
		m_ReportFile.AddColumn ( "Time Finalised", TA_LEFT, 280 );
	}
	else
	{
		m_ReportFile.AddColumn ( "Date Finalised", TA_LEFT, 280 );
		m_ReportFile.AddColumn ( "Seq No", TA_LEFT, 150 );
		m_ReportFile.AddColumn ( "Table", TA_LEFT, 150 );
		m_ReportFile.AddColumn ( "Time Finalised", TA_LEFT, 280 );
		m_ReportFile.AddColumn ( "Time Voided", TA_LEFT, 280 );
	}

	m_ReportFile.AddColumn ( "Void Type", TA_LEFT, 300 );
	
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
	m_ReportFile.AddColumn ( "Reason", TA_LEFT, 250 );

	CEposReportConsolVoidReason infoLastBlock;
	infoLastBlock.m_nBlock = -1;
	
	int nSize = m_ConsolArray.GetSize();
	StatusProgress.Lock( TRUE, "Creating Report" );

	for ( int i = 0 ; i < nSize ; i++ )
	{
		StatusProgress.SetPos( i, nSize );

		CEposReportConsolVoidReason infoConsol;
		m_ConsolArray.GetAt( i, infoConsol );

		double dLineModQty = infoConsol.m_dModQty;
		double dLinePluQty = infoConsol.m_dPluQty;
		double dLineValue = infoConsol.m_dValue;

		CString strNewBlockType = "";

		switch( infoLastBlock.m_nBlock )
		{
		case VOID_SERVER_SALES_BEFORE_LINE:
		case VOID_SERVER_SALES_AFTER_LINE:
		case VOID_SERVER_REFUND_BEFORE_LINE:
		case VOID_SERVER_REFUND_AFTER_LINE:
		case VOID_SERVER_DISCOUNT_BEFORE_LINE:
		case VOID_SERVER_DISCOUNT_AFTER_LINE:
			break;

		default:
			switch( infoConsol.m_nBlock )
			{
			case VOID_SERVER_SALES_BEFORE_LINE:
			case VOID_SERVER_SALES_AFTER_LINE:
				strNewBlockType = " (Sales)";
				break;

			case VOID_SERVER_REFUND_BEFORE_LINE:
			case VOID_SERVER_REFUND_AFTER_LINE:
				strNewBlockType = " (Refunds)";
				break;
			
			case VOID_SERVER_DISCOUNT_BEFORE_LINE:
			case VOID_SERVER_DISCOUNT_AFTER_LINE:
				strNewBlockType = " (Discounts)";
				break;
			}
			break;
		}

		if ( strNewBlockType != "" )
		{
			CString strHeader = GetServerHeader( infoConsol.m_nServerLocOrDbIdx, infoConsol.m_nServerNo );
			m_ReportFile.RequestBlankLines(2);;
			m_ReportFile.WriteReportMiscLine( strHeader + strNewBlockType  );
			m_ReportFile.WriteReportMiscLine ( "<LI>" );
		}

		CString strGrandTotalType = "";
		switch( infoLastBlock.m_nBlock )
		{
		case VOID_TOTAL_SALES_BEFORE:
		case VOID_TOTAL_REFUND_BEFORE:
		case VOID_TOTAL_DISCOUNT_BEFORE:
			break;
			
		default:
			switch( infoConsol.m_nBlock )
			{
			case VOID_TOTAL_SALES_BEFORE:
			case VOID_TOTAL_SALES_AFTER:
				strGrandTotalType = " (Sales)";
				break;

			case VOID_TOTAL_REFUND_BEFORE:
			case VOID_TOTAL_REFUND_AFTER:
				strGrandTotalType = " (Refunds)";
				break;

			case VOID_TOTAL_DISCOUNT_BEFORE:
			case VOID_TOTAL_DISCOUNT_AFTER:
				strGrandTotalType = " (Discounts)";
				break;
			}
		}

		if ( strGrandTotalType != "" )
		{
			CString strText = "Grand Totals";
			strText += strGrandTotalType;

			m_ReportFile.RequestBlankLines(1);
			m_ReportFile.WriteReportMiscLine( strText );
			m_ReportFile.WriteReportMiscLine ( "<LI>" );
		}

		bool bIsTotal = FALSE;
		int nPreviousTotal = 0;
		bool bLineBeforePreviousTotal = FALSE;
		bool bLineBeforeCurrentTotal = FALSE;
		bool bSpaceAfterCurrentTotal = FALSE;

		switch( infoConsol.m_nBlock )
		{
		case VOID_SERVER_SALES_BEFORE_LINE:
		case VOID_SERVER_SALES_AFTER_LINE:
		case VOID_SERVER_REFUND_BEFORE_LINE:
		case VOID_SERVER_REFUND_AFTER_LINE:
			{
				CString strReportDate;
				strReportDate.Format ( "%s/%s/%s",
					(const char*) infoConsol.m_strReportDate.Right(2),
					(const char*) infoConsol.m_strReportDate.Mid(4,2),
					(const char*) infoConsol.m_strReportDate.Left(4) );

				CString strReportTime;
				strReportTime.Format ( "%s:%s:%s",
					(const char*) infoConsol.m_strReportTime.Left(2),
					(const char*) infoConsol.m_strReportTime.Mid(2,2),
					(const char*) infoConsol.m_strReportTime.Right(2) );

				CString strOtherTime;
				strOtherTime.Format ( "%s:%s:%s",
					(const char*) infoConsol.m_strOtherTime.Left(2),
					(const char*) infoConsol.m_strOtherTime.Mid(2,2),
					(const char*) infoConsol.m_strOtherTime.Right(2) );

				CString strPluNo = ReportHelpers.GetDisplayPluNo( infoConsol.m_nPluNo, TRUE );

				if ( infoConsol.m_nModifier != 0 )
				{
					CString strExtra;
					strExtra.Format( " + %d", infoConsol.m_nModifier );
					strPluNo += strExtra;
				}

				CString strType = "";
				switch( infoConsol.m_nBlock )
				{
				case VOID_SERVER_SALES_BEFORE_LINE:
				case VOID_SERVER_REFUND_BEFORE_LINE:
					strType = "Before";
					break;

				case VOID_SERVER_SALES_AFTER_LINE:
				case VOID_SERVER_REFUND_AFTER_LINE:
					strType = "After";

					if ( TRUE == infoConsol.m_bWaste )
						strType += " (Waste)";

					break;
				}

				CString strTable = "";
				if ( infoConsol.m_nTableNo != 0 )
					strTable.Format( "%d", infoConsol.m_nTableNo );

				CString strSeqNo;
				strSeqNo.Format( "%6.6d", infoConsol.m_nSeqNo );
				
				CCSV csv ( '\t' );
				csv.Add( strReportDate );
				csv.Add( strSeqNo );
				csv.Add( strTable );
				csv.Add( strReportTime );
				csv.Add( strOtherTime );
				csv.Add( strType );
				csv.Add( strPluNo );
				csv.Add( infoConsol.m_strPluText );
				csv.Add( infoConsol.m_nDeptNo );
				
				if (SysInfo.GetMaxBasePluLen() != 0)
				{
					csv.Add(dLineModQty, SysInfo.GetDPQty(), TRUE);
				}

				csv.Add ( dLinePluQty, SysInfo.GetDPQty() );
				csv.Add ( dLineValue, SysInfo.GetDPValue() );
				csv.Add ( infoConsol.m_strReason );

				if ( VOID_SERVER_SALES_AFTER_LINE == infoConsol.m_nBlock )
				{
					if (VOID_SERVER_SALES_BEFORE_LINE == infoLastBlock.m_nBlock)
					{
						m_ReportFile.WriteReportMiscLine("<LI>");
					}
				}
				else if ( VOID_SERVER_REFUND_AFTER_LINE == infoConsol.m_nBlock )
				{
					if (VOID_SERVER_REFUND_BEFORE_LINE == infoLastBlock.m_nBlock)
					{
						m_ReportFile.WriteReportMiscLine("<LI>");
					}
				}

				m_ReportFile.WriteReportDataLine( csv.GetLine() );
			}
			break;

		case VOID_SERVER_DISCOUNT_BEFORE_LINE:
		case VOID_SERVER_DISCOUNT_AFTER_LINE:
			{
				CString strReportDate;
				strReportDate.Format ( "%s/%s/%s",
					(const char*) infoConsol.m_strReportDate.Right(2),
					(const char*) infoConsol.m_strReportDate.Mid(4,2),
					(const char*) infoConsol.m_strReportDate.Left(4) );

				CString strReportTime;
				strReportTime.Format ( "%s:%s:%s",
					(const char*) infoConsol.m_strReportTime.Left(2),
					(const char*) infoConsol.m_strReportTime.Mid(2,2),
					(const char*) infoConsol.m_strReportTime.Right(2) );

				CString strOtherTime;
				strOtherTime.Format ( "%s:%s:%s",
					(const char*) infoConsol.m_strOtherTime.Left(2),
					(const char*) infoConsol.m_strOtherTime.Mid(2,2),
					(const char*) infoConsol.m_strOtherTime.Right(2) );

				CString strPluNo = ReportHelpers.GetDisplayPluNo( infoConsol.m_nPluNo, TRUE );

				if ( infoConsol.m_nModifier != 0 )
				{
					CString strExtra;
					strExtra.Format( " + %d", infoConsol.m_nModifier );
					strPluNo += strExtra;
				}

				CString strType = "";
				if (VOID_SERVER_DISCOUNT_BEFORE_LINE == infoConsol.m_nBlock)
				{
					strType = "Before";
				}
				else
				{
					strType = "After";
				}

				CString strTable = "";
				if (infoConsol.m_nTableNo != 0)
				{
					strTable.Format("%d", infoConsol.m_nTableNo);
				}

				CString strSeqNo;
				strSeqNo.Format( "%6.6d", infoConsol.m_nSeqNo );
				
				CCSV csv ( '\t' );
				csv.Add( strReportDate );
				csv.Add( strSeqNo );
				csv.Add( strTable );
				csv.Add( strReportTime );
				csv.Add( strOtherTime );
				csv.Add( strType );
				csv.Add( infoConsol.m_strDiscountType );
				csv.Add( infoConsol.m_strPromo );
				
				if (SysInfo.GetMaxBasePluLen() != 0)
				{
					csv.Add("");
				}
				
				csv.Add ( "" );
				csv.Add ( infoConsol.m_strRate );
				csv.Add ( dLineValue, SysInfo.GetDPValue() );
				csv.Add ( infoConsol.m_strReason );

				if (VOID_SERVER_DISCOUNT_AFTER_LINE == infoConsol.m_nBlock)
				{
					if (VOID_SERVER_DISCOUNT_BEFORE_LINE == infoLastBlock.m_nBlock)
					{
						m_ReportFile.WriteReportMiscLine("<LI>");
					}
				}

				m_ReportFile.WriteReportDataLine( csv.GetLine() );
			}
			break;

		case VOID_SERVER_SALES_BEFORE_TOTAL:
		case VOID_SERVER_REFUND_BEFORE_TOTAL:
		case VOID_SERVER_DISCOUNT_BEFORE_TOTAL:
			bIsTotal = TRUE;
			bLineBeforeCurrentTotal = TRUE;
			break;

		case VOID_SERVER_SALES_AFTER_TOTAL:
			bIsTotal = TRUE;
			nPreviousTotal = VOID_SERVER_SALES_BEFORE_TOTAL;
			bLineBeforePreviousTotal = TRUE;
			break;

		case VOID_SERVER_REFUND_AFTER_TOTAL:
			bIsTotal = TRUE;
			nPreviousTotal = VOID_SERVER_REFUND_BEFORE_TOTAL;
			bLineBeforePreviousTotal = TRUE;
			break;

		case VOID_SERVER_DISCOUNT_AFTER_TOTAL:
			bIsTotal = TRUE;
			nPreviousTotal = VOID_SERVER_DISCOUNT_BEFORE_TOTAL;
			bLineBeforePreviousTotal = TRUE;
			break;

		case VOID_SERVER_SALES_COMBINED_TOTAL:
			bIsTotal = TRUE;
			nPreviousTotal = VOID_SERVER_SALES_AFTER_TOTAL;
			bLineBeforeCurrentTotal = TRUE;
			bSpaceAfterCurrentTotal = TRUE;
			break;

		case VOID_SERVER_REFUND_COMBINED_TOTAL:
			bIsTotal = TRUE;
			nPreviousTotal = VOID_SERVER_REFUND_AFTER_TOTAL;
			bLineBeforeCurrentTotal = TRUE;
			bSpaceAfterCurrentTotal = TRUE;
			break;

		case VOID_SERVER_DISCOUNT_COMBINED_TOTAL:
			bIsTotal = TRUE;
			nPreviousTotal = VOID_SERVER_DISCOUNT_AFTER_TOTAL;
			bLineBeforeCurrentTotal = TRUE;
			bSpaceAfterCurrentTotal = TRUE;
			break;

		case VOID_TOTAL_SALES_BEFORE:
		case VOID_TOTAL_REFUND_BEFORE:
		case VOID_TOTAL_DISCOUNT_BEFORE:
			bIsTotal = TRUE;
			break;

		case VOID_TOTAL_SALES_AFTER:
			bIsTotal = TRUE;
			nPreviousTotal = VOID_TOTAL_SALES_BEFORE;
			break;

		case VOID_TOTAL_REFUND_AFTER:
			bIsTotal = TRUE;
			nPreviousTotal = VOID_TOTAL_REFUND_BEFORE;
			break;

		case VOID_TOTAL_DISCOUNT_AFTER:
			bIsTotal = TRUE;
			nPreviousTotal = VOID_TOTAL_DISCOUNT_BEFORE;
			break;

		case VOID_TOTAL_SALES_COMBINED:
			bIsTotal = TRUE;
			nPreviousTotal = VOID_TOTAL_SALES_AFTER ;
			bLineBeforeCurrentTotal = TRUE;
			bSpaceAfterCurrentTotal = TRUE;
			break;

		case VOID_TOTAL_REFUND_COMBINED:
			bIsTotal = TRUE;
			nPreviousTotal = VOID_TOTAL_REFUND_AFTER ;
			bLineBeforeCurrentTotal = TRUE;
			bSpaceAfterCurrentTotal = TRUE;
			break;

		case VOID_TOTAL_DISCOUNT_COMBINED:
			bIsTotal = TRUE;
			nPreviousTotal = VOID_TOTAL_DISCOUNT_AFTER ;
			bLineBeforeCurrentTotal = TRUE;
			bSpaceAfterCurrentTotal = TRUE;
			break;
		}

		if ( TRUE == bIsTotal )
		{
			if ( ( nPreviousTotal != 0 ) && ( nPreviousTotal != infoLastBlock.m_nBlock ) )
			{
				if ( TRUE == bLineBeforePreviousTotal )
					WriteTotalUnderline( nPreviousTotal );

				WriteDummyTotal( nPreviousTotal );
			}

			if ( TRUE == bLineBeforeCurrentTotal )
				WriteTotalUnderline( infoConsol.m_nBlock );

			WriteTotal( infoConsol );

			if ( TRUE == bSpaceAfterCurrentTotal )
				m_ReportFile.RequestBlankLines(2);
		}

		infoLastBlock = infoConsol;
	}

	StatusProgress.Unlock();

	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/

void CEposReportVoidReason::WriteTotalUnderline( int nType )
{
	CString strUnderline = "";

	switch( nType )
	{
	case VOID_SERVER_SALES_BEFORE_TOTAL:
	case VOID_SERVER_SALES_AFTER_TOTAL:
	case VOID_SERVER_SALES_COMBINED_TOTAL:
	case VOID_TOTAL_SALES_BEFORE:
	case VOID_TOTAL_SALES_AFTER:
	case VOID_TOTAL_SALES_COMBINED:
	/*****/
	case VOID_SERVER_REFUND_BEFORE_TOTAL:
	case VOID_SERVER_REFUND_AFTER_TOTAL:
	case VOID_SERVER_REFUND_COMBINED_TOTAL:
	case VOID_TOTAL_REFUND_BEFORE:
	case VOID_TOTAL_REFUND_AFTER:
	case VOID_TOTAL_REFUND_COMBINED:
		if (SysInfo.GetMaxBasePluLen() != 0)
		{
			strUnderline = "\t\t\t\t\t\t\t\t\t<LI>\t<LI>\t<LI>";
		}
		else
		{
			strUnderline = "\t\t\t\t\t\t\t\t\t<LI>\t<LI>";
		}
		break;

	case VOID_SERVER_DISCOUNT_BEFORE_TOTAL:
	case VOID_SERVER_DISCOUNT_AFTER_TOTAL:
	case VOID_SERVER_DISCOUNT_COMBINED_TOTAL:
	case VOID_TOTAL_DISCOUNT_BEFORE:
	case VOID_TOTAL_DISCOUNT_AFTER:
	case VOID_TOTAL_DISCOUNT_COMBINED:
		if (SysInfo.GetMaxBasePluLen() != 0)
		{
			strUnderline = "\t\t\t\t\t\t\t\t\t\t\t<LI>";
		}
		else
		{
			strUnderline = "\t\t\t\t\t\t\t\t\t\t<LI>";
		}
		break;
	}

	m_ReportFile.WriteReportMiscLine( strUnderline );
}

/**********************************************************************/

void CEposReportVoidReason::WriteDummyTotal( int nType )
{
	CEposReportConsolVoidReason Total;
	Total.m_nBlock = nType;
	Total.m_dModQty = 0.0;
	Total.m_dPluQty = 0.0;
	Total.m_dValue = 0.0;
	WriteTotal( Total );
}

/**********************************************************************/

void CEposReportVoidReason::WriteTotal( CEposReportConsolVoidReason& block )
{
	CString strUnderline = "";

	CString strType = "<..>";

	switch( block.m_nBlock )
	{
	case VOID_TOTAL_SALES_BEFORE:
	case VOID_SERVER_SALES_BEFORE_TOTAL:		strType += "Sales : Before";			break;
	case VOID_TOTAL_SALES_AFTER:
	case VOID_SERVER_SALES_AFTER_TOTAL:			strType += "Sales : After";				break;
	case VOID_TOTAL_SALES_COMBINED:
	case VOID_SERVER_SALES_COMBINED_TOTAL:		strType += "Sales : Total";				break;
	/*****/
	case VOID_TOTAL_REFUND_BEFORE:
	case VOID_SERVER_REFUND_BEFORE_TOTAL:		strType += "Refund : Before";			break;
	case VOID_TOTAL_REFUND_AFTER:
	case VOID_SERVER_REFUND_AFTER_TOTAL:		strType += "Refund : After";			break;
	case VOID_TOTAL_REFUND_COMBINED:
	case VOID_SERVER_REFUND_COMBINED_TOTAL:		strType += "Refund : Total";			break;
	/*****/
	case VOID_TOTAL_DISCOUNT_BEFORE:
	case VOID_SERVER_DISCOUNT_BEFORE_TOTAL:		strType += "Discount : Before";			break;
	case VOID_TOTAL_DISCOUNT_AFTER:
	case VOID_SERVER_DISCOUNT_AFTER_TOTAL:		strType += "Discount : After";			break;
	case VOID_TOTAL_DISCOUNT_COMBINED:
	case VOID_SERVER_DISCOUNT_COMBINED_TOTAL:	strType += "Discount : Total";			break;
	}

	CCSV csv ( '\t' );
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( "");
	csv.Add ( "" );
	csv.Add ( strType );
	csv.Add ( "" );

	switch( block.m_nBlock )
	{
	case VOID_SERVER_SALES_BEFORE_TOTAL:
	case VOID_SERVER_SALES_AFTER_TOTAL:
	case VOID_SERVER_SALES_COMBINED_TOTAL:
	case VOID_TOTAL_SALES_BEFORE:
	case VOID_TOTAL_SALES_AFTER:
	case VOID_TOTAL_SALES_COMBINED:
	/*****/
	case VOID_SERVER_REFUND_BEFORE_TOTAL:
	case VOID_SERVER_REFUND_AFTER_TOTAL:
	case VOID_SERVER_REFUND_COMBINED_TOTAL:
	case VOID_TOTAL_REFUND_BEFORE:
	case VOID_TOTAL_REFUND_AFTER:
	case VOID_TOTAL_REFUND_COMBINED:
		if (SysInfo.GetMaxBasePluLen() != 0)
		{
			csv.Add(block.m_dModQty, SysInfo.GetDPQty(), TRUE);
		}
		csv.Add ( block.m_dPluQty, SysInfo.GetDPQty() );
		csv.Add ( block.m_dValue, SysInfo.GetDPValue() );
		break;

	case VOID_SERVER_DISCOUNT_BEFORE_TOTAL:
	case VOID_SERVER_DISCOUNT_AFTER_TOTAL:
	case VOID_SERVER_DISCOUNT_COMBINED_TOTAL:
	case VOID_TOTAL_DISCOUNT_BEFORE:
	case VOID_TOTAL_DISCOUNT_AFTER:
	case VOID_TOTAL_DISCOUNT_COMBINED:
		if (SysInfo.GetMaxBasePluLen() != 0)
		{
			csv.Add("");
		}
		
		csv.Add ( "" );
		csv.Add ( block.m_dValue, SysInfo.GetDPValue() );
		break;
	}

	m_ReportFile.WriteReportMiscLine ( csv.GetLine() );
}

/**********************************************************************/