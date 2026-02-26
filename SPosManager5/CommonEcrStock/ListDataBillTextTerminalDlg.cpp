/**********************************************************************/
#include "BillAndReceiptTexts.h"
#include "EditBillTextDlg.h"
#include "LocationCSVArray.h"
#include "LocationSelectorEntity.h"
/**********************************************************************/
#include "ListDataBillTextTerminalDlg.h"
/**********************************************************************/

CSingleListContentHelpersBillTextTerminal::CSingleListContentHelpersBillTextTerminal( int nLocIdx ) 
{
	m_nLocIdx = nLocIdx;
}

/**********************************************************************/

int CSingleListContentHelpersBillTextTerminal::GetArraySize()
{
	return dbLocation.GetTerminalCount( m_nLocIdx );
}

/**********************************************************************/

bool CSingleListContentHelpersBillTextTerminal::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		strLine.Format( "TNo %d Texts", dbLocation.GetTNo( m_nLocIdx, nIndex ) );
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataBillTextTerminalDlg::CListDataBillTextTerminalDlg( int nLocIdx, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_bMultiSelect = FALSE;
	m_nLocIdx = nLocIdx;
	m_pContentHelpers = new CSingleListContentHelpersBillTextTerminal( nLocIdx );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataBillTextTerminalDlg::~CListDataBillTextTerminalDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataBillTextTerminalDlg::SetTitle()
{
	SetWindowText ( "Receipt/Bill Texts by Terminal" );
}

/**********************************************************************/

void CListDataBillTextTerminalDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Edit" );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.ShowWindow ( SW_HIDE );
	m_button4.ShowWindow ( SW_HIDE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );
	m_buttonCancel.ShowWindow( SW_HIDE );
}

/**********************************************************************/

void CListDataBillTextTerminalDlg::SetColumns()
{
	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.InsertColumn ( 0, dbLocation.GetName( m_nLocIdx ), LVCFMT_LEFT, 220 );

	ResizeControl( &m_listData, 170, 150 );
	MoveControl( &m_button1, 193, 7 );
	MoveControl( &m_button2, 193, 7 );
	MoveControl( &m_button3, 193, 7 );
	MoveControl( &m_button4, 193, 7 );
	MoveControl( &m_button5, 193, 7 );
	MoveControl( &m_button6, 193, 7 );
	MoveControl( &m_buttonOK, 193, 143 );
	MoveControl( &m_buttonCancel, 193, 143 );
	ResizeDialog( 250, 164 );
}

/**********************************************************************/

void CListDataBillTextTerminalDlg::HandleButton1() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )	
		return;

	CLocationSelectorEntity LocSelEdit;
	LocSelEdit.SetLocationNode( m_nLocIdx );
	LocSelEdit.SetTerminalNode( nIndex );

	CBillAndReceiptTexts Texts;
	Texts.SetLocSelEntity( LocSelEdit, FALSE );
	
	CSSFile fileLock;
	if ( fileLock.Open( Texts.GetLockFilename(), "wb" ) == FALSE )
	{
		MessageBox ( "This text file is currently being modified by another user.\n\nPlease try again later.", "Access Denied", MB_ICONEXCLAMATION | MB_OK );
	}
	else
	{
		
		Texts.Read();
		
		CEditBillTextDlg dlg( LocSelEdit, Texts, this );
		
		if ( dlg.DoModal() == IDOK )
		{
			if ( ( dlg.GetUseParentFlag() == FALSE ) || ( LocSelEdit.GetConType() == NODE_SYSTEM ) )
				Texts.Write();
			else
				Texts.DeleteFile();
		}
	}
}

/**********************************************************************/
