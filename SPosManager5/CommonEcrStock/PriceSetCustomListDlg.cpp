/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "LocationCSVArray.h"
#include "MaxLengths.h"
#include "Password.h"
#include "PluDatabaseHeader.h"
#include "PriceSetCustomListReport.h"
#include "PriceSetCustomListReportHelpers.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PriceSetCustomListDlg.h"
/**********************************************************************/

CPriceSetCustomListDlg::CPriceSetCustomListDlg( int nDbIdx, CWnd* pParent /*=NULL*/)
	: CSSDialog(CPriceSetCustomListDlg::IDD, pParent), 
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName )
{
	//{{AFX_DATA_INIT(CPriceSetCustomListDlg)
	//}}AFX_DATA_INIT
	m_nDepartmentFilter = DEPARTMENTSET_ALL;

	m_nDbIdx = nDbIdx;
	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( m_nDbIdx, info, FALSE );
	m_nPriceSet = 0;

	SetPluRangeAll();
}

/**********************************************************************/

CPriceSetCustomListDlg::~CPriceSetCustomListDlg()
{
	DataManager.CloseDatabase();
}

/**********************************************************************/

void CPriceSetCustomListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPriceSetCustomListDlg)
	DDX_Control(pDX, IDC_STATIC_KEYBOARD, m_staticKeyboard);
	DDX_Control(pDX, IDC_COMBO_KEYBOARD_LOC, m_comboKeyboardLoc);
	DDX_Control(pDX, IDC_COMBO_KEYBOARD_NUM, m_comboKeyboardNum);
	DDX_Control(pDX, IDC_STATIC_DEPARTMENT, m_staticDepartment);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_CHECK_MODIFIERS, m_checkModifiers);
	DDX_Control(pDX, IDC_CHECK_ZEROPRICED, m_checkZeroPriced);
	DDX_Control(pDX, IDC_CHECK_SEPARATE, m_checkSeparate);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_DEPT, m_checkDeptSort);
	DDX_Control(pDX, IDC_RADIO_PRICESET1, m_radioPriceSet1 );
	DDX_Control(pDX, IDC_RADIO_PRICESET2, m_radioPriceSet2 );
	DDX_Control(pDX, IDC_RADIO_PRICESET3, m_radioPriceSet3 );
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_RADIO_PRICESET1, m_nPriceSet);
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_START, m_strPluFrom);
	DDV_IntegerString(pDX, m_strPluFrom, "1", Super.PluEnd() );
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_END, m_strPluTo);
	DDV_IntegerString(pDX, m_strPluTo, "1", Super.PluEnd() );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPriceSetCustomListDlg, CDialog)
	//{{AFX_MSG_MAP(CPriceSetCustomListDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_STATIC_RANGE, OnStaticRange)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_START, OnKillFocusPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_END, OnKillFocusPluNo)
	ON_CBN_SELCHANGE(IDC_COMBO_KEYBOARD_LOC, OnSelectKeyboardLoc)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusEditDeptNumber)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelchangeComboDeptname)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_UP, OnButtonFieldUp)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_DOWN, OnButtonFieldDown)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_RESETORDER, OnButtonFieldResetOrder)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_ALL, OnButtonFieldAll)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_NONE, OnButtonFieldNone)
	ON_BN_CLICKED(IDC_BUTTON_PSET_UP, OnButtonPSetUp)
	ON_BN_CLICKED(IDC_BUTTON_PSET_DOWN, OnButtonPSetDown)
	ON_BN_CLICKED(IDC_BUTTON_PSET_RESETORDER, OnButtonPSetResetOrder)
	ON_BN_CLICKED(IDC_BUTTON_PSET_ALL, OnButtonPSetAll)
	ON_BN_CLICKED(IDC_BUTTON_PSET_NONE, OnButtonPSetNone)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPriceSetCustomListDlg::GetEditPluFrom() { return GetEdit( IDC_EDIT_PLU_RANGE_START ); }
CEdit* CPriceSetCustomListDlg::GetEditPluTo() { return GetEdit( IDC_EDIT_PLU_RANGE_END ); }
/**********************************************************************/

void CPriceSetCustomListDlg::SetPluRangeAll()
{
	__int64 nPluStart = 1;
	__int64 nPluEnd = Plu::PluNo.Max;

	if ( DataManager.Plu.GetSize() != 0 )
	{
		nPluStart = DataManager.Plu.GetPluNoInt(0);
		nPluEnd = DataManager.Plu.GetPluNoInt( DataManager.Plu.GetSize() - 1 );
	}

	::FormatPluNo( nPluStart, m_strPluFrom );
	::FormatPluNo( nPluEnd, m_strPluTo );
}

/**********************************************************************/

BOOL CPriceSetCustomListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strTitle;
	strTitle.Format ( "Print Price Set Custom List (%s)", dbDatabase.GetName( m_nDbIdx ) );
	SetWindowText( strTitle );
	CFilenameUpdater FnUp( SysFiles::PriceSetCustomDef, m_nDbIdx );
	m_strFilenameSettings = FnUp.GetFilenameToUse();
				
	m_KeyboardFilterSelector.FillCombos( &m_comboKeyboardLoc, &m_comboKeyboardNum );

	if ( EcrmanOptions.GetFeaturesKeyboardFlag() == FALSE )
	{
		m_comboKeyboardLoc.EnableWindow( FALSE );
		m_comboKeyboardNum.EnableWindow( FALSE );
	}
	
	SubclassEdit ( IDC_EDIT_PLU_RANGE_START, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_RANGE_END, SS_NUM, Super.MaxPluLen(), "%s" );

	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_nDepartmentFilter, DEPARTMENT_SELECTOR_NORMAL );

	if ( m_nDepartmentFilter != DEPARTMENTSET_ALL )
	{
		m_editDeptNo.EnableWindow ( FALSE );
		m_comboDeptName.EnableWindow ( FALSE );
	}

	m_listLocations.SubclassDlgItem ( IDC_LIST_LOCATION, this );
	m_listLocations.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listLocations.InsertColumn ( 0, "Location", LVCFMT_LEFT, 180 );
	m_listLocations.SetLockItemZeroFlag( FALSE );

	m_listFields.SubclassDlgItem ( IDC_LIST_FIELD, this );
	m_listFields.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listFields.InsertColumn ( 0, "Fields", LVCFMT_LEFT, 180 );
	m_listFields.SetLockItemZeroFlag( TRUE );
	
	CreateAvailableFieldList();	
	AddLocations();

	//READ LINE OF SELECTED FIELDS IF AVAILABLE
	CString strFields = "";
	CString strLocs = "";
	CString strOptions = "";

	CSSFile fileFields;
	if ( fileFields.Open ( m_strFilenameSettings, "rb" ) == TRUE ) 
	{
		fileFields.ReadString ( strFields );
		fileFields.ReadString ( strLocs );
		fileFields.ReadString ( strOptions );
		fileFields.Close();
	}

	CreateAvailableFieldList();

	AddFieldToList( CustomPluList::PluNo.ListLabel, CustomPluList::PluNo.Type, 1, TAGSTATETYPE_2STATEBLUE );

	//TICK THE OPTIONS THAT WERE FOUND IN THE FILE
	CCSV csvFields ( strFields ); 
	for ( int nPos = 0; nPos < csvFields.GetSize(); nPos += 2 )
	{
		int nField = csvFields.GetInt(nPos);
		int nTagState = csvFields.GetInt(nPos + 1 );
	
		CSortedIntAndStringByInt item;	
		item.m_nItem = nField;

		int nFindPos;
		if ( m_arrayAvailableFields.Find( item, nFindPos ) == TRUE )
		{
			m_arrayAvailableFields.GetAt( nFindPos, item );
			AddFieldToList( item.m_strData, item.m_nItem, nTagState, item.m_nData );
			m_arrayAvailableFields.RemoveAt(nFindPos);
		}
	}

	//ADD THE REMAINING UNTICKED ITEMS
	for ( int n = 0; n < m_arrayDefaultFields.GetSize(); n++ )
	{
		CSortedIntAndStringByInt item;
		item.m_nItem = m_arrayDefaultFields.GetAt(n);

		int nFindPos;
		if ( m_arrayAvailableFields.Find( item, nFindPos ) == TRUE )
		{
			m_arrayAvailableFields.GetAt( nFindPos, item );
			AddFieldToList( item.m_strData, item.m_nItem, 0, item.m_nData );
			m_arrayAvailableFields.RemoveAt(nFindPos);
		}
	}

	//SELECT THE LOCATIONS THAT WERE FOUND IN THE FILE
	{
		int nNextTickPos = 0;

		CCSV csvLocs ( strLocs ); 
		for ( int nPos = 0; nPos < csvLocs.GetSize(); nPos++ )
		{
			int nNwkLocNo = csvLocs.GetInt(nPos);
				
			for ( int nIndex = nNextTickPos; nIndex < m_listLocations.GetItemCount(); nIndex++ )
			{
				CSSListTaggedSelectItem ListItem;
				m_listLocations.GetListItem( nIndex, ListItem );

				int nArrayNwkLocNo = dbLocation.GetNetworkLocNo( ListItem.m_nData );
				
				if ( nArrayNwkLocNo == nNwkLocNo )
				{	
					m_listLocations.SetItemTagState( nIndex, 1 );
					m_listLocations.SwapItems( nIndex, nNextTickPos );
					nNextTickPos++;
					break;
				}
			}
		}
	}

	//SELECT THE OPTIONS THAT WERE FOUND IN THE FILE
	{
		CCSV csvOptions ( strOptions ); 
		for ( int nPos = 0; nPos < csvOptions.GetSize(); nPos++ )
		{
			int nOption = csvOptions.GetInt(nPos);
			switch ( nOption )
			{
			case 1000:	m_checkZeroPriced.SetCheck( TRUE );				break;
			case 1001:	m_checkSeparate.SetCheck( TRUE );				break;
			case 1002:	m_checkModifiers.SetCheck( TRUE );				break;
			case 1003:	m_checkDeptSort.SetCheck( TRUE );				break;
			case 1004:	m_nPriceSet = csvOptions.GetInt(++nPos );		break;
			}
		}

		if ( ( m_nPriceSet < 0 ) || ( m_nPriceSet > 2 ) )
			m_nPriceSet = 0;

		m_radioPriceSet1.SetCheck( 0 == m_nPriceSet );
		m_radioPriceSet2.SetCheck( 1 == m_nPriceSet );
		m_radioPriceSet3.SetCheck( 2 == m_nPriceSet );
	}

	m_listFields.SetItemTagState( 0, 1 );

	if ( SysInfo.GetMaxBasePluLen() == 0 )
	{
		m_checkModifiers.SetCheck( FALSE );
		m_checkModifiers.EnableWindow( FALSE );
		m_checkModifiers.ShowWindow( SW_HIDE );
	}

	return TRUE;  
}

/**********************************************************************/

void CPriceSetCustomListDlg::AddLocations()
{
	CArray<int,int> arrayLocIdx;
	dbLocation.GetDatabaseLocIdxSet( m_nDbIdx, arrayLocIdx );

	for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
	{
		int nLocIdx = arrayLocIdx.GetAt(n);

		if ( PasswordArray.CheckLocationAccessLocIdx( nLocIdx ) == TRUE )
		{
			CSSListTaggedSelectItem item;
			item.m_strText = dbLocation.GetName( nLocIdx );
			item.m_nData = nLocIdx;
			item.SetTagState(0);

			m_listLocations.AddItemToList( item );
		}
	}
}

/**********************************************************************/

void CPriceSetCustomListDlg::CreateAvailableFieldList()
{
	m_arrayAvailableFields.RemoveAll();
	m_arrayDefaultFields.RemoveAll();

	//DESCRIPTION AND TYPE
	AddAvailableField(CustomPluList::EposText, TAGSTATETYPE_3STATEBLUEGREEN);
	AddAvailableField(CustomPluList::RepText);
	AddAvailableField(CustomPluList::KeyText, TAGSTATETYPE_3STATEBLUEGREEN);
	AddAvailableField(CustomPluList::SortCode);

#ifdef STOCKMAN_SYSTEM
	AddAvailableField(CustomPluList::StockCode);
#endif

	if (DealerFlags.Get_LIPA_LJMU_ExportType() != 0)
	{
		AddAvailableField(CustomPluList::AgressoGLCode);
		AddAvailableField(CustomPluList::AgressoCostCentre);
		AddAvailableField(CustomPluList::AgressoProject);
	}

	AddAvailableField(CustomPluList::DeptNo);
	AddAvailableField(CustomPluList::Category);
	AddAvailableField(CustomPluList::PluType);
	AddAvailableField(CustomPluList::ExternalProcess);
	AddAvailableField(CustomPluList::TaxBand, TAGSTATETYPE_3STATEBLUEGREEN);

	//PRICES AND POINTS
	AddAvailableField(CustomPluList::Price1, TAGSTATETYPE_3STATEBLUEGREEN);
	AddAvailableField(CustomPluList::Price2, TAGSTATETYPE_3STATEBLUEGREEN);
	AddAvailableField(CustomPluList::Price3, TAGSTATETYPE_3STATEBLUEGREEN);
	AddAvailableField(CustomPluList::Price4, TAGSTATETYPE_3STATEBLUEGREEN);
	AddAvailableField(CustomPluList::Price5, TAGSTATETYPE_3STATEBLUEGREEN);
	AddAvailableField(CustomPluList::Price6, TAGSTATETYPE_3STATEBLUEGREEN);
	AddAvailableField(CustomPluList::Price7, TAGSTATETYPE_3STATEBLUEGREEN);
	AddAvailableField(CustomPluList::Price8, TAGSTATETYPE_3STATEBLUEGREEN);
	AddAvailableField(CustomPluList::Price9, TAGSTATETYPE_3STATEBLUEGREEN);
	AddAvailableField(CustomPluList::Price10, TAGSTATETYPE_3STATEBLUEGREEN);
	AddAvailableField(CustomPluList::AllowModifier);

	if (SysInfo.GotLoyaltyOrSmartPayLink())
	{
		AddAvailableField(CustomPluList::Points);
		AddAvailableField(CustomPluList::StampWeight);
	}

	//COST
	AddAvailableField(CustomPluList::PurchaseCost);

	if (DealerFlags.GetReportCostPriceFlag() == TRUE)
	{
		AddAvailableField(CustomPluList::ReportCost, TAGSTATETYPE_3STATEBLUEGREEN);
	}

	//ALLERGENS
	if (EcrmanOptions.GetFeaturesAllergenFlag() == TRUE)
	{
		AddAvailableField(CustomPluList::Allergen1);
		AddAvailableField(CustomPluList::Allergen2);
		AddAvailableField(CustomPluList::Allergen3);
		AddAvailableField(CustomPluList::Allergen4);
		AddAvailableField(CustomPluList::Allergen5);
		AddAvailableField(CustomPluList::Allergen6);
	}

	//LISTS
	AddAvailableField(CustomPluList::Menu, TAGSTATETYPE_3STATEBLUEGREEN);
	AddAvailableField(CustomPluList::Qualifier, TAGSTATETYPE_3STATEBLUEGREEN);
	AddAvailableField(CustomPluList::Extra, TAGSTATETYPE_3STATEBLUEGREEN);

	//ORDER PRINTERS
	AddAvailableField(CustomPluList::OrderPrinterGroup, TAGSTATETYPE_3STATEBLUEGREEN);
	AddAvailableField(CustomPluList::Priority, TAGSTATETYPE_3STATEBLUEGREEN);

	//MISC PROGRAMMING
	AddAvailableField(CustomPluList::PriceType);
	AddAvailableField(CustomPluList::AgeVerificationType);
	AddAvailableField(CustomPluList::KPNotAlone);
	AddAvailableField(CustomPluList::KPConsolidateTopItem);
	AddAvailableField(CustomPluList::KPImagePrint);
	AddAvailableField(CustomPluList::AutoCoversQty);
	AddAvailableField(CustomPluList::EANOnReceipt);
	AddAvailableField(CustomPluList::SalesTracking);
	AddAvailableField(CustomPluList::Discountable);
	AddAvailableField(CustomPluList::StaffDiscount);
	AddAvailableField(CustomPluList::FreeText);
	AddAvailableField(CustomPluList::FreeTextDescription);
	AddAvailableField(CustomPluList::AccountRequired);

	//LABELS
	AddAvailableField(CustomPluList::PrintShelf);
	AddAvailableField(CustomPluList::PrintProduct);

	//MISC
	AddAvailableField(CustomPluList::PluInfoModifier);
}

/**********************************************************************/

void CPriceSetCustomListDlg::AddAvailableField( CustomPluField Field, int nTagStateType )
{
	CSortedIntAndStringByInt item;
	
	item.m_nItem = Field.Type;

	if ( Field.Type == CustomPluList::SortCode.Type )
		item.m_strData = DealerFlags.GetSortCodeLabel();
	else
		item.m_strData = Field.ListLabel;

	item.m_nData = nTagStateType;

	m_arrayAvailableFields.Consolidate( item );
	m_arrayDefaultFields.Add( item.m_nItem );
}

/**********************************************************************/

void CPriceSetCustomListDlg::AddFieldToList( const char* szField, int nData, int nTagState, int nTagStateType )
{
	CSSListTaggedSelectItem item( nTagStateType );
	item.m_strText = szField;
	item.m_nData = nData;
	item.SetTagState( nTagState );
	m_listFields.AddItemToList( item );
}

/**********************************************************************/

void CPriceSetCustomListDlg::CreateReportFieldsLine()
{
	CCSV csv;
	AppendFieldTicks ( csv );
	m_strReportFieldsLine = csv.GetLine();
}

/**********************************************************************/

void CPriceSetCustomListDlg::AppendFieldTicks ( CCSV& csv )
{
	for ( int nPos = 0; nPos < m_listFields.GetItemCount(); nPos++ )
	{
		CSSListTaggedSelectItem ListItem;
		m_listFields.GetListItem( nPos, ListItem );
		
		int nTagState = ListItem.GetTagState();
		if ( nTagState != 0 )
		{
			csv.Add( ListItem.m_nData );
			csv.Add( nTagState );
		}
	}
}

/**********************************************************************/

void CPriceSetCustomListDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	switch( nIDCtl )
	{
	case IDC_LIST_FIELD:
		m_listFields.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;

	case IDC_LIST_LOCATION:
		m_listLocations.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;
	}
}

/**********************************************************************/

void CPriceSetCustomListDlg::OnStaticRange() 
{
	SetPluRangeAll();
	GetEditPluFrom() -> SetWindowText ( m_strPluFrom );
	GetEditPluTo() -> SetWindowText ( m_strPluTo );
}

/**********************************************************************/

void CPriceSetCustomListDlg::OnKillFocusPluNo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluFrom = BarcodePluNoTable.ConvertPlu ( m_strPluFrom );
	m_strPluTo = BarcodePluNoTable.ConvertPlu ( m_strPluTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPriceSetCustomListDlg::OnSelectKeyboardLoc()
{
	m_KeyboardFilterSelector.SelectLocation();
}

/**********************************************************************/

void CPriceSetCustomListDlg::OnKillFocusEditDeptNumber() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CPriceSetCustomListDlg::OnSelchangeComboDeptname() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CPriceSetCustomListDlg::OnButtonFieldUp() 
{
	int nSel = m_listFields.GetCurSel();
	if ( nSel > 1 && nSel < m_listFields.GetItemCount() )
		m_listFields.SwapItems( nSel, nSel - 1 );
}

/**********************************************************************/

void CPriceSetCustomListDlg::OnButtonFieldDown() 
{
	int nSel = m_listFields.GetCurSel();
	if ( nSel > 0 && nSel < m_listFields.GetItemCount() - 1 )
		m_listFields.SwapItems( nSel, nSel + 1 );
}

/**********************************************************************/

void CPriceSetCustomListDlg::OnButtonFieldResetOrder() 
{
	int nSelectedField = -1;

	int nSel = m_listFields.GetCurSel();
	if ( nSel >= 0 && nSel < m_listFields.GetItemCount() )
	{
		CSSListTaggedSelectItem ListItem;
		m_listFields.GetListItem( nSel, ListItem );
		nSelectedField = ListItem.m_nData;
	}

	CReportConsolidationArray<CSortedIntByInt> arraySelected;
	for ( int nPos = 0; nPos < m_listFields.GetItemCount(); nPos++ )
	{
		CSSListTaggedSelectItem ListItem;
		m_listFields.GetListItem( nPos, ListItem );

		int nTagState = ListItem.GetTagState(); 
		if ( nTagState != 0 )
		{
			CSortedIntByInt item;
			item.m_nKey = ListItem.m_nData;
			item.m_nVal = nTagState;
			arraySelected.Consolidate( item );
		}
	}

	CreateAvailableFieldList();
	
	m_listFields.ClearList();
	AddFieldToList( CustomPluList::PluNo.ListLabel, CustomPluList::PluNo.Type, 1, TAGSTATETYPE_2STATEBLUE );
	
	int nNewSel = 0;
	for ( int c = 0; c < m_arrayDefaultFields.GetSize(); c++ )
	{
		CSortedIntAndStringByInt itemField;
		itemField.m_nItem = m_arrayDefaultFields.GetAt(c);

		int nFieldFindPos;
		if ( m_arrayAvailableFields.Find( itemField, nFieldFindPos ) == TRUE )
		{
			m_arrayAvailableFields.GetAt( nFieldFindPos, itemField );

			int nTickFindPos;
			int nItemTagState = 0;

			CSortedIntByInt itemTickCheck;
			itemTickCheck.m_nKey = itemField.m_nItem;
			if ( arraySelected.Find( itemTickCheck, nTickFindPos ) == TRUE )
			{
				arraySelected.GetAt( nTickFindPos, itemTickCheck );
				nItemTagState = itemTickCheck.m_nVal;
			}
			
			AddFieldToList( itemField.m_strData, itemField.m_nItem, nItemTagState, itemField.m_nData );
			
			m_arrayAvailableFields.RemoveAt(nFieldFindPos);
	
			if ( itemField.m_nItem == nSelectedField )
			{
				nNewSel = m_listFields.GetItemCount() - 1;
			}
		}
	}
	
	m_listFields.SetCurSel(nNewSel);
}

/**********************************************************************/

void CPriceSetCustomListDlg::OnButtonFieldAll() 
{
	for ( int nIndex = 1; nIndex < m_listFields.GetItemCount(); nIndex++ )
	{
		if ( m_listFields.GetItemTagState( nIndex ) == 0 )
		{
			m_listFields.SetItemTagState( nIndex, 1 );
			m_listFields.Update( nIndex );
		}
	}

	SetPluRangeAll();
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPriceSetCustomListDlg::OnButtonFieldNone() 
{
	m_listFields.SelectNone();
}

/**********************************************************************/

void CPriceSetCustomListDlg::OnButtonPSetUp() 
{
	int nSel = m_listLocations.GetCurSel();
	if ( nSel > 0 && nSel < m_listLocations.GetItemCount() )
		m_listLocations.SwapItems( nSel, nSel - 1 );
}

/**********************************************************************/

void CPriceSetCustomListDlg::OnButtonPSetDown() 
{
	int nSel = m_listLocations.GetCurSel();
	if ( nSel >= 0 && nSel < m_listLocations.GetItemCount() - 1 )
		m_listLocations.SwapItems( nSel, nSel + 1 );
}

/**********************************************************************/

void CPriceSetCustomListDlg::OnButtonPSetResetOrder() 
{
	int nSelectedField = -1;

	int nSel = m_listLocations.GetCurSel();
	if ( nSel >= 0 && nSel < m_listLocations.GetItemCount() )
	{
		CSSListTaggedSelectItem ListItem;
		m_listLocations.GetListItem( nSel, ListItem );
		nSelectedField = ListItem.m_nData;
	}

	CReportConsolidationArray<CSortedIntItem> arraySelected;
	for ( int nPos = 0; nPos < m_listLocations.GetItemCount(); nPos++ )
	{
		CSSListTaggedSelectItem ListItem;
		m_listLocations.GetListItem( nPos, ListItem );

		if ( ListItem.GetTagState() == 1 )
		{
			CSortedIntItem item;
			item.m_nItem = ListItem.m_nData;
			arraySelected.Consolidate( item );
		}
	}

	m_listLocations.ClearList();
	AddLocations();
	
	int nNewSel = 0;
	for ( int c = 0; c < m_listLocations.GetItemCount(); c++ )
	{
		CSSListTaggedSelectItem ListItem;
		m_listLocations.GetListItem( c, ListItem );

		CSortedIntItem item;
		item.m_nItem = ListItem.m_nData;

		int nPos;
		if ( arraySelected.Find( item, nPos ) == TRUE )
		{
			m_listLocations.SetItemTagState( c, 1 );
			m_listLocations.Update(c);
		}

		if ( item.m_nItem == nSelectedField )
			nNewSel = c;
	}
	
	m_listLocations.SetCurSel(nNewSel);
}

/**********************************************************************/

void CPriceSetCustomListDlg::OnButtonPSetAll() 
{
	m_listLocations.SelectAll();
}

/**********************************************************************/

void CPriceSetCustomListDlg::OnButtonPSetNone() 
{
	m_listLocations.SelectNone();
}

/**********************************************************************/

void CPriceSetCustomListDlg::OnButtonSave() 
{
	UpdateData ( TRUE, FALSE );

	CSSFile fileFields;
	if ( fileFields.Open ( m_strFilenameSettings, "wb" ) == TRUE )
	{
		{
			CCSV csvFields;
			AppendFieldTicks ( csvFields );
			fileFields.WriteLine( csvFields.GetLine() );
		}

		{
			CCSV csvLocs;
			for ( int nPos = 0; nPos < m_listLocations.GetItemCount(); nPos++ )
			{
				CSSListTaggedSelectItem ListItem;
				m_listLocations.GetListItem( nPos, ListItem );

				if ( ListItem.GetTagState() == 1 )
					csvLocs.Add( dbLocation.GetNetworkLocNo( ListItem.m_nData ) );
				
			}
			fileFields.WriteLine( csvLocs.GetLine() );
		}

		{
			CCSV csvOptions;
			if ( m_checkZeroPriced.GetCheck() != 0 )	csvOptions.Add ( 1000 );
			if ( m_checkSeparate.GetCheck() != 0 )		csvOptions.Add ( 1001 );
			if ( m_checkModifiers.GetCheck() != 0 )		csvOptions.Add ( 1002 );
			if ( m_checkDeptSort.GetCheck() != 0 )		csvOptions.Add ( 1003 );
			csvOptions.Add( 1004 );
			csvOptions.Add( m_nPriceSet );
			fileFields.WriteLine ( csvOptions.GetLine() );
		}
	}
}

/**********************************************************************/

void CPriceSetCustomListDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		OnKillFocusPluNo();
		OnKillFocusEditDeptNumber();

		{
			CKeyboardFilterSelectorLoc Loc; 
			int nKbNo;
			m_KeyboardFilterSelector.GetSelection( Loc, nKbNo );
			m_KeyboardFilterPluArray.SelectKeyboard( Loc, nKbNo );
		}

		CreateReportFieldsLine();

		CPriceSetCustomListReportHelpers Helpers( m_nPriceSet + 1 );
		Helpers.SetFieldList( m_strReportFieldsLine );

		for ( int nPos = 0; nPos < m_listLocations.GetItemCount(); nPos++ )
		{
			CSSListTaggedSelectItem ListItem;
			m_listLocations.GetListItem( nPos, ListItem );
			
			if ( ListItem.GetTagState() == 1 )
				Helpers.AddLocation( ListItem.m_nData );
		} 

		CPriceSetCustomListReport CustomListReport ( m_KeyboardFilterPluArray, Helpers );
		CustomListReport.SetPluFrom ( m_strPluFrom );
		CustomListReport.SetPluTo ( m_strPluTo );
		CustomListReport.SetDepartmentFilter ( m_DepartmentSelector.GetDepartmentFilter() );
		CustomListReport.SetSortByDeptFlag ( m_checkDeptSort.GetCheck() != 0 );
		CustomListReport.SetModifiersFlag( m_checkModifiers.GetCheck() != 0 );
		CustomListReport.SetZeroPricedFlag( m_checkZeroPriced.GetCheck() != 0 );
		CustomListReport.SetSeparateFlag( m_checkSeparate.GetCheck() != 0 );
		CustomListReport.CreateReport();
	}
}

/**********************************************************************/
