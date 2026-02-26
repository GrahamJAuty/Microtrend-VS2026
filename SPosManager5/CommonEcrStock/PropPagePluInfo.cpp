/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "PriceSetItemManager.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPagePluInfo.h"
/**********************************************************************/
const int MAX_PLUINFO_EDITABLE_TEXT = 2048;
/**********************************************************************/

CPropPagePluInfo::CPropPagePluInfo() : CSSPropertyPage(CPropPagePluInfo::IDD)
{
	m_pPluRecord = NULL;
	
	m_bReadOnly = FALSE;
	m_strInitialDir = ".\\";
	m_nTabType = PLUINFO_TYPE_TEXT;
	m_bAllowChange = FALSE;

	//{{AFX_DATA_INIT(CPropPagePluInfo)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPagePluInfo::~CPropPagePluInfo()
{
}

/**********************************************************************/

void CPropPagePluInfo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePluInfo)
	DDX_Control(pDX, IDC_TAB_INFOTYPE, m_TabInfoType);
	DDX_Control(pDX, IDC_EDIT_TEXT, m_editText);
	DDX_Control(pDX, IDC_CHECK_MODIFIER, m_checkModifier);
	DDX_Control(pDX, IDC_BUTTON_SELECT, m_buttonSelect);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_buttonDelete);
	DDX_Control(pDX, IDC_STATIC_TOOLARGE, m_staticTooLarge);
	DDX_Control(pDX, IDC_STATIC_WIDTH, m_staticWidth);
	DDX_Control(pDX, IDC_STATIC_HEIGHT, m_staticHeight);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_editWidth);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_editHeight);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePluInfo, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePluInfo)
	//}}AFX_MSG_MAP
	ON_WM_MEASUREITEM()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_INFOTYPE, OnSelectTab)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_FILES, OnGetDispInfoList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FILES, OnSelectResource)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePluInfo::OnInitDialog() 
{
	m_staticImage.SubclassDlgItem(IDC_STATIC_IMAGE,this);
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_TabInfoType.InsertItem( 0, "Text" );
	m_TabInfoType.InsertItem( 1, "Image" );
	m_TabInfoType.InsertItem( 2, "HTML Page" );
	m_TabInfoType.InsertItem( 3, "HTML Files" );
	
	m_listFiles.SubclassDlgItem ( IDC_LIST_FILES, this );
	m_listFiles.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listFiles.InsertColumn ( 0, "Filename", LVCFMT_LEFT, 320 );

	m_editText.LimitText( MAX_PLUINFO_EDITABLE_TEXT );
		
	SetRecordControls();

	if ( SysInfo.GetMaxBasePluLen() == 0 )
	{
		m_checkModifier.SetCheck( FALSE );
		m_checkModifier.EnableWindow( FALSE );
		m_checkModifier.ShowWindow( SW_HIDE );
	}

	MoveControl( &m_staticImage, 7, 50, 507, 293 );
	MoveControl( &m_editText, 7, 50, 507, 293 );
	MoveControl( &m_listFiles, 7, 50, 250, 293 );
	MoveControl( &m_staticWidth, 396, 352 );
	MoveControl( &m_editWidth, 421, 352 );
	MoveControl( &m_staticHeight, 463, 352 );
	MoveControl( &m_editHeight, 488, 352 );
	
	return TRUE;  
}

/**********************************************************************/

void CPropPagePluInfo::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST_FILES )
   {
	   m_listFiles.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

void CPropPagePluInfo::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo -> item.iItem;

		if ( pDispInfo -> item.iSubItem == 0 )
			lstrcpy ( pDispInfo -> item.pszText, DataManager.ResourceWrapperPlu.GetResourceFileName( nIndex ) );
	}
	*pResult = 0;
}

/**********************************************************************/

BOOL CPropPagePluInfo::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePluInfo::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPagePluInfo::Refresh()
{
	GetRecordData();
	SetRecordControls();
	UpdateData ( FALSE );
}

/**********************************************************************/

bool CPropPagePluInfo::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPagePluInfo::SaveRecord()
{
	if ( FALSE == m_bAllowChange )
		return;

	switch( m_nTabType )
	{
	case PLUINFO_TYPE_TEXT:
		SaveTextTabInfo();
		break;

	case PLUINFO_TYPE_HTML:
		SaveHTMLTabInfo();
		break;
	}

	m_pPluRecord -> SetPluInfoModifierFlag( m_checkModifier.GetCheck() != 0 );
}

/**********************************************************************/

void CPropPagePluInfo::GetRecordData()
{
	int nTabType = DataManager.ResourceWrapperPlu.GetDefaultTabType();
	if ( nTabType != -1 )
		m_nTabType = nTabType;
}

/**********************************************************************/

void CPropPagePluInfo::SetRecordControls()
{
	m_bAllowChange = TRUE;

	if ( ( PriceSetItemManager.IsPluMaster() == FALSE ) || ( m_bReadOnly == TRUE ) )
	{
		m_bAllowChange = FALSE;
		
		int nPluStatus = PriceSetItemManager.GetPluStatus();

		if ( ( nPluStatus != LOCALPLU_LOCAL ) || ( m_bReadOnly == TRUE ) )
			m_checkModifier.EnableWindow( FALSE );	
		else
			m_checkModifier.EnableWindow( PriceSetItemManager.CheckField( Plu::PluInfoModifier.Label ) );
	}

	m_checkModifier.SetCheck( m_pPluRecord -> GetPluInfoModifierFlag() );

	switch( m_nTabType )
	{
	case PLUINFO_TYPE_IMAGE:
		ShowImageTab();
		break;

	case PLUINFO_TYPE_HTML:
		ShowHTMLTab();
		break;

	case PLUINFO_TYPE_TEXT:
	default:
		ShowTextTab();
		break;
	}
}

/**********************************************************************/

void CPropPagePluInfo::ShowImageTab()
{
	m_TabInfoType.SetCurSel(1);
	MoveControl( &m_staticImage, 7, 50, 507, 293 );
	m_editText.ShowWindow( SW_HIDE );
	m_staticImage.ShowWindow( SW_SHOW );
	m_listFiles.ShowWindow( SW_HIDE );
	m_staticTooLarge.ShowWindow( SW_HIDE );
	MoveControl( &m_staticWidth, 396, 352 );
	MoveControl( &m_editWidth, 421, 352 );
	MoveControl( &m_staticHeight, 463, 352 );
	MoveControl( &m_editHeight, 488, 352 );
	m_staticWidth.ShowWindow( SW_SHOW );
	m_editWidth.ShowWindow( SW_SHOW );
	m_staticHeight.ShowWindow( SW_SHOW );
	m_editHeight.ShowWindow( SW_SHOW );

	m_buttonSelect.SetWindowText( "Load\nImage File" );
	m_buttonDelete.SetWindowText( "Delete\nImage File" );

	if ( DataManager.ResourceWrapperPlu.SavedAsBlank( PLUINFO_TYPE_IMAGE ) == TRUE )
	{
		m_staticImage.SetBlankFlag( TRUE );
		m_buttonDelete.EnableWindow( FALSE );
		m_staticImage.Invalidate();
		m_editWidth.SetWindowText( "" );
		m_editHeight.SetWindowText( "" );
	}
	else
	{
		bool bFailed = FALSE;
		bool bGotFile = FALSE;

		CString strFile = DataManager.ResourceWrapperPlu.GetFilePathVisible( PLUINFO_TYPE_IMAGE );

		if ( ::FileExists( strFile )== TRUE )
		{
			bGotFile = ( m_staticImage.LoadImage( strFile ) == TRUE );
			bFailed = ( FALSE == bGotFile );
		}

		if ( TRUE == bGotFile )
		{
			m_staticImage.SetBlankFlag( FALSE );
			m_staticImage.SetFailedFlag( FALSE );
			m_staticImage.Invalidate();
			m_editWidth.SetWindowText( m_staticImage.GetWidthString() );
			m_editHeight.SetWindowText( m_staticImage.GetHeightString() );
		}
		else
		{
			m_staticImage.SetBlankFlag( TRUE );
			m_staticImage.SetFailedFlag( bFailed );
			m_staticImage.Invalidate();
			m_editWidth.SetWindowText( "" );
			m_editHeight.SetWindowText( "" );
		}

		m_buttonDelete.EnableWindow( bGotFile | bFailed );
	}

	if ( FALSE == m_bAllowChange )
	{
		ShowAndEnableWindow( &m_buttonDelete, FALSE );
		ShowAndEnableWindow( &m_buttonSelect, FALSE );
		m_editText.SetReadOnly( TRUE );
	}
}

/**********************************************************************/

void CPropPagePluInfo::ShowTextTab()
{
	m_TabInfoType.SetCurSel(0);
	m_editText.ShowWindow( SW_SHOW );
	m_staticImage.ShowWindow( SW_HIDE );
	m_listFiles.ShowWindow( SW_HIDE );
	m_buttonSelect.SetWindowText( "Load\nText File" );
	m_buttonDelete.SetWindowText( "Delete\nText File" );
	m_staticWidth.ShowWindow( SW_HIDE );
	m_editWidth.ShowWindow( SW_HIDE );
	m_staticHeight.ShowWindow( SW_HIDE );
	m_editHeight.ShowWindow( SW_HIDE );

	if ( DataManager.ResourceWrapperPlu.SavedAsBlank( PLUINFO_TYPE_TEXT ) == TRUE )
	{
		m_editText.SetWindowText( "" );
		m_buttonDelete.EnableWindow( FALSE );
		m_staticTooLarge.ShowWindow( SW_HIDE );
	}
	else
	{
		CString strFilename = DataManager.ResourceWrapperPlu.GetFilePathVisible( PLUINFO_TYPE_TEXT );
		bool bGotFile = LoadTextField( strFilename );
		m_buttonDelete.EnableWindow( bGotFile );
	}

	if ( FALSE == m_bAllowChange )
	{
		ShowAndEnableWindow( &m_buttonDelete, FALSE );
		ShowAndEnableWindow( &m_buttonSelect, FALSE );
		m_editText.SetReadOnly( TRUE );
	}
}

/**********************************************************************/

void CPropPagePluInfo::ShowHTMLTab()
{
	m_TabInfoType.SetCurSel(2);
	m_editText.ShowWindow( SW_SHOW );
	m_staticImage.ShowWindow( SW_HIDE );
	m_listFiles.ShowWindow( SW_HIDE );
	m_buttonSelect.SetWindowText( "Load\nHTML File" );
	m_buttonDelete.SetWindowText( "Delete\nHTML File" );
	m_staticWidth.ShowWindow( SW_HIDE );
	m_editWidth.ShowWindow( SW_HIDE );
	m_staticHeight.ShowWindow( SW_HIDE );
	m_editHeight.ShowWindow( SW_HIDE );

	if ( DataManager.ResourceWrapperPlu.SavedAsBlank( PLUINFO_TYPE_HTML ) == TRUE )
	{
		m_editText.SetWindowText( "" );
		m_buttonDelete.EnableWindow( FALSE );
		m_staticTooLarge.ShowWindow( SW_HIDE );
	}
	else
	{
		CString strFilename = DataManager.ResourceWrapperPlu.GetFilePathVisible( PLUINFO_TYPE_HTML );
		bool bGotFile = LoadTextField( strFilename );
		m_buttonDelete.EnableWindow( bGotFile );
	}

	if ( FALSE == m_bAllowChange )
	{
		ShowAndEnableWindow( &m_buttonDelete, FALSE );
		ShowAndEnableWindow( &m_buttonSelect, FALSE );
		m_editText.SetReadOnly( TRUE );
	}
}

/**********************************************************************/

void CPropPagePluInfo::ShowResourceTab()
{
	m_TabInfoType.SetCurSel(3);
	MoveControl( &m_staticImage, 264, 50, 250, 200 );
	m_listFiles.SetItemCountEx( DataManager.ResourceWrapperPlu.GetResourceFileCount() );
	m_editText.ShowWindow( SW_HIDE );
	m_staticImage.ShowWindow( SW_SHOW );
	m_listFiles.ShowWindow( SW_SHOW );
	m_buttonSelect.SetWindowText( "Add\nFile" );
	m_buttonDelete.SetWindowText( "Remove\nFile" );
	MoveControl( &m_staticWidth, 396, 260 );
	MoveControl( &m_editWidth, 421, 260 );
	MoveControl( &m_staticHeight, 463, 260 );
	MoveControl( &m_editHeight, 488, 260 );
	m_staticWidth.ShowWindow( SW_SHOW );
	m_editWidth.ShowWindow( SW_SHOW );
	m_staticHeight.ShowWindow( SW_SHOW );
	m_editHeight.ShowWindow( SW_SHOW );
	
	int nSel = m_listFiles.GetCurSel();
	if ( ( nSel < 0 ) || ( nSel >= DataManager.ResourceWrapperPlu.GetResourceFileCount() ) )
		SelectResourceListItem(0);

	ShowSelectedResource();

	if ( FALSE == m_bAllowChange )
	{
		ShowAndEnableWindow( &m_buttonDelete, FALSE );
		ShowAndEnableWindow( &m_buttonSelect, FALSE );
		m_editText.SetReadOnly( TRUE );
	}
}

/**********************************************************************/

void CPropPagePluInfo::OnSelectResource(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if ((pNMListView->uChanged & LVIF_STATE) 
        && (pNMListView->uNewState & LVNI_SELECTED))
    {
		ShowSelectedResource();
    }
}

/**********************************************************************/

void CPropPagePluInfo::ShowSelectedResource()
{
	int nSel = m_listFiles.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < DataManager.ResourceWrapperPlu.GetResourceFileCount() ) )
	{
		bool bFailed = FALSE;
		bool bGotFile = FALSE;

		CString strFile = DataManager.ResourceWrapperPlu.GetResourceFilePath( nSel );

		if ( ::FileExists( strFile )== TRUE )
		{
			bGotFile = ( m_staticImage.LoadImage( strFile ) == TRUE );
			bFailed = ( FALSE == bGotFile );
		}

		if ( TRUE == bGotFile )
		{
			m_staticImage.SetBlankFlag( FALSE );
			m_staticImage.SetFailedFlag( FALSE );
			m_staticImage.Invalidate();
			m_editWidth.SetWindowText( m_staticImage.GetWidthString() );
			m_editHeight.SetWindowText( m_staticImage.GetHeightString() );
		}
		else
		{
			m_staticImage.SetBlankFlag( TRUE );
			m_staticImage.SetFailedFlag( bFailed );
			m_staticImage.Invalidate();
			m_editWidth.SetWindowText( "" );
			m_editHeight.SetWindowText( "" );
		}
	}
	else
	{
		m_staticImage.SetBlankFlag( TRUE );
		m_staticImage.Invalidate();
	}
}

/**********************************************************************/

bool CPropPagePluInfo::LoadTextField( const char* szFilename )
{
	CString strText = "";
	bool bGotFile = ::FileExists ( szFilename );

	if ( TRUE == bGotFile )
	{
		CSSFile fileText;
		if ( fileText.Open( szFilename, "rb" ) == TRUE )
		{
			CString strBuffer;
			while( fileText.ReadString( strBuffer ) == TRUE )
				strText += strBuffer + "\r\n";
		}
	}

	m_editText.SetWindowText( strText );
	
	if ( strText.GetLength() > MAX_PLUINFO_EDITABLE_TEXT )
	{
		m_staticTooLarge.ShowWindow( SW_SHOW );
		m_editText.SetReadOnly( TRUE );
	}
	else
	{
		m_staticTooLarge.ShowWindow( SW_HIDE );
		m_editText.SetReadOnly( FALSE );
	}

	return bGotFile;
}

/**********************************************************************/

void CPropPagePluInfo::OnSelectTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	switch( m_nTabType )
	{
	case PLUINFO_TYPE_TEXT:
		SaveTextTabInfo();
		break;

	case PLUINFO_TYPE_HTML:
		SaveHTMLTabInfo();
		break;
	}

	int nTabPage = m_TabInfoType.GetCurSel();
	if ( ( nTabPage < 0 ) || ( nTabPage >= 4 ) )
		nTabPage = 0;

	switch( nTabPage )
	{
	case 0:
		m_nTabType = PLUINFO_TYPE_TEXT;
		ShowTextTab();
		break;

	case 1:
		m_nTabType = PLUINFO_TYPE_IMAGE;
		ShowImageTab();
		break;

	case 2:
		m_nTabType = PLUINFO_TYPE_HTML;
		ShowHTMLTab();
		break;

	case 3:
		m_nTabType = PLUINFO_TYPE_RESOURCE;
		ShowResourceTab();
		break;
	}
}

/**********************************************************************/

void CPropPagePluInfo::SaveTextTabInfo()
{
	DataManager.ResourceWrapperPlu.UpdateFileEdit002( PLUINFO_TYPE_TEXT, m_editText );
}

/**********************************************************************/

void CPropPagePluInfo::SaveHTMLTabInfo()
{
	DataManager.ResourceWrapperPlu.UpdateFileEdit002( PLUINFO_TYPE_HTML, m_editText );
}

/**********************************************************************/

void CPropPagePluInfo::OnButtonSelect() 
{
	if ( TRUE == m_bAllowChange )
	{
		switch( m_nTabType )
		{
		case PLUINFO_TYPE_TEXT:		SelectTextFile();		break;
		case PLUINFO_TYPE_IMAGE:	SelectImageFile();		break;
		case PLUINFO_TYPE_HTML:		SelectHTMLFile();		break;
		case PLUINFO_TYPE_RESOURCE:	SelectResourceFile();	break;
		}
	}
}

/**********************************************************************/

void CPropPagePluInfo::SelectTextFile()
{
	CFileDialog dlgFile ( TRUE, "", "", OFN_NOCHANGEDIR|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, "Text Files (*.txt)|*.txt||", NULL );
	dlgFile.m_ofn.lpstrTitle = "Select Text File";
	dlgFile.m_ofn.lpstrInitialDir = m_strInitialDir;
	
	if ( dlgFile.DoModal() == IDOK )
	{
		CString strFilepath = dlgFile.GetPathName();
		CString strFilename = dlgFile.GetFileName();
		CString strExt = dlgFile.GetFileExt();

		int nPathLen = strFilepath.GetLength();
		int nFileLen = strFilename.GetLength();

		if ( nPathLen >= nFileLen )
			m_strInitialDir = strFilepath.Left ( nPathLen - nFileLen );
			
		strExt.MakeUpper();
		if ( strExt == "TXT" )
		{
			DataManager.ResourceWrapperPlu.UpdateFileEdit002( PLUINFO_TYPE_TEXT, strFilepath );
			ShowTextTab();
		}
	}
}

/**********************************************************************/

void CPropPagePluInfo::SelectImageFile()
{
	CFileDialog dlgFile ( TRUE, "", "", OFN_NOCHANGEDIR|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, "Image Files (*.bmp;*.gif;*.jpg)|*.bmp;*.gif;*.jpg||", NULL );
	dlgFile.m_ofn.lpstrTitle = "Select Image File";
	dlgFile.m_ofn.lpstrInitialDir = m_strInitialDir;
	
	if ( dlgFile.DoModal() == IDOK )
	{
		CString strFilepath = dlgFile.GetPathName();
		CString strFilename = dlgFile.GetFileName();
		CString strExt = dlgFile.GetFileExt();

		int nPathLen = strFilepath.GetLength();
		int nFileLen = strFilename.GetLength();

		if ( nPathLen >= nFileLen )
			m_strInitialDir = strFilepath.Left ( nPathLen - nFileLen );
			
		strExt.MakeUpper();
		if ( strExt == "BMP" || strExt == "GIF" || strExt == "JPG" )
		{
			DataManager.ResourceWrapperPlu.UpdateFileEdit002( PLUINFO_TYPE_IMAGE, strFilepath );
			ShowImageTab();
		}
	}
}

/**********************************************************************/

void CPropPagePluInfo::SelectHTMLFile()
{
	CFileDialog dlgFile ( TRUE, "", "", OFN_NOCHANGEDIR|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, "HTML Files (*.htm)|*.htm||", NULL );
	dlgFile.m_ofn.lpstrTitle = "Select HTML File";
	dlgFile.m_ofn.lpstrInitialDir = m_strInitialDir;
	
	if ( dlgFile.DoModal() == IDOK )
	{
		CString strFilepath = dlgFile.GetPathName();
		CString strFilename = dlgFile.GetFileName();
		CString strExt = dlgFile.GetFileExt();

		int nPathLen = strFilepath.GetLength();
		int nFileLen = strFilename.GetLength();

		if ( nPathLen >= nFileLen )
			m_strInitialDir = strFilepath.Left ( nPathLen - nFileLen );
			
		strExt.MakeUpper();
		if ( strExt == "HTM" )
		{
			DataManager.ResourceWrapperPlu.UpdateFileEdit002( PLUINFO_TYPE_HTML, strFilepath );
			ShowHTMLTab();
		}
	}
}

/**********************************************************************/

void CPropPagePluInfo::SelectResourceFile()
{
	CFileDialog dlgFile ( TRUE, "", "", OFN_NOCHANGEDIR|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, "All Files (*.*)|*.*||", NULL );
	dlgFile.m_ofn.lpstrTitle = "Select HTML Resource File";
	dlgFile.m_ofn.lpstrInitialDir = m_strInitialDir;
	
	if ( dlgFile.DoModal() == IDOK )
	{
		CString strFilepath = dlgFile.GetPathName();
		CString strFilename = dlgFile.GetFileName();
		
		int nPathLen = strFilepath.GetLength();
		int nFileLen = strFilename.GetLength();

		if ( nPathLen >= nFileLen )
			m_strInitialDir = strFilepath.Left ( nPathLen - nFileLen );
			
		SelectResourceListItem( DataManager.ResourceWrapperPlu.AddResourceFile( strFilepath, nFileLen ) );

		ShowSelectedResource();
	}
}

/**********************************************************************/

void CPropPagePluInfo::OnButtonDelete() 
{
	if ( TRUE == m_bAllowChange )
	{
		switch( m_nTabType )
		{
		case PLUINFO_TYPE_TEXT:		DeleteTextFile();		break;
		case PLUINFO_TYPE_IMAGE:	DeleteImageFile();		break;
		case PLUINFO_TYPE_HTML:		DeleteHTMLFile();		break;
		case PLUINFO_TYPE_RESOURCE:	DeleteResourceFile();	break;
		}
	}
}

/**********************************************************************/

void CPropPagePluInfo::DeleteTextFile()
{
	if ( Prompter.YesNo( "Are you sure you wish to delete this text file", FALSE ) == IDYES )
	{
		DataManager.ResourceWrapperPlu.DeleteFileEdit002( PLUINFO_TYPE_TEXT );
		DataManager.ResourceWrapperPlu.SetSavedAsBlankFlag( PLUINFO_TYPE_TEXT, TRUE );
		ShowTextTab();
	}
}

/**********************************************************************/

void CPropPagePluInfo::DeleteImageFile()
{
	if ( Prompter.YesNo( "Are you sure you wish to delete this image file", FALSE ) == IDYES )
	{
		DataManager.ResourceWrapperPlu.DeleteFileEdit002( PLUINFO_TYPE_IMAGE );
		DataManager.ResourceWrapperPlu.SetSavedAsBlankFlag( PLUINFO_TYPE_IMAGE, TRUE );
		ShowImageTab();
	}
}

/**********************************************************************/

void CPropPagePluInfo::DeleteHTMLFile()
{
	if ( Prompter.YesNo( "Are you sure you wish to delete this HTML file", FALSE ) == IDYES )
	{
		DataManager.ResourceWrapperPlu.DeleteFileEdit002( PLUINFO_TYPE_HTML );
		DataManager.ResourceWrapperPlu.SetSavedAsBlankFlag( PLUINFO_TYPE_HTML, TRUE );
		ShowHTMLTab();
	}
}

/**********************************************************************/

void CPropPagePluInfo::DeleteResourceFile()
{
	int nSel = m_listFiles.GetSelectionMark();
	if ( ( nSel >= 0 ) && ( nSel < DataManager.ResourceWrapperPlu.GetResourceFileCount() ) )
	{
		CString strMsg = "";
		strMsg += "Are you sure you want to delete this file\n\n";
		strMsg += DataManager.ResourceWrapperPlu.GetResourceFileName( nSel );

		if ( Prompter.YesNo( strMsg, FALSE ) == IDYES )
		{
			DataManager.ResourceWrapperPlu.DeleteResourceFile( nSel );
			
			if ( nSel < DataManager.ResourceWrapperPlu.GetResourceFileCount() )
				SelectResourceListItem( nSel );
			else if ( nSel > 0 )
				SelectResourceListItem( nSel - 1 );
			
			ShowSelectedResource();
		}
	}
}

/**********************************************************************/

void CPropPagePluInfo::SelectResourceListItem( int nSel )
{
	m_listFiles.SetItemCountEx( DataManager.ResourceWrapperPlu.GetResourceFileCount() );
	m_listFiles.SetSelectionMark( nSel );
	m_listFiles.SetItemState( nSel, LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
	m_listFiles.EnsureVisible( nSel, FALSE );
	m_listFiles.Invalidate();
}

/**********************************************************************/
