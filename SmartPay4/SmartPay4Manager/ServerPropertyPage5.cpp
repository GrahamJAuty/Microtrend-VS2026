//$$******************************************************************
#include "SmartPay4Manager.h"
//$$******************************************************************
#include "ServerPropertyPage5.h"
//$$******************************************************************

CServerPropertyPage5::CServerPropertyPage5()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CServerPropertyPage5::IDD)
{
	m_nRewardType = 0;
	m_nRewardPoints = 0;
	m_dRewardValue = 0.0;
	m_dRewardMultiplierTrigger = 0.0;
	m_dSpendLimit = 0.0;
	m_dRewardTrip = 0.0;
	
	for (int nDay = BONUS_MON; nDay <= BONUS_SUN; nDay++)
	{
		m_nRewardMultiplierFactor[nDay] = 0;
	}
}

//$$******************************************************************

void CServerPropertyPage5::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_CHECK_PURSE1, m_checkPurse1);
	DDX_Control(pDX, IDC_CHECK_PURSE2, m_checkPurse2);
	DDX_Control(pDX, IDC_CHECK_PURSE3, m_checkPurse3);
	DDX_Control(pDX, IDC_CHECK_CASH, m_checkCash);
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

BEGIN_MESSAGE_MAP(CServerPropertyPage5, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_PURSE1, OnTogglePurse1)
	ON_BN_CLICKED(IDC_CHECK_PURSE2, OnTogglePurse2)
	ON_BN_CLICKED(IDC_CHECK_PURSE3, OnTogglePurse3)
	ON_BN_CLICKED(IDC_CHECK_CASH, OnToggleCash)
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
	m_dRewardTrip = Server.GetFileRewardTrip();
	m_nRewardPoints = Server.GetRewardPoints();
	m_dRewardValue = Server.GetRewardValue();
	/*****/
	m_nRewardMultiplierFactor[BONUS_MON] = Server.GetRewardMultiplierFactorMon();
	m_nRewardMultiplierFactor[BONUS_TUE] = Server.GetRewardMultiplierFactorTue();
	m_nRewardMultiplierFactor[BONUS_WED] = Server.GetRewardMultiplierFactorWed();
	m_nRewardMultiplierFactor[BONUS_THU] = Server.GetRewardMultiplierFactorThu();
	m_nRewardMultiplierFactor[BONUS_FRI] = Server.GetRewardMultiplierFactorFri();
	m_nRewardMultiplierFactor[BONUS_SAT] = Server.GetRewardMultiplierFactorSat();
	m_nRewardMultiplierFactor[BONUS_SUN] = Server.GetRewardMultiplierFactorSun();
	/*****/
	m_dRewardMultiplierTrigger = Server.GetRewardMultiplierTrigger();
	/*****/
	
	CPropertyPage::OnInitDialog();

	m_checkPurse1.SetCheck(Server.GetRewardIncludePurse1SpendFlag());
	m_checkPurse2.SetCheck(Server.GetRewardIncludePurse2SpendFlag());
	m_checkPurse3.SetCheck(System.GetEnablePurse3Flag() && Server.GetRewardIncludePurse3SpendFlag());
	m_checkPurse3.EnableWindow(System.GetEnablePurse3Flag());
	m_checkCash.SetCheck(Server.GetRewardIncludeCashSpendFlag());
	/*****/
	SetRewardDisplay(Server.GetRewardType());
	m_editRewardComment.SetWindowText(Server.GetRewardComment());

	SubclassEdit ( IDC_EDIT_SPENDLIMIT,	SS_NUM_SDP, 11, "%.2f" );
	SubclassEdit ( IDC_EDIT_REWARDTRIP,	SS_NUM_SDP, 11, "%.2f" );
	SubclassEdit ( IDC_EDIT_REWARDPOINTS, SS_NUM_S,	 8, "%ld" );
	SubclassEdit ( IDC_EDIT_REWARDVALUE, SS_NUM_SDP, 11, "%.2f" );
	SubclassEdit ( IDC_EDIT_MULTIPLIERTRIGGER, SS_NUM_SDP, 11, "%.2f" );
	
	for ( int nDay = BONUS_MON; nDay <= BONUS_SUN; nDay++ )
	{
		m_dtcStart[nDay].SetFormat ( "HH:mm" );
		m_dtcEnd[nDay].SetFormat ( "HH:mm" );
	}

	COleDateTime time;
	time.SetTime(Server.GetMonStart().GetHour(), Server.GetMonStart().GetMinute(), 0);
	m_dtcStart[BONUS_MON].SetTime(time);
	time.SetTime(Server.GetMonEnd().GetHour(), Server.GetMonEnd().GetMinute(), 0);
	m_dtcEnd[BONUS_MON].SetTime(time);

	time.SetTime(Server.GetTueStart().GetHour(), Server.GetTueStart().GetMinute(), 0);
	m_dtcStart[BONUS_TUE].SetTime(time);
	time.SetTime(Server.GetTueEnd().GetHour(), Server.GetTueEnd().GetMinute(), 0);
	m_dtcEnd[BONUS_TUE].SetTime(time);

	time.SetTime(Server.GetWedStart().GetHour(), Server.GetWedStart().GetMinute(), 0);
	m_dtcStart[BONUS_WED].SetTime(time);
	time.SetTime(Server.GetWedEnd().GetHour(), Server.GetWedEnd().GetMinute(), 0);
	m_dtcEnd[BONUS_WED].SetTime(time);

	time.SetTime(Server.GetThuStart().GetHour(), Server.GetThuStart().GetMinute(), 0);
	m_dtcStart[BONUS_THU].SetTime(time);
	time.SetTime(Server.GetThuEnd().GetHour(), Server.GetThuEnd().GetMinute(), 0);
	m_dtcEnd[BONUS_THU].SetTime(time);

	time.SetTime(Server.GetFriStart().GetHour(), Server.GetFriStart().GetMinute(), 0);
	m_dtcStart[BONUS_FRI].SetTime(time);
	time.SetTime(Server.GetFriEnd().GetHour(), Server.GetFriEnd().GetMinute(), 0);
	m_dtcEnd[BONUS_FRI].SetTime(time);

	time.SetTime(Server.GetSatStart().GetHour(), Server.GetSatStart().GetMinute(), 0);
	m_dtcStart[BONUS_SAT].SetTime(time);
	time.SetTime(Server.GetSatEnd().GetHour(), Server.GetSatEnd().GetMinute(), 0);
	m_dtcEnd[BONUS_SAT].SetTime(time);

	time.SetTime(Server.GetSunStart().GetHour(), Server.GetSunStart().GetMinute(), 0);
	m_dtcStart[BONUS_SUN].SetTime(time);
	time.SetTime(Server.GetSunEnd().GetHour(), Server.GetSunEnd().GetMinute(), 0);
	m_dtcEnd[BONUS_SUN].SetTime(time);

	for ( int nDay = BONUS_MON; nDay <= BONUS_SUN; nDay++ )
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
	if ( UpdateData() == TRUE )
	{
		CString strComment;
		m_editRewardComment.GetWindowText( strComment );

		Server.SetRewardIncludePurse1SpendFlag(IsTicked(m_checkPurse1));
		Server.SetRewardIncludePurse2SpendFlag(IsTicked(m_checkPurse2));
		Server.SetRewardIncludePurse3SpendFlag(IsTicked(m_checkPurse3));
		Server.SetRewardIncludeCashSpendFlag(IsTicked(m_checkCash));
		Server.SetRewardUpperSpendLimit(m_dSpendLimit);
		/*****/
		Server.SetFileRewardTrip(m_dRewardTrip);
		Server.SetRewardType(m_nRewardType);
		Server.SetRewardPoints(m_nRewardPoints);
		Server.SetRewardValue(m_dRewardValue);
		Server.SetRewardComment(strComment);
		/*****/
		Server.SetRewardMultiplierFactorMon(m_nRewardMultiplierFactor[BONUS_MON]);
		Server.SetRewardMultiplierFactorTue(m_nRewardMultiplierFactor[BONUS_TUE]);
		Server.SetRewardMultiplierFactorWed(m_nRewardMultiplierFactor[BONUS_WED]);
		Server.SetRewardMultiplierFactorThu(m_nRewardMultiplierFactor[BONUS_THU]);
		Server.SetRewardMultiplierFactorFri(m_nRewardMultiplierFactor[BONUS_FRI]);
		Server.SetRewardMultiplierFactorSat(m_nRewardMultiplierFactor[BONUS_SAT]);
		Server.SetRewardMultiplierFactorSun(m_nRewardMultiplierFactor[BONUS_SUN]);
		Server.SetRewardMultiplierTrigger(m_dRewardMultiplierTrigger);

		COleDateTime time;

		m_dtcStart[BONUS_MON].GetTime ( time );
		Server.SetMonStart ( time.GetHour(), time.GetMinute() );
		m_dtcEnd[BONUS_MON].GetTime ( time );
		Server.SetMonEnd ( time.GetHour(), time.GetMinute() );

		m_dtcStart[BONUS_TUE].GetTime ( time );
		Server.SetTueStart ( time.GetHour(), time.GetMinute() );
		m_dtcEnd[BONUS_TUE].GetTime ( time );
		Server.SetTueEnd ( time.GetHour(), time.GetMinute() );

		m_dtcStart[BONUS_WED].GetTime ( time );
		Server.SetWedStart ( time.GetHour(), time.GetMinute() );
		m_dtcEnd[BONUS_WED].GetTime ( time );
		Server.SetWedEnd ( time.GetHour(), time.GetMinute() );

		m_dtcStart[BONUS_THU].GetTime ( time );
		Server.SetThuStart ( time.GetHour(), time.GetMinute() );
		m_dtcEnd[BONUS_THU].GetTime ( time );
		Server.SetThuEnd ( time.GetHour(), time.GetMinute() );

		m_dtcStart[BONUS_FRI].GetTime ( time );
		Server.SetFriStart ( time.GetHour(), time.GetMinute() );
		m_dtcEnd[BONUS_FRI].GetTime ( time );
		Server.SetFriEnd ( time.GetHour(), time.GetMinute() );

		m_dtcStart[BONUS_SAT].GetTime ( time );
		Server.SetSatStart ( time.GetHour(), time.GetMinute() );
		m_dtcEnd[BONUS_SAT].GetTime ( time );
		Server.SetSatEnd ( time.GetHour(), time.GetMinute() );

		m_dtcStart[BONUS_SUN].GetTime ( time );
		Server.SetSunStart ( time.GetHour(), time.GetMinute() );
		m_dtcEnd[BONUS_SUN].GetTime ( time );
		Server.SetSunEnd ( time.GetHour(), time.GetMinute() );
	}

	return CSSPropertyPage::OnKillActive();
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************
void CServerPropertyPage5::OnTogglePurse1()	{ CheckQualifyingSpendTypes(); }
void CServerPropertyPage5::OnTogglePurse2()	{ CheckQualifyingSpendTypes(); }
void CServerPropertyPage5::OnTogglePurse3()	{ CheckQualifyingSpendTypes(); }
void CServerPropertyPage5::OnToggleCash()	{ CheckQualifyingSpendTypes(); }
//$$******************************************************************

void CServerPropertyPage5::CheckQualifyingSpendTypes()
{
	if ( UpdateData() == TRUE )
	{
		bool bAllowed = ( m_checkPurse1.GetCheck() || m_checkPurse2.GetCheck() || m_checkPurse3.GetCheck() || m_checkCash.GetCheck() );
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
