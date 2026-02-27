/******************************************************************************/
#include "SmartPay4Manager.h"
/******************************************************************************/
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
/******************************************************************************/
#include "SetupPebbleSPOSPayTypeDlg.h"
/******************************************************************************/

CSetupPebbleSPOSPayTypeDlg::CSetupPebbleSPOSPayTypeDlg(CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CSetupPebbleSPOSPayTypeDlg::IDD, pParent)
{
	m_nPaymentType = 0;
}

/******************************************************************************/

CSetupPebbleSPOSPayTypeDlg::~CSetupPebbleSPOSPayTypeDlg()
{
}

/******************************************************************************/

void CSetupPebbleSPOSPayTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SPOSTYPE, m_comboSPOSPayType);
}

/******************************************************************************/

BEGIN_MESSAGE_MAP(CSetupPebbleSPOSPayTypeDlg, CSSAutoShutdownDialog)
END_MESSAGE_MAP()

/******************************************************************************/

BOOL CSetupPebbleSPOSPayTypeDlg::CSSAutoShutdownPostInitDialog()
{
	m_comboSPOSPayType.AddString("Cash");
	m_comboSPOSPayType.AddString("Cheque");
	m_comboSPOSPayType.AddString("EFT");
	m_comboSPOSPayType.AddString("EFT CNP");
	m_comboSPOSPayType.AddString("EFT SHARED");
	m_comboSPOSPayType.AddString("Loyalty");
	m_comboSPOSPayType.SetCurSel(0);
	return TRUE;
}

/******************************************************************************/

void CSetupPebbleSPOSPayTypeDlg::OnOK()
{
	m_nPaymentType = 0;

	switch (m_comboSPOSPayType.GetCurSel())
	{
	case 0: //CASH
	case 1: //CHEQUE
	case 2: //EFT
	case 3: //EFT-CNP
		m_nPaymentType = m_comboSPOSPayType.GetCurSel();
		break;

	case 4: //EFT SHARED
		m_nPaymentType = 9;
		break;

	case 5: //LOYALTY
		m_nPaymentType = 6;
		break;
	}

	EndDialog(IDOK);
}

/******************************************************************************/
