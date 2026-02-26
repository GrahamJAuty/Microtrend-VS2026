/**********************************************************************/
#include "ColourEditDlg.h"
/**********************************************************************/
#include "SPOSKeyboardColourServerDlg.h"
/**********************************************************************/

CSPOSKeyboardColourServerDlg::CSPOSKeyboardColourServerDlg( CSPOSKeyboardKeyRecord& KeyRecord, int nType, CWnd* pParent /*=NULL*/)
	: CDialog(CSPOSKeyboardColourServerDlg::IDD, pParent), m_KeyRecord( KeyRecord )
{
	//{{AFX_DATA_INIT(CSPOSKeyboardColourServerDlg)
	//}}AFX_DATA_INIT
	m_nType = nType;

	m_nForeground = 1;
	m_nBackground = 4;
	m_nFontSize = 999;
	m_nHAlign = 1;
	m_nVAlign = 1;
	m_nBold = 0;
	
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

	for ( int n = 0; n < 65; n++ )
		m_buttonColour[n].m_nButtonNo = n;
}

/**********************************************************************/

void CSPOSKeyboardColourServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardColourServerDlg)
	DDX_Control(pDX, IDC_COMBO_VERTICAL, m_comboVertical);
	DDX_Control(pDX, IDC_COMBO_HORIZONTAL, m_comboHorizontal);
	DDX_Control(pDX, IDC_COMBO_BOLD, m_comboBold);
	DDX_Control(pDX, IDC_COMBO_TEXTSIZE, m_comboFontSize);
	DDX_Control(pDX, IDC_STATIC1, m_static1);
	DDX_Control(pDX, IDC_STATIC2, m_static2);
	DDX_Control(pDX, IDC_STATIC3, m_static3);
	DDX_Control(pDX, IDC_STATIC4, m_static4);
	//}}AFX_DATA_MAP
	for ( int n = 0; n < 65; n++ )
		DDX_Control(pDX, m_nButtonID[n], m_buttonColour[n]);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardColourServerDlg, CDialog)
	//{{AFX_MSG_MAP(CSPOSKeyboardColourServerDlg)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_CBN_SELCHANGE(IDC_COMBO_TEXTSIZE, OnSelectFontSize)
	ON_CBN_SELCHANGE(IDC_COMBO_BOLD, OnSelectBold)
	ON_CBN_SELCHANGE(IDC_COMBO_HORIZONTAL, OnSelectHorizontal)
	ON_CBN_SELCHANGE(IDC_COMBO_VERTICAL, OnSelectVertical)
	//}}AFX_MSG_MAP
	ON_WM_GETMINMAXINFO()
	ON_MESSAGE ( WM_APP, OnColourButtonClick)
	ON_MESSAGE ( WM_APP + 1, OnColourButtonDoubleClick)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardColourServerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	switch( m_nType )
	{
	case 0:
		SetWindowText( "Set Active Server Background Colour (Logged On)" );
		break;

	case 1:
		SetWindowText( "Set Active Server Background Colour (Logged Off)" );
		break;

	case 2:
		SetWindowText( "Set Active Server Background Colour (Unused Key)" );
		break;

	case 3:
		SetWindowText( "Set Active Server Line Format (Server Name)" );
		break;

	case 4:
		SetWindowText( "Set Active Server Line Format (Server Number)" );
		break;

	case 5:
		SetWindowText( "Set Active Server Line Format (Till Number)" );
		break;

	case 6:
		SetWindowText( "Set Active Server Line Format (Overlap Amount)" );
		break;
	}

	for ( int n = 1; n <= 64; n++ )
		m_buttonColour[n].m_ColourRefBack = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB(n);

	switch( m_nType )
	{
	case 0:
	case 1:
	case 2:
		m_buttonColour[0].SetWindowText( "" );
		m_comboFontSize.ShowWindow( SW_HIDE );
		m_comboHorizontal.ShowWindow( SW_HIDE );
		m_comboVertical.ShowWindow( SW_HIDE );
		m_comboBold.ShowWindow( SW_HIDE );
		m_static1.ShowWindow( SW_HIDE );
		m_static2.ShowWindow( SW_HIDE );
		m_static3.ShowWindow( SW_HIDE );
		m_static4.ShowWindow( SW_HIDE );
		break;

	default:
		m_buttonColour[0].SetWindowText( "ABCD" );

		FillFontCombo();
		FillHorizontalCombo();
		FillVerticalCombo();
		FillBoldCombo();
	
		SetForeground( m_KeyRecord.GetForeground() );
		break;
	}

	SetBackground( m_KeyRecord.GetBackground() );
	UpdatePreviewButton();

	return TRUE;  
}

/**********************************************************************/

void CSPOSKeyboardColourServerDlg::FillFontCombo()
{
	m_comboFontSize.ResetContent();

	CWordArray arraySizes;
	arraySizes.Add(8);
	arraySizes.Add(10);
	arraySizes.Add(11);
	arraySizes.Add(12);
	arraySizes.Add(14);
	arraySizes.Add(16);
	arraySizes.Add(20);
	arraySizes.Add(24);
	
	int nPos = 0;
	for( int n = 0; n < arraySizes.GetSize(); n++ )
	{
		CString strSize;
		int nSize = arraySizes.GetAt(n);
		strSize.Format( "%d point", nSize );
		m_comboFontSize.AddString( strSize );
		m_comboFontSize.SetItemData( n, nSize );

		if ( m_KeyRecord.GetFontSize() == nSize )
			nPos = n;
	}

	m_comboFontSize.SetCurSel(nPos);
}

/**********************************************************************/

void CSPOSKeyboardColourServerDlg::FillHorizontalCombo()
{
	m_comboHorizontal.ResetContent();
	m_comboHorizontal.AddString( "Left" );
	m_comboHorizontal.AddString( "Centre" );
	m_comboHorizontal.AddString( "Right" );
	
	int nPos = m_KeyRecord.GetAlignmentH();
	if ( ( nPos >= 0 ) && ( nPos <= 2 ) )
		m_comboHorizontal.SetCurSel( nPos );
	else
		m_comboHorizontal.SetCurSel( 0 );
}

/**********************************************************************/

void CSPOSKeyboardColourServerDlg::FillVerticalCombo()
{
	m_comboVertical.ResetContent();
	m_comboVertical.AddString( "Top" );
	m_comboVertical.AddString( "Centre" );
	m_comboVertical.AddString( "Bottom" );

	int nPos = m_KeyRecord.GetAlignmentV();
	if ( ( nPos >= 0 ) && ( nPos <= 2 ) )
		m_comboVertical.SetCurSel( nPos );
	else
		m_comboVertical.SetCurSel( 0 );
}

/**********************************************************************/

void CSPOSKeyboardColourServerDlg::FillBoldCombo()
{
	m_comboBold.ResetContent();
	m_comboBold.AddString( "Yes" );
	m_comboBold.AddString( "No" );
	m_comboBold.SetCurSel( m_KeyRecord.GetBoldFlag() ? 0 : 1 );
}

/**********************************************************************/

void CSPOSKeyboardColourServerDlg::SetForeground( int n )
{
	if ( n >= 1 && n <= 64 )
	{
		m_nForeground = n;
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourServerDlg::SetBackground( int n )
{
	if ( n >= 1 && n <= 64 )
	{
		m_nBackground = n;
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourServerDlg::SetFontSize( int n )
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

void CSPOSKeyboardColourServerDlg::SetHAlign( int n )
{	
	if ( ( n >= 0 ) && ( n <= 2 ) )
	{
		m_comboHorizontal.SetCurSel(n);
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourServerDlg::SetVAlign( int n )
{	
	if ( ( n >= 0 ) && ( n <= 2 ) )
	{
		m_comboVertical.SetCurSel(n);
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourServerDlg::SetBold( int n )
{	
	if ( ( n >= 0 ) && ( n <= 1) ) 
	{
		m_nBold = n;
		m_comboBold.SetCurSel( 1 - n );
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourServerDlg::OnSelectFontSize() 
{
	int nSel = m_comboFontSize.GetCurSel();

	if ( nSel >= 0 && nSel < m_comboFontSize.GetCount() )
	{
		m_nFontSize = m_comboFontSize.GetItemData( nSel );
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourServerDlg::OnSelectHorizontal() 
{
	int nSel = m_comboHorizontal.GetCurSel();

	if ( nSel >= 0 && nSel < m_comboHorizontal.GetCount() )
	{
		m_nHAlign = nSel;
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourServerDlg::OnSelectVertical() 
{
	int nSel = m_comboVertical.GetCurSel();

	if ( nSel >= 0 && nSel < m_comboVertical.GetCount() )
	{
		m_nVAlign = nSel;
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourServerDlg::OnSelectBold() 
{
	int nSel = m_comboBold.GetCurSel();

	if ( ( nSel >= 0 ) && ( nSel <= 1 ) )
	{
		m_nBold = 1 - nSel;
		UpdatePreviewButton();
	}
}

/**********************************************************************/

long CSPOSKeyboardColourServerDlg::OnColourButtonClick( WPARAM wIndex, LPARAM lParam )
{
	if ( wIndex >= 1 && wIndex <= 64 )
	{
		switch( lParam )
		{
		case 0:
		case 1:
			{
				switch( m_nType )
				{
				case 0:
				case 1:
				case 2:
					m_nBackground = wIndex;
					break;

				default:
					m_nForeground = wIndex;
					break;
				}

				UpdatePreviewButton();
			}
			break;
		}
	}

	return 0l;
}

/**********************************************************************/

long CSPOSKeyboardColourServerDlg::OnColourButtonDoubleClick( WPARAM wIndex, LPARAM lParam )
{
	if ( wIndex >= 1 && wIndex <= 64 )
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

void CSPOSKeyboardColourServerDlg::EditColour( int nColourId )
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

void CSPOSKeyboardColourServerDlg::OnButtonDefault()
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

void CSPOSKeyboardColourServerDlg::OnButtonReset()
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

void CSPOSKeyboardColourServerDlg::UpdatePreviewButton()
{
	COLORREF refFore = m_buttonColour[m_nForeground].m_ColourRefBack;
	COLORREF refBack = m_buttonColour[m_nBackground].m_ColourRefBack;
	
	switch( m_nType )
	{
	case 0:
	case 1:
	case 2:
		break;

	default:
		{
			int nFontSize = 8;
			int nSel = m_comboFontSize.GetCurSel();
			if ( nSel >= 0 && nSel < m_comboFontSize.GetCount() )
				nFontSize = m_comboFontSize.GetItemData(nSel);

			int nHAlign = 1;
			nSel = m_comboHorizontal.GetCurSel();
			if ( nSel >= 0 && nSel < m_comboHorizontal.GetCount() )
				nHAlign = nSel;

			int nVAlign = 1;
			nSel = m_comboVertical.GetCurSel();
			if ( nSel >= 0 && nSel < m_comboVertical.GetCount() )
				nVAlign = nSel;

			bool bBold = ( m_comboBold.GetCurSel() == 0 );

			m_buttonColour[0].SetFontSize( nFontSize );
			m_buttonColour[0].SetHAlign( nHAlign );
			m_buttonColour[0].SetVAlign( nVAlign );
			m_buttonColour[0].SetBoldFlag( bBold );
			m_buttonColour[0].m_ColourRefText = refFore;
		}
		break;
	}

	m_buttonColour[0].m_ColourRefBack = refBack;		
	m_buttonColour[0].Invalidate();
}

/**********************************************************************/

void CSPOSKeyboardColourServerDlg::OnOK() 
{
	switch( m_nType )
	{
	case 0:
	case 1:
	case 2:
		break;

	default:
		{	
			int nSel = m_comboFontSize.GetCurSel();
			m_KeyRecord.SetFontSize( m_comboFontSize.GetItemData(nSel) );
			m_KeyRecord.SetAlignmentH( m_comboHorizontal.GetCurSel() );
			m_KeyRecord.SetAlignmentV( m_comboVertical.GetCurSel() );
			m_KeyRecord.SetBoldFlag( m_comboBold.GetCurSel() == 0 );
			m_KeyRecord.SetForeground( m_nForeground );
		}
		break;
	}

	m_KeyRecord.SetBackground( m_nBackground );
	EndDialog( IDOK );
}

/**********************************************************************/
