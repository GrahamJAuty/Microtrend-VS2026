/**********************************************************************/
#include "CashRSPImporterDiscountHelpers.h"
#include "DatabaseCSVArray.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "PMSOptions.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportDiscountReason.h"
/**********************************************************************/

CEposReportDiscountReason::CEposReportDiscountReason( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_TRANSERVER_DISCOUNT_REASON;
}

/**********************************************************************/

void CEposReportDiscountReason::Consolidate()
{
	m_nReportType = ( m_bItemServer ) ? REPORT_TYPE_ITEMSERVER_DISCOUNT_REASON : REPORT_TYPE_TRANSERVER_DISCOUNT_REASON;

	int nLastDbIdx = -1;

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_REASONS_NEW, m_SelectArray, FALSE, nTotalFileSize );
	
	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );

	CCSV csvDiscount;
	m_nLineNo = 0;
	
	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		CString strThisSaleDate = "";
		CString strDummy = "";
		m_PMSModes.GetInitialSaleDateTime( FALSE, infoFile, m_bSaleTime, strThisSaleDate, strDummy );

		if ( infoFile.m_nDbIdx != nLastDbIdx )
		{
			CDataManagerInfo info;
			DataManager.OpenDatabaseReadOnly( infoFile.m_nDbIdx, info, FALSE );
			nLastDbIdx = infoFile.m_nDbIdx;
		}

		CSSFile fileReasons;
		if ( fileReasons.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		CString strBuffer = "";
		while ( fileReasons.ReadString( strBuffer ) == TRUE )
		{
			StatusProgress.SetPos ( nProgress + fileReasons.Tell(), nTotalFileSize );
			
			CReasonInfo ReasonInfo;
			csvDiscount.ParseLine( strBuffer );
			ReasonInfo.LoadCSVLine( csvDiscount );

			switch( ReasonInfo.GetReasonType() )
			{
			case SS_REASON_DISCOUNT_ITEM:	ConsolidateItemDiscount( infoFile, strThisSaleDate, ReasonInfo );			break;
			case SS_REASON_DISCOUNT_SUBT:	ConsolidateSubtotalDiscount( infoFile, strThisSaleDate, ReasonInfo );		break;
			}
		}

		nProgress += fileReasons.GetLength();
		fileReasons.Close();
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportDiscountReason::CheckServer( CTermFileListInfo& infoFile, CEposReportConsolDiscount& infoConsol, int nServerNo )
{
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
		infoConsol.m_nServerLocOrDbSort = dbDatabase.GetSortIdxFromDbIdx( infoFile.m_nDbIdx );
		infoConsol.m_nServerLocOrDbIdx = infoFile.m_nDbIdx;
		break;
					
	case SERVER_REPORT_LOC:
		infoConsol.m_nServerLocOrDbSort = dbLocation.GetSortIdxFromLocIdx( infoFile.m_nLocIdx );	
		infoConsol.m_nServerLocOrDbIdx = infoFile.m_nLocIdx;
		break;
	}

	infoConsol.m_nServerNo = nServerNo;
	
	return TRUE;
}

/**********************************************************************/

bool CEposReportDiscountReason::ConsolidateSubtotalDiscount( CTermFileListInfo& infoFile, CString& strThisSaleDate, CReasonInfo& ReasonInfo )
{
	CString strThisSaleTime = "";
	if ( CheckReasonInfoDateTime( infoFile, ReasonInfo, strThisSaleDate, strThisSaleTime ) == FALSE )
		return FALSE;

	int nServerNo = ReasonInfo.GetTranServerNo();
	
	CEposReportConsolDiscount infoConsol;
	if ( CheckServer( infoFile, infoConsol, nServerNo ) == FALSE )
		return FALSE;

	CString strDiscType = ReasonInfo.GetSDiscType();
		
	int nDiscType = -1;
	
	if ( strDiscType == "M" )		nDiscType = DISCOUNT_TYPE_MARKDOWN;
	else if ( strDiscType == "D" )	nDiscType = DISCOUNT_TYPE_DISCOUNT;
	else if ( strDiscType == "P" )	nDiscType = DISCOUNT_TYPE_PREMIUM;
	else if ( strDiscType == "?" )	nDiscType = DISCOUNT_TYPE_UNKNOWN;
	
	if ( nDiscType == -1 )
		return FALSE;

	int nPromo = ReasonInfo.GetSDiscPromoNo();
	int nAmount = ReasonInfo.GetSDiscAmount();
	int nRate = ReasonInfo.GetSDiscRate();
	if ( nDiscType != DISCOUNT_TYPE_PREMIUM ) nAmount *= -1;

	infoConsol.m_nDbIdx = infoFile.m_nDbIdx;
	infoConsol.m_strDate = strThisSaleDate;
	infoConsol.m_strTime = strThisSaleTime;
	infoConsol.m_nLineType = 0;
	infoConsol.m_nDiscountType = nDiscType;
	infoConsol.m_nDiscountMode = DISCOUNT_MODE_SUBTOTAL;
	infoConsol.m_nPromoNo = nPromo;
	infoConsol.m_nPluNo = 0;
	infoConsol.m_nLineNo = m_nLineNo++;
	infoConsol.m_strText = "";
	infoConsol.m_nAmount = nAmount;
	infoConsol.m_nRate = nRate;
	infoConsol.m_strReason = ReasonInfo.GetReasonText();
	m_ConsolArray.Consolidate( infoConsol );

	CEposReportConsolDiscount infoTotal = infoConsol;
	infoConsol.m_nLineType = 1;
	infoConsol.m_strDate = "";
	infoConsol.m_strTime = "";
	infoConsol.m_nLineNo = 0;
	m_ConsolArray.Consolidate( infoTotal );

	return TRUE;
}

/**********************************************************************/

bool CEposReportDiscountReason::ConsolidateItemDiscount( CTermFileListInfo& infoFile, CString& strThisSaleDate, CReasonInfo& ReasonInfo )
{		
	CString strThisSaleTime = "";
	if ( CheckReasonInfoDateTime( infoFile, ReasonInfo, strThisSaleDate, strThisSaleTime ) == FALSE )
		return FALSE;

	int nServerNo = ( m_bItemServer ) ? ReasonInfo.GetItemServerNo() : ReasonInfo.GetTranServerNo();
	
	CEposReportConsolDiscount infoConsol;
	if ( CheckServer( infoFile, infoConsol, nServerNo ) == FALSE )
		return FALSE;

	CSQLPluNoInfo infoPluNo;
	infoPluNo.m_nEposPluNo = ReasonInfo.GetIDiscPluNo();
	::ProcessPluNo( infoPluNo );
	if ( FALSE == infoPluNo.m_bValid )
		return FALSE;

	CString strDiscType = ReasonInfo.GetIDiscType();

	int nDiscType = -1;
	
	if ( strDiscType == "M" )		nDiscType = DISCOUNT_TYPE_MARKDOWN;
	else if ( strDiscType == "D" )	nDiscType = DISCOUNT_TYPE_DISCOUNT;
	else if ( strDiscType == "P" )	nDiscType = DISCOUNT_TYPE_PREMIUM;
	
	if ( nDiscType == -1 )
		return FALSE;
	
	CString strDescription = "**Unknown";

	int nPluIdx = 0;
	if ( DataManager.Plu.FindPluByNumber( infoPluNo.m_nBasePluNo, nPluIdx ) == TRUE )
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( nPluIdx, PluRecord );
		strDescription = PluRecord.GetModifierReportText( infoPluNo.m_nModifier );
	}

	int nAmount = ReasonInfo.GetIDiscAmount();
	int nRate = ReasonInfo.GetIDiscRate();
	if ( nDiscType == DISCOUNT_TYPE_PREMIUM ) nAmount *= -1;
	
	infoConsol.m_nDbIdx = infoFile.m_nDbIdx;
	infoConsol.m_strDate = strThisSaleDate;
	infoConsol.m_strTime = strThisSaleTime;
	infoConsol.m_nLineType = 0;
	infoConsol.m_nDiscountType = nDiscType;
	infoConsol.m_nDiscountMode = DISCOUNT_MODE_ITEM;
	infoConsol.m_nPromoNo = ReasonInfo.GetIDiscPromoNo();
	infoConsol.m_nPluNo = infoPluNo.m_nEposPluNo;
	infoConsol.m_nLineNo = m_nLineNo++;
	infoConsol.m_strText = strDescription;
	infoConsol.m_nAmount = nAmount;
	infoConsol.m_nRate = nRate;
	infoConsol.m_strReason = ReasonInfo.GetReasonText();
	m_ConsolArray.Consolidate( infoConsol );

	CEposReportConsolDiscount infoTotal = infoConsol;
	infoTotal.m_nLineType = 1;
	infoTotal.m_strDate = "";
	infoTotal.m_strTime = "";
	infoTotal.m_nPluNo = 0;
	infoTotal.m_nLineNo = 0;
	infoTotal.m_strText = "";
	m_ConsolArray.Consolidate( infoTotal );
		
	return TRUE;
}

/***********************************************************************/

bool CEposReportDiscountReason::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Date", TA_LEFT, 250 );
	m_ReportFile.AddColumn ( "Time", TA_LEFT, 200 );
	m_ReportFile.AddColumn ( "Type", TA_LEFT, 400 );
	m_ReportFile.AddColumn ( "Promo", TA_LEFT, 600 );
	m_ReportFile.AddColumn ( "Amount", TA_RIGHT, 250 );
	m_ReportFile.AddColumn ( "Item", TA_LEFT, 600 );
	m_ReportFile.AddColumn ( "Reason", TA_LEFT, 600 );

	int nServerDiscount = 0;
	int nServerPremium = 0;
	int nServerMarkdown = 0;
	int nServerUnknown = 0;
	int nTotalDiscount = 0;
	int nTotalPremium = 0;
	int nTotalMarkdown = 0;
	int nTotalUnknown = 0;
	bool bServerTotalSection = FALSE;

	CEposReportConsolDiscount CurrentServer;
	CurrentServer.m_nServerLocOrDbSort = -1;
	CurrentServer.m_nServerLocOrDbIdx = -1;
	
	bool bNeedTotal = FALSE;

	int nSize = m_ConsolArray.GetSize();
	StatusProgress.Lock( TRUE, "Creating Report" );

	int nCurrentDbIdx = -1;

	for ( int i = 0 ; i < nSize ; i++ )
	{
		StatusProgress.SetPos( i, nSize );

		CEposReportConsolDiscount infoConsol;
		m_ConsolArray.GetAt( i, infoConsol );

		if ( 1 == infoConsol.m_nLineType )
		{
			if ( FALSE == bServerTotalSection )
			{
				bServerTotalSection = TRUE;
				m_ReportFile.WriteReportMiscLine ( "\t\t\t\t<LI>" );
			}
		}
		else
			bServerTotalSection = FALSE;

		if ( infoConsol.m_nDbIdx != nCurrentDbIdx )
		{
			CDataManagerInfo infoDb;
			DataManager.OpenDatabaseReadOnly( infoConsol.m_nDbIdx, infoDb, FALSE );
			nCurrentDbIdx = infoConsol.m_nDbIdx;
		}
		
		CString strLineDate = "";
		CString strLineTime = "";

		if ( FALSE == bServerTotalSection )
		{
			strLineDate.Format ( "%s/%s/%s",
				(const char*) infoConsol.m_strDate.Right(2),
				(const char*) infoConsol.m_strDate.Mid(4,2),
				(const char*) infoConsol.m_strDate.Left(4) );

			strLineTime.Format ( "%s:%s:%s",
				(const char*) infoConsol.m_strTime.Left(2),
				(const char*) infoConsol.m_strTime.Mid(2,2),
				(const char*) infoConsol.m_strTime.Right(2) );
		}

		if ( infoConsol.CompareServer( CurrentServer ) == FALSE )
		{
			if ( bNeedTotal )
				WriteTotals( FALSE, nServerDiscount, nServerMarkdown, nServerPremium, nServerUnknown );
				
			m_ReportFile.RequestBlankLines(2);
			m_ReportFile.WriteReportMiscLine( GetServerHeader( infoConsol.m_nServerLocOrDbIdx, infoConsol.m_nServerNo ) );
			m_ReportFile.WriteReportMiscLine ( "<LI>" );
			
			CurrentServer = infoConsol;

			nServerDiscount = 0;
			nServerMarkdown = 0;
			nServerPremium = 0;
			nServerUnknown = 0;

			bNeedTotal = TRUE;
		}

		CString strText = "<..>";

		switch( infoConsol.m_nDiscountMode )
		{
		case DISCOUNT_MODE_SUBTOTAL:	strText += "Subtotal ";	break;
		case DISCOUNT_MODE_LOYALTY:		strText += "Loyalty ";	break;
		case DISCOUNT_MODE_ITEM:		strText += "Item ";		break;
		}

		switch( infoConsol.m_nDiscountType )
		{
		case DISCOUNT_TYPE_MARKDOWN:
			strText += "Markdown";
			if ( FALSE == bServerTotalSection )
			{
				nServerMarkdown += infoConsol.m_nAmount;
				nTotalMarkdown += infoConsol.m_nAmount;
			}
			break;

		case DISCOUNT_TYPE_DISCOUNT:
			strText += "Discount";
			if ( FALSE == bServerTotalSection )
			{
				nServerDiscount += infoConsol.m_nAmount;
				nTotalDiscount += infoConsol.m_nAmount;
			}
			break;

		case DISCOUNT_TYPE_PREMIUM:
			strText += "Premium";
			if ( FALSE == bServerTotalSection )
			{
				nServerPremium += infoConsol.m_nAmount;
				nTotalPremium += infoConsol.m_nAmount;
			}
			break;

		case DISCOUNT_TYPE_UNKNOWN:
			strText += "Unknown";
			if ( FALSE == bServerTotalSection )
			{
				nServerUnknown += infoConsol.m_nAmount;
				nTotalUnknown += infoConsol.m_nAmount;
			}
			break;
		}

		CCSV csvPromo;
		{
			CString strPromoNo = "";
			CString strPromoName = "";
			CString strRate = "";

			if ( infoConsol.m_nPromoNo == 0 )
			{
				strPromoName = "Unknown Promo";
			}
			else
			{
				strPromoNo.Format ( "P%2.2d", infoConsol.m_nPromoNo );

				int nPromoIdx;
				if ( DataManager.Promotion.FindTableByNumber( infoConsol.m_nPromoNo, nPromoIdx ) == FALSE )
				{
					strPromoName.Format( "Promo %d", infoConsol.m_nPromoNo );
				}
				else
				{
					CPromotionsCSVRecord Promo;
					DataManager.Promotion.GetAt( nPromoIdx, Promo );
					strPromoName = Promo.GetDisplayName();
				}
				
				if ( infoConsol.m_nDiscountType != DISCOUNT_TYPE_MARKDOWN )
					if ( infoConsol.m_nRate != 0 )
						strRate.Format ( "%.2f%%", ( (double) infoConsol.m_nRate ) / 100.0 );
			}
			
			if ( strPromoNo != "" ) csvPromo.Add( strPromoNo );
			csvPromo.Add( strPromoName );
			if ( strRate != "" ) csvPromo.Add( strRate );

		}

		CCSV csvPluNo;
		if ( infoConsol.m_nPluNo != 0 )
		{		
			CSQLPluNoInfo infoPluNo;
			infoPluNo.m_nEposPluNo = infoConsol.m_nPluNo;
			::ProcessPluNo( infoPluNo );

			if ( TRUE == infoPluNo.m_bValid )
			{
				CString strPluNo = ReportHelpers.GetDisplayPluNo( infoPluNo.m_nBasePluNo, TRUE );
				
				if ( infoPluNo.m_nModifier != 0 )
				{
					CString strExtra;
					strExtra.Format( " + %d",
						infoPluNo.m_nModifier );

					strPluNo += strExtra;
				}
				
				csvPluNo.Add( strPluNo );
				csvPluNo.Add( infoConsol.m_strText );
			}
		}
		
		double dAmount = ( (double) infoConsol.m_nAmount ) / 100.0;

		CCSV csvReport ( '\t' );
		if ( FALSE == bServerTotalSection )
		{
			csvReport.Add( strLineDate );
			csvReport.Add( strLineTime );
			csvReport.Add( strText );
			csvReport.Add( csvPromo.GetLine() );
			csvReport.Add( dAmount, SysInfo.GetDPValue() );
			csvReport.Add( csvPluNo.GetLine() );
			csvReport.Add( infoConsol.m_strReason );
		}
		else
		{
			csvReport.Add( "" );
			csvReport.Add( "" );
			csvReport.Add( strText );
			csvReport.Add( csvPromo.GetLine() );
			csvReport.Add( dAmount, SysInfo.GetDPValue() );
		}

		m_ReportFile.WriteReportDataLine ( csvReport.GetLine() );
	}
	StatusProgress.Unlock();

	if ( bNeedTotal )
		WriteTotals( FALSE, nServerDiscount, nServerMarkdown, nServerPremium, nServerUnknown );
	
	m_ReportFile.RequestBlankLines(1);
	WriteTotals( TRUE, nTotalDiscount, nTotalMarkdown, nTotalPremium, nTotalUnknown );
		
	ShowEposDetails();
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/

void CEposReportDiscountReason::WriteTotals( bool bGrandTotal, int nDiscount, int nMarkdown, int nPremium, int nUnknown )
{
	CString strDiscount, strMarkdown, strPremium, strUnknown, strNet;

	if ( bGrandTotal == FALSE )
	{
		strDiscount = "<..>Server Discount";
		strMarkdown = "<..>Server Markdown";
		strPremium = "<..>Server Premium";
		strUnknown = "<..>Server Unknown Type";
		strNet = "<..>Server Net Discount";
		m_ReportFile.WriteReportMiscLine ( "\t\t\t\t<LI>" );
	}
	else
	{
		strDiscount = "<..>Total Discount";
		strMarkdown = "<..>Total Markdown";
		strPremium = "<..>Total Premium";
		strUnknown = "<..>Total Unknown Type";
		strNet = "<..>Total Net Discount";
		m_ReportFile.WriteReportMiscLine( "<..>Grand Totals" );
		m_ReportFile.WriteReportMiscLine( "<LI>" );
	}
	
	CCSV csv ( '\t' );
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( strDiscount );
	csv.Add ( ( ( double ) nDiscount ) / 100.0, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportMiscLine ( csv.GetLine() );

	csv.RemoveAll();
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( strMarkdown );
	csv.Add ( ( ( double ) nMarkdown ) / 100.0, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportMiscLine ( csv.GetLine() );

	if ( nUnknown != 0 )
	{
		csv.RemoveAll();
		csv.Add ( "" );
		csv.Add ( "" );
		csv.Add ( "" );
		csv.Add ( strUnknown );
		csv.Add ( ( ( double ) nUnknown ) / 100.0, SysInfo.GetDPValue() );
		m_ReportFile.WriteReportMiscLine ( csv.GetLine() );
	}

	m_ReportFile.WriteReportMiscLine ( "\t\t\t\t<LI>" );
	
	csv.RemoveAll();
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( strPremium );
	csv.Add ( ( ( double ) nPremium ) / 100.0, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportMiscLine ( csv.GetLine() );

	m_ReportFile.WriteReportMiscLine ( "\t\t\t\t<LI>" );

	csv.RemoveAll();
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( "" );
	csv.Add ( strNet );
	csv.Add ( ( ( double ) nDiscount + nMarkdown + nUnknown - nPremium ) / 100.0, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportMiscLine ( csv.GetLine() );
}

/**********************************************************************/
