/**********************************************************************/
#include "ButtonDefines.h"
#include "ButtonManager.h"
#include "ButtonOptions.h"
#include "ColourEditDlg.h"
#include "Consolidation.h"
#include "DatabaseCSVArray.h"
#include "EposTaskDatabase.h"
#include "FileRemover.h"
#include "KeyboardSetCSVArray.h"
#include "LocationCSVArray.h"
#include "PresetReportManager.h"
#include "..\SPosStockManager5\StockOptionsIni.h"
#include "SysInfo.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "ButtonEditDlg.h"
/**********************************************************************/

CButtonEditDlg::CButtonEditDlg( CButtonInfo& infoDisplay, int nMode, CWnd* pParent)
	: CSSDialog(CButtonEditDlg::IDD, pParent), m_infoOriginal( infoDisplay )
{
	//{{AFX_DATA_INIT(CButtonEditDlg)
	//}}AFX_DATA_INIT

	m_bGotOriginalImage = FALSE;
	m_TaskDatabase.Read();

	switch( nMode )
	{
	case BUTTON_EDIT_COPYALL:
	case BUTTON_EDIT_COPYSTYLE:
		m_nMode = nMode;
		ButtonManager.SetButtonEditDlg( this );
		break;

	case BUTTON_EDIT_NORMAL:
	default:
		m_nMode = BUTTON_EDIT_NORMAL;
		break;
	}

	m_buttonColour.SetTransparentFlag( infoDisplay.GetImageTransparentFlag() );
			
	m_staticLineID[0] = IDC_STATIC_LINE1;
	m_staticLineID[1] = IDC_STATIC_LINE2;
	m_staticLineID[2] = IDC_STATIC_LINE3;
	m_editLineID[0] = IDC_EDIT_LINE1;
	m_editLineID[1] = IDC_EDIT_LINE2;
	m_editLineID[2] = IDC_EDIT_LINE3;
	
	m_nButtonType = BUTTON_TYPE_UNUSED;
	m_nMenuNo = 0;
	m_nDetail1 = 0;
	m_nDetail2 = 0;
	m_nDetail3 = 0;

	m_nOriginalMenuNo = 0;
	m_nOriginalButtonType = 0;
	m_nOriginalDetail1 = 0;
	m_nOriginalDetail2 = 0;
	m_nOriginalDetail3 = 0;
	m_strOriginalLine[0] = "";
	m_strOriginalLine[1] = "";
	m_strOriginalLine[2] = "";

	CDataManagerInfo info;
	PresetReportManager.Open( NODE_PRESET_PLU_HIST, DB_READONLY, info );
	PresetReportManager.Open( NODE_PRESET_EPOS, DB_READONLY, info );
	PresetReportManager.Open( NODE_PRESET_CUST_HIST, DB_READONLY, info );
	PresetReportManager.Open( NODE_PRESET_TIMEATTEND, DB_READONLY, info );

#ifdef STOCKMAN_SYSTEM
	PresetReportManager.Open( NODE_PRESET_STOCK_HIST, DB_READONLY, info );
	PresetReportManager.Open( NODE_PRESET_STOCK_LEVEL, DB_READONLY, info );
	PresetReportManager.Open( NODE_PRESET_STOCK_AUDIT, DB_READONLY, info );
#else
	PresetReportManager.Open( NODE_PRESET_ECR_STOCK, DB_READONLY, info );
#endif

	CreatePresetList( PresetReportManager.Epos, m_listPresetsEpos );
	CreatePresetList( PresetReportManager.PluHist, m_listPresetsHistoryPlu );
	CreatePresetList( PresetReportManager.CustHist, m_listPresetsHistoryCustomer );
	CreatePresetList( PresetReportManager.TimeAttend, m_listPresetsTimeAttend );

#ifdef STOCKMAN_SYSTEM
	CreatePresetList( PresetReportManager.StockHist, m_listPresetsHistoryStock );
	CreatePresetList( PresetReportManager.StockLevel, m_listPresetsStockLevel );
	CreatePresetList( PresetReportManager.StockAudit, m_listPresetsStockAudit );
#else
	CreatePresetList( PresetReportManager.EcrStock, m_listPresetsEcrmanStock );
#endif

	PresetReportManager.Close( NODE_PRESET_PLU_HIST, info );
	PresetReportManager.Close( NODE_PRESET_EPOS, info );
	PresetReportManager.Close( NODE_PRESET_CUST_HIST, info );
	PresetReportManager.Close( NODE_PRESET_TIMEATTEND, info );

#ifdef STOCKMAN_SYSTEM
	PresetReportManager.Close( NODE_PRESET_STOCK_HIST, info );
	PresetReportManager.Close( NODE_PRESET_STOCK_LEVEL, info );
	PresetReportManager.Close( NODE_PRESET_STOCK_AUDIT, info );
#else
	PresetReportManager.Close( NODE_PRESET_ECR_STOCK, info );
#endif
}

/**********************************************************************/

CButtonEditDlg::~CButtonEditDlg()
{
	CString strFilename;
	m_infoOriginal.GetButtonImageFilename( m_infoOriginal.GetListNo(), 2, strFilename );
	CFileRemover FileRemover( strFilename );
}

/**********************************************************************/

void CButtonEditDlg::CreatePresetList( CPresetReportArray& arrayPreset, CArray<CSortedIntAndStringItem,CSortedIntAndStringItem>& listPresets ) 
{
	for ( int nPresetIdx = 0; nPresetIdx < arrayPreset.GetSize(); nPresetIdx++ )
	{
		int nEntityNo;
		CString strListName;
		arrayPreset.GetEntityNoAndListName( nPresetIdx, nEntityNo, strListName ); 

		if ( nEntityNo != 0 )
		{
			CSortedIntAndStringItem item;
			item.m_nItem = nEntityNo;
			item.m_strItem = strListName;
			listPresets.Add( item );
		}
	}
}

/**********************************************************************/

void CButtonEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CButtonEditDlg)
	//}}AFX_DATA_MAP

	for ( int i = 0; i < 3; i++ )
	{
		DDX_Control(pDX, m_staticLineID[i], m_staticLine[i]);
		DDX_Control(pDX, m_editLineID[i], m_editLine[i]);
	}

	DDX_Control(pDX, IDC_STATIC_TYPE, m_staticType);
	DDX_Control(pDX, IDC_RADIO_UNUSED, m_radioUnused);
	DDX_Control(pDX, IDC_RADIO_MENU, m_radioMenu);
	DDX_Control(pDX, IDC_RADIO_ACTION, m_radioAction);

	DDX_Control(pDX, IDC_STATIC_COLOUR, m_staticColour);
	DDX_Control(pDX, IDC_CHECK_COLOUR, m_checkColour);
	DDX_Control(pDX, IDC_BUTTON_COLOUR, m_buttonColour );
	DDX_Control(pDX, IDC_BUTTON_BACK, m_buttonBack );
	DDX_Control(pDX, IDC_BUTTON_IMAGE, m_buttonImage );
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_buttonClear );
	DDX_Control(pDX, IDC_BUTTON_TEXT, m_buttonText );
	DDX_Control(pDX, IDC_STATIC_FONT, m_staticFont);
	DDX_Control(pDX, IDC_COMBO_FONT, m_comboFont);
	DDX_Control(pDX, IDC_CHECK_TRANSPARENT, m_checkTransparent);

	DDX_Control(pDX, IDC_STATIC_DETAIL, m_staticDetail);
	DDX_Control(pDX, IDC_STATIC_DETAIL1, m_staticDetail1);
	DDX_Control(pDX, IDC_COMBO_DETAIL1, m_comboDetail1);
	DDX_Control(pDX, IDC_STATIC_DETAIL2, m_staticDetail2);
	DDX_Control(pDX, IDC_COMBO_DETAIL2, m_comboDetail2);
	DDX_Control(pDX, IDC_COMBO_DETAIL3, m_comboDetail3);
	DDX_Control(pDX, IDC_CHECK_PRESET, m_checkPreset);
	DDX_Control(pDX, IDC_COMBO_MENU, m_comboMenu);
	DDX_Control(pDX, IDC_COMBO_UNUSED, m_comboUnused);
	DDX_Control(pDX, IDC_STATIC_AUTO, m_staticAuto);
	DDX_Control(pDX, IDC_COMBO_AUTO, m_comboAuto);
	
	DDX_Control(pDX, IDC_STATIC_TEXT, m_staticText);
	DDX_Control(pDX, IDOK, m_buttonOK );
	DDX_Control(pDX, IDCANCEL, m_buttonCancel );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CButtonEditDlg, CDialog)
	//{{AFX_MSG_MAP(CButtonEditDlg
	ON_BN_CLICKED( IDC_RADIO_UNUSED, OnRadioUnused )
	ON_BN_CLICKED( IDC_RADIO_MENU, OnRadioMenu )
	ON_BN_CLICKED( IDC_RADIO_ACTION, OnRadioAction )
	ON_BN_CLICKED( IDC_CHECK_COLOUR, OnToggleOwnColour )
	ON_BN_CLICKED( IDC_BUTTON_BACK, OnButtonBackColour )
	ON_BN_CLICKED( IDC_BUTTON_TEXT, OnButtonTextColour )
	ON_BN_CLICKED( IDC_BUTTON_IMAGE, OnButtonImage )
	ON_BN_CLICKED( IDC_BUTTON_CLEAR, OnButtonClear )
	ON_BN_CLICKED( IDC_CHECK_TRANSPARENT, OnToggleTransparent )
	ON_CBN_SELCHANGE( IDC_COMBO_FONT, OnSelectFont )
	ON_CBN_SELCHANGE( IDC_COMBO_MENU, OnSelectMenu )
	ON_CBN_SELCHANGE( IDC_COMBO_DETAIL1, OnSelectDetail1 )
	ON_CBN_SELCHANGE( IDC_COMBO_DETAIL2, OnSelectDetail2 )
	ON_CBN_SELCHANGE( IDC_COMBO_DETAIL3, OnSelectDetail3 )
	ON_MESSAGE ( WM_COLOUR_CHANGE, OnColourButtonMessage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CButtonEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	m_buttonColour.m_bBlackBorder = TRUE;
	m_buttonColour.m_strText = "ABC";

	if ( m_nMode != BUTTON_EDIT_COPYSTYLE )
	{		
		m_strImageImportPath = "";
		GetDataProgramPath( m_strImageImportPath );
		m_strNewImageType = "";

		int nListNo = m_infoOriginal.GetListNo();

		CString strImagePath = "";
		if ( m_infoOriginal.GetButtonImageFilename( nListNo, 1, strImagePath ) == FALSE )
			m_infoOriginal.GetButtonImageFilename( nListNo, 0, strImagePath );

		if ( strImagePath != "" )	
		{
			m_buttonColour.LoadImage( strImagePath );
			m_bGotOriginalImage = TRUE;
		}
	
		if ( BUTTON_EDIT_COPYALL == m_nMode )
		{
			if ( strImagePath == "" )
				m_strNewImageType = "XXX";
			else
			{
				m_strNewImageType = m_infoOriginal.GetImageTypeTemp();
				
				if ( m_strNewImageType == "" )
					m_strNewImageType = m_infoOriginal.GetImageType();

				CString strTempFilename = "";
				m_infoOriginal.GetButtonImageFilename( nListNo, 2, strTempFilename );
				::CopyFile( strImagePath, strTempFilename, FALSE );
			}

			m_bGotOriginalImage = TRUE;
		}
	}

	{
		int nSize = 8;
		for ( int nPos = 0; nPos <= 11; nPos++ )
		{
			CString strSize;
			strSize.Format( "%d", nSize );
			m_comboFont.AddString( strSize );
			m_comboFont.SetItemData( nPos, nSize );
			nSize += 2;
		}
	}

	MoveControl( &m_comboUnused, 291, 30 );
	MoveControl( &m_comboMenu, 291, 30 );

	m_comboUnused.AddString( "Unused" );
	m_comboUnused.SetCurSel(0);

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(35);
	m_comboMenu.SetTabStops( arrayStops );
	
	for ( int n = 1; n <= BUTMGR_MAX_LIST_NO; n++ )
	{
		CString strNum;
		strNum.Format( "%2.2d", n );

		CCSV csv;
		csv.Add( strNum );
		csv.Add( ButtonManager.GetListName(n) );
		m_comboMenu.AddItem( csv.GetLine() );
		m_comboMenu.SetItemData( n - 1 , n );
	}

	m_comboDetail1.AddString( "Configure System" );

	if ( SysInfo.GotLoyaltyOrSmartPayLink() )
	{
		m_comboDetail1.AddString( "External Program" );
	}

	m_comboDetail1.AddString( "Edit Epos Data" );
	m_comboDetail1.AddString( "Edit Keyboards" );
	m_comboDetail1.AddString( "Edit Price Sets (Location)" );
	m_comboDetail1.AddString( "Edit Price Sets (Database)" );
#ifdef STOCKMAN_SYSTEM
	m_comboDetail1.AddString( "Edit Stock Data" );
	m_comboDetail1.AddString( "Process Stock" );
#endif
	m_comboDetail1.AddString( "Manage Epos Tasks" );
	m_comboDetail1.AddString( "View Reports" );

	int nPos = 0;
	m_comboDetail1.SetItemData( nPos++, BUTTON_DETAIL1_SETUP );

	if ( SysInfo.GotLoyaltyOrSmartPayLink() )
	{
		m_comboDetail1.SetItemData( nPos++, BUTTON_DETAIL1_EXTERNAL );
	}

	m_comboDetail1.SetItemData( nPos++, BUTTON_DETAIL1_EDITEPOS );
	m_comboDetail1.SetItemData( nPos++, BUTTON_DETAIL1_KEYBOARD );
	m_comboDetail1.SetItemData( nPos++, BUTTON_DETAIL1_PSET_LOC );
	m_comboDetail1.SetItemData( nPos++, BUTTON_DETAIL1_PSET_DB );
#ifdef STOCKMAN_SYSTEM
	m_comboDetail1.SetItemData( nPos++, BUTTON_DETAIL1_EDITSTK );
	m_comboDetail1.SetItemData( nPos++, BUTTON_DETAIL1_MOVESTK );
#endif
	m_comboDetail1.SetItemData( nPos++, BUTTON_DETAIL1_TASKS );
	m_comboDetail1.SetItemData( nPos++, BUTTON_DETAIL1_REPORTS );

	m_editLine[0].LimitText( BUTMGR_MAX_TEXTLEN );
	m_editLine[1].LimitText( BUTMGR_MAX_TEXTLEN );
	m_editLine[2].LimitText( BUTMGR_MAX_TEXTLEN );

	CenterWindow();

	switch( m_nMode )
	{
	case BUTTON_EDIT_COPYSTYLE:
		SetCopyStyleControls();
		break;

	case BUTTON_EDIT_COPYALL:
		SetCopyAllControls();
		break;

	case BUTTON_EDIT_NORMAL:
	default:
		ShowAndEnableWindow( &m_staticAuto, FALSE );
		ShowAndEnableWindow( &m_comboAuto, FALSE );
		break;
	}

	SetStyleControlsFromButton( m_infoOriginal );

	if ( BUTTON_EDIT_COPYSTYLE != m_nMode )
		SetActionControlsFromButton( m_infoOriginal );
	else
	{
		m_nButtonType = m_infoOriginal.GetButtonType();
		OnToggleOwnColour();
	}

	return TRUE;  
}

/**********************************************************************/

void CButtonEditDlg::SetCopyStyleControls()
{
	SetWindowText( "Copy Button Style" );
	ShowAndEnableWindow( &m_staticType, FALSE );
	ShowAndEnableWindow( &m_radioUnused, FALSE );
	ShowAndEnableWindow( &m_radioMenu, FALSE );
	ShowAndEnableWindow( &m_radioAction, FALSE );
	MoveControl( &m_staticColour, 7, 7 );
	MoveControl( &m_checkColour, 17, 27 );
	MoveControl( &m_buttonColour, 61, 27 );
	MoveControl( &m_buttonBack, 157, 71 );
	MoveControl( &m_buttonText, 157, 89 );
	MoveControl( &m_staticFont, 17, 122 );
	MoveControl( &m_comboFont, 61, 120 );
	ShowAndEnableWindow( &m_staticDetail, FALSE );
	ShowAndEnableWindow( &m_staticDetail1, FALSE );
	ShowAndEnableWindow( &m_comboDetail1, FALSE );
	ShowAndEnableWindow( &m_staticDetail2, FALSE );
	ShowAndEnableWindow( &m_comboDetail2, FALSE );
	ShowAndEnableWindow( &m_comboDetail3, FALSE );
	ShowAndEnableWindow( &m_staticAuto, FALSE );
	ShowAndEnableWindow( &m_comboAuto, FALSE );
	ShowAndEnableWindow( &m_comboMenu, FALSE );
	ShowAndEnableWindow( &m_comboUnused, FALSE );
	ShowAndEnableWindow( &m_staticText, FALSE );
	ShowAndEnableWindow( &m_staticLine[0], FALSE );
	ShowAndEnableWindow( &m_staticLine[1], FALSE );
	ShowAndEnableWindow( &m_staticLine[2], FALSE );
	ShowAndEnableWindow( &m_editLine[0], FALSE );
	ShowAndEnableWindow( &m_editLine[1], FALSE );
	ShowAndEnableWindow( &m_editLine[2], FALSE );
	ShowAndEnableWindow( &m_buttonImage, FALSE );
	ShowAndEnableWindow( &m_buttonClear, FALSE );
	ShowAndEnableWindow( &m_checkTransparent, FALSE );
	ShowAndEnableWindow( &m_buttonOK, FALSE );
	m_buttonCancel.SetWindowText( "Close" );
	MoveControl( &m_buttonCancel, 155, 170 );
	ResizeDialog( 212, 191 );
}

/**********************************************************************/

void CButtonEditDlg::SetCopyAllControls()
{
	m_nMode = BUTTON_EDIT_COPYALL;
	SetWindowText( "Copy Button" );
	ShowAndEnableWindow( &m_staticType, FALSE );
	ShowAndEnableWindow( &m_radioUnused, FALSE );
	ShowAndEnableWindow( &m_radioMenu, FALSE );
	ShowAndEnableWindow( &m_radioAction, FALSE );
	MoveControl( &m_staticColour, 7, 7 );
	MoveControl( &m_checkColour, 17, 27 );
	MoveControl( &m_buttonColour, 61, 27 );
	MoveControl( &m_buttonBack, 157, 71 );
	MoveControl( &m_buttonText, 157, 89 );
	MoveControl( &m_buttonImage, 17, 71 );
	MoveControl( &m_buttonClear, 17, 89 );
	MoveControl( &m_checkTransparent, 61, 107 );
	MoveControl( &m_staticFont, 17, 130 );
	MoveControl( &m_comboFont, 61, 132 );
	ResizeControl( &m_staticDetail, 262, 157 );
	ShowAndEnableWindow( &m_staticAuto, TRUE );
	ShowAndEnableWindow( &m_comboAuto, TRUE );

	switch( m_infoOriginal.GetButtonType() )
	{
	case BUTTON_TYPE_LIST:
		m_comboAuto.ResetContent();
		m_comboAuto.AddString( "None" );
		m_comboAuto.AddString( "Next List" );
		m_staticAuto.ShowWindow( SW_SHOW );
		m_comboAuto.ShowWindow( SW_SHOW );
		break;

	case BUTTON_TYPE_ACTION:
		m_comboAuto.ResetContent();
		m_comboAuto.AddString( "None" );
		m_comboAuto.AddString( "Action Detail (Upper)" );
		m_comboAuto.AddString( "Action Detail (Lower)" );	
		m_staticAuto.ShowWindow( SW_SHOW );
		m_comboAuto.ShowWindow( SW_SHOW );
		break;

	case BUTTON_TYPE_UNUSED:
	default:
		m_comboAuto.ResetContent();
		m_comboAuto.AddString( "None" );
		m_staticAuto.ShowWindow( SW_HIDE );
		m_comboAuto.ShowWindow( SW_HIDE );
		break;
	}

	MoveControl( &m_staticAuto, 221, 122 );
	MoveControl( &m_comboAuto, 291, 120 );
	m_comboAuto.SetCurSel(0);

	ShowAndEnableWindow( &m_staticDetail, TRUE );
	ShowAndEnableWindow( &m_staticDetail1, TRUE );
	ShowAndEnableWindow( &m_comboDetail1, TRUE );
	ShowAndEnableWindow( &m_staticDetail2, TRUE );
	ShowAndEnableWindow( &m_comboDetail2, TRUE );
	ShowAndEnableWindow( &m_comboDetail3, TRUE );
	ShowAndEnableWindow( &m_checkPreset, TRUE );
	ShowAndEnableWindow( &m_comboMenu, TRUE );
	ShowAndEnableWindow( &m_comboUnused, TRUE );
	ShowAndEnableWindow( &m_staticText, FALSE );
	ShowAndEnableWindow( &m_staticLine[0], FALSE );
	ShowAndEnableWindow( &m_staticLine[1], FALSE );
	ShowAndEnableWindow( &m_staticLine[2], FALSE );
	ShowAndEnableWindow( &m_editLine[0], FALSE );
	ShowAndEnableWindow( &m_editLine[1], FALSE );
	ShowAndEnableWindow( &m_editLine[2], FALSE );
	ShowAndEnableWindow( &m_buttonOK, FALSE );
	m_buttonCancel.SetWindowText( "Close" );
	MoveControl( &m_buttonCancel, 423, 170 );
	ResizeDialog( 480, 191 );
}

/**********************************************************************/

void CButtonEditDlg::SetStyleControlsFromButton( CButtonInfo& infoButton )
{
	m_checkColour.SetCheck( infoButton.GetOwnColourFlag() );
	m_buttonColour.m_ColourRefBack = infoButton.GetDisplayBackColour();
	m_buttonColour.m_ColourRefText = infoButton.GetDisplayTextColour();
	int nFontSize = infoButton.GetFontSize();
	SelectComboItem( m_comboFont, nFontSize );
	OnSelectFont();
	m_checkTransparent.SetCheck( infoButton.GetImageTransparentFlag() );
}

/**********************************************************************/

void CButtonEditDlg::SetActionControlsFromButton( CButtonInfo& infoButton )
{
	m_nButtonType = BUTTON_TYPE_UNUSED;
	m_nMenuNo = 0;
	m_nDetail1 = 0;
	m_nDetail2 = 0;
	m_nDetail3 = 0;

	switch( infoButton.GetButtonType() )
	{
	case BUTTON_TYPE_LIST:
		m_nButtonType = BUTTON_TYPE_LIST;
		m_nMenuNo = infoButton.GetDetail1();
		OnRadioMenu();
		break;

	case BUTTON_TYPE_ACTION:
		m_nButtonType = BUTTON_TYPE_ACTION;
		m_nDetail1 = infoButton.GetDetail1();
		m_nDetail2 = infoButton.GetDetail2();
		m_nDetail3 = infoButton.GetDetail3();
		OnRadioAction();
		break;

	case BUTTON_TYPE_UNUSED:
		OnRadioUnused();
		break;
	}

	SelectComboItem( m_comboMenu, m_nMenuNo );
	SelectComboItem( m_comboDetail1, m_nDetail1 );
	FillDetail2Combo();
	SelectComboItem( m_comboDetail2, m_nDetail2 );
	FillDetail3Combo();
	SelectComboItem( m_comboDetail3, m_nDetail3 );
	ShowOrHideDetail3();

	m_checkPreset.SetCheck( infoButton.GetActionFlag1() == FALSE );

	CStringArray arrayTexts;
	infoButton.GetTextLines( arrayTexts );
	int nSize = arrayTexts.GetSize();

	CString strLine0 = "";
	CString strLine1 = "";
	CString strLine2 = "";
	if (nSize > 0) strLine0 = arrayTexts.GetAt(0);
	if (nSize > 1) strLine1 = arrayTexts.GetAt(1);
	if (nSize > 2) strLine2 = arrayTexts.GetAt(2);
	
	if ( ( strLine0 == "" ) && ( strLine1 == "" ) && ( strLine2 == "" ) )
	{
		infoButton.SetDefaultTexts();
		infoButton.GetTextLines( arrayTexts );
		int nSize = arrayTexts.GetSize();

		strLine0 = "";
		strLine1 = "";
		strLine2 = "";
		if (nSize > 0) strLine0 = arrayTexts.GetAt(0);
		if (nSize > 1) strLine1 = arrayTexts.GetAt(1);
		if (nSize > 2) strLine2 = arrayTexts.GetAt(2);
	}

	m_editLine[0].SetWindowText( strLine0 );
	m_editLine[1].SetWindowText( strLine1 );
	m_editLine[2].SetWindowText( strLine2 );

	m_nOriginalMenuNo = m_nMenuNo;
	m_nOriginalButtonType = m_nButtonType;
	m_nOriginalDetail1 = m_nDetail1;
	m_nOriginalDetail2 = m_nDetail2;
	m_nOriginalDetail3 = m_nDetail3;
	m_strOriginalLine[0] = strLine0;
	m_strOriginalLine[1] = strLine1;
	m_strOriginalLine[2] = strLine2;
}

/**********************************************************************/
void CButtonEditDlg::OnRadioUnused(){ SelectButtonType( BUTTON_TYPE_UNUSED ); }
void CButtonEditDlg::OnRadioMenu(){ SelectButtonType( BUTTON_TYPE_LIST ); }
void CButtonEditDlg::OnRadioAction(){ SelectButtonType( BUTTON_TYPE_ACTION ); }
/**********************************************************************/

void CButtonEditDlg::SelectComboItem( CComboBox& combo, int& nTarget )
{
	int nSel = 0;
	for ( int n = 0; n < combo.GetCount(); n++ )
	{
		if ( combo.GetItemData(n) == nTarget )
		{
			nSel = n;
			break;
		}
	}

	combo.SetCurSel( nSel );
	nTarget = combo.GetItemData( nSel );
}

/**********************************************************************/

void CButtonEditDlg::SelectButtonType( int nType )
{
	int nCurrentButtonType = m_nButtonType;

	switch( nType )
	{
	case BUTTON_TYPE_UNUSED:
	case BUTTON_TYPE_LIST:
	case BUTTON_TYPE_ACTION:
		{
			m_nButtonType = nType;
			
			m_radioUnused.SetCheck( BUTTON_TYPE_UNUSED == nType );
			m_radioMenu.SetCheck( BUTTON_TYPE_LIST == nType );
			m_radioAction.SetCheck( BUTTON_TYPE_ACTION == nType );
			
			ShowAndEnableWindow( &m_comboUnused, BUTTON_TYPE_UNUSED == nType, FALSE );
			ShowAndEnableWindow( &m_comboMenu, BUTTON_TYPE_LIST == nType );
			ShowAndEnableWindow( &m_comboDetail1, BUTTON_TYPE_ACTION == nType );
			ShowAndEnableWindow( &m_comboDetail2, BUTTON_TYPE_ACTION == nType );
			ShowAndEnableWindow( &m_comboDetail3, BUTTON_TYPE_ACTION == nType );
			ShowAndEnableWindow( &m_staticDetail2, BUTTON_TYPE_ACTION == nType );
			ShowAndEnableWindow( &m_checkPreset, BUTTON_TYPE_ACTION == nType );

			if ( BUTTON_EDIT_NORMAL == m_nMode )
			{
				for ( int n = 0; n < 3; n++ )
					m_editLine[n].EnableWindow( BUTTON_TYPE_UNUSED != nType );
			}

			m_comboFont.EnableWindow( BUTTON_TYPE_UNUSED != nType );

			m_staticDetail1.SetWindowText( BUTTON_TYPE_LIST == nType ? "List" : "Action type" );

			ShowOrHideDetail3();
		}
		break;
	}

	m_checkColour.EnableWindow( nType != BUTTON_TYPE_UNUSED );
	m_buttonImage.EnableWindow( nType != BUTTON_TYPE_UNUSED );
	m_buttonClear.EnableWindow( nType != BUTTON_TYPE_UNUSED );
	OnToggleOwnColour();

	if ( nCurrentButtonType != m_nButtonType )
		UpdateText();
}

/**********************************************************************/

void CButtonEditDlg::OnToggleOwnColour()
{
	if ( m_checkColour.GetCheck() == 0 )
	{
		switch( m_nButtonType )
		{
		case BUTTON_TYPE_LIST:		
			m_buttonColour.m_ColourRefBack = ButtonOptions.GetButtonColourBackListInt();
			m_buttonColour.m_ColourRefText = ButtonOptions.GetButtonColourTextListInt();
			break;

		case BUTTON_TYPE_ACTION:		
			m_buttonColour.m_ColourRefBack = ButtonOptions.GetButtonColourBackActionInt();
			m_buttonColour.m_ColourRefText = ButtonOptions.GetButtonColourTextActionInt();
			break;

		case BUTTON_TYPE_UNUSED:
		default:	
			m_buttonColour.m_ColourRefBack = ButtonOptions.GetButtonColourBackUnusedInt();
			m_buttonColour.m_ColourRefBack = ButtonOptions.GetButtonColourBackUnusedInt();
			break;
		}

		m_buttonBack.EnableWindow( FALSE );
		m_buttonText.EnableWindow( FALSE );
		m_buttonColour.Invalidate();
	}
	else
	{
		bool bEnable = ( BUTTON_EDIT_COPYSTYLE == m_nMode ) || ( m_nButtonType != BUTTON_TYPE_UNUSED );
		m_buttonBack.EnableWindow( bEnable );
		m_buttonText.EnableWindow( bEnable );
	}
}

/**********************************************************************/

void CButtonEditDlg::OnSelectMenu()
{
	int nCurrentMenuNo = m_nMenuNo;
	m_nMenuNo = m_comboMenu.GetItemData( m_comboMenu.GetCurSel() ) ;
	
	if ( nCurrentMenuNo != m_nMenuNo )
		UpdateText();
}

/**********************************************************************/

void CButtonEditDlg::OnSelectFont()
{
	m_buttonColour.m_nFontSize = ( 8 + ( m_comboFont.GetCurSel() * 2 ) );
	m_buttonColour.Invalidate();
}

/**********************************************************************/

void CButtonEditDlg::OnSelectDetail1()
{
	int nDetail1 = m_comboDetail1.GetItemData( m_comboDetail1.GetCurSel() ) ;

	if ( nDetail1 != m_nDetail1 )
	{
		m_nDetail1 = nDetail1;
		
		FillDetail2Combo();
		m_nDetail2 = -1;
		SelectComboItem( m_comboDetail2, m_nDetail2 );

		FillDetail3Combo();
		m_nDetail3 = -1;
		SelectComboItem( m_comboDetail3, m_nDetail3 );

		ShowOrHideDetail3();
		UpdateText();
	}
}

/**********************************************************************/

void CButtonEditDlg::OnSelectDetail2()
{
	int nDetail2 = m_comboDetail2.GetItemData( m_comboDetail2.GetCurSel() ) ;

	if ( nDetail2 != m_nDetail2 )
	{
		m_nDetail2 = nDetail2;

		switch( m_nDetail1 )
		{
		case BUTTON_DETAIL1_KEYBOARD:
		case BUTTON_DETAIL1_PSET_DB:
		case BUTTON_DETAIL1_PSET_LOC:
		case BUTTON_DETAIL1_TASKS:
		case BUTTON_DETAIL1_REPORTS:
#ifdef STOCKMAN_SYSTEM
		case BUTTON_DETAIL1_EDITSTK:
#endif
			{
				FillDetail3Combo();
				m_nDetail3 = -1;
				SelectComboItem( m_comboDetail3, m_nDetail3 );
				ShowOrHideDetail3();
			}
			break;
		}
		UpdateText();
	}
}

/**********************************************************************/

void CButtonEditDlg::OnSelectDetail3()
{
	int nDetail3 = m_comboDetail3.GetItemData( m_comboDetail3.GetCurSel() ) ;

	if ( nDetail3 != m_nDetail3 )
	{
		m_nDetail3 = nDetail3;
		UpdateText();
	}
}

/**********************************************************************/

void CButtonEditDlg::FillDetail2Combo()
{
	m_comboDetail2.ResetContent();
	switch( m_nDetail1 )
	{
	case BUTTON_DETAIL1_SETUP:
		m_comboDetail2.AddString( "Setup System" );
		m_comboDetail2.AddString( "Setup Options" );
		m_comboDetail2.SetItemData(0, BUTTON_DETAIL2_SETUP_SYSTEM );
		m_comboDetail2.SetItemData(1, BUTTON_DETAIL2_SETUP_OPTIONS );
		break;

	case BUTTON_DETAIL1_EDITEPOS:
		{
			m_comboDetail2.AddString("Plu Database");
			m_comboDetail2.AddString("Departments");
			m_comboDetail2.AddString("Department Groups");
			m_comboDetail2.AddString("Analysis Categories");
			m_comboDetail2.AddString("Mix Match");

			switch (SysInfo.GetLoyaltyOrSmartPayType())
			{
			case LOYALTY_TYPE_SMP_V4:
				m_comboDetail2.AddString("Purchase Control");
				break;

			case LOYALTY_TYPE_LOY_V4:
			default:
				m_comboDetail2.AddString("Stamp Offers");
				break;

			}
		
			m_comboDetail2.AddString("Offer Groups");
			m_comboDetail2.AddString("Promotions");
			m_comboDetail2.AddString("Time Plans");
			m_comboDetail2.AddString("Payment Methods");
			m_comboDetail2.AddString(EcrmanOptions.GetTaxString("%T Bands"));
			m_comboDetail2.AddString("Servers");
			m_comboDetail2.AddString("Loyalty Schemes");
			
			m_comboDetail2.SetItemData(0, BUTTON_DETAIL2_EDITEPOS_PLU);
			m_comboDetail2.SetItemData(1, BUTTON_DETAIL2_EDITEPOS_DEPT);
			m_comboDetail2.SetItemData(2, BUTTON_DETAIL2_EDITEPOS_GROUP);
			m_comboDetail2.SetItemData(3, BUTTON_DETAIL2_EDITEPOS_ACAT);
			m_comboDetail2.SetItemData(4, BUTTON_DETAIL2_EDITEPOS_MIXMATCH);
			m_comboDetail2.SetItemData(5, BUTTON_DETAIL2_EDITEPOS_WALLETSTAMPRULE);
			m_comboDetail2.SetItemData(6, BUTTON_DETAIL2_EDITEPOS_OFFERGROUP);
			m_comboDetail2.SetItemData(7, BUTTON_DETAIL2_EDITEPOS_PROMOTION);
			m_comboDetail2.SetItemData(8, BUTTON_DETAIL2_EDITEPOS_TIMEPLAN);
			m_comboDetail2.SetItemData(9, BUTTON_DETAIL2_EDITEPOS_PAYMENT);
			m_comboDetail2.SetItemData(10, BUTTON_DETAIL2_EDITEPOS_TAX);
			m_comboDetail2.SetItemData(11, BUTTON_DETAIL2_EDITEPOS_SERVER);
			m_comboDetail2.SetItemData(12, BUTTON_DETAIL2_EDITEPOS_LOYALTYSCHEME);
		}
		break;

#ifdef STOCKMAN_SYSTEM
	case BUTTON_DETAIL1_EDITSTK:
		m_comboDetail2.AddString( "Stock Database" );
		m_comboDetail2.AddString( "Suppliers" );
		m_comboDetail2.AddString( "Categories" );
		m_comboDetail2.AddString( "Custom Fields" );
		m_comboDetail2.AddString( "Allowances" );
		m_comboDetail2.AddString( "Delivery Units" );
		m_comboDetail2.AddString( "Stock Units" );
		m_comboDetail2.AddString( "Sub Units" );
		m_comboDetail2.AddString( "Adjustment Texts" );
		m_comboDetail2.SetItemData(0, BUTTON_DETAIL2_EDITSTK_STOCK );
		m_comboDetail2.SetItemData(1, BUTTON_DETAIL2_EDITSTK_SUPPLIER );
		m_comboDetail2.SetItemData(2, BUTTON_DETAIL2_EDITSTK_CATEGORY );
		m_comboDetail2.SetItemData(3, BUTTON_DETAIL2_EDITSTK_CUSTFLD );
		m_comboDetail2.SetItemData(4, BUTTON_DETAIL2_EDITSTK_ALLOWANCE );
		m_comboDetail2.SetItemData(5, BUTTON_DETAIL2_EDITSTK_DELUNIT );
		m_comboDetail2.SetItemData(6, BUTTON_DETAIL2_EDITSTK_STKUNIT );
		m_comboDetail2.SetItemData(7, BUTTON_DETAIL2_EDITSTK_SUBUNIT );
		m_comboDetail2.SetItemData(8, BUTTON_DETAIL2_EDITSTK_ADJTEXT );
		break;

	case BUTTON_DETAIL1_MOVESTK:
		m_comboDetail2.AddString( "Opening Stock" );
		m_comboDetail2.AddString( "Deliveries" );
		m_comboDetail2.AddString( "Returns" );
		m_comboDetail2.AddString( "Manual Sales (PLU)" );
		m_comboDetail2.AddString( "Manual Sales (Stock)" );
		m_comboDetail2.AddString( "Adjustments (PLU)" );
		m_comboDetail2.AddString( "Adjustments (Stock)" );
		m_comboDetail2.AddString( "Transfers" );
		m_comboDetail2.SetItemData(0, BUTTON_DETAIL2_MOVESTK_OPENING );
		m_comboDetail2.SetItemData(1, BUTTON_DETAIL2_MOVESTK_DELIVERY );
		m_comboDetail2.SetItemData(2, BUTTON_DETAIL2_MOVESTK_RETURN );
		m_comboDetail2.SetItemData(3, BUTTON_DETAIL2_MOVESTK_SALEPLU );
		m_comboDetail2.SetItemData(4, BUTTON_DETAIL2_MOVESTK_SALESTK );
		m_comboDetail2.SetItemData(5, BUTTON_DETAIL2_MOVESTK_ADJUSTPLU );
		m_comboDetail2.SetItemData(6, BUTTON_DETAIL2_MOVESTK_ADJUSTSTK );
		m_comboDetail2.SetItemData(7, BUTTON_DETAIL2_MOVESTK_TRANSFER );
		break;
		
#endif

	case BUTTON_DETAIL1_KEYBOARD:
		m_comboDetail2.AddString( "User Select Location" );
		m_comboDetail2.SetItemData(0,0);

		for ( int n = 0; n < m_DataAccessHelpersKeyboard.GetKeyboardMapSize(); n++ )
		{
			CKeyboardMapInfo infoKeyboard;
			m_DataAccessHelpersKeyboard.GetKeyboardMapInfo( n, infoKeyboard );

			switch( infoKeyboard.m_nEntryType )
			{
			case KBD_MAP_SET:
				if ( ( infoKeyboard.m_nKbsIdx >= 0 ) && ( infoKeyboard.m_nKbsIdx < dbKeyboardSet.GetSize() ) )
				{
					int nItemData = 0;
					nItemData += 1000000;
					nItemData += ( dbKeyboardSet.GetDbNo( infoKeyboard.m_nKbsIdx ) * 1000 );
					nItemData += dbKeyboardSet.GetKbsNo( infoKeyboard.m_nKbsIdx );

					int nPos = m_comboDetail2.AddString( dbKeyboardSet.GetName( infoKeyboard.m_nKbsIdx ) );
					m_comboDetail2.SetItemData( nPos, nItemData );
				}
				break;

			case KBD_MAP_LOC:
				if ( ( infoKeyboard.m_nLocIdx >= 0 ) && ( infoKeyboard.m_nLocIdx < dbLocation.GetSize() ) )
				{
					int nItemData = dbLocation.GetNetworkLocNo( infoKeyboard.m_nLocIdx );
					int nPos = m_comboDetail2.AddString( dbLocation.GetName( infoKeyboard.m_nLocIdx ) );
					m_comboDetail2.SetItemData( nPos, nItemData );
				}
				break;
			}
		}
		break;

	case BUTTON_DETAIL1_PSET_LOC:
		{
			m_comboDetail2.AddString( "User Select Location" );
			m_comboDetail2.SetItemData(0,0);

			for ( int n = 0; n < dbLocation.GetSize(); n++ )
			{
				m_comboDetail2.AddString( dbLocation.GetName(n) );
				m_comboDetail2.SetItemData( n + 1, dbLocation.GetNetworkLocNo(n) );
			}
		}
		break;

	case BUTTON_DETAIL1_PSET_DB:
		{
			m_comboDetail2.AddString( "User Select Database" );
			m_comboDetail2.SetItemData(0,0);

			for ( int n = 0; n < dbDatabase.GetSize(); n++ )
			{
				m_comboDetail2.AddString( dbDatabase.GetName(n) );
				m_comboDetail2.SetItemData( n + 1, dbDatabase.GetDbNo(n) );
			}
		}
		break;
	
	case BUTTON_DETAIL1_REPORTS:
		m_comboDetail2.AddString( "Epos Reports (View)" );
		m_comboDetail2.AddString( "Epos Reports (Print)" );
		m_comboDetail2.AddString( "Plu Sales History" );
		m_comboDetail2.AddString( "Customer Transactions" );
		m_comboDetail2.AddString( "Time & Attendance" );
		m_comboDetail2.SetItemData(0, BUTTON_DETAIL2_REPORTS_EPOS_VIEW );
		m_comboDetail2.SetItemData(1, BUTTON_DETAIL2_REPORTS_EPOS_PRINT );
		m_comboDetail2.SetItemData(2, BUTTON_DETAIL2_REPORTS_PLUHISTORY );
		m_comboDetail2.SetItemData(3, BUTTON_DETAIL2_REPORTS_CUSTOMER );
		m_comboDetail2.SetItemData(4, BUTTON_DETAIL2_REPORTS_TIMEATTEND );

#ifdef STOCKMAN_SYSTEM
		m_comboDetail2.AddString( "Stock Sales History" );
		m_comboDetail2.AddString( "Stock Levels" );
		m_comboDetail2.AddString( "Stock Audits" );
		m_comboDetail2.SetItemData(5, BUTTON_DETAIL2_REPORTS_STKHISTORY );
		m_comboDetail2.SetItemData(6, BUTTON_DETAIL2_REPORTS_STKLEVEL );
		m_comboDetail2.SetItemData(7, BUTTON_DETAIL2_REPORTS_STKAUDIT );
#else
		m_comboDetail2.AddString( "Plu Stock Levels" );
		m_comboDetail2.SetItemData(5, BUTTON_DETAIL2_REPORTS_ECRMANSTK );
#endif

		break;

	case BUTTON_DETAIL1_TASKS:
		m_comboDetail2.AddString( "Adhoc Task" );
		m_comboDetail2.AddString( "Edit Preset Tasks" );
		m_comboDetail2.AddString( "Run Preset Task" );
		m_comboDetail2.SetItemData(0, BUTTON_DETAIL2_TASKS_ADHOC );
		m_comboDetail2.SetItemData(1, BUTTON_DETAIL2_TASKS_EDITPRESET );
		m_comboDetail2.SetItemData(2, BUTTON_DETAIL2_TASKS_RUNPRESET );
		break;

	case BUTTON_DETAIL1_EXTERNAL:

		if ( SysInfo.GotLoyaltyLink() )
		{
			m_comboDetail2.AddString( "Loyalty Manager" );
		}
		else
		{
			m_comboDetail2.AddString( "SmartPay" );
		}

		m_comboDetail2.SetItemData(0, BUTTON_DETAIL2_EXTERNAL_LOYALTY );
		break;
	}

	if ( m_comboDetail2.GetCount() == 0 )
	{
		m_comboDetail2.AddString( "N/A" );
		m_comboDetail2.SetItemData(0,0);
		m_comboDetail2.SetCurSel(0);
	}

	m_comboDetail2.EnableWindow( m_comboDetail2.GetCount() >= 2 );
}

/**********************************************************************/

void CButtonEditDlg::FillDetail3ComboPreset( CArray<CSortedIntAndStringItem,CSortedIntAndStringItem>& listPresets )
{
	m_comboDetail3.AddString( "Open Dialog" );
	m_comboDetail3.SetItemData(0,BUTTON_DETAIL3_REPORTS_OPENDIALOG);

	m_comboDetail3.AddString( "User Select" );
	m_comboDetail3.SetItemData(1,BUTTON_DETAIL3_REPORTS_USERSELECT);
				
	int nPos = 2;
	for ( int nPresetIdx = 0; nPresetIdx < listPresets.GetSize(); nPresetIdx++ )
	{
		CSortedIntAndStringItem item = listPresets.GetAt( nPresetIdx );
		m_comboDetail3.AddString( item.m_strItem );
		m_comboDetail3.SetItemData( nPos++, item.m_nItem );
	}
}

/**********************************************************************/

void CButtonEditDlg::FillDetail3ComboTask()
{
	m_comboDetail3.AddString( "User Select" );
	m_comboDetail3.SetItemData(1,BUTTON_DETAIL3_TASKS_USERSELECT);
				
	int nPos = 1;
	for ( int nTaskNo = 0x81; nTaskNo <= 0x94; nTaskNo++ )
	{
		m_comboDetail3.AddString( m_TaskDatabase.GetLibraryTaskName( nTaskNo ) );
		m_comboDetail3.SetItemData( nPos++, nTaskNo );
	}
}

/**********************************************************************/

void CButtonEditDlg::FillDetail3Combo()
{
	m_comboDetail3.ResetContent();

	switch( m_nDetail1 )
	{
	case BUTTON_DETAIL1_EDITEPOS:
		{
			int nGlobalType = 0;

			switch( m_nDetail2 )
			{
			case BUTTON_DETAIL2_EDITEPOS_PLU:
			case BUTTON_DETAIL2_EDITEPOS_MIXMATCH:
			case BUTTON_DETAIL2_EDITEPOS_OFFERGROUP:
			case BUTTON_DETAIL2_EDITEPOS_WALLETSTAMPRULE:
			case BUTTON_DETAIL2_EDITEPOS_PROMOTION:
			case BUTTON_DETAIL2_EDITEPOS_TIMEPLAN:
			case BUTTON_DETAIL2_EDITEPOS_LOYALTYSCHEME:
				nGlobalType = 2;
				break;

			case BUTTON_DETAIL2_EDITEPOS_DEPT:
				nGlobalType = EcrmanOptions.GetGlobalDepartmentFlag() ? 1 : 2;
				break;

			case BUTTON_DETAIL2_EDITEPOS_GROUP:
				nGlobalType = EcrmanOptions.GetGlobalDeptGroupEposFlag() ? 1 : 2;
				break;

			case BUTTON_DETAIL2_EDITEPOS_TAX:
				nGlobalType = EcrmanOptions.GetGlobalTaxFlag() ? 1 : 2;
				break;

			case BUTTON_DETAIL2_EDITEPOS_ACAT:
				nGlobalType = EcrmanOptions.GetGlobalAnalysisCategoryFlag() ? 1 : 2;
				break;

			case BUTTON_DETAIL2_EDITEPOS_PAYMENT:
				nGlobalType = EcrmanOptions.GetGlobalPaymentFlag() ? 1 : 2;
				break;

			case BUTTON_DETAIL2_EDITEPOS_SERVER:
				switch( EcrmanOptions.GetFeaturesServerSettingsType() )
				{
				case SERVER_DB:		nGlobalType = 2;	break;
				case SERVER_LOC:	nGlobalType = 3;	break;
				case SERVER_SYS:
				default:			nGlobalType = 1;	break;
				}

				if ( EcrmanOptions.GetFeaturesServerRestrictionFlag() )
					nGlobalType = 3;

				break;
			}

			if ( nGlobalType != 0 )
				AddDatabasesToDetail3( nGlobalType );
		}
		break;

#ifdef STOCKMAN_SYSTEM

	case BUTTON_DETAIL1_EDITSTK:
		{
			int nGlobalType = 0;

			switch( m_nDetail2 )
			{
			case BUTTON_DETAIL2_EDITSTK_STOCK:
				nGlobalType = 2;
				break;

			case BUTTON_DETAIL2_EDITSTK_CATEGORY:
				nGlobalType = StockOptions.GetGlobalCategoryFlag() ? 1 : 2;
				break;

			case BUTTON_DETAIL2_EDITSTK_CUSTFLD:
				nGlobalType = StockOptions.GetGlobalCustomFieldFlag() ? 1 : 2;
				break;

			case BUTTON_DETAIL2_EDITSTK_ALLOWANCE:
				nGlobalType = StockOptions.GetGlobalAllowanceFlag() ? 1 : 2;
				break;
			}

			if ( nGlobalType != 0 )
				AddDatabasesToDetail3( nGlobalType );
		}
		break;

	case BUTTON_DETAIL1_MOVESTK:

		switch( m_nDetail2 )
		{
		case BUTTON_DETAIL2_MOVESTK_OPENING:
		case BUTTON_DETAIL2_MOVESTK_DELIVERY:
		case BUTTON_DETAIL2_MOVESTK_RETURN:
		case BUTTON_DETAIL2_MOVESTK_SALEPLU:
		case BUTTON_DETAIL2_MOVESTK_SALESTK:
		case BUTTON_DETAIL2_MOVESTK_ADJUSTPLU:
		case BUTTON_DETAIL2_MOVESTK_ADJUSTSTK:
		case BUTTON_DETAIL2_MOVESTK_TRANSFER:
			AddDatabasesToDetail3(4);
			break;
		}
		break;

#endif

	case BUTTON_DETAIL1_REPORTS:
		switch( m_nDetail2 )
		{
		case BUTTON_DETAIL2_REPORTS_PLUHISTORY:
			FillDetail3ComboPreset( m_listPresetsHistoryPlu );
			break;

		case BUTTON_DETAIL2_REPORTS_EPOS_VIEW:
		case BUTTON_DETAIL2_REPORTS_EPOS_PRINT:
			FillDetail3ComboPreset( m_listPresetsEpos );
			break;
			
		case BUTTON_DETAIL2_REPORTS_CUSTOMER:
			FillDetail3ComboPreset( m_listPresetsHistoryCustomer );
			break;

		case BUTTON_DETAIL2_REPORTS_TIMEATTEND:
			FillDetail3ComboPreset( m_listPresetsTimeAttend );
			break;

#ifdef STOCKMAN_SYSTEM

		case BUTTON_DETAIL2_REPORTS_STKHISTORY:
			FillDetail3ComboPreset( m_listPresetsHistoryStock );
			break;

		case BUTTON_DETAIL2_REPORTS_STKLEVEL:
			FillDetail3ComboPreset( m_listPresetsStockLevel );
			break;

		case BUTTON_DETAIL2_REPORTS_STKAUDIT:
			FillDetail3ComboPreset( m_listPresetsStockAudit );
			break;

#else

		case BUTTON_DETAIL2_REPORTS_ECRMANSTK:
			FillDetail3ComboPreset( m_listPresetsEcrmanStock );
			break;

#endif

		}
		break;

	case BUTTON_DETAIL1_KEYBOARD:
		m_comboDetail3.AddString( "User Select Keyboard" );
		m_comboDetail3.SetItemData( 0, BUTTON_DETAIL3_KEYBOARD_USERSELECT );

		for ( int nKbNo = 1; nKbNo <= EcrmanOptions.GetFeaturesKeyboardsPerLocation(); nKbNo++ )
		{
			CString str;
			str.Format( "Keyboard %d", nKbNo );
			int nPos = m_comboDetail3.AddString( str );
			m_comboDetail3.SetItemData( nPos, nKbNo );
		}
		break;

	case BUTTON_DETAIL1_PSET_LOC:
		m_comboDetail3.AddString( "User Select Price Set" );
		m_comboDetail3.SetItemData( 0, BUTTON_DETAIL3_PSET_USERSELECT );

		for ( int nPSet = 1; nPSet <= 3; nPSet++ )
		{
			CString str;
			str.Format( "Price Set %d", nPSet );
			int nPos = m_comboDetail3.AddString( str );
			m_comboDetail3.SetItemData( nPos, nPSet );
		}
		break;

	case BUTTON_DETAIL1_PSET_DB:
		m_comboDetail3.AddString( "User Select Price Set" );
		m_comboDetail3.SetItemData( 0, BUTTON_DETAIL3_PSET_USERSELECT );

		for ( int nPSet = 1; nPSet <= 20; nPSet++ )
		{
			CString str;
			str.Format( "Price Set %d", nPSet );
			int nPos = m_comboDetail3.AddString( str );
			m_comboDetail3.SetItemData( nPos, nPSet );
		}
		break;

	case BUTTON_DETAIL1_TASKS:
		switch ( m_nDetail2 )
		{
		case BUTTON_DETAIL2_TASKS_RUNPRESET:
			FillDetail3ComboTask();
			break;
		}
		break;
	}
	
	if ( m_comboDetail3.GetCount() == 0 )
	{
		m_comboDetail3.AddString( "N/A" );
		m_comboDetail3.SetItemData(0,0);
		m_comboDetail3.SetCurSel(0);
	}

	m_comboDetail3.EnableWindow( m_comboDetail3.GetCount() >= 2 );
}

/**********************************************************************/

void CButtonEditDlg::AddDatabasesToDetail3( int nLocationType )
{
	switch( nLocationType )
	{

#ifdef STOCKMAN_SYSTEM

	case 4:	//STOCKPOINT
		{
			m_comboDetail3.AddString( "User Select" );
			m_comboDetail3.SetItemData(0,0);

			for ( int n = 0; n < dbStockpoint.GetSize(); n++ )
			{
				m_comboDetail3.AddString( dbStockpoint.GetName(n) );
				m_comboDetail3.SetItemData( n + 1, ( dbStockpoint.GetDbNo(n) * 1000 ) + dbStockpoint.GetSpNo(n) );
			}
		}
		break;

#endif
		
	case 3:	//LOCATION
		{
			m_comboDetail3.AddString( "User Select" );
			m_comboDetail3.SetItemData(0,0);

			for ( int n = 0; n < dbLocation.GetSize(); n++ )
			{
				m_comboDetail3.AddString( dbLocation.GetName(n) );
				m_comboDetail3.SetItemData( n + 1, dbLocation.GetNetworkLocNo(n) );
			}
		}
		break;

	case 2:	//DATABASE
		{
			m_comboDetail3.AddString( "User Select" );
			m_comboDetail3.SetItemData(0,0);

			for ( int n = 0; n < dbDatabase.GetSize(); n++ )
			{
				m_comboDetail3.AddString( dbDatabase.GetName(n) );
				m_comboDetail3.SetItemData( n + 1, dbDatabase.GetDbNo(n) );
			}
		}
		break;

	case 1:	//GLOBAL
	default:
		m_comboDetail3.AddString( "Global" );
		m_comboDetail3.SetItemData(0,0);
		break;
	}
}

/**********************************************************************/

void CButtonEditDlg::ShowOrHideDetail3()
{
	bool bShowDetailCombo = FALSE;
	bool bShowPresetTick = FALSE;
	
	if ( BUTTON_TYPE_ACTION == m_nButtonType )
	{
		switch( m_nDetail1 )
		{
		case BUTTON_DETAIL1_EDITEPOS:
			switch( m_nDetail2 )
			{
			case BUTTON_DETAIL2_EDITEPOS_PLU:
			case BUTTON_DETAIL2_EDITEPOS_DEPT:
			case BUTTON_DETAIL2_EDITEPOS_GROUP:
			case BUTTON_DETAIL2_EDITEPOS_TAX:
			case BUTTON_DETAIL2_EDITEPOS_ACAT:
			case BUTTON_DETAIL2_EDITEPOS_PAYMENT:
			case BUTTON_DETAIL2_EDITEPOS_SERVER:
			case BUTTON_DETAIL2_EDITEPOS_MIXMATCH:
			case BUTTON_DETAIL2_EDITEPOS_WALLETSTAMPRULE:
			case BUTTON_DETAIL2_EDITEPOS_OFFERGROUP:
			case BUTTON_DETAIL2_EDITEPOS_PROMOTION:
			case BUTTON_DETAIL2_EDITEPOS_TIMEPLAN:
			case BUTTON_DETAIL2_EDITEPOS_LOYALTYSCHEME:
				bShowDetailCombo = TRUE;
				break;
			}
			break;

#ifdef STOCKMAN_SYSTEM

		case BUTTON_DETAIL1_EDITSTK:
			switch( m_nDetail2 )
			{
			case BUTTON_DETAIL2_EDITSTK_STOCK:
			case BUTTON_DETAIL2_EDITSTK_CATEGORY:
			case BUTTON_DETAIL2_EDITSTK_CUSTFLD:
			case BUTTON_DETAIL2_EDITSTK_ALLOWANCE:
				bShowDetailCombo = TRUE;
				break;
			}
			break;

		case BUTTON_DETAIL1_MOVESTK:
			switch( m_nDetail2 )
			{
			case BUTTON_DETAIL2_MOVESTK_OPENING:
			case BUTTON_DETAIL2_MOVESTK_DELIVERY:
			case BUTTON_DETAIL2_MOVESTK_RETURN:
			case BUTTON_DETAIL2_MOVESTK_SALEPLU:
			case BUTTON_DETAIL2_MOVESTK_SALESTK:
			case BUTTON_DETAIL2_MOVESTK_ADJUSTPLU:
			case BUTTON_DETAIL2_MOVESTK_ADJUSTSTK:
			case BUTTON_DETAIL2_MOVESTK_TRANSFER:
				bShowDetailCombo = TRUE;
				break;
			}
			break;

#endif

		case BUTTON_DETAIL1_REPORTS:
			switch( m_nDetail2 )
			{
#ifdef STOCKMAN_SYSTEM
			case BUTTON_DETAIL2_REPORTS_STKHISTORY:
			case BUTTON_DETAIL2_REPORTS_STKLEVEL:
			case BUTTON_DETAIL2_REPORTS_STKAUDIT:
#else
			case BUTTON_DETAIL2_REPORTS_ECRMANSTK:
#endif
			case BUTTON_DETAIL2_REPORTS_PLUHISTORY:
			case BUTTON_DETAIL2_REPORTS_EPOS_VIEW:
			case BUTTON_DETAIL2_REPORTS_EPOS_PRINT:
			case BUTTON_DETAIL2_REPORTS_CUSTOMER:
			case BUTTON_DETAIL2_REPORTS_TIMEATTEND:
				bShowDetailCombo = TRUE;
				bShowPresetTick = TRUE;
				break;
			}

		case BUTTON_DETAIL1_KEYBOARD:
		case BUTTON_DETAIL1_PSET_LOC:
		case BUTTON_DETAIL1_PSET_DB:
			bShowDetailCombo = TRUE;
			break;

		case BUTTON_DETAIL1_TASKS:
			switch( m_nDetail2 )
			{
			case BUTTON_DETAIL2_TASKS_RUNPRESET:
				bShowDetailCombo = TRUE;
				break;
			}
			break;
		}
	}

	ShowAndEnableWindow( &m_comboDetail3, bShowDetailCombo, ( m_comboDetail3.GetCount() >= 2 ) );
	ShowAndEnableWindow( &m_checkPreset, bShowPresetTick, ( m_comboDetail3.GetCount() >= 3 ) );
}

/**********************************************************************/

void CButtonEditDlg::OnButtonBackColour()
{
	m_bEditTextColour = FALSE;
	CColourEditDlg dlg( m_buttonColour.m_ColourRefBack, "Set Button Background Colour", this, FALSE, this );
	if ( dlg.DoModal() == IDOK )
	{
		m_buttonColour.m_ColourRefBack = dlg.GetColourRef();
		m_buttonColour.Invalidate();
	}
}

/**********************************************************************/

void CButtonEditDlg::OnButtonTextColour()
{
	m_bEditTextColour = TRUE;
	CColourEditDlg dlg( m_buttonColour.m_ColourRefText, "Set Button Text Colour", this, FALSE, this );
	if ( dlg.DoModal() == IDOK )
	{
		m_buttonColour.m_ColourRefText = dlg.GetColourRef();
		m_buttonColour.Invalidate();
	}
}

/**********************************************************************/

void CButtonEditDlg::OnButtonImage()
{
	CFileDialog dlgFile ( TRUE, "", "", OFN_NOCHANGEDIR|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, "Image Files (*.bmp;*.gif;*.jpg;*.png)|*.bmp;*.gif;*.jpg;*.png||", NULL );
	dlgFile.m_ofn.lpstrTitle = "Select Button Image";
	dlgFile.m_ofn.lpstrInitialDir = m_strImageImportPath;
	
	if ( dlgFile.DoModal() != IDOK )
		return;

	CString strFilepath = dlgFile.GetPathName();
	CString strFilename = dlgFile.GetFileName();
	CString strExt = dlgFile.GetFileExt();

	int nPathLen = strFilepath.GetLength();
	int nFileLen = strFilename.GetLength();

	if ( nPathLen >= nFileLen )
		m_strImageImportPath = strFilepath.Left ( nPathLen - nFileLen );

	strExt.MakeUpper();

	if ( ( strExt == "BMP" ) || ( strExt == "GIF" ) || ( strExt == "JPG" ) || ( strExt == "PNG" ) )
	{
		CString strDestFilename = "";
		m_infoOriginal.GetButtonImageFilename( m_infoOriginal.GetListNo(), 2, strDestFilename );

		m_strNewImageType = strExt;

		m_buttonColour.SwapImage();
		if ( m_buttonColour.LoadImage( strFilepath ) == FALSE )
		{
			m_buttonColour.SwapImage();
			m_buttonColour.Invalidate();
			Prompter.Error ( "The selected image is too large.\n\nMaximum accepted size is 350 x 350 pixels." );
		}
		else
		{
			CopyFile( strFilepath, strDestFilename, FALSE );
			m_buttonColour.Invalidate();
		}
	}
}

/**********************************************************************/

void CButtonEditDlg::OnButtonClear()
{
	if ( Prompter.YesNo( "Are you sure you wish to remove this button image." ) == IDYES )
	{
		CString strOldImageType = m_infoOriginal.GetImageTypeTemp();
		
		if ( FALSE == m_bGotOriginalImage )
			m_strNewImageType = "";
		else
			m_strNewImageType = "XXX";

		CString strFilename;
		m_infoOriginal.GetButtonImageFilename( m_infoOriginal.GetListNo(), 2, strFilename );
		CFileRemover FileRemover( strFilename );

		m_buttonColour.LoadImage( "" );
		m_buttonColour.Invalidate();
	}
}

/**********************************************************************/

void CButtonEditDlg::OnToggleTransparent()
{
	m_buttonColour.SetTransparentFlag( m_checkTransparent.GetCheck() != 0 );
	m_buttonColour.Invalidate();
}

/**********************************************************************/

long CButtonEditDlg::OnColourButtonMessage( WPARAM w, LPARAM l )
{
	if ( FALSE == m_bEditTextColour )
		m_buttonColour.m_ColourRefBack = l;
	else
		m_buttonColour.m_ColourRefText = l;

	m_buttonColour.Invalidate();
	return 0l;
}

/**********************************************************************/

void CButtonEditDlg::UpdateText()
{
	bool bOriginalText = FALSE;

	switch ( m_nButtonType )
	{
	case BUTTON_TYPE_UNUSED:
		bOriginalText = ( BUTTON_TYPE_UNUSED == m_nOriginalButtonType );
		break;

	case BUTTON_TYPE_LIST:
		bOriginalText = TRUE;
		bOriginalText &= ( m_nMenuNo == m_nOriginalMenuNo );
		bOriginalText &= ( BUTTON_TYPE_LIST == m_nOriginalButtonType );
		break;

	case BUTTON_TYPE_ACTION:
		bOriginalText = TRUE;
		bOriginalText &= ( m_nDetail1 == m_nOriginalDetail1 );
		bOriginalText &= ( m_nDetail2 == m_nOriginalDetail2 );
		bOriginalText &= ( m_nDetail3 == m_nOriginalDetail3 );
		bOriginalText &= ( BUTTON_TYPE_ACTION == m_nOriginalButtonType );
		break;
	}
	
	if ( TRUE == bOriginalText )
	{
		m_editLine[0].SetWindowText( m_strOriginalLine[0] );
		m_editLine[1].SetWindowText( m_strOriginalLine[1] );
		m_editLine[2].SetWindowText( m_strOriginalLine[2] );
	}
	else
	{

		CStringArray arrayTexts;

		switch (m_nButtonType)
		{
		case BUTTON_TYPE_ACTION:
		{
			bool bUseComboText = FALSE;

			switch (m_nDetail1)
			{
			case BUTTON_DETAIL1_REPORTS:
				switch (m_nDetail3)
				{
				case BUTTON_DETAIL3_REPORTS_USERSELECT:
				case BUTTON_DETAIL3_REPORTS_OPENDIALOG:
					break;

				default:
					bUseComboText = TRUE;
					break;
				}
			}

			if (FALSE == bUseComboText)
			{
				CButtonInfo ButtonInfo;
				CreateButton(ButtonInfo, FALSE);
				ButtonInfo.SetDefaultTexts();
				ButtonInfo.GetTextLines(arrayTexts);
			}
			else
			{
				CString strText;
				m_comboDetail3.GetWindowText(strText);
				::TokeniseSpacedLine(strText, arrayTexts);
			}
		}
		break;

		case BUTTON_TYPE_LIST:
		{
			int nListNo = m_comboMenu.GetCurSel() + 1;
			CString strText = ButtonManager.GetListName(nListNo);

			::TrimSpaces(strText, FALSE);
			if (strText == "")
				strText.Format("List %d", nListNo);

			::TokeniseSpacedLine(strText, arrayTexts);
		}
		break;
		}

		int nSize = arrayTexts.GetSize();
		CString strText1 = "";
		CString strText2 = "";
		CString strText3 = "";
		if (nSize >= 1) strText1 = arrayTexts.GetAt(0);
		if (nSize >= 2) strText2 = arrayTexts.GetAt(1);
		if (nSize >= 3) strText3 = arrayTexts.GetAt(2);
		m_editLine[0].SetWindowText(strText1);
		m_editLine[1].SetWindowText(strText2);
		m_editLine[2].SetWindowText(strText3);
	}
}

/**********************************************************************/

void CButtonEditDlg::CreateButton( CButtonInfo& infoButton, bool bUpdateImage )
{
	infoButton.SetOwnColourFlag ( m_checkColour.GetCheck() != 0 );
	infoButton.SetOwnBackColour( m_buttonColour.m_ColourRefBack );
	infoButton.SetOwnTextColour( m_buttonColour.m_ColourRefText );
	infoButton.SetFontSize( 8 + ( m_comboFont.GetCurSel() * 2 ) );
	infoButton.SetImageTransparentFlag( m_checkTransparent.GetCheck() != 0 );

	if ( TRUE == bUpdateImage )
	{
		switch( m_nMode )
		{
		case BUTTON_EDIT_NORMAL:
		case BUTTON_EDIT_COPYALL:
			if ( m_strNewImageType != "" )
			{
				if ( m_strNewImageType == "XXX" )
				{
					CString strFilename;
					infoButton.SetImageTypeTemp( "JPG" );
					infoButton.GetButtonImageFilename( infoButton.GetListNo(), 1, strFilename );
					CFileRemover FileRemover( strFilename );
				}
				else
				{
					CString strSource, strDest;
					m_infoOriginal.GetButtonImageFilename( m_infoOriginal.GetListNo(), 2, strSource );
					infoButton.SetImageTypeTemp( "BMP" );
					infoButton.GetButtonImageFilename( infoButton.GetListNo(), 1, strDest );
					::CopyFile( strSource, strDest, FALSE );
				}

				infoButton.SetImageTypeTemp( m_strNewImageType );
			}
			break;
		}
	}

	switch( m_nMode )
	{
	case BUTTON_EDIT_NORMAL:
	case BUTTON_EDIT_COPYALL:
		
		switch( m_nButtonType )
		{
		case BUTTON_TYPE_LIST:		
			infoButton.SetButtonType( BUTTON_TYPE_LIST );	
			infoButton.SetDetail1( m_nMenuNo );
			infoButton.SetDetail2( 0 );
			infoButton.SetDetail3( 0 );
			break;

		case BUTTON_TYPE_ACTION:		
			infoButton.SetButtonType( BUTTON_TYPE_ACTION );
			infoButton.SetDetail1( m_nDetail1 );
			infoButton.SetDetail2( m_nDetail2 );
			infoButton.SetDetail3( m_nDetail3 );
			infoButton.SetActionFlag1( m_checkPreset.GetCheck() == 0 ); 
			break;

		case BUTTON_TYPE_UNUSED:
		default:	
			infoButton.SetButtonType( BUTTON_TYPE_UNUSED );
			infoButton.SetDetail1( 0 );
			infoButton.SetDetail2( 0 );
			infoButton.SetDetail3( 0 );
			infoButton.SetOwnColourFlag( FALSE );
			break;
		}

		{
			CCSV csv;
			for ( int n = 0; n < 3; n++ )
			{
				CString strText;
				m_editLine[n].GetWindowText( strText );
				csv.Add( strText );
			}

			infoButton.SetTextLines( csv );
		}
	}
}

/**********************************************************************/

void CButtonEditDlg::MergeSettings( CButtonInfo& LayoutButton )
{
	CButtonInfo NewSettings;
	NewSettings.SetListNo( LayoutButton.GetListNo() );
	NewSettings.SetLineNo( LayoutButton.GetLineNo() );
	NewSettings.SetButtonNo( LayoutButton.GetButtonNo() );
	CreateButton( NewSettings, TRUE );

	if ( ( BUTTON_EDIT_COPYALL == m_nMode ) || ( LayoutButton.GetButtonType() != BUTTON_TYPE_UNUSED ) )
	{
		LayoutButton.SetOwnColourFlag( NewSettings.GetOwnColourFlag() );
		LayoutButton.SetOwnBackColour( NewSettings.GetOwnBackColour() );
		LayoutButton.SetOwnTextColour( NewSettings.GetOwnTextColour() );
		LayoutButton.SetFontSize( NewSettings.GetFontSize() );
	}
	
	if ( BUTTON_EDIT_COPYALL == m_nMode )
	{
		LayoutButton.SetButtonType( NewSettings.GetButtonType() );
		LayoutButton.SetDetail1( NewSettings.GetDetail1() );
		LayoutButton.SetDetail2( NewSettings.GetDetail2() );
		LayoutButton.SetDetail3( NewSettings.GetDetail3() );
		LayoutButton.SetActionFlag1( NewSettings.GetActionFlag1() );
		LayoutButton.CopyTextLines( NewSettings );
		LayoutButton.SetImageTypeTemp( NewSettings.GetImageTypeTemp() );
		LayoutButton.SetImageTransparentFlag( NewSettings.GetImageTransparentFlag() );
		
		if ( LayoutButton.GetButtonType() == BUTTON_TYPE_UNUSED )
		{
			LayoutButton.SetDetail1( 0 );
			LayoutButton.SetDetail2( 0 );
			LayoutButton.SetDetail3( 0 );
			LayoutButton.SetActionFlag1( FALSE );
			LayoutButton.SetOwnColourFlag( FALSE );
			LayoutButton.SetOwnBackColour( ButtonOptions.GetButtonColourBackUnusedInt() );
			LayoutButton.SetOwnTextColour( ButtonOptions.GetButtonColourBackUnusedInt() );
		}

		switch( m_infoOriginal.GetButtonType() )
		{
		case BUTTON_TYPE_LIST:
			if ( m_comboAuto.GetCurSel() == 1 )
			{
				int nSel = m_comboMenu.GetCurSel();
				
				if ( nSel < m_comboMenu.GetCount() - 1 )
					m_comboMenu.SetCurSel( nSel + 1 );
				else
					m_comboMenu.SetCurSel( 0 );

				OnSelectMenu();
			}
			break;

		case BUTTON_TYPE_ACTION:
			switch( m_comboAuto.GetCurSel() )
			{
			case 1:
				{
					int nSel = m_comboDetail2.GetCurSel();
					
					if ( nSel < m_comboDetail2.GetCount() - 1 )
						m_comboDetail2.SetCurSel( nSel + 1 );
					else
						m_comboDetail2.SetCurSel( 0 );
						
					OnSelectDetail2();
				}
				break;

			case 2:
				if ( m_comboDetail3.IsWindowEnabled() == TRUE )
				{
					int nSel = m_comboDetail3.GetCurSel();
					
					if ( nSel < m_comboDetail3.GetCount() - 1 )
						m_comboDetail3.SetCurSel( nSel + 1 );
					else
						m_comboDetail3.SetCurSel( 0 );
						
					OnSelectDetail3();
				}
				break;
			}
			break;
		}
	}
}

/**********************************************************************/

void CButtonEditDlg::OnOK() 
{
	if ( BUTTON_EDIT_NORMAL == m_nMode )
	{
		CreateButton( m_infoOriginal, TRUE );

		CStringArray arrayTexts;
		m_infoOriginal.GetDisplayTexts( arrayTexts );

		if ( arrayTexts.GetSize() == 0 )
			m_infoOriginal.SetDefaultTexts();

		EndDialog ( IDOK );
	}
}

/**********************************************************************/

void CButtonEditDlg::OnCancel() 
{
	if ( BUTTON_EDIT_NORMAL != m_nMode )
		DestroyWindow();
	else
		EndDialog( IDCANCEL );
}

/**********************************************************************/

void CButtonEditDlg::PostNcDestroy() 
{
	if ( BUTTON_EDIT_NORMAL != m_nMode )
	{
		ButtonManager.SetButtonEditDlg( NULL );
		delete this;
	}
}

/**********************************************************************/
