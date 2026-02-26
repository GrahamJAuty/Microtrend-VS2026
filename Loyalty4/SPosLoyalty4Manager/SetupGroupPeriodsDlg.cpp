//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
//$$******************************************************************
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
//$$******************************************************************
#include "SetupGroupPeriodsDlg.h"
//$$******************************************************************

CSetupGroupPeriodsDlg::CSetupGroupPeriodsDlg(CSQLRowGroup& RowGroup, bool bPurse2Allowed, CWnd* pParent)
	: CSSDialog(CSetupGroupPeriodsDlg::IDD, pParent), m_RowGroup(RowGroup)
{
	m_bPurse2Allowed = bPurse2Allowed;
	m_strTitle.Format ( "Setup Group %d Period Settings", RowGroup.GetGroupNo() );
	SQLRowSetAuditPeriod.LoadPeriods(NULL);
}

//$$******************************************************************

CSetupGroupPeriodsDlg::~CSetupGroupPeriodsDlg()
{
}

//$$******************************************************************

void CSetupGroupPeriodsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PERIODTEXT1, m_editPeriodText1);
	DDX_Control(pDX, IDC_EDIT_PERIODTEXT2, m_editPeriodText2);
	DDX_Control(pDX, IDC_EDIT_PERIODTEXT3, m_editPeriodText3);
	DDX_Control(pDX, IDC_EDIT_PERIODTEXT4, m_editPeriodText4);
	DDX_Control(pDX, IDC_EDIT_PERIODTEXT5, m_editPeriodText5);
	DDX_Control(pDX, IDC_EDIT_PERIODTEXT6, m_editPeriodText6);
	DDX_Control(pDX, IDC_CHECK_P1_PURSE1, m_checkEnable[1][0]);
	DDX_Control(pDX, IDC_CHECK_P1_PURSE2, m_checkEnable[2][0]);
	DDX_Control(pDX, IDC_CHECK_P2_PURSE1, m_checkEnable[1][1]);
	DDX_Control(pDX, IDC_CHECK_P2_PURSE2, m_checkEnable[2][1]);
	DDX_Control(pDX, IDC_CHECK_P3_PURSE1, m_checkEnable[1][2]);
	DDX_Control(pDX, IDC_CHECK_P3_PURSE2, m_checkEnable[2][2]);
	DDX_Control(pDX, IDC_CHECK_P4_PURSE1, m_checkEnable[1][3]);
	DDX_Control(pDX, IDC_CHECK_P4_PURSE2, m_checkEnable[2][3]);
	DDX_Control(pDX, IDC_CHECK_P5_PURSE1, m_checkEnable[1][4]);
	DDX_Control(pDX, IDC_CHECK_P5_PURSE2, m_checkEnable[2][4]);
	DDX_Control(pDX, IDC_CHECK_P6_PURSE1, m_checkEnable[1][5]);
	DDX_Control(pDX, IDC_CHECK_P6_PURSE2, m_checkEnable[2][5]);

	if (TRUE == pDX->m_bSaveAndValidate)
	{
		for (int nPeriod = 1; nPeriod <= 6; nPeriod++)
		{
			if (ValidateMaxSpend(
				GetEditMaxSpend(nPeriod - 1),
				nPeriod,
				Group::MaxSpendPeriod1.Min,
				Group::MaxSpendPeriod1.Max,
				"Max Spend") == FALSE)
			{
				pDX->Fail();
				return;
			}

			if (ValidateMaxSpend(
				GetEditOverSpend(nPeriod - 1),
				nPeriod,
				Group::MaxOverdraftPeriod1.Min,
				Group::MaxOverdraftPeriod1.Max,
				"Overdraft") == FALSE)
			{
				pDX->Fail();
				return;
			}
		}
	}
}

//$$******************************************************************

bool CSetupGroupPeriodsDlg::ValidateMaxSpend(CEdit* pEdit, int nPeriod, double dMin, double dMax, CString strType)
{
	bool bResult = TRUE;

	if (pEdit->IsWindowEnabled() == TRUE)
	{
		double dValue = GetEditBoxDouble(*pEdit);

		if ((dValue < dMin) || (dValue > dMax))
		{
			CString strError = "";
			strError.Format("The %s value for Period %d is out of range.\n\nPlease enter a value between %.2f and %.2f.",
				(const char*)strType,
				nPeriod,
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
BEGIN_MESSAGE_MAP(CSetupGroupPeriodsDlg, CDialog)
END_MESSAGE_MAP()
//$$******************************************************************

BOOL CSetupGroupPeriodsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText ( m_strTitle );

	m_editPeriodText1.SetWindowText(SQLRowSetAuditPeriod.GetPeriodName(0));
	m_editPeriodText2.SetWindowText(SQLRowSetAuditPeriod.GetPeriodName(1));
	m_editPeriodText3.SetWindowText(SQLRowSetAuditPeriod.GetPeriodName(2));
	m_editPeriodText4.SetWindowText(SQLRowSetAuditPeriod.GetPeriodName(3));
	m_editPeriodText5.SetWindowText(SQLRowSetAuditPeriod.GetPeriodName(4));
	m_editPeriodText6.SetWindowText(SQLRowSetAuditPeriod.GetPeriodName(5));

	SubclassEdit(IDC_EDIT_MAXSPEND1, SS_NUM_SDP, 8, "%.2f");
	SubclassEdit(IDC_EDIT_MAXSPEND2, SS_NUM_SDP, 8, "%.2f");
	SubclassEdit(IDC_EDIT_MAXSPEND3, SS_NUM_SDP, 8, "%.2f");
	SubclassEdit(IDC_EDIT_MAXSPEND4, SS_NUM_SDP, 8, "%.2f");
	SubclassEdit(IDC_EDIT_MAXSPEND5, SS_NUM_SDP, 8, "%.2f");
	SubclassEdit(IDC_EDIT_MAXSPEND6, SS_NUM_SDP, 8, "%.2f");
	SubclassEdit(IDC_EDIT_OVERSPEND1, SS_NUM_SDP, 8, "%.2f");
	SubclassEdit(IDC_EDIT_OVERSPEND2, SS_NUM_SDP, 8, "%.2f");
	SubclassEdit(IDC_EDIT_OVERSPEND3, SS_NUM_SDP, 8, "%.2f");
	SubclassEdit(IDC_EDIT_OVERSPEND4, SS_NUM_SDP, 8, "%.2f");
	SubclassEdit(IDC_EDIT_OVERSPEND5, SS_NUM_SDP, 8, "%.2f");
	SubclassEdit(IDC_EDIT_OVERSPEND6, SS_NUM_SDP, 8, "%.2f");

	for (int nPeriod = 0; nPeriod <= 5; nPeriod++)
	{
		for (int nPurse = 1; nPurse <= 2; nPurse++)
		{
			m_checkEnable[nPurse][nPeriod].SetCheck(m_RowGroup.IsPurseAllowed(nPurse, nPeriod));
		}

		if (SQLRowSetAuditPeriod.IsPeriodSet(nPeriod) == FALSE)
		{
			GetEditMaxSpend(nPeriod)->EnableWindow(FALSE);
			m_checkEnable[1][nPeriod].EnableWindow(FALSE);
			m_checkEnable[2][nPeriod].EnableWindow(FALSE);
			GetEditOverSpend(nPeriod)->EnableWindow(FALSE);
		}
		else
		{
			m_checkEnable[2][nPeriod].EnableWindow(m_bPurse2Allowed);
		}
	}

	SetEditBoxDouble(*GetEditMaxSpend(0), m_RowGroup.GetMaxSpendPeriod1() ,2);
	SetEditBoxDouble(*GetEditMaxSpend(1), m_RowGroup.GetMaxSpendPeriod2(), 2);
	SetEditBoxDouble(*GetEditMaxSpend(2), m_RowGroup.GetMaxSpendPeriod3(), 2);
	SetEditBoxDouble(*GetEditMaxSpend(3), m_RowGroup.GetMaxSpendPeriod4(), 2);
	SetEditBoxDouble(*GetEditMaxSpend(4), m_RowGroup.GetMaxSpendPeriod5(), 2);
	SetEditBoxDouble(*GetEditMaxSpend(5), m_RowGroup.GetMaxSpendPeriod6(), 2);

	SetEditBoxDouble(*GetEditOverSpend(0), m_RowGroup.GetMaxOverdraftPeriod1(), 2);
	SetEditBoxDouble(*GetEditOverSpend(1), m_RowGroup.GetMaxOverdraftPeriod2(), 2);
	SetEditBoxDouble(*GetEditOverSpend(2), m_RowGroup.GetMaxOverdraftPeriod3(), 2);
	SetEditBoxDouble(*GetEditOverSpend(3), m_RowGroup.GetMaxOverdraftPeriod4(), 2);
	SetEditBoxDouble(*GetEditOverSpend(4), m_RowGroup.GetMaxOverdraftPeriod5(), 2);
	SetEditBoxDouble(*GetEditOverSpend(5), m_RowGroup.GetMaxOverdraftPeriod6(), 2);
	
	return TRUE;
}

//*******************************************************************

CEdit* CSetupGroupPeriodsDlg::GetEditMaxSpend(int n)
{
	switch (n)
	{
	case 1:		return GetEdit(IDC_EDIT_MAXSPEND2);
	case 2:		return GetEdit(IDC_EDIT_MAXSPEND3);
	case 3:		return GetEdit(IDC_EDIT_MAXSPEND4);
	case 4:		return GetEdit(IDC_EDIT_MAXSPEND5);
	case 5:		return GetEdit(IDC_EDIT_MAXSPEND6);
	default:	return GetEdit(IDC_EDIT_MAXSPEND1);
	}
}

//*******************************************************************

CEdit* CSetupGroupPeriodsDlg::GetEditOverSpend(int n)
{
	switch (n)
	{
	case 1:		return GetEdit(IDC_EDIT_OVERSPEND2);
	case 2:		return GetEdit(IDC_EDIT_OVERSPEND3);
	case 3:		return GetEdit(IDC_EDIT_OVERSPEND4);
	case 4:		return GetEdit(IDC_EDIT_OVERSPEND5);
	case 5:		return GetEdit(IDC_EDIT_OVERSPEND6);
	default:	return GetEdit(IDC_EDIT_OVERSPEND1);
	}
}

//*******************************************************************

void CSetupGroupPeriodsDlg::OnOK()
{
	if (UpdateData(TRUE) == TRUE)
	{
		for (int nPurse = 1; nPurse <= 2; nPurse++)
		{
			for (int nPeriod = 0; nPeriod <= 5; nPeriod++)
			{
				m_RowGroup.SetPurseAllowed(nPurse, nPeriod, IsTicked(m_checkEnable[nPurse][nPeriod]));
			}
		}

		m_RowGroup.SetMaxSpendPeriod1(GetEditBoxDouble(*GetEditMaxSpend(0)));
		m_RowGroup.SetMaxSpendPeriod2(GetEditBoxDouble(*GetEditMaxSpend(1)));
		m_RowGroup.SetMaxSpendPeriod3(GetEditBoxDouble(*GetEditMaxSpend(2)));
		m_RowGroup.SetMaxSpendPeriod4(GetEditBoxDouble(*GetEditMaxSpend(3)));
		m_RowGroup.SetMaxSpendPeriod5(GetEditBoxDouble(*GetEditMaxSpend(4)));
		m_RowGroup.SetMaxSpendPeriod6(GetEditBoxDouble(*GetEditMaxSpend(5)));

		m_RowGroup.SetMaxOverdraftPeriod1(GetEditBoxDouble(*GetEditOverSpend(0)));
		m_RowGroup.SetMaxOverdraftPeriod2(GetEditBoxDouble(*GetEditOverSpend(1)));
		m_RowGroup.SetMaxOverdraftPeriod3(GetEditBoxDouble(*GetEditOverSpend(2)));
		m_RowGroup.SetMaxOverdraftPeriod4(GetEditBoxDouble(*GetEditOverSpend(3)));
		m_RowGroup.SetMaxOverdraftPeriod5(GetEditBoxDouble(*GetEditOverSpend(4)));
		m_RowGroup.SetMaxOverdraftPeriod6(GetEditBoxDouble(*GetEditOverSpend(5)));

		EndDialog(IDOK);
	}
}

//*******************************************************************
