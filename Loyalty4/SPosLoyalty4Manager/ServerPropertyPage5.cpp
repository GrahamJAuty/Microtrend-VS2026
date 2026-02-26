//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************
#include "ServerPropertyPage5.h"
//$$******************************************************************

CServerPropertyPage5::CServerPropertyPage5()
	: CSSPropertyPage(CServerPropertyPage5::IDD)
{
}

//$$******************************************************************

void CServerPropertyPage5::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_CHECK_PURSE1, m_checkPurse1);
	DDX_Control(pDX, IDC_CHECK_PURSE2, m_checkPurse2);
	DDX_Control(pDX, IDC_CHECK_CASH, m_checkCash);
	DDX_Control(pDX, IDC_CHECK_MANUAL, m_checkManual);
	DDX_Control(pDX, IDC_CHECK_NOREWARDPLUPTS, m_checkNoRewardPluPts);
	DDX_Text(pDX, IDC_EDIT_SPENDLIMIT, m_dSpendLimit);
	/*****/
	DDX_Text(pDX, IDC_EDIT_REWARDTRIP, m_dRewardTrip);
	DDX_Control(pDX, IDC_RADIO_REWARDPOINTS, m_radioRewardPoints);
	DDX_Control(pDX, IDC_RADIO_REWARDVALUE, m_radioRewardValue);
	DDX_Text(pDX, IDC_EDIT_REWARDPOINTS, m_nRewardPoints);
	DDX_Text(pDX, IDC_EDIT_REWARDVALUE, m_dRewardValue);
	DDX_Control(pDX, IDC_EDIT_COMMENT, m_editRewardComment);
	/*****/
	DDX_Text(pDX, IDC_EDIT_MULTIPLIERFACTOR_MON, m_nRewardMultiplierFactor[BONUS_MON]);
	DDV_MinMaxInt(pDX, m_nRewardMultiplierFactor[BONUS_MON], 0, 9);
	DDX_Text(pDX, IDC_EDIT_MULTIPLIERFACTOR_TUE, m_nRewardMultiplierFactor[BONUS_TUE]);
	DDV_MinMaxInt(pDX, m_nRewardMultiplierFactor[BONUS_TUE], 0, 9);
	DDX_Text(pDX, IDC_EDIT_MULTIPLIERFACTOR_WED, m_nRewardMultiplierFactor[BONUS_WED]);
	DDV_MinMaxInt(pDX, m_nRewardMultiplierFactor[BONUS_WED], 0, 9);
	DDX_Text(pDX, IDC_EDIT_MULTIPLIERFACTOR_THU, m_nRewardMultiplierFactor[BONUS_THU]);
	DDV_MinMaxInt(pDX, m_nRewardMultiplierFactor[BONUS_THU], 0, 9);
	DDX_Text(pDX, IDC_EDIT_MULTIPLIERFACTOR_FRI, m_nRewardMultiplierFactor[BONUS_FRI]);
	DDV_MinMaxInt(pDX, m_nRewardMultiplierFactor[BONUS_FRI], 0, 9);
	DDX_Text(pDX, IDC_EDIT_MULTIPLIERFACTOR_SAT, m_nRewardMultiplierFactor[BONUS_SAT]);
	DDV_MinMaxInt(pDX, m_nRewardMultiplierFactor[BONUS_SAT], 0, 9);
	DDX_Text(pDX, IDC_EDIT_MULTIPLIERFACTOR_SUN, m_nRewardMultiplierFactor[BONUS_SUN]);
	DDV_MinMaxInt(pDX, m_nRewardMultiplierFactor[BONUS_SUN], 0, 9);
	/*****/
	DDX_Control(pDX, IDC_DATETIMEPICKER_MONS, m_dtcStart[BONUS_MON]);
	DDX_Control(pDX, IDC_DATETIMEPICKER_MONE, m_dtcEnd[BONUS_MON]);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TUES, m_dtcStart[BONUS_TUE]);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TUEE, m_dtcEnd[BONUS_TUE]);
	DDX_Control(pDX, IDC_DATETIMEPICKER_WEDS, m_dtcStart[BONUS_WED]);
	DDX_Control(pDX, IDC_DATETIMEPICKER_WEDE, m_dtcEnd[BONUS_WED]);
	DDX_Control(pDX, IDC_DATETIMEPICKER_THUS, m_dtcStart[BONUS_THU]);
	DDX_Control(pDX, IDC_DATETIMEPICKER_THUE, m_dtcEnd[BONUS_THU]);
	DDX_Control(pDX, IDC_DATETIMEPICKER_FRIS, m_dtcStart[BONUS_FRI]);
	DDX_Control(pDX, IDC_DATETIMEPICKER_FRIE, m_dtcEnd[BONUS_FRI]);
	DDX_Control(pDX, IDC_DATETIMEPICKER_SATS, m_dtcStart[BONUS_SAT]);
	DDX_Control(pDX, IDC_DATETIMEPICKER_SATE, m_dtcEnd[BONUS_SAT]);
	DDX_Control(pDX, IDC_DATETIMEPICKER_SUNS, m_dtcStart[BONUS_SUN]);
	DDX_Control(pDX, IDC_DATETIMEPICKER_SUNE, m_dtcEnd[BONUS_SUN]);
	DDX_Text(pDX, IDC_EDIT_MULTIPLIERTRIGGER, m_dRewardMultiplierTrigger);	
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CServerPropertyPage5, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_PURSE1, OnTogglePurse1)
	ON_BN_CLICKED(IDC_CHECK_PURSE2, OnTogglePurse2)
	ON_BN_CLICKED(IDC_CHECK_CASH, OnToggleCash)
	ON_BN_CLICKED(IDC_CHECK_MANUAL, OnToggleManual)
	/*****/
	ON_BN_CLICKED(IDC_RADIO_REWARDPOINTS, OnRadioRewardPoints)
	ON_BN_CLICKED(IDC_RADIO_REWARDVALUE, OnRadioRewardValue)
	/*****/
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MULTIPLIER_MON, OnSpinMultiplierMon)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MULTIPLIER_TUE, OnSpinMultiplierTue)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MULTIPLIER_WED, OnSpinMultiplierWed)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MULTIPLIER_THU, OnSpinMultiplierThu)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MULTIPLIER_FRI, OnSpinMultiplierFri)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MULTIPLIER_SAT, OnSpinMultiplierSat)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MULTIPLIER_SUN, OnSpinMultiplierSun)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CServerPropertyPage5::OnInitDialog()
{
	m_dSpendLimit = Server.GetRewardUpperSpendLimit();
	/*****/
	m_dRewardTrip = Server.GetRewardTrip();
	m_nRewardPoints = Server.GetRewardPoints();
	m_dRewardValue = Server.GetRewardValue();
	/*****/
	m_nRewardMultiplierFactor[BONUS_MON] = Server.GetRewardMultiplierMon();
	m_nRewardMultiplierFactor[BONUS_TUE] = Server.GetRewardMultiplierTue();
	m_nRewardMultiplierFactor[BONUS_WED] = Server.GetRewardMultiplierWed();
	m_nRewardMultiplierFactor[BONUS_THU] = Server.GetRewardMultiplierThu();
	m_nRewardMultiplierFactor[BONUS_FRI] = Server.GetRewardMultiplierFri();
	m_nRewardMultiplierFactor[BONUS_SAT] = Server.GetRewardMultiplierSat();
	m_nRewardMultiplierFactor[BONUS_SUN] = Server.GetRewardMultiplierSun();
	/*****/
	m_dRewardMultiplierTrigger = Server.GetRewardMultiplierTrigger();

	CPropertyPage::OnInitDialog();

	m_checkPurse1.SetCheck(Server.GetRewardPurse1SpendFlag());
	m_checkPurse2.SetCheck(System.GetEnablePurse2Flag() && Server.GetRewardPurse2SpendFlag());
	m_checkCash.SetCheck(Server.GetRewardCashSpendFlag());
	m_checkManual.SetCheck(Server.GetRewardManualSpendFlag());
	m_checkNoRewardPluPts.SetCheck(Server.GetRewardNoBonusWithPluPointsFlag());
	/*****/
	SetRewardDisplay(Server.GetRewardType());
	m_editRewardComment.SetWindowText(Server.GetRewardComment());
	/*****/

	SubclassEdit(IDC_EDIT_SPENDLIMIT, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REWARDTRIP, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_REWARDPOINTS, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_REWARDVALUE, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_MULTIPLIERTRIGGER, SS_NUM_SDP, 11, "%.2f");

	SubclassEdit(IDC_EDIT_MULTIPLIERFACTOR_MON, SS_NUM, 1, "%d");
	SubclassEdit(IDC_EDIT_MULTIPLIERFACTOR_TUE, SS_NUM, 1, "%d");
	SubclassEdit(IDC_EDIT_MULTIPLIERFACTOR_WED, SS_NUM, 1, "%d");
	SubclassEdit(IDC_EDIT_MULTIPLIERFACTOR_THU, SS_NUM, 1, "%d");
	SubclassEdit(IDC_EDIT_MULTIPLIERFACTOR_FRI, SS_NUM, 1, "%d");
	SubclassEdit(IDC_EDIT_MULTIPLIERFACTOR_SAT, SS_NUM, 1, "%d");
	SubclassEdit(IDC_EDIT_MULTIPLIERFACTOR_SUN, SS_NUM, 1, "%d");

	for (int nDay = BONUS_MON; nDay <= BONUS_SUN; nDay++)
	{
		m_dtcStart[nDay].SetFormat("HH:mm");
		m_dtcEnd[nDay].SetFormat("HH:mm");
	}

	COleDateTime time;

	time.SetTime(Server.GetRewardMonStart().GetHour(), Server.GetRewardMonStart().GetMinute(), 0);
	m_dtcStart[BONUS_MON].SetTime(time);
	time.SetTime(Server.GetRewardMonEnd().GetHour(), Server.GetRewardMonEnd().GetMinute(), 0);
	m_dtcEnd[BONUS_MON].SetTime(time);

	time.SetTime(Server.GetRewardTueStart().GetHour(), Server.GetRewardTueStart().GetMinute(), 0);
	m_dtcStart[BONUS_TUE].SetTime(time);
	time.SetTime(Server.GetRewardTueEnd().GetHour(), Server.GetRewardTueEnd().GetMinute(), 0);
	m_dtcEnd[BONUS_TUE].SetTime(time);

	time.SetTime(Server.GetRewardWedStart().GetHour(), Server.GetRewardWedStart().GetMinute(), 0);
	m_dtcStart[BONUS_WED].SetTime(time);
	time.SetTime(Server.GetRewardWedEnd().GetHour(), Server.GetRewardWedEnd().GetMinute(), 0);
	m_dtcEnd[BONUS_WED].SetTime(time);

	time.SetTime(Server.GetRewardThuStart().GetHour(), Server.GetRewardThuStart().GetMinute(), 0);
	m_dtcStart[BONUS_THU].SetTime(time);
	time.SetTime(Server.GetRewardThuEnd().GetHour(), Server.GetRewardThuEnd().GetMinute(), 0);
	m_dtcEnd[BONUS_THU].SetTime(time);

	time.SetTime(Server.GetRewardFriStart().GetHour(), Server.GetRewardFriStart().GetMinute(), 0);
	m_dtcStart[BONUS_FRI].SetTime(time);
	time.SetTime(Server.GetRewardFriEnd().GetHour(), Server.GetRewardFriEnd().GetMinute(), 0);
	m_dtcEnd[BONUS_FRI].SetTime(time);

	time.SetTime(Server.GetRewardSatStart().GetHour(), Server.GetRewardSatStart().GetMinute(), 0);
	m_dtcStart[BONUS_SAT].SetTime(time);
	time.SetTime(Server.GetRewardSatEnd().GetHour(), Server.GetRewardSatEnd().GetMinute(), 0);
	m_dtcEnd[BONUS_SAT].SetTime(time);

	time.SetTime(Server.GetRewardSunStart().GetHour(), Server.GetRewardSunStart().GetMinute(), 0);
	m_dtcStart[BONUS_SUN].SetTime(time);
	time.SetTime(Server.GetRewardSunEnd().GetHour(), Server.GetRewardSunEnd().GetMinute(), 0);
	m_dtcEnd[BONUS_SUN].SetTime(time);

	for (int nDay = BONUS_MON; nDay <= BONUS_SUN; nDay++)
	{
		SetBonusEnables(nDay);
	}

	CheckQualifyingSpendTypes();

	return TRUE;
}

//$$******************************************************************
CEdit* CServerPropertyPage5::GetEditSpendLimit(){ return GetEdit( IDC_EDIT_SPENDLIMIT ); }
CEdit* CServerPropertyPage5::GetEditRewardTrip(){ return GetEdit( IDC_EDIT_REWARDTRIP ); }
CEdit* CServerPropertyPage5::GetEditRewardPoints(){ return GetEdit( IDC_EDIT_REWARDPOINTS ); }
CEdit* CServerPropertyPage5::GetEditRewardValue(){ return GetEdit( IDC_EDIT_REWARDVALUE ); }
CEdit* CServerPropertyPage5::GetEditMultiplierTrigger(){ return GetEdit( IDC_EDIT_MULTIPLIERTRIGGER ); }
//$$******************************************************************

BOOL CServerPropertyPage5::OnKillActive()
{
	if (UpdateData() == TRUE)
	{
		CString strComment;
		m_editRewardComment.GetWindowText(strComment);

		Server.SetRewardPurse1SpendFlag(IsTicked(m_checkPurse1));
		Server.SetRewardPurse2SpendFlag(IsTicked(m_checkPurse2));
		Server.SetRewardCashSpendFlag(IsTicked(m_checkCash));
		Server.SetRewardManualSpendFlag(IsTicked(m_checkManual));
		Server.SetRewardNoBonusWithPluPointsFlag(IsTicked(m_checkNoRewardPluPts));
		Server.SetRewardUpperSpendLimit(m_dSpendLimit);
		/*****/
		Server.SetRewardTrip(m_dRewardTrip);
		Server.SetRewardType(m_nRewardType);
		Server.SetRewardPoints(m_nRewardPoints);
		Server.SetRewardValue(m_dRewardValue);
		Server.SetRewardComment(strComment);
		/*****/
		Server.SetRewardMultiplierMon(m_nRewardMultiplierFactor[BONUS_MON]);
		Server.SetRewardMultiplierTue(m_nRewardMultiplierFactor[BONUS_TUE]);
		Server.SetRewardMultiplierWed(m_nRewardMultiplierFactor[BONUS_WED]);
		Server.SetRewardMultiplierThu(m_nRewardMultiplierFactor[BONUS_THU]);
		Server.SetRewardMultiplierFri(m_nRewardMultiplierFactor[BONUS_FRI]);
		Server.SetRewardMultiplierSat(m_nRewardMultiplierFactor[BONUS_SAT]);
		Server.SetRewardMultiplierSun(m_nRewardMultiplierFactor[BONUS_SUN]);
		Server.SetRewardMultiplierTrigger(m_dRewardMultiplierTrigger);

		COleDateTime time;

		m_dtcStart[BONUS_MON].GetTime(time);
		Server.SetRewardMonStart(time.GetHour(), time.GetMinute());
		m_dtcEnd[BONUS_MON].GetTime(time);
		Server.SetRewardMonEnd(time.GetHour(), time.GetMinute());

		m_dtcStart[BONUS_TUE].GetTime(time);
		Server.SetRewardTueStart(time.GetHour(), time.GetMinute());
		m_dtcEnd[BONUS_TUE].GetTime(time);
		Server.SetRewardTueEnd(time.GetHour(), time.GetMinute());

		m_dtcStart[BONUS_WED].GetTime(time);
		Server.SetRewardWedStart(time.GetHour(), time.GetMinute());
		m_dtcEnd[BONUS_WED].GetTime(time);
		Server.SetRewardWedEnd(time.GetHour(), time.GetMinute());

		m_dtcStart[BONUS_THU].GetTime(time);
		Server.SetRewardThuStart(time.GetHour(), time.GetMinute());
		m_dtcEnd[BONUS_THU].GetTime(time);
		Server.SetRewardThuEnd(time.GetHour(), time.GetMinute());

		m_dtcStart[BONUS_FRI].GetTime(time);
		Server.SetRewardFriStart(time.GetHour(), time.GetMinute());
		m_dtcEnd[BONUS_FRI].GetTime(time);
		Server.SetRewardFriEnd(time.GetHour(), time.GetMinute());

		m_dtcStart[BONUS_SAT].GetTime(time);
		Server.SetRewardSatStart(time.GetHour(), time.GetMinute());
		m_dtcEnd[BONUS_SAT].GetTime(time);
		Server.SetRewardSatEnd(time.GetHour(), time.GetMinute());

		m_dtcStart[BONUS_SUN].GetTime(time);
		Server.SetRewardSunStart(time.GetHour(), time.GetMinute());
		m_dtcEnd[BONUS_SUN].GetTime(time);
		Server.SetRewardSunEnd(time.GetHour(), time.GetMinute());
	}

	return CSSPropertyPage::OnKillActive();
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************
void CServerPropertyPage5::OnTogglePurse1()	{ CheckQualifyingSpendTypes(); }
void CServerPropertyPage5::OnTogglePurse2()	{ CheckQualifyingSpendTypes(); }
void CServerPropertyPage5::OnToggleCash()	{ CheckQualifyingSpendTypes(); }
void CServerPropertyPage5::OnToggleManual() { CheckQualifyingSpendTypes(); }
//$$******************************************************************

void CServerPropertyPage5::CheckQualifyingSpendTypes()
{
	if ( UpdateData() == TRUE )
	{
		bool bAllowed = ( m_checkPurse1.GetCheck() || m_checkPurse2.GetCheck() || m_checkCash.GetCheck() || m_checkManual.GetCheck() );
		GetEditRewardTrip() -> EnableWindow( bAllowed );
		GetEditMultiplierTrigger() ->EnableWindow(bAllowed);
		GetEditSpendLimit() -> EnableWindow(bAllowed);
		m_radioRewardPoints.EnableWindow(bAllowed);
		m_radioRewardValue.EnableWindow(bAllowed);
		GetEditRewardPoints() -> EnableWindow( bAllowed & ( 0 == m_nRewardType ) );
		GetEditRewardValue() -> EnableWindow( bAllowed & ( 1 == m_nRewardType ) );
		m_editRewardComment.EnableWindow( bAllowed );
	}
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************
void CServerPropertyPage5::OnRadioRewardPoints() {	SetRewardDisplay(0); }
void CServerPropertyPage5::OnRadioRewardValue()  {	SetRewardDisplay(1); }
//$$******************************************************************

void CServerPropertyPage5::SetRewardDisplay( int nType )
{
	if ( ( nType < 0 ) || ( nType > 1 ) )
	{
		nType = 0;
	}

	m_nRewardType = nType;

	m_radioRewardPoints.SetCheck( 0 == m_nRewardType );
	m_radioRewardValue.SetCheck( 1 == m_nRewardType );

	GetEditRewardPoints() -> EnableWindow( 0 == m_nRewardType );
	GetEditRewardValue() -> EnableWindow( 1 == m_nRewardType );
		
	if ( 0 == m_nRewardType ) 
		GetEditRewardPoints() -> SetFocus();
	else  
		GetEditRewardValue() -> SetFocus();
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CServerPropertyPage5::OnSpinMultiplierMon(NMHDR *pNMHDR, LRESULT *pResult)
{
	HandleMultiplierSpin( pNMHDR, pResult, BONUS_MON );
}

//$$******************************************************************

void CServerPropertyPage5::OnSpinMultiplierTue(NMHDR *pNMHDR, LRESULT *pResult)
{
	HandleMultiplierSpin( pNMHDR, pResult, BONUS_TUE );
}

//$$******************************************************************

void CServerPropertyPage5::OnSpinMultiplierWed(NMHDR *pNMHDR, LRESULT *pResult)
{
	HandleMultiplierSpin( pNMHDR, pResult, BONUS_WED );
}

//$$******************************************************************

void CServerPropertyPage5::OnSpinMultiplierThu(NMHDR *pNMHDR, LRESULT *pResult)
{
	HandleMultiplierSpin( pNMHDR, pResult, BONUS_THU );
}

//$$******************************************************************

void CServerPropertyPage5::OnSpinMultiplierFri(NMHDR *pNMHDR, LRESULT *pResult)
{
	HandleMultiplierSpin( pNMHDR, pResult, BONUS_FRI );
}

//$$******************************************************************

void CServerPropertyPage5::OnSpinMultiplierSat(NMHDR *pNMHDR, LRESULT *pResult)
{
	HandleMultiplierSpin( pNMHDR, pResult, BONUS_SAT );
}

//$$******************************************************************

void CServerPropertyPage5::OnSpinMultiplierSun(NMHDR *pNMHDR, LRESULT *pResult)
{
	HandleMultiplierSpin( pNMHDR, pResult, BONUS_SUN );
}

//$$******************************************************************

void CServerPropertyPage5::HandleMultiplierSpin(NMHDR *pNMHDR, LRESULT *pResult, int nDay )
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if ( UpdateData() == TRUE )
	{
		if ( pNMUpDown->iDelta > 0 )			// previous / Down
		{	
			if ( m_nRewardMultiplierFactor[nDay] > 0 )
				--m_nRewardMultiplierFactor[nDay];	
		}
		else									// next / Up
		{
			if ( m_nRewardMultiplierFactor[nDay] < 9 )
				++m_nRewardMultiplierFactor[nDay];
		}	
		UpdateData(FALSE);	
		SetBonusEnables( nDay );
	}
	*pResult = 0;
}

//$$******************************************************************

void CServerPropertyPage5::SetBonusEnables( int nDay )
{
	if ( UpdateData() == TRUE )
	{
		bool bAllowed = ( m_nRewardMultiplierFactor[nDay] == 1 ) ? FALSE : TRUE;	
		m_dtcStart[nDay].EnableWindow(bAllowed);
		m_dtcEnd[nDay].EnableWindow(bAllowed);
	}
}

//$$******************************************************************
