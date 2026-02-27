//$$******************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNameSpaces.h"
//$$******************************************************************
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLPurse2Handler.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLTable_PeriodRefresh\SQLRepositoryPeriodRefresh.h"
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SmartPayAuditFile.h"
#include "..\SmartPay4Shared\SystemData.h"
//$$******************************************************************
#include "AddCashDlg.h"
#include "AddPointsDlg.h"
#include "AuditReportWrapper.h"
#include "EditRecordPropertySheet.h"
#include "RedeemPointsDlg.h"
//$$******************************************************************
#include "EditRecordPropertyPageBalance.h"
//$$******************************************************************

CEditRecordPropertyPageBalance::CEditRecordPropertyPageBalance(CSQLRowAccountFull& Row )
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CEditRecordPropertyPageBalance::IDD)
	,m_SQLRowAccount(Row)
{
	m_nMode = 0;
	m_pEditAccountBuffer = NULL;
	m_pLeaversData = NULL;
	m_bAuditTurnedOff = FALSE;
	SQLRowSetAuditPeriod.LoadPeriods();
	m_dPendingPebbleAdjust = 0.0;
	m_bDonePurse1Topup = FALSE;
	m_bDonePebbleAdjust = FALSE;

	// Primary audit is for a pc line
	m_atc1.Set(&m_SQLRowAccount);
	m_atc1.SetOperatorID(Passwords.GetPasswordInUse());

	// second audit record to catch manual changes
	m_atc2.Set(&m_SQLRowAccount);
	m_atc2.SetOperatorID(Passwords.GetPasswordInUse());
}

//$$******************************************************************

CEditRecordPropertyPageBalance::~CEditRecordPropertyPageBalance()
{
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::SetPointer ( CEditAccountBuffer* pEditAccountBuffer, int nMode, CLeaversHandler* pLeaversData)
{
	m_pEditAccountBuffer = pEditAccountBuffer;
	m_nMode = nMode;
	m_pLeaversData = pLeaversData;
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::DoDataExchange(CDataExchange* pDX)
{
	CSSPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_PURSE1_TOPUP, m_buttonPurse1Topup);
	DDX_Control(pDX, IDC_BUTTON_PURSE2_TOPUP, m_buttonPurse2Topup);
	DDX_Control(pDX, IDC_BUTTON_PURSE3_TOPUP, m_buttonPurse3Topup);
	DDX_Control(pDX, IDC_BUTTON_PEBBLE, m_buttonPebble);
	DDX_Control(pDX, IDC_STATIC_REFRESH, m_staticRefresh);
	DDX_Control(pDX, IDC_EDIT_PURSE1_USED_TIME, m_editPurse1UsedTime);
	DDX_Control(pDX, IDC_EDIT_PURSE2_USED_TIME, m_editPurse2UsedTime);
	DDX_Control(pDX, IDC_EDIT_PURSE3_USED_TIME, m_editPurse3UsedTime);
	DDX_Control(pDX, IDC_EDIT_PURSE2_REFRESH_TIME, m_editPurse2RefreshTime);
	DDX_Control(pDX, IDC_BUTTON_POINTS_TOPUP, m_buttonPointsTopup);
	DDX_Control(pDX, IDC_BUTTON_POINTS_REDEEM, m_buttonPointsRedeem);
	DDX_Control(pDX, IDC_STATIC_PURSE1_MAXSPEND, m_staticPurse1MaxSpend);
	DDX_Control(pDX, IDC_STATIC_PURSE1_OVERDRAFT, m_staticPurse1Overdraft);
	DDX_Control(pDX, IDC_STATIC_PURSE1_LIABILITY, m_staticPurse1LabelLiability);
	DDX_Control(pDX, IDC_STATIC_PURSE1_CREDIT, m_staticPurse1LabelCredit);
	DDX_Control(pDX, IDC_STATIC_PURSE1_TOTAL, m_staticPurse1LabelTotal);
	DDX_Control(pDX, IDC_STATIC_PURSE3_MAXSPEND, m_staticPurse3MaxSpend);
	DDX_Control(pDX, IDC_STATIC_PURSE3_OVERDRAFT, m_staticPurse3Overdraft);
	DDX_Control(pDX, IDC_STATIC_PURSE3_LABEL1, m_staticPurse3Label1);
	DDX_Control(pDX, IDC_STATIC_PURSE3_LABEL2, m_staticPurse3Label2);
	DDX_Control(pDX, IDC_STATIC_PURSE3_LABEL3, m_staticPurse3Label3);
	DDX_Control(pDX, IDC_STATIC_PURSE3_LABEL4, m_staticPurse3Label4);
	DDX_Control(pDX, IDC_STATIC_PURSE3_LIABILITY, m_staticPurse3LabelLiability);
	DDX_Control(pDX, IDC_STATIC_PURSE3_CREDIT, m_staticPurse3LabelCredit);
	DDX_Control(pDX, IDC_STATIC_PURSE3_TOTAL, m_staticPurse3LabelTotal);
	DDX_Control(pDX, IDC_STATIC_PURSE3_LABEL8, m_staticPurse3Label8);
	DDX_Control(pDX, IDC_STATIC_PURSE3_LABEL9, m_staticPurse3Label9);
	DDX_Control(pDX, IDC_STATIC_PURSE3_LABEL10, m_staticPurse3Label10);
	DDX_Control(pDX, IDC_STATIC_POINTS_LABEL1, m_staticPointsLabel1);
	DDX_Control(pDX, IDC_STATIC_POINTS_LABEL2, m_staticPointsLabel2);
	DDX_Control(pDX, IDC_STATIC_POINTS_LABEL3, m_staticPointsLabel3);
	DDX_Control(pDX, IDC_STATIC_POINTS_LABEL4, m_staticPointsLabel4);
	DDX_Control(pDX, IDC_STATIC_GENERAL_LABEL1, m_staticGeneralLabel1);
	DDX_Control(pDX, IDC_STATIC_GENERAL_LABEL2, m_staticGeneralLabel2);
	DDX_Control(pDX, IDC_STATIC_GENERAL_LABEL3, m_staticGeneralLabel3);
	DDX_Control(pDX, IDC_STATIC_AUDITOFF, m_staticAuditOff);

	if (TRUE == pDX->m_bSaveAndValidate)
	{
		if (ValidateMaxSpend(
			GetEditPurse1MaxSpend(),
			Account::MaxSpendPurse1Period1.Min, 
			Account::MaxSpendPurse1Period1.Max, 
			"Purse1 Max Spend") == FALSE)
		{
			pDX->Fail();
			return;
		}

		if (ValidateMaxSpend(
			GetEditPurse3MaxSpend(), 
			Account::MaxSpendPurse3Period1.Min, 
			Account::MaxSpendPurse3Period1.Max,
			"Purse3 Max Spend") == FALSE)
		{
			pDX->Fail();
			return;
		}

		if (ValidateMaxSpend(
			GetEditPurse1Overdraft(),
			Account::MaxOverdraftPurse1Period1.Min,
			Account::MaxOverdraftPurse1Period1.Max, 
			"Purse1 Max Overdraft") == FALSE)
		{
			pDX->Fail();
			return;
		}

		if (ValidateMaxSpend(
			GetEditPurse3Overdraft(), 
			Account::MaxOverdraftPurse3Period1.Min,
			Account::MaxOverdraftPurse3Period1.Max, 
			"Purse3 Max Overdraft") == FALSE)
		{
			pDX->Fail();
			return;
		}
	}
}

//$$******************************************************************

bool CEditRecordPropertyPageBalance::ValidateMaxSpend(CEdit* pEdit, double dMin, double dMax, CString strType)
{
	bool bResult = TRUE;

	if (pEdit->IsWindowEnabled() == TRUE)
	{
		double dValue = GetEditBoxDouble(pEdit);

		if ((dValue < dMin) || (dValue > dMax))
		{
			CString strError = "";
			strError.Format("The %s value is out of range.\n\nPlease enter a value between %.2f and %.2f.",
				(const char*)strType,
				dMin,
				dMax);

			Prompter.Error(strError, "Error");
			pEdit->SetFocus();
			pEdit->SetSel(0, -1);
			bResult = FALSE;
		}
	}

	return bResult;
}

//*******************************************************

BEGIN_MESSAGE_MAP(CEditRecordPropertyPageBalance, CSSAutoShutdownPropertyPage)
	ON_STN_DBLCLK(IDC_STATIC_PURSE1_BALANCE, OnDoubleClickStaticBalance)
	ON_BN_CLICKED(IDC_BUTTON_PURSE1_TOPUP, OnButtonPurse1Topup)
	ON_BN_CLICKED(IDC_BUTTON_PURSE2_TOPUP, OnButtonPurse2Topup)
	ON_BN_CLICKED(IDC_BUTTON_PURSE3_TOPUP, OnButtonPurse3Topup)
	ON_BN_CLICKED(IDC_BUTTON_PEBBLE, OnButtonPebble)
	ON_EN_KILLFOCUS(IDC_EDIT_PURSE1_LIABILITY, OnKillFocusPurse)
	ON_EN_KILLFOCUS(IDC_EDIT_PURSE1_CREDIT, OnKillFocusPurse)
	ON_EN_KILLFOCUS(IDC_EDIT_PURSE2_BALANCE, OnKillFocusPurse)
	ON_EN_KILLFOCUS(IDC_EDIT_PURSE3_LIABILITY, OnKillFocusPurse)
	ON_EN_KILLFOCUS(IDC_EDIT_PURSE3_CREDIT, OnKillFocusPurse)
	ON_EN_KILLFOCUS(IDC_EDIT_POINTS_BALANCE, OnKillFocusPoints)
	ON_BN_CLICKED(IDC_BUTTON_POINTS_TOPUP, OnButtonPointsTopup)
	ON_BN_CLICKED(IDC_BUTTON_POINTS_REDEEM, OnButtonPointsRedeem)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CEditRecordPropertyPageBalance::OnInitDialog()
{
	CSSPropertyPage::OnInitDialog();

	m_staticAuditOff.ShowWindow(SW_HIDE);
	m_buttonPebble.ShowWindow(SW_HIDE);

	SubclassEdit(IDC_EDIT_PURSE1_LIABILITY, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE1_CREDIT, SS_NUM_DP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE1_TOTAL, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE1_OVERDRAFT, SS_NUM_SDP, 9, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE1_MAXSPEND, SS_NUM_SDP, 9, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE1_STD, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE1_USED_DATE, SS_DATE, 10, "");
	SubclassEdit(IDC_EDIT_PURSE1_USED_VALUE, SS_NUM_SDP, 11, "%.2f");

	SubclassEdit(IDC_EDIT_PURSE2_BALANCE, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE2_STD, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE2_USED_DATE, SS_DATE, 10, "");
	SubclassEdit(IDC_EDIT_PURSE2_USED_VALUE, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE2_REFRESH_DATE, SS_DATE, 10, "");
	SubclassEdit(IDC_EDIT_PURSE2_REFRESH_VALUE, SS_NUM_SDP, 11, "%.2f");

	SubclassEdit(IDC_EDIT_PURSE3_LIABILITY, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE3_CREDIT, SS_NUM_DP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE3_TOTAL, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE3_OVERDRAFT, SS_NUM_SDP, 9, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE3_MAXSPEND, SS_NUM_SDP, 9, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE3_STD, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE3_USED_DATE, SS_DATE, 10, "");
	SubclassEdit(IDC_EDIT_PURSE3_USED_VALUE, SS_NUM_SDP, 11, "%.2f");

	SubclassEdit(IDC_EDIT_POINTS_BALANCE, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_POINTS_TODATE, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_POINTS_VALUE, SS_NUM_SDP, 11, "%.2f");

	SubclassEdit(IDC_EDIT_GENERAL_BALANCE, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_GENERAL_CASHSPEND, SS_NUM_SDP, 11, "%.2f");

	//COPY CURRENT VALUES INTO POINTS FIELDS
	SetEditBoxInt(*GetEditPointsBalance(), m_SQLRowAccount.GetPoints());
	SetEditBoxInt(*GetEditPointsToDate(), m_SQLRowAccount.GetPointsToDate());

	//COPY CURRENT PURSE VALUES INTO EDIT BOXES
	SetEditBoxDouble(GetEditPurse1Liability(), m_SQLRowAccount.GetPurse1Liability());
	SetEditBoxDouble(GetEditPurse1Credit(), m_SQLRowAccount.GetPurse1Credit());
	SetEditBoxDouble(GetEditPurse1Total(), m_SQLRowAccount.GetPurse1Balance());
	SetEditBoxDouble(GetEditPurse1STD(), m_SQLRowAccount.GetPurse1SpendToDate());

	SetEditBoxDouble(GetEditPurse2Balance(), m_SQLRowAccount.GetPurse2Balance());
	SetEditBoxDouble(GetEditPurse2STD(), m_SQLRowAccount.GetPurse2SpendToDate());

	SetEditBoxDouble(GetEditPurse3Liability(), m_SQLRowAccount.GetPurse3Liability());
	SetEditBoxDouble(GetEditPurse3Credit(), m_SQLRowAccount.GetPurse3Credit());
	SetEditBoxDouble(GetEditPurse3Total(), m_SQLRowAccount.GetPurse3Balance());
	SetEditBoxDouble(GetEditPurse3STD(), m_SQLRowAccount.GetPurse3SpendToDate());

	//PURSE1 LAST USED
	{
		CString strDateLastUsed = m_SQLRowAccount.GetPurse1LastSpendDate();

		if (strDateLastUsed.GetLength() > 0)
		{
			GetEditPurse1UsedDate()->SetWindowText(strDateLastUsed);
			m_editPurse1UsedTime.SetWindowText(m_SQLRowAccount.GetPurse1LastSpendTime());
			SetEditBoxDouble(GetEditPurse1UsedValue(), m_SQLRowAccount.GetPurse1LastSpend());
		}
		else
		{
			GetEditPurse1UsedValue()->SetWindowText("");
		}
	}

	//PURSE2 LAST USED
	{
		CString strDateLastUsed = m_SQLRowAccount.GetPurse2LastSpendDate();

		if (strDateLastUsed.GetLength() > 0)
		{
			GetEditPurse2UsedDate()->SetWindowText(strDateLastUsed);
			m_editPurse2UsedTime.SetWindowText(m_SQLRowAccount.GetPurse2LastSpendTime());
			SetEditBoxDouble(GetEditPurse2UsedValue(), m_SQLRowAccount.GetPurse2LastSpend());
		}
		else
		{
			GetEditPurse2UsedValue()->SetWindowText("");
		}
	}

	{
		CString strDateRefresh = m_SQLRowAccount.GetPurse2RefreshedDate();

		if (strDateRefresh.GetLength() > 0)
		{
			GetEditPurse2RefreshDate()->SetWindowText(strDateRefresh);
			m_editPurse2RefreshTime.SetWindowText(m_SQLRowAccount.GetPurse2RefreshedTime());
		}
	}

	//PURSE3 LAST USED
	{
		CString strDateLastUsed = m_SQLRowAccount.GetPurse3LastSpendDate();

		if (strDateLastUsed.GetLength() > 0)
		{
			GetEditPurse3UsedDate()->SetWindowText(strDateLastUsed);
			m_editPurse3UsedTime.SetWindowText(m_SQLRowAccount.GetPurse3LastSpendTime());
			SetEditBoxDouble(GetEditPurse3UsedValue(), m_SQLRowAccount.GetPurse3LastSpend());
		}
		else
		{
			GetEditPurse3UsedValue()->SetWindowText("");
		}
	}

	//COPY CURRENT VALUES INTO GENERAL FIELDS
	SetEditBoxDouble(GetEditGeneralCashSpend(), m_SQLRowAccount.GetCashSpendToDate());
	GetEditGeneralCashSpend()->SetReadOnly(TRUE);

	return TRUE;
}

//$$******************************************************************

BOOL CEditRecordPropertyPageBalance::OnSetActive()
{
	m_SQLRowGroup.Reset();
	m_SQLRowGroup.SetGroupNo(m_pEditAccountBuffer->m_nGroupNo);
	CSQLRepositoryGroup RepoGroup;
	RepoGroup.SelectRow(m_SQLRowGroup, NULL);

	m_atc1.SetUserName(m_pEditAccountBuffer->m_strCardName);
	m_atc1.SetGroupNo(m_pEditAccountBuffer->m_nGroupNo);
	m_atc2.SetUserName(m_pEditAccountBuffer->m_strCardName);
	m_atc2.SetGroupNo(m_pEditAccountBuffer->m_nGroupNo);

	RemoveUnusedControls();
	SetEditEnablesSystem();
	SetOneOffFields();
	SetEditEnablesGroup();
	SetMaxSpendFields();
	SetGroupFields();
	SetAccountBalance();
	SetTabStops();
	SetPointsValue();

	return CSSPropertyPage::OnSetActive();
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::SetEditEnablesSystem()
{
	if (DB_READWRITE == m_nMode)
	{
		m_editEnables.m_bSystemEditMaxSpend = Passwords.IsEditGeneralAllowed();
		m_editEnables.m_bEditPurses = Passwords.IsEditPurseAllowed();
		m_editEnables.m_bEditSpendTD = Passwords.IsEditPurseAllowed();
		m_editEnables.m_bShowP1CashButton = Passwords.IsTopupButtonAllowed();
		m_editEnables.m_bShowP2CashButton = Passwords.IsTopupButtonAllowed();
		m_editEnables.m_bShowP3CashButton = Passwords.IsTopupButtonAllowed();
	}
	else
	{
		m_editEnables.m_bSystemEditMaxSpend = FALSE;
		m_editEnables.m_bEditPurses = FALSE;
		m_editEnables.m_bEditSpendTD = FALSE;
		m_editEnables.m_bShowP1CashButton = FALSE;
		m_editEnables.m_bShowP2CashButton = FALSE;
		m_editEnables.m_bShowP3CashButton = FALSE;
	}

	m_editEnables.m_bEditPurse1 = m_editEnables.m_bEditPurses && System.GetActiveEditPurse1Flag();
	m_editEnables.m_bEditPurse2 = m_editEnables.m_bEditPurses && System.GetActiveEditPurse2Flag();
	m_editEnables.m_bEditPurse3 = m_editEnables.m_bEditPurses && System.GetActiveEditPurse3Flag();

	m_editEnables.m_bShowMaxSpend = TRUE;
	m_editEnables.m_bShowOverdraft = Server.GetAllowOverdraftFlag();

	if (m_pLeaversData != NULL)
	{
		m_editEnables.m_bEditPurses = TRUE;
		m_editEnables.m_bEditPurse1 = TRUE;
		m_editEnables.m_bEditPurse2 = TRUE;
		m_editEnables.m_bEditPurse3 = TRUE;
		m_editEnables.m_bSystemEditMaxSpend = FALSE;
		m_editEnables.m_bEditSpendTD = FALSE;
		m_editEnables.m_bShowP1CashButton = FALSE;
		m_editEnables.m_bShowP2CashButton = FALSE;
		m_editEnables.m_bShowP3CashButton = FALSE;
	}

	if ((System.GetShowTopupButtonFlag() == FALSE) || (IsHotlisted() == TRUE))
	{
		m_editEnables.m_bShowP1CashButton = FALSE;
		m_editEnables.m_bShowP2CashButton = FALSE;
		m_editEnables.m_bShowP3CashButton = FALSE;
	}

	if ( m_SQLRowGroup.GetRefreshType() != nREFRESHTYPE_MANUAL)
	{
		m_editEnables.m_bShowP2CashButton = FALSE;
	}

	if (FALSE == System.GetEnablePurse3Flag())
	{
		m_editEnables.m_bEditPurse3 = FALSE;
		m_editEnables.m_bShowP3CashButton = FALSE;
	}

	m_editEnables.m_bShowPointsTopupButton = TRUE;
	m_editEnables.m_bShowPointsTopupButton &= System.GetActiveShowPointsButtonFlag();
	m_editEnables.m_bShowPointsTopupButton &= (IsHotlisted() == FALSE);
	m_editEnables.m_bShowPointsTopupButton &= Passwords.IsPointsButtonAllowed();
	
	m_editEnables.m_bShowPointsRedeemButton = TRUE;
	m_editEnables.m_bShowPointsRedeemButton &= Server.RedeemPointsEnabled();
	m_editEnables.m_bShowPointsRedeemButton &= (IsHotlisted() == FALSE);
	
	m_editEnables.m_bEditPoints = TRUE;
	m_editEnables.m_bEditPoints &= System.GetActiveEditPointsFlag();
	m_editEnables.m_bEditPoints &= Passwords.IsEditPointsAllowed();
	m_editEnables.m_bEditPoints &= (IsHotlisted() == FALSE);
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::SetEditEnablesGroup()
{
	m_editEnables.m_bShowPurse2 = FALSE;
	m_editEnables.m_bShowPurse2Refresh = FALSE;

	if (m_SQLRowGroup.IsPurse2Refreshable() == TRUE)
	{
		m_editEnables.m_bShowPurse2 = TRUE;
		m_editEnables.m_bShowPurse2Refresh = TRUE;
	}
	else
	{
		if (m_SQLRowGroup.GetRefreshType() != nREFRESHTYPE_MANUAL)				//13/05/2016 - v1.12b
		{
			CSQLRowPeriodRefresh RowRefresh;
			RowRefresh.SetUserID(m_SQLRowAccount.GetUserID());

			CSQLRepositoryPeriodRefresh RepoPeriod;
			if ( RepoPeriod.SelectRow(RowRefresh,NULL).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				m_editEnables.m_bShowPurse2 = TRUE;
			}
		}
	}

	m_pointsRedeemer.m_bDisabled = FALSE;
	m_pointsRedeemer.m_nTrip = Server.GetActiveRedeemPointsTrip();
	m_pointsRedeemer.m_dValue = Server.GetRedeemValue();
	m_pointsRedeemer.m_nPurse = Server.GetRedeemPurse();

	if (m_SQLRowGroup.GetActiveRedeemFlag() == TRUE)							// group specifc redeem points
	{
		m_pointsRedeemer.m_bDisabled = m_SQLRowGroup.GetRedeemDisable();
		m_pointsRedeemer.m_nTrip = m_SQLRowGroup.GetRedeemTrip();
		m_pointsRedeemer.m_dValue = m_SQLRowGroup.GetRedeemValue();
		m_pointsRedeemer.m_nPurse = m_SQLRowGroup.GetRedeemPurse();
	}

	//MAX SPEND AND OVERDRAFT
	m_editEnables.m_bMaxSpendFromGroupP1 = FALSE;
	m_editEnables.m_bMaxSpendFromGroupP3 = FALSE;
	m_editEnables.m_bMaxSpendFromItemP1 = FALSE;
	m_editEnables.m_bMaxSpendFromItemP3 = FALSE;
	m_editEnables.m_bAccountEditMaxSpend = TRUE;

	switch (m_SQLRowGroup.GetMaxSpendType())
	{
	case nGRPSPENDTYPE_DAILY_LOCKED:
		m_editEnables.m_bMaxSpendFromGroupP1 = TRUE;
		m_editEnables.m_bMaxSpendFromGroupP3 = TRUE;
		m_editEnables.m_bAccountEditMaxSpend = FALSE;
		break;

	case nGRPSPENDTYPE_DAILY_RECORD1:
		m_editEnables.m_bMaxSpendFromGroupP1 = ((m_SQLRowAccount.GetMaxSpendPurse1Period1() == 0) && (m_SQLRowAccount.GetMaxOverdraftPurse1Period1() == 0));
		m_editEnables.m_bMaxSpendFromGroupP3 = ((m_SQLRowAccount.GetMaxSpendPurse3Period1() == 0) && (m_SQLRowAccount.GetMaxOverdraftPurse3Period1() == 0));
		m_editEnables.m_bMaxSpendFromItemP1 = (FALSE == m_editEnables.m_bMaxSpendFromGroupP1);
		m_editEnables.m_bMaxSpendFromItemP3 = (FALSE == m_editEnables.m_bMaxSpendFromGroupP3);
		break;

	case nGRPSPENDTYPE_DAILY_RECORD2:
		m_editEnables.m_bMaxSpendFromItemP1 = m_pEditAccountBuffer->m_bOwnMaxSpend;
		m_editEnables.m_bMaxSpendFromGroupP1 = (FALSE == m_editEnables.m_bMaxSpendFromItemP1);
		m_editEnables.m_bMaxSpendFromItemP3 = m_editEnables.m_bMaxSpendFromItemP1;
		m_editEnables.m_bMaxSpendFromGroupP3 = m_editEnables.m_bMaxSpendFromGroupP1;
		if (FALSE == m_editEnables.m_bMaxSpendFromItemP1) m_editEnables.m_bAccountEditMaxSpend = FALSE;
		break;

	case nGRPSPENDTYPE_PERIOD_LOCKED:
	case nGRPSPENDTYPE_PERIOD_RECORD1:
	case nGRPSPENDTYPE_PERIOD_RECORD2:
	default:
		m_editEnables.m_bAccountEditMaxSpend = FALSE;
		m_editEnables.m_bShowMaxSpend = FALSE;
		m_editEnables.m_bShowOverdraft = FALSE;
		break;
	}
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::SetMaxSpendFields()
{
	if (TRUE == m_editEnables.m_bMaxSpendFromGroupP1)
	{
		SetEditBoxDouble(GetEditPurse1MaxSpend(), m_SQLRowGroup.GetMaxSpendPurse1Period1());
		SetEditBoxDouble(GetEditPurse1Overdraft(), m_SQLRowGroup.GetMaxOverdraftPurse1Period1());
	}
	else if (FALSE == m_editEnables.m_bMaxSpendFromItemP1)
	{
		SetEditBoxDouble(GetEditPurse1MaxSpend(), 0.0);
		SetEditBoxDouble(GetEditPurse1Overdraft(), 0.0);
	}
	else
	{
		SetEditBoxDouble(GetEditPurse1MaxSpend(), m_SQLRowAccount.GetMaxSpendPurse1Period1());
		SetEditBoxDouble(GetEditPurse1Overdraft(), m_SQLRowAccount.GetMaxOverdraftPurse1Period1());
	}

	if (TRUE == m_editEnables.m_bMaxSpendFromGroupP3)
	{
		SetEditBoxDouble(GetEditPurse3MaxSpend(), m_SQLRowGroup.GetMaxSpendPurse3Period1());
		SetEditBoxDouble(GetEditPurse3Overdraft(), m_SQLRowGroup.GetMaxOverdraftPurse3Period1());
	}
	else if (FALSE == m_editEnables.m_bMaxSpendFromItemP3)
	{
		SetEditBoxDouble(GetEditPurse3MaxSpend(), 0.0);
		SetEditBoxDouble(GetEditPurse3Overdraft(), 0.0);
	}
	else
	{
		SetEditBoxDouble(GetEditPurse3MaxSpend(), m_SQLRowAccount.GetMaxSpendPurse3Period1());
		SetEditBoxDouble(GetEditPurse3Overdraft(), m_SQLRowAccount.GetMaxOverdraftPurse3Period1());
	}

	// max spend
	ShowAndEnableWindow(GetEditPurse1MaxSpend(), m_editEnables.m_bShowMaxSpend, m_editEnables.m_bAccountEditMaxSpend && m_editEnables.m_bSystemEditMaxSpend );
	ShowAndEnableWindow(GetEditPurse3MaxSpend(), m_editEnables.m_bShowMaxSpend, m_editEnables.m_bAccountEditMaxSpend && m_editEnables.m_bSystemEditMaxSpend );
	ShowAndEnableWindow(&m_staticPurse1MaxSpend, m_editEnables.m_bShowMaxSpend, m_editEnables.m_bShowMaxSpend);
	ShowAndEnableWindow(&m_staticPurse3MaxSpend, m_editEnables.m_bShowMaxSpend, m_editEnables.m_bShowMaxSpend);

	// over spend
	ShowAndEnableWindow(GetEditPurse1Overdraft(), m_editEnables.m_bShowOverdraft, m_editEnables.m_bAccountEditMaxSpend && m_editEnables.m_bSystemEditMaxSpend );
	ShowAndEnableWindow(GetEditPurse3Overdraft(), m_editEnables.m_bShowOverdraft, m_editEnables.m_bAccountEditMaxSpend && m_editEnables.m_bSystemEditMaxSpend );
	ShowAndEnableWindow(&m_staticPurse1Overdraft, m_editEnables.m_bShowOverdraft, m_editEnables.m_bShowOverdraft);
	ShowAndEnableWindow(&m_staticPurse3Overdraft, m_editEnables.m_bShowOverdraft, m_editEnables.m_bShowOverdraft);
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::SetGroupFields()
{
	if (FALSE == m_editEnables.m_bShowPurse2Refresh)
	{
		m_staticRefresh.ShowWindow(SW_HIDE);
		GetEditPurse2RefreshValue()->ShowWindow(SW_HIDE);
	}
	else
	{
		m_staticRefresh.ShowWindow(SW_SHOW);
		GetEditPurse2RefreshValue()->ShowWindow(SW_SHOW);
		
		CString strValue = "";
		strValue.Format("%.2f", GetRefreshValue());
		GetEditPurse2RefreshValue()->SetWindowText(strValue);
	}

	switch (m_SQLRowGroup.GetRefreshType())
	{
	case nREFRESHTYPE_EOD:
		m_editPurse2RefreshTime.ShowWindow(SW_HIDE);
		break;

	default:
		m_editPurse2RefreshTime.ShowWindow(SW_SHOW);
		break;
	}
}

//$$******************************************************************

double CEditRecordPropertyPageBalance::GetRefreshValue()
{
	double dRefreshValue = 0.0;
	CString strRefreshValueText = "";

	switch (m_SQLRowGroup.GetRefreshType())
	{
	case nREFRESHTYPE_DISABLED:
	case nREFRESHTYPE_MANUAL:
		break;

	case nREFRESHTYPE_PERIOD:
	{
		int nPeriodIndex = SQLRowSetAuditPeriod.GetPeriodIndex();							// get current period index ( 0-5 )

		if (m_SQLRowGroup.IsPurseAllowed(2, nPeriodIndex) == TRUE)				// see if Purse2 allowed in current period
		{
			dRefreshValue = m_SQLRowGroup.GetRefreshValueByPeriod(nPeriodIndex);	// get refresh value for current period
		}

		strRefreshValueText = SQLRowSetAuditPeriod.GetPeriodName(nPeriodIndex);					// display current period
	}
	break;

	default:
	{
		dRefreshValue = m_SQLRowGroup.GetRefreshValue();
		strRefreshValueText = m_SQLRowGroup.GetRefreshTypeText();				// display refresh type
	}
	break;
	}

	if (m_SQLRowGroup.GetRefreshAppendFlag() != 0)
	{
		strRefreshValueText += " (+)";										// show value is appended
	}

	m_staticRefresh.SetWindowText(strRefreshValueText);

	return dRefreshValue;
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::SetOneOffFields()
{		
	m_buttonPurse1Topup.ShowWindow(m_editEnables.m_bShowP1CashButton ? SW_SHOW : SW_HIDE);
	m_buttonPurse2Topup.ShowWindow(m_editEnables.m_bShowP2CashButton ? SW_SHOW : SW_HIDE);
	m_buttonPurse3Topup.ShowWindow(m_editEnables.m_bShowP3CashButton ? SW_SHOW : SW_HIDE);
	GetEditPurse1Liability()->SetReadOnly(FALSE == m_editEnables.m_bEditPurse1 );
	GetEditPurse1Credit()->SetReadOnly(FALSE == m_editEnables.m_bEditPurse1);
	GetEditPurse1Total()->SetReadOnly(TRUE);
	GetEditPurse2Balance()->SetReadOnly(FALSE == m_editEnables.m_bEditPurse2 );
	GetEditPurse3Liability()->SetReadOnly(FALSE == m_editEnables.m_bEditPurse3 );
	GetEditPurse3Credit()->SetReadOnly(FALSE == m_editEnables.m_bEditPurse3);
	GetEditPurse3Total()->SetReadOnly(TRUE);
	GetEditPurse1STD()->SetReadOnly(FALSE == ( m_editEnables.m_bEditSpendTD && m_editEnables.m_bEditPurse1));
	GetEditPurse2STD()->SetReadOnly(FALSE == (m_editEnables.m_bEditSpendTD && m_editEnables.m_bEditPurse2));
	GetEditPurse3STD()->SetReadOnly(FALSE == (m_editEnables.m_bEditSpendTD && m_editEnables.m_bEditPurse3));
	GetEditPointsBalance() -> SetReadOnly(FALSE == m_editEnables.m_bEditPoints);
	GetEditPointsToDate()->SetReadOnly(FALSE == m_editEnables.m_bEditPoints);
	m_buttonPointsRedeem.ShowWindow(m_editEnables.m_bShowPointsRedeemButton);
	m_buttonPointsTopup.ShowWindow(m_editEnables.m_bShowPointsTopupButton);
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::RemoveUnusedControls()
{
	if (FALSE == System.GetEnablePurse3Flag())
	{
		//REMOVE PURSE AREA AND USE POINTS AREA TO FILL SPACE
		ShowAndEnableWindow(&m_staticPurse3Label1, FALSE);
		ShowAndEnableWindow(&m_staticPurse3Label2, FALSE);
		ShowAndEnableWindow(&m_staticPurse3Label3, FALSE);
		ShowAndEnableWindow(&m_staticPurse3Label4, FALSE);
		ShowAndEnableWindow(&m_staticPurse3LabelLiability, FALSE);
		ShowAndEnableWindow(&m_staticPurse3LabelCredit, FALSE);
		ShowAndEnableWindow(&m_staticPurse3LabelTotal, FALSE);
		ShowAndEnableWindow(&m_staticPurse3Label8, FALSE);
		ShowAndEnableWindow(&m_staticPurse3Label9, FALSE);
		ShowAndEnableWindow(&m_staticPurse3Label10, FALSE);
		ShowAndEnableWindow(&m_staticPurse3MaxSpend, FALSE);
		ShowAndEnableWindow(&m_staticPurse3Overdraft, FALSE);
		ShowAndEnableWindow(GetEditPurse3Liability(), FALSE);
		ShowAndEnableWindow(GetEditPurse3Credit(), FALSE);
		ShowAndEnableWindow(GetEditPurse3Total(), FALSE);
		ShowAndEnableWindow(GetEditPurse3STD(), FALSE);
		ShowAndEnableWindow(GetEditPurse3MaxSpend(), FALSE);
		ShowAndEnableWindow(GetEditPurse3Overdraft(), FALSE);
		ShowAndEnableWindow(GetEditPurse3UsedDate(), FALSE);
		ShowAndEnableWindow(GetEditPurse3UsedValue(), FALSE);
		ShowAndEnableWindow(&m_editPurse3UsedTime, FALSE);
		ShowAndEnableWindow(&m_buttonPurse3Topup, FALSE);

		MoveControl(&m_staticPointsLabel1, 7, 195, 305, 184);
		MoveControl(&m_staticPointsLabel2, 23, 232);
		MoveControl(&m_staticPointsLabel3, 23, 252);
		MoveControl(&m_staticPointsLabel4, 23, 283);
		MoveControl(GetEditPointsBalance(), 100, 230);
		MoveControl(GetEditPointsValue(), 100, 250);
		MoveControl(GetEditPointsToDate(), 100, 281);
		MoveControl(&m_buttonPointsTopup, 23, 329);
		MoveControl(&m_buttonPointsRedeem, 97, 329);

		if (FALSE == System.GetActiveEnablePointsFlag())
		{
			ShowAndEnableWindow(&m_staticPointsLabel1, TRUE, FALSE );
			ShowAndEnableWindow(&m_staticPointsLabel2, TRUE, FALSE);
			ShowAndEnableWindow(&m_staticPointsLabel3, TRUE, FALSE);
			ShowAndEnableWindow(&m_staticPointsLabel4, TRUE, FALSE);
			ShowAndEnableWindow(GetEditPointsBalance(), TRUE, FALSE);
			ShowAndEnableWindow(GetEditPointsValue(), TRUE, FALSE);
			ShowAndEnableWindow(GetEditPointsToDate(), TRUE, FALSE);
			ShowAndEnableWindow(&m_buttonPointsTopup, TRUE, FALSE);
			ShowAndEnableWindow(&m_buttonPointsRedeem, TRUE, FALSE);
		}
	}
	else if (FALSE == System.GetActiveEnablePointsFlag())
	{
		//REMOVE POINTS AREA
		ShowAndEnableWindow(&m_staticPointsLabel1, FALSE);
		ShowAndEnableWindow(&m_staticPointsLabel2, FALSE);
		ShowAndEnableWindow(&m_staticPointsLabel3, FALSE);
		ShowAndEnableWindow(&m_staticPointsLabel4, FALSE);
		ShowAndEnableWindow(GetEditPointsBalance(), FALSE);
		ShowAndEnableWindow(GetEditPointsValue(), FALSE);
		ShowAndEnableWindow(GetEditPointsToDate(), FALSE);
		ShowAndEnableWindow(&m_buttonPointsRedeem, FALSE);
		ShowAndEnableWindow(&m_buttonPointsTopup, FALSE);
	}

	//EXPAND SIZE OF GENERAL AREA
	if ((FALSE == System.GetEnablePurse3Flag()) || (FALSE == System.GetActiveEnablePointsFlag()))
	{
		MoveControl(&m_staticGeneralLabel1, 318, 195, 305, 184);
		MoveControl(&m_staticGeneralLabel2, 334, 232);
		MoveControl(&m_staticGeneralLabel3, 334, 283);
		MoveControl(GetEditGeneralBalance(), 411, 230);
		MoveControl(GetEditGeneralCashSpend(), 411, 281);
	}

	if (FALSE == System.GetCreditOptionsFlag())
	{
		if (CompareDoubles(m_SQLRowAccount.GetPurse1Credit(), 0.0, 3) == 0)
		{
			ShowAndEnableWindow(&m_staticPurse1LabelLiability, FALSE);
			ShowAndEnableWindow(&m_staticPurse1LabelCredit, FALSE);
			ShowAndEnableWindow(GetEditPurse1Credit(), FALSE);
			ShowAndEnableWindow(&m_staticPurse1LabelTotal, FALSE);
			ShowAndEnableWindow(GetEditPurse1Total(), FALSE);
			MoveControl(&m_buttonPurse1Topup, 220, 41);
		}

		if (CompareDoubles(m_SQLRowAccount.GetPurse3Credit(), 0.0, 3) == 0)
		{
			ShowAndEnableWindow(&m_staticPurse3LabelLiability, FALSE);
			ShowAndEnableWindow(&m_staticPurse3LabelCredit, FALSE);
			ShowAndEnableWindow(GetEditPurse3Credit(), FALSE);
			ShowAndEnableWindow(&m_staticPurse3LabelTotal, FALSE);
			ShowAndEnableWindow(GetEditPurse3Total(), FALSE);
			MoveControl(&m_buttonPurse3Topup, 220, 230);
		}
	}
}

//$$******************************************************************
CEdit* CEditRecordPropertyPageBalance::GetEditPurse1Liability() { return GetEdit(IDC_EDIT_PURSE1_LIABILITY); }
CEdit* CEditRecordPropertyPageBalance::GetEditPurse1Credit() { return GetEdit(IDC_EDIT_PURSE1_CREDIT); }
CEdit* CEditRecordPropertyPageBalance::GetEditPurse1Total() { return GetEdit(IDC_EDIT_PURSE1_TOTAL); }
CEdit* CEditRecordPropertyPageBalance::GetEditPurse1Overdraft() { return GetEdit(IDC_EDIT_PURSE1_OVERDRAFT); }
CEdit* CEditRecordPropertyPageBalance::GetEditPurse1MaxSpend() { return GetEdit(IDC_EDIT_PURSE1_MAXSPEND); }
CEdit* CEditRecordPropertyPageBalance::GetEditPurse1STD() { return GetEdit(IDC_EDIT_PURSE1_STD); }
CEdit* CEditRecordPropertyPageBalance::GetEditPurse1UsedValue() { return GetEdit(IDC_EDIT_PURSE1_USED_VALUE); }
CEdit* CEditRecordPropertyPageBalance::GetEditPurse1UsedDate() { return GetEdit(IDC_EDIT_PURSE1_USED_DATE); }
//$$******************************************************************
CEdit* CEditRecordPropertyPageBalance::GetEditPurse2Balance() { return GetEdit(IDC_EDIT_PURSE2_BALANCE); }
CEdit* CEditRecordPropertyPageBalance::GetEditPurse2STD() { return GetEdit(IDC_EDIT_PURSE2_STD); }
CEdit* CEditRecordPropertyPageBalance::GetEditPurse2UsedValue() { return GetEdit(IDC_EDIT_PURSE2_USED_VALUE); }
CEdit* CEditRecordPropertyPageBalance::GetEditPurse2UsedDate() { return GetEdit(IDC_EDIT_PURSE2_USED_DATE); }
CEdit* CEditRecordPropertyPageBalance::GetEditPurse2RefreshValue() { return GetEdit(IDC_EDIT_PURSE2_REFRESH_VALUE); }
CEdit* CEditRecordPropertyPageBalance::GetEditPurse2RefreshDate() { return GetEdit(IDC_EDIT_PURSE2_REFRESH_DATE); }
//$$******************************************************************
CEdit* CEditRecordPropertyPageBalance::GetEditPurse3Liability() { return GetEdit(IDC_EDIT_PURSE3_LIABILITY); }
CEdit* CEditRecordPropertyPageBalance::GetEditPurse3Credit() { return GetEdit(IDC_EDIT_PURSE3_CREDIT); }
CEdit* CEditRecordPropertyPageBalance::GetEditPurse3Total() { return GetEdit(IDC_EDIT_PURSE3_TOTAL); }
CEdit* CEditRecordPropertyPageBalance::GetEditPurse3Overdraft() { return GetEdit(IDC_EDIT_PURSE3_OVERDRAFT); }
CEdit* CEditRecordPropertyPageBalance::GetEditPurse3MaxSpend() { return GetEdit(IDC_EDIT_PURSE3_MAXSPEND); }
CEdit* CEditRecordPropertyPageBalance::GetEditPurse3STD() { return GetEdit(IDC_EDIT_PURSE3_STD); }
CEdit* CEditRecordPropertyPageBalance::GetEditPurse3UsedValue() { return GetEdit(IDC_EDIT_PURSE3_USED_VALUE); }
CEdit* CEditRecordPropertyPageBalance::GetEditPurse3UsedDate() { return GetEdit(IDC_EDIT_PURSE3_USED_DATE); }
//$$******************************************************************
CEdit* CEditRecordPropertyPageBalance::GetEditPointsBalance() { return GetEdit(IDC_EDIT_POINTS_BALANCE); }
CEdit* CEditRecordPropertyPageBalance::GetEditPointsValue() { return GetEdit(IDC_EDIT_POINTS_VALUE); }
CEdit* CEditRecordPropertyPageBalance::GetEditPointsToDate() { return GetEdit(IDC_EDIT_POINTS_TODATE); }
//$$******************************************************************
CEdit* CEditRecordPropertyPageBalance::GetEditGeneralBalance() { return GetEdit(IDC_EDIT_GENERAL_BALANCE); }
CEdit* CEditRecordPropertyPageBalance::GetEditGeneralCashSpend() { return GetEdit(IDC_EDIT_GENERAL_CASHSPEND); }
//$$******************************************************************

void CEditRecordPropertyPageBalance::SetEditBoxDouble(CEdit* pEdit, double d)
{
	CString str = "";
	str.Format("%.2f", d);
	pEdit->SetWindowText(str);
}

//$$******************************************************************

double CEditRecordPropertyPageBalance::GetEditBoxDouble(CEdit* pEdit )
{
	return atof(GetEditBoxText(*pEdit));
}

//$$******************************************************************

double CEditRecordPropertyPageBalance::AddToEditBoxDouble(CEdit* pEdit, double dValue)
{
	double dNewValue = GetEditBoxDouble(pEdit) + dValue;
	SetEditBoxDouble(pEdit, dNewValue);
	return dNewValue;
}

//$$******************************************************************

int CEditRecordPropertyPageBalance::AddToEditBoxInt(CEdit* pEdit, int nValue)
{
	int nNewValue = GetEditBoxInt(*pEdit) + nValue;
	SetEditBoxInt(*pEdit, nNewValue);
	return nNewValue;
}

//$$******************************************************************

bool CEditRecordPropertyPageBalance::IsHotlisted()
{
	bool bResult = FALSE;

	if (m_SQLRowAccount.IsAlertSet() == TRUE)
	{
		if (System.GetInterfaceType() == nINTERFACE_MIFAREv1)
		{
			bResult = TRUE;
		}
	}

	return bResult;
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::OnButtonAltKey1()
{
	if ( System.GetEnableNoAuditFlag() == TRUE)
	{
		if ( m_pLeaversData == NULL )
		{
			m_bAuditTurnedOff = TRUE;
			m_staticAuditOff.ShowWindow(SW_SHOW);
			m_staticAuditOff.SetWindowText ( "** AUDIT IS TURNED OFF **" );
		}
	}
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::OnDoubleClickStaticBalance()
{
	if ( m_arrayAuditBuffer.GetSize() != 0 )
	{
		CAuditReportWrapper Wrapper( "", "R123", m_SQLRowAccount.GetUserID(), this);
		Wrapper.SetAuditArray(&m_arrayAuditBuffer);
		Wrapper.ShowEditSessionAudit();
	}
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::OnButtonPurse1Topup()
{
	if (TRUE == m_bDonePebbleAdjust)
	{
		Prompter.Error("You cannot add a Pebble Adjustment and a top-up in the same session.");
	}
	else
	{
		HandlePurse1TopupButton(FALSE);
	}
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::OnButtonPebble()
{
	if ((Passwords.GetUsingMagicPasswordFlag() == TRUE) &&
		(Server.GetAllowPebbleAdjustFlag() == TRUE) &&
		(System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE))
	{
		if (TRUE == m_bDonePurse1Topup)
		{
			Prompter.Error("You cannot add a Pebble Adjustment and a top-up in the same session.");
			return;
		}
		else if (m_bDonePebbleAdjust)
		{
			CString str = "";
			str.Format("Pending Pebble Adjustment:\n\n");
			
			if (m_dPendingPebbleAdjust < 0.0)
			{
				str += "minus ";
				str.AppendFormat("%.2f", m_dPendingPebbleAdjust * (-1));
			}
			else
			{
				str.AppendFormat("%.2f", m_dPendingPebbleAdjust);
			}

			Prompter.Info(str);
		}
		else
		{
			HandlePurse1TopupButton(TRUE);
		}
	}
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::HandlePurse1TopupButton(bool bPebbleAdjust)
{
	double dOldBalance = GetEditBoxDouble(GetEditPurse1Liability());

	CAddCashDlg dlg(1, this);
	dlg.m_dPurseBalance = dOldBalance;

	if (TRUE == bPebbleAdjust)
	{
		dlg.SetPebbleAdjustMode();
		dlg.SetPendingPebbleAdjust(m_dPendingPebbleAdjust);
	}
	
	if (dlg.DoModal() == TRUE)
	{
		double dValue = dlg.m_dValue;
		if (dValue != 0.0)
		{
			if (FALSE == bPebbleAdjust)
			{
				SetEditBoxDouble(GetEditPurse1Liability(), dOldBalance + dValue);
			}

			m_atc1.SetTransactionPoints(0);					// no points added
			m_atc1.SetApplicationNo(APPNO_ADDCASH);
			m_atc1.SetLineComment(dlg.m_strComment);

			m_atc1.SetCurrentPurse1Liability(dOldBalance + dValue);				// new balance purse1

			m_atc1.SetTransactionPurse1Liability(dValue);
			m_atc1.SetTransactionPurse1Credit(0.0);
			m_atc1.SetTransactionPurse2Balance(0.0);
			m_atc1.SetTransactionPurse3Liability(0.0);
			m_atc1.SetTransactionPurse3Credit(0.0);

			m_atc2.SetCurrentPurse1Liability(m_atc2.GetCurrentPurse1Liability() + dValue);	// update second audit record

			m_atc1.SaveLineToArray(m_arrayAuditBuffer);				// save in temp file

			if (FALSE == bPebbleAdjust)
			{
				SetAccountBalance();
				m_bDonePurse1Topup = TRUE;			
			}
			else
			{
				m_dPendingPebbleAdjust += dValue;
				m_bDonePebbleAdjust = TRUE;		
			}
		}
	}
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::OnButtonPurse2Topup()
{
	double dOldBalance = GetEditBoxDouble(GetEditPurse2Balance());

	CAddCashDlg dlg(2, this);
	dlg.m_dPurseBalance = dOldBalance;

	if (dlg.DoModal() == TRUE)
	{
		double dValue = dlg.m_dValue;
		if (dValue != 0.0)
		{
			SetEditBoxDouble(GetEditPurse2Balance(), dOldBalance + dValue);

			m_atc1.SetTransactionPoints(0);					// no points added
			m_atc1.SetApplicationNo(APPNO_ADDCASH);
			m_atc1.SetLineComment(dlg.m_strComment);

			m_atc1.SetCurrentPurse2Balance(dOldBalance + dValue);

			m_atc1.SetTransactionPurse1Liability(0.00);
			m_atc1.SetTransactionPurse1Credit(0.0);
			m_atc1.SetTransactionPurse2Balance(dValue);			// new purse2 
			m_atc1.SetTransactionPurse3Liability(0.0);
			m_atc1.SetTransactionPurse3Credit(0.0);

			m_atc2.SetCurrentPurse2Balance(m_atc2.GetCurrentPurse2Balance() + dValue);	// update second audit record
			
			m_atc1.SaveLineToArray(m_arrayAuditBuffer);				// save in temp file
			SetAccountBalance();									// show new balance does UdateData(FALSE)
		}
	}
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::OnButtonPurse3Topup()
{
	double dOldBalance = GetEditBoxDouble(GetEditPurse3Liability());

	CAddCashDlg dlg(3, this);
	dlg.m_dPurseBalance = dOldBalance;

	if (dlg.DoModal() == TRUE)
	{
		double dValue = dlg.m_dValue;
		if (dValue != 0.0)
		{
			SetEditBoxDouble(GetEditPurse3Liability(), dOldBalance + dValue);

			m_atc1.SetCurrentPurse1Liability(dOldBalance + dValue);				// new balance purse1

			m_atc1.SetTransactionPurse1Liability(0.0);
			m_atc1.SetTransactionPurse1Credit(0.0);
			m_atc1.SetTransactionPurse2Balance(0.0);
			m_atc1.SetTransactionPurse3Liability(dValue);
			m_atc1.SetTransactionPurse3Credit(0.0);
			
			m_atc1.SetCurrentPurse3Liability(m_atc2.GetCurrentPurse3Liability() + dValue);	// update second audit record
			
			m_atc1.SaveLineToArray(m_arrayAuditBuffer);				// save in temp file
			SetAccountBalance();									// show new balance does UdateData(FALSE)
		}
	}
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::OnButtonPointsTopup()
{
	int nOldPointsBalance = GetEditBoxInt(*GetEditPointsBalance());
	int nOldPointsToDate = GetEditBoxInt(*GetEditPointsToDate());

	CAddPointsDlg dlg{};
	dlg.m_nPointsBalance = nOldPointsBalance;
	if (dlg.DoModal() == TRUE)
	{
		int nPointsToAdd = dlg.m_nValue;
		
		if (nPointsToAdd != 0)
		{
			SetEditBoxInt(*GetEditPointsBalance(), nOldPointsBalance + nPointsToAdd);
			SetEditBoxInt(*GetEditPointsToDate(), nOldPointsToDate + nPointsToAdd);
			
			m_atc1.SetToDatePoints(nOldPointsToDate + nPointsToAdd);
			m_atc1.SetCurrentPoints(nOldPointsBalance + nPointsToAdd);
			m_atc1.SetTransactionPoints(nPointsToAdd);
			m_atc1.SetTransactionPurse1Liability(0.0);								// no purse transaction
			m_atc1.SetTransactionPurse1Credit(0.0);
			m_atc1.SetTransactionPurse3Liability(0.0);
			m_atc1.SetTransactionPurse3Credit(0.0);
			m_atc1.SetTransactionCash(0.0);
			m_atc1.SetApplicationNo(APPNO_ADDPOINTS);
			m_atc1.SetLineComment(dlg.m_strComment);
			m_atc1.SaveLineToArray(m_arrayAuditBuffer);			// save in temp audit file

			m_atc2.SetToDatePoints(m_atc2.GetToDatePoints() + nPointsToAdd);
			m_atc2.SetCurrentPoints(m_atc2.GetCurrentPoints() + nPointsToAdd);				// update secondary audit record

			SetPointsValue();									//recaluclate point sd=redemption value
		}
	}
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::OnButtonPointsRedeem()
{
	int nMaxRedeemPoints = SetPointsValue();				// max point sthat an ve redeemed

	CRedeemPointsDlg dlg(this);

	{
		int nLastError = m_pointsRedeemer.GetLastError();
		if (nLastError != 0)
		{
			dlg.DisplayNoShowReason(nLastError);
			return;
		}
	}

	dlg.m_strComment = Server.GetRedeemComment();
	dlg.m_nRedeemPoints = nMaxRedeemPoints;
	dlg.m_dRedeemValue = GetEditBoxDouble(GetEditPointsValue());
	dlg.m_nTripFactor = m_pointsRedeemer.m_nTrip;
	dlg.m_dTripValue = m_pointsRedeemer.m_dValue;
	dlg.m_nRedeemPurse = m_pointsRedeemer.m_nPurse;

	if (dlg.DoModal() == IDOK)
	{
		double dValueAdded = dlg.m_dRedeemValue;
		int nPointsRedeemed = dlg.m_nRedeemPoints;

		if (nPointsRedeemed > 0)
		{
			int nOldPointsBalance = GetEditBoxInt(*GetEditPointsBalance());
			int nNewPointsBalance = nOldPointsBalance - nPointsRedeemed;
			SetEditBoxInt(*GetEditPointsBalance(), nNewPointsBalance);

			m_atc1.SetCurrentPoints(nNewPointsBalance);
			m_atc1.SetTransactionPoints(-nPointsRedeemed);

			m_atc2.SetCurrentPoints(m_atc2.GetCurrentPoints() - nPointsRedeemed);	// update second audit record

			if (m_pointsRedeemer.m_nPurse == 3)
			{
				double dOldLiability = GetEditBoxDouble(GetEditPurse3Liability());
				double dNewLiability = dOldLiability + dValueAdded;

				SetEditBoxDouble(GetEditPurse3Liability(), dNewLiability);

				m_atc1.SetCurrentPurse3Liability(dNewLiability);				// new balance purse1
				m_atc1.SetTransactionPurse1Liability(0.0);						// add to purse3
				m_atc1.SetTransactionPurse2Balance(0.0);
				m_atc1.SetTransactionPurse3Liability(dValueAdded);
				m_atc1.SetTransactionPurse1Credit(0.0);
				m_atc1.SetTransactionPurse3Credit(0.0);

				m_atc2.SetCurrentPurse3Liability(m_atc2.GetCurrentPurse3Liability() + dValueAdded);	// update second audit record
			}
			else if (m_pointsRedeemer.m_nPurse == 2)
			{
				double dOldBalance = GetEditBoxDouble(GetEditPurse2Balance());
				double dNewBalance = dOldBalance + dValueAdded;

				SetEditBoxDouble(GetEditPurse2Balance(), dNewBalance);

				m_atc1.SetCurrentPurse2Balance(dNewBalance);						// new balance purse1
				m_atc1.SetTransactionPurse1Liability(0.0);						// add to purse2
				m_atc1.SetTransactionPurse2Balance(dValueAdded);
				m_atc1.SetTransactionPurse3Liability(0.0);
				m_atc1.SetTransactionPurse1Credit(0.0);
				m_atc1.SetTransactionPurse3Credit(0.0);

				m_atc2.SetCurrentPurse2Balance(m_atc2.GetCurrentPurse2Balance() + dValueAdded);	// update second audit record
			}
			else
			{
				double dOldLiability = GetEditBoxDouble(GetEditPurse1Liability());
				double dNewLiability = dOldLiability + dValueAdded;

				SetEditBoxDouble(GetEditPurse1Liability(), dNewLiability);

				m_atc1.SetCurrentPurse1Liability(dNewLiability);						// new balance purse1
				m_atc1.SetTransactionPurse1Liability(dValueAdded);
				m_atc1.SetTransactionPurse2Balance(0.0);
				m_atc1.SetTransactionPurse3Liability(0.0);
				m_atc1.SetTransactionPurse1Credit(0.0);
				m_atc1.SetTransactionPurse3Credit(0.0);

				m_atc2.SetCurrentPurse1Liability(m_atc2.GetCurrentPurse1Liability() + dValueAdded);	// update second audit record
			}

			m_atc1.SetSourceType(AUDIT_PC);
			m_atc1.SetApplicationNo(APPNO_REDEEM);			// points redemption
			m_atc1.SetLineComment(dlg.m_strComment);
			m_atc1.SaveLineToArray(m_arrayAuditBuffer);		// save in temp audit buffer

			SetPointsValue();								// calculate points redemption value
			SetAccountBalance();							// redo current balance
		}
	}
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::OnKillFocusPurse()
{
	SetAccountBalance();
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::OnKillFocusPoints()
{
	SetPointsValue();
}

//$$******************************************************************

int CEditRecordPropertyPageBalance::SetPointsValue()
{
	double dValue = 0.0;
	int nPoints = GetEditBoxInt(*GetEditPointsBalance());
	int nMaxRedeem = m_pointsRedeemer.Redeem(nPoints, dValue);
	SetEditBoxDouble(GetEditPointsValue(), dValue);
	return nMaxRedeem;
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::SetAccountBalance()
{
	// assume no Purse2
	double dPurse2 = 0;											
	
	// if manual purse2 - always show balance ( even if not allowed in cuurent period )
	if (m_SQLRowGroup.GetRefreshType() == nREFRESHTYPE_MANUAL)
	{
		dPurse2 = GetEditBoxDouble(GetEditPurse2Balance());
	}
	else
	{
		// get purse balance for currrent period - but don't save if refresh occurs
		CSQLPurse2Handler purse2(m_SQLRowAccount, m_SQLRowGroup);
		dPurse2 = purse2.GetBalance(FALSE);							
		
		// don't add refresh for hotlisted card
		if ( ( purse2.Purse2Allowed() == FALSE ) || ( IsHotlisted() == TRUE ) )
		{
			dPurse2 = 0;												
		}
	}

	SetEditBoxDouble(GetEditPurse1Total(), GetEditBoxDouble(GetEditPurse1Liability()) + GetEditBoxDouble(GetEditPurse1Credit()));
	SetEditBoxDouble(GetEditPurse3Total(), GetEditBoxDouble(GetEditPurse3Liability()) + GetEditBoxDouble(GetEditPurse3Credit()));
	SetEditBoxDouble(GetEditGeneralBalance(), GetEditBoxDouble(GetEditPurse1Total()) + GetEditBoxDouble(GetEditPurse3Total()) + dPurse2);
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::SetTabStops()
{
	CArray<CEdit*, CEdit*> arrayEditBoxes;
	arrayEditBoxes.Add(GetEditPurse1Liability());
	arrayEditBoxes.Add(GetEditPurse1Credit());
	arrayEditBoxes.Add(GetEditPurse1STD());
	arrayEditBoxes.Add(GetEditPurse1Overdraft());
	arrayEditBoxes.Add(GetEditPurse1MaxSpend());
	arrayEditBoxes.Add(GetEditPurse2Balance());
	arrayEditBoxes.Add(GetEditPurse2STD());
	arrayEditBoxes.Add(GetEditPurse3Liability());
	arrayEditBoxes.Add(GetEditPurse3Credit());
	arrayEditBoxes.Add(GetEditPurse3STD());
	arrayEditBoxes.Add(GetEditPurse3Overdraft());
	arrayEditBoxes.Add(GetEditPurse3MaxSpend());
	arrayEditBoxes.Add(GetEditPointsBalance());
	arrayEditBoxes.Add(GetEditPointsToDate());
	arrayEditBoxes.Add(GetEditGeneralCashSpend());

	CEdit* pFirstEnabled = NULL;

	for (int n = 0; n < arrayEditBoxes.GetSize(); n++)
	{
		CEdit* pEdit = arrayEditBoxes.GetAt(n);

		if ((pEdit->GetStyle() & ES_READONLY) == 0)
		{
			pEdit->ModifyStyle(0, WS_TABSTOP);
		}
		else
		{
			pEdit->ModifyStyle(WS_TABSTOP, 0);

			if (NULL == pFirstEnabled)
			{
				pFirstEnabled = pEdit;
			}
		}
	}

	if (pFirstEnabled != NULL)
	{
		pFirstEnabled->SetFocus();
	}
}

//$$******************************************************************

BOOL CEditRecordPropertyPageBalance::OnKillActive()
{
	return CSSPropertyPage::OnKillActive();
}

//$$******************************************************************

BOOL CEditRecordPropertyPageBalance::OnQueryCancel()
{
	return CEditRecordPropertySheet::PromptBeforeCancel();
}

//********************************************************************

void CEditRecordPropertyPageBalance::OnCancel()
{	
	CSSPropertyPage::OnCancel();
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::OnOK()
{
	// check for any manual field changes
	m_SQLRowAccount.SetPurse1Liability(GetEditBoxDouble(GetEditPurse1Liability()));
	m_SQLRowAccount.SetPurse1Credit(GetEditBoxDouble(GetEditPurse1Credit()));
	m_SQLRowAccount.SetPurse1SpendToDate(GetEditBoxDouble(GetEditPurse1STD()));

	if (TRUE == m_editEnables.m_bSystemEditMaxSpend && m_editEnables.m_bAccountEditMaxSpend )
	{
		m_SQLRowAccount.SetMaxSpendPurse1Period1(GetEditBoxDouble(GetEditPurse1MaxSpend()));
	}
	if ((TRUE == m_editEnables.m_bSystemEditMaxSpend && m_editEnables.m_bAccountEditMaxSpend ) && (TRUE == Server.GetAllowOverdraftFlag()))
	{
		m_SQLRowAccount.SetMaxOverdraftPurse1Period1(GetEditBoxDouble(GetEditPurse1Overdraft()));
	}

	m_SQLRowAccount.SetPurse2Balance(GetEditBoxDouble(GetEditPurse2Balance()));
	m_SQLRowAccount.SetPurse2SpendToDate(GetEditBoxDouble(GetEditPurse2STD()));

	if (TRUE == System.GetEnablePurse3Flag())
	{
		m_SQLRowAccount.SetPurse3Liability(GetEditBoxDouble(GetEditPurse3Liability()));
		m_SQLRowAccount.SetPurse3Credit(GetEditBoxDouble(GetEditPurse3Credit()));
		m_SQLRowAccount.SetPurse3SpendToDate(GetEditBoxDouble(GetEditPurse3STD()));

		if (TRUE == m_editEnables.m_bSystemEditMaxSpend && m_editEnables.m_bAccountEditMaxSpend )
		{
			m_SQLRowAccount.SetMaxSpendPurse3Period1(GetEditBoxDouble(GetEditPurse3MaxSpend()));
		}
		if ((TRUE == TRUE == m_editEnables.m_bSystemEditMaxSpend && m_editEnables.m_bAccountEditMaxSpend) && (TRUE == Server.GetAllowOverdraftFlag()))
		{
			m_SQLRowAccount.SetMaxOverdraftPurse3Period1(GetEditBoxDouble(GetEditPurse3Overdraft()));
		}
	}

	if (TRUE == System.GetActiveEnablePointsFlag())
	{
		m_SQLRowAccount.SetPoints(GetEditBoxInt(*GetEditPointsBalance()));
		m_SQLRowAccount.SetPointsToDate(GetEditBoxInt(*GetEditPointsToDate()));
	}

	m_SQLRowAccount.SetCashSpendToDate(GetEditBoxDouble(GetEditGeneralCashSpend()));
	
	if (m_bAuditTurnedOff == TRUE)
	{
		Prompter.Error("No audit lines have been recorded for this account!", "Edit Account");
	}
	
	CSSPropertyPage::OnOK();
}

//$$******************************************************************

void CEditRecordPropertyPageBalance::UpdateAudit(bool bCancel)
{
	// check for any session audit lines
	CString strMainAuditFile = "";

	if (m_pLeaversData == NULL)
	{
		if (m_bAuditTurnedOff == TRUE)
		{
			strMainAuditFile = Filenames.GetHiddenAuditFilename();	//  hidden file to hold turned off audit lines
		}
		else
		{
			strMainAuditFile = Filenames.GetAuditFilename();		// main audit file
		}
	}
	else
	{
		strMainAuditFile = m_pLeaversData->m_strAuditFilename;		// leavers audit file
	}

	bool bAuditUpdated = FALSE;
	if (m_arrayAuditBuffer.GetSize() > 0)
	{
		CSmartPayAuditFile auditFile(strMainAuditFile);			// main audit file
		auditFile.AppendArray(m_arrayAuditBuffer, bCancel);
		m_arrayAuditBuffer.RemoveAll();
		bAuditUpdated = TRUE;
	}

	if (m_atc2.AuditChanges(&m_SQLRowAccount) == TRUE)				// see if any fields edited manually
	{
		m_atc2.SetApplicationNo(APPNO_EDIT);
		m_atc2.SaveNewLine(strMainAuditFile,bCancel);
		bAuditUpdated = TRUE;
	}
}

//$$******************************************************************

