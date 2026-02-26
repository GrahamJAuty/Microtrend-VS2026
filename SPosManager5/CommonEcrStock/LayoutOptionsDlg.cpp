/**********************************************************************/
#include "BackgroundLayoutInfo.h"
#include "ButtonManager.h"
#include "ButtonOptions.h"
#include "ColourEditDlg.h"
#include "FileRemover.h"
#include "..\SPosEcrManager5\MainFrmEcrman.h"
#include "..\SPosStockManager5\MainFrmStockman.h"
#include "WMNumbers.h"
/**********************************************************************/
#include "LayoutOptionsDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CLayoutOptionsDlg, CDialog)
/**********************************************************************/

CLayoutOptionsDlg::CLayoutOptionsDlg(CWnd* pParent)
	: CSSDialog(CLayoutOptionsDlg::IDD, pParent)
{
	m_bShrunk = FALSE;
	m_nTabPage = 0;
	m_pMainWnd = pParent;

	m_strImagePath = "";
	GetDataProgramPath( m_strImagePath );

	m_nColourMode = 0;
}

/**********************************************************************/

CLayoutOptionsDlg::~CLayoutOptionsDlg()
{
}

/**********************************************************************/

void CLayoutOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SHRINK, m_buttonShrink );
	DDX_Control(pDX, IDC_TAB_SETTINGS, m_TabSettings );
	DDX_Control(pDX, IDC_COLOURBUTTON_STATUS, m_buttonColourStatus );
	/*****/
	DDX_Control(pDX, IDC_CHECK_ENABLE, m_checkEnable );
	DDX_Control(pDX, IDC_STATIC_WIDTH, m_staticWidth );
	DDX_Control(pDX, IDC_CHECK_WIDTH, m_checkWidth );
	DDX_Control(pDX, IDC_CHECK_HEIGHT, m_checkHeight );
	DDX_Control(pDX, IDC_STATIC_HEIGHT, m_staticHeight );
	DDX_Control(pDX, IDC_STATIC_HEIGHT2, m_staticHeight2 );
	DDX_Control(pDX, IDC_STATIC_TASKBAR, m_staticTaskbar );
	DDX_Control(pDX, IDC_STATIC_FILLCOLOUR, m_staticColourFill );
	DDX_Control(pDX, IDC_COLOURBUTTON_FILL, m_buttonColourFill );
	DDX_Control(pDX, IDC_STATIC_IMAGEACTION, m_staticImageAction );
	DDX_Control(pDX, IDC_BUTTON_IMAGE, m_buttonImage );
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_buttonClear );
	DDX_Control(pDX, IDC_STATIC_IMAGESIZE, m_staticImageSize );
	DDX_Control(pDX, IDC_COMBO_IMAGESIZE, m_comboImageSize );
	DDX_Control(pDX, IDC_STATIC_XALIGN, m_staticXAlign );
	DDX_Control(pDX, IDC_COMBO_XALIGN, m_comboXAlign );
	DDX_Control(pDX, IDC_STATIC_MARGIN_LEFT, m_staticMarginLeft );
	DDX_Control(pDX, IDC_STATIC_MARGIN_RIGHT, m_staticMarginRight );
	DDX_Control(pDX, IDC_STATIC_YALIGN, m_staticYAlign );
	DDX_Control(pDX, IDC_COMBO_YALIGN, m_comboYAlign );
	DDX_Control(pDX, IDC_STATIC_MARGIN_TOP, m_staticMarginTop );
	DDX_Control(pDX, IDC_STATIC_MARGIN_BOTTOM, m_staticMarginBottom );
	/*****/
	DDX_Control(pDX, IDC_STATIC_BUTTONBAR_HOME, m_staticButtonBarHome);
	DDX_Control(pDX, IDC_STATIC_BUTTONBAR_DIRECTION, m_staticButtonBarDirection);
	DDX_Control(pDX, IDC_STATIC_BUTTONBAR_ALIGN, m_staticButtonBarAlign);
	DDX_Control(pDX, IDC_STATIC_BUTTONBAR_CONTROL_LINE, m_staticButtonBarControlLine);
	DDX_Control(pDX, IDC_STATIC_BUTTONBAR_CONTROL_KEY, m_staticButtonBarControlKey);
	DDX_Control(pDX, IDC_STATIC_BUTTONBAR_SIZE, m_staticButtonBarSize);
	DDX_Control(pDX, IDC_STATIC_BUTTONBAR_LINES, m_staticButtonBarLines);
	DDX_Control(pDX, IDC_COMBO_BUTTONBAR_HOME, m_comboButtonBarHome);
	DDX_Control(pDX, IDC_COMBO_BUTTONBAR_DIRECTION, m_comboButtonBarDirection);
	DDX_Control(pDX, IDC_COMBO_BUTTONBAR_ALIGN, m_comboButtonBarAlign);
	DDX_Control(pDX, IDC_COMBO_BUTTONBAR_CONTROL_LINE, m_comboButtonBarControlLine);
	DDX_Control(pDX, IDC_COMBO_BUTTONBAR_CONTROL_KEY, m_comboButtonBarControlKey);
	DDX_Control(pDX, IDC_CHECK_BUTTONBAR_PARENT, m_checkButtonBarParent);
	DDX_Control(pDX, IDC_STATIC_BUTTONBAR_PARENT, m_staticButtonBarParent);
	DDX_Control(pDX, IDC_COMBO_BUTTONBAR_SIZE, m_comboButtonBarSize);
	DDX_Control(pDX, IDC_COMBO_BUTTONBAR_LINES, m_comboButtonBarLines);
	/*****/
	DDX_Control(pDX, IDC_STATIC_BUTTON_WIDTH, m_staticButtonWidth);
	DDX_Control(pDX, IDC_STATIC_GAP_WIDTH, m_staticGapWidth);
	DDX_Control(pDX, IDC_STATIC_BUTTON_HEIGHT, m_staticButtonHeight);
	DDX_Control(pDX, IDC_STATIC_GAP_HEIGHT, m_staticGapHeight);
	DDX_Control(pDX, IDC_COMBO_BUTTON_WIDTH, m_comboButtonWidth);
	DDX_Control(pDX, IDC_COMBO_BUTTON_HEIGHT, m_comboButtonHeight);
	DDX_Control(pDX, IDC_STATIC_BUTTON_ROUND, m_staticButtonRound);
	DDX_Control(pDX, IDC_COMBO_BUTTON_ROUND, m_comboButtonRound);
	/*****/
	DDX_Control(pDX, IDC_STATIC_COLOUR_ACTION, m_staticColourAction);
	DDX_Control(pDX, IDC_COLOURBUTTON_ACTION, m_buttonColourAction);
	DDX_Control(pDX, IDC_BUTTON_ACTION_BACK, m_buttonActionBack);
	DDX_Control(pDX, IDC_BUTTON_ACTION_TEXT, m_buttonActionText);
	DDX_Control(pDX, IDC_STATIC_COLOUR_LIST, m_staticColourList);
	DDX_Control(pDX, IDC_COLOURBUTTON_LIST, m_buttonColourList);
	DDX_Control(pDX, IDC_BUTTON_LIST_BACK, m_buttonListBack);
	DDX_Control(pDX, IDC_BUTTON_LIST_TEXT, m_buttonListText);
	DDX_Control(pDX, IDC_STATIC_COLOUR_PARENT, m_staticColourParent);
	DDX_Control(pDX, IDC_COLOURBUTTON_PARENT, m_buttonColourParent);
	DDX_Control(pDX, IDC_BUTTON_PARENT_BACK, m_buttonParentBack);
	DDX_Control(pDX, IDC_BUTTON_PARENT_TEXT, m_buttonParentText);
	DDX_Control(pDX, IDC_STATIC_COLOUR_CONTROL, m_staticColourControl);
	DDX_Control(pDX, IDC_COLOURBUTTON_CONTROL, m_buttonColourControl);
	DDX_Control(pDX, IDC_BUTTON_CONTROL_BACK, m_buttonControlBack);
	DDX_Control(pDX, IDC_BUTTON_CONTROL_TEXT, m_buttonControlText);
	DDX_Control(pDX, IDC_STATIC_COLOUR_UNUSED, m_staticColourUnused);
	DDX_Control(pDX, IDC_COLOURBUTTON_UNUSED, m_buttonColourUnused);
	DDX_Control(pDX, IDC_BUTTON_UNUSED_BACK, m_buttonUnusedBack);
	/*****/
	DDX_Control(pDX, IDOK, m_buttonOK );
	DDX_Control(pDX, IDCANCEL, m_buttonCancel );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CLayoutOptionsDlg, CDialog)
	ON_BN_CLICKED( IDC_BUTTON_SHRINK, OnButtonShrink)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SETTINGS, OnTcnSelchangeTabs)
	/*****/
	ON_BN_CLICKED( IDC_CHECK_ENABLE, OnToggleEnable)
	ON_BN_CLICKED( IDC_CHECK_WIDTH, OnToggleWidth)
	ON_EN_KILLFOCUS( IDC_EDIT_WIDTH, OnKillFocusWidth)
	ON_BN_CLICKED( IDC_CHECK_HEIGHT, OnToggleUpperHeight)
	ON_EN_KILLFOCUS( IDC_EDIT_HEIGHT, OnKillFocusHeight)
	ON_EN_KILLFOCUS( IDC_EDIT_TASKBAR, OnKillFocusTaskbar)
	ON_BN_CLICKED( IDC_COLOURBUTTON_FILL,OnButtonFillColour)
	ON_BN_CLICKED( IDC_BUTTON_IMAGE,OnButtonImage)
	ON_BN_CLICKED( IDC_BUTTON_CLEAR,OnButtonClear)
	ON_CBN_SELCHANGE( IDC_COMBO_IMAGESIZE, OnSelectImageSize)
	ON_CBN_SELCHANGE( IDC_COMBO_XALIGN, OnSelectXAlign)
	ON_EN_KILLFOCUS( IDC_EDIT_MARGIN_LEFT,OnKillFocusMarginLeft)
	ON_EN_KILLFOCUS( IDC_EDIT_MARGIN_RIGHT,OnKillFocusMarginRight)
	ON_CBN_SELCHANGE( IDC_COMBO_YALIGN, OnSelectYAlign)
	ON_EN_KILLFOCUS( IDC_EDIT_MARGIN_TOP,OnKillFocusMarginTop)
	ON_EN_KILLFOCUS( IDC_EDIT_MARGIN_BOTTOM,OnKillFocusMarginBottom)
	/*****/
	ON_CBN_SELCHANGE( IDC_COMBO_BUTTONBAR_HOME, OnSelectButtonBarHome)
	ON_CBN_SELCHANGE( IDC_COMBO_BUTTONBAR_DIRECTION, OnSelectButtonBarDirection)
	ON_CBN_SELCHANGE( IDC_COMBO_BUTTONBAR_ALIGN, OnSelectButtonBarAlign)
	ON_CBN_SELCHANGE( IDC_COMBO_BUTTONBAR_CONTROL_LINE, OnSelectButtonBarControlLine)
	ON_CBN_SELCHANGE( IDC_COMBO_BUTTONBAR_CONTROL_KEY, OnSelectButtonBarControlKey)
	ON_BN_CLICKED( IDC_CHECK_BUTTONBAR_PARENT, OnToggleButtonBarParent)
	ON_CBN_SELCHANGE( IDC_COMBO_BUTTONBAR_SIZE, OnSelectButtonBarSize)
	ON_CBN_SELCHANGE( IDC_COMBO_BUTTONBAR_LINES, OnSelectButtonBarLines)
	ON_CBN_SELCHANGE( IDC_COMBO_BUTTON_WIDTH, OnSelectButtonWidth)
	ON_EN_KILLFOCUS( IDC_EDIT_BUTTON_WIDTH_PERCENT, OnKillFocusButtonWidthPercent)
	ON_EN_KILLFOCUS( IDC_EDIT_BUTTON_WIDTH_PIXEL, OnKillFocusButtonWidthPixel)
	ON_EN_KILLFOCUS( IDC_EDIT_GAP_WIDTH, OnKillFocusGapWidth)
	ON_CBN_SELCHANGE( IDC_COMBO_BUTTON_HEIGHT, OnSelectButtonHeight)
	ON_EN_KILLFOCUS( IDC_EDIT_BUTTON_HEIGHT_PERCENT, OnKillFocusButtonHeightPercent)
	ON_EN_KILLFOCUS( IDC_EDIT_BUTTON_HEIGHT_PIXEL, OnKillFocusButtonHeightPixel)
	ON_EN_KILLFOCUS( IDC_EDIT_GAP_HEIGHT, OnKillFocusGapHeight)
	ON_CBN_SELCHANGE( IDC_COMBO_BUTTON_ROUND, OnSelectButtonRounding)
	/*****/
	ON_BN_CLICKED( IDC_BUTTON_ACTION_BACK, OnButtonActionBack)
	ON_BN_CLICKED( IDC_BUTTON_ACTION_TEXT, OnButtonActionText)
	ON_BN_CLICKED( IDC_BUTTON_LIST_BACK, OnButtonListBack)
	ON_BN_CLICKED( IDC_BUTTON_LIST_TEXT, OnButtonListText)
	ON_BN_CLICKED( IDC_BUTTON_PARENT_BACK, OnButtonParentBack)
	ON_BN_CLICKED( IDC_BUTTON_PARENT_TEXT, OnButtonParentText)
	ON_BN_CLICKED( IDC_BUTTON_CONTROL_BACK, OnButtonControlBack)
	ON_BN_CLICKED( IDC_BUTTON_CONTROL_TEXT, OnButtonControlText)
	ON_BN_CLICKED( IDC_BUTTON_UNUSED_BACK, OnButtonUnusedBack)
	/*****/
	ON_MESSAGE ( WM_COLOUR_CHANGE, OnColourButtonMessage )
END_MESSAGE_MAP()
/**********************************************************************/

BOOL CLayoutOptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_TabSettings.InsertItem( 0, "Upper Area" );
	m_TabSettings.InsertItem( 1, "Lower Area" );
	m_TabSettings.InsertItem( 2, "Button Bar" );
	m_TabSettings.InsertItem( 3, "Button Size" );
	m_TabSettings.InsertItem( 4, "Button Appearance" );
	
	SubclassEdit( IDC_EDIT_WIDTH, SS_NUM, 4, "%s" );
	SubclassEdit( IDC_EDIT_HEIGHT, SS_NUM, 4, "%s" );
	SubclassEdit( IDC_EDIT_MARGIN_LEFT, SS_NUM, 3, "%s" );
	SubclassEdit( IDC_EDIT_MARGIN_RIGHT, SS_NUM, 3, "%s" );
	SubclassEdit( IDC_EDIT_MARGIN_TOP, SS_NUM, 3, "%s" );
	SubclassEdit( IDC_EDIT_MARGIN_BOTTOM, SS_NUM, 3, "%s" );
	SubclassEdit( IDC_EDIT_BUTTON_WIDTH_PERCENT, SS_NUM, 2, "%s" );
	SubclassEdit( IDC_EDIT_BUTTON_WIDTH_PIXEL, SS_NUM, 3, "%s" );
	SubclassEdit( IDC_EDIT_GAP_WIDTH, SS_NUM, 2, "%s" );
	SubclassEdit( IDC_EDIT_BUTTON_HEIGHT_PERCENT, SS_NUM, 2, "%s" );
	SubclassEdit( IDC_EDIT_BUTTON_HEIGHT_PIXEL, SS_NUM, 3, "%s" );
	SubclassEdit( IDC_EDIT_GAP_HEIGHT, SS_NUM, 2, "%s" );
	SubclassEdit( IDC_EDIT_TASKBAR, SS_NUM, 3, "%s" );

	m_comboImageSize.AddString( "Original Size" );
	m_comboImageSize.AddString( "Fill Image Area" );
	
	m_comboXAlign.AddString( "Left" );
	m_comboXAlign.AddString( "Centre" );
	m_comboXAlign.AddString( "Right" );

	m_comboYAlign.AddString( "Top" );
	m_comboYAlign.AddString( "Centre" );
	m_comboYAlign.AddString( "Bottom" );

	m_comboButtonBarHome.AddString( "Top Left" );
	m_comboButtonBarHome.AddString( "Top Right" );
	m_comboButtonBarHome.AddString( "Bottom Left" );
	m_comboButtonBarHome.AddString( "Bottom Right" );
	
	m_comboButtonBarDirection.AddString( "Left => Right" );
	m_comboButtonBarDirection.AddString( "Right => Left" );
	m_comboButtonBarDirection.AddString( "Top => Bottom" );
	m_comboButtonBarDirection.AddString( "Bottom => Top" );
	
	m_comboButtonBarAlign.AddString( "Margin" );
	m_comboButtonBarAlign.AddString( "Centred" );

	m_comboButtonBarControlLine.AddString( "Before User Buttons" );
	m_comboButtonBarControlLine.AddString( "After User Buttons" );

	m_comboButtonBarControlKey.AddString( "Start of Line" );
	m_comboButtonBarControlKey.AddString( "End of Line" );

	m_comboButtonBarSize.AddString( "3" );
	m_comboButtonBarSize.AddString( "4" );
	m_comboButtonBarSize.AddString( "5" );
	m_comboButtonBarSize.AddString( "6" );
	m_comboButtonBarSize.AddString( "7" );
	m_comboButtonBarSize.AddString( "8" );
	m_comboButtonBarSize.AddString( "9" );
	m_comboButtonBarSize.AddString( "10" );

	m_comboButtonBarLines.AddString( "2" );
	m_comboButtonBarLines.AddString( "3" );
	m_comboButtonBarLines.AddString( "4" );
	m_comboButtonBarLines.AddString( "5" );

	m_comboButtonRound.AddString( "none" );
	m_comboButtonRound.AddString( "10 pixels" );
	m_comboButtonRound.AddString( "20 pixels" );
	m_comboButtonRound.AddString( "30 pixels" );
	m_comboButtonRound.AddString( "40 pixels" );
	m_comboButtonRound.AddString( "50 pixels" );

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(90);
	m_comboButtonWidth.SetTabStops( arrayStops );
	m_comboButtonWidth.AddItem( "Whole Row,(%)" );
	m_comboButtonWidth.AddItem( "Per Button,(pixels)" );

	m_comboButtonHeight.SetTabStops( arrayStops );
	m_comboButtonHeight.AddItem( "Whole Column,(%)" );
	m_comboButtonHeight.AddItem( "Per Button,(pixels)" );

	m_buttonColourFill.m_bBlackBorder = TRUE;

	m_buttonColourAction.m_bBlackBorder = TRUE;
	m_buttonColourAction.m_strText = "Action";

	m_buttonColourList.m_bBlackBorder = TRUE;
	m_buttonColourList.m_strText = "List";

	m_buttonColourParent.m_bBlackBorder = TRUE;
	m_buttonColourParent.m_strText = "Parent";

	m_buttonColourControl.m_bBlackBorder = TRUE;
	m_buttonColourControl.m_strText = "Back/Home";

	m_buttonColourUnused.m_bBlackBorder = TRUE;

	m_buttonColourStatus.m_bBlackBorder = FALSE;
	m_buttonColourStatus.m_ColourRefBack = 0x17269A;
	m_buttonColourStatus.m_ColourRefText = 0xFFFFFF;
	m_buttonColourStatus.m_strText = "Test";

	AddDialogItem( &m_buttonShrink, 7, 7, TRUE );
	AddDialogItem( &m_buttonColourStatus, 65, 6 , TRUE );
	AddDialogItem( &m_TabSettings, 7, 29, TRUE );
	AddDialogItem( &m_checkEnable, 21, 59 );
	AddDialogItem( &m_checkWidth, 21, 90 );
	AddDialogItem( &m_staticWidth, 33, 90 );
	AddDialogItem( GetEditWidth(), 107, 90 );
	AddDialogItem( &m_checkHeight, 21, 110 );
	AddDialogItem( &m_staticHeight2, 33, 110 );
	AddDialogItem( &m_staticHeight, 21, 110 );
	AddDialogItem( GetEditHeight(), 107, 110 );
	AddDialogItem( &m_staticTaskbar, 21, 130 );
	AddDialogItem( GetEditTaskbar(), 107, 130 );
	AddDialogItem( &m_staticColourFill, 259, 90 );
	AddDialogItem( &m_buttonColourFill, 345, 90 );
	AddDialogItem( &m_staticImageAction, 259, 110 );
	AddDialogItem( &m_buttonImage, 345, 110 );
	AddDialogItem( &m_buttonClear, 408, 110 );
	AddDialogItem( &m_staticImageSize, 259, 130 );
	AddDialogItem( &m_comboImageSize, 345, 130 );
	AddDialogItem( &m_staticXAlign, 21, 182 );
	AddDialogItem( &m_comboXAlign, 107, 182 );
	AddDialogItem( &m_staticMarginLeft, 21, 202 );
	AddDialogItem( GetEditMarginLeft(), 107, 202 );
	AddDialogItem( &m_staticMarginRight, 21, 222 );
	AddDialogItem( GetEditMarginRight(), 107, 222 );
	AddDialogItem( &m_staticYAlign, 259, 182 );
	AddDialogItem( &m_comboYAlign, 345, 182 );
	AddDialogItem( &m_staticMarginTop, 259, 202 );
	AddDialogItem( GetEditMarginTop(), 345, 202 );
	AddDialogItem( &m_staticMarginBottom, 259, 222 );
	AddDialogItem( GetEditMarginBottom(), 345, 222 );
	/*****/
	AddDialogItem( &m_staticButtonBarHome, 21, 89 );
	AddDialogItem( &m_comboButtonBarHome, 107, 89 );
	AddDialogItem( &m_staticButtonBarDirection, 21, 108 );
	AddDialogItem( &m_comboButtonBarDirection, 107, 108 );
	AddDialogItem( &m_staticButtonBarAlign, 21, 127 );
	AddDialogItem( &m_comboButtonBarAlign, 107, 127 );
	AddDialogItem( &m_staticButtonBarSize, 21, 146 );
	AddDialogItem( &m_comboButtonBarSize, 107, 146 );
	AddDialogItem( &m_staticButtonBarLines, 21, 165 );
	AddDialogItem( &m_comboButtonBarLines, 107, 165 );
	AddDialogItem( &m_staticButtonBarControlLine, 259, 89 );
	AddDialogItem( &m_comboButtonBarControlLine, 345, 89 );
	AddDialogItem( &m_staticButtonBarControlKey, 259, 108 );
	AddDialogItem( &m_comboButtonBarControlKey, 345, 108 );
	AddDialogItem( &m_checkButtonBarParent, 345, 127 );
	AddDialogItem( &m_staticButtonBarParent, 357, 127 );
	/*****/
	AddDialogItem( &m_staticButtonWidth, 21, 89 );
	AddDialogItem( &m_comboButtonWidth, 107, 89 );
	AddDialogItem( GetEditButtonWidthPercent(), 217, 89 );
	AddDialogItem( GetEditButtonWidthPixel(), 217, 89 );
	AddDialogItem( &m_staticGapWidth, 21, 108 );
	AddDialogItem( GetEditGapWidth(), 107, 108 );
	AddDialogItem( &m_staticButtonHeight, 21, 137 );
	AddDialogItem( &m_comboButtonHeight, 107, 137 );
	AddDialogItem( GetEditButtonHeightPercent(), 217, 137 );
	AddDialogItem( GetEditButtonHeightPixel(), 217, 137 );
	AddDialogItem( &m_staticGapHeight, 21, 156 );
	AddDialogItem( GetEditGapHeight(), 107, 156 );
	AddDialogItem( &m_staticButtonRound, 21, 186 );
	AddDialogItem( &m_comboButtonRound, 107, 186 );
	/*****/
	AddDialogItem( &m_staticColourAction, 21, 108 );
	AddDialogItem( &m_buttonColourAction, 81, 101 );
	AddDialogItem( &m_buttonActionBack, 168, 105 );
	AddDialogItem( &m_buttonActionText, 226, 105 );
	AddDialogItem( &m_staticColourList, 21, 137 );
	AddDialogItem( &m_buttonColourList, 81, 130 );
	AddDialogItem( &m_buttonListBack, 168, 134 );
	AddDialogItem( &m_buttonListText, 226, 134 );
	AddDialogItem( &m_staticColourParent, 21, 166 );
	AddDialogItem( &m_buttonColourParent, 81, 159 );
	AddDialogItem( &m_buttonParentBack, 168, 163 );
	AddDialogItem( &m_buttonParentText, 226, 163 );
	AddDialogItem( &m_staticColourControl, 21, 195 );
	AddDialogItem( &m_buttonColourControl, 81, 188 );
	AddDialogItem( &m_buttonControlBack, 168, 192 );
	AddDialogItem( &m_buttonControlText, 226, 192 );
	AddDialogItem( &m_staticColourUnused, 21, 224 );
	AddDialogItem( &m_buttonColourUnused, 81, 217 );
	AddDialogItem( &m_buttonUnusedBack, 168, 221 );
	/*****/
	AddDialogItem( &m_buttonOK, 0, 0, TRUE );
	AddDialogItem( &m_buttonCancel, 0, 0, TRUE );
	/*****/
	ResizeDialog( 518, 274 );
	MoveControl( &m_buttonOK, 461, 237 );
	MoveControl( &m_buttonCancel, 461, 253 );
	SelectTab(0);

	return TRUE;
}

/**********************************************************************/
CEdit* CLayoutOptionsDlg::GetEditWidth(){ return GetEdit( IDC_EDIT_WIDTH ); }
CEdit* CLayoutOptionsDlg::GetEditHeight(){ return GetEdit( IDC_EDIT_HEIGHT ); }
CEdit* CLayoutOptionsDlg::GetEditTaskbar(){ return GetEdit( IDC_EDIT_TASKBAR ); }
CEdit* CLayoutOptionsDlg::GetEditMarginLeft(){ return GetEdit( IDC_EDIT_MARGIN_LEFT ); }
CEdit* CLayoutOptionsDlg::GetEditMarginRight(){ return GetEdit( IDC_EDIT_MARGIN_RIGHT ); }
CEdit* CLayoutOptionsDlg::GetEditMarginTop(){ return GetEdit( IDC_EDIT_MARGIN_TOP ); }
CEdit* CLayoutOptionsDlg::GetEditMarginBottom(){ return GetEdit( IDC_EDIT_MARGIN_BOTTOM ); }
CEdit* CLayoutOptionsDlg::GetEditButtonWidthPercent(){ return GetEdit( IDC_EDIT_BUTTON_WIDTH_PERCENT ); }
CEdit* CLayoutOptionsDlg::GetEditButtonWidthPixel(){ return GetEdit( IDC_EDIT_BUTTON_WIDTH_PIXEL ); }
CEdit* CLayoutOptionsDlg::GetEditGapWidth(){ return GetEdit( IDC_EDIT_GAP_WIDTH ); }
CEdit* CLayoutOptionsDlg::GetEditButtonHeightPercent(){ return GetEdit( IDC_EDIT_BUTTON_HEIGHT_PERCENT ); }
CEdit* CLayoutOptionsDlg::GetEditButtonHeightPixel(){ return GetEdit( IDC_EDIT_BUTTON_HEIGHT_PIXEL ); }
CEdit* CLayoutOptionsDlg::GetEditGapHeight(){ return GetEdit( IDC_EDIT_GAP_HEIGHT ); }
/**********************************************************************/

void CLayoutOptionsDlg::UpdateParent()
{
#ifndef STOCKMAN_SYSTEM
	( (CMainFrameEcrman*) GetParent() ) -> RefreshLayout(); 
#else
	( (CMainFrameStockman*) GetParent() ) -> RefreshLayout();
#endif
	UpdateErrorBar();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnButtonShrink()
{
	if ( FALSE == m_bShrunk )
	{
		m_bShrunk = TRUE;
		m_buttonShrink.SetWindowText( "==>" );
		
		MoveControl( &m_buttonOK, 121, 7 );
		MoveControl( &m_buttonCancel, 176, 7 );
		ShowAndEnableWindow( &m_TabSettings, FALSE );
		SelectTab( -1 );
		ResizeDialog( 233, 28 );
	}
	else
	{
		m_bShrunk = FALSE;
		m_buttonShrink.SetWindowText( "<==" );
		
		ResizeDialog( 518, 274 );
		MoveControl( &m_buttonOK, 461, 237 );
		MoveControl( &m_buttonCancel, 461, 253 );
		ShowAndEnableWindow( &m_TabSettings, TRUE );
		SelectTab( m_nTabPage );
	}
}

/**********************************************************************/

void CLayoutOptionsDlg::SetEditBoxTextFromInt( CEdit* pEdit, int nNum )
{
	CString strNum;
	strNum.Format( "%d", nNum );
	pEdit -> SetWindowText( strNum );
}

/**********************************************************************/

void CLayoutOptionsDlg::OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	m_nTabPage = m_TabSettings.GetCurSel();
	if ( ( m_nTabPage < 0 ) || ( m_nTabPage >= m_TabSettings.GetItemCount() ) )
		m_nTabPage = 0;

	SelectTab( m_nTabPage );

	if ( m_nTabPage < 5 )
		m_checkEnable.SetFocus();
}

/**********************************************************************/

void CLayoutOptionsDlg::UpdateErrorBar()
{
	if ( ButtonOptions.GetLayoutEnable() && ( BackgroundLayoutInfo.GetErrorCount() > 0 ) )
	{
		m_buttonColourStatus.m_strText = BackgroundLayoutInfo.GetError(0);
		m_buttonColourStatus.m_strText.MakeUpper();
		m_buttonColourStatus.ShowWindow( SW_SHOW );
	}
	else
	{
		m_buttonColourStatus.ShowWindow( SW_HIDE );
	}

	m_buttonColourStatus.Invalidate();
}

/**********************************************************************/

void CLayoutOptionsDlg::SelectTab( int nPage )
{
	bool bEnable = ButtonOptions.GetLayoutEnable();

	switch( nPage )
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		m_checkEnable.SetCheck( bEnable );
		break;
	}

	switch( nPage )
	{
	case 0:
	case 1:
		{
			if ( ButtonOptions.GetWindowWidthAuto() == TRUE )
			{
				m_checkWidth.SetCheck( FALSE );
				ButtonOptions.SetWindowWidthManual( BackgroundLayoutInfo.GetTotalWindowWidth() );
			}
			else
			{
				m_checkWidth.SetCheck( TRUE );
			}

			SetEditBoxTextFromInt( GetEditWidth(), ButtonOptions.GetWindowWidthManual() );
		}
		break;
	}

	switch( nPage )
	{
	case 0:
		{
			if ( ButtonOptions.GetUpperAreaHeightAuto() == TRUE )
			{
				m_checkHeight.SetCheck( FALSE );
				ButtonOptions.SetUpperAreaHeightManual( BackgroundLayoutInfo.GetUpperAreaHeight() );
			}
			else
			{
				m_checkHeight.SetCheck( TRUE );
			}
			SetEditBoxTextFromInt( GetEditHeight(), ButtonOptions.GetUpperAreaHeightManual() );
			
			m_buttonColourFill.m_ColourRefBack = ButtonOptions.GetUpperAreaFillColourInt();
			m_buttonColourFill.Invalidate();

			m_comboImageSize.SetCurSel( ButtonOptions.GetUpperImageStretch() ? 1 : 0 );

			switch( ButtonOptions.GetUpperImageXAlignInt() )
			{
			case 0:		m_comboXAlign.SetCurSel(0);	break;
			case 1:		m_comboXAlign.SetCurSel(1);	break;
			case 2:		m_comboXAlign.SetCurSel(2);	break;
			default:	m_comboXAlign.SetCurSel(1);	break;
			}

			SetEditBoxTextFromInt( GetEditMarginLeft(), ButtonOptions.GetUpperImageMarginLeft() );
			SetEditBoxTextFromInt( GetEditMarginRight(), ButtonOptions.GetUpperImageMarginRight() );

			switch( ButtonOptions.GetUpperImageYAlignInt() )
			{
			case 0:		m_comboYAlign.SetCurSel(0);	break;
			case 1:		m_comboYAlign.SetCurSel(1);	break;
			case 2:		m_comboYAlign.SetCurSel(2);	break;
			default:	m_comboYAlign.SetCurSel(1);	break;
			}

			SetEditBoxTextFromInt( GetEditMarginTop(), ButtonOptions.GetUpperImageMarginTop() );
			SetEditBoxTextFromInt( GetEditMarginBottom(), ButtonOptions.GetUpperImageMarginBottom() );
		}
		break;

	case 1:
		{
			SetEditBoxTextFromInt( GetEditHeight(), ButtonOptions.GetLowerAreaHeightManual() );
			SetEditBoxTextFromInt( GetEditTaskbar(), ButtonOptions.GetLowerAreaTaskbarGap() );

			m_buttonColourFill.m_ColourRefBack = ButtonOptions.GetLowerAreaFillColourInt();
			m_buttonColourFill.Invalidate();

			m_comboImageSize.SetCurSel( ButtonOptions.GetLowerImageStretch() ? 1 : 0 );

			switch( ButtonOptions.GetLowerImageXAlignInt() )
			{
			case 0:		m_comboXAlign.SetCurSel(0);	break;
			case 1:		m_comboXAlign.SetCurSel(1);	break;
			case 2:		m_comboXAlign.SetCurSel(2);	break;
			default:	m_comboXAlign.SetCurSel(1);	break;
			}

			SetEditBoxTextFromInt( GetEditMarginLeft(), ButtonOptions.GetLowerImageMarginLeft() );
			SetEditBoxTextFromInt( GetEditMarginRight(), ButtonOptions.GetLowerImageMarginRight() );

			switch( ButtonOptions.GetLowerImageYAlignInt() )
			{
			case 0:		m_comboYAlign.SetCurSel(0);	break;
			case 1:		m_comboYAlign.SetCurSel(1);	break;
			case 2:		m_comboYAlign.SetCurSel(2);	break;
			default:	m_comboYAlign.SetCurSel(1);	break;
			}

			SetEditBoxTextFromInt( GetEditMarginTop(), ButtonOptions.GetLowerImageMarginTop() );
			SetEditBoxTextFromInt( GetEditMarginBottom(), ButtonOptions.GetLowerImageMarginBottom() );
		}
		break;

	case 2:

		switch( ButtonOptions.GetButtonBarHomeInt() )
		{
		case 0:		m_comboButtonBarHome.SetCurSel(0);	break;
		case 1:		m_comboButtonBarHome.SetCurSel(1);	break;
		case 2:		m_comboButtonBarHome.SetCurSel(3);	break;
		case 3:
		default:	m_comboButtonBarHome.SetCurSel(2);	break;
		}

		switch( ButtonOptions.GetButtonBarDirectionInt() )
		{
		case 0:		m_comboButtonBarDirection.SetCurSel(3);	break;
		case 2:		m_comboButtonBarDirection.SetCurSel(2);	break;
		case 3:		m_comboButtonBarDirection.SetCurSel(1);	break;
		case 1:
		default:	m_comboButtonBarDirection.SetCurSel(0);	break;
		}

		m_comboButtonBarAlign.SetCurSel( ButtonOptions.GetButtonBarCentre() ? 1 : 0 );
		m_comboButtonBarControlLine.SetCurSel( ButtonOptions.GetButtonBarControlLinePosInt() == 1 ? 1 : 0 );
		m_comboButtonBarControlKey.SetCurSel( ButtonOptions.GetButtonBarControlButtonPosInt() == 1 ? 1 : 0 );
		m_checkButtonBarParent.SetCheck( ButtonOptions.GetButtonBarParentButtonFlag() );
		m_comboButtonBarSize.SetCurSel( ButtonOptions.GetButtonBarLineSize() - 3 );
		m_comboButtonBarLines.SetCurSel( ButtonOptions.GetButtonBarLineCount() - 2 );
		
		SetEditBoxTextFromInt( GetEditMarginLeft(), ButtonOptions.GetButtonBarMarginLeft() );
		SetEditBoxTextFromInt( GetEditMarginRight(), ButtonOptions.GetButtonBarMarginRight() );
		SetEditBoxTextFromInt( GetEditMarginTop(), ButtonOptions.GetButtonBarMarginTop() );
		SetEditBoxTextFromInt( GetEditMarginBottom(), ButtonOptions.GetButtonBarMarginBottom() );
	
		break;

	case 3:
		m_comboButtonWidth.SetCurSel( ( ButtonOptions.GetButtonWidthType() == 1 ) ? 1 : 0 );
		SetEditBoxTextFromInt( GetEditButtonWidthPercent(), ButtonOptions.GetButtonWidthPercent() );
		SetEditBoxTextFromInt( GetEditButtonWidthPixel(), ButtonOptions.GetButtonWidthPixel() );
		SetEditBoxTextFromInt( GetEditGapWidth(), ButtonOptions.GetButtonXGap() );
			
		m_comboButtonHeight.SetCurSel( ( ButtonOptions.GetButtonHeightType() == 1 ) ? 1 : 0 );
		SetEditBoxTextFromInt( GetEditButtonHeightPercent(), ButtonOptions.GetButtonHeightPercent() );
		SetEditBoxTextFromInt( GetEditButtonHeightPixel(), ButtonOptions.GetButtonHeightPixel() );
		SetEditBoxTextFromInt( GetEditGapHeight(), ButtonOptions.GetButtonYGap() );

		m_comboButtonRound.SetCurSel( ButtonOptions.GetButtonRounding() );
		break;
			

	case 4:
		m_buttonColourAction.m_ColourRefBack = ButtonOptions.GetButtonColourBackActionInt();
		m_buttonColourAction.m_ColourRefText = ButtonOptions.GetButtonColourTextActionInt();
		m_buttonColourList.m_ColourRefBack = ButtonOptions.GetButtonColourBackListInt();
		m_buttonColourList.m_ColourRefText = ButtonOptions.GetButtonColourTextListInt();
		m_buttonColourParent.m_ColourRefBack = ButtonOptions.GetButtonColourBackParentInt();
		m_buttonColourParent.m_ColourRefText = ButtonOptions.GetButtonColourTextParentInt();
		m_buttonColourControl.m_ColourRefBack = ButtonOptions.GetButtonColourBackControlInt();
		m_buttonColourControl.m_ColourRefText = ButtonOptions.GetButtonColourTextControlInt();
		m_buttonColourUnused.m_ColourRefBack = ButtonOptions.GetButtonColourBackUnusedInt();

		SetEditBoxTextFromInt( GetEditMarginLeft(), ButtonOptions.GetButtonImageLeftMarginPercent() );
		SetEditBoxTextFromInt( GetEditMarginRight(), ButtonOptions.GetButtonImageRightMarginPercent() );
		SetEditBoxTextFromInt( GetEditMarginTop(), ButtonOptions.GetButtonImageTopMarginPercent() );
		SetEditBoxTextFromInt( GetEditMarginBottom(), ButtonOptions.GetButtonImageBottomMarginPercent() );

		break;
	}

	ClearTabFixBuffer();

	bool bUpper = ( 0 == nPage );
	bool bLower = ( 1 == nPage );
	bool bWidth = ( m_checkWidth.GetCheck() != 0 );
	bool bHeight = ( m_checkHeight.GetCheck() != 0 );

	switch( nPage )
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		AddTabFixItem( &m_checkEnable );
		break;
	}

	switch( nPage )
	{
	case 0:
		AddTabFixItem( &m_checkWidth,					TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticWidth );
		AddTabFixItem( GetEditWidth(),					TRUE, TRUE, bWidth && bEnable );
		AddTabFixItem( &m_checkHeight,					TRUE, TRUE, bEnable );	
		AddTabFixItem( &m_staticHeight2 );
		AddTabFixItem( GetEditHeight(),					TRUE, TRUE, bHeight && bEnable );
		AddTabFixItem( &m_staticColourFill );
		AddTabFixItem( &m_buttonColourFill,				TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticImageAction );
		AddTabFixItem( &m_buttonImage,					TRUE, TRUE, bEnable );
		AddTabFixItem( &m_buttonClear,					TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticImageSize );
		AddTabFixItem( &m_comboImageSize,				TRUE, TRUE, bEnable );
		break;

	case 1:
		AddTabFixItem( &m_checkWidth,					TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticWidth );
		AddTabFixItem( GetEditWidth(),					TRUE, TRUE, bWidth && bEnable );
		AddTabFixItem( &m_staticHeight );
		AddTabFixItem( GetEditHeight(),					TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticTaskbar );
		AddTabFixItem( GetEditTaskbar(),				TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticColourFill );
		AddTabFixItem( &m_buttonColourFill,				TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticImageAction );
		AddTabFixItem( &m_buttonImage,					TRUE, TRUE, bEnable );
		AddTabFixItem( &m_buttonClear,					TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticImageSize );
		AddTabFixItem( &m_comboImageSize,				TRUE, TRUE, bEnable );
		break;

	case 2:
		AddTabFixItem( &m_staticButtonBarHome );
		AddTabFixItem( &m_comboButtonBarHome,			TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticButtonBarDirection );
		AddTabFixItem( &m_comboButtonBarDirection,		TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticButtonBarAlign );
		AddTabFixItem( &m_comboButtonBarAlign,			TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticButtonBarSize );
		AddTabFixItem( &m_comboButtonBarSize,			TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticButtonBarLines );
		AddTabFixItem( &m_comboButtonBarLines,			TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticButtonBarControlLine );
		AddTabFixItem( &m_comboButtonBarControlLine,	TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticButtonBarControlKey );
		AddTabFixItem( &m_comboButtonBarControlKey,		TRUE, TRUE, bEnable );
		AddTabFixItem( &m_checkButtonBarParent,			TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticButtonBarParent );
		break;

	case 3:
		AddTabFixItem( &m_staticButtonWidth );
		AddTabFixItem( &m_comboButtonWidth,				TRUE, TRUE, bEnable );
		AddTabFixItem( GetEditButtonWidthPercent(),		TRUE, ButtonOptions.GetButtonWidthType() == 0, bEnable );
		AddTabFixItem( GetEditButtonWidthPixel(),		TRUE, ButtonOptions.GetButtonWidthType() == 1, bEnable );
		AddTabFixItem( &m_staticGapWidth );
		AddTabFixItem( GetEditGapWidth(),				TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticButtonHeight );
		AddTabFixItem( &m_comboButtonHeight,			TRUE, TRUE, bEnable );
		AddTabFixItem( GetEditButtonHeightPercent(),	TRUE, ButtonOptions.GetButtonHeightType() == 0, bEnable );
		AddTabFixItem( GetEditButtonHeightPixel(),		TRUE, ButtonOptions.GetButtonHeightType() == 1, bEnable);
		AddTabFixItem( &m_staticGapHeight );
		AddTabFixItem( GetEditGapHeight(),				TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticButtonRound );
		AddTabFixItem( &m_comboButtonRound,				TRUE, TRUE, bEnable );
		break;
	
	case 4:
		AddTabFixItem( &m_staticColourAction );
		AddTabFixItem( &m_buttonColourAction,			TRUE, TRUE, bEnable );
		AddTabFixItem( &m_buttonActionBack,				TRUE, TRUE, bEnable );
		AddTabFixItem( &m_buttonActionText,				TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticColourList );
		AddTabFixItem( &m_buttonColourList,				TRUE, TRUE, bEnable );
		AddTabFixItem( &m_buttonListBack,				TRUE, TRUE, bEnable );
		AddTabFixItem( &m_buttonListText,				TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticColourParent );
		AddTabFixItem( &m_buttonColourParent,			TRUE, TRUE, bEnable );
		AddTabFixItem( &m_buttonParentBack,				TRUE, TRUE, bEnable );
		AddTabFixItem( &m_buttonParentText,				TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticColourControl );
		AddTabFixItem( &m_buttonColourControl,			TRUE, TRUE, bEnable );
		AddTabFixItem( &m_buttonControlBack,			TRUE, TRUE, bEnable );
		AddTabFixItem( &m_buttonControlText,			TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticColourUnused );
		AddTabFixItem( &m_buttonColourUnused,			TRUE, TRUE, bEnable );
		AddTabFixItem( &m_buttonUnusedBack,				TRUE, TRUE, bEnable );
		break;
	}

	switch( nPage )
	{
	case 0:
	case 1:
		AddTabFixItem( &m_staticXAlign );
		AddTabFixItem( &m_comboXAlign,					TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticMarginLeft );
		AddTabFixItem( GetEditMarginLeft(),				TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticMarginRight );
		AddTabFixItem( GetEditMarginRight(),			TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticYAlign );
		AddTabFixItem( &m_comboYAlign,					TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticMarginTop );
		AddTabFixItem( GetEditMarginTop(),				TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticMarginBottom );
		AddTabFixItem( GetEditMarginBottom(),			TRUE, TRUE, bEnable );
		break;

	case 2:
	case 4:
		AddTabFixItem( &m_staticMarginLeft );
		AddTabFixItem( GetEditMarginLeft(),				TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticMarginRight );
		AddTabFixItem( GetEditMarginRight(),			TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticMarginTop );
		AddTabFixItem( GetEditMarginTop(),				TRUE, TRUE, bEnable );
		AddTabFixItem( &m_staticMarginBottom );
		AddTabFixItem( GetEditMarginBottom(),			TRUE, TRUE, bEnable );
		break;
	}

	AddTabFixItem( &m_buttonOK );
	AddTabFixItem( &m_buttonCancel );
	
	m_TabSettings.SetFocus();
	ProcessTabFixBuffer( &m_TabSettings );

	if ( 4 == nPage )
	{
		m_staticMarginLeft.SetWindowText( "Image margin left" );
		m_staticMarginRight.SetWindowText( "Image margin right" );
		m_staticMarginTop.SetWindowText( "Image margin top" );
		m_staticMarginBottom.SetWindowText( "Image margin bottom" );
		MoveControl( &m_staticMarginLeft, 310, 108 );
		MoveControl( GetEditMarginLeft(), 396, 106 );
		MoveControl( &m_staticMarginRight, 310, 137 );
		MoveControl( GetEditMarginRight(), 396, 135 );
		MoveControl( &m_staticMarginTop, 310, 166 );
		MoveControl( GetEditMarginTop(), 396, 164 );
		MoveControl( &m_staticMarginBottom, 310, 195 );
		MoveControl( GetEditMarginBottom(), 396, 193 );
	}

	if ( nPage >= 0 )
		UpdateErrorBar();
	else
		m_buttonColourStatus.ShowWindow( SW_HIDE );
}

/**********************************************************************/

void CLayoutOptionsDlg::OnToggleEnable()
{
	ButtonOptions.SetLayoutEnable( m_checkEnable.GetCheck() == 1 );
	UpdateParent();
	SelectTab( m_nTabPage );
}

/**********************************************************************/

void CLayoutOptionsDlg::OnToggleWidth()
{
	int nOldWidth = BackgroundLayoutInfo.GetTotalWindowWidth();

	if ( m_checkWidth.GetCheck() == 1 )
	{
		GetEditWidth() -> EnableWindow( TRUE );
		ButtonOptions.SetWindowWidthAuto( FALSE );
		ButtonOptions.SetWindowWidthManual( nOldWidth );
		UpdateParent();
	}
	else
	{
		GetEditWidth() -> EnableWindow( FALSE ); 
		ButtonOptions.SetWindowWidthAuto( TRUE );
		UpdateParent();
		ButtonOptions.SetWindowWidthManual( BackgroundLayoutInfo.GetTotalWindowWidth() );	
	}
	
	SetEditBoxTextFromInt( GetEditWidth(), ButtonOptions.GetWindowWidthManual() );
}

/**********************************************************************/

void CLayoutOptionsDlg::OnKillFocusWidth()
{
	CString strWidth;
	GetEditWidth() -> GetWindowText( strWidth );
	ButtonOptions.SetWindowWidthManual( atoi( strWidth ) );
	strWidth.Format( "%d", ButtonOptions.GetWindowWidthManual() );
	GetEditWidth() -> SetWindowText( strWidth );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnToggleUpperHeight()
{
	int nOldHeight = BackgroundLayoutInfo.GetUpperAreaHeight();

	if ( m_checkHeight.GetCheck() == 1 )
	{
		GetEditHeight() -> EnableWindow( TRUE );
		ButtonOptions.SetUpperAreaHeightAuto( FALSE );
		ButtonOptions.SetUpperAreaHeightManual( nOldHeight );
		UpdateParent();
	}
	else
	{
		GetEditHeight() -> EnableWindow( FALSE ); 
		ButtonOptions.SetUpperAreaHeightAuto( TRUE );
		UpdateParent();
		ButtonOptions.SetUpperAreaHeightManual( BackgroundLayoutInfo.GetUpperAreaHeight() );	
	}
	
	SetEditBoxTextFromInt( GetEditHeight(), ButtonOptions.GetUpperAreaHeightManual() );
}

/**********************************************************************/

void CLayoutOptionsDlg::OnKillFocusHeight()
{
	CString strHeight;
	GetEditHeight() -> GetWindowText( strHeight );
	int nHeight = atoi( strHeight );

	if ( 0 == m_nTabPage )
	{
		ButtonOptions.SetUpperAreaHeightManual( nHeight );
		nHeight = ButtonOptions.GetUpperAreaHeightManual();
	}
	else
	{
		ButtonOptions.SetLowerAreaHeightManual( nHeight );
		nHeight = ButtonOptions.GetLowerAreaHeightManual();
	}

	SetEditBoxTextFromInt( GetEditHeight(), nHeight );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnKillFocusTaskbar()
{
	CString strTaskbarGap;
	GetEditTaskbar() -> GetWindowText( strTaskbarGap );
	int nTaskbarGap = atoi( strTaskbarGap );

	ButtonOptions.SetLowerAreaTaskbarGap( nTaskbarGap );
	nTaskbarGap = ButtonOptions.GetLowerAreaTaskbarGap();
	
	SetEditBoxTextFromInt( GetEditTaskbar(), nTaskbarGap );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnSelectXAlign()
{
	if ( 0 == m_nTabPage )
	{
		switch( m_comboXAlign.GetCurSel() )
		{
		case 0:	ButtonOptions.SetUpperImageXAlignInt(0);	break;
		case 1:	ButtonOptions.SetUpperImageXAlignInt(1);	break;
		case 2:	ButtonOptions.SetUpperImageXAlignInt(2);	break;
		}
	}
	else
	{
		switch( m_comboXAlign.GetCurSel() )
		{
		case 0:	ButtonOptions.SetLowerImageXAlignInt(0);	break;
		case 1:	ButtonOptions.SetLowerImageXAlignInt(1);	break;
		case 2:	ButtonOptions.SetLowerImageXAlignInt(2);	break;
		}
	}
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnKillFocusMarginLeft()
{
	CString strMargin;
	GetEditMarginLeft() -> GetWindowText( strMargin );
	
	switch ( m_nTabPage )
	{
	case 0:
		ButtonOptions.SetUpperImageMarginLeft( atoi( strMargin ) );
		strMargin.Format( "%d", ButtonOptions.GetUpperImageMarginLeft() );
		break;

	case 1:
		ButtonOptions.SetLowerImageMarginLeft( atoi( strMargin ) );
		strMargin.Format( "%d", ButtonOptions.GetLowerImageMarginLeft() );
		break;

	case 2:
		ButtonOptions.SetButtonBarMarginLeft( atoi( strMargin ) );
		strMargin.Format( "%d", ButtonOptions.GetButtonBarMarginLeft() );
		break;

	case 4:
		ButtonOptions.SetButtonImageLeftMarginPercent( atoi( strMargin ) );
		strMargin.Format( "%d", ButtonOptions.GetButtonImageLeftMarginPercent() );
		break;
	}

	GetEditMarginLeft() -> SetWindowText( strMargin );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnKillFocusMarginRight()
{
	CString strMargin;
	GetEditMarginRight() -> GetWindowText( strMargin );
	
	switch ( m_nTabPage )
	{
	case 0:
		ButtonOptions.SetUpperImageMarginRight( atoi( strMargin ) );
		strMargin.Format( "%d", ButtonOptions.GetUpperImageMarginRight() );
		break;

	case 1:
		ButtonOptions.SetLowerImageMarginRight( atoi( strMargin ) );
		strMargin.Format( "%d", ButtonOptions.GetLowerImageMarginRight() );
		break;

	case 2:
		ButtonOptions.SetButtonBarMarginRight( atoi( strMargin ) );
		strMargin.Format( "%d", ButtonOptions.GetButtonBarMarginRight() );
		break;

	case 4:
		ButtonOptions.SetButtonImageRightMarginPercent( atoi( strMargin ) );
		strMargin.Format( "%d", ButtonOptions.GetButtonImageRightMarginPercent() );
		break;
	}
	
	GetEditMarginRight() -> SetWindowText( strMargin );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnSelectYAlign()
{
	if ( 0 == m_nTabPage )
	{
		switch( m_comboYAlign.GetCurSel() )
		{
		case 0:	ButtonOptions.SetUpperImageYAlignInt(0);	break;
		case 1:	ButtonOptions.SetUpperImageYAlignInt(1);	break;
		case 2:	ButtonOptions.SetUpperImageYAlignInt(2);	break;
		}
	}
	else
	{
		switch( m_comboYAlign.GetCurSel() )
		{
		case 0:	ButtonOptions.SetLowerImageYAlignInt(0);	break;
		case 1:	ButtonOptions.SetLowerImageYAlignInt(1);	break;
		case 2:	ButtonOptions.SetLowerImageYAlignInt(2);	break;
		}
	}
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnKillFocusMarginTop()
{
	CString strMargin;
	GetEditMarginTop() -> GetWindowText( strMargin );
	
	switch ( m_nTabPage )
	{
	case 0:
		ButtonOptions.SetUpperImageMarginTop( atoi( strMargin ) );
		strMargin.Format( "%d", ButtonOptions.GetUpperImageMarginTop() );
		break;

	case 1:
		ButtonOptions.SetLowerImageMarginTop( atoi( strMargin ) );
		strMargin.Format( "%d", ButtonOptions.GetLowerImageMarginTop() );
		break;

	case 2:
		ButtonOptions.SetButtonBarMarginTop( atoi( strMargin ) );
		strMargin.Format( "%d", ButtonOptions.GetButtonBarMarginTop() );
		break;

	case 4:
		ButtonOptions.SetButtonImageTopMarginPercent( atoi( strMargin ) );
		strMargin.Format( "%d", ButtonOptions.GetButtonImageTopMarginPercent() );
		break;
	}

	GetEditMarginTop() -> SetWindowText( strMargin );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnKillFocusMarginBottom()
{
	CString strMargin;
	GetEditMarginBottom() -> GetWindowText( strMargin );
	
	switch ( m_nTabPage )
	{
	case 0:
		ButtonOptions.SetUpperImageMarginBottom( atoi( strMargin ) );
		strMargin.Format( "%d", ButtonOptions.GetUpperImageMarginBottom() );
		break;

	case 1:
		ButtonOptions.SetLowerImageMarginBottom( atoi( strMargin ) );
		strMargin.Format( "%d", ButtonOptions.GetLowerImageMarginBottom() );
		break;

	case 2:
		ButtonOptions.SetButtonBarMarginBottom( atoi( strMargin ) );
		strMargin.Format( "%d", ButtonOptions.GetButtonBarMarginBottom() );
		break;

	case 4:
		ButtonOptions.SetButtonImageBottomMarginPercent( atoi( strMargin ) );
		strMargin.Format( "%d", ButtonOptions.GetButtonImageBottomMarginPercent() );
		break;
	}

	GetEditMarginBottom() -> SetWindowText( strMargin );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnSelectButtonBarHome()
{
	switch( m_comboButtonBarHome.GetCurSel() )
	{
	case 0:	ButtonOptions.SetButtonBarHomeInt(0);	break;
	case 1:	ButtonOptions.SetButtonBarHomeInt(1);	break;
	case 2:	ButtonOptions.SetButtonBarHomeInt(3);	break;
	case 3:	ButtonOptions.SetButtonBarHomeInt(2);	break;
	}
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnSelectButtonBarDirection()
{
	switch( m_comboButtonBarDirection.GetCurSel() )
	{
	case 0:	ButtonOptions.SetButtonBarDirectionInt(1);	break;
	case 1:	ButtonOptions.SetButtonBarDirectionInt(3);	break;
	case 2:	ButtonOptions.SetButtonBarDirectionInt(2);	break;
	case 3:	ButtonOptions.SetButtonBarDirectionInt(0);	break;
	}
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnSelectButtonBarAlign()
{
	switch( m_comboButtonBarAlign.GetCurSel() )
	{
	case 0:	ButtonOptions.SetButtonBarCentre( FALSE );	break;
	case 1:	ButtonOptions.SetButtonBarCentre( TRUE);	break;
	}
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnSelectButtonBarControlLine()
{
	ButtonOptions.SetButtonBarControlLinePosInt( m_comboButtonBarControlLine.GetCurSel() );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnSelectButtonBarControlKey()
{
	ButtonOptions.SetButtonBarControlButtonPosInt( m_comboButtonBarControlKey.GetCurSel() );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnToggleButtonBarParent()
{
	ButtonOptions.SetButtonBarParentButtonFlag( m_checkButtonBarParent.GetCheck() != 0 );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnSelectButtonBarSize()
{
	ButtonOptions.SetButtonBarLineSize( m_comboButtonBarSize.GetCurSel() + 3 );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnSelectButtonBarLines()
{
	ButtonOptions.SetButtonBarLineCount( m_comboButtonBarLines.GetCurSel() + 2 );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnSelectButtonWidth()
{
	ButtonOptions.SetButtonWidthType( m_comboButtonWidth.GetCurSel() );
	ShowAndEnableWindow( GetEditButtonWidthPercent(), ButtonOptions.GetButtonWidthType() == 0 );
	ShowAndEnableWindow( GetEditButtonWidthPixel(), ButtonOptions.GetButtonWidthType() == 1 );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnKillFocusButtonWidthPercent()
{
	CString strWidth;
	GetEditButtonWidthPercent() -> GetWindowText( strWidth );
	ButtonOptions.SetButtonWidthPercent( atoi( strWidth ) );
	strWidth.Format( "%d", ButtonOptions.GetButtonWidthPercent() );
	GetEditButtonWidthPercent() -> SetWindowText( strWidth );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnKillFocusButtonWidthPixel()
{
	CString strWidth;
	GetEditButtonWidthPixel() -> GetWindowText( strWidth );
	ButtonOptions.SetButtonWidthPixel( atoi( strWidth ) );
	strWidth.Format( "%d", ButtonOptions.GetButtonWidthPixel() );
	GetEditButtonWidthPixel() -> SetWindowText( strWidth );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnKillFocusGapWidth()
{
	CString strWidth;
	GetEditGapWidth() -> GetWindowText( strWidth );
	ButtonOptions.SetButtonXGap( atoi( strWidth ) );
	strWidth.Format( "%d", ButtonOptions.GetButtonXGap() );
	GetEditGapWidth() -> SetWindowText( strWidth );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnSelectButtonHeight()
{
	ButtonOptions.SetButtonHeightType( m_comboButtonHeight.GetCurSel() );
	ShowAndEnableWindow( GetEditButtonHeightPercent(), ButtonOptions.GetButtonHeightType() == 0 );
	ShowAndEnableWindow( GetEditButtonHeightPixel(), ButtonOptions.GetButtonHeightType() == 1 );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnKillFocusButtonHeightPercent()
{
	CString strHeight;
	GetEditButtonHeightPercent() -> GetWindowText( strHeight );
	ButtonOptions.SetButtonHeightPercent( atoi( strHeight ) );
	strHeight.Format( "%d", ButtonOptions.GetButtonHeightPercent() );
	GetEditButtonHeightPercent() -> SetWindowText( strHeight );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnKillFocusButtonHeightPixel()
{
	CString strHeight;
	GetEditButtonHeightPixel() -> GetWindowText( strHeight );
	ButtonOptions.SetButtonHeightPixel( atoi( strHeight ) );
	strHeight.Format( "%d", ButtonOptions.GetButtonHeightPixel() );
	GetEditButtonHeightPixel() -> SetWindowText( strHeight );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnKillFocusGapHeight()
{
	CString strHeight;
	GetEditGapHeight() -> GetWindowText( strHeight );
	ButtonOptions.SetButtonYGap( atoi( strHeight ) );
	strHeight.Format( "%d", ButtonOptions.GetButtonYGap() );
	GetEditGapHeight() -> SetWindowText( strHeight );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnSelectButtonRounding()
{
	ButtonOptions.SetButtonRounding( m_comboButtonRound.GetCurSel() );
	UpdateParent();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnButtonImage()
{
	CFileDialog dlgFile ( TRUE, "", "", OFN_NOCHANGEDIR|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, "Image Files (*.bmp;*.gif;*.jpg;*.png)|*.bmp;*.gif;*.jpg;*.png||", NULL );
	
	if ( 0 == m_nTabPage )
		dlgFile.m_ofn.lpstrTitle = "Select Upper Area Image";
	else
		dlgFile.m_ofn.lpstrTitle = "Select Lower Area Image";
	
	dlgFile.m_ofn.lpstrInitialDir = m_strImagePath;
	
	if ( dlgFile.DoModal() != IDOK )
		return;

	CString strFilepath = dlgFile.GetPathName();
	CString strFilename = dlgFile.GetFileName();
	CString strExt = dlgFile.GetFileExt();

	int nPathLen = strFilepath.GetLength();
	int nFileLen = strFilename.GetLength();

	if ( nPathLen >= nFileLen )
		m_strImagePath = strFilepath.Left ( nPathLen - nFileLen );

	strExt.MakeUpper();

	int nType = 0;
	if ( strExt == "GIF" )	nType = 1;
	else if ( strExt == "JPG" )	nType = 2;
	else if ( strExt == "BMP" )	nType = 3;
	else if ( strExt == "PNG" )	nType = 4;

	if ( nType != 0 )
	{
		if ( 0 == m_nTabPage )
		{
			CString strDestFilename = "";
			strDestFilename += "Layout\\Temp\\Upper.";
			strDestFilename += strExt;
			::GetSyssetProgramPath( strDestFilename );
			
			CopyFile( strFilepath, strDestFilename, FALSE );
			ButtonOptions.SetUpperImageTypeInt( nType );
			UpdateParent();
		}
		else
		{
			CString strDestFilename = "";
			strDestFilename += "Layout\\Temp\\Lower.";
			strDestFilename += strExt;
			::GetSyssetProgramPath( strDestFilename );
			
			CopyFile( strFilepath, strDestFilename, FALSE );
			ButtonOptions.SetLowerImageTypeInt( nType );
			UpdateParent();
		}
	}
}

/**********************************************************************/

void CLayoutOptionsDlg::OnButtonClear()
{
	if ( Prompter.YesNo( "Are you sure you wish to remove\nany background images" ) == IDYES )
	{
		CString strDestFilename = "";

		if ( 0 == m_nTabPage )
			strDestFilename += "Layout\\Temp\\Upper.";
		else
			strDestFilename += "Layout\\Temp\\Lower.";
		
		::GetSyssetProgramPath( strDestFilename );

		CFileRemover FileRemover;
		FileRemover.RemoveFile( strDestFilename + ".GIF" );
		FileRemover.RemoveFile( strDestFilename + ".JPG" );
		FileRemover.RemoveFile( strDestFilename + ".BMP" );
		FileRemover.RemoveFile( strDestFilename + ".PNG" );

		if ( 0 == m_nTabPage )
			ButtonOptions.SetUpperImageTypeInt(0);
		else
			ButtonOptions.SetLowerImageTypeInt(0);
		
		UpdateParent();
	}
}

/**********************************************************************/

void CLayoutOptionsDlg::OnSelectImageSize()
{
	if ( 0 == m_nTabPage )
	{
		ButtonOptions.SetUpperImageStretch( m_comboImageSize.GetCurSel() == 1 );
		UpdateParent();
	}
	else
	{
		ButtonOptions.SetLowerImageStretch( m_comboImageSize.GetCurSel() == 1 );
		UpdateParent();
	}
}

/**********************************************************************/

void CLayoutOptionsDlg::OnButtonFillColour()
{
	int nNewColour = 0;
	CString strText = "";

	m_nColourMode = 0;
	
	if ( 0 == m_nTabPage )
	{
		nNewColour = ButtonOptions.GetUpperAreaFillColourInt();
		strText = "Set Upper Area Background Colour";
	}
	else
	{
		nNewColour = ButtonOptions.GetUpperAreaFillColourInt();
		strText = "Set Lower Area Background Colour";
	}

	CColourEditDlg dlg( nNewColour, strText, this, FALSE, this );
	
	if ( dlg.DoModal() == IDOK )
		nNewColour = dlg.GetColourRef();

	if ( 0 == m_nTabPage )
		ButtonOptions.SetUpperAreaFillColourInt( nNewColour );
	else
		ButtonOptions.SetLowerAreaFillColourInt( nNewColour );
	
	m_buttonColourFill.m_ColourRefBack = nNewColour;
	m_buttonColourFill.Invalidate();
	
	GetParent() -> Invalidate();
}

/**********************************************************************/

long CLayoutOptionsDlg::OnColourButtonMessage( WPARAM w, LPARAM l )
{
	switch( m_nColourMode )
	{
	case 0:

		m_buttonColourFill.m_ColourRefBack = l;
		m_buttonColourFill.Invalidate();

		if ( 0 == m_nTabPage )
			ButtonOptions.SetUpperAreaFillColourInt( l );
		else
			ButtonOptions.SetLowerAreaFillColourInt( l );

		break;

	case 1:
		m_buttonColourAction.m_ColourRefBack = l;
		m_buttonColourAction.Invalidate();
		ButtonOptions.SetButtonColourBackActionInt( l );
		ButtonManager.LoadCurrentDisplayList();
		break;

	case 2:
		m_buttonColourAction.m_ColourRefText = l;
		m_buttonColourAction.Invalidate();
		ButtonOptions.SetButtonColourTextActionInt( l );
		ButtonManager.LoadCurrentDisplayList();
		break;

	case 3:
		m_buttonColourList.m_ColourRefBack = l;
		m_buttonColourList.Invalidate();
		ButtonOptions.SetButtonColourBackListInt( l );
		ButtonManager.LoadCurrentDisplayList();
		break;

	case 4:
		m_buttonColourList.m_ColourRefText = l;
		m_buttonColourList.Invalidate();
		ButtonOptions.SetButtonColourTextListInt( l );
		ButtonManager.LoadCurrentDisplayList();
		break;

	case 5:
		m_buttonColourParent.m_ColourRefBack = l;
		m_buttonColourParent.Invalidate();
		ButtonOptions.SetButtonColourBackParentInt( l );
		ButtonManager.LoadCurrentDisplayList();
		break;

	case 6:
		m_buttonColourParent.m_ColourRefText = l;
		m_buttonColourParent.Invalidate();
		ButtonOptions.SetButtonColourTextParentInt( l );
		ButtonManager.LoadCurrentDisplayList();
		break;

	case 7:
		m_buttonColourControl.m_ColourRefBack = l;
		m_buttonColourControl.Invalidate();
		ButtonOptions.SetButtonColourBackControlInt( l );
		ButtonManager.LoadCurrentDisplayList();
		break;

	case 8:
		m_buttonColourControl.m_ColourRefText = l;
		m_buttonColourControl.Invalidate();
		ButtonOptions.SetButtonColourTextControlInt( l );
		ButtonManager.LoadCurrentDisplayList();
		break;

	case 9:
		m_buttonColourUnused.m_ColourRefBack = l;
		m_buttonColourUnused.Invalidate();
		ButtonOptions.SetButtonColourBackUnusedInt( l );
		ButtonManager.LoadCurrentDisplayList();
		break;
	}

	GetParent() -> Invalidate();

	return 0l;
}

/**********************************************************************/

void CLayoutOptionsDlg::OnButtonActionBack()
{
	m_nColourMode = 1;
	
	int nNewColour = ButtonOptions.GetButtonColourBackActionInt();
	CColourEditDlg dlg( nNewColour, "Set Action Button Background Colour", this, FALSE, this );
	if ( dlg.DoModal() == IDOK )
		nNewColour = dlg.GetColourRef();

	m_buttonColourAction.m_ColourRefBack = nNewColour;
	m_buttonColourAction.Invalidate();
	ButtonOptions.SetButtonColourBackActionInt( nNewColour );
	ButtonManager.LoadCurrentDisplayList();
	GetParent() -> Invalidate();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnButtonActionText()
{
	m_nColourMode = 2;
	
	int nNewColour = ButtonOptions.GetButtonColourTextActionInt();
	CColourEditDlg dlg( nNewColour, "Set Action Button Text Colour", this, FALSE, this );
	if ( dlg.DoModal() == IDOK )
		nNewColour = dlg.GetColourRef();

	m_buttonColourAction.m_ColourRefText = nNewColour;
	m_buttonColourAction.Invalidate();
	ButtonOptions.SetButtonColourTextActionInt( nNewColour );
	ButtonManager.LoadCurrentDisplayList();
	GetParent() -> Invalidate();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnButtonListBack()
{
	m_nColourMode = 3;
	
	int nNewColour = ButtonOptions.GetButtonColourBackListInt();
	CColourEditDlg dlg( nNewColour, "Set List Button Background Colour", this, FALSE, this );
	if ( dlg.DoModal() == IDOK )
		nNewColour = dlg.GetColourRef();

	m_buttonColourList.m_ColourRefBack = nNewColour;
	m_buttonColourList.Invalidate();
	ButtonOptions.SetButtonColourBackListInt( nNewColour );
	ButtonManager.LoadCurrentDisplayList();
	GetParent() -> Invalidate();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnButtonListText()
{
	m_nColourMode = 4;
	
	int nNewColour = ButtonOptions.GetButtonColourTextListInt();
	CColourEditDlg dlg( nNewColour, "Set List Button Text Colour", this, FALSE, this );
	if ( dlg.DoModal() == IDOK )
		nNewColour = dlg.GetColourRef();

	m_buttonColourList.m_ColourRefText = nNewColour;
	m_buttonColourList.Invalidate();
	ButtonOptions.SetButtonColourTextListInt( nNewColour );
	ButtonManager.LoadCurrentDisplayList();
	GetParent() -> Invalidate();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnButtonParentBack()
{
	m_nColourMode = 5;
	
	int nNewColour = ButtonOptions.GetButtonColourBackParentInt();
	CColourEditDlg dlg( nNewColour, "Set Parent Button Background Colour", this, FALSE, this );
	if ( dlg.DoModal() == IDOK )
		nNewColour = dlg.GetColourRef();

	m_buttonColourParent.m_ColourRefBack = nNewColour;
	m_buttonColourParent.Invalidate();
	ButtonOptions.SetButtonColourBackParentInt( nNewColour );
	ButtonManager.LoadCurrentDisplayList();
	GetParent() -> Invalidate();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnButtonParentText()
{
	m_nColourMode = 6;
	
	int nNewColour = ButtonOptions.GetButtonColourTextParentInt();
	CColourEditDlg dlg( nNewColour, "Set Parent Button Text Colour", this, FALSE, this );
	if ( dlg.DoModal() == IDOK )
		nNewColour = dlg.GetColourRef();

	m_buttonColourParent.m_ColourRefText = nNewColour;
	m_buttonColourParent.Invalidate();
	ButtonOptions.SetButtonColourTextParentInt( nNewColour );
	ButtonManager.LoadCurrentDisplayList();
	GetParent() -> Invalidate();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnButtonControlBack()
{
	m_nColourMode = 7;
	
	int nNewColour = ButtonOptions.GetButtonColourBackControlInt();
	CColourEditDlg dlg( nNewColour, "Set Control Button Background Colour", this, FALSE, this );
	if ( dlg.DoModal() == IDOK )
		nNewColour = dlg.GetColourRef();

	m_buttonColourControl.m_ColourRefBack = nNewColour;
	m_buttonColourControl.Invalidate();
	ButtonOptions.SetButtonColourBackControlInt( nNewColour );
	ButtonManager.LoadCurrentDisplayList();
	GetParent() -> Invalidate();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnButtonControlText()
{
	m_nColourMode = 8;
	
	int nNewColour = ButtonOptions.GetButtonColourTextControlInt();
	CColourEditDlg dlg( nNewColour, "Set Control Button Text Colour", this, FALSE, this );
	if ( dlg.DoModal() == IDOK )
		nNewColour = dlg.GetColourRef();

	m_buttonColourControl.m_ColourRefText = nNewColour;
	m_buttonColourControl.Invalidate();
	ButtonOptions.SetButtonColourTextControlInt( nNewColour );
	ButtonManager.LoadCurrentDisplayList();
	GetParent() -> Invalidate();
}

/**********************************************************************/

void CLayoutOptionsDlg::OnButtonUnusedBack()
{
	m_nColourMode = 9;
	
	int nNewColour = ButtonOptions.GetButtonColourBackUnusedInt();
	CColourEditDlg dlg( nNewColour, "Set Unused Button Background Colour", this, FALSE, this );
	if ( dlg.DoModal() == IDOK )
		nNewColour = dlg.GetColourRef();

	m_buttonColourUnused.m_ColourRefBack = nNewColour;
	m_buttonColourUnused.Invalidate();
	ButtonOptions.SetButtonColourBackUnusedInt( nNewColour );
	ButtonManager.LoadCurrentDisplayList();
	GetParent() -> Invalidate();
}

/**********************************************************************/
