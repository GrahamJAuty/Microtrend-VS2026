/**********************************************************************/
#include "BarcodeCSVArray.h"
#include "DefEditHiddenDlg.h"
/**********************************************************************/
#include "PropPageHiddenOptionsEcrman.h"
/**********************************************************************/

CPropPageHiddenOptionsEcrman::CPropPageHiddenOptionsEcrman() : CSSPropertyPage(CPropPageHiddenOptionsEcrman::IDD)
{
	//{{AFX_DATA_INIT(CPropPageHiddenOptionsEcrman)
	//}}AFX_DATA_INIT
	m_nPluFlagMode = 0;
}

/**********************************************************************/

CPropPageHiddenOptionsEcrman::~CPropPageHiddenOptionsEcrman()
{
}

/**********************************************************************/

void CPropPageHiddenOptionsEcrman::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageHiddenOptionsEcrman)
	DDX_Control(pDX, IDC_COMBO_SERVER, m_comboReportFlag9);
	DDX_Control(pDX, IDC_CHECK_CASHRSPFLAG1, m_checkCashRSPFlag1);
	DDX_Control(pDX, IDC_CHECK_CASHRSPFLAG2, m_checkCashRSPFlag2);
	DDX_Control(pDX, IDC_CHECK_CASHRSPFLAG3, m_checkCashRSPFlag3);
	DDX_Control(pDX, IDC_CHECK_CASHRSPFLAG4, m_checkCashRSPFlag4);
	DDX_Control(pDX, IDC_CHECK_CASHRSPFLAG6, m_checkCashRSPFlag6);
	DDX_Control(pDX, IDC_CHECK_CASHRSPFLAG7, m_checkCashRSPFlag7);
	DDX_Control(pDX, IDC_CHECK_CASHRSPFLAG8, m_checkCashRSPFlag8);
	DDX_Control(pDX, IDC_CHECK_PLUFLAG6, m_checkPluFlag6);
	DDX_Control(pDX, IDC_CHECK_PLUFLAG8, m_checkPluFlag8);
	DDX_Control(pDX, IDC_CHECK_LABELFLAG1, m_checkLabelFlag1);
	DDX_Control(pDX, IDC_COMBO_LABELFLAG2, m_comboLabelFlag2);
	DDX_Control(pDX, IDC_EDIT_LOCCOMBO, m_editLocCombo);
	DDX_Control(pDX, IDC_CHECK_REPORTFLAG1, m_checkReportFlag1);
	DDX_Control(pDX, IDC_CHECK_REPORTFLAG8, m_checkReportFlag8);
	DDX_Control(pDX, IDC_CHECK_REPORTFLAG17, m_checkReportFlag17);
	DDX_Control(pDX, IDC_CHECK_REPORTFLAG18, m_checkReportFlag18);
	DDX_Control(pDX, IDC_CHECK_REPORTFLAG19, m_checkReportFlag19);
	DDX_Control(pDX, IDC_CHECK_REPORTFLAG20, m_checkReportFlag20);
	DDX_Control(pDX, IDC_CHECK_GRAPHFLAG1, m_checkGraphFlag1);
	DDX_Radio(pDX, IDC_RADIO_PLUFLAG0, m_nPluFlagMode);
	DDX_Control(pDX, IDC_CHECK_PLUFLAG10, m_checkPluFlag10);
	DDX_Control(pDX, IDC_EDIT_PLUFLAG10, m_editPluFlag10);
	DDX_Control(pDX, IDC_CHECK_PLUFLAG12, m_checkPluFlag12);
	DDX_Control(pDX, IDC_BUTTON_BESPOKE, m_buttonBespoke);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageHiddenOptionsEcrman, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageHiddenOptionsEcrman)
	ON_EN_KILLFOCUS(IDC_EDIT_LOCCOMBO, OnKillFocusLocCombo)
	ON_BN_CLICKED(IDC_CHECK_REPORTFLAG1, OnCheckReportFlag1)
	ON_BN_CLICKED(IDC_CHECK_PLUFLAG10, OnCheckPluflag10)
	ON_BN_CLICKED(IDC_BUTTON_BESPOKE, OnButtonBespoke)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageHiddenOptionsEcrman::OnInitDialog() 
{
	m_nPluFlagMode = DealerFlags.GetAddPluMode();
	if (m_nPluFlagMode < 0 || m_nPluFlagMode > 2)
	{
		m_nPluFlagMode = 0;
	}

	CPropertyPage::OnInitDialog();	

	m_buttonBespoke.ShowWindow( SW_HIDE );

	m_comboReportFlag9.ResetContent();
	m_comboReportFlag9.AddString( "Consolidate by server number only" );
	m_comboReportFlag9.AddString( "Consolidate by server number and name" );
	m_comboReportFlag9.AddString( "Consolidate by server number and server file" );

	m_comboLabelFlag2.AddString( "Auto Detect Barcode Length" );
	m_comboLabelFlag2.SetItemData( 0, 1 );
	m_comboLabelFlag2.AddString( "Remove leading zeros" );
	m_comboLabelFlag2.SetItemData( 1, 0 );

	for ( int n = 8; n <= Super.MaxBarcodeLen(); n++ )
	{
		CString strText = "";
		strText.Format ( "Expand to %d digits", n );
		int nPos = m_comboLabelFlag2.AddString( strText );
		m_comboLabelFlag2.SetItemData( nPos, n );
	}

	UpdateControls();

	return TRUE;  
}

/**********************************************************************/

void CPropPageHiddenOptionsEcrman::UpdateControls()
{
	//PLU FLAG 1 AND 2 HANDLE BY RADIO DDX
	
	//PLU FLAG 6
	m_checkPluFlag6.SetCheck( DealerFlags.GetPriceSetExcludeByDefaultFlag() );

	//PLU FLAG 8
	m_checkPluFlag8.SetCheck( DealerFlags.GetSpecialChangesOnlyFlag() );

	//PLUFLAG10
	CString strDept = DealerFlags.GetDeptZeroText();
	CString strGroup = DealerFlags.GetGroupZeroText();

	//PLU FLAG 12
	m_checkPluFlag12.SetCheck( DealerFlags.GetLegacyPluExportFlag() );

	if ( strDept != "" || strGroup != "" )
	{
		m_checkPluFlag10.SetCheck( TRUE );
		m_editPluFlag10.EnableWindow( TRUE );
		
		CCSV csv;
		csv.Add( strDept );
		csv.Add( strGroup );
		m_editPluFlag10.SetWindowText( csv.GetLine() );
	}
	else
	{
		m_checkPluFlag10.SetCheck( FALSE );
		m_editPluFlag10.EnableWindow( FALSE );
		m_editPluFlag10.SetWindowText( "" );
	}
	
	//LABEL FLAG 1
	m_checkLabelFlag1.SetCheck( DealerFlags.GetEcrTextOnLabelsFlag() );
	
	//LABELFLAG2
	m_comboLabelFlag2.SetCurSel(0);
	for ( int n = 0; n < m_comboLabelFlag2.GetCount(); n++ )
	{
		if ( m_comboLabelFlag2.GetItemData(n) == (WORD) DealerFlags.GetSPosBarcodeLength() )
		{
			m_comboLabelFlag2.SetCurSel(n);
			break;
		}
	}

	//REPORT FLAG 1
	m_checkReportFlag1.SetCheck( DealerFlags.GetLocComboOrderFlag() );
	m_editLocCombo.SetWindowText( DealerFlags.GetLocComboOrder() );
	OnCheckReportFlag1();

	//REPORT FLAG 8
	m_checkReportFlag8.SetCheck( DealerFlags.GetSalesHistoryDateControlFlag() );
	
	//REPORT FLAG 9
	m_comboReportFlag9.SetCurSel( DealerFlags.GetServerConsolidationType() % 3 );

	//REPORT FLAG 17
	m_checkReportFlag17.SetCheck( DealerFlags.GetHideNoDataFlag() );

	//REPORT FLAG 18
	m_checkReportFlag18.SetCheck( DealerFlags.GetPrice1SeparateLineFlag() );

	//REPORT FLAG 19
	m_checkReportFlag19.SetCheck( DealerFlags.GetAssumeCoversFlag() );

	//REPORT FLAG 20
	m_checkReportFlag20.SetCheck(DealerFlags.GetLoyaltyReconReportFlag());
	
	//GRAPH FLAG 1
	m_checkGraphFlag1.SetCheck( DealerFlags.GetWeekNoGraphFlag() );

	//CASHRSP FLAG 1
	m_checkCashRSPFlag1.SetCheck( DealerFlags.GetUseCashRSPTillNoFlag() );

	//CASHRSP FLAG 2
	m_checkCashRSPFlag2.SetCheck( DealerFlags.GetCashRSPPromoFixFlag() );

	//CASHRSP FLAG 3
	m_checkCashRSPFlag3.SetCheck( DealerFlags.GetLogAllCashRSPFilesFlag() );

	//CASHRSP FLAG 4
	m_checkCashRSPFlag4.SetCheck( DealerFlags.GetCashRSPV19SmartPayFlag() );

	//CASHRSP FLAG 6
	m_checkCashRSPFlag6.SetCheck( DealerFlags.GetCashRSPFixItemServerZeroFlag() );

	//CASHRSP FLAG 7
	m_checkCashRSPFlag7.SetCheck( DealerFlags.GetAutoRetryTermFileImportFlag() );

	//CASHRSP FLAG 8
	m_checkCashRSPFlag8.SetCheck( DealerFlags.GetCashRSPCatchDuplicatesFlag() );
}

/***************************************************************/

void CPropPageHiddenOptionsEcrman::OnCheckReportFlag1() 
{
	bool bEnable = ( m_checkReportFlag1.GetCheck() != 0 );
	m_editLocCombo.EnableWindow( bEnable );
}

/***************************************************************/

void CPropPageHiddenOptionsEcrman::OnCheckPluflag10()
{
	bool bEnable = ( m_checkPluFlag10.GetCheck() != 0 );
	m_editPluFlag10.EnableWindow( bEnable );
}

/***************************************************************/

void CPropPageHiddenOptionsEcrman::OnKillFocusLocCombo() 
{
	ValidateLocComboString();	
}

/***************************************************************/

void CPropPageHiddenOptionsEcrman::ValidateLocComboString()
{
	CString strLocCombo;
	m_editLocCombo.GetWindowText( strLocCombo );

	CCSV csvIn( strLocCombo );
	CCSV csvOut;

	for ( int n = 0; n < csvIn.GetSize(); n++ )
	{
		CString strType = csvIn.GetString(n);
		strType.MakeUpper();
		DealerFlags.AddToLocComboString( csvOut, strType );
	}

	strLocCombo = csvOut.GetLine();
	m_editLocCombo.SetWindowText( strLocCombo );
}

/***************************************************************/

BOOL CPropPageHiddenOptionsEcrman::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	
	//PLU FLAG 1 AND 2
	switch( m_nPluFlagMode )
	{
	case 1:	DealerFlags.SetAddPluMode1();	break;
	case 2:	DealerFlags.SetAddPluMode2();	break;
	default:	DealerFlags.SetAddPluMode0();	break;
	}
		
	//PLU FLAG 6
	DealerFlags.SetPriceSetExcludeByDefaultFlag( m_checkPluFlag6.GetCheck() != 0 );

	//PLU FLAG 8
	DealerFlags.SetSpecialChangesOnlyFlag( m_checkPluFlag8.GetCheck() != 0 );

	//PLU FLAG 10
	if ( m_checkPluFlag10.GetCheck() != 0 )
	{
		CString strBuffer;
		m_editPluFlag10.GetWindowText( strBuffer );

		CCSV csv( strBuffer );
		DealerFlags.SetDeptZeroText( csv.GetString(0) );
		DealerFlags.SetGroupZeroText( csv.GetString(1) );
	}
	else
	{
		DealerFlags.SetDeptZeroText( "" );
		DealerFlags.SetGroupZeroText( "" );
	}

	//PLU FLAG 12
	DealerFlags.SetLegacyPluExportFlag( m_checkPluFlag12.GetCheck() != 0 );

	//LABEL FLAG 1
	DealerFlags.SetEcrTextOnLabelsFlag( m_checkLabelFlag1.GetCheck() != 0 );

	//LABEL FLAG 2
	int nSel = m_comboLabelFlag2.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboLabelFlag2.GetCount() )
		DealerFlags.SetSPosBarcodeLength( m_comboLabelFlag2.GetItemData( nSel ) );

	//REPORT FLAG 1
	ValidateLocComboString();

	CString strLocCombo;
	m_editLocCombo.GetWindowText( strLocCombo );

	if ( ( strLocCombo != "" ) && ( m_checkReportFlag1.GetCheck() != 0 ) )
	{
		DealerFlags.SetLocComboOrderFlag( TRUE );
		DealerFlags.SetLocComboOrder( strLocCombo );
	}
	else
	{
		DealerFlags.SetLocComboOrderFlag( FALSE );
		DealerFlags.SetLocComboOrder ( "" );
	}
	
	//REPORT FLAG 8
	DealerFlags.SetSalesHistoryDateControlFlag( m_checkReportFlag8.GetCheck() != 0 );

	//REPORT FLAG 9
	DealerFlags.SetServerConsolidationType( m_comboReportFlag9.GetCurSel() );
	
	//REPORT FLAG 17
	DealerFlags.SetHideNoDataFlag( m_checkReportFlag17.GetCheck() != 0 );

	//REPORT FLAG 18
	DealerFlags.SetPrice1SeparateLineFlag( m_checkReportFlag18.GetCheck() != 0 );

	//REPORT FLAG 19
	DealerFlags.SetAssumeCoversFlag( m_checkReportFlag19.GetCheck() != 0 );

	//REPORT FLAG 20
	DealerFlags.SetLoyaltyReconReportFlag(m_checkReportFlag20.GetCheck() != 0);

	//GRAPH FLAG 1
	DealerFlags.SetWeekNoGraphFlag( m_checkGraphFlag1.GetCheck() != 0 );
	
	//CASHRSPFLAG 1
	DealerFlags.SetUseCashRSPTillNoFlag( m_checkCashRSPFlag1.GetCheck() != 0 );

	//CASHRSPFLAG 2
	DealerFlags.SetCashRSPPromoFixFlag( m_checkCashRSPFlag2.GetCheck() != 0 );

	//CASHRSPFLAG 3
	DealerFlags.SetLogAllCashRSPFilesFlag( m_checkCashRSPFlag3.GetCheck() != 0 );

	//CASHRSPFLAG 4
	DealerFlags.SetCashRSPV19SmartPayFlag( m_checkCashRSPFlag4.GetCheck() != 0 );

	//CASHRSPFLAG 6
	DealerFlags.SetCashRSPFixItemServerZeroFlag( m_checkCashRSPFlag6.GetCheck() != 0 );

	//CASHRSPFLAG 7
	DealerFlags.SetAutoRetryTermFileImportFlag( m_checkCashRSPFlag7.GetCheck() != 0 );

	//CASHRSPFLAG 8
	DealerFlags.SetCashRSPCatchDuplicatesFlag( m_checkCashRSPFlag8.GetCheck() != 0 );

	return bResult;
}

/**********************************************************************/

BOOL CPropPageHiddenOptionsEcrman::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPageHiddenOptionsEcrman::OnButtonBespoke()
{
	CDefEditHiddenDlg dlg( this );
	dlg.DoModal();
}

/**********************************************************************/
