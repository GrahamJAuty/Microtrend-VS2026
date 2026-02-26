/**********************************************************************/
 
/**********************************************************************/
//#include "EcrmanOptionsIni.h"
#include "NodeTypes.h"
 
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportPaymentReportGroup.h"
/**********************************************************************/

CEposReportPaymentReportGroup::CEposReportPaymentReportGroup( CEposSelectArray& SelectArray ) : CEposReportPaymentBase( SelectArray )
{
	m_nReportType = REPORT_TYPE_PAYMENT_GROUP_SUMMARY;
}

/**********************************************************************/

void CEposReportPaymentReportGroup::InitialiseConsolidationBlocks()
{
	CEposReportPaymentBase::InitialiseConsolidationBlocksStageOne();
	CEposReportPaymentBase::InitialiseConsolidationBlocksStageTwo();
}

/**********************************************************************/

bool CEposReportPaymentReportGroup::CheckGraph()
{
	if ( ( EcrmanOptions.GetGlobalPaymentFlag() == FALSE ) && ( NODE_SYSTEM == m_nConLevel ) )
	{
		Prompter.Error( "You cannot create a graph of this report across multiple databases\n\nwhen payment methods are database specific." );
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

bool CEposReportPaymentReportGroup::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Type", TA_LEFT, 120 );
	m_ReportFile.AddColumn ( "Name", TA_LEFT, 350 );

	if ( EcrmanOptions.GetReportsShowPaymentQuantityFlag() == TRUE )
		m_ReportFile.AddColumn ( "Qty", TA_RIGHT, 200 );
	
	m_ReportFile.AddColumn ( "Tender", TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( "Amount", TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( "Gratuity", TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( "Cashback", TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( "Surplus", TA_RIGHT, 200 );

	if ( EcrmanOptions.GetReportsShowCashChangeFlag() == TRUE )
		m_ReportFile.AddColumn ( "Change", TA_RIGHT, 200 );

	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportPaySumBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.GetLineCount();
	}
	
	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if ( m_nCreateReportMiniTarget < 1 )
		m_nCreateReportMiniTarget = 1;

	StatusProgress.Lock( TRUE, "Creating report" );

	m_BlockMap.BuildSortArray();
	for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
	{
		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportPaySumBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

		if ( ( ReportBlock.GotData() == FALSE ) && ( NODE_SYSTEM != ReportBlock.m_nEntityType ) )
			continue;

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( ReportBlock.m_nDbIdx, info, FALSE );
		
		m_ReportFile.HandleNewPage();

		CString strHeader;
		ReportBlock.GetHeaderText( strHeader );
		m_ReportFile.WriteReportMiscLine( strHeader );
		m_ReportFile.WriteReportMiscLine( "<LI>" );
		m_ReportFile.LockBlankLines(1);

		CreateSalesSection( ReportBlock );		
	}

	StatusProgress.Unlock();
	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/

bool CEposReportPaymentReportGroup::CreateGraph()
{
	CSSFile fileGraph;
	if ( fileGraph.Open( Super.ReportFilename(), "wb" ) == FALSE )
		return FALSE;

	CCSV csvOut;
	csvOut.Add( "Payment Type" );
	csvOut.Add( "Sales Value" );
	csvOut.Add( GetReportNameInternal( m_nReportType ) );
	csvOut.Add( GetReportTitle(TRUE) );
	fileGraph.WriteLine( csvOut.GetLine() );
	
	StatusProgress.Lock( TRUE, "Creating graph" );

	CEposReportPaySumBlock ReportBlock = m_arrayReportBlocks.GetAt( 0 );

	int nSize = ReportBlock.m_pNet -> GetSize();

	for ( int i = 0 ; i < nSize ; i++ )
	{
		StatusProgress.SetPos( i, nSize );

		CEposReportConsolPaySumOneVal ConsolInfo;
		ReportBlock.m_pNet -> GetAt( i, ConsolInfo );

		CCSV csvOut;
		csvOut.Add ( DataManager.Payment.GetDisplayName( ConsolInfo.m_nSubType ) );
		csvOut.Add ( ConsolInfo.GetReportValue(), SysInfo.GetDPValue() );
		fileGraph.WriteLine( csvOut.GetLine() );
	}
	StatusProgress.Unlock();
	return TRUE;
}

/**********************************************************************/

bool CEposReportPaymentReportGroup::WritePaymentLines( CReportConsolidationArray<CEposReportConsolPaySum>* pPayments, CEposReportConsolPaySum& Totals, bool bShowNonPTR, bool bShowPTR, bool bConsolidate )
{
	bool bResult = FALSE;

	int nLastReportGroup = -1;
	CEposReportConsolPaySum ReportGroupTotals;

	for ( int nPos = 0; nPos < pPayments -> GetSize(); nPos++ )
	{
		UpdateCreateReportProgress();

		CEposReportConsolPaySum ConsolInfo;
		pPayments -> GetAt( nPos, ConsolInfo );
		
		int nPaymentNo = ConsolInfo.m_nSubType;

		bool bShow = TRUE;
		if ( ( FALSE == bShowPTR ) || ( FALSE == bShowNonPTR ) )
		{
			int nPayIdx;
			if ( DataManager.Payment.FindPaymentByNumber( nPaymentNo, nPayIdx ) == TRUE )
			{
				CPaymentCSVRecord Payment;
				DataManager.Payment.GetAt( nPayIdx, Payment );

				switch( Payment.GetPaymentType() )
				{
				case PAYMENT_TYPE_ROOM_AUTO:
				case PAYMENT_TYPE_ROOM_MANUAL:
					bShow = bShowPTR;
					break;

				default:
					bShow = bShowNonPTR;
					break;
				}
			}
			else
				bShow = bShowNonPTR;
		}
	
		if ( TRUE == bShow )
		{
			bResult = TRUE;

			if ( TRUE == bConsolidate ) 
				Totals.Add( ConsolInfo );

			//SHOW SUBTOTAL FOR PREVIOUS REPORT GROUP
			if ( ConsolInfo.m_nReportGroup != nLastReportGroup )
			{
				if ( nLastReportGroup != -1 )
				{
					CString strTotalText;
					GetTotalText( nLastReportGroup, strTotalText );
					WriteTotals ( strTotalText, ReportGroupTotals );
					m_ReportFile.RequestBlankLines(1);
				}

				ReportGroupTotals.Reset();
				nLastReportGroup = ConsolInfo.m_nReportGroup;
			}

			int nDP = SysInfo.GetDPValue(); 

			CCSV csv ( '\t' );
			csv.Add ( nPaymentNo );
			csv.Add ( DataManager.Payment.GetDisplayName( nPaymentNo ) );

			if ( EcrmanOptions.GetReportsShowPaymentQuantityFlag() == TRUE )
				csv.Add( ConsolInfo.m_nQty );
			
			csv.Add ( ConsolInfo.GetReportTender(), nDP );
			csv.Add ( ConsolInfo.GetReportAmount(), nDP );
			csv.Add ( ConsolInfo.GetReportGratuity(), nDP );
			csv.Add ( ConsolInfo.GetReportCashback(), nDP );
			csv.Add ( ConsolInfo.GetReportSurplus(), nDP );
			
			if (EcrmanOptions.GetReportsShowCashChangeFlag() == TRUE)
			{
				csv.Add(ConsolInfo.GetReportChange(), nDP);
			}

			m_ReportFile.WriteReportDataLine ( csv.GetLine() );
			ReportGroupTotals.Add( ConsolInfo );
		}
	}

	if ( nLastReportGroup != -1 )
	{
		CString strTotalText;
		GetTotalText( nLastReportGroup, strTotalText );
		WriteTotals ( strTotalText, ReportGroupTotals );
		m_ReportFile.RequestBlankLines(1);
	}

	return bResult;
}

/**********************************************************************/

bool CEposReportPaymentReportGroup::WriteNetLines( CReportConsolidationArray<CEposReportConsolPaySumOneVal>* pPayments, CEposReportConsolPaySumOneVal& Totals, bool bShowNonPTR, bool bShowPTR, bool bConsolidate )
{
	bool bResult = FALSE;

	int nLastReportGroup = -1;
	CEposReportConsolPaySumOneVal ReportGroupTotals;

	for ( int nPos = 0; nPos < pPayments -> GetSize(); nPos++ )
	{
		UpdateCreateReportProgress();

		CEposReportConsolPaySumOneVal ConsolInfo;
		pPayments -> GetAt( nPos, ConsolInfo );
		
		int nPaymentNo = ConsolInfo.m_nSubType;

		bool bShow = TRUE;
		if ( ( FALSE == bShowPTR ) || ( FALSE == bShowNonPTR ) )
		{
			int nPayIdx;
			if ( DataManager.Payment.FindPaymentByNumber( nPaymentNo, nPayIdx ) == TRUE )
			{
				CPaymentCSVRecord Payment;
				DataManager.Payment.GetAt( nPayIdx, Payment );

				switch( Payment.GetPaymentType() )
				{
				case PAYMENT_TYPE_ROOM_AUTO:
				case PAYMENT_TYPE_ROOM_MANUAL:
					bShow = bShowPTR;
					break;

				default:
					bShow = bShowNonPTR;
					break;
				}
			}
			else
				bShow = bShowNonPTR;
		}
	
		if ( TRUE == bShow )
		{
			bResult = TRUE;

			if ( TRUE == bConsolidate ) 
				Totals.Add( ConsolInfo );

			//SHOW SUBTOTAL FOR PREVIOUS REPORT GROUP
			if ( ConsolInfo.m_nReportGroup != nLastReportGroup )
			{
				if ( nLastReportGroup != -1 )
				{
					CString strTotalText;
					GetTotalText( nLastReportGroup, strTotalText );
					WriteTotals ( strTotalText, ReportGroupTotals );
					m_ReportFile.RequestBlankLines(1);
				}

				ReportGroupTotals.Reset();
				nLastReportGroup = ConsolInfo.m_nReportGroup;
			}

			int nDP = SysInfo.GetDPValue(); 

			CCSV csv ( '\t' );
			csv.Add ( nPaymentNo );
			csv.Add ( DataManager.Payment.GetDisplayName( nPaymentNo ) );

			if (EcrmanOptions.GetReportsShowPaymentQuantityFlag() == TRUE)
			{
				csv.Add("");
			}

			csv.Add ( ConsolInfo.GetReportValue(), nDP );

			m_ReportFile.WriteReportDataLine ( csv.GetLine() );
			ReportGroupTotals.Add( ConsolInfo );
		}
	}

	if ( nLastReportGroup != -1 )
	{
		CString strTotalText;
		GetTotalText( nLastReportGroup, strTotalText );
		WriteTotals ( strTotalText, ReportGroupTotals );
		m_ReportFile.RequestBlankLines(1);
	}

	return bResult;
}

/**********************************************************************/

void CEposReportPaymentReportGroup::GetTotalText( int nGroupNo, CString& strTotalText )
{
	CGroupCSVRecordPayment PaymentGroup;
	PaymentGroup.SetGroupNo(nGroupNo);
	
	int nGroupIdx;
	if ( DataManager.PaymentGroup.FindGroupByNumber( nGroupNo, nGroupIdx ) == TRUE )
		DataManager.PaymentGroup.GetAt( nGroupIdx, PaymentGroup );

	CString strGroupText = PaymentGroup.GetRepText();
	if ( strGroupText == "" )
		strGroupText = PaymentGroup.GetDefaultText();

	strTotalText.Format( "G%2.2d ", nGroupNo );
	strTotalText += strGroupText;
}

/**********************************************************************/
