/**********************************************************************/
#include "..\SPosLoyalty4Shared\PresentationOptions.h"
/**********************************************************************/
#include "ColourEditDlg.h"
#include "MyMessageBoxDlg.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "SystemPropertyPage5.h"
/**********************************************************************/
IMPLEMENT_DYNCREATE(CSystemPropertyPage5, CPropertyPage)
/**********************************************************************/

CSystemPropertyPage5::CSystemPropertyPage5() : CSSPropertyPage(CSystemPropertyPage5::IDD)
{
	//{{AFX_DATA_INIT(CSystemPropertyPage5)
	//}}AFX_DATA_INIT
	m_nEditingColour = 0;
}

/**********************************************************************/

CSystemPropertyPage5::~CSystemPropertyPage5()
{
}

/**********************************************************************/

void CSystemPropertyPage5::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemPropertyPage5)

	DDX_Control(pDX, IDC_CHECK_MESSAGE, m_checkMessage);
	DDX_Control(pDX, IDC_STATIC_MESSAGE1, m_staticMessage1);
	DDX_Control(pDX, IDC_COLOURBUTTON_MESSAGEBACK, m_buttonColourMessageBack);
	DDX_Control(pDX, IDC_STATIC_MESSAGE2, m_staticMessage2);
	DDX_Control(pDX, IDC_COLOURBUTTON_MESSAGETEXT, m_buttonColourMessageText);
	DDX_Control(pDX, IDC_STATIC_MESSAGE3, m_staticMessage3);
	DDX_Control(pDX, IDC_COMBO_MESSAGE_FONT, m_comboMessageFont);
	DDX_Control(pDX, IDC_STATIC_MESSAGE4, m_staticMessage4);
	DDX_Control(pDX, IDC_COMBO_MESSAGE_SPACE, m_comboMessageSpace);
	DDX_Control(pDX, IDC_BUTTON_MESSAGE_PREVIEW, m_buttonMessagePreview);
	
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSystemPropertyPage5, CPropertyPage)
	//{{AFX_MSG_MAP(CSystemPropertyPage5)
	ON_BN_CLICKED( IDC_CHECK_MESSAGE, ToggleEnables )
	ON_BN_CLICKED( IDC_COLOURBUTTON_MESSAGEBACK, OnButtonMessageBack )
	ON_BN_CLICKED( IDC_COLOURBUTTON_MESSAGETEXT, OnButtonMessageText )
	ON_BN_CLICKED( IDC_BUTTON_MESSAGE_PREVIEW, OnButtonMessagePreview )
	//}}AFX_MSG_MAP
	ON_MESSAGE ( WM_COLOUR_CHANGE, OnColourButtonMessage )
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSystemPropertyPage5::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	//MESSAGE BOXES
	m_checkMessage.SetCheck( PresentationOptions.GetCustomMessageBoxFlag() );
	m_buttonColourMessageBack.m_bBlackBorder = TRUE;
	m_buttonColourMessageBack.m_ColourRefBack = PresentationOptions.GetCustomMessageBoxColourBackInt();
	m_buttonColourMessageBack.Invalidate();
	m_buttonColourMessageText.m_bBlackBorder = TRUE;
	m_buttonColourMessageText.m_ColourRefBack = PresentationOptions.GetCustomMessageBoxColourTextInt();

	m_comboMessageFont.AddString("8");
	m_comboMessageFont.AddString("10");
	m_comboMessageFont.AddString("11");
	m_comboMessageFont.AddString("12");
	m_comboMessageFont.AddString("14");
	
	switch( PresentationOptions.GetCustomMessageBoxFont() )
	{
	case 10:	m_comboMessageFont.SetCurSel(1);	break;
	case 11:	m_comboMessageFont.SetCurSel(2);	break;
	case 12:	m_comboMessageFont.SetCurSel(3);	break;
	case 14:	m_comboMessageFont.SetCurSel(4);	break;
	case 8:
	default:	m_comboMessageFont.SetCurSel(0);	break;
	}

	{
		for ( int n = 5; n <= 15; n++ )
		{
			CString str;
			str.Format( "%d", n );
			m_comboMessageSpace.AddString(str);
		}

		int nSel = PresentationOptions.GetCustomMessageBoxLineGap() - 5;
		if ( ( nSel >= 0 ) && ( nSel < m_comboMessageSpace.GetCount() ) )
			m_comboMessageSpace.SetCurSel( nSel );
		else
			m_comboMessageSpace.SetCurSel( 0 );
	}

	m_buttonColourMessageText.Invalidate();

	SetRecordControls();
	ToggleEnables();

	return TRUE;  
}

/**********************************************************************/

void CSystemPropertyPage5::ToggleEnables()
{
	bool bEnable = ( m_checkMessage.GetCheck() != 0 );
	m_buttonColourMessageBack.EnableWindow( bEnable );
	m_buttonColourMessageText.EnableWindow( bEnable );
	m_comboMessageFont.EnableWindow( bEnable );
	m_comboMessageSpace.EnableWindow( bEnable );
	m_buttonMessagePreview.EnableWindow( bEnable );
}

/**********************************************************************/

BOOL CSystemPropertyPage5::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CSystemPropertyPage5::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CSystemPropertyPage5::GetRecordData()
{
}

/**********************************************************************/

void CSystemPropertyPage5::SaveRecord()
{
	//CUSTOM MESSAGE BOX
	PresentationOptions.SetCustomMessageBoxFlag( m_checkMessage.GetCheck() != 0 );

	switch( m_comboMessageFont.GetCurSel() )
	{
	case 1:		PresentationOptions.SetCustomMessageBoxFont(10);	break;
	case 2:		PresentationOptions.SetCustomMessageBoxFont(11);	break;
	case 3:		PresentationOptions.SetCustomMessageBoxFont(12);	break;
	case 4:		PresentationOptions.SetCustomMessageBoxFont(14);	break;
	case 0:	
	default:	PresentationOptions.SetCustomMessageBoxFont(8);	break;
	}

	PresentationOptions.SetCustomMessageBoxLineGap( 5 + m_comboMessageSpace.GetCurSel() );
}

/**********************************************************************/

void CSystemPropertyPage5::SetRecordControls()
{
}

/**********************************************************************/

void CSystemPropertyPage5::OnButtonMessageBack()
{
	int nNewColour = PresentationOptions.GetCustomMessageBoxColourBackInt();
	CColourEditDlg dlg( nNewColour, "Set Message Box Background Colour", this, this );
	dlg.SetSuggestedColour( 16, 0xFFFFFF );

	m_nEditingColour = 2;
	if ( dlg.DoModal() == IDOK )
		nNewColour = dlg.GetColourRef();
	
	PresentationOptions.SetCustomMessageBoxColourBackInt( nNewColour );
	
	m_buttonColourMessageBack.m_ColourRefBack = nNewColour;
	m_buttonColourMessageBack.Invalidate();
}

/**********************************************************************/

void CSystemPropertyPage5::OnButtonMessageText()
{
	int nNewColour = PresentationOptions.GetCustomMessageBoxColourTextInt();
	CColourEditDlg dlg( nNewColour, "Set Message Box Text Colour", this, this );
	dlg.SetSuggestedColour( 16, 0x0 );

	m_nEditingColour = 3;
	if ( dlg.DoModal() == IDOK )
		nNewColour = dlg.GetColourRef();
	
	PresentationOptions.SetCustomMessageBoxColourTextInt( nNewColour );
	
	m_buttonColourMessageText.m_ColourRefBack = nNewColour;
	m_buttonColourMessageText.Invalidate();
}

/**********************************************************************/

long CSystemPropertyPage5::OnColourButtonMessage( WPARAM w, LPARAM l )
{
	switch( m_nEditingColour )
	{
	case 2:
		m_buttonColourMessageBack.m_ColourRefBack = l;
		m_buttonColourMessageBack.Invalidate();
		PresentationOptions.SetCustomMessageBoxColourBackInt(l);
		break;

	case 3:
		m_buttonColourMessageText.m_ColourRefBack = l;
		m_buttonColourMessageText.Invalidate();
		PresentationOptions.SetCustomMessageBoxColourTextInt(l);
		break;
	}

	return 0l;
}

/**********************************************************************/

void CSystemPropertyPage5::OnButtonMessagePreview()
{
	int nFontSize = 8;
	switch( m_comboMessageFont.GetCurSel() )
	{
	case 1:		nFontSize = 10;	break;
	case 2:		nFontSize = 11;	break;
	case 3:		nFontSize = 12;	break;
	case 4:		nFontSize = 14;	break;
	case 0:	
	default:	nFontSize = 8;	break;
	}

	CMyMessageBoxInfo infoMessage;
	infoMessage.SetIconType( MB_ICONINFORMATION );
	infoMessage.SetButtonMode( MB_OK );
	infoMessage.SetLineGap( 5 + m_comboMessageSpace.GetCurSel() );
	infoMessage.SetFontSize( nFontSize );
	infoMessage.SetBackColour( PresentationOptions.GetCustomMessageBoxColourBackInt() );
	infoMessage.SetTextColour( PresentationOptions.GetCustomMessageBoxColourTextInt() );
	infoMessage.SetTitle( ::GetTidyAppName(FALSE) );
	infoMessage.SetTexts( "This is an example message box\nwith three lines of text\nautomatically sized to fit the longest line" );	
	CMyMessageBoxDlg dlg( infoMessage, this );
	dlg.DoModal();
}

/**********************************************************************/
