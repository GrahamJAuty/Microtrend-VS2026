/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "DefEditStockCategoryDlg.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "ListDataAllowanceDlg.h"
/**********************************************************************/

CSingleListContentHelpersAllowance::CSingleListContentHelpersAllowance()
{
}

/**********************************************************************/

int CSingleListContentHelpersAllowance::GetArraySize()
{
	return DataManager.Allowance.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersAllowance::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		strLine.Format ( "%s\t%s",
			DataManager.Allowance.GetText ( nIndex + 1 ),
			CTaxArray::GetTaxBandFromNumber( DataManager.Allowance.GetTaxBand(nIndex + 1), 1, " " ) );

		if ( DataManager.Allowance.GetReportEnable ( nIndex + 1 ) == FALSE )
			strLine += "\tDisable";

		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataAllowanceDlg::CListDataAllowanceDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_bMultiSelect = FALSE;
	m_pContentHelpers = new CSingleListContentHelpersAllowance();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataAllowanceDlg::~CListDataAllowanceDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataAllowanceDlg::SetTitle()
{
	CString strTitle = "Setup Stock Allowance Texts ";
	strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), StockOptions.GetGlobalAllowanceFlag() ); 
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataAllowanceDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Edit" );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.ShowWindow ( SW_HIDE );
	m_button4.ShowWindow ( SW_HIDE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );
}

/**********************************************************************/

void CListDataAllowanceDlg::SetColumns()
{
	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.InsertColumn ( 0, "Name", LVCFMT_LEFT, 320 );
	m_listData.InsertColumn ( 1, EcrmanOptions.GetTaxBandString(), LVCFMT_RIGHT, 80 );
	m_listData.InsertColumn ( 2, "Report", LVCFMT_RIGHT, 80 );
}

/**********************************************************************/

void CListDataAllowanceDlg::HandleButton1() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )	
		return;

	CDefEditStockCategoryDlg dlg ( DEFEDIT_STOCKCAT_ALLOWANCE, nIndex + 1, m_WarnCancelAfterChange, this );
	dlg.SetListDataDlg( this );
	dlg.DoModal();
	SelectLine ( dlg.GetIndex() - 1 );
}

/**********************************************************************/
