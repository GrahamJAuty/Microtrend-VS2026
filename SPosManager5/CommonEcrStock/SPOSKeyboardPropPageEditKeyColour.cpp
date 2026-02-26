/**********************************************************************/
#include "SPOSKeyboardView.h"
/**********************************************************************/
#include "ColourEditDlg.h"
#include "SPOSKeyboardPropertySheetEditKey.h"
#include "SPOSKeyboardPropPageEditKeyColour.h"
/**********************************************************************/

CSPOSKeyboardPropPageEditKeyColour::CSPOSKeyboardPropPageEditKeyColour() : CSSPropertyPage(CSPOSKeyboardPropPageEditKeyColour::IDD)
{
	//{{AFX_DATA_INIT(CSPOSKeyboardPropPageEditKeyColour)
	//}}AFX_DATA_INIT
	m_nForeground = 0;
	m_nBackground = 0;
	m_nFontSize = 0;
	m_bBold = FALSE;
	m_pView = NULL;
	m_pKeyRecord = NULL;

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

	m_pParentSheet = NULL;
}

/**********************************************************************/

CSPOSKeyboardPropPageEditKeyColour::~CSPOSKeyboardPropPageEditKeyColour()
{
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyColour::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardPropPageEditKeyColour)
	DDX_Control(pDX, IDC_COMBO_VERTICAL, m_comboVertical);
	DDX_Control(pDX, IDC_COMBO_HORIZONTAL, m_comboHorizontal);
	DDX_Control(pDX, IDC_COMBO_TEXTSIZE, m_comboFontSize);
	DDX_Text(pDX, IDC_EDIT_FOREGROUND, m_nForeground);
	DDX_Text(pDX, IDC_EDIT_BACKGROUND, m_nBackground);
	DDX_Check(pDX, IDC_CHECK_BOLD, m_bBold);
	//}}AFX_DATA_MAP

	for (int n = 0; n < 65; n++)
	{
		DDX_Control(pDX, m_nButtonID[n], m_buttonColour[n]);
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPropPageEditKeyColour, CPropertyPage)
	//{{AFX_MSG_MAP(CSPOSKeyboardPropPageEditKeyColour)
	ON_CBN_SELCHANGE(IDC_COMBO_TEXTSIZE, OnSelectFont)
	ON_EN_KILLFOCUS(IDC_EDIT_BACKGROUND, OnKillFocusBackground)
	ON_EN_KILLFOCUS(IDC_EDIT_FOREGROUND, OnKillFocusForeground)
	ON_BN_CLICKED(IDC_CHECK_BOLD, OnCheckBold)
	ON_CBN_SELCHANGE(IDC_COMBO_HORIZONTAL, OnSelectAlign)
	ON_CBN_SELCHANGE(IDC_COMBO_VERTICAL, OnSelectAlign)
	//}}AFX_MSG_MAP
	ON_MESSAGE ( WM_APP, OnColourButtonClick)
	ON_MESSAGE ( WM_APP + 1, OnColourButtonDoubleClick)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditKeyColour::OnInitDialog() 
{
	//MUST DO THIS HERE SO THAT DDX/DDV WILL WORK FOR EDITS
	m_nForeground = m_pKeyRecord -> GetForeground(); 
	m_nBackground = m_pKeyRecord -> GetBackground(); 
	m_nFontSize = m_pKeyRecord-> GetFontSize(); 
	m_bBold = m_pKeyRecord-> GetBoldFlag();

	ValidateKeySettings();
	
	CPropertyPage::OnInitDialog();

	for (int n = 1; n <= 64; n++)
	{
		m_buttonColour[n].m_ColourRefBack = SPOSKeyboardManager.GetColourTable()->GetCurrentRGB(n);
	}

	SubclassEdit( IDC_EDIT_FOREGROUND, SS_NUM, 2, "%d" );
	SubclassEdit( IDC_EDIT_BACKGROUND, SS_NUM, 2, "%d" );

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

	WORD nTextSize = (WORD) m_nFontSize;

	m_comboFontSize.SetCurSel(0);
	for ( int n = 0; n < m_comboFontSize.GetCount(); n++ )
	{
		if ( m_comboFontSize.GetItemData(n) == nTextSize  )
		{
			m_comboFontSize.SetCurSel(n);
			m_buttonColour[0].SetFontSize( nTextSize );
			m_buttonColour[0].Invalidate();
			break;
		}
	}

	m_comboHorizontal.AddString( "Left" );
	m_comboHorizontal.AddString( "Centre" );
	m_comboHorizontal.AddString( "Right" );

	int nHAlign = m_pKeyRecord -> GetAlignmentH();
	if ((nHAlign >= 0) && (nHAlign <= 2))
	{
		m_comboHorizontal.SetCurSel(nHAlign);
	}
	else
	{
		m_comboHorizontal.SetCurSel(0);
	}

	m_comboVertical.AddString( "Top" );
	m_comboVertical.AddString( "Centre" );
	m_comboVertical.AddString( "Bottom" );

	int nVAlign = m_pKeyRecord -> GetAlignmentV();
	if ((nVAlign >= 0) && (nVAlign <= 2))
	{
		m_comboVertical.SetCurSel(nVAlign);
	}
	else
	{
		m_comboVertical.SetCurSel(0);
	}

	m_buttonColour[0].m_ColourRefBack = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB( m_nBackground );
	m_buttonColour[0].m_ColourRefText = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB( m_nForeground );
	m_buttonColour[0].SetFontSize ( m_nFontSize );
	m_buttonColour[0].SetBoldFlag( m_bBold != 0 );
	m_buttonColour[0].SetHAlign( m_comboHorizontal.GetCurSel() );
	m_buttonColour[0].SetVAlign( m_comboVertical.GetCurSel() );
	m_buttonColour[0].SetWindowText( "ABCD" );
	
	return TRUE;
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyColour::ValidateKeySettings()
{
	if (m_nForeground < 1)
	{
		m_nForeground = 1;
	}

	if (m_nBackground < 1)
	{
		m_nBackground = 1;
	}

	m_nForeground = ((m_nForeground - 1) % 64) + 1;
	m_nBackground = ((m_nBackground - 1) % 64) + 1;

	if (CSPOSKeyboardKeyRecord::IsValidFontSize(m_nFontSize) == FALSE)
	{
		m_nFontSize = 8;
	}
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditKeyColour::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditKeyColour::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

long CSPOSKeyboardPropPageEditKeyColour::OnColourButtonClick(WPARAM wIndex, LPARAM lParam)
{
	UpdateData(TRUE, FALSE);

	if ((wIndex >= 1) && (wIndex <= 64))
	{
		switch (lParam)
		{
		case 0:
		{
			int nColourRef = m_buttonColour[wIndex].m_ColourRefBack;
			m_buttonColour[0].m_ColourRefText = nColourRef;
			m_buttonColour[0].Invalidate();
			m_nForeground = wIndex;
		}
		break;

		case 1:
		{
			int nColourRef = m_buttonColour[wIndex].m_ColourRefBack;
			m_buttonColour[0].m_ColourRefBack = nColourRef;
			m_buttonColour[0].Invalidate();
			m_nBackground = wIndex;
		}
		break;
		}
	}

	UpdateData(FALSE);
	RedrawKey();
	return 0l;
}

/**********************************************************************/

long CSPOSKeyboardPropPageEditKeyColour::OnColourButtonDoubleClick(WPARAM wIndex, LPARAM lParam)
{
	if ((wIndex >= 1) && (wIndex <= 64))
	{
		switch (lParam)
		{
		case 0:
		case 1:
			EditColour(wIndex);
			break;
		}
	}
	else if (65 == wIndex)
	{
		switch (lParam)
		{
		case 0:
			EditColour(m_nForeground);
			break;

		case 1:
			EditColour(m_nBackground);
			break;
		}
	}

	RedrawKey();
	return 0l;
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyColour::EditColour(int nColourId)
{
	if ((nColourId >= 33) && (nColourId <= 64))
	{
		CColourEditDlg dlg(nColourId, "", NULL, TRUE, this);

		if (dlg.DoModal() == IDOK)
		{
			int nRed = (dlg.m_nRed & 0xFF);
			int nGreen = (dlg.m_nGreen & 0xFF);
			int nBlue = (dlg.m_nBlue & 0xFF);

			int nColourRef = (nBlue * 0x10000) + (nGreen * 0x100) + nRed;
			SPOSKeyboardManager.GetColourTable()->SetCurrentRGB(nColourId, nColourRef);

			m_buttonColour[nColourId].m_ColourRefBack = nColourRef;
			m_buttonColour[nColourId].Invalidate();

			m_buttonColour[0].m_ColourRefText = m_buttonColour[m_nForeground].m_ColourRefBack;
			m_buttonColour[0].m_ColourRefBack = m_buttonColour[m_nBackground].m_ColourRefBack;
			m_buttonColour[0].Invalidate();

			m_pView->Invalidate();
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyColour::OnKillFocusForeground() 
{
	UpdateData( TRUE, FALSE );
	ValidateKeySettings();
	UpdateData( FALSE );
	m_buttonColour[0].m_ColourRefText = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB( m_nForeground );
	m_buttonColour[0].Invalidate();
	RedrawKey();
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyColour::OnKillFocusBackground() 
{
	UpdateData( TRUE, FALSE );
	ValidateKeySettings();
	UpdateData( FALSE );
	m_buttonColour[0].m_ColourRefBack = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB( m_nBackground );
	m_buttonColour[0].Invalidate();
	RedrawKey();
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyColour::OnSelectFont()
{
	int nSel = m_comboFontSize.GetCurSel();
	if ((nSel >= 0) && (nSel < m_comboFontSize.GetCount()))
	{
		m_nFontSize = m_comboFontSize.GetItemData(nSel);
		m_buttonColour[0].SetFontSize(m_nFontSize);
		m_buttonColour[0].Invalidate();
		RedrawKey();
	}
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyColour::OnSelectAlign() 
{
	m_buttonColour[0].SetHAlign( m_comboHorizontal.GetCurSel() );
	m_buttonColour[0].SetVAlign( m_comboVertical.GetCurSel() );	
	m_buttonColour[0].Invalidate();
	RedrawKey();
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyColour::OnCheckBold() 
{
	UpdateData( TRUE, FALSE );
	m_buttonColour[0].SetBoldFlag( m_bBold != 0 );
	m_buttonColour[0].Invalidate();
	RedrawKey();
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyColour::SaveRecord()
{
	ValidateKeySettings();
	m_pKeyRecord -> SetForeground( m_nForeground );
	m_pKeyRecord -> SetBackground( m_nBackground );
	m_pKeyRecord -> SetFontSize( m_nFontSize );
	m_pKeyRecord -> SetAlignmentH( m_comboHorizontal.GetCurSel() );
	m_pKeyRecord -> SetAlignmentV( m_comboVertical.GetCurSel() );
	m_pKeyRecord -> SetBoldFlag( m_bBold != 0 );
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyColour::RedrawKey()
{
	if ( m_pParentSheet != NULL )
	{
		SaveRecord();
		( (CSPOSKeyboardPropertySheetEditKey*) m_pParentSheet ) -> RedrawKey();
	}
}

/**********************************************************************/
