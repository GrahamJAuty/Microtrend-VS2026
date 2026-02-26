/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "ListCustomFieldValueDlg.h"
 #include "StockOptionsIni.h"
/**********************************************************************/
#include "ListDataCustomFieldDlg.h"
/**********************************************************************/

CSingleListContentHelpersCustomField::CSingleListContentHelpersCustomField()
{
}

/**********************************************************************/

int CSingleListContentHelpersCustomField::GetArraySize()
{
	return MAX_ITEM_CUSTOM_FIELDS;
}

/**********************************************************************/

bool CSingleListContentHelpersCustomField::PrepareLine( int nIndex, CString& strLine )
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

CListDataCustomFieldDlg::CListDataCustomFieldDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_bMultiSelect = FALSE;
	m_pContentHelpers = new CSingleListContentHelpersCustomField();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataCustomFieldDlg::~CListDataCustomFieldDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataCustomFieldDlg::SetTitle()
{
	CString strTitle = "Setup Custom Fields ";
	strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), StockOptions.GetGlobalCustomFieldFlag() ); 
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataCustomFieldDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Edit" );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.ShowWindow ( SW_HIDE );
	m_button4.ShowWindow ( SW_HIDE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );
	m_buttonOK.ShowWindow ( SW_HIDE );
	m_buttonCancel.SetWindowText( "Close" );
}

/**********************************************************************/

void CListDataCustomFieldDlg::SetColumns()
{
	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.InsertColumn ( 0, "Name", LVCFMT_LEFT, 480 );
}

/**********************************************************************/

void CListDataCustomFieldDlg::HandleButton1() 
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

	DataManager.SetActiveCustomFieldNo( nFieldNo );

	CListCustomFieldValueDlg dlg ( FALSE, 0, this );	

	if ( dlg.DoModal() == IDOK )
	{
		if ( DataManager.WriteCustomFieldValueBuffer( nFieldNo, info ) == FALSE )
			Prompter.WriteError( info );

		DataManagerNonDb.CustomFieldNames.SetName( nFieldNo, dlg.m_strName );

		CDataManagerInfo info;
		DataManagerNonDb.WriteCustomFieldNames( info );
	}

	DataManager.CloseCustomFieldValueBuffer( nFieldNo, info );
	SelectLine( nIndex );
}

/**********************************************************************/
