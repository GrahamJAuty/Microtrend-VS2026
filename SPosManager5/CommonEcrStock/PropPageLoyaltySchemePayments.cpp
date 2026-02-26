/**********************************************************************/
#include "PropPageLoyaltySchemePayments.h"
/**********************************************************************/

CPropPageLoyaltySchemePayments::CPropPageLoyaltySchemePayments() : CSSPropertyPage(CPropPageLoyaltySchemePayments::IDD),
m_TwinListHelpers( m_listAllow, m_listDeny, m_arrayAllow, m_arrayDeny )
{
	//{{AFX_DATA_INIT(CPropPageLoyaltySchemePayments)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPageLoyaltySchemePayments::~CPropPageLoyaltySchemePayments()
{
}

/**********************************************************************/

void CPropPageLoyaltySchemePayments::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLoyaltySchemePayments)
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_CHECK_ACTIVE, m_checkActive);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageLoyaltySchemePayments, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageLoyaltySchemePayments)
	//}}AFX_MSG_MAP
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_ALLOW, OnGetDispInfoListAllow)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ALLOW, OnDoubleClickListAllow)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_DENY, OnGetDispInfoListDeny)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DENY, OnDoubleClickListDeny)
	ON_BN_CLICKED(IDC_BUTTON_ALLOW, OnButtonAllow)
	ON_BN_CLICKED(IDC_BUTTON_DENY, OnButtonDeny)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageLoyaltySchemePayments::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();
	
	m_editName.LimitText( LoyaltyScheme::SchemeName.Max );

	m_listAllow.SubclassDlgItem ( IDC_LIST_ALLOW, this );
	m_listAllow.InsertColumn ( 0, "Payments", LVCFMT_LEFT, 180 );
	m_listAllow.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	m_listDeny.SubclassDlgItem ( IDC_LIST_DENY, this );
	m_listDeny.InsertColumn ( 0, "Payments", LVCFMT_LEFT, 180 );
	m_listDeny.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	m_TwinListHelpers.SelectLine( FALSE, 0 );
	m_TwinListHelpers.SelectLine( TRUE, 0 );
	
	SetRecordControls();

	return TRUE;  
}

/**********************************************************************/

void CPropPageLoyaltySchemePayments::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	switch( nIDCtl )
	{
	case IDC_LIST_ALLOW:
		m_listAllow.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;

	case IDC_LIST_DENY:
		m_listDeny.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;
	}
}

/**********************************************************************/

void CPropPageLoyaltySchemePayments::GetLineText( CReportConsolidationArray<CSortedIntWithFlag>& arrayItems, int nIdx, CString& strLine )
{
	if ( ( nIdx >= 0 ) && ( nIdx < arrayItems.GetSize() ) )
	{
		CSortedIntWithFlag arrayItem;
		arrayItems.GetAt( nIdx, arrayItem );

		CPaymentCSVRecord Payment;
		DataManager.Payment.GetAt( arrayItem.m_nInt, Payment );
		strLine.Format ( "P%3.3d  %s", Payment.GetPaymentNo(), Payment.GetDisplayName() );
	}
}

/**********************************************************************/

void CPropPageLoyaltySchemePayments::OnGetDispInfoListAllow(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		CString strLine = "";
		GetLineText( m_arrayAllow,  pDispInfo->item.iItem, strLine );
		lstrcpy ( pDispInfo->item.pszText, strLine );
	}
	*pResult = 0;
}

/**********************************************************************/

void CPropPageLoyaltySchemePayments::OnDoubleClickListAllow(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if ( m_TwinListHelpers.SelectSingleLine( FALSE ) == TRUE )
		m_TwinListHelpers.MoveItems( FALSE );

	*pResult = 0;
}

/**********************************************************************/

void CPropPageLoyaltySchemePayments::OnGetDispInfoListDeny(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		CString strLine = "";
		GetLineText( m_arrayDeny,  pDispInfo->item.iItem, strLine );
		lstrcpy ( pDispInfo->item.pszText, strLine );
	}
	*pResult = 0;
}

/**********************************************************************/

void CPropPageLoyaltySchemePayments::OnDoubleClickListDeny(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if ( m_TwinListHelpers.SelectSingleLine( TRUE ) == TRUE )
		m_TwinListHelpers.MoveItems( TRUE );

	*pResult = 0;
}

/**********************************************************************/

void CPropPageLoyaltySchemePayments::OnButtonAllow()
{
	m_TwinListHelpers.MoveItems( TRUE );
}

/**********************************************************************/

void CPropPageLoyaltySchemePayments::OnButtonDeny()
{
	m_TwinListHelpers.MoveItems( FALSE );
}

/**********************************************************************/

void CPropPageLoyaltySchemePayments::OnButtonAll()
{
	m_arrayAllow.RemoveAll();
	m_arrayDeny.RemoveAll();

	for ( int nPayIdx = 0; nPayIdx < DataManager.Payment.GetSize(); nPayIdx++ )
	{
		CSortedIntWithFlag item;
		item.m_nInt = nPayIdx;
		item.m_nFlag = FALSE;
		m_arrayAllow.DirectAdd( item );
	}

	RedrawLists();
}

/**********************************************************************/

void CPropPageLoyaltySchemePayments::OnButtonNone()
{
	m_arrayAllow.RemoveAll();
	m_arrayDeny.RemoveAll();

	for ( int nPayIdx = 0; nPayIdx < DataManager.Payment.GetSize(); nPayIdx++ )
	{
		CSortedIntWithFlag item;
		item.m_nInt = nPayIdx;
		item.m_nFlag = FALSE;
		m_arrayDeny.DirectAdd( item );
	}

	RedrawLists();
}

/**********************************************************************/

void CPropPageLoyaltySchemePayments::RedrawLists()
{
	m_listAllow.SetItemCountEx( m_arrayAllow.GetSize() );
	m_listDeny.SetItemCountEx( m_arrayDeny.GetSize() );
	m_TwinListHelpers.SelectLine( FALSE, 0 );
	m_TwinListHelpers.SelectLine( TRUE, 0 );
	m_listAllow.RedrawWindow();
	m_listDeny.RedrawWindow();
}

/**********************************************************************/

BOOL CPropPageLoyaltySchemePayments::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageLoyaltySchemePayments::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageLoyaltySchemePayments::GetRecordData()
{
	m_pSchemeRecord -> CopyDeniedPaymentsToArray( m_arrayAllow, m_arrayDeny );
}

/**********************************************************************/

void CPropPageLoyaltySchemePayments::SetRecordControls()
{
	m_editName.SetWindowText( m_pSchemeRecord -> GetSchemeName() );
	m_checkActive.SetCheck( m_pSchemeRecord -> GetActiveFlag() );
	RedrawLists();
}

/**********************************************************************/

bool CPropPageLoyaltySchemePayments::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageLoyaltySchemePayments::SaveRecord()
{
	CString strName;
	m_pSchemeRecord -> SetSchemeName( GetEditBoxText( m_editName ) );
	m_pSchemeRecord -> SetActiveFlag( IsTicked( m_checkActive ) );
	m_pSchemeRecord -> CopyDeniedPaymentsFromArray( m_arrayDeny, TRUE );
}

/**********************************************************************/

void CPropPageLoyaltySchemePayments::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/
