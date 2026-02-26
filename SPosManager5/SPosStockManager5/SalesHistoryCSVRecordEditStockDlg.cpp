/**********************************************************************/
 
/**********************************************************************/
//#include "globalfunctions.h"
/**********************************************************************/
#include "SalesHistoryCSVRecordEditStockDlg.h"
/**********************************************************************/

CSalesHistoryCSVRecordEditStockDlg::CSalesHistoryCSVRecordEditStockDlg( CSalesHistoryCSVRecordStock& Record, bool bAdhoc, CWnd* pParent)
	: CSalesHistoryCSVRecordEditDlg(pParent), m_SalesHistoryStock( Record )
{
	m_bAdhoc = bAdhoc;
}

/**********************************************************************/

const char* CSalesHistoryCSVRecordEditStockDlg::GetName()
{
	m_strName = m_SalesHistoryStock.GetActualName();
	return m_strName;
}

/**********************************************************************/

const char* CSalesHistoryCSVRecordEditStockDlg::GetTitle()
{
	m_strTitle = "Setup ";
	m_strTitle += ( m_bAdhoc ) ? "Adhoc " : "Custom ";
	m_strTitle += "Stock Sales History Report";
	return m_strTitle;
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditStockDlg::PrepareCombos( CWordArray& validFields, CWordArray& reportFields )
{	
	validFields.RemoveAll();
	reportFields.RemoveAll();

	m_SalesHistoryStock.GetValidFieldList( validFields );	
	m_SalesHistoryStock.GetReportFields( reportFields );	
	
	for ( int nIndex = 0; nIndex < 8; nIndex++ )
	{
		int n = 0;
		m_comboField[nIndex].ResetContent();
		m_comboField[nIndex].AddString( " -- -- " );
		m_comboField[nIndex].SetItemData( n++, 0 );
		
		for ( int x = 0; x < validFields.GetSize(); x++ )
		{
			int nField = validFields.GetAt(x);
			m_comboField[nIndex].AddString ( m_SalesHistoryStock.GetFieldListName( nField ) );	
			m_comboField[nIndex].SetItemData( n++, nField );
		}
	
		m_comboField[nIndex].SetCurSel(0);
		m_checkTotal[nIndex].EnableWindow( FALSE );
		m_checkHeader[nIndex].EnableWindow( FALSE );
	}

	if ( m_SalesHistoryStock.GetReportMode() == SALESHISTORY_REPORTMODE_CHART )
		for ( int nIndex = 0; nIndex < 8; nIndex++ )
			m_checkTotal[nIndex].EnableWindow(FALSE);

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(120);
	m_comboType.SetTabStops( arrayStops );
	
	m_comboType.ResetContent();
	m_comboType.AddItem( "Normal" );
	m_comboType.AddItem( "Top 100" );
	m_comboType.AddItem( "Weekday,(Value)" );
	m_comboType.AddItem( "Weekday,(Quantity)" );
	m_comboType.AddItem( "Monthday,(Value)" );
	m_comboType.AddItem( "Monthday,(Quantity)" );

	int nSel = 0;
	switch( m_SalesHistoryStock.GetReportMode() )
	{
	case SALESHISTORY_REPORTMODE_CHART:				nSel = 1;	break;
	case SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL:	nSel = 2;	break;
	case SALESHISTORY_REPORTMODE_WEEKLY_QTY:		nSel = 3;	break;
	case SALESHISTORY_REPORTMODE_MONTHLY_VAL_FULL:	nSel = 4;	break;
	case SALESHISTORY_REPORTMODE_MONTHLY_QTY:		nSel = 5;	break;
	}
	m_comboType.SetCurSel( nSel );
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditStockDlg::UpdateReportModeFromCombo()
{
	int nType = 0;
	switch( m_comboType.GetCurSel() )
	{
	case 1:	nType = SALESHISTORY_REPORTMODE_CHART;				break;
	case 2:	nType = SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL;	break;
	case 3:	nType = SALESHISTORY_REPORTMODE_WEEKLY_QTY;			break;
	case 4: nType = SALESHISTORY_REPORTMODE_MONTHLY_VAL_FULL;	break;
	case 5: nType = SALESHISTORY_REPORTMODE_MONTHLY_QTY;		break;
	}
	m_SalesHistoryStock.SetReportMode( nType );
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditStockDlg::TidyFields() 
{
	CWordArray arrayTemp;
	GetFieldSettings( arrayTemp );
	m_SalesHistoryStock.SetReportFields( arrayTemp );
	FillCombos();
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditStockDlg::ProcessChanges()
{
	::TrimSpaces ( m_strName, FALSE );
	if ( m_strName == "" ) m_strName = "Custom Report";

	CWordArray arrayTemp;
		
	//GET CURRENT FIELD SETTINGS FROM CONTROLS
	GetFieldSettings( arrayTemp );	
	m_SalesHistoryStock.SetReportFields( arrayTemp );
	UpdateReportModeFromCombo();
		
	//TIDY THEM UP AND GET THEM AGAIN
	FillCombos();					
	GetFieldSettings( arrayTemp );
	m_SalesHistoryStock.SetReportFields( arrayTemp );
	UpdateReportModeFromCombo();
	m_SalesHistoryStock.SetName ( m_strName );
}

/**********************************************************************/

