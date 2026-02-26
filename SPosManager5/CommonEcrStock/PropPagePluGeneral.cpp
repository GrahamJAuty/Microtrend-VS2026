/**********************************************************************/
#include "..\CommonEcrStockTray\PluTypeHelpers.h"
/**********************************************************************/
#include "..\SPosStockManager5\PluStockActionDlg.h"
#include "..\SPosStockManager5\PluStockItemDlg.h"
#include "..\SPosStockManager5\PropertySheetPluStock.h"
#include "..\SPosStockManager5\StockOptionsIni.h"
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "MaxLengths.h"
#include "ModifierCSVArray.h"
#include "AnalysisCategoryCSVArray.h"
#include "PriceHelpers.h"
#include "PriceSetItemManager.h"
#include "ReportHelpers.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPagePluGeneral.h"
/**********************************************************************/

CPropPagePluGeneral::CPropPagePluGeneral() : CSSPropertyPage(CPropPagePluGeneral::IDD),
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName ),
	m_AllergenSelector1 ( m_editAllergen[0], m_comboAllergen[0], NODE_ALLERGEN ),
	m_AllergenSelector2 ( m_editAllergen[1], m_comboAllergen[1], NODE_ALLERGEN ),
	m_AllergenSelector3 ( m_editAllergen[2], m_comboAllergen[2], NODE_ALLERGEN ),
	m_AllergenSelector4 ( m_editAllergen[3], m_comboAllergen[3], NODE_ALLERGEN ),
	m_AllergenSelector5 ( m_editAllergen[4], m_comboAllergen[4], NODE_ALLERGEN ),
	m_AllergenSelector6 ( m_editAllergen[5], m_comboAllergen[5], NODE_ALLERGEN ),
	m_KbdListSelectorMenu( m_editFirstMenu, m_comboFirstMenu, FALSE ),
	m_KbdListSelectorExtra( m_editFirstExtra, m_comboFirstExtra, FALSE ),
	m_KbdListSelectorQualifier( m_editFirstQualifier, m_comboFirstQualifier, FALSE )
{
	//{{AFX_DATA_INIT(CPropPagePluGeneral)
	m_nGPType = 0;
	//}}AFX_DATA_INIT
	m_pPluRecord = NULL;

#ifdef STOCKMAN_SYSTEM
	m_pPropertySheetPluStock = NULL;
	m_bNewStock = FALSE;
	m_bSimpleEdit = FALSE;
#endif

	m_staticAllergenID[0] = IDC_STATIC_ALLERGEN1;
	m_staticAllergenID[1] = IDC_STATIC_ALLERGEN2;
	m_staticAllergenID[2] = IDC_STATIC_ALLERGEN3;
	m_staticAllergenID[3] = IDC_STATIC_ALLERGEN4;
	m_staticAllergenID[4] = IDC_STATIC_ALLERGEN5;
	m_staticAllergenID[5] = IDC_STATIC_ALLERGEN6;

	m_editAllergenID[0] = IDC_EDIT_ALLERGEN1;
	m_editAllergenID[1] = IDC_EDIT_ALLERGEN2;
	m_editAllergenID[2] = IDC_EDIT_ALLERGEN3;
	m_editAllergenID[3] = IDC_EDIT_ALLERGEN4;
	m_editAllergenID[4] = IDC_EDIT_ALLERGEN5;
	m_editAllergenID[5] = IDC_EDIT_ALLERGEN6;

	m_comboAllergenID[0] = IDC_COMBO_ALLERGEN1;
	m_comboAllergenID[1] = IDC_COMBO_ALLERGEN2;
	m_comboAllergenID[2] = IDC_COMBO_ALLERGEN3;
	m_comboAllergenID[3] = IDC_COMBO_ALLERGEN4;
	m_comboAllergenID[4] = IDC_COMBO_ALLERGEN5;
	m_comboAllergenID[5] = IDC_COMBO_ALLERGEN6;

	m_editBasePriceID[0] = IDC_EDIT_PRICE1;
	m_editBasePriceID[1] = IDC_EDIT_PRICE2;
	m_editBasePriceID[2] = IDC_EDIT_PRICE3;
	m_editBasePriceID[3] = IDC_EDIT_PRICE4;
	m_editBasePriceID[4] = IDC_EDIT_PRICE5;
	m_editBasePriceID[5] = IDC_EDIT_PRICE6;
	m_editBasePriceID[6] = IDC_EDIT_PRICE7;
	m_editBasePriceID[7] = IDC_EDIT_PRICE8;
	m_editBasePriceID[8] = IDC_EDIT_PRICE9;
	m_editBasePriceID[9] = IDC_EDIT_PRICE10;

	m_staticBasePriceID[0] = IDC_STATIC_PRICE1;
	m_staticBasePriceID[1] = IDC_STATIC_PRICE2;
	m_staticBasePriceID[2] = IDC_STATIC_PRICE3;
	m_staticBasePriceID[3] = IDC_STATIC_PRICE4;
	m_staticBasePriceID[4] = IDC_STATIC_PRICE5;
	m_staticBasePriceID[5] = IDC_STATIC_PRICE6;
	m_staticBasePriceID[6] = IDC_STATIC_PRICE7;
	m_staticBasePriceID[7] = IDC_STATIC_PRICE8;
	m_staticBasePriceID[8] = IDC_STATIC_PRICE9;
	m_staticBasePriceID[9] = IDC_STATIC_PRICE10;

	m_bReadOnly = FALSE;
}

/**********************************************************************/

CPropPagePluGeneral::~CPropPagePluGeneral()
{
}

/**********************************************************************/

void CPropPagePluGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePluGeneral)

	DDX_Control(pDX, IDC_EDIT_EPOSTEXT, m_editEposText);
	DDX_Control(pDX, IDC_EDIT_REPTEXT, m_editRepText);
	DDX_Control(pDX, IDC_EDIT_KEYTEXT, m_editKeyText);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_STATIC_SORTCODE, m_staticSortCode);
	/*****/
	DDX_Control(pDX, IDC_STATIC_PRICE, m_staticPriceGroup);
	/*****/
	DDX_Control(pDX, IDC_STATIC_ALLERGEN, m_staticAllergenGroup);
	/*****/
	DDX_Control(pDX, IDC_EDIT_AGRESSO_GLCODE, m_editAgressoGLCode);
	DDX_Control(pDX, IDC_EDIT_AGRESSO_COSTCENTRE, m_editAgressoCostCentre);
	DDX_Control(pDX, IDC_EDIT_AGRESSO_PROJECT, m_editAgressoProject);
	/*****/
	DDX_Control(pDX, IDC_STATIC_DATE, m_staticDate);
	DDX_Control(pDX, IDC_DATEPICKER, m_DatePicker);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_staticStatus);
	DDX_Control(pDX, IDC_COMBO_STATUS, m_comboStatus);
	/*****/
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Control(pDX, IDC_COMBO_PLUTYPE, m_comboPluType);
	DDX_Control(pDX, IDC_STATIC_PROCESS, m_staticProcess);
	DDX_Control(pDX, IDC_COMBO_PROCESS, m_comboProcess);
	DDX_Control(pDX, IDC_STATIC_ACTIVE, m_staticActive);
	DDX_Control(pDX, IDC_COMBO_ACTIVE, m_comboActive);
	DDX_Control(pDX, IDC_CHECK_ALLOWMOD, m_checkAllowMod);
	/*****/
	DDX_Control(pDX, IDC_STATIC_STOCK, m_staticStock);
	DDX_Control(pDX, IDC_BUTTON_STOCK, m_buttonStock);
	DDX_Control(pDX, IDC_EDIT_STOCK, m_editStock);
	/*****/
	DDX_Control(pDX, IDC_STATIC_TAXBAND, m_staticTaxBand);
	DDX_Control(pDX, IDC_COMBO_TAXBAND, m_comboTaxBand);
	DDX_Control(pDX, IDC_STATIC_COST, m_staticPurchaseCost);
	DDX_Control(pDX, IDC_CHECK_REPORT_COST, m_checkReportCost);
	DDX_Control(pDX, IDC_STATIC_POINTS, m_staticPoints);
	DDX_Control(pDX, IDC_STATIC_STAMPWEIGHT, m_staticStampWeight);
	/*****/
	DDX_Control(pDX, IDC_RADIO_GP_PERCENT, m_radioGPPercent);
	DDX_Control(pDX, IDC_RADIO_GP_FIXED, m_radioGPFixed);
	DDX_Control(pDX, IDC_RADIO_GP_TYPE, m_radioGPType);
	/*****/
	DDX_Text(pDX, IDC_EDIT_EPOSTEXT, m_strEposText);
	DDX_Text(pDX, IDC_EDIT_REPTEXT, m_strRepText);
	DDX_Text(pDX, IDC_EDIT_KEYTEXT, m_strKeyText);
	/*****/
	DDX_Text(pDX, IDC_EDIT_AGRESSO_GLCODE, m_strAgressoGLCode);
	DDX_Text(pDX, IDC_EDIT_AGRESSO_COSTCENTRE, m_strAgressoCostCentre);
	DDX_Text(pDX, IDC_EDIT_AGRESSO_PROJECT, m_strAgressoProject);
	/*****/
	DDX_Text(pDX, IDC_EDIT_SORTCODE, m_strSortCode);
	/*****/
	DDX_Text(pDX, IDC_EDIT_PURCHASE_COST, m_dPurchaseCost);
	DDX_Text(pDX, IDC_EDIT_REPORT_COST, m_dReportCost);
	/*****/
	DDX_Radio(pDX, IDC_RADIO_GP_TYPE, m_nGPType);
	DDX_Text(pDX, IDC_EDIT_GP_FIXED, m_dGPFixed);
	DDX_Text(pDX, IDC_EDIT_GP_PERCENT, m_dGPPercent);
	//}}AFX_DATA_MAP

	if (SysInfo.GotLoyaltyOrSmartPayLink())
	{
		DDX_Text(pDX, IDC_EDIT_POINTS, m_nPoints);
		DDV_MinMaxInt(pDX, m_nPoints, Plu::Points.Min, Plu::Points.Max);
		DDX_Text(pDX, IDC_EDIT_STAMPWEIGHT, m_nStampWeight);
		DDV_MinMaxInt(pDX, m_nStampWeight, Plu::StampWeight.Min, Plu::StampWeight.Max);
	}

	for (int i = 0; i < 10; i++)
	{
		DDX_Control(pDX, m_staticBasePriceID[i], m_staticBasePrice[i]);
		DDX_Text(pDX, m_editBasePriceID[i], m_dBasePrice[i]);
	}

	for (int i = 0; i < 6; i++)
	{
		DDX_Control(pDX, m_staticAllergenID[i], m_staticAllergen[i]);
		DDX_Control(pDX, m_editAllergenID[i], m_editAllergen[i]);
		DDX_Control(pDX, m_comboAllergenID[i], m_comboAllergen[i]);
	}

	DDX_Control(pDX, IDC_EDIT_FIRSTQUALIFIER, m_editFirstQualifier);
	DDX_Control(pDX, IDC_EDIT_FIRSTMENU, m_editFirstMenu);
	DDX_Control(pDX, IDC_EDIT_FIRSTEXTRA, m_editFirstExtra);
	DDX_Control(pDX, IDC_COMBO_FIRSTQUALIFIER, m_comboFirstQualifier);
	DDX_Control(pDX, IDC_COMBO_FIRSTMENU, m_comboFirstMenu);
	DDX_Control(pDX, IDC_COMBO_FIRSTEXTRA, m_comboFirstExtra);
	DDX_Control(pDX, IDC_EDIT_SORTCODE, m_editSortCode);

	if (DealerFlags.Get_LIPA_LJMU_ExportType() != 0)
	{
		if (pDX->m_bSaveAndValidate == TRUE)
		{
			pDX->PrepareEditCtrl(IDC_EDIT_AGRESSO_GLCODE);

			CString str = m_strAgressoGLCode;
			::TrimSpaces(str, FALSE);

			bool bAccept = FALSE;

			if (str == "")
			{
				bAccept = TRUE;
			}
			else
			{
				bAccept = ((str.GetLength() == 4) && (str.Left(1) != "0"));
			}

			if (FALSE == bAccept)
			{
				Prompter.Error("The GL Code must be in the range 1000 - 9999, or left blank.");
				m_editAgressoGLCode.SetFocus();
				m_editAgressoGLCode.SetSel(0, -1);
				pDX->Fail();
			}
		}
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePluGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePluGeneral)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelectDept)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_CBN_SELCHANGE(IDC_COMBO_ALLERGEN1, OnSelectAllergen1)
	ON_EN_KILLFOCUS(IDC_EDIT_ALLERGEN1, OnKillFocusAllergen1)
	ON_CBN_SELCHANGE(IDC_COMBO_ALLERGEN2, OnSelectAllergen2)
	ON_EN_KILLFOCUS(IDC_EDIT_ALLERGEN2, OnKillFocusAllergen2)
	ON_CBN_SELCHANGE(IDC_COMBO_ALLERGEN3, OnSelectAllergen3)
	ON_EN_KILLFOCUS(IDC_EDIT_ALLERGEN3, OnKillFocusAllergen3)
	ON_CBN_SELCHANGE(IDC_COMBO_ALLERGEN4, OnSelectAllergen4)
	ON_EN_KILLFOCUS(IDC_EDIT_ALLERGEN4, OnKillFocusAllergen4)
	ON_CBN_SELCHANGE(IDC_COMBO_ALLERGEN5, OnSelectAllergen5)
	ON_EN_KILLFOCUS(IDC_EDIT_ALLERGEN5, OnKillFocusAllergen5)
	ON_CBN_SELCHANGE(IDC_COMBO_ALLERGEN6, OnSelectAllergen6)
	ON_EN_KILLFOCUS(IDC_EDIT_ALLERGEN6, OnKillFocusAllergen6)
	ON_CBN_SELCHANGE(IDC_COMBO_TAXBAND, OnCalculateProfit)
	ON_BN_CLICKED(IDC_RADIO_GP_FIXED, OnRadioGpFixed)
	ON_BN_CLICKED(IDC_RADIO_GP_PERCENT, OnRadioGpPercent)
	ON_BN_CLICKED(IDC_RADIO_GP_TYPE, OnRadioGpType)
	ON_BN_CLICKED(IDC_BUTTON_STOCK, OnButtonStock)
	ON_CBN_SELCHANGE(IDC_COMBO_STATUS, OnChangePluStatus)
	ON_EN_KILLFOCUS(IDC_EDIT_GP_FIXED, OnCalculateProfit)
	ON_EN_KILLFOCUS(IDC_EDIT_GP_PERCENT, OnCalculateProfit)
	ON_EN_KILLFOCUS(IDC_EDIT_PRICE1, OnCalculateProfit)
	ON_EN_KILLFOCUS(IDC_EDIT_PURCHASE_COST, OnKillFocusPurchaseCost)
	ON_CBN_SELCHANGE(IDC_COMBO_PLUTYPE, OnSelectType)
	ON_BN_CLICKED(IDC_STATIC_PRICE2,OnCopyPrice2)
	ON_BN_CLICKED(IDC_STATIC_PRICE3,OnCopyPrice3)
	ON_BN_CLICKED(IDC_STATIC_PRICE4,OnCopyPrice4)
	ON_BN_CLICKED(IDC_STATIC_PRICE5,OnCopyPrice5)
	ON_BN_CLICKED(IDC_STATIC_PRICE6,OnCopyPrice6)
	ON_BN_CLICKED(IDC_STATIC_PRICE7,OnCopyPrice7)
	ON_BN_CLICKED(IDC_STATIC_PRICE8,OnCopyPrice8)
	ON_BN_CLICKED(IDC_STATIC_PRICE9,OnCopyPrice9)
	ON_BN_CLICKED(IDC_STATIC_PRICE10,OnCopyPrice10)
	ON_EN_KILLFOCUS(IDC_EDIT_FIRSTMENU, OnKillFocusFirstMenu)
	ON_CBN_SELCHANGE(IDC_COMBO_FIRSTMENU, OnSelectFirstMenu)
	ON_EN_KILLFOCUS(IDC_EDIT_FIRSTEXTRA, OnKillFocusFirstExtra)
	ON_EN_KILLFOCUS(IDC_EDIT_FIRSTQUALIFIER, OnKillFocusFirstQualifier)
	ON_CBN_SELCHANGE(IDC_COMBO_FIRSTEXTRA, OnSelectFirstExtra)
	ON_CBN_SELCHANGE(IDC_COMBO_FIRSTQUALIFIER, OnSelectFirstQualifier)
	ON_BN_CLICKED(IDC_CHECK_REPORT_COST,OnToggleReportCost)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePluGeneral::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	if ( DealerFlags.GetReportCostPriceFlag() == FALSE )
	{
		ShowAndEnableWindow( &m_checkReportCost, FALSE );
		ShowAndEnableWindow( GetEditReportCost(), FALSE );
	}

	m_editEposText.LimitText ( Plu::EposText.Max );
	m_editRepText.LimitText ( Plu::RepText.Max );
	m_editKeyText.LimitText ( Plu::KeyText.Max );
	m_editSortCode.LimitText ( Plu::SortCode.Max );
	m_editAgressoGLCode.LimitText( 4 );
	m_editAgressoCostCentre.LimitText( Plu::AgressoCostCentre.Max );
	m_editAgressoProject.LimitText( Plu::AgressoProject.Max );
	
	SubclassEdit ( IDC_EDIT_PURCHASE_COST, SS_NUM_DP, Super.MaxCostLen(), "%.*f", 5 );
	SubclassEdit ( IDC_EDIT_REPORT_COST, SS_NUM_DP, Super.MaxCostLen(), "%.*f", 5 );
	SubclassEdit ( IDC_EDIT_GP_FIXED, SS_NUM_DP, Super.MaxPriceLen(), "%.*f", SysInfo.GetDPValue() );
	SubclassEdit ( IDC_EDIT_GP_PERCENT, SS_NUM_DP, 6, "%.2f" );
	SubclassEdit ( IDC_EDIT_POINTS, SS_NUM, 4, "%d" );
	SubclassEdit ( IDC_EDIT_STAMPWEIGHT, SS_NUM, 6, "%d");

	switch( DealerFlags.GetSortCodeRenameType() )
	{
	case 1:	
		m_staticSortCode.SetWindowText( "SKU" );
		break;

	case 2:
		m_staticSortCode.SetWindowText( "Stock code" );	
		break;
	}

	for ( int i = 0; i < 10; i++ )
	{
		m_staticBasePrice[i].SetWindowText( DataManager.PriceText.GetReportText(i) );
		SubclassEdit ( m_editBasePriceID[i], SS_NUM_DP, Super.MaxPriceLen(), "%.*f", SysInfo.GetDPValue() );
	}
	
	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_nDeptNo, DEPARTMENT_SELECTOR_SINGLE );

	if ( PriceSetItemManager.IsPluMaster() == TRUE )
	{	
		ShowAndEnableWindow( &m_staticStatus, FALSE );
		ShowAndEnableWindow( &m_comboStatus, FALSE );
	}
	else
	{
		m_comboStatus.AddString ( "Location" );
		m_comboStatus.AddString ( "Master" );
		m_comboStatus.AddString ( "Excluded" );
	}

	int nAllergenMode = 0;
	if (DealerFlags.Get_LIPA_LJMU_ExportType() != 0)
	{
		nAllergenMode = 2;
	}
	else if (EcrmanOptions.GetFeaturesAllergenFlag())
	{
		nAllergenMode = 1;
	}

	//NO ALLERGENS, SO REMOVE ALLERGEN FIELDS, EXPECT FOR STATICS IF USED BY LIPA
	if ( nAllergenMode != 1 ) 
	{
		for ( int i = 0; i < 6; i++ )
		{
			ShowAndEnableWindow( &m_staticAllergen[i], ( i < 3 ) && ( 2 == nAllergenMode ) );
			ShowAndEnableWindow( &m_editAllergen[i], FALSE );
			ShowAndEnableWindow( &m_comboAllergen[i], FALSE );
		}
	}

	//NO ALLERGENS OR LIPA EXPORT, SO PRICE FIELDS TAKE OVER ALLERGEN SPACE
	if ( 0 == nAllergenMode )
	{
		ResizeControl( &m_staticPriceGroup, 313, 255 );
		ShowAndEnableWindow( &m_staticAllergenGroup, FALSE );

		int nYPos = 240;
		for ( int i = 5; i < 10; i++ )
		{
			MoveControl( &m_staticBasePrice[i], 21, nYPos + 1 );
			MoveControl( GetEditBasePrice(i), 92, nYPos );
			nYPos += 20;
		}
	}

	//LIPA EXPORT MODE
	if ( 2 == nAllergenMode )
	{
		m_staticAllergenGroup.SetWindowText( "Agresso Export" );
		ResizeControl( &m_staticAllergen[0], 60, 14 );
		ResizeControl( &m_staticAllergen[1], 60, 14 );
		ResizeControl( &m_staticAllergen[2], 60, 14 );
		m_staticAllergen[0].SetWindowText( "GL Code" );
		m_staticAllergen[1].SetWindowText( "Cost Centre" );
		m_staticAllergen[2].SetWindowText( "Project" );
		MoveControl( &m_editAgressoGLCode, 92, 303 );
		MoveControl( &m_editAgressoCostCentre, 92, 323 );
		MoveControl( &m_editAgressoProject, 92, 343 );
	}
	else
	{
		ShowAndEnableWindow( &m_editAgressoGLCode, FALSE );
		ShowAndEnableWindow( &m_editAgressoCostCentre, FALSE );
		ShowAndEnableWindow( &m_editAgressoProject, FALSE );
	}

	bool bPoints = ( SysInfo.GotLoyaltyOrSmartPayLink() );
	
#ifndef STOCKMAN_SYSTEM

	ShowAndEnableWindow( &m_buttonStock, FALSE );
	ShowAndEnableWindow( &m_staticStock, FALSE );
	ShowAndEnableWindow( &m_editStock, FALSE );

	MoveControl( &m_staticTaxBand, 340, 141 );
	MoveControl( &m_comboTaxBand, 388, 140 );
	MoveControl( &m_staticPurchaseCost, 340, 161 );
	MoveControl( GetEditPurchaseCost(), 388, 160 );
	MoveControl( &m_checkReportCost, 449, 161 );
	MoveControl( GetEditReportCost(), 494, 160 );

	int nYPos = 0;
	
	{
		bool bHideStampWeight = TRUE;
		if (TRUE == bPoints)
		{
			MoveControl(&m_staticPoints, 340, 181);
			MoveControl(GetEditPoints(), 388, 180);
			MoveControl(&m_staticStampWeight, 445, 181);
			MoveControl(GetEditStampWeight(), 494, 180);

			if (SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_SMP_V4)
			{
				bHideStampWeight = FALSE;
			}

			nYPos = 210;
		}
		else
		{
			ShowAndEnableWindow(&m_staticPoints, FALSE);
			ShowAndEnableWindow(GetEditPoints(), FALSE);
			nYPos = 190;
		}

		if (TRUE == bHideStampWeight)
		{
			ShowAndEnableWindow(&m_staticStampWeight, FALSE);
			ShowAndEnableWindow(GetEditStampWeight(), FALSE);
		}
	}

	MoveControl( &m_radioGPType, 340, nYPos );
	MoveControl( &m_radioGPFixed, 340, nYPos + 21 );
	MoveControl( GetEditGPFixed(), 388, nYPos + 19 );
	MoveControl( &m_radioGPPercent, 340, nYPos + 42 );
	MoveControl( GetEditGPPercent(), 388, nYPos + 40 );

#endif
	
	{
		CArray<int, int> arrayStops;
		arrayStops.Add(5);
		arrayStops.Add(30);
		arrayStops.Add(130);
		m_comboTaxBand.SetTabStops( arrayStops );
	}

	{
		CArray<int, int> arrayStops;
		arrayStops.Add(5);
		arrayStops.Add(40);
		m_comboCategory.SetTabStops( arrayStops );
	}

	m_editFirstMenu.LimitText(5);
	m_editFirstExtra.LimitText(5);
	m_editFirstQualifier.LimitText(5);

	m_KbdListSelectorMenu.FillKbdListCombo( m_nFirstMenu );
	m_KbdListSelectorExtra.FillKbdListCombo( m_nFirstExtra );
	m_KbdListSelectorQualifier.FillKbdListCombo( m_nFirstQualifier );

	m_staticTaxBand.SetWindowText( EcrmanOptions.GetTaxBandString() );

	SetRecordControls();
	FillTypeCombo();
	FillActiveCombo();
	FillProcessCombo();
	FillCategoryCombo();
	FillTaxCombo();
	OnSelectType();
	return TRUE;  
}

/**********************************************************************/

void CPropPagePluGeneral::FillTypeCombo()
{
	m_comboPluType.ResetContent();
	CPluTypeHelpers::AddComboStrings(m_comboPluType);

	int nType = m_pPluRecord -> GetMicrotrendPluType();

	for ( int nIndex = 0; nIndex < m_comboPluType.GetCount(); nIndex++ )
	{
		if ( m_comboPluType.GetItemData ( nIndex ) == (WORD) nType )
		{
			m_comboPluType.SetCurSel( nIndex );
			return;
		}
	}

	CString strType = "";
	strType.Format ( "%d", nType );

	m_comboPluType.InsertString ( 0, strType );
	m_comboPluType.SetItemData ( 0, nType );
	m_comboPluType.SetCurSel(0);
}

/**********************************************************************/

void CPropPagePluGeneral::FillActiveCombo()
{
	m_comboActive.ResetContent();
	m_comboActive.AddString( "Yes" );
	m_comboActive.AddString( "No" );
	m_comboActive.SetCurSel( m_pPluRecord -> GetActiveItemFlag() ? 0 : 1 );
}

/**********************************************************************/

void CPropPagePluGeneral::FillProcessCombo()
{
	m_comboProcess.ResetContent();
	m_comboProcess.AddString( "None" );
	m_comboProcess.AddString( "1" );
	m_comboProcess.AddString( "2" );
	m_comboProcess.AddString( "3" );
	m_comboProcess.AddString( "4" );

	m_comboProcess.SetItemData( 0, 0 );
	m_comboProcess.SetItemData( 1, 1 );
	m_comboProcess.SetItemData( 2, 2 );
	m_comboProcess.SetItemData( 3, 3 );
	m_comboProcess.SetItemData( 4, 4 );

	int nSel = m_pPluRecord -> GetExternalProcess();

	if ((nSel >= 0) && (nSel <= 4))
	{
		m_comboProcess.SetCurSel(nSel);
	}
	else
	{
		CString strType = "";
		strType.Format ( "%d", nSel );

		m_comboProcess.InsertString( 0, strType );
		m_comboProcess.SetItemData( 0, nSel );
		m_comboProcess.SetCurSel(0);
	}
}

/**********************************************************************/

void CPropPagePluGeneral::FillCategoryCombo()
{
	m_comboCategory.ResetContent();
	m_comboCategory.AddItem( ",Follow On" );
	m_comboCategory.SetItemData( 0, 0 );

	DataManager.AnalysisCategory.CreateInUseArray();

	for ( int i = 0; i < DataManager.AnalysisCategory.GetInUseArraySize(); i++ )
	{
		int nCatNo = DataManager.AnalysisCategory.GetInUseCatNo(i);
		m_comboCategory.AddItem( DataManager.AnalysisCategory.GetTabbedComboText( nCatNo ) );
		m_comboCategory.SetItemData ( i + 1, nCatNo );
	}
	
	int nCategory = m_pPluRecord -> GetBaseAnalysisCategory();

	if ( m_comboCategory.SelectUsingItemData( nCategory ) == FALSE )
	{
		CString strCategory;
		strCategory.Format( "C%2.2d, Category %d", nCategory, nCategory );
		m_comboCategory.InsertItem ( 1, strCategory );
		m_comboCategory.SetItemData ( 1, nCategory );
		m_comboCategory.SetCurSel(1);
	}
}

/**********************************************************************/

void CPropPagePluGeneral::FillTaxCombo()
{
	m_comboTaxBand.ResetContent();

	int nCurrentTaxBand = m_pPluRecord->GetBaseTaxBandInt();
	if ((nCurrentTaxBand < 1) || (nCurrentTaxBand > MAX_TAX_BANDS))
	{
		m_comboTaxBand.AddItem(",Unknown,???");
	}

	CString strLetter = "A";
	for (int nTaxBand = 1; nTaxBand <= MAX_TAX_BANDS; nTaxBand++)
	{
		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo(nTaxBand, TaxRateInfo);

		strLetter.SetAt(0, 'A' + nTaxBand - 1);

		CString strRate;
		strRate.Format("%.4lf", TaxRateInfo.m_dTaxRate);

		CCSV csv;
		csv.Add(strLetter);
		csv.Add(TaxRateInfo.m_strReportText);
		csv.Add(strRate);
		m_comboTaxBand.AddItem(csv.GetLine());
	}

	int nItemData;
	if ((nCurrentTaxBand < 1) || (nCurrentTaxBand > MAX_TAX_BANDS))
	{
		m_comboTaxBand.SetCurSel(0);
		nItemData = 0;
	}
	else
	{
		m_comboTaxBand.SetCurSel(nCurrentTaxBand - 1);
		nItemData = 1;
	}

	for (int n = 0; n < m_comboTaxBand.GetCount(); n++)
	{
		m_comboTaxBand.SetItemData(n, nItemData++);
	}
}

/**********************************************************************/

int CPropPagePluGeneral::GetPluType()
{
	int nSelect = m_comboPluType.GetCurSel();
	if (nSelect >= 0 && nSelect < m_comboPluType.GetCount())
	{
		return m_comboPluType.GetItemData(nSelect);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/

int CPropPagePluGeneral::GetExternalProcess()
{
	int nSelect = m_comboProcess.GetCurSel();
	if (nSelect >= 0 && nSelect < m_comboProcess.GetCount())
	{
		return m_comboProcess.GetItemData(nSelect);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/

int CPropPagePluGeneral::GetAnalysisCategory()
{
	int nCategory = 1;

	int nSelect = m_comboCategory.GetCurSel();
	if ((nSelect >= 0) && (nSelect < m_comboCategory.GetCount()))
	{
		nCategory = m_comboCategory.GetItemData(nSelect);
	}

	return nCategory;
}

/**********************************************************************/

int CPropPagePluGeneral::GetTaxBand()
{
	int nSelect = m_comboTaxBand.GetCurSel();
	if (nSelect >= 0 && nSelect < m_comboTaxBand.GetCount())
	{
		return m_comboTaxBand.GetItemData(nSelect);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
CEdit* CPropPagePluGeneral::GetEditGPFixed() { return GetEdit( IDC_EDIT_GP_FIXED ); }
CEdit* CPropPagePluGeneral::GetEditGPPercent() { return GetEdit( IDC_EDIT_GP_PERCENT ); }
CEdit* CPropPagePluGeneral::GetEditPurchaseCost() { return GetEdit( IDC_EDIT_PURCHASE_COST ); }
CEdit* CPropPagePluGeneral::GetEditReportCost() { return GetEdit( IDC_EDIT_REPORT_COST ); }
CEdit* CPropPagePluGeneral::GetEditBasePrice( int nLevel ) { return GetEdit( m_editBasePriceID[nLevel] ); }
CEdit* CPropPagePluGeneral::GetEditPoints() { return GetEdit( IDC_EDIT_POINTS ); }
CEdit* CPropPagePluGeneral::GetEditStampWeight() { return GetEdit(IDC_EDIT_STAMPWEIGHT); }
/**********************************************************************/

void CPropPagePluGeneral::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
	FillTypeCombo();
	FillActiveCombo();
	FillProcessCombo();
	FillCategoryCombo();
	FillTaxCombo();
	OnSelectType();
}

/**********************************************************************/

BOOL CPropPagePluGeneral::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	FillTypeCombo();
	FillActiveCombo();
	FillProcessCombo();
	FillCategoryCombo();
	FillTaxCombo();
	OnSelectType();
	return bResult;
}

/**********************************************************************/

bool CPropPagePluGeneral::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPagePluGeneral::SaveRecord()
{
	if (m_bReadOnly == TRUE)
	{
		return;
	}

	if ( EcrmanOptions.GetPluDownloadByDateFlag() == TRUE )
	{
		COleDateTime date;
		m_DatePicker.GetTime ( date );
	
		CString strPluDate;
		int nYear = date.GetYear();
		int nMonth = date.GetMonth();
		int nDay = date.GetDay();
	
		strPluDate.Format ( "%4.4d%2.2d%2.2d", nYear, nMonth, nDay );
		m_pPluRecord -> SetDownloadDatePlu ( strPluDate );
	}

	OnKillFocusDeptNo();
	OnKillFocusFirstMenu();
	OnKillFocusFirstQualifier();
	OnKillFocusFirstExtra();

	UpdatePrice1( GetTaxBand() );

	int nGLCode = atoi( m_strAgressoGLCode );
	if (nGLCode < 1000)
	{
		nGLCode = 0;
	}

	m_pPluRecord -> SetEposText ( m_strEposText );
	m_pPluRecord -> SetRepText ( m_strRepText );
	m_pPluRecord -> SetSortCode ( m_strSortCode );
	m_pPluRecord -> SetKeyText ( m_strKeyText );

	if ( DealerFlags.Get_LIPA_LJMU_ExportType() != 0 )
	{
		m_pPluRecord -> SetAgressoGLCode ( nGLCode );
		m_pPluRecord -> SetAgressoCostCentre ( m_strAgressoCostCentre );
		m_pPluRecord -> SetAgressoProject ( m_strAgressoProject );
	}

	m_pPluRecord -> SetBaseDeptNo ( m_nDeptNo );
	m_pPluRecord -> SetBaseTaxBandInt ( GetTaxBand() );
	m_pPluRecord -> SetMicrotrendPluType ( GetPluType() );
	m_pPluRecord -> SetActiveItemFlag( m_comboActive.GetCurSel() == 0 );
	m_pPluRecord -> SetAllowModifierFlag(IsTicked(m_checkAllowMod));
	m_pPluRecord -> SetExternalProcess ( GetExternalProcess() );
	m_pPluRecord -> SetBaseAnalysisCategory ( GetAnalysisCategory() );
	m_pPluRecord -> SetGPType ( m_nGPType );

	if ( SysInfo.GotLoyaltyOrSmartPayLink() )
	{
		m_pPluRecord -> SetPoints( m_nPoints );
		m_pPluRecord-> SetStampWeight(m_nStampWeight);
	}

	for (int i = 0; i < 10; i++)
	{
		m_pPluRecord->SetPrice(i, m_dBasePrice[i]);
	}

	if ( TRUE == EcrmanOptions.GetFeaturesAllergenFlag() )
	{
		m_AllergenSelector1.SelectGroupFromEditBox();
		m_AllergenSelector2.SelectGroupFromEditBox();
		m_AllergenSelector3.SelectGroupFromEditBox();
		m_AllergenSelector4.SelectGroupFromEditBox();
		m_AllergenSelector5.SelectGroupFromEditBox();
		m_AllergenSelector6.SelectGroupFromEditBox();
		m_pPluRecord -> SetAllergenNo( 0, m_AllergenSelector1.GetGroupFilter() );
		m_pPluRecord -> SetAllergenNo( 1, m_AllergenSelector2.GetGroupFilter() );
		m_pPluRecord -> SetAllergenNo( 2, m_AllergenSelector3.GetGroupFilter() );
		m_pPluRecord -> SetAllergenNo( 3, m_AllergenSelector4.GetGroupFilter() );
		m_pPluRecord -> SetAllergenNo( 4, m_AllergenSelector5.GetGroupFilter() );
		m_pPluRecord -> SetAllergenNo( 5, m_AllergenSelector6.GetGroupFilter() );
	}
		
	switch ( m_nGPType )
	{
	case PLU_GPTYPE_VARIABLE:
		m_pPluRecord -> SetProfitMargin ( 0.0 );
		break;

	case PLU_GPTYPE_FIXED:
		m_pPluRecord -> SetProfitMargin ( m_dGPFixed );
		break;

	case PLU_GPTYPE_PERCENT:
		m_pPluRecord -> SetProfitMargin ( m_dGPPercent );
		break;
	}

	m_pPluRecord -> SetFirstExtra ( m_nFirstExtra );
	m_pPluRecord -> SetFirstMenu( m_nFirstMenu );
	m_pPluRecord -> SetFirstQualifier( m_nFirstQualifier );
	m_pPluRecord -> SetReportCostEnable( m_checkReportCost.GetCheck() != 0 );
	m_pPluRecord -> SetReportCost( m_dReportCost );

#ifdef STOCKMAN_SYSTEM

	if ( m_pRecipeRecord -> GetPluStockLinkFlag() == FALSE )
		m_pPluRecord -> SetPurchaseCost( m_dPurchaseCost );

	if ( ( m_bNewStock || DealerFlags.GetCopyPluToStockFlag() ) && ( m_pRecipeRecord -> GetSize() == 1 ) )
	{
		CString strStockCode = m_pRecipeRecord -> GetStockCode(0);

		int nStockIdx;
		if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == TRUE )
		{
			m_pPluChangesBuffer -> LogStockChange ( strStockCode );
			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt ( nStockIdx, StockRecord );
				
			if ( StockRecord.GetDisablePluRangeFlag() == FALSE )
			{
				CString strRepText = m_pPluRecord -> GetRepText();

				if ( strRepText != "" )
					StockRecord.SetDescription ( strRepText );
				else
					StockRecord.SetDescription ( m_pPluRecord -> GetEposText() );
				
				StockRecord.SetCategory ( m_pPluRecord -> GetBaseDeptNo() );
				DataManager.Stock.SetAt ( nStockIdx, StockRecord );
			}
		}
	}
	
	UpdateStockCost();

#else

	m_pPluRecord -> SetPurchaseCost( m_dPurchaseCost );

#endif
}

/**********************************************************************/

BOOL CPropPagePluGeneral::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

void CPropPagePluGeneral::GetRecordData()
{
	m_strRepText = m_pPluRecord->GetRepText();
	m_strSortCode = m_pPluRecord->GetSortCode();
	m_strEposText = m_pPluRecord->GetEposText();
	m_strKeyText = m_pPluRecord->GetKeyText();
	m_strAgressoCostCentre = m_pPluRecord->GetAgressoCostCentre();
	m_strAgressoProject = m_pPluRecord->GetAgressoProject();
	m_nDeptNo = m_pPluRecord->GetBaseDeptNo();

	int nGLCode = m_pPluRecord->GetAgressoGLCode();
	if ((nGLCode >= 1000) && (nGLCode <= 9999))
	{
		m_strAgressoGLCode.Format("%d", nGLCode);
	}
	else
	{
		m_strAgressoGLCode = "";
	}

#ifdef STOCKMAN_SYSTEM

	if (m_pRecipeRecord->GetPluStockLinkFlag() == TRUE)
	{
		m_dPurchaseCost = DataManager.Plu.GetRecipeCost(*m_pRecipeRecord);
	}
	else
	{
		m_dPurchaseCost = m_pPluRecord->GetPurchaseCost();
	}

#else

	m_dPurchaseCost = m_pPluRecord->GetPurchaseCost();

#endif

	m_dReportCost = m_pPluRecord->GetReportCost();
	m_nGPType = m_pPluRecord->GetGPType();
	m_nPoints = m_pPluRecord->GetPoints();
	m_nStampWeight = m_pPluRecord->GetStampWeight();

	for (int i = 0; i < 10; i++)
	{
		m_dBasePrice[i] = m_pPluRecord->GetPrice(i);
	}

	switch (m_nGPType)
	{
	case PLU_GPTYPE_VARIABLE:
		m_dGPFixed = 0.0;
		m_dGPPercent = 0.0;
		break;

	case PLU_GPTYPE_FIXED:
		m_dGPFixed = m_pPluRecord->GetProfitMargin();
		m_dGPPercent = 0.0;
		break;

	case PLU_GPTYPE_PERCENT:
		m_dGPFixed = 0.0;
		m_dGPPercent = m_pPluRecord->GetProfitMargin();
		break;
	}
	UpdatePrice1(m_pPluRecord->GetBaseTaxBandInt());

	m_nFirstExtra = m_pPluRecord->GetFirstExtra();
	m_nFirstMenu = m_pPluRecord->GetFirstMenu();
	m_nFirstQualifier = m_pPluRecord->GetFirstQualifier();
}

/**********************************************************************/

void CPropPagePluGeneral::SetRecordControls()
{
#ifdef STOCKMAN_SYSTEM

	UpdateStockDisplay();

	{
		bool bEditStock = ((FALSE == m_bReadOnly) && (m_pRecipeRecord->GetSize() != 0) && (PriceSetItemManager.IsPluMaster() == TRUE));
		ShowAndEnableWindow(&m_buttonStock, TRUE == bEditStock);
		ShowAndEnableWindow(&m_staticStock, FALSE == bEditStock);
	}

#endif

	if (DealerFlags.GetReportCostPriceFlag() == TRUE)
	{
		m_checkReportCost.SetCheck(m_pPluRecord->GetReportCostEnable());
	}

	m_DepartmentSelector.UpdateDepartmentCombo(m_nDeptNo);
	m_DepartmentSelector.UpdateDepartmentEditBox();
	m_nDeptNo = m_DepartmentSelector.GetDepartmentFilter();

	bool bAllergen = EcrmanOptions.GetFeaturesAllergenFlag();
	m_AllergenSelector1.FillGroupCombo(m_pPluRecord->GetAllergenNo(0));
	m_AllergenSelector2.FillGroupCombo(m_pPluRecord->GetAllergenNo(1));
	m_AllergenSelector3.FillGroupCombo(m_pPluRecord->GetAllergenNo(2));
	m_AllergenSelector4.FillGroupCombo(m_pPluRecord->GetAllergenNo(3));
	m_AllergenSelector5.FillGroupCombo(m_pPluRecord->GetAllergenNo(4));
	m_AllergenSelector6.FillGroupCombo(m_pPluRecord->GetAllergenNo(5));

	CTime start(1975, 1, 1, 0, 0, 0, -1);
	CTime end(2035, 1, 1, 0, 0, 0, -1);
	m_DatePicker.SetRange(&start, &end);
	m_DatePicker.SetFormat("ddd dd MMM yyy");

	bool bPluByDate = EcrmanOptions.GetPluDownloadByDateFlag();
	COleDateTime time = COleDateTime::GetCurrentTime();

	if (bPluByDate == TRUE)
	{
		CString strPluDate = m_pPluRecord->GetDownloadDatePlu();
		if (strPluDate.GetLength() >= 8)
		{
			int nMDay = atoi(strPluDate.Mid(6, 2));
			int nMonth = atoi(strPluDate.Mid(4, 2));
			int nYear = atoi(strPluDate.Left(4));
			time = COleDateTime(nYear, nMonth, nMDay, 0, 0, 0);
		}
	}
	else
	{
		CString strPluDate = m_pPluRecord->GetModifiedDateTimePlu();
		if (strPluDate.GetLength() >= 8)
		{
			int nMDay = atoi(strPluDate.Mid(6, 2));
			int nMonth = atoi(strPluDate.Mid(4, 2));
			int nYear = atoi(strPluDate.Left(4));
			time = COleDateTime(nYear, nMonth, nMDay, 0, 0, 0);
		}
	}

	m_DatePicker.SetTime(time);
	m_DatePicker.EnableWindow(bPluByDate);
	m_staticDate.SetWindowText((bPluByDate == TRUE) ? "Download" : "Modified");

	m_KbdListSelectorMenu.UpdateKbdListCombo(m_nFirstMenu);
	m_KbdListSelectorMenu.UpdateKbdListEditBox();
	m_nFirstMenu = m_KbdListSelectorMenu.GetKbdListNo();

	m_KbdListSelectorExtra.UpdateKbdListCombo(m_nFirstExtra);
	m_KbdListSelectorExtra.UpdateKbdListEditBox();
	m_nFirstExtra = m_KbdListSelectorExtra.GetKbdListNo();

	m_KbdListSelectorQualifier.UpdateKbdListCombo(m_nFirstQualifier);
	m_KbdListSelectorQualifier.UpdateKbdListEditBox();
	m_nFirstQualifier = m_KbdListSelectorQualifier.GetKbdListNo();

	m_checkAllowMod.SetCheck(m_pPluRecord->GetAllowModifierFlag());

	UpdateData(FALSE);
	SetGPEnables();

	if (m_bReadOnly == TRUE)
	{
		m_comboStatus.EnableWindow(FALSE);
	}

	if ((PriceSetItemManager.IsPluMaster() == FALSE) || (m_bReadOnly == TRUE))
	{
		int nPluStatus = PriceSetItemManager.GetPluStatus();

		switch (nPluStatus)
		{
		case LOCALPLU_MASTER:	
			m_comboStatus.SetCurSel(1);
			break;

		case LOCALPLU_LOCAL:
			m_comboStatus.SetCurSel(0);
			break;

		case LOCALPLU_EXCLUDE:
			m_comboStatus.SetCurSel(2);
			break;

		default:
			m_comboStatus.SetCurSel(1);
			PriceSetItemManager.SetPluStatus(LOCALPLU_MASTER);
			break;
		}

		m_radioGPType.EnableWindow(FALSE);
		m_radioGPFixed.EnableWindow(FALSE);
		m_radioGPPercent.EnableWindow(FALSE);
		GetEditGPFixed()->EnableWindow(FALSE);
		GetEditGPPercent()->EnableWindow(FALSE);

		if ((nPluStatus != LOCALPLU_LOCAL) || (m_bReadOnly == TRUE))
		{
			m_editEposText.EnableWindow(FALSE);
			m_editRepText.EnableWindow(FALSE);
			m_editKeyText.EnableWindow(FALSE);
			m_editSortCode.EnableWindow(FALSE);
			m_editDeptNo.EnableWindow(FALSE);
			m_comboDeptName.EnableWindow(FALSE);
			m_DatePicker.EnableWindow(FALSE);
			m_comboPluType.EnableWindow(FALSE);
			m_comboActive.EnableWindow(FALSE);
			m_comboProcess.EnableWindow(FALSE);
			m_comboTaxBand.EnableWindow(FALSE);
			m_comboCategory.EnableWindow(FALSE);
			GetEditPoints()->EnableWindow(FALSE);
			GetEditStampWeight()->EnableWindow(FALSE);

			for (int i = 0; i < 10; i++)
			{
				GetEditBasePrice(i)->EnableWindow(FALSE);
			}

			for (int i = 0; i < 6; i++)
			{
				m_editAllergen[i].EnableWindow(FALSE);
				m_comboAllergen[i].EnableWindow(FALSE);
			}

			m_editFirstExtra.EnableWindow(FALSE);
			m_comboFirstExtra.EnableWindow(FALSE);
			m_editFirstMenu.EnableWindow(FALSE);
			m_comboFirstMenu.EnableWindow(FALSE);
			m_editFirstQualifier.EnableWindow(FALSE);
			m_comboFirstQualifier.EnableWindow(FALSE);
			m_checkReportCost.EnableWindow(FALSE);
			m_checkAllowMod.EnableWindow(FALSE);
		}
		else
		{
			m_editEposText.EnableWindow(PriceSetItemManager.CheckField(Plu::EposText.Label));
			m_editRepText.EnableWindow(PriceSetItemManager.CheckField(Plu::RepText.Label));
			m_editKeyText.EnableWindow(PriceSetItemManager.CheckField(Plu::KeyText.Label));
			m_editDeptNo.EnableWindow(PriceSetItemManager.CheckField(Plu::DeptNo.Label));
			m_comboDeptName.EnableWindow(PriceSetItemManager.CheckField(Plu::DeptNo.Label));
			m_comboPluType.EnableWindow(PriceSetItemManager.CheckField(Plu::MicrotrendPluType.Label));
			m_comboActive.EnableWindow(PriceSetItemManager.CheckField(Plu::ActiveItem.Label));
			m_comboProcess.EnableWindow(PriceSetItemManager.CheckField(Plu::ExternalProcess.Label));
			m_comboTaxBand.EnableWindow(PriceSetItemManager.CheckField(Plu::TaxBand.Label));
			m_comboCategory.EnableWindow(PriceSetItemManager.CheckField(Plu::AnalysisCategory.Label));
			GetEditPoints()->EnableWindow(PriceSetItemManager.CheckField(Plu::Points.Label));
			GetEditStampWeight()->EnableWindow(PriceSetItemManager.CheckField(Plu::StampWeight.Label));
			m_editFirstExtra.EnableWindow(PriceSetItemManager.CheckField(Plu::FirstExtra.Label));
			m_comboFirstExtra.EnableWindow(PriceSetItemManager.CheckField(Plu::FirstExtra.Label));
			m_editFirstMenu.EnableWindow(PriceSetItemManager.CheckField(Plu::FirstMenu.Label));
			m_comboFirstMenu.EnableWindow(PriceSetItemManager.CheckField(Plu::FirstMenu.Label));
			m_editFirstQualifier.EnableWindow(PriceSetItemManager.CheckField(Plu::FirstQualifier.Label));
			m_comboFirstQualifier.EnableWindow(PriceSetItemManager.CheckField(Plu::FirstQualifier.Label));
			m_checkAllowMod.EnableWindow(PriceSetItemManager.CheckField(Plu::AllowModifier.Label));
			m_DatePicker.EnableWindow(FALSE);
			m_editSortCode.EnableWindow(FALSE);
			m_checkReportCost.EnableWindow(FALSE);

			for (int i = 0; i < 10; i++)
			{
				CString strLabel = "";
				CPluCSVRecord::GetPriceLabel(i + 1, strLabel);
				GetEditBasePrice(i)->EnableWindow(PriceSetItemManager.CheckField(strLabel));
			}

			for (int i = 0; i < 6; i++)
			{
				CString strLabel = "";
				CPluCSVRecord::GetAllergenLabel(i + 1, strLabel);
				bool bEnable = PriceSetItemManager.CheckField(strLabel) && (TRUE == bAllergen);
				m_editAllergen[i].EnableWindow(bEnable);
				m_comboAllergen[i].EnableWindow(bEnable);
			}
		}
	}

	if (m_bEditDepartments == FALSE)
	{
		m_editDeptNo.EnableWindow(FALSE);
		m_comboDeptName.EnableWindow(FALSE);
	}

	{
		bool bHideStampWeight = TRUE;
		if (SysInfo.GotLoyaltyOrSmartPayLink() == FALSE)
		{
			ShowAndEnableWindow(&m_staticPoints, FALSE);
			ShowAndEnableWindow(GetEditPoints(), FALSE);
		}
		else if (SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_SMP_V4)
		{
			bHideStampWeight = FALSE;
		}

		if (TRUE == bHideStampWeight)
		{
			ShowAndEnableWindow(&m_staticStampWeight, FALSE);
			ShowAndEnableWindow(GetEditStampWeight(), FALSE);
		}
	}


	GetEditPurchaseCost()->EnableWindow(CanEditPurchaseCost());
	GetEditReportCost()->EnableWindow(CanEditReportCost());

	if (m_checkReportCost.GetCheck() == 0)
	{
		GetEditReportCost()->SetWindowText("");
	}

	__int64 nSharpPluNo = 0;
	bool bModifiable = FALSE;
	CMinimumSPOSVersion Version;
	m_pPluRecord -> CheckMinimumSPOSVersion(Version, nSharpPluNo, bModifiable);
	m_checkAllowMod.ShowWindow(bModifiable ? SW_SHOW : SW_HIDE);
}

/**********************************************************************/

void CPropPagePluGeneral::OnToggleReportCost()
{
	if (DealerFlags.GetReportCostPriceFlag() == TRUE)
	{
		bool bEnable = (m_checkReportCost.GetCheck() != 0);

		CString str = "";
		if (TRUE == bEnable)
		{
			str.Format("%.5f", m_dReportCost);
		}
		else
		{
			CString strTmp = "";
			GetEditReportCost()->GetWindowText(strTmp);
			m_dReportCost = atof(strTmp);
		}

		GetEditReportCost()->EnableWindow(bEnable);
		GetEditReportCost()->SetWindowText(str);
	}
}

/**********************************************************************/

void CPropPagePluGeneral::OnKillFocusDeptNo() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
	m_nDeptNo = m_DepartmentSelector.GetDepartmentFilter();
}

/**********************************************************************/

void CPropPagePluGeneral::OnSelectDept() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
	m_nDeptNo = m_DepartmentSelector.GetDepartmentFilter();
}

/**********************************************************************/
void CPropPagePluGeneral::OnKillFocusAllergen1(){ KillFocusAllergen( m_AllergenSelector1 ); }
void CPropPagePluGeneral::OnKillFocusAllergen2(){ KillFocusAllergen( m_AllergenSelector2 ); } 
void CPropPagePluGeneral::OnKillFocusAllergen3(){ KillFocusAllergen( m_AllergenSelector3 ); }
void CPropPagePluGeneral::OnKillFocusAllergen4(){ KillFocusAllergen( m_AllergenSelector4 ); }
void CPropPagePluGeneral::OnKillFocusAllergen5(){ KillFocusAllergen( m_AllergenSelector5 ); } 
void CPropPagePluGeneral::OnKillFocusAllergen6(){ KillFocusAllergen( m_AllergenSelector6 ); } 
/**********************************************************************/

void CPropPagePluGeneral::KillFocusAllergen( CGroupSelector& AllergenSelector )
{
	AllergenSelector.SelectGroupFromEditBox();
}

/**********************************************************************/
void CPropPagePluGeneral::OnSelectAllergen1(){ SelectAllergen( m_AllergenSelector1 ); }
void CPropPagePluGeneral::OnSelectAllergen2(){ SelectAllergen( m_AllergenSelector2 ); } 
void CPropPagePluGeneral::OnSelectAllergen3(){ SelectAllergen( m_AllergenSelector3 ); }
void CPropPagePluGeneral::OnSelectAllergen4(){ SelectAllergen( m_AllergenSelector4 ); }
void CPropPagePluGeneral::OnSelectAllergen5(){ SelectAllergen( m_AllergenSelector5 ); } 
void CPropPagePluGeneral::OnSelectAllergen6(){ SelectAllergen( m_AllergenSelector6 ); } 
/**********************************************************************/

void CPropPagePluGeneral::SelectAllergen( CGroupSelector& AllergenSelector )
{
	AllergenSelector.SelectGroupFromCombo();
}

/**********************************************************************/

void CPropPagePluGeneral::OnSelectType()
{
	if ( GetPluType() == 9 )
	{
		m_comboProcess.ShowWindow( SW_SHOW );
		m_staticProcess.ShowWindow( SW_SHOW );
	}
	else
	{
		m_comboProcess.ShowWindow( SW_HIDE );
		m_staticProcess.ShowWindow( SW_HIDE );
	}
}

/**********************************************************************/
void CPropPagePluGeneral::OnCopyPrice2(){ CopyPrice(2); }
void CPropPagePluGeneral::OnCopyPrice3(){ CopyPrice(3); }
void CPropPagePluGeneral::OnCopyPrice4(){ CopyPrice(4); }
void CPropPagePluGeneral::OnCopyPrice5(){ CopyPrice(5); }
void CPropPagePluGeneral::OnCopyPrice6(){ CopyPrice(6); }
void CPropPagePluGeneral::OnCopyPrice7(){ CopyPrice(7); }
void CPropPagePluGeneral::OnCopyPrice8(){ CopyPrice(8); }
void CPropPagePluGeneral::OnCopyPrice9(){ CopyPrice(9); }
void CPropPagePluGeneral::OnCopyPrice10(){ CopyPrice(10); }
/**********************************************************************/

void CPropPagePluGeneral::CopyPrice( int n )
{
	CString str = "";
	GetEditBasePrice( 0 ) -> GetWindowText( str );
	GetEditBasePrice( n - 1 ) -> SetWindowText( str );
}

/**********************************************************************/

void CPropPagePluGeneral::OnKillFocusPurchaseCost() 
{
	UpdateData( TRUE, FALSE );
	UpdateStockCost();
	OnCalculateProfit();
}

/**********************************************************************/

void CPropPagePluGeneral::OnCalculateProfit() 
{
	UpdateData( TRUE, FALSE );
	UpdatePrice1( GetTaxBand() );
	SetGPEnables();
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPropPagePluGeneral::SetGPEnables()
{
	switch ( m_nGPType )
	{
	case PLU_GPTYPE_VARIABLE:
	default:
		GetEditGPFixed() -> EnableWindow ( FALSE );
		GetEditGPPercent() -> EnableWindow ( FALSE );
		GetEditBasePrice(0) -> EnableWindow ( TRUE );
		break;

	case PLU_GPTYPE_FIXED:
		GetEditGPFixed() -> EnableWindow ( TRUE );
		GetEditGPPercent() -> EnableWindow ( FALSE );
		GetEditBasePrice(0) -> EnableWindow ( FALSE );
		break;

	case PLU_GPTYPE_PERCENT:
		GetEditGPFixed() -> EnableWindow ( FALSE );
		GetEditGPPercent() -> EnableWindow ( TRUE );
		GetEditBasePrice(0) -> EnableWindow ( FALSE );
		break;
	}
}

/**********************************************************************/

void CPropPagePluGeneral::UpdatePrice1(int nTaxBand)
{
	CTaxRateInfo TaxRateInfo;
	DataManager.TaxRates.GetTaxRateInfo(nTaxBand, TaxRateInfo);

	switch (m_nGPType)
	{
	default:
	case PLU_GPTYPE_VARIABLE:
		m_dGPFixed = ReportHelpers.CalcGP(m_dBasePrice[0], TaxRateInfo.m_dTaxRate, m_dPurchaseCost);
		m_dGPPercent = ReportHelpers.CalcGPPercent(m_dBasePrice[0], TaxRateInfo.m_dTaxRate, m_dPurchaseCost);
		break;

	case PLU_GPTYPE_FIXED:
		SetPrice1GPFixed(TaxRateInfo.m_dTaxRate);
		m_dGPPercent = ReportHelpers.CalcGPPercent(m_dBasePrice[0], TaxRateInfo.m_dTaxRate, m_dPurchaseCost);
		break;

	case PLU_GPTYPE_PERCENT:
		SetPrice1GPPercent(TaxRateInfo.m_dTaxRate);
		m_dGPFixed = ReportHelpers.CalcGP(m_dBasePrice[0], TaxRateInfo.m_dTaxRate, m_dPurchaseCost);
		break;
	}

	if (m_dBasePrice[0] < 0.0)
	{
		m_dBasePrice[0] = 0.0;
	}
}

/**********************************************************************/

void CPropPagePluGeneral::SetPrice1GPFixed ( double dTaxRate )
{
	m_dBasePrice[0] = CPriceHelpers::GetPrice1GPFixed ( dTaxRate, m_dPurchaseCost, m_dGPFixed );
}

/**********************************************************************/

void CPropPagePluGeneral::SetPrice1GPPercent ( double dTaxRate )
{
	m_dBasePrice[0] = CPriceHelpers::GetPrice1GPPercent ( dTaxRate, m_dPurchaseCost, m_dGPPercent );
}

/**********************************************************************/

void CPropPagePluGeneral::OnRadioGpType() 
{
	m_nGPType = 0;
	SetGPEnables();	
}

/**********************************************************************/

void CPropPagePluGeneral::OnRadioGpFixed() 
{
	m_nGPType = 1;
	SetGPEnables();	
}

/**********************************************************************/

void CPropPagePluGeneral::OnRadioGpPercent() 
{
	m_nGPType = 2;
	SetGPEnables();	
}

/**********************************************************************/

void CPropPagePluGeneral::OnButtonStock() 
{
#ifdef STOCKMAN_SYSTEM

	if ( TRUE == m_bSimpleEdit )
	{
		if ( m_pRecipeRecord -> GetSize() != 1 )
			return;

		if ( m_pRecipeRecord -> GetPluStockLinkFlag() == FALSE )
			return;

		CRecipeCSVRecord Recipe;
		Recipe.AddStockCode( m_pRecipeRecord -> GetStockCode(0), m_pRecipeRecord -> GetStockQty(0) );
		Recipe.SetSpNo( m_pRecipeRecord -> GetSpNo() );
		Recipe.SetReverseSaleFlag( m_pRecipeRecord -> GetReverseSaleFlag() );

		bool bApparent = ( DealerFlags.GetCurrentStockLevelsAccessType() != STOCKLEVELS_ACCESS_NONE );

		CPluStockItemDlg dlg ( Recipe, "Edit Stock Details", TRUE, TRUE, bApparent, this );		

		if ( dlg.DoModal() != IDOK )
			return;

		if ( Recipe.GetSize() == 0 )
			return;
		
		int nNewStockIdx;
		bool bNewStockRecord = FALSE;
		CString	strNewStockCode = Recipe.GetStockCode(0);
		if ( DataManager.Stock.FindStockItemByCode ( strNewStockCode, nNewStockIdx ) == FALSE )
		{
			CString strMsg = "There is no item with the stock code\n";
			strMsg += strNewStockCode;
			strMsg += " in the stock database.\n\n";
			strMsg += "Would you like to create this item now?";
			
			if ( Prompter.YesNo ( strMsg ) != IDYES )
				return;

			bNewStockRecord = TRUE;
		}
			
		if ( m_pPropertySheetPluStock != NULL )
		{	
			if ( bNewStockRecord == TRUE )
			{
				CStockCSVRecord StockRecord;
				StockRecord.SetStockCode ( strNewStockCode );

				CString strRepText = m_pPluRecord -> GetRepText();

				if ( strRepText != "" )
					StockRecord.SetDescription ( strRepText );
				else
					StockRecord.SetDescription ( m_pPluRecord -> GetEposText() );
				
				StockRecord.SetCategory ( m_pPluRecord -> GetBaseDeptNo() );
				DataManager.Stock.InsertRecord ( StockRecord, nNewStockIdx );
			}

			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt( nNewStockIdx, StockRecord );

			m_pRecipeRecord -> RemoveAll();
			m_pRecipeRecord -> AddStockCode ( strNewStockCode, Recipe.GetStockQty(0) );
			m_pRecipeRecord -> SetSpNo( Recipe.GetSpNo() );
			m_pRecipeRecord -> SetReverseSaleFlag( Recipe.GetReverseSaleFlag() );

			UpdateStockDisplay();
			
			CString strMsg;
			strMsg.Format ( "%.5f", DataManager.Plu.GetRecipeCost( *m_pRecipeRecord ) );
			GetEditPurchaseCost() -> SetWindowText( strMsg );
			
			CPropertySheetPluStock* pPropertySheet = (CPropertySheetPluStock*) m_pPropertySheetPluStock;
			pPropertySheet -> SetStockIdx ( nNewStockIdx );
			m_pPluChangesBuffer -> LogStockChange ( strNewStockCode );
		}
		OnCalculateProfit();
		GetEditPurchaseCost() -> EnableWindow( CanEditPurchaseCost() );
	}
	else
	{
		if ( m_pRecipeRecord -> GetPluStockLinkFlag() == FALSE )
			return;

		CRecipeCSVRecord Recipe;
		Recipe.SetSpNo( m_pRecipeRecord -> GetSpNo() );
		Recipe.SetReverseSaleFlag( m_pRecipeRecord -> GetReverseSaleFlag() );

		CPluStockActionDlg dlg ( Recipe, this );		

		if ( dlg.DoModal() != IDOK )
			return;

		m_pRecipeRecord -> SetSpNo( Recipe.GetSpNo() );
		m_pRecipeRecord -> SetReverseSaleFlag( Recipe.GetReverseSaleFlag() );

		UpdateStockDisplay();
	}
#endif
}

/**********************************************************************/

void CPropPagePluGeneral::OnChangePluStatus() 
{
	int nNewStatus = LOCALPLU_MASTER;

	switch ( m_comboStatus.GetCurSel() )
	{
	case 0:
		nNewStatus = LOCALPLU_LOCAL;	
		break;

	case 1:
		nNewStatus = LOCALPLU_MASTER;	
		break;

	case 2:
		nNewStatus = LOCALPLU_EXCLUDE;
		break;
	}

	if ( nNewStatus != PriceSetItemManager.GetPluStatus() )
	{
		switch( nNewStatus )
		{
		case LOCALPLU_LOCAL:
			PriceSetItemManager.RememberMasterRecord();
			PriceSetItemManager.SetPluStatus ( LOCALPLU_LOCAL );
			SetRecordControls();
			break;

		default:
			if ( PriceSetItemManager.GetPluStatus() == LOCALPLU_LOCAL )
			{
				PriceSetItemManager.ReloadMasterRecord( *m_pPluRecord );
				PriceSetItemManager.SetPluStatus( nNewStatus );
				Refresh();
			}
			else
			{
				PriceSetItemManager.SetPluStatus( nNewStatus );
				SetRecordControls();	
			}
			break;
		}
	}
}

/**********************************************************************/

int CPropPagePluGeneral::GetStockIdxForCost()
{
#ifdef STOCKMAN_SYSTEM

	if ( PriceSetItemManager.IsPluMaster() == FALSE )
		return -1;

	if ( StockOptions.GetStockMaxSuppliersPerItem() != 1 )
		return -1;

	if ( m_pRecipeRecord -> GetSize() != 1 )
		return -1;

	if ( m_pRecipeRecord -> GetPluStockLinkFlag() == FALSE )
		return -1;

	int nStockIdx;
	CString strStockCode = m_pRecipeRecord -> GetStockCode(0);
	if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == FALSE )
		return -1;

	double dSaleQty = m_pRecipeRecord -> GetStockQty(0);
	if ( CPriceHelpers::CompareDoubles( dSaleQty, 1.0, 5 ) != 0 )
		return -1;

	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	
	if ( StockRecord.GetSuppNo(0) <= 0 )
		return -1;

	if ( StockRecord.GetDUItems(0) != 1 )
		return -1;

	if ( CPriceHelpers::CompareDoubles ( StockRecord.GetDUItemSize(0), 1.0, 3 ) != 0 )
		return -1;

	return nStockIdx;

#else

	return -1;

#endif
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

bool CPropPagePluGeneral::CanEditPurchaseCost()
{
	bool bResult = FALSE;

	if ( m_pRecipeRecord -> GetPluStockLinkFlag() == FALSE )
	{
		bResult = ( FALSE == m_bReadOnly );
		if ( ( PriceSetItemManager.IsPluMaster() == FALSE ) && ( TRUE == bResult ) )
		{
			if ( PriceSetItemManager.GetPluStatus() != LOCALPLU_LOCAL )
				bResult = FALSE;
			else
				bResult = PriceSetItemManager.CheckField ( Plu::PurchaseCost.Label );
		}
	}
	else 
	{
		bResult = ( GetStockIdxForCost() != -1 );
	}

	return bResult;
}

/**********************************************************************/
#else
/**********************************************************************/

bool CPropPagePluGeneral::CanEditPurchaseCost()
{
	bool bResult = ( FALSE == m_bReadOnly );
	if ( ( PriceSetItemManager.IsPluMaster() == FALSE ) && ( TRUE == bResult ) )
	{
		if ( PriceSetItemManager.GetPluStatus() != LOCALPLU_LOCAL )
			bResult = FALSE;
		else
			bResult = PriceSetItemManager.CheckField ( Plu::PurchaseCost.Label );
	}
	
	return bResult;
}

/**********************************************************************/
#endif
/**********************************************************************/

bool CPropPagePluGeneral::CanEditReportCost()
{
	bool bResult = (FALSE == m_bReadOnly);
	bResult &= DealerFlags.GetReportCostPriceFlag();
	bResult &= (m_checkReportCost.GetCheck() != 0);

	if ((PriceSetItemManager.IsPluMaster() == FALSE) && (TRUE == bResult))
	{
		if (PriceSetItemManager.GetPluStatus() != LOCALPLU_LOCAL)
		{
			bResult = FALSE;
		}
		else
		{
			bResult = PriceSetItemManager.CheckField(Plu::ReportCost.Label);
		}
	}

	return bResult;
}

/**********************************************************************/

void CPropPagePluGeneral::UpdateStockCost()
{
#ifdef STOCKMAN_SYSTEM
	if ( m_pRecipeRecord -> GetPluStockLinkFlag() == TRUE )
	{
		int nStockIdx = GetStockIdxForCost();
		if ( nStockIdx >= 0 && nStockIdx < DataManager.Stock.GetSize() )
		{
			CString strStockCode = DataManager.Stock.GetStockCode( nStockIdx );
			m_pPluChangesBuffer -> LogStockChange ( strStockCode );
				
			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt ( nStockIdx, StockRecord );
			StockRecord.SetCost( 0, m_dPurchaseCost );	
			DataManager.Stock.SetAt ( nStockIdx, StockRecord );
		}
	}
#endif
}

/**********************************************************************/

void CPropPagePluGeneral::UpdateStockDisplay()
{
#ifdef STOCKMAN_SYSTEM
	
	CString strStock = "";
	
	switch( m_pRecipeRecord -> GetSize() )
	{
	case 0:
		if ( m_pRecipeRecord -> GetPluStockLinkFlag() == FALSE )
			strStock += "Non Stock Item";
		else
			strStock += "No Linked Stock Item";
		break;

	case 1:
		{
			CString strStockCode = m_pRecipeRecord -> GetStockCode(0);
			strStock += strStockCode;

			int nStockIdx;
			if ( DataManager.Stock.FindStockItemByCode( strStockCode, nStockIdx ) == TRUE )
			{
				CStockCSVRecord StockRecord;
				DataManager.Stock.GetAt( nStockIdx, StockRecord );

				double dQty = m_pRecipeRecord -> GetStockQty(0);
			
				strStock += "\x0D\x0A";
				strStock += StockRecord.GetDescription();
				strStock += "\x0D\x0A";
				strStock += ( DataManager.Stock.GetStockLevelStringForPluTab( StockRecord, dQty, 5 ) );
			}
		}
		break;

	default:
		strStock += "Recipe";
		break;
	}

	if ( m_pRecipeRecord -> GetSize() != 0 )
	{
		if ( StockOptions.GetPluRedirectToStockpointFlag() == TRUE )
		{
			int nSpNo = m_pRecipeRecord -> GetSpNo();
			if ( nSpNo != 0 )
			{
				int nSpIdx;
				int nDbNo = dbDatabase.GetDbNo( DataManager.GetActiveDbIdx() );
				if ( dbStockpoint.FindStockpointByNumber( nDbNo, nSpNo, nSpIdx ) == TRUE )
				{
					strStock += "\x0D\x0A";
					strStock += "Assign sales to ";
					strStock += dbStockpoint.GetName( nSpIdx );
				}
			}
		}

		if ( m_pRecipeRecord -> GetReverseSaleFlag() == TRUE )
		{
			strStock += "\x0D\x0A";
			strStock += "Sales increase apparent stock";
		}
	}
	
	m_editStock.SetWindowText( strStock );
#endif
}

/**********************************************************************/

void CPropPagePluGeneral::OnKillFocusFirstMenu() 
{
	m_KbdListSelectorMenu.SelectKbdListFromEditBox();
	m_nFirstMenu = m_KbdListSelectorMenu.GetKbdListNo();
}

/**********************************************************************/

void CPropPagePluGeneral::OnSelectFirstMenu() 
{
	m_KbdListSelectorMenu.SelectKbdListFromCombo();
	m_nFirstMenu = m_KbdListSelectorMenu.GetKbdListNo();
}

/**********************************************************************/

void CPropPagePluGeneral::OnKillFocusFirstExtra() 
{
	m_KbdListSelectorExtra.SelectKbdListFromEditBox();
	m_nFirstExtra = m_KbdListSelectorExtra.GetKbdListNo();
}

/**********************************************************************/

void CPropPagePluGeneral::OnSelectFirstExtra() 
{
	m_KbdListSelectorExtra.SelectKbdListFromCombo();
	m_nFirstExtra = m_KbdListSelectorExtra.GetKbdListNo();
}

/**********************************************************************/

void CPropPagePluGeneral::OnKillFocusFirstQualifier() 
{
	m_KbdListSelectorQualifier.SelectKbdListFromEditBox();
	m_nFirstQualifier = m_KbdListSelectorQualifier.GetKbdListNo();	
}

/**********************************************************************/

void CPropPagePluGeneral::OnSelectFirstQualifier() 
{
	m_KbdListSelectorQualifier.SelectKbdListFromCombo();
	m_nFirstQualifier = m_KbdListSelectorQualifier.GetKbdListNo();
}

/**********************************************************************/

