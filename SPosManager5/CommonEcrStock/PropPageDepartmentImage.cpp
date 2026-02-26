/**********************************************************************/
#include "PropPageDepartmentImage.h"
/**********************************************************************/

CPropPageDepartmentImage::CPropPageDepartmentImage() : CSSPropertyPage(CPropPageDepartmentImage::IDD)
{
	//{{AFX_DATA_INIT(CPropPageDepartmentImage)
	//}}AFX_DATA_INIT
	m_strInitialDir = ".\\";
}

/**********************************************************************/

CPropPageDepartmentImage::~CPropPageDepartmentImage()
{
}

/**********************************************************************/

void CPropPageDepartmentImage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageDepartmentImage)	
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_editWidth);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_editHeight);
	DDX_Control(pDX, IDC_EDIT_IMAGENAME, m_editImageName);
	DDX_Control(pDX, IDC_BUTTON_IMAGE, m_buttonImage);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_buttonDelete);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageDepartmentImage, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageDepartmentImage)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE, OnButtonImage)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageDepartmentImage::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();
	m_staticImage.SubclassDlgItem(IDC_STATIC_IMAGE,this);
	return TRUE;  
}

/**********************************************************************/

BOOL CPropPageDepartmentImage::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageDepartmentImage::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageDepartmentImage::GetRecordData()
{
}

/**********************************************************************/

void CPropPageDepartmentImage::SetRecordControls()
{
	if ( DataManager.ListKeyImageWrapperDept.SavedAsBlank() == TRUE )
	{
		m_staticImage.SetBlankFlag( TRUE );
		m_staticImage.SetFailedFlag( FALSE );
		m_buttonDelete.EnableWindow( FALSE );
		m_staticImage.Invalidate();
		m_editWidth.SetWindowText( "" );
		m_editHeight.SetWindowText( "" );
	}
	else
	{
		bool bFailed = FALSE;
		bool bGotFile = FALSE;

		{
			CString strImageFilename = m_pDeptRecord -> GetImageFilename();
			
			if ( strImageFilename != "" )
			{
				CString strFile = DataManager.ListKeyImageWrapperDept.GetFilePathVisible();

				if ( ::FileExists( strFile )== TRUE )
				{
					bGotFile = ( m_staticImage.LoadImage( strFile ) == TRUE );
					bFailed = ( FALSE == bGotFile );
				}
				else
				{
					bFailed = TRUE;
				}
			}
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

	m_editImageName.SetWindowText( m_pDeptRecord -> GetImageFilename() );
}

/**********************************************************************/

bool CPropPageDepartmentImage::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageDepartmentImage::SaveRecord()
{
}

/**********************************************************************/

void CPropPageDepartmentImage::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/

void CPropPageDepartmentImage::OnButtonImage() 
{
	CFileDialog dlgFile ( TRUE, "", "", OFN_NOCHANGEDIR|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, "Image Files (*.bmp;*.gif;*.jpg)|*.bmp;*.gif;*.jpg||", NULL );
	dlgFile.m_ofn.lpstrTitle = "Select Key Image File";
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
			DataManager.ListKeyImageWrapperDept.UpdateFileEdit002( strFilepath, nFileLen );
			m_pDeptRecord -> SetImageFilename( DataManager.ListKeyImageWrapperDept.GetNewKeyImageFileName() );
			SetRecordControls();
		}
	}
}

/**********************************************************************/

void CPropPageDepartmentImage::OnButtonDelete() 
{
	if ( Prompter.YesNo( "Are you sure you wish to remove this image", FALSE ) == IDYES )
	{
		DataManager.ListKeyImageWrapperDept.DeleteFileEdit002();
		DataManager.ListKeyImageWrapperDept.SetSavedAsBlankFlag(TRUE );
		m_pDeptRecord -> SetImageFilename( "" );
		SetRecordControls();
	}
}

/**********************************************************************/