/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSOptions.h"
/**********************************************************************/
#include "PropPagePMSOptionsDinner.h"
/**********************************************************************/

CPropPagePMSOptionsDinner::CPropPagePMSOptionsDinner() : CSSPropertyPage(CPropPagePMSOptionsDinner::IDD),
	m_TwinListHelpers( m_listSelected, m_listAvailable, m_arraySelected, m_arrayAvailable )
{
	//{{AFX_DATA_INIT(CPropPagePMSOptionsDinner)
	//}}AFX_DATA_INIT
	m_pPMSOptions = NULL;
}

/**********************************************************************/

CPropPagePMSOptionsDinner::~CPropPagePMSOptionsDinner()
{
}

/**********************************************************************/

void CPropPagePMSOptionsDinner::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePMSOptionsDinner)
	//}}AFX_DATA_MAP
	DDX_Text( pDX, IDC_EDIT_ALLOWANCE, m_dDinnerAllowance );
	DDV_MinMaxDouble( pDX, m_dDinnerAllowance, 0.0, 100.0 );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePMSOptionsDinner, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePMSOptionsDinner)
	//}}AFX_MSG_MAP
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_SELECTED, OnGetDispInfoListSelected)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SELECTED, OnDoubleClickListSelected)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_AVAILABLE, OnGetDispInfoListAvailable)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_AVAILABLE, OnDoubleClickListAvailable)
	ON_BN_CLICKED( IDC_BUTTON_ADD, OnButtonAdd )
	ON_BN_CLICKED( IDC_BUTTON_REMOVE, OnButtonRemove )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePMSOptionsDinner::OnInitDialog() 
{
	m_dDinnerAllowance = m_pPMSOptions -> GetDinnerAllowance();

	CPropertyPage::OnInitDialog();
	
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
			m_arrayAvailable.Consolidate( item );
		}
	}

	CCSV csv( m_pPMSOptions -> GetDinnerDepartments() );
	for ( int n = 0; ( n < csv.GetSize() ) && ( n < 999 ); n++ )
	{
		int nDeptNo = csv.GetInt(n);
		if ( ( nDeptNo >= 1 ) && ( nDeptNo <= 999 ) )
		{
			CSortedIntWithFlag item;
			item.m_nInt = nDeptNo;
			item.m_nFlag = 0;
			m_arraySelected.Consolidate( item );

			int nPos;
			if ( m_arrayAvailable.Find( item, nPos ) == TRUE )
				m_arrayAvailable.RemoveAt( nPos );
		}
	}

	SubclassEdit( IDC_EDIT_ALLOWANCE, SS_NUM_DP, 6, "%.2f" );

	m_listSelected.SubclassDlgItem ( IDC_LIST_SELECTED, this );
	m_listSelected.InsertColumn ( 0, "No", LVCFMT_LEFT, 50 );
	m_listSelected.InsertColumn ( 1, "Department", LVCFMT_LEFT, 190 );
	m_listSelected.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listSelected.SetItemCountEx( m_arraySelected.GetSize() );

	m_listAvailable.SubclassDlgItem ( IDC_LIST_AVAILABLE, this );
	m_listAvailable.InsertColumn ( 0, "No", LVCFMT_LEFT, 50 );
	m_listAvailable.InsertColumn ( 1, "Department", LVCFMT_LEFT, 190 );
	m_listAvailable.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listAvailable.SetItemCountEx( m_arrayAvailable.GetSize() );

	m_TwinListHelpers.SelectLine( FALSE, 0 );
	m_TwinListHelpers.SelectLine( TRUE, 0 );

	return TRUE;  
}

/**********************************************************************/

void CPropPagePMSOptionsDinner::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
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

void CPropPagePMSOptionsDinner::GetLineText( CReportConsolidationArray<CSortedIntWithFlag>& arrayItems, int nIdx, CCSV& csv )
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

void CPropPagePMSOptionsDinner::OnGetDispInfoListSelected(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		CCSV csv( '\t' );
		GetLineText( m_arraySelected,  pDispInfo->item.iItem, csv );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CPropPagePMSOptionsDinner::OnGetDispInfoListAvailable(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		CCSV csv( '\t' );
		GetLineText( m_arrayAvailable,  pDispInfo->item.iItem, csv );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CPropPagePMSOptionsDinner::OnButtonAdd()
{
	m_TwinListHelpers.MoveItems( TRUE );
}

/**********************************************************************/

void CPropPagePMSOptionsDinner::OnButtonRemove()
{
	m_TwinListHelpers.MoveItems( FALSE );
}

/**********************************************************************/

void CPropPagePMSOptionsDinner::OnDoubleClickListSelected(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if ( m_TwinListHelpers.SelectSingleLine( FALSE ) == TRUE )
		m_TwinListHelpers.MoveItems( FALSE );

	*pResult = 0;
}

/**********************************************************************/

void CPropPagePMSOptionsDinner::OnDoubleClickListAvailable(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if ( m_TwinListHelpers.SelectSingleLine( TRUE ) == TRUE )
		m_TwinListHelpers.MoveItems( TRUE );

	*pResult = 0;
}

/**********************************************************************/

BOOL CPropPagePMSOptionsDinner::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePMSOptionsDinner::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPagePMSOptionsDinner::SaveRecord()
{
	m_pPMSOptions -> SetDinnerAllowance( m_dDinnerAllowance );

	CCSV csv;

	for ( int n = 0; n < m_arraySelected.GetSize(); n++ )
	{
		CSortedIntWithFlag item;
		m_arraySelected.GetAt( n, item );
		csv.Add( item.m_nInt );
	}

	m_pPMSOptions -> SetDinnerDepartments( csv.GetLine() );
}

/**********************************************************************/
#endif
/**********************************************************************/
