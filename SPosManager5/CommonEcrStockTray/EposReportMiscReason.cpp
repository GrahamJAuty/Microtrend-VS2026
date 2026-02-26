/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportMiscReason.h"
/**********************************************************************/

CEposReportMiscReason::CEposReportMiscReason( int nReportType, CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_strEventDate = "";
	m_strEventTime = "";

	switch( nReportType )
	{
	case REPORT_TYPE_TRANSERVER_VIP_REASON:
		m_nReportType = REPORT_TYPE_TRANSERVER_VIP_REASON;
		m_nReasonType = SS_REASON_VIP;
		m_strEventDate = "Sale Date";
		m_strEventTime = "Sale Time";
		break;

	case REPORT_TYPE_TRANSERVER_WASTAGE_REASON:
		m_nReportType = REPORT_TYPE_TRANSERVER_WASTAGE_REASON;
		m_nReasonType = SS_REASON_WASTAGE;
		m_strEventDate = "Wastage Date";
		m_strEventTime = "Wastage Time";
		break;

	case REPORT_TYPE_TRANSERVER_REFUND_REASON:
	default:
		m_nReportType = REPORT_TYPE_TRANSERVER_REFUND_REASON;
		m_nReasonType = SS_REASON_REFUNDVOID;
		m_strEventDate = "Date Refunded";
		m_strEventTime = "Time Refunded";
		break;
	}
		
	SetPMSAllowMode( PMS_ALLOWMODE_NONE );
}

/**********************************************************************/

void CEposReportMiscReason::Consolidate()
{
	if ( TRUE == m_bItemServer )
	{
		switch( m_nReportType )
		{
		case REPORT_TYPE_TRANSERVER_VIP_REASON:			
			m_nReportType = REPORT_TYPE_ITEMSERVER_VIP_REASON;	
			break;

		case REPORT_TYPE_TRANSERVER_WASTAGE_REASON:		
			m_nReportType = REPORT_TYPE_ITEMSERVER_WASTAGE_REASON;
			break;

		case REPORT_TYPE_TRANSERVER_REFUND_REASON:
		default:
			m_nReportType = REPORT_TYPE_ITEMSERVER_REFUND_REASON;
			break;
		}
	}

	int nLineNo = 0;

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_REASONS_NEW, m_SelectArray, FALSE, nTotalFileSize );

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

		CCSV csvRefund( "" );

		bool bLegacy = FALSE;
		
		if ( SS_REASON_REFUNDVOID == m_nReasonType )
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
			
			csvRefund.ParseLine( strBuffer );
			CReasonInfo ReasonInfo;
			ReasonInfo.LoadCSVLine( csvRefund );

			//CHECK REFUND DATE AND TIME
			if ( CheckReasonInfoDateTime( infoFile, ReasonInfo, strThisSaleDate, strThisSaleTime ) == FALSE )
				continue;

			//IGNORE VOID DISCOUNT LINES
			if ( ReasonInfo.GetReasonType() != m_nReasonType )
				continue;

			switch( m_nReportType )
			{
			case REPORT_TYPE_TRANSERVER_REFUND_REASON:
			case REPORT_TYPE_ITEMSERVER_REFUND_REASON:
				if ( ReasonInfo.GetItemRefundFlag() == FALSE )
					continue;

				if ( ReasonInfo.GetItemVoidFlag() == TRUE )
					continue;
			}

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
					
			if ( infoPluNo.m_nModifier != 0 )
				dMultiplier = DataManager.Modifier.GetMultiplierForQty( infoPluNo.m_nModifier );
			
			m_PluInfoFinder.SetPluNo( infoPluNo.m_nBasePluNo );
			
			//QTY AND VALUE
			double dQty = ReasonInfo.GetItemQty();
			double dLineValue = ReasonInfo.GetItemVal();

			if ( SS_REASON_REFUNDVOID == m_nReasonType )
			{
				dQty *= -1;
				dLineValue *= -1;
			}
			
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

			CEposReportConsolMiscReason infoConsol;
			
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
			infoConsol.m_nSeqNo = ReasonInfo.GetTranSeqNo();
			infoConsol.m_nTableNo = ReasonInfo.GetTranTableNo();
			infoConsol.m_nLineNo = nLineNo++;
			infoConsol.m_nPluNo = infoPluNo.m_nBasePluNo;
			infoConsol.m_nModifier = infoPluNo.m_nModifier;
			infoConsol.m_nDeptNo = nDeptNo;
			infoConsol.m_strPluText = m_PluInfoFinder.GetPluInfoRepText( TRUE, infoPluNo.m_nModifier, TRUE );
			infoConsol.m_dModQty = dQty;
			infoConsol.m_dPluQty = dQty * dMultiplier;
			infoConsol.m_dValue = dLineValue;
			infoConsol.m_strReason = csvReason.GetString(0);

			infoConsol.m_nBlock = MISCREASON_SERVER_SALES_LINE;
			m_ConsolArray.Consolidate( infoConsol );
			infoConsol.m_nBlock = MISCREASON_SERVER_SALES_TOTAL;
			m_ConsolArray.Consolidate( infoConsol );
					
			infoConsol.m_bGrandTotal = TRUE;
			infoConsol.m_nBlock = MISCREASON_TOTAL_SALES;
			m_ConsolArray.Consolidate( infoConsol );					
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportMiscReason::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );

	if ( TRUE == m_bSaleTime )
	{
		m_ReportFile.AddColumn ( m_strEventDate, TA_LEFT, 280 );
		m_ReportFile.AddColumn ( "Seq No", TA_LEFT, 150 );
		m_ReportFile.AddColumn ( "Table", TA_LEFT, 150 );
		m_ReportFile.AddColumn ( m_strEventTime, TA_LEFT, 280 );
		m_ReportFile.AddColumn ( "Time Finalised", TA_LEFT, 280 );
	}
	else
	{
		m_ReportFile.AddColumn ( "Date Finalised", TA_LEFT, 280 );
		m_ReportFile.AddColumn ( "Seq No", TA_LEFT, 150 );
		m_ReportFile.AddColumn ( "Table", TA_LEFT, 150 );
		m_ReportFile.AddColumn ( "Time Finalised", TA_LEFT, 280 );
		m_ReportFile.AddColumn ( m_strEventTime, TA_LEFT, 280 );
	}
	
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

	CEposReportConsolMiscReason infoLastBlock;
	infoLastBlock.m_nBlock = -1;
	
	int nSize = m_ConsolArray.GetSize();
	StatusProgress.Lock( TRUE, "Creating Report" );

	for ( int i = 0 ; i < nSize ; i++ )
	{
		StatusProgress.SetPos( i, nSize );

		CEposReportConsolMiscReason infoConsol;
		m_ConsolArray.GetAt( i, infoConsol );

		double dLineModQty = infoConsol.m_dModQty;
		double dLinePluQty = infoConsol.m_dPluQty;
		double dLineValue = infoConsol.m_dValue;

		switch( infoConsol.m_nBlock )
		{
		case MISCREASON_SERVER_SALES_LINE:
			switch( infoLastBlock.m_nBlock )
			{
				case MISCREASON_SERVER_SALES_LINE:
					break;

				default:
					m_ReportFile.RequestBlankLines(2);
					m_ReportFile.WriteReportMiscLine( GetServerHeader( infoConsol.m_nServerLocOrDbIdx, infoConsol.m_nServerNo ) );
					m_ReportFile.WriteReportMiscLine ( "<LI>" );
			}
			break;
		}

		switch( infoConsol.m_nBlock )
		{
		case MISCREASON_SERVER_SALES_LINE:
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

				m_ReportFile.WriteReportDataLine( csv.GetLine() );
			}
			break;

		case MISCREASON_SERVER_SALES_TOTAL:
		case MISCREASON_TOTAL_SALES:
			WriteTotalUnderline();
			WriteTotal( infoConsol );
			break;
		}

		infoLastBlock = infoConsol;
	}

	StatusProgress.Unlock();

	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/

void CEposReportMiscReason::WriteTotalUnderline()
{
	if (SysInfo.GetMaxBasePluLen() != 0)
	{
		m_ReportFile.WriteReportMiscLine("\t\t\t\t\t\t\t\t<LI>\t<LI>\t<LI>");
	}
	else
	{
		m_ReportFile.WriteReportMiscLine("\t\t\t\t\t\t\t\t<LI>\t<LI>");
	}
}

/**********************************************************************/

void CEposReportMiscReason::WriteTotal( CEposReportConsolMiscReason& block )
{
	CString strUnderline = "";

	CString strType = "<..>";

	switch( block.m_nBlock )
	{
	case MISCREASON_SERVER_SALES_TOTAL:		strType += "Server Total";		break;
	case MISCREASON_TOTAL_SALES:			strType += "Grand Total";		break;
	}

	CCSV csv ( '\t' );
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( "");
	csv.Add ( "" );
	csv.Add ( strType );
	csv.Add ( "" );

	if (SysInfo.GetMaxBasePluLen() != 0)
	{
		csv.Add(block.m_dModQty, SysInfo.GetDPQty(), TRUE);
	}

	csv.Add ( block.m_dPluQty, SysInfo.GetDPQty() );
	csv.Add ( block.m_dValue, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportMiscLine ( csv.GetLine() );
}

/**********************************************************************/