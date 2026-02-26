//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************
#include "BatchPropertyPage2.h"
//$$******************************************************************

CBatchPropertyPage2::CBatchPropertyPage2( CBatchAccountBalanceMemory& Memory)
	: CSSPropertyPage(CBatchPropertyPage2::IDD), m_Memory(Memory)
{
}

//$$******************************************************************

void CBatchPropertyPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ADDPURSE, m_checkAddPurse1);
	DDX_Control(pDX, IDC_CHECK_ADDPURSE2, m_checkAddPurse2);
	DDX_Control(pDX, IDC_CHECK_ADDPOINTS, m_checkAddPoints);
	DDX_Control(pDX, IDC_CHECK_ADDPURSE1SPENDTD, m_checkAddPurse1SpendTD);
	DDX_Control(pDX, IDC_CHECK_ADDPURSE2SPENDTD, m_checkAddPurse2SpendTD);
	DDX_Control(pDX, IDC_CHECK_ADDCASHSPENDTD, m_checkAddCashSpendTD);
	DDX_Control(pDX, IDC_CHECK_ADDPOINTSTD, m_checkAddPointsTD);	
	DDX_Control(pDX, IDC_CHECK_CLEARPURSE1, m_checkClearPurse1);
	DDX_Control(pDX, IDC_CHECK_CLEARPURSE2, m_checkClearPurse2);
	DDX_Control(pDX, IDC_CHECK_CLEARPOINTS, m_checkClearPoints);
	DDX_Control(pDX, IDC_CHECK_CLEARPURSE1SPENDTD, m_checkClearPurse1SpendTD);
	DDX_Control(pDX, IDC_CHECK_CLEARPURSE2SPENDTD, m_checkClearPurse2SpendTD);
	DDX_Control(pDX, IDC_CHECK_CLEARCASHSPENDTD, m_checkClearCashSpendTD);
	DDX_Control(pDX, IDC_CHECK_CLEARPOINTSTD, m_checkClearPointsTD);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CBatchPropertyPage2, CPropertyPage)
	ON_EN_KILLFOCUS(IDC_EDIT_POINTS, OnKillFocusEditPoints)
	ON_BN_CLICKED(IDC_CHECK_ADDPURSE, OnToggleAddPurse1)
	ON_BN_CLICKED(IDC_CHECK_ADDPURSE2, OnToggleAddPurse2)
	ON_BN_CLICKED(IDC_CHECK_ADDPOINTS, OnToggleAddPoints)
	ON_BN_CLICKED(IDC_CHECK_ADDPURSE1SPENDTD, OnToggleAddPurse1SpendTD)
	ON_BN_CLICKED(IDC_CHECK_ADDPURSE2SPENDTD, OnToggleAddPurse2SpendTD)
	ON_BN_CLICKED(IDC_CHECK_ADDCASHSPENDTD, OnToggleAddCashSpendTD)
	ON_BN_CLICKED(IDC_CHECK_ADDPOINTSTD, OnToggleAddPointsTD)
	ON_BN_CLICKED(IDC_CHECK_CLEARPURSE1, OnToggleClearPurse1)
	ON_BN_CLICKED(IDC_CHECK_CLEARPURSE2, OnToggleClearPurse2)
	ON_BN_CLICKED(IDC_CHECK_CLEARPOINTS, OnToggleClearPoints)
	ON_BN_CLICKED(IDC_CHECK_CLEARPURSE1SPENDTD, OnToggleClearPurse1SpendTD)
	ON_BN_CLICKED(IDC_CHECK_CLEARPURSE2SPENDTD, OnToggleClearPurse2SpendTD)
	ON_BN_CLICKED(IDC_CHECK_CLEARCASHSPENDTD, OnToggleClearCashSpendTD)
	ON_BN_CLICKED(IDC_CHECK_CLEARPOINTSTD, OnToggleClearPointsTD)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CBatchPropertyPage2::OnInitDialog()
{
	CSSPropertyPage::OnInitDialog();

	m_Memory.TidyOptions();

	m_checkAddPurse1.SetCheck( m_Memory.m_bAddPurse1 );
	m_checkAddPurse2.SetCheck( m_Memory.m_bAddPurse2 );
	m_checkAddPoints.SetCheck( m_Memory.m_bAddPoints );
	m_checkAddPurse1SpendTD.SetCheck( m_Memory.m_bAddPurse1SpendTD );
	m_checkAddPurse2SpendTD.SetCheck( m_Memory.m_bAddPurse2SpendTD );
	m_checkAddCashSpendTD.SetCheck( m_Memory.m_bAddCashSpendTD );
	m_checkAddPointsTD.SetCheck( m_Memory.m_bAddPointsTD );
	
	m_checkClearPurse1.SetCheck( m_Memory.m_bClearPurse1 );
	m_checkClearPurse2.SetCheck( m_Memory.m_bClearPurse2 );
	m_checkClearPoints.SetCheck( m_Memory.m_bClearPoints );
	m_checkClearPurse1SpendTD.SetCheck( m_Memory.m_bClearPurse1SpendTD );
	m_checkClearPurse2SpendTD.SetCheck( m_Memory.m_bClearPurse2SpendTD );
	m_checkClearCashSpendTD.SetCheck( m_Memory.m_bClearCashSpendTD );
	m_checkClearPointsTD.SetCheck( m_Memory.m_bClearPointsTD );

	SubclassEdit ( IDC_EDIT_PURSE,			SS_NUM_SDP, 11, "%.2f" ); 
	SubclassEdit ( IDC_EDIT_PURSE2,			SS_NUM_SDP, 11, "%.2f" ); 
	SubclassEdit ( IDC_EDIT_POINTS,			SS_NUM_S,	 8, "%ld" );
	SubclassEdit ( IDC_EDIT_PURSE1SPENDTD,	SS_NUM_SDP, 11, "%.2f" );
	SubclassEdit ( IDC_EDIT_PURSE2SPENDTD,	SS_NUM_SDP, 11, "%.2f" );
	SubclassEdit ( IDC_EDIT_CASHSPENDTD,	SS_NUM_SDP, 11, "%.2f" );
	SubclassEdit ( IDC_EDIT_POINTSTD,		SS_NUM_S,	 8, "%ld" );

	SetEditDouble( GetEditPurse1(), m_Memory.m_dPurse1 );
	SetEditDouble( GetEditPurse2(), m_Memory.m_dPurse2 );
	SetEditInt( GetEditPoints(), m_Memory.m_nPoints );
	SetEditDouble( GetEditPurse1SpendTD(), m_Memory.m_dPurse1SpendTD );
	SetEditDouble( GetEditPurse2SpendTD(), m_Memory.m_dPurse2SpendTD );
	SetEditDouble( GetEditCashSpendTD(), m_Memory.m_dCashSpendTD );
	SetEditInt( GetEditPointsTD(), m_Memory.m_nPointsTD );
	
	bool bAllowPurse1 = System.GetBatchUpdatePurse1Flag();
	bool bAllowPurse2 = System.GetEnablePurse2Flag() && System.GetBatchUpdatePurse2Flag();
	bool bAllowPoints = System.GetBatchUpdatePointsFlag();
	bool bAllowToDate = System.GetBatchUpdateSpendTDFlag();

// points

	m_checkAddPoints.EnableWindow(bAllowPoints);
	m_checkClearPoints.EnableWindow(bAllowPoints);

// purse1

	m_checkAddPurse1.EnableWindow(bAllowPurse1);
	m_checkClearPurse1.EnableWindow(bAllowPurse1);

// purse2

	m_checkAddPurse2.EnableWindow(bAllowPurse2);
	m_checkClearPurse2.EnableWindow(bAllowPurse2);

// SpendTD
	m_checkAddPointsTD.EnableWindow( ( bAllowToDate | m_Memory.m_bAddPoints ) & bAllowPoints );
	m_checkClearPointsTD.EnableWindow( bAllowPoints & bAllowToDate );

	m_checkAddPurse1SpendTD.EnableWindow( bAllowToDate & bAllowPurse1 );
	m_checkClearPurse1SpendTD.EnableWindow(  bAllowToDate & bAllowPurse1 );

	m_checkAddPurse2SpendTD.EnableWindow( bAllowToDate & bAllowPurse2 );
	m_checkClearPurse2SpendTD.EnableWindow( bAllowToDate & bAllowPurse2 );

	m_checkAddCashSpendTD.EnableWindow( bAllowToDate );
	m_checkClearCashSpendTD.EnableWindow( bAllowToDate );

	GetEditPurse1() -> EnableWindow( m_Memory.m_bAddPurse1 );
	GetEditPurse2() -> EnableWindow( m_Memory.m_bAddPurse2 );
	GetEditPoints() -> EnableWindow( m_Memory.m_bAddPoints );
	GetEditPurse1SpendTD() -> EnableWindow( m_Memory.m_bAddPurse1SpendTD );
	GetEditPurse2SpendTD() -> EnableWindow( m_Memory.m_bAddPurse2SpendTD );
	GetEditCashSpendTD() -> EnableWindow( m_Memory.m_bAddCashSpendTD );
	GetEditPointsTD() -> EnableWindow( m_Memory.m_bAddPointsTD & bAllowToDate );

	return TRUE;  
}

//$$******************************************************************
CEdit* CBatchPropertyPage2::GetEditPurse1(){ return GetEdit( IDC_EDIT_PURSE ); }
CEdit* CBatchPropertyPage2::GetEditPurse2(){ return GetEdit( IDC_EDIT_PURSE2 ); }
CEdit* CBatchPropertyPage2::GetEditPoints(){ return GetEdit( IDC_EDIT_POINTS ); }
CEdit* CBatchPropertyPage2::GetEditPurse1SpendTD(){ return GetEdit( IDC_EDIT_PURSE1SPENDTD ); }
CEdit* CBatchPropertyPage2::GetEditPurse2SpendTD(){ return GetEdit( IDC_EDIT_PURSE2SPENDTD ); }
CEdit* CBatchPropertyPage2::GetEditCashSpendTD(){ return GetEdit( IDC_EDIT_CASHSPENDTD ); }
CEdit* CBatchPropertyPage2::GetEditPointsTD(){ return GetEdit( IDC_EDIT_POINTSTD ); }
//$$******************************************************************

void CBatchPropertyPage2::SetEditDouble( CEdit* pEdit, double d )
{
	CString str;
	str.Format( "%.2f", d );
	pEdit -> SetWindowText( str );
}

//$$******************************************************************

void CBatchPropertyPage2::SetEditInt( CEdit* pEdit, int n )
{
	CString str;
	str.Format( "%d", n );
	pEdit -> SetWindowText( str );
}

//$$******************************************************************

double CBatchPropertyPage2::GetEditDouble( CEdit* pEdit )
{
	CString str;
	pEdit -> GetWindowText( str );
	return atof( str );
}

//$$******************************************************************

int CBatchPropertyPage2::GetEditInt( CEdit* pEdit )
{
	CString str;
	pEdit -> GetWindowText( str );
	return atoi( str );
}

//$$******************************************************************

void CBatchPropertyPage2::UpdateMemory()
{
	m_Memory.m_bAddPurse1 = ( m_checkAddPurse1.GetCheck() != 0 );
	m_Memory.m_bAddPurse2 = ( m_checkAddPurse2.GetCheck() != 0 );
	m_Memory.m_bAddPoints = ( m_checkAddPoints.GetCheck() != 0 );
	m_Memory.m_bAddPurse1SpendTD = ( m_checkAddPurse1SpendTD.GetCheck() != 0 );
	m_Memory.m_bAddPurse2SpendTD = ( m_checkAddPurse2SpendTD.GetCheck() != 0 );
	m_Memory.m_bAddCashSpendTD = ( m_checkAddCashSpendTD.GetCheck() != 0 );
	m_Memory.m_bAddPointsTD = ( m_checkAddPointsTD.GetCheck() != 0 );
	m_Memory.m_bClearPurse1 = ( m_checkClearPurse1.GetCheck() != 0 );
	m_Memory.m_bClearPurse2 = ( m_checkClearPurse2.GetCheck() != 0 );
	m_Memory.m_bClearPoints = ( m_checkClearPoints.GetCheck() != 0 );
	m_Memory.m_bClearPurse1SpendTD = ( m_checkClearPurse1SpendTD.GetCheck() != 0 );
	m_Memory.m_bClearPurse2SpendTD = ( m_checkClearPurse2SpendTD.GetCheck() != 0 );
	m_Memory.m_bClearCashSpendTD = ( m_checkClearCashSpendTD.GetCheck() != 0 );
	m_Memory.m_bClearPointsTD = ( m_checkClearPointsTD.GetCheck() != 0 );
	m_Memory.m_dPurse1 = GetEditDouble( GetEditPurse1() );
	m_Memory.m_dPurse2 = GetEditDouble( GetEditPurse2() );
	m_Memory.m_nPoints = GetEditInt( GetEditPoints() );
	m_Memory.m_dPurse1SpendTD = GetEditDouble( GetEditPurse1SpendTD() );
	m_Memory.m_dPurse2SpendTD = GetEditDouble( GetEditPurse2SpendTD() );
	m_Memory.m_dCashSpendTD = GetEditDouble( GetEditCashSpendTD() );
	m_Memory.m_nPointsTD = GetEditInt( GetEditPointsTD() );
	m_Memory.TidyOptions();
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleAddPurse1()
{
	m_checkClearPurse1.SetCheck(FALSE);
	GetEditPurse1()->EnableWindow( m_checkAddPurse1.GetCheck() != 0 );
	GetEditPurse1()->SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleClearPurse1()
{
	m_checkAddPurse1.SetCheck( FALSE );
	SetEditDouble( GetEditPurse1(), 0.0 );
	GetEditPurse1()->EnableWindow(FALSE);
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleAddPurse2()
{
	m_checkClearPurse2.SetCheck(FALSE);
	GetEditPurse2()->EnableWindow(m_checkAddPurse2.GetCheck() != 0 );
	GetEditPurse2()->SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleClearPurse2()
{
	m_checkAddPurse2.SetCheck( FALSE );
	SetEditDouble( GetEditPurse2(), 0.0 );
	GetEditPurse2()->EnableWindow(FALSE);
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleAddPoints()
{
	bool bToggleOn = (m_checkAddPoints.GetCheck() != 0);

	GetEditPoints()->EnableWindow(bToggleOn);

	if (FALSE == System.GetBatchUpdateSpendTDFlag())
	{
		if (TRUE == bToggleOn)
		{
			m_checkAddPointsTD.EnableWindow(TRUE);
			m_checkAddPointsTD.SetCheck(TRUE);
			SetEditInt(GetEditPointsTD(), GetEditInt(GetEditPoints()));
		}
		else
		{
			m_checkAddPointsTD.EnableWindow(FALSE);
			m_checkAddPointsTD.SetCheck(FALSE);
			SetEditInt(GetEditPointsTD(), 0);
		}
	}
	else
	{
		if (TRUE == bToggleOn)
		{
			m_checkAddPointsTD.SetCheck(TRUE);
			m_checkClearPointsTD.SetCheck(FALSE);
			SetEditInt(GetEditPointsTD(), GetEditInt(GetEditPoints()));
			GetEditPointsTD()->EnableWindow(TRUE);
		}
		else
		{
			m_checkAddPointsTD.SetCheck(FALSE);
			m_checkClearPointsTD.SetCheck(FALSE);
			SetEditInt(GetEditPointsTD(), GetEditInt(GetEditPoints()));
			GetEditPointsTD()->EnableWindow(FALSE);
		}
	}

	m_checkClearPoints.SetCheck(FALSE);
	GetEditPoints()->SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleClearPoints()
{
	if ( System.GetBatchUpdateSpendTDFlag() == FALSE )
	{
		m_checkAddPointsTD.SetCheck(FALSE);
		m_checkAddPointsTD.EnableWindow(FALSE);
		SetEditInt( GetEditPointsTD(), 0 );
	}

	m_checkAddPoints.SetCheck( FALSE );
	SetEditInt( GetEditPoints(), 0 );
	GetEditPoints()->EnableWindow(FALSE);
}

//$$******************************************************************

void CBatchPropertyPage2::OnKillFocusEditPoints()
{
	SetEditInt( GetEditPointsTD(), GetEditInt( GetEditPoints() ) );
	m_checkAddPointsTD.SetCheck(TRUE);
	GetEditPointsTD() -> EnableWindow( System.GetBatchUpdateSpendTDFlag() );	
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleAddPointsTD()
{
	SetEditInt(GetEditPointsTD(), GetEditInt(GetEditPoints()));

	if (System.GetBatchUpdateSpendTDFlag() == TRUE)
	{
		GetEditPointsTD()->EnableWindow(m_checkAddPointsTD.GetCheck() != 0);
		GetEditPointsTD()->SetFocus();
	}

	m_checkClearPointsTD.SetCheck(FALSE);
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleClearPointsTD()
{
	m_checkAddPointsTD.SetCheck(FALSE);
	SetEditInt( GetEditPointsTD(), 0 );
	GetEditPointsTD()->EnableWindow(FALSE);
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleAddCashSpendTD()
{
	m_checkClearCashSpendTD.SetCheck(FALSE);
	GetEditCashSpendTD()->EnableWindow(m_checkAddCashSpendTD.GetCheck() != 0 );
	GetEditCashSpendTD()->SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleClearCashSpendTD()
{
	m_checkAddCashSpendTD.SetCheck(FALSE);
	SetEditDouble( GetEditCashSpendTD(), 0.0 );
	GetEditCashSpendTD()->EnableWindow(FALSE);	
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleAddPurse1SpendTD()
{
	m_checkClearPurse1SpendTD.SetCheck(FALSE);
	GetEditPurse1SpendTD()->EnableWindow(m_checkAddPurse1SpendTD.GetCheck() != 0 );
	GetEditPurse1SpendTD()->SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleClearPurse1SpendTD()
{
	m_checkAddPurse1SpendTD.SetCheck(FALSE);
	SetEditDouble( GetEditPurse1SpendTD(), 0.0 );
	GetEditPurse1SpendTD()->EnableWindow(FALSE);
	UpdateData(FALSE);
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleAddPurse2SpendTD()
{
	m_checkClearPurse2SpendTD.SetCheck(FALSE);
	GetEditPurse2SpendTD()->EnableWindow(m_checkAddPurse2SpendTD.GetCheck() != 0 );
	GetEditPurse2SpendTD()->SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleClearPurse2SpendTD()
{
	m_checkAddPurse2SpendTD.SetCheck(FALSE);
	SetEditDouble( GetEditPurse2SpendTD(), 0.0 );
	GetEditPurse2SpendTD()->EnableWindow(FALSE);
}

//$$******************************************************************

bool CBatchPropertyPage2::HaveUpdates()
{
	return m_Memory.HaveUpdates();
}

//$$******************************************************************
// modify record with updates - return true if audit of update reqd

bool CBatchPropertyPage2::UpdateRecord( CSQLRowAccountFull& RowAccount)
{
	return m_Memory.UpdateRecord(RowAccount);
}

//$$******************************************************************

BOOL CBatchPropertyPage2::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	UpdateMemory();
	return bResult;
}

/**********************************************************************/
