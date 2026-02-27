/******************************************************************************/
#include "SmartPay4Manager.h"
/******************************************************************************/
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
/******************************************************************************/
#include "SetupPebblePaymentDlg.h"
/******************************************************************************/

CSetupPebblePaymentDlg::CSetupPebblePaymentDlg(bool bAdd, CSQLRowPebblePaymentType& RowPayment, CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CSetupPebblePaymentDlg::IDD, pParent), m_RowPayment(RowPayment)
{
	m_bAdd = bAdd;

	if (TRUE == m_bAdd)
	{
		switch (m_RowPayment.GetSPOSPaymentType())
		{
		case 0:
			m_RowPayment.SetPebblePaymentName("CASH");
			m_RowPayment.SetPebbleTranType(0);
			break;

		case 1:
			m_RowPayment.SetPebblePaymentName("CHEQUE");
			m_RowPayment.SetPebbleTranType(1);
			break;

		case 2:
		case 3:
		case 9:
			m_RowPayment.SetPebblePaymentName("CARD");
			m_RowPayment.SetPebbleTranType(2);
			break;

		case 6:
			m_RowPayment.SetPebblePaymentName("ACCOUNT");
			m_RowPayment.SetPebbleTranType(2);
			break;

		default:
			m_RowPayment.SetPebblePaymentName("UNKNOWN");
			m_RowPayment.SetPebbleTranType(0);
			break;
		}
	}
}

/******************************************************************************/

void CSetupPebblePaymentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SPOS, m_editSPOS);
	DDX_Control(pDX, IDC_EDIT_SALENAME, m_editSaleName);
	DDX_Control(pDX, IDC_STATIC_TOPUPTYPE, m_staticTopupType);
	DDX_Control(pDX, IDC_COMBO_TOPUPTYPE, m_comboTopupType);
}

/******************************************************************************/

BEGIN_MESSAGE_MAP(CSetupPebblePaymentDlg, CSSAutoShutdownDialog)
END_MESSAGE_MAP()

/******************************************************************************/

BOOL CSetupPebblePaymentDlg::CSSAutoShutdownPostInitDialog()
{
	CString strTitle = "";
	strTitle.Format("%s SPOS Payment Type",
		m_bAdd ? "Add" : "Edit");
		
	SetWindowText(strTitle);

	CString strSPOS = "";
	strSPOS.Format("%d: ", m_RowPayment.GetSPOSPaymentType());

	switch (m_RowPayment.GetSPOSPaymentType())
	{
	case 0:
		strSPOS += "Cash";
		break;

	case 1:
		strSPOS += "Cheque";
		break;

	case 2:
		strSPOS += "EFT";
		break;

	case 3:
		strSPOS += "EFT CNP";
		break;

	case 6:
		strSPOS += "Loyalty";
		break;

	case 9:
		strSPOS += "EFT Shared";
		break;

	default:
		strSPOS += "Not Supported";
		break;
	}

	m_editSPOS.SetWindowText(strSPOS);
	m_editSaleName.LimitText(PebblePaymentType::PebblePaymentName.Max);

	for (int n = 0; n <= PebblePaymentType::PebbleTranType.Max; n++)
	{
		m_comboTopupType.AddString(m_RowPayment.GetPebbleTranTypeName(n));
	}

	/*****/

	m_editSaleName.SetWindowText(m_RowPayment.GetPebblePaymentName());

	int nType = m_RowPayment.GetPebbleTranType();
	if ((nType < 0) || (nType >= m_comboTopupType.GetCount()))
	{
		nType = 0;
	}

	m_comboTopupType.SetCurSel(nType);

	ShowAndEnableWindow(&m_staticTopupType, m_RowPayment.GetSPOSPaymentType() != 6);
	ShowAndEnableWindow(&m_comboTopupType, m_RowPayment.GetSPOSPaymentType() != 6);

	return TRUE;
}

/******************************************************************************/

void CSetupPebblePaymentDlg::OnOK()
{
	m_RowPayment.SetPebblePaymentName(GetEditBoxText(m_editSaleName));
	
	if (m_RowPayment.GetSPOSPaymentType() != 6)
	{
		m_RowPayment.SetPebbleTranType(m_comboTopupType.GetCurSel());
	}
	
	EndDialog(IDOK);
}

/******************************************************************************/
