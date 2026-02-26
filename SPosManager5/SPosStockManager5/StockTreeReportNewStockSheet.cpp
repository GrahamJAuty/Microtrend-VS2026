/**********************************************************************/
#include "ReportHelpers.h"
#include "SalesHistoryFields.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockTreeReportNewStockSheet.h"
/**********************************************************************/

CStockTreeReportNewStockSheet::CStockTreeReportNewStockSheet( CStockLevelTreeNew& StockLevelTree ) : CStockTreeReportNew( StockLevelTree )
{
	m_nReportParamsType = STOCK_SHEET;
	m_bSupplierName = FALSE;
	m_bSupplierRef = FALSE;
	m_nTotalisersPerLine = 0;
}

/**********************************************************************/

void CStockTreeReportNewStockSheet::PrepareArrays()
{
	CStockTreeReportNew::PrepareArrays();
	
	for ( int n = 0; n < m_arrayHeaderFlags.GetSize(); n++ )
		m_arrayHeaderFlags.SetAt( n, 0 );

	for ( int n = 0; n < m_arrayReportFields.GetSize(); n++ )
	{
		int nType = ( m_arrayReportFields.GetAt(n) ) % 100;
		if ( nType == SH_FIELD_CODE )
			m_arrayHeaderFlags.SetAt( n, 1 );
	}
}

/**********************************************************************/
	
void CStockTreeReportNewStockSheet::PrepareReport()
{
	m_strReportName = GetReportTitle();
	
	m_arrayColumnNames.RemoveAll();
	m_arrayTotalPos.RemoveAll();

	AddReportColumns();
	AddSupplierColumns( m_bSupplierName, m_bSupplierRef );
}

/**********************************************************************/

void CStockTreeReportNewStockSheet::AddReportColumns()
{
	AddColumn ( "Code", TA_LEFT, 380 );
	AddColumn ( "Plu No", TA_LEFT, 380 );
	AddColumn ( "Description", TA_LEFT, 300 );
	AddColumn ( "Category", TA_LEFT, 250 );
	AddColumn ( "Unit", TA_LEFT, 300 );
	AddColumn ( "", TA_LEFT, 1000 );
	AddColumn ( "Price1", TA_RIGHT, 180 );
	AddColumn ( "Opening", TA_RIGHT, 180 );
}

/**********************************************************************/

bool CStockTreeReportNewStockSheet::CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels )
{
	int nSubUnits = m_LabelStockRecord.GetSubUnits( m_bSubUnits );

	double dSURetail = 0.0;

	if ( GetRetailPluInfo( TRUE ) == TRUE )
	{
		if ( m_dRetailStockQty != 0.0 )
			dSURetail = m_dRetailPrice1 / m_dRetailStockQty;
	}

	csv.AppendString ( ReportHelpers.GetDisplayStockCode( m_LabelStockRecord.GetStockCode() ) );
	
	if ( m_strRetailPluNo != "" )
		csv.AppendString ( ReportHelpers.GetDisplayPluNo( _atoi64(m_strRetailPluNo) ) );
	else
		csv.AppendBlanks(1);

	csv.AppendString ( m_LabelStockRecord.GetDescription() );
	csv.AppendString ( DataManager.Category.GetText( m_LabelStockRecord.GetCategory() ) );
	csv.AppendString ( DataManager.Stock.GetStockSubUnitName( m_LabelStockRecord ) );
	csv.AppendBlanks ( 1 );
	csv.AppendVal( dSURetail );
	csv.AppendStockQty( StockLevels.GetOpeningQty(), nSubUnits );

	OutputSupplierFields( csv, m_bSupplierName, m_bSupplierRef );

	return TRUE;
}

/**********************************************************************/

