/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "ListDataCustomFieldSetValueDlg.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "ListDataCustomFieldSetDlg.h"
/**********************************************************************/

CSingleListContentHelpersCustomFieldSet::CSingleListContentHelpersCustomFieldSet()
{
}

/**********************************************************************/

int CSingleListContentHelpersCustomFieldSet::GetArraySize()
{
	return MAX_ITEM_CUSTOM_FIELDS;
}

/**********************************************************************/

bool CSingleListContentHelpersCustomFieldSet::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		strLine = DataManagerNonDb.CustomFieldNames.GetName( nIndex + 1 );
	
		if ( strLine == "" )
			strLine.Format ( "Custom Field %d", nIndex + 1 );

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataCustomFieldSetDlg::CListDataCustomFieldSetDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_bMultiSelect = FALSE;
	m_pContentHelpers = new CSingleListContentHelpersCustomFieldSet();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataCustomFieldSetDlg::~CListDataCustomFieldSetDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataCustomFieldSetDlg::SetTitle()
{
	CString strTitle = "Setup Custom Field Sets ";
	strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), StockOptions.GetGlobalCustomFieldFlag() ); 
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataCustomFieldSetDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Sets" );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.ShowWindow ( SW_HIDE );
	m_button4.ShowWindow ( SW_HIDE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );
	m_buttonOK.ShowWindow ( SW_HIDE );
	m_buttonCancel.SetWindowText( "Close" );
}

/**********************************************************************/

void CListDataCustomFieldSetDlg::SetColumns()
{
	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.InsertColumn ( 0, "Name", LVCFMT_LEFT, 480 );
}

/**********************************************************************/

void CListDataCustomFieldSetDlg::HandleButton1() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )	
		return;

	int nFieldNo = nIndex + 1;

	CDataManagerInfo info;
	if ( DataManager.OpenCustomFieldValueBuffer( nFieldNo, info ) == FALSE )
	{
		Prompter.ShareError( info );
		return;
	}

	CDataManagerInfo info2;
	if ( DataManager.OpenCustomFieldSetBuffer( nFieldNo, info2 ) == FALSE )
	{
		DataManager.CloseCustomFieldValueBuffer( nFieldNo, info );
		Prompter.ShareError( info2 );
		return;
	}

	DataManager.SetActiveCustomFieldNo( nFieldNo );
	DataManager.GetActiveCustomFieldSet() -> InitialiseInUseFlags();

	CListDataCustomFieldSetValueDlg dlg ( this );	

	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManager.WriteCustomFieldValueBuffer( nFieldNo, info ) == FALSE )
			Prompter.WriteError( info );
		else if ( DataManager.WriteCustomFieldSetBuffer( nFieldNo, info2 ) == FALSE )
			Prompter.WriteError( info2 );
	}

	DataManager.CloseCustomFieldValueBuffer( nFieldNo, info );
	DataManager.CloseCustomFieldSetBuffer( nFieldNo, info2 );
	SelectLine( nIndex );
}

/**********************************************************************/
