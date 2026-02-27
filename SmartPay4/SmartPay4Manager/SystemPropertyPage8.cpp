//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\PhotoIDCopy.h"
//$$******************************************************************
#include "DbExportEditor.h"
//$$******************************************************************
#include "SystemPropertyPage8.h"
//$$******************************************************************

CSystemPropertyPage8::CSystemPropertyPage8()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CSystemPropertyPage8::IDD)
{
}

//$$******************************************************************

CSystemPropertyPage8::~CSystemPropertyPage8()
{
}

//$$******************************************************************

void CSystemPropertyPage8::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_CHECK_EXPORTPHOTO, m_checkExportPhoto );
	DDX_Control( pDX, IDC_EDIT_EPOSPHOTOFOLDER, m_editEposFolder );
	DDX_Control( pDX, IDC_CHECK_COPYFILES, m_checkCopyPhoto );
	DDX_Control( pDX, IDC_BUTTON_COPY, m_buttonCopyNow );
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSystemPropertyPage8, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_EXPORTPHOTO, OnToggleExportPhoto)
	ON_BN_CLICKED(IDC_CHECK_COPYFILES, OnToggleCopyPhoto)
	ON_BN_CLICKED(IDC_BUTTON_COPY, OnButtonCopyNow)
	/*****/
	ON_BN_CLICKED(IDC_BUTTON_ONDELETE2, OnButtonDeleteNotification)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CSystemPropertyPage8::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_checkExportPhoto.SetCheck( System.GetExportEposPhotoFlag() );
	m_editEposFolder.SetWindowText( System.GetEposPhotoIDFolder() );
	m_checkCopyPhoto.SetCheck( FALSE );

	SetDisplay();

	return TRUE;
}

//$$******************************************************************

BOOL CSystemPropertyPage8::OnKillActive()
{
	CString strEposFolder = "";
	m_editEposFolder.GetWindowText(strEposFolder);

	System.SetExportEposPhotoFlag(IsTicked(m_checkExportPhoto));
	System.SetEposPhotoIDFolder(strEposFolder);

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

void CSystemPropertyPage8::SetDisplay()
{
	bool bExportPhoto = ( m_checkExportPhoto.GetCheck() != 0 );
	bool bCopyPhoto = ( m_checkCopyPhoto.GetCheck() != 0 );

	m_editEposFolder.EnableWindow(bExportPhoto);
	m_checkCopyPhoto.EnableWindow(bExportPhoto);

	m_buttonCopyNow.EnableWindow( bExportPhoto & bCopyPhoto );

}

//$$******************************************************************

void CSystemPropertyPage8::OnToggleExportPhoto()
{
	SetDisplay();
	m_editEposFolder.SetFocus();
}

//$$******************************************************************

void CSystemPropertyPage8::OnToggleCopyPhoto()
{
	SetDisplay();
	m_buttonCopyNow.SetFocus();
}

//$$******************************************************************

void CSystemPropertyPage8::OnButtonCopyNow()
{
	CPhotoIDCopy photo{};
	int nCount = photo.EposExport();										// copy PhotoID to to Epos folder	

	CString strMsg = "";
	if (nCount == 0)
	{
		strMsg = "No files found in PhotoID folder!";
	}
	else
	{
		strMsg.Format("%d files copied from PhotoID folder to Epos folder.", nCount);
	}

	Prompter.Info( strMsg, "Copy PhotoID files" );
}

//$$******************************************************************

void CSystemPropertyPage8::OnButtonDeleteNotification()
{
	CDbExportEditor editor ( this );
	editor.EditExportLabel ( "DEL2"  );							// biometric deletion file
}

//$$******************************************************************
