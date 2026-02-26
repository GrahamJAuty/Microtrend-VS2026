/**********************************************************************/
#include "CustomFieldEditDlg.h"
#include "..\CommonEcrStock\ListDataDlg.h"
#include "SupplierKeyDlg.h"
/**********************************************************************/
#include "ListCustomFieldValueDlg.h"
/**********************************************************************/

CSingleListContentHelpersCustomFieldValue::CSingleListContentHelpersCustomFieldValue( bool bInContextEdit )
{
	m_bInContextEdit = bInContextEdit;
}

/**********************************************************************/

int CSingleListContentHelpersCustomFieldValue::GetArraySize()
{
	return DataManager.GetActiveCustomField() -> GetSize();
}

/**********************************************************************/

void CSingleListContentHelpersCustomFieldValue::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CCustomFieldValueCSVRecord ValueRecord;
		DataManager.GetActiveCustomField() -> GetAt( nIndex, ValueRecord );
		DataManager.GetActiveCustomField() -> DeleteValue ( nIndex );
	}
}

/**********************************************************************/

bool CSingleListContentHelpersCustomFieldValue::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CCustomFieldValueCSVRecord ValueRecord;
		DataManager.GetActiveCustomField() -> GetAt ( nIndex, ValueRecord );

		CCSV csv ( '\t' );
		csv.Add( ValueRecord.GetKey() );
		csv.Add( ValueRecord.GetName() );

		if ( TRUE == m_bInContextEdit )
			csv.Add( ( ValueRecord.GetProtectFlag() ) ? "No" : "Yes" );
	
		strLine = csv.GetLine();
	}

	return TRUE;
}

/**********************************************************************/

void CSingleListContentHelpersCustomFieldValue::FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>& arraySelected )
{
	m_bPreventDeletion = FALSE;

	if ( TRUE == m_bInContextEdit )
	{	
		for ( int n = arraySelected.GetSize() - 1; n >= 0; n-- )
		{
			CSortedIntItem item;
			arraySelected.GetAt(n, item);
			
			CCustomFieldValueCSVRecord ValueRecord;
			DataManager.GetActiveCustomField() -> GetAt ( item.m_nItem, ValueRecord );

			if ( ValueRecord.GetProtectFlag() == TRUE )
			{
				arraySelected.RemoveAt(n);
				m_bPreventDeletion = TRUE;
			}
		}
	}

	m_bPreventDeletion &= ( arraySelected.GetSize() == 0 );
}

/**********************************************************************/

void CSingleListContentHelpersCustomFieldValue::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );

	if ( TRUE == m_bInContextEdit )
	{
		list.InsertColumn ( 0, "Key", LVCFMT_LEFT, 215 );
		list.InsertColumn ( 1, "Value", LVCFMT_LEFT, 215 );
		list.InsertColumn ( 2, "New", LVCFMT_LEFT, 50 );
	}
	else
	{
		list.InsertColumn ( 0, "Key", LVCFMT_LEFT, 240 );
		list.InsertColumn ( 1, "Value", LVCFMT_LEFT, 240 );
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListCustomFieldValueDlg::CListCustomFieldValueDlg( bool bInContextEdit, int nInitialValueNo, CWnd* pParent)
	: CDialog(CListCustomFieldValueDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListCustomFieldValueDlg)
	//}}AFX_DATA_INIT

	m_bInContextEdit = bInContextEdit;
	
	m_pContentHelpers = new CSingleListContentHelpersCustomFieldValue( m_bInContextEdit );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
	m_pDisplayHelpers -> SetTopSingleText( "custom value" );
	m_pDisplayHelpers -> SetTopPluralText( "custom values" );
	m_pDisplayHelpers -> SetBottomSingleText( "value" );
	m_pDisplayHelpers -> SetBottomPluralText( "values" );

	m_nListSelection = -1;
	m_nInitialValueNo = nInitialValueNo;

	for ( int n = 0; n < DataManager.GetActiveCustomField() -> GetSize(); n++ )
	{
		CCustomFieldValueCSVRecord Value;
		DataManager.GetActiveCustomField() -> GetAt( n, Value );
		Value.SetProtectFlag( TRUE );
		DataManager.GetActiveCustomField() -> SetAt( n, Value );
	}

	m_strName = DataManagerNonDb.CustomFieldNames.GetName( DataManager.GetActiveCustomFieldNo() );
}

/**********************************************************************/

CListCustomFieldValueDlg::~CListCustomFieldValueDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListCustomFieldValueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListCustomFieldValueDlg)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_STATIC_NAME, m_staticName );
	DDX_Control( pDX, IDC_EDIT_NAME, m_editName );
	DDX_Text( pDX, IDC_EDIT_NAME, m_strName );
	DDV_MaxChars( pDX, m_strName, MAX_CUSTOM_FIELD_NAME_LEN );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CListCustomFieldValueDlg, CDialog)
	//{{AFX_MSG_MAP(CListCustomFieldValueDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST, OnKillFocusList)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST, OnSetFocusList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CListCustomFieldValueDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	int nFieldNo = DataManager.GetActiveCustomFieldNo();

	CString strTitle;
	strTitle.Format ( "Setup Custom Field Values (%s)",
		DataManagerNonDb.CustomFieldNames.GetName( nFieldNo ) ); 
	
	SetWindowText( strTitle );

	m_pContentHelpers -> SetColumns( m_listData, this );
	m_pDisplayHelpers -> PrepareDatabaseList( TRUE, FALSE );
	
	if ( TRUE == m_bInContextEdit )
	{
		m_staticName.ShowWindow( SW_HIDE );
		m_editName.ShowWindow( SW_HIDE );
	}

	return TRUE;  
}

/**********************************************************************/

void CListCustomFieldValueDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )  
   {
	   m_listData.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

int CListCustomFieldValueDlg::GetArraySize()
{
	return m_pContentHelpers -> GetArraySize();
}

/**********************************************************************/

void CListCustomFieldValueDlg::SelectLine ( int nIndex )
{
	m_pDisplayHelpers -> SelectDatabaseLine( nIndex );
}

/**********************************************************************/

void CListCustomFieldValueDlg::OnButton1() 
{
	int nNumber = DataManager.GetActiveCustomField() -> FindFreeNumber();
	if ( nNumber == 0 )
	{
		Prompter.Error ( "You have already created the\nmaximum number of custom field values." );
		return;
	}
	
	CSupplierKeyDlg dlgKey ( TRUE, TRUE, this );
	if ( dlgKey.DoModal() != IDOK )
		return;

	CString strKey = dlgKey.m_strKey;
	if ( strKey == "" )
		return;

	int nPos;
	if ( DataManager.GetActiveCustomField() -> FindValueByKey ( strKey, nPos ) == TRUE )
	{
		CString strMsg = "You have already created a custom field value\nwith key ";
		strMsg += strKey;
		Prompter.Error ( strMsg );
		return;
	}

	CCustomFieldValueCSVRecord NewValue;
	NewValue.SetNumber ( nNumber );
	NewValue.SetKey ( strKey );
	NewValue.SetName( strKey );
	int nIdx = DataManager.GetActiveCustomField() -> AddValue ( NewValue );
	m_listData.SetItemCountEx ( GetArraySize() );
	
	SelectLine ( nIdx );
	
	CCustomFieldEditDlg dlg ( "Add Custom Field Value", nIdx, this );
	dlg.DoModal();

	SelectLine ( dlg.GetValueIdx() );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListCustomFieldValueDlg::OnButton2() 
{
	int nValueIdx = GetSafeSelection();
	if ( nValueIdx == -1 )
		return;

	CCustomFieldValueCSVRecord ValueRecord;
	DataManager.GetActiveCustomField() -> GetAt ( nValueIdx, ValueRecord );
	CCustomFieldEditDlg dlg (  "Edit Custom Field Value", nValueIdx, this );
	
	if ( dlg.DoModal() == IDOK )
	{
		m_WarnCancelAfterChange.SetFlag();
	}

	SelectLine ( dlg.GetValueIdx() );
}

/**********************************************************************/

void CListCustomFieldValueDlg::OnButton3() 
{
	int nSizeOld = GetArraySize();

	m_pDisplayHelpers -> HandleDelete( this );

	if ( m_pContentHelpers -> GetPreventDeletionFlag() )
		Prompter.Error( "You cannot delete field values that were added\nbefore the start of this editing session." );

	if ( nSizeOld != GetArraySize() )
		m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

int CListCustomFieldValueDlg::GetSafeSelection()
{
	return GetSafeSelection ( 0, GetArraySize() - 1 );
}

/**********************************************************************/

int CListCustomFieldValueDlg::GetSafeSelection( int nMin, int nMax )
{
	int nSel = m_listData.GetCurSel();
	if ( ( nSel < nMin ) || ( nSel > nMax ) || ( nSel >= m_listData.GetItemCount() ) )
		nSel = -1;
	
	return nSel;
}

/**********************************************************************/

void CListCustomFieldValueDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLine( nIndex );
		CCSV csv ( m_strDisplayLine, '\t' );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

bool CListCustomFieldValueDlg::PrepareLine( int nIndex )
{
	return m_pContentHelpers -> PrepareLine( nIndex, m_strDisplayLine );
}

/**********************************************************************/

void CListCustomFieldValueDlg::OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDefID ( IDC_BUTTON2 );
	*pResult = 0;
}

/**********************************************************************/

void CListCustomFieldValueDlg::OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	SetDefID ( IDOK );
	*pResult = 0;
}

/**********************************************************************/

void CListCustomFieldValueDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnButton2();
	*pResult = 0;
}

/**********************************************************************/

void CListCustomFieldValueDlg::OnOK() 
{
	UpdateData( TRUE );

	m_nCustomFieldValueNo = 0;

	int nSel = GetSafeSelection();
	if ( ( nSel >= 0 ) && ( nSel < DataManager.GetActiveCustomField() -> GetSize() ) )
		m_nCustomFieldValueNo = DataManager.GetActiveCustomField() -> GetNumber( nSel );

	m_nListSelection = m_listData.GetCurSel();
	EndDialog ( IDOK );
}

/**********************************************************************/

void CListCustomFieldValueDlg::OnCancel() 
{
	if ( CListDataDlg::CheckBeforeCancel( FALSE, m_WarnCancelAfterChange.GetFlag() ) == TRUE )
	{
		EndDialog ( IDCANCEL );
	}
}

/**********************************************************************/
