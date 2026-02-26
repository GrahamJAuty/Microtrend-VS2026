/**********************************************************************/
#include "NodeTypes.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportPluByPriceBandSummary.h"
/**********************************************************************/

CEposReportPluByPriceBandSummary::CEposReportPluByPriceBandSummary( CEposSelectArray& SelectArray, CString strCustomSettings ) : CEposReportPluByPriceBandBase( SelectArray, strCustomSettings )
{
	m_nServerBlockIdxStart = 0;

	switch (m_ReportSettings.GetReportSubType())
	{
	case EPOSPRICEBANDREPORT_VIP:
		m_bAllowValue = EcrmanOptions.GetReportsVIPRetailFlag();
		m_bDiscountDetail = m_bAllowValue && EcrmanOptions.GetReportsDiscountDetailSalesFlag();
		m_bPremiumDetail = m_bAllowValue && EcrmanOptions.AllowPremiumDetailSales();
		m_bVIPReport = TRUE;
		break;

	case EPOSPRICEBANDREPORT_WASTE:
		m_bAllowValue = TRUE;
		m_bDiscountDetail = FALSE;
		m_bPremiumDetail = FALSE;
		m_bVIPReport = FALSE;
		break;

	case EPOSPRICEBANDREPORT_NOZEROSKIP:
		m_bAllowValue = TRUE;
		m_bDiscountDetail = EcrmanOptions.GetReportsDiscountDetailSalesFlag();
		m_bPremiumDetail = EcrmanOptions.AllowPremiumDetailSales();
		m_bVIPReport = FALSE;
		break;

	case EPOSPRICEBANDREPORT_PLUSALES:
	default:
		m_bAllowValue = TRUE;
		m_bDiscountDetail = EcrmanOptions.GetReportsDiscountDetailSalesFlag();
		m_bPremiumDetail = EcrmanOptions.AllowPremiumDetailSales();
		m_bVIPReport = FALSE;
		break;
	}

	m_bNetOfTax = EcrmanOptions.GetReportsNetOfTaxSalesFlag();
}

/**********************************************************************/

void CEposReportPluByPriceBandSummary::InitialiseConsolidationBlocks()
{
	CEposReportPluByPriceBandBase::InitialiseConsolidationBlocksStageOne();
	CEposReportPluByPriceBandBase::InitialiseConsolidationBlocksStageTwo();
}

/**********************************************************************/

bool CEposReportPluByPriceBandSummary::CreateReport()
{
	if (m_ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return FALSE;
	}

	CDataManagerInfo info;
	DataManager.OpenDatabaseQuickNoFiles(0, TRUE);
	DataManager.OpenPriceText(DB_READONLY, info);

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	AddReportColumn ( "Plu Number", TA_LEFT, 350, FALSE );
	AddReportColumn ( "Description", TA_LEFT, 350, FALSE );
	AddReportColumn ( "Dept", TA_RIGHT, 200, FALSE );
	
	for (int n = 0; n < MAX_PLU_PRICES; n++)
	{
		CString strPrice = "";

		if (m_ReportSettings.GetColumnNamesFlag() == FALSE)
		{
			strPrice.Format("Price%d", n + 1);
		}
		else
		{
			strPrice = DataManager.PriceText.GetReportText(n);
		}

		if (m_ReportSettings.IsPriceBandEnabled(n) == TRUE)
		{
			AddReportColumn(strPrice + "Qty", TA_RIGHT, 250, TRUE);

			if (TRUE == m_bAllowValue)
			{
				AddReportColumn(strPrice + "Val", TA_RIGHT, 250, TRUE);
			}
		}
	}

	DataManager.ClosePriceText(info);

	AddReportColumn("TotalQty", TA_RIGHT, 250, TRUE);

	if ( TRUE == m_bAllowValue )
	{
		if (IsWastageReport() == TRUE)
		{
			AddReportColumn("Waste", TA_RIGHT, 250, TRUE);
		}
		else
		{
			AddReportColumn ( "Sales", TA_RIGHT, 250, TRUE );

			if (TRUE == m_bDiscountDetail)
			{
				AddReportColumn("Discount", TA_RIGHT, 250, TRUE);

				if (TRUE == m_bPremiumDetail)
				{
					AddReportColumn("Premium", TA_RIGHT, 250, TRUE);
				}

				AddReportColumn ( "Retail", TA_RIGHT, 250, TRUE );
			}
		}

		AddReportColumn ( "Average", TA_RIGHT, 250, TRUE );
		AddReportColumn ( EcrmanOptions.GetTaxBandString(), TA_RIGHT, 200, FALSE );
		AddReportColumn ( EcrmanOptions.GetEstTaxString(), TA_RIGHT, 250, TRUE );

		if (TRUE == m_bNetOfTax)
		{
			AddReportColumn("Est. Net", TA_RIGHT, 250, TRUE);
		}
	}

	AddReportColumn ( "Est. Cost", TA_RIGHT, 250, TRUE );

	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportPluByPriceBandBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.m_pSales -> GetSize();
	}
	
	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if (m_nCreateReportMiniTarget < 1)
	{
		m_nCreateReportMiniTarget = 1;
	}

	StatusProgress.Lock( TRUE, "Creating report" );

	int nTotalBlocks = 0;
	m_BlockMap.BuildSortArray();

	m_ReportFilters.ResetDbIdx();
	for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
	{
		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportPluByPriceBandBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

		if (ReportBlock.GotData() == FALSE)
		{
			continue;
		}

		if (TRUE == ReportBlock.m_bAddToTotal)
		{
			nTotalBlocks++;
		}

		m_ReportFilters.SetDatabase( ReportBlock.m_nDbIdx );
		
		m_ReportFile.HandleNewPage();
	
		CString strHeader;
		ReportBlock.GetHeaderText( strHeader );
		m_ReportFile.WriteReportMiscLine( strHeader );
		m_ReportFile.WriteReportMiscLine( "<LI>" );
	
		CreateSalesSection( ReportBlock );
		WriteBlockTotals( ReportBlock );
	}

	StatusProgress.Unlock();

	if ( nTotalBlocks >= 2 )
	{
		m_ReportFile.RequestBlankLines(2);
		m_ReportFile.WriteReportMiscLine( "<..>Grand Total" );
		m_ReportFile.WriteReportMiscLine( "<LI>" );
		WriteConsolidatedTotals( m_Totals1, "Grand", 0, TRUE );
	}

	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/