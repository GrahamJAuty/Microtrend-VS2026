/**********************************************************************/
#include "CategorySetEditDlg.h"
#include "DatabaseCSVArray.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "ListDataCategorySetDlg.h"
/**********************************************************************/

CSingleListContentHelpersCategorySet::CSingleListContentHelpersCategorySet( CCategorySetCSVArray* pCatSetArray ) 
{
	m_pCatSetArray = pCatSetArray;
}

/**********************************************************************/

int CSingleListContentHelpersCategorySet::GetArraySize()
{
	return m_pCatSetArray -> GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersCategorySet::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		strLine = m_pCatSetArray -> GetName ( nIndex );
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersCategorySet::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_pCatSetArray -> GetSize() ) )
	{
		m_pCatSetArray -> RemoveAt( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersCategorySet::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Sets", LVCFMT_LEFT, 480 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataCategorySetDlg::CListDataCategorySetDlg( bool bSystem, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_bSystem = bSystem;
	m_pCatSetArray = bSystem ? &DataManagerNonDb.SystemCategorySet : &DataManager.CategorySet;

	m_pContentHelpers = new CSingleListContentHelpersCategorySet( m_pCatSetArray );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "category set" );
	m_pDisplayHelpers -> SetTopPluralText( "category sets" );
	m_pDisplayHelpers -> SetBottomSingleText( "set" );
	m_pDisplayHelpers -> SetBottomPluralText( "sets" );
}

/**********************************************************************/

CListDataCategorySetDlg::~CListDataCategorySetDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataCategorySetDlg::SetTitle()
{
	if ( TRUE == m_bSystem )
	{
		SetWindowText( "Setup System Category Sets" );
	}
	else
	{
		CString strTitle = "Setup Category Sets ";
		strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), StockOptions.GetGlobalCategoryFlag() ); 
		SetWindowText ( strTitle );
	}
}

/**********************************************************************/

void CListDataCategorySetDlg::SetButtonTexts()
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

void CListDataCategorySetDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataCategorySetDlg::HandleButton1() 
{
	int nInsertPos = GetSafeSelection() + 1;
	if ( nInsertPos == 0 ) nInsertPos = m_pCatSetArray -> GetSize();

	CCategorySetCSVRecord NewRecord;
	NewRecord.SetName ( "CATEGORY SET" );
	CCategorySetEditDlg dlg ( m_bSystem, NewRecord, this );
	dlg.DoModal();
	
	m_pCatSetArray -> InsertAt ( nInsertPos, NewRecord );
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nInsertPos );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataCategorySetDlg::HandleButton2() 
{
	int nSelection = GetSafeSelection();
	if ( nSelection == -1 )
		return;
	
	CCategorySetCSVRecord CategorySetRecord;
	m_pCatSetArray -> GetAt ( nSelection, CategorySetRecord );
	CCategorySetEditDlg dlg ( m_bSystem, CategorySetRecord, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		m_pCatSetArray -> SetAt ( nSelection, CategorySetRecord );
		m_WarnCancelAfterChange.SetFlag();
	}

	SelectLine ( nSelection );	
}
		
/**********************************************************************/
	
void CListDataCategorySetDlg::HandleButton3() 
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataCategorySetDlg::HandleButton4()  
{
	int nSelection = GetSafeSelection( 1, GetArraySize() - 1 );
	if ( nSelection == -1 )
		return;
	
	CCategorySetCSVRecord TempRecord1; 
	CCategorySetCSVRecord TempRecord2; 
	m_pCatSetArray -> GetAt ( nSelection, TempRecord1 );
	m_pCatSetArray -> GetAt ( nSelection - 1, TempRecord2 );
	m_pCatSetArray -> SetAt ( nSelection, TempRecord2 );
	m_pCatSetArray -> SetAt ( nSelection - 1, TempRecord1 );
	SelectLine ( nSelection - 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataCategorySetDlg::HandleButton5() 
{
	int nSelection = GetSafeSelection( 0, GetArraySize() - 2 );
	if ( nSelection == -1 )
		return;
	
	CCategorySetCSVRecord TempRecord1; 
	CCategorySetCSVRecord TempRecord2; 
	m_pCatSetArray -> GetAt ( nSelection, TempRecord1 );
	m_pCatSetArray -> GetAt ( nSelection + 1, TempRecord2 );
	m_pCatSetArray -> SetAt ( nSelection, TempRecord2 );
	m_pCatSetArray -> SetAt ( nSelection + 1, TempRecord1 );
	SelectLine ( nSelection + 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/