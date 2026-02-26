/**********************************************************************/
#include "Prompter.h"
/**********************************************************************/
#include "SPOSKeyboardImageImportDlg.h"
/**********************************************************************/

CSPOSKeyboardImageImportDlg::CSPOSKeyboardImageImportDlg(CWnd* pParent)
	: CDialog(CSPOSKeyboardImageImportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSPOSKeyboardImageImportDlg)
	//}}AFX_DATA_INIT
	m_strNewSelectedImage = "";
	m_strInitialDir = ".\\";
}

/**********************************************************************/

void CSPOSKeyboardImageImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardImageImportDlg)
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_editWidth);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_editHeight);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardImageImportDlg, CDialog)
	//{{AFX_MSG_MAP(CSPOSKeyboardImageImportDlg)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnSelectListItem)
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_FOLDER, OnButtonFolder)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardImageImportDlg::OnInitDialog() 
{
	m_staticImage.SubclassDlgItem(IDC_STATIC_IMAGE,this);
	CDialog::OnInitDialog();
	
	m_listFiles.SubclassDlgItem ( IDC_LIST, this );
	m_listFiles.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listFiles.InsertColumn ( 0, "Files", LVCFMT_LEFT, 190 );
	m_listFiles.SetLockItemZeroFlag( FALSE );

	m_editWidth.SetWindowText( "" );
	m_editHeight.SetWindowText( "" );
	return TRUE;  
}

/**********************************************************************/

void CSPOSKeyboardImageImportDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( nIDCtl == IDC_LIST )	
	{
		m_listFiles.AdjustRowHeight( lpMeasureItemStruct, 10 );
	}
}

/**********************************************************************/

void CSPOSKeyboardImageImportDlg::AddFileToList( int nPos, const char* szText )
{
	CSSListTaggedSelectItem item;
	item.m_strText = szText;
	item.m_nData = 0;
	item.SetTagState(0);
	m_listFiles.InsertItemIntoList( nPos, item );
}

/**********************************************************************/

void CSPOSKeyboardImageImportDlg::OnButtonFile() 
{
	CFileDialog dlgFile ( TRUE, "", "", OFN_NOCHANGEDIR|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, "Image Files (*.bmp;*.gif;*.jpg)|*.bmp;*.gif;*.jpg||", NULL );
	dlgFile.m_ofn.lpstrTitle = "Import Image File";
	dlgFile.m_ofn.lpstrInitialDir = m_strInitialDir;
	
	if ( dlgFile.DoModal() != IDOK )
		return;

	CString strFilepath = dlgFile.GetPathName();
	CString strFilename = dlgFile.GetFileName();
	CString strExt = dlgFile.GetFileExt();

	int nPathLen = strFilepath.GetLength();
	int nFileLen = strFilename.GetLength();

	if ( nPathLen >= nFileLen )
		m_strInitialDir = strFilepath.Left ( nPathLen - nFileLen );
		
	strExt.MakeUpper();
	if ( strExt != "BMP" && strExt != "GIF" && strExt != "JPG" )
		return;

	if ( strFilename.GetLength() > MAX_BITMAP_FILENAME + 4 )
	{
		strFilename.MakeUpper();

		CString strMsg;
		strMsg.Format( "Unable to import the image file : %s\n\nSPos V4 cannot handle image filenames of more than %d characters.", 
			(const char*) strFilename,
			MAX_BITMAP_FILENAME );

		Prompter.Error( strMsg );
		return;
	}

	CSortedImageInfo infoImage;
	infoImage.m_strFileName = strFilename;

	int nPos;
	if ( m_arrayImageInfo.Find( infoImage, nPos ) == FALSE )
	{
		AddFileToList( nPos, strFilename ); 

		infoImage.m_strFilePath = strFilepath;
		infoImage.m_bImageOK = TRUE;
		m_arrayImageInfo.InsertAt( nPos, infoImage );

		m_listFiles.SetItemTagState( nPos, 1 );
		m_listFiles.Update( nPos );
	}

	m_listFiles.SetCurSel( nPos );
	SelectListItem();
}

/**********************************************************************/

void CSPOSKeyboardImageImportDlg::OnButtonFolder() 
{
	CString strPathname = "";
	if ( BrowseFolder ( strPathname, "Select Image Folder", NULL, this ) == TRUE )
	{
		CFileFind FileFinder;
		bool bWorking = ( FileFinder.FindFile ( strPathname + "\\*.*" ) != 0 );
	
		int nPos = 0;
		while (bWorking)   		
		{
			( bWorking = FileFinder.FindNextFile() != 0 );

			//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
			if ( FileFinder.IsDots() == TRUE )
				continue;

			//WE ONLY NEED TO CHECK FILES
			if ( FileFinder.IsDirectory() == TRUE )
				continue;

			CString strFilename = FileFinder.GetFileName();
			if ( strFilename.GetLength() < 5 || strFilename.GetLength() > MAX_BITMAP_FILENAME + 4 )
				continue;

			strFilename.MakeUpper();
		
			bool bAccept = FALSE;
			if ( strFilename.Right(4) == ".BMP" )		bAccept = TRUE;
			else if ( strFilename.Right(4) == ".GIF" )	bAccept = TRUE;
			else if ( strFilename.Right(4) == ".JPG" )	bAccept = TRUE;

			if ( bAccept == FALSE )
				continue;

			strFilename = FileFinder.GetFileName();
			CString strFilepath = FileFinder.GetFilePath();

			CSortedImageInfo infoImage;
			infoImage.m_strFileName = strFilename;

			int nPos;
			if ( m_arrayImageInfo.Find( infoImage, nPos ) == FALSE )
			{
				AddFileToList( nPos, strFilename ); 

				infoImage.m_strFilePath = strFilepath;
				infoImage.m_bImageOK = TRUE;
				m_arrayImageInfo.InsertAt( nPos, infoImage );

				m_listFiles.SetItemTagState( nPos, 1 );
				m_listFiles.Update( nPos );
			}

			m_listFiles.SetCurSel( nPos );
		}
		SelectListItem();
	}
}

/**********************************************************************/

void CSPOSKeyboardImageImportDlg::OnSelectListItem(NMHDR* pNMHDR, LRESULT* pResult)  
{
	SelectListItem();
	*pResult = 0;
}

/**********************************************************************/

void CSPOSKeyboardImageImportDlg::SelectListItem()
{
	int nSel = m_listFiles.GetCurSel();
	if ( nSel >= 0 && nSel < m_arrayImageInfo.GetSize() )
	{
		CSortedImageInfo info;
		m_arrayImageInfo.GetAt(nSel,info);

		CString strFile = info.m_strFilePath;

		if ( m_staticImage.LoadImage( strFile ) == TRUE )
		{
			info.m_bImageOK = TRUE;
			m_arrayImageInfo.SetAt( nSel, info );

			m_staticImage.SetFailedFlag( FALSE );
			m_staticImage.SetBlankFlag( FALSE );
			m_staticImage.Invalidate();
			m_editWidth.SetWindowText( m_staticImage.GetWidthString() );
			m_editHeight.SetWindowText( m_staticImage.GetHeightString() );
		}
		else
		{
			info.m_bImageOK = FALSE;
			m_arrayImageInfo.SetAt( nSel, info );

			m_staticImage.SetFailedFlag( TRUE );
			m_staticImage.SetBlankFlag( TRUE );
			m_staticImage.Invalidate();
			m_editWidth.SetWindowText( "" );
			m_editHeight.SetWindowText( "" );
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardImageImportDlg::OnButtonAll() 
{
	m_listFiles.SelectAll();
}

/**********************************************************************/

void CSPOSKeyboardImageImportDlg::OnButtonNone() 
{
	m_listFiles.SelectNone();
}

/**********************************************************************/

void CSPOSKeyboardImageImportDlg::OnOK() 
{
	bool bGotSelection = FALSE;
	m_strNewSelectedImage = "";

	for ( int n = 0; n < m_listFiles.GetItemCount(); n++ )
	{
		if ( m_listFiles.GetItemTagState(n) != 0 )
		{
			if ( n < m_arrayImageInfo.GetSize() )
			{
				CSortedImageInfo info;
				m_arrayImageInfo.GetAt(n,info);

				if ( TRUE == info.m_bImageOK )
				{
					CString strSource = info.m_strFilePath;
					
					CString strDest = SPOSKeyboardManager.GetLayoutOptions() -> GetImageFolder();
					strDest += "\\";
					strDest += info.m_strFileName;

					CopyFile( strSource, strDest, FALSE );

					if ( FALSE == bGotSelection )
					{
						m_strNewSelectedImage = info.m_strFileName;
						bGotSelection = TRUE;
					}
					else
					{
						m_strNewSelectedImage = "";
					}
				}
			}
		}
	}

	EndDialog( IDOK );
}

/**********************************************************************/
