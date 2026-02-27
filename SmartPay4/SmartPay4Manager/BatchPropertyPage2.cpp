//$$******************************************************************
#include "BatchPropertyPage2.h"
//$$******************************************************************

CBatchPropertyPage2::CBatchPropertyPage2(CBatchAccountBalanceMemory& BalanceMemory)
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CBatchPropertyPage2::IDD),
	m_Memory(BalanceMemory)
{
}

//$$******************************************************************

void CBatchPropertyPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ADDPURSE1_LIABILITY, m_checkAddPurse1Liability);
	DDX_Control(pDX, IDC_CHECK_ADDPURSE1_CREDIT, m_checkAddPurse1Credit);
	DDX_Control(pDX, IDC_CHECK_ADDPURSE3_LIABILITY, m_checkAddPurse3Liability);
	DDX_Control(pDX, IDC_CHECK_ADDPURSE3_CREDIT, m_checkAddPurse3Credit);
	DDX_Control(pDX, IDC_CHECK_ADDPOINTS, m_checkAddPoints);
	DDX_Control(pDX, IDC_CHECK_ADDPURSE1SPENDTD, m_checkAddPurse1SpendTD);
	DDX_Control(pDX, IDC_CHECK_ADDPURSE2SPENDTD, m_checkAddPurse2SpendTD);
	DDX_Control(pDX, IDC_CHECK_ADDPURSE3SPENDTD, m_checkAddPurse3SpendTD);
	DDX_Control(pDX, IDC_CHECK_ADDPOINTSTD, m_checkAddPointsTD);	
	DDX_Control(pDX, IDC_CHECK_CLEARPURSE1_LIABILITY, m_checkClearPurse1Liability);
	DDX_Control(pDX, IDC_CHECK_CLEARPURSE1_CREDIT, m_checkClearPurse1Credit);
	DDX_Control(pDX, IDC_CHECK_CLEARPURSE3_LIABILITY, m_checkClearPurse3Liability);
	DDX_Control(pDX, IDC_CHECK_CLEARPURSE3_CREDIT, m_checkClearPurse3Credit);
	DDX_Control(pDX, IDC_CHECK_CLEARPOINTS, m_checkClearPoints);
	DDX_Control(pDX, IDC_CHECK_CLEARPURSE1SPENDTD, m_checkClearPurse1SpendTD);
	DDX_Control(pDX, IDC_CHECK_CLEARPURSE2SPENDTD, m_checkClearPurse2SpendTD);
	DDX_Control(pDX, IDC_CHECK_CLEARPURSE3SPENDTD, m_checkClearPurse3SpendTD);
	DDX_Control(pDX, IDC_CHECK_CLEARCASHSPENDTD, m_checkClearCashSpendTD);
	DDX_Control(pDX, IDC_CHECK_CLEARPOINTSTD, m_checkClearPointsTD);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CBatchPropertyPage2, CSSAutoShutdownPropertyPage)
	ON_EN_KILLFOCUS(IDC_EDIT_POINTS, OnKillFocusEditPoints)
	ON_BN_CLICKED(IDC_CHECK_ADDPURSE1_LIABILITY, OnToggleAddPurse1Liability)
	ON_BN_CLICKED(IDC_CHECK_ADDPURSE1_CREDIT, OnToggleAddPurse1Credit)
	ON_BN_CLICKED(IDC_CHECK_ADDPURSE3_LIABILITY, OnToggleAddPurse3Liability)
	ON_BN_CLICKED(IDC_CHECK_ADDPURSE3_CREDIT, OnToggleAddPurse3Credit)
	ON_BN_CLICKED(IDC_CHECK_ADDPOINTS, OnToggleAddPoints)
	ON_BN_CLICKED(IDC_CHECK_ADDPURSE1SPENDTD, OnToggleAddPurse1SpendTD)
	ON_BN_CLICKED(IDC_CHECK_ADDPURSE2SPENDTD, OnToggleAddPurse2SpendTD)
	ON_BN_CLICKED(IDC_CHECK_ADDPURSE3SPENDTD, OnToggleAddPurse3SpendTD)
	ON_BN_CLICKED(IDC_CHECK_ADDPOINTSTD, OnToggleAddPointsTD)
	ON_BN_CLICKED(IDC_CHECK_CLEARPURSE1_LIABILITY, OnToggleClearPurse1Liability)
	ON_BN_CLICKED(IDC_CHECK_CLEARPURSE1_CREDIT, OnToggleClearPurse1Credit)
	ON_BN_CLICKED(IDC_CHECK_CLEARPURSE3_LIABILITY, OnToggleClearPurse3Liability)
	ON_BN_CLICKED(IDC_CHECK_CLEARPURSE3_CREDIT, OnToggleClearPurse3Credit)
	ON_BN_CLICKED(IDC_CHECK_CLEARPOINTS, OnToggleClearPoints)
	ON_BN_CLICKED(IDC_CHECK_CLEARPURSE1SPENDTD, OnToggleClearPurse1SpendTD)
	ON_BN_CLICKED(IDC_CHECK_CLEARPURSE2SPENDTD, OnToggleClearPurse2SpendTD)
	ON_BN_CLICKED(IDC_CHECK_CLEARPURSE3SPENDTD, OnToggleClearPurse3SpendTD)
	ON_BN_CLICKED(IDC_CHECK_CLEARPOINTSTD, OnToggleClearPointsTD)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CBatchPropertyPage2::OnInitDialog()
{
	CSSPropertyPage::OnInitDialog();

	m_Memory.TidyOptions();

	m_checkAddPurse1Liability.SetCheck(m_Memory.m_bAddPurse1Liability);
	m_checkAddPurse1Credit.SetCheck(m_Memory.m_bAddPurse1Credit);
	m_checkAddPurse3Liability.SetCheck(m_Memory.m_bAddPurse3Liability);
	m_checkAddPurse3Credit.SetCheck(m_Memory.m_bAddPurse3Credit);
	m_checkAddPoints.SetCheck(m_Memory.m_bAddPoints);
	m_checkAddPurse1SpendTD.SetCheck(m_Memory.m_bAddPurse1SpendTD);
	m_checkAddPurse2SpendTD.SetCheck(m_Memory.m_bAddPurse2SpendTD);
	m_checkAddPurse3SpendTD.SetCheck(m_Memory.m_bAddPurse3SpendTD);
	m_checkAddPointsTD.SetCheck(m_Memory.m_bAddPointsTD);

	m_checkClearPurse1Liability.SetCheck(m_Memory.m_bClearPurse1Liability);
	m_checkClearPurse1Credit.SetCheck(m_Memory.m_bClearPurse1Credit);
	m_checkClearPurse3Liability.SetCheck(m_Memory.m_bClearPurse3Liability);
	m_checkClearPurse3Credit.SetCheck(m_Memory.m_bClearPurse3Credit);
	m_checkClearPoints.SetCheck(m_Memory.m_bClearPoints);
	m_checkClearPurse1SpendTD.SetCheck(m_Memory.m_bClearPurse1SpendTD);
	m_checkClearPurse2SpendTD.SetCheck(m_Memory.m_bClearPurse2SpendTD);
	m_checkClearPurse3SpendTD.SetCheck(m_Memory.m_bClearPurse3SpendTD);
	m_checkClearCashSpendTD.SetCheck(m_Memory.m_bClearCashSpendTD);
	m_checkClearPointsTD.SetCheck(m_Memory.m_bClearPointsTD);

	SubclassEdit(IDC_EDIT_PURSE1_LIABILITY, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE1_CREDIT, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE2, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE3_LIABILITY, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE3_CREDIT, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_POINTS, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_PURSE1SPENDTD, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE2SPENDTD, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE3SPENDTD, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_POINTSTD, SS_NUM_S, 8, "%ld");

	SetEditDouble(GetEditPurse1Liability(), m_Memory.m_dPurse1Liability);
	SetEditDouble(GetEditPurse1Credit(), m_Memory.m_dPurse1Credit);
	SetEditDouble(GetEditPurse3Liability(), m_Memory.m_dPurse3Liability);
	SetEditDouble(GetEditPurse3Credit(), m_Memory.m_dPurse3Credit);
	SetEditInt(GetEditPoints(), m_Memory.m_nPoints);
	SetEditDouble(GetEditPurse1SpendTD(), m_Memory.m_dPurse1SpendTD);
	SetEditDouble(GetEditPurse2SpendTD(), m_Memory.m_dPurse2SpendTD);
	SetEditDouble(GetEditPurse3SpendTD(), m_Memory.m_dPurse3SpendTD);
	SetEditInt(GetEditPointsTD(), m_Memory.m_nPointsTD);

	bool bAllowPurse3 = System.GetEnablePurse3Flag();
	bool bAllowPoints = System.GetActiveEnablePointsFlag();

	// points

	m_checkAddPoints.EnableWindow(bAllowPoints);
	m_checkClearPoints.EnableWindow(bAllowPoints);

	// purse2

	m_checkAddPurse3Liability.EnableWindow(bAllowPurse3);
	m_checkAddPurse3Credit.EnableWindow(bAllowPurse3);
	m_checkClearPurse3Liability.EnableWindow(bAllowPurse3);
	m_checkClearPurse3Credit.EnableWindow(bAllowPurse3);

	// SpendTD
	m_checkAddPointsTD.EnableWindow(bAllowPoints);
	m_checkClearPointsTD.EnableWindow(bAllowPoints);

	m_checkAddPurse3SpendTD.EnableWindow(bAllowPurse3);
	m_checkClearPurse3SpendTD.EnableWindow(bAllowPurse3);

	GetEditPurse1Liability()->EnableWindow(m_Memory.m_bAddPurse1Liability);
	GetEditPurse1Credit()->EnableWindow(m_Memory.m_bAddPurse1Credit);
	GetEditPurse3Liability()->EnableWindow(m_Memory.m_bAddPurse3Liability);
	GetEditPurse3Credit()->EnableWindow(m_Memory.m_bAddPurse3Credit);
	GetEditPoints()->EnableWindow(m_Memory.m_bAddPoints);
	GetEditPurse1SpendTD()->EnableWindow(m_Memory.m_bAddPurse1SpendTD);
	GetEditPurse2SpendTD()->EnableWindow(m_Memory.m_bAddPurse2SpendTD);
	GetEditPurse3SpendTD()->EnableWindow(m_Memory.m_bAddPurse3SpendTD);
	GetEditPointsTD()->EnableWindow(m_Memory.m_bAddPointsTD);

	if (System.GetCreditOptionsFlag() == FALSE)
	{
		m_checkAddPurse1Credit.SetCheck(FALSE);
		m_checkClearPurse1Credit.SetCheck(FALSE);
		m_checkAddPurse3Credit.SetCheck(FALSE);
		m_checkClearPurse3Credit.SetCheck(FALSE);
		ShowAndEnableWindow(&m_checkAddPurse1Credit, FALSE);
		ShowAndEnableWindow(GetEditPurse1Credit(), FALSE);	
		ShowAndEnableWindow(&m_checkClearPurse1Credit, FALSE);
		ShowAndEnableWindow(&m_checkAddPurse3Credit, FALSE);
		ShowAndEnableWindow(GetEditPurse3Credit(), FALSE);
		ShowAndEnableWindow(&m_checkClearPurse3Credit, FALSE);

		MoveControl(&m_checkAddPurse1SpendTD, 7, 34);
		MoveControl(GetEditPurse1SpendTD(), 100, 32);
		MoveControl(&m_checkClearPurse1SpendTD, 182, 34);
		
		MoveControl(&m_checkAddPurse2SpendTD, 7, 64);
		MoveControl(GetEditPurse2SpendTD(), 100, 62);
		MoveControl(&m_checkClearPurse2SpendTD, 182, 64);

		MoveControl(&m_checkAddPurse3Liability, 7, 94);
		MoveControl(GetEditPurse3Liability(), 100, 92);
		MoveControl(&m_checkClearPurse3Liability, 182, 94);
		MoveControl(&m_checkAddPurse3SpendTD, 7, 114);
		MoveControl(GetEditPurse3SpendTD(), 100, 112);
		MoveControl(&m_checkClearPurse3SpendTD, 182, 114);

		MoveControl(&m_checkClearCashSpendTD, 182, 144);

		MoveControl(&m_checkAddPoints, 7, 174);
		MoveControl(GetEditPoints(), 100, 172);
		MoveControl(&m_checkClearPoints, 182, 174);
		MoveControl(&m_checkAddPointsTD, 7, 194);
		MoveControl(GetEditPointsTD(), 100, 192);
		MoveControl(&m_checkClearPointsTD, 182, 194);

		m_checkAddPurse1Liability.SetWindowText("Add Purse1 Balance");
		m_checkClearPurse1Liability.SetWindowText("Clear Purse1 Balance");
		m_checkAddPurse3Liability.SetWindowText("Add Purse3 Balance");
		m_checkClearPurse3Liability.SetWindowText("Clear Purse3 Balance");
	}

	return TRUE;
}

//$$******************************************************************
CEdit* CBatchPropertyPage2::GetEditPurse1Liability(){ return GetEdit( IDC_EDIT_PURSE1_LIABILITY ); }
CEdit* CBatchPropertyPage2::GetEditPurse1Credit() { return GetEdit(IDC_EDIT_PURSE1_CREDIT); }
CEdit* CBatchPropertyPage2::GetEditPurse3Liability() { return GetEdit(IDC_EDIT_PURSE3_LIABILITY); }
CEdit* CBatchPropertyPage2::GetEditPurse3Credit() { return GetEdit(IDC_EDIT_PURSE3_CREDIT); }
CEdit* CBatchPropertyPage2::GetEditPoints(){ return GetEdit( IDC_EDIT_POINTS ); }
CEdit* CBatchPropertyPage2::GetEditPurse1SpendTD(){ return GetEdit( IDC_EDIT_PURSE1SPENDTD ); }
CEdit* CBatchPropertyPage2::GetEditPurse2SpendTD(){ return GetEdit( IDC_EDIT_PURSE2SPENDTD ); }
CEdit* CBatchPropertyPage2::GetEditPurse3SpendTD(){ return GetEdit( IDC_EDIT_PURSE3SPENDTD ); }
CEdit* CBatchPropertyPage2::GetEditPointsTD(){ return GetEdit( IDC_EDIT_POINTSTD ); }
//$$******************************************************************

void CBatchPropertyPage2::SetEditDouble( CEdit* pEdit, double d )
{
	CString str = "";
	str.Format( "%.2f", d );
	pEdit -> SetWindowText( str );
}

//$$******************************************************************

void CBatchPropertyPage2::SetEditInt( CEdit* pEdit, int n )
{
	CString str = "";
	str.Format( "%d", n );
	pEdit -> SetWindowText( str );
}

//$$******************************************************************

double CBatchPropertyPage2::GetEditDouble( CEdit* pEdit )
{
	CString str = "";
	pEdit -> GetWindowText( str );
	return atof( str );
}

//$$******************************************************************

int CBatchPropertyPage2::GetEditInt( CEdit* pEdit )
{
	CString str = "";
	pEdit -> GetWindowText( str );
	return atoi( str );
}

//$$******************************************************************

void CBatchPropertyPage2::UpdateMemory()
{
	m_Memory.m_bAddPurse1Liability = ( m_checkAddPurse1Liability.GetCheck() != 0 );
	m_Memory.m_bAddPurse1Credit = (m_checkAddPurse1Credit.GetCheck() != 0);
	m_Memory.m_bAddPurse3Liability = (m_checkAddPurse3Liability.GetCheck() != 0);
	m_Memory.m_bAddPurse3Credit = (m_checkAddPurse3Credit.GetCheck() != 0);
	m_Memory.m_bAddPoints = ( m_checkAddPoints.GetCheck() != 0 );
	m_Memory.m_bAddPurse1SpendTD = ( m_checkAddPurse1SpendTD.GetCheck() != 0 );
	m_Memory.m_bAddPurse2SpendTD = ( m_checkAddPurse2SpendTD.GetCheck() != 0 );
	m_Memory.m_bAddPurse3SpendTD = ( m_checkAddPurse3SpendTD.GetCheck() != 0 );
	m_Memory.m_bAddPointsTD = ( m_checkAddPointsTD.GetCheck() != 0 );
	m_Memory.m_bClearPurse1Liability = ( m_checkClearPurse1Liability.GetCheck() != 0 );
	m_Memory.m_bClearPurse1Credit = (m_checkClearPurse1Credit.GetCheck() != 0);
	m_Memory.m_bClearPurse3Liability = (m_checkClearPurse3Liability.GetCheck() != 0);
	m_Memory.m_bClearPurse3Credit = (m_checkClearPurse3Credit.GetCheck() != 0);
	m_Memory.m_bClearPoints = ( m_checkClearPoints.GetCheck() != 0 );
	m_Memory.m_bClearPurse1SpendTD = ( m_checkClearPurse1SpendTD.GetCheck() != 0 );
	m_Memory.m_bClearPurse2SpendTD = ( m_checkClearPurse2SpendTD.GetCheck() != 0 );
	m_Memory.m_bClearPurse3SpendTD = ( m_checkClearPurse3SpendTD.GetCheck() != 0 );
	m_Memory.m_bClearPointsTD = ( m_checkClearPointsTD.GetCheck() != 0 );
	m_Memory.m_dPurse1Liability = GetEditDouble( GetEditPurse1Liability() );
	m_Memory.m_dPurse1Credit = GetEditDouble(GetEditPurse1Credit());
	m_Memory.m_dPurse3Liability = GetEditDouble(GetEditPurse3Liability());
	m_Memory.m_dPurse3Credit = GetEditDouble(GetEditPurse3Credit());
	m_Memory.m_nPoints = GetEditInt( GetEditPoints() );
	m_Memory.m_dPurse1SpendTD = GetEditDouble( GetEditPurse1SpendTD() );
	m_Memory.m_dPurse2SpendTD = GetEditDouble( GetEditPurse2SpendTD() );
	m_Memory.m_dPurse3SpendTD = GetEditDouble( GetEditPurse3SpendTD() );
	m_Memory.m_nPointsTD = GetEditInt( GetEditPointsTD() );

	if (System.GetCreditOptionsFlag() == FALSE)
	{
		m_Memory.m_bAddPurse1Credit = FALSE;
		m_Memory.m_bClearPurse1Credit = FALSE;
		m_Memory.m_bAddPurse3Credit = FALSE;
		m_Memory.m_bClearPurse3Credit = FALSE;
		m_Memory.m_dPurse1Credit = 0.0;
		m_Memory.m_dPurse3Credit = 0.0;
	}

	m_Memory.TidyOptions();
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleAddPurse1Liability()
{
	m_checkClearPurse1Liability.SetCheck(FALSE);
	GetEditPurse1Liability()->EnableWindow( m_checkAddPurse1Liability.GetCheck() != 0 );
	GetEditPurse1Liability()->SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleAddPurse1Credit()
{
	m_checkClearPurse1Credit.SetCheck(FALSE);
	GetEditPurse1Credit()->EnableWindow(m_checkAddPurse1Credit.GetCheck() != 0);
	GetEditPurse1Credit()->SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleClearPurse1Liability()
{
	m_checkAddPurse1Liability.SetCheck( FALSE );
	SetEditDouble( GetEditPurse1Liability(), 0.0 );
	GetEditPurse1Liability()->EnableWindow(FALSE);
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleClearPurse1Credit()
{
	m_checkAddPurse1Credit.SetCheck(FALSE);
	SetEditDouble(GetEditPurse1Credit(), 0.0);
	GetEditPurse1Credit()->EnableWindow(FALSE);
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleAddPurse3Liability()
{
	m_checkClearPurse3Liability.SetCheck(FALSE);
	GetEditPurse3Liability()->EnableWindow(m_checkAddPurse3Liability.GetCheck() != 0);
	GetEditPurse3Liability()->SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleAddPurse3Credit()
{
	m_checkClearPurse3Credit.SetCheck(FALSE);
	GetEditPurse3Credit()->EnableWindow(m_checkAddPurse3Credit.GetCheck() != 0);
	GetEditPurse3Credit()->SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleClearPurse3Liability()
{
	m_checkAddPurse3Liability.SetCheck(FALSE);
	SetEditDouble(GetEditPurse3Liability(), 0.0);
	GetEditPurse3Liability()->EnableWindow(FALSE);
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleClearPurse3Credit()
{
	m_checkAddPurse3Credit.SetCheck(FALSE);
	SetEditDouble(GetEditPurse3Credit(), 0.0);
	GetEditPurse3Credit()->EnableWindow(FALSE);
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleAddPoints()
{
	bool bToggleOn = (m_checkAddPoints.GetCheck() != 0 );

	GetEditPoints() -> EnableWindow( bToggleOn );

	if ( TRUE == bToggleOn )
	{
		m_checkAddPointsTD.SetCheck(TRUE);
		m_checkClearPointsTD.SetCheck(FALSE);
		SetEditInt( GetEditPointsTD(), GetEditInt( GetEditPoints() ) );
		GetEditPointsTD() -> EnableWindow( TRUE );
	}
	else
	{
		m_checkAddPointsTD.SetCheck(FALSE);
		m_checkClearPointsTD.SetCheck(FALSE);
		SetEditInt( GetEditPointsTD(), GetEditInt( GetEditPoints() ) );
		GetEditPointsTD() -> EnableWindow(FALSE);
	}
	
	m_checkClearPoints.SetCheck(FALSE);
	GetEditPoints()->SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleClearPoints()
{
	m_checkAddPoints.SetCheck( FALSE );
	SetEditInt( GetEditPoints(), 0 );
	GetEditPoints()->EnableWindow(FALSE);
}

//$$******************************************************************

void CBatchPropertyPage2::OnKillFocusEditPoints()
{
	SetEditInt( GetEditPointsTD(), GetEditInt( GetEditPoints() ) );
	m_checkAddPointsTD.SetCheck(TRUE);
	GetEditPointsTD() -> EnableWindow( TRUE );	
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleAddPointsTD()
{
	SetEditInt( GetEditPointsTD(), GetEditInt( GetEditPoints() ) );
	GetEditPointsTD()->EnableWindow(m_checkAddPointsTD.GetCheck() != 0 );
	GetEditPointsTD()->SetFocus();
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

void CBatchPropertyPage2::OnToggleAddPurse3SpendTD()
{
	m_checkClearPurse3SpendTD.SetCheck(FALSE);
	GetEditPurse3SpendTD()->EnableWindow(m_checkAddPurse3SpendTD.GetCheck() != 0 );
	GetEditPurse3SpendTD()->SetFocus();
}

//$$******************************************************************

void CBatchPropertyPage2::OnToggleClearPurse3SpendTD()
{
	m_checkAddPurse3SpendTD.SetCheck(FALSE);
	SetEditDouble( GetEditPurse3SpendTD(), 0.0 );
	GetEditPurse3SpendTD()->EnableWindow(FALSE);
}

//$$******************************************************************

bool CBatchPropertyPage2::HaveUpdates()
{
	return m_Memory.HaveUpdates();
}

//$$******************************************************************
// modify record with updates - return true if audit of update reqd

bool CBatchPropertyPage2::UpdateRecord(CSQLRowAccountFull& RowAccount)
{
	return m_Memory.UpdateRecord( RowAccount );
}

//$$******************************************************************

void CBatchPropertyPage2::AddWarnings ( CStringArray* pArray )
{
	m_Memory.AddWarnings( pArray );
}

//$$******************************************************************

BOOL CBatchPropertyPage2::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	UpdateMemory();
	return bResult;
}

/**********************************************************************/
