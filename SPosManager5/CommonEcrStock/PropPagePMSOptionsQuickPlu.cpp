/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "FilterTypes.h"
#include "..\SPosEcrManager5\PluFilterDlgEcrman.h"
#include "..\SPosStockManager5\PluFilterDlgStockman.h"
#include "PMSOptions.h"
/**********************************************************************/
#include "PropPagePMSOptionsQuickPlu.h"
/**********************************************************************/

CPropPagePMSOptionsQuickPlu::CPropPagePMSOptionsQuickPlu() : CSSPropertyPage(CPropPagePMSOptionsQuickPlu::IDD), m_PluFilterArray ( FILTERTYPE_BROWSE_PLU )
{
	//{{AFX_DATA_INIT(CPropPagePMSOptionsQuickPlu)
	//}}AFX_DATA_INIT

	m_nEnableID[0] = IDC_CHECK_ENABLE1;
	m_nEnableID[1] = IDC_CHECK_ENABLE2;
	m_nEnableID[2] = IDC_CHECK_ENABLE3;
	m_nEnableID[3] = IDC_CHECK_ENABLE4;
	m_nEnableID[4] = IDC_CHECK_ENABLE5;
	m_nEnableID[5] = IDC_CHECK_ENABLE6;
	m_nEnableID[6] = IDC_CHECK_ENABLE7;
	m_nEnableID[7] = IDC_CHECK_ENABLE8;
	m_nEnableID[8] = IDC_CHECK_ENABLE9;
	m_nEnableID[9] = IDC_CHECK_ENABLE10;
	
	m_nPluNoID[0] = IDC_EDIT_PLUNO1;
	m_nPluNoID[1] = IDC_EDIT_PLUNO2;
	m_nPluNoID[2] = IDC_EDIT_PLUNO3;
	m_nPluNoID[3] = IDC_EDIT_PLUNO4;
	m_nPluNoID[4] = IDC_EDIT_PLUNO5;
	m_nPluNoID[5] = IDC_EDIT_PLUNO6;
	m_nPluNoID[6] = IDC_EDIT_PLUNO7;
	m_nPluNoID[7] = IDC_EDIT_PLUNO8;
	m_nPluNoID[8] = IDC_EDIT_PLUNO9;
	m_nPluNoID[9] = IDC_EDIT_PLUNO10;

	m_nDescriptionID[0] = IDC_EDIT_DESCRIPTION1;
	m_nDescriptionID[1] = IDC_EDIT_DESCRIPTION2;
	m_nDescriptionID[2] = IDC_EDIT_DESCRIPTION3;
	m_nDescriptionID[3] = IDC_EDIT_DESCRIPTION4;
	m_nDescriptionID[4] = IDC_EDIT_DESCRIPTION5;
	m_nDescriptionID[5] = IDC_EDIT_DESCRIPTION6;
	m_nDescriptionID[6] = IDC_EDIT_DESCRIPTION7;
	m_nDescriptionID[7] = IDC_EDIT_DESCRIPTION8;
	m_nDescriptionID[8] = IDC_EDIT_DESCRIPTION9;
	m_nDescriptionID[9] = IDC_EDIT_DESCRIPTION10;

	m_nQtyID[0] = IDC_COMBO_QTY1;
	m_nQtyID[1] = IDC_COMBO_QTY2;
	m_nQtyID[2] = IDC_COMBO_QTY3;
	m_nQtyID[3] = IDC_COMBO_QTY4;
	m_nQtyID[4] = IDC_COMBO_QTY5;
	m_nQtyID[5] = IDC_COMBO_QTY6;
	m_nQtyID[6] = IDC_COMBO_QTY7;
	m_nQtyID[7] = IDC_COMBO_QTY8;
	m_nQtyID[8] = IDC_COMBO_QTY9;
	m_nQtyID[9] = IDC_COMBO_QTY10;
	
	m_nBrowseID[0] = IDC_BUTTON_BROWSE_PLU1;
	m_nBrowseID[1] = IDC_BUTTON_BROWSE_PLU2;
	m_nBrowseID[2] = IDC_BUTTON_BROWSE_PLU3;
	m_nBrowseID[3] = IDC_BUTTON_BROWSE_PLU4;
	m_nBrowseID[4] = IDC_BUTTON_BROWSE_PLU5;
	m_nBrowseID[5] = IDC_BUTTON_BROWSE_PLU6;
	m_nBrowseID[6] = IDC_BUTTON_BROWSE_PLU7;
	m_nBrowseID[7] = IDC_BUTTON_BROWSE_PLU8;
	m_nBrowseID[8] = IDC_BUTTON_BROWSE_PLU9;
	m_nBrowseID[9] = IDC_BUTTON_BROWSE_PLU10;
}

/**********************************************************************/

CPropPagePMSOptionsQuickPlu::~CPropPagePMSOptionsQuickPlu()
{
}

/**********************************************************************/

void CPropPagePMSOptionsQuickPlu::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePMSOptionsQuickPlu)
	//}}AFX_DATA_MAP

	for ( int n = 0; n < 10; n++ )
	{
		DDX_Control(pDX, m_nEnableID[n], m_checkEnable[n] );
		DDX_Control(pDX, m_nDescriptionID[n], m_editDescription[n] );
		DDX_Control(pDX, m_nQtyID[n], m_comboQty[n] );
		DDX_Control(pDX, m_nBrowseID[n], m_buttonBrowse[n] );
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePMSOptionsQuickPlu, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePMSOptionsQuickPlu)
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDC_CHECK_ENABLE1, OnToggleEnable1)
	ON_BN_CLICKED(IDC_CHECK_ENABLE2, OnToggleEnable2)
	ON_BN_CLICKED(IDC_CHECK_ENABLE3, OnToggleEnable3)
	ON_BN_CLICKED(IDC_CHECK_ENABLE4, OnToggleEnable4)
	ON_BN_CLICKED(IDC_CHECK_ENABLE5, OnToggleEnable5)
	ON_BN_CLICKED(IDC_CHECK_ENABLE6, OnToggleEnable6)
	ON_BN_CLICKED(IDC_CHECK_ENABLE7, OnToggleEnable7)
	ON_BN_CLICKED(IDC_CHECK_ENABLE8, OnToggleEnable8)
	ON_BN_CLICKED(IDC_CHECK_ENABLE9, OnToggleEnable9)
	ON_BN_CLICKED(IDC_CHECK_ENABLE10, OnToggleEnable10)

	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO1, OnKillFocusPluNo1)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO2, OnKillFocusPluNo2)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO3, OnKillFocusPluNo3)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO4, OnKillFocusPluNo4)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO5, OnKillFocusPluNo5)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO6, OnKillFocusPluNo6)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO7, OnKillFocusPluNo7)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO8, OnKillFocusPluNo8)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO9, OnKillFocusPluNo9)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO10, OnKillFocusPluNo10)
	
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU1, OnButtonBrowse1)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU2, OnButtonBrowse2)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU3, OnButtonBrowse3)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU4, OnButtonBrowse4)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU5, OnButtonBrowse5)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU6, OnButtonBrowse6)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU7, OnButtonBrowse7)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU8, OnButtonBrowse8)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU9, OnButtonBrowse9)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU10, OnButtonBrowse10)

END_MESSAGE_MAP()

/**********************************************************************/

void CPropPagePMSOptionsQuickPlu::UpdateDescription( int n, __int64 nPluNo )
{
	if ( 0 == nPluNo )
		m_editDescription[n].SetWindowText( "" );
	else
	{
		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE )
		{
			CPluCSVRecord Plu;
			DataManager.Plu.GetAt( nPluIdx, Plu );
			m_editDescription[n].SetWindowText( Plu.GetReportText() );
		}
		else
			m_editDescription[n].SetWindowText( "Unknown" );
	}
}

/**********************************************************************/

BOOL CPropPagePMSOptionsQuickPlu::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	for ( int n = 0; n < 10; n++ )
		SubclassEdit( m_nPluNoID[n], SS_NUM, Super.MaxPluLen(), "%s" );

	for ( int n = 0; n < 10; n++ )
	{
		m_checkEnable[n].SetCheck( PMSOptions.GetQuickSaleEnable(n) == 2 );
		ToggleEnable(n);

		__int64 nPluNo = PMSOptions.GetQuickSalePlu(n);

		if ( 0 == nPluNo )
		{
			GetEditPluNo(n) -> SetWindowText( "" );
			m_editDescription[n].SetWindowText( "" );
		}
		else
		{
			CString strPluNo;
			::FormatPluNo( nPluNo, strPluNo, FALSE );
			GetEditPluNo(n) -> SetWindowText( strPluNo);

			UpdateDescription( n, nPluNo );
		}

		m_comboQty[n].AddString( "Sale" );
		m_comboQty[n].AddString( "Sale with Qty" );
		m_comboQty[n].AddString( "Allowance" );
		m_comboQty[n].AddString( "Allowance with Qty" );

		int nType = PMSOptions.GetQuickSaleType(n);

		if ( ( nType >= 0 ) && ( nType <= 3 ) )
			m_comboQty[n].SetCurSel( nType );
		else
			m_comboQty[n].SetCurSel( 0 );
	}

	return TRUE;  
}

/**********************************************************************/
CEdit* CPropPagePMSOptionsQuickPlu::GetEditPluNo( int n ){ return GetEdit( m_nPluNoID[n] ); }
/**********************************************************************/

BOOL CPropPagePMSOptionsQuickPlu::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePMSOptionsQuickPlu::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/
void CPropPagePMSOptionsQuickPlu::OnToggleEnable1(){ ToggleEnable(0); }
void CPropPagePMSOptionsQuickPlu::OnToggleEnable2(){ ToggleEnable(1); }
void CPropPagePMSOptionsQuickPlu::OnToggleEnable3(){ ToggleEnable(2); }
void CPropPagePMSOptionsQuickPlu::OnToggleEnable4(){ ToggleEnable(3); }
void CPropPagePMSOptionsQuickPlu::OnToggleEnable5(){ ToggleEnable(4); }
void CPropPagePMSOptionsQuickPlu::OnToggleEnable6(){ ToggleEnable(5); }
void CPropPagePMSOptionsQuickPlu::OnToggleEnable7(){ ToggleEnable(6); }
void CPropPagePMSOptionsQuickPlu::OnToggleEnable8(){ ToggleEnable(7); }
void CPropPagePMSOptionsQuickPlu::OnToggleEnable9(){ ToggleEnable(8); }
void CPropPagePMSOptionsQuickPlu::OnToggleEnable10(){ ToggleEnable(9); }
/**********************************************************************/

void CPropPagePMSOptionsQuickPlu::ToggleEnable( int n )
{
	bool bEnable = ( m_checkEnable[n].GetCheck() != 0 );
	GetEditPluNo(n) -> EnableWindow( bEnable );
	m_comboQty[n].EnableWindow( bEnable );
	m_buttonBrowse[n].EnableWindow( bEnable );
}

/**********************************************************************/
void CPropPagePMSOptionsQuickPlu::OnKillFocusPluNo1(){ KillFocusPluNo(0); }
void CPropPagePMSOptionsQuickPlu::OnKillFocusPluNo2(){ KillFocusPluNo(1); }
void CPropPagePMSOptionsQuickPlu::OnKillFocusPluNo3(){ KillFocusPluNo(2); }
void CPropPagePMSOptionsQuickPlu::OnKillFocusPluNo4(){ KillFocusPluNo(3); }
void CPropPagePMSOptionsQuickPlu::OnKillFocusPluNo5(){ KillFocusPluNo(4); }
void CPropPagePMSOptionsQuickPlu::OnKillFocusPluNo6(){ KillFocusPluNo(5); }
void CPropPagePMSOptionsQuickPlu::OnKillFocusPluNo7(){ KillFocusPluNo(6); }
void CPropPagePMSOptionsQuickPlu::OnKillFocusPluNo8(){ KillFocusPluNo(7); }
void CPropPagePMSOptionsQuickPlu::OnKillFocusPluNo9(){ KillFocusPluNo(8); }
void CPropPagePMSOptionsQuickPlu::OnKillFocusPluNo10(){ KillFocusPluNo(9); }
/**********************************************************************/

void CPropPagePMSOptionsQuickPlu::KillFocusPluNo( int n )
{
	UpdateData ( TRUE, FALSE );

	CString strPluNo;
	GetEditPluNo(n) -> GetWindowText( strPluNo );
	strPluNo = BarcodePluNoTable.ConvertPlu( strPluNo );
	GetEditPluNo(n) -> SetWindowText( strPluNo );

	__int64 nPluNo = _atoi64( strPluNo );
	UpdateDescription( n, nPluNo );
}

/**********************************************************************/
void CPropPagePMSOptionsQuickPlu::OnButtonBrowse1(){ BrowsePlu(0); }
void CPropPagePMSOptionsQuickPlu::OnButtonBrowse2(){ BrowsePlu(1); }
void CPropPagePMSOptionsQuickPlu::OnButtonBrowse3(){ BrowsePlu(2); }
void CPropPagePMSOptionsQuickPlu::OnButtonBrowse4(){ BrowsePlu(3); }
void CPropPagePMSOptionsQuickPlu::OnButtonBrowse5(){ BrowsePlu(4); }
void CPropPagePMSOptionsQuickPlu::OnButtonBrowse6(){ BrowsePlu(5); }
void CPropPagePMSOptionsQuickPlu::OnButtonBrowse7(){ BrowsePlu(6); }
void CPropPagePMSOptionsQuickPlu::OnButtonBrowse8(){ BrowsePlu(7); }
void CPropPagePMSOptionsQuickPlu::OnButtonBrowse9(){ BrowsePlu(8); }
void CPropPagePMSOptionsQuickPlu::OnButtonBrowse10(){ BrowsePlu(9); }
/**********************************************************************/

void CPropPagePMSOptionsQuickPlu::BrowsePlu( int n )
{
	UpdateData( TRUE, FALSE );

	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_PluFilterArray, this, nDlgResult, nPluIdx );

	if ( IDOK != nDlgResult )
		return;

	if ( nPluIdx < 0 )
		return;
		
	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt ( nPluIdx, PluRecord );
	__int64 nPluNo = PluRecord.GetPluNoInt();
	UpdateDescription( n, nPluNo );

	CString strPluNo = PluRecord.GetPluNoString();
	GetEditPluNo(n) -> SetWindowText( strPluNo );
	GetEditPluNo(n) -> SetFocus();
	GetEditPluNo(n) -> SetSel ( 0, -1 );
}

/**********************************************************************/

void CPropPagePMSOptionsQuickPlu::SaveRecord()
{
	for ( int n = 0; n < 10; n++ )
	{
		CString strPluNo;
		GetEditPluNo(n) -> GetWindowText( strPluNo );
		__int64 nPluNo = _atoi64( strPluNo );

		PMSOptions.SetQuickSaleEnable( n, ( m_checkEnable[n].GetCheck() != 0 ) ? 2 : 1 ); 
		PMSOptions.SetQuickSalePlu( n, nPluNo );
		PMSOptions.SetQuickSaleType( n, m_comboQty[n].GetCurSel() );
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
