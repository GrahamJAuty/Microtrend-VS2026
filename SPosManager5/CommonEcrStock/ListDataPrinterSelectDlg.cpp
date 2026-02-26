/**********************************************************************/
#include "ListDataPrinterSelectDlg.h"
/**********************************************************************/

CSingleListContentHelpersPrinterSelect::CSingleListContentHelpersPrinterSelect( CReportConsolidationArray<CSortedStringItem>& arrayPrinters ) : m_arrayPrinters( arrayPrinters )
{
}

/**********************************************************************/

int CSingleListContentHelpersPrinterSelect::GetArraySize()
{
	return m_arrayPrinters.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersPrinterSelect::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CSortedStringItem item;
		m_arrayPrinters.GetAt( nIndex, item );
		strLine = item.m_strItem;
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataPrinterSelectDlg::CListDataPrinterSelectDlg( const char* szTitle, CReportConsolidationArray<CSortedStringItem>& arrayPrinters, CWnd* pParent) : CListDataDlg ( pParent ), m_arrayPrinters( arrayPrinters )
{
	m_bMultiSelect = FALSE;
	m_pContentHelpers = new CSingleListContentHelpersPrinterSelect( m_arrayPrinters );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
	m_strTitle = szTitle;
}

/**********************************************************************/

CListDataPrinterSelectDlg::~CListDataPrinterSelectDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataPrinterSelectDlg::SetTitle()
{
	SetWindowText ( m_strTitle );
}

/**********************************************************************/

void CListDataPrinterSelectDlg::SetButtonTexts()
{
	m_button1.ShowWindow ( SW_HIDE );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.ShowWindow ( SW_HIDE );
	m_button4.ShowWindow ( SW_HIDE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );
	MoveControl( &m_button1, 368, 7 );
}

/**********************************************************************/

void CListDataPrinterSelectDlg::PrepareList()
{
	CListDataDlg::PrepareList();
	SelectLine( m_nSelection );
}

/**********************************************************************/

void CListDataPrinterSelectDlg::SetColumns()
{
	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.ModifyStyle( 0, LVS_SINGLESEL, 0 );
	m_listData.InsertColumn ( 0, "Printer Name", LVCFMT_LEFT, 480 );
}

/**********************************************************************/

bool CListDataPrinterSelectDlg::HandleOK()
{
	m_nSelection = GetSafeSelection();
	return TRUE;
}

/**********************************************************************/
