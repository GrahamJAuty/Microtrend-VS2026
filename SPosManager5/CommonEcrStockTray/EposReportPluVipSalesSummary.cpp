/**********************************************************************/
#include "NodeTypes.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportPluVipSalesSummary.h"
/**********************************************************************/

CEposReportPluVipSalesSummary::CEposReportPluVipSalesSummary( CEposSelectArray& SelectArray, int nReportType ) : CEposReportPluVipSalesBase( SelectArray )
{
	m_nServerBlockIdxStart = 0;

	switch( nReportType )
	{
	case REPORT_TYPE_VIPSALES:
		m_nReportType = nReportType;
		m_bAllowValue = EcrmanOptions.GetReportsVIPRetailFlag();
		m_bDiscountDetail = m_bAllowValue && EcrmanOptions.GetReportsDiscountDetailSalesFlag();
		m_bPremiumDetail = m_bAllowValue && EcrmanOptions.AllowPremiumDetailSales();
		m_bVIPReport = TRUE;	
		break;

	case REPORT_TYPE_PLUWASTE:
		m_nReportType = nReportType;
		m_bAllowValue = TRUE;
		m_bDiscountDetail = FALSE;
		m_bPremiumDetail = FALSE;
		m_bVIPReport = FALSE;	
		break;

	case REPORT_TYPE_PLUSALES_NOZEROSKIP:
		m_nReportType = nReportType;
		m_bAllowValue = TRUE;
		m_bDiscountDetail = EcrmanOptions.GetReportsDiscountDetailSalesFlag();
		m_bPremiumDetail = EcrmanOptions.AllowPremiumDetailSales();
		m_bVIPReport = FALSE;	
		break;

	case REPORT_TYPE_PLUSALES:
	default:
		m_nReportType = REPORT_TYPE_PLUSALES;
		m_bAllowValue = TRUE;
		m_bDiscountDetail = EcrmanOptions.GetReportsDiscountDetailSalesFlag();
		m_bPremiumDetail = EcrmanOptions.AllowPremiumDetailSales();
		m_bVIPReport = FALSE;	
		break;
	}

	m_bNetOfTax = EcrmanOptions.GetReportsNetOfTaxSalesFlag();
}

/**********************************************************************/

void CEposReportPluVipSalesSummary::InitialiseConsolidationBlocks()
{
	CEposReportPluVipSalesBase::InitialiseConsolidationBlocksStageOne();
	CEposReportPluVipSalesBase::InitialiseConsolidationBlocksStageTwo();
}

/**********************************************************************/

bool CEposReportPluVipSalesSummary::CreateReport()
{
	if (m_ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return FALSE;
	}

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	AddReportColumn ( "Plu Number", TA_LEFT, 350, FALSE );
	AddReportColumn ( "Description", TA_LEFT, 350, FALSE );
	AddReportColumn ( "Dept", TA_RIGHT, 200, FALSE );
	AddReportColumn ( "Quantity", TA_RIGHT, 250, TRUE );

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
		CEposReportPluSalesBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
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
		CEposReportPluSalesBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

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
		WriteConsolidatedTotals( m_arrayTotals1, "Grand", 0, TRUE );
	}

	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/