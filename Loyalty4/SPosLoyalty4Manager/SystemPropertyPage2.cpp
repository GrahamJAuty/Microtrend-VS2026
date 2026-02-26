//$$******************************************************************
#include "LoyaltyManager.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "SystemPropertyPage2.h"
//$$******************************************************************

CSystemPropertyPage2::CSystemPropertyPage2()
	: CSSPropertyPage(CSystemPropertyPage2::IDD)
{

}

//$$******************************************************************

CSystemPropertyPage2::~CSystemPropertyPage2()
{
}

//$$******************************************************************

void CSystemPropertyPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_REFRESH, m_comboEditRefresh);
	DDX_Control(pDX, IDC_COMBO_EDITPAGE, m_comboEditSelectPage);
	DDX_Control(pDX, IDC_CHECK_SHOWBUTTON_PURSE1TOPUP, m_checkShowButtonPurse1Topup);
	DDX_Control(pDX, IDC_CHECK_SHOWBUTTON_PURSE1SPEND, m_checkShowButtonPurse1Spend);
	DDX_Control(pDX, IDC_CHECK_SHOWBUTTON_PURSE2TOPUP, m_checkShowButtonPurse2Topup);
	DDX_Control(pDX, IDC_CHECK_SHOWBUTTON_POINTSTOPUP, m_checkShowButtonPointsTopup);
	DDX_Control(pDX, IDC_CHECK_SHOWBUTTON_POINTSREDEEM, m_checkShowButtonPointsRedeem);
	DDX_Control(pDX, IDC_CHECK_SHOWBUTTON_MANUALBONUS, m_checkShowButtonManualBonus);
	DDX_Control(pDX, IDC_CHECK_COLOUR, m_checkColour);
	DDX_Control(pDX, IDC_CHECK_PRELOAD, m_checkAccountPreload);
	DDX_Control(pDX, IDC_CHECK_CANCELPROMPT, m_checkCancelPrompt);
	/*****/
	DDX_Control(pDX, IDC_CHECK_EDITPURSE1, m_checkEditPurse1);
	DDX_Control(pDX, IDC_CHECK_EDITPURSE2, m_checkEditPurse2);
	DDX_Control(pDX, IDC_CHECK_EDITPOINTS, m_checkEditPoints);
	DDX_Control(pDX, IDC_CHECK_EDITCASHSPEND, m_checkEditCashSpend);
	/*****/
	DDX_Control(pDX, IDC_CHECK_BATCHUPDATES, m_checkEnableBatchUpdates);
	DDX_Control(pDX, IDC_CHECK_BATCHPOINTS, m_checkBatchUpdatePoints);
	DDX_Control(pDX, IDC_CHECK_BATCHPURSE1, m_checkBatchUpdatePurse1);
	DDX_Control(pDX, IDC_CHECK_BATCHPURSE2, m_checkBatchUpdatePurse2);
	DDX_Control(pDX, IDC_CHECK_BATCHSPENDTD, m_checkBatchUpdateSpendTD);
	/*****/
	DDX_Control(pDX, IDC_CHECK_CONTEXTPURSE1TOPUP, m_checkContextMenuPurse1Topup);
	DDX_Control(pDX, IDC_CHECK_CONTEXTPURSE2TOPUP, m_checkContextMenuPurse2Topup);
	/*****/
	DDX_Control(pDX, IDC_CHECK_FINDER_VALID, m_checkFinderValid);
	DDX_Control(pDX, IDC_CHECK_FINDER_EXPIRED, m_checkFinderExpired);
	DDX_Control(pDX, IDC_CHECK_FINDER_HOTLISTED, m_checkFinderHotlisted);
	DDX_Control(pDX, IDC_CHECK_FINDER_ALERTSET, m_checkFinderAlertSet);
	DDX_Control(pDX, IDC_CHECK_FINDER_INACTIVE, m_checkFinderInactive);
	DDX_Control(pDX, IDC_CHECK_FINDER_USERNAME, m_checkFinderUsername);
	DDX_Control(pDX, IDC_CHECK_FINDER_FORENAME, m_checkFinderForename);
	DDX_Control(pDX, IDC_CHECK_FINDER_SURNAME, m_checkFinderSurname);
	DDX_Control(pDX, IDC_CHECK_FINDER_ADDRESS, m_checkFinderAddress);
	DDX_Control(pDX, IDC_CHECK_FINDER_POSTCODE, m_checkFinderPostcode);
	DDX_Control(pDX, IDC_CHECK_FINDER_PHONE, m_checkFinderPhone);
	DDX_Control(pDX, IDC_CHECK_FINDER_INFO, m_checkFinderInfo);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSystemPropertyPage2, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_BATCHUPDATES, OnToggleBatchUpdates)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CSystemPropertyPage2::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	CStandardCombos::FillEditPageCombo( m_comboEditSelectPage, System.GetDefaultAccountEditPage() );

	{
		m_comboEditRefresh.AddString("Off");
		m_comboEditRefresh.AddString("5 Seconds");
		m_comboEditRefresh.AddString("10 Seconds");
		m_comboEditRefresh.AddString("15 Seconds");
		m_comboEditRefresh.AddString("20 Seconds");
		m_comboEditRefresh.AddString("30 Seconds");

		int nSel = 0;
		switch (System.GetListRefreshRate())
		{
		case 5:		nSel = 1;	break;
		case 10:	nSel = 2;	break;
		case 15:	nSel = 3;	break;
		case 20:	nSel = 4;	break;
		case 30:	nSel = 5;	break;
		}
		m_comboEditRefresh.SetCurSel(nSel);
	}

	m_checkShowButtonPurse1Topup.SetCheck(System.GetShowButtonPurse1TopupFlag());
	m_checkShowButtonPurse1Spend.SetCheck(System.GetShowButtonPurse1SpendFlag());
	m_checkShowButtonPurse2Topup.SetCheck(System.GetShowButtonPurse2TopupFlag());
	m_checkShowButtonPointsTopup.SetCheck(System.GetShowButtonPointsTopupFlag());
	m_checkShowButtonPointsRedeem.SetCheck(System.GetShowButtonPointsRedeemFlag());
	m_checkShowButtonManualBonus.SetCheck(System.GetShowButtonManualBonusFlag());
	m_checkColour.SetCheck(System.GetEnableAccountColourFlag());
	m_checkAccountPreload.SetCheck(System.GetAccountPreloadFlag());
	m_checkCancelPrompt.SetCheck(System.GetAccountCancelPromptFlag());
	/*****/
	m_checkEditPurse1.SetCheck( System.GetEditPurse1Flag() );
	m_checkEditPurse2.SetCheck(System.GetEditPurse2Flag());
	m_checkEditPoints.SetCheck(System.GetEditPointsFlag());
	m_checkEditCashSpend.SetCheck(System.GetEditCashSpendFlag());
	/*****/
	m_checkEnableBatchUpdates.SetCheck( System.GetEnableBatchUpdatesFlag() );
	m_checkBatchUpdatePoints.SetCheck( System.GetBatchUpdatePointsFlag() );
	m_checkBatchUpdatePurse1.SetCheck( System.GetBatchUpdatePurse1Flag() );
	m_checkBatchUpdatePurse2.SetCheck( System.GetBatchUpdatePurse2Flag() );
	m_checkBatchUpdateSpendTD.SetCheck( System.GetBatchUpdateSpendTDFlag() );
	/*****/
	m_checkContextMenuPurse1Topup.SetCheck(System.GetContextMenuPurse1TopupFlag());
	m_checkContextMenuPurse2Topup.SetCheck(System.GetContextMenuPurse2TopupFlag());
	/*****/
	m_checkFinderValid.SetCheck( System.GetAccountFindValidFlag() );
	m_checkFinderExpired.SetCheck( System.GetAccountFindExpiredFlag() );
	m_checkFinderHotlisted.SetCheck( System.GetAccountFindHotlistedFlag() );
	m_checkFinderAlertSet.SetCheck(System.GetAccountFindAlertSetFlag());
	m_checkFinderInactive.SetCheck( System.GetAccountFindInactiveFlag() );
	m_checkFinderUsername.SetCheck(System.GetAccountFindUsernameFlag());
	m_checkFinderForename.SetCheck(System.GetAccountFindForenameFlag());
	m_checkFinderSurname.SetCheck(System.GetAccountFindSurnameFlag());
	m_checkFinderAddress.SetCheck(System.GetAccountFindAddressFlag());
	m_checkFinderPostcode.SetCheck(System.GetAccountFindPostcodeFlag());
	m_checkFinderPhone.SetCheck(System.GetAccountFindPhoneFlag());
	m_checkFinderInfo.SetCheck(System.GetAccountFindInfoFlag());

	return TRUE;
}

//$$******************************************************************

BOOL CSystemPropertyPage2::OnSetActive()
{
	SetDisplay();
	return CPropertyPage::OnSetActive();
}

//$$******************************************************************

BOOL CSystemPropertyPage2::OnKillActive()
{
	System.SetDefaultAccountEditPage( m_comboEditSelectPage.GetCurSel() );

	{
		int nRate = 0;
		switch (m_comboEditRefresh.GetCurSel())
		{
		case 1:	nRate = 5;	break;
		case 2:	nRate = 10;	break;
		case 3:	nRate = 15;	break;
		case 4:	nRate = 20;	break;
		case 5:	nRate = 30;	break;
		}
		System.SetListRefreshRate(nRate);
	}

	System.SetShowButtonPurse1TopupFlag(IsTicked(m_checkShowButtonPurse1Topup));
	System.SetShowButtonPurse1SpendFlag(IsTicked(m_checkShowButtonPurse1Spend));
	System.SetShowButtonPurse2TopupFlag(IsTicked(m_checkShowButtonPurse2Topup));
	System.SetShowButtonPointsTopupFlag(IsTicked(m_checkShowButtonPointsTopup));
	System.SetShowButtonPointsRedeemFlag(IsTicked(m_checkShowButtonPointsRedeem));
	System.SetShowButtonManualBonusFlag(IsTicked(m_checkShowButtonManualBonus));
	System.SetEnableAccountColourFlag(IsTicked(m_checkColour));
	System.SetAccountPreloadFlag(IsTicked(m_checkAccountPreload));
	System.SetAccountCancelPromptFlag(IsTicked(m_checkCancelPrompt));
	/*****/
	System.SetEditPurse1Flag( IsTicked(m_checkEditPurse1) );
	System.SetEditPurse2Flag( IsTicked(m_checkEditPurse2) );
	System.SetEditPointsFlag( IsTicked(m_checkEditPoints) );
	System.SetEditCashSpendFlag(IsTicked(m_checkEditPoints));
	/*****/
	System.SetEnableBatchUpdatesFlag( IsTicked(m_checkEnableBatchUpdates) );
	System.SetBatchUpdatePointsFlag( IsTicked(m_checkBatchUpdatePoints) );
	System.SetBatchUpdatePurse1Flag( IsTicked(m_checkBatchUpdatePurse1) );
	System.SetBatchUpdatePurse2Flag( IsTicked(m_checkBatchUpdatePurse2) );
	System.SetBatchUpdateSpendTDFlag( IsTicked(m_checkBatchUpdateSpendTD) );
	/*****/
	System.SetContextMenuPurse1TopupFlag(IsTicked(m_checkContextMenuPurse1Topup));
	System.SetContextMenuPurse2TopupFlag(IsTicked(m_checkContextMenuPurse2Topup));
	/*****/
	System.SetAccountFindValidFlag(IsTicked(m_checkFinderValid));
	System.SetAccountFindExpiredFlag(IsTicked(m_checkFinderExpired));
	System.SetAccountFindHotlistedFlag(IsTicked(m_checkFinderHotlisted));
	System.SetAccountFindAlertSetFlag(IsTicked(m_checkFinderAlertSet));
	System.SetAccountFindInactiveFlag(IsTicked(m_checkFinderInactive));
	System.SetAccountFindUsernameFlag(IsTicked(m_checkFinderUsername));
	System.SetAccountFindForenameFlag(IsTicked(m_checkFinderForename));
	System.SetAccountFindSurnameFlag(IsTicked(m_checkFinderSurname));
	System.SetAccountFindAddressFlag(IsTicked(m_checkFinderAddress));
	System.SetAccountFindPostcodeFlag(IsTicked(m_checkFinderPostcode));
	System.SetAccountFindPhoneFlag(IsTicked(m_checkFinderPhone));
	System.SetAccountFindInfoFlag(IsTicked(m_checkFinderInfo));

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

void CSystemPropertyPage2::OnToggleBatchUpdates()	
{ 
	SetDisplay(); 
}

//$$******************************************************************

void CSystemPropertyPage2::SetDisplay()
{
	bool bBatchUpdates = IsTicked(m_checkEnableBatchUpdates);
	m_checkBatchUpdatePoints.EnableWindow(bBatchUpdates);
	m_checkBatchUpdatePurse1.EnableWindow(bBatchUpdates);
	m_checkBatchUpdateSpendTD.EnableWindow(bBatchUpdates);

	bool bPurse2 = System.GetEnablePurse2Flag();
	m_checkEditPurse2.EnableWindow(bPurse2);
	m_checkShowButtonPurse2Topup.EnableWindow(bPurse2);
	m_checkContextMenuPurse2Topup.EnableWindow(bPurse2);

	m_checkBatchUpdatePurse2.EnableWindow(bBatchUpdates & bPurse2);
}

//$$******************************************************************
