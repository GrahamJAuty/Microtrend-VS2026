/**********************************************************************/
#include "DateRangeHelpers.h"
#include "MaxLengths.h"
#include "BarcodePluNoTable.h"
#include "PriceSetItemManager.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PluCustomListDlg.h"
/**********************************************************************/

CPluCustomListDlg::CPluCustomListDlg( CPluFilterArray& FilterArray, int nMode, int nRepNum, CWnd* pParent /*=NULL*/)
	: CSSDialog(CPluCustomListDlg::IDD, pParent), 
	m_FilterArray ( FilterArray ), 
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName ),
	m_DateSelectorSingle( m_comboDates, m_DatePickerFrom )
{
	//{{AFX_DATA_INIT(CPluCustomListDlg)
	//}}AFX_DATA_INIT
	
	m_nDepartmentFilter = m_FilterArray.GetDepartmentFilter();
	
	m_nMode = nMode;
	m_nRepNum = nRepNum;
}

/**********************************************************************/

void CPluCustomListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluCustomListDlg)
	DDX_Control(pDX, IDC_STATIC_KEYBOARD, m_staticKeyboard);
	DDX_Control(pDX, IDC_COMBO_KEYBOARD_LOC, m_comboKeyboardLoc);
	DDX_Control(pDX, IDC_COMBO_KEYBOARD_NUM, m_comboKeyboardNum);
	DDX_Control(pDX, IDC_STATIC_DEPARTMENT, m_staticDepartment);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_STATIC_DOWNLOAD1, m_staticDownload1);
	DDX_Control(pDX, IDC_STATIC_DOWNLOAD2, m_staticDownload2);
	DDX_Control(pDX, IDC_STATIC_DOWNLOAD3, m_staticDownload3);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerFrom);
	DDX_Control(pDX, IDC_CHECK_MODIFIERS, m_checkModifiers);
	DDX_Control(pDX, IDC_CHECK_ZEROPRICED, m_checkZeroPriced);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_CHECK_SEPARATE, m_checkSeparate);
	DDX_Control(pDX, IDC_CHECK_SORT_BY_DEPT, m_checkDeptSort);
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_START, m_strPluFrom);
	DDV_IntegerString(pDX, m_strPluFrom, "1", Super.PluEnd() );
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_END, m_strPluTo);
	DDV_IntegerString(pDX, m_strPluTo, "1", Super.PluEnd() );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluCustomListDlg, CDialog)
	//{{AFX_MSG_MAP(CPluCustomListDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_STATIC_RANGE, OnStaticRange)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_START, OnKillFocusPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_END, OnKillFocusPluNo)
	ON_CBN_SELCHANGE(IDC_COMBO_KEYBOARD_LOC, OnSelectKeyboardLoc)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusEditDeptNumber)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelchangeComboDeptname)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDate)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_RESETORDER, OnButtonResetOrder)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPluCustomListDlg::GetEditPluFrom() { return GetEdit( IDC_EDIT_PLU_RANGE_START ); }
CEdit* CPluCustomListDlg::GetEditPluTo() { return GetEdit( IDC_EDIT_PLU_RANGE_END ); }
/**********************************************************************/

BOOL CPluCustomListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_editName.LimitText(25);
	m_editName.EnableWindow( m_nMode == PLUCUSTOMLIST_FULL );
	{
		CString strTitle = "";
		CString strRepName = "";

		switch( m_nMode )
		{
		case PLUCUSTOMLIST_PRICE:
			{
				CFilenameUpdater FnUp( SysFiles::PluCustomDef, 0, 1 );
				m_strFilenameSettings = FnUp.GetFilenameToUse();
				strTitle = "Print Plu Price List";
				strRepName = "Plu Price List";
			}
			break;

		case PLUCUSTOMLIST_PROGRAM:
			{
				CFilenameUpdater FnUp( SysFiles::PluCustomDef, 0, 2 );
				m_strFilenameSettings = FnUp.GetFilenameToUse();
				strTitle = "Print Plu Programming";
				strRepName = "Plu Programming";
			}
			break;

		case PLUCUSTOMLIST_FULL:
		default:
			{
				m_nMode = PLUCUSTOMLIST_FULL;
				CFilenameUpdater FnUp( SysFiles::PluCustomDef, 0, m_nRepNum + 2 );
				m_strFilenameSettings = FnUp.GetFilenameToUse();
				
				CReportPluCustomList::GetCSVReportName( m_nRepNum, strRepName );

				strTitle.Format( "Print Plu Custom List %d (%s)",
					m_nRepNum,
					(const char*) strRepName );
			}
			break;
		}

		SetWindowText( strTitle );
		m_editName.SetWindowText( strRepName );
	}

	m_KeyboardFilterSelector.FillCombos( &m_comboKeyboardLoc, &m_comboKeyboardNum );

	if ( EcrmanOptions.GetFeaturesKeyboardFlag() == FALSE )
	{
		m_comboKeyboardLoc.EnableWindow( FALSE );
		m_comboKeyboardNum.EnableWindow( FALSE );
	}
	else
	{
		CKeyboardFilterSelectorLoc Loc;
		int nKbNo;
		m_FilterArray.GetKeyboardFilter( Loc, nKbNo );

		if ( Loc.m_nNodeType != NODE_SYSTEM )
		{
			m_KeyboardFilterSelector.SetSelection( Loc, nKbNo );
			m_comboKeyboardLoc.EnableWindow( FALSE );
			m_comboKeyboardNum.EnableWindow( FALSE );
		}
	}

	m_DateSelectorSingle.InitialiseDateControl();
	m_DateSelectorSingle.AddReportRange( szRANGE_ALL );
	m_DateSelectorSingle.AddStandardReportRanges( FALSE );

	CString strType;
	CDateRangeHelpers DateRangeHelpers;
	DateRangeHelpers.GetEarliestReportDateString ( strType );
	m_DateSelectorSingle.SetComboSelection( 0, strType );
	
	//CANNOT DETECT CHANGES FOR PRICE SETS
	if ( PriceSetItemManager.IsPluMaster() == FALSE )
		m_comboDates.EnableWindow( FALSE );

	SubclassEdit ( IDC_EDIT_PLU_RANGE_START, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_RANGE_END, SS_NUM, Super.MaxPluLen(), "%s" );

	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_nDepartmentFilter, DEPARTMENT_SELECTOR_NORMAL );

	if ( m_nDepartmentFilter != DEPARTMENTSET_ALL )
	{
		m_editDeptNo.EnableWindow ( FALSE );
		m_comboDeptName.EnableWindow ( FALSE );
	}

	m_listFields.SubclassDlgItem ( IDC_LIST, this );
	m_listFields.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listFields.InsertColumn ( 0, "Fields", LVCFMT_LEFT, 200 );
	m_listFields.SetLockItemZeroFlag( TRUE );
	
	switch( m_nMode )
	{
	case PLUCUSTOMLIST_PRICE:	AddPriceFields();	break;
	case PLUCUSTOMLIST_PROGRAM:	AddProgramFields(); break;
	case PLUCUSTOMLIST_FULL:	AddCustomFields();	break;
	}

	//READ LINE OF SELECTED FIELDS IF AVAILABLE
	CString strBuffer;
	CSSFile fileFields;
	if ( fileFields.Open ( m_strFilenameSettings, "rb" ) == TRUE ) 
	{
		fileFields.ReadString ( strBuffer );
		fileFields.Close();
	}
	else
	{
		strBuffer = "";
	}

	//PLU NO IS ALREADY SELECTED
	AddFieldToList( CustomPluList::PluNo.ListLabel, CustomPluList::PluNo.Type, 1 );

	//TICK THE OPTIONS THAT WERE FOUND IN THE FILE
	CCSV csvFields ( strBuffer ); 
	for ( int nPos = 0; nPos < csvFields.GetSize(); nPos++ )
	{
		int nField = csvFields.GetInt(nPos);
		
		switch ( nField )
		{
		case 1000:
			m_checkZeroPriced.SetCheck( TRUE );
			break;

		case 1001:	
			m_checkSeparate.SetCheck( TRUE );
			break;

		case 1002:	
			m_checkModifiers.SetCheck( TRUE );
			break;

		case 1003:	
			m_checkDeptSort.SetCheck( TRUE );	
			break;

		case 9999:	
			nPos++; /*SKIP NAME*/	
			break;

		default:
			{
				CSortedStringByInt item;
				item.m_nItem = nField;

				int nFindPos;
				if ( m_arrayAvailableFields.Find( item, nFindPos ) == TRUE )
				{
					m_arrayAvailableFields.GetAt( nFindPos, item );
					AddFieldToList( item.m_strItem, item.m_nItem, 1 );
					m_arrayAvailableFields.RemoveAt(nFindPos);
				}
			}
			break;
		}
	}

	//ADD THE REMAINING UNTICKED ITEMS
	for ( int n = 0; n < m_arrayDefaultFields.GetSize(); n++ )
	{
		CSortedStringByInt item;
		item.m_nItem = m_arrayDefaultFields.GetAt(n);

		int nFindPos;
		if ( m_arrayAvailableFields.Find( item, nFindPos ) == TRUE )
		{
			m_arrayAvailableFields.GetAt( nFindPos, item );
			AddFieldToList( item.m_strItem, item.m_nItem, 0 );
			m_arrayAvailableFields.RemoveAt(nFindPos);
		}
	}
	
	if ( SysInfo.GetMaxBasePluLen() == 0 )
	{
		m_checkModifiers.SetCheck( FALSE );
		m_checkModifiers.EnableWindow( FALSE );
		m_checkModifiers.ShowWindow( SW_HIDE );
	}

	return TRUE;  
}

/**********************************************************************/

void CPluCustomListDlg::AddPriceFields()
{
	m_arrayAvailableFields.RemoveAll();
	m_arrayDefaultFields.RemoveAll();

	//DESCRIPTION AND TYPE
	AddAvailableField ( CustomPluList::RepText );
	AddAvailableField ( CustomPluList::EposText );
	AddAvailableField ( CustomPluList::KeyText );
	AddAvailableField ( CustomPluList::SortCode );

#ifdef STOCKMAN_SYSTEM
	AddAvailableField ( CustomPluList::StockCode );
#endif

	if ( DealerFlags.Get_LIPA_LJMU_ExportType() != 0 )
	{
		AddAvailableField ( CustomPluList::AgressoGLCode );
		AddAvailableField ( CustomPluList::AgressoCostCentre );
		AddAvailableField ( CustomPluList::AgressoProject );
	}

	AddAvailableField ( CustomPluList::DeptNo );
	AddAvailableField ( CustomPluList::Category );
	AddAvailableField ( CustomPluList::PluType );
	AddAvailableField ( CustomPluList::TaxBand );
	
	//PRICES AND POINTS
	AddAvailableField ( CustomPluList::Price1 );
	AddAvailableField ( CustomPluList::Price2 );
	AddAvailableField ( CustomPluList::Price3 );
	AddAvailableField ( CustomPluList::Price4 );
	AddAvailableField ( CustomPluList::Price5 );
	AddAvailableField ( CustomPluList::Price6 );
	AddAvailableField ( CustomPluList::Price7 );
	AddAvailableField ( CustomPluList::Price8 );
	AddAvailableField ( CustomPluList::Price9 );
	AddAvailableField ( CustomPluList::Price10 );
	AddAvailableField ( CustomPluList::AllowModifier );
	
	if (SysInfo.GotLoyaltyOrSmartPayLink())
	{
		AddAvailableField(CustomPluList::Points);

		if (SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_SMP_V4)
		{
			AddAvailableField(CustomPluList::StampWeight);
		}
	}
	
	//COST AND PROFIT
	AddAvailableField ( CustomPluList::PurchaseCost );

	if ( DealerFlags.GetReportCostPriceFlag() == TRUE )
	{
		AddAvailableField ( CustomPluList::ReportCost );
	}

	AddAvailableField ( CustomPluList::GP );
	AddAvailableField ( CustomPluList::GPPercent );
	
	if ( Sysset.IsBarmanSystem() )
	{
		AddAvailableField ( CustomPluList::UnitPrice );
	}

	//MISC PROGRAMMING
	AddAvailableField ( CustomPluList::SoldOut );
	
	//MISC
	AddAvailableField ( CustomPluList::ActiveItem );

	//MODIFIED DATE TIME
	AddAvailableField ( CustomPluList::ModifiedDatePlu );
	AddAvailableField ( CustomPluList::ModifiedDateInfo );
}

/**********************************************************************/

void CPluCustomListDlg::AddProgramFields()
{
	m_arrayAvailableFields.RemoveAll();
	m_arrayDefaultFields.RemoveAll();

	//DESCRIPTION AND TYPE
	AddAvailableField(CustomPluList::RepText);
	AddAvailableField(CustomPluList::EposText);
	AddAvailableField(CustomPluList::KeyText);
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
	AddAvailableField(CustomPluList::TaxBand);

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

	//KEYBOARD LISTS
	AddAvailableField(CustomPluList::Menu);
	AddAvailableField(CustomPluList::Qualifier);
	AddAvailableField(CustomPluList::Extra);

	//ORDER PRINTERS
	AddAvailableField(CustomPluList::OrderPrinterGroup);
	AddAvailableField(CustomPluList::OrderPrinters);
	AddAvailableField(CustomPluList::Priority);

	//MISC PROGRAMMING
	AddAvailableField(CustomPluList::PriceType);
	AddAvailableField(CustomPluList::AgeVerificationType);
	AddAvailableField(CustomPluList::KPNotAlone);
	AddAvailableField(CustomPluList::KPConsolidateTopItem);
	AddAvailableField(CustomPluList::KPImagePrint);
	AddAvailableField(CustomPluList::AutoCoversQty);
	AddAvailableField(CustomPluList::EANOnReceipt);
	AddAvailableField(CustomPluList::SalesTracking);
	AddAvailableField(CustomPluList::SoldOut);
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
	AddAvailableField(CustomPluList::ImageFilename);
	AddAvailableField(CustomPluList::ActiveItem);

	//RECORD VERSION
	AddAvailableField(CustomPluList::MinimumSPOSVersion);
	AddAvailableField(CustomPluList::MinimumEposReason);

	//MODIFIED DATE TIME
	AddAvailableField(CustomPluList::ModifiedDatePlu);
	AddAvailableField(CustomPluList::ModifiedDateInfo);
}

/**********************************************************************/

void CPluCustomListDlg::AddCustomFields()
{
	m_arrayAvailableFields.RemoveAll();
	m_arrayDefaultFields.RemoveAll();

	//DESCRIPTION AND TYPE
	AddAvailableField(CustomPluList::RepText);
	AddAvailableField(CustomPluList::EposText);
	AddAvailableField(CustomPluList::KeyText);
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
	AddAvailableField(CustomPluList::TaxBand);

	//PRICES AND POINTS
	AddAvailableField(CustomPluList::Price1);
	AddAvailableField(CustomPluList::Price2);
	AddAvailableField(CustomPluList::Price3);
	AddAvailableField(CustomPluList::Price4);
	AddAvailableField(CustomPluList::Price5);
	AddAvailableField(CustomPluList::Price6);
	AddAvailableField(CustomPluList::Price7);
	AddAvailableField(CustomPluList::Price8);
	AddAvailableField(CustomPluList::Price9);
	AddAvailableField(CustomPluList::Price10);
	AddAvailableField(CustomPluList::AllowModifier);

	if (SysInfo.GotLoyaltyOrSmartPayLink())
	{
		AddAvailableField(CustomPluList::Points);

		if (SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_SMP_V4)
		{
			AddAvailableField(CustomPluList::StampWeight);
		}
	}

	//COSTS AND PROFIT
	AddAvailableField(CustomPluList::PurchaseCost);

	if (DealerFlags.GetReportCostPriceFlag() == TRUE)
	{
		AddAvailableField(CustomPluList::ReportCost);
	}

	AddAvailableField(CustomPluList::GP);
	AddAvailableField(CustomPluList::GPPercent);

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

	//KEYBOARD LISTS
	AddAvailableField(CustomPluList::Menu);
	AddAvailableField(CustomPluList::Qualifier);
	AddAvailableField(CustomPluList::Extra);

	//ORDER PRINTERS
	AddAvailableField(CustomPluList::OrderPrinterGroup);
	AddAvailableField(CustomPluList::OrderPrinters);
	AddAvailableField(CustomPluList::Priority);

	//MISC PROGRAMMING
	AddAvailableField(CustomPluList::PriceType);
	AddAvailableField(CustomPluList::AgeVerificationType);
	AddAvailableField(CustomPluList::KPNotAlone);
	AddAvailableField(CustomPluList::KPConsolidateTopItem);
	AddAvailableField(CustomPluList::KPImagePrint);
	AddAvailableField(CustomPluList::AutoCoversQty);
	AddAvailableField(CustomPluList::EANOnReceipt);
	AddAvailableField(CustomPluList::SalesTracking);
	AddAvailableField(CustomPluList::SoldOut);
	AddAvailableField(CustomPluList::Discountable);
	AddAvailableField(CustomPluList::StaffDiscount);
	AddAvailableField(CustomPluList::FreeText);
	AddAvailableField(CustomPluList::FreeTextDescription);
	AddAvailableField(CustomPluList::AccountRequired);

	//LABELS
	AddAvailableField(CustomPluList::PrintShelf);
	AddAvailableField(CustomPluList::PrintProduct);

	if (Sysset.IsBarmanSystem())
	{
		AddAvailableField(CustomPluList::UnitPrice);
	}

#ifndef STOCKMAN_SYSTEM
	if (EcrmanOptions.GetFeaturesPluStockFlag())
	{
		AddAvailableField(CustomPluList::ApparentLevel);
		AddAvailableField(CustomPluList::MinimumLevel);
		AddAvailableField(CustomPluList::SupplierRef);
		AddAvailableField(CustomPluList::SupplierNo);
		AddAvailableField(CustomPluList::NonStockItem);
	}
#endif

	//MISC
	AddAvailableField(CustomPluList::PluInfoModifier);
	AddAvailableField(CustomPluList::ImageFilename);
	AddAvailableField(CustomPluList::ActiveItem);

	//RECORD VERSION
	AddAvailableField(CustomPluList::MinimumSPOSVersion);
	AddAvailableField(CustomPluList::MinimumEposReason);

	//MODIFIED DATE TIME
	AddAvailableField(CustomPluList::ModifiedDatePlu);
	AddAvailableField(CustomPluList::ModifiedDateInfo);

	//EMPTY COLUMN
	AddAvailableField(CustomPluList::EmptyColumn);
}

/**********************************************************************/

void CPluCustomListDlg::AddFieldToList( const char* szField, int nData, int nTagState  )
{
	CSSListTaggedSelectItem item;
	item.m_strText = szField;
	item.m_nData = nData;
	item.SetTagState(nTagState);
	m_listFields.AddItemToList( item );
}

/**********************************************************************/

void CPluCustomListDlg::AddAvailableField( CustomPluField Field )
{
	CSortedStringByInt item;
	
	item.m_nItem = Field.Type;

	if (Field.Type == CustomPluList::SortCode.Type)
	{
		item.m_strItem = DealerFlags.GetSortCodeLabel();
	}
	else if (Field.Type == CustomPluList::TaxBand.Type)
	{
		item.m_strItem = EcrmanOptions.GetTaxBandString();
	}
	else
	{
		item.m_strItem = Field.ListLabel;
	}

	m_arrayDefaultFields.Add( item.m_nItem );
	m_arrayAvailableFields.Consolidate( item );
}

/**********************************************************************/

void CPluCustomListDlg::CreateSavedSettingsLine()
{
	CCSV csv;

	if ( m_nMode == PLUCUSTOMLIST_FULL )
	{
		CString strName;
		m_editName.GetWindowText( strName );

		csv.Add( 9999 );
		csv.Add( strName );
	}

	AppendFieldTicks ( csv );

	if (m_checkZeroPriced.GetCheck() != 0)
	{
		csv.Add(1000);
	}

	if (m_checkSeparate.GetCheck() != 0)
	{
		csv.Add(1001);
	}

	if (m_checkModifiers.GetCheck() != 0)
	{
		csv.Add(1002);
	}

	if (m_checkDeptSort.GetCheck() != 0)
	{
		csv.Add(1003);
	}

	m_strSavedSettingsLine = csv.GetLine();
}

/**********************************************************************/

void CPluCustomListDlg::CreateReportFieldsLine()
{
	CCSV csv;
	AppendFieldTicks ( csv );
	m_strReportFieldsLine = csv.GetLine();
}

/**********************************************************************/

void CPluCustomListDlg::AppendFieldTicks ( CCSV& csv )
{
	for (int nPos = 0; nPos < m_listFields.GetItemCount(); nPos++)
	{
		CSSListTaggedSelectItem ListItem;
		m_listFields.GetListItem(nPos, ListItem);

		if (ListItem.GetTagState() == 1)
		{
			csv.Add(ListItem.m_nData);
		}
	}
}

/**********************************************************************/

void CPluCustomListDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )  
   {
	   m_listFields.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

void CPluCustomListDlg::OnStaticRange() 
{
	CString strStart = "1";
	::AddLeading ( strStart, Super.MaxPluLen(), '0' );
	GetEditPluFrom() -> SetWindowText ( strStart );
	
	CString strEnd = "9";
	::AddLeading ( strEnd, Super.MaxPluLen(), '9' );
	GetEditPluTo() -> SetWindowText( strEnd );
}

/**********************************************************************/

void CPluCustomListDlg::OnKillFocusPluNo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluFrom = BarcodePluNoTable.ConvertPlu ( m_strPluFrom );
	m_strPluTo = BarcodePluNoTable.ConvertPlu ( m_strPluTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPluCustomListDlg::OnSelectKeyboardLoc()
{
	m_KeyboardFilterSelector.SelectLocation();
}

/**********************************************************************/

void CPluCustomListDlg::OnKillFocusEditDeptNumber() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CPluCustomListDlg::OnSelchangeComboDeptname() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CPluCustomListDlg::OnSelectDate() 
{
	m_DateSelectorSingle.UpdateDateControl();	
}

/**********************************************************************/

void CPluCustomListDlg::OnChangeDate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorSingle.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CPluCustomListDlg::OnButtonUp()
{
	int nSel = m_listFields.GetCurSel();
	if (nSel > 1 && nSel < m_listFields.GetItemCount())
	{
		m_listFields.SwapItems(nSel, nSel - 1);
	}
}

/**********************************************************************/

void CPluCustomListDlg::OnButtonDown() 
{
	int nSel = m_listFields.GetCurSel();
	if (nSel > 0 && nSel < m_listFields.GetItemCount() - 1)
	{
		m_listFields.SwapItems(nSel, nSel + 1);
	}
}

/**********************************************************************/

void CPluCustomListDlg::OnButtonResetOrder() 
{
	int nSelectedField = -1;

	int nSel = m_listFields.GetCurSel();
	if ( nSel >= 0 && nSel < m_listFields.GetItemCount() )
	{
		CSSListTaggedSelectItem ListItem;
		m_listFields.GetListItem( nSel, ListItem );
		nSelectedField = ListItem.m_nData;
	}

	CReportConsolidationArray<CSortedIntItem> arraySelected;
	for ( int nPos = 0; nPos < m_listFields.GetItemCount(); nPos++ )
	{
		CSSListTaggedSelectItem ListItem;
		m_listFields.GetListItem( nPos, ListItem );

		if ( ListItem.GetTagState() == 1 )
		{
			CSortedIntItem item;
			item.m_nItem = ListItem.m_nData;
			arraySelected.Consolidate( item );
		}
	}

	switch( m_nMode )
	{
	case PLUCUSTOMLIST_PRICE:		AddPriceFields();	break;
	case PLUCUSTOMLIST_PROGRAM:		AddProgramFields(); break;
	case PLUCUSTOMLIST_FULL:		AddCustomFields();	break;
	}

	m_listFields.ClearList();
	AddFieldToList( CustomPluList::PluNo.ListLabel, CustomPluList::PluNo.Type, 1 );
		
	int nNewSel = 0;
	for ( int c = 0; c < m_arrayDefaultFields.GetSize(); c++ )
	{
		CSortedStringByInt item;
		item.m_nItem = m_arrayDefaultFields.GetAt(c);

		int nFieldFindPos;
		if ( m_arrayAvailableFields.Find( item, nFieldFindPos ) == TRUE )
		{
			m_arrayAvailableFields.GetAt( nFieldFindPos, item );

			int nTickFindPos;
			CSortedIntItem itemTickCheck;
			itemTickCheck.m_nItem = item.m_nItem;
			bool bSelected = arraySelected.Find( itemTickCheck, nTickFindPos );
			
			AddFieldToList( item.m_strItem, item.m_nItem, bSelected ? 1 : 0 );
			m_arrayAvailableFields.RemoveAt(nFieldFindPos);
		
			if ( item.m_nItem == nSelectedField )
			{
				nNewSel = m_listFields.GetItemCount() - 1;
			}
		}
	}
	
	m_listFields.SetCurSel(nNewSel);
}

/**********************************************************************/

void CPluCustomListDlg::OnButtonAll() 
{
	m_listFields.SelectAll();

	__int64 nPluFrom, nPluTo;
	m_FilterArray.GetDefaultRange ( 0, nPluFrom, nPluTo );
	::FormatPluNo( nPluFrom, m_strPluFrom );
	::FormatPluNo( nPluTo, m_strPluTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPluCustomListDlg::OnButtonNone() 
{
	m_listFields.SelectNone();
}

/**********************************************************************/

void CPluCustomListDlg::OnButtonSave() 
{
	UpdateData ( TRUE, FALSE );

	CSSFile fileFields;
	if ( fileFields.Open ( m_strFilenameSettings, "wb" ) == TRUE )
	{
		CreateSavedSettingsLine();
		fileFields.WriteLine ( m_strSavedSettingsLine );
	}
}

/**********************************************************************/

void CPluCustomListDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		OnKillFocusPluNo();
		OnKillFocusEditDeptNumber();

		COleDateTime date;
		m_DatePickerFrom.GetTime ( date );

		{
			CKeyboardFilterSelectorLoc Loc; 
			int nKbNo;
			m_KeyboardFilterSelector.GetSelection( Loc, nKbNo );
			m_KeyboardFilterPluArray.SelectKeyboard( Loc, nKbNo );
		}

		CreateReportFieldsLine();
		CReportPluCustomList CustomListReport ( m_nMode, m_FilterArray, m_KeyboardFilterPluArray, m_strReportFieldsLine );
		CustomListReport.SetChangesDate( date );
		CustomListReport.SetNoDateFilterFlag( m_comboDates.GetCurSel() == 0 );
		CustomListReport.SetPluFrom ( m_strPluFrom );
		CustomListReport.SetPluTo ( m_strPluTo );
		CustomListReport.SetDepartmentFilter ( m_DepartmentSelector.GetDepartmentFilter() );
		CustomListReport.SetSortByDeptFlag ( m_checkDeptSort.GetCheck() != 0 );
		CustomListReport.SetModifiersFlag( m_checkModifiers.GetCheck() != 0 );
		CustomListReport.SetZeroPricedFlag( m_checkZeroPriced.GetCheck() != 0 );
		CustomListReport.SetSeparateFlag( m_checkSeparate.GetCheck() != 0 );
		
		if ( m_nMode == PLUCUSTOMLIST_FULL )
		{
			CString strName;
			m_editName.GetWindowText( strName );
			CustomListReport.SetCustomName( strName );
		}
		
		CustomListReport.CreateReport();
	}
}

/**********************************************************************/