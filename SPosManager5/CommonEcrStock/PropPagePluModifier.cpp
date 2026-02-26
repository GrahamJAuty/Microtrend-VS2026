/**********************************************************************/
#include "PriceSetItemManager.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPagePluModifier.h"
/**********************************************************************/

CPropPagePluModifier::CPropPagePluModifier() : CSSPropertyPage(CPropPagePluModifier::IDD),
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName, TRUE ),
	m_KbdListSelectorMenu( m_editFirstMenu, m_comboFirstMenu, TRUE ),
	m_KbdListSelectorExtra( m_editFirstExtra, m_comboFirstExtra, TRUE ),
	m_KbdListSelectorQualifier( m_editFirstQualifier, m_comboFirstQualifier, TRUE )
{
	//{{AFX_DATA_INIT(CPropPagePluModifier)
	//}}AFX_DATA_INIT
	m_pPluRecord = NULL;

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

	m_staticModPriceID[0] = IDC_STATIC_MOD_PRICE1;
	m_staticModPriceID[1] = IDC_STATIC_MOD_PRICE2;
	m_staticModPriceID[2] = IDC_STATIC_MOD_PRICE3;
	m_staticModPriceID[3] = IDC_STATIC_MOD_PRICE4;
	m_staticModPriceID[4] = IDC_STATIC_MOD_PRICE5;
	m_staticModPriceID[5] = IDC_STATIC_MOD_PRICE6;
	m_staticModPriceID[6] = IDC_STATIC_MOD_PRICE7;
	m_staticModPriceID[7] = IDC_STATIC_MOD_PRICE8;
	m_staticModPriceID[8] = IDC_STATIC_MOD_PRICE9;
	m_staticModPriceID[9] = IDC_STATIC_MOD_PRICE10;
	
	m_editModPriceID[0] = IDC_EDIT_MOD_PRICE1;
	m_editModPriceID[1] = IDC_EDIT_MOD_PRICE2;
	m_editModPriceID[2] = IDC_EDIT_MOD_PRICE3;
	m_editModPriceID[3] = IDC_EDIT_MOD_PRICE4;
	m_editModPriceID[4] = IDC_EDIT_MOD_PRICE5;
	m_editModPriceID[5] = IDC_EDIT_MOD_PRICE6;
	m_editModPriceID[6] = IDC_EDIT_MOD_PRICE7;
	m_editModPriceID[7] = IDC_EDIT_MOD_PRICE8;
	m_editModPriceID[8] = IDC_EDIT_MOD_PRICE9;
	m_editModPriceID[9] = IDC_EDIT_MOD_PRICE10;
	
	m_bReadOnly = FALSE;
	m_nActiveTab = 0;
}

/**********************************************************************/

CPropPagePluModifier::~CPropPagePluModifier()
{
}

/**********************************************************************/

void CPropPagePluModifier::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePluModifier)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_EDIT_EPOSTEXT, m_editBaseEposText);
	DDX_Control(pDX, IDC_EDIT_KEYTEXT, m_editBaseKeyText);
	DDX_Control(pDX, IDC_TAB_MODIFIERS, m_TabModifiers);
	DDX_Control(pDX, IDC_CHECK_MOD_ENABLE, m_checkModEnable);
	DDX_Control(pDX, IDC_CHECK_MOD_EPOSTEXT, m_checkModEposText);
	DDX_Control(pDX, IDC_EDIT_MOD_EPOSTEXT, m_editModEposText);
	DDX_Control(pDX, IDC_CHECK_MOD_KEYTEXT, m_checkModKeyText);
	DDX_Control(pDX, IDC_EDIT_MOD_KEYTEXT, m_editModKeyText);
	DDX_Control(pDX, IDC_STATIC_DEPT, m_staticDept);	
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_STATIC_OP_GROUP, m_staticOPGroup);
	DDX_Control(pDX, IDC_COMBO_OP_GROUP, m_comboOPGroup);
	DDX_Control(pDX, IDC_STATIC_TAX, m_staticTaxBand);
	DDX_Control(pDX, IDC_COMBO_TAX, m_comboTaxBand);
	DDX_Control(pDX, IDC_STATIC_CATEGORY, m_staticCategory);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Control(pDX, IDC_STATIC_PLUTYPE, m_staticPluType);
	DDX_Control(pDX, IDC_COMBO_PLUTYPE, m_comboPluType);
	DDX_Control(pDX, IDC_STATIC_FIRSTMENU, m_staticFirstMenu);
	DDX_Control(pDX, IDC_STATIC_FIRSTQUALIFIER, m_staticFirstQualifier);
	DDX_Control(pDX, IDC_STATIC_FIRSTEXTRA, m_staticFirstExtra);
	DDX_Control(pDX, IDC_EDIT_FIRSTMENU, m_editFirstMenu);
	DDX_Control(pDX, IDC_EDIT_FIRSTQUALIFIER, m_editFirstQualifier);
	DDX_Control(pDX, IDC_EDIT_FIRSTEXTRA, m_editFirstExtra);
	DDX_Control(pDX, IDC_COMBO_FIRSTMENU, m_comboFirstMenu);
	DDX_Control(pDX, IDC_COMBO_FIRSTQUALIFIER, m_comboFirstQualifier);
	DDX_Control(pDX, IDC_COMBO_FIRSTEXTRA, m_comboFirstExtra);
	
	for ( int p = 0; p < 10; p++ )
	{
		DDX_Control(pDX, m_staticBasePriceID[p], m_staticBasePrice[p] );
		DDX_Control(pDX, m_staticModPriceID[p], m_staticModPrice[p] );
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePluModifier, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePluModifier)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MODIFIERS, OnTcnSelchangeTabs)
	ON_BN_CLICKED(IDC_CHECK_MOD_ENABLE, OnToggleModifier)
	ON_BN_CLICKED(IDC_CHECK_MOD_EPOSTEXT, OnToggleEposText)
	ON_BN_CLICKED(IDC_CHECK_MOD_KEYTEXT, OnToggleKeyText)
	ON_EN_KILLFOCUS(IDC_EDIT_EPOSTEXT, OnToggleEposText)
	ON_EN_KILLFOCUS(IDC_EDIT_KEYTEXT, OnToggleKeyText)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelectDept)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_EN_KILLFOCUS(IDC_EDIT_FIRSTMENU, OnKillFocusFirstMenu)
	ON_CBN_SELCHANGE(IDC_COMBO_FIRSTMENU, OnSelectFirstMenu)
	ON_EN_KILLFOCUS(IDC_EDIT_FIRSTQUALIFIER, OnKillFocusFirstQualifier)
	ON_CBN_SELCHANGE(IDC_COMBO_FIRSTQUALIFIER, OnSelectFirstQualifier)
	ON_EN_KILLFOCUS(IDC_EDIT_FIRSTEXTRA, OnKillFocusFirstExtra)
	ON_CBN_SELCHANGE(IDC_COMBO_FIRSTEXTRA, OnSelectFirstExtra)
	ON_BN_CLICKED(IDC_STATIC_PRICE2, OnCopyBasePrice2)
	ON_BN_CLICKED(IDC_STATIC_PRICE3, OnCopyBasePrice3)
	ON_BN_CLICKED(IDC_STATIC_PRICE4, OnCopyBasePrice4)
	ON_BN_CLICKED(IDC_STATIC_PRICE5, OnCopyBasePrice5)
	ON_BN_CLICKED(IDC_STATIC_PRICE6, OnCopyBasePrice6)
	ON_BN_CLICKED(IDC_STATIC_PRICE7, OnCopyBasePrice7)
	ON_BN_CLICKED(IDC_STATIC_PRICE8, OnCopyBasePrice8)
	ON_BN_CLICKED(IDC_STATIC_PRICE9, OnCopyBasePrice9)
	ON_BN_CLICKED(IDC_STATIC_PRICE10, OnCopyBasePrice10)
	ON_BN_CLICKED(IDC_STATIC_MOD_PRICE2, OnCopyModPrice2)
	ON_BN_CLICKED(IDC_STATIC_MOD_PRICE3, OnCopyModPrice3)
	ON_BN_CLICKED(IDC_STATIC_MOD_PRICE4, OnCopyModPrice4)
	ON_BN_CLICKED(IDC_STATIC_MOD_PRICE5, OnCopyModPrice5)
	ON_BN_CLICKED(IDC_STATIC_MOD_PRICE6, OnCopyModPrice6)
	ON_BN_CLICKED(IDC_STATIC_MOD_PRICE7, OnCopyModPrice7)
	ON_BN_CLICKED(IDC_STATIC_MOD_PRICE8, OnCopyModPrice8)
	ON_BN_CLICKED(IDC_STATIC_MOD_PRICE9, OnCopyModPrice9)
	ON_BN_CLICKED(IDC_STATIC_MOD_PRICE10, OnCopyModPrice10)
	//}}AFX_MSG_MAP
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePluModifier::OnInitDialog() 
{
	for ( int p = 0; p < 10; p++ )
	{
		m_pEditBasePrice[p] = GetEdit( m_editBasePriceID[p] );
		m_pEditModPrice[p] = GetEdit( m_editModPriceID[p] );
	}
	
	GetRecordData();
	CPropertyPage::OnInitDialog();

	{
		CArray<int, int> arrayStops;
		arrayStops.Add(5);
		arrayStops.Add(40);
		arrayStops.Add(140);
		m_comboTaxBand.SetTabStops( arrayStops );
	}

	{
		CArray<int, int> arrayStops;
		arrayStops.Add(5);
		arrayStops.Add(40);
		m_comboCategory.SetTabStops( arrayStops );
		m_comboOPGroup.SetTabStops( arrayStops );
		m_comboPluType.SetTabStops( arrayStops );
	}

	{
		int nTabNo = 0;
		for ( int nMod = 1; nMod <= 9; nMod++ )
		{
			CString strTabName = DataManager.Modifier.GetDisplayName( nMod );
			::FixAmpersandsForWindowsText( strTabName );
			m_TabModifiers.InsertItem ( nTabNo++, strTabName );
		}
	}

	m_editBaseEposText.LimitText( Plu::EposText.Max );
	m_editBaseKeyText.LimitText( Plu::KeyText.Max );
	
	m_editModEposText.LimitText( Plu::EposText.Max );
	m_editModKeyText.LimitText( Plu::KeyText.Max );

	m_editFirstMenu.LimitText(5);
	m_editFirstExtra.LimitText(5);
	m_editFirstQualifier.LimitText(5);

	m_KbdListSelectorMenu.FillKbdListCombo(-1);
	m_KbdListSelectorExtra.FillKbdListCombo(-1);
	m_KbdListSelectorQualifier.FillKbdListCombo(-1);

	for ( int p = 0; p < 10; p++ )
	{
		m_staticBasePrice[p].SetWindowText( DataManager.PriceText.GetReportText(p) );
		SubclassEdit ( m_editBasePriceID[p], SS_NUM_DP, Super.MaxPriceLen(), "%.*f", SysInfo.GetDPValue() );

		SubclassEdit ( m_editModPriceID[p], SS_NUM_DP, Super.MaxPriceLen(), "%.*f", SysInfo.GetDPValue() );
		m_staticModPrice[p].SetWindowText( DataManager.PriceText.GetReportText(p) );
	}

	m_nActiveTab = 0;
	InitialisePriceSetEnables();
	UpdateModifierEnable();
	UpdateModifierControls();
	SetRecordControls();

	return TRUE;  
}

/**********************************************************************/
CEdit* CPropPagePluModifier::GetEditBasePrice( int nLevel ) { return GetEdit( m_editBasePriceID[nLevel] ); }
CEdit* CPropPagePluModifier::GetEditModPrice( int nLevel ) { return GetEdit( m_editModPriceID[nLevel] ); }
/**********************************************************************/

BOOL CPropPagePluModifier::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePluModifier::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	InitialisePriceSetEnables();
	UpdateModifierEnable();
	UpdateModifierControls();
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPagePluModifier::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	InitialisePriceSetEnables();
	UpdateModifierEnable();
	UpdateModifierControls();
	SetRecordControls();
}

/**********************************************************************/

bool CPropPagePluModifier::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPagePluModifier::FillDepartmentCombo()
{
	int nMod = m_nActiveTab + 1;
	int nFilter = m_PluBuffer.GetModifierDeptNoRaw( nMod ) + 1;
	m_DepartmentSelector.FillDepartmentComboDatabase( nFilter, DEPARTMENT_SELECTOR_SINGLE );
}

/**********************************************************************/

void CPropPagePluModifier::FillOPGroupCombo()
{
	int nMod = m_nActiveTab + 1;

	if ( m_comboOPGroup.GetCount() == 0 )
	{
		m_comboOPGroup.AddItem( ",As Base Plu" );

		for ( int n = 0; n < DataManagerNonDb.OPGroupSystem.GetOPGroupCount(); n++ )
		{
			COPGroupMiniInfo info;
			DataManagerNonDb.OPGroupSystem.GetOPGroup( n, info );
			
			CString strGroup;
			strGroup.Format( "G%2.2d,%s", info.m_nGroupNo, (const char*) info.m_strName );
			
			m_comboOPGroup.AddItem( strGroup );
		}
	}

	int nSel = m_PluBuffer.GetModifierOrderPrinterGroup( nMod );
	if ( ( nSel >= 0 ) && ( nSel < m_comboOPGroup.GetCount() ) )
		m_comboOPGroup.SetCurSel( nSel );
	else
		m_comboOPGroup.SetCurSel( 0 );
}

/**********************************************************************/

void CPropPagePluModifier::FillTaxCombo()
{
	int nMod = m_nActiveTab + 1;

	//BASE ITEM TAX RATE MAY HAVE CHANGED IN GENERAL TAB
	m_comboTaxBand.ResetContent();
	{
		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo( m_PluBuffer.GetBaseTaxBandInt(), TaxRateInfo );

		CString strBand;
		strBand.Format( "[%s]", m_PluBuffer.GetBaseTaxBandString() );

		CString strRate;
		strRate.Format( "[%.4lf]", TaxRateInfo.m_dTaxRate );

		CCSV csv;
		csv.Add( strBand );
		csv.Add( "As Base Plu" );
		csv.Add( strRate );
		m_comboTaxBand.AddItem( csv.GetLine() );
	}

	CString strLetter = "A";
	for ( int nTaxBand = 1; nTaxBand <= MAX_TAX_BANDS; nTaxBand++ )
	{
		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo( nTaxBand, TaxRateInfo );

		strLetter.SetAt( 0, 'A' + nTaxBand - 1 );

		CString strRate;
		strRate.Format ( "%.4lf", TaxRateInfo.m_dTaxRate );
		
		CCSV csv;
		csv.Add( strLetter );
		csv.Add( TaxRateInfo.m_strReportText );
		csv.Add( strRate );
		m_comboTaxBand.AddItem( csv.GetLine() );
	}
	

	int nCurrentTaxBand = m_PluBuffer.GetModifierTaxBandIntRaw( nMod );
	if ( ( nCurrentTaxBand < 0 ) || ( nCurrentTaxBand > MAX_TAX_BANDS ) )
		nCurrentTaxBand = 0;

	m_comboTaxBand.SetCurSel( nCurrentTaxBand );
}

/**********************************************************************/

void CPropPagePluModifier::FillCategoryCombo()
{
	int nMod = m_nActiveTab + 1;

	//BASE ITEM CATEGORY MAY HAVE CHANGED IN GENERAL TAB
	m_comboCategory.ResetContent();
	{
		m_comboCategory.AddItem( ",As Base Plu" );
		m_comboCategory.SetItemData(0,0);
	}

	m_comboCategory.AddItem( ",Follow On" );
	m_comboCategory.SetItemData( 1, 1 );

	for ( int i = 0; i < DataManager.AnalysisCategory.GetInUseArraySize(); i++ )
	{
		int nCatNo = DataManager.AnalysisCategory.GetInUseCatNo(i);
		m_comboCategory.AddItem( DataManager.AnalysisCategory.GetTabbedComboText( nCatNo ) );
		m_comboCategory.SetItemData ( i + 2, nCatNo + 1 );
	}

	int nCatNo = m_PluBuffer.GetModifierAnalysisCategoryRaw( nMod );

	if ( m_comboCategory.SelectUsingItemData( nCatNo + 1 ) == FALSE )
	{
		CString strCategory;
		strCategory.Format( "C%2.2d, Category %d", nCatNo, nCatNo );
		m_comboCategory.InsertItem ( 2, strCategory );
		m_comboCategory.SetItemData ( 2, nCatNo + 1 );
		m_comboCategory.SetCurSel(1);
	}
}

/**********************************************************************/

void CPropPagePluModifier::FillPluTypeCombo()
{
	int nMod = m_nActiveTab + 1;

	m_comboPluType.ResetContent();
	m_comboPluType.AddItem( ",As Base Plu" );
	m_comboPluType.AddItem( ",General" );
	m_comboPluType.AddItem( ",Qualifier" );
	
	m_comboPluType.SetItemData ( 0, 0 );
	m_comboPluType.SetItemData ( 1, 1 );
	m_comboPluType.SetItemData ( 2, 5 );
	
	int nPluType = m_PluBuffer.GetModifierMicrotrendPluType( nMod );

	if ( m_comboPluType.SelectUsingItemData( nPluType + 1 ) == FALSE )
	{
		m_comboPluType.SetCurSel(0);
	}
}

/**********************************************************************/

void CPropPagePluModifier::GetRecordData()
{
	CPluCSVArray arrayTemp;
	arrayTemp.Add( *m_pPluRecord );
	arrayTemp.GetAt( 0, m_PluBuffer );
}

/**********************************************************************/

void CPropPagePluModifier::SaveTabChanges()
{ 
	if ( m_bReadOnly == TRUE )
		return;

	int nMod = m_nActiveTab + 1;
	
	CString strText;
	m_editBaseEposText.GetWindowText( strText );
	m_PluBuffer.SetEposText( strText );
	m_editBaseKeyText.GetWindowText( strText );
	m_PluBuffer.SetKeyText( strText );

	m_PluBuffer.SetModifierEnable( nMod, ( m_checkModEnable.GetCheck() != 0 ) );

	if ( m_checkModEposText.GetCheck() != 0 )
	{
		m_editModEposText.GetWindowText( strText );
		m_PluBuffer.SetModifierEposText( nMod, TRUE, strText );
	}
	else
	{
		m_PluBuffer.SetModifierEposText( nMod, FALSE, "" );
	}

	if ( m_checkModKeyText.GetCheck() != 0 )
	{
		m_editModKeyText.GetWindowText( strText );
		m_PluBuffer.SetModifierKeyText( nMod, TRUE, strText );
	}
	else
	{
		m_PluBuffer.SetModifierKeyText( nMod, FALSE, "" );
	}

	m_PluBuffer.SetModifierDeptNoRaw( nMod, m_DepartmentSelector.GetDepartmentFilter() - 1 );
	m_PluBuffer.SetModifierOrderPrinterGroup( nMod, m_comboOPGroup.GetCurSel() );
	m_PluBuffer.SetModifierTaxBandIntRaw( nMod, m_comboTaxBand.GetCurSel() );
	
	int nCategory = -1;
	int nSel = m_comboCategory.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_comboCategory.GetCount() ) )
	{
		nCategory = m_comboCategory.GetItemData(nSel) - 1;
	}

	int nPluType = -1;
	nSel = m_comboPluType.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_comboPluType.GetCount() ) )
	{
		nPluType = m_comboPluType.GetItemData(nSel) - 1;
	}

	m_PluBuffer.SetModifierAnalysisCategoryRaw( nMod, nCategory );
	m_PluBuffer.SetModifierMicrotrendPluType( nMod, nPluType );
	m_PluBuffer.SetModifierFirstMenu( nMod, m_nFirstMenu );
	m_PluBuffer.SetModifierFirstQualifier( nMod, m_nFirstQualifier );
	m_PluBuffer.SetModifierFirstExtra( nMod, m_nFirstExtra );

	for ( int p = 0; p < 10; p++ )
	{
		m_pEditBasePrice[p] -> GetWindowText( strText );
		m_PluBuffer.SetPrice ( p, atof( strText ) );

		m_pEditModPrice[p] -> GetWindowText( strText );
		m_PluBuffer.SetModifierPrice ( nMod, p, atof( strText ) );
	}
}

/**********************************************************************/

void CPropPagePluModifier::SaveRecord()
{ 
	if ( m_bReadOnly == TRUE )
		return;

	SaveTabChanges();

	m_pPluRecord -> SetEposText( m_PluBuffer.GetEposText() );
	m_pPluRecord -> SetKeyText( m_PluBuffer.GetKeyText() );

	for ( int nLevel = 0; nLevel < 10; nLevel++ )
	{
		m_pPluRecord -> SetPrice( nLevel, m_PluBuffer.GetPrice( nLevel ) );
	}

	for ( int nMod = 1; nMod <= 9; nMod++ )
	{
		m_pPluRecord -> SetModifierEposText( nMod, m_PluBuffer.GetModifierEposTextFlag( nMod ), m_PluBuffer.GetModifierEposText( nMod ) );
		m_pPluRecord -> SetModifierKeyText( nMod, m_PluBuffer.GetModifierKeyTextFlag( nMod ), m_PluBuffer.GetModifierKeyText( nMod ) );
		m_pPluRecord -> SetModifierDeptNoRaw( nMod, m_PluBuffer.GetModifierDeptNoRaw(nMod) );
		m_pPluRecord -> SetModifierOrderPrinterGroup( nMod, m_PluBuffer.GetModifierOrderPrinterGroup(nMod) );
		m_pPluRecord -> SetModifierTaxBandStringRaw( nMod, m_PluBuffer.GetModifierTaxBandStringRaw(nMod) );
		m_pPluRecord -> SetModifierAnalysisCategoryRaw( nMod, m_PluBuffer.GetModifierAnalysisCategoryRaw(nMod) );
		m_pPluRecord -> SetModifierMicrotrendPluType( nMod, m_PluBuffer.GetModifierMicrotrendPluType(nMod) );
		m_pPluRecord -> SetModifierFirstMenu( nMod, m_PluBuffer.GetModifierFirstMenu(nMod) );
		m_pPluRecord -> SetModifierFirstQualifier( nMod, m_PluBuffer.GetModifierFirstQualifier(nMod) );
		m_pPluRecord -> SetModifierFirstExtra( nMod, m_PluBuffer.GetModifierFirstExtra(nMod) );
		
		for ( int nLevel = 0; nLevel < 10; nLevel++ )
			m_pPluRecord -> SetModifierPrice( nMod, nLevel, m_PluBuffer.GetModifierPrice( nMod, nLevel ) );

		if ( nMod != 0 )		
			m_pPluRecord -> SetModifierEnable( nMod, m_PluBuffer.GetModifierEnable( nMod ) );
	}
}

/**********************************************************************/

void CPropPagePluModifier::OnToggleModifier()
{ 
	SetRecordControls(); 
}

/**********************************************************************/

void CPropPagePluModifier::OnToggleEposText()
{
	int nMod = m_nActiveTab + 1;

	SetRecordControls();

	if ( m_checkModEposText.GetCheck() == 0 )
	{
		CString strText;
		m_editBaseEposText.GetWindowText( strText );
		m_PluBuffer.SetEposText( strText );
		m_PluBuffer.SetModifierEposText( nMod, FALSE, "" );
		m_editModEposText.SetWindowText( m_PluBuffer.GetModifierEposText( nMod ) );
	}
}

/**********************************************************************/

void CPropPagePluModifier::OnToggleKeyText()
{ 
	int nMod = m_nActiveTab + 1;

	SetRecordControls();

	if ( m_checkModKeyText.GetCheck() == 0 )
	{
		CString strText;
		m_editBaseKeyText.GetWindowText( strText );
		m_PluBuffer.SetKeyText( strText );
		m_PluBuffer.SetModifierKeyText( nMod, FALSE, "" );
		m_editModKeyText.SetWindowText( m_PluBuffer.GetModifierKeyText( nMod ) );
	}
}

/**********************************************************************/

void CPropPagePluModifier::OnKillFocusDeptNo() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
	//m_DepartmentSelector.GetDepartmentFilter();
}

/**********************************************************************/

void CPropPagePluModifier::OnSelectDept() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
	//m_DepartmentSelector.GetDepartmentFilter();
}

/**********************************************************************/

void CPropPagePluModifier::OnKillFocusFirstMenu() 
{
	m_KbdListSelectorMenu.SelectKbdListFromEditBox();
	m_nFirstMenu = m_KbdListSelectorMenu.GetKbdListNo();
}

/**********************************************************************/

void CPropPagePluModifier::OnSelectFirstMenu() 
{
	m_KbdListSelectorMenu.SelectKbdListFromCombo();
	m_nFirstMenu = m_KbdListSelectorMenu.GetKbdListNo();
}

/**********************************************************************/

void CPropPagePluModifier::OnKillFocusFirstExtra() 
{
	m_KbdListSelectorExtra.SelectKbdListFromEditBox();
	m_nFirstExtra = m_KbdListSelectorExtra.GetKbdListNo();
}

/**********************************************************************/

void CPropPagePluModifier::OnSelectFirstExtra() 
{
	m_KbdListSelectorExtra.SelectKbdListFromCombo();
	m_nFirstExtra = m_KbdListSelectorExtra.GetKbdListNo();
}

/**********************************************************************/

void CPropPagePluModifier::OnKillFocusFirstQualifier() 
{
	m_KbdListSelectorQualifier.SelectKbdListFromEditBox();
	m_nFirstQualifier = m_KbdListSelectorQualifier.GetKbdListNo();	
}

/**********************************************************************/

void CPropPagePluModifier::OnSelectFirstQualifier() 
{
	m_KbdListSelectorQualifier.SelectKbdListFromCombo();
	m_nFirstQualifier = m_KbdListSelectorQualifier.GetKbdListNo();
}

/**********************************************************************/

void CPropPagePluModifier::OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	SaveTabChanges();

	m_nActiveTab = m_TabModifiers.GetCurSel();
	if ( ( m_nActiveTab < 0 ) || ( m_nActiveTab > 8 ) )
		m_nActiveTab = 0;

	InitialisePriceSetEnables();
	UpdateModifierEnable();
	UpdateModifierControls();
	UpdateData( FALSE );
	SetRecordControls();
}

/**********************************************************************/

void CPropPagePluModifier::InitialisePriceSetEnables()
{
	int nMod = m_nActiveTab + 1;

	bool bGlobalSetting = FALSE;
	bool bGlobalValue = FALSE;

	if ( TRUE == m_bReadOnly )
	{
		bGlobalSetting = TRUE;
		bGlobalValue = FALSE;
	}
	else if ( PriceSetItemManager.IsPluMaster() == FALSE )
	{
		int nPluStatus = PriceSetItemManager.GetPluStatus();

		if ( nPluStatus != LOCALPLU_LOCAL )
		{
			bGlobalSetting = TRUE;
			bGlobalValue = FALSE;
		}
	}
	else
	{
		bGlobalSetting = TRUE;
		bGlobalValue = TRUE;
	}

	if ( TRUE == bGlobalSetting )
	{
		m_bPriceSetBaseEposTextEdit = bGlobalValue;
		m_bPriceSetBaseKeyTextEdit = bGlobalValue;

		for ( int p = 0; p < 10; p++ )
		{
			m_bPriceSetBasePrice[p] = bGlobalValue;
		}

		m_bPriceSetModEposTextTick = bGlobalValue;
		m_bPriceSetModEposTextEdit = bGlobalValue;
		m_bPriceSetModKeyTextTick = bGlobalValue;
		m_bPriceSetModKeyTextEdit = bGlobalValue;
		m_bPriceSetModEnable = bGlobalValue;
		m_bPriceSetModDept = bGlobalValue;
		m_bPriceSetModOPG = bGlobalValue;
		m_bPriceSetModTaxBand = bGlobalValue;
		m_bPriceSetModCategory = bGlobalValue;
		m_bPriceSetModPluType = bGlobalValue;
		m_bPriceSetModFirstMenu = bGlobalValue;
		m_bPriceSetModFirstQualifier = bGlobalValue;
		m_bPriceSetModFirstExtra = bGlobalValue;

		for ( int p = 0; p < 10; p++ )
		{
			m_bPriceSetModPrice[p] = bGlobalValue;	
		}		
	}
	else
	{
		//LOCAL PLU MODE WITH LOCAL RECORD
		CString strLabel = "";

		//ENABLE MODIFIERS
		CPluCSVRecord::GetModifierLabelEnable( nMod, FALSE, strLabel );
		m_bPriceSetModEnable = PriceSetItemManager.CheckField ( strLabel );
			
		CPluCSVRecord::GetModifierLabelEposTextFlag( nMod, FALSE, strLabel );
		m_bPriceSetModEposTextTick = PriceSetItemManager.CheckField ( strLabel );

		CPluCSVRecord::GetModifierLabelEposText( nMod, FALSE, strLabel );
		m_bPriceSetModEposTextEdit = PriceSetItemManager.CheckField ( strLabel );

		CPluCSVRecord::GetModifierLabelKeyTextFlag( nMod, FALSE, strLabel );
		m_bPriceSetModKeyTextTick = PriceSetItemManager.CheckField ( strLabel );

		CPluCSVRecord::GetModifierLabelKeyText( nMod, FALSE, strLabel );
		m_bPriceSetModKeyTextEdit = PriceSetItemManager.CheckField ( strLabel );

		CPluCSVRecord::GetModifierLabelDeptNo( nMod, strLabel );
		m_bPriceSetModDept = PriceSetItemManager.CheckField( strLabel );

		CPluCSVRecord::GetModifierLabelOrderPrinterGroup( nMod, strLabel );
		m_bPriceSetModOPG = PriceSetItemManager.CheckField( strLabel );

		CPluCSVRecord::GetModifierLabelTaxBand( nMod, FALSE, strLabel );
		m_bPriceSetModTaxBand = PriceSetItemManager.CheckField( strLabel );

		CPluCSVRecord::GetModifierLabelAnalysisCategory( nMod, strLabel );
		m_bPriceSetModCategory = PriceSetItemManager.CheckField( strLabel );

		CPluCSVRecord::GetModifierLabelMicrotrendPluType( nMod, strLabel );
		m_bPriceSetModPluType = PriceSetItemManager.CheckField( strLabel );

		CPluCSVRecord::GetModifierLabelFirstMenu( nMod, strLabel );
		m_bPriceSetModFirstMenu = PriceSetItemManager.CheckField( strLabel );

		CPluCSVRecord::GetModifierLabelFirstQualifier( nMod, strLabel );
		m_bPriceSetModFirstQualifier = PriceSetItemManager.CheckField( strLabel );

		CPluCSVRecord::GetModifierLabelFirstExtra( nMod, strLabel );
		m_bPriceSetModFirstExtra = PriceSetItemManager.CheckField( strLabel );
			
		for ( int p = 0; p < 10; p++ )
		{
			CPluCSVRecord::GetModifierLabelPrice( nMod, p + 1, FALSE, strLabel );
			m_bPriceSetModPrice[p] = PriceSetItemManager.CheckField( strLabel );
		}
		
		//ENABLE ROOT PLU DATA
		m_bPriceSetBaseEposTextEdit = PriceSetItemManager.CheckField ( Plu::EposText.Label );
		m_bPriceSetBaseKeyTextEdit = PriceSetItemManager.CheckField ( Plu::KeyText.Label );
		
		for ( int p = 0; p < 10; p++ )
		{
			CString strLabel;
			CPluCSVRecord::GetPriceLabel( p + 1, strLabel );
			m_bPriceSetBasePrice[p] = PriceSetItemManager.CheckField ( strLabel );
		}
	}
}

/**********************************************************************/

void CPropPagePluModifier::UpdateModifierEnable()
{
	int nMod = m_nActiveTab + 1;	
	m_checkModEnable.SetCheck( m_PluBuffer.GetModifierEnable( nMod ) );
	m_checkModEposText.SetCheck( m_PluBuffer.GetModifierEposTextFlag( nMod ) );
	m_checkModKeyText.SetCheck( m_PluBuffer.GetModifierKeyTextFlag( nMod ) );
}

/**********************************************************************/

void CPropPagePluModifier::SetRecordControls()
{
	int nMod = m_nActiveTab + 1;

	//SET ENABLES FOR MASTER RECORD
	m_editBaseEposText.EnableWindow ( m_bPriceSetBaseEposTextEdit );
	m_editBaseKeyText.EnableWindow ( m_bPriceSetBaseKeyTextEdit );

	for ( int p = 0; p < 10; p++ )
	{
		m_pEditBasePrice[p] -> EnableWindow ( m_bPriceSetBasePrice[p] );
	}

	{
		CString strModName;
		strModName.Format( "Enable Modifier (%s)", DataManager.Modifier.GetDisplayName( nMod ) );
		m_checkModEnable.SetWindowText( strModName );
	}

	m_checkModEnable.EnableWindow( m_bPriceSetModEnable );	
	bool bEnableModifier = IsTicked( m_checkModEnable );
	bool bEnableEposText = IsTicked( m_checkModEposText );
	bool bEnableKeyText = IsTicked( m_checkModKeyText );

	m_checkModEposText.EnableWindow( m_bPriceSetModEposTextTick & bEnableModifier );
	m_editModEposText.EnableWindow( m_bPriceSetModEposTextEdit & bEnableModifier & bEnableEposText );

	m_checkModKeyText.EnableWindow( m_bPriceSetModKeyTextTick & bEnableModifier );
	m_editModKeyText.EnableWindow( m_bPriceSetModKeyTextEdit & bEnableModifier & bEnableKeyText );
	m_staticDept.EnableWindow( m_bPriceSetModDept & bEnableModifier );
	m_editDeptNo.EnableWindow( m_bPriceSetModDept & bEnableModifier );
	m_comboDeptName.EnableWindow( m_bPriceSetModDept & bEnableModifier );
	m_staticOPGroup.EnableWindow( m_bPriceSetModOPG & bEnableModifier );
	m_comboOPGroup.EnableWindow( m_bPriceSetModOPG & bEnableModifier );
	m_staticTaxBand.EnableWindow( m_bPriceSetModTaxBand & bEnableModifier );
	m_staticTaxBand.SetWindowText( EcrmanOptions.GetSalesTaxName() );
	m_comboTaxBand.EnableWindow( m_bPriceSetModTaxBand & bEnableModifier );
	m_staticCategory.EnableWindow( m_bPriceSetModCategory & bEnableModifier );
	m_comboCategory.EnableWindow( m_bPriceSetModCategory & bEnableModifier );
	m_staticPluType.EnableWindow( m_bPriceSetModPluType & bEnableModifier );
	m_comboPluType.EnableWindow( m_bPriceSetModPluType & bEnableModifier );
	
	m_staticFirstMenu.EnableWindow( m_bPriceSetModFirstMenu & bEnableModifier );
	m_editFirstMenu.EnableWindow( m_bPriceSetModFirstMenu & bEnableModifier );
	m_comboFirstMenu.EnableWindow( m_bPriceSetModFirstMenu & bEnableModifier );
	
	m_staticFirstQualifier.EnableWindow( m_bPriceSetModFirstQualifier & bEnableModifier );
	m_editFirstQualifier.EnableWindow( m_bPriceSetModFirstQualifier & bEnableModifier );
	m_comboFirstQualifier.EnableWindow( m_bPriceSetModFirstQualifier & bEnableModifier );
	
	m_staticFirstExtra.EnableWindow( m_bPriceSetModFirstExtra & bEnableModifier );
	m_editFirstExtra.EnableWindow( m_bPriceSetModFirstExtra & bEnableModifier );
	m_comboFirstExtra.EnableWindow( m_bPriceSetModFirstExtra & bEnableModifier );

	for ( int p = 0; p < 10; p++ )
	{
		m_staticModPrice[p].EnableWindow ( m_bPriceSetModPrice[p] & bEnableModifier );
		m_pEditModPrice[p] -> EnableWindow ( m_bPriceSetModPrice[p] & bEnableModifier );
	}
}

/**********************************************************************/

void CPropPagePluModifier::UpdateModifierControls()
{
	int nMod = m_nActiveTab + 1;

	m_editBaseEposText.SetWindowText( m_PluBuffer.GetEposText() );
	m_editBaseKeyText.SetWindowText( m_PluBuffer.GetKeyText() );
	
	m_editModEposText.SetWindowText( m_PluBuffer.GetModifierEposText( nMod ) );
	m_editModKeyText.SetWindowText( m_PluBuffer.GetModifierKeyText( nMod ) );
	
	for ( int p = 0; p < 10; p++ )
	{
		CString strPrice;
		strPrice.Format( "%.*f", SysInfo.GetDPValue(), m_PluBuffer.GetPrice(p) ); 
		m_pEditBasePrice[p] -> SetWindowText( strPrice );

		strPrice.Format( "%.*f", SysInfo.GetDPValue(), m_PluBuffer.GetModifierPrice ( nMod, p ) ); 
		m_pEditModPrice[p] -> SetWindowText( strPrice );
	}

	FillDepartmentCombo();
	FillOPGroupCombo();
	FillTaxCombo();
	FillCategoryCombo();
	FillPluTypeCombo();

	m_KbdListSelectorMenu.UpdateKbdListCombo( m_PluBuffer.GetModifierFirstMenu(nMod) );
	m_KbdListSelectorMenu.UpdateKbdListEditBox();
	m_nFirstMenu = m_KbdListSelectorMenu.GetKbdListNo();

	m_KbdListSelectorExtra.UpdateKbdListCombo( m_PluBuffer.GetModifierFirstExtra(nMod) );
	m_KbdListSelectorExtra.UpdateKbdListEditBox();
	m_nFirstExtra = m_KbdListSelectorExtra.GetKbdListNo();

	m_KbdListSelectorQualifier.UpdateKbdListCombo( m_PluBuffer.GetModifierFirstQualifier(nMod) );
	m_KbdListSelectorQualifier.UpdateKbdListEditBox();
	m_nFirstQualifier = m_KbdListSelectorQualifier.GetKbdListNo();
}

/**********************************************************************/
void CPropPagePluModifier::OnCopyBasePrice2(){ CopyBasePrice(2); }
void CPropPagePluModifier::OnCopyBasePrice3(){ CopyBasePrice(3); }
void CPropPagePluModifier::OnCopyBasePrice4(){ CopyBasePrice(4); }
void CPropPagePluModifier::OnCopyBasePrice5(){ CopyBasePrice(5); }
void CPropPagePluModifier::OnCopyBasePrice6(){ CopyBasePrice(6); }
void CPropPagePluModifier::OnCopyBasePrice7(){ CopyBasePrice(7); }
void CPropPagePluModifier::OnCopyBasePrice8(){ CopyBasePrice(8); }
void CPropPagePluModifier::OnCopyBasePrice9(){ CopyBasePrice(9); }
void CPropPagePluModifier::OnCopyBasePrice10(){ CopyBasePrice(10); }
/**********************************************************************/

void CPropPagePluModifier::CopyBasePrice( int p )
{
	CString str;
	GetEditBasePrice( 0 ) -> GetWindowText( str );
	GetEditBasePrice( p - 1 ) -> SetWindowText( str );
}

/**********************************************************************/
void CPropPagePluModifier::OnCopyModPrice2(){ CopyModPrice(2); }
void CPropPagePluModifier::OnCopyModPrice3(){ CopyModPrice(3); }
void CPropPagePluModifier::OnCopyModPrice4(){ CopyModPrice(4); }
void CPropPagePluModifier::OnCopyModPrice5(){ CopyModPrice(5); }
void CPropPagePluModifier::OnCopyModPrice6(){ CopyModPrice(6); }
void CPropPagePluModifier::OnCopyModPrice7(){ CopyModPrice(7); }
void CPropPagePluModifier::OnCopyModPrice8(){ CopyModPrice(8); }
void CPropPagePluModifier::OnCopyModPrice9(){ CopyModPrice(9); }
void CPropPagePluModifier::OnCopyModPrice10(){ CopyModPrice(10); }
/**********************************************************************/

void CPropPagePluModifier::CopyModPrice( int p )
{
	if ( ( GetEditModPrice( p - 1 ) -> IsWindowEnabled() == TRUE ) && ( GetEditModPrice( 0 ) -> IsWindowEnabled() == TRUE ) ) 
	{
		CString str;
		GetEditModPrice( 0 ) -> GetWindowText( str );
		GetEditModPrice( p - 1 ) -> SetWindowText( str );
	}
}

/**********************************************************************/
