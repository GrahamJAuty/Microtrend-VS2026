/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
#include "LocationCSVArray.h"
#include "PriceHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportGeneralDorbiere.h"
/**********************************************************************/

CEposReportGeneralDorbiere::CEposReportGeneralDorbiere( CEposSelectArray& SelectArray ) : CEposReportGeneral( SelectArray, REPORT_TYPE_GENERAL_GROUP )
{
	m_BlockMap.SetDorbiereMode();
	m_nReportType = REPORT_TYPE_GENERAL_GROUP;
	m_ReportSettings.SetWeekdayFlag( FALSE );
	m_ReportSettings.SetPromoDetailFlag( FALSE );
	m_ReportSettings.SetPromoSummaryFlag( FALSE );
	m_ReportSettings.SetReconciliationFlag( TRUE );	
	m_ReportSettings.SetVoidFlag( FALSE );
	m_ReportSettings.SetNoSaleFlag( FALSE );
	m_ReportSettings.SetConsolDeptFlag( FALSE );
	m_ReportSettings.SetConsolGroupReportFlag( FALSE );
	m_ReportSettings.SetConsolGroupConsolFlag( FALSE );
	m_ReportSettings.SetConsolGroupEposFlag( TRUE );
	m_ReportSettings.SetQuantityFlag( FALSE );
}

/**********************************************************************/

void CEposReportGeneralDorbiere::GetConsolidationParams( CEposReportConsolParamsStandard& Params )
{
	Params.m_bConsolTerm = FALSE;
	Params.m_bConsolLoc = TRUE;
	Params.m_bConsolDb = FALSE;
	Params.m_bConsolSys = FALSE;
}

/**********************************************************************/

void CEposReportGeneralDorbiere::ConsolidatePaymentBuffer( int nWeekday ) 
{
	if ( m_arrayPaidIOBuffer.GetSize() != 0 )
	{
		CCSVEposTermLine csvIn ( m_arrayPaidIOBuffer.GetAt(0) );
		bool bPaidOut = ( csvIn.GetPaidIODirection() == 0 );
				
		for ( int n = 0; n < m_arrayPaidIOBuffer.GetSize(); n++ )
		{
			CCSVEposTermLine csvIn ( m_arrayPaidIOBuffer.GetAt(n) );

			CString strVoid = csvIn.GetPaidIOVoidFlag();
			if ( strVoid != "V" )
			{
				int nNo = csvIn.GetPaidIONumber();
				double dQty = csvIn.GetPaidIOQty();
				
				if ( ( nNo < 1 ) || ( nNo > PAIDIO_TEXT_COUNT ) )
					nNo = PAIDIO_TEXT_COUNT + 1;

				double dValue = ( ( double ) csvIn.GetPaidIOValue() ) / 100.0;
					
				if ( FALSE == bPaidOut )
					ConsolidateReconcile( nWeekday, 100 + nNo, dQty, dValue );		//PAID IN
				else
					ConsolidateReconcile( nWeekday, 300 + nNo, dQty, -dValue );		//PAID OUT

				if ( FALSE == bPaidOut )
					ConsolidatePayment( nWeekday, 0, 1.0, dValue );
				else
					ConsolidatePayment( nWeekday, 0, 1.0, -dValue );					
				
				ConsolidateReconcile( nWeekday, GENREP_RECON_BALANCE, dQty, dValue );	//RECONCILE BALANCE
			}
		}
	}
	
	for ( int n = 0; n < m_arrayPaymentBuffer.GetSize(); n++ )
	{
		CCSVEposTermLine csvIn ( m_arrayPaymentBuffer.GetAt(n) );

		int nPaymentNumber = csvIn.GetPaymentNumber();
		int nTender = csvIn.GetPaymentTender();
		int nAmount = csvIn.GetPaymentAmount();
		int nGratuity = csvIn.GetPaymentGratuity();
		int nCashback = csvIn.GetPaymentCashback();
		int nSurplus = csvIn.GetPaymentSurplus( m_CashRSPVersionChecker.GetCurrentTransactionVersionNo() );
		int nChange = nTender - ( nAmount + nGratuity + nSurplus );
		
		int nAccountType, nDummy32;
		__int64 nDummy64;
		bool bIsCash;
		csvIn.GetPaymentAccountTypeAndID( m_CashRSPVersionChecker, nAccountType, nDummy32, nDummy64, bIsCash );
		bool bUsedDeposit = ( CASHRSP_ITEMPAYTYPE_DEPOSIT == nAccountType );

		double dTender = ( (double) nTender ) / 100.0;

		int nLineType = 0;

		switch( nAccountType )
		{
		case CASHRSP_ITEMPAYTYPE_DEPOSIT:		nLineType = GENREP_RECON_CHARGE_DEPOSIT;		break;
		case CASHRSP_ITEMPAYTYPE_CUSTOMER:		nLineType = GENREP_RECON_CHARGE_CUSTOMER;		break;
		case CASHRSP_ITEMPAYTYPE_ROOM:			nLineType = GENREP_RECON_CHARGE_ROOM;			break;
		case CASHRSP_ITEMPAYTYPE_LOYALTY:		nLineType = GENREP_RECON_CHARGE_LOYALTY;		break;
		case CASHRSP_ITEMPAYTYPE_SMARTPAY:		nLineType = GENREP_RECON_CHARGE_SMARTPAY;		break;
		case CASHRSP_ITEMPAYTYPE_SMARTPHONE:	nLineType = GENREP_RECON_CHARGE_SMARTPHONE;		break;
		case CASHRSP_ITEMPAYTYPE_SPTBOOK:		nLineType = GENREP_RECON_CHARGE_SPTBOOK;		break;
		case CASHRSP_ITEMPAYTYPE_NORMAL:	
		default:								nLineType = GENREP_RECON_CHARGE_PAID;			break;
		}

		ConsolidateReconcile( nWeekday, nLineType, 1.0, dTender );				//RECONCILE PAYMENT
		ConsolidateReconcile( nWeekday, GENREP_RECON_BALANCE, 1.0, -dTender );	//RECONCILE BALANCE

		if ( CASHRSP_ITEMPAYTYPE_NORMAL == nAccountType )
		{
			int nDorbierePayMode = 0;
			switch( csvIn.GetPaymentType() )
			{
			case 0:
				nDorbierePayMode = 0;
				break;

			case 2:
			case 3:
			case 9:
				nDorbierePayMode = 1;
				break;
			
			default:
				nDorbierePayMode = 2;
				break;
			}
			ConsolidatePayment( nWeekday, nDorbierePayMode, 1.0, dTender );
		}

		if ( 0 != nChange )
		{
			double dChange = ( (double) nChange ) / 100.0;

			ConsolidatePayment( nWeekday, 0, 0.0, -dChange );
			
			if ( m_ReportSettings.GetSeparateChangeFlag() == TRUE )
				ConsolidateReconcile( nWeekday, GENREP_RECON_CHANGE, 0.0, dChange );	//RECONCILE PAYMENT
			else
				ConsolidateReconcile( nWeekday, GENREP_RECON_CHARGE_PAID, 0.0, -dChange );	//RECONCILE CASHBACK

			ConsolidateReconcile( nWeekday, GENREP_RECON_BALANCE, 0.0, dChange );	//RECONCILE BALANCE
		}
		
		if ( 0 != nCashback )
		{
			double dCashback = ( (double) nCashback ) / 100.0;

			ConsolidatePayment( nWeekday, 0, 0.0, -dCashback );

			if ( m_ReportSettings.GetSeparateChangeFlag() == TRUE )
				ConsolidateReconcile( nWeekday, GENREP_RECON_CASHBACK, 0.0, dCashback );		//RECONCILE CASHBACK
			else
				ConsolidateReconcile( nWeekday, GENREP_RECON_CHARGE_PAID, 0.0, -dCashback );	//RECONCILE CASHBACK

			ConsolidateReconcile( nWeekday, GENREP_RECON_BALANCE, 0.0, dCashback );		//RECONCILE BALANCE
		}

		if ( 0 != nSurplus )
		{
			double dSurplus = ( (double) nSurplus ) / 100.0; 
			ConsolidateReconcile( nWeekday, GENREP_RECON_SURPLUS, 0.0, dSurplus );		//RECONCILE CASHBACK
			ConsolidateReconcile( nWeekday, GENREP_RECON_BALANCE, 0.0, dSurplus );		//RECONCILE BALANCE
		}

		if ( 0 != nGratuity )
		{
			double dGratuity = ( (double) nGratuity ) / 100.0;

			ConsolidateReconcile( nWeekday, GENREP_RECON_GRATUITY, 1.0, dGratuity );	//RECONCILE GRATUITY
			ConsolidateReconcile( nWeekday, GENREP_RECON_BALANCE, 0.0, dGratuity );		//RECONCILE BALANCE
		}
	}

	m_arrayPaymentBuffer.RemoveAll();
	m_arrayPaidIOBuffer.RemoveAll();
}

/**********************************************************************/

bool CEposReportGeneralDorbiere::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;
	
	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "No", TA_LEFT, 200 );
	m_ReportFile.AddColumn ( "Description", TA_LEFT, 550 );
	m_ReportFile.AddColumn ( "Value", TA_RIGHT, 250 );
		
	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportGeneralBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.GetLineCount( m_ReportSettings );		
	}

	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if ( m_nCreateReportMiniTarget < 1 )
		m_nCreateReportMiniTarget = 1;

	StatusProgress.Lock( TRUE, "Creating report" );

	m_BlockMap.BuildSortArray();
	for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
	{
		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportGeneralBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

		int nExportCode = 0;
		
		{
			CEposReportBlockMapEntity BlockMapEntity;
			m_BlockMap.GetBlockMapEntity( n, BlockMapEntity );
			int nSpIdx = BlockMapEntity.m_nRpIdx;
			if ( ( nSpIdx >= 0 ) && ( nSpIdx < dbStockpoint.GetSize() ) )
			{
				nExportCode = dbStockpoint.GetImportSiteNo( nSpIdx );
				
				if ( nExportCode != 0 )
					nExportCode += 10000;
			}
		}

		if ( 0 == nExportCode )
			continue;

		if ( ReportBlock.GotData() == FALSE )
			continue;

		m_ReportFile.HandleNewPage();

		if ( FALSE == m_bCanConsolidateGlobally )
		{
			CDataManagerInfo info;
			DataManager.OpenDatabaseReadOnly( ReportBlock.m_nDbIdx, info, FALSE );
		}

		CString strHeader = "<..>";
		switch( ReportBlock.m_nEntityType )
		{
		case NODE_STOCKPOINT:
			strHeader += dbStockpoint.GetName( ReportBlock.m_nRpIdx );
			break;

		default:
			strHeader += "Unknown";
			break;
		}

		if ( ReportBlock.m_pSales -> GetSize() > 0 )
		{
			m_ReportFile.RequestBlankLines(2);
			m_ReportFile.WriteReportMiscLine( strHeader + ", Sales" );
			m_ReportFile.WriteReportMiscLine( "<LI>" );
			m_ReportFile.LockBlankLines(0);
			CreateSalesSection( nExportCode, ReportBlock, NULL );
		}

		double dPaidInTotal = 0.0;
		double dPaidOutTotal = 0.0;
		CreatePaidIOSection( nExportCode, ReportBlock, NULL, strHeader, dPaidInTotal, dPaidOutTotal );

		double dCashTotal = 0.0;
		if ( ReportBlock.m_pPayment -> GetSize() > 0 )
		{
			m_ReportFile.RequestBlankLines(2);
			m_ReportFile.WriteReportMiscLine( strHeader + ", Payments" );
			m_ReportFile.WriteReportMiscLine( "<LI>" );
			m_ReportFile.LockBlankLines(0);
			CreatePaymentSection( nExportCode, ReportBlock, NULL, dCashTotal );
		}

		m_ReportFile.RequestBlankLines(2);
		m_ReportFile.WriteReportMiscLine( strHeader + ", Cash Summary" );
		m_ReportFile.WriteReportMiscLine( "<LI>" );
		m_ReportFile.LockBlankLines(0);
		CreateCashSummary( nExportCode, ReportBlock, NULL, dCashTotal, dPaidInTotal, dPaidOutTotal );			
	}

	StatusProgress.Unlock();
	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;	
}

/**********************************************************************/

bool CEposReportGeneralDorbiere::CreateExport()
{
	CString strFolder = "DORBIERE";
	GetDataProgramPath( strFolder );
	CreateSubdirectory( strFolder );
	CreateSubdirectory( strFolder + "\\EXPORT" );

	CSSFile fileExport;
	if ( fileExport.Open( strFolder + "\\EXPORT\\export.csv", "wb" ) == FALSE )
		return FALSE;
		
	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportGeneralBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.GetLineCount( m_ReportSettings );		
	}

	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if ( m_nCreateReportMiniTarget < 1 )
		m_nCreateReportMiniTarget = 1;

	StatusProgress.Lock( TRUE, "Creating export" );

	m_BlockMap.BuildSortArray();
	for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
	{
		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportGeneralBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

		int nExportCode = 0;
		
		{
			CEposReportBlockMapEntity BlockMapEntity;
			m_BlockMap.GetBlockMapEntity( n, BlockMapEntity );
			int nSpIdx = BlockMapEntity.m_nRpIdx;
			if ( ( nSpIdx >= 0 ) && ( nSpIdx < dbStockpoint.GetSize() ) )
			{
				nExportCode = dbStockpoint.GetImportSiteNo( nSpIdx );
				
				if ( nExportCode != 0 )
					nExportCode += 10000;
			}
		}

		if ( 0 == nExportCode )
			continue;

		if ( ReportBlock.GotData() == FALSE )
			continue;

		if ( FALSE == m_bCanConsolidateGlobally )
		{
			CDataManagerInfo info;
			DataManager.OpenDatabaseReadOnly( ReportBlock.m_nDbIdx, info, FALSE );
		}

		if ( ReportBlock.m_pSales -> GetSize() > 0 )
			CreateSalesSection( nExportCode, ReportBlock, &fileExport );

		CString strDummy = "";
		double dPaidInTotal = 0.0;
		double dPaidOutTotal = 0.0;
		CreatePaidIOSection( nExportCode, ReportBlock, &fileExport, strDummy, dPaidInTotal, dPaidOutTotal );
	
		double dCashTotal = 0.0;
		if ( ReportBlock.m_pPayment -> GetSize() > 0 )
			CreatePaymentSection( nExportCode, ReportBlock, &fileExport, dCashTotal );
	
		CreateCashSummary( nExportCode, ReportBlock, &fileExport, dCashTotal, dPaidInTotal, dPaidOutTotal );
	}

	StatusProgress.Unlock();
	return TRUE;	
}

/**********************************************************************/

const char* CEposReportGeneralDorbiere::GetReportNameInternal( int nReportType )
{
	m_strReportName = "Dorbiere Sales Export Preview";
	return m_strReportName;
}

/**********************************************************************/

void CEposReportGeneralDorbiere::CreateSalesSection( int nExportCode, CEposReportGeneralBlock& Block, CSSFile* pExportFile )
{
	CEntityTreeGeneralSalesNode BlockTotalNode;
	Block.m_pSales -> GetGrandTotalNode( BlockTotalNode );
	
	int nBranchSize = Block.m_pSales -> GetBranchSize(0);
	if ( nBranchSize == 0 )
		return;

	CGeneralReportSalesBlockEntity ItemEntity;
	ItemEntity.m_nEntityType = NODE_DEPT_GROUP_EPOS;
	
	for ( int nLeafIdx = 0; nLeafIdx < nBranchSize; nLeafIdx++ )
	{
		UpdateCreateReportProgress();

		CEntityTreeGeneralSalesNode CurrentNode;
		Block.m_pSales -> GetNode( 0, nLeafIdx, CurrentNode );
		
		int nLeafData = Block.m_pSales ->  GetLabelInt( 0, nLeafIdx );
		if (1000 == nLeafData)
		{
			nLeafData = 0;
		}

		ItemEntity.m_nEntityNum = nLeafData;
		GetGroupNumberAndName( ItemEntity );
		
		if ( NULL == pExportFile )
		{
			CCSV csv( '\t' );
			
			AppendLineTotals( csv, CurrentNode.m_dQty, CurrentNode.m_dVal, FALSE, TRUE, FALSE );
				
			CString strLine = "";
			strLine += ItemEntity.m_strHeader;
			strLine += "\t";
			strLine += csv.GetLine();
			m_ReportFile.WriteReportDataLine( strLine );
		}
		else
		{
			CCSV csvName( ItemEntity.m_strHeader, '\t' );

			CCSV csv( ',' );
			csv.Add( nExportCode );
			csv.Add( "Sales" );
			csv.Add( csvName.GetString(1) );
			csv.Add( CurrentNode.m_dVal[0], SysInfo.GetDPValue() );
			pExportFile -> WriteLine( csv.GetLine() );
		}
	}

	if ( NULL == pExportFile )
	{
		CCSV csv( '\t' );
		AppendLineTotals( csv, BlockTotalNode.m_dQty, BlockTotalNode.m_dVal, FALSE, TRUE, FALSE );
		
		CString strTotal = "\tTotal\t";
		strTotal += csv.GetLine();		
		m_ReportFile.WriteReportMiscLine( m_strUnderline );
		m_ReportFile.WriteReportMiscLine( strTotal );
	}
	else
	{
		CCSV csv( ',' );
		csv.Add( nExportCode );
		csv.Add( "Sales" );
		csv.Add( "Total" );
		csv.Add( BlockTotalNode.m_dVal[0], SysInfo.GetDPValue() );
		pExportFile -> WriteLine( csv.GetLine() );
	}
}

/**********************************************************************/

void CEposReportGeneralDorbiere::CreatePaymentSection( int nExportCode, CEposReportGeneralBlock& Block, CSSFile* pExportFile, double& dDorbiereCashTotal )
{
	dDorbiereCashTotal = 0.0;

	if (Block.m_pSales->GetSize() > 0)
	{
		m_ReportFile.RequestBlankLines(1);
	}

	for ( int p = 0; p < Block.m_pPayment -> GetSize(); p++ )
	{
		CEposReportConsolGeneralMisc Payment;
		Block.m_pPayment -> GetAt( p, Payment );
		UpdateCreateReportProgress();

		if ( GENREP_PAYMENT_TOTAL == Payment.m_nLineType )
		{
			if ( NULL == pExportFile )
			{
				m_ReportFile.WriteReportMiscLine( m_strUnderline );
				CCSV csv( '\t' );
				csv.Add( "" );
				csv.Add( "Total" );
				AppendLineTotals( csv, Payment.m_dQty, Payment.m_dValue, FALSE, FALSE, FALSE );
				m_ReportFile.WriteReportMiscLine( csv.GetLine() );
			}
			else
			{
				CCSV csv( ',' );
				csv.Add( nExportCode );
				csv.Add( "Payment" );
				csv.Add( "Total" );
				csv.Add( Payment.m_dValue[0], SysInfo.GetDPValue() );
				pExportFile -> WriteLine( csv.GetLine() );
			}
		}
		else
		{
			CString strPayType = "";

			switch( Payment.m_nLineData )
			{
			case 0:		
				strPayType = "Cash";
				dDorbiereCashTotal += Payment.m_dValue[0];
				break;

			case 1:		
				strPayType = "Credit Card";	
				break;
			
			default:	
				strPayType = "Other";			
				break;
			}

			if ( NULL == pExportFile )
			{
				CCSV csv( '\t' );
				csv.Add( "" );
				csv.Add( strPayType );
				AppendLineTotals( csv, Payment.m_dQty, Payment.m_dValue, FALSE, FALSE, FALSE );
				m_ReportFile.WriteReportDataLine( csv.GetLine() );
			}
			else
			{
				CCSV csv( ',' );
				csv.Add( nExportCode );
				csv.Add( "Payment" );
				csv.Add( strPayType );
				csv.Add( Payment.m_dValue[0], SysInfo.GetDPValue() );
				pExportFile -> WriteLine( csv.GetLine() );
			}
		}
	}
}

/**********************************************************************/

void CEposReportGeneralDorbiere::CreatePaidIOSection( int nExportCode, CEposReportGeneralBlock& Block, CSSFile* pExportFile, CString& strHeader, double& dPaidInTotal, double& dPaidOutTotal )
{
	dPaidInTotal = 0.0;
	dPaidOutTotal = 0.0;

	for ( int nSection = 0; nSection <= 1; nSection++ )
	{
		double dSectionTotal = 0.0;

		CStringArray arraySectionBuffer;

		for ( int r = 0; r < Block.m_pRecon -> GetSize(); r++ )
		{
			CEposReportConsolGeneralMisc Recon;
			Block.m_pRecon -> GetAt( r, Recon );
			UpdateCreateReportProgress();

			CString strText = "";

			switch( Recon.m_nLineType )
			{
			case GENREP_RECON_PAYIN01:	
			case GENREP_RECON_PAYIN02:	
			case GENREP_RECON_PAYIN03:	
			case GENREP_RECON_PAYIN04:	
			case GENREP_RECON_PAYIN05:	
			case GENREP_RECON_PAYIN06:	
			case GENREP_RECON_PAYIN07:	
			case GENREP_RECON_PAYIN08:
			case GENREP_RECON_PAYIN09:
			case GENREP_RECON_PAYIN10:
			case GENREP_RECON_PAYIN11:
			case GENREP_RECON_PAYIN12:
			case GENREP_RECON_PAYIN13:
			case GENREP_RECON_PAYIN14:
			case GENREP_RECON_PAYIN15:
			case GENREP_RECON_PAYIN16:
			case GENREP_RECON_PAYIN17:
			case GENREP_RECON_PAYIN18:
			case GENREP_RECON_PAYIN19:
			case GENREP_RECON_PAYIN20:
			case GENREP_RECON_PAYINXX:
				if ( 0 == nSection )
				{
					CPaidInOutTextCSVRecord Text;
					DataManager.PaidInText.GetReportPaidInOutText( Recon.m_nLineType - 100, Text );
					strText = Text.GetEposText();
				}
				break;

			case GENREP_RECON_PAYOUT01:	
			case GENREP_RECON_PAYOUT02:	
			case GENREP_RECON_PAYOUT03:	
			case GENREP_RECON_PAYOUT04:	
			case GENREP_RECON_PAYOUT05:	
			case GENREP_RECON_PAYOUT06:	
			case GENREP_RECON_PAYOUT07:	
			case GENREP_RECON_PAYOUT08:
			case GENREP_RECON_PAYOUT09:
			case GENREP_RECON_PAYOUT10:
			case GENREP_RECON_PAYOUT11:
			case GENREP_RECON_PAYOUT12:
			case GENREP_RECON_PAYOUT13:
			case GENREP_RECON_PAYOUT14:
			case GENREP_RECON_PAYOUT15:
			case GENREP_RECON_PAYOUT16:
			case GENREP_RECON_PAYOUT17:
			case GENREP_RECON_PAYOUT18:
			case GENREP_RECON_PAYOUT19:
			case GENREP_RECON_PAYOUT20:
			case GENREP_RECON_PAYOUTXX:
				if ( 1 == nSection )	
				{
					CPaidInOutTextCSVRecord Text;
					DataManager.PaidOutText.GetReportPaidInOutText( Recon.m_nLineType - 300, Text );
					strText = Text.GetEposText();
				}				
				break;
			}

			if ( strText != "" )
			{
				if ( CPriceHelpers::CompareDoubles( Recon.m_dValue[0], 0.0, SysInfo.GetDPValue() != 0 ) )
				{
					if ( NULL == pExportFile )
					{
						CCSV csv( '\t' );
						csv.Add( "" );
						csv.Add( strText );
						AppendLineTotals( csv, Recon.m_dQty, Recon.m_dValue, TRUE, FALSE, FALSE );
						arraySectionBuffer.Add( csv.GetLine() );
						dSectionTotal += Recon.m_dValue[0];
					}
					else
					{
						CCSV csv( ',' );
						csv.Add( nExportCode );
						csv.Add( ( 0 == nSection ) ?  "PaidIn" : "PaidOut"  );
						csv.Add( strText );
						csv.Add( Recon.m_dValue[0], SysInfo.GetDPValue() );
						arraySectionBuffer.Add( csv.GetLine() );
						dSectionTotal += Recon.m_dValue[0];
					}
				}
			}
		}

		if ( arraySectionBuffer.GetSize() > 0 )
		{
			if ( 0 == nSection )
				dPaidInTotal = dSectionTotal;
			else
				dPaidOutTotal = dSectionTotal;

			if ( NULL == pExportFile )
			{
				CString strSectionHeader = strHeader;
				strSectionHeader += ( nSection == 0 ) ? ", Paid In" : ", Paid Out";
				arraySectionBuffer.InsertAt( 0, strSectionHeader );
				arraySectionBuffer.InsertAt( 1, "<LI>" );
				arraySectionBuffer.Add( m_strReconUnderline );
			}

			double dTotalQty[8];
			double dTotalVal[8];
			dTotalQty[0] = 0.0;
			dTotalVal[0] = dSectionTotal;

			if ( NULL == pExportFile )
			{
				CCSV csv( '\t' );
				csv.Add( "" );
				csv.Add( "Total" );
				AppendLineTotals( csv, dTotalQty, dTotalVal, TRUE, FALSE, FALSE );
				arraySectionBuffer.Add( csv.GetLine() );

				m_ReportFile.RequestBlankLines(2);
				for (int n = 0; n < arraySectionBuffer.GetSize(); n++)
				{
					m_ReportFile.WriteReportDataLine(arraySectionBuffer.GetAt(n));
				}
			}
			else
			{
				CCSV csv( ',' );
				csv.Add( nExportCode );
				csv.Add( ( 0 == nSection ) ?  "PaidIn" : "PaidOut" );
				csv.Add( "Total" );
				csv.Add( dTotalVal[0], SysInfo.GetDPValue() );
				arraySectionBuffer.Add( csv.GetLine() );

				for (int n = 0; n < arraySectionBuffer.GetSize(); n++)
				{
					pExportFile->WriteLine(arraySectionBuffer.GetAt(n));
				}
			}
		}
	}
}

/**********************************************************************/

void CEposReportGeneralDorbiere::CreateCashSummary( int nExportCode, CEposReportGeneralBlock& Block, CSSFile* pExportFile, double dCashTotal, double dPaidIn, double dPaidOut )
{
	if ( NULL == pExportFile )
	{
		double dQty[8];
		double dVal[8];

		{
			dVal[0] = dCashTotal;

			CCSV csv( '\t' );
			csv.Add( "" );
			csv.Add( "Cash Sales" );
			AppendLineTotals( csv, dQty, dVal, FALSE, FALSE, FALSE );
			m_ReportFile.WriteReportDataLine( csv.GetLine() );
		}

		m_ReportFile.WriteReportMiscLine( m_strUnderline );

		{
			dVal[0] = dPaidIn;

			CCSV csv( '\t' );
			csv.Add( "" );
			csv.Add( "Paid In" );
			AppendLineTotals( csv, dQty, dVal, FALSE, FALSE, FALSE );
			m_ReportFile.WriteReportMiscLine( csv.GetLine() );
		}

		{
			dVal[0] = dPaidOut;

			CCSV csv( '\t' );
			csv.Add( "" );
			csv.Add( "Paid Out" );
			AppendLineTotals( csv, dQty, dVal, FALSE, FALSE, FALSE );
			m_ReportFile.WriteReportDataLine( csv.GetLine() );
		}

		m_ReportFile.WriteReportMiscLine( m_strUnderline );

		{
			dVal[0] = dCashTotal + dPaidIn - dPaidOut;

			CCSV csv( '\t' );
			csv.Add( "" );
			csv.Add( "Net Cash" );
			AppendLineTotals( csv, dQty, dVal, FALSE, FALSE, FALSE );
			m_ReportFile.WriteReportMiscLine( csv.GetLine() );
		}
	}
	else
	{
		CCSV csv( ',' );
		csv.Add( nExportCode );
		csv.Add( "NetCash" );
		csv.Add( "Total" );
		csv.Add( dCashTotal + dPaidIn - dPaidOut, SysInfo.GetDPValue() );
		pExportFile -> WriteLine( csv.GetLine() );
	}
}

/**********************************************************************/
