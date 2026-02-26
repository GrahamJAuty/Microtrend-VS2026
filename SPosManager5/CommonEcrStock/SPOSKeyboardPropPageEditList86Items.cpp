/**********************************************************************/
#include "SPOSKeyboardPropPageEditList86Items.h"
/**********************************************************************/

CSPOSKeyboardPropPageEditList86Items::CSPOSKeyboardPropPageEditList86Items() : CSSPropertyPage(CSPOSKeyboardPropPageEditList86Items::IDD)
{
	//{{AFX_DATA_INIT(CSPOSKeyboardPropPageEditList86Items)
	m_nForeground = 0;
	m_nBackground = 0;
	m_nHeight = 0;
	m_nWidth = 0;
	m_bNotTransparent = FALSE;
	m_bBold = FALSE;
	m_bBorder = FALSE;
	//}}AFX_DATA_INIT
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

void CSPOSKeyboardPropPageEditList86Items::SetListRecord( CSPOSKeyboardListRecord* pRecord )
{
	m_pListRecord = pRecord;
}

/**********************************************************************/

CSPOSKeyboardPropPageEditList86Items::~CSPOSKeyboardPropPageEditList86Items()
{
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditList86Items::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardPropPageEditList86Items)
	DDX_Control(pDX, IDC_COMBO_TEXTSIZE, m_comboTextSize);
	DDX_Text(pDX, IDC_EDIT_FOREGROUND, m_nForeground);
	DDX_Text(pDX, IDC_EDIT_BACKGROUND, m_nBackground);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nWidth);
	DDX_Check(pDX, IDC_CHECK_TRANS, m_bNotTransparent);
	DDX_Check(pDX, IDC_CHECK_BOLD, m_bBold);
	DDX_Check(pDX, IDC_CHECK_BORDER, m_bBorder);
	//}}AFX_DATA_MAP
	for (int n = 0; n < 65; n++)
	{
		DDX_Control(pDX, m_nButtonID[n], m_buttonColour[n]);
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPropPageEditList86Items, CPropertyPage)
	//{{AFX_MSG_MAP(CSPOSKeyboardPropPageEditList86Items)
	ON_EN_KILLFOCUS(IDC_EDIT_BACKGROUND, OnKillFocusBackground)
	ON_EN_KILLFOCUS(IDC_EDIT_FOREGROUND, OnKillFocusForeground)
	ON_EN_KILLFOCUS(IDC_EDIT_HEIGHT, OnKillFocus86Size)
	ON_BN_CLICKED(IDC_CHECK_TRANS, OnCheckTrans)
	ON_CBN_SELCHANGE(IDC_COMBO_TEXTSIZE, OnSelectFontSize)
	ON_EN_KILLFOCUS(IDC_EDIT_WIDTH, OnKillFocus86Size)
	ON_BN_CLICKED(IDC_CHECK_BOLD, OnCheckBold)
	//}}AFX_MSG_MAP
	ON_MESSAGE ( WM_APP, OnColourButtonClick)
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CSPOSKeyboardPropPageEditList86Items::GetEditBackground() { return GetEdit( IDC_EDIT_BACKGROUND ); }
/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditList86Items::OnInitDialog()
{
	//MUST DO THIS HERE SO THAT DDX/DDV WILL WORK FOR EDITS
	m_nForeground = m_pListRecord->GetForeground86();
	m_nBackground = m_pListRecord->GetBackground86();
	m_nWidth = m_pListRecord->GetWidth86();
	m_nHeight = m_pListRecord->GetHeight86();
	m_bBold = m_pListRecord->GetBold86();
	m_bBorder = m_pListRecord->GetBorder86();

	if (m_nBackground == 0)
	{
		m_nBackground = 1;
		m_bNotTransparent = FALSE;
	}
	else
	{
		m_bNotTransparent = TRUE;
	}

	ValidateKeySettings();

	CPropertyPage::OnInitDialog();

	for (int n = 1; n <= 64; n++)
	{
		m_buttonColour[n].m_ColourRefBack = SPOSKeyboardManager.GetColourTable()->GetCurrentRGB(n);
	}

	m_buttonColour[0].Set86ItemMode(TRUE);
	m_buttonColour[0].Set86ItemWidth(m_nWidth);
	m_buttonColour[0].Set86ItemHeight(m_nHeight);
	m_buttonColour[0].Set86TransparentFlag(m_bNotTransparent == 0);
	m_buttonColour[0].m_ColourRefBack = SPOSKeyboardManager.GetColourTable()->GetCurrentRGB(m_nBackground);
	m_buttonColour[0].m_ColourRefText = SPOSKeyboardManager.GetColourTable()->GetCurrentRGB(m_nForeground);
	m_buttonColour[0].SetFontSize(8);
	m_buttonColour[0].SetBoldFlag(m_bBold != 0);
	m_buttonColour[0].SetWindowText("86");

	SubclassEdit(IDC_EDIT_FOREGROUND, SS_NUM, 2, "%d");
	SubclassEdit(IDC_EDIT_BACKGROUND, SS_NUM, 2, "%d");
	SubclassEdit(IDC_EDIT_WIDTH, SS_NUM, 2, "%d");
	SubclassEdit(IDC_EDIT_HEIGHT, SS_NUM, 2, "%d");

	CWordArray arraySizes;
	arraySizes.Add(8);
	arraySizes.Add(10);
	arraySizes.Add(11);
	arraySizes.Add(12);
	arraySizes.Add(14);
	arraySizes.Add(16);
	arraySizes.Add(20);
	arraySizes.Add(24);

	for (int n = 0; n < arraySizes.GetSize(); n++)
	{
		CString strSize = "";
		int nSize = arraySizes.GetAt(n);
		strSize.Format("%d point", nSize);
		m_comboTextSize.AddString(strSize);
		m_comboTextSize.SetItemData(n, nSize);
	}

	WORD nTextSize = (WORD)m_pListRecord->GetFontSize86();

	m_comboTextSize.SetCurSel(0);
	for (int n = 0; n < m_comboTextSize.GetCount(); n++)
	{
		if (m_comboTextSize.GetItemData(n) == nTextSize)
		{
			m_comboTextSize.SetCurSel(n);
			m_buttonColour[0].SetFontSize(nTextSize);
			m_buttonColour[0].Invalidate();
			break;
		}
	}

	GetEditBackground()->EnableWindow(m_bNotTransparent != 0);

	return TRUE;
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditList86Items::OnKillActive()
{
	bool bResult = (CPropertyPage::OnKillActive() != 0);
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditList86Items::OnSetActive()
{
	bool bResult = (CPropertyPage::OnSetActive() != 0);
	return bResult;
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditList86Items::OnKillFocusForeground()
{
	UpdateData(TRUE, FALSE);
	ValidateKeySettings();
	UpdateData(FALSE);
	m_buttonColour[0].m_ColourRefText = SPOSKeyboardManager.GetColourTable()->GetCurrentRGB(m_nForeground);
	m_buttonColour[0].Invalidate();
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditList86Items::OnKillFocusBackground()
{
	UpdateData(TRUE, FALSE);
	ValidateKeySettings();
	UpdateData(FALSE);
	m_buttonColour[0].m_ColourRefBack = SPOSKeyboardManager.GetColourTable()->GetCurrentRGB(m_nBackground);
	m_buttonColour[0].Invalidate();
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditList86Items::OnKillFocus86Size()
{
	UpdateData(TRUE, FALSE);
	ValidateKeySettings();
	UpdateData(FALSE);
	m_buttonColour[0].Set86ItemWidth(m_nWidth);
	m_buttonColour[0].Set86ItemHeight(m_nHeight);
	m_buttonColour[0].Invalidate();
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditList86Items::ValidateKeySettings()
{
	if (m_nForeground < 1) m_nForeground = 1;
	if (m_nBackground < 1) m_nBackground = 1;
	m_nForeground = ((m_nForeground - 1) % 64) + 1;
	m_nBackground = ((m_nBackground - 1) % 64) + 1;
}

/**********************************************************************/

long CSPOSKeyboardPropPageEditList86Items::OnColourButtonClick(WPARAM wIndex, LPARAM lParam)
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
	return 0l;
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditList86Items::OnCheckTrans()
{
	UpdateData(TRUE, FALSE);
	m_buttonColour[0].Set86TransparentFlag(m_bNotTransparent == 0);
	GetEditBackground()->EnableWindow(m_bNotTransparent != 0);
	m_buttonColour[0].Invalidate();
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditList86Items::OnCheckBold()
{
	UpdateData(TRUE, FALSE);
	m_buttonColour[0].SetBoldFlag(m_bBold != 0);
	m_buttonColour[0].Invalidate();
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditList86Items::OnSelectFontSize()
{
	int nSel = m_comboTextSize.GetCurSel();
	if ((nSel >= 0) && (nSel <= m_comboTextSize.GetCount()))
	{
		int nFontSize = m_comboTextSize.GetItemData(nSel);
		m_buttonColour[0].SetFontSize(nFontSize);
		m_buttonColour[0].Invalidate();
	}
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditList86Items::SaveRecord()
{
	ValidateKeySettings();
	m_pListRecord->SetForeground86(m_nForeground);

	if (m_bNotTransparent != 0)
	{
		m_pListRecord->SetBackground86(m_nBackground);
	}
	else
	{
		m_pListRecord->SetBackground86(0);
	}

	m_pListRecord->SetWidth86(m_nWidth);
	m_pListRecord->SetHeight86(m_nHeight);
	m_pListRecord->SetBold86(m_bBold != 0);
	m_pListRecord->SetBorder86(m_bBorder != 0);

	int nSel = m_comboTextSize.GetCurSel();
	if (nSel >= 0 && nSel < m_comboTextSize.GetCount())
	{
		m_pListRecord->SetFontSize86(m_comboTextSize.GetItemData(nSel));
	}
}

/**********************************************************************/


