/**********************************************************************/
#include "ColourEditDlg.h"
#include "SPOSKeyboardPropertySheetListSelect.h"
/**********************************************************************/
#include "SPOSKeyboardColourGlobalDlg.h"
/**********************************************************************/

CSPOSKeyboardColourGlobalDlg::CSPOSKeyboardColourGlobalDlg( __int64 nVisibleListNo, CWnd* pParent /*=NULL*/)
	: CDialog(CSPOSKeyboardColourGlobalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSPOSKeyboardColourGlobalDlg)
	m_nVisibleListNo = nVisibleListNo;
	m_bControl = FALSE;
	m_bEmpty = FALSE;
	m_bFunction = FALSE;
	m_bKeyText = FALSE;
	m_bRemoveImage = FALSE;
	m_bList = FALSE;
	m_bMacro = FALSE;
	m_bModifier = FALSE;
	m_bPayment = FALSE;
	m_bPlu = FALSE;
	m_bServer = FALSE;
	//}}AFX_DATA_INIT

	m_nForeground = 1;
	m_nBackground = 4;
	m_nFontSize = 999;
	m_nHAlign = 1;
	m_nVAlign = 1;
	m_nBold = 0;
	m_nHide = 0;
	m_nDefault = 0;
	
	m_nButtonID[0] = IDC_BUTTON_COLOUR0;
	m_nButtonID[1] = IDC_BUTTON_COLOUR1;
	m_nButtonID[2] = IDC_BUTTON_COLOUR2;
	m_nButtonID[3] = IDC_BUTTON_COLOUR3;
	m_nButtonID[4] = IDC_BUTTON_COLOUR4;
	m_nButtonID[5] = IDC_BUTTON_COLOUR5;
	m_nButtonID[6] = IDC_BUTTON_COLOUR6;
	m_nButtonID[7] = IDC_BUTTON_COLOUR7;
	m_nButtonID[8] = IDC_BUTTON_COLOUR8;
	m_nButtonID[9] = IDC_BUTTON_COLOUR9;
	m_nButtonID[10] = IDC_BUTTON_COLOUR10;
	m_nButtonID[11] = IDC_BUTTON_COLOUR11;
	m_nButtonID[12] = IDC_BUTTON_COLOUR12;
	m_nButtonID[13] = IDC_BUTTON_COLOUR13;
	m_nButtonID[14] = IDC_BUTTON_COLOUR14;
	m_nButtonID[15] = IDC_BUTTON_COLOUR15;
	m_nButtonID[16] = IDC_BUTTON_COLOUR16;
	m_nButtonID[17] = IDC_BUTTON_COLOUR17;
	m_nButtonID[18] = IDC_BUTTON_COLOUR18;
	m_nButtonID[19] = IDC_BUTTON_COLOUR19;
	m_nButtonID[20] = IDC_BUTTON_COLOUR20;
	m_nButtonID[21] = IDC_BUTTON_COLOUR21;
	m_nButtonID[22] = IDC_BUTTON_COLOUR22;
	m_nButtonID[23] = IDC_BUTTON_COLOUR23;
	m_nButtonID[24] = IDC_BUTTON_COLOUR24;
	m_nButtonID[25] = IDC_BUTTON_COLOUR25;
	m_nButtonID[26] = IDC_BUTTON_COLOUR26;
	m_nButtonID[27] = IDC_BUTTON_COLOUR27;
	m_nButtonID[28] = IDC_BUTTON_COLOUR28;
	m_nButtonID[29] = IDC_BUTTON_COLOUR29;
	m_nButtonID[30] = IDC_BUTTON_COLOUR30;
	m_nButtonID[31] = IDC_BUTTON_COLOUR31;
	m_nButtonID[32] = IDC_BUTTON_COLOUR32;
	m_nButtonID[33] = IDC_BUTTON_COLOUR33;
	m_nButtonID[34] = IDC_BUTTON_COLOUR34;
	m_nButtonID[35] = IDC_BUTTON_COLOUR35;
	m_nButtonID[36] = IDC_BUTTON_COLOUR36;
	m_nButtonID[37] = IDC_BUTTON_COLOUR37;
	m_nButtonID[38] = IDC_BUTTON_COLOUR38;
	m_nButtonID[39] = IDC_BUTTON_COLOUR39;
	m_nButtonID[40] = IDC_BUTTON_COLOUR40;
	m_nButtonID[41] = IDC_BUTTON_COLOUR41;
	m_nButtonID[42] = IDC_BUTTON_COLOUR42;
	m_nButtonID[43] = IDC_BUTTON_COLOUR43;
	m_nButtonID[44] = IDC_BUTTON_COLOUR44;
	m_nButtonID[45] = IDC_BUTTON_COLOUR45;
	m_nButtonID[46] = IDC_BUTTON_COLOUR46;
	m_nButtonID[47] = IDC_BUTTON_COLOUR47;
	m_nButtonID[48] = IDC_BUTTON_COLOUR48;
	m_nButtonID[49] = IDC_BUTTON_COLOUR49;
	m_nButtonID[50] = IDC_BUTTON_COLOUR50;
	m_nButtonID[51] = IDC_BUTTON_COLOUR51;
	m_nButtonID[52] = IDC_BUTTON_COLOUR52;
	m_nButtonID[53] = IDC_BUTTON_COLOUR53;
	m_nButtonID[54] = IDC_BUTTON_COLOUR54;
	m_nButtonID[55] = IDC_BUTTON_COLOUR55;
	m_nButtonID[56] = IDC_BUTTON_COLOUR56;
	m_nButtonID[57] = IDC_BUTTON_COLOUR57;
	m_nButtonID[58] = IDC_BUTTON_COLOUR58;
	m_nButtonID[59] = IDC_BUTTON_COLOUR59;
	m_nButtonID[60] = IDC_BUTTON_COLOUR60;
	m_nButtonID[61] = IDC_BUTTON_COLOUR61;
	m_nButtonID[62] = IDC_BUTTON_COLOUR62;
	m_nButtonID[63] = IDC_BUTTON_COLOUR63;
	m_nButtonID[64] = IDC_BUTTON_COLOUR64;

	for (int n = 0; n < 65; n++)
	{
		m_buttonColour[n].m_nButtonNo = n;
	}

	for ( int n = 0; n < SPOSKeyboardManager.GetListManager() -> GetListCount(); n++ )
	{
		CSPOSKeyboardListRecord ListRecord;
		SPOSKeyboardManager.GetListManager() -> GetListRecord( n, ListRecord );
		ListRecord.SetAllowChangeFlag( TRUE );
		SPOSKeyboardManager.GetListManager() -> SetListRecord( n, ListRecord );
	}
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardColourGlobalDlg)
	DDX_Control(pDX, IDC_COMBO_VERTICAL, m_comboVertical);
	DDX_Control(pDX, IDC_COMBO_HORIZONTAL, m_comboHorizontal);
	DDX_Control(pDX, IDC_COMBO_BOLD, m_comboBold);
	DDX_Control(pDX, IDC_COMBO_COLOUR, m_comboColour);
	DDX_Control(pDX, IDC_COMBO_TEXTSIZE, m_comboFontSize);
	DDX_Control(pDX, IDC_COMBO_HIDE, m_comboHide);
	DDX_Control(pDX, IDC_COMBO_DEFAULT, m_comboDefault);
	DDX_Control(pDX, IDC_COMBO_CHANGES, m_comboChanges);
	DDX_Control(pDX, IDC_CHECK_ITEM_STANDARD, m_checkItemStandard);
	DDX_Control(pDX, IDC_CHECK_ITEM_LARGE, m_checkItemLarge);
	DDX_Control(pDX, IDC_CHECK_FUNCTION_STANDARD, m_checkFunctionStandard);
	DDX_Control(pDX, IDC_CHECK_FUNCTION_LARGE, m_checkFunctionLarge);
	DDX_Control(pDX, IDC_CHECK_BOTTOM_BAR, m_checkBottomBar);
	DDX_Check(pDX, IDC_CHECK_CONTROL, m_bControl);
	DDX_Check(pDX, IDC_CHECK_EMPTY, m_bEmpty);
	DDX_Check(pDX, IDC_CHECK_FUNCTION, m_bFunction);
	DDX_Check(pDX, IDC_CHECK_KEYTEXT, m_bKeyText);
	DDX_Check(pDX, IDC_CHECK_REMOVEIMAGE, m_bRemoveImage);
	DDX_Check(pDX, IDC_CHECK_LIST, m_bList);
	DDX_Check(pDX, IDC_CHECK_MACRO, m_bMacro);
	DDX_Check(pDX, IDC_CHECK_MODIFIER, m_bModifier);
	DDX_Check(pDX, IDC_CHECK_PAYMENT, m_bPayment);
	DDX_Check(pDX, IDC_CHECK_PLUNUMBER, m_bPlu);
	DDX_Check(pDX, IDC_CHECK_SVRPRESET, m_bServer);
	//}}AFX_DATA_MAP

	for (int n = 0; n < 65; n++)
	{
		DDX_Control(pDX, m_nButtonID[n], m_buttonColour[n]);
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardColourGlobalDlg, CDialog)
	//{{AFX_MSG_MAP(CSPOSKeyboardColourGlobalDlg)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_CBN_SELCHANGE(IDC_COMBO_TEXTSIZE, OnSelectFontSize)
	ON_CBN_SELCHANGE(IDC_COMBO_COLOUR, OnSelectColourMode)
	ON_CBN_SELCHANGE(IDC_COMBO_BOLD, OnSelectBold)
	ON_CBN_SELCHANGE(IDC_COMBO_HORIZONTAL, OnSelectHorizontal)
	ON_CBN_SELCHANGE(IDC_COMBO_VERTICAL, OnSelectVertical)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANGES, OnSelectChanges)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	ON_WM_GETMINMAXINFO()
	ON_MESSAGE ( WM_APP, OnColourButtonClick)
	ON_MESSAGE ( WM_APP + 1, OnColourButtonDoubleClick)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardColourGlobalDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	for (int n = 1; n <= 64; n++)
	{
		m_buttonColour[n].m_ColourRefBack = SPOSKeyboardManager.GetColourTable()->GetCurrentRGB(n);
	}

	m_buttonColour[0].SetWindowText( "ABCD" );

	FillFontCombo();
	FillColourCombo();
	FillHorizontalCombo();
	FillVerticalCombo();
	FillBoldCombo();
	FillHideCombo();
	FillDefaultCombo();

	m_comboColour.SetCurSel(0);
	
	SetForeground(1);
	SetBackground(4);
	SetFontSize(999);
	SetHAlign(999);
	SetVAlign(999);
	SetBold(999);
	SetHide(999);
	SetDefault(999);

	m_checkItemStandard.SetCheck( TRUE );
	m_checkItemLarge.SetCheck( TRUE );
	m_checkFunctionStandard.SetCheck( TRUE );
	m_checkFunctionLarge.SetCheck( TRUE );
	m_checkBottomBar.SetCheck( TRUE );

	m_comboChanges.AddString( "Visible List Only" );
	m_comboChanges.AddString( "Ticked List Types" );
	m_comboChanges.AddString( "Selected Lists" );
	m_comboChanges.SetCurSel(0);
	OnSelectChanges();

	UpdatePreviewButton();

	return TRUE;  
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::FillFontCombo()
{
	m_comboFontSize.ResetContent();

	CWordArray arraySizes;
	CSPOSKeyboardKeyRecord::GetFontSizeList(arraySizes);
	
	for( int n = 0; n < arraySizes.GetSize(); n++ )
	{
		CString strSize = "";
		int nSize = arraySizes.GetAt(n);
		strSize.Format( "%d point", nSize );
		m_comboFontSize.AddString( strSize );
		m_comboFontSize.SetItemData( n, nSize );
	}

	m_comboFontSize.InsertString( 0, "No Change" );
	m_comboFontSize.SetItemData( 0, 999 );

	m_comboFontSize.InsertString( 1, "Key Default" );
	m_comboFontSize.SetItemData( 1, 998 );
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::FillColourCombo()
{
	m_comboColour.ResetContent();
	m_comboColour.AddString( "No Change" );
	m_comboColour.AddString( "Key Default" );
	m_comboColour.AddString( "As Shown" );
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::FillHorizontalCombo()
{
	m_comboHorizontal.ResetContent();
	m_comboHorizontal.AddString( "No Change" );
	m_comboHorizontal.AddString( "Key Default" );
	m_comboHorizontal.AddString( "Left" );
	m_comboHorizontal.AddString( "Centre" );
	m_comboHorizontal.AddString( "Right" );
	m_comboHorizontal.SetItemData( 0, 999 );
	m_comboHorizontal.SetItemData( 1, 998 );
	m_comboHorizontal.SetItemData( 2, 0 );
	m_comboHorizontal.SetItemData( 3, 1 );
	m_comboHorizontal.SetItemData( 4, 2 );
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::FillVerticalCombo()
{
	m_comboVertical.ResetContent();
	m_comboVertical.AddString( "No Change" );
	m_comboVertical.AddString( "Key Default" );
	m_comboVertical.AddString( "Top" );
	m_comboVertical.AddString( "Centre" );
	m_comboVertical.AddString( "Bottom" );
	m_comboVertical.SetItemData( 0, 999 );
	m_comboVertical.SetItemData( 1, 998 );
	m_comboVertical.SetItemData( 2, 0 );
	m_comboVertical.SetItemData( 3, 1 );
	m_comboVertical.SetItemData( 4, 2 );
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::FillBoldCombo()
{
	m_comboBold.ResetContent();
	m_comboBold.AddString( "No Change" );
	m_comboBold.AddString( "Key Default" );
	m_comboBold.AddString( "Yes" );
	m_comboBold.AddString( "No" );
	m_comboBold.SetItemData( 0, 999 );
	m_comboBold.SetItemData( 1, 998 );
	m_comboBold.SetItemData( 2, 1 );
	m_comboBold.SetItemData( 3, 0 );
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::FillHideCombo()
{
	m_comboHide.ResetContent();
	m_comboHide.AddString( "No Change" );
	//m_comboHide.AddString( "Key Default" );
	m_comboHide.AddString( "Yes" );
	m_comboHide.AddString( "No" );
	m_comboHide.SetItemData( 0, 999 );
	//m_comboHide.SetItemData( 1, 998 );
	m_comboHide.SetItemData( 1, 1 );
	m_comboHide.SetItemData( 2, 0 );
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::FillDefaultCombo()
{
	m_comboDefault.ResetContent();
	m_comboDefault.AddString( "No Change" );
	//m_comboDefault.AddString( "Key Default" );
	m_comboDefault.AddString( "Yes" );
	m_comboDefault.AddString( "No" );
	m_comboDefault.SetItemData( 0, 999 );
	//m_comboDefault.SetItemData( 1, 998 );
	m_comboDefault.SetItemData( 1, 1 );
	m_comboDefault.SetItemData( 2, 0 );
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::SetForeground( int n )
{
	if ( n >= 1 && n <= 64 )
	{
		m_nForeground = n;
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::SetBackground( int n )
{
	if ( (n >= 1) && (n <= 64) )
	{
		m_nBackground = n;
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::SetFontSize( int n )
{	
	for ( int x = 0; x < m_comboFontSize.GetCount(); x++ )
	{
		if ( m_comboFontSize.GetItemData(x) == (WORD) n )
		{
			m_nFontSize = n;
			m_comboFontSize.SetCurSel(x);
			UpdatePreviewButton();
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::SetHAlign( int n )
{	
	for ( int x = 0; x < m_comboHorizontal.GetCount(); x++ )
	{
		if ( m_comboHorizontal.GetItemData(x) == (WORD) n )
		{
			m_nHAlign = n;
			m_comboHorizontal.SetCurSel(x);
			UpdatePreviewButton();
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::SetVAlign( int n )
{	
	for ( int x = 0; x < m_comboVertical.GetCount(); x++ )
	{
		if ( m_comboVertical.GetItemData(x) == (WORD) n )
		{
			m_nVAlign = n;
			m_comboVertical.SetCurSel(x);
			UpdatePreviewButton();
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::SetBold( int n )
{	
	for ( int x = 0; x < m_comboBold.GetCount(); x++ )
	{
		if ( m_comboBold.GetItemData(x) == (WORD) n )
		{
			m_nBold = n;
			m_comboBold.SetCurSel(x);
			UpdatePreviewButton();
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::SetHide( int n )
{	
	for ( int x = 0; x < m_comboHide.GetCount(); x++ )
	{
		if ( m_comboHide.GetItemData(x) == (WORD) n )
		{
			m_nHide = n;
			m_comboHide.SetCurSel(x);
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::SetDefault( int n )
{	
	for ( int x = 0; x < m_comboDefault.GetCount(); x++ )
	{
		if ( m_comboDefault.GetItemData(x) == (WORD) n )
		{
			m_nDefault = n;
			m_comboDefault.SetCurSel(x);
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::OnSelectChanges()
{
	bool bEnable = ( m_comboChanges.GetCurSel() != 0 );
	m_checkItemStandard.EnableWindow( bEnable );
	m_checkItemLarge.EnableWindow( bEnable );
	m_checkFunctionStandard.EnableWindow( bEnable );
	m_checkFunctionLarge.EnableWindow( bEnable );
	m_checkBottomBar.EnableWindow( bEnable );
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::OnSelectFontSize() 
{
	int nSel = m_comboFontSize.GetCurSel();

	if ( nSel >= 0 && nSel < m_comboFontSize.GetCount() )
	{
		m_nFontSize = m_comboFontSize.GetItemData( nSel );
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::OnSelectHorizontal() 
{
	int nSel = m_comboHorizontal.GetCurSel();

	if ( nSel >= 0 && nSel < m_comboHorizontal.GetCount() )
	{
		m_nHAlign = m_comboHorizontal.GetItemData( nSel );
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::OnSelectVertical() 
{
	int nSel = m_comboVertical.GetCurSel();

	if ( nSel >= 0 && nSel < m_comboVertical.GetCount() )
	{
		m_nVAlign = m_comboVertical.GetItemData( nSel );
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::OnSelectBold()
{
	int nSel = m_comboBold.GetCurSel();

	if ((nSel >= 0) && (nSel < m_comboBold.GetCount()))
	{
		m_nBold = m_comboBold.GetItemData(nSel);
		UpdatePreviewButton();
	}
}

/**********************************************************************/

long CSPOSKeyboardColourGlobalDlg::OnColourButtonClick( WPARAM wIndex, LPARAM lParam )
{
	if ( (wIndex >= 1) && (wIndex <= 64) )
	{
		switch( lParam )
		{
		case 0:
			{
				m_nForeground = wIndex;
				m_comboColour.SetCurSel(2);
				UpdatePreviewButton();
			}
			break;

		case 1:
			{
				m_nBackground = wIndex;
				m_comboColour.SetCurSel(2);
				UpdatePreviewButton();
			}
			break;
		}
	}

	return 0l;
}

/**********************************************************************/

long CSPOSKeyboardColourGlobalDlg::OnColourButtonDoubleClick( WPARAM wIndex, LPARAM lParam )
{
	if ( (wIndex >= 1) && (wIndex <= 64) )
	{
		switch( lParam )
		{
		case 0:
		case 1:
			EditColour( wIndex );
			break;
		}
	}
	else if ( 65 == wIndex )
	{
		switch( lParam )
		{
		case 0:
			EditColour( m_nForeground );
			break;

		case 1:
			EditColour( m_nBackground );
			break;
		}
	}

	return 0l;
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::EditColour( int nColourId )
{
	if ( ( nColourId >= 33 ) && ( nColourId <= 64 ) )
	{
		CColourEditDlg dlg ( nColourId, "", NULL, true, this );
	
		if ( dlg.DoModal() == IDOK )
		{
			int nRed = ( dlg.m_nRed & 0xFF );
			int nGreen = ( dlg.m_nGreen & 0xFF );
			int nBlue = ( dlg.m_nBlue & 0xFF );

			int nColourRef = ( nBlue * 0x10000 ) + ( nGreen * 0x100 ) + nRed;
			SPOSKeyboardManager.GetColourTable() -> SetCurrentRGB( nColourId, nColourRef );
		
			m_buttonColour[nColourId].m_ColourRefBack = nColourRef;
			m_buttonColour[nColourId].Invalidate();

			UpdatePreviewButton();
			SPOSKeyboardManager.GetModelessPointers() -> RedrawView();
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::OnButtonDefault()
{
	if ( Prompter.YesNo( "Are you sure you wish to restore the default user colours" ) == IDYES )
	{
		SPOSKeyboardManager.GetColourTable() -> RestoreDefaultRGB( 33, 64 );
		SPOSKeyboardManager.GetModelessPointers() -> RedrawView();
		
		for ( int n = 33; n <= 64; n++ )
		{
			m_buttonColour[n].m_ColourRefBack = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB(n);
			m_buttonColour[n].Invalidate();
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::OnButtonReset()
{
	if ( Prompter.YesNo( "Are you sure you wish to reset the user colours\nto their settings when this dialog was opened." ) == IDYES )
	{
		SPOSKeyboardManager.GetColourTable() -> RestoreUserColours();
		SPOSKeyboardManager.GetModelessPointers() -> RedrawView();
		
		for ( int n = 33; n <= 64; n++ )
		{
			m_buttonColour[n].m_ColourRefBack = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB(n);
			m_buttonColour[n].Invalidate();
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::OnButtonAll()
{
	SelectKeyTypes( TRUE );
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::OnButtonNone()
{
	SelectKeyTypes( FALSE );
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::SelectKeyTypes( bool b)
{
	m_bControl = b;
	m_bEmpty = b;
	m_bFunction = b;
	m_bList = b;
	m_bMacro = b;
	m_bModifier = b;
	m_bPayment = b;
	m_bPlu = b;
	m_bServer = b;
	UpdateData( FALSE );
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::OnSelectColourMode() 
{
	UpdatePreviewButton();
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::UpdatePreviewButton()
{
	COLORREF refFore, refBack;
	
	switch( m_comboColour.GetCurSel() )
	{
	case 0:
	case 1:
		//NO CHANGE
		//KEY DEFAULTS
		refFore = COLORREF ( 0x0 );
		refBack = COLORREF ( 0xCCCCCC );
		break;

	case 2:
		//AS SHOWN
		refFore = m_buttonColour[m_nForeground].m_ColourRefBack;
		refBack = m_buttonColour[m_nBackground].m_ColourRefBack;
		break;
	}

	int nFontSize = 8;
	int nSel = m_comboFontSize.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboFontSize.GetCount() )
	{
		int nData = m_comboFontSize.GetItemData(nSel);

		switch( nData )
		{
		case 999:	nFontSize = 8;	break;
		case 998:	nFontSize = 8;	break;
		default:	nFontSize = nData; break;
		}
	}

	int nHAlign = 1;
	nSel = m_comboHorizontal.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboHorizontal.GetCount() )
	{
		int nData = m_comboHorizontal.GetItemData( nSel );

		switch( nData )
		{
		case 999:	nHAlign = 1; break;
		case 998: nHAlign = 1; break;
		default:	nHAlign = nData; break;
		}
	}

	int nVAlign = 1;
	nSel = m_comboVertical.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboVertical.GetCount() )
	{
		int nData = m_comboVertical.GetItemData( nSel );

		switch( nData )
		{
		case 999:	nVAlign = 1; break;
		case 998: nVAlign = 1; break;
		default:	nVAlign = nData; break;
		}
	}

	bool bBold = FALSE;
	nSel = m_comboBold.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboBold.GetCount() )
	{
		int nData = m_comboBold.GetItemData( nSel );

		switch( nData )
		{
		case 999:	bBold = FALSE; break;
		case 998: bBold = FALSE; break;
		default:	bBold = ( nData == 1); break;
		}
	}

	m_buttonColour[0].SetFontSize( nFontSize );
	m_buttonColour[0].SetHAlign( nHAlign );
	m_buttonColour[0].SetVAlign( nVAlign );
	m_buttonColour[0].SetBoldFlag( bBold );
	m_buttonColour[0].m_ColourRefText = refFore;
	m_buttonColour[0].m_ColourRefBack = refBack;
	m_buttonColour[0].Invalidate();
}

/**********************************************************************/

void CSPOSKeyboardColourGlobalDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		int nColourMode = m_comboColour.GetCurSel();
		int nFontSel = m_comboFontSize.GetCurSel();
		int nHoriSel = m_comboHorizontal.GetCurSel();
		int nVertSel = m_comboVertical.GetCurSel();
		int nBoldSel = m_comboBold.GetCurSel();
		int nHideSel = m_comboHide.GetCurSel();
		int nDefaultSel = m_comboDefault.GetCurSel();
		
		int nChangesSel = m_comboChanges.GetCurSel();
		bool bChangeItemStandard = ( m_checkItemStandard.GetCheck() != 0 );
		bool bChangeItemLarge = ( m_checkItemLarge.GetCheck() != 0 );
		bool bChangeFunctionStandard = ( m_checkFunctionStandard.GetCheck() != 0 );
		bool bChangeFunctionLarge = ( m_checkFunctionLarge.GetCheck() != 0 );
		bool bChangeBottomBar = ( m_checkBottomBar.GetCheck() != 0 );

		if (	( 0 == nColourMode ) && 
				( 0 == nFontSel ) && 
				( 0 == m_bKeyText ) &&
				( 0 == m_bRemoveImage ) &&
				( 0 == nHoriSel ) && 
				( 0 == nVertSel ) && 
				( 0 == nBoldSel ) && 
				( 0 == nHideSel ) && 
				( 0 == nDefaultSel ) )
		{
			Prompter.Error ( "You have not selected any settings to change" );
			SPOSKeyboardManager.GetModelessPointers() -> RedrawView();
			return;
		}

		if (	( FALSE == m_bEmpty ) && 
				( FALSE == m_bFunction ) &&
				( FALSE == m_bPayment ) && 
				( FALSE == m_bModifier ) &&
				( FALSE == m_bControl ) &&
				( FALSE == m_bList ) &&
				( FALSE == m_bServer ) &&
				( FALSE == m_bMacro ) &&
				( FALSE == m_bPlu ) )
		{
			Prompter.Error ( "You have not selected any key types to change" );
			SPOSKeyboardManager.GetModelessPointers() -> RedrawView();
			return;
		}

		if ( nChangesSel != 0 )
		{
			if (	( FALSE == bChangeItemStandard ) &&
					( FALSE == bChangeItemLarge ) &&
					( FALSE == bChangeFunctionStandard ) &&
					( FALSE == bChangeFunctionLarge ) &&
					( FALSE == bChangeBottomBar ) )
			{
				Prompter.Error ( "You have not selected any list types to change" );
				SPOSKeyboardManager.GetModelessPointers() -> RedrawView();
				return;
			}
		}

		if ( 2 == nChangesSel )
		{
			CSPOSKeyboardPropertySheetListSelect propSheet( bChangeItemStandard, bChangeItemLarge, bChangeFunctionStandard, bChangeFunctionLarge, bChangeBottomBar, this );
			
			if ( propSheet.DoModal() != IDOK )
				return;
		}

		int nFontSize = m_comboFontSize.GetItemData( nFontSel );
		int nHori = m_comboHorizontal.GetItemData( nHoriSel );
		int nVert = m_comboVertical.GetItemData( nVertSel );
		int nBold = m_comboBold.GetItemData( nBoldSel );
		int nHide = m_comboHide.GetItemData( nHideSel );
		int nDefault = m_comboDefault.GetItemData( nDefaultSel );
		
		CSPOSKeyboardGlobalChangeRequest ChangeRequest;
		ChangeRequest.m_nColourMode = nColourMode;
		ChangeRequest.m_nFontSize = nFontSize;
		ChangeRequest.m_nHorizontal = nHori;
		ChangeRequest.m_nVertical = nVert;
		ChangeRequest.m_nBold = nBold;
		ChangeRequest.m_nHide = nHide;
		ChangeRequest.m_nDefault = nDefault;
		ChangeRequest.m_nForeground = m_nForeground;
		ChangeRequest.m_nBackground = m_nBackground;
		ChangeRequest.m_bKeyText = ( m_bKeyText != 0 );
		ChangeRequest.m_bRemoveImage = (m_bRemoveImage != 0);
		ChangeRequest.m_bControl = ( m_bControl != 0 );
		ChangeRequest.m_bEmpty = ( m_bEmpty != 0 );
		ChangeRequest.m_bFunction = ( m_bFunction != 0 );
		ChangeRequest.m_bList = ( m_bList != 0 );
		ChangeRequest.m_bMacro = ( m_bMacro != 0 );
		ChangeRequest.m_bModifier = ( m_bModifier != 0 );
		ChangeRequest.m_bPayment = ( m_bPayment != 0 );
		ChangeRequest.m_bPlu = ( m_bPlu != 0 );
		ChangeRequest.m_bServer = ( m_bServer != 0 );
		ChangeRequest.m_nChangesMode = nChangesSel;
		ChangeRequest.m_bChangeItemStandard = bChangeItemStandard;
		ChangeRequest.m_bChangeItemLarge = bChangeItemLarge;
		ChangeRequest.m_bChangeFunctionStandard = bChangeFunctionStandard;
		ChangeRequest.m_bChangeFunctionLarge = bChangeFunctionLarge;
		ChangeRequest.m_bChangeBottomBar = bChangeBottomBar;
		ChangeRequest.m_nVisibleListNo = m_nVisibleListNo;

		SPOSKeyboardManager.GetListManager() -> UpdateSettingsGlobally( ChangeRequest );
		SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
		SPOSKeyboardManager.GetModelessPointers() -> RedrawView();
		
		CString strMsg = "Key settings have been changed as requested.";

		Prompter.Info ( strMsg );
	}
}

/**********************************************************************/
