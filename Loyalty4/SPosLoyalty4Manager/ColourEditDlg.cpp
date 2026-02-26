/**********************************************************************/
#include "ColourEditDlg.h"
#include "WMNumbers.h"
/**********************************************************************/

CColourEditDlg::CColourEditDlg( int nColourRef, const char* szTitle, CWnd* pMainWnd, CWnd* pParent)
	: CSSDialog(CColourEditDlg::IDD, pParent)
{
	m_pMainWnd = pMainWnd;

	//{{AFX_DATA_INIT(CColourEditDlg)
	m_nBlue = 0;
	m_nGreen = 0;
	m_nRed = 0;
	//}}AFX_DATA_INIT

	m_nColourRef = nColourRef;
	m_strTitle = szTitle;

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
	
	m_buttonColour[1].m_ColourRefBack = COLORREF( 0x0 );
	m_buttonColour[2].m_ColourRefBack = COLORREF( 0xA00000 );
	m_buttonColour[3].m_ColourRefBack = COLORREF( 0xA000 );
	m_buttonColour[4].m_ColourRefBack = COLORREF( 0xA0A000 );
	m_buttonColour[5].m_ColourRefBack = COLORREF( 0xA0 );
	m_buttonColour[6].m_ColourRefBack = COLORREF( 0xA000A0 );
	m_buttonColour[7].m_ColourRefBack = COLORREF( 0xA0A0 );
	m_buttonColour[8].m_ColourRefBack = COLORREF( 0xC0C0C0 );
	m_buttonColour[9].m_ColourRefBack = COLORREF( 0xA0A0A0 );
	m_buttonColour[10].m_ColourRefBack = COLORREF( 0xFF0000 );
	m_buttonColour[11].m_ColourRefBack = COLORREF( 0xFF00 );
	m_buttonColour[12].m_ColourRefBack = COLORREF( 0xFFFF00 );
	m_buttonColour[13].m_ColourRefBack = COLORREF( 0xFF );
	m_buttonColour[14].m_ColourRefBack = COLORREF( 0xFF00FF );
	m_buttonColour[15].m_ColourRefBack = COLORREF( 0xFFFF );
	m_buttonColour[16].m_ColourRefBack = COLORREF( 0xF0F0F0 );
	m_buttonColour[17].m_ColourRefBack = COLORREF( m_nColourRef );

	for ( int n = 0; n < 18; n++ )
	{
		m_buttonColour[n].m_nButtonNo = n;
		m_buttonColour[n].m_bBlackBorder = TRUE;
	}
	
	m_buttonColour[0].m_ColourRefBack = COLORREF( m_nColourRef );

	m_nBlue = ( m_nColourRef & 0xFF0000 ) / 0x10000;
	m_nGreen = ( m_nColourRef & 0xFF00 ) / 0x100;
	m_nRed = ( m_nColourRef & 0xFF );
}

/**********************************************************************/

void CColourEditDlg::SetSuggestedColour( int nID, int nColour )
{
	if ( ( nID >= 1 ) && ( nID <= 16 ) )
		m_buttonColour[nID].m_ColourRefBack = nColour;
}

/**********************************************************************/

void CColourEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColourEditDlg)
	DDX_Control(pDX, IDC_SLIDER_RED, m_sliderRed);
	DDX_Control(pDX, IDC_SLIDER_GREEN, m_sliderGreen);
	DDX_Control(pDX, IDC_SLIDER_BLUE, m_sliderBlue);
	DDX_Text(pDX, IDC_EDIT_BLUE, m_nBlue);
	DDX_Text(pDX, IDC_EDIT_GREEN, m_nGreen);
	DDX_Text(pDX, IDC_EDIT_RED, m_nRed);
	//}}AFX_DATA_MAP

	for ( int n = 0; n < 18; n++ )
		DDX_Control(pDX, m_nButtonID[n], m_buttonColour[n]);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CColourEditDlg, CDialog)
	//{{AFX_MSG_MAP(CColourEditDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_RED, OnKillFocusColour)
	ON_EN_KILLFOCUS(IDC_EDIT_GREEN, OnKillFocusColour)
	ON_EN_KILLFOCUS(IDC_EDIT_BLUE, OnKillFocusColour)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	ON_MESSAGE ( WM_APP, OnColourButtonMessage)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CColourEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	SetWindowText( m_strTitle );

	SubclassEdit ( IDC_EDIT_RED, SS_NUM, 3, "%d" );
	SubclassEdit ( IDC_EDIT_GREEN, SS_NUM, 3, "%d" );
	SubclassEdit ( IDC_EDIT_BLUE, SS_NUM, 3, "%d" );

	m_sliderRed.SetRange( 0, 255 );
	m_sliderGreen.SetRange( 0, 255 );
	m_sliderBlue.SetRange( 0, 255 );
	
	m_sliderRed.SetPos( m_nRed );
	m_sliderGreen.SetPos( m_nGreen );
	m_sliderBlue.SetPos( m_nBlue );

	return TRUE; 
}

/**********************************************************************/

long CColourEditDlg::OnColourButtonMessage( WPARAM wIndex, LPARAM lParam )
{
	UpdateData( TRUE, FALSE );

	if ( wIndex >= 1 && wIndex <= 17 && lParam == 0 )
	{
		int nColourRef = m_buttonColour[ wIndex ].m_ColourRefBack;
		
		m_buttonColour[0].m_ColourRefBack = nColourRef;
		m_buttonColour[0].Invalidate();

		m_nBlue = ( nColourRef & 0xFF0000 ) / 0x10000;
		m_nGreen = ( nColourRef & 0xFF00 ) / 0x100;
		m_nRed = ( nColourRef & 0xFF );
	}
	
	UpdateData( FALSE );

	m_sliderRed.SetPos( m_nRed );
	m_sliderGreen.SetPos( m_nGreen );
	m_sliderBlue.SetPos( m_nBlue );

	SendColourMessage();

	return 0l;
}

/**********************************************************************/

void CColourEditDlg::OnKillFocusColour() 
{
	UpdateData( TRUE, FALSE );
	
	m_nRed &= 0xFF;
	m_nGreen &= 0xFF;
	m_nBlue &= 0xFF;

	m_buttonColour[0].m_ColourRefBack = ( m_nBlue * 0x10000 ) + ( m_nGreen * 0x100 ) + m_nRed;
	m_buttonColour[0].Invalidate();

	UpdateData( FALSE );

	m_sliderRed.SetPos( m_nRed );
	m_sliderGreen.SetPos( m_nGreen );
	m_sliderBlue.SetPos( m_nBlue );

	SendColourMessage();
}

/**********************************************************************/

void CColourEditDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSliderCtrl* pSlider = (CSliderCtrl*) pScrollBar;

	if( pSlider == &m_sliderRed )	
	{
		int nNewPos = pSlider -> GetPos();
		UpdateSliderRed( nNewPos );
	}
	else if( pSlider == &m_sliderGreen )
	{
		int nNewPos = pSlider -> GetPos();
		UpdateSliderGreen( nNewPos );
	}
	else if( pSlider == &m_sliderBlue )	
	{
		int nNewPos = pSlider -> GetPos();
		UpdateSliderBlue( nNewPos );
	}

	SendColourMessage();
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

/**********************************************************************/

void CColourEditDlg::UpdateSliderRed( int nPos )
{
	UpdateData( TRUE, FALSE );
	
	m_nRed = nPos & 0xFF;
	m_nGreen &= 0xFF;
	m_nBlue &= 0xFF;

	m_buttonColour[0].m_ColourRefBack = ( m_nBlue * 0x10000 ) + ( m_nGreen * 0x100 ) + m_nRed;
	m_buttonColour[0].Invalidate();

	UpdateData( FALSE );
}

/**********************************************************************/

void CColourEditDlg::UpdateSliderGreen( int nPos )
{
	UpdateData( TRUE, FALSE );
	
	m_nRed &= 0xFF;
	m_nGreen = nPos & 0xFF;
	m_nBlue &= 0xFF;

	m_buttonColour[0].m_ColourRefBack = ( m_nBlue * 0x10000 ) + ( m_nGreen * 0x100 ) + m_nRed;
	m_buttonColour[0].Invalidate();

	UpdateData( FALSE );
}

/**********************************************************************/

void CColourEditDlg::UpdateSliderBlue( int nPos )
{
	UpdateData( TRUE, FALSE );
	
	m_nRed &= 0xFF;
	m_nGreen &= 0xFF;
	m_nBlue = nPos & 0xFF;

	m_buttonColour[0].m_ColourRefBack = ( m_nBlue * 0x10000 ) + ( m_nGreen * 0x100 ) + m_nRed;
	m_buttonColour[0].Invalidate();

	UpdateData( FALSE );
}

/**********************************************************************/

int CColourEditDlg::GetColourRef()
{
	return ( m_nBlue * 0x10000 ) + ( m_nGreen * 0x100 ) + m_nRed;
}

/**********************************************************************/

void CColourEditDlg::SendColourMessage()
{
	if ( m_pMainWnd != NULL )
		m_pMainWnd -> SendMessage( WM_COLOUR_CHANGE, 0, GetColourRef() );
}

/**********************************************************************/
