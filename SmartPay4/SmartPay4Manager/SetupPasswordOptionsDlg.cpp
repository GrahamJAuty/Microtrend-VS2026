//*******************************************************************
#include "SmartPay4Manager.h"
//*******************************************************************
#include "SetupPasswordOptionsDlg.h"
//*******************************************************************

CSetupPasswordOptionsDlg::CSetupPasswordOptionsDlg( int nPasswordNo, CPasswordData* pPasswords, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog( AutoShutdownHelper, CSetupPasswordOptionsDlg::IDD, pParent)
{
	m_nPasswordNo = nPasswordNo;						// Passwordno = 01 to 20
	m_pPasswords = pPasswords;
}

//*******************************************************************

CSetupPasswordOptionsDlg::~CSetupPasswordOptionsDlg()
{
}

//*******************************************************************

void CSetupPasswordOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUserName);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_CHECK_ADDRECORD, m_checkAddRecord);
	DDX_Control(pDX, IDC_CHECK_MODIFYRECORD, m_checkModifyRecord);
	DDX_Control(pDX, IDC_CHECK_EDITGENERAL, m_checkEditGeneral);
	DDX_Control(pDX, IDC_CHECK_EDITPURSE, m_checkEditPurse);
	DDX_Control(pDX, IDC_CHECK_EDITPOINTS, m_checkEditPoints);
	DDX_Control(pDX, IDC_CHECK_EDITEXTERNAL, m_checkEditExternal);
	DDX_Control(pDX, IDC_CHECK_ENABLETOPUP, m_checkEnableTopup);
	DDX_Control(pDX, IDC_CHECK_ENABLEPOINTS, m_checkEnablePoints);
	DDX_Control(pDX, IDC_CHECK_TRANSFERRECORD, m_checkTransferRecord);
	DDX_Control(pDX, IDC_CHECK_DELETERECORD, m_checkDeleteRecord);
	DDX_Control(pDX, IDC_CHECK_BATCH, m_checkBatch);
	DDX_Control(pDX, IDC_CHECK_IMPORT, m_checkImport);
	DDX_Control(pDX, IDC_CHECK_EXPORT, m_checkExport);
	DDX_Control(pDX, IDC_CHECK_REPORTS, m_checkReports);
	DDX_Control(pDX, IDC_CHECK_SETUPSYSTEM, m_checkSetupSystem);
	DDX_Control(pDX, IDC_CHECK_SETUPSERVER, m_checkSetupServer);
	DDX_Control(pDX, IDC_CHECK_INSTALLSERVER, m_checkInstallServer);
	DDX_Control(pDX, IDC_CHECK_SETUPBGND, m_checkSetupBgnd);
	DDX_Control(pDX, IDC_CHECK_INSTALLBGND, m_checkInstallBgnd);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CSetupPasswordOptionsDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_CHECK_MODIFYRECORD, OnToggleModifyRecord)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CSetupPasswordOptionsDlg::CSSAutoShutdownPostInitDialog()
{
	m_editPassword.SetWindowText(m_pPasswords->GetPassword(m_nPasswordNo));
	m_editPassword.LimitText(MAX_PASSWORD_LEN);

	m_editUserName.SetWindowText(m_pPasswords->GetUserName(m_nPasswordNo));
	m_editUserName.LimitText(MAX_USERNAME_LEN);

	m_checkAddRecord.SetCheck(m_pPasswords->IsAddRecordAllowed());
	m_checkModifyRecord.SetCheck(m_pPasswords->IsEditRecordAllowed());
	m_checkEditGeneral.SetCheck(m_pPasswords->IsEditGeneralAllowed());
	m_checkEditPurse.SetCheck(m_pPasswords->IsEditPurseAllowed());
	m_checkEditPoints.SetCheck(m_pPasswords->IsEditPointsAllowed());
	m_checkEditExternal.SetCheck(m_pPasswords->IsEditExternalAccountLinkAllowed());
	m_checkEnableTopup.SetCheck(m_pPasswords->IsTopupButtonAllowed());
	m_checkEnablePoints.SetCheck(m_pPasswords->IsPointsButtonAllowed());
	m_checkTransferRecord.SetCheck(m_pPasswords->IsTransferRecordAllowed());
	m_checkDeleteRecord.SetCheck(m_pPasswords->IsDeleteRecordAllowed());
	m_checkBatch.SetCheck(m_pPasswords->IsBatchUpdatesAllowed());
	m_checkImport.SetCheck(m_pPasswords->IsImportAllowed());
	m_checkExport.SetCheck(m_pPasswords->IsExportAllowed());
	m_checkReports.SetCheck(m_pPasswords->IsReportsAllowed());
	m_checkSetupSystem.SetCheck(m_pPasswords->IsSetupSystemAllowed());
	m_checkSetupServer.SetCheck(m_pPasswords->IsSetupServerAllowed());
	m_checkInstallServer.SetCheck(m_pPasswords->IsInstallServerAllowed());
	m_checkSetupBgnd.SetCheck(m_pPasswords->IsSetupBgndAllowed());
	m_checkInstallBgnd.SetCheck(m_pPasswords->IsInstallBgndAllowed());

	CString strTitle;
	strTitle.Format ( "Setup Details for Password %2.2d", m_nPasswordNo );
	SetWindowText ( strTitle );

	OnToggleModifyRecord();

	return TRUE;  
}

//*******************************************************************

void CSetupPasswordOptionsDlg::OnToggleModifyRecord()
{
	bool bEnable = IsTicked(m_checkModifyRecord);

	m_checkEditGeneral.EnableWindow(bEnable);
	m_checkEditPurse.EnableWindow(bEnable);
	m_checkEditPoints.EnableWindow(bEnable);
	m_checkEditExternal.EnableWindow(bEnable);
	m_checkEnableTopup.EnableWindow(bEnable);
	m_checkEnablePoints.EnableWindow(bEnable);
	
	if ( FALSE == bEnable )
	{
		m_checkEditGeneral.SetCheck(FALSE);
		m_checkEditPurse.SetCheck(FALSE);
		m_checkEditPoints.SetCheck(FALSE);
		m_checkEditExternal.SetCheck(FALSE);
		m_checkEnableTopup.SetCheck(FALSE);
		m_checkEnablePoints.SetCheck(FALSE);
	}
}

//********************************************************************

void CSetupPasswordOptionsDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		m_pPasswords->SetUserPassword(m_nPasswordNo, GetEditBoxText(m_editPassword));
		m_pPasswords->SetUserName(m_nPasswordNo, GetEditBoxText(m_editUserName));
		
		m_pPasswords->SetAddRecordAllowed(IsTicked(m_checkAddRecord));
		m_pPasswords->SetEditRecordAllowed(IsTicked(m_checkModifyRecord));
		m_pPasswords->SetEditGeneralAllowed(IsTicked(m_checkEditGeneral));
		m_pPasswords->SetEditPurseAllowed(IsTicked(m_checkEditPurse));
		m_pPasswords->SetEditPointsAllowed(IsTicked(m_checkEditPoints));
		m_pPasswords->SetEditExternalAccountLinkAllowed(IsTicked(m_checkEditExternal));
		m_pPasswords->SetTopupButtonAllowed(IsTicked(m_checkEnableTopup));
		m_pPasswords->SetPointsButtonAllowed(IsTicked(m_checkEnablePoints));
		m_pPasswords->SetTransferRecordAllowed(IsTicked(m_checkTransferRecord));
		m_pPasswords->SetDeleteRecordAllowed(IsTicked(m_checkDeleteRecord));
		m_pPasswords->SetBatchUpdatesAllowed(IsTicked(m_checkBatch));
		m_pPasswords->SetImportAllowed(IsTicked(m_checkImport));
		m_pPasswords->SetExportAllowed(IsTicked(m_checkExport));
		m_pPasswords->SetReportsAllowed(IsTicked(m_checkReports));
		m_pPasswords->SetSetupSystemAllowed(IsTicked(m_checkSetupSystem));
		m_pPasswords->SetSetupServerAllowed(IsTicked(m_checkSetupServer));
		m_pPasswords->SetInstallServerAllowed(IsTicked(m_checkInstallServer));
		m_pPasswords->SetSetupBgndAllowed(IsTicked(m_checkSetupBgnd));
		m_pPasswords->SetInstallBgndAllowed(IsTicked(m_checkInstallBgnd));
		
		m_pPasswords->SaveOptions(m_nPasswordNo);

		EndDialog(IDOK);
	}
}

//********************************************************************

