/**********************************************************************/
#include "SalesHistoryFields.h"
/**********************************************************************/
#include "SalesHistoryCSVRecordEditPluDlg.h"
/**********************************************************************/

CSalesHistoryCSVRecordEditPluDlg::CSalesHistoryCSVRecordEditPluDlg( CSalesHistoryCSVRecordPlu& Record, bool bAdhoc, CWnd* pParent)
	: CSalesHistoryCSVRecordEditDlg(pParent), m_SalesHistoryPlu( Record )
{
	m_bAdhoc = bAdhoc;
}

/**********************************************************************/

const char* CSalesHistoryCSVRecordEditPluDlg::GetName()
{
	m_strName = m_SalesHistoryPlu.GetActualName();
	return m_strName;
}

/**********************************************************************/

const char* CSalesHistoryCSVRecordEditPluDlg::GetTitle()
{
	m_strTitle = "Setup ";
	m_strTitle += ( m_bAdhoc ) ? "Adhoc " : "Custom ";
	m_strTitle += "Plu Sales History Report";
	return m_strTitle;
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditPluDlg::PrepareCombos(CWordArray& validFields, CWordArray& reportFields)
{
	validFields.RemoveAll();
	reportFields.RemoveAll();

	m_SalesHistoryPlu.GetValidFieldList(validFields);
	m_SalesHistoryPlu.GetReportFields(reportFields);

	for (int nIndex = 0; nIndex < 8; nIndex++)
	{
		int n = 0;
		m_comboField[nIndex].ResetContent();
		m_comboField[nIndex].AddString(" -- -- ");
		m_comboField[nIndex].SetItemData(n++, 0);

		for (int x = 0; x < validFields.GetSize(); x++)
		{
			int nField = validFields.GetAt(x);
			m_comboField[nIndex].AddString(m_SalesHistoryPlu.GetFieldListName(nField));
			m_comboField[nIndex].SetItemData(n++, nField);
		}

		m_comboField[nIndex].SetCurSel(0);
		m_checkTotal[nIndex].EnableWindow(FALSE);
		m_checkHeader[nIndex].EnableWindow(FALSE);
	}

	if (m_SalesHistoryPlu.GetReportMode() == SALESHISTORY_REPORTMODE_CHART)
	{
		for (int nIndex = 0; nIndex < 8; nIndex++)
		{
			m_checkTotal[nIndex].EnableWindow(FALSE);
		}
	}

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(80);
	m_comboType.SetTabStops(arrayStops);

	m_comboType.ResetContent();
	m_comboType.AddItem("Normal");
	m_comboType.AddItem("Normal,(Top / Bottom 100)");
	m_comboType.AddItem("Weekday,(Quantity & Full Value)");
	m_comboType.AddItem("Weekday,(Quantity & Discounted Value)");
	m_comboType.AddItem("Weekday,(Full Value)");
	m_comboType.AddItem("Weekday,(Discounted Value)");
	m_comboType.AddItem("Weekday,(Quantity)");
	m_comboType.AddItem("Monthday,(Full Value)");
	m_comboType.AddItem("Monthday,(Discounted Value)");
	m_comboType.AddItem("Monthday,(Quantity)");

	int nSel = 0;
	switch (m_SalesHistoryPlu.GetReportMode())
	{
	case SALESHISTORY_REPORTMODE_CHART:					nSel = 1;	break;
	case SALESHISTORY_REPORTMODE_WEEKLY_QTYVAL_FULL:	nSel = 2;	break;
	case SALESHISTORY_REPORTMODE_WEEKLY_QTYVAL_DISC:	nSel = 3;	break;
	case SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL:		nSel = 4;	break;
	case SALESHISTORY_REPORTMODE_WEEKLY_VAL_DISC:		nSel = 5;	break;
	case SALESHISTORY_REPORTMODE_WEEKLY_QTY:			nSel = 6;	break;
	case SALESHISTORY_REPORTMODE_MONTHLY_VAL_FULL:		nSel = 7;	break;
	case SALESHISTORY_REPORTMODE_MONTHLY_VAL_DISC:		nSel = 8;	break;
	case SALESHISTORY_REPORTMODE_MONTHLY_QTY:			nSel = 9;	break;
	}
	m_comboType.SetCurSel(nSel);
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditPluDlg::UpdateReportModeFromCombo()
{
	int nType = 0;
	switch( m_comboType.GetCurSel() )
	{
	case 1:	nType = SALESHISTORY_REPORTMODE_CHART;				break;
	case 2:	nType = SALESHISTORY_REPORTMODE_WEEKLY_QTYVAL_FULL;	break;
	case 3:	nType = SALESHISTORY_REPORTMODE_WEEKLY_QTYVAL_DISC;	break;	
	case 4:	nType = SALESHISTORY_REPORTMODE_WEEKLY_VAL_FULL;	break;
	case 5:	nType = SALESHISTORY_REPORTMODE_WEEKLY_VAL_DISC;	break;
	case 6:	nType = SALESHISTORY_REPORTMODE_WEEKLY_QTY;			break;
	case 7:	nType = SALESHISTORY_REPORTMODE_MONTHLY_VAL_FULL;	break;
	case 8:	nType = SALESHISTORY_REPORTMODE_MONTHLY_VAL_DISC;	break;
	case 9:	nType = SALESHISTORY_REPORTMODE_MONTHLY_QTY;		break;
	}
	m_SalesHistoryPlu.SetReportMode( nType );
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditPluDlg::TidyFields() 
{
	CWordArray arrayTemp;
	GetFieldSettings( arrayTemp );
	m_SalesHistoryPlu.SetReportFields( arrayTemp );
	FillCombos();
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditPluDlg::ProcessChanges()
{
	::TrimSpaces ( m_strName, FALSE );
	
	if (m_strName == "")
	{
		m_strName = "Custom Report";
	}

	CWordArray arrayTemp;
		
	//GET CURRENT FIELD SETTINGS FROM CONTROLS
	GetFieldSettings( arrayTemp );	
	m_SalesHistoryPlu.SetReportFields( arrayTemp );
	UpdateReportModeFromCombo();
		
	//TIDY THEM UP AND GET THEM AGAIN
	FillCombos();					
	GetFieldSettings( arrayTemp );
	m_SalesHistoryPlu.SetReportFields( arrayTemp );
	UpdateReportModeFromCombo();
	m_SalesHistoryPlu.SetName ( m_strName );
}

/**********************************************************************/

bool CSalesHistoryCSVRecordEditPluDlg::SpecialValidate()
{
	for ( int nIndex = 0; nIndex < 8; nIndex++ )
	{
		int nSel = m_comboField[nIndex].GetCurSel();
		if (nSel < 1 || nSel >= m_comboField[nIndex].GetCount())
		{
			continue;
		}

		int nField = m_comboField[nIndex].GetItemData( nSel );
		
		if (nField == SH_FIELD_LOC)
		{
			return TRUE;
		}

		CString strBadField = "";
		switch( nField )
		{
		case SH_FIELD_TERMINAL:	strBadField = "Terminal No";	break;
		case SH_FIELD_TABLENO:	strBadField = "Table Name";		break;
		case SH_FIELD_TABLEGRP:	strBadField = "Table Group";	break;
		}

		if ( strBadField != "" )
		{
			CString strError = "";
			strError += "You cannot sort a custom report by ";
			strError += strBadField;
			strError += " unless\n\n";
			strError += "(1) The report is also sorted by location.\n\n";
			strError += "(2) Location is above Terminal No in the sort order.";

			Prompter.Error( strError );
			m_comboField[nIndex].SetFocus();
			return FALSE;
		}
	}

	return TRUE;
}

/**********************************************************************/
