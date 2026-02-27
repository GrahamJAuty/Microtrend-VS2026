//*******************************************************************
#include "..\SmartPay4ManagerBgnd\ImportSimsFile.h"
#include "..\SmartPay4ManagerBgnd\ImportSimsPhotoFiles.h"
//*******************************************************************
#include "ReportCSVFile.h"
//*******************************************************************
#include "ImportSimsDlg.h"
//*******************************************************************

CImportSimsDlg::CImportSimsDlg(CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CImportSimsDlg::IDD, pParent)
{
	m_sims.Read();
	m_strImportFilename = m_sims.m_strImportFilename;
	m_bDeleteFile = m_sims.m_bDeleteImportFile;
}

//*******************************************************************

void CImportSimsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_VIEW, m_buttonView);
	DDX_Control(pDX, IDC_BUTTON_VIEW2, m_buttonView2);
	DDX_Control(pDX, IDC_BUTTON_VIEW3, m_buttonView3);
	DDX_Control(pDX, IDC_BUTTON_EXECUTE1, m_buttonExecute1);
	DDX_Control(pDX, IDC_BUTTON_EXECUTE2, m_buttonExecute2);
	DDX_Control(pDX, IDC_CHECK_DELETEFILE, m_checkDeleteFile);
	DDX_Text(pDX, IDC_EDIT_IMPORTFILENAME, m_strImportFilename);
	DDX_Check(pDX, IDC_CHECK_DELETEFILE, m_bDeleteFile);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CImportSimsDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_BUTTON_VIEW2, OnButtonView2)
	ON_BN_CLICKED(IDC_BUTTON_VIEW3, OnButtonView3)
	ON_BN_CLICKED(IDC_BUTTON_EXECUTE1, OnButtonExecute1)
	ON_BN_CLICKED(IDC_BUTTON_EXECUTE2, OnButtonExecute2)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_EN_KILLFOCUS(IDC_EDIT_IMPORTFILENAME, OnKillFocusImportFilename)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CImportSimsDlg::CSSAutoShutdownPostInitDialog()
{
	SetDisplay();
	return TRUE;
}

//*******************************************************************

void CImportSimsDlg::OnKillFocusImportFilename()
{ 
	SetDisplay(); 
}

//*******************************************************************

void CImportSimsDlg::SetDisplay()
{
	if ( UpdateData() == TRUE )
	{
// import file

		bool bAllowed = ::FileExists ( m_strImportFilename );
		m_buttonView.EnableWindow(bAllowed);
		m_buttonView2.EnableWindow(bAllowed);
		m_checkDeleteFile.EnableWindow(bAllowed);
		m_buttonExecute1.EnableWindow(bAllowed);

// photo import

		bAllowed = FALSE;
		CString strMask = m_sims.m_strSimsPhotoFolder;
		if ( strMask != "" )	
		{
			strMask += "\\*.*";

			CFileFind FileFinder;	
			BOOL bWorking = FileFinder.FindFile ( strMask );

			while ( bWorking )   
			{
				bWorking = FileFinder.FindNextFile();
				if (FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE)
				{
					continue;
				}

				bAllowed = TRUE;
				break;
			}
		}

		m_buttonView3.EnableWindow( bAllowed );
		m_buttonExecute2.EnableWindow( bAllowed );
	}
}

//*******************************************************************

void CImportSimsDlg::OnButtonView()
{
	if ( UpdateData() == TRUE )
	{
		CCSVFileReport report ( "R211", m_strImportFilename, this );
		report.Show ( "SIMS Raw Import File");
	}
}

//*******************************************************************

void CImportSimsDlg::OnButtonView2()
{
	if ( UpdateData() == TRUE )
	{
		CImportSimsFile simsFile ( &m_sims, this );
		simsFile.ShowImports ( m_strImportFilename );
	}
}

//*******************************************************************

void CImportSimsDlg::OnButtonExecute1()
{
	if ( UpdateData() == TRUE )
	{
		m_sims.m_bDeleteImportFile = ( m_bDeleteFile != 0 );

		CImportSimsFile simsFile ( &m_sims, this );
		if (simsFile.Import(m_strImportFilename) == TRUE)
		{
			SetDisplay();
		}
		else
		{
			CString strError = simsFile.GetError();
			if (strError != "")
			{
				Prompter.FatalError(simsFile.GetError(), "Import SIMS Update File");
			}
		}
	}
}

//*******************************************************************

void CImportSimsDlg::OnButtonView3()
{
	CImportSimsPhotoFiles simsPhoto ( &m_sims, this );
	simsPhoto.ShowImports();
}

//*******************************************************************

void CImportSimsDlg::OnButtonExecute2()
{
	CImportSimsPhotoFiles simsPhoto(&m_sims, this);

	if (simsPhoto.Import() == TRUE)
	{
		SetDisplay();
	}
	else
	{
		Prompter.Error(simsPhoto.GetError(), "Import SIMS Photo Files");
	}
}

//*******************************************************************

void CImportSimsDlg::OnButtonBrowse()
{
	if ( UpdateData() == TRUE )
	{
		CFileDialog dlg ( TRUE, "csv", szFILENAME_IMPORTSIMS, OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "SIMS import file (*.csv)|*.csv|All Files (*.*)|*.*||", this );
		dlg.m_ofn.lpstrTitle = "Locate SIMS Import file";
		dlg.m_ofn.lpstrInitialDir = Filenames.GetImportsPath();

		if ( DoFileDialog(dlg) == IDOK )
		{
			m_strImportFilename = dlg.GetPathName();
			UpdateData(FALSE);
			SetDisplay();
		}
	}
}

//*******************************************************************

