/**********************************************************************/
 
/**********************************************************************/
//#include "DataManagerNonDb.h"
/**********************************************************************/
#include "StockReportSortOrderEditDlg.h"
/**********************************************************************/

CStockReportSortOrderEditDlg::CStockReportSortOrderEditDlg( CStockReportSortOrderCSVRecord& Record, bool bAdhoc, CWnd* pParent)
	: CSalesHistoryCSVRecordEditDlg(pParent), m_StockReportSortOrder( Record )
{
	m_bAdhoc = bAdhoc;
	m_bChart = FALSE;
}

/**********************************************************************/

const char* CStockReportSortOrderEditDlg::GetName()
{
	m_strName = m_StockReportSortOrder.GetActualName();
	return m_strName;
}

/**********************************************************************/

const char* CStockReportSortOrderEditDlg::GetTitle()
{
	CString strName = "";
	CStockReportSortOrderCSVArray::GetReportName( DataManagerNonDb.GetActiveStockReportSortOrderType(), strName );

	m_strTitle = "Setup ";
	m_strTitle += ( m_bAdhoc ) ? "Adhoc " : "Custom ";
	m_strTitle += "Sort Order for ";
	m_strTitle += strName;
	m_strTitle += " Report";
		

	return m_strTitle;
}

/**********************************************************************/

void CStockReportSortOrderEditDlg::PrepareCombos( CWordArray& validFields, CWordArray& sortorderFields )
{	
	m_comboType.ShowWindow( SW_HIDE );
	m_comboType.EnableWindow( FALSE );

	validFields.RemoveAll();
	sortorderFields.RemoveAll();

	m_StockReportSortOrder.GetValidFieldList( validFields );	
	m_StockReportSortOrder.GetSortOrderFieldsInternal( sortorderFields );	
	
	for ( int nIndex = 0; nIndex < 8; nIndex++ )
	{
		int n = 0;
		m_comboField[nIndex].ResetContent();
		m_comboField[nIndex].AddString( " -- -- " );
		m_comboField[nIndex].SetItemData( n++, 0 );

		for ( int x = 0; x < validFields.GetSize(); x++ )
		{
			int nField = validFields.GetAt(x);

			bool bSkip = FALSE;

			if ( nIndex != 0 )
			{
				switch( DataManagerNonDb.GetActiveStockReportSortOrderType() )
				{
				case STOCKREPORT_SORTORDER_SUPPLIER:
				case STOCKREPORT_SORTORDER_REORDER:
					bSkip = ( SH_FIELD_SUPP == nField );
					break;
				}
			}

			if ( FALSE == bSkip )
			{
				m_comboField[nIndex].AddString ( m_StockReportSortOrder.GetFieldListName( nField ) );	
				m_comboField[nIndex].SetItemData( n++, nField );
			}
		}
	
		m_comboField[nIndex].SetCurSel(0);
		m_checkTotal[nIndex].EnableWindow( FALSE );
		m_checkHeader[nIndex].EnableWindow( FALSE );
	}
}

/**********************************************************************/

void CStockReportSortOrderEditDlg::FillCombos()
{
	CSalesHistoryCSVRecordEditDlg::FillCombos();
	CheckSupplierField();
}

/**********************************************************************/

void CStockReportSortOrderEditDlg::SelectField()
{
	CSalesHistoryCSVRecordEditDlg::SelectField();
	CheckSupplierField();
}

/**********************************************************************/

void CStockReportSortOrderEditDlg::CheckSupplierField()
{
	switch( DataManagerNonDb.GetActiveStockReportSortOrderType() )
	{
	case STOCKREPORT_SORTORDER_SUPPLIER:
	case STOCKREPORT_SORTORDER_REORDER:
		{
			int nSel = m_comboField[0].GetCurSel();
			if ( ( nSel >= 0 ) && ( nSel < m_comboField[0].GetCount() ) )
			{
				int nItemData = m_comboField[0].GetItemData( nSel );

				if ( SH_FIELD_SUPP == nItemData )
				{
					m_comboField[0].EnableWindow( FALSE );
					m_checkHeader[0].EnableWindow( FALSE );
					m_checkTotal[0].EnableWindow( FALSE );
				}
			}
		}
		break;
	}
}

/**********************************************************************/

void CStockReportSortOrderEditDlg::TidyFields() 
{
	CWordArray arrayTemp;
	GetFieldSettings( arrayTemp );
	m_StockReportSortOrder.SetSortOrderFields( arrayTemp );
	FillCombos();
}

/**********************************************************************/

void CStockReportSortOrderEditDlg::ProcessChanges()
{
	::TrimSpaces ( m_strName, FALSE );
	if ( m_strName == "" ) m_strName = "Custom Report";

	CWordArray arrayTemp;
		
	//GET CURRENT FIELD SETTINGS FROM CONTROLS
	GetFieldSettings( arrayTemp );	
	m_StockReportSortOrder.SetSortOrderFields( arrayTemp );
		
	//TIDY THEM UP AND GET THEM AGAIN
	FillCombos();					
	GetFieldSettings( arrayTemp );
	m_StockReportSortOrder.SetSortOrderFields( arrayTemp );
	m_StockReportSortOrder.SetName ( m_strName );
}

/**********************************************************************/

