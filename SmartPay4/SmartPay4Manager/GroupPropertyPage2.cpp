//$$******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
//$$******************************************************************
#include "GroupPropertyPage2.h"
//$$******************************************************************

CGroupPropertyPage2::CGroupPropertyPage2(CSQLRowGroupFull& RowGroup)
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CGroupPropertyPage2::IDD)
	, m_RowGroup(RowGroup)
{
	m_bShowCash2 = TRUE;
	m_bRevaluationAllowed = TRUE;
	m_bSpendAllowed = TRUE;
}

//$$******************************************************************

CGroupPropertyPage2::~CGroupPropertyPage2()
{
}

//$$******************************************************************

void CGroupPropertyPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_CHECK_REVALUATIONTYPE, m_checkUseGroupRevaluation);
	DDX_Control(pDX, IDC_CHECK_REVALUEDISABLED, m_checkRevaluationDisabled);
	DDX_Control(pDX, IDC_STATIC_2NDBONUSTRIP, m_staticSecondBonusTrip);
	DDX_Control(pDX, IDC_RADIO_REVALUATIONPOINTS, m_radioRevaluationPoints);
	DDX_Control(pDX, IDC_RADIO_REVALUATIONVALUE, m_radioRevaluationValue);
	DDX_Control(pDX, IDC_BUTTON_DEFAULT1, m_buttonDefault1);
	/*****/
	DDX_Control(pDX, IDC_CHECK_REWARDTYPE, m_checkUseGroupReward);
	DDX_Control(pDX, IDC_CHECK_REWARDDISABLED, m_checkRewardDisabled);
	DDX_Control(pDX, IDC_RADIO_REWARDPOINTS, m_radioRewardPoints);
	DDX_Control(pDX, IDC_RADIO_REWARDVALUE, m_radioRewardValue);
	DDX_Control(pDX, IDC_BUTTON_DEFAULT2, m_buttonDefault2);
	/*****/
	DDX_Text(pDX, IDC_EDIT_REVALUETRIP, m_dRevaluationTrip);
	DDX_Text(pDX, IDC_EDIT_REVALUETRIP2, m_dRevaluationTrip2);
	DDX_Radio(pDX, IDC_RADIO_REVALUATIONPOINTS, m_nRevaluationType);
	DDX_Text(pDX, IDC_EDIT_REVALUEPOINTS, m_nRevaluationPoints);
	DDX_Text(pDX, IDC_EDIT_REVALUEVALUE, m_dRevaluationValue);
	/*****/
	DDX_Text(pDX, IDC_EDIT_REWARDSPENDTRIP, m_dRewardTrip);
	DDX_Radio(pDX, IDC_RADIO_REWARDPOINTS, m_nRewardType);
	DDX_Text(pDX, IDC_EDIT_REWARDPOINTS, m_nRewardPoints);
	DDX_Text(pDX, IDC_EDIT_REWARDVALUE, m_dRewardValue);
	DDX_Text(pDX, IDC_EDIT_UPPERSPENDLIMIT, m_dRewardUpperSpendLimit);
	DDX_Text(pDX, IDC_EDIT_MULTIPLIERTRIGGER, m_dRewardMultiplierTrigger);
}

//$$******************************************************************

void CGroupPropertyPage2::SetPointer(bool bShowCash2, bool bRevaluationAllowed, bool bSpendAllowed)
{
	m_bShowCash2 = bShowCash2;
	m_bRevaluationAllowed = bRevaluationAllowed;
	m_bSpendAllowed = bSpendAllowed;
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CGroupPropertyPage2, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_REVALUATIONTYPE, OnToggleRevaluationType)
	ON_BN_CLICKED(IDC_CHECK_REVALUEDISABLED, OnToggleRevalueDisabled)
	ON_BN_CLICKED(IDC_RADIO_REVALUATIONPOINTS, OnRadioRevaluationPoints)
	ON_BN_CLICKED(IDC_RADIO_REVALUATIONVALUE, OnRadioRevaluationValue)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT1, OnButtonDefault1)
	/*****/
	ON_BN_CLICKED(IDC_CHECK_REWARDTYPE, OnToggleRewardType)
	ON_BN_CLICKED(IDC_CHECK_REWARDDISABLED, OnToggleRewardDisabled)
	ON_BN_CLICKED(IDC_RADIO_REWARDPOINTS, OnRadioRewardPoints)
	ON_BN_CLICKED(IDC_RADIO_REWARDVALUE, OnRadioRewardValue)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT2, OnButtonDefault2)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CGroupPropertyPage2::OnInitDialog()
{
	m_nRevaluationType = m_RowGroup.GetRevalueType();
	m_dRevaluationTrip = m_RowGroup.GetRevalueTrip1();
	m_dRevaluationTrip2 = m_RowGroup.GetRevalueTrip2();
	m_nRevaluationPoints = m_RowGroup.GetRevaluePoints();
	m_dRevaluationValue = m_RowGroup.GetRevalueBonus();

	m_nRewardType = m_RowGroup.GetRewardType();
	m_dRewardTrip = m_RowGroup.GetRewardTrip();
	m_nRewardPoints = m_RowGroup.GetRewardPoints();
	m_dRewardValue = m_RowGroup.GetRewardBonus();
	m_dRewardUpperSpendLimit = m_RowGroup.GetRewardLimit();
	m_dRewardMultiplierTrigger = m_RowGroup.GetRewardTrigger();

	CSSPropertyPage::OnInitDialog();

	m_checkUseGroupRevaluation.SetCheck(m_RowGroup.GetFileRevalueFlag());
	m_checkUseGroupRevaluation.EnableWindow(m_bRevaluationAllowed);
	m_checkRevaluationDisabled.SetCheck(m_RowGroup.GetRedeemDisable());

	m_checkUseGroupReward.SetCheck(m_RowGroup.GetFileRewardFlag());
	m_checkUseGroupReward.EnableWindow(m_bSpendAllowed);
	m_checkRewardDisabled.SetCheck(m_RowGroup.GetRewardDisable());

	SubclassEdit(IDC_EDIT_REVALUETRIP, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVALUETRIP2, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVALUEVALUE, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REVALUEPOINTS, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_REWARDSPENDTRIP, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REWARDVALUE, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REWARDPOINTS, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_UPPERSPENDLIMIT, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_MULTIPLIERTRIGGER, SS_NUM_SDP, 11, "%.2f");

	OnToggleRevaluationType();
	OnToggleRewardType();

	return TRUE;
}

//$$******************************************************************
CEdit* CGroupPropertyPage2::GetEditRevalueTrip(){ return GetEdit( IDC_EDIT_REVALUETRIP ); }
CEdit* CGroupPropertyPage2::GetEditRevalueTrip2(){ return GetEdit( IDC_EDIT_REVALUETRIP2 ); }
CEdit* CGroupPropertyPage2::GetEditRevalueValue(){ return GetEdit( IDC_EDIT_REVALUEVALUE ); }
CEdit* CGroupPropertyPage2::GetEditRevaluePoints(){ return GetEdit( IDC_EDIT_REVALUEPOINTS ); }
CEdit* CGroupPropertyPage2::GetEditRewardSpendTrip(){ return GetEdit( IDC_EDIT_REWARDSPENDTRIP ); }
CEdit* CGroupPropertyPage2::GetEditRewardValue(){ return GetEdit( IDC_EDIT_REWARDVALUE ); }
CEdit* CGroupPropertyPage2::GetEditRewardPoints(){ return GetEdit( IDC_EDIT_REWARDPOINTS ); }
CEdit* CGroupPropertyPage2::GetEditUpperSpendLimit(){ return GetEdit( IDC_EDIT_UPPERSPENDLIMIT ); }
CEdit* CGroupPropertyPage2::GetEditMultiplierTrigger(){ return GetEdit( IDC_EDIT_MULTIPLIERTRIGGER ); }
//$$******************************************************************

BOOL CGroupPropertyPage2::OnKillActive()
{
	if (UpdateData() == TRUE)
	{
		m_RowGroup.SetFileRevalueFlag(m_checkUseGroupRevaluation.GetCheck() != 0);
		m_RowGroup.SetRevalueDisable(m_checkRevaluationDisabled.GetCheck() != 0);
		m_RowGroup.SetRevalueType(m_nRevaluationType);
		m_RowGroup.SetRevalueTrip1(m_dRevaluationTrip);
		m_RowGroup.SetRevalueTrip2(m_dRevaluationTrip2);
		m_RowGroup.SetRevaluePoints(m_nRevaluationPoints);
		m_RowGroup.SetRevalueBonus(m_dRevaluationValue);

		m_RowGroup.SetFileRewardFlag(m_checkUseGroupReward.GetCheck() != 0);
		m_RowGroup.SetRewardDisable(m_checkRewardDisabled.GetCheck() != 0);
		m_RowGroup.SetRewardType(m_nRewardType);
		m_RowGroup.SetRewardTrip(m_dRewardTrip);
		m_RowGroup.SetRewardPoints(m_nRewardPoints);
		m_RowGroup.SetRewardBonus(m_dRewardValue);
		m_RowGroup.SetRewardLimit(m_dRewardUpperSpendLimit);
		m_RowGroup.SetRewardTrigger(m_dRewardMultiplierTrigger);
	}

	return CSSPropertyPage::OnKillActive();
}

//$$******************************************************************
void CGroupPropertyPage2::OnToggleRevalueDisabled()	{ OnToggleRevaluationType(); }
//$$******************************************************************

void CGroupPropertyPage2::OnToggleRevaluationType()
{
	if ( UpdateData() == TRUE )
	{
		BOOL bAllowed = ( m_bRevaluationAllowed == FALSE ) ? FALSE : ( m_checkUseGroupRevaluation.GetCheck() != 0 );
		m_checkRevaluationDisabled.EnableWindow(bAllowed);

		if ( bAllowed == TRUE )
			bAllowed = ( m_checkRevaluationDisabled.GetCheck() == 0 );

		GetEditRevalueTrip()->EnableWindow(bAllowed);

		if ( m_bShowCash2 == TRUE )
			GetEditRevalueTrip2()->EnableWindow(bAllowed);
		else
		{
			m_staticSecondBonusTrip.ShowWindow(SW_HIDE);
			GetEditRevalueTrip2()->ShowWindow(SW_HIDE);	
		}

		m_radioRevaluationPoints.EnableWindow(bAllowed);
		m_radioRevaluationValue.EnableWindow(bAllowed);
		GetEditRevaluePoints()->EnableWindow(bAllowed);
		GetEditRevalueValue()->EnableWindow(bAllowed);
		m_buttonDefault1.EnableWindow(bAllowed);

		if ( bAllowed == TRUE )
		{
			SetRevaluationDisplay();
			GetEditRevalueTrip()->SetFocus();
			GetEditRevalueTrip()->SetSel(0,-1);
		}
	}
}

//$$******************************************************************

void CGroupPropertyPage2::OnToggleRewardDisabled()	{ OnToggleRewardType(); }

void CGroupPropertyPage2::OnToggleRewardType()
{
	if ( UpdateData() == TRUE )
	{
		BOOL bAllowed = ( m_bSpendAllowed == FALSE ) ? FALSE : ( m_checkUseGroupReward.GetCheck() != 0 );
		m_checkRewardDisabled.EnableWindow(bAllowed);

		if ( bAllowed == TRUE )
			bAllowed = ( m_checkRewardDisabled.GetCheck() == 0 );

		GetEditRewardSpendTrip()->EnableWindow(bAllowed);
		m_radioRewardPoints.EnableWindow(bAllowed);
		m_radioRewardValue.EnableWindow(bAllowed);
		GetEditRewardPoints()->EnableWindow(bAllowed);
		GetEditRewardValue()->EnableWindow(bAllowed);
		GetEditUpperSpendLimit()->EnableWindow(bAllowed);
		GetEditMultiplierTrigger()->EnableWindow(bAllowed);
		m_buttonDefault2.EnableWindow(bAllowed);

		if ( bAllowed == TRUE )
		{
			SetRewardDisplay();
			GetEditRewardSpendTrip()->SetFocus();
			GetEditRewardSpendTrip()->SetSel(0,-1);
		}
	}
}

//$$******************************************************************
void CGroupPropertyPage2::OnRadioRevaluationPoints() { SetRevaluationDisplay(); }
void CGroupPropertyPage2::OnRadioRevaluationValue()  { SetRevaluationDisplay(); }
//$$******************************************************************

void CGroupPropertyPage2::SetRevaluationDisplay()
{
	if ( UpdateData() == TRUE )
	{
		bool bPointsReqd	= TRUE;
		bool bValueReqd		= TRUE;
		switch ( m_nRevaluationType )
		{
		case 0:	bValueReqd	= FALSE;		break;
		case 1:	bPointsReqd	= FALSE;		break;
		}

		GetEditRevaluePoints()->EnableWindow(bPointsReqd);
		GetEditRevalueValue()->EnableWindow(bValueReqd);

		if ( bPointsReqd == TRUE )		GetEditRevaluePoints()->SetFocus();
		else if ( bValueReqd == TRUE )	GetEditRevalueValue()->SetFocus();
	}
}

//$$******************************************************************
void CGroupPropertyPage2::OnRadioRewardPoints()	{ SetRewardDisplay(); }
void CGroupPropertyPage2::OnRadioRewardValue()	{ SetRewardDisplay(); }
//$$******************************************************************

void CGroupPropertyPage2::SetRewardDisplay()
{
	if ( UpdateData() == TRUE )
	{
		bool bPointsReqd	= TRUE;
		bool bValueReqd		= TRUE;

		switch ( m_nRewardType )
		{
		case 0:	bValueReqd	= FALSE;		break;
		case 1:	bPointsReqd = FALSE;		break;
		}

		GetEditRewardPoints()->EnableWindow(bPointsReqd);
		GetEditRewardValue()->EnableWindow(bValueReqd);
		if ( bPointsReqd == TRUE )	GetEditRewardPoints()->SetFocus();
		else if ( bValueReqd == TRUE )	GetEditRewardValue()->SetFocus();
	}
}

//$$******************************************************************

void CGroupPropertyPage2::OnButtonDefault1()
{
	if (UpdateData() == TRUE)
	{
		Server.Read(Filenames.GetServerDataFilename());

		m_dRevaluationTrip = Server.GetFileRevaluationTrip();
		m_dRevaluationTrip2 = Server.GetFileRevaluationTrip2();
		m_nRevaluationType = Server.GetRevaluationType();
		m_nRevaluationPoints = Server.GetRevaluationPoints();
		m_dRevaluationValue = Server.GetRevaluationValue();
		m_checkRevaluationDisabled.SetCheck(FALSE);

		UpdateData(FALSE);
		SetRevaluationDisplay();
	}
}

//$$******************************************************************

void CGroupPropertyPage2::OnButtonDefault2()
{
	if (UpdateData() == TRUE)
	{
		Server.Read(Filenames.GetServerDataFilename());

		m_dRewardTrip = Server.GetFileRewardTrip();
		m_nRewardType = Server.GetRewardType();
		m_nRewardPoints = Server.GetRewardPoints();
		m_dRewardValue = Server.GetRewardValue();
		m_dRewardUpperSpendLimit = Server.GetRewardUpperSpendLimit();
		m_dRewardMultiplierTrigger = Server.GetRewardMultiplierTrigger();
		m_checkRewardDisabled.SetCheck(FALSE);

		UpdateData(FALSE);
		SetRewardDisplay();
	}
}

//$$******************************************************************
