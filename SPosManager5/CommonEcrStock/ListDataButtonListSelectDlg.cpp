/**********************************************************************/
#include "ButtonManager.h"
#include "..\CommonEcrStock\DefEditDlg.h"
/**********************************************************************/
#include "ListDataButtonListSelectDlg.h"
/**********************************************************************/

CSingleListContentHelpersButtonListSelect::CSingleListContentHelpersButtonListSelect() 
{
}

/**********************************************************************/

int CSingleListContentHelpersButtonListSelect::GetArraySize()
{
	return 100;
}

/**********************************************************************/

bool CSingleListContentHelpersButtonListSelect::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		strLine.Format ( "%2.2d\t%s",
			nIndex,
			ButtonManager.GetListName( nIndex ) );

		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataButtonListSelectDlg::CListDataButtonListSelectDlg( int nInitialSel, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_nInitialSel = nInitialSel;
	m_bMultiSelect = FALSE;
	m_pContentHelpers = new CSingleListContentHelpersButtonListSelect();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataButtonListSelectDlg::~CListDataButtonListSelectDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataButtonListSelectDlg::SetTitle()
{
	SetWindowText ( "Select Button List" );
}

/**********************************************************************/

void CListDataButtonListSelectDlg::PrepareList()
{
	CListDataDlg::PrepareList();
	SelectLine( m_nInitialSel );
}

/**********************************************************************/

void CListDataButtonListSelectDlg::SetButtonTexts()
{
	m_button1.ShowWindow ( SW_HIDE );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.ShowWindow ( SW_HIDE );
	m_button4.ShowWindow ( SW_HIDE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );
	m_buttonOK.ShowWindow ( SW_HIDE );
	m_buttonCancel.ShowWindow ( SW_HIDE );
	ResizeDialog( 370, 299 );
}

/**********************************************************************/

void CListDataButtonListSelectDlg::SetColumns()
{
	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.InsertColumn ( 0, "List No", LVCFMT_LEFT, 80 );
	m_listData.InsertColumn ( 1, "Name", LVCFMT_LEFT, 400 );
}

/**********************************************************************/

