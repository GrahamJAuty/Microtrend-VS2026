//*******************************************************************
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//*******************************************************************
#include "ReportCSVFile.h"
//*******************************************************************
#include "SetupCsvImportPropertyPage2.h"
//*******************************************************************

CSetupCsvImportPropertyPage2::CSetupCsvImportPropertyPage2()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CSetupCsvImportPropertyPage2::IDD)
{
	m_pData = NULL;
	m_nLocateOn = 0;
	m_bEnableImport = FALSE;
	m_bAddUnknowns = FALSE;
	m_bDeleteFile = FALSE;
	m_strImportFilename = "";
	m_strStartFromUserID = "1";
}

//*******************************************************************

void CSetupCsvImportPropertyPage2::SetPointer ( CCsvImportData* pData )
{
	m_pData = pData;
}

//*******************************************************************

void CSetupCsvImportPropertyPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LOCATE, m_comboLocate);
	DDX_Control(pDX, IDC_EDIT_HEADER, m_editHeader);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
	DDX_Control(pDX, IDC_BUTTON_VIEW, m_buttonView);
	DDX_Control(pDX, IDC_CHECK_ADDUNKNOWNS, m_checkAddUnknowns);
	DDX_Control(pDX, IDC_CHECK_DELETEFILE, m_checkDeleteFile);
	DDX_Control(pDX, IDC_EDIT_IMPORTFILENAME, m_editImportFilename);
	DDX_Control(pDX, IDC_EDIT_CARDNO, m_editCardNo);
	DDX_Text(pDX, IDC_EDIT_IMPORTFILENAME, m_strImportFilename);
	DDX_CBIndex(pDX, IDC_COMBO_LOCATE, m_nLocateOn);
	DDX_Check(pDX, IDC_CHECK_DELETEFILE, m_bDeleteFile);
	DDX_Check(pDX, IDC_CHECK_ENABLE, m_bEnableImport);
	DDX_Check(pDX, IDC_CHECK_ADDUNKNOWNS, m_bAddUnknowns);
	DDX_Text(pDX, IDC_EDIT_CARDNO, m_strStartFromUserID);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CSetupCsvImportPropertyPage2, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_EN_KILLFOCUS(IDC_EDIT_IMPORTFILENAME, OnKillFocusImportFilename)
	ON_BN_CLICKED(IDC_CHECK_ENABLE, OnToggleEnable)
	ON_BN_CLICKED(IDC_CHECK_ADDUNKNOWNS, OnToggleAddUnknowns)
END_MESSAGE_MAP()

//*********************************************************************
// CSetupCsvImportPropertyPage2 message handlers

BOOL CSetupCsvImportPropertyPage2::OnInitDialog()
{
	m_bEnableImport = m_pData->m_bEnableTopupImport;
	m_strImportFilename = m_pData->m_strTopupFilename;
	m_nLocateOn = m_pData->m_nTopupLocateOn;
	m_bAddUnknowns = m_pData->m_bTopupAddUnknown;
	m_strStartFromUserID = m_pData->m_strTopupNextNewUserID;
	m_bDeleteFile = m_pData->m_bDeleteTopupFile;

	CPropertyPage::OnInitDialog();

	SubclassEdit(IDC_EDIT_CACHE, SS_NUM, 3, "%d");

	m_editHeader.SetWindowText(m_pData->GetMandatoryHeaderTopUp());

	CSQLRepositoryUserText RepoText;

	m_comboLocate.AddString(Account::UserID.Label);
	m_comboLocate.AddString(RepoText.GetDisplayMemberText());

	for (int n = 1; n <= nMAX_INFOTEXTS; n++)
	{
		m_comboLocate.AddString(RepoText.GetDisplayInfoText(n));
	}

	if (System.GetInterfaceType() == nINTERFACE_MIFAREv2 || System.GetInterfaceType() == nINTERFACE_DUAL)
	{
		m_comboLocate.AddString(RepoText.GetDisplayExtRef1Text());
		m_comboLocate.AddString(RepoText.GetDisplayExtRef2Text());
	}

	if (m_nLocateOn >= m_comboLocate.GetCount())
	{
		m_nLocateOn = 0;
	}

	m_comboLocate.SetCurSel ( m_nLocateOn );

	SetEditBoxInt(*GetEditCache(), m_pData->GetFilenameCacheHoursTopUp());

	SetDisplay();

	return TRUE;
}

//*********************************************************************

BOOL CSetupCsvImportPropertyPage2::OnKillActive()
{
	if (UpdateData() == TRUE)
	{
		m_pData->m_bEnableTopupImport = (m_bEnableImport != 0);
		m_pData->m_strTopupFilename = m_strImportFilename;
		m_pData->m_nTopupLocateOn = m_nLocateOn;
		m_pData->m_bTopupAddUnknown = (m_bAddUnknowns != 0);
		m_pData->m_strTopupNextNewUserID = m_strStartFromUserID;
		m_pData->m_bDeleteTopupFile = (m_bDeleteFile != 0);
		m_pData->SetMandatoryHeaderTopUp(GetEditBoxText(m_editHeader));
		m_pData->SetFilenameCacheHoursTopUp(GetEditBoxInt(*GetEditCache()));

		return CPropertyPage::OnKillActive();
	}
	return FALSE;
}

//*******************************************************************

void CSetupCsvImportPropertyPage2::OnToggleEnable()	
{ 
	SetDisplay();
}

//*******************************************************************

void CSetupCsvImportPropertyPage2::OnKillFocusImportFilename()
{ 
	SetDisplay();	
}

//*******************************************************************

void CSetupCsvImportPropertyPage2::OnToggleAddUnknowns()	
{ 
	SetDisplay(); 
}

//*******************************************************************

void CSetupCsvImportPropertyPage2::SetDisplay()
{
	if (UpdateData() == TRUE)
	{
		BOOL bAllowed = ::FileExists(m_strImportFilename);
		m_buttonView.EnableWindow(bAllowed);

		bAllowed = (System.GetInterfaceType() == nINTERFACE_BIOMETRIC) ? FALSE : m_bEnableImport;
		m_checkAddUnknowns.EnableWindow(bAllowed);

		if (bAllowed == TRUE)
		{
			bAllowed = (m_bEnableImport == TRUE) ? m_bAddUnknowns : FALSE;
		}

		m_editCardNo.EnableWindow(bAllowed);

		BOOL bComboAllowed = (System.GetInterfaceType() == nINTERFACE_BIOMETRIC) ? FALSE : m_bEnableImport;
		m_comboLocate.EnableWindow(bComboAllowed);

		m_checkDeleteFile.EnableWindow(m_bEnableImport);
		m_editImportFilename.EnableWindow(m_bEnableImport);
		m_buttonBrowse.EnableWindow(m_bEnableImport);

		m_editHeader.EnableWindow(m_bEnableImport);
		GetEditCache()->EnableWindow(m_bEnableImport);
	}
}

//***************************************************************************

void CSetupCsvImportPropertyPage2::OnButtonBrowse()
{
	if ( UpdateData() == TRUE )
	{
		CFileDialog dlg ( TRUE, "csv", szFILENAME_IMPORTTOPUP, OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "CSV Topup import file (*.csv)|*.csv|All Files (*.*)|*.*||", this );
		dlg.m_ofn.lpstrTitle = "Locate CSV Topup Import file";
		dlg.m_ofn.lpstrInitialDir = Filenames.GetImportsPath();

		if ( DoFileDialog(dlg) == IDOK )
		{
			m_strImportFilename = dlg.GetPathName();
			UpdateData(FALSE);
			SetDisplay();
		}
	}
}

//***************************************************************************

void CSetupCsvImportPropertyPage2::OnButtonView()
{
	CCSVFileReport csv ( "R119", m_strImportFilename, this );
	csv.Show();
}

//***************************************************************************

