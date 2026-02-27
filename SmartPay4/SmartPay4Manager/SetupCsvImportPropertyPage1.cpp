//$$******************************************************************
#include "..\SmartPay4Shared\AlertText.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//$$******************************************************************
#include "ReportCSVFile.h"
//$$******************************************************************
#include "SetupCsvImportPropertyPage1.h"
//$$******************************************************************

CSetupCsvImportPropertyPage1::CSetupCsvImportPropertyPage1()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CSetupCsvImportPropertyPage1::IDD)
{
	m_pData = NULL;
}

//$$******************************************************************

CSetupCsvImportPropertyPage1::~CSetupCsvImportPropertyPage1()
{
}

//*******************************************************************

void CSetupCsvImportPropertyPage1::SetPointer ( CCsvImportData* pData )
{
	m_pData = pData;
}

//$$******************************************************************

void CSetupCsvImportPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LOCATE, m_comboLocate);
	DDX_Control(pDX, IDC_BUTTON_VIEW, m_buttonView);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
	DDX_Control(pDX, IDC_CHECK_DELETEFILE, m_checkDeleteFile);
	DDX_Control(pDX, IDC_CHECK_ENABLE, m_checkEnableImport);
	DDX_Control(pDX, IDC_RADIO_IMPORTALL, m_radioImportAll);
	DDX_Control(pDX, IDC_RADIO_NEW, m_radioImportNew);
	DDX_Control(pDX, IDC_RADIO_EXISTING, m_radioImportExisting);
	DDX_Control(pDX, IDC_EDIT_IMPORTFILENAME, m_editImportFilename);
	DDX_Control(pDX, IDC_EDIT_CARDNO, m_editStartFromUserID);
	DDX_Control(pDX, IDC_EDIT_HEADER, m_editHeader);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSetupCsvImportPropertyPage1, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED(IDC_RADIO_IMPORTALL, SetDisplay)
	ON_BN_CLICKED(IDC_RADIO_NEW, SetDisplay)
	ON_BN_CLICKED(IDC_RADIO_EXISTING, SetDisplay)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_EN_KILLFOCUS(IDC_EDIT_IMPORTFILENAME, OnKillFocusFilename)
	ON_BN_CLICKED(IDC_CHECK_ENABLE, OnToggleEnable)
END_MESSAGE_MAP()

//*********************************************************************
// CSetupCsvImportPropertyPage1 message handlers

BOOL CSetupCsvImportPropertyPage1::OnInitDialog()
{	
	CPropertyPage::OnInitDialog();

	SubclassEdit(IDC_EDIT_CACHE, SS_NUM, 3, "%d");

	m_checkEnableImport.SetCheck(m_pData->m_bEnableImport);
	m_checkDeleteFile.SetCheck(m_pData->m_bDeleteFile);
	m_editImportFilename.SetWindowText(m_pData->m_strImportFilename);
	m_editStartFromUserID.SetWindowText(m_pData->m_strStandardNextNewUserID);
	m_editHeader.SetWindowText(m_pData->GetMandatoryHeaderStandard());
	SetEditBoxInt(*GetEditCache(), m_pData->GetFilenameCacheHoursStandard());	

	{
		int nImportMethod = m_pData->m_nImportMethod;
		m_radioImportAll.SetCheck((nImportMethod != 1) && (nImportMethod != 2));
		m_radioImportNew.SetCheck(1 == nImportMethod);
		m_radioImportExisting.SetCheck(2 == nImportMethod);
	}

	CSQLRepositoryUserText RepoText;

	m_comboLocate.AddString ( Account::UserID.Label );
	m_comboLocate.AddString ( RepoText.GetDisplayMemberText() );

	for (int n = 1; n <= nMAX_INFOTEXTS; n++)
	{
		m_comboLocate.AddString(RepoText.GetDisplayInfoText(n));
	}

	if ( System.GetInterfaceType() == nINTERFACE_MIFAREv2 || System.GetInterfaceType() == nINTERFACE_DUAL )
	{
		m_comboLocate.AddString (RepoText.GetDisplayExtRef1Text() );
		m_comboLocate.AddString (RepoText.GetDisplayExtRef2Text() );
	}

	{
		int nLocateOn = m_pData->m_nLocateOn;
	
		if (nLocateOn >= m_comboLocate.GetCount())
		{
			nLocateOn = 0;
		}

		m_comboLocate.SetCurSel(nLocateOn);
	}

	SetDisplay();

	return TRUE;
}

//$$******************************************************************

BOOL CSetupCsvImportPropertyPage1::OnKillActive()
{
	int nImportMethod = 0;

	if (m_radioImportNew.GetCheck() != 0)
	{
		nImportMethod = 1;
	}

	if (m_radioImportExisting.GetCheck() != 0)
	{
		nImportMethod = 2;
	}

	m_pData->m_bEnableImport = IsTicked(m_checkEnableImport);
	m_pData->m_strImportFilename = GetEditBoxText(m_editImportFilename);
	m_pData->m_nImportMethod = nImportMethod;
	m_pData->m_nLocateOn = m_comboLocate.GetCurSel();
	m_pData->m_strStandardNextNewUserID = GetEditBoxText(m_editStartFromUserID);
	m_pData->SetMandatoryHeaderStandard(GetEditBoxText(m_editHeader));
	m_pData->SetFilenameCacheHoursStandard(GetEditBoxInt(*GetEditCache()));

	m_pData->m_bDeleteFile = IsTicked(m_checkDeleteFile);

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************
void CSetupCsvImportPropertyPage1::OnToggleEnable()			{	SetDisplay();	}
void CSetupCsvImportPropertyPage1::OnKillFocusFilename()	{	SetDisplay();	}
//$$******************************************************************

void CSetupCsvImportPropertyPage1::SetDisplay()
{
	bool bEnableImport = IsTicked(m_checkEnableImport);

	bool bComboAllowed = bEnableImport;
	bComboAllowed &= (System.GetInterfaceType() != nINTERFACE_BIOMETRIC);

	bool bStartFromAllowed = bComboAllowed;
	bStartFromAllowed &= (m_radioImportExisting.GetCheck() == 0);

	m_comboLocate.EnableWindow(bComboAllowed);
	m_editStartFromUserID.EnableWindow(bStartFromAllowed);

	CString strImportFilename = GetEditBoxText(m_editImportFilename);
	bool bViewAllowed = ::FileExists(strImportFilename);
	m_buttonView.EnableWindow(bViewAllowed);

	m_radioImportAll.EnableWindow(bEnableImport);
	m_radioImportNew.EnableWindow(bEnableImport);
	m_radioImportExisting.EnableWindow(bEnableImport);
	m_checkDeleteFile.EnableWindow(bEnableImport);
	m_editImportFilename.EnableWindow(bEnableImport);
	m_buttonBrowse.EnableWindow(bEnableImport);

	m_editHeader.EnableWindow(bEnableImport);
	GetEditCache()->EnableWindow(bEnableImport);
}

//***************************************************************************

void CSetupCsvImportPropertyPage1::OnButtonBrowse()
{
	CFileDialog dlg(TRUE, "csv", szFILENAME_IMPORTCSV, OFN_HIDEREADONLY | OFN_NOCHANGEDIR, "CSV import file (*.csv)|*.csv|All Files (*.*)|*.*||", this);
	dlg.m_ofn.lpstrTitle = "Locate CSV Import file";
	dlg.m_ofn.lpstrInitialDir = Filenames.GetImportsPath();

	if (DoFileDialog(dlg) == IDOK)
	{
		m_editImportFilename.SetWindowText(dlg.GetPathName());
		SetDisplay();
	}
}

//***************************************************************************

void CSetupCsvImportPropertyPage1::OnButtonView()
{
	CString strImportFilename = GetEditBoxText(m_editImportFilename);
	CCSVFileReport csv ( "R119", strImportFilename, this );
	csv.Show();
}

//***************************************************************************
