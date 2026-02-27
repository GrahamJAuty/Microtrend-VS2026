//******************************************************************************
#include "..\SmartPay4Shared\EODTextData.h"
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLPurse2Handler.h"
//******************************************************************************
#include "ReportDefaultHandler.h"
//******************************************************************************
#include "OfficeMealDlg.h"
//******************************************************************************
#define MEALPAY_X 0
#define MEALPAY_P1 1
#define MEALPAY_P2 2
#define MEALPAY_P1P2 3
#define MEALPAY_P3 4
#define MEALPAY_C 5
#define MEALPAY_P1C 6
#define MEALPAY_P2C 7
#define MEALPAY_P1P2C 8
#define MEALPAY_P3C 9
//******************************************************************************

COfficeMealDlg::COfficeMealDlg(CSQLRowAccountFull& RowAccount, CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, COfficeMealDlg::IDD, pParent), m_RowAccount(RowAccount)
{
	m_pParent = pParent;
	m_dMealValue = 0.0;
	m_dUnapplied = 0.0;
	m_dCashDeduction = 0.0;
	m_dPurse1Deduction = 0.0;
	m_dPurse2Deduction = 0.0;
	m_dPurse3Deduction = 0.0;
	m_strComment = "";
	m_strReportLabel = "R801";

	m_strTitle.Format("Office Meal : UserID %s",
		(const char*)System.FormatCardNo(m_RowAccount.GetUserIDString()));

	CString strBuf = "";

	m_strUserName = m_RowAccount.GetUsername();
	m_dPurse1Balance = m_RowAccount.GetPurse1Balance();
	m_dPurse2Balance = m_RowAccount.GetPurse2Balance();
	m_dPurse3Balance = m_RowAccount.GetPurse3Balance();

	m_dateTransaction.SetCurrentDate();
	m_timeTransaction.SetCurrentTime();;

	m_strPurse1Date = m_RowAccount.GetPurse1LastSpendDate();
	if (m_strPurse1Date != "")
	{
		strBuf.Format("%s @ %s",
			(const char*)m_strPurse1Date,
			(const char*)m_RowAccount.GetPurse1LastSpendTime());

		m_strPurse1Date = strBuf;
	}

	m_strPurse2Date = m_RowAccount.GetPurse2LastSpendDate();
	if (m_strPurse2Date != "")
	{
		strBuf.Format("%s @ %s",
			(const char*)m_strPurse2Date,
			(const char*)m_RowAccount.GetPurse2LastSpendTime());

		m_strPurse2Date = strBuf;
	}

	m_strPurse3Date = m_RowAccount.GetPurse3LastSpendDate();
	if (m_strPurse3Date != "")
	{
		strBuf.Format("%s @ %s",
			(const char*)m_strPurse3Date,
			(const char*)m_RowAccount.GetPurse3LastSpendTime());

		m_strPurse3Date = strBuf;
	}

	m_RowGroup.SetGroupNo(m_RowAccount.GetGroupNo());

	CSQLRepositoryGroup RepoGroup;
	if (RepoGroup.SelectRow(m_RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		m_strGroupName = m_RowGroup.GetListboxText();
	}
	else
	{
		m_strGroupName.Format("%d  :  Unknown", m_RowGroup.GetGroupNo());
	}

	if (m_RowGroup.GetRefreshType() == nREFRESHTYPE_DISABLED)			// not refresh group
	{
		m_dPurse2Balance = 0;
	}
	else
	{
		CSQLPurse2Handler purse2(m_RowAccount, m_RowGroup);
		m_dPurse2Balance = purse2.GetBalance(FALSE, m_dateTransaction.GetDate(), m_timeTransaction.GetTime());	// get purse balance for currrent period - but don't save if refresh occurs

		if (purse2.Purse2Allowed() == FALSE)
		{
			m_dPurse2Balance = 0;										// don't add refresh for hotlisted card
		}
	}

	m_dPurse1Starting = m_dPurse1Balance;
	m_dPurse2Starting = m_dPurse2Balance;
	m_dPurse3Starting = m_dPurse3Balance;
}

//******************************************************************************

COfficeMealDlg::~COfficeMealDlg()
{
}

//******************************************************************************

void COfficeMealDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_PURSE1, m_checkPurse1);
	DDX_Control(pDX, IDC_CHECK_PURSE2, m_checkPurse2);
	DDX_Control(pDX, IDC_CHECK_PURSE3, m_checkPurse3);
	DDX_Control(pDX, IDC_CHECK_CASH, m_checkCash);
	DDX_Text(pDX, IDC_EDIT_MEALVALUE, m_dMealValue);
	DDX_Text(pDX, IDC_EDIT_PURSE1, m_dPurse1Balance);
	DDX_Text(pDX, IDC_EDIT_PURSE2, m_dPurse2Balance);
	DDX_Text(pDX, IDC_EDIT_PURSE3, m_dPurse3Balance);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_GROUPNO, m_strGroupName);
	DDX_Text(pDX, IDC_EDIT_PURSE1DATELASTUSED, m_strPurse1Date);
	DDX_Text(pDX, IDC_EDIT_PURSE2DATELASTUSED, m_strPurse2Date);
	DDX_Text(pDX, IDC_EDIT_PURSE3DATELASTUSED, m_strPurse3Date);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
	DDX_Text(pDX, IDC_EDIT_DEDUCT4, m_dCashDeduction);
	DDX_Text(pDX, IDC_EDIT_UNAPPLIED, m_dUnapplied);
}

//******************************************************************************

BEGIN_MESSAGE_MAP(COfficeMealDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_CHECK_PURSE1, OnTogglePurse1)
	ON_BN_CLICKED(IDC_CHECK_PURSE2, OnTogglePurse2)
	ON_BN_CLICKED(IDC_CHECK_PURSE3, OnTogglePurse3)
	ON_BN_CLICKED(IDC_CHECK_CASH, OnToggleCash)
	ON_EN_KILLFOCUS(IDC_EDIT_MEALVALUE, OnKillFocusMealValue)
END_MESSAGE_MAP()

//******************************************************************************
// COfficeMealDlg message handlers

void COfficeMealDlg::CSSAutoShutdownPreInitDialog()
{
	CString strLine = "";
	CReportDefaultHandler repdefs;
	if (repdefs.Read(m_strReportLabel, strLine) == TRUE)
	{
		CCSV csv(strLine);
		m_dMealValue = csv.GetDouble(0);
		m_strComment = csv.GetString(1);
	}

	if (m_strComment == "")
	{
		CEODTextData eodText{};
		eodText.Read();
		m_strComment = eodText.GetText(nOFFICEMEAL);						// use eod text

		if (m_strComment == "")
		{
			m_strComment = "Office Meal";
		}
	}

	SubclassEdit(IDC_EDIT_MEALVALUE, SS_NUM_SDP, 11, "%.2f");			// meal
	SubclassEdit(IDC_EDIT_UNAPPLIED, SS_NUM_SDP, 11, "%.2f");			// Unappled meal value
	SubclassEdit(IDC_EDIT_PURSE1, SS_NUM_SDP, 11, "%.2f");			// Purse1
	SubclassEdit(IDC_EDIT_PURSE2, SS_NUM_SDP, 11, "%.2f");			// Purse2
	SubclassEdit(IDC_EDIT_PURSE3, SS_NUM_SDP, 11, "%.2f");			// Purse3
	SubclassEdit(IDC_EDIT_DEDUCT4, SS_NUM_SDP, 11, "%.2f");			// Deduct cash
}

//******************************************************************************

BOOL COfficeMealDlg::CSSAutoShutdownPostInitDialog()
{
	SetWindowText ( m_strTitle );

	bool bAllowed = ( m_dPurse2Balance > 0 );
	m_checkPurse2.EnableWindow ( bAllowed );
	GetEditPurse2() -> EnableWindow ( bAllowed );

	m_checkPurse3.EnableWindow ( System.GetEnablePurse3Flag() );
	GetEditPurse3() -> EnableWindow ( System.GetEnablePurse3Flag() );

	return TRUE;
}

//*******************************************************************************
void COfficeMealDlg::OnKillFocusMealValue()	{ TogglePayment(0); }
void COfficeMealDlg::OnTogglePurse1()		{ TogglePayment(1); }
void COfficeMealDlg::OnTogglePurse2()		{ TogglePayment(2); }
void COfficeMealDlg::OnTogglePurse3()		{ TogglePayment(3); }
void COfficeMealDlg::OnToggleCash()			{ TogglePayment(4); }
//******************************************************************************

void COfficeMealDlg::TogglePayment(int nTickedNow)
{
	if (UpdateData() == TRUE)
	{
		if ((m_dMealValue == 0.0) && (IsTicked(m_checkCash) || IsTicked(m_checkPurse1) || IsTicked(m_checkPurse2) || IsTicked(m_checkPurse3)))
		{
			m_checkPurse1.SetCheck(FALSE);
			m_checkPurse2.SetCheck(FALSE);
			m_checkPurse3.SetCheck(FALSE);
			m_checkCash.SetCheck(FALSE);
			Prompter.Info("Please enter a value for the meal.", m_strTitle);
			GetEditMealValue()->SetFocus();
		}
	}

	int nPayType = MEALPAY_X;

	if (IsTicked(m_checkPurse1))
	{
		nPayType = MEALPAY_P1;
	}

	if (IsTicked(m_checkPurse2))
	{
		if (MEALPAY_P1 == nPayType)
		{
			nPayType = MEALPAY_P1P2;
		}
		else
		{
			nPayType = MEALPAY_P2;
		}
	}

	if (IsTicked(m_checkPurse3))
	{
		if ((3 == nTickedNow) || (MEALPAY_X == nPayType))
		{
			nPayType = MEALPAY_P3;
		}
	}

	if (IsTicked(m_checkCash))
	{
		switch (nPayType)
		{
		case MEALPAY_X:
			nPayType = MEALPAY_C;
			break;

		case MEALPAY_P1:
			nPayType = MEALPAY_P1C;
			break;

		case MEALPAY_P2:
			nPayType = MEALPAY_P2C;
			break;

		case MEALPAY_P1P2:
			nPayType = MEALPAY_P1P2C;
			break;

		case MEALPAY_P3:
			nPayType = MEALPAY_P3C;
			break;
		}
	}

	m_dPurse1Balance = m_dPurse1Starting;
	m_dPurse2Balance = m_dPurse2Starting;
	m_dPurse3Balance = m_dPurse3Starting;
	m_dPurse1Deduction = 0.0;
	m_dPurse2Deduction = 0.0;
	m_dPurse3Deduction = 0.0;
	m_dCashDeduction = 0.0;

	bool bPurse1 = FALSE;
	bool bPurse2 = FALSE;
	bool bPurse3 = FALSE;
	bool bCash = FALSE;

	switch (nPayType)
	{
	case MEALPAY_P1:
	case MEALPAY_P1C:
		m_dPurse1Deduction = m_dMealValue;
		bPurse1 = TRUE;
		break;

	case MEALPAY_P2:
	case MEALPAY_P2C:
		m_dPurse2Deduction = FastMin(m_dMealValue, m_dPurse2Starting);
		m_dPurse2Deduction = FastMax(m_dPurse2Deduction, 0.0);
		bPurse2 = TRUE;
		break;

	case MEALPAY_P1P2:
	case MEALPAY_P1P2C:
		m_dPurse2Deduction = FastMin(m_dMealValue, m_dPurse2Starting);
		m_dPurse2Deduction = FastMax(m_dPurse2Deduction, 0.0);
		m_dPurse1Deduction = FastMax(m_dMealValue - m_dPurse2Deduction, 0.0);
		bPurse1 = TRUE;
		bPurse2 = TRUE;
		break;

	case MEALPAY_P3:
	case MEALPAY_P3C:
		m_dPurse3Deduction = m_dMealValue;
		bPurse3 = TRUE;
		break;

	case MEALPAY_C:
		m_dCashDeduction = m_dMealValue;
		bCash = TRUE;
		break;

	case MEALPAY_X:
	default:
		break;
	}

	switch (nPayType)
	{
	case MEALPAY_P1C:
	case MEALPAY_P1P2C:
		if (m_dPurse1Deduction > m_dPurse1Starting)
		{
			if (m_dPurse1Starting > 0.0)
			{
				m_dCashDeduction = m_dPurse1Deduction - m_dPurse1Starting;
				m_dPurse1Deduction = m_dPurse1Starting;
			}
			else
			{
				m_dCashDeduction = m_dPurse1Deduction;
				m_dPurse1Deduction = 0.0;
			}
		}
		bCash = TRUE;
		break;

	case MEALPAY_P2C:
		if (m_dPurse2Deduction < m_dMealValue)
		{
			m_dCashDeduction = m_dMealValue - m_dPurse2Deduction;
		}
		bCash = TRUE;
		break;

	case MEALPAY_P3C:
		if (m_dPurse3Deduction > m_dPurse3Starting)
		{
			if (m_dPurse3Starting > 0.0)
			{
				m_dCashDeduction = m_dPurse3Deduction - m_dPurse3Starting;
				m_dPurse3Deduction = m_dPurse3Starting;
			}
			else
			{
				m_dCashDeduction = m_dPurse3Deduction;
				m_dPurse3Deduction = 0.0;
			}
		}
		bCash = TRUE;
		break;
	}

	m_dPurse1Balance -= m_dPurse1Deduction;
	m_dPurse2Balance -= m_dPurse2Deduction;
	m_dPurse3Balance -= m_dPurse3Deduction;

	m_checkPurse1.SetCheck(bPurse1);
	m_checkPurse2.SetCheck(bPurse2);
	m_checkPurse3.SetCheck(bPurse3);
	m_checkCash.SetCheck(bCash);

	m_dUnapplied = m_dMealValue - (m_dCashDeduction + m_dPurse1Deduction + m_dPurse2Deduction + m_dPurse3Deduction);

	UpdateData(FALSE);
}

//******************************************************************************

bool COfficeMealDlg::UpdateAccount()
{
	if (m_dMealValue == 0.0)
	{
		return FALSE;
	}

	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.SelectRow(m_RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		m_atcRecord.Set(&m_RowAccount);
		m_atcRecord.SetSourceType(AUDIT_PC);
		m_atcRecord.SetOperatorID(Passwords.GetPasswordInUse());

		// Refresh purse2

		if (m_dPurse2Deduction > 0)
		{
			CSQLPurse2Handler purse2(m_RowAccount, m_RowGroup);
			double dNewPurse2 = purse2.GetBalance(TRUE, m_dateTransaction.GetDate(), m_timeTransaction.GetTime());	// get purse2 balance ***** NOTE will refresh & update RefreshDatabase if refresh required *****
			if (purse2.IsRefreshReqd() == TRUE)								// see if purse2 was refreshed / Refresh Database will have been updated
			{
				CString strDate = purse2.GetEODRefreshDate();

				if (strDate == "")
				{
					m_RowAccount.RevaluePurse2(dNewPurse2);						// Set purse2 in database record
					m_atcRecord.SetTransactionPurse2Balance(purse2.GetRefreshValue());	// set refresh value
					m_atcRecord.SetApplicationNo(APPNO_REFRESH);
				}
				else
				{
					m_RowAccount.RevaluePurse2(dNewPurse2, strDate, "00:00");		// Set purse2 in database record
					m_atcRecord.SetTransactionPurse2Balance(purse2.GetRefreshValue());	// set refresh value
					m_atcRecord.SetApplicationNo(APPNO_REFRESH);
				}
					
				SaveAudit();
			}
		}

			// save meal details

		m_atcRecord.SetTransactionPurse2Balance(0);									// clear any refresh value still lurking
		m_atcRecord.SetApplicationNo(APPNO_MEAL);
		m_atcRecord.SetLineComment(m_strComment);

		CString strTrnDate = m_dateTransaction.GetDate();
		CString strTrnTime = m_timeTransaction.GetTime();

		if (m_dPurse1Deduction > 0)
		{
			double dCreditUsed = 0.0;
			double dLiabilityUsed = 0.0;
			m_RowAccount.AllocatePurse1Spend(m_dPurse1Deduction, dLiabilityUsed, dCreditUsed);

			m_atcRecord.SetTransactionPurse1Liability(-(dLiabilityUsed));
			m_atcRecord.SetTransactionPurse1Credit(-(dCreditUsed));

			m_RowAccount.SubtractFromPurse1Liability(dLiabilityUsed);
			m_RowAccount.SubtractFromPurse1Credit(dCreditUsed);
			m_RowAccount.AddToPurse1SpendToDate(m_dPurse1Deduction);

			CSSDate dbDate(m_RowAccount.GetPurse1LastSpendDate());
			CSSTime dbTime(m_RowAccount.GetPurse1LastSpendTime());

			if ((m_dateTransaction > dbDate) || ((m_dateTransaction == dbDate) && (m_timeTransaction >= dbTime)))
			{
				m_RowAccount.SetPurse1LastSpendDate(strTrnDate);
				m_RowAccount.SetPurse1LastSpendTime(strTrnTime);
				m_RowAccount.AddToPurse1LastSpend(m_dPurse1Deduction);
			}	
		}

		if (m_dPurse2Deduction > 0)
		{
			m_atcRecord.SetTransactionPurse2Balance(-(m_dPurse2Deduction));

			m_RowAccount.SubtractFromPurse2Balance(m_dPurse2Deduction);
			m_RowAccount.AddToPurse2SpendToDate(m_dPurse2Deduction);

			CSSDate dbDate(m_RowAccount.GetPurse2LastSpendDate());
			CSSTime dbTime(m_RowAccount.GetPurse2LastSpendTime());

			if ((m_dateTransaction > dbDate) || ((m_dateTransaction == dbDate) && (m_timeTransaction >= dbTime)))
			{
				m_RowAccount.SetPurse2LastSpendDate(strTrnDate);
				m_RowAccount.SetPurse2LastSpendTime(strTrnTime);
				m_RowAccount.AddToPurse2LastSpend(m_dPurse2Deduction);
			}
		}

		if (m_dPurse3Deduction > 0)
		{
			double dCreditUsed = 0.0;
			double dLiabilityUsed = 0.0;
			m_RowAccount.AllocatePurse3Spend(m_dPurse3Deduction, dLiabilityUsed, dCreditUsed);

			m_atcRecord.SetTransactionPurse3Liability(-(dLiabilityUsed));
			m_atcRecord.SetTransactionPurse3Credit(-(dCreditUsed));

			m_RowAccount.SubtractFromPurse3Liability(dLiabilityUsed);
			m_RowAccount.SubtractFromPurse3Credit(dCreditUsed);
			m_RowAccount.AddToPurse3SpendToDate(m_dPurse3Deduction);

			CSSDate dbDate(m_RowAccount.GetPurse3LastSpendDate());
			CSSTime dbTime(m_RowAccount.GetPurse3LastSpendTime());

			if ((m_dateTransaction > dbDate) || ((m_dateTransaction == dbDate) && (m_timeTransaction >= dbTime)))
			{
				m_RowAccount.SetPurse3LastSpendDate(strTrnDate);
				m_RowAccount.SetPurse3LastSpendTime(strTrnTime);
				m_RowAccount.AddToPurse3LastSpend(m_dPurse3Deduction);
			}
		}

		if (m_dCashDeduction > 0)
		{
			m_atcRecord.SetTransactionCash(-(m_dCashDeduction));
			m_RowAccount.AddToCashSpendToDate(m_dCashDeduction);
		}

		SaveAudit();
		RepoAccount.UpdateRow(m_RowAccount,NULL);
	}
	
	return TRUE;
}

//***********************************************************************************

void COfficeMealDlg::SaveAudit()
{
	m_atcRecord.SetCurrentPurse1Liability(m_RowAccount.GetPurse1Liability());
	m_atcRecord.SetCurrentPurse2Balance(m_RowAccount.GetPurse2Balance());
	m_atcRecord.SetCurrentPurse3Liability(m_RowAccount.GetPurse3Liability());
	m_atcRecord.SetCurrentPurse1Credit(m_RowAccount.GetPurse1Credit());
	m_atcRecord.SetCurrentPurse3Credit(m_RowAccount.GetPurse3Credit());
	m_atcRecord.SetToDateCashSpend(m_RowAccount.GetCashSpendToDate());
	m_atcRecord.SetToDatePurse1Spend(m_RowAccount.GetPurse1SpendToDate());
	m_atcRecord.SetToDatePurse2Spend(m_RowAccount.GetPurse2SpendToDate());
	m_atcRecord.SetToDatePurse3Spend(m_RowAccount.GetPurse3SpendToDate());
	m_atcRecord.SaveNewLine();
}

//***********************************************************************************

void COfficeMealDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		if (::CompareDoubles(m_dMealValue, 0.0, 3) == 0)
		{
			Prompter.Error("No meal has been purchased.");
			return;
		}

		double dPurses = m_dPurse1Deduction + m_dPurse2Deduction + m_dPurse3Deduction;
		double dTotalDeductions = m_dCashDeduction + dPurses;

		if ( ::CompareDoubles(dTotalDeductions, m_dMealValue, 3) == 0 )
		{
			CString strMsg = "";
			if (dPurses > 0.0)
			{
				strMsg.Format("Deduct a total of £%.2lf from the account", dPurses);
			}

			if (::CompareDoubles(m_dCashDeduction, 0.0, 3) != 0)
			{
				CString strMsg1 = "";

				if (strMsg == "")
				{
					strMsg1.Format("Assign £%.2lf to cash", m_dCashDeduction);
				}
				else
				{
					strMsg1.Format(" and assign £%.2lf to cash", m_dCashDeduction);
				}

				strMsg += strMsg1;
			}

			int nReply = Prompter.YesNo(strMsg, m_strTitle, TRUE);

			if (nReply == IDNO)
			{
				return;
			}
			else
			{
				CCSV csv{};
				csv.Add(m_dMealValue, System.GetDPValue());
				csv.Add(m_strComment);

				CReportDefaultHandler repdefs;
				repdefs.Save(m_strReportLabel, &csv);

				EndDialog(IDOK);
			}
		}
		else
		{
			CString strMsg = "";

			if (  ::CompareDoubles( dTotalDeductions, m_dMealValue, 3) == 1 )
			{
				strMsg.Format("Total deductions ( £%.2lf  ) excess the value of the meal (£%.2lf )!", 
					dTotalDeductions, 
					m_dMealValue);
			}
			else
			{
				if (::CompareDoubles(dTotalDeductions, 0.0, 3) == 0)
				{
					strMsg = "No meal value has been applied!";
				}
				else
				{
					strMsg.Format("£%.2lf still needs to be applied!", 
						m_dMealValue - dTotalDeductions);
				}
			}

			Prompter.Info(strMsg, m_strTitle);
			GetEditMealValue() -> SetFocus();
		}
	}
}

//***********************************************************************************
