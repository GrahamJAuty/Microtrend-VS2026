//$$******************************************************************
#include "Resource.h"
//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************
#include "PasswordOptionsDlg.h"
//$$******************************************************************

CPasswordOptionsDlg::CPasswordOptionsDlg(int nPasswordIndex, CPasswordData* pPasswords, bool bHaveReader, CWnd* pParent /*=NULL*/)
	: CSSDialog(CPasswordOptionsDlg::IDD, pParent)
{
	m_nPasswordIndex = nPasswordIndex;
	m_pPasswords = pPasswords;
	m_bHaveReader = bHaveReader;
	m_pPasswords->GetPassword(nPasswordIndex);
}

//$$******************************************************************

CPasswordOptionsDlg::~CPasswordOptionsDlg()
{
}

//$$******************************************************************

void CPasswordOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUserName);
	DDX_Control(pDX, IDC_STATIC_CARD, m_staticCard);
	DDX_Control(pDX, IDC_EDIT_CARD, m_editCard);
	/*****/
	DDX_Control(pDX, IDC_CHECK_ADDRECORD, m_checkAddRecord);
	DDX_Control(pDX, IDC_CHECK_EDITRECORD, m_checkEditRecord);
	DDX_Control(pDX, IDC_CHECK_EDITGENERAL, m_checkEditGeneral);
	DDX_Control(pDX, IDC_CHECK_EDITPURSE, m_checkEditPurse);
	DDX_Control(pDX, IDC_CHECK_EDITPOINTS, m_checkEditPoints);
	DDX_Control(pDX, IDC_CHECK_EDITCASHSPEND, m_checkEditCashSpend);
	DDX_Control(pDX, IDC_CHECK_ENABLEBUTTON_PURSE1TOPUP, m_checkEnableButtonPurse1Topup);
	DDX_Control(pDX, IDC_CHECK_ENABLEBUTTON_PURSE1SPEND, m_checkEnableButtonPurse1Spend);
	DDX_Control(pDX, IDC_CHECK_ENABLEBUTTON_POINTSTOPUP, m_checkEnableButtonPointsTopup);
	DDX_Control(pDX, IDC_CHECK_ENABLEBUTTON_POINTSREDEEM, m_checkEnableButtonPointsRedeem);
	DDX_Control(pDX, IDC_CHECK_ENABLEBUTTON_MANUALBONUS, m_checkEnableButtonManualBonus);
	DDX_Control(pDX, IDC_CHECK_TRANSFERRECORD, m_checkTransferRecord);
	DDX_Control(pDX, IDC_CHECK_HOTLISTRECORD, m_checkHotlistRecord);
	DDX_Control(pDX, IDC_CHECK_DELETERECORD, m_checkDeleteRecord);
	DDX_Control(pDX, IDC_CHECK_IMPORT, m_checkImport);
	DDX_Control(pDX, IDC_CHECK_EXPORT, m_checkExport);
	DDX_Control(pDX, IDC_CHECK_BATCH, m_checkBatch);
	DDX_Control(pDX, IDC_CHECK_REPORTS, m_checkReports);
	DDX_Control(pDX, IDC_CHECK_SETUPSYSTEM, m_checkSetupSystem);
	DDX_Control(pDX, IDC_CHECK_SETUPSERVER, m_checkSetupServer);
	DDX_Control(pDX, IDC_CHECK_INSTALLSERVER, m_checkInstallServer);
	DDX_Control(pDX, IDC_CHECK_EXTERNAL_ACCOUNT, m_checkEditExternalAccountLink);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CPasswordOptionsDlg, CSSDialog)
	ON_BN_CLICKED(IDC_CHECK_EDITRECORD, OnToggleEditRecord)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CPasswordOptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strTitle;	
	strTitle.Format ( "Setup Details for Password %2.2d", m_nPasswordIndex );
	SetWindowText ( strTitle );

	m_editPassword.SetWindowText(m_pPasswords->GetPassword(m_nPasswordIndex));
	m_editPassword.LimitText(MAX_PASSWORD_LEN);

	m_editUserName.SetWindowText(m_pPasswords->GetUserName(m_nPasswordIndex));
	m_editUserName.LimitText(MAX_USERNAME_LEN);

	if ( FALSE == m_bHaveReader )
	{
		m_staticCard.ShowWindow(SW_HIDE);
		m_editCard.ShowWindow(SW_HIDE);
	}
	else
	{
		m_editCard.SetWindowText(m_pPasswords->GetCardNo(m_nPasswordIndex));
		m_editCard.LimitText(MAX_PASSWORD_CARDNO_LEN);
	}

	m_checkAddRecord.SetCheck(m_pPasswords->IsAddRecordAllowed());
	m_checkEditRecord.SetCheck(m_pPasswords->IsEditRecordAllowed());
	m_checkEditGeneral.SetCheck(m_pPasswords->IsEditGeneralAllowed());
	m_checkEditPurse.SetCheck(m_pPasswords->IsEditPurseAllowed());
	m_checkEditPoints.SetCheck(m_pPasswords->IsEditPointsAllowed());
	m_checkEditCashSpend.SetCheck(m_pPasswords->IsEditCashSpendAllowed());
	m_checkEnableButtonPurse1Topup.SetCheck(m_pPasswords->IsPurse1TopupButtonAllowed());
	m_checkEnableButtonPurse1Spend.SetCheck(m_pPasswords->IsPurse1SpendButtonAllowed());
	m_checkEnableButtonPointsTopup.SetCheck(m_pPasswords->IsPointsTopupButtonAllowed());
	m_checkEnableButtonPointsRedeem.SetCheck(m_pPasswords->IsPointsRedeemButtonAllowed());
	m_checkEnableButtonManualBonus.SetCheck(m_pPasswords->IsManualBonusButtonAllowed());
	m_checkTransferRecord.SetCheck(m_pPasswords->IsTransferRecordAllowed());
	m_checkHotlistRecord.SetCheck(m_pPasswords->IsHotlistRecordAllowed());
	m_checkDeleteRecord.SetCheck(m_pPasswords->IsDeleteRecordAllowed());
	m_checkImport.SetCheck(m_pPasswords->IsImportRecordsAllowed());
	m_checkExport.SetCheck(m_pPasswords->IsExportRecordsAllowed());
	m_checkBatch.SetCheck(m_pPasswords->IsBatchUpdatesAllowed());
	m_checkReports.SetCheck(m_pPasswords->IsReportsAllowed());
	m_checkSetupSystem.SetCheck(m_pPasswords->IsSystemSetupAllowed());
	m_checkSetupServer.SetCheck(m_pPasswords->IsServerSetupAllowed());
	m_checkInstallServer.SetCheck(m_pPasswords->IsServerInstallAllowed());
	m_checkEditExternalAccountLink.SetCheck(m_pPasswords->IsEditExternalAccountLinkAllowed());

	OnToggleEditRecord();

	return TRUE;  
}

//$$******************************************************************

void CPasswordOptionsDlg::OnToggleEditRecord()
{
	bool bEditRecord = IsTicked(m_checkEditRecord);

	m_checkEditGeneral.EnableWindow(bEditRecord);
	m_checkEditPurse.EnableWindow(bEditRecord);
	m_checkEditPoints.EnableWindow(bEditRecord);
	m_checkEditCashSpend.EnableWindow(bEditRecord);
	m_checkEnableButtonPurse1Topup.EnableWindow(bEditRecord);
	m_checkEnableButtonPurse1Spend.EnableWindow(bEditRecord);
	m_checkEnableButtonPointsTopup.EnableWindow(bEditRecord);
	m_checkEnableButtonPointsRedeem.EnableWindow(bEditRecord);
	m_checkEnableButtonManualBonus.EnableWindow(bEditRecord);
	m_checkEditExternalAccountLink.EnableWindow(bEditRecord);

	if (FALSE == bEditRecord)
	{
		m_checkEditGeneral.SetCheck(FALSE);
		m_checkEditPurse.SetCheck(FALSE);
		m_checkEditPoints.SetCheck(FALSE);
		m_checkEditCashSpend.SetCheck(FALSE);
		m_checkEnableButtonPurse1Topup.SetCheck(FALSE);
		m_checkEnableButtonPurse1Spend.SetCheck(FALSE);
		m_checkEnableButtonPointsTopup.SetCheck(FALSE);
		m_checkEnableButtonPointsRedeem.SetCheck(FALSE);
		m_checkEnableButtonManualBonus.SetCheck(FALSE);
		m_checkEditExternalAccountLink.SetCheck(FALSE);
	}	
}

//$$******************************************************************

void CPasswordOptionsDlg::OnOK()
{
	m_pPasswords->SetUserPassword(m_nPasswordIndex, GetEditBoxText(m_editPassword));
	m_pPasswords->SetUserName(m_nPasswordIndex, GetEditBoxText(m_editUserName));

	if (TRUE == m_bHaveReader)
	{
		m_pPasswords->SetCardNo(m_nPasswordIndex, GetEditBoxText(m_editCard));
	}

	m_pPasswords->SetAddRecordAllowed(IsTicked(m_checkAddRecord));
	m_pPasswords->SetEditRecordAllowed(IsTicked(m_checkEditRecord));
	m_pPasswords->SetEditGeneralAllowed(IsTicked(m_checkEditGeneral));
	m_pPasswords->SetEditPurseAllowed(IsTicked(m_checkEditPurse));
	m_pPasswords->SetEditPointsAllowed(IsTicked(m_checkEditPoints));
	m_pPasswords->SetEditCashSpendAllowed(IsTicked(m_checkEditCashSpend));
	m_pPasswords->SetPurse1TopupButtonAllowed(IsTicked(m_checkEnableButtonPurse1Topup));
	m_pPasswords->SetPurse1SpendButtonAllowed(IsTicked(m_checkEnableButtonPurse1Spend));
	m_pPasswords->SetPointsTopupButtonAllowed(IsTicked(m_checkEnableButtonPointsTopup));
	m_pPasswords->SetPointsRedeemButtonAllowed(IsTicked(m_checkEnableButtonPointsRedeem));
	m_pPasswords->SetManualBonusButtonAllowed(IsTicked(m_checkEnableButtonManualBonus));
	m_pPasswords->SetDeleteRecordAllowed(IsTicked(m_checkDeleteRecord));
	m_pPasswords->SetHotlistRecordAllowed(IsTicked(m_checkHotlistRecord));
	m_pPasswords->SetTransferRecordAllowed(IsTicked(m_checkTransferRecord));
	m_pPasswords->SetImportRecordsAllowed(IsTicked(m_checkImport));
	m_pPasswords->SetExportRecordsAllowed(IsTicked(m_checkExport));
	m_pPasswords->SetBatchUpdatesAllowed(IsTicked(m_checkBatch));
	m_pPasswords->SetReportsAllowed(IsTicked(m_checkReports));
	m_pPasswords->SetSystemSetupAllowed(IsTicked(m_checkSetupSystem));
	m_pPasswords->SetServerSetupAllowed(IsTicked(m_checkSetupServer));
	m_pPasswords->SetServerInstallAllowed(IsTicked(m_checkInstallServer));
	m_pPasswords->SetEditExternalAccountLinkAllowed(IsTicked(m_checkEditExternalAccountLink));
	
	m_pPasswords->SaveOptions(m_nPasswordIndex);

	EndDialog(IDOK);
}

//$$******************************************************************
