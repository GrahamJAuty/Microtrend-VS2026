/**********************************************************************/
#include "DateRangeHelpers.h"
#include "EposReportFileListMaker.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "LocationSelectorEntity.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "ReportPointCSVArray.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "PosTrayExportConsolidatorVoid.h"
/**********************************************************************/

CPosTrayExportConsolidatorVoid::CPosTrayExportConsolidatorVoid( int nReportDateType, CEposSelectArray& SelectArray, bool& bQuitNow, void* pHostDlg ) : CPosTrayExportConsolidator( nReportDateType, SelectArray, bQuitNow, pHostDlg )
{
	m_PMSModes.SetDateMode( PMS_DATEMODE_ACTUAL );
	m_PMSModes.SetAllowMode( PMS_ALLOWMODE_NONE );
	m_PMSModes.SetEposFlag( FALSE );

	m_bEODMode = FALSE;
	m_bSaleTime = FALSE;

	m_TimeSliceMap.SetReportType( SH_TIMESLICE_SECOND );
	m_TimeSliceMap.SetTimeSliceFileNo(0);

	AddAcceptedField ( POSTRAY_EXPORTFIELD_DBNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_LOCNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_RPNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_TRMNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DATE_CAL );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DATE_BUS );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_YEARNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_MONTHNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DATE_WEEK );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_WEEKNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_WEEKDAYNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_TIME_TRAN );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_TIME_VOID );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_PLUNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_MODNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DPTNO_DBASE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DPTNO_SALE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_GRPNO_DBASE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_GRPNO_SALE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_ACATNO_DBASE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_ACATNO_SALE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_SERVER_TRAN );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_SERVER_ITEM );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_SERVER_AUTH );
	/**********/
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DBNAME );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_LOCNAME );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_RPNAME );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_REPTEXT_BASE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_REPTEXT_MOD );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_MODNAME );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_SUPPREF );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_SORTCODE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_STOCKCODE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DPTNAME_DBASE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DPTNAME_SALE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_GRPNAME_DBASE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_GRPNAME_SALE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_ACATNAME_DBASE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_ACATNAME_SALE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_SRVNAME_TRAN );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_SRVNAME_ITEM );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_SRVNAME_AUTH );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_MONTHNAME );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_WEEKDAYNAME );
	/**********/
	AddAcceptedField ( POSTRAY_EXPORTFIELD_VOID_SEQNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_VOID_TABLENO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_VOID_LINE_TYPE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_VOID_BEFOREAFTER );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_VOID_REASON );
	/**********/
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DISCOUNT_USAGE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DISCOUNT_TYPE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DISCOUNT_RATE );
	/**********/
	AddAcceptedField ( POSTRAY_EXPORTFIELD_PROMONO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_PROMONAME );
	/**********/
	AddAcceptedField ( POSTRAY_EXPORTFIELD_LINE_COUNT );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_QTY_BASE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_QTY_MOD );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_VALUE_VOID );
}

/**********************************************************************/

void CPosTrayExportConsolidatorVoid::ResetTaskSpecific( CPosTrayTask& EmailTask )
{
	if ( m_arrayOutputFields.GetSize() == 0 )
	{
		m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_VOID_LINE_TYPE );
		m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_PLUNO );
		m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_REPTEXT_BASE );
		m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_QTY_BASE );
		m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_VALUE_VOID );
		m_arrayConsolidationFields.Add( POSTRAY_EXPORTFIELD_VOID_LINE_TYPE );
		m_arrayConsolidationFields.Add( POSTRAY_EXPORTFIELD_PLUNO );

	}
	else if ( FALSE == m_bGotVoidLineType )
	{
		m_arrayOutputFields.InsertAt( 0, POSTRAY_EXPORTFIELD_VOID_LINE_TYPE );
		m_arrayConsolidationFields.InsertAt( 0, POSTRAY_EXPORTFIELD_VOID_LINE_TYPE );
	}

	if ( m_arrayConsolidationFields.GetSize() == 0 )
		m_arrayConsolidationFields.Add( POSTRAY_EXPORTFIELD_DUMMY );

	m_PMSModes.SetEposFlag( EmailTask.GetBatchPMSTerminalFlag() );

	CLocationSelectorLookup lookup( EmailTask );
	CLocationSelectorEntity entity( lookup );
	entity.GetEPOSSelectArray( m_SelectArray );
	m_SelectArray.MakeList( TRUE );

	m_VoidTree.ClearConsolidatedLevels();
	m_VoidTree.ClearFieldStructure();
}

/**********************************************************************/

bool CPosTrayExportConsolidatorVoid::PrepareFields()
{
	return CPosTrayExportConsolidator::PrepareFields( m_VoidTree );
}

/**********************************************************************/

void CPosTrayExportConsolidatorVoid::Consolidate() 
{
	dbDatabase.BuildSortIdxTables();
	dbLocation.BuildSortIdxTables();
	dbLocation.ResetRpNoReport();
	dbReportpoint.BuildSortIdxTables();
	
	m_VoidTree.ClearConsolidatedLevels();

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );

	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_REASONS_ALL, m_SelectArray, FALSE, nTotalFileSize );

	int nProgress = 0;

	if ( NULL == m_pHostDialog )
		StatusProgress.Lock( TRUE, "Consolidating report data" );
	
	int nLastDbIdx = -1;
	CDateRangeHelpers DateRangeHelpers;
	
	for ( int nFileIdx = 0; ( nFileIdx < arrayFileInfo.GetSize() ) && ( FALSE == m_bQuitNow ); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		//CHECK PMS STATUS
		if ( dbLocation.IsPMSPseudoLocation( infoFile.m_nLocIdx ) == TRUE )
			continue;

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		CString strThisFilterDate = "";
		CString strThisFilterTime = "";
		
		m_PMSModes.GetInitialSaleDateTime( FALSE, infoFile, m_bSaleTime, strThisFilterDate, strThisFilterTime );
		COleDateTime oleCalendarDate = COleDateTime( atoi( strThisFilterDate.Left(4) ), atoi( strThisFilterDate.Mid(4,2) ), atoi( strThisFilterDate.Mid(6,2) ), 0, 0, 0 );

		if ( infoFile.m_nDbIdx != nLastDbIdx )
		{
			CDataManagerInfo info;
			DataManager.OpenDatabaseReadOnly( infoFile.m_nDbIdx, info, FALSE );
			nLastDbIdx = infoFile.m_nDbIdx;
		}

		//CHECK SPORTS BOOKER NETWORK STATUS
		int nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
		{
			int nDummy = 0;
			dbLocation.GetLocSPOSVersion( infoFile.m_nLocIdx, nDummy, nConnectionType );
		}

		//CHECK LOCATION SETTINGS
		int nDbNo = dbLocation.GetDbNo( infoFile.m_nLocIdx );
		
		int nRpIdx = 0;
		if ( infoFile.m_nTermIdx >= 0 )
		{
			int nRpNo = dbLocation.GetRpNoReport( infoFile.m_nLocIdx, infoFile.m_nTermIdx );
			if ( dbReportpoint.FindReportpointByNumber( nDbNo, nRpNo, nRpIdx ) == TRUE )
				nRpIdx = dbReportpoint.GetSortIdxFromRpIdx( nRpIdx ) + 1;
			else
				nRpIdx = 0;
		}

		int nSortDbIdx = dbDatabase.GetSortIdxFromDbIdx( infoFile.m_nDbIdx );
		int nSortLocIdx = dbLocation.GetSortIdxFromLocIdx( infoFile.m_nLocIdx );
		
		int nTNo = 0;
		if ( infoFile.m_nTermIdx >= 0 )
			nTNo = dbLocation.GetTNo( infoFile.m_nLocIdx, infoFile.m_nTermIdx );

		//LOCATION SETTINGS APPLY TO THE ENTIRE FILE
		for ( int n = 0; n < m_arrayConsolidationFields.GetSize(); n++ )
		{
			switch ( m_arrayConsolidationFields.GetAt( n ) )
			{
			case POSTRAY_EXPORTFIELD_DUMMY:	m_VoidTree.SetBufferInt( n, 0 );				break;
			case POSTRAY_EXPORTFIELD_DBNO:	m_VoidTree.SetBufferInt( n, nSortDbIdx );		break;
			case POSTRAY_EXPORTFIELD_LOCNO:	m_VoidTree.SetBufferInt( n, nSortLocIdx );		break;
			case POSTRAY_EXPORTFIELD_TRMNO:	m_VoidTree.SetBufferInt( n, nTNo );				break;
			case POSTRAY_EXPORTFIELD_RPNO:	m_VoidTree.SetBufferInt( n, nRpIdx );			break;	
			}
		}

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
			
			int nReasonType = ReasonInfo.GetReasonType(); 

			if ( SS_REASON_REFUNDVOID == nReasonType )
				if ( ReasonInfo.GetItemVoidFlag() == FALSE )
					continue;

			CString strTranTime = "";
			CString strVoidTime = "";

			switch( nReasonType  )
			{
			case SS_REASON_REFUNDVOID:
			case SS_REASON_DISCOUNT_VOID:
				strTranTime = ReasonInfo.GetTimeTran();
				strVoidTime = ReasonInfo.GetTimeAction();
				break;

			default:
				continue;
			}

			ExpandTimeString( strTranTime );
			ExpandTimeString( strVoidTime );

			strThisFilterTime = ( m_bSaleTime ) ? strVoidTime : strTranTime;

			//CHECK FOR PREVIOUS DATE
			if ( m_BusinessDateInfo.ValidateBusinessDate( infoFile.m_nLocIdx, m_bEODMode, strThisFilterDate, strThisFilterTime ) == FALSE )
				continue;

			CreateTimeLabel( strTranTime, ( FALSE == m_bSaleTime ) );
			CreateTimeLabel( strVoidTime, ( TRUE == m_bSaleTime ) );

			for ( int n = 0; n < m_arrayConsolidationFields.GetSize(); n++ )
			{
				switch ( m_arrayConsolidationFields.GetAt( n ) )
				{
					case POSTRAY_EXPORTFIELD_DATE_BUS:				m_VoidTree.SetBufferDate( n, m_BusinessDateInfo.GetBusinessDate() );	 break;
					case POSTRAY_EXPORTFIELD_DATE_CAL:				m_VoidTree.SetBufferDate( n, oleCalendarDate ); break;
					case POSTRAY_EXPORTFIELD_YEARNO:				m_VoidTree.SetBufferInt( n, m_BusinessDateInfo.GetBusinessDate().GetYear() ); break;
					case POSTRAY_EXPORTFIELD_MONTHNO:				m_VoidTree.SetBufferInt( n, m_BusinessDateInfo.GetBusinessDate().GetMonth() ); break;
					case POSTRAY_EXPORTFIELD_DATE_WEEK:				m_VoidTree.SetBufferDate( n, m_BusinessDateInfo.GetBusinessDateWeek() );	break;
					case POSTRAY_EXPORTFIELD_WEEKNO:				m_VoidTree.SetBufferInt( n, DateRangeHelpers.GetWeekNo( m_BusinessDateInfo.GetBusinessDateWeek() ) );	break; 
					case POSTRAY_EXPORTFIELD_WEEKDAYNO:				m_VoidTree.SetBufferInt( n, m_BusinessDateInfo.GetWeekday() ); break;
					case POSTRAY_EXPORTFIELD_TIME_TRAN:				m_VoidTree.SetBufferString( n, strTranTime );	break;
					case POSTRAY_EXPORTFIELD_TIME_VOID:				m_VoidTree.SetBufferString( n, strVoidTime );	break;
				}
			}

			switch( nReasonType )
			{
			case SS_REASON_REFUNDVOID:
				{
					CSQLPluNoInfo infoPluNo;
					infoPluNo.m_nEposPluNo = ReasonInfo.GetItemPluNo();
					::ProcessPluNo( infoPluNo, FALSE, TRUE );
						
					if ( FALSE == infoPluNo.m_bValid )
						continue;
	
					__int64 nBasePluNo = infoPluNo.m_nBasePluNo;
					int nModifier = infoPluNo.m_nModifier;
					double dMultiplier = 1.0;

					if ( nModifier != 0 )
						dMultiplier = DataManager.Modifier.GetMultiplierForQty ( nModifier );

					int nDeptNoSale = ReasonInfo.GetItemDeptNo();
					int nDeptGrpSale = ReasonInfo.GetItemGroupNo();
					int nCategorySale = ReasonInfo.GetItemACatNo();
	
					int nDeptNoItem = 0;
					int nDeptGrpItem = 0;
					int nCategoryItem = 0;
				
					if ( TRUE == m_bWantPluInfo )
					{
						int nPluIdx;
						if ( DataManager.Plu.FindPluByNumber( nBasePluNo, nPluIdx ) == TRUE )
						{
							CPluCSVRecord PluRecord;
							DataManager.Plu.GetAt( nPluIdx, PluRecord );
							nCategoryItem = PluRecord.GetModifierAnalysisCategoryForReport(nModifier);

							if ( 0 == nCategorySale )
								nCategorySale = nCategoryItem;

							nDeptNoItem = PluRecord.GetModifierDeptNoForReport(nModifier);
						}
					}
			
					if ( m_bWantDeptInfo == TRUE )
					{
						int nDeptIdx;
						if ( DataManager.Department.FindDeptByNumber( nDeptNoItem, nDeptIdx ) == TRUE )
						{
							CDepartmentCSVRecord deptRecord;
							DataManager.Department.GetAt( nDeptIdx, deptRecord );
							nDeptGrpItem = deptRecord.GetEposGroup();
						}
					}

					CString strTranServer = "";
					if ( TRUE == m_bWantTranServer )
					{
						int nServerNo = ReasonInfo.GetTranServerNo();
						DataManagerNonDb.ServerNameTable.GetServerConsolidationString( infoFile.m_nDbIdx, infoFile.m_nLocIdx, nServerNo, strTranServer );
					}

					CString strItemServer = "";
					if ( TRUE == m_bWantItemServer )
					{
						int nServerNo = ReasonInfo.GetItemServerNo();
						DataManagerNonDb.ServerNameTable.GetServerConsolidationString( infoFile.m_nDbIdx, infoFile.m_nLocIdx, nServerNo, strItemServer );
					}

					CString strAuthServer = "";
					if ( TRUE == m_bWantAuthServer )
					{
						int nServerNo = ReasonInfo.GetItemAuthServerNo();
						
						if ( 0 == nServerNo )
							nServerNo = ReasonInfo.GetItemServerNo();
					
						DataManagerNonDb.ServerNameTable.GetServerConsolidationString( infoFile.m_nDbIdx, infoFile.m_nLocIdx, nServerNo, strAuthServer );
					}

					int nVoidBeforeAfter = 0;
					{
						CString strBeforeAfter = ReasonInfo.GetItemVoidBAType();
						if ( strBeforeAfter == "A" ) nVoidBeforeAfter = 1;
					}

					int nLineType = ReasonInfo.GetItemRefundFlag() ? 1 : 0;

					CString strReasonText = ReasonInfo.GetReasonText();
					CCSV csvReason( strReasonText, ',', '\'' );

					for ( int n = 0; n < m_arrayConsolidationFields.GetSize(); n++ )
					{
						CString strDummy = "";
						switch ( m_arrayConsolidationFields.GetAt( n ) )
						{
						case POSTRAY_EXPORTFIELD_VOID_LINE_TYPE:		m_VoidTree.SetBufferInt( n, nLineType ); break;
						case POSTRAY_EXPORTFIELD_VOID_SEQNO:			m_VoidTree.SetBufferInt( n, ReasonInfo.GetTranSeqNo() );  break;
						case POSTRAY_EXPORTFIELD_VOID_TABLENO:			m_VoidTree.SetBufferInt( n, ReasonInfo.GetTranTableNo() );  break;
						case POSTRAY_EXPORTFIELD_VOID_BEFOREAFTER:		m_VoidTree.SetBufferInt( n, nVoidBeforeAfter ); break;
						case POSTRAY_EXPORTFIELD_VOID_REASON:			m_VoidTree.SetBufferString( n, strReasonText ); break;
						case POSTRAY_EXPORTFIELD_SERVER_TRAN:			m_VoidTree.SetBufferString( n, strTranServer );  break;
						case POSTRAY_EXPORTFIELD_SERVER_ITEM:			m_VoidTree.SetBufferString( n, strItemServer );  break;
						case POSTRAY_EXPORTFIELD_SERVER_AUTH:			m_VoidTree.SetBufferString( n, strAuthServer );  break;
						case POSTRAY_EXPORTFIELD_ACATNO_SALE:			m_VoidTree.SetBufferInt( n, nCategorySale ); break;
						case POSTRAY_EXPORTFIELD_ACATNO_DBASE:			m_VoidTree.SetBufferInt( n, nCategoryItem ); break;
						case POSTRAY_EXPORTFIELD_DPTNO_SALE:			m_VoidTree.SetBufferInt( n, nDeptNoSale ); break;
						case POSTRAY_EXPORTFIELD_DPTNO_DBASE:			m_VoidTree.SetBufferInt( n, nDeptNoItem ); break;
						case POSTRAY_EXPORTFIELD_GRPNO_SALE:			m_VoidTree.SetBufferInt( n, nDeptGrpSale );	break;
						case POSTRAY_EXPORTFIELD_GRPNO_DBASE:			m_VoidTree.SetBufferInt( n, nDeptGrpItem );	break;				
						case POSTRAY_EXPORTFIELD_PLUNO:					m_VoidTree.SetBufferInt64( n, nBasePluNo ); break;
						case POSTRAY_EXPORTFIELD_MODNO:					m_VoidTree.SetBufferInt( n, nModifier ); break;
						case POSTRAY_EXPORTFIELD_DISCOUNT_USAGE:		m_VoidTree.SetBufferInt( n, 0 ); break;
						case POSTRAY_EXPORTFIELD_DISCOUNT_TYPE:			m_VoidTree.SetBufferInt( n, 0 ); break;
						case POSTRAY_EXPORTFIELD_DISCOUNT_RATE:			m_VoidTree.SetBufferString( n, strDummy ); break;
						case POSTRAY_EXPORTFIELD_PROMONO:				m_VoidTree.SetBufferInt( n, 0 ); break;
						}
					}

					double dLineModQty = ReasonInfo.GetItemQty();
					double dLineValue = ReasonInfo.GetItemVal();
					double dLineBaseQty = dLineModQty * dMultiplier;

					if ( TRUE == ReasonInfo.GetItemRefundFlag() )
					{
						dLineBaseQty *= -1;
						dLineModQty *= -1;
						dLineValue *= -1;
					}
						
					CEntityTreeExportVoidNode Node;
					Node.SetLineCount(1);
					Node.SetBaseQty( dLineBaseQty );
					Node.SetModQty( dLineModQty );
					Node.SetVal( dLineValue );
					m_VoidTree.Consolidate( Node );
				}
				break;
			
			case SS_REASON_DISCOUNT_VOID:
				{
					CString strTranServer = "";
					if ( ( TRUE == m_bWantTranServer ) || ( TRUE == m_bWantItemServer ) )
					{
						int nServerNo = ReasonInfo.GetTranServerNo();
						DataManagerNonDb.ServerNameTable.GetServerConsolidationString( infoFile.m_nDbIdx, infoFile.m_nLocIdx, nServerNo, strTranServer );
					}

					int nVoidBeforeAfter = 0;
					{
						CString strBeforeAfter = ReasonInfo.GetVoidDiscountBAType();
						if ( strBeforeAfter == "A" ) nVoidBeforeAfter = 1;
					}

					CString strReasonText = ReasonInfo.GetReasonText();
					CCSV csvReason( strReasonText, ',', '\'' );

					int nLineType = 2;
					int nDiscountType = 0;
					{
						CString strType = ReasonInfo.GetVoidDiscountType();					
						if		( strType == "M" )	nDiscountType = 1;
						else if	( strType == "D" )	nDiscountType = 2;
						else if	( strType == "P" )	nLineType = 3;
						else						nDiscountType = 3;
					}

					CString strDiscountRate = "";
					if ( nDiscountType != 1 )
						strDiscountRate.Format ( "%.2f%%", ( (double) ReasonInfo.GetVoidDiscountRate() ) / 100.0 );

					for ( int n = 0; n < m_arrayConsolidationFields.GetSize(); n++ )
					{
						switch ( m_arrayConsolidationFields.GetAt( n ) )
						{
						case POSTRAY_EXPORTFIELD_VOID_LINE_TYPE:		m_VoidTree.SetBufferInt( n, nLineType ); break;
						case POSTRAY_EXPORTFIELD_VOID_SEQNO:			m_VoidTree.SetBufferInt( n, ReasonInfo.GetTranSeqNo() );  break;
						case POSTRAY_EXPORTFIELD_VOID_TABLENO:			m_VoidTree.SetBufferInt( n, ReasonInfo.GetTranTableNo() );  break;
						case POSTRAY_EXPORTFIELD_VOID_BEFOREAFTER:		m_VoidTree.SetBufferInt( n, nVoidBeforeAfter ); break;
						case POSTRAY_EXPORTFIELD_VOID_REASON:			m_VoidTree.SetBufferString( n, strReasonText ); break;
						case POSTRAY_EXPORTFIELD_SERVER_TRAN:			m_VoidTree.SetBufferString( n, strTranServer );  break;
						case POSTRAY_EXPORTFIELD_SERVER_ITEM:			m_VoidTree.SetBufferString( n, strTranServer );  break;
						case POSTRAY_EXPORTFIELD_SERVER_AUTH:			m_VoidTree.SetBufferString( n, strTranServer );  break;
						case POSTRAY_EXPORTFIELD_DISCOUNT_USAGE:		m_VoidTree.SetBufferInt( n, ReasonInfo.GetVoidDiscountUsage() ); break;
						case POSTRAY_EXPORTFIELD_DISCOUNT_TYPE:			m_VoidTree.SetBufferInt( n, nDiscountType ); break;
						case POSTRAY_EXPORTFIELD_DISCOUNT_RATE:			m_VoidTree.SetBufferString( n, strDiscountRate ); break;
						case POSTRAY_EXPORTFIELD_PROMONO:				m_VoidTree.SetBufferInt( n, ReasonInfo.GetVoidDiscountPromoNo() ); break;
						case POSTRAY_EXPORTFIELD_ACATNO_SALE:			m_VoidTree.SetBufferInt( n, 0 ); break;
						case POSTRAY_EXPORTFIELD_ACATNO_DBASE:			m_VoidTree.SetBufferInt( n, 0 ); break;
						case POSTRAY_EXPORTFIELD_DPTNO_SALE:			m_VoidTree.SetBufferInt( n, 0 ); break;
						case POSTRAY_EXPORTFIELD_DPTNO_DBASE:			m_VoidTree.SetBufferInt( n, 0 ); break;
						case POSTRAY_EXPORTFIELD_GRPNO_SALE:			m_VoidTree.SetBufferInt( n, 0 );	break;
						case POSTRAY_EXPORTFIELD_GRPNO_DBASE:			m_VoidTree.SetBufferInt( n, 0 );	break;				
						case POSTRAY_EXPORTFIELD_PLUNO:					m_VoidTree.SetBufferInt64( n, 0 ); break;
						case POSTRAY_EXPORTFIELD_MODNO:					m_VoidTree.SetBufferInt( n, 0 ); break;
						}
					}

					double dDiscountAmount = 0.0;
					{
						int nDiscountAmount = ReasonInfo.GetVoidDiscountAmount();
						if ( nLineType != 3 ) nDiscountAmount *= -1;
						dDiscountAmount = ( (double) nDiscountAmount ) / 100.0;
					}

					CEntityTreeExportVoidNode Node;
					Node.SetLineCount(1);
					Node.SetBaseQty(1);
					Node.SetModQty(1);
					Node.SetVal( dDiscountAmount  );
					m_VoidTree.Consolidate( Node );
				}
				break;
			}
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}

	if ( NULL == m_pHostDialog )
		StatusProgress.Unlock();
}

/**********************************************************************/

__int64 CPosTrayExportConsolidatorVoid::CreateExport( const char* szFilename )
{
	bool bResult;
	return CPosTrayExportConsolidator::CreateExport( &m_VoidTree, "Creating Void Export", szFilename, bResult, FALSE );

};

/**********************************************************************/

void CPosTrayExportConsolidatorVoid::GetNodeTotals( int nBranchIdx, int nLeafIdx )
{
	CEntityTreeExportVoidNode VoidNode;
	m_VoidTree.GetNode( nBranchIdx, nLeafIdx, VoidNode );
	m_NodeTotals.m_dQtyBase = VoidNode.GetBaseQty();
	m_NodeTotals.m_dQtyMod = VoidNode.GetModQty();
	m_NodeTotals.m_dValueVoid = VoidNode.GetVal();
	m_NodeTotals.m_nLineCount = VoidNode.GetLineCount();
}

/**********************************************************************/

