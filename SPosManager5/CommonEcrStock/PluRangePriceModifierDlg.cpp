/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "MaxLengths.h"
#include "MessageLogger.h"
#include "PluCSVArray.h"
#include "PriceSetItemManager.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PluRangePriceModifierDlg.h"
/**********************************************************************/
static const int FIXED_ADJUST = 0;
static const int PERCENT_ADJUST = 1;
static const int SET_AT = 2;
static const int COPY_PRICE = 3;
/**********************************************************************/

CPluRangePriceModifierDlg::CPluRangePriceModifierDlg( CPluFilterArray& FilterArray, CWnd* pParent /*=NULL*/)
	: CSSDialog(CPluRangePriceModifierDlg::IDD, pParent), m_FilterArray ( FilterArray ),
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName )
{
	//{{AFX_DATA_INIT(CPluRangePriceModifierDlg)
	m_nMethod = 0;
	//}}AFX_DATA_INIT

	m_nActiveTab = 0;
	m_nModifierType = 0;

	for ( int nMod = 0; nMod < 10; nMod++ )
	{
		for ( int nPrice = 0; nPrice < 10; nPrice++ )
		{
			m_bPrice [ nMod ] [ nPrice ] = FALSE;
			m_dPrice [ nMod ] [ nPrice ] = 0.0;
		}
	}

	m_checkPriceID [ 0 ][ 0 ] = IDC_CHECK_PRICE1;
	m_checkPriceID [ 0 ][ 1 ] = IDC_CHECK_PRICE2;
	m_checkPriceID [ 0 ][ 2 ] = IDC_CHECK_PRICE3;
	m_checkPriceID [ 0 ][ 3 ] = IDC_CHECK_PRICE4;
	m_checkPriceID [ 0 ][ 4 ] = IDC_CHECK_PRICE5;
	m_checkPriceID [ 0 ][ 5 ] = IDC_CHECK_PRICE6;
	m_checkPriceID [ 0 ][ 6 ] = IDC_CHECK_PRICE7;
	m_checkPriceID [ 0 ][ 7 ] = IDC_CHECK_PRICE8;
	m_checkPriceID [ 0 ][ 8 ] = IDC_CHECK_PRICE9;
	m_checkPriceID [ 0 ][ 9 ] = IDC_CHECK_PRICE10;

	m_checkPriceID [ 1 ][ 0 ] = IDC_CHECK_PRICE1_MOD1;
	m_checkPriceID [ 1 ][ 1 ] = IDC_CHECK_PRICE2_MOD1;
	m_checkPriceID [ 1 ][ 2 ] = IDC_CHECK_PRICE3_MOD1;
	m_checkPriceID [ 1 ][ 3 ] = IDC_CHECK_PRICE4_MOD1;
	m_checkPriceID [ 1 ][ 4 ] = IDC_CHECK_PRICE5_MOD1;
	m_checkPriceID [ 1 ][ 5 ] = IDC_CHECK_PRICE6_MOD1;
	m_checkPriceID [ 1 ][ 6 ] = IDC_CHECK_PRICE7_MOD1;
	m_checkPriceID [ 1 ][ 7 ] = IDC_CHECK_PRICE8_MOD1;
	m_checkPriceID [ 1 ][ 8 ] = IDC_CHECK_PRICE9_MOD1;
	m_checkPriceID [ 1 ][ 9 ] = IDC_CHECK_PRICE10_MOD1;

	m_editPriceID [ 0 ][ 0 ] = IDC_EDIT_PRICE1;
	m_editPriceID [ 0 ][ 1 ] = IDC_EDIT_PRICE2;
	m_editPriceID [ 0 ][ 2 ] = IDC_EDIT_PRICE3;
	m_editPriceID [ 0 ][ 3 ] = IDC_EDIT_PRICE4;
	m_editPriceID [ 0 ][ 4 ] = IDC_EDIT_PRICE5;
	m_editPriceID [ 0 ][ 5 ] = IDC_EDIT_PRICE6;
	m_editPriceID [ 0 ][ 6 ] = IDC_EDIT_PRICE7;
	m_editPriceID [ 0 ][ 7 ] = IDC_EDIT_PRICE8;
	m_editPriceID [ 0 ][ 8 ] = IDC_EDIT_PRICE9;
	m_editPriceID [ 0 ][ 9 ] = IDC_EDIT_PRICE10;

	m_editPriceID [ 1 ][ 0 ] = IDC_EDIT_PRICE1_MOD1;
	m_editPriceID [ 1 ][ 1 ] = IDC_EDIT_PRICE2_MOD1;
	m_editPriceID [ 1 ][ 2 ] = IDC_EDIT_PRICE3_MOD1;
	m_editPriceID [ 1 ][ 3 ] = IDC_EDIT_PRICE4_MOD1;
	m_editPriceID [ 1 ][ 4 ] = IDC_EDIT_PRICE5_MOD1;
	m_editPriceID [ 1 ][ 5 ] = IDC_EDIT_PRICE6_MOD1;
	m_editPriceID [ 1 ][ 6 ] = IDC_EDIT_PRICE7_MOD1;
	m_editPriceID [ 1 ][ 7 ] = IDC_EDIT_PRICE8_MOD1;
	m_editPriceID [ 1 ][ 8 ] = IDC_EDIT_PRICE9_MOD1;
	m_editPriceID [ 1 ][ 9 ] = IDC_EDIT_PRICE10_MOD1;

	m_nCopyLevel = 1;
	m_nMethod = FIXED_ADJUST;
}

/**********************************************************************/

void CPluRangePriceModifierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluRangePriceModifierDlg)
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Control(pDX, IDC_COMBO_MODIFIER_ENABLE, m_comboModifierEnable);
	DDX_Control(pDX, IDC_TAB_MODIFIERS, m_TabModifiers);
	DDX_Control(pDX, IDC_STATIC_MODIFIER, m_staticModifier);
	DDX_Radio(pDX, IDC_RADIO_MODIFIER1, m_nModifierType);
	DDX_Radio(pDX, IDC_RADIO_FIXED, m_nMethod);
	//}}AFX_DATA_MAP

	int nMod = m_nActiveTab + 1;
	if ( ( nMod < 1 ) || ( nMod > 9 ) )
	{
		nMod = 1;
	}
	
	for ( int n = 0; n < 2; n++ )
	{
		int nArrayMod = ( 0 == n ) ? 0 : nMod;
		
		for ( int nPrice = 0; nPrice < 10; nPrice++ )
		{
			DDX_Control(pDX, m_checkPriceID[n][nPrice], m_checkPrice[n][nPrice]);
			DDX_Check(pDX, m_checkPriceID[n][nPrice], m_bPrice[nArrayMod][nPrice]);
			DDX_Text(pDX, m_editPriceID[n][nPrice], m_dPrice[nArrayMod][nPrice] );	
		}
	}

	CString strMaxBase = "";
	::AddLeading( strMaxBase, SysInfo.GetMaxBasePluLen(), '9' );
	
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_START, m_strPluFrom);
	DDV_IntegerString(pDX, m_strPluFrom, "1", strMaxBase );
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_END, m_strPluTo);
	DDV_IntegerString(pDX, m_strPluTo, "1", strMaxBase );
	DDX_Text(pDX, IDC_EDIT_LEVEL, m_nCopyLevel );
	DDV_MinMaxInt( pDX, m_nCopyLevel, 1, MAX_PLU_PRICES );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluRangePriceModifierDlg, CDialog)
	//{{AFX_MSG_MAP(CPluRangePriceModifierDlg)
	
	ON_BN_CLICKED(IDC_STATIC_RANGE, OnStaticRange)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_START, OnKillFocusPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_END, OnKillFocusPluNo)
	
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNumber)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelchangeComboDeptname)
	
	ON_BN_CLICKED(IDC_STATIC_PRICE1, OnToggleBasePlu)
	ON_BN_CLICKED(IDC_CHECK_PRICE1, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE2, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE3, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE4, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE5, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE6, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE7, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE8, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE9, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE10, OnCheckPrice)
	
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MODIFIERS, OnTcnSelchangeTabs)
	
	ON_BN_CLICKED(IDC_STATIC_PRICE1_MOD1, OnToggleModifier)
	ON_BN_CLICKED(IDC_CHECK_PRICE1_MOD1, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE2_MOD1, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE3_MOD1, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE4_MOD1, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE5_MOD1, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE6_MOD1, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE7_MOD1, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE8_MOD1, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE9_MOD1, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE10_MOD1, OnCheckPrice)
		
	ON_BN_CLICKED(IDC_RADIO_COPY, OnRadioCopy)
	ON_BN_CLICKED(IDC_RADIO_FIXED, OnRadioFixed)
	ON_BN_CLICKED(IDC_RADIO_PERCENT, OnRadioPercent)
	ON_BN_CLICKED(IDC_RADIO_SETAT, OnRadioSetAt)
	
	ON_EN_KILLFOCUS(IDC_EDIT_LEVEL, OnKillFocusLevel)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPluRangePriceModifierDlg::GetEditPluFrom() { return GetEdit( IDC_EDIT_PLU_RANGE_START ); }
CEdit* CPluRangePriceModifierDlg::GetEditPluTo() { return GetEdit( IDC_EDIT_PLU_RANGE_END ); }
CEdit* CPluRangePriceModifierDlg::GetEditCopyLevel() { return GetEdit( IDC_EDIT_LEVEL ); }
CEdit* CPluRangePriceModifierDlg::GetEditModifier( int nMod, int nLevel ) { return GetEdit( m_editPriceID[nMod][nLevel] ); }
/**********************************************************************/

BOOL CPluRangePriceModifierDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SubclassEdit ( IDC_EDIT_PLU_RANGE_START, SS_NUM, SysInfo.GetMaxBasePluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_RANGE_END, SS_NUM, SysInfo.GetMaxBasePluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_LEVEL, SS_NUM, 2, "%d" );

	for ( int nMod = 0; nMod < 2; nMod++ )
	{
		for ( int nPrice = 0; nPrice < 10; nPrice++ )
		{
			SubclassEdit ( m_editPriceID[nMod][nPrice], SS_NUM_SDP, Super.MaxPriceLen(), "%.*f", SysInfo.GetDPValue() );
		}
	}

	for ( int nMod = 1; nMod <= 9; nMod++ )
	{	
		m_TabModifiers.InsertItem ( nMod - 1, DataManager.Modifier.GetDisplayName( nMod ) );
	}

	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_FilterArray.GetDepartmentFilter(), DEPARTMENT_SELECTOR_NORMAL );

	if ( m_FilterArray.GetDepartmentFilter() != DEPARTMENTSET_ALL )
	{
		m_comboDeptName.EnableWindow ( FALSE );
		m_editDeptNo.EnableWindow ( FALSE );
	}

	CAnalysisCategoryCSVArray::PrepareFilterCombo(m_comboCategory);
	
	if ( PriceSetItemManager.IsPluMaster() == TRUE )
	{
		for ( int nMod = 0; nMod < 10; nMod++ )
		{
			m_bAllowModEnable[ nMod ] = TRUE;
			m_bAllowModTextEpos[ nMod ] = TRUE;
			m_bAllowModTextKey[ nMod ] = TRUE;

			for ( int nPrice = 0; nPrice < 10; nPrice++ )
				m_bAllowPrice [ nMod ][ nPrice ] = TRUE;
		}
	}
	else
	{
		for ( int nPrice = 0; nPrice < 10; nPrice++ )
		{
			CString strField;
			CPluCSVRecord::GetPriceLabel( nPrice + 1, strField );
			m_bAllowPrice [ 0 ][ nPrice ] = PriceSetItemManager.CheckField ( strField );
		}

		for ( int nMod = 1; nMod < 10; nMod++ )
		{
			CString strField;

			CPluCSVRecord::GetModifierLabelEnable( nMod, FALSE, strField );
			m_bAllowModEnable[ nMod ] = PriceSetItemManager.CheckField( strField );

			CPluCSVRecord::GetModifierLabelEposTextFlag( nMod, FALSE, strField );
			m_bAllowModTextEpos[ nMod ] = PriceSetItemManager.CheckField( strField );

			CPluCSVRecord::GetModifierLabelKeyTextFlag( nMod, FALSE, strField );
			m_bAllowModTextKey[ nMod ] = PriceSetItemManager.CheckField( strField );

			for ( int nModPrice = 0; nModPrice < 10; nModPrice++ )
			{
				CPluCSVRecord::GetModifierLabelPrice( nMod, nModPrice + 1, FALSE, strField );
				m_bAllowPrice[ nMod ][ nModPrice ] = PriceSetItemManager.CheckField ( strField );
			}
		} 
	}

	m_comboModifierEnable.AddString( "No Change" );
	m_comboModifierEnable.AddString( "Enable Selected" );
	m_comboModifierEnable.AddString( "Disable Selected" );
	m_comboModifierEnable.SetCurSel(0);

	CString strModifier;
	strModifier.Format( "Modifier 1 : %ss", DataManager.Modifier.GetDisplayName(1) );
	m_staticModifier.SetWindowText( strModifier );

	SetPriceEnables();
	return TRUE; 
}

/**********************************************************************/

void CPluRangePriceModifierDlg::OnStaticRange() 
{
	CString strStart = "1";
	::AddLeading ( strStart, SysInfo.GetMaxBasePluLen(), '0' );
	GetEditPluFrom() -> SetWindowText ( strStart );
	
	CString strEnd = "9";
	::AddLeading ( strEnd, SysInfo.GetMaxBasePluLen(), '9' );
	GetEditPluTo() -> SetWindowText( strEnd );
}

/**********************************************************************/

void CPluRangePriceModifierDlg::OnKillFocusPluNo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluFrom = BarcodePluNoTable.ConvertShortPlu ( m_strPluFrom );
	m_strPluTo = BarcodePluNoTable.ConvertShortPlu ( m_strPluTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPluRangePriceModifierDlg::OnKillFocusDeptNumber() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CPluRangePriceModifierDlg::OnSelchangeComboDeptname() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/
void CPluRangePriceModifierDlg::OnToggleBasePlu()	{ ToggleModifier(0); }
void CPluRangePriceModifierDlg::OnToggleModifier()	{ ToggleModifier(1); }
/**********************************************************************/

void CPluRangePriceModifierDlg::ToggleModifier( int nMod )
{
	bool bGotEnabledTick = FALSE;
	bool bNewTickState = FALSE;
	
	for ( int nPrice = 0; nPrice < 10; nPrice++ )
	{
		if ( m_checkPrice[nMod][nPrice].IsWindowEnabled()  )
		{
			bGotEnabledTick = TRUE;
			if ( m_checkPrice[nMod][nPrice].GetCheck() == FALSE )
				bNewTickState = TRUE;
		}
	}

	if ( bGotEnabledTick == FALSE )
		return;

	for ( int nPrice = 0; nPrice < 10; nPrice++ )
	{
		if ( m_checkPrice[nMod][nPrice].IsWindowEnabled()  )
			m_checkPrice[nMod][nPrice].SetCheck(bNewTickState);
	}

	OnCheckPrice();
}

/**********************************************************************/

void CPluRangePriceModifierDlg::OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	UpdateData( TRUE, FALSE );

	m_nActiveTab = m_TabModifiers.GetCurSel();
	if ( ( m_nActiveTab < 0 ) || ( m_nActiveTab > 8 ) )
	{
		m_nActiveTab = 0;
	}

	CString strModifier;
	strModifier.Format( "Modifier %d : %s", 
		m_nActiveTab + 1,
		DataManager.Modifier.GetDisplayName( m_nActiveTab + 1 ) );

	m_staticModifier.SetWindowText( strModifier );

	UpdateData( FALSE );
	SetPriceEnables();
}

/**********************************************************************/

void CPluRangePriceModifierDlg::SetPriceEnables()
{	
	int nMod = m_nActiveTab + 1;
	if ( ( nMod < 1 ) || ( nMod > 9 ) )
	{
		nMod = 1;
	}

	for ( int n = 0; n < 2; n++ )
	{
		int nArrayMod = ( 0 == n ) ? 0 : nMod;

		for ( int nPrice = 0; nPrice < 10; nPrice++ )
		{
			bool bTicked = ( m_checkPrice[n][nPrice].GetCheck() != 0 );
			bool bAllowed = m_bAllowPrice [nArrayMod][nPrice];
			m_checkPrice[n][nPrice].EnableWindow ( bAllowed );
			GetEditModifier( n, nPrice ) -> EnableWindow ( bTicked && bAllowed && m_nMethod != COPY_PRICE );
		}
	}

	GetEditCopyLevel() -> EnableWindow ( m_nMethod == COPY_PRICE  );
}

/**********************************************************************/

void CPluRangePriceModifierDlg::OnKillFocusLevel() 
{
	UpdateData( TRUE, FALSE );
	m_nMethod = COPY_PRICE;
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPluRangePriceModifierDlg::OnCheckPrice() 
{
	SetPriceEnables();
}

/**********************************************************************/

void CPluRangePriceModifierDlg::OnRadioCopy() 
{
	m_nMethod = COPY_PRICE;
	SetPriceEnables();	
}

/**********************************************************************/

void CPluRangePriceModifierDlg::OnRadioFixed() 
{
	m_nMethod = FIXED_ADJUST;
	SetPriceEnables();
}

/**********************************************************************/

void CPluRangePriceModifierDlg::OnRadioPercent() 
{
	m_nMethod = PERCENT_ADJUST;
	SetPriceEnables();	
}

/**********************************************************************/

void CPluRangePriceModifierDlg::OnRadioSetAt() 
{
	m_nMethod = SET_AT;
	SetPriceEnables();
}

/**********************************************************************/

bool CPluRangePriceModifierDlg::IsRelevant()
{
	if ( PriceSetItemManager.IsPluMaster() == TRUE ) return TRUE;

	//NO POINT ENABLING THIS TO JUST DO THE BASIC PRICES
	//AS THE PRICE BY RANGE DIALOG WILL SUFFICE FOR THAT
	CString strField = "";
	for ( int nMod = 1; nMod < 10; nMod++ )
	{
		for ( int nPrice = 0; nPrice < 10; nPrice++ )
		{
			CPluCSVRecord::GetModifierLabelPrice( nMod, nPrice + 1, FALSE, strField );
			if ( PriceSetItemManager.CheckField( strField ) ) return TRUE;
		}
	}
		
	return FALSE;
}

/**********************************************************************/

int CPluRangePriceModifierDlg::UpdatePrices()
{
	int nStartIdx, nRange;
	__int64 nPluFrom = _atoi64( m_strPluFrom );
	__int64 nPluTo = _atoi64( m_strPluTo );
	m_FilterArray.GetArrayIdxRange( nPluFrom, nPluTo, nStartIdx, nRange );

	CDepartmentSetCSVRecord DepartmentSet;
	DepartmentSet.CreateDepartmentSet ( m_DepartmentSelector.GetDepartmentFilter() );

	int nCategory = CAnalysisCategoryCSVArray::GetFilterFromCombo(m_comboCategory);

	int nCount = 0;
	for ( int nOffset = 0; nOffset < nRange; nOffset++ )
	{
		StatusProgress.SetPos ( nOffset, nRange );  
			
		CPluCSVRecord NewRecord;
		int nPluIdx = m_FilterArray.GetPluItemIndexAt ( nStartIdx + nOffset );
		DataManager.Plu.GetAt ( nPluIdx, NewRecord );

		__int64 nPluNo = NewRecord.GetPluNoInt(); 
		if ( ( nPluNo < nPluFrom ) || ( nPluNo > nPluTo ) )
			continue;

		//skip plus with fixed profit
		if ( NewRecord.GetGPType() != PLU_GPTYPE_VARIABLE )	
			continue;

		//cannot change prices for database record in price set
		if ( PriceSetItemManager.IsPluMaster() == FALSE )
			if ( PriceSetItemManager.GetPluStatus( nPluIdx ) != LOCALPLU_LOCAL )
				continue;
	
		for ( int nMod = 0; nMod < 10; nMod++ )
		{
			if ( DepartmentSet.GetDepartmentStatus ( NewRecord.GetModifierDeptNoForReport(nMod) ) == FALSE )
				continue;

			if ((nCategory != 9999) && (nCategory != NewRecord.GetModifierAnalysisCategoryForReport(nMod)))
				continue;

			bool bCanChangeModifierSettings = FALSE;
			
			for ( int nPrice = 0; nPrice < 10; nPrice++ )
			{
				bool bTicked = ( m_bPrice [ nMod ][ nPrice ] != 0 );
				bool bAllowed = m_bAllowPrice [ nMod ][ nPrice ];
				double dValue = m_dPrice[ nMod ][ nPrice ];

				if ( ( bTicked && bAllowed ) == FALSE )
					continue;

				bCanChangeModifierSettings = TRUE;

				switch ( m_nMethod )
				{
				case SET_AT:
					if ( dValue >= 0.0 ) NewRecord.SetModifierPrice ( nMod, nPrice, dValue );
					break;

				case FIXED_ADJUST:
					{
						double dNewPrice;
						if ( nMod == 0 || m_nModifierType != 1 )
							dNewPrice = NewRecord.GetModifierPrice ( nMod, nPrice ) + dValue;
						else
							dNewPrice = NewRecord.GetPrice( nPrice ) + dValue;

						if ( dNewPrice >= 0.0 ) NewRecord.SetModifierPrice ( nMod, nPrice, dNewPrice );
					}
					break;
						
				case PERCENT_ADJUST:
					{
						double dChange, dNewPrice;
						if ( nMod == 0 || m_nModifierType != 1 )
						{
							dChange = ( NewRecord.GetModifierPrice ( nMod, nPrice ) * dValue ) / 100.0;
							dNewPrice = NewRecord.GetModifierPrice ( nMod, nPrice ) + dChange;
						}
						else
						{
							dChange = ( NewRecord.GetPrice ( nPrice ) * dValue ) / 100.0;
							dNewPrice = NewRecord.GetPrice ( nPrice ) + dChange;
						}

						if ( dNewPrice >= 0.0 ) NewRecord.SetModifierPrice ( nMod, nPrice, dNewPrice );
					}
					break;

				case COPY_PRICE:
					if ( nMod == 0 || m_nModifierType != 1 )
						NewRecord.SetModifierPrice ( nMod, nPrice, NewRecord.GetModifierPrice ( nMod, m_nCopyLevel - 1 ) );
					else
						NewRecord.SetModifierPrice ( nMod, nPrice, NewRecord.GetPrice ( m_nCopyLevel - 1 ) );
					break;
				}
			}

			if ( TRUE == bCanChangeModifierSettings )
			{
				if ( TRUE == m_bAllowModEnable[ nMod ] )
				{
					switch( m_comboModifierEnable.GetCurSel() )
					{
					case 1:		NewRecord.SetModifierEnable( nMod, TRUE );	break;
					case 2:		NewRecord.SetModifierEnable( nMod, FALSE );	break;
					}
				}
			}

		}
		if ( DataManager.Plu.SavePluRecord ( nPluIdx, NewRecord ) != 0 )
			nCount++;
	}
	
	return nCount;
}

/**********************************************************************/

void CPluRangePriceModifierDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	OnKillFocusDeptNumber();
	OnKillFocusPluNo();

	StatusProgress.Lock( TRUE, "Updating PLU modifiers" );
	int nCount = UpdatePrices();
	StatusProgress.Unlock();
	
	CString strBuf;
	switch ( nCount )
	{
	case 0:
		Prompter.Info ( "No records were changed" );
		break;

	case 1:
		Prompter.Info ( "1 record was changed" );
		break;

	default:
		strBuf.Format ( "%d records were changed", nCount );
		Prompter.Info ( strBuf );
		break;
	}

	CDWordArray dw;
	dw.Add( nCount );
	MessageLogger.LogSystem( 2, 6, FALSE, dw );
		
	EndDialog ( IDOK );
}

/**********************************************************************/
