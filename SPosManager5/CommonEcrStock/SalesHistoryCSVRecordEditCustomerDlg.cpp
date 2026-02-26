/**********************************************************************/
 
/**********************************************************************/
//#include "DealerFlags.h"
//#include "globalfunctions.h"
 
/**********************************************************************/
#include "SalesHistoryCSVRecordEditCustomerDlg.h"
/**********************************************************************/

CSalesHistoryCSVRecordEditCustomerDlg::CSalesHistoryCSVRecordEditCustomerDlg( CSalesHistoryCSVRecordCustomer& Record, bool bAdhoc, CWnd* pParent)
	: CSalesHistoryCSVRecordEditDlg(pParent), m_SalesHistoryCustomer( Record )
{
	m_bAdhoc = bAdhoc;
	m_bChart = FALSE;
}

/**********************************************************************/

const char* CSalesHistoryCSVRecordEditCustomerDlg::GetName()
{
	m_strName = m_SalesHistoryCustomer.GetActualName();
	return m_strName;
}

/**********************************************************************/

const char* CSalesHistoryCSVRecordEditCustomerDlg::GetTitle()
{
	m_strTitle = "Setup ";
	m_strTitle += ( m_bAdhoc ) ? "Adhoc " : "";
	m_strTitle += "Customer Sales Report";
	return m_strTitle;
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditCustomerDlg::PrepareCombos( CWordArray& validFields, CWordArray& reportFields )
{	
	validFields.RemoveAll();
	reportFields.RemoveAll();

	m_SalesHistoryCustomer.GetValidFieldList( validFields );	
	m_SalesHistoryCustomer.GetReportFields( reportFields );	

	//TEMPORARILY COPY TOTALS TICK TO HEADER TICK
	for ( int n = 0; n < reportFields.GetSize(); n++ )
	{
		WORD w = reportFields.GetAt(n);
		
		if ( w > 100 )
			reportFields.SetAt( n, 200 + ( w % 100 ) );
	}
	
	for ( int nIndex = 0; nIndex < 8; nIndex++ )
	{
		int n = 0;
		m_comboField[nIndex].ResetContent();
		m_comboField[nIndex].AddString( " -- -- " );
		m_comboField[nIndex].SetItemData( n++, 0 );
		
		for ( int x = 0; x < validFields.GetSize(); x++ )
		{
			int nField = validFields.GetAt(x);
			m_comboField[nIndex].AddString ( m_SalesHistoryCustomer.GetFieldListName( nField ) );	
			m_comboField[nIndex].SetItemData( n++, nField );
		}
	
		m_comboField[nIndex].SetCurSel(0);
		m_checkTotal[nIndex].EnableWindow( FALSE );
		m_checkHeader[nIndex].EnableWindow( FALSE );
		m_checkTotal[nIndex].ShowWindow( SW_HIDE );
		m_checkHeader[nIndex].SetWindowText( "Total" );
	}

	m_comboType.EnableWindow( FALSE );
	m_comboType.ShowWindow( SW_HIDE );
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditCustomerDlg::TidyFields() 
{
	CWordArray arrayTemp;
	GetFieldSettings( arrayTemp );
	m_SalesHistoryCustomer.SetReportFields( arrayTemp );
	FillCombos();
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditCustomerDlg::ProcessChanges()
{
	::TrimSpaces ( m_strName, FALSE );
	if ( m_strName == "" ) m_strName = "Custom Report";

	CWordArray arrayTemp;
		
	//GET CURRENT FIELD SETTINGS FROM CONTROLS
	GetFieldSettings( arrayTemp );	
	m_SalesHistoryCustomer.SetReportFields( arrayTemp );
		
	//TIDY THEM UP AND GET THEM AGAIN
	FillCombos();					
	GetFieldSettings( arrayTemp );

	//COPY HEADER TICKS BACK TO TOTAL TICKS
	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		WORD w = arrayTemp.GetAt(n);
		
		if ( w > 100 )
			arrayTemp.SetAt( n, 100 + ( w % 100 ) );
	}
	
	m_SalesHistoryCustomer.SetReportFields( arrayTemp );
	m_SalesHistoryCustomer.SetName ( m_strName );
}

/**********************************************************************/

bool CSalesHistoryCSVRecordEditCustomerDlg::SpecialValidate()
{
	bool bGotLoc = FALSE;

	for ( int nIndex = 0; nIndex < 8; nIndex++ )
	{
		int nSel = m_comboField[nIndex].GetCurSel();
		if ( nSel < 1 || nSel >= m_comboField[nIndex].GetCount() )
			continue;

		int nField = m_comboField[nIndex].GetItemData( nSel );
		
		if ( nField == SH_FIELD_LOC )
			bGotLoc = TRUE;

		if ( nField == SH_FIELD_TERMINAL )
		{
			if ( bGotLoc == TRUE )
				return TRUE;
			else
			{
				CString strMsg = "";
				strMsg += "You cannot sort a custom report by terminal unless\n\n";
				strMsg += "(1) The report is also sorted by location.\n\n";
				strMsg += "(2) Location is above terminal in the sort order.";
					
				Prompter.Error( strMsg );
				m_comboField[nIndex].SetFocus();
				return FALSE;
			}
		}
	}

	return TRUE;
}

/**********************************************************************/
