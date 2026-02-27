//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\ImportWondeFile.h"
#include "..\SmartPay4ManagerBgnd\ImportWondePhotoFiles.h"
//$$******************************************************************
#include "ImportWondeDlg.h"
//$$******************************************************************

CImportWondeDlg::CImportWondeDlg(CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CImportWondeDlg::IDD, pParent)
{
	m_data.Read();
	m_strImportFilename = m_data.m_strImportFilename;
	m_bDeleteFile = m_data.m_bDeleteImportFile;
}

//$$******************************************************************

void CImportWondeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_VIEW, m_buttonView);
	DDX_Control(pDX, IDC_BUTTON_VIEW2, m_buttonView2);
	DDX_Control(pDX, IDC_BUTTON_EXECUTE, m_buttonExecute);
	DDX_Control(pDX, IDC_BUTTON_EXECUTE2, m_buttonExecute2);
	DDX_Control(pDX, IDC_EDIT_IMPORTFILENAME, m_editImportFilename);
	DDX_Control(pDX, IDC_CHECK_DELETEFILE, m_checkDeleteFile);
	DDX_Text(pDX, IDC_EDIT_IMPORTFILENAME, m_strImportFilename);
	DDX_Check(pDX, IDC_CHECK_DELETEFILE, m_bDeleteFile);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CImportWondeDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_BUTTON_VIEW2, OnButtonView2)
	ON_BN_CLICKED(IDC_BUTTON_EXECUTE, OnButtonExecute)
	ON_BN_CLICKED(IDC_BUTTON_EXECUTE2, OnButtonExecute2)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_EN_KILLFOCUS(IDC_EDIT_IMPORTFILENAME, OnKillFocusImportFilename)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CImportWondeDlg::CSSAutoShutdownPostInitDialog()
{
	SetDisplay();
	return TRUE;
}

//$$******************************************************************

void CImportWondeDlg::OnKillFocusImportFilename()
{ 
	SetDisplay(); 
}

//$$******************************************************************

void CImportWondeDlg::SetDisplay()
{
	if ( UpdateData() == TRUE )
	{
// import file

		bool bAllowed = ::FileExists ( m_strImportFilename );
		m_buttonView.EnableWindow(bAllowed);
		m_checkDeleteFile.EnableWindow(bAllowed);
		m_buttonExecute.EnableWindow(bAllowed);

// photo import

		bAllowed = FALSE;
		CString strMask = m_data.m_strPhotoFolder;
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

		m_buttonView2.EnableWindow( bAllowed );
		m_buttonExecute2.EnableWindow( bAllowed );
	}
}

//***************************************************************

void CImportWondeDlg::OnButtonView()
{
	if ( UpdateData() == TRUE )
	{
		CImportWondeFile wonde( &m_data, this );
		wonde.ShowImports ( m_strImportFilename );
	}
}

//***************************************************************

void CImportWondeDlg::OnButtonView2()
{
	if ( UpdateData() == TRUE )
	{
		CImportWondePhotoFiles wonde( &m_data, this );
		wonde.ShowImports();
	}
}

//***************************************************************

void CImportWondeDlg::OnButtonExecute()
{
	if ( UpdateData() == TRUE )
	{
		m_data.m_bDeleteImportFile = ( m_bDeleteFile != 0 );

		CImportWondeFile wonde( &m_data, this );
		if (wonde.Import(m_strImportFilename) == TRUE)
		{
			SetDisplay();
		}
		else
		{
			CString strError = wonde.GetError();
			if (strError != "")
			{
				Prompter.FatalError(strError, "Import Wonde Update File");
			}
		}
	}
}

//*******************************************************************

void CImportWondeDlg::OnButtonExecute2()
{
	CImportWondePhotoFiles photo ( &m_data, this );
	photo.Import();														// will display import stats when finished

	SetDisplay();
}

//*******************************************************************

void CImportWondeDlg::OnButtonBrowse()
{
	if ( UpdateData() == TRUE )
	{
		CFileDialog dlg ( TRUE, "json", szFILENAME_IMPORTWONDE, OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "Import Wonde file (*.json)|*.json|All Files (*.*)|*.*||", this );
		dlg.m_ofn.lpstrTitle = "Locate Wonde JSON Import file";
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
