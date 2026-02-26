/**********************************************************************/
#include "EposTaskDatabase.h"
#include "JobListDlg.h"
#include "PluCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPageOptionsPlu.h"
/**********************************************************************/

CPropPageOptionsPlu::CPropPageOptionsPlu() : CSSPropertyPage(CPropPageOptionsPlu::IDD)
{
	//{{AFX_DATA_INIT(CPropPageOptionsPlu)
	//}}AFX_DATA_INIT
	m_bAllowModifiers = ( SysInfo.GetMaxBasePluLen() != 0 );
}

/**********************************************************************/

CPropPageOptionsPlu::~CPropPageOptionsPlu()
{
}

/**********************************************************************/

void CPropPageOptionsPlu::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageOptionsPlu)

	DDX_Control( pDX, IDC_TAB_ACTIONS, m_TabActions );

	DDX_Control( pDX, IDC_STATIC_EDITING1, m_staticEditing1 );
	DDX_Control(pDX, IDC_CHECK_TAB_BARCODE, m_checkTabBarcode);
	DDX_Control(pDX, IDC_CHECK_TAB_LINK, m_checkTabLink);
	DDX_Control(pDX, IDC_CHECK_TAB_INFO, m_checkTabInfo);
	DDX_Control(pDX, IDC_CHECK_DOWNLOAD_DATE, m_checkDownloadDate);
	DDX_Control(pDX, IDC_CHECK_PRICESETS, m_checkPriceSets);
	DDX_Control(pDX, IDC_CHECK_INACTIVE_NODOWNLOAD, m_checkInactiveNoDownload);
	DDX_Control(pDX, IDC_CHECK_INACTIVE_AUTOHIDE, m_checkInactiveAutoHide);
	DDX_Control(pDX, IDC_CHECK_TASK, m_checkTask);
	DDX_Control(pDX, IDC_BUTTON_TASK, m_buttonTask);
	DDX_Control(pDX, IDC_COMBO_PLU_TEXT, m_comboPluText);
	
	DDX_Control(pDX, IDC_CHECK_IMPORT_CSV_AUTO, m_checkImportCSVAuto);
	DDX_Control(pDX, IDC_CHECK_IMPORT_CSV_MANUAL, m_checkImportCSVManual);
	DDX_Control(pDX, IDC_CHECK_EXPORT_CSV_MANUAL, m_checkExportCSVManual);
	DDX_Control(pDX, IDC_CHECK_EXPORT_PURCHASE_AUTO, m_checkExportPurchaseAuto);
	DDX_Control(pDX, IDC_CHECK_IMPORT_PRODCSV_MANUAL, m_checkImportProdCSVManual );
	DDX_Control(pDX, IDC_CHECK_IMPORT_PRODCSV_AUTO, m_checkImportProdCSVAuto );
	
	DDX_Control(pDX, IDC_STATIC_MODIFIERS1, m_staticModifiers1 );
	DDX_Control(pDX, IDC_STATIC_MODIFIERS2, m_staticModifiers2 );
	DDX_Control(pDX, IDC_CHECK_MODIFIER_DISCARD, m_checkDiscardModifiers );
	DDX_Control(pDX, IDC_CHECK_BARCODE_MOD, m_checkBarcodeModifier);
	DDX_Control(pDX, IDC_COMBO_MODTEXT_EPOS, m_comboModTextEpos);
	DDX_Control(pDX, IDC_COMBO_MODTEXT_KEY, m_comboModTextKey);
	
	DDX_Control(pDX, IDC_CHECK_DELETEBARCODE, m_checkDeleteBarcode);
	DDX_Control(pDX, IDC_COMBO_DELETEBARCODE, m_comboDeleteBarcode);
	
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageOptionsPlu, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageOptionsPlu)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ACTIONS, OnTcnSelchangeTabs)
	ON_BN_CLICKED(IDC_CHECK_TASK, SetActiveTabControls)
	ON_BN_CLICKED(IDC_BUTTON_TASK, OnEditTask)
	ON_BN_CLICKED(IDC_CHECK_IMPORT_PRODCSV_MANUAL, SetActiveTabControls)
	ON_BN_CLICKED(IDC_CHECK_DELETEBARCODE, SetActiveTabControls)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageOptionsPlu::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_arrayTabTypes.Add(0);
	m_arrayTabTypes.Add(1);

	m_TabActions.InsertItem( 0, "Edit && Download" );
	m_TabActions.InsertItem( 1, "Import/Export" );
	
	if ( TRUE == m_bAllowModifiers ) 
	{
		m_TabActions.InsertItem( 2, "Modifiers" );
		m_TabActions.InsertItem( 3, "Barcodes" );
		m_arrayTabTypes.Add(2);
		m_arrayTabTypes.Add(3);
	}
	else
	{
		m_TabActions.InsertItem( 2, "Barcodes" );
		m_arrayTabTypes.Add(3);
	}

	m_TabActions.SetCurSel(0);

	MoveControl( &m_checkTabBarcode, 19, 40 );
	MoveControl( &m_checkTabLink, 19, 54 );
	MoveControl( &m_checkTabInfo, 19, 68 );
	MoveControl( &m_checkDownloadDate, 19, 92 );
	MoveControl( &m_checkPriceSets, 19, 106 );
	
	{
		int nYPos = 116;
		if ( SysInfo.IsEcrLiteSystem() == FALSE )
			nYPos += 14;
		else
			ShowAndEnableWindow( &m_checkPriceSets, FALSE );

		MoveControl( &m_checkInactiveNoDownload, 19, nYPos );
		nYPos += 14;
		MoveControl( &m_checkInactiveAutoHide, 19, nYPos );
		nYPos += 24;
		MoveControl( &m_checkTask, 19, nYPos ); 
		nYPos += 14;
		MoveControl( &m_buttonTask, 19, nYPos );
		nYPos += 36;
		MoveControl( &m_staticEditing1, 19, nYPos );
		MoveControl( &m_comboPluText, 88, nYPos );
	}
	/*****/
	{
		MoveControl( &m_checkImportCSVAuto, 19, 40 );
		MoveControl( &m_checkImportCSVManual, 19, 54 );
		MoveControl( &m_checkExportCSVManual, 19, 68 );
		MoveControl( &m_checkExportPurchaseAuto, 19, 82 );
		int nYOffset = ( SysInfo.GotLoyaltyLink() ) ? 106 : 92;
		MoveControl( &m_checkImportProdCSVManual, 19, nYOffset  );
		MoveControl( &m_checkImportProdCSVAuto, 29, nYOffset + 14 );
	}
	/*****/
	MoveControl( &m_checkBarcodeModifier, 19, 40 );
	MoveControl( &m_checkDiscardModifiers, 19, 54 );
	MoveControl( &m_staticModifiers1, 19, 78 );
	MoveControl( &m_comboModTextEpos, 88, 78 );
	MoveControl( &m_staticModifiers2, 19, 98 );
	MoveControl( &m_comboModTextKey, 88, 98 );
	/*****/
	{
		int nYPos = ( m_bAllowModifiers ) ? 54: 40;
		MoveControl( &m_checkDeleteBarcode, 19, nYPos );
		MoveControl( &m_comboDeleteBarcode, 220, nYPos );
	}
	/*****/

	//DATABASE EDITING
	{
		m_checkTabBarcode.SetCheck( EcrmanOptions.GetPluTabBarcodeFlag() );
		m_checkTabLink.SetCheck( EcrmanOptions.GetPluTabLinkFlag() );
		m_checkTabInfo.SetCheck( EcrmanOptions.GetPluTabInfoFlag() );
		m_checkPriceSets.SetCheck( EcrmanOptions.GetPluPriceSetsFlag() );
		m_checkDownloadDate.SetCheck( EcrmanOptions.GetPluDownloadByDateFlag() );
		m_checkInactiveNoDownload.SetCheck( EcrmanOptions.GetPluInactiveNoDownloadFlag() );
		m_checkInactiveAutoHide.SetCheck( EcrmanOptions.GetPluInactiveAutoHideFlag() );
		m_checkTask.SetCheck( EcrmanOptions.GetPluTaskAfterChangesFlag() );
		
		m_comboPluText.AddString( "Plu" );
		m_comboPluText.AddString( "Plu  123" );
		m_comboPluText.AddString( "Plu  0000123" );

		int nType = EcrmanOptions.GetPluDefaultTextType();
		if ( ( nType >= 0 ) && ( nType <= 2 ) )
			m_comboPluText.SetCurSel( nType );
		else
			m_comboPluText.SetCurSel( 0 );
	}

	//IMPORT EXPORT
	m_checkImportCSVAuto.SetCheck( EcrmanOptions.GetPluCSVAutoImportFlag() );
	m_checkImportCSVManual.SetCheck( EcrmanOptions.GetPluCSVManualImportFlag() );
	m_checkExportCSVManual.SetCheck( EcrmanOptions.GetPluCSVManualExportFlag() );
	m_checkExportPurchaseAuto.SetCheck( EcrmanOptions.GetPluExportPurchaseHistoryFlag() );
	
	switch( EcrmanOptions.GetPluProdCSVImportType() )
	{
	case 2:
		m_checkImportProdCSVManual.SetCheck( TRUE );
		m_checkImportProdCSVAuto.SetCheck( TRUE );
		break;

	case 1:
		m_checkImportProdCSVManual.SetCheck( TRUE );
		m_checkImportProdCSVAuto.SetCheck( FALSE );
		break;

	case 0:
	default:
		m_checkImportProdCSVManual.SetCheck( FALSE );
		m_checkImportProdCSVAuto.SetCheck( FALSE );
		break;
	}

	//MODIFIERS
	m_checkDiscardModifiers.SetCheck( EcrmanOptions.GetPluDiscardUnusedModifierFlag() );
	m_checkBarcodeModifier.SetCheck( EcrmanOptions.GetPluBarcodeModifierFlag() );

	m_comboModTextEpos.AddString( "<Base Plu Text>" );
	m_comboModTextEpos.AddString( "<Modifier Name> <Base Plu Text>" );
	m_comboModTextEpos.SetCurSel( ( EcrmanOptions.GetPluAutoModEposTextType() == 1 ) ? 1 : 0 );

	m_comboModTextKey.AddString( "<Base Plu Text>" );
	m_comboModTextKey.AddString( "<Modifier Name> <Base Plu Text>" );
	m_comboModTextKey.SetCurSel( ( EcrmanOptions.GetPluAutoModKeyTextType() == 1 ) ? 1 : 0 );

	//BARCODES
	m_comboDeleteBarcode.AddString( "Prompt to show list" );
	m_comboDeleteBarcode.AddString( "Prompt to delete barcodes" );
	m_comboDeleteBarcode.AddString( "Delete barcodes immediately" );

	int nType = EcrmanOptions.GetPluAutoDeleteBarcodeType();
	if ( ( nType < 1 ) || ( nType > 3 ) )
	{
		m_checkDeleteBarcode.SetCheck( FALSE );
		m_comboDeleteBarcode.EnableWindow( FALSE );
		m_comboDeleteBarcode.SetCurSel(0);
	}
	else
	{
		m_checkDeleteBarcode.SetCheck( TRUE );
		m_comboDeleteBarcode.EnableWindow( TRUE );
		m_comboDeleteBarcode.SetCurSel(nType - 1);
	}

	m_nActiveTab = 0;
	SetTabControls( FALSE );
		
	return TRUE;  
}

/**********************************************************************/

void CPropPageOptionsPlu::OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	int nTab = m_TabActions.GetCurSel();
	if ( ( nTab < 0 ) || ( nTab >= m_arrayTabTypes.GetSize() ) )
		nTab = 0;

	m_nActiveTab = m_arrayTabTypes.GetAt( nTab );

	SetTabControls( FALSE );
}

/**********************************************************************/

void CPropPageOptionsPlu::SetActiveTabControls()
{
	SetTabControls( TRUE );
};

/**********************************************************************/

void CPropPageOptionsPlu::SetTabControls( bool bActiveTabOnly )
{
	bool bEcrLite = SysInfo.IsEcrLiteSystem();

	if ( ( FALSE == bActiveTabOnly ) || ( 0 == m_nActiveTab ) )
	{
		bool bEnable1 = ( m_checkTask.GetCheck() != 0 );
		ShowAndEnableWindow( &m_checkTabBarcode,			( 0 == m_nActiveTab ) );
		ShowAndEnableWindow( &m_checkTabLink,				( 0 == m_nActiveTab ) );
		ShowAndEnableWindow( &m_checkTabInfo,				( 0 == m_nActiveTab ) );
		ShowAndEnableWindow( &m_checkDownloadDate,			( 0 == m_nActiveTab ) );
		ShowAndEnableWindow( &m_checkPriceSets,				( ( 0 == m_nActiveTab ) & (!bEcrLite) ) );
		ShowAndEnableWindow( &m_checkInactiveNoDownload,	( 0 == m_nActiveTab ) );
		ShowAndEnableWindow( &m_checkInactiveAutoHide,		( 0 == m_nActiveTab ) );
		ShowAndEnableWindow( &m_checkTask,					( 0 == m_nActiveTab ) );
		ShowAndEnableWindow( &m_buttonTask,					( 0 == m_nActiveTab ), bEnable1 );
		ShowAndEnableWindow( &m_staticEditing1,				( 0 == m_nActiveTab ) );
		ShowAndEnableWindow( &m_comboPluText,				( 0 == m_nActiveTab ) );
	}

	if ( ( FALSE == bActiveTabOnly ) || ( 1 == m_nActiveTab ) )
	{
		bool bPurchase = ( SysInfo.GotLoyaltyLink() );
		bool bEnable1 = ( m_checkImportProdCSVManual.GetCheck() != 0 );
		ShowAndEnableWindow( &m_checkImportCSVAuto,			( 1 == m_nActiveTab ) );
		ShowAndEnableWindow( &m_checkImportCSVManual,		( 1 == m_nActiveTab ) );
		ShowAndEnableWindow( &m_checkExportCSVManual,		( 1 == m_nActiveTab ) );
		ShowAndEnableWindow( &m_checkExportPurchaseAuto,	( 1 == m_nActiveTab ) & bPurchase );
		ShowAndEnableWindow( &m_checkImportProdCSVManual,	( 1 == m_nActiveTab ) );
		ShowAndEnableWindow( &m_checkImportProdCSVAuto,		( 1 == m_nActiveTab ), bEnable1 );
	}
	
	if ( ( FALSE == bActiveTabOnly ) || ( 2 == m_nActiveTab ) )
	{
		ShowAndEnableWindow( &m_checkDiscardModifiers,		( 2 == m_nActiveTab ) );
		ShowAndEnableWindow( &m_checkBarcodeModifier,		m_bAllowModifiers && ( ( 2 == m_nActiveTab ) || ( 3 == m_nActiveTab ) ) );
		ShowAndEnableWindow( &m_staticModifiers1,			( 2 == m_nActiveTab ) );
		ShowAndEnableWindow( &m_comboModTextEpos,			( 2 == m_nActiveTab ) );
		ShowAndEnableWindow( &m_staticModifiers2,			( 2 == m_nActiveTab ) );
		ShowAndEnableWindow( &m_comboModTextKey,			( 2 == m_nActiveTab ) );
	}
	
	if ( ( FALSE == bActiveTabOnly ) || ( 3 == m_nActiveTab ) )
	{
		bool bEnable1 = ( m_checkDeleteBarcode.GetCheck() != 0 );
		ShowAndEnableWindow( &m_checkBarcodeModifier,		m_bAllowModifiers && ( ( 2 == m_nActiveTab ) || ( 3 == m_nActiveTab ) ) );
		ShowAndEnableWindow( &m_checkDeleteBarcode,			( 3 == m_nActiveTab ) );
		ShowAndEnableWindow( &m_comboDeleteBarcode,			( 3 == m_nActiveTab ), bEnable1 );
	}
}

/**********************************************************************/

void CPropPageOptionsPlu::OnEditTask()
{
	CEposTaskDatabase UserTasks;

	if ( UserTasks.ReadPluChangesTask() == FALSE )
		Prompter.Error( "Unable to open task file" );
	else
	{
		CEposTaskDatabase EditableTask;
		UserTasks.LoadEditableTask( EditableTask, 255 );

		CJobListDlg dlg ( EditableTask, FALSE, this );
		
		if ( dlg.DoModal() == IDOK )
		{
			UserTasks.SaveEditableTask( EditableTask );

			if ( UserTasks.WritePluChangesTask() == FALSE )
				Prompter.Error( "Unable to save task file" );
		}
	}
}

/**********************************************************************/

BOOL CPropPageOptionsPlu::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageOptionsPlu::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPageOptionsPlu::GetRecordData()
{
}

/**********************************************************************/

void CPropPageOptionsPlu::SaveRecord()
{
	//DATABASE EDITING
	EcrmanOptions.SetPluTabBarcodeFlag( m_checkTabBarcode.GetCheck() != 0 );
	EcrmanOptions.SetPluTabLinkFlag( m_checkTabLink.GetCheck() != 0 );
	EcrmanOptions.SetPluTabInfoFlag( m_checkTabInfo.GetCheck() != 0 );
	EcrmanOptions.SetPluPriceSetsFlag ( m_checkPriceSets.GetCheck() != 0 );
	EcrmanOptions.SetPluDownloadByDateFlag( m_checkDownloadDate.GetCheck() != 0 );
	EcrmanOptions.SetPluInactiveNoDownloadFlag( m_checkInactiveNoDownload.GetCheck() != 0 );
	EcrmanOptions.SetPluInactiveAutoHideFlag( m_checkInactiveAutoHide.GetCheck() != 0 );
	EcrmanOptions.SetPluTaskAfterChangesFlag( m_checkTask.GetCheck() != 0 );
	EcrmanOptions.SetPluDefaultTextType( m_comboPluText.GetCurSel() );

	if ( SysInfo.IsEcrLiteSystem() == TRUE )
		EcrmanOptions.SetPluPriceSetsFlag( FALSE );

	//IMPORT EXPORT
	{
		EcrmanOptions.SetPluCSVAutoImportFlag ( m_checkImportCSVAuto.GetCheck() != 0 );
		EcrmanOptions.SetPluCSVManualImportFlag ( m_checkImportCSVManual.GetCheck() != 0 );
		EcrmanOptions.SetPluCSVManualExportFlag ( m_checkExportCSVManual.GetCheck() != 0 );
		EcrmanOptions.SetPluExportPurchaseHistoryFlag( m_checkExportPurchaseAuto.GetCheck() != 0 );

		int nProdCSVType = 0;
		if ( m_checkImportProdCSVManual.GetCheck() != 0 )
			nProdCSVType = ( ( m_checkImportProdCSVAuto.GetCheck() == 0 ) ? 1 : 2 );
			
		EcrmanOptions.SetPluProdCSVImportType( nProdCSVType );
	}

	//MODIFIERS
	if ( TRUE == m_bAllowModifiers )
	{
		EcrmanOptions.SetPluDiscardUnusedModifierFlag( m_checkDiscardModifiers.GetCheck() != 0 );
		EcrmanOptions.SetPluBarcodeModifierFlag( m_checkBarcodeModifier.GetCheck() != 0 );
		EcrmanOptions.SetPluAutoModEposTextType( m_comboModTextEpos.GetCurSel() );
		EcrmanOptions.SetPluAutoModKeyTextType( m_comboModTextKey.GetCurSel() );
	}

	//BARCODES
	if ( m_checkDeleteBarcode.GetCheck() == 0 )
		EcrmanOptions.SetPluAutoDeleteBarcodeType(0);
	else
		EcrmanOptions.SetPluAutoDeleteBarcodeType( m_comboDeleteBarcode.GetCurSel() + 1 );
}

/**********************************************************************/
