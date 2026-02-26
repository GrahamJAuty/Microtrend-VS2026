/**********************************************************************/
#include "SupplierSetEditDlg.h"
#include "DatabaseCSVArray.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "ListDataSupplierSetDlg.h"
/**********************************************************************/

CSingleListContentHelpersSupplierSet::CSingleListContentHelpersSupplierSet() 
{
}

/**********************************************************************/

int CSingleListContentHelpersSupplierSet::GetArraySize()
{
	return DataManager.SupplierSet.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersSupplierSet::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		strLine = DataManager.SupplierSet.GetName ( nIndex );
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersSupplierSet::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManager.SupplierSet.GetSize() ) )
	{
		DataManager.SupplierSet.RemoveAt( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersSupplierSet::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Sets", LVCFMT_LEFT, 480 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataSupplierSetDlg::CListDataSupplierSetDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_pContentHelpers = new CSingleListContentHelpersSupplierSet();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "supplier set" );
	m_pDisplayHelpers -> SetTopPluralText( "supplier sets" );
	m_pDisplayHelpers -> SetBottomSingleText( "set" );
	m_pDisplayHelpers -> SetBottomPluralText( "sets" );
}

/**********************************************************************/

CListDataSupplierSetDlg::~CListDataSupplierSetDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataSupplierSetDlg::SetTitle()
{
	SetWindowText ( "Setup Supplier Sets" );
}

/**********************************************************************/

void CListDataSupplierSetDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "Move &Up" );
	m_button5.SetWindowText ( "Move D&own" );
	m_button6.ShowWindow ( SW_HIDE );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 147 );
	MoveControl( &m_button5, 368, 187 );
}

/**********************************************************************/

void CListDataSupplierSetDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataSupplierSetDlg::HandleButton1() 
{
	int nInsertPos = GetSafeSelection() + 1;
	if ( nInsertPos == 0 ) nInsertPos = DataManager.SupplierSet.GetSize();

	CSupplierSetCSVRecord NewRecord;
	NewRecord.SetName ( "SUPPLIER SET" );
	CSupplierSetEditDlg dlg ( NewRecord, this );
	dlg.DoModal();
	
	DataManager.SupplierSet.InsertAt ( nInsertPos, NewRecord );
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nInsertPos );	

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataSupplierSetDlg::HandleButton2() 
{
	int nSelection = GetSafeSelection();
	if ( nSelection == -1 )
		return;
	
	CSupplierSetCSVRecord SupplierSetRecord;
	DataManager.SupplierSet.GetAt ( nSelection, SupplierSetRecord );
	CSupplierSetEditDlg dlg ( SupplierSetRecord, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		DataManager.SupplierSet.SetAt ( nSelection, SupplierSetRecord );
		m_WarnCancelAfterChange.SetFlag();
	}

	SelectLine ( nSelection );	
}
		
/**********************************************************************/

void CListDataSupplierSetDlg::HandleButton3() 
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataSupplierSetDlg::HandleButton4()  
{
	int nSelection = GetSafeSelection ( 1, GetArraySize() - 1 );
	if ( nSelection == -1 )
		return;
	
	CSupplierSetCSVRecord TempRecord1; 
	CSupplierSetCSVRecord TempRecord2; 
	DataManager.SupplierSet.GetAt ( nSelection, TempRecord1 );
	DataManager.SupplierSet.GetAt ( nSelection - 1, TempRecord2 );
	DataManager.SupplierSet.SetAt ( nSelection, TempRecord2 );
	DataManager.SupplierSet.SetAt ( nSelection - 1, TempRecord1 );
	SelectLine ( nSelection - 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataSupplierSetDlg::HandleButton5() 
{
	int nSelection = GetSafeSelection ( 0, GetArraySize() - 2 );
	if ( nSelection == -1 )
		return;
	
	CSupplierSetCSVRecord TempRecord1; 
	CSupplierSetCSVRecord TempRecord2; 
	DataManager.SupplierSet.GetAt ( nSelection, TempRecord1 );
	DataManager.SupplierSet.GetAt ( nSelection + 1, TempRecord2 );
	DataManager.SupplierSet.SetAt ( nSelection, TempRecord2 );
	DataManager.SupplierSet.SetAt ( nSelection + 1, TempRecord1 );
	SelectLine ( nSelection + 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/