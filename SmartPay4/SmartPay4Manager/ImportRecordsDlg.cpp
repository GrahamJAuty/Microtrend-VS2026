//$$******************************************************************
#include "SmartPay4Manager.h"
//$$******************************************************************
#include "AuditReportWrapper.h"
#include "ReportCSVFile.h"
//$$******************************************************************
#include "ImportRecordsDlg.h"
//$$******************************************************************
extern CSysset Sysset;
//$$******************************************************************

CImportRecordsDlg::CImportRecordsDlg(const char* szCaption, const char* szFilename, CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CImportRecordsDlg::IDD, pParent)
{
	m_strCaption = szCaption;
	m_strImportFilename = szFilename;
	m_pParent = pParent;
	m_nImportMethod = 0;
	m_bDeleteFile = FALSE;

	m_bEnableSelective = TRUE;						// can be set externally
	m_bShowDeleteFile = TRUE;
	m_strFileType = "csv";							// csv / dat can be changed externally
}

//$$******************************************************************

CImportRecordsDlg::~CImportRecordsDlg()
{
}

//$$******************************************************************

void CImportRecordsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_VIEW, m_buttonView);
	DDX_Control(pDX, IDC_CHECK_DELETEFILE, m_checkDeleteFile);
	DDX_Control(pDX, IDC_RADIO_IMPORTALL, m_radioImportAll);
	DDX_Control(pDX, IDC_RADIO_NEW, m_radioImportNew);
	DDX_Control(pDX, IDC_RADIO_EXISTING, m_radioImportExisting);
	DDX_Control(pDX, IDOK, m_buttonOK);
	/*****/
	DDX_Radio(pDX, IDC_RADIO_IMPORTALL, m_nImportMethod);
	DDX_Check(pDX, IDC_CHECK_DELETEFILE, m_bDeleteFile);
	DDX_Text(pDX, IDC_EDIT_IMPORTFILENAME, m_strImportFilename);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CImportRecordsDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnBnClickedButtonView)
	ON_EN_KILLFOCUS(IDC_EDIT_IMPORTFILENAME, OnEnKillfocusEditImportfilename)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnBnClickedButtonBrowse)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CImportRecordsDlg::CSSAutoShutdownPostInitDialog()
{
	SetWindowText ( m_strCaption );
	SetDisplay();
	return TRUE;
}

//$$******************************************************************

void CImportRecordsDlg::OnEnKillfocusEditImportfilename()	
{ 
	SetDisplay();	
}

//$$******************************************************************

void CImportRecordsDlg::SetDisplay()
{
	if ( UpdateData() == TRUE )
	{
		bool bAllowed = ::FileExists ( m_strImportFilename );

		m_buttonView.EnableWindow(bAllowed);
		m_buttonOK.EnableWindow(bAllowed);
		m_checkDeleteFile.EnableWindow(bAllowed);

		if (m_bEnableSelective == FALSE)
		{
			bAllowed = FALSE;
		}

		m_radioImportAll.EnableWindow(bAllowed);
		m_radioImportNew.EnableWindow(bAllowed);
		m_radioImportExisting.EnableWindow(bAllowed);

		if (m_bShowDeleteFile == FALSE)
		{
			m_checkDeleteFile.ShowWindow(SW_HIDE);
		}
	}
}

//$$******************************************************************

void CImportRecordsDlg::OnBnClickedButtonView()
{
	if ( UpdateData() == TRUE )
	{
		if ( m_strFileType == "csv" )
		{
			CCSVFileReport csv ( "R119", m_strImportFilename, m_pParent );
			csv.Show();
		}
		else
		{
			CAuditReportWrapper Wrapper ( m_strImportFilename, "R129", 0, this );
			Wrapper.ShowReport();
		}
	}
}

//$$******************************************************************

void CImportRecordsDlg::OnBnClickedButtonBrowse()
{
	if ( UpdateData() == TRUE )
	{
		CString strMask;
		CString strDefaultName = "";
		if ( m_strFileType == "csv" )
		{
			strMask = "CSV import file (*.csv)|*.csv|All Files (*.*)|*.*||";
			strDefaultName = szFILENAME_IMPORTCSV;
		}
		else
		{
			strMask = "Import file (*.dat)|*.dat|All Files (*.*)|*.*||";
		}
				
		CFileDialog dlg ( TRUE, m_strFileType, strDefaultName, OFN_HIDEREADONLY|OFN_NOCHANGEDIR, strMask, this );
		dlg.m_ofn.lpstrTitle = "Locate Import file";
		dlg.m_ofn.lpstrInitialDir = Filenames.GetImportsPath();

		if ( DoFileDialog ( dlg ) == IDOK )
		{
			m_strImportFilename = dlg.GetPathName();
			UpdateData(FALSE);
			SetDisplay();
		}
	}
}

//$$******************************************************************

void CImportRecordsDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		EndDialog(IDOK);
	}
}

//$$******************************************************************
