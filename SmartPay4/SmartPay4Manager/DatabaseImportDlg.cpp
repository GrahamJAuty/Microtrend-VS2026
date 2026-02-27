//*******************************************************************
#include "..\SmartPay4ManagerBgnd\ImportSimsFile.h"
#include "..\SmartPay4ManagerBgnd\ImportWondeFile.h"
//*******************************************************************
#include "ReportCSVFile.h"
#include "StandardCombos.h"
//*******************************************************************
#include "DatabaseImportDlg.h"
//*******************************************************************

CDatabaseImportDlg::CDatabaseImportDlg(CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CDatabaseImportDlg::IDD, pParent)
{
	m_strImportFilename = "";
	m_nCsvFileFormat = 0;
	m_nImportMethod = 0;
	m_bImportPhotoID = FALSE;
	m_data.Read();
}

//*******************************************************************

void CDatabaseImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CSVFORMAT, m_comboFileFormat);
	DDX_Control(pDX, IDC_BUTTON_VIEW, m_buttonView);
	DDX_Text(pDX, IDC_EDIT_IMPORTFILENAME, m_strImportFilename);
	DDX_CBIndex(pDX, IDC_COMBO_CSVFORMAT, m_nCsvFileFormat);
	DDX_Radio(pDX, IDC_RADIO_IMPORTALL, m_nImportMethod);
	DDX_Check(pDX, IDC_CHECK_PHOTOID, m_bImportPhotoID);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CDatabaseImportDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_EN_KILLFOCUS(IDC_EDIT_IMPORTFILENAME, OnKillFocusImportFilename)
END_MESSAGE_MAP()

//*******************************************************************

void CDatabaseImportDlg::CSSAutoShutdownPreInitDialog()
{
	m_nCsvFileFormat = m_data.m_nFileFormat;
	m_strImportFilename = m_data.m_strImportFilename;
	m_nImportMethod = m_data.m_nImportMethod;
	m_bImportPhotoID = m_data.m_bImportPhotoID;
}

//*******************************************************************

BOOL CDatabaseImportDlg::CSSAutoShutdownPostInitDialog()
{
	CStandardCombos::FillDatabaseImportCombo( m_comboFileFormat, m_nCsvFileFormat );
	SetDisplay();
	return TRUE;  
}

//*******************************************************************

void CDatabaseImportDlg::OnOK()
{
	if ( UpdateData() == TRUE )
	{
		m_data.m_nFileFormat		= m_nCsvFileFormat;
		m_data.m_strImportFilename	= m_strImportFilename;
		m_data.m_nImportMethod		= m_nImportMethod;
		m_data.m_bImportPhotoID		= ( m_bImportPhotoID != 0 );
		m_data.Save();

		EndDialog(IDOK);
	}
}

//*******************************************************************

void CDatabaseImportDlg::OnKillFocusImportFilename()
{ 
	SetDisplay();	
}

//*******************************************************************

void CDatabaseImportDlg::SetDisplay()
{
	if ( UpdateData() == TRUE )
	{
		BOOL bAllowed = ::FileExists ( m_strImportFilename );
		m_buttonView.EnableWindow ( bAllowed );
	}
}

//*******************************************************************

void CDatabaseImportDlg::OnButtonDefault()
{
	if ( UpdateData() == TRUE )
	{
		m_strImportFilename = Filenames.GetImportsPath(szFILENAME_IMPORTCSV);
		UpdateData(FALSE);
		SetDisplay();
	}
}

//*******************************************************************

void CDatabaseImportDlg::OnButtonView()
{
	if ( UpdateData() == TRUE )
	{
		if ( m_strImportFilename != "" )
		{
			if (::FileExists(m_strImportFilename) == FALSE)
			{
				Prompter.Error(m_strImportFilename, "File not found");
			}
			else
			{
				if ( m_nCsvFileFormat == nIMPORT_FORMAT_SIMS )
				{
					CSimsData sims;
					sims.Read();

					CImportSimsFile simsFile ( &sims, this );
					simsFile.SetImportMethod ( m_nImportMethod );
					simsFile.ShowImports ( m_strImportFilename );
				}

				else if ( m_nCsvFileFormat == nIMPORT_FORMAT_WONDE )
				{
					CWondeData wonde;
					wonde.Read();

					CImportWondeFile wondeFile ( &wonde, this );
					wondeFile.SetImportMethod ( m_nImportMethod );
					wondeFile.ShowImports ( m_strImportFilename );
				}

				else
				{
					CCSVFileReport report ( "R216", m_strImportFilename, this );
					report.Show ( "View Import File" );
				}
			}
		}
	}
}

//*******************************************************************

void CDatabaseImportDlg::OnButtonBrowse()
{
	if ( UpdateData() == TRUE )
	{
		CFileDialog dlg ( TRUE, "csv", szFILENAME_IMPORTCSV, OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "CSV import file (*.csv)|*.csv|All Files (*.*)|*.*||", this );
		dlg.m_ofn.lpstrTitle = "Specify Account CSV Import file";
		CString strn = Filenames.GetImportsPath();
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
