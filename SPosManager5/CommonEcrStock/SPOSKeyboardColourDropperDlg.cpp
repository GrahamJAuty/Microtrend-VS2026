/**********************************************************************/
#include "ColourEditDlg.h"
#include "MaxLengths.h"
#include "Password.h"
#include "SPOSKeyboardKeyHelpers.h"
#include "SPOSKeyboardKeyRecord.h"
#include "SPOSKeyboardModelessPointers.h"
#include "SPOSKeyboardPropertySheetEditDefaultText.h"
/**********************************************************************/
#include "SPOSKeyboardColourDropperDlg.h"
/**********************************************************************/

CSPOSKeyboardColourDropperDlg::CSPOSKeyboardColourDropperDlg( int nMode, CWnd* pParent /*=NULL*/)
	: CSSDialog(CSPOSKeyboardColourDropperDlg::IDD, pParent),
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName ),
	m_CategorySelector ( m_editCatNo, m_comboCatName )
{
	//{{AFX_DATA_INIT(CSPOSKeyboardColourDropperDlg)
	//}}AFX_DATA_INIT
	m_nDialogMinPixelSizeX = 0;
	m_nDialogMinPixelSizeY = 0;
	m_nDialogMaxPixelSizeX = 0;
	m_nDialogMaxPixelSizeY = 0;
	m_nListPosPixelX = 0;
	m_nListPosPixelY = 0;
	m_nListPixelSizeX = 0;
	m_nListPixelBorderY = 0;
	m_hIcon = NULL;

	switch ( nMode )
	{
	case COLOURDROPPER_LISTKEY:
	case COLOURDROPPER_COPYKEY:
	case COLOURDROPPER_COPYSTYLE:
	case COLOURDROPPER_GROUPKEY:
		m_nMode = nMode;
		break;

	default:
		m_nMode = COLOURDROPPER_COPYSTYLE;
		break;
	}

	m_nKeyType = KEY_TYPE_EMPTY;
	m_nKeyValue = 0;
	m_nForeground = 1;
	m_nBackground = 4;
	m_nFontSize = 8;
	m_nHAlign = 1;
	m_nVAlign = 1;
	m_nBold = 0;
	m_strCopyText = "";
	m_strImageFilename = "";
	m_nImageScale = 0;
	m_nActiveTab = 0;
	m_nDefTextAlways = 0;
	m_bDeptAndCategory = SPOSKeyboardManager.GetPluDatabase() -> GotDeptAndCategory();

	//m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );
	m_bInitDialog = FALSE;
	
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

void CSPOSKeyboardColourDropperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardColourDropperDlg)
	DDX_Control(pDX, IDC_TAB_SETTINGS, m_TabSettings );

	for ( int n = 0; n < 65; n++ )
		DDX_Control(pDX, m_nButtonID[n], m_buttonColour[n]);

	DDX_Control(pDX, IDC_STATIC_COLOUR, m_staticColour);
	DDX_Control(pDX, IDC_COMBO_COLOUR, m_comboColour);
	DDX_Control(pDX, IDC_STATIC_TEXTSIZE, m_staticFontSize);
	DDX_Control(pDX, IDC_COMBO_TEXTSIZE, m_comboFontSize);
	DDX_Control(pDX, IDC_STATIC_VERTICAL, m_staticVertical);
	DDX_Control(pDX, IDC_COMBO_VERTICAL, m_comboVertical);
	DDX_Control(pDX, IDC_STATIC_HORIZONTAL, m_staticHorizontal);
	DDX_Control(pDX, IDC_COMBO_HORIZONTAL, m_comboHorizontal);
	DDX_Control(pDX, IDC_STATIC_BOLD, m_staticBold);
	DDX_Control(pDX, IDC_COMBO_BOLD, m_comboBold);
	DDX_Control(pDX, IDC_STATIC_DEFTEXT, m_staticDefText);
	DDX_Control(pDX, IDC_COMBO_DEFTEXT, m_comboDefText);
	DDX_Control(pDX, IDC_STATIC_DEPARTMENT, m_staticDepartment);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPARTMENT, m_comboDeptName);
	DDX_Control(pDX, IDC_STATIC_CATEGORY, m_staticCategory);
	DDX_Control(pDX, IDC_EDIT_CATNO, m_editCatNo);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCatName);
	DDX_Control(pDX, IDC_CHECK_COPYIMAGE, m_checkCopyImage);
	DDX_Control(pDX, IDC_EDIT_IMAGE, m_editImage);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardColourDropperDlg, CDialog)
	//{{AFX_MSG_MAP(CSPOSKeyboardColourDropperDlg)
	ON_WM_SIZE()
	ON_WM_MEASUREITEM()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SETTINGS, OnTcnSelchangeTab)
	ON_CBN_SELCHANGE(IDC_COMBO_TEXTSIZE, OnSelectFontSize)
	ON_COMMAND(IDM_KEYTYPE_CONTROL, OnKeyTypeControl)
	ON_COMMAND(IDM_KEYTYPE_EMPTY, OnKeyTypeEmpty)
	ON_COMMAND(IDM_KEYTYPE_FUNCTION, OnKeyTypeFunction)
	ON_COMMAND(IDM_KEYTYPE_LIST, OnKeyTypeList)
	ON_COMMAND(IDM_KEYTYPE_MACRO, OnKeyTypeMacro)
	ON_COMMAND(IDM_KEYTYPE_MODIFIER, OnKeyTypeModifier)
	ON_COMMAND(IDM_KEYTYPE_PAYMENT, OnKeyTypePayment)
	ON_COMMAND(IDM_KEYTYPE_PLUNO, OnKeyTypePluNo)
	ON_COMMAND(IDM_KEYTYPE_SVRPRESET, OnKeyTypeSvrPreset)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnSelectValue)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_COMMAND(IDM_FILTER, OnFilter)
	ON_CBN_SELCHANGE(IDC_COMBO_COLOUR, OnSelectColourMode)
	ON_CBN_SELCHANGE(IDC_COMBO_HORIZONTAL, OnSelectHorizontal)
	ON_CBN_SELCHANGE(IDC_COMBO_VERTICAL, OnSelectVertical)
	ON_CBN_SELCHANGE(IDC_COMBO_BOLD, OnSelectBold)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPARTMENT, OnSelectDepartment)
	ON_EN_KILLFOCUS(IDC_EDIT_CATNO, OnKillFocusCatNo)
	ON_CBN_SELCHANGE(IDC_COMBO_CATEGORY, OnSelectCategory)
	ON_CBN_SELCHANGE(IDC_COMBO_DEFTEXT, OnSelectDefTextAlways)
	//}}AFX_MSG_MAP
	ON_WM_GETMINMAXINFO()
	ON_MESSAGE ( WM_APP, OnColourButtonClick)
	ON_MESSAGE ( WM_APP + 1, OnColourButtonDoubleClick)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardColourDropperDlg::OnInitDialog() 
{
	m_staticImage.SubclassDlgItem(IDC_STATIC_IMAGE,this);
	CDialog::OnInitDialog();
	SetIcon ( m_hIcon, TRUE );

	m_listKeyInfo.SubclassDlgItem ( IDC_LIST, this );
	m_listKeyInfo.InsertColumn ( 0, "", LVCFMT_LEFT, 230 );
	m_listKeyInfo.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	m_TabSettings.InsertItem( 0, "Colour1" );
	m_TabSettings.InsertItem( 1, "Colour2" );
	m_TabSettings.InsertItem( 2, "Text" );
	m_TabSettings.InsertItem( 3, "Align" );
	m_TabSettings.InsertItem( 4, "Image" );

	if ( ( TRUE == m_bDeptAndCategory ) && ( COLOURDROPPER_LISTKEY == m_nMode ) )
		m_TabSettings.InsertItem( 5, "Filter" );
	
	for ( int n = 1; n <= 64; n++ )
		m_buttonColour[n].m_ColourRefBack = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB(n);

	{
		int nButtonIdx = 1;
		for ( int nTab = 0; nTab <= 1; nTab++ )
		{
			int nYPos = 21;
			for ( int nRow = 0; nRow < 4; nRow++ )
			{
				int nXPos = 7;
				for ( int nCol = 0; nCol < 8; nCol++ )
				{
					MoveControl( &m_buttonColour[nButtonIdx++], nXPos, nYPos, 19, 17 );
					nXPos += 20;
				}
				nYPos += 18;
			}
		}
	}
	
	m_buttonColour[0].SetWindowText( "ABCD" );

	FillFontCombo();
	FillColourCombo();
	FillHorizontalCombo();
	FillVerticalCombo();
	FillBoldCombo();
	FillDefTextAlwaysCombo();

	m_comboColour.SetCurSel(2);
	SetForeground(1);
	SetBackground(4);
	SetFontSize(8);
	SetHAlign(1);
	SetVAlign(1);
	SetBold(0);
	SetDefTextAlways(0);

	m_MenuChanger.EatMenuTopLevel( GetMenu() );

	if ( SPOSKeyboardManager.GetLayoutOptions() -> GetMaxBasePluLen() == 0 )
		m_MenuChanger.KillItem( IDM_FILTER );

	switch ( m_nMode )
	{
	case COLOURDROPPER_LISTKEY:		InitDialogListKey();	break;
	case COLOURDROPPER_GROUPKEY:		
	case COLOURDROPPER_COPYKEY:		InitDialogCopyKey();	break;
	case COLOURDROPPER_COPYSTYLE:	InitDialogCopyColour();	break;
	}

	UpdateTitle();

	if ( ( TRUE == m_bDeptAndCategory ) && ( COLOURDROPPER_LISTKEY == m_nMode ) )
	{
		m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
		m_DepartmentSelector.FillDepartmentComboDatabase( SPOSKeyboardManager.GetModelessPointers() -> GetDepartmentFilter(), DEPARTMENT_SELECTOR_NORMAL );

		m_editCatNo.LimitText ( MAX_LENGTH_CATNO );
		m_CategorySelector.FillCategoryCombo ( SPOSKeyboardManager.GetModelessPointers() -> GetCategoryFilter() );
	}

	//TEXT TAB
	MoveControl( &m_staticFontSize, 7, 28 );
	MoveControl( &m_comboFontSize, 32, 27 );
	MoveControl( &m_staticBold, 7, 48 );
	MoveControl( &m_comboBold, 32, 47 );
	MoveControl( &m_staticDefText, 7, 68 );
	MoveControl( &m_comboDefText, 32, 67 );
	
	//ALIGN TAB
	MoveControl( &m_staticVertical, 7, 28 );
	MoveControl( &m_comboVertical, 32, 27 );
	MoveControl( &m_staticHorizontal, 7, 48 );
	MoveControl( &m_comboHorizontal, 32, 47 );

	//IMAGE TAB
	MoveControl( &m_checkCopyImage, 7, 28 );

	if ( m_nMode == COLOURDROPPER_LISTKEY )
	{
		MoveControl( &m_staticImage, 7, 42, 178, 89 );
		MoveControl( &m_editImage, 67, 28, 118, 11 );
	}
	else
	{
		MoveControl( &m_staticImage, 7, 42, 158, 89 );
		MoveControl( &m_editImage, 67, 28, 98, 11 );
	}

	//FILTER TAB
	MoveControl( &m_staticDepartment, 7, 28 );
	MoveControl( &m_editDeptNo, 7, 40 );
	MoveControl( &m_comboDeptName, 30, 40 );
	MoveControl( &m_staticCategory, 7, 60 );
	MoveControl( &m_editCatNo, 7, 72 );
	MoveControl( &m_comboCatName, 30, 72 );
	
	m_nActiveTab = 0;
	SelectTab();

	return TRUE;  
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listKeyInfo.AdjustRowHeight( lpMeasureItemStruct, 5 );
   }
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::InitDialogListKey()
{
	m_MenuChanger.BuildMenu ( GetMenu() );
	DrawMenuBar();

	{
		//POSITION OF LIST IN PIXELS
		CRect rectListPos( 0, 0, 7, 140 );
		MapDialogRect( rectListPos );
		m_nListPosPixelX = rectListPos.right;
		m_nListPosPixelY = rectListPos.bottom;

		//WIDTH OF LIST AND LOWER BORDER IN PIXELS
		CRect rectListSize( 0, 0, 179, 7 );
		MapDialogRect( rectListSize );
		m_nListPixelSizeX = rectListSize.Width();
		m_nListPixelBorderY = rectListSize.Height();

		int w, h;
		GetDialogBorderPixels( w, h );

		//INCLUDE 7 DIALOG UNITS OF PADDING FROM BOTTOM RIGHT OF LIST
		CRect rectBottomRight( 0, 0, 193, 159 );
		MapDialogRect( rectBottomRight );

		m_nDialogMinPixelSizeX = rectBottomRight.Width() + w;
		m_nDialogMaxPixelSizeX = m_nDialogMinPixelSizeX;

		m_nDialogMinPixelSizeY = rectBottomRight.Height() + h;
		m_nDialogMaxPixelSizeY = GetSystemMetrics( SM_CYSCREEN ) - 100;
	}

	m_bInitDialog = TRUE;
	SetWindowPos ( NULL, 0, 0, m_nDialogMinPixelSizeX, SPOSKeyboardManager.GetLayoutOptions() -> GetKeyListHeight(), SWP_NOMOVE );
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::InitDialogCopyKey()
{
	m_MenuChanger.KillItem( IDM_FILTER );
	m_MenuChanger.KillParent( IDM_KEYTYPE_EMPTY );
	m_MenuChanger.BuildMenu ( GetMenu() );
	DrawMenuBar();

	{
		int w, h;
		GetDialogBorderPixels( w, h );
		
		//INCLUDE 7 DIALOG UNITS OF PADDING FROM BOTTOM RIGHT OF LIST
		CRect rectBottomRight( 0, 0, 173, 166 );
		MapDialogRect( rectBottomRight );

		m_nDialogMinPixelSizeX = rectBottomRight.Width() + w;
		m_nDialogMaxPixelSizeX = m_nDialogMinPixelSizeX;

		m_nDialogMinPixelSizeY = rectBottomRight.Height() + h;
		m_nDialogMaxPixelSizeY = m_nDialogMinPixelSizeY;
	}

	m_bInitDialog = TRUE;
	SetWindowPos ( NULL, 0, 0, m_nDialogMinPixelSizeX, m_nDialogMinPixelSizeY, SWP_NOMOVE );
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::InitDialogCopyColour()
{
	m_MenuChanger.KillItem( IDM_FILTER );
	m_MenuChanger.KillParent( IDM_KEYTYPE_EMPTY );
	m_MenuChanger.BuildMenu ( GetMenu() );
	DrawMenuBar();

	{
		int w, h;
		GetDialogBorderPixels( w, h );
		
		//INCLUDE 7 DIALOG UNITS OF PADDING FROM BOTTOM RIGHT OF COLOUR COMBO
		CRect rectBottomRight( 0, 0, 173, 138 );
		MapDialogRect( rectBottomRight );

		m_nDialogMinPixelSizeX = rectBottomRight.Width() + w;
		m_nDialogMaxPixelSizeX = m_nDialogMinPixelSizeX;

		m_nDialogMinPixelSizeY = rectBottomRight.Height() + h;
		m_nDialogMaxPixelSizeY = m_nDialogMinPixelSizeY;
	}

	m_listKeyInfo.ShowWindow ( SW_HIDE );
		
	m_bInitDialog = TRUE;
	SetWindowPos ( NULL, 0, 0, m_nDialogMinPixelSizeX, m_nDialogMinPixelSizeY, SWP_NOMOVE );
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);

		if ( m_nMode == COLOURDROPPER_LISTKEY )
		{
			m_listKeyInfo.MoveWindow ( m_nListPosPixelX, m_nListPosPixelY, m_nListPixelSizeX, cy - m_nListPosPixelY - m_nListPixelBorderY, TRUE );
			
			int w, h;
			GetDialogBorderPixels( w, h );
			SPOSKeyboardManager.GetLayoutOptions() -> SetKeyListHeight( cy + h );
			SPOSKeyboardManager.GetLayoutOptions() -> SetKeyListWidth( cx + w );
		}
		
		Invalidate();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnGetMinMaxInfo( MINMAXINFO FAR* lpMMI )
{
	CDialog::OnGetMinMaxInfo(lpMMI);

	int w, h;
	if ( GetDialogBorderPixels ( w, h ) == FALSE )
	{
		lpMMI->ptMinTrackSize.x = 100;
		lpMMI->ptMaxTrackSize.x = 500;
		lpMMI->ptMinTrackSize.y = 100;
		lpMMI->ptMaxTrackSize.y = 500;
		return;
	}

	lpMMI->ptMinTrackSize.x = m_nDialogMinPixelSizeX;
	lpMMI->ptMaxTrackSize.x = m_nDialogMaxPixelSizeX;
	lpMMI->ptMinTrackSize.y = m_nDialogMinPixelSizeY;
	lpMMI->ptMaxTrackSize.y = m_nDialogMaxPixelSizeY;
}

/**********************************************************************/

bool CSPOSKeyboardColourDropperDlg::GetDialogBorderPixels( int& w, int& h )
{
	CRect rectClient;
	GetClientRect( &rectClient );

	CRect rectWindow;
	GetWindowRect( &rectWindow );

	if ( rectClient.Width() < 10 || rectClient.Height() < 10 )
		return FALSE;

	w = rectWindow.Width() - rectClient.Width();
	h = rectWindow.Height() - rectClient.Height();
	return TRUE;
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::SelectImageTab()
{
	m_TabSettings.SetCurSel(4);
	m_nActiveTab = 4;
	SelectTab();
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	m_nActiveTab = m_TabSettings.GetCurSel();
	if ( ( m_nActiveTab < 0 ) || ( m_nActiveTab > 5 ) )
		m_nActiveTab = 0;

	SelectTab();
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::SelectTab()
{
	bool bIsColourTab = ( ( 0 == m_nActiveTab ) || ( 1 == m_nActiveTab ) );

	for ( int n = 1; n <= 32; n++ )
		ShowAndEnableWindow( &m_buttonColour[n], 0 == m_nActiveTab );

	for ( int n = 33; n <= 64; n++ )
		ShowAndEnableWindow( &m_buttonColour[n], 1 == m_nActiveTab );

	ShowAndEnableWindow( &m_staticColour, bIsColourTab );
	ShowAndEnableWindow( &m_comboColour, bIsColourTab );

	ShowAndEnableWindow( &m_buttonColour[0], m_nActiveTab < 4 );
	
	ShowAndEnableWindow( &m_staticFontSize, 2 == m_nActiveTab );
	ShowAndEnableWindow( &m_comboFontSize, 2 == m_nActiveTab );
	ShowAndEnableWindow( &m_staticBold, 2 == m_nActiveTab );
	ShowAndEnableWindow( &m_comboBold, 2 == m_nActiveTab );
	ShowAndEnableWindow( &m_staticDefText, 2 == m_nActiveTab );
	ShowAndEnableWindow( &m_comboDefText, 2 == m_nActiveTab );
	
	ShowAndEnableWindow( &m_staticVertical, 3 == m_nActiveTab );
	ShowAndEnableWindow( &m_comboVertical, 3 == m_nActiveTab );
	ShowAndEnableWindow( &m_staticHorizontal, 3 == m_nActiveTab );
	ShowAndEnableWindow( &m_comboHorizontal, 3 == m_nActiveTab );
	
	ShowAndEnableWindow( &m_checkCopyImage, 4 == m_nActiveTab );
	ShowAndEnableWindow( &m_editImage, 4 == m_nActiveTab );
	ShowAndEnableWindow( &m_staticImage, 4 == m_nActiveTab );

	ShowAndEnableWindow( &m_staticDepartment, 5 == m_nActiveTab );
	ShowAndEnableWindow( &m_editDeptNo, 5 == m_nActiveTab );
	ShowAndEnableWindow( &m_comboDeptName, 5 == m_nActiveTab );
	ShowAndEnableWindow( &m_staticCategory, 5 == m_nActiveTab );
	ShowAndEnableWindow( &m_editCatNo, 5 == m_nActiveTab );
	ShowAndEnableWindow( &m_comboCatName, 5 == m_nActiveTab );
}

/**********************************************************************/

int CSPOSKeyboardColourDropperDlg::GetSafeListSize()
{
	int nCount1 = m_listKeyInfo.GetItemCount();
	int nCount2 = m_arrayKeyValues.GetSize();

	if ( nCount1 < nCount2 )
		return nCount1;
	else
		return nCount2;
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::FillFontCombo()
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

void CSPOSKeyboardColourDropperDlg::FillColourCombo()
{
	m_comboColour.ResetContent();
	m_comboColour.AddString( "No Change" );
	m_comboColour.AddString( "Key Default" );
	m_comboColour.AddString( "As Shown" );
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::FillHorizontalCombo()
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

void CSPOSKeyboardColourDropperDlg::FillVerticalCombo()
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

void CSPOSKeyboardColourDropperDlg::FillBoldCombo()
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

void CSPOSKeyboardColourDropperDlg::FillDefTextAlwaysCombo()
{
	m_comboDefText.ResetContent();
	m_comboDefText.AddString( "----------" );
	m_comboDefText.AddString( "Default text always" );
	m_comboDefText.AddString( "Individual key text" );
	m_comboDefText.SetItemData( 0, 999 );
	m_comboDefText.SetItemData( 1, 1 );
	m_comboDefText.SetItemData( 2, 0 );
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::SetColoursToNoChange()
{
	m_comboColour.SetCurSel(0);
	UpdatePreviewButton();
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::SetColoursToDropAsShown()
{
	m_comboColour.SetCurSel(2);
	UpdatePreviewButton();
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::SetColoursAndTextToDefault()
{
	m_comboColour.SetCurSel(1);
	SetFontSize(998);
	SetHAlign(998);
	SetVAlign(998);
	SetBold(998);
	UpdatePreviewButton();
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::UpdateTitle()
{
	CString strWindowText = "";

	switch ( m_nMode )
	{
	case COLOURDROPPER_LISTKEY:
		switch ( m_nKeyType )
		{
		case KEY_TYPE_CONTROL:		strWindowText = "Control Keys";	break;
		case KEY_TYPE_EMPTY:		strWindowText = "Empty Keys";		break;
		case KEY_TYPE_FUNCTION:		strWindowText = "Function Keys";	break;
		case KEY_TYPE_LIST:			strWindowText = "List Keys";		break;
		case KEY_TYPE_MACRO:		strWindowText = "Macro Keys";		break;
		case KEY_TYPE_MODIFIER:		strWindowText = "Modifier Keys";	break;
		case KEY_TYPE_PAYMENT:		strWindowText = "Payment Keys";	break;
		case KEY_TYPE_PLUNUMBER:	strWindowText = "Plu Keys";		break;
		case KEY_TYPE_SVRPRESET:	strWindowText = "Svr Preset Keys";	break;
		}
		break;

	case COLOURDROPPER_COPYKEY:
		switch ( m_nKeyType )
		{
		case KEY_TYPE_CONTROL:		strWindowText = "Copy Control Key";	break;
		case KEY_TYPE_EMPTY:		strWindowText = "Copy Empty Key";		break;
		case KEY_TYPE_FUNCTION:		strWindowText = "Copy Function Key";	break;
		case KEY_TYPE_LIST:			strWindowText = "Copy List Key";		break;
		case KEY_TYPE_MACRO:		strWindowText = "Copy Macro Key";		break;
		case KEY_TYPE_MODIFIER:		strWindowText = "Copy Modifier Key";	break;
		case KEY_TYPE_PAYMENT:		strWindowText = "Copy Payment Key";	break;
		case KEY_TYPE_PLUNUMBER:	strWindowText = "Copy Plu Key";		break;
		case KEY_TYPE_SVRPRESET:	strWindowText = "Copy Svr Preset Key";	break;
		}
		break;

	case COLOURDROPPER_GROUPKEY:
		switch ( m_nKeyType )
		{
		case KEY_TYPE_CONTROL:		strWindowText = "Copy and Group Control Key";	break;
		case KEY_TYPE_EMPTY:		strWindowText = "Copy and Group Empty Key";		break;
		case KEY_TYPE_FUNCTION:		strWindowText = "Copy and Group Function Key";	break;
		case KEY_TYPE_LIST:			strWindowText = "Copy and Group List Key";		break;
		case KEY_TYPE_MACRO:		strWindowText = "Copy and Group Macro Key";		break;
		case KEY_TYPE_MODIFIER:		strWindowText = "Copy and Group Modifier Key";	break;
		case KEY_TYPE_PAYMENT:		strWindowText = "Copy and Group Payment Key";	break;
		case KEY_TYPE_PLUNUMBER:	strWindowText = "Copy and Group Plu Key";		break;
		case KEY_TYPE_SVRPRESET:	strWindowText = "Copy and Group Svr Preset Key";	break;
		}
		break;

	case COLOURDROPPER_COPYSTYLE:
		strWindowText = "Copy Key Style";
		break;
	}

	SetWindowText ( strWindowText );
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::SetCopyImageTitle()
{
	SetWindowText( "Copy Key Image" );
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::SetForeground( int n )
{
	if ( n >= 1 && n <= 64 )
	{
		m_nForeground = n;
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::SetBackground( int n )
{
	if ( n >= 1 && n <= 64 )
	{
		m_nBackground = n;
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::SetFontSize( int n )
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

void CSPOSKeyboardColourDropperDlg::SetHAlign( int n )
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

void CSPOSKeyboardColourDropperDlg::SetVAlign( int n )
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

void CSPOSKeyboardColourDropperDlg::SetImageFlag( bool b )
{
	m_checkCopyImage.SetCheck( b );
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::SetBold( int n )
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

void CSPOSKeyboardColourDropperDlg::SetDefTextAlways( int n )
{	
	for ( int x = 0; x < m_comboDefText.GetCount(); x++ )
	{
		if ( m_comboDefText.GetItemData(x) == (WORD) n )
		{
			m_nDefTextAlways = n;
			m_comboDefText.SetCurSel(x);
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnSelectFontSize() 
{
	int nSel = m_comboFontSize.GetCurSel();

	if ( nSel >= 0 && nSel < m_comboFontSize.GetCount() )
	{
		m_nFontSize = m_comboFontSize.GetItemData( nSel );
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnSelectHorizontal() 
{
	int nSel = m_comboHorizontal.GetCurSel();

	if ( nSel >= 0 && nSel < m_comboHorizontal.GetCount() )
	{
		m_nHAlign = m_comboHorizontal.GetItemData( nSel );
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnSelectVertical() 
{
	int nSel = m_comboVertical.GetCurSel();

	if ( nSel >= 0 && nSel < m_comboVertical.GetCount() )
	{
		m_nVAlign = m_comboVertical.GetItemData( nSel );
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnSelectBold() 
{
	int nSel = m_comboBold.GetCurSel();

	if ( nSel >= 0 && nSel < m_comboBold.GetCount() )
	{
		m_nBold = m_comboBold.GetItemData( nSel );
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnSelectDefTextAlways() 
{
	int nSel = m_comboDefText.GetCurSel();

	if ( nSel >= 0 && nSel < m_comboDefText.GetCount() )
		m_nDefTextAlways = m_comboDefText.GetItemData( nSel );
}

/**********************************************************************/

long CSPOSKeyboardColourDropperDlg::OnColourButtonClick( WPARAM wIndex, LPARAM lParam )
{
	if ( wIndex >= 1 && wIndex <= 64 )
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

long CSPOSKeyboardColourDropperDlg::OnColourButtonDoubleClick( WPARAM wIndex, LPARAM lParam )
{
	if ( (wIndex >= 33) && (wIndex <= 64) )
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

void CSPOSKeyboardColourDropperDlg::EditColour( int nColourId )
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

void CSPOSKeyboardColourDropperDlg::OnSelectColourMode() 
{
	UpdatePreviewButton();
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::UpdatePreviewButton()
{
	CSPOSKeyboardDefaultTextRecord TextRecord;

	switch( m_nMode )
	{
	case COLOURDROPPER_COPYKEY:
	case COLOURDROPPER_GROUPKEY:
	case COLOURDROPPER_LISTKEY:
		{
			CSPOSKeyboardKeyRecord KeyRecord;
			KeyRecord.SetType( m_nKeyType );
			KeyRecord.SetValue( m_nKeyValue );

			CSPOSKeyboardKeyHelpers KeyHelpers;
			KeyHelpers.GetDefaultTextRecord( KeyRecord, TextRecord );
		}
		break;
	}

	COLORREF refFore, refBack;
	
	switch( m_comboColour.GetCurSel() )
	{
	case 0:
		//NO CHANGE
		refFore = COLORREF ( 0x0 );
		refBack = COLORREF ( 0xCCCCCC );
		break;

	case 1:
		//KEY DEFAULTS
		m_nForeground = TextRecord.GetForeground();			
		m_nBackground = TextRecord.GetBackground();
		refFore = m_buttonColour[m_nForeground].m_ColourRefBack;
		refBack = m_buttonColour[m_nBackground].m_ColourRefBack;
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
		case 999:	
			nFontSize = 8;
			break;

		case 998:	
			nFontSize = TextRecord.GetFontSize();	
			break;

		default:	
			nFontSize = nData;
			break;
		}
	}

	int nHAlign = 1;
	nSel = m_comboHorizontal.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboHorizontal.GetCount() )
	{
		int nData = m_comboHorizontal.GetItemData( nSel );

		switch( nData )
		{
		case 999:
			nHAlign = 1; 
			break;

		case 998: 
			nHAlign = TextRecord.GetHAlign();
			break;

		default:	
			nHAlign = nData;
			break;
		}
	}

	int nVAlign = 1;
	nSel = m_comboVertical.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboVertical.GetCount() )
	{
		int nData = m_comboVertical.GetItemData( nSel );

		switch( nData )
		{
		case 999:	
			nVAlign = 1;
			break;

		case 998: 
			nVAlign = TextRecord.GetVAlign();
			break;

		default:	
			nVAlign = nData;
			break;
		}
	}

	bool bBold = FALSE;
	nSel = m_comboBold.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboBold.GetCount() )
	{
		int nData = m_comboBold.GetItemData( nSel );

		switch( nData )
		{
		case 999:	
			bBold = FALSE; 
			break;

		case 998:
			bBold = TextRecord.GetBoldFlag();
			break;

		default:	
			bBold = ( nData == 1); 
			break;
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

void CSPOSKeyboardColourDropperDlg::OnKillFocusDeptNo()
{
	if ((TRUE == m_bDeptAndCategory) && (COLOURDROPPER_LISTKEY == m_nMode))
	{
		m_DepartmentSelector.SelectDepartmentFromEditBox();
		SPOSKeyboardManager.GetModelessPointers()->SetDepartmentFilter(m_DepartmentSelector.GetDepartmentFilter());

		if (KEY_TYPE_PLUNUMBER == m_nKeyType)
		{
			CreateList();
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnSelectDepartment()
{
	if ((TRUE == m_bDeptAndCategory) && (COLOURDROPPER_LISTKEY == m_nMode))
	{
		m_DepartmentSelector.SelectDepartmentFromCombo();
		SPOSKeyboardManager.GetModelessPointers()->SetDepartmentFilter(m_DepartmentSelector.GetDepartmentFilter());

		if (KEY_TYPE_PLUNUMBER == m_nKeyType)
		{
			CreateList();
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnKillFocusCatNo()
{
	if ((TRUE == m_bDeptAndCategory) && (COLOURDROPPER_LISTKEY == m_nMode))
	{
		m_CategorySelector.SelectCategoryFromEditBox();
		SPOSKeyboardManager.GetModelessPointers()->SetCategoryFilter(m_CategorySelector.GetCategoryFilter());

		if (KEY_TYPE_PLUNUMBER == m_nKeyType)
		{
			CreateList();
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnSelectCategory()
{
	if ((TRUE == m_bDeptAndCategory) && (COLOURDROPPER_LISTKEY == m_nMode))
	{
		m_CategorySelector.SelectCategoryFromCombo();
		SPOSKeyboardManager.GetModelessPointers()->SetCategoryFilter(m_CategorySelector.GetCategoryFilter());

		if (KEY_TYPE_PLUNUMBER == m_nKeyType)
		{
			CreateList();
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::SetKeyType ( int n )
{
	m_nKeyType = n;

	if ( m_nMode == COLOURDROPPER_LISTKEY )
	{
		switch ( m_nKeyType )
		{
		case KEY_TYPE_EMPTY:		OnKeyTypeEmpty();		break;
		case KEY_TYPE_FUNCTION:		OnKeyTypeFunction();	break;
		case KEY_TYPE_PAYMENT:		OnKeyTypePayment();		break;
		case KEY_TYPE_MODIFIER:		OnKeyTypeModifier();	break;
		case KEY_TYPE_CONTROL:		OnKeyTypeControl();		break;
		case KEY_TYPE_LIST:			OnKeyTypeList();		break;
		case KEY_TYPE_SVRPRESET:	OnKeyTypeSvrPreset();	break;
		case KEY_TYPE_MACRO:		OnKeyTypeMacro();		break;
		case KEY_TYPE_PLUNUMBER:	OnKeyTypePluNo();		break;
		default:					OnKeyTypePluNo();		break;
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::SetKeyValue( __int64 n )
{
	if ( m_nMode == COLOURDROPPER_LISTKEY )
	{
		int nSize = GetSafeListSize();

		for ( int nPos = 0; nPos < nSize; nPos++ )
		{
			if ( m_arrayKeyValues.GetAt(nPos) == n )
			{
				m_listKeyInfo.SetCurSel( nPos );
				m_nKeyValue = n;
				break;
			}
		}
	
		UpdatePreviewButton();
		UpdateOptionalMenus();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::UpdateOptionalMenus()
{
	if (SPOSKeyboardManager.GetLayoutOptions()->GetMaxBasePluLen() != 0)
	{
		bool bShowFilter = (m_nKeyType == KEY_TYPE_PLUNUMBER);

		if (bShowFilter == TRUE)
		{
			if (SPOSKeyboardManager.GetModelessPointers()->GetFilterFlag())
			{
				m_MenuChanger.SetItemText(IDM_FILTER, "&Show Modifiers");
			}
			else
			{
				m_MenuChanger.SetItemText(IDM_FILTER, "&Hide Modifiers");
			}

			m_MenuChanger.ShowItem(IDM_FILTER, TRUE);
		}

		m_MenuChanger.ShowItem(IDM_FILTER, bShowFilter);

		m_MenuChanger.BuildMenu(GetMenu());
		DrawMenuBar();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnFilter() 
{
	if ( SPOSKeyboardManager.GetLayoutOptions() -> GetMaxBasePluLen() != 0 )
	{
		SPOSKeyboardManager.GetModelessPointers() -> SetFilterFlag ( !SPOSKeyboardManager.GetModelessPointers() -> GetFilterFlag() );	
		UpdateOptionalMenus();
		CreateList();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnKeyTypeControl()
{ 
	SelectKeyTypeFromMenu ( KEY_TYPE_CONTROL );
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnKeyTypeEmpty()
{ 
	SelectKeyTypeFromMenu ( KEY_TYPE_EMPTY );
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnKeyTypeFunction()
{
	SelectKeyTypeFromMenu ( KEY_TYPE_FUNCTION );
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnKeyTypeList()	
{ 
	SelectKeyTypeFromMenu ( KEY_TYPE_LIST ); 
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnKeyTypeMacro()
{ 
	SelectKeyTypeFromMenu ( KEY_TYPE_MACRO ); 
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnKeyTypeModifier()
{ 
	SelectKeyTypeFromMenu ( KEY_TYPE_MODIFIER );
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnKeyTypePayment()
{ 
	SelectKeyTypeFromMenu ( KEY_TYPE_PAYMENT );
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnKeyTypePluNo()	
{ 
	SelectKeyTypeFromMenu ( KEY_TYPE_PLUNUMBER ); 
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnKeyTypeSvrPreset()	
{ 
	SelectKeyTypeFromMenu ( KEY_TYPE_SVRPRESET ); 
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::SelectKeyTypeFromMenu( int nType )
{
	m_nKeyType = nType;
	UpdateTitle();
	CreateList();
	UpdateOptionalMenus();

	m_nKeyValue = 0;
	if ( GetSafeListSize() > 0 )
	{
		m_listKeyInfo.SetCurSel(0);
		m_nKeyValue = m_arrayKeyValues.GetAt(0);
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::CreateList()
{	
	m_listKeyInfo.DeleteAllItems();
	m_arrayKeyValues.RemoveAll();

	CSPOSKeyboardDefaultTextArray* pFunctionTable = NULL;

	switch ( m_nKeyType )
	{
	case KEY_TYPE_FUNCTION:		
		pFunctionTable = SPOSKeyboardManager.GetFunctionTable();
		break;

	case KEY_TYPE_PAYMENT:	
		pFunctionTable = SPOSKeyboardManager.GetPaymentTable();
		break;

	case KEY_TYPE_CONTROL:	
		pFunctionTable = SPOSKeyboardManager.GetControlTable();	
		break;

	case KEY_TYPE_MODIFIER:	
		pFunctionTable = SPOSKeyboardManager.GetModifierTable();
		break;

	case KEY_TYPE_SVRPRESET:
		pFunctionTable = SPOSKeyboardManager.GetSvrPresetTable();
		break;

	case KEY_TYPE_LIST:				
	case KEY_TYPE_MACRO:
	case KEY_TYPE_PLUNUMBER:	
		pFunctionTable = NULL;	
		break;

	case KEY_TYPE_EMPTY:	
	default:					
		pFunctionTable = SPOSKeyboardManager.GetEmptyTable();	
		break;
	}

	if ( pFunctionTable != NULL )
	{
		for ( int nIndex = 0; nIndex < pFunctionTable -> GetSize(); nIndex++ )
		{
			CSPOSKeyboardDefaultTextRecord TextRecord;
			pFunctionTable -> GetTextRecord( nIndex, TextRecord );
			
			CSPOSKeyboardKeyHelpers KeyHelpers;
			CString strCombiText = KeyHelpers.GetCombiText( m_nKeyType, TextRecord.GetValue() );
			
			int nPos = m_listKeyInfo.AddString ( strCombiText );
			m_arrayKeyValues.InsertAt ( nPos, TextRecord.GetValue() );
		}
	}
	else if (m_nKeyType == KEY_TYPE_PLUNUMBER)
	{
		for (int nIndex = 0; nIndex < SPOSKeyboardManager.GetPluDatabase()->GetSize(); nIndex++)
		{
			CSPOSKeyboardImportPluCSVRecord ImportPlu;
			SPOSKeyboardManager.GetPluDatabase()->GetAt(nIndex, ImportPlu);
			__int64 nPluNo = ImportPlu.GetPluNumber();

			CString strPluNo = "";
			strPluNo.Format("%I64d", nPluNo);
			int nPluLen = strPluNo.GetLength();

			int nMinDirectPluLen = 1;
			if (SPOSKeyboardManager.GetLayoutOptions()->GetMaxBasePluLen() != 0)
			{
				nMinDirectPluLen = SPOSKeyboardManager.GetLayoutOptions()->GetMaxBasePluLen() + 2;
			}

			{
				int nDeptFilter = SPOSKeyboardManager.GetModelessPointers()->GetDepartmentFilter();
				int nCatFilter = SPOSKeyboardManager.GetModelessPointers()->GetCategoryFilter();
				bool bPluFilter = SPOSKeyboardManager.GetModelessPointers()->GetFilterFlag();

				if ((nDeptFilter == DEPARTMENTSET_ALL) || (nDeptFilter == ImportPlu.GetDeptNo()))
				{
					if ((nCatFilter == CATEGORYSET_ALL) || (nCatFilter == ImportPlu.GetCategoryNo()))
					{
						if ((FALSE == bPluFilter) || ((nPluNo % 10) == 0) || (nPluLen >= nMinDirectPluLen))
						{
							ImportPlu.AddToList(m_listKeyInfo);
							m_arrayKeyValues.Add(nPluNo);
						}
					}
				}
			}
		}
	}
	else if ( m_nKeyType == KEY_TYPE_LIST )
	{
		for ( int nIndex = 0; nIndex < SPOSKeyboardManager.GetListManager() -> GetListCount(); nIndex++ )
		{
			CSPOSKeyboardListRecord ListRecord;
			SPOSKeyboardManager.GetListManager() -> GetListRecord( nIndex, ListRecord );
			__int64 nListNo = ListRecord.GetListNo();

			CSPOSKeyboardKeyHelpers KeyHelpers;
			CString strCombiText = KeyHelpers.GetCombiText( m_nKeyType, nListNo );
			
			int nPos = m_listKeyInfo.AddString( strCombiText );
			m_arrayKeyValues.InsertAt( nPos, nListNo );
		}
	}
	else if ( m_nKeyType == KEY_TYPE_MACRO )
	{
		for ( int nIndex = 0; nIndex < DataManagerNonDb.CCMacroActive.GetSize(); nIndex++ )
		{
			CCascadingMacroCSVRecord Macro;
			DataManagerNonDb.CCMacroActive.GetAt( nIndex, Macro );
			__int64 nMacroNo = Macro.GetMacroNo();

			CSPOSKeyboardKeyHelpers KeyHelpers;
			CString strCombiText = KeyHelpers.GetCombiText( m_nKeyType, nMacroNo );
			
			int nPos = m_listKeyInfo.AddString( strCombiText );
			m_arrayKeyValues.InsertAt( nPos, nMacroNo );
		}
	}
	
	int nCount = GetSafeListSize();
	m_listKeyInfo.SetCurSel( ( nCount >= 1 ) ? 0 : -1 );
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnSelectValue(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	if ( m_nMode == COLOURDROPPER_LISTKEY )
	{
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
		if ( ( pNMListView -> uNewState & LVIS_SELECTED ) != 0 )
		{
			int nSel = m_listKeyInfo.GetCurSel();
			if ( ( nSel >= 0 ) && ( nSel < GetSafeListSize() ) )
			{
				CString str;
				str.Format( "Key Dropper Selection %d\n", nSel );
				TRACE( str );

				m_nKeyValue = m_arrayKeyValues.GetAt( nSel );
				UpdatePreviewButton();
			}
		}
	}
	*pResult = 0;
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::SelectNext()
{
	if ( m_nMode == COLOURDROPPER_LISTKEY )
	{
		int nSel = m_listKeyInfo.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < GetSafeListSize() - 1 ) )
		{
			m_listKeyInfo.SetCurSel( nSel + 1 );
			m_nKeyValue = m_arrayKeyValues.GetAt ( nSel + 1 );
			UpdatePreviewButton();
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::SetKeyToCopy ( CSPOSKeyboardKeyRecord& Key )
{
	m_nKeyType = Key.GetType();
	m_nKeyValue = Key.GetValue();

	CSPOSKeyboardKeyHelpers KeyHelpers;
	CString strCombiText = KeyHelpers.GetCombiText( m_nKeyType, m_nKeyValue );

	m_listKeyInfo.DeleteAllItems();
	m_arrayKeyValues.RemoveAll();

	m_listKeyInfo.AddString ( strCombiText );
	m_arrayKeyValues.InsertAt( 0, m_nKeyValue );

	m_listKeyInfo.SetCurSel( -1 );
	m_listKeyInfo.EnableWindow ( TRUE );

	UpdateTitle();

	SetForeground ( Key.GetForeground() );
	SetBackground( Key.GetBackground() );
	SetFontSize ( Key.GetFontSize() );
	SetHAlign( Key.GetAlignmentH() );
	SetVAlign( Key.GetAlignmentV() );
	SetBold( Key.GetBoldFlag() );
	SetImageFlag( TRUE );
	SetImageFilename( Key.GetImageFilename() );
	SetImageScale( Key.GetImageScale() );
	SetDefTextAlways( Key.GetDefaultTextAlwaysFlag() );
	
	m_strCopyText = Key.GetKeyText();

	UpdatePreviewButton();
	UpdateImage();
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::UpdateKeyRecord( CSPOSKeyboardKeyRecord& KeyRecord )
{	
	//CHANGE KEY TYPE AND TEXT
	switch( m_nMode )
	{
	case COLOURDROPPER_COPYSTYLE:
		break;

	case COLOURDROPPER_COPYKEY:
	case COLOURDROPPER_GROUPKEY:
		KeyRecord.SetType( m_nKeyType );
		KeyRecord.SetValue( m_nKeyValue );
		KeyRecord.SetKeyText( m_strCopyText );
		break;

	case COLOURDROPPER_LISTKEY:
		{
			KeyRecord.SetType( m_nKeyType );
			KeyRecord.SetValue( m_nKeyValue );

			CSPOSKeyboardKeyHelpers KeyHelpers;
			KeyHelpers.GetDefaultText( m_nKeyType, m_nKeyValue, m_strCopyText, FALSE ); 
			KeyRecord.SetKeyText( m_strCopyText );

			SelectNext();
		}
		break;
	}

	CSPOSKeyboardKeyHelpers KeyHelpers;
	CSPOSKeyboardDefaultTextRecord TextRecord;
	KeyHelpers.GetDefaultTextRecord( KeyRecord, TextRecord );

	//CHANGE TEXT SIZE
	switch( m_nFontSize )
	{
	case 999:		//NO CHANGE
		break;

	case 998:		//KEY DEFAULT
		KeyRecord.SetFontSize( TextRecord.GetFontSize() );		
		break;

	default:		//SPECIFIED
		KeyRecord.SetFontSize( m_nFontSize );
		break;
	}

	//CHANGE H ALIGN
	switch( m_nHAlign )
	{
	case 999:		//NO CHANGE
		break;

	case 998:		//KEY DEFAULT
		KeyRecord.SetAlignmentH( TextRecord.GetHAlign() );		
		break;

	default:		//SPECIFIED
		KeyRecord.SetAlignmentH( m_nHAlign );
		break;
	}

	//CHANGE V ALIGN
	switch( m_nVAlign )
	{
	case 999:		//NO CHANGE
		break;

	case 998:		//KEY DEFAULT
		KeyRecord.SetAlignmentV( TextRecord.GetVAlign() );		
		break;

	default:		//SPECIFIED
		KeyRecord.SetAlignmentV( m_nVAlign );
		break;
	}

	//CHANGE BOLD
	switch( m_nBold )
	{
	case 999:		//NO CHANGE
		break;

	case 998:		//KEY DEFAULT
		KeyRecord.SetBoldFlag( TextRecord.GetBoldFlag() );		
		break;

	default:		//SPECIFIED
		KeyRecord.SetBoldFlag( m_nBold == 1 );
		break;
	}

	//CHANGE DEF TEXT
	switch( m_nDefTextAlways )
	{
	case 999:		//NO CHANGE
		break;

	default:		//SPECIFIED
		KeyRecord.SetDefaultTextAlwaysFlag( m_nDefTextAlways == 1 );
		break;
	}

	//CHANGE IMAGE SETTINGS
	switch( m_nMode )
	{
	case COLOURDROPPER_LISTKEY:
		KeyRecord.SetImageFilename( TextRecord.GetImageFilename() );
		KeyRecord.SetImageScale( TextRecord.GetImageScale() );
		break;
	}

	//COPY IMAGE TICK OVERRIDES KEY DEFAULTS
	if ( m_checkCopyImage.GetCheck() != 0 )
	{
		KeyRecord.SetImageFilename( m_strImageFilename );
		KeyRecord.SetImageScale( m_nImageScale );
	}

	switch( m_comboColour.GetCurSel() )
	{
	case 0:	
		break;
	
	case 1:	
		KeyRecord.SetForeground( TextRecord.GetForeground() );	
		KeyRecord.SetBackground( TextRecord.GetBackground() );	
		break;
		
	case 2:
		KeyRecord.SetForeground( m_nForeground );
		KeyRecord.SetBackground( m_nBackground );
		break;
	}
}

/**********************************************************************/

bool CSPOSKeyboardColourDropperDlg::CanDropKeyNow()
{
	switch( m_nMode )
	{
	case COLOURDROPPER_COPYKEY:
	case COLOURDROPPER_GROUPKEY:
	case COLOURDROPPER_COPYSTYLE:
		return TRUE;

	case COLOURDROPPER_LISTKEY:
		{
			int nSel = m_listKeyInfo.GetCurSel();

			if ( ( nSel < 0 ) || ( nSel >= GetSafeListSize() ) )
				return FALSE;

			m_nKeyValue = m_arrayKeyValues.GetAt( nSel );
			return TRUE;
		}

	default:
		return FALSE;
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HandleDoubleClickList();
	*pResult = 0;
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::HandleDoubleClickList()
{
	switch ( m_nMode )
	{
	case COLOURDROPPER_LISTKEY:
	case COLOURDROPPER_GROUPKEY:
	case COLOURDROPPER_COPYKEY:	break;
	default:					return;
	}

	int nSel = m_listKeyInfo.GetCurSel();
	if ( ( nSel < 0 ) || ( nSel >= GetSafeListSize() ) )
		return;

	CSPOSKeyboardDefaultTextArray* pTextArray = NULL;

	switch ( m_nKeyType )
	{
	case KEY_TYPE_FUNCTION:		pTextArray = SPOSKeyboardManager.GetFunctionTable();	break;
	case KEY_TYPE_PAYMENT:		pTextArray = SPOSKeyboardManager.GetPaymentTable();		break;
	case KEY_TYPE_CONTROL:		pTextArray = SPOSKeyboardManager.GetControlTable();		break;
	case KEY_TYPE_MODIFIER:		pTextArray = SPOSKeyboardManager.GetModifierTable();	break;
	case KEY_TYPE_SVRPRESET:	pTextArray = SPOSKeyboardManager.GetSvrPresetTable();	break;
	case KEY_TYPE_LIST:		
	case KEY_TYPE_MACRO:
	case KEY_TYPE_PLUNUMBER:	pTextArray = NULL;										break;
	case KEY_TYPE_EMPTY:	
	default:					pTextArray = SPOSKeyboardManager.GetEmptyTable();		break;
	}
	
	if ( pTextArray != NULL )
	{
		EditDefaultKeySettings( pTextArray, nSel );	
		UpdatePreviewButton();
	}
	else if ( m_nKeyType == KEY_TYPE_LIST )
	{
		pTextArray = SPOSKeyboardManager.GetListTable();
		__int64 nListNo = m_arrayKeyValues.GetAt( nSel );
		
		int nIdx;
		if ( pTextArray -> FindTextByValue( nListNo, nIdx ) == FALSE )
		{
			CString strListNo = "";
			strListNo.Format( "List|%I64d", nListNo );
			pTextArray -> AddText ( nListNo, strListNo );
		}

		EditDefaultKeySettings( pTextArray, nSel );	
		UpdatePreviewButton();
	}
	else if ( m_nKeyType == KEY_TYPE_MACRO )
	{		
		pTextArray = SPOSKeyboardManager.GetMacroTable();
		__int64 nMacroNo = m_arrayKeyValues.GetAt( nSel );
		
		int nIdx;
		if ( pTextArray -> FindTextByValue( nMacroNo, nIdx ) == FALSE )
		{
			CString strMacroNo = "";
			strMacroNo.Format( "Macro|%I64d", nMacroNo );
			pTextArray -> AddText ( nMacroNo, strMacroNo );
		}

		EditDefaultKeySettings( pTextArray, nSel );	
		UpdatePreviewButton();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::EditDefaultKeySettings( CSPOSKeyboardDefaultTextArray* pTextArray, int nSel )
{
	__int64 nValue = m_arrayKeyValues.GetAt ( nSel );
	
	int nIndex;
	if ( pTextArray -> FindTextByValue ( nValue, nIndex ) )
	{
		CSPOSKeyboardDefaultTextRecord TextRecord;
		pTextArray -> GetTextRecord( nIndex, TextRecord );

		int nMacroIdx = 0;
		CCascadingMacroCSVRecord Macro;
		CDataManagerInfo infoDM;

		bool bWantMacroTab = ( m_nKeyType == KEY_TYPE_MACRO ) && ( PasswordArray.GetEnable( PasswordTicks::SetupMenuMacros ) == TRUE ); 
			
		if ( TRUE == bWantMacroTab )
		{
			if ( DataManagerNonDb.OpenCCMacroKeyboard( DB_READWRITE, infoDM ) == TRUE )
			{
				SPOSKeyboardManager.MergeParentMacros();

				int nMacroNo = (int) nValue;

				if ( DataManagerNonDb.CCMacroKeyboard.FindMacroByNumber( nMacroNo, nMacroIdx ) == FALSE )
				{
					CString strName;
					strName.Format( "Macro %d", nMacroNo );

					Macro.SetMacroNo( nMacroNo );
					Macro.SetParentMacroTypeExternal( NODE_PLACEHOLDER );
					Macro.SetParentMacroName( "" );
					Macro.SetParentMacroData( "" );
					Macro.SetLocalMacroFlag( TRUE );
					Macro.SetLocalMacroData( "" );
					Macro.SetLocalMacroName( strName );
					DataManagerNonDb.CCMacroKeyboard.InsertAt( nMacroIdx, Macro );
				}
				else
				{
					DataManagerNonDb.CCMacroKeyboard.GetAt( nMacroIdx, Macro );
				}
			}
			else
			{
				Prompter.ShareError( infoDM );
				return;
			}
		}

		CSPOSKeyboardPropertySheetEditDefaultText propSheet ( m_nKeyType, TextRecord, Macro, bWantMacroTab, this );
		
		if ( propSheet.DoModal() == IDOK )
		{
			propSheet.SaveRecord();
			pTextArray -> SetTextRecord ( nIndex, TextRecord );
			
			if ( TRUE == bWantMacroTab )
			{
				DataManagerNonDb.CCMacroKeyboard.SetAt( nMacroIdx, Macro );
			
				if ( DataManagerNonDb.WriteCCMacroKeyboard( infoDM ) == FALSE )
				{
					Prompter.WriteError( infoDM );
				}
				else
				{
					SPOSKeyboardManager.CreateActiveMacroList();
				}
			}
	
			CSPOSKeyboardKeyHelpers KeyHelpers;
			CString strCombiText = KeyHelpers.GetCombiText( m_nKeyType, TextRecord.GetValue() );
			
			m_listKeyInfo.DeleteString( nSel );
			m_listKeyInfo.InsertString( nSel, strCombiText );
			m_listKeyInfo.SetCurSel( nSel );
			
			m_arrayKeyValues.SetAt( nSel, nValue );
			
			SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
		}
	
		if ( TRUE == bWantMacroTab )
		{
			DataManagerNonDb.CloseCCMacroKeyboard( infoDM );
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::UpdateImage() 
{
	m_editImage.SetWindowText( m_strImageFilename );

	bool bFailed = FALSE;
	bool bGotFile = FALSE;

	if ( m_strImageFilename != "" )
	{
		CString strFilename = SPOSKeyboardManager.GetLayoutOptions() -> GetImageFolder();
		strFilename += "\\";
		strFilename += m_strImageFilename;

		if ( ::FileExists( strFilename )== TRUE )
		{
			bGotFile = ( m_staticImage.LoadImage( strFilename ) == TRUE );
			bFailed = ( FALSE == bGotFile );
		}
		else
		{
			bFailed = TRUE;
		}
	}
		
	if ( TRUE == bGotFile )
	{
		m_staticImage.SetBlankFlag( FALSE );
		m_staticImage.SetFailedFlag( FALSE );
		m_staticImage.Invalidate();
	}
	else
	{
		m_staticImage.SetBlankFlag( TRUE );
		m_staticImage.SetFailedFlag( bFailed );
		m_staticImage.Invalidate();
	}
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnOK() 
{
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::OnCancel() 
{
	DestroyWindow();
}

/**********************************************************************/

void CSPOSKeyboardColourDropperDlg::PostNcDestroy() 
{
	SPOSKeyboardManager.GetLayoutOptions() -> SetLastFgndId ( m_nForeground );
	SPOSKeyboardManager.GetLayoutOptions() -> SetLastBgndId ( m_nBackground );
	SPOSKeyboardManager.GetLayoutOptions() -> SetLastFontSize ( m_nFontSize );
	
	if ( m_nMode == COLOURDROPPER_LISTKEY )
	{
		SPOSKeyboardManager.GetLayoutOptions() -> SetLastKeyType( m_nKeyType );
		SPOSKeyboardManager.GetLayoutOptions() -> SetLastKeyValue( m_nKeyValue );
	}
	
	SPOSKeyboardManager.GetLayoutOptions() -> Write();
	
	SPOSKeyboardManager.GetModelessPointers() -> NullifyColourDropper();	
	delete this;
}

/**********************************************************************/

