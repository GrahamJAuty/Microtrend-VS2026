//**********************************************************************
#include "AuditReportWrapper.h"
//**********************************************************************
#include "SetupCsvImportPropertyPage4.h"
//**********************************************************************

CSetupCsvImportPropertyPage4::CSetupCsvImportPropertyPage4()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CSetupCsvImportPropertyPage4::IDD)
{
}

//**********************************************************************

void CSetupCsvImportPropertyPage4::SetPointer( CCsvImportData* pData)
{
	m_pData = pData;
}

//**********************************************************************

void CSetupCsvImportPropertyPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
	DDX_Control(pDX, IDC_BUTTON_VIEW, m_buttonView);
	DDX_Control(pDX, IDC_EDIT_IMPORTFILENAME, m_editImportFilename);
	DDX_Control(pDX, IDC_RADIO_EXISTING, m_radioExisting);
	DDX_Control(pDX, IDC_RADIO_IMPORTALL, m_radioImportAll);
	DDX_Control(pDX, IDC_RADIO_NEW, m_radioNew);
	DDX_Text(pDX, IDC_EDIT_IMPORTFILENAME, m_strImportFilename);
	DDX_Check(pDX, IDC_CHECK_ENABLE, m_bEnableImport);
	DDX_Radio(pDX, IDC_RADIO_IMPORTALL, m_nImportMethod);
}

//**********************************************************************

BEGIN_MESSAGE_MAP(CSetupCsvImportPropertyPage4, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_ENABLE, OnToggleEnable)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
END_MESSAGE_MAP()

//*********************************************************************

BOOL CSetupCsvImportPropertyPage4::OnInitDialog()
{
	m_bEnableImport = m_pData->m_bEnableDbaseAuditImport;
	m_strImportFilename = m_pData->m_strDbaseAuditFilename;
	m_nImportMethod = m_pData->m_nDbaseImportMethod;

	CPropertyPage::OnInitDialog();

	m_buttonView.EnableWindow(FALSE);

	SetDisplay();

	return TRUE;
}

//**********************************************************************

BOOL CSetupCsvImportPropertyPage4::OnKillActive()
{
	if (UpdateData() == TRUE)
	{
		m_pData->m_bEnableDbaseAuditImport = (m_bEnableImport != 0);
		m_pData->m_strDbaseAuditFilename = m_strImportFilename;
		m_pData->m_nDbaseImportMethod = m_nImportMethod;

		return CPropertyPage::OnKillActive();
	}
	return FALSE;
}

//**********************************************************************

void CSetupCsvImportPropertyPage4::OnRadioImportall()	
{	
	SetDisplay();
}

//**********************************************************************

void CSetupCsvImportPropertyPage4::OnRadioNew()
{	
	SetDisplay();
}

//**********************************************************************

void CSetupCsvImportPropertyPage4::OnRadioExisting()	
{	
	SetDisplay();
}

//**********************************************************************

void CSetupCsvImportPropertyPage4::OnToggleEnable()
{
	SetDisplay();
	m_editImportFilename.SetFocus();
}

//**********************************************************************

void CSetupCsvImportPropertyPage4::SetDisplay()
{
	if (UpdateData() == TRUE)
	{
		bool bAllowed = ::FileExists(m_strImportFilename);
		m_buttonView.EnableWindow(bAllowed);

		m_editImportFilename.EnableWindow(m_bEnableImport);
		m_buttonBrowse.EnableWindow(m_bEnableImport);

		m_radioImportAll.EnableWindow(m_bEnableImport);
		m_radioNew.EnableWindow(m_bEnableImport);
		m_radioExisting.EnableWindow(m_bEnableImport);
	}
}

//**********************************************************************

void CSetupCsvImportPropertyPage4::OnButtonBrowse()
{
	if (UpdateData() == TRUE)
	{
		CFileDialog dlg(TRUE, "dat", "", OFN_HIDEREADONLY | OFN_NOCHANGEDIR, "Audit file (*.dat)|*.dat|All Files (*.*)|*.*||", this);
		dlg.m_ofn.lpstrTitle = "Locate Audit File";
		dlg.m_ofn.lpstrInitialDir = Filenames.GetImportsPath();

		if (DoFileDialog(dlg) == IDOK)
		{
			m_strImportFilename = dlg.GetPathName();
			UpdateData(FALSE);
			SetDisplay();
		}
	}
}

//**********************************************************************

void CSetupCsvImportPropertyPage4::OnButtonView()
{
	CAuditReportWrapper Wrapper(m_strImportFilename, "R129", 0, this);
	Wrapper.ShowReport();
}

//**********************************************************************
