/**********************************************************************/
#include "Password.h"
#include "SPOSKeyboardSQLExporter.h"
/**********************************************************************/
#include "SPOSKeyboardDlg.h"
/**********************************************************************/

CSPOSKeyboardDlg::CSPOSKeyboardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSPOSKeyboardDlg::IDD, pParent)
{
	m_bInitDialog = FALSE;

	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME );

	CSPOSKeyboardListComboItem item;
	item.m_nListNo = 0;
	item.m_nPageNo = 0;
	item.m_nScreenNo = 0;
	item.m_nScreenListType = 0;
	item.m_strText = "";
	
	m_arrayPreviousComboItems.Add( item );	//STANDARD ITEM LISTS
	m_arrayPreviousComboItems.Add( item );	//LARGE ITEM LISTS
	m_arrayPreviousComboItems.Add( item );	//STANDARD FUNCTION LISTS
	m_arrayPreviousComboItems.Add( item );	//LARGE FUNCTION LISTS
	m_arrayPreviousComboItems.Add( item );	//BOTTOM FUNCTION LISTS
	m_arrayPreviousComboItems.Add( item );	//SCREENS

	m_nViewMode = VIEWMODE_LIST_ITEM_STANDARD;
}

/**********************************************************************/

CSPOSKeyboardDlg::~CSPOSKeyboardDlg()
{
}

/**********************************************************************/

void CSPOSKeyboardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_VIEW, m_comboView);
	DDX_Control(pDX, IDC_COMBO_LIST, m_comboList);
	DDX_Control(pDX, IDC_COMBO_PAGE, m_comboPage);
	DDX_Control(pDX, IDC_CHECK_INVISIBLE, m_checkInvisible);
	DDX_Control(pDX, IDC_CHECK_IMAGES, m_checkImages);
	DDX_Control(pDX, IDC_BUTTON_PLUSORT, m_buttonPluSort);
}

/**********************************************************************/
BEGIN_MESSAGE_MAP(CSPOSKeyboardDlg, CDialog)

	ON_WM_SIZE()
	/******/
	ON_COMMAND(IDM_SAVE_KEYBOARD, OnSaveKeyboard)
	ON_COMMAND(IDM_IMPORT_SQL, OnImportWizardData)
	ON_COMMAND(IDM_IMPORT_IMAGES, OnImportImages)
	ON_COMMAND(IDM_EXIT_KEYBOARD, OnOK)
	ON_COMMAND(IDM_ZOOM_SMALL, OnZoomSmall)
	ON_COMMAND(IDM_ZOOM_SMALL, OnZoomSmall)
	ON_COMMAND(IDM_ZOOM_NORMAL, OnZoomNormal)
	ON_COMMAND(IDM_ZOOM_BIG, OnZoomBig)
	ON_COMMAND(IDM_EDIT_KEYTYPES, OnEditKeyTypes)
	ON_COMMAND(IDM_GLOBAL_SETTINGS, OnGlobalSettings)
	ON_COMMAND(IDM_EDIT_MACROS, OnEditMacros)
	ON_COMMAND(IDM_OPTIONS, OnSetOptions)
	/******/
	ON_CBN_SELCHANGE(IDC_COMBO_VIEW, OnSelectViewMode)
	ON_BN_CLICKED(IDC_CHECK_INVISIBLE, OnCheckInvisible)
	ON_BN_CLICKED(IDC_CHECK_IMAGES, OnCheckImages)
	ON_CBN_SELCHANGE(IDC_COMBO_LIST, OnSelectList)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_LIST, OnButtonPrintList)
	ON_CBN_SELCHANGE(IDC_COMBO_PAGE, OnSelectPage)
	ON_BN_CLICKED(IDC_BUTTON_ADDPAGE, OnButtonAddPage)
	ON_BN_CLICKED(IDC_BUTTON_EDITPAGE, OnButtonEditPage)
	ON_BN_CLICKED(IDC_BUTTON_DELETEPAGE, OnButtonDeletePage)
	ON_BN_CLICKED(IDC_BUTTON_PLUSORT, OnButtonPluSort)
	
END_MESSAGE_MAP()
/**********************************************************************/

BOOL CSPOSKeyboardDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetIcon ( m_hIcon, TRUE );
	m_bInitDialog = TRUE;

	m_nViewMode = SPOSKeyboardManager.GetLayoutOptions() -> GetInitialViewMode();
	if ( ( m_nViewMode < 0 ) || ( m_nViewMode > 5 ) )
		m_nViewMode = 0;

	m_comboView.AddString( "Standard List Area" );
	m_comboView.AddString( "Large List Area" );
	m_comboView.AddString( "Standard Function Area" );
	m_comboView.AddString( "Large Function Area" );
	m_comboView.AddString( "Bottom Function Bar" );
	m_comboView.AddString( "Screens" );
	m_comboView.SetCurSel( m_nViewMode );

	CArray<int,int> arrayTabStops;
	arrayTabStops.Add(0);
	arrayTabStops.Add(50);
	arrayTabStops.Add(130);
	m_comboList.SetTabStops( arrayTabStops );

	m_checkInvisible.SetCheck( TRUE );
	m_checkImages.SetCheck( TRUE );

	RECT  rectSize;
    GetClientRect (&rectSize);
    rectSize.top += 100;

    CCreateContext pContext;
    pContext.m_pCurrentDoc = NULL;
    pContext.m_pNewViewClass = RUNTIME_CLASS(CSPOSKeyboardView);

	int  nID = AFX_IDW_PANE_FIRST;
	m_pView = (CSPOSKeyboardView*)pContext.m_pNewViewClass->CreateObject();
    ASSERT(m_pView);
    ASSERT_KINDOF(CWnd, m_pView);

    if (m_pView)
	{
        if (!m_pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0,0,0,0), this, nID, &pContext)) 
		{
            TRACE0("Warning: could not create view for dialog.\n");
            return FALSE;		
		}
    } 
	else 
	{
        TRACE0("Warning: dynamic create of CScrollView for dialog failed.\n");
        return FALSE;
    }

	SPOSKeyboardManager.GetDlgViewLink() -> SetDlgPointer(this);
	SPOSKeyboardManager.GetDlgViewLink() -> SetViewPointer(m_pView);

	m_pView -> InitialUpdate();

	if ( PasswordArray.GetEnable( PasswordTicks::SetupMenuMacros ) == FALSE )
	{
		CMenuChanger MenuChanger;
		MenuChanger.EatMenuTopLevel( GetMenu() );
		MenuChanger.KillItem( IDM_EDIT_MACROS );
		MenuChanger.BuildMenu( GetMenu() );
	}		

    CSize sizeTotal;
    sizeTotal.cx = rectSize.right;
    sizeTotal.cy = rectSize.bottom;
    m_pView->SetScrollSizes(MM_TEXT, sizeTotal);
    m_pView->ShowWindow(SW_NORMAL);
	
    m_pView->MoveWindow(&rectSize);
	ShowWindow( SW_MAXIMIZE );
    return TRUE;
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnSize(UINT nType, int cx, int cy) 
{
	if ( m_bInitDialog == TRUE )
	{
		CDialog::OnSize(nType, cx, cy);
		m_pView -> MoveWindow ( 0, 100, cx, cy - 100 );
		m_pView -> HandleResize();
	}
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnSaveKeyboard()
{
	m_pView -> SaveKeyboard();
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnImportWizardData()
{
	m_pView -> ImportWizardData();
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnImportImages()
{
	m_pView -> ImportImages();
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnZoomSmall()
{
	m_pView -> ZoomSmall();
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnZoomNormal()
{
	m_pView -> ZoomNormal();
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnZoomBig()
{
	m_pView -> ZoomBig();
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnEditKeyTypes()
{
	m_pView -> EditKeyTypes();
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnGlobalSettings()
{
	m_pView -> GlobalSettings();
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnEditMacros()
{
	m_pView -> EditMacros();
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnSetOptions()
{
	m_pView -> SetOptions();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void CSPOSKeyboardDlg::GetPreviousComboItem( CSPOSKeyboardListComboItem& item )
{
	item = m_arrayPreviousComboItems[m_nViewMode];
}

/**********************************************************************/

void CSPOSKeyboardDlg::SetPreviousComboItem( CSPOSKeyboardListComboItem& item )
{
	m_arrayPreviousComboItems[m_nViewMode] = item;
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnSelectViewMode()
{
	if ( SelectViewMode() == TRUE )
		m_pView -> SelectViewMode();
}

/**********************************************************************/

bool CSPOSKeyboardDlg::SelectViewMode()
{
	int nNewViewMode = m_comboView.GetCurSel();
	if ( nNewViewMode == m_nViewMode )
		return FALSE;

	int nSel = m_comboList.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_arrayListComboItems.GetSize() ) )
	{
		m_arrayPreviousComboItems[m_nViewMode] = m_arrayListComboItems[nSel];
		m_arrayPreviousComboItems[m_nViewMode].m_nPageNo = m_comboPage.GetCurSel() + 1;
	}

	m_nViewMode = nNewViewMode;
	return TRUE;
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnCheckInvisible()
{
	m_pView -> CheckInvisible();
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnCheckImages()
{
	m_pView -> CheckImages();
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnSelectList()
{
	if ( SelectList() == TRUE )
		m_pView -> SelectList();
}

/**********************************************************************/

bool CSPOSKeyboardDlg::SelectList()
{
	int nSel = m_comboList.GetCurSel();
	if ( nSel < 0 || nSel >= m_arrayListComboItems.GetSize() )
		return FALSE;

	if ( m_arrayPreviousComboItems[m_nViewMode].m_nListNo == m_arrayListComboItems[nSel].m_nListNo )
		return FALSE;

	m_arrayPreviousComboItems[m_nViewMode] = m_arrayListComboItems[nSel];
	m_arrayPreviousComboItems[m_nViewMode].m_nPageNo = 0;

	return TRUE;
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnButtonAdd()
{
	m_pView -> ButtonAdd();
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnButtonEdit()
{
	m_pView -> ButtonEdit();
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnButtonDelete()
{
	m_pView -> ButtonDelete();
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnButtonPrintList()
{
	m_pView -> ButtonPrintList();
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnSelectPage()
{
	SelectPage();
	m_pView -> SelectPage();
}

/**********************************************************************/

void CSPOSKeyboardDlg::SelectPage()
{
	int nPageNo = m_comboPage.GetCurSel() + 1;
	m_arrayPreviousComboItems[m_nViewMode].m_nPageNo = nPageNo;
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnButtonAddPage()
{
	m_pView -> ButtonAddPage();
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnButtonEditPage()
{
	m_pView -> ButtonEditPage();
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnButtonDeletePage()
{
	m_pView -> ButtonDeletePage();
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnButtonPluSort()
{
	m_pView -> PluSort();
}

/**********************************************************************/

void CSPOSKeyboardDlg::RefreshListCombo()
{
	SPOSKeyboardManager.GetListManager() -> GetListComboInfo( m_nViewMode, m_arrayListComboItems );

	int nSel = -1;

	m_comboList.ResetContent();
	m_comboList.SetViewMode( m_nViewMode );

	for ( int n = 0; n < m_arrayListComboItems.GetSize(); n++ )
	{
		m_comboList.AddItem( m_arrayListComboItems[n].m_strText );
		
		switch( m_nViewMode )
		{
		case VIEWMODE_LIST_ITEM_STANDARD:
		case VIEWMODE_LIST_ITEM_LARGE:
		case VIEWMODE_LIST_FUNCTION_STANDARD:
		case VIEWMODE_LIST_FUNCTION_LARGE:
		case VIEWMODE_LIST_FUNCTION_BOTTOM:
			if ( m_arrayListComboItems[n].m_nListNo == m_arrayPreviousComboItems[m_nViewMode].m_nListNo )
				nSel = n;
			break;

		case VIEWMODE_SCREEN:
			{
				if ( m_arrayListComboItems[n].m_nScreenNo == m_arrayPreviousComboItems[m_nViewMode].m_nScreenNo &&
					m_arrayListComboItems[n].m_nScreenListType == m_arrayPreviousComboItems[m_nViewMode].m_nScreenListType )
						nSel = n;
			}
			break;
		}
	}

	if ( nSel != -1 )
	{
		m_comboList.SetCurSel( nSel );
		m_arrayPreviousComboItems[m_nViewMode] = m_arrayListComboItems[nSel];
	}
	else if ( m_comboList.GetCount() != 0 )
	{
		m_comboList.SetCurSel( 0 );
		m_arrayPreviousComboItems[m_nViewMode] = m_arrayListComboItems[0];
	}
	else
	{
		m_arrayPreviousComboItems[m_nViewMode].m_nListNo = 0;
		m_arrayPreviousComboItems[m_nViewMode].m_nPageNo = 0;
		m_arrayPreviousComboItems[m_nViewMode].m_nScreenNo = 0;
		m_arrayPreviousComboItems[m_nViewMode].m_nScreenListType = 0;
	}

	m_comboList.EnableWindow( m_comboList.GetCount() >= 2 );
}

/**********************************************************************/

void CSPOSKeyboardDlg::SetListNo( __int64 nListNo )
{
	m_arrayPreviousComboItems[m_nViewMode].m_nListNo = nListNo;
	m_arrayPreviousComboItems[m_nViewMode].m_nPageNo = 1;
}	

/**********************************************************************/

void CSPOSKeyboardDlg::SetPageNo( int nPageNo )
{
	m_arrayPreviousComboItems[m_nViewMode].m_nPageNo = nPageNo;
}	

/**********************************************************************/

void CSPOSKeyboardDlg::RefreshPageCombo()
{
	m_comboPage.ResetContent();

	int nPageCount = 0;

	int nListIdx;
	__int64 nListNo = m_arrayPreviousComboItems[m_nViewMode].m_nListNo;
	if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == TRUE )
	{
		CSPOSKeyboardListRecord ListRecord;
		SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );
		nPageCount = ListRecord.GetPageCount();
	}

	for ( int n = 1; n <= nPageCount; n++ )
	{
		CString strPageNo;
		strPageNo.Format ( "Page %d of %d", n, nPageCount );
		m_comboPage.AddString( strPageNo );
	}

	int nPageNo = m_arrayPreviousComboItems[m_nViewMode].m_nPageNo;

	if ( nPageNo >= 1 && nPageNo <= nPageCount )
	{
		m_comboPage.SetCurSel( nPageNo - 1 );
		m_arrayPreviousComboItems[m_nViewMode].m_nPageNo = nPageNo;
	}
	else if ( m_comboPage.GetCount() != 0 )
	{
		m_comboPage.SetCurSel( 0 );
		m_arrayPreviousComboItems[m_nViewMode].m_nPageNo = 1;
	}
	else
		m_arrayPreviousComboItems[m_nViewMode].m_nPageNo = 0;

	m_comboPage.EnableWindow( m_comboPage.GetCount() >= 2 );
}

/**********************************************************************/

void CSPOSKeyboardDlg::GetVisibleListDetails( int& nScreenNo, __int64& nListNo, int& nPageNo, int& nKeyCount )
{
	nScreenNo = 0;
	nListNo = 0;
	nPageNo = 0;
	nKeyCount = 0;

	if ( ::IsWindow( m_comboList.m_hWnd ) == FALSE )
		return;

	int nSel = m_comboList.GetCurSel();
	if ( nSel < 0 || nSel >= m_arrayListComboItems.GetSize() )
		return;

	nScreenNo = m_arrayListComboItems[nSel].m_nScreenNo;
	nListNo = m_arrayListComboItems[nSel].m_nListNo;
	
	int nListIdx;
	int nMaxPageNo = 1;
	
	if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == TRUE )
	{
		CSPOSKeyboardListRecord ListRecord;
		SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );
		nKeyCount = ListRecord.GetKeysPerPage();
		nMaxPageNo = ListRecord.GetPageCount();
	}
	
	nPageNo = m_comboPage.GetCurSel() + 1;
	if ( nPageNo < 1 || nPageNo > nMaxPageNo )
		nPageNo = 1;
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnCancel()
{
	OnOK();
}

/**********************************************************************/

void CSPOSKeyboardDlg::OnOK()
{
	if ( SaveModified() == TRUE )
		EndDialog( IDOK );
}

/**********************************************************************/

bool CSPOSKeyboardDlg::SaveModified() 
{
	bool bReallyExit = TRUE;

	if ( SPOSKeyboardManager.GetListManager() -> IsModified() )
	{
		CString strMsg = "The keyboard layout has been modified\n";
		strMsg += "Do you wish to save your changes?";

		int nResult = Prompter.YesNoCancel( strMsg );
		
		switch ( nResult )
		{
		case IDYES:
			{
				::CreateSubdirectory( SPOSKeyboardManager.GetLayoutOptions() -> GetDataPath() );

				CSPOSKeyboardSQLExporter SQLExporter( SPOSKeyboardManager.GetSPOSVersion() -> GetSPOSVersion() );
				
				SQLExporter.ExportFileForSave( 
					SPOSKeyboardManager.GetLayoutOptions() -> GetSQLOutFilename(), 
					SPOSKeyboardManager.GetMergedLegacyMacrosFlag() == FALSE );

				SPOSKeyboardManager.GetListManager() -> WriteListNames();
				SPOSKeyboardManager.GetColourTable() -> Write();
				SPOSKeyboardManager.GetFunctionTable() -> Write();
				SPOSKeyboardManager.GetListTable() -> Write();
				SPOSKeyboardManager.GetMacroTable() -> Write();
				SPOSKeyboardManager.GetPaymentTable() -> Write();
				SPOSKeyboardManager.GetControlTable() -> Write();
				SPOSKeyboardManager.GetModifierTable() -> Write();
				SPOSKeyboardManager.GetSvrPresetTable() -> Write();
				SPOSKeyboardManager.GetEmptyTable() -> Write();
			}
			break;

		case IDNO:
			break;

		case IDCANCEL:
			bReallyExit = FALSE;
			break;
		}
	}	
	return bReallyExit;
}

/**********************************************************************/

