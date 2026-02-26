/**********************************************************************/
#include "CustomFieldSetEditDlg.h"
#include "DatabaseCSVArray.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "ListDataCustomFieldSetValueDlg.h"
/**********************************************************************/

CSingleListContentHelpersCustomFieldSetValue::CSingleListContentHelpersCustomFieldSetValue() 
{
}

/**********************************************************************/

int CSingleListContentHelpersCustomFieldSetValue::GetArraySize()
{
	return DataManager.GetActiveCustomFieldSet() -> GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersCustomFieldSetValue::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		strLine = DataManager.GetActiveCustomFieldSet() -> GetName ( nIndex );
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersCustomFieldSetValue::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManager.GetActiveCustomFieldSet() -> GetSize() ) )
	{
		int nNumber = DataManager.GetActiveCustomFieldSet() -> GetNumber ( nIndex );
		
		DataManager.GetActiveCustomFieldSet() -> RemoveAt ( nIndex );
		DataManager.GetActiveCustomFieldSet() -> SetInUseFlag( nNumber, FALSE );

		for ( int n = 0; n < DataManager.GetActiveCustomField() -> GetSize(); n++ )
		{
			CCustomFieldValueCSVRecord record;
			DataManager.GetActiveCustomField() -> GetAt( n, record );
			
			CString strFlags = record.GetFieldSetFlags();
			::SetHexStringFlag( strFlags, nNumber, FALSE );
			record.SetFieldSetFlags( strFlags );

			DataManager.GetActiveCustomField() -> SetAt( n, record );
		}

		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersCustomFieldSetValue::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Sets", LVCFMT_LEFT, 480 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataCustomFieldSetValueDlg::CListDataCustomFieldSetValueDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_pContentHelpers = new CSingleListContentHelpersCustomFieldSetValue();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "custom field set" );
	m_pDisplayHelpers -> SetTopPluralText( "custom field sets" );
	m_pDisplayHelpers -> SetBottomSingleText( "set" );
	m_pDisplayHelpers -> SetBottomPluralText( "sets" );
}

/**********************************************************************/

CListDataCustomFieldSetValueDlg::~CListDataCustomFieldSetValueDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataCustomFieldSetValueDlg::SetTitle()
{
	int nFieldNo = DataManager.GetActiveCustomFieldNo();

	CString strTitle;
	strTitle.Format ( "Setup Custom Field Sets (%s)",
		DataManagerNonDb.CustomFieldNames.GetName( nFieldNo ) ); 
	
	SetWindowText( strTitle );
}

/**********************************************************************/

void CListDataCustomFieldSetValueDlg::SetButtonTexts()
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

void CListDataCustomFieldSetValueDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataCustomFieldSetValueDlg::HandleButton1() 
{
	int nInsertPos = GetSafeSelection() + 1;
	if ( nInsertPos == 0 ) nInsertPos = DataManager.GetActiveCustomFieldSet() -> GetSize();

	int nNumber = DataManager.GetActiveCustomFieldSet() -> GetNextFreeNumber();

	if ( 0 == nNumber )
		Prompter.Error( "You have already created the maximum\namount of custom field sets" );
	else
	{
		CCustomFieldSetCSVRecord NewRecord;
		NewRecord.SetName ( "CUSTOM FIELD SET" );
		NewRecord.SetNumber( nNumber );
		CCustomFieldSetEditDlg dlg ( NewRecord, this );
		dlg.DoModal();
	
		DataManager.GetActiveCustomFieldSet() -> InsertAt ( nInsertPos, NewRecord );
		DataManager.GetActiveCustomFieldSet() -> SetInUseFlag( nNumber, TRUE );
		m_listData.SetItemCountEx ( GetArraySize() );
		SelectLine ( nInsertPos );	

		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/

void CListDataCustomFieldSetValueDlg::HandleButton2() 
{
	int nSelection = GetSafeSelection();
	if ( nSelection == -1 )
		return;
	
	CCustomFieldSetCSVRecord CustomFieldSetRecord;
	DataManager.GetActiveCustomFieldSet() -> GetAt ( nSelection, CustomFieldSetRecord );
	CCustomFieldSetEditDlg dlg ( CustomFieldSetRecord, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		DataManager.GetActiveCustomFieldSet() -> SetAt ( nSelection, CustomFieldSetRecord );
		m_WarnCancelAfterChange.SetFlag();
	}

	SelectLine ( nSelection );	
}
		
/**********************************************************************/

void CListDataCustomFieldSetValueDlg::HandleButton3() 
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataCustomFieldSetValueDlg::HandleButton4()  
{
	int nSelection = GetSafeSelection( 1, GetArraySize() - 1 );
	if ( nSelection == -1 )
		return;
	
	CCustomFieldSetCSVRecord TempRecord1; 
	CCustomFieldSetCSVRecord TempRecord2; 
	DataManager.GetActiveCustomFieldSet() -> GetAt ( nSelection, TempRecord1 );
	DataManager.GetActiveCustomFieldSet() -> GetAt ( nSelection - 1, TempRecord2 );
	DataManager.GetActiveCustomFieldSet() -> SetAt ( nSelection, TempRecord2 );
	DataManager.GetActiveCustomFieldSet() -> SetAt ( nSelection - 1, TempRecord1 );
	SelectLine ( nSelection - 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataCustomFieldSetValueDlg::HandleButton5() 
{
	int nSelection = GetSafeSelection( 0, GetArraySize() - 2 );
	if ( nSelection == -1 )
		return;
	
	CCustomFieldSetCSVRecord TempRecord1; 
	CCustomFieldSetCSVRecord TempRecord2; 
	DataManager.GetActiveCustomFieldSet() -> GetAt ( nSelection, TempRecord1 );
	DataManager.GetActiveCustomFieldSet() -> GetAt ( nSelection + 1, TempRecord2 );
	DataManager.GetActiveCustomFieldSet() -> SetAt ( nSelection, TempRecord2 );
	DataManager.GetActiveCustomFieldSet() -> SetAt ( nSelection + 1, TempRecord1 );
	SelectLine ( nSelection + 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

