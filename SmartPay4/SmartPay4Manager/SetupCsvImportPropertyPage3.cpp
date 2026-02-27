//**********************************************************************
#include "..\SmartPay4Shared\SmartPayHeaderHelpers.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//**********************************************************************
#include "ReportCSVFile.h"
#include "EditCustomListDlg.h"
//**********************************************************************
#include "SetupCsvImportPropertyPage3.h"
//**********************************************************************

CSetupCsvImportPropertyPage3::CSetupCsvImportPropertyPage3()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CSetupCsvImportPropertyPage3::IDD), m_dbCustomList("")
{
}

//**********************************************************************

CSetupCsvImportPropertyPage3::~CSetupCsvImportPropertyPage3()
{
	m_dbCustomList.Close();
}

//**********************************************************************

void CSetupCsvImportPropertyPage3::SetPointer ( CCsvImportData* pData )
{
	m_pData = pData;
}

//**********************************************************************

void CSetupCsvImportPropertyPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LOCATE, m_comboLocate);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
	DDX_Control(pDX, IDC_BUTTON_VIEW, m_buttonView);
	DDX_Control(pDX, IDC_CHECK_DELETEFILE, m_checkDeleteFile);
	DDX_Control(pDX, IDC_CHECK_IGNORECASE, m_checkIgnoreCase);
	DDX_Control(pDX, IDC_EDIT_CARDNO, m_editCardNo);
	DDX_Control(pDX, IDC_EDIT_IMPORTFILENAME, m_editImportFilename);
	DDX_Control(pDX, IDC_EDIT_MENUTEXT, m_editMenuText);
	DDX_Control(pDX, IDC_RADIO_EXISTING, m_radioExisting);
	DDX_Control(pDX, IDC_RADIO_IMPORTALL, m_radioImportAll);
	DDX_Control(pDX, IDC_RADIO_NEW, m_radioNew);
	DDX_CBIndex(pDX, IDC_COMBO_LOCATE, m_nLocateOn);
	DDX_Text(pDX, IDC_EDIT_IMPORTFILENAME, m_strImportFilename);
	DDX_Check(pDX, IDC_CHECK_DELETEFILE, m_bDeleteFile);
	DDX_Check(pDX, IDC_CHECK_IGNORECASE, m_bIgnoreCase);
	DDX_Check(pDX, IDC_CHECK_ENABLE, m_bEnableCustomImport);
	DDX_Radio(pDX, IDC_RADIO_IMPORTALL, m_nImportMethod);
	DDX_Text(pDX, IDC_EDIT_MENUTEXT, m_strMenuText);
	DDX_Text(pDX, IDC_EDIT_CARDNO, m_strStartFromUserID);
}

//**********************************************************************

BEGIN_MESSAGE_MAP(CSetupCsvImportPropertyPage3, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_CHECK_ENABLE, OnToggleEnable)
	ON_EN_KILLFOCUS(IDC_EDIT_IMPORTFILENAME, OnKillFocusImportFilename)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDoubleClickList)
	ON_BN_CLICKED(IDC_RADIO_NEW, SetDisplay)
	ON_BN_CLICKED(IDC_RADIO_EXISTING, SetDisplay)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, SetDisplay)
END_MESSAGE_MAP()

//**********************************************************************

BOOL CSetupCsvImportPropertyPage3::OnInitDialog()
{
	m_bEnableCustomImport = m_pData->m_bEnableCustomImport;
	m_strStartFromUserID = m_pData->m_strCustomNextNewUserID;
	m_strMenuText = m_pData->m_strCustomMenuText;
	m_strImportFilename = m_pData->m_strCustomFilename;
	m_bDeleteFile = m_pData->m_bDeleteCustomFile;
	m_nImportMethod = m_pData->m_nImportCustomMethod;
	m_bIgnoreCase = m_pData->m_bIgnoreCustomCase;
	m_nLocateOn = m_pData->m_nCustomLocateOn;

	m_dbCustomList.SetDatabaseName(m_pData->m_strCustomDbFilename);
	m_dbCustomList.OpenForSharing(DB_READWRITE);

	CPropertyPage::OnInitDialog();

	m_listbox.SubclassDlgItem(IDC_LIST1, this);
	m_listbox.InsertColumn(0, "Database Fields", LVCFMT_LEFT, 150);
	m_listbox.InsertColumn(1, "Import Field Names", LVCFMT_LEFT, 150);

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
	m_comboLocate.SetCurSel(m_nLocateOn);

	CCSV csvHeader{};

	{
		CString strHeader = "";
		if (nINTERFACE_BIOMETRIC == System.GetInterfaceType())
		{
			strHeader = SmartPayHeaderHelpers.GetHeaderBiometric();
		}
		else
		{
			strHeader = SmartPayHeaderHelpers.GetHeaderFull();
		}
		csvHeader.ParseLine(strHeader);
	}

	CString strLine;
	for (int i = 0; i < csvHeader.GetSize(); i++)
	{
		CString strLabel = csvHeader.GetString(i);

		if (m_dbCustomList.FindRecord(strLabel) == FALSE)
		{
			m_dbCustomList.AddBlankRecord(strLabel);
		}

		strLine.Format("%s\t%s", RepoText.GetDbDisplayText(strLabel), m_dbCustomList.GetUserID());
		m_listbox.AddString(strLine);
	}

	SetDisplay();

	return TRUE;
}

//*********************************************************************

BOOL CSetupCsvImportPropertyPage3::OnKillActive()
{
	if (UpdateData() == TRUE)
	{
		m_pData->m_bEnableCustomImport = (m_bEnableCustomImport != 0);
		m_pData->m_strCustomMenuText = m_strMenuText;
		m_pData->m_strCustomFilename = m_strImportFilename;
		m_pData->m_bDeleteCustomFile = (m_bDeleteFile != 0);
		m_pData->m_nImportCustomMethod = m_nImportMethod;
		m_pData->m_bIgnoreCustomCase = (m_bIgnoreCase != 0);
		m_pData->m_strCustomNextNewUserID = m_strStartFromUserID;
		m_pData->m_nCustomLocateOn = m_nLocateOn;

		return CPropertyPage::OnKillActive();
	}
	return FALSE;
}

//*******************************************************************

void CSetupCsvImportPropertyPage3::OnToggleEnable()				
{ 
	SetDisplay();	
}

//*******************************************************************

void CSetupCsvImportPropertyPage3::OnKillFocusImportFilename()	
{ 
	SetDisplay();	
}

//*******************************************************************

void CSetupCsvImportPropertyPage3::SetDisplay()
{
	if (UpdateData() == TRUE)
	{
		BOOL bComboAllowed = (System.GetInterfaceType() == nINTERFACE_BIOMETRIC) ? FALSE : m_bEnableCustomImport;
		m_comboLocate.EnableWindow(bComboAllowed);

		BOOL bStartFromAllowed = (System.GetInterfaceType() == nINTERFACE_BIOMETRIC) ? FALSE : m_bEnableCustomImport;

		if (bStartFromAllowed == TRUE)
		{
			bStartFromAllowed = (m_nImportMethod != 2);
		}

		m_editCardNo.EnableWindow(bStartFromAllowed);
		m_radioImportAll.EnableWindow(m_bEnableCustomImport);
		m_radioNew.EnableWindow(m_bEnableCustomImport);
		m_radioExisting.EnableWindow(m_bEnableCustomImport);
		m_checkDeleteFile.EnableWindow(m_bEnableCustomImport);
		m_checkIgnoreCase.EnableWindow(m_bEnableCustomImport);
		m_editImportFilename.EnableWindow(m_bEnableCustomImport);
		m_editMenuText.EnableWindow(m_bEnableCustomImport);
		m_buttonBrowse.EnableWindow(m_bEnableCustomImport);

		bool bAllowed = ::FileExists(m_strImportFilename);
		m_buttonView.EnableWindow(bAllowed);
	}
}

//***************************************************************************

void CSetupCsvImportPropertyPage3::OnButtonBrowse()
{
	if (UpdateData() == TRUE)
	{
		CFileDialog dlg(TRUE, "csv", szFILENAME_IMPORTTOPUP, OFN_HIDEREADONLY | OFN_NOCHANGEDIR, "CSV Topup import file (*.csv)|*.csv|All Files (*.*)|*.*||", this);
		dlg.m_ofn.lpstrTitle = "Locate CSV Topup Import file";
		dlg.m_ofn.lpstrInitialDir = Filenames.GetImportsPath();

		if (DoFileDialog(dlg) == IDOK)
		{
			m_strImportFilename = dlg.GetPathName();
			UpdateData(FALSE);
			SetDisplay();
		}
	}
}

//***************************************************************************

void CSetupCsvImportPropertyPage3::OnButtonView()
{
	CCSVFileReport csv ( "R119", m_strImportFilename, this );
	csv.Show();
}

//***************************************************************************

void CSetupCsvImportPropertyPage3::OnDoubleClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	if ( UpdateData() == TRUE )
	{
		if ( m_bEnableCustomImport == TRUE )
		{
			int nCurSel = 0;
			if ( ( nCurSel = m_listbox.GetCurSel() ) >= 0 )
			{
				CSQLRepositoryUserText RepoText;
				CString strDisplayLabel = m_listbox.GetItemText ( nCurSel, 0 );
				CString strDbLabel = RepoText.GetDbLabel ( strDisplayLabel );
				m_dbCustomList.FindRecord ( strDbLabel );

				CEditCustomListDlg dlg{};
				dlg.m_strFieldName = strDisplayLabel;
				dlg.m_strCustomField = m_dbCustomList.GetUserID();

				if ( dlg.DoModal() == IDOK )
				{
					CString strCustomField = dlg.m_strCustomField;
					m_dbCustomList.SetUserID ( strCustomField );

					m_listbox.DeleteString ( nCurSel );

					CString strLine = "";
					strLine.Format ( "%s\t%s", 
						(const char*) strDisplayLabel, 
						(const char*) strCustomField );

					m_listbox.InsertString ( nCurSel, strLine );

					m_listbox.SetCurSel ( nCurSel );
				}
			}
		}
	}
	*pResult = 0;
}

//***************************************************************************

