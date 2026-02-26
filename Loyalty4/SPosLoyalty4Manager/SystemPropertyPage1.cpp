//$$******************************************************************
#include "LoyaltyManager.h"
#include "PasswordListDlg.h"
#include "PasswordLogger.h"
#include "SetupReaderDlg.h"
#include "SQLTweakDlg.h"
//$$******************************************************************
#include "SystemPropertyPage1.h"
//$$******************************************************************
extern CSysset Sysset;
//$$******************************************************************

CSystemPropertyPage1::CSystemPropertyPage1()
	: CSSPropertyPage(CSystemPropertyPage1::IDD)
{
	m_bChangedServerSettings = FALSE;
	m_nChangedPaxtonMode = 0;
	m_nCurrentInterface = 0;
}

//$$******************************************************************

CSystemPropertyPage1::~CSystemPropertyPage1()
{
}

//$$******************************************************************

void CSystemPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_COMBO_INTERFACE, m_comboInterface);
	DDX_Control(pDX, IDC_BUTTON_READER, m_buttonReader);
	DDX_Control(pDX, IDC_STATIC_PREFIXTEXT, m_staticPrefixText);
	DDX_Control(pDX, IDC_EDIT_CARDNOPREFIX, m_editCardNoPrefix);
	DDX_Control(pDX, IDC_CHECK_CHECKDIGIT, m_checkCheckDigit);
	DDX_Control(pDX, IDC_CHECK_QRINFO, m_checkShowQRInfo);
	DDX_Control(pDX, IDC_STATIC_PAXTON, m_staticPaxton);
	DDX_Control(pDX, IDC_CHECK_QREMBED, m_checkQREmbed);
	DDX_Control(pDX, IDC_EDIT_QREMBED, m_editQREmbed);
	/*****/
	DDX_Control(pDX, IDC_CHECK_ENABLEPURSE2, m_checkEnablePurse2);
	DDX_Control(pDX, IDC_CHECK_PENDINGREFRESH, m_checkPendingRefresh);
	/*****/
	DDX_Control(pDX, IDC_BUTTON_PASSWORDS, m_buttonPasswords);
	DDX_Control(pDX, IDC_COMBO_SIGNON, m_comboSignOn);
	/*****/
	DDX_Control(pDX, IDC_CHECK_EXTERNAL_ACCOUNT, m_checkExternalAccount);
	DDX_Control(pDX, IDC_CHECK_SRVIMPORT_UNKNOWN, m_checkSrvImportUnknown);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSystemPropertyPage1, CSSPropertyPage)

	ON_BN_CLICKED(IDC_BUTTON_INTERFACE, OnButtonInterface)
	ON_CBN_SELCHANGE(IDC_COMBO_INTERFACE, OnSelectInterface)
	ON_BN_CLICKED(IDC_BUTTON_READER, OnButtonReader)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
	ON_BN_CLICKED(IDC_CHECK_QREMBED, OnToggleQREmbed)
	/*****/
	ON_BN_CLICKED(IDC_BUTTON_PASSWORDS, OnButtonPasswords)
	ON_CBN_SELCHANGE(IDC_COMBO_SIGNON, OnSelectSignOn)
	
END_MESSAGE_MAP()

//$$******************************************************************

void CSystemPropertyPage1::SetDisplay()
{
	bool bPasswordAllowed = (m_comboSignOn.GetCurSel() == 0) ? FALSE : TRUE;
	bool bReaderAllowed = FALSE;
	bool bPrefixAllowed = FALSE;
	bool bCheckDigitAllowed = FALSE;
	bool bQRInfoAllowed = FALSE;

	switch (GetInterfaceTypeFromCombo())
	{
	case nINTERFACE_SWIPEv1:
		bCheckDigitAllowed = TRUE;
		break;

	case nINTERFACE_SWIPEv2:
		bPrefixAllowed = TRUE;
		bCheckDigitAllowed = TRUE;
		break;

	case nINTERFACE_MIFAREv1:
		bReaderAllowed = TRUE;
		break;

	case nINTERFACE_MIFAREv2:
	case nINTERFACE_MIFAREv3:
		bReaderAllowed = TRUE;
		bQRInfoAllowed = TRUE;
		break;
	}

	m_buttonReader.EnableWindow(bReaderAllowed);
	m_checkCheckDigit.EnableWindow(bCheckDigitAllowed);
	m_checkShowQRInfo.EnableWindow(bQRInfoAllowed);
	m_staticPrefixText.EnableWindow(bPrefixAllowed);
	m_editCardNoPrefix.EnableWindow(bPrefixAllowed);
	m_buttonPasswords.EnableWindow(bPasswordAllowed);
}

//$$******************************************************************

BOOL CSystemPropertyPage1::OnInitDialog()
{
	m_nCurrentInterface = System.GetInterfaceType();

	CPropertyPage::OnInitDialog();

	m_editQREmbed.LimitText(MAX_QRCODE_PREFIX);
	
	m_checkCheckDigit.SetCheck(System.GetCheckDigitFlag());
	m_checkShowQRInfo.SetCheck(System.GetShowQRInfoFlag());
	m_checkEnablePurse2.SetCheck(System.GetEnablePurse2Flag());
	m_checkPendingRefresh.SetCheck(System.GetPurse2PendingRefreshFlag());
	m_checkExternalAccount.SetCheck(System.GetExternalAccountFlag());
	m_checkSrvImportUnknown.SetCheck(System.GetSrvImportUnknownFlag());
	m_checkQREmbed.SetCheck(System.GetQRCodeEmbedFlag());
	m_editQREmbed.SetWindowText(System.GetQRCodeEmbedPrefix());
	OnToggleQREmbed();

	{
		CString strPrefix = System.GetCardNoPrefix();

		if (strPrefix == "")
		{
			strPrefix.Format("%6.6d", Sysset.GetSerialNo());
		}

		m_editCardNoPrefix.SetWindowText(strPrefix);
	}

	m_comboInterface.AddString("Swipe-1");
	m_comboInterface.AddString("Swipe-2");
	m_comboInterface.AddString("MiFare-1");
	m_comboInterface.AddString("MiFare-2");
	m_comboInterface.AddString("MiFare-3");
	m_comboInterface.AddString("12 Digit Card No");

	int nSel = 0;
	switch (m_nCurrentInterface)
	{
	case nINTERFACE_SWIPEv1:		nSel = 0;	break;
	case nINTERFACE_SWIPEv2:		nSel = 1;	break;
	case nINTERFACE_MIFAREv1:		nSel = 2;	break;
	case nINTERFACE_MIFAREv2:		nSel = 3;	break;
	case nINTERFACE_MIFAREv3:		nSel = 4;	break;
	case nINTERFACE_GENERALv1:		nSel = 5;	break;
	default:						nSel = 0;	break;
	}
	m_comboInterface.SetCurSel(nSel);

	// passwords

	FillSignOnCombo(System.IsMiFareSystem(), System.GetSignOnMethod());

	m_comboInterface.EnableWindow(FALSE);
	
	SetDisplay();

	//FIDDLE m_staticPaxton.SetWindowText( System.GetPaxtonModeFlag() ? "(Paxton mode on)" : "(Paxton mode off)"  );
	m_staticPaxton.SetWindowText("");

	if (Passwords.GetPasswordInUse() != 0)
	{
		m_comboInterface.EnableWindow(FALSE);
		m_comboSignOn.EnableWindow(FALSE);
		m_buttonPasswords.EnableWindow(FALSE);
		m_buttonReader.EnableWindow(FALSE);
		m_staticPrefixText.EnableWindow(FALSE);
		m_editCardNoPrefix.EnableWindow(FALSE);
		m_checkCheckDigit.EnableWindow(FALSE);
		m_checkShowQRInfo.EnableWindow(FALSE);
		m_editQREmbed.EnableWindow(FALSE);
	}

	return TRUE;
}

//$$******************************************************************

void CSystemPropertyPage1::OnToggleQREmbed()
{
	m_editQREmbed.EnableWindow(IsTicked(m_checkQREmbed));	
}

//$$******************************************************************

BOOL CSystemPropertyPage1::OnKillActive()
{
	if (UpdateData() == TRUE)
	{
		if (Passwords.GetPasswordInUse() == 0)
		{
			int nInterfaceType = GetInterfaceTypeFromCombo();

			if (nInterfaceType != m_nCurrentInterface)
			{
				m_bChangedServerSettings = TRUE;
			}

			System.SetInterfaceType(nInterfaceType);
			System.SetCheckDigitFlag(IsTicked(m_checkCheckDigit));
			System.SetCardNoPrefix(GetEditBoxText(m_editCardNoPrefix));
			System.SetSignOnMethod(m_comboSignOn.GetCurSel());
		}

		System.SetShowQRInfoFlag(IsTicked(m_checkShowQRInfo));
		System.SetEnablePurse2Flag(IsTicked(m_checkEnablePurse2));
		System.SetPurse2PendingRefreshFlag(IsTicked(m_checkPendingRefresh));
		System.SetExternalAccountFlag(IsTicked(m_checkExternalAccount));
		System.SetSrvImportUnknownFlag(IsTicked(m_checkSrvImportUnknown));
		System.SetQRCodeEmbedFlag(IsTicked(m_checkQREmbed));
		System.SetQRCodeEmbedPrefix(GetEditBoxText(m_editQREmbed));
	}

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

int CSystemPropertyPage1::GetInterfaceTypeFromCombo()
{
	switch( m_comboInterface.GetCurSel() )
	{
	case 0:		return nINTERFACE_SWIPEv1;
	case 1:		return nINTERFACE_SWIPEv2;	
	case 2:		return nINTERFACE_MIFAREv1;
	case 3:		return nINTERFACE_MIFAREv2;
	case 4:		return nINTERFACE_MIFAREv3;
	case 5:		return nINTERFACE_GENERALv1;
	default:	return nINTERFACE_SWIPEv1;
	}
}

//$$******************************************************************

void CSystemPropertyPage1::OnButtonInterface()
{
	CString strMsg = "Changing the system interface setting will alter the behaviour of Loyalty Manager!\n\nAre you sure you wish to change the setting?";
	if ( Prompter.YesNo( strMsg, "Confirmation Required" ) == IDYES )
	{
		m_comboInterface.EnableWindow(TRUE);
		m_comboInterface.SetFocus();
	}
}

//$$******************************************************************

void CSystemPropertyPage1::OnSelectInterface()
{
	if (Passwords.GetPasswordInUse() == 0)
	{
		bool bMiFare = FALSE;
		switch (GetInterfaceTypeFromCombo())
		{
		case nINTERFACE_MIFAREv1:
		case nINTERFACE_MIFAREv2:
		case nINTERFACE_MIFAREv3:
			bMiFare = TRUE;
			break;
		}

		FillSignOnCombo(bMiFare, m_comboSignOn.GetCurSel());
		SetDisplay();
		m_comboInterface.SetFocus();
	}
}

//$$******************************************************************

void CSystemPropertyPage1::OnButtonReader()
{
	CSetupReaderDlg dlg ( this );
	dlg.DoModal();

	//FIDDLE
	//if ( dlg.GetChangedPaxtonMode() != 0 )
	//{
	//	m_nChangedPaxtonMode = dlg.GetChangedPaxtonMode();
	//	m_staticPaxton.SetWindowText( ( 2 == m_nChangedPaxtonMode ) ? "(Paxton mode on)" : "(Paxton mode off)" );
	//}
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CSystemPropertyPage1::FillSignOnCombo( bool bMiFare, int nSel )
{
	int nCountNow = m_comboSignOn.GetCount();
	int nCountNew = bMiFare ? 3 : 2;

	if ( nCountNow != nCountNew )
	{
		m_comboSignOn.ResetContent();

		m_comboSignOn.AddString ( "Disabled" );
		m_comboSignOn.AddString ( "via Password" );

		if (TRUE == bMiFare)
		{
			m_comboSignOn.AddString("via Account card");
		}

		if ((nSel < 0) || (nSel >= m_comboSignOn.GetCount()))
		{
			nSel = 0;
		}

		m_comboSignOn.SetCurSel ( nSel );
	}
}

//$$******************************************************************

void CSystemPropertyPage1::OnSelectSignOn()
{
	if (Passwords.GetPasswordInUse() == 0)
	{
		SetDisplay();
		m_comboSignOn.SetFocus();
	}
}

//$$******************************************************************

void CSystemPropertyPage1::OnButtonPasswords()
{
	if (Passwords.GetPasswordInUse() == 0)
	{
		CPasswordListDlg dlg( &Passwords, (m_comboSignOn.GetCurSel() == 2), this);

		if (dlg.DoModal() == IDOK)
		{
			Passwords.Save();			// save new settings
		}
		else
		{
			Passwords.ReLoad();			// reload old settings
		}

		Passwords.GetPassword(0);									// reload master password
	}
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CSystemPropertyPage1::OnButtonAltKey1()
{
	if ((System.GetDoneSQLFileSizeFlag() == FALSE) || (System.GetDoneSQLShrinkLogFlag() == FALSE))
	{
		CSQLTweakDlg dlg(this);
		dlg.DoModal();
	}
}

//$$******************************************************************

