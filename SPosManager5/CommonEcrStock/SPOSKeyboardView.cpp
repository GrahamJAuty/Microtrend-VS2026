/**********************************************************************/
#include "Password.h"
#include "ListDataMacroDlg.h"
#include "SPOSKeyboardDlg.h"
#include "SPOSKeyboardColourGlobalDlg.h"
#include "SPOSKeyboardGlobalFunctions.h"
#include "SPOSKeyboardImageImportDlg.h"
#include "SPOSKeyboardKeyHelpers.h"
#include "SPOSKeyboardListAddDlg.h"
#include "SPOSKeyboardListPrintDlg.h"
#include "SPOSKeyboardOptionsDlg.h"
#include "SPOSKeyboardPageAddDlg.h"
#include "SPOSKeyboardPageListDlg.h"
#include "SPOSKeyboardPastePromptDlg.h"
#include "..\SPosEcrManager5\SPOSKeyboardPluEditEcrman.h"
#include "SPOSKeyboardPluSortOptionDlg.h"
#include "SPOSKeyboardPluSortPromptDlg.h"
#include "SPOSKeyboardPropertySheetEditKey.h"
#include "SPOSKeyboardPropertySheetEditList.h"
#include "SPOSKeyboardPropertySheetEditScreen.h"
#include "SPOSKeyboardScreenAddDlg.h"
#include "SPOSKeyboardSQLExporter.h"
#include "SPOSKeyboardSQLImporter.h"
#include "SPOSKeyboardSQLImportStatsDlg.h"
/**********************************************************************/
#include "SPOSKeyboardView.h"
/**********************************************************************/

IMPLEMENT_DYNCREATE(CSPOSKeyboardView, CScrollView)

BEGIN_MESSAGE_MAP(CSPOSKeyboardView, CScrollView)
	//{{AFX_MSG_MAP(CSPOSKeyboardView)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEACTIVATE()
	ON_COMMAND(IDM_EDIT_KEY, OnEditKey)
	ON_COMMAND(IDM_EDIT_PLU_MASTER, OnEditPluMaster)
	ON_COMMAND(IDM_EDIT_PLU_PRICESET1, OnEditPluPriceSet1)
	ON_COMMAND(IDM_EDIT_PLU_PRICESET2, OnEditPluPriceSet2)
	ON_COMMAND(IDM_EDIT_PLU_PRICESET3, OnEditPluPriceSet3)
	ON_COMMAND(IDM_CREATE_PLU, OnCreatePlu)
	ON_COMMAND(IDM_LINK_PLU, OnLinkPlu)
	ON_COMMAND(IDM_SPLIT_KEY, OnSplitKey)
	ON_COMMAND(IDM_GROUP_KEY, OnGroupKey)
	ON_COMMAND(IDM_PICK_STYLE, OnCopyStyle)
	ON_COMMAND(IDM_PICK_IMAGE, OnCopyImage)
	ON_COMMAND(IDM_PICK_KEY, OnCopyKey)
	ON_COMMAND(IDM_TAG_KEY, OnTagKey)
	ON_COMMAND(IDM_UNTAG_KEY, OnUntagKey)
	ON_COMMAND(IDM_PASTE_KEY, OnPasteKey)
	ON_MESSAGE( WM_APP + 2, OnPluSortPromptMessage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

CSPOSKeyboardView::CSPOSKeyboardView()
{
	m_nEditSharpKeyPos = 0;
	m_nMenuSharpKeyPos = 0;
	m_bMenuVisible = FALSE;
	m_bMenuIsPluKey = FALSE;
	m_bMenuPluExists = FALSE;
	m_nMenuPluNo = 0;
	m_bSortPreviewMode = FALSE;
	m_nZoomType = 2;
	m_nNewPluDeptNo = 0;

	SPOSKeyboardManager.GetModelessPointers() -> SetView ( this );
}

/**********************************************************************/

CSPOSKeyboardView::~CSPOSKeyboardView()
{
}

/**********************************************************************/
#ifdef _DEBUG
/**********************************************************************/

void CSPOSKeyboardView::AssertValid() const
{
	CScrollView::AssertValid();
}

/**********************************************************************/

void CSPOSKeyboardView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

/**********************************************************************/
#endif 
/**********************************************************************/

BOOL CSPOSKeyboardView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

/**********************************************************************/

void CSPOSKeyboardView::InitialUpdate() 
{
	RefreshListCombo();
	RefreshPageCombo();
	SetKeyboardDialogTitle();
	
	CDC* pDC = GetDC();
	SPOSKeyboardManager.GetFontTable() -> BuildFontTable ( pDC );
	ReleaseDC ( pDC );

	switch( SPOSKeyboardManager.GetLayoutOptions() -> GetInitialZoomType() )
	{
	case 1:		ZoomSmall();	break;
	case 3:		ZoomBig();		break;
	default:	ZoomNormal();	break;
	}
}

/**********************************************************************/

void CSPOSKeyboardView::SetKeyboardDialogTitle()
{
	__int64 nListNo;
	int nScreenNo, nPageNo, nKeyCount;
	SPOSKeyboardManager.GetDlgViewLink() -> GetVisibleListDetails( nScreenNo, nListNo, nPageNo, nKeyCount);

	CString strZoom = "";
	switch( m_nZoomType )
	{
	case 1:	strZoom = " (75% actual size)";		break;
	case 3:	strZoom = " (200% actual size)";	break;
	}	

	CString strName = "";
	CString strTemp = SPOSKeyboardManager.GetLayoutOptions() -> GetKeyboardName();
	if ( strTemp != "" )
	{
		strName = " (";
		strName += strTemp;
		strName += ")";
	}

	CString strTitle;
	strTitle.Format ( "SPOS V4 Keyboard Editor%s%s",
		(const char*) strName,
		(const char*) strZoom );

	SPOSKeyboardManager.GetDlgViewLink() -> SetDialogTitle( strTitle );
}

/**********************************************************************/

void CSPOSKeyboardView::ZoomSmall() 
{
	m_nZoomType = 1;
	RebuildKeymap();
	SetKeyboardDialogTitle();
	Invalidate();
	UpdateWindow();
}

/**********************************************************************/

void CSPOSKeyboardView::ZoomNormal() 
{
	m_nZoomType = 2;
	RebuildKeymap();
	SetKeyboardDialogTitle();
	Invalidate();
	UpdateWindow();
}

/**********************************************************************/

void CSPOSKeyboardView::ZoomBig() 
{
	m_nZoomType = 3;
	RebuildKeymap();
	SetKeyboardDialogTitle();
	Invalidate();
	UpdateWindow();
}

/**********************************************************************/
//IMPORT FROM DATA TRANSFER WIZARD
/**********************************************************************/

void CSPOSKeyboardView::ImportWizardData() 
{
	CFileDialog dlgFile ( TRUE, "sql", "kbdv3.sql", OFN_NOCHANGEDIR|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, "SQL Import Files (*.sql)|*.sql||", NULL );
	dlgFile.m_ofn.lpstrTitle = "Import Data Transfer Wizard File";
	dlgFile.m_ofn.lpstrInitialDir = ".\\";
	
	if ( dlgFile.DoModal() == IDCANCEL )
		return;

	SPOSKeyboardManager.GetMultiKeySelectInfo() -> ClearSelection();

	CSPOSKeyboardSQLImporter SQLImporter;
	SQLImporter.ImportFile( dlgFile.GetPathName(), FALSE, TRUE, TRUE );

	if ( SQLImporter.GotFile() == TRUE )
	{
		CSPOSKeyboardSQLImportStatsDlg dlg ( SQLImporter, this );
		
		if ( dlg.DoModal() == IDOK )
		{
			bool bWantLayout = ( dlg.m_bWantLayout != 0 );
			bool bWantMacros = ( dlg.m_bWantMacros != 0 );
			bool bWantDelete = ( dlg.m_bDelete != 0 );
		
			if ( ( TRUE == bWantLayout ) && ( TRUE == bWantDelete ) )
			{
				SPOSKeyboardManager.GetListManager() -> Reset();
			}

			SQLImporter.ImportFile( dlgFile.GetPathName(), TRUE, bWantLayout, bWantMacros );

			if ( TRUE == bWantMacros )
			{
				if ( ( SQLImporter.GotMacros() == TRUE ) || ( TRUE == bWantDelete ) )
				{
					CDataManagerInfo infoDM;
					if ( DataManagerNonDb.OpenCCMacroKeyboard( DB_READWRITE, infoDM ) == TRUE )
					{
						if ( TRUE == bWantDelete )
						{
							DataManagerNonDb.CCMacroKeyboard.Reset();
						}

						SQLImporter.MergeMacroBufferKeyboard(FALSE);

						if ( DataManagerNonDb.WriteCCMacroKeyboard( infoDM ) == FALSE )
						{
							Prompter.WriteError( infoDM );
						}
						else
						{
							SPOSKeyboardManager.CreateActiveMacroList();
						}

						DataManagerNonDb.CloseCCMacroKeyboard( infoDM );
					}
				}
			}

			if ( TRUE == bWantLayout )
			{
				SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();

				RefreshListCombo();
				RefreshPageCombo();

				RebuildKeymap();
				SetKeyboardDialogTitle();
				Invalidate();
				UpdateWindow();
			}
		}
	}
	else
	{
		Prompter.Error( "Unable to open import file" );
	}
}

/**********************************************************************/
//IMPORT IMAGES
/**********************************************************************/

void CSPOSKeyboardView::ImportImages() 
{
	CSPOSKeyboardImageImportDlg dlg( this );
	
	if ( dlg.DoModal() == IDOK )
	{
		SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();

		RefreshListCombo();
		RefreshPageCombo();

		RebuildKeymap();
		SetKeyboardDialogTitle();
		Invalidate();
		UpdateWindow();
	}
}

/**********************************************************************/
//COMBO DISPLAY AND SELECTION
/**********************************************************************/

void CSPOSKeyboardView::RefreshListCombo()
{
	SPOSKeyboardManager.GetDlgViewLink() -> RefreshListCombo();
}

/**********************************************************************/

void CSPOSKeyboardView::SelectPage( int nPageNo )
{
	SPOSKeyboardManager.GetDlgViewLink() -> SetPageNo( nPageNo );
	RefreshPageCombo();
	RebuildKeymap();
	SetKeyboardDialogTitle();
	Invalidate();
	UpdateWindow();

}

/**********************************************************************/

void CSPOSKeyboardView::RefreshPageCombo()
{
	SPOSKeyboardManager.GetDlgViewLink() -> RefreshPageCombo();
}

/**********************************************************************/

void CSPOSKeyboardView::SelectViewMode() 
{
	CancelKeyGrouper();

	RefreshListCombo();
	RefreshPageCombo();

	RebuildKeymap();
	SetKeyboardDialogTitle();
	Invalidate();
	UpdateWindow();
}

/**********************************************************************/

void CSPOSKeyboardView::SelectList() 
{
	CancelKeyGrouper();

	RefreshPageCombo();

	RebuildKeymap();
	SetKeyboardDialogTitle();
	Invalidate();
	UpdateWindow();
}

/**********************************************************************/

void CSPOSKeyboardView::SelectPage() 
{
	CancelKeyGrouper();

	RebuildKeymap();
	SetKeyboardDialogTitle();
	Invalidate();
	UpdateWindow();	
}

/**********************************************************************/
//DIALOG BAR LIST BUTTONS
/**********************************************************************/

void CSPOSKeyboardView::ButtonAdd() 
{
	CancelKeyGrouper();

	__int64 nListNo;
	int nScreenNo, nPageNo, nKeyCount;
	GetVisibleListDetails( nScreenNo, nListNo, nPageNo, nKeyCount );

	switch( SPOSKeyboardManager.GetDlgViewLink() -> GetViewMode() )
	{
	case VIEWMODE_LIST_ITEM_STANDARD:		AddList( nListNo, LISTTYPE_ITEM_STANDARD );		break;
	case VIEWMODE_LIST_ITEM_LARGE:			AddList( nListNo, LISTTYPE_ITEM_LARGE );		break;
	case VIEWMODE_LIST_FUNCTION_STANDARD:	AddList( nListNo, LISTTYPE_FUNCTION_STANDARD );	break;
	case VIEWMODE_LIST_FUNCTION_LARGE:		AddList( nListNo, LISTTYPE_FUNCTION_LARGE );	break;
	case VIEWMODE_LIST_FUNCTION_BOTTOM:		AddList( nListNo, LISTTYPE_FUNCTION_BOTTOM );	break;
	case VIEWMODE_SCREEN:					AddScreen( nScreenNo );							break;
	}
}

/**********************************************************************/

void CSPOSKeyboardView::AddList( __int64 nListNo, int nListType )
{
	CSPOSKeyboardListAddDlg dlg ( nListNo, nListType, this );
		
	if ( dlg.m_nListNo == 0 )
	{
		Prompter.Error ( "All available list numbers are in use" );
		return;
	}

	if ( dlg.DoModal() == IDOK )
	{
		if ( dlg.m_bCopy == FALSE )
		{
			int nListIdx;
			SPOSKeyboardManager.GetListManager() -> FindOrInsertList( dlg.m_nListNo, nListIdx );

			CSPOSKeyboardListRecord ListRecord;
			SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );

			//FIDDLE - CAN WE HAVE MULTI PAGES FOR ALL LIST TYPES ?
			ListRecord.SetListType( nListType );
			ListRecord.SetDefaultListFormat();
			ListRecord.SetPageCount( 1 );
			
			SPOSKeyboardManager.GetListManager() -> SetListRecord( nListIdx, ListRecord );
			SPOSKeyboardManager.GetListManager() -> InitialiseKeysForList( nListIdx );

			SPOSKeyboardManager.GetDlgViewLink() -> SetListNo( dlg.m_nListNo );

			RefreshListCombo();
			RefreshPageCombo();

			RebuildKeymap();
			SetKeyboardDialogTitle();
			Invalidate();
			UpdateWindow();
				
			ButtonEdit();
			SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
		}
		else
		{
			if ( SPOSKeyboardManager.GetListManager() -> CopyList ( nListNo, dlg.m_nListNo ) == TRUE )
			{
				SPOSKeyboardManager.GetDlgViewLink() -> SetListNo( dlg.m_nListNo );
	
				RefreshListCombo();
				RefreshPageCombo();

				RebuildKeymap();
				SetKeyboardDialogTitle();
				Invalidate();
				UpdateWindow();
				
				SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
			}
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardView::AddScreen( int nScreenNo )
{
	CSPOSKeyboardScreenAddDlg dlg ( nScreenNo, this );
		
	if ( dlg.m_nScreenNo == 0 )
	{
		Prompter.Error ( "All available screen numbers are in use" );
		return;
	}

	if ( dlg.DoModal() == IDOK )
	{
		if ( dlg.m_bCopy == FALSE )
		{
			int nScreenIdx;
			SPOSKeyboardManager.GetListManager() -> FindOrInsertScreen( dlg.m_nScreenNo, nScreenIdx );

			CSPOSKeyboardListComboItem item;
			SPOSKeyboardManager.GetDlgViewLink() -> GetPreviousComboItem( item );
			item.m_nScreenNo = dlg.m_nScreenNo;
			item.m_nScreenListType = SCREENLIST_TYPE_TRADING_TOPLEVEL;
			item.m_nListNo = 0;
			item.m_nPageNo = 1;
			SPOSKeyboardManager.GetDlgViewLink() -> SetPreviousComboItem( item );
			
			RefreshListCombo();
			RefreshPageCombo();

			RebuildKeymap();
			SetKeyboardDialogTitle();
			Invalidate();
			UpdateWindow();
				
			ButtonEdit();
			SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
		}
		else
		{
			if ( SPOSKeyboardManager.GetListManager() -> CopyScreen ( nScreenNo, dlg.m_nScreenNo ) == TRUE )
			{
				CSPOSKeyboardListComboItem item;
				SPOSKeyboardManager.GetDlgViewLink() -> GetPreviousComboItem( item );
				item.m_nScreenNo = dlg.m_nScreenNo;
				item.m_nScreenListType = SCREENLIST_TYPE_TRADING_TOPLEVEL;
				item.m_nPageNo = 1;
				SPOSKeyboardManager.GetDlgViewLink() -> SetPreviousComboItem( item );
			
				RefreshListCombo();
				RefreshPageCombo();

				RebuildKeymap();
				SetKeyboardDialogTitle();
				Invalidate();
				UpdateWindow();
				
				SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
			}
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardView::ButtonEdit() 
{
	CancelKeyGrouper();

	__int64 nListNo;
	int nScreenNo, nPageNo, nKeyCount;
	GetVisibleListDetails( nScreenNo, nListNo, nPageNo, nKeyCount );

	switch( SPOSKeyboardManager.GetDlgViewLink() -> GetViewMode() )
	{
	case VIEWMODE_LIST_ITEM_STANDARD:
	case VIEWMODE_LIST_ITEM_LARGE:
	case VIEWMODE_LIST_FUNCTION_STANDARD:
	case VIEWMODE_LIST_FUNCTION_LARGE:
	case VIEWMODE_LIST_FUNCTION_BOTTOM:	
		EditList( nListNo );		
		break;
	
	case VIEWMODE_SCREEN:		
		EditScreen( nScreenNo );		
		break;
	}
}

/**********************************************************************/

void CSPOSKeyboardView::EditList( __int64 nListNo )
{
	int nListIdx;
	if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == FALSE )
		return;

	CSPOSKeyboardPropertySheetEditList propSheet( nListIdx, this );
	if ( propSheet.DoModal() != IDOK )
		return;

	if ( propSheet.CheckKeyCount() == FALSE )
		return;

	if ( nListNo == SPOSKeyboardManager.GetMultiKeySelectInfo() -> GetListNo() )
		SPOSKeyboardManager.GetMultiKeySelectInfo() -> ClearSelection();

	propSheet.SaveRecord();

	RefreshListCombo();
	RefreshPageCombo();

	RebuildKeymap();
	SetKeyboardDialogTitle();
	Invalidate();
	UpdateWindow();
				
	SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
}

/**********************************************************************/

void CSPOSKeyboardView::EditScreen( int nScreenNo )
{
	int nScreenIdx;
	if ( SPOSKeyboardManager.GetListManager() -> FindScreen( nScreenNo, nScreenIdx ) == FALSE )
		return;

	CSPOSKeyboardPropertySheetEditScreen propSheet( nScreenIdx, this );
	
	if ( propSheet.DoModal() == IDOK )
	{
		propSheet.SaveRecord();

		RefreshListCombo();
		RefreshPageCombo();

		RebuildKeymap();
		SetKeyboardDialogTitle();
		Invalidate();
		UpdateWindow();
				
		SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
	}
}

/**********************************************************************/

void CSPOSKeyboardView::ButtonPrintList() 
{
	CancelKeyGrouper();
	CSPOSKeyboardListPrintDlg dlg( this );
	dlg.DoModal();
}

/**********************************************************************/

void CSPOSKeyboardView::ButtonDelete() 
{
	CancelKeyGrouper();

	__int64 nListNo;
	int nScreenNo, nPageNo, nKeyCount;
	GetVisibleListDetails( nScreenNo, nListNo, nPageNo, nKeyCount );

	switch( SPOSKeyboardManager.GetDlgViewLink() -> GetViewMode() )
	{
	case VIEWMODE_LIST_ITEM_STANDARD:
	case VIEWMODE_LIST_ITEM_LARGE:
	case VIEWMODE_LIST_FUNCTION_STANDARD:
	case VIEWMODE_LIST_FUNCTION_LARGE:	
	case VIEWMODE_LIST_FUNCTION_BOTTOM:
		DeleteList( nListNo );		
		break;

	case VIEWMODE_SCREEN:		
		DeleteScreen( nScreenNo );	
		break;
	}
}

/**********************************************************************/

void CSPOSKeyboardView::DeleteList( __int64 nListNo )
{
	int nListIdx;
	if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == FALSE )
		return;

	CSPOSKeyboardKeyHelpers KeyHelpers;
	CString strCombiText = KeyHelpers.GetCombiText( KEY_TYPE_LIST, nListNo );

	CString strMsg = "Are you sure you wish to delete list\n\n";
	strMsg += strCombiText;

	if ( Prompter.YesNo( strMsg, FALSE ) != IDYES )
		return;

	if ( nListNo == SPOSKeyboardManager.GetMultiKeySelectInfo() -> GetListNo() )
		SPOSKeyboardManager.GetMultiKeySelectInfo() -> ClearSelection();

	CSPOSKeyboardListComboItem item; 
	SPOSKeyboardManager.GetDlgViewLink() -> GetPreviousComboItem( item );
	
	item.m_nPageNo = 0;

	if ( nListIdx < SPOSKeyboardManager.GetListManager() -> GetListCount() - 1 )
	{
		CSPOSKeyboardListRecord nextRecord;
		SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx + 1, nextRecord );
		item.m_nListNo = nextRecord.GetListNo();
	}
	else if ( nListIdx > 0 )
	{
		CSPOSKeyboardListRecord prevRecord;
		SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx - 1, prevRecord );
		item.m_nListNo = prevRecord.GetListNo();
	}
	else
		item.m_nListNo = 0;

	SPOSKeyboardManager.GetListManager() -> DeleteList( nListIdx );

	SPOSKeyboardManager.GetDlgViewLink() -> SetPreviousComboItem( item );
	
	RefreshListCombo();
	RefreshPageCombo();

	RebuildKeymap();
	SetKeyboardDialogTitle();
	Invalidate();
	UpdateWindow();
				
	SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
}

/**********************************************************************/

void CSPOSKeyboardView::DeleteScreen( int nScreenNo )
{
	int nScreenIdx;
	if ( SPOSKeyboardManager.GetListManager() -> FindScreen( nScreenNo, nScreenIdx ) == FALSE )
		return;

	CSPOSKeyboardScreenRecord ScreenRecord;
	SPOSKeyboardManager.GetListManager() -> GetScreenRecord( nScreenIdx, ScreenRecord );

	CString strMsg;
	strMsg.Format ( "Are you sure you wish to delete screen\n\n%s", ScreenRecord.GetComboText() );

	if ( Prompter.YesNo( strMsg, FALSE ) != IDYES )
		return;

	CSPOSKeyboardListComboItem item;
	SPOSKeyboardManager.GetDlgViewLink() -> GetPreviousComboItem( item );

	item.m_nPageNo = 0;

	if ( nScreenIdx < SPOSKeyboardManager.GetListManager() -> GetScreenCount() - 1 )
	{
		CSPOSKeyboardScreenRecord nextRecord;
		SPOSKeyboardManager.GetListManager() -> GetScreenRecord( nScreenIdx + 1, nextRecord );
		item.m_nScreenNo = nextRecord.GetScreenNo();
	}
	else if ( nScreenIdx > 0 )
	{
		CSPOSKeyboardScreenRecord prevRecord;
		SPOSKeyboardManager.GetListManager() -> GetScreenRecord( nScreenIdx - 1, prevRecord );
		item.m_nScreenNo = prevRecord.GetScreenNo();
	}
	else
		item.m_nScreenNo = 0;

	SPOSKeyboardManager.GetListManager() -> DeleteScreen( nScreenIdx );

	SPOSKeyboardManager.GetDlgViewLink() -> SetPreviousComboItem( item );

	RefreshListCombo();
	RefreshPageCombo();

	RebuildKeymap();
	SetKeyboardDialogTitle();
	Invalidate();
	UpdateWindow();
				
	SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
}

/**********************************************************************/
//DIALOG BAR LIST BUTTONS
/**********************************************************************/

void CSPOSKeyboardView::ButtonAddPage() 
{
	CancelKeyGrouper();

	__int64 nListNo;
	int nScreenNo, nPageNo, nKeyCount;
	GetVisibleListDetails( nScreenNo, nListNo, nPageNo, nKeyCount );

	int nListIdx;
	if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == FALSE )
		return;

	CSPOSKeyboardListRecord ListRecord;
	SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );
	
	int nPageCount = ListRecord.GetPageCount();
	if ( nPageNo < 1 || nPageNo > nPageCount )
		return;

	int nMaxNewPages = ListRecord.GetMaxNewPages( nPageCount );
	
	if ( nMaxNewPages == -1 )
	{
		Prompter.Error( ListRecord.GetPageError(1, nPageCount ) );
		return;
	}

	if ( nMaxNewPages == -2 )
	{
		Prompter.Error( ListRecord.GetPageError(2, nPageCount ) );
		return;
	}
	
	CSPOSKeyboardPageAddDlg dlg( nPageNo, nMaxNewPages, this );
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	if (nListNo == SPOSKeyboardManager.GetMultiKeySelectInfo()->GetListNo())
	{
		SPOSKeyboardManager.GetMultiKeySelectInfo()->ClearSelection();
	}

	CWordArray arrayNewPages;
	for (int n = 1; n <= nPageCount; n++)
	{
		arrayNewPages.Add(n);
	}

	int nNewPageToInsert = 0;
	if (dlg.m_bCopyVisible == TRUE)
	{
		nNewPageToInsert = nPageNo;
	}

	int nInsertPos = 0;
	switch( dlg.m_nMethod )
	{
	case ADD_PAGES_END:		
		nInsertPos = nPageCount;	
		break;
	
	case ADD_PAGES_START:
		{
			nInsertPos = 0;
			
			CSPOSKeyboardListComboItem item;
			SPOSKeyboardManager.GetDlgViewLink() -> GetPreviousComboItem( item );
			item.m_nPageNo += dlg.m_nPagesToAdd;
			SPOSKeyboardManager.GetDlgViewLink() -> SetPreviousComboItem( item );
		}
		break;
	
	case ADD_PAGES_MIDDLE:	
		nInsertPos = nPageNo;	
		break;
	}

	arrayNewPages.InsertAt( nInsertPos, nNewPageToInsert, dlg.m_nPagesToAdd );

	SPOSKeyboardManager.GetListManager() -> MovePages( nListNo, arrayNewPages );
	SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
	
	RefreshPageCombo();

	RebuildKeymap();
	SetKeyboardDialogTitle();
	Invalidate();
	UpdateWindow();
}

/**********************************************************************/

void CSPOSKeyboardView::ButtonEditPage() 
{
	CancelKeyGrouper();

	__int64 nListNo;
	int nScreenNo, nPageNo, nKeyCount;
	GetVisibleListDetails( nScreenNo, nListNo, nPageNo, nKeyCount );

	int nListIdx;
	if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == FALSE )
		return;

	CSPOSKeyboardListRecord ListRecord;
	SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );
	
	int nPageCount = ListRecord.GetPageCount();
	if ( nPageNo < 1 || nPageNo > nPageCount )
		return;

	CWordArray arrayPageTypes;
	CSPOSKeyboardPageListDlg dlg( nListIdx, nPageNo, arrayPageTypes, this );
	dlg.SetView( this );
	if ( dlg.DoModal() != IDOK )
		return;	

	if ( nListNo == SPOSKeyboardManager.GetMultiKeySelectInfo() -> GetListNo() )
		SPOSKeyboardManager.GetMultiKeySelectInfo() -> ClearSelection();

	SPOSKeyboardManager.GetListManager() -> MovePages( nListNo, arrayPageTypes );
	
	SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
	
	RefreshPageCombo();

	RebuildKeymap();
	SetKeyboardDialogTitle();
	Invalidate();
	UpdateWindow();
}

/**********************************************************************/

void CSPOSKeyboardView::ButtonDeletePage() 
{
	CancelKeyGrouper();

	__int64 nListNo;
	int nScreenNo, nPageNo, nKeyCount;
	GetVisibleListDetails( nScreenNo, nListNo, nPageNo, nKeyCount );

	int nListIdx;
	if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == FALSE )
		return;

	CSPOSKeyboardListRecord ListRecord;
	SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );
	
	int nPageCount = ListRecord.GetPageCount();
	if ( nPageNo < 1 || nPageNo > nPageCount )
		return;
	
	if ( nPageCount < 2 )
	{
		Prompter.Error ( "You must leave at least one page in this list." );
		return;
	}

	CString strMsg;
	strMsg.Format ( "Are you sure you wish to delete this page\n\nList %I64d, Page %d",
			nListNo,
			nPageNo );

	if ( Prompter.YesNo( strMsg, FALSE ) != IDYES )
		return;

	if ( nListNo == SPOSKeyboardManager.GetMultiKeySelectInfo() -> GetListNo() )
		SPOSKeyboardManager.GetMultiKeySelectInfo() -> ClearSelection();

	CWordArray arrayNewPages;
	for ( int n = 1; n <= nPageCount; n++ )
		arrayNewPages.Add(n);

	if ( nPageNo == nPageCount )
	{
		CSPOSKeyboardListComboItem item;
		SPOSKeyboardManager.GetDlgViewLink() -> GetPreviousComboItem( item );
		item.m_nPageNo = nPageCount - 1;
		SPOSKeyboardManager.GetDlgViewLink() -> SetPreviousComboItem( item );
	}

	arrayNewPages.RemoveAt( nPageNo - 1 );
	SPOSKeyboardManager.GetListManager() -> MovePages( nListNo, arrayNewPages );
	
	SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
	
	RefreshPageCombo();

	RebuildKeymap();
	SetKeyboardDialogTitle();
	Invalidate();
	UpdateWindow();
}

/**********************************************************************/

void CSPOSKeyboardView::PluSort() 
{
	CancelKeyGrouper();
	RebuildKeymap();
	
	__int64 nListNo;
	int nScreenNo, nPageNo, nKeyCount;
	GetVisibleListDetails( nScreenNo, nListNo, nPageNo, nKeyCount );

	int nListIdx;
	if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == FALSE )
		return;

	if ( nListNo == SPOSKeyboardManager.GetMultiKeySelectInfo() -> GetListNo() )
		SPOSKeyboardManager.GetMultiKeySelectInfo() -> ClearSelection();

	CSPOSKeyboardListRecord ListRecord;
	SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );
	
	int nStartPage = 1;
	int nEndPage = ListRecord.GetPageCount();

	bool bOnePageFromList = FALSE;
	bool bMultiPageList = FALSE;

	if ( nEndPage != 1 )
	{
		bMultiPageList = TRUE;

		CSPOSKeyboardPluSortOptionDlg dlgOption;
		if ( dlgOption.DoModal() == IDCANCEL )
			return;

		if ( dlgOption.GetAllFlag() == FALSE )
		{
			nStartPage = nPageNo;
			nEndPage = nPageNo;
			bOnePageFromList = TRUE;
		}
	}

	SPOSKeyboardManager.GetListManager() -> RememberKeysForList( nListNo );

	CArray<int,int> arrayPluKeyIdx;
	CArray<CSPOSKeyboardKeyRecord,CSPOSKeyboardKeyRecord> arrayKeyBuffer;
	CReportConsolidationArray<CSortedIntByString> mapDescriptionToKeyBuffer( TRUE );

	for ( int nPageNo = nStartPage; nPageNo <= nEndPage; nPageNo++ )
	{
		int nKeyIdxStart;
		SPOSKeyboardManager.GetListManager() -> FindKey( nListNo, nPageNo, 0, nKeyIdxStart );

		CArray<CSPOSKeyboardKeyExtent,CSPOSKeyboardKeyExtent> KeyExtentMap;
		SPOSKeyboardManager.GetListManager() -> GetKeyExtentMap( nListNo, nPageNo, KeyExtentMap );

		for ( int nPos = 0; nPos < KeyExtentMap.GetSize(); nPos++ )
		{
			CSPOSKeyboardKeyExtent KeyExtent = KeyExtentMap.GetAt( nPos );

			int nKeyIdxToTry = nKeyIdxStart + KeyExtent.m_nKeyNo - 1;

			CSPOSKeyboardKeyRecord KeyRecord;
			if ( ( nKeyIdxToTry >= 0 ) && ( nKeyIdxToTry < SPOSKeyboardManager.GetListManager() -> GetKeyCount() ) )
			{
				SPOSKeyboardManager.GetListManager() -> GetKeyRecord( nKeyIdxToTry, KeyRecord );

				if ( ( KeyRecord.GetType() == KEY_TYPE_PLUNUMBER ) && ( KeyRecord.GetGJANoSortFlag() == FALSE ) ) 
				{
					CStringArray Texts;
					
					if ( KeyRecord.GetDefaultTextAlwaysFlag() == FALSE )
					{
						KeyRecord.GetDisplayTexts( Texts, TRUE );
					}
					else
					{
						CString strText;

						CSPOSKeyboardKeyHelpers KeyHelpers;
						KeyHelpers.GetDefaultText( KeyRecord.GetType(), KeyRecord.GetValue(), strText, TRUE );

						CSPOSKeyboardKeyRecord TempKeyRecord;
						TempKeyRecord.SetKeyText( strText );
						TempKeyRecord.GetDisplayTexts( Texts, TRUE );
					}

					CString strText = "";
					for ( int n = 0; n < Texts.GetSize(); n++ )
						strText += Texts.GetAt(n);

					arrayPluKeyIdx.Add( nKeyIdxToTry );
					arrayKeyBuffer.Add( KeyRecord );

					CSortedIntByString item;
					item.m_strItem = strText;
					item.m_nItem = arrayKeyBuffer.GetSize() - 1;
					mapDescriptionToKeyBuffer.ConsolidateWithHint( item, 1 );
				}
			}
		}
	}

	if ( mapDescriptionToKeyBuffer.GetSize() == arrayPluKeyIdx.GetSize() )
	{
		for ( int n = 0; n < arrayPluKeyIdx.GetSize(); n++ )
		{
			CSPOSKeyboardKeyRecord KeyRecordArray;
			int nPluKeyIdx = arrayPluKeyIdx.GetAt(n);
			SPOSKeyboardManager.GetListManager() -> GetKeyRecord( nPluKeyIdx, KeyRecordArray );

			CSortedIntByString item;
			mapDescriptionToKeyBuffer.GetAt( n, item );
			int nKeyBufferIdx = item.m_nItem;
			
			CSPOSKeyboardKeyRecord KeyRecordSorted = arrayKeyBuffer.GetAt( nKeyBufferIdx );

			KeyRecordArray.SetValue( KeyRecordSorted.GetValue() );
			KeyRecordArray.SetDefaultTextAlwaysFlag( KeyRecordSorted.GetDefaultTextAlwaysFlag() );
			KeyRecordArray.SetBackground( KeyRecordSorted.GetBackground() );
			KeyRecordArray.SetForeground( KeyRecordSorted.GetForeground() );
			KeyRecordArray.SetBoldFlag( KeyRecordSorted.GetBoldFlag() );
			KeyRecordArray.SetFontSize( KeyRecordSorted.GetFontSize() );
			KeyRecordArray.SetAlignmentH( KeyRecordSorted.GetAlignmentH() );
			KeyRecordArray.SetAlignmentV( KeyRecordSorted.GetAlignmentV() );
			KeyRecordArray.SetKeyText( KeyRecordSorted.GetKeyText() );
			KeyRecordArray.SetHideTextFlag( KeyRecordSorted.GetHideTextFlag() );
			KeyRecordArray.SetImageFilename( KeyRecordSorted.GetImageFilename() );
			KeyRecordArray.SetImageScale( KeyRecordSorted.GetImageScale() );

			SPOSKeyboardManager.GetListManager() -> SetKeyRecord( nPluKeyIdx, KeyRecordArray );
			SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
		}
	}

	m_bSortPreviewMode = TRUE;
	Invalidate();
	UpdateWindow();

	CSPOSKeyboardPluSortPromptDlg dlg( bOnePageFromList, bMultiPageList, this );
	if ( dlg.DoModal() != IDOK )
		SPOSKeyboardManager.GetListManager() -> RestoreKeysForList( nListNo );
		
	m_bSortPreviewMode = FALSE;
	Invalidate();	
	UpdateWindow();
}

/**********************************************************************/

long CSPOSKeyboardView::OnPluSortPromptMessage( WPARAM wIndex, LPARAM lParam )
{
	__int64 nListNo;
	int nScreenNo, nPageNo, nKeyCount;
	GetVisibleListDetails( nScreenNo, nListNo, nPageNo, nKeyCount );

	int nListIdx;
	if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == TRUE )
	{
		CSPOSKeyboardListRecord ListRecord;
		SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );
		int nPageCount = ListRecord.GetPageCount();

		if ( wIndex == 1 )
		{
			nPageNo++;
			if ( nPageNo > nPageCount )
				nPageNo++;
		}
		else
		{
			nPageNo--;
			if ( nPageNo < 1 )
				nPageNo = nPageCount;
		}

		SelectPage( nPageNo );
	}
	
	return 0l;
}

/**********************************************************************/
//SCREEN DISPLAY AND PRINTING
/**********************************************************************/

void CSPOSKeyboardView::OnDraw(CDC* pDC)
{
	__int64 nListNo;
	int nScreenNo, nPageNo, nKeyCount;
	SPOSKeyboardManager.GetDlgViewLink() -> GetVisibleListDetails( nScreenNo, nListNo, nPageNo, nKeyCount);
	SPOSKeyboardManager.GetKeymapTable() -> BuildKeymap( nListNo, nPageNo, m_nZoomType );

	CSize size = GetTotalSize();
	
	CRect rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = size.cx;
	rect.bottom = size.cy;
	pDC->FillSolidRect(&rect,RGB(90,101,140));
	
	CreateScreen ( pDC, nListNo, nPageNo );
}

/**********************************************************************/

void CSPOSKeyboardView::CreateScreen(CDC* pDC, __int64 nListNo, int nPageNo )
{
	UINT bBrushBits[8];
	for ( int n = 0; n < 8; n++ )
		bBrushBits[n] = 240;

	HBITMAP hbm = CreateBitmap( 8, 8, 1, 1, (LPBYTE)bBrushBits);

	bool bSelectionPage = SPOSKeyboardManager.GetMultiKeySelectInfo() -> CheckPage( nListNo, nPageNo );

	int nKeyIdx;
	SPOSKeyboardManager.GetListManager() -> FindKey( nListNo, nPageNo, 0, nKeyIdx );

	int nListIdx;
	int nListFormat = LIST_FORMAT_3x3;
	if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == TRUE )
	{
		CSPOSKeyboardListRecord ListRecord;
		SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );
		nListFormat = ListRecord.GetFormat();
	}
	
	for ( int nPos = 0; nPos < SPOSKeyboardManager.GetKeymapTable() -> GetKeyCount(); nPos++ )
	{
		CSPOSKeyboardKeyExtent KeyExtent;
		SPOSKeyboardManager.GetKeymapTable() -> GetKeyInfoByIndex( nPos, KeyExtent );

		CRect rectKey = KeyExtent.m_rectDisplay;
		
		CRect rectText = rectKey;
		rectText.top += 5;
		rectText.bottom -= 5;
		rectText.left += 5;
		rectText.right -= 5;

		int nKeyIdxToTry = nKeyIdx + KeyExtent.m_nKeyNo - 1;

		CSPOSKeyboardKeyRecord KeyRecord;
		if ( nKeyIdxToTry >= 0 && nKeyIdxToTry < SPOSKeyboardManager.GetListManager() -> GetKeyCount() )
			SPOSKeyboardManager.GetListManager() -> GetKeyRecord( nKeyIdxToTry, KeyRecord );
		
		int nKeyColour = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB( KeyRecord.GetBackground() );
		int nTextColour = SPOSKeyboardManager.GetColourTable() -> GetCurrentRGB( KeyRecord.GetForeground() );
		int nCursorColour = SPOSKeyboardManager.GetColourTable() -> GetCursorColour( nKeyColour );

		CStringArray Texts;

		if ( KeyRecord.GetDefaultTextAlwaysFlag() == FALSE )
		{
			KeyRecord.GetDisplayTexts( Texts, FALSE );
		}
		else
		{
			CString strText;

			CSPOSKeyboardKeyHelpers KeyHelpers;
			KeyHelpers.GetDefaultText( KeyRecord.GetType(), KeyRecord.GetValue(), strText, TRUE );

			CSPOSKeyboardKeyRecord TempKeyRecord;
			TempKeyRecord.SetType( KeyRecord.GetType() );
			TempKeyRecord.SetValue( KeyRecord.GetValue() );
			TempKeyRecord.SetKeyText( strText );
			TempKeyRecord.GetDisplayTexts( Texts, FALSE );
		}

		bool bShowKey = ( ( nKeyColour != nTextColour ) || ( SPOSKeyboardManager.GetDlgViewLink() -> GetInvisibleTick() == TRUE ) );
			
		if ( bShowKey == TRUE )
		{	
			m_KeyBrush.CreateSolidBrush ( COLORREF ( nKeyColour ) );
			pDC -> FillRect ( rectKey, &m_KeyBrush );
			m_KeyBrush.DeleteObject();
		
			if ( SPOSKeyboardManager.GetDlgViewLink() -> GetImagesTick() == TRUE )
				ShowPicture( pDC, rectKey, KeyRecord );
		
			if ( KeyRecord.GetHideTextFlag() == FALSE )
				DrawViewKeyText ( pDC, Texts, rectText, KeyRecord, nTextColour ); 
		}

		bool bHighlight = FALSE;
		if ( FALSE == m_bSortPreviewMode )
			bHighlight = ( KeyExtent.m_nKeyNo == m_nEditSharpKeyPos );
		else
			bHighlight = KeyRecord.GetGJANoSortFlag();

		if ( TRUE == bHighlight )
		{
			m_KeyBrush.CreateSolidBrush ( nCursorColour );
			
			CRgn region;
			region.CreateRectRgnIndirect ( &rectKey );
			pDC -> FrameRgn ( &region, &m_KeyBrush, 5, 5 );
			m_KeyBrush.DeleteObject();
		}
		else if ( ( TRUE == bSelectionPage ) && ( SPOSKeyboardManager.GetMultiKeySelectInfo() -> IsSelected( KeyExtent.m_nColStart, KeyExtent.m_nRowStart ) ) )
		{
			m_KeyBrush.CreatePatternBrush( CBitmap::FromHandle(hbm) );
			
			CRgn region;
			region.CreateRectRgnIndirect ( &rectKey );
			pDC -> FrameRgn ( &region, &m_KeyBrush, 8, 8 );
			m_KeyBrush.DeleteObject();
		}
	}

	DeleteObject( hbm );
}

/**********************************************************************/

void CSPOSKeyboardView::ShowPicture( CDC* pDC, CRect& rect, CSPOSKeyboardKeyRecord& KeyRecord )
{
	CString strFilename = KeyRecord.GetImageFilename();
	if ( strFilename == "" )
		return;

	int nScale = KeyRecord.GetImageScale();
	if ( nScale < 10 || nScale > 100 )
		return;

	strFilename.MakeUpper();

	CString strImage = SPOSKeyboardManager.GetLayoutOptions() -> GetImageFolder();
	strImage += "\\";
	strImage += strFilename;

	CPicture picture;

	if ( picture.Load( strImage ) == FALSE )
		return;

	CRect rectImage = rect;

	if ( nScale != 100 )
	{
		int nMarginPercent = ( 100 - nScale ) / 2;
		
		int nWidth = rectImage.right - rectImage.left;
		int nHMargin = ( nWidth * nMarginPercent ) / 100;

		int nHeight = rectImage.bottom - rectImage.top;
		int nVMargin = ( nHeight * nMarginPercent ) / 100;

		rectImage.top += nVMargin;
		rectImage.bottom -= nVMargin;
		rectImage.right -= nHMargin;
		rectImage.left += nHMargin;
	}
							
	if ( ( rectImage.right > rectImage.left ) && ( rectImage.bottom > rectImage.top ) )
		picture.Render(pDC,rectImage);
}

/**********************************************************************/

void CSPOSKeyboardView::DrawViewKeyText ( CDC* pDC, CStringArray& Texts, CRect rect, CSPOSKeyboardKeyRecord& KeyRecord, int nTextColour )
{
	int nFontSize = KeyRecord.GetFontSize();
	bool bBold = KeyRecord.GetBoldFlag();
	int nHAlign = KeyRecord.GetAlignmentH();
	int nVAlign = KeyRecord.GetAlignmentV();
 
	//FIRST ELEMENT IS MINI PLU NO THEN UP TO THREE LINES OF KEY TEXT
	int nLines = Texts.GetSize() - 1;
	if ( nLines < 0 )
		return;

	pDC -> SetBkMode ( TRANSPARENT );
	pDC -> SetTextColor ( COLORREF ( nTextColour ) );

	CFont ZoomedFont;
	double dZoomRatio = SPOSKeyboardManager.GetKeymapTable() -> GetZoomFactor();
	SPOSKeyboardManager.GetFontTable() -> GetKeyDisplayFont( pDC, &ZoomedFont, nFontSize, bBold, dZoomRatio );
	
	UINT nOldAlign = pDC-> SetTextAlign ( TA_CENTER );
	CFont* pOldFont = pDC -> SelectObject ( &ZoomedFont );

	TEXTMETRIC tm;
	pDC -> GetTextMetrics ( &tm );

	//SPLIT LINES THAT ARE TOO WIDE
	CStringArray SplitTexts;
	SplitTexts.Add( Texts.GetAt(0) );

	for ( int n = 1; n < Texts.GetSize(); n++ )
	{
		CStringArray arrayTokens;
		CString strText = Texts.GetAt(n);
		::TokeniseKeyTextLine( strText, arrayTokens );

		CString strBuffer = "";
		
		for ( int i = 0; i < arrayTokens.GetSize(); i++ )
		{
			CString strToken = arrayTokens.GetAt(i);

			CRect rectTemp = rect;
			pDC -> DrawText( strBuffer + strToken, strBuffer.GetLength() + strToken.GetLength(), &rectTemp, DT_CALCRECT );
		
			if ( rectTemp.Width() > rect.Width() )
			{
				if ( strBuffer != "" )
				{
					SplitTexts.Add( strBuffer );
					strBuffer = strToken;
				}
				else
					SplitTexts.Add( strToken );
			}
			else
				strBuffer += strToken;
		}

		if ( strBuffer != "" )
			SplitTexts.Add( strBuffer );
	}

	nLines = SplitTexts.GetSize() - 1;

	int nLineHeight = tm.tmHeight + tm.tmExternalLeading;
	int nLineSpace = ( nLineHeight * nLines ) - tm.tmExternalLeading;
	int nHeight = rect.Height();

	int nYStart;
	switch( nVAlign )
	{
	case 0:	//TOP ALIGN
		nYStart = rect.top;
		break;

	case 2:	//BOTTOM ALIGN
		nYStart = rect.bottom - nLineSpace;
		break;

	case 1:
	default:	//CENTRE ALIGN
		nYStart = rect.top + ( ( rect.Height() - nLineSpace ) / 2 );
		break;
	}

	int nXPos;

	switch( nHAlign )
	{
	case 0:
		pDC -> SetTextAlign( TA_LEFT );
		nXPos = rect.left;
		break;

	case 2:
		pDC -> SetTextAlign( TA_RIGHT );
		nXPos = rect.right;
		break;

	case 1:
	default:
		pDC -> SetTextAlign( TA_CENTER );
		nXPos = rect.left + ( rect.Width() / 2 );
		break;
	}
	
	for ( int nToken = 0; nToken < nLines; nToken++ )
	{			
		CString strToken = SplitTexts.GetAt ( nToken + 1 );
	
		if ( strToken != "" )
			pDC -> ExtTextOut ( nXPos, nYStart, ETO_CLIPPED, rect, strToken, strToken.GetLength(), NULL );
			
		nYStart += nLineHeight;
	}
	
	CString strPluNoText = SplitTexts.GetAt(0);

	if ( strPluNoText != "" )
	{
		CFont TinyFont;
		SPOSKeyboardManager.GetFontTable() -> GetKeyDisplayFont( pDC, &TinyFont, 60, FALSE, 1.0 );
	
		pDC -> SelectObject ( &TinyFont );
		pDC -> GetTextMetrics ( &tm );
		nLineHeight = tm.tmHeight + tm.tmExternalLeading;
		pDC -> SetTextAlign ( TA_LEFT );
		CSize s = pDC -> GetTextExtent ( strPluNoText, strPluNoText.GetLength() );
		pDC -> ExtTextOut ( rect.right - s.cx, rect.bottom - nLineHeight, ETO_CLIPPED, rect, strPluNoText, strPluNoText.GetLength(), NULL );
	}
	
	pDC -> SetTextAlign ( nOldAlign );
	pDC -> SelectObject ( pOldFont );
}

/**********************************************************************/
//FILE MENU
/**********************************************************************/

void CSPOSKeyboardView::SaveKeyboard() 
{
	if ( SPOSKeyboardManager.GetListManager() -> IsModified() == FALSE )
	{
		Prompter.Error ( "You have not made any changes since the\nkeyboard files were previously saved." );
		return;
	}

	if ( Prompter.YesNo ( "Are you sure you wish to save the changes\nfrom the current editing session." ) != IDYES )
	{
		Prompter.Info ( "The save has been cancelled." );
		return;
	}

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
	SPOSKeyboardManager.GetListManager() -> ClearModifiedFlag();

	Prompter.Info ( "The keyboard files have been saved." );
}

/**********************************************************************/
//KEY LIST MENU
/**********************************************************************/

void CSPOSKeyboardView::EditKeyTypes() 
{
	SPOSKeyboardManager.GetModelessPointers() -> OpenColourDropper( COLOURDROPPER_LISTKEY );
}

/**********************************************************************/
//MACRO MENU
/**********************************************************************/

void CSPOSKeyboardView::EditMacros() 
{
	CDataManagerInfo infoDM;
	if ( DataManagerNonDb.OpenCCMacroKeyboard( DB_READWRITE, infoDM ) == TRUE )
	{
		SPOSKeyboardManager.MergeParentMacros();

		CLocationSelectorEntity LocSelMacroEdit;
		LocSelMacroEdit.SetKeyboardMapNode(0);
		CListDataMacroDlg dlg( LocSelMacroEdit, this );

		if ( dlg.DoModal() == IDOK )
		{
			if ( DataManagerNonDb.WriteCCMacroKeyboard( infoDM ) == FALSE )
			{
				Prompter.WriteError( infoDM );
			}
			else
			{
				SPOSKeyboardManager.CreateActiveMacroList();
			}
		}

		DataManagerNonDb.CloseCCMacroKeyboard( infoDM );
				
	}
	else
	{
		Prompter.ShareError( infoDM );
	}
}

/**********************************************************************/
//GLOBAL SETTINGS MENU
/**********************************************************************/

void CSPOSKeyboardView::GlobalSettings() 
{
	__int64 nListNo;
	int nScreenNo, nPageNo, nKeyCount;
	GetVisibleListDetails( nScreenNo, nListNo, nPageNo, nKeyCount );

	SPOSKeyboardManager.GetColourTable() -> RememberUserColours();
	CSPOSKeyboardColourGlobalDlg dlg ( nListNo, this );
	dlg.DoModal();

	if ( SPOSKeyboardManager.GetColourTable() -> CompareUserColours() == TRUE )
		SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
}

/**********************************************************************/
//OPTIONS MENU
/**********************************************************************/

void CSPOSKeyboardView::SetOptions() 
{
	CSPOSKeyboardOptionsDlg dlg ( this );
	if ( dlg.DoModal() == IDOK )
	{
		SPOSKeyboardManager.GetLayoutOptions() -> Write();
		RebuildKeymap();
		Invalidate();
		UpdateWindow();
	}
}

/**********************************************************************/

void CSPOSKeyboardView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if ( SPOSKeyboardManager.GetModelessPointers() -> IsColourDropperActive() )
		DropKey ( point );
	
	CScrollView::OnLButtonDown(nFlags, point);
}

/**********************************************************************/

void CSPOSKeyboardView::RedrawKey( int nSharpKeyPos )
{
	CSPOSKeyboardKeyExtent KeyExtent;
	SPOSKeyboardManager.GetKeymapTable() -> GetKeyInfoBySharpKeyPos( nSharpKeyPos, KeyExtent );
	CRect rect = KeyExtent.m_rectDisplay;

	rect.left -= 10;
	rect.right += 10;
	rect.top -= 10;
	rect.bottom += 10;
		
	CDC* pDC = GetDC();
	pDC -> SetMapMode ( MM_TEXT );
	pDC -> LPtoDP ( &rect );
	ReleaseDC ( pDC );

	rect -= GetScrollPosition();
		
	InvalidateRect( rect );
	UpdateWindow();
}

/**********************************************************************/

void CSPOSKeyboardView::HandleResize() 
{
	RebuildKeymap();
}

/**********************************************************************/

void CSPOSKeyboardView::RebuildKeymap()
{
	__int64 nListNo;
	int nScreenNo, nPageNo, nKeyCount;
	SPOSKeyboardManager.GetDlgViewLink() -> GetVisibleListDetails( nScreenNo, nListNo, nPageNo, nKeyCount);

	SPOSKeyboardManager.GetKeymapTable() -> Invalidate();
	SPOSKeyboardManager.GetKeymapTable() -> BuildKeymap( nListNo, nPageNo, m_nZoomType );	

	CSize size;
	size.cx = 40;
	size.cy = 40;
	size.cx += SPOSKeyboardManager.GetLayoutOptions() -> GetXMarginDisplay();
	size.cx += SPOSKeyboardManager.GetKeymapTable() -> GetListWidth() + 100;
	size.cy += SPOSKeyboardManager.GetKeymapTable() -> GetListHeight() + 100;

	switch ( m_nZoomType )
	{
	case 3:
		size.cx += SPOSKeyboardManager.GetKeymapTable() -> GetListWidth();
		size.cy += SPOSKeyboardManager.GetKeymapTable() -> GetListHeight();
		break;

	case 1:
		size.cx -= ( SPOSKeyboardManager.GetKeymapTable() -> GetListWidth() / 4 );
		size.cy -= ( SPOSKeyboardManager.GetKeymapTable() -> GetListHeight() / 4 );
		break;
	}

	if ( SPOSKeyboardManager.GetKeymapTable() -> GetKeyCount() == 0 )
	{
		size.cx = 0;
		size.cy = 0;
	}

	SetScrollSizes( MM_TEXT, size );
}

/**********************************************************************/

void CSPOSKeyboardView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if ( m_bMenuVisible == FALSE )
	{
		CPoint testPoint = point;

		CDC* pDC = GetDC();
		pDC -> SetMapMode ( MM_TEXT );
		pDC -> DPtoLP ( &testPoint );
		ReleaseDC ( pDC );

		testPoint += GetScrollPosition();

		m_nMenuSharpKeyPos = SPOSKeyboardManager.GetKeymapTable() -> GetKeyAtPoint ( testPoint );
		m_bMenuIsPluKey = FALSE;
		m_bMenuPluExists = FALSE;
		m_nMenuPluNo = 0;
	
		if ( m_nMenuSharpKeyPos != 0 )
		{
			ClientToScreen ( &point );
			CMenu PopUp;
			PopUp.LoadMenu ( IDR_SPOS_KEYBOARD_CONTEXT );
			CMenu* pSubMenu = PopUp.GetSubMenu(0);

			CSPOSKeyboardKeyExtent KeyExtent;
			SPOSKeyboardManager.GetKeymapTable() -> GetKeyInfoBySharpKeyPos( m_nMenuSharpKeyPos, KeyExtent );

			CMenuChanger MenuChanger;
			MenuChanger.EatMenuPopup( pSubMenu );
				
			if ( ( KeyExtent.m_nRowSize < 2 ) && ( KeyExtent.m_nColSize < 2 ) )
				MenuChanger.ShowItem( IDM_SPLIT_KEY, FALSE );

			if ( SPOSKeyboardManager.GetMultiKeySelectInfo() -> GotSelection() == FALSE )
			{
				MenuChanger.ShowItem( IDM_UNTAG_KEY, FALSE );
				MenuChanger.ShowItem( IDM_PASTE_KEY, FALSE );
			}
			
			{
				__int64 nListNo;
				int nPageNo, nDummy1, nDummy2;
				SPOSKeyboardManager.GetDlgViewLink() -> GetVisibleListDetails( nDummy1, nListNo, nPageNo, nDummy2 );

				int nKeyIdx;
				if ( SPOSKeyboardManager.GetListManager() -> FindKey( nListNo, nPageNo, m_nMenuSharpKeyPos, nKeyIdx ) == TRUE )
				{
					CSPOSKeyboardKeyRecord KeyRecord;
					SPOSKeyboardManager.GetListManager()-> GetKeyRecord( nKeyIdx, KeyRecord );

					if ( KeyRecord.GetType() == KEY_TYPE_PLUNUMBER )
					{
						m_bMenuIsPluKey = TRUE;

						CSQLPluNoInfo PluNoInfo;
						PluNoInfo.m_nEposPluNo = KeyRecord.GetValue();
						::ProcessPluNo( PluNoInfo );
						
						if ( TRUE == PluNoInfo.m_bValid )
						{
							int nPluIdx;
							m_nMenuPluNo = PluNoInfo.m_nBasePluNo;
							m_bMenuPluExists = DataManager.Plu.FindPluByNumber( m_nMenuPluNo, nPluIdx );
						}
					}
				}
			}

			if ( ( FALSE == m_bMenuIsPluKey ) || ( FALSE == m_bMenuPluExists ) )
			{
				MenuChanger.ShowItem( IDM_EDIT_PLU_MASTER, FALSE );
				MenuChanger.ShowItem( IDM_EDIT_PLU_PRICESET1, FALSE );
				MenuChanger.ShowItem( IDM_EDIT_PLU_PRICESET2, FALSE );
				MenuChanger.ShowItem( IDM_EDIT_PLU_PRICESET3, FALSE );
			}
			else
			{
				//MenuChanger.ShowItem( IDM_CREATE_PLU, FALSE );

				int nPriceSets = SPOSKeyboardManager.GetPriceSetCount();
				MenuChanger.ShowItem( IDM_EDIT_PLU_PRICESET1, ( nPriceSets >= 1 ) );
				MenuChanger.ShowItem( IDM_EDIT_PLU_PRICESET2, ( nPriceSets >= 2 ) );
				MenuChanger.ShowItem( IDM_EDIT_PLU_PRICESET3, ( nPriceSets >= 3 ) );
					
				if ( nPriceSets <= 0 )
				{
					MenuChanger.SetItemText( IDM_EDIT_PLU_MASTER, "Edit Plu Item" );
				}
			}

			{
				bool bCanEditPlu = TRUE;
				bCanEditPlu &= PasswordArray.GetEnable( PasswordTicks::DatabaseMenu );
				bCanEditPlu &= PasswordArray.GetEnable( PasswordTicks::EditEposData );
				bCanEditPlu &= PasswordArray.GetEnable( PasswordTicks::EditPluRecords );

				if ( FALSE == bCanEditPlu )
				{
					MenuChanger.ShowItem( IDM_CREATE_PLU, FALSE );
					MenuChanger.ShowItem( IDM_LINK_PLU, FALSE );
					MenuChanger.ShowItem( IDM_EDIT_PLU_MASTER, FALSE );
					MenuChanger.ShowItem( IDM_EDIT_PLU_PRICESET1, FALSE );
					MenuChanger.ShowItem( IDM_EDIT_PLU_PRICESET2, FALSE );
					MenuChanger.ShowItem( IDM_EDIT_PLU_PRICESET3, FALSE );
				}
			}

			MenuChanger.BuildMenu( pSubMenu );

			m_bMenuVisible = TRUE;
			pSubMenu -> TrackPopupMenu ( 0, point.x, point.y, this );
			m_bMenuVisible = FALSE;
		
			PopUp.DestroyMenu();
		}
	}
	
	CScrollView::OnRButtonDown(nFlags, point);
}

/**********************************************************************/
//EDITING AND COPYING KEYS
/**********************************************************************/

void CSPOSKeyboardView::DropKey ( CPoint point )
{
	if ( SPOSKeyboardManager.GetModelessPointers() -> CanDropKeyNow() == FALSE )
		return;

	CPoint testPoint = point;

	CDC* pDC = GetDC();
	pDC -> SetMapMode ( MM_TEXT );
	pDC -> DPtoLP ( &testPoint );
	ReleaseDC ( pDC );

	testPoint += GetScrollPosition();

	int nKeyNo = SPOSKeyboardManager.GetKeymapTable() -> GetKeyAtPoint ( testPoint );
	if ( nKeyNo <= 0 )
		return;

	__int64 nListNo;
	int nPageNo, nDummy1, nDummy2;
	SPOSKeyboardManager.GetDlgViewLink() -> GetVisibleListDetails( nDummy1, nListNo, nPageNo, nDummy2 );

	int nKeyIdx;
	if ( SPOSKeyboardManager.GetListManager() -> FindKey( nListNo, nPageNo, nKeyNo, nKeyIdx ) == FALSE )
		return;
	
	CSPOSKeyboardKeyRecord KeyRecord;
	SPOSKeyboardManager.GetListManager() -> GetKeyRecord( nKeyIdx, KeyRecord );
	SPOSKeyboardManager.GetModelessPointers() -> UpdateKeyRecord( KeyRecord );
	SPOSKeyboardManager.GetListManager() -> SetKeyRecord( nKeyIdx, KeyRecord );

	if ( SPOSKeyboardManager.GetModelessPointers() -> GetColourDropperMode() == COLOURDROPPER_GROUPKEY )
	{
		SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
		SPOSKeyboardManager.GetListManager() -> GroupKey( nListNo, nPageNo, nKeyNo, m_nEditSharpKeyPos );

		m_nEditSharpKeyPos = 0;
		RebuildKeymap();
		Invalidate();
		RedrawWindow();
		
		SPOSKeyboardManager.GetModelessPointers() -> CloseColourDropper();
	}
	else
	{
		SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
		RedrawKey( nKeyNo );
	}
}

/**********************************************************************/
//CONTEXT MENU
/**********************************************************************/

void CSPOSKeyboardView::OnEditKey() 
{
	if ( m_nMenuSharpKeyPos == 0 )
		return;

	__int64 nListNo;
	int nPageNo, nDummy1, nDummy2;
	SPOSKeyboardManager.GetDlgViewLink() -> GetVisibleListDetails( nDummy1, nListNo, nPageNo, nDummy2 );

	int nKeyIdx;
	if ( SPOSKeyboardManager.GetListManager() -> FindKey( nListNo, nPageNo, m_nMenuSharpKeyPos, nKeyIdx ) == FALSE )
		return;

	CancelKeyGrouper();

	m_nEditSharpKeyPos = m_nMenuSharpKeyPos;
	RedrawKey( m_nEditSharpKeyPos );

	CRect rect;
	GetClientRect( rect );
	ClientToScreen( rect );
	CSPOSKeyboardPropertySheetEditKey propSheet( nKeyIdx, rect, this );
	if ( propSheet.DoModal() == IDOK )
	{
		propSheet.SaveRecord();
		SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
	}
	else
	{
		propSheet.RestoreRecord();
	}

	int nPos = m_nEditSharpKeyPos;
	m_nEditSharpKeyPos = 0;
	RedrawKey( nPos );
}

/**********************************************************************/

void CSPOSKeyboardView::RedrawEditedKey()
{
	RedrawKey( m_nEditSharpKeyPos );
}

/**********************************************************************/
void CSPOSKeyboardView::OnEditPluMaster(){ EditPlu( SPOSKEYBOARD_PLUEDIT_MASTER ); }
void CSPOSKeyboardView::OnEditPluPriceSet1(){ EditPlu( SPOSKEYBOARD_PLUEDIT_PRICESET1 ); }
void CSPOSKeyboardView::OnEditPluPriceSet2(){ EditPlu( SPOSKEYBOARD_PLUEDIT_PRICESET2 ); }
void CSPOSKeyboardView::OnEditPluPriceSet3(){ EditPlu( SPOSKEYBOARD_PLUEDIT_PRICESET3 ); }
void CSPOSKeyboardView::OnCreatePlu(){ EditPlu( SPOSKEYBOARD_PLUEDIT_CREATE ); }
void CSPOSKeyboardView::OnLinkPlu(){ EditPlu( SPOSKEYBOARD_PLUEDIT_LINK ); }
/**********************************************************************/

void CSPOSKeyboardView::EditPlu( int nMode )
{
	if (m_nMenuSharpKeyPos == 0)
	{
		return;
	}

	__int64 nListNo;
	int nPageNo, nDummy1, nDummy2;
	SPOSKeyboardManager.GetDlgViewLink() -> GetVisibleListDetails( nDummy1, nListNo, nPageNo, nDummy2 );

	int nKeyIdx;
	if (SPOSKeyboardManager.GetListManager()->FindKey(nListNo, nPageNo, m_nMenuSharpKeyPos, nKeyIdx) == FALSE)
	{
		return;
	}

	CSPOSKeyboardKeyRecord KeyRecord;
	SPOSKeyboardManager.GetListManager() -> GetKeyRecord( nKeyIdx, KeyRecord );

	bool bUseDefaultText = TRUE;
	bool bUseDefaultImage = TRUE;
	__int64 nPluKeyNoSave = 0;

	switch( nMode )
	{
	case SPOSKEYBOARD_PLUEDIT_MASTER:
	case SPOSKEYBOARD_PLUEDIT_PRICESET1:
	case SPOSKEYBOARD_PLUEDIT_PRICESET2:
	case SPOSKEYBOARD_PLUEDIT_PRICESET3:
		{
			CSPOSKeyboardKeyHelpers KeyHelpers;
			CSPOSKeyboardDefaultTextRecord TextRecord;
			KeyHelpers.GetDefaultTextRecord( KeyRecord, TextRecord );
			
			CString strCurrentDefaultText;
			TextRecord.GetDefaultText( strCurrentDefaultText );
			CString strCurrentDefaultImage = TextRecord.GetImageFilename();
			
			bUseDefaultText = ( strCurrentDefaultText == KeyRecord.GetKeyText() );
			bUseDefaultImage = ( strCurrentDefaultImage == KeyRecord.GetImageFilename() );

			nPluKeyNoSave = KeyRecord.GetValue();
		}
		break;
	}

	CancelKeyGrouper();
	
	int nColourDropperMode = SPOSKeyboardManager.GetModelessPointers() -> GetColourDropperMode();
	SPOSKeyboardManager.GetModelessPointers() -> CloseColourDropper();
	
	m_nEditSharpKeyPos = m_nMenuSharpKeyPos;
	RedrawKey( m_nEditSharpKeyPos );

	__int64 nPluNo = m_nMenuPluNo;
	if ((SPOSKEYBOARD_PLUEDIT_CREATE == nMode) && (TRUE == m_bMenuPluExists))
	{
		nPluNo = 0;
	}

#ifndef STOCKMAN_SYSTEM
	CSPOSKeyboardPluEditEcrman PluEdit( nPluNo, nMode, m_nNewPluDeptNo, KeyRecord.GetImageFilename(), this );
#else
	CSPOSKeyboardPluEditStockman PluEdit( nPluNo, nMode, m_nNewPluDeptNo, KeyRecord.GetImageFilename(), this );
#endif
		
	if ( PluEdit.LoadDatabase() == TRUE )
	{
		bool bEdited = PluEdit.EditPlu();

		if ( TRUE == bEdited )
		{
			PluEdit.SaveChanges();
		}

		PluEdit.Finalise();

		if ( TRUE == bEdited )
		{
			SPOSKeyboardManager.CreatePluItemList();
			CreatePluKey( nKeyIdx, PluEdit, bUseDefaultText, bUseDefaultImage, nPluKeyNoSave );
		}
	}
	PluEdit.LoadReadOnlyDatabase();

	if (nColourDropperMode == COLOURDROPPER_LISTKEY)
	{
		SPOSKeyboardManager.GetModelessPointers()->OpenColourDropper(COLOURDROPPER_LISTKEY);
	}

	int nPos = m_nEditSharpKeyPos;
	m_nEditSharpKeyPos = 0;
	RedrawKey( nPos );
}

/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
void CSPOSKeyboardView::CreatePluKey( int nKeyIdx, CSPOSKeyboardPluEditEcrman& PluEdit, bool bUseDefaultText, bool bUseDefaultImage, __int64 nPluNoSave ) 
/**********************************************************************/
#else
/**********************************************************************/
void CSPOSKeyboardView::CreatePluKey( int nKeyIdx, CSPOSKeyboardPluEditStockman& PluEdit, bool bUseDefaultText, bool bUseDefaultImage, __int64 nPluNoSave)
/**********************************************************************/
#endif
/**********************************************************************/
{
	CSPOSKeyboardKeyRecord KeyRecord;
	SPOSKeyboardManager.GetListManager() -> GetKeyRecord( nKeyIdx, KeyRecord );
	
	KeyRecord.SetType( KEY_TYPE_PLUNUMBER );
	
	if (nPluNoSave != 0)
	{
		KeyRecord.SetValue(nPluNoSave);
	}
	else
	{
		KeyRecord.SetValue(PluEdit.GetSharpPosPluNo());
	}

	if ( ( TRUE == bUseDefaultText ) || ( TRUE == bUseDefaultImage ) )
	{
		CSPOSKeyboardKeyHelpers KeyHelpers;
		CSPOSKeyboardDefaultTextRecord TextRecord;
		KeyHelpers.GetDefaultTextRecord( KeyRecord, TextRecord );

		CString strText = "";
		TextRecord.GetDefaultText( strText );
		CString strImageFilename = TextRecord.GetImageFilename();

		if (TRUE == bUseDefaultText)
		{
			KeyRecord.SetKeyText(strText);
		}

		if ( ( strImageFilename != "" ) && ( TRUE == bUseDefaultImage ) )
		{
			KeyRecord.SetImageFilename( TextRecord.GetImageFilename() );
			KeyRecord.SetImageScale( TextRecord.GetImageScale() );
		}
	}

	SPOSKeyboardManager.GetListManager() -> SetKeyRecord( nKeyIdx, KeyRecord );
	SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
}

/**********************************************************************/

void CSPOSKeyboardView::OnSplitKey() 
{
	if ( m_nMenuSharpKeyPos == 0 )
		return;

	m_nEditSharpKeyPos = m_nMenuSharpKeyPos;
	RedrawKey( m_nEditSharpKeyPos );

	if ( Prompter.YesNo( "Are you sure you wish to split this key" ) == IDYES )
	{
		__int64 nListNo;
		int nScreenNo, nPageNo, nKeyCount;
		SPOSKeyboardManager.GetDlgViewLink() -> GetVisibleListDetails( nScreenNo, nListNo, nPageNo, nKeyCount );
		SPOSKeyboardManager.GetListManager() -> SplitKey( nListNo, nPageNo, m_nEditSharpKeyPos );

		if ( nListNo == SPOSKeyboardManager.GetMultiKeySelectInfo() -> GetListNo() )
			SPOSKeyboardManager.GetMultiKeySelectInfo() -> ClearSelection();
		
		m_nEditSharpKeyPos = 0;

		RebuildKeymap();
		SetKeyboardDialogTitle();
		Invalidate();
		UpdateWindow();

		SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
	}
	else
	{
		int nPos = m_nEditSharpKeyPos;
		m_nEditSharpKeyPos = 0;
		RedrawKey( nPos );
	}
}

/**********************************************************************/

void CSPOSKeyboardView::OnCopyKey() 
{
	__int64 nListNo;
	int nPageNo, nDummy1, nDummy2;
	SPOSKeyboardManager.GetDlgViewLink() -> GetVisibleListDetails( nDummy1, nListNo, nPageNo, nDummy2 );

	int nKeyIdx;
	if ( SPOSKeyboardManager.GetListManager() -> FindKey( nListNo, nPageNo, m_nMenuSharpKeyPos, nKeyIdx ) == FALSE )
		return;

	SPOSKeyboardManager.GetModelessPointers() -> OpenColourDropper( COLOURDROPPER_COPYKEY );
		
	CSPOSKeyboardKeyRecord KeyRecord;
	SPOSKeyboardManager.GetListManager() -> GetKeyRecord( nKeyIdx, KeyRecord );
	SPOSKeyboardManager.GetModelessPointers() -> SetKeyToCopy( KeyRecord );
	SPOSKeyboardManager.GetModelessPointers() -> SetColoursToDropAsShown();

}

/**********************************************************************/

void CSPOSKeyboardView::OnGroupKey() 
{
	__int64 nListNo;
	int nPageNo, nDummy1, nDummy2;
	SPOSKeyboardManager.GetDlgViewLink() -> GetVisibleListDetails( nDummy1, nListNo, nPageNo, nDummy2 );

	int nKeyIdx;
	if ( SPOSKeyboardManager.GetListManager() -> FindKey( nListNo, nPageNo, m_nMenuSharpKeyPos, nKeyIdx ) == FALSE )
		return;

	if ( nListNo == SPOSKeyboardManager.GetMultiKeySelectInfo() -> GetListNo() )
		SPOSKeyboardManager.GetMultiKeySelectInfo() -> ClearSelection();

	m_nEditSharpKeyPos = m_nMenuSharpKeyPos;
	RedrawKey( m_nEditSharpKeyPos );

	SPOSKeyboardManager.GetModelessPointers() -> OpenColourDropper( COLOURDROPPER_GROUPKEY );
		
	CSPOSKeyboardKeyRecord KeyRecord;
	SPOSKeyboardManager.GetListManager() -> GetKeyRecord( nKeyIdx, KeyRecord );
	SPOSKeyboardManager.GetModelessPointers() -> SetKeyToCopy( KeyRecord );
	SPOSKeyboardManager.GetModelessPointers() -> SetColoursToDropAsShown();
}

/**********************************************************************/

void CSPOSKeyboardView::OnCopyStyle() 
{
	__int64 nListNo;
	int nPageNo, nDummy1, nDummy2;
	SPOSKeyboardManager.GetDlgViewLink() -> GetVisibleListDetails( nDummy1, nListNo, nPageNo, nDummy2 );

	int nKeyIdx;
	if ( SPOSKeyboardManager.GetListManager() -> FindKey( nListNo, nPageNo, m_nMenuSharpKeyPos, nKeyIdx ) == FALSE )
		return;

	if ( SPOSKeyboardManager.GetModelessPointers() -> IsColourDropperActive() == FALSE )
		SPOSKeyboardManager.GetModelessPointers() -> OpenColourDropper( COLOURDROPPER_COPYSTYLE );
	else
		SPOSKeyboardManager.GetModelessPointers() -> UpdateTitle();
		
	CSPOSKeyboardKeyRecord KeyRecord;
	SPOSKeyboardManager.GetListManager() -> GetKeyRecord( nKeyIdx, KeyRecord );

	if ( SPOSKeyboardManager.GetLayoutOptions() -> GetImageWithStyleFlag() == TRUE )
	{
		SPOSKeyboardManager.GetModelessPointers() -> SetImageFlag( TRUE );
		SPOSKeyboardManager.GetModelessPointers() -> SetImageFilename( KeyRecord.GetImageFilename() );
		SPOSKeyboardManager.GetModelessPointers() -> SetImageScale( KeyRecord.GetImageScale() );
		SPOSKeyboardManager.GetModelessPointers() -> UpdateImage();
	}

	SPOSKeyboardManager.GetModelessPointers() -> SetForeground( KeyRecord.GetForeground() );
	SPOSKeyboardManager.GetModelessPointers() -> SetBackground( KeyRecord.GetBackground() );
	SPOSKeyboardManager.GetModelessPointers() -> SetFontSize( KeyRecord.GetFontSize() );
	SPOSKeyboardManager.GetModelessPointers() -> SetHAlign( KeyRecord.GetAlignmentH() );
	SPOSKeyboardManager.GetModelessPointers() -> SetVAlign( KeyRecord.GetAlignmentV() );
	SPOSKeyboardManager.GetModelessPointers() -> SetBoldStatus( KeyRecord.GetBoldFlag() ? 1 : 0 );
	SPOSKeyboardManager.GetModelessPointers() -> SetDefaultTextAlwaysStatus( KeyRecord.GetDefaultTextAlwaysFlag() ? 1 : 0 );
	SPOSKeyboardManager.GetModelessPointers() -> SetColoursToDropAsShown();
}

/**********************************************************************/

void CSPOSKeyboardView::OnCopyImage() 
{
	__int64 nListNo;
	int nPageNo, nDummy1, nDummy2;
	SPOSKeyboardManager.GetDlgViewLink() -> GetVisibleListDetails( nDummy1, nListNo, nPageNo, nDummy2 );

	int nKeyIdx;
	if ( SPOSKeyboardManager.GetListManager() -> FindKey( nListNo, nPageNo, m_nMenuSharpKeyPos, nKeyIdx ) == FALSE )
		return;

	bool bAlreadyOpen = SPOSKeyboardManager.GetModelessPointers() -> IsColourDropperActive(); 
	if ( FALSE == bAlreadyOpen )
	{
		SPOSKeyboardManager.GetModelessPointers() -> OpenColourDropper( COLOURDROPPER_COPYSTYLE );
		SPOSKeyboardManager.GetModelessPointers() -> SetCopyImageTitle();
	}

	CSPOSKeyboardKeyRecord KeyRecord;
	SPOSKeyboardManager.GetListManager() -> GetKeyRecord( nKeyIdx, KeyRecord );
	SPOSKeyboardManager.GetModelessPointers() -> SetImageFlag( TRUE );
	SPOSKeyboardManager.GetModelessPointers() -> SetImageFilename( KeyRecord.GetImageFilename() );
	SPOSKeyboardManager.GetModelessPointers() -> SetImageScale( KeyRecord.GetImageScale() );
	
	SPOSKeyboardManager.GetModelessPointers() -> UpdateImage();

	if ( FALSE == bAlreadyOpen )
	{
		SPOSKeyboardManager.GetModelessPointers() -> SetColoursToNoChange(); 
		SPOSKeyboardManager.GetModelessPointers() -> SetFontSize( 999 );
		SPOSKeyboardManager.GetModelessPointers() -> SetHAlign( 999 );
		SPOSKeyboardManager.GetModelessPointers() -> SetVAlign( 999 );
		SPOSKeyboardManager.GetModelessPointers() -> SetBoldStatus( 999 );
		SPOSKeyboardManager.GetModelessPointers() -> SetDefaultTextAlwaysStatus( 999 );
		SPOSKeyboardManager.GetModelessPointers() -> SelectImageTab();
	}
}

/**********************************************************************/

void CSPOSKeyboardView::OnTagKey() 
{
	int nKeyNo = m_nMenuSharpKeyPos;
	if ( nKeyNo > 0 )
	{
		__int64 nListNo;
		int nPageNo, nDummy1, nDummy2;
		SPOSKeyboardManager.GetDlgViewLink() -> GetVisibleListDetails( nDummy1, nListNo, nPageNo, nDummy2 );

		CSPOSKeyboardKeyExtent KeyExtent;
		SPOSKeyboardManager.GetKeymapTable() -> GetKeyInfoBySharpKeyPos( nKeyNo, KeyExtent );

		SPOSKeyboardManager.GetMultiKeySelectInfo() -> TagKey( nListNo, nPageNo, KeyExtent );
		Invalidate();
	}
}

/**********************************************************************/

void CSPOSKeyboardView::OnUntagKey() 
{
	if ( SPOSKeyboardManager.GetMultiKeySelectInfo() -> GotSelection() == TRUE )
	{
		SPOSKeyboardManager.GetMultiKeySelectInfo() -> ClearSelection();
		Invalidate();
	}
}

/**********************************************************************/

void CSPOSKeyboardView::OnPasteKey() 
{
	if ( SPOSKeyboardManager.GetMultiKeySelectInfo() -> GotSelection() == FALSE )
	{
		Prompter.Error( "You have not marked an area to copy" );
		return;
	}

	int nKeyNo = m_nMenuSharpKeyPos;
	if ( nKeyNo <= 0 )
		return;
	
	__int64 nListNo;
	int nPageNo, nDummy1, nDummy2;
	SPOSKeyboardManager.GetDlgViewLink() -> GetVisibleListDetails( nDummy1, nListNo, nPageNo, nDummy2 );

	int nListIdx;
	if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == FALSE )
		return;
		
	CSPOSKeyboardKeyExtent TargetKeyExtent;
	SPOSKeyboardManager.GetKeymapTable() -> GetKeyInfoBySharpKeyPos( nKeyNo, TargetKeyExtent );

	CSPOSKeyboardListRecord ListRecord;
	SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );
	int nListCols = ListRecord.GetColumnCount();
	int nListRows = ListRecord.GetRowCount();

	if (	( TargetKeyExtent.m_nColStart + SPOSKeyboardManager.GetMultiKeySelectInfo() -> GetCopyWidth() > nListCols ) ||
			( TargetKeyExtent.m_nRowStart + SPOSKeyboardManager.GetMultiKeySelectInfo() -> GetCopyHeight() > nListRows ) )
	{
		Prompter.Error( "The marked area is too large to fit in the selected space." );
		return;
	}

	SPOSKeyboardManager.GetListManager() -> RememberKeysForList( nListNo );
	SPOSKeyboardManager.GetMultiKeySelectInfo() -> RememberSettings();

	int nColOffset = SPOSKeyboardManager.GetMultiKeySelectInfo() -> GetColStart() - TargetKeyExtent.m_nColStart;
	int nRowOffset = SPOSKeyboardManager.GetMultiKeySelectInfo() -> GetRowStart() - TargetKeyExtent.m_nRowStart;

	int nKeyIdx;
	SPOSKeyboardManager.GetListManager() -> FindKey( nListNo, nPageNo, 0, nKeyIdx );

	for ( int nPos = 0; nPos < SPOSKeyboardManager.GetKeymapTable() -> GetKeyCount(); nPos++ )
	{
		CSPOSKeyboardKeyExtent SplitKeyExtent;
		SPOSKeyboardManager.GetKeymapTable() -> GetKeyInfoByIndex( nPos, SplitKeyExtent );

		int nKeyIdxToTry = nKeyIdx + SplitKeyExtent.m_nKeyNo - 1;

		CSPOSKeyboardKeyRecord KeyRecord;
		if ( ( nKeyIdxToTry >= 0 ) && ( nKeyIdxToTry < SPOSKeyboardManager.GetListManager() -> GetKeyCount() ) )
		{
			SPOSKeyboardManager.GetListManager() -> GetKeyRecord( nKeyIdxToTry, KeyRecord );

			bool bIsSelected = FALSE;
			for ( int c = 0; ( c < SplitKeyExtent.m_nColSize) && ( FALSE == bIsSelected ); c++ )
			{
				for ( int r = 0; ( r < SplitKeyExtent.m_nRowSize) && ( FALSE == bIsSelected ); r++ )
				{
					if ( SPOSKeyboardManager.GetMultiKeySelectInfo() -> IsSelected( SplitKeyExtent.m_nColStart + nColOffset + c, SplitKeyExtent.m_nRowStart + nRowOffset + r ) )
					{	
						SPOSKeyboardManager.GetListManager() -> SplitKey( nListNo, nPageNo, SplitKeyExtent.m_nKeyNo );
						bIsSelected = TRUE;
					}
				}
			}
		}
	}
						
	RebuildKeymap();
	SPOSKeyboardManager.GetMultiKeySelectInfo() -> BuildCopyBuffer();

	int nNewKeyNoCorner1 = 1;
	int nNewKeyNoCorner2 = 1;

	for ( int n = 0; n < SPOSKeyboardManager.GetMultiKeySelectInfo() -> GetCopyBufferSize(); n++ )
	{
		CSPOSKeyboardKeyExtent CopyKeyExtent;
		CSPOSKeyboardKeyRecord CopyKeyRecord;
		SPOSKeyboardManager.GetMultiKeySelectInfo() -> GetCopyBufferKey( n, CopyKeyExtent, CopyKeyRecord );

		int nDestKeyIdx = 0;
		nDestKeyIdx += nKeyIdx;
		nDestKeyIdx += TargetKeyExtent.m_nColStart;
		nDestKeyIdx += CopyKeyExtent.m_nColStart;
		nDestKeyIdx -= SPOSKeyboardManager.GetMultiKeySelectInfo() -> GetColStart();
		nDestKeyIdx += ( TargetKeyExtent.m_nRowStart * nListCols );
		nDestKeyIdx += ( CopyKeyExtent.m_nRowStart * nListCols );
		nDestKeyIdx -= ( SPOSKeyboardManager.GetMultiKeySelectInfo() -> GetRowStart() * nListCols );

		if ( ( nDestKeyIdx >= 0 ) && ( nDestKeyIdx < SPOSKeyboardManager.GetListManager() -> GetKeyCount() ) )
		{
			CSPOSKeyboardKeyRecord TargetKeyRecord;
			SPOSKeyboardManager.GetListManager() -> GetKeyRecord( nDestKeyIdx, TargetKeyRecord );
			if ( ( TargetKeyRecord.GetListNo() == nListNo ) && ( TargetKeyRecord.GetPageNo() == nPageNo ) && ( TargetKeyRecord.GetColSize() == 1 ) && ( TargetKeyRecord.GetRowSize() == 1 ) )
			{
				int nKeyNo = TargetKeyRecord.GetKeyNo();
				TargetKeyRecord = CopyKeyRecord;
				TargetKeyRecord.SetListNo( nListNo );
				TargetKeyRecord.SetPageNo( nPageNo );
				TargetKeyRecord.SetKeyNo( nKeyNo );
				SPOSKeyboardManager.GetListManager() -> SetKeyRecord( nDestKeyIdx, TargetKeyRecord );

				if ( CopyKeyRecord.GetKeyNo() == SPOSKeyboardManager.GetMultiKeySelectInfo() -> GetKeyNoCorner1() )
					nNewKeyNoCorner1 = nKeyNo;

				if ( CopyKeyRecord.GetKeyNo() == SPOSKeyboardManager.GetMultiKeySelectInfo() -> GetKeyNoCorner2() )
					nNewKeyNoCorner2 = nKeyNo;

			}
		}
	}

	RebuildKeymap();

	{
		CSPOSKeyboardKeyExtent KeyExtent1, KeyExtent2;
		SPOSKeyboardManager.GetKeymapTable() -> GetKeyInfoBySharpKeyPos( nNewKeyNoCorner1, KeyExtent1 );
		SPOSKeyboardManager.GetKeymapTable() -> GetKeyInfoBySharpKeyPos( nNewKeyNoCorner2, KeyExtent2 );
		SPOSKeyboardManager.GetMultiKeySelectInfo() -> TagKey( nListNo, nPageNo, KeyExtent1 );
		SPOSKeyboardManager.GetMultiKeySelectInfo() -> TagKey( nListNo, nPageNo, KeyExtent2 );
	}
	
	m_bSortPreviewMode = TRUE;
	Invalidate();
	UpdateWindow();

	CSPOSKeyboardPastePromptDlg dlg( ( ListRecord.GetPageCount() > 1 ), this );
	if ( dlg.DoModal() != IDOK )
	{
		SPOSKeyboardManager.GetListManager() -> RestoreKeysForList( nListNo );
		RebuildKeymap();
		SPOSKeyboardManager.GetMultiKeySelectInfo() -> RestoreSettings();
	}
	else
		SPOSKeyboardManager.GetListManager() -> SetModifiedFlag();
		
	m_bSortPreviewMode = FALSE;
	Invalidate();	
	UpdateWindow();
}

/**********************************************************************/

void CSPOSKeyboardView::CancelKeyGrouper()
{
	if ( SPOSKeyboardManager.GetModelessPointers() -> GetColourDropperMode() == COLOURDROPPER_GROUPKEY )
	{
		SPOSKeyboardManager.GetModelessPointers() -> CloseColourDropper();
		m_nEditSharpKeyPos = 0;
		RebuildKeymap();
		Invalidate();
		RedrawWindow();
	}
}

/**********************************************************************/

void CSPOSKeyboardView::CheckInvisible() 
{
	RebuildKeymap();
	SetKeyboardDialogTitle();
	Invalidate();
	UpdateWindow();
}

/**********************************************************************/

void CSPOSKeyboardView::CheckImages() 
{
	RebuildKeymap();
	SetKeyboardDialogTitle();
	Invalidate();
	UpdateWindow();
}

/**********************************************************************/

void CSPOSKeyboardView::GetVisibleListDetails( int& nScreenNo, __int64& nListNo, int& nPageNo, int& nKeyCount )
{
	SPOSKeyboardManager.GetDlgViewLink() -> GetVisibleListDetails( nScreenNo, nListNo, nPageNo, nKeyCount );
}

/**********************************************************************/

int CSPOSKeyboardView::OnMouseActivate( CWnd* pDesktopWnd, 
                                  UINT nHitTest, 
                                  UINT message )
{
  return MA_ACTIVATE;
}

/**********************************************************************/
