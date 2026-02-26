/**********************************************************************/
#include "NewRecordNumberDlg.h"
#include "PropertySheetLoyaltyScheme.h"
#include "PropertySheetPromotion.h"
#include "ReportConsolidationArray.h"
#include "SelectMultiplePromoDlg.h"
/**********************************************************************/
#include "PropPageLoyaltySchemeAction.h"
/**********************************************************************/

CPropPageLoyaltySchemeAction::CPropPageLoyaltySchemeAction() : CSSPropertyPage(CPropPageLoyaltySchemeAction::IDD), m_MacroSelector( m_editMacro, m_comboMacro )
{
	//{{AFX_DATA_INIT(CPropPageLoyaltySchemeAction)
	//}}AFX_DATA_INIT
	m_nLastActionComboSel = 0;
	m_nMacroNo = 0;
	m_pSchemeRecord = NULL;
}

/**********************************************************************/

CPropPageLoyaltySchemeAction::~CPropPageLoyaltySchemeAction()
{
}

/**********************************************************************/

void CPropPageLoyaltySchemeAction::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLoyaltySchemeAction)
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_CHECK_ACTIVE, m_checkActive);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboActionType);
	DDX_Control(pDX, IDC_COMBO_SELECTION, m_comboActionDetail);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_buttonEdit);
	DDX_Control(pDX, IDC_BUTTON_SELECT, m_buttonSelect);
	DDX_Control(pDX, IDC_STATIC_MACRO, m_staticMacro);
	DDX_Control(pDX, IDC_EDIT_MACRO, m_editMacro);
	DDX_Control(pDX, IDC_COMBO_MACRO, m_comboMacro);
	DDX_Control(pDX, IDC_STATIC_PENCE, m_staticPence);
	DDX_Control(pDX, IDC_STATIC_PENCE2, m_staticPence2);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageLoyaltySchemeAction, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageLoyaltySchemeAction)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelectType)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_EN_KILLFOCUS(IDC_EDIT_MACRO, OnKillFocusMacro)
	ON_CBN_SELCHANGE(IDC_COMBO_MACRO, OnSelectMacro)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageLoyaltySchemeAction::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	SubclassEdit( IDC_EDIT_PENCE, SS_NUM, 6, "%d" );
	
	m_editName.LimitText( LoyaltyScheme::SchemeName.Max );

	m_comboActionType.AddString( "None" );
	m_comboActionType.AddString( "Apply Item Promotion" );
	m_comboActionType.AddString( "Apply Subtotal Promotion" );
	m_comboActionType.AddString( "Apply Multiple Subtotal Promotions" );
	m_comboActionType.AddString( "Set Price Band" );
	m_comboActionType.SetItemData(0,LOYALTYSCHEME_ACTION_NONE);
	m_comboActionType.SetItemData(1,LOYALTYSCHEME_ACTION_PROMO_ITEM);
	m_comboActionType.SetItemData(2,LOYALTYSCHEME_ACTION_PROMO_SUBTOTAL);
	m_comboActionType.SetItemData(3,LOYALTYSCHEME_ACTION_PROMO_MULTIPLE);
	m_comboActionType.SetItemData(4,LOYALTYSCHEME_ACTION_PRICEBAND);

	m_listPromotion.SubclassDlgItem ( IDC_LIST, this );
	m_listPromotion.InsertColumn ( 0, "Promotions", LVCFMT_LEFT, 270 );
	
	m_listPromotion.SetItemCountEx(5);
	m_listPromotion.ModifyStyle( 0, LVS_SINGLESEL, 0 );
	m_listPromotion.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	MoveControl( &m_listPromotion, 101, 115, 200, 100 );
	MoveControl( &m_buttonSelect, 309, 115, 33, 13 );
		
	m_editMacro.LimitText( 4 );
	m_MacroSelector.FillMacroCombo(0);

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(40);
	m_comboActionDetail.SetTabStops( arrayStops );
		
	SetRecordControls();

	return TRUE;  
}

/**********************************************************************/
CEdit* CPropPageLoyaltySchemeAction::GetEditPence(){ return GetEdit( IDC_EDIT_PENCE ); }
/**********************************************************************/

void CPropPageLoyaltySchemeAction::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listPromotion.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

void CPropPageLoyaltySchemeAction::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		int nPromoNo = m_pSchemeRecord -> GetPromoNo( nIndex );
		
		CString strLine = "";
		if ( nPromoNo != 0 )
		{
			strLine = DataManager.Promotion.GetSimpleListText(nPromoNo);
		}

		lstrcpy ( pDispInfo->item.pszText, strLine );
	}
	*pResult = 0;
}

/**********************************************************************/

void CPropPageLoyaltySchemeAction::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonSelect();
	*pResult = 0;
}

/**********************************************************************/

BOOL CPropPageLoyaltySchemeAction::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageLoyaltySchemeAction::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageLoyaltySchemeAction::GetRecordData()
{
	if ( m_pSchemeRecord -> GetActionType() == 4 )
		m_pSchemeRecord -> TidyPromos();
}

/**********************************************************************/

void CPropPageLoyaltySchemeAction::SetRecordControls()
{
	m_editName.SetWindowText( m_pSchemeRecord -> GetSchemeName() );
	m_checkActive.SetCheck( m_pSchemeRecord -> GetActiveFlag() );

	int nActionType = m_pSchemeRecord -> GetActionType();
	int nSelection = m_pSchemeRecord -> GetSelection();

	m_comboActionType.SelectUsingItemData( nActionType, 0 );
	m_nLastActionComboSel = m_comboActionType.GetCurSel();

	FillSelectionCombo();
	ForceSelection( nSelection );

	m_listPromotion.Invalidate();
	
	m_nMacroNo = m_pSchemeRecord -> GetMacroNo();
	m_MacroSelector.UpdateMacroCombo( m_nMacroNo );
	m_MacroSelector.UpdateMacroEditBox();

	SetEditBoxInt( *GetEditPence(), m_pSchemeRecord -> GetPencePerPoint() );
}

/**********************************************************************/

bool CPropPageLoyaltySchemeAction::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageLoyaltySchemeAction::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/

void CPropPageLoyaltySchemeAction::FillSelectionCombo()
{
	m_comboActionDetail.ResetContent();

	switch( m_pSchemeRecord -> GetActionType() )
	{
	case LOYALTYSCHEME_ACTION_NONE:	
		ShowAndEnableWindow( &m_buttonSelect, FALSE );
		ShowAndEnableWindow( &m_buttonEdit, FALSE );
		ShowAndEnableWindow( &m_comboActionDetail, TRUE, FALSE );
		ShowAndEnableWindow( &m_listPromotion, FALSE );
		break;

	case LOYALTYSCHEME_ACTION_PROMO_MULTIPLE:	
		ShowAndEnableWindow( &m_buttonSelect, TRUE );
		ShowAndEnableWindow( &m_buttonEdit, TRUE );
		MoveControl( &m_buttonEdit, 309, 135, 33, 13 );
		ShowAndEnableWindow( &m_comboActionDetail, FALSE );
		ShowAndEnableWindow( &m_listPromotion, TRUE );
		m_listPromotion.Invalidate();
		break;

	case LOYALTYSCHEME_ACTION_PROMO_ITEM:		
	case LOYALTYSCHEME_ACTION_PROMO_SUBTOTAL: 
		{
			ShowAndEnableWindow( &m_buttonSelect, FALSE );
			ShowAndEnableWindow( &m_buttonEdit, TRUE );
			MoveControl( &m_buttonEdit, 268, 115, 33, 13 );
			ShowAndEnableWindow( &m_comboActionDetail, TRUE );
			ShowAndEnableWindow( &m_listPromotion, FALSE );
	
			m_comboActionDetail.AddItem( "P00,None" );
			m_comboActionDetail.SetItemData( 0, 0 );
			
			for ( int n = 0; n < DataManager.Promotion.GetSize(); n++ )
			{
				int nPromoNo = DataManager.Promotion.GetPromoNoByIndex(n);
				m_comboActionDetail.AddItem( DataManager.Promotion.GetTabbedComboText(nPromoNo) );
				m_comboActionDetail.SetItemData( n + 1, nPromoNo );
			}
		}
		break;

	case LOYALTYSCHEME_ACTION_PRICEBAND:
		{
			ShowAndEnableWindow( &m_buttonSelect, FALSE );
			ShowAndEnableWindow( &m_buttonEdit, FALSE );
			ShowAndEnableWindow( &m_comboActionDetail, TRUE );
			ShowAndEnableWindow( &m_listPromotion, FALSE );
	
			for ( int p = 0; p < MAX_PLU_PRICES; p++ )
			{
				m_comboActionDetail.AddItem( DataManager.PriceText.GetTabbedComboText(p) );
				m_comboActionDetail.SetItemData( p, p );
			}
		}
		break;
	}

	int nYOffset = ( m_comboActionType.GetCurSel() == 3 ) ? 0 : 70;
	MoveControl( &m_staticMacro, 21, 224 - nYOffset );
	MoveControl( &m_editMacro, 101, 223 - nYOffset );
	MoveControl( &m_comboMacro, 145, 224 - nYOffset );
	MoveControl( &m_staticPence, 21, 244 - nYOffset );
	MoveControl( &m_staticPence2, 27, 258 - nYOffset );
	MoveControl( GetEditPence(), 101, 243 - nYOffset );
}

/**********************************************************************/

void CPropPageLoyaltySchemeAction::ForceSelection( int nSelection )
{
	switch( m_pSchemeRecord -> GetActionType() )
	{
	case LOYALTYSCHEME_ACTION_PROMO_ITEM: 
	case LOYALTYSCHEME_ACTION_PROMO_SUBTOTAL: 
	case LOYALTYSCHEME_ACTION_PRICEBAND: 
		{
			if ( m_comboActionDetail.SelectUsingItemData( nSelection ) == FALSE )
			{
				CString str;
				str.Format( "%s%2.2d,Unknown", 
					( m_pSchemeRecord -> GetActionType() == LOYALTYSCHEME_ACTION_PRICEBAND ) ? "B" : "P",
					nSelection );
				
				m_comboActionDetail.InsertItem( 0, str );
				m_comboActionDetail.SetItemData( 0, nSelection );
				m_comboActionDetail.SetCurSel(0);
			}

			m_comboActionDetail.EnableWindow( m_comboActionDetail.GetCount() > 1 );
		}
		break;

	case LOYALTYSCHEME_ACTION_PROMO_MULTIPLE: 
		{
			for ( int n = 0; n < 5; n++ )
			{
				if ( m_pSchemeRecord -> GetPromoNo(n) == nSelection )
				{
					m_listPromotion.SetCurSel(n);
					break;
				}
			}
		}
		break;
	}
}

/**********************************************************************/

void CPropPageLoyaltySchemeAction::OnSelectType() 
{
	int nActionComboSel = m_comboActionType.GetCurSel();

	if ( nActionComboSel != m_nLastActionComboSel )
	{
		m_pSchemeRecord -> SetActionTypeAndSelection( m_comboActionType.GetItemData( nActionComboSel ), 0 );

		FillSelectionCombo();
		m_nLastActionComboSel = nActionComboSel;

		switch( m_pSchemeRecord -> GetActionType() )
		{
		case LOYALTYSCHEME_ACTION_PROMO_ITEM: 
		case LOYALTYSCHEME_ACTION_PROMO_SUBTOTAL: 
		case LOYALTYSCHEME_ACTION_PRICEBAND: 
			{
				if ( m_comboActionDetail.GetCount() >= 1 )
					m_comboActionDetail.SetCurSel(0);
				else
					ForceSelection(0);
			}
			break;
		}

		switch( m_pSchemeRecord -> GetActionType() )
		{
		case LOYALTYSCHEME_ACTION_PROMO_ITEM: 
		case LOYALTYSCHEME_ACTION_PROMO_SUBTOTAL: 
		case LOYALTYSCHEME_ACTION_PROMO_MULTIPLE: 
			{
				for ( int nSlot = 0; nSlot < 5; nSlot++ )
				{
					m_pSchemeRecord -> SetPromoNo ( nSlot, 0 );
					m_listPromotion.SetCurSel(0);
				}
			}
			break;
		}
	}
}

/**********************************************************************/

void CPropPageLoyaltySchemeAction::OnButtonSelect()
{
	if( m_comboActionType.GetCurSel() == 3 )
	{
		m_pSchemeRecord -> TidyPromos();

		CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo> arrayPromos;
		CReportConsolidationArray<CSortedIntItem> arrayInUse;

		for ( int n = 0; n < 5; n++ )
		{
			int nPromoNo = m_pSchemeRecord -> GetPromoNo(n);

			if ( nPromoNo > 0 )
			{
				CSortedIntItem item;
				item.m_nItem = nPromoNo;
				arrayInUse.DirectAdd( item );

				CSelectMultipleEntityInfo info;
				info.m_nPromoNo = nPromoNo;
				info.m_bSelected = TRUE;
				arrayPromos.Add( info );
			}
		}

		for ( int n = 0; n < DataManager.Promotion.GetSize(); n++ )
		{
			CPromotionsCSVRecord Promo;
			DataManager.Promotion.GetAt( n, Promo );
			int nPromoNo = Promo.GetPromoNo();

			switch( Promo.GetUsage() )
			{
			case 2: //SUBTOTAL
			case 3:	//ITEM OR SUBTOTAL
				CSortedIntItem item;
				item.m_nItem = nPromoNo;

				int nIdx;
				if ( arrayInUse.Find( item, nIdx ) == FALSE )
				{
					CSelectMultipleEntityInfo info;
					info.m_nPromoNo = nPromoNo;
					info.m_bSelected = FALSE;
					arrayPromos.Add( info );
				}
				break;
			}
		}
		
		CSelectMultiplePromoDlg dlg( arrayPromos, this );
		if ( dlg.DoModal() == IDOK )
		{
			int nSlot = 0;
			for ( int n = 0; ( n < arrayPromos.GetSize() ) && ( nSlot < 5 ); n++ )
			{
				if ( TRUE == arrayPromos[n].m_bSelected )
				{
					m_pSchemeRecord -> SetPromoNo( nSlot++, arrayPromos[n].m_nPromoNo );
				}
			}

			for (; nSlot < 5; nSlot++)
			{
				m_pSchemeRecord->SetPromoNo(nSlot, 0);
			}

			m_pSchemeRecord -> TidyPromos();
			m_listPromotion.Invalidate();
		}
	}
}

/**********************************************************************/

void CPropPageLoyaltySchemeAction::OnButtonEdit()
{
	int nOldPromoNo = -1;

	int nPromoSel = 0;
	
	switch( m_pSchemeRecord -> GetActionType() )
	{
	case LOYALTYSCHEME_ACTION_PROMO_ITEM: 
	case LOYALTYSCHEME_ACTION_PROMO_SUBTOTAL: 
		{	
			nPromoSel = m_comboActionDetail.GetCurSel();
			if ( ( nPromoSel >= 0 ) && ( nPromoSel < m_comboActionDetail.GetCount() ) )
			{
				nOldPromoNo = m_comboActionDetail.GetItemData( nPromoSel );
			}
		}
		break;

	case LOYALTYSCHEME_ACTION_PROMO_MULTIPLE:
		{
			nPromoSel = m_listPromotion.GetCurSel();
			if ( ( nPromoSel >= 0 ) && ( nPromoSel <= 4 ) )
			{
				nOldPromoNo = m_pSchemeRecord -> GetPromoNo(nPromoSel);
			}
		}
		break;
	}

	bool bMultiPromo = ( m_pSchemeRecord -> GetActionType() == LOYALTYSCHEME_ACTION_PROMO_MULTIPLE );
		
	if ( nOldPromoNo == 0 )
	{
		int nNewPromoNo = DataManager.Promotion.FindFirstFreeNumber();

		if ( nNewPromoNo < 1 )
		{
			Prompter.Error ( "All available promotions are already in use." );
			return;
		}

		CNewRecordNumberDlg dlg ( NODE_PROMOTION, this );
		dlg.m_nNumber = nNewPromoNo;
		if (dlg.DoModal() != IDOK)
		{
			return;
		}

		int nPromoIdx = 0;
		nNewPromoNo = dlg.m_nNumber;
		if ( DataManager.Promotion.FindTableByNumber ( nNewPromoNo, nPromoIdx ) )
		{
			Prompter.Error ( "The selected promotion is already in use." );
			return;
		}

		CPromotionsCSVRecord Promo;
		Promo.SetPromoNo( nNewPromoNo );
		Promo.SetDescription ( "" );

		if ( TRUE == bMultiPromo )
		{
			Promo.SetUsage(2);
		}

		DataManager.Promotion.InsertAt ( nPromoIdx, Promo );

		DataManager.Department.SetDeptZero( "None" );

		CPropertySheetPromotion propSheet( IDC_SPIN, FALSE, nPromoIdx, this );
		propSheet.SetSubtotalOnlyFlag( bMultiPromo );
		if (propSheet.DoModal() == IDOK)
		{
			propSheet.SaveRecord();
		}

		DataManager.Department.SetDeptZero( "PROMO" );

		if ( TRUE == bMultiPromo )
		{
			m_pSchemeRecord -> SetPromoNo( nPromoSel, Promo.GetPromoNo() );
			m_pSchemeRecord -> TidyPromos();
			ForceSelection( nNewPromoNo );
			m_listPromotion.Invalidate();
		}
		else
		{
			FillSelectionCombo();
			ForceSelection( nNewPromoNo );
		}
	}
	else
	{
		int nPromoIdx = 0;
		if ( DataManager.Promotion.FindTableByNumber( nOldPromoNo, nPromoIdx ) == TRUE )
		{
			DataManager.Department.SetDeptZero( "None" );

			CPropertySheetPromotion propSheet( IDC_SPIN, FALSE, nPromoIdx, this );
			if ( propSheet.DoModal() == IDOK )
			{
				propSheet.SaveRecord();

				if ( TRUE == bMultiPromo )
				{
					m_pSchemeRecord -> TidyPromos();
					m_listPromotion.Invalidate();
				}
				else
				{
					FillSelectionCombo();
					ForceSelection( nOldPromoNo );
				}
			}

			DataManager.Department.SetDeptZero( "PROMO" );
		}
		else
		{
			if ( nOldPromoNo >= 1 && nOldPromoNo <= Promotions::PromoNo.Max )
			{
				CPromotionsCSVRecord Promo;
				Promo.SetPromoNo( nOldPromoNo );
				Promo.SetDescription ( "" );
				DataManager.Promotion.InsertAt ( nPromoIdx, Promo );

				DataManager.Department.SetDeptZero( "None" );
				
				CPropertySheetPromotion propSheet( IDC_SPIN, FALSE, nPromoIdx, this );
				if ( propSheet.DoModal() == IDOK )
					propSheet.SaveRecord();

				DataManager.Department.SetDeptZero( "PROMO" );
	
				if ( TRUE == bMultiPromo )
				{
					m_pSchemeRecord -> SetPromoNo( nPromoSel, nOldPromoNo );
					m_pSchemeRecord -> TidyPromos();
					ForceSelection( nOldPromoNo );
					m_listPromotion.Invalidate();
				}
				else
				{
					FillSelectionCombo();
					ForceSelection( nOldPromoNo );
				}
			}
		}
	}
}

/**********************************************************************/

void CPropPageLoyaltySchemeAction::OnSelectMacro()
{
	m_MacroSelector.SelectMacroFromCombo();
	m_nMacroNo = m_MacroSelector.GetMacroNo();
}

/**********************************************************************/

void CPropPageLoyaltySchemeAction::OnKillFocusMacro()
{
	m_MacroSelector.SelectMacroFromEditBox();
	m_nMacroNo = m_MacroSelector.GetMacroNo();
}

/**********************************************************************/

void CPropPageLoyaltySchemeAction::SaveRecord()
{
	CString strName;
	m_pSchemeRecord -> SetSchemeName( GetEditBoxText( m_editName ) );
	m_pSchemeRecord -> SetActiveFlag( IsTicked( m_checkActive ) );

	{
		int nActionType = m_comboActionType.GetItemData( m_comboActionType.GetCurSel() );
		
		int nSelection = 0;

		switch( nActionType )
		{
		case LOYALTYSCHEME_ACTION_PROMO_ITEM: 
		case LOYALTYSCHEME_ACTION_PROMO_SUBTOTAL: 
		case LOYALTYSCHEME_ACTION_PRICEBAND: 
			{
				int nComboSel = m_comboActionDetail.GetCurSel();
				if ( ( nComboSel >= 0 ) && ( nComboSel < m_comboActionDetail.GetCount() ) )
					nSelection = m_comboActionDetail.GetItemData( nComboSel );
			}
			break;
			
		case LOYALTYSCHEME_ACTION_PROMO_MULTIPLE: 
			nSelection = m_pSchemeRecord -> GetPromoNo(0);
			break;
		}

		m_pSchemeRecord -> SetActionTypeAndSelection( nActionType, nSelection );
	}

	m_pSchemeRecord -> SetPencePerPoint( GetEditBoxInt(*GetEditPence() ) );
	m_pSchemeRecord -> SetMacroNo( m_nMacroNo );
}

/**********************************************************************/
