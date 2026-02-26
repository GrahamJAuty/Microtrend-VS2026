/**********************************************************************/
#include "SPOSKeyboardOptionsDlg.h"
/**********************************************************************/

CSPOSKeyboardOptionsDlg::CSPOSKeyboardOptionsDlg(CWnd* pParent)
	: CSSDialog(CSPOSKeyboardOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSPOSKeyboardOptionsDlg)
	//}}AFX_DATA_INIT
	m_bShowPlu = SPOSKeyboardManager.GetLayoutOptions() -> GetShowPluNoFlag();
	m_bBold = SPOSKeyboardManager.GetLayoutOptions() -> GetDefaultBoldFlag();
	m_bEditKeyTopLeft = SPOSKeyboardManager.GetLayoutOptions() -> GetEditKeyTopLeftFlag();
	m_bImageWithStyle = SPOSKeyboardManager.GetLayoutOptions() -> GetImageWithStyleFlag();
	m_nXMarginDisplay = SPOSKeyboardManager.GetLayoutOptions() -> GetXMarginDisplay();	
}

/**********************************************************************/

void CSPOSKeyboardOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardOptionsDlg)
	DDX_Check(pDX, IDC_CHECK_SHOWPLU, m_bShowPlu);
	DDX_Check(pDX, IDC_CHECK_BOLD, m_bBold);
	DDX_Check(pDX, IDC_CHECK_EDITKEY, m_bEditKeyTopLeft);
	DDX_Check(pDX, IDC_CHECK_IMAGEWITHSTYLE, m_bImageWithStyle);
	DDX_Control(pDX, IDC_COMBO_VIEW, m_comboView);
	DDX_Control(pDX, IDC_COMBO_ZOOM, m_comboZoom);
	DDX_Control(pDX, IDC_COMBO_SCREEN, m_comboScreen);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_XMARGIN_DISPLAY, m_nXMarginDisplay);
	DDV_MinMaxInt(pDX, m_nXMarginDisplay, 0, 400 );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CSPOSKeyboardOptionsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardOptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	
	SubclassEdit ( IDC_EDIT_XMARGIN_DISPLAY, SS_NUM, 3, "%d" );
	
	m_comboView.AddString( "Standard List Area" );
	m_comboView.AddString( "Large List Area" );
	m_comboView.AddString( "Standard Function Area" );
	m_comboView.AddString( "Large Function Area" );
	m_comboView.AddString( "Bottom Function Bar" );
	m_comboView.AddString( "Screens" );

	int nSel = SPOSKeyboardManager.GetLayoutOptions() -> GetInitialViewMode();
	if ( ( nSel < 0 ) || ( nSel > 5 ) )
		nSel = 0;

	m_comboView.SetCurSel( nSel );

	m_comboZoom.AddString( "Shrink (75%)" );
	m_comboZoom.AddString( "Normal Size" );
	m_comboZoom.AddString( "Zoom (200%)" );

	int nZoom = SPOSKeyboardManager.GetLayoutOptions() -> GetInitialZoomType();
	if ( ( nZoom < 1 ) || ( nZoom > 3 ) )
		nZoom = 2;

	m_comboZoom.SetCurSel( nZoom - 1 );

	CArray<int,int> arrayTabStops;
	arrayTabStops.Add(2);
	arrayTabStops.Add(20);
	arrayTabStops.Add(50);
	arrayTabStops.Add(62);
	arrayTabStops.Add(200);
	m_comboScreen.SetTabStops( arrayTabStops );

	m_comboScreen.AddItem( "0,640,x,480" );
	m_comboScreen.AddItem( "1,800,x,600" );
	m_comboScreen.AddItem( "2,1024,x,768" );
	m_comboScreen.AddItem( "3,1366,x,768" );
	m_comboScreen.AddItem( "4,1920,x,1080" );
	m_comboScreen.AddItem( "5,1280,x,800" );
	m_comboScreen.AddItem( "6,1920,x,1200" );
	m_comboScreen.AddItem( "7,1024,x,600" );

	int nScreen = SPOSKeyboardManager.GetLayoutOptions() -> GetSharpPosResolutionType();
	if ( ( nScreen < 0 ) || ( nScreen > 7 ) )
		nScreen = 2;

	m_comboScreen.SetCurSel( nScreen );

	return TRUE;  
}

/**********************************************************************/

void CSPOSKeyboardOptionsDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SPOSKeyboardManager.GetLayoutOptions() -> SetShowPluNoFlag( m_bShowPlu != 0 );
		SPOSKeyboardManager.GetLayoutOptions() -> SetDefaultBoldFlag( m_bBold != 0 );
		SPOSKeyboardManager.GetLayoutOptions() -> SetEditKeyTopLeftFlag( m_bEditKeyTopLeft != 0 );
		SPOSKeyboardManager.GetLayoutOptions() -> SetImageWithStyleFlag( m_bImageWithStyle != 0 );
		SPOSKeyboardManager.GetLayoutOptions() -> SetXMarginDisplay ( m_nXMarginDisplay );
		SPOSKeyboardManager.GetLayoutOptions() -> SetSharpPosResolutionType( m_comboScreen.GetCurSel() );
		SPOSKeyboardManager.GetLayoutOptions() -> SetInitialViewMode( m_comboView.GetCurSel() );
		SPOSKeyboardManager.GetLayoutOptions() -> SetInitialZoomType( m_comboZoom.GetCurSel() + 1 );
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
