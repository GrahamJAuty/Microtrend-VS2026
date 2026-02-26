/**********************************************************************/
#include "SPOSKeyboardPropertySheetEditList.h"
/**********************************************************************/
#include "SPOSKeyboardPropPageEditListGeneral.h"
/**********************************************************************/

CSPOSKeyboardPropPageEditListGeneral::CSPOSKeyboardPropPageEditListGeneral() : CSSPropertyPage(CSPOSKeyboardPropPageEditListGeneral::IDD)
{
	//{{AFX_DATA_INIT(CSPOSKeyboardPropPageEditListGeneral)
	m_strName = _T("");
	m_nMinItems = 0;
	m_nMaxItems = 0;
	m_nTableNoLo = 0;
	m_nTableNoHi = 0;
	m_nTableArea = 0;
	m_nNextList = 0;
	//}}AFX_DATA_INIT
	m_bWantPriority = FALSE;
	m_bWantStackMode = FALSE;
	m_bWantTableArea = FALSE;
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditListGeneral::SetListRecord( CSPOSKeyboardListRecord* pRecord )
{
	m_pListRecord = pRecord;
}

/**********************************************************************/

CSPOSKeyboardPropPageEditListGeneral::~CSPOSKeyboardPropPageEditListGeneral()
{
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditListGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardPropPageEditListGeneral)
	DDX_Control(pDX, IDC_STATIC_TABLENOLO, m_staticTableNoLo);
	DDX_Control(pDX, IDC_STATIC_TABLENOHI, m_staticTableNoHi);
	DDX_Control(pDX, IDC_STATIC_TABLEAREA, m_staticTableArea);
	DDX_Control(pDX, IDC_STATIC_PRIORITY, m_staticPriority);
	DDX_Control(pDX, IDC_STATIC_STACKMODE, m_staticStackMode);
	DDX_Control(pDX, IDC_EDIT_KEYS, m_editKeys);
	DDX_Control(pDX, IDC_EDIT_PAGES, m_editPages);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_Control(pDX, IDC_COMBO_STACKMODE, m_comboStackMode);
	DDX_Control(pDX, IDC_COMBO_STAYDOWN, m_comboStaydown);
	DDX_Control(pDX, IDC_COMBO_PLULINK, m_comboPluLink);
	DDX_Control(pDX, IDC_COMBO_PRIORITY, m_comboPriority);
	DDX_Control(pDX, IDC_COMBO_FORMAT, m_comboFormat);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 25);
	DDX_Text(pDX, IDC_EDIT_MINITEMS, m_nMinItems);
	DDX_Text(pDX, IDC_EDIT_MAXITEMS, m_nMaxItems);
	DDX_Text(pDX, IDC_EDIT_TABLENOLO, m_nTableNoLo);
	DDX_Text(pDX, IDC_EDIT_TABLENOHI, m_nTableNoHi);
	DDX_Text(pDX, IDC_EDIT_TABLEAREA, m_nTableArea);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_NEXTLIST, m_nNextList);
	DDV_MinMaxInt(pDX, m_nNextList, 0, MAX_LIST_NO); //CHECKED 28-07-03
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPropPageEditListGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CSPOSKeyboardPropPageEditListGeneral)
	ON_CBN_SELCHANGE(IDC_COMBO_FORMAT, OnSelectFormat)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelectType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditListGeneral::OnInitDialog() 
{
	m_strName = m_pListRecord -> GetName();
	m_nMinItems = m_pListRecord -> GetMinItems();
	m_nMaxItems = m_pListRecord -> GetMaxItems();
	m_nTableNoLo = m_pListRecord -> GetTableNoLo();
	m_nTableNoHi = m_pListRecord -> GetTableNoHi();
	m_nTableArea = m_pListRecord -> GetTableArea();
	m_nNextList = m_pListRecord -> GetNextList();
	m_nListType = m_pListRecord -> GetListType();

	CPropertyPage::OnInitDialog();
	SubclassEdit ( IDC_EDIT_MINITEMS, SS_NUM, 2, "%d" );
	SubclassEdit ( IDC_EDIT_MAXITEMS, SS_NUM, 2, "%d" );
	SubclassEdit ( IDC_EDIT_TABLENOLO, SS_NUM, 3, "%d" );
	SubclassEdit ( IDC_EDIT_TABLENOHI, SS_NUM, 3, "%d" );
	SubclassEdit ( IDC_EDIT_TABLEAREA, SS_NUM, 2, "%d" );
	SubclassEdit ( IDC_EDIT_NEXTLIST, SS_NUM, 5, "%d" );

	int nType = m_pListRecord -> GetListActionType();
	if ( ( nType < 0 ) || ( nType > 2 ) )
		nType = 0;

	m_comboType.AddString( "Normal action" );
	m_comboType.AddString( "Active servers" );
	m_comboType.AddString( "Active tables" );
	m_comboType.SetCurSel( nType );

	int nMode = m_pListRecord -> GetStackMode();
	if ( ( nMode < 0 ) || ( nMode > 1 ) )
		nMode = 0;

	m_comboStackMode.AddString( "Normal stack" );
	m_comboStackMode.AddString( "Don't stack" );
	m_comboStackMode.SetCurSel( nMode );

	m_comboPluLink.AddString("Show in drop down list");
	m_comboPluLink.AddString("Select by number only");
	m_comboPluLink.SetCurSel(m_pListRecord->GetOfferPluLinkFlag() ? 0 : 1);

	if ( nType != 2 )
	{
		m_staticTableNoLo.ShowWindow( SW_HIDE );
		m_staticTableNoHi.ShowWindow( SW_HIDE );
		m_staticTableArea.ShowWindow( SW_HIDE );
		GetEditTableNoLo() -> ShowWindow( SW_HIDE );
		GetEditTableNoHi() -> ShowWindow( SW_HIDE );
		GetEditTableArea() -> ShowWindow( SW_HIDE );
	}

	CWordArray arrayFormats;

	switch( m_nListType )
	{
	case LISTTYPE_ITEM_STANDARD:
		arrayFormats.Add ( LIST_FORMAT_1x5 );
		arrayFormats.Add ( LIST_FORMAT_2x3 );
		arrayFormats.Add ( LIST_FORMAT_2x4 );
		arrayFormats.Add ( LIST_FORMAT_3x3 );
		arrayFormats.Add ( LIST_FORMAT_3x4 );
		arrayFormats.Add ( LIST_FORMAT_4x4 );
		arrayFormats.Add ( LIST_FORMAT_4x5 );
		arrayFormats.Add ( LIST_FORMAT_4x6 );
		arrayFormats.Add ( LIST_FORMAT_5x4 );
		arrayFormats.Add ( LIST_FORMAT_5x6 );
		arrayFormats.Add ( LIST_FORMAT_5x7 );
		arrayFormats.Add ( LIST_FORMAT_6x5 );
		arrayFormats.Add ( LIST_FORMAT_12x10 );
		break;
				
	case LISTTYPE_ITEM_LARGE:
		arrayFormats.Add ( LIST_FORMAT_2x1 );
		arrayFormats.Add ( LIST_FORMAT_2x2 );
		arrayFormats.Add ( LIST_FORMAT_2x3 );
		arrayFormats.Add ( LIST_FORMAT_3x1 );
		arrayFormats.Add ( LIST_FORMAT_3x2 );
		arrayFormats.Add ( LIST_FORMAT_3x3 );
		arrayFormats.Add ( LIST_FORMAT_4x1 );
		arrayFormats.Add ( LIST_FORMAT_4x3 );
		arrayFormats.Add ( LIST_FORMAT_4x4 );
		arrayFormats.Add ( LIST_FORMAT_4x5 );
		arrayFormats.Add ( LIST_FORMAT_5x1 );
		arrayFormats.Add ( LIST_FORMAT_5x2 );
		arrayFormats.Add ( LIST_FORMAT_5x4 );
		arrayFormats.Add ( LIST_FORMAT_5x5 );
		arrayFormats.Add ( LIST_FORMAT_6x1 );
		arrayFormats.Add ( LIST_FORMAT_6x5 );
		arrayFormats.Add ( LIST_FORMAT_6x6 );
		arrayFormats.Add ( LIST_FORMAT_7x1 );
		arrayFormats.Add ( LIST_FORMAT_7x7 );
		arrayFormats.Add ( LIST_FORMAT_9x6 );

		switch( SPOSKeyboardManager.GetSPOSVersion() -> GetSPOSVersion() )
		{
		case SPOS_V4_2_714:
		case SPOS_V4_2_919:
		case SPOS_V4_2_959:
		case SPOS_V4_2_1081:
		case SPOS_V4_2_1087:
		case SPOS_V4_2_1384:
		case SPOS_V4_2_1456:
		case SPOS_V4_2_1530:
		case SPOS_V4_2_1579:
		case SPOS_V4_2_1605:
		case SPOS_V4_2_1618:
		case SPOS_V4_2_1622:
		case SPOS_V4_3_18:
		case SPOS_V4_3_93:
		case SPOS_V4_3_104:
			arrayFormats.Add ( LIST_FORMAT_8x5 );
			arrayFormats.Add ( LIST_FORMAT_10x5 );
			arrayFormats.Add ( LIST_FORMAT_12x5 );
			break;	
		}

		arrayFormats.Add ( LIST_FORMAT_12x10 );
		arrayFormats.Add ( LIST_FORMAT_15x12 );
		break;

	case LISTTYPE_FUNCTION_STANDARD:
		arrayFormats.Add ( LIST_FORMAT_5x3 );
		break;

	case LISTTYPE_FUNCTION_LARGE:
		arrayFormats.Add ( LIST_FORMAT_5x6 );
		break;

	case LISTTYPE_FUNCTION_BOTTOM:
		arrayFormats.Add ( LIST_FORMAT_1x10 );
		arrayFormats.Add ( LIST_FORMAT_1x14 );
		break;
	}

	CArray<int,int> arrayTabStops;
	arrayTabStops.Add(2);
	arrayTabStops.Add(22);
	arrayTabStops.Add(37);
	arrayTabStops.Add(100);
	m_comboFormat.SetTabStops( arrayTabStops );
	
	CString strLabel;
	for ( int nIndex = 0; nIndex < arrayFormats.GetSize(); nIndex++ )
	{
		int nFormat = arrayFormats.GetAt ( nIndex );
		strLabel.Format ( "%d,x,%d", ( nFormat / 100 ), ( nFormat % 100 ) );
		int nPos = m_comboFormat.AddItem ( strLabel );
		m_comboFormat.SetItemData ( nPos, nFormat );
	}

	m_comboFormat.SetCurSel(0);
	int nCurrentFormat = m_pListRecord -> GetFormat();
	for ( int nIndex = 0; nIndex < m_comboFormat.GetCount(); nIndex++ )
	{
		if ( m_comboFormat.GetItemData( nIndex ) == (WORD) nCurrentFormat )
		{
			m_comboFormat.SetCurSel( nIndex );
			break;
		}
	}

	m_comboFormat.EnableWindow( m_comboFormat.GetCount() > 1 );

	m_comboStaydown.AddString( "Yes" );
	m_comboStaydown.AddString( "No" );
	m_comboStaydown.SetCurSel ( m_pListRecord -> GetStaydownFlag() ? 0 : 1 );

	m_comboPriority.AddString( "Yes" );
	m_comboPriority.AddString( "No" );
	m_comboPriority.SetCurSel ( m_pListRecord -> GetPriorityFlag() ? 0 : 1 );

	CString strPages;
	strPages.Format( "%d", m_pListRecord -> GetPageCount() );
	m_editPages.SetWindowText( strPages );

	CString strKeys;
	strKeys.Format ( "%d", m_pListRecord -> GetPageCount() * m_pListRecord -> GetKeysPerPage() );
	m_editKeys.SetWindowText( strKeys );

	m_bWantPriority = TRUE;
	m_bWantStackMode = TRUE;
	m_bWantTableArea = TRUE;

	switch( SPOSKeyboardManager.GetSPOSVersion() -> GetSPOSVersion() )
	{
	case SPOS_V4:
	case SPOS_V4_210:
		m_bWantPriority = FALSE;
		m_bWantStackMode = FALSE;
		m_bWantTableArea = FALSE;
		break;

	case SPOS_V4_321:
	case SPOS_V4_389:
	case SPOS_V4_423:
	case SPOS_V4_425:
	case SPOS_V4_494:
	case SPOS_V4_614:
	case SPOS_V4_634:
	case SPOS_V4_682:
	case SPOS_V4_719:
	case SPOS_V4_753:
	case SPOS_V4_780:
	case SPOS_V4_827:
	case SPOS_V4_837:
	case SPOS_V4_861:
	case SPOS_V4_958:
	case SPOS_V4_992:
	case SPOS_V4_999:
	case SPOS_V4_1071:
		m_bWantStackMode = FALSE;
		m_bWantTableArea = FALSE;
		break;

	case SPOS_V4_1092:
	case SPOS_V4_1096:
	case SPOS_V4_1124:
	case SPOS_V4_1216:
	case SPOS_V4_1300:
	case SPOS_V4_1392:
		m_bWantTableArea = FALSE;
		break;
	}

	if ( FALSE == m_bWantPriority )
	{
		m_staticPriority.ShowWindow( SW_HIDE );
		m_comboPriority.ShowWindow( SW_HIDE );
		m_comboPriority.EnableWindow( FALSE );
	}

	if ( FALSE == m_bWantStackMode )
	{
		m_staticStackMode.ShowWindow( SW_HIDE );
		m_comboStackMode.ShowWindow( SW_HIDE );
		m_comboStackMode.EnableWindow( FALSE );
	}

	if ( FALSE == m_bWantTableArea )
	{
		m_staticTableArea.ShowWindow( SW_HIDE );
		GetEditTableArea() -> ShowWindow( SW_HIDE );
	}

	return TRUE;  
}

/**********************************************************************/
CEdit* CSPOSKeyboardPropPageEditListGeneral::GetEditTableNoLo(){ return GetEdit( IDC_EDIT_TABLENOLO ); }
CEdit* CSPOSKeyboardPropPageEditListGeneral::GetEditTableNoHi(){ return GetEdit( IDC_EDIT_TABLENOHI ); }
CEdit* CSPOSKeyboardPropPageEditListGeneral::GetEditTableArea(){ return GetEdit( IDC_EDIT_TABLEAREA ); }
/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditListGeneral::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditListGeneral::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditListGeneral::OnSelectFormat() 
{
	int nSel = m_comboFormat.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboFormat.GetCount() )
	{
		CSPOSKeyboardListRecord temp;
		temp.SetListType( m_nListType );
		temp.SetFormat( m_comboFormat.GetItemData( nSel ) );

		CString strKeys;
		strKeys.Format ( "%d", m_pListRecord -> GetPageCount() * temp.GetKeysPerPage() );
		m_editKeys.SetWindowText( strKeys );
	}
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditListGeneral::OnSelectType() 
{
	int nType = m_comboType.GetCurSel();
	((CSPOSKeyboardPropertySheetEditList*) m_pPropertySheet ) -> SetSecondTabType( nType );

	int nShowType = ( 2 == nType ) ? SW_SHOW : SW_HIDE;
	m_staticTableNoLo.ShowWindow( nShowType );
	m_staticTableNoHi.ShowWindow( nShowType );
	GetEditTableNoLo() -> ShowWindow( nShowType );
	GetEditTableNoHi() -> ShowWindow( nShowType );

	if ( TRUE == m_bWantTableArea )
	{
		m_staticTableArea.ShowWindow( nShowType );
		GetEditTableArea() -> ShowWindow( nShowType );
	}
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditListGeneral::SaveRecord()
{
	int nActionType = m_comboType.GetCurSel();

	m_pListRecord -> SetName( m_strName );
	m_pListRecord -> SetListActionType( nActionType );
	m_pListRecord -> SetStackMode( m_comboStackMode.GetCurSel() );
	m_pListRecord -> SetStaydownFlag ( m_comboStaydown.GetCurSel() == 0 );
	m_pListRecord -> SetPriorityFlag ( m_comboPriority.GetCurSel() == 0 );
	m_pListRecord -> SetMinItems( m_nMinItems );
	m_pListRecord -> SetMaxItems( m_nMaxItems );
	m_pListRecord -> SetNextList( m_nNextList );

	if ( 2 == nActionType )
	{
		m_pListRecord -> SetTableNoLo( m_nTableNoLo );
		m_pListRecord -> SetTableNoHi( m_nTableNoHi );
		m_pListRecord -> SetTableArea( m_nTableArea );
	}

	int nSel = m_comboFormat.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboFormat.GetCount() )
		m_pListRecord -> SetFormat( m_comboFormat.GetItemData( nSel ) );

	m_pListRecord->SetOfferPluLinkFlag(m_comboPluLink.GetCurSel() != 1);
}

/**********************************************************************/


