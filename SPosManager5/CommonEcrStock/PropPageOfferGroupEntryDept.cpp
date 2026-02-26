/**********************************************************************/
#include "NodeTypes.h"
/**********************************************************************/
#include "PropPageOfferGroupEntryDept.h"
/**********************************************************************/

CPropPageOfferGroupEntryDept::CPropPageOfferGroupEntryDept() : CSSPropertyPage(CPropPageOfferGroupEntryDept::IDD),
	m_TwinListHelpers( m_listSelected, m_listAvailable, m_arraySelected, m_arrayAvailable )
{
	//{{AFX_DATA_INIT(CPropPageOfferGroupEntryDept)
	//}}AFX_DATA_INIT
	m_bDialogReady = FALSE;
	m_nEntryType = OFFERENTRY_DEPT;
}

/**********************************************************************/

CPropPageOfferGroupEntryDept::~CPropPageOfferGroupEntryDept()
{
}

/**********************************************************************/

void CPropPageOfferGroupEntryDept::SetEntryType( int nEntryType )
{
	switch( nEntryType )
	{
	case OFFERENTRY_DEPT:
	case OFFERENTRY_DEPT_SAMEPLU:
		m_nEntryType = nEntryType;
	}
}

/**********************************************************************/

void CPropPageOfferGroupEntryDept::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageOfferGroupEntryDept)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_EDIT_NAME, m_editName );
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_buttonRemove);
	DDX_Control(pDX, IDC_STATIC_SELECTED, m_staticSelected);
	DDX_Control(pDX, IDC_STATIC_AVAILABLE, m_staticAvailable);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageOfferGroupEntryDept, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageOfferGroupEntryDept)
	//}}AFX_MSG_MAP
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_SELECTED, OnGetDispInfoListSelected)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_AVAILABLE, OnGetDispInfoListAvailable)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SELECTED, OnDoubleClickListSelected)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_AVAILABLE, OnDoubleClickListAvailable)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageOfferGroupEntryDept::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_editName.LimitText( OfferGroups::GroupName.Max );

	m_listSelected.SubclassDlgItem ( IDC_LIST_SELECTED, this );
	m_listSelected.InsertColumn ( 0, "DeptNo", LVCFMT_LEFT, 60 );
	m_listSelected.InsertColumn ( 1, "Description", LVCFMT_LEFT, 200 );
	m_listSelected.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	
	m_listAvailable.SubclassDlgItem ( IDC_LIST_AVAILABLE, this );
	m_listAvailable.InsertColumn ( 0, "DeptNo", LVCFMT_LEFT, 60 );
	m_listAvailable.InsertColumn ( 1, "Description", LVCFMT_LEFT, 200 );
	m_listAvailable.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	
	m_bDialogReady = TRUE;
	SetRecordControls();

	m_staticSelected.SetWindowText( "Selected Departments" );
	m_staticAvailable.SetWindowText( "Other Departments" );
	
	return TRUE;  
}

/**********************************************************************/

void CPropPageOfferGroupEntryDept::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	switch( nIDCtl )
	{
	case IDC_LIST_SELECTED:
		m_listSelected.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;

	case IDC_LIST_AVAILABLE:
		m_listAvailable.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;
	}
}

/**********************************************************************/

BOOL CPropPageOfferGroupEntryDept::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageOfferGroupEntryDept::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageOfferGroupEntryDept::Refresh()
{
	BuildDeptArray();
	SetRecordControls();
}

/**********************************************************************/

void CPropPageOfferGroupEntryDept::SetRecordControls()
{
	if ( TRUE == m_bDialogReady )
	{
		m_editName.SetWindowText( m_pOfferGroupRecord -> GetGroupName() );
		m_listSelected.SetItemCountEx( m_arraySelected.GetSize() );
		m_listAvailable.SetItemCountEx( m_arrayAvailable.GetSize() );
		m_listSelected.Invalidate();
		m_listAvailable.Invalidate();
	}
}

/**********************************************************************/

void CPropPageOfferGroupEntryDept::SaveRecord()
{
	CString strName;
	m_editName.GetWindowText( strName );
	m_pOfferGroupRecord -> SetGroupName( strName );
}

/**********************************************************************/

void CPropPageOfferGroupEntryDept::BuildDeptArray()
{
	m_arrayAvailable.RemoveAll();
	m_arraySelected.RemoveAll();

	int nIdx1, nIdx2;
	DataManager.OfferGroupEntry.GetGroupSubRange( m_pOfferGroupRecord -> GetGroupNo(), m_nEntryType, nIdx1, nIdx2 );

	for ( int n = nIdx1; n <= nIdx2; n++ )
	{
		COfferGroupEntriesCSVRecord entry;
		DataManager.OfferGroupEntry.GetAt( n, entry );

		CSortedIntWithFlag item;
		item.m_nInt = entry.GetEntryDeptNo();
		item.m_nFlag = 0;
		m_arraySelected.Consolidate( item );
	}

	for ( int n = 0; n < DataManager.Department.GetSize(); n++ )
	{
		CDepartmentCSVRecord DeptRecord;
		DataManager.Department.GetAt( n, DeptRecord );

		int nDeptNo = DeptRecord.GetDeptNo();
		if ( ( nDeptNo >= 1 ) && ( nDeptNo <= 999 ) )
		{
			CSortedIntWithFlag item;
			item.m_nInt = nDeptNo;
			item.m_nFlag = 0;
			
			int nIdx;
			if ( m_arraySelected.Find( item, nIdx ) == FALSE )
				m_arrayAvailable.Consolidate( item );
		}
	}
}

/**********************************************************************/

void CPropPageOfferGroupEntryDept::GetLineText( CReportConsolidationArray<CSortedIntWithFlag>& arrayItems, int nIdx, CCSV& csv )
{
	csv.RemoveAll();

	if ( ( nIdx >= 0 ) && ( nIdx < arrayItems.GetSize() ) )
	{
		CSortedIntWithFlag arrayItem;
		arrayItems.GetAt( nIdx, arrayItem );
		
		CString strDeptNo;
		strDeptNo.Format( "D%3.3d", arrayItem.m_nInt );
		
		csv.Add( strDeptNo );
		csv.Add( DataManager.Department.GetReportTextByDeptNo( arrayItem.m_nInt ) );
	}
}

/**********************************************************************/

void CPropPageOfferGroupEntryDept::OnGetDispInfoListSelected(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ( ( pDispInfo-> item.mask & LVIF_TEXT ) != 0 )
	{
		CCSV csv( '\t' );	
		GetLineText( m_arraySelected, pDispInfo->item.iItem, csv );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CPropPageOfferGroupEntryDept::OnGetDispInfoListAvailable(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ( ( pDispInfo-> item.mask & LVIF_TEXT ) != 0 )
	{
		CCSV csv( '\t' );	
		GetLineText( m_arrayAvailable, pDispInfo->item.iItem, csv );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/
void CPropPageOfferGroupEntryDept::OnButtonAdd()	{ m_TwinListHelpers.MoveItems( TRUE ); }
void CPropPageOfferGroupEntryDept::OnButtonRemove()	{ m_TwinListHelpers.MoveItems( FALSE ); }
/**********************************************************************/

void CPropPageOfferGroupEntryDept::OnDoubleClickListSelected(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if ( m_TwinListHelpers.SelectSingleLine( FALSE ) == TRUE )
		m_TwinListHelpers.MoveItems( FALSE );

	*pResult = 0;
}

/**********************************************************************/

void CPropPageOfferGroupEntryDept::OnDoubleClickListAvailable(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if ( m_TwinListHelpers.SelectSingleLine( TRUE ) == TRUE )
		m_TwinListHelpers.MoveItems( TRUE );

	*pResult = 0;
}

/**********************************************************************/

void CPropPageOfferGroupEntryDept::SaveOfferGroupEntries()
{
	int nOfferNo = m_pOfferGroupRecord -> GetGroupNo();

	for ( int n = 0; n < m_arraySelected.GetSize(); n++ )
	{
		CSortedIntWithFlag item;
		m_arraySelected.GetAt( n, item );
		int nDeptNo = item.m_nInt;
		
		COfferGroupEntriesCSVRecord entry;
		entry.SetGroupNo( nOfferNo );
		entry.SetEntryDeptNo( m_nEntryType, nDeptNo );
		
		int nIdx;
		DataManager.OfferGroupEntry.InsertRecord( entry, nIdx );
	}
}	

/**********************************************************************/
