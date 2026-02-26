//$$******************************************************************
#include "..\SPosLoyalty4Shared\LoyaltyAuditAppend.h"
#include "..\SPosLoyalty4Shared\LoyaltySubs.h"
#include "..\SPosLoyalty4Shared\SQLDefines.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SPosLoyalty4Shared\SystemData.h"
//$$******************************************************************
#include "AddCashDlg.h"
#include "AddLoyaltyDlg.h"
#include "AddPointsDlg.h"
#include "CalculateBonus.h"
#include "CalculateSpendBonus.h"
#include "EditAccountPropertySheet.h"
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "PasswordData.h"
#include "RedeemPointsDlg.h"
//$$******************************************************************
#include "EditAccountPropertyPage2.h"
//$$******************************************************************

CEditAccountPropertyPage2::CEditAccountPropertyPage2(CSQLRowAccountFull& RowAccount)
	: CSSPropertyPage(CEditAccountPropertyPage2::IDD), m_RowAccount(RowAccount)
{
	m_nLastGroupNo = -1;
	m_bGroupExists = FALSE;
	m_pEditAccountBuffer = NULL;
	m_bCanTopupPurse2 = FALSE;

	// Primary audit is for a pc line
	m_atc1.SetSQL(m_RowAccount);
	m_atc1.SetOperatorNo(Passwords.GetPasswordInUse());

	// second audit record to catch manual changes
	m_atc2.SetSourceType(AUDIT_PC);
	m_atc2.SetSQL(m_RowAccount);
	m_atc2.SetOperatorNo(Passwords.GetPasswordInUse());
}

//$$******************************************************************

CEditAccountPropertyPage2::~CEditAccountPropertyPage2()
{
}

//$$******************************************************************

void CEditAccountPropertyPage2::SetPointer ( CEditAccountBuffer* pEditAccountBuffer )
{
	m_pEditAccountBuffer = pEditAccountBuffer;
}

//$$******************************************************************

void CEditAccountPropertyPage2::DoDataExchange(CDataExchange* pDX)
{
	CSSPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_PURSE1_TOPUP, m_buttonPurse1Topup);
	DDX_Control(pDX, IDC_BUTTON_PURSE1_SPEND, m_buttonPurse1Spend);
	DDX_Control(pDX, IDC_STATIC_REFRESH, m_staticRefresh);
	DDX_Control(pDX, IDC_EDIT_PURSE1_USED_TIME, m_editPurse1UsedTime);
	DDX_Control(pDX, IDC_EDIT_PURSE2_USED_TIME, m_editPurse2UsedTime);
	DDX_Control(pDX, IDC_EDIT_CASH_USED_TIME, m_editCashUsedTime);
	DDX_Control(pDX, IDC_EDIT_PURSE2_REFRESH_TIME, m_editPurse2RefreshTime);
	DDX_Control(pDX, IDC_BUTTON_POINTS_TOPUP, m_buttonPointsTopup);
	DDX_Control(pDX, IDC_BUTTON_POINTS_REDEEM, m_buttonPointsRedeem);
	DDX_Control(pDX, IDC_BUTTON_MANUAL_BONUS, m_buttonManualBonus);
	DDX_Control(pDX, IDC_BUTTON_REFRESHNOW, m_buttonRefreshNow);
	DDX_Control(pDX, IDC_STATIC_BALANCE, m_staticBalance);

	if (TRUE == pDX->m_bSaveAndValidate)
	{
		if (ValidateMaxSpend(
			GetEditGeneralMaxSpend(),
			Account::MaxSpend.Min,
			Account::MaxSpend.Max,
			"Max Spend") == FALSE)
		{
			pDX->Fail();
			return;
		}

		if (ValidateMaxSpend(
			GetEditGeneralOverdraft(),
			Account::MaxOverdraft.Min,
			Account::MaxOverdraft.Max,
			"Overdraft") == FALSE)
		{
			pDX->Fail();
			return;
		}
	}
}

//$$******************************************************************

bool CEditAccountPropertyPage2::ValidateMaxSpend(CEdit* pEdit, double dMin, double dMax, CString strType)
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

//$$******************************************************************

BEGIN_MESSAGE_MAP(CEditAccountPropertyPage2, CSSPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_PURSE1_TOPUP, OnButtonPurse1Topup)
	ON_BN_CLICKED(IDC_BUTTON_PURSE1_SPEND, OnButtonPurse1Spend)
	ON_EN_KILLFOCUS(IDC_EDIT_PURSE1_BALANCE, OnKillFocusPurse)
	ON_EN_KILLFOCUS(IDC_EDIT_PURSE2_BALANCE, OnKillFocusPurse)
	ON_EN_KILLFOCUS(IDC_EDIT_POINTS_BALANCE, OnKillFocusPoints)
	ON_BN_CLICKED(IDC_BUTTON_POINTS_TOPUP, OnButtonPointsTopup)
	ON_BN_CLICKED(IDC_BUTTON_POINTS_REDEEM, OnButtonPointsRedeem)
	ON_BN_CLICKED(IDC_BUTTON_MANUAL_BONUS, OnButtonManualBonus)
	ON_BN_CLICKED(IDC_BUTTON_REFRESHNOW, OnButtonRefreshNow)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CEditAccountPropertyPage2::OnInitDialog()
{
	CSSPropertyPage::OnInitDialog();

	m_nLastGroupNo = m_pEditAccountBuffer->m_nGroupNo;
	m_RowGroup.SetGroupNo(m_nLastGroupNo);
	CSQLRepositoryGroup repoGroup;
	m_bGroupExists = (repoGroup.SelectRow(m_RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE);

	SubclassEdit(IDC_EDIT_PURSE1_BALANCE, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE1_STD, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE1_USED_DATE, SS_DATE, 10, "");
	SubclassEdit(IDC_EDIT_PURSE1_USED_VALUE, SS_NUM_SDP, 11, "%.2f");
	
	SubclassEdit(IDC_EDIT_PURSE2_BALANCE, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE2_STD, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE2_USED_DATE, SS_DATE, 10, "");
	SubclassEdit(IDC_EDIT_PURSE2_USED_VALUE, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE2_REFRESH_DATE, SS_DATE, 10, "");
	SubclassEdit(IDC_EDIT_PURSE2_REFRESH_VALUE, SS_NUM_SDP, 11, "%.2f");
	
	SubclassEdit(IDC_EDIT_POINTS_BALANCE, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_POINTS_TODATE, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_POINTS_VALUE, SS_NUM_SDP, 11, "%.2f");

	SubclassEdit(IDC_EDIT_GENERAL_BALANCE, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_GENERAL_OVERDRAFT, SS_NUM_SDP, 8, "%.2f");
	SubclassEdit(IDC_EDIT_GENERAL_MAXSPEND, SS_NUM_SDP, 8, "%.2f");
	SubclassEdit(IDC_EDIT_GENERAL_CASHSPEND, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_CASH_USED_DATE, SS_DATE, 10, "");
	SubclassEdit(IDC_EDIT_CASH_USED_VALUE, SS_NUM_SDP, 11, "%.2f");

	//COPY CURRENT VALUES INTO PURSE1 FIELDS
	SetEditBoxDouble(GetEditPurse1Balance(), m_RowAccount.GetPurse1Balance());
	SetEditBoxDouble(GetEditPurse1STD(), m_RowAccount.GetPurse1SpendToDate());
	
	{
		CString strDateLastUsed = m_RowAccount.GetPurse1LastSpendDate();

		if (strDateLastUsed.GetLength() > 0)
		{
			GetEditPurse1UsedDate()->SetWindowText(strDateLastUsed);
			m_editPurse1UsedTime.SetWindowText(m_RowAccount.GetPurse1LastSpendTime());
			SetEditBoxDouble(GetEditPurse1UsedValue(), m_RowAccount.GetPurse1LastSpend());
		}
		else
		{
			GetEditPurse1UsedValue()->SetWindowText("");
		}
	}

	//COPY CURRENT VALUES INTO PURSE2 FIELDS
	SetEditBoxDouble(GetEditPurse2Balance(), m_RowAccount.GetPurse2Balance());
	SetEditBoxDouble(GetEditPurse2STD(), m_RowAccount.GetPurse2SpendToDate());
	
	{
		CString strDateLastUsed = m_RowAccount.GetPurse2LastSpendDate();

		if (strDateLastUsed.GetLength() > 0)
		{
			GetEditPurse2UsedDate()->SetWindowText(strDateLastUsed);
			m_editPurse2UsedTime.SetWindowText(m_RowAccount.GetPurse2LastSpendTime());
			SetEditBoxDouble(GetEditPurse2UsedValue(), m_RowAccount.GetPurse2LastSpend());
		}
		else
		{
			GetEditPurse2UsedValue()->SetWindowText("");
		}
	}

	{
		CString strDateRefresh = m_RowAccount.GetPurse2RefreshedDate();

		if (strDateRefresh.GetLength() > 0)
		{
			GetEditPurse2RefreshDate()->SetWindowText(strDateRefresh);
			m_editPurse2RefreshTime.SetWindowText(m_RowAccount.GetPurse2RefreshedTime());
		}
	}

	//ENABLE PURSE FIELDS
	{
		bool bShowTopupButton = TRUE;
		bShowTopupButton &= (IsHotlisted() == FALSE);
		bShowTopupButton &= Passwords.IsPurse1TopupButtonAllowed();
		bShowTopupButton &= System.GetShowButtonPurse1TopupFlag();
		ShowAndEnableWindow(&m_buttonPurse1Topup, bShowTopupButton);

		bool bShowSpendButton = TRUE;
		bShowSpendButton &= (IsHotlisted() == FALSE);
		bShowSpendButton &= Passwords.IsPurse1SpendButtonAllowed();
		bShowSpendButton &= System.GetShowButtonPurse1SpendFlag();
		ShowAndEnableWindow(&m_buttonPurse1Spend, bShowSpendButton);
	
		if ((TRUE == bShowSpendButton) && (FALSE == bShowTopupButton))
		{
			MoveControl(&m_buttonPurse1Spend, 203, 28);
		}

		bool bEditPurse = TRUE;
		bEditPurse &= Passwords.IsEditPurseAllowed();
		bEditPurse &= (IsHotlisted() == FALSE);

		bool bEditPurse1 = bEditPurse && System.GetEditPurse1Flag();
		GetEditPurse1Balance()->SetReadOnly(FALSE == bEditPurse1);
		GetEditPurse1STD()->SetReadOnly(FALSE==bEditPurse1);

		bool bEditPurse2 = bEditPurse && System.GetEditPurse2Flag();		
		GetEditPurse2Balance()->SetReadOnly(FALSE==bEditPurse2);
		GetEditPurse2STD()->SetReadOnly(FALSE==bEditPurse2);
	}

	//COPY CURRENT VALUES INTO POINTS FIELDS
	SetEditBoxInt(*GetEditPointsBalance(), m_RowAccount.GetPoints());
	SetEditBoxInt(*GetEditPointsToDate(), m_RowAccount.GetPointsToDate());

	//ENABLE POINTS FIELDS
	{
		bool bShowPointsButton = TRUE;
		bShowPointsButton &= System.GetShowButtonPointsTopupFlag();
		bShowPointsButton &= (IsHotlisted() == FALSE);
		bShowPointsButton &= Passwords.IsPointsTopupButtonAllowed();
		ShowAndEnableWindow(&m_buttonPointsTopup, bShowPointsButton);

		bool bShowRedeemButton = TRUE;
		bShowRedeemButton &= System.GetShowButtonPointsRedeemFlag();
		bShowRedeemButton &= (IsHotlisted() == FALSE);
		bShowRedeemButton &= Passwords.IsPointsRedeemButtonAllowed();
		ShowAndEnableWindow(&m_buttonPointsRedeem, bShowRedeemButton);

		bool bEditPoints = TRUE;
		bEditPoints &= System.GetEditPointsFlag();
		bEditPoints &= Passwords.IsEditPointsAllowed();
		bEditPoints &= (IsHotlisted() == FALSE);
		GetEditPointsBalance()->SetReadOnly(FALSE==bEditPoints);
		GetEditPointsToDate()->SetReadOnly(FALSE==bEditPoints);
	}

	//COPY CURRENT VALUES INTO GENERAL FIELDS
	SetEditBoxDouble(GetEditGeneralCashSpend(), m_RowAccount.GetCashSpendToDate());

	//MANUAL BONUS BUTTON
	{
		bool bShowManualBonusButton = TRUE;
		bShowManualBonusButton &= System.GetShowButtonManualBonusFlag();
		bShowManualBonusButton &= (IsHotlisted() == FALSE);
		bShowManualBonusButton &= Passwords.IsManualBonusButtonAllowed();
		ShowAndEnableWindow(&m_buttonManualBonus, bShowManualBonusButton);
	}

	//CASH SPEND
	{
		bool bEditCashSpend = TRUE;
		bEditCashSpend &= System.GetEditCashSpendFlag();
		bEditCashSpend &= (IsHotlisted() == FALSE);
		bEditCashSpend &= Passwords.IsEditCashSpendAllowed();
		GetEditGeneralCashSpend()->SetReadOnly(FALSE == bEditCashSpend);
	}

	{
		CString strDateLastUsed = m_RowAccount.GetCashLastSpendDate();

		if (strDateLastUsed.GetLength() > 0)
		{
			GetEditGeneralCashUsedDate()->SetWindowText(strDateLastUsed);
			m_editCashUsedTime.SetWindowText(m_RowAccount.GetCashLastSpendTime());
			SetEditBoxDouble(GetEditGeneralCashUsedValue(), m_RowAccount.GetCashLastSpend());
		}
		else
		{
			GetEditGeneralCashUsedValue()->SetWindowText("");
		}
	}

	ApplyGroupSettings();
	SetAccountBalance();
	SetTabStops();
	
	return TRUE;
}

//$$******************************************************************
CEdit* CEditAccountPropertyPage2::GetEditPurse1Balance() { return GetEdit(IDC_EDIT_PURSE1_BALANCE); }
CEdit* CEditAccountPropertyPage2::GetEditPurse1STD() { return GetEdit(IDC_EDIT_PURSE1_STD); }
CEdit* CEditAccountPropertyPage2::GetEditPurse1UsedValue() { return GetEdit(IDC_EDIT_PURSE1_USED_VALUE); }
CEdit* CEditAccountPropertyPage2::GetEditPurse1UsedDate() { return GetEdit(IDC_EDIT_PURSE1_USED_DATE); }
//$$******************************************************************
CEdit* CEditAccountPropertyPage2::GetEditPurse2Balance() { return GetEdit(IDC_EDIT_PURSE2_BALANCE); }
CEdit* CEditAccountPropertyPage2::GetEditPurse2STD() { return GetEdit(IDC_EDIT_PURSE2_STD); }
CEdit* CEditAccountPropertyPage2::GetEditPurse2UsedValue() { return GetEdit(IDC_EDIT_PURSE2_USED_VALUE); }
CEdit* CEditAccountPropertyPage2::GetEditPurse2UsedDate() { return GetEdit(IDC_EDIT_PURSE2_USED_DATE); }
CEdit* CEditAccountPropertyPage2::GetEditPurse2RefreshValue() { return GetEdit(IDC_EDIT_PURSE2_REFRESH_VALUE); }
CEdit* CEditAccountPropertyPage2::GetEditPurse2RefreshDate() { return GetEdit(IDC_EDIT_PURSE2_REFRESH_DATE); }
//$$******************************************************************
CEdit* CEditAccountPropertyPage2::GetEditPointsBalance() { return GetEdit(IDC_EDIT_POINTS_BALANCE); }
CEdit* CEditAccountPropertyPage2::GetEditPointsValue() { return GetEdit(IDC_EDIT_POINTS_VALUE); }
CEdit* CEditAccountPropertyPage2::GetEditPointsToDate() { return GetEdit(IDC_EDIT_POINTS_TODATE); }
//$$******************************************************************
CEdit* CEditAccountPropertyPage2::GetEditGeneralBalance() { return GetEdit(IDC_EDIT_GENERAL_BALANCE); }
CEdit* CEditAccountPropertyPage2::GetEditGeneralOverdraft() { return GetEdit(IDC_EDIT_GENERAL_OVERDRAFT); }
CEdit* CEditAccountPropertyPage2::GetEditGeneralMaxSpend() { return GetEdit(IDC_EDIT_GENERAL_MAXSPEND); }
CEdit* CEditAccountPropertyPage2::GetEditGeneralCashSpend() { return GetEdit(IDC_EDIT_GENERAL_CASHSPEND); }
CEdit* CEditAccountPropertyPage2::GetEditGeneralCashUsedValue() { return GetEdit(IDC_EDIT_CASH_USED_VALUE); }
CEdit* CEditAccountPropertyPage2::GetEditGeneralCashUsedDate() { return GetEdit(IDC_EDIT_CASH_USED_DATE); }
//$$******************************************************************

void CEditAccountPropertyPage2::SetEditBoxDouble(CEdit* pEdit, double d)
{
	CString str = "";
	str.Format("%.2f", d);
	pEdit->SetWindowText(str);
}

//$$******************************************************************

double CEditAccountPropertyPage2::GetEditBoxDouble(CEdit* pEdit )
{
	return atof(GetEditBoxText(*pEdit));
}

//$$******************************************************************

double CEditAccountPropertyPage2::AddToEditBoxDouble(CEdit* pEdit, double dValue)
{
	double dNewValue = GetEditBoxDouble(pEdit) + dValue;
	SetEditBoxDouble(pEdit, dNewValue);
	return dNewValue;
}

//$$******************************************************************

int CEditAccountPropertyPage2::AddToEditBoxInt(CEdit* pEdit, int nValue)
{
	int nNewValue = GetEditBoxInt(*pEdit) + nValue;
	SetEditBoxInt(*pEdit, nNewValue);
	return nNewValue;
}

//$$******************************************************************

bool CEditAccountPropertyPage2::IsHotlisted()
{
	bool bResult = FALSE;

	if (m_RowAccount.IsCardHotlisted() == TRUE)
	{
		if (System.GetInterfaceType() == nINTERFACE_MIFAREv1)
		{
			bResult = TRUE;
		}
	}
	return bResult;
}

//$$******************************************************************

void CEditAccountPropertyPage2::OnButtonPurse1Topup()
{
	double dOldBalance = atof(GetEditBoxText(*GetEditPurse1Balance()));

	CAddCashDlg dlg(FALSE, FALSE, this);
	dlg.m_dPurseBalance = dOldBalance;

	if (dlg.DoModal() == TRUE)
	{
		double dTopupAmount = dlg.m_dValue;

		if (dTopupAmount != 0.0)
		{
			double dNewBalance = dOldBalance + dTopupAmount;
			SetEditBoxDouble(GetEditPurse1Balance(), dNewBalance);

			m_atc1.SetPurse1(dNewBalance);								// audit cash added line
			m_atc1.SetPurse1Transaction(dTopupAmount);
			m_atc2.SetPurse2Transaction(0.0);								// no purse transaction
			m_atc1.SetCashTransaction(0.0);
			m_atc1.SetPointsTransaction(0);								// no points added
			m_atc1.SetApplicationNo(APPNO_PURSE1_MANTOPUP);
			m_atc1.SetComment(dlg.m_strComment);
			m_atc1.SaveLineToArray(m_arrayAuditBuffer);

			m_atc2.SetPurse1(m_atc2.GetPurse1() + dTopupAmount);		// update secondary audit record with cash added value

			CalculateTopupBonus(FALSE, dNewBalance, dTopupAmount);				// get bonus cash value ( cash will be 0 if used bonus points )
			SetAccountBalance();
		}
	}
}

//$$******************************************************************

void CEditAccountPropertyPage2::OnButtonPurse1Spend()
{
	double dOldBalance = atof(GetEditBoxText(*GetEditPurse1Balance()));
	double dOldSpendTD = atof(GetEditBoxText(*GetEditPurse1STD()));

	CAddCashDlg dlg(TRUE, FALSE, this);
	dlg.m_dPurseBalance = dOldBalance;

	if (dlg.DoModal() == TRUE)
	{
		double dSpendAmount = dlg.m_dValue;

		if (dSpendAmount != 0.0)
		{
			if (dSpendAmount > 0)
			{
				m_atc1.SetPurse1SpendTD(dOldSpendTD + dSpendAmount);
				m_atc2.SetPurse1SpendTD(m_atc2.GetPurse1SpendTD() + dSpendAmount);
				SetEditBoxDouble(GetEditPurse1STD(), dOldSpendTD + dSpendAmount);
			}

			double dNewBalance = dOldBalance - dSpendAmount;
			SetEditBoxDouble(GetEditPurse1Balance(), dNewBalance);
			
			m_atc1.SetPurse1(dNewBalance);								// audit cash added line
			m_atc1.SetPurse1Transaction(dSpendAmount * (-1));
			m_atc2.SetPurse2Transaction(0.0);
			m_atc1.SetCashTransaction(0.0);
			m_atc1.SetPointsTransaction(0);								// no points added
			m_atc1.SetApplicationNo( dSpendAmount < 0.0 ? APPNO_MANUAL_REFUND : APPNO_MANUAL_SPEND);
			m_atc1.SetComment(dlg.m_strComment);
			m_atc1.SaveLineToArray(m_arrayAuditBuffer);

			m_atc2.SetPurse1(m_atc2.GetPurse1() - dSpendAmount);		// update secondary audit record with cash added value

			CalculateSpendBonus(dNewBalance, dSpendAmount);				// get bonus cash value ( cash will be 0 if used bonus points )
			SetAccountBalance();
		}
	}
}

//$$******************************************************************

void CEditAccountPropertyPage2::OnButtonPointsTopup()
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
			if (nPointsToAdd > 0)
			{
				m_atc1.SetPointsTD(nOldPointsToDate + nPointsToAdd);
				m_atc2.SetPointsTD(m_atc2.GetPointsTD() + nPointsToAdd);
				SetEditBoxInt(*GetEditPointsToDate(), nOldPointsToDate + nPointsToAdd);
			}

			SetEditBoxInt(*GetEditPointsBalance(), nOldPointsBalance + nPointsToAdd);
			
			m_atc1.SetPoints(nOldPointsBalance + nPointsToAdd);
			m_atc1.SetPointsTransaction(nPointsToAdd);
			m_atc1.SetPurse1Transaction(0.0);								// no purse transaction
			m_atc1.SetPurse2Transaction(0.0);								// no purse transaction
			m_atc1.SetCashTransaction(0.0);
			m_atc1.SetApplicationNo(APPNO_ADDPOINTS);
			m_atc1.SetComment(dlg.m_strComment);
			m_atc1.SaveLineToArray(m_arrayAuditBuffer);			// save in temp audit file

			m_atc2.SetPoints(m_atc2.GetPoints() + nPointsToAdd);				// update secondary audit record

			SetPointsValue();									//recaluclate point sd=redemption value
		}
	}
}

//$$******************************************************************

void CEditAccountPropertyPage2::OnButtonPointsRedeem()
{
	int nMaxRedeemPoints = SetPointsValue();				// max points that can be redeemed

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

	if (dlg.DoModal() == IDOK)
	{
		double dValueAdded = dlg.m_dRedeemValue;
		int nPointsRedeemed = dlg.m_nRedeemPoints;

		if (nPointsRedeemed > 0)
		{
			int nPointsLeft = AddToEditBoxInt(GetEditPointsBalance(), -nPointsRedeemed);

			m_atc1.SetPoints(nPointsLeft);
			m_atc1.SetPointsTransaction(-nPointsRedeemed);
			m_atc2.SetPoints(m_atc2.GetPoints() - nPointsRedeemed);	// update second audit record

			if (Server.GetSpendBonusToPurse2Flag() == FALSE)
			{
				double dPurse1Balance = AddToEditBoxDouble(GetEditPurse1Balance(), dValueAdded);

				m_atc1.SetPurse1(dPurse1Balance);						// new balance purse1
				m_atc1.SetPurse1Transaction(dValueAdded);
				m_atc1.SetPurse2Transaction(0.0);

				m_atc2.SetPurse1(m_atc2.GetPurse1() + dValueAdded);	// update second audit record
			}
			else
			{
				double dPurse2Balance = AddToEditBoxDouble(GetEditPurse2Balance(), dValueAdded);

				m_atc1.SetPurse2(dPurse2Balance);						// new balance purse2
				m_atc1.SetPurse2Transaction(dValueAdded);
				m_atc1.SetPurse1Transaction(0.0);

				m_atc2.SetPurse2(m_atc2.GetPurse2() + dValueAdded);	// update second audit record
			}


			m_atc1.SetSourceType(AUDIT_PC);
			m_atc1.SetApplicationNo(APPNO_REDEEM);						// points redemption
			m_atc1.SetComment(dlg.m_strComment);
			m_atc1.SaveLineToArray(m_arrayAuditBuffer);		// save in temp file

			SetPointsValue();								// calculate points redemption value
			SetAccountBalance();							// redo current balance
		}
	}
}

//$$******************************************************************

void CEditAccountPropertyPage2::OnButtonManualBonus()
{
	CAddLoyaltyDlg dlg(m_RowAccount.GetGroupNo());

	if (dlg.CanShowDialog() == FALSE)
	{
		dlg.DisplayNoShowReason();
		return;
	}

	{
		double dSpendToDate = 0.0;
		dSpendToDate += GetEditBoxDouble(GetEditPurse1STD());
		dSpendToDate += GetEditBoxDouble(GetEditPurse2STD());
		dSpendToDate += GetEditBoxDouble(GetEditGeneralCashSpend());
		dlg.SetSpendToDate(dSpendToDate);
	}

	{
		double dCurrentPurse = 0.0;
		dCurrentPurse += GetEditBoxDouble(GetEditPurse1Balance());
		dCurrentPurse += GetEditBoxDouble(GetEditPurse2Balance());
		dlg.SetCurrentPurse(dCurrentPurse);
	}

	dlg.SetCurrentPoints(GetEditBoxInt(*GetEditPointsBalance()));

	if (dlg.DoModal() == TRUE)
	{
		double dCashSpend = dlg.GetNewCashSpend();		// cash spend value
		double dCardSpend = dlg.GetNewPurseSpend();		// card spend Purse1 + Purse2
		double dTotalSpend = dCashSpend + dCardSpend;

		if (dTotalSpend != 0.0)
		{
			int nRewardPoints = dlg.GetNewPointsReward();	// bonus points given (if any)
			double dRewardCash = dlg.GetNewCashReward();	// bonus value given (if any)

			int nPointsBalance = AddToEditBoxInt(GetEditPointsBalance(), nRewardPoints);
			int nPointsTD = AddToEditBoxInt(GetEditPointsToDate(), nRewardPoints);
			double dPurse1Balance = AddToEditBoxDouble(GetEditPurse1Balance(), dRewardCash);
			double dPurse1STD = AddToEditBoxDouble(GetEditPurse1STD(), dCardSpend);
			double dCashSpendTD = AddToEditBoxDouble(GetEditGeneralCashSpend(), dCashSpend);

			m_atc1.SetPurse1Transaction(dRewardCash);				// may be 0
			m_atc1.SetPurse2Transaction(0.0);						// no purse2 transaction
			m_atc1.SetCashTransaction(dCashSpend + dCardSpend);	// total spend entered
			m_atc1.SetPointsTransaction(nRewardPoints);			//  ""
			m_atc1.SetPurse1(dPurse1Balance);
			m_atc1.SetPoints( nPointsBalance);
			m_atc1.SetPointsTD(nPointsTD);
			m_atc1.SetCashSpendTD(dCashSpendTD);
			m_atc1.SetPurse1SpendTD(dPurse1STD);

			if (dlg.GetLoyaltyType() == 0)
			{
				m_atc1.SetApplicationNo(APPNO_REWARDBONUSPOINTS);
			}
			else
			{
				m_atc1.SetApplicationNo(APPNO_REWARDBONUSCASH);
			}

			m_atc1.SetComment(dlg.GetComment());
			m_atc1.SaveLineToArray(m_arrayAuditBuffer);	// save in temp audit file

			m_atc2.SetPurse1(m_atc2.GetPurse1() + dRewardCash);
			m_atc2.SetPoints(m_atc2.GetPoints() + nRewardPoints);
			m_atc2.SetPointsTD(m_atc2.GetPointsTD() + nRewardPoints);
			m_atc2.SetCashSpendTD(m_atc2.GetCashSpendTD() + dCashSpend);
			m_atc2.SetPurse1SpendTD(m_atc2.GetPurse1SpendTD() + dCardSpend);

			SetAccountBalance();					
		}
	}
}

//$$******************************************************************

void CEditAccountPropertyPage2::OnKillFocusPurse()
{
	SetAccountBalance();
}

//$$******************************************************************

void CEditAccountPropertyPage2::OnKillFocusPoints()
{
	SetPointsValue();
}

//$$******************************************************************

void CEditAccountPropertyPage2::CalculateTopupBonus(bool bPurse2, double dCurrentBalance, double dCashAdded)
{
	bool bSaveReqd = FALSE;												// set if audit save required

	CCalculateBonus bonus(bPurse2, dCashAdded, &m_atc1);							// atc record contains relevant account details inc GroupNo

	double dBonusCash = bonus.GetBonusCash();							// return bonus cash cash value ( will be 0 if bonus points given )
	if (dBonusCash != 0.0)												// atc will have been updated
	{
		bSaveReqd = TRUE;

		// update secondary audit record
		if (FALSE == bPurse2)
		{
			SetEditBoxDouble(GetEditPurse1Balance(), dCurrentBalance + dBonusCash);
			m_atc2.SetPurse1(m_atc2.GetPurse1() + dBonusCash);
		}
		else
		{
			SetEditBoxDouble(GetEditPurse2Balance(), dCurrentBalance + dBonusCash);
			m_atc2.SetPurse2(m_atc2.GetPurse2() + dBonusCash);
		}
	}
	else
	{
		int nBonusPoints = bonus.GetBonusPoints();
		if (nBonusPoints != 0)										// atc will have been updated
		{
			bSaveReqd = TRUE;
			AddToEditBoxInt(GetEditPointsBalance(), nBonusPoints);
			AddToEditBoxInt(GetEditPointsToDate(), nBonusPoints);
			m_atc2.SetPoints(m_atc2.GetPoints() + nBonusPoints);		// update secondary audit record
			m_atc2.SetPointsTD(m_atc2.GetPointsTD() + nBonusPoints);
		}
	}

	if (bSaveReqd == TRUE)
	{
		m_atc1.SaveLineToArray(m_arrayAuditBuffer);	// save in temp file
	}
}

//$$******************************************************************

void CEditAccountPropertyPage2::CalculateSpendBonus(double dCurrentBalance, double dSpend)
{
	bool bSaveReqd = FALSE;												// set if audit save required

	CCalculateSpendBonus bonus(dSpend, &m_atc1);							// atc record contains relevant account details inc GroupNo

	double dBonusCash = bonus.GetBonusCash();							// return bonus cash cash value ( will be 0 if bonus points given )
	if (dBonusCash != 0.0)												// atc will have been updated
	{
		bSaveReqd = TRUE;
		SetEditBoxDouble(GetEditPurse1Balance(), dCurrentBalance + dBonusCash);
		m_atc2.SetPurse1(m_atc2.GetPurse1() + dBonusCash);			// update secondary audit record
	}
	else
	{
		int nBonusPoints = bonus.GetBonusPoints();
		if (nBonusPoints != 0)										// atc will have been updated
		{
			bSaveReqd = TRUE;
			AddToEditBoxInt(GetEditPointsBalance(), nBonusPoints);
			AddToEditBoxInt(GetEditPointsToDate(), nBonusPoints);
			m_atc2.SetPoints(m_atc2.GetPoints() + nBonusPoints);		// update secondary audit record
			m_atc2.SetPointsTD(m_atc2.GetPointsTD() + nBonusPoints);
		}
	}

	if (bSaveReqd == TRUE)
	{
		m_atc1.SaveLineToArray(m_arrayAuditBuffer);	// save in temp file
	}
}

//$$******************************************************************

int CEditAccountPropertyPage2::SetPointsValue()
{
	double dValue = 0.0;
	int nPoints = GetEditBoxInt(*GetEditPointsBalance());
	int nMaxRedeem = m_pointsRedeemer.Redeem(nPoints, dValue);
	SetEditBoxDouble(GetEditPointsValue(), dValue);
	return nMaxRedeem;
}

//$$******************************************************************

void CEditAccountPropertyPage2::ApplyGroupSettings()
{
	bool bEditMaxSpend = Passwords.IsEditGeneralAllowed();

	m_pointsRedeemer.m_bDisabled = FALSE;
	m_pointsRedeemer.m_nTrip = Server.GetRedeemPointsTrip();
	m_pointsRedeemer.m_dValue = Server.GetRedeemPointsValue();
	m_pointsRedeemer.m_nType = Server.GetRedeemPointsType();

	if ( TRUE == m_bGroupExists )
	{
		switch (m_RowGroup.GetMaxSpendType())
		{
		case nSPENDTYPE_DAILY:
			SetEditBoxDouble(GetEditGeneralMaxSpend(), m_RowGroup.GetMaxSpendPeriod1());
			SetEditBoxDouble(GetEditGeneralOverdraft(), m_RowGroup.GetMaxOverdraftPeriod1());
			bEditMaxSpend = FALSE;
			break;

		case nSPENDTYPE_PERIOD:
			SetEditBoxDouble(GetEditGeneralMaxSpend(), 0.0);
			SetEditBoxDouble(GetEditGeneralOverdraft(), 0.0);
			bEditMaxSpend = FALSE;
			break;

		case nSPENDTYPE_RECORD:
			if (GetEditBoxDouble(GetEditGeneralMaxSpend()) == 0.0)
			{
				SetEditBoxDouble(GetEditGeneralMaxSpend(), m_RowAccount.GetMaxSpend());
			}

			if (GetEditBoxDouble(GetEditGeneralOverdraft()) == 0)
			{
				SetEditBoxDouble(GetEditGeneralOverdraft(), m_RowAccount.GetMaxOverdraft());
			}
			break;
		}

		if (m_RowGroup.GetRedeemFlag() == TRUE)							// group specifc redeem points
		{
			m_pointsRedeemer.m_bDisabled = m_RowGroup.GetRedeemDisable();
			m_pointsRedeemer.m_nTrip = m_RowGroup.GetRedeemTrip();
			m_pointsRedeemer.m_dValue = m_RowGroup.GetRedeemValue();
			m_pointsRedeemer.m_nType = m_RowGroup.GetRedeemType();
		}
	}

	GetEditGeneralMaxSpend()->SetReadOnly(FALSE == bEditMaxSpend);

	SetPointsValue();								// calculate points redemption value

// over spend

	{
		bool bEditOverdraft = TRUE;
		bEditOverdraft &= bEditMaxSpend;
		bEditOverdraft &= Server.GetAllowOverSpendFlag();
		GetEditGeneralOverdraft()->SetReadOnly(FALSE == bEditOverdraft);
	}

	// purse2

	SetEditBoxDouble(GetEditPurse2RefreshValue(), m_RowGroup.GetRefreshValue());

	if (m_RowGroup.GetRefreshType() != nREFRESHTYPE_NORMAL)
	{
		CString strText;
		strText.Format("%s refresh", 
			(const char*) m_RowGroup.GetRefreshTypeText());
		
		m_staticRefresh.SetWindowText(strText);
	}
	else
	{
		m_staticRefresh.SetWindowText("Refresh value");
	}
}

//$$******************************************************************

void CEditAccountPropertyPage2::SetAccountBalance()
{
	m_bCanTopupPurse2 = FALSE;

	bool bEditPurse2 = TRUE;
	bEditPurse2 &= Passwords.IsEditPurseAllowed();
	bEditPurse2 &= (IsHotlisted() == FALSE);
	bEditPurse2 &= System.GetEditPurse2Flag();

	double dPurse1 = GetEditBoxDouble(GetEditPurse1Balance());
	double dPurse2 = GetEditBoxDouble(GetEditPurse2Balance());

	bool bShowPurse2Button = FALSE;
	bool bCanRefreshNow = FALSE;

	if (TRUE == m_bGroupExists)
	{
		if (m_RowGroup.GetRefreshType() != nREFRESHTYPE_NORMAL)
		{
			CSSDate dateToday{};
			dateToday.SetCurrentDate();

			if (LoyaltySubs.IsRefreshPurse2Reqd(m_RowAccount, m_RowGroup, dateToday) == TRUE)
			{
				bCanRefreshNow = TRUE;
				bShowPurse2Button = TRUE;

				if (System.GetPurse2PendingRefreshFlag() == TRUE)
				{
					dPurse2 = LoyaltySubs.GetPurse2NextSpendBalance(m_RowAccount, m_RowGroup);
				}
			}
		}
		else
		{
			bShowPurse2Button = System.GetShowButtonPurse2TopupFlag() && Passwords.IsPurse1TopupButtonAllowed();
		}
	}

	if (TRUE == bShowPurse2Button)
	{
		if (TRUE == bCanRefreshNow)
		{
			m_bCanTopupPurse2 = FALSE;
			m_buttonRefreshNow.SetWindowText("Refresh\nNow");
		}
		else
		{
			m_bCanTopupPurse2 = TRUE;
			m_buttonRefreshNow.SetWindowText("Topup\nPurse");
		}
	}


	ShowAndEnableWindow(&m_staticBalance, bCanRefreshNow && (System.GetPurse2PendingRefreshFlag()) );
	ShowAndEnableWindow(&m_buttonRefreshNow, bShowPurse2Button);
	GetEditPurse2Balance()->SetReadOnly(bCanRefreshNow || ( FALSE == bEditPurse2 ) );

	SetEditBoxDouble(GetEditGeneralBalance(), dPurse1 + dPurse2);
}

//$$******************************************************************

void CEditAccountPropertyPage2::OnButtonRefreshNow()
{
	if (FALSE == m_bCanTopupPurse2)
	{
		RefreshPurse2();
	}
	else
	{
		TopupPurse2();
	}
}

//$$******************************************************************

void CEditAccountPropertyPage2::RefreshPurse2()
{
	bool bTriedRefresh = FALSE;

	if (TRUE == m_bGroupExists)
	{
		if (m_RowGroup.GetRefreshType() != nREFRESHTYPE_NORMAL)
		{
			CSSDate dateToday{};
			dateToday.SetCurrentDate();

			CSSTime timeToday{};
			timeToday.SetCurrentTime();

			if (LoyaltySubs.IsRefreshPurse2Reqd(m_RowAccount, m_RowGroup, dateToday) == TRUE)
			{
				bTriedRefresh = TRUE;
				m_RowAccount.RefreshPurse2(m_RowGroup.GetRefreshValue(), dateToday.GetDate(), timeToday.GetTime());		// Set purse2 in database record

				double dPurse2 = m_RowAccount.GetPurse2Balance();

				m_atc1.SetPurse1Transaction(0.0);
				m_atc1.SetPurse2Transaction(dPurse2);
				m_atc1.SetPurse2(dPurse2);
				m_atc1.SetPointsTransaction(0);
				m_atc1.SetSourceType(AUDIT_SERVER);
				m_atc1.SetApplicationNo(APPNO_REFRESH);
				m_atc1.SaveLineToArray(m_arrayAuditBuffer);

				m_atc2.SetPurse2(dPurse2);

				SetEditBoxDouble(GetEditPurse2Balance(), dPurse2);
			}
		}
	}

	if (FALSE == bTriedRefresh)
	{
		Prompter.Info("Purse2 Refresh is no longer due based\non current account settings");
	}
	else
	{
		CString strDateRefresh = m_RowAccount.GetPurse2RefreshedDate();

		if (strDateRefresh.GetLength() > 0)
		{
			GetEditPurse2RefreshDate()->SetWindowText(strDateRefresh);
			m_editPurse2RefreshTime.SetWindowText(m_RowAccount.GetPurse2RefreshedTime());
		}
	}

	SetAccountBalance();
	
	if (GetEditPurse2Balance()->IsWindowEnabled())
	{
		GetEditPurse2Balance()->SetFocus();
		GetEditPurse2Balance()->SetSel(0,-1);
	}
}

//$$******************************************************************

void CEditAccountPropertyPage2::TopupPurse2()
{
	double dOldBalance = atof(GetEditBoxText(*GetEditPurse2Balance()));

	CAddCashDlg dlg(FALSE, TRUE, this);
	dlg.m_dPurseBalance = dOldBalance;

	if (dlg.DoModal() == TRUE)
	{
		double dTopupAmount = dlg.m_dValue;

		if (dTopupAmount != 0.0)
		{
			double dNewBalance = dOldBalance + dTopupAmount;
			SetEditBoxDouble(GetEditPurse2Balance(), dNewBalance);

			m_atc1.SetPurse2(dNewBalance);								// audit cash added line
			m_atc1.SetPurse1Transaction(0.0);
			m_atc1.SetPurse2Transaction(dTopupAmount);
			m_atc1.SetCashTransaction(0.0);
			m_atc1.SetPointsTransaction(0);								// no points added
			m_atc1.SetApplicationNo(APPNO_PURSE2_MANTOPUP);
			m_atc1.SetComment(dlg.m_strComment);
			m_atc1.SaveLineToArray(m_arrayAuditBuffer);

			m_atc2.SetPurse2(m_atc2.GetPurse2() + dTopupAmount);		// update secondary audit record with cash added value

			CalculateTopupBonus(TRUE, dNewBalance, dTopupAmount);				// get bonus cash value ( cash will be 0 if used bonus points )
			SetAccountBalance();
		}
	}
}

//$$******************************************************************

void CEditAccountPropertyPage2::SetTabStops()
{
	CArray<CEdit*, CEdit*> arrayEditBoxes;
	arrayEditBoxes.Add(GetEditPurse1Balance());
	arrayEditBoxes.Add(GetEditPurse1STD());
	arrayEditBoxes.Add(GetEditPurse2Balance());
	arrayEditBoxes.Add(GetEditPurse2STD());
	arrayEditBoxes.Add(GetEditPointsBalance());
	arrayEditBoxes.Add(GetEditPointsToDate());
	arrayEditBoxes.Add(GetEditGeneralOverdraft());
	arrayEditBoxes.Add(GetEditGeneralMaxSpend());
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

BOOL CEditAccountPropertyPage2::OnSetActive()
{
	if (m_nLastGroupNo != m_pEditAccountBuffer->m_nGroupNo)
	{
		m_nLastGroupNo = m_pEditAccountBuffer->m_nGroupNo;
		m_RowGroup.SetGroupNo(m_nLastGroupNo);
		CSQLRepositoryGroup repoGroup;
		m_bGroupExists = (repoGroup.SelectRow(m_RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE);
	}

	m_atc1.SetCardName(m_pEditAccountBuffer->m_strCardName);
	m_atc1.SetGroupNo(m_pEditAccountBuffer->m_nGroupNo);
	m_atc2.SetCardName(m_pEditAccountBuffer->m_strCardName);
	m_atc2.SetGroupNo(m_pEditAccountBuffer->m_nGroupNo);
	ApplyGroupSettings();
	SetAccountBalance();
	SetTabStops();

	return CSSPropertyPage::OnSetActive();
}

//$$******************************************************************

BOOL CEditAccountPropertyPage2::OnKillActive()
{
	if (UpdateData(TRUE) == TRUE)
	{
		return CSSPropertyPage::OnKillActive();
	}
	else
	{
		return FALSE;
	}
}

//$$******************************************************************

BOOL CEditAccountPropertyPage2::OnQueryCancel()
{
	return CEditAccountPropertySheet::PromptBeforeCancel();
}

//********************************************************************

void CEditAccountPropertyPage2::OnCancel()
{	
	CSSPropertyPage::OnCancel();
}

//$$******************************************************************

void CEditAccountPropertyPage2::OnOK()
{
	m_RowAccount.SetPurse1Balance(GetEditBoxDouble(GetEditPurse1Balance()));
	m_RowAccount.SetPurse2Balance(GetEditBoxDouble(GetEditPurse2Balance()));
	m_RowAccount.SetPurse1SpendToDate(GetEditBoxDouble(GetEditPurse1STD()));
	m_RowAccount.SetPurse2SpendToDate(GetEditBoxDouble(GetEditPurse2STD()));
	m_RowAccount.SetPoints(GetEditBoxInt(*GetEditPointsBalance()));
	m_RowAccount.SetPointsToDate(GetEditBoxInt(*GetEditPointsToDate()));
	m_RowAccount.SetCashSpendToDate(GetEditBoxDouble(GetEditGeneralCashSpend()));
	m_RowAccount.SetMaxOverdraft(GetEditBoxDouble(GetEditGeneralOverdraft()));
	m_RowAccount.SetMaxSpend(GetEditBoxDouble(GetEditGeneralMaxSpend()));
	CSSPropertyPage::OnOK();
}

//$$******************************************************************

void CEditAccountPropertyPage2::UpdateAudit()
{
	// check for any session audit lines
	bool bAuditUpdated = FALSE;
	if (m_arrayAuditBuffer.GetSize() > 0)
	{
		CLoyaltyAuditAppend auditFile;									// main audit file
		auditFile.AppendArray(m_arrayAuditBuffer);
		m_arrayAuditBuffer.RemoveAll();
		bAuditUpdated = TRUE;
	}

	if (m_atc2.AuditChanges(m_RowAccount) == TRUE)				// see if any fields edited manually
	{
		m_atc2.SetApplicationNo(APPNO_EDIT);
		m_atc2.SaveLineToFile();
		bAuditUpdated = TRUE;
	}

	if (bAuditUpdated == FALSE)								// see if any audit saved
	{
		m_atc2.SetComment("");
		m_atc2.SetApplicationNo(APPNO_BALANCE);				// no changes made to record - save a balance line
		m_atc2.SaveLineToFile();
	}
}

//$$******************************************************************
