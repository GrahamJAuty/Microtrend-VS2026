/**********************************************************************/
#include "..\CommonEcrStock\DefEditDlg.h"
#include "..\CommonEcrStock\SMTPResultsDlg.h"
/**********************************************************************/
#include "..\SPosStockManager5\StockOptionsIni.h"
/**********************************************************************/
#include "CustomerStatementErrorInfo.h"
#include "EmailOptions.h"
#include "ListDataEmailAddressDlg.h"
#include "MaxLengths.h"
#include "PropPageOptionsEmail.h"
/**********************************************************************/

CPropPageOptionsEmail::CPropPageOptionsEmail() : CSSPropertyPage(CPropPageOptionsEmail::IDD)
{
	//{{AFX_DATA_INIT(CPropPageOptionsEmail)
	//}}AFX_DATA_INIT
	m_strNewPassword = "";
	m_bGotOldPassword = FALSE;
	m_nMethod = 1;
	m_nPortNo = 0;
}

/**********************************************************************/

CPropPageOptionsEmail::~CPropPageOptionsEmail()
{
}

/**********************************************************************/

void CPropPageOptionsEmail::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageOptionsEmail)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_SERVER, m_staticServer);
	DDX_Control(pDX, IDC_STATIC_USERNAME, m_staticUsername);
	DDX_Control(pDX, IDC_STATIC_PASSWORD, m_staticPassword);
	DDX_Control(pDX, IDC_STATIC_PORT, m_staticPort);
	DDX_Control(pDX, IDC_STATIC_TO1, m_staticTo1);
	DDX_Control(pDX, IDC_STATIC_TO2, m_staticTo2);
	DDX_Control(pDX, IDC_RADIO_METHOD1, m_buttonMethod1);
	DDX_Control(pDX, IDC_RADIO_METHOD2, m_buttonMethod2);
	DDX_Control(pDX, IDC_RADIO_METHOD3, m_buttonMethod3);
	DDX_Control(pDX, IDC_EDIT_SERVER, m_editServer);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUsername);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_BUTTON_PASSWORD, m_buttonPassword);
	DDX_Control(pDX, IDC_CHECK_FROM, m_checkAddressFrom);
	DDX_Control(pDX, IDC_EDIT_ADDRESS_FROM, m_editAddressFrom);
	DDX_Control(pDX, IDC_CHECK_REPLYTO, m_checkReplyTo);
	DDX_Control(pDX, IDC_EDIT_REPLYTO, m_editReplyTo);
	DDX_Control(pDX, IDC_COMBO_EMAIL, m_comboEmail);
	DDX_Control(pDX, IDC_BUTTON_EMAIL_LIST, m_buttonEmail);
	DDX_Control(pDX, IDC_CHECK_SSL, m_checkSSL);
	DDX_Control(pDX, IDC_BUTTON_TEST, m_buttonTest);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPortNo);

	if (2 == m_nMethod)
	{
		DDV_MinMaxInt(pDX, m_nPortNo, 1, 65535);
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageOptionsEmail, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageOptionsEmail)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_RADIO_METHOD1, OnButtonMethod1 )
	ON_BN_CLICKED( IDC_RADIO_METHOD2, OnButtonMethod2 )
	ON_BN_CLICKED( IDC_RADIO_METHOD3, OnButtonMethod3 )
	ON_BN_CLICKED( IDC_BUTTON_PASSWORD, OnButtonPassword )
	ON_BN_CLICKED( IDC_BUTTON_EMAIL_LIST, OnButtonEmailList )
	ON_BN_CLICKED( IDC_CHECK_REPLYTO, OnToggleReplyTo )
	ON_BN_CLICKED( IDC_CHECK_FROM, OnToggleFrom )
	ON_BN_CLICKED( IDC_BUTTON_TEST, OnButtonTest )
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPropPageOptionsEmail::GetEditPort() { return GetEdit( IDC_EDIT_PORT ); }
/**********************************************************************/

BOOL CPropPageOptionsEmail::OnInitDialog()
{
	GetRecordData();
	CPropertyPage::OnInitDialog();
	SubclassEdit(IDC_EDIT_PORT, SS_NUM, 5, "%d");

	if (SysInfo.IsSPOSSMTPServerSystem() == FALSE)
	{
		ShowAndEnableWindow(&m_buttonMethod3, FALSE);

		if (3 == m_nMethod)
		{
			m_nMethod = 2;
		}
	}

	switch (m_nMethod)
	{
	case 2:
		OnButtonMethod2();
		break;

	case 3:
		OnButtonMethod3();
		break;

	case 1:
	default:
		OnButtonMethod1();
		break;
	}

	m_editServer.LimitText(MAX_LENGTH_SMTP_SERVER);
	m_editUsername.LimitText(MAX_LENGTH_SMTP_USERNAME);
	m_editAddressFrom.LimitText(MAX_LENGTH_SMTP_ADDRESS);

	m_editServer.SetWindowText(EmailOptions.GetEmailSMTPServerInternal());
	m_editUsername.SetWindowText(EmailOptions.GetEmailSMTPUsernameInternal());
	m_editAddressFrom.SetWindowText(EmailOptions.GetEmailSMTPFromAddressInternal());

	{
		CString strFrom = EmailOptions.GetEmailSMTPFromAddressInternal();
		bool bEnable = (strFrom != "");
		m_checkAddressFrom.SetCheck(bEnable);
		m_editAddressFrom.EnableWindow(bEnable);
		m_editAddressFrom.SetWindowText(strFrom);
	}

	{
		CString strReplyTo = EmailOptions.GetEmailSMTPReplyToInternal();
		bool bEnable = (strReplyTo != "");
		m_checkReplyTo.SetCheck(bEnable);
		m_editReplyTo.EnableWindow(bEnable);
		m_editReplyTo.SetWindowText(strReplyTo);
	}

	m_checkSSL.SetCheck(EmailOptions.GetEmailSMTPUseSSLFlagInternal());

	CString strPassword = EmailOptions.GetEmailSMTPPasswordInternal();

	if (strPassword != "")
	{
		m_editPassword.SetWindowText("<The password has been set>");
		m_bGotOldPassword = TRUE;
	}
	else
	{
		m_editPassword.SetWindowText("<No password set>");
		m_bGotOldPassword = FALSE;
	}

	FillEmailCombo();

	CString strEmail = EmailOptions.GetEmailRepmanToAddressInternal();
	int nPos = DataManagerNonDb.EmailAddress.FindNameOrFirstAddress(strEmail);

	if (nPos != -1)
	{
		m_comboEmail.SetCurSel(nPos);
	}
	else
	{
		m_comboEmail.SetWindowText(strEmail);
	}

	return TRUE;
}

/**********************************************************************/

void CPropPageOptionsEmail::FillEmailCombo()
{
	m_comboEmail.ResetContent();

	for (int nIndex = 0; nIndex < DataManagerNonDb.EmailAddress.GetSize(); nIndex++)
	{
		m_comboEmail.AddString(DataManagerNonDb.EmailAddress.GetComboText(nIndex));
	}
}

/**********************************************************************/

BOOL CPropPageOptionsEmail::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageOptionsEmail::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPageOptionsEmail::OnButtonMethod1()
{
	SelectMethod(1);
}

/**********************************************************************/

void CPropPageOptionsEmail::OnButtonMethod2()
{
	SelectMethod(2);
}

/**********************************************************************/

void CPropPageOptionsEmail::OnButtonMethod3()
{
	SelectMethod(3);
}

/**********************************************************************/

void CPropPageOptionsEmail::OnToggleFrom()
{
	m_editAddressFrom.EnableWindow(IsTicked(m_checkAddressFrom));
}

/**********************************************************************/

void CPropPageOptionsEmail::OnToggleReplyTo()
{
	m_editReplyTo.EnableWindow(IsTicked(m_checkReplyTo));
}

/**********************************************************************/

void CPropPageOptionsEmail::SelectMethod(int n)
{
	if ((n >= 1) && (n <= 3))
	{
		m_nMethod = n;
		m_buttonMethod1.SetCheck(1 == m_nMethod);
		m_buttonMethod2.SetCheck(2 == m_nMethod);
		m_buttonMethod3.SetCheck(3 == m_nMethod);
		ShowAndEnableWindow(&m_staticServer, 2 == m_nMethod);
		ShowAndEnableWindow(&m_staticUsername, 2 == m_nMethod);
		ShowAndEnableWindow(&m_staticPassword, 2 == m_nMethod);
		ShowAndEnableWindow(&m_checkAddressFrom, 1 != m_nMethod);
		ShowAndEnableWindow(&m_checkReplyTo, 1 != m_nMethod);
		ShowAndEnableWindow(&m_staticPort, 2 == m_nMethod);
		ShowAndEnableWindow(&m_editServer, 2 == m_nMethod);
		ShowAndEnableWindow(&m_editUsername, 2 == m_nMethod);
		ShowAndEnableWindow(&m_editPassword, 2 == m_nMethod);
		ShowAndEnableWindow(&m_buttonPassword, 2 == m_nMethod);
		ShowAndEnableWindow(&m_editAddressFrom, 1 != m_nMethod, IsTicked(m_checkAddressFrom));
		ShowAndEnableWindow(&m_editReplyTo, 1 != m_nMethod, IsTicked(m_checkReplyTo));
		ShowAndEnableWindow(GetEditPort(), 2 == m_nMethod);
		ShowAndEnableWindow(&m_checkSSL, 2 == m_nMethod);
		ShowAndEnableWindow(&m_buttonTest, 1 != m_nMethod);
		
		switch( m_nMethod )
		{
		case 1:
			MoveControl(&m_staticTo1, 40, 79);
			MoveControl(&m_staticTo2, 40, 91);
			MoveControl(&m_comboEmail, 120, 77);
			MoveControl(&m_buttonEmail, 120, 93);
			break;

		case 2:
			MoveControl(&m_checkAddressFrom, 40, 139);
			MoveControl(&m_checkReplyTo, 40, 159);
			MoveControl(&m_staticTo1, 40, 209);
			MoveControl(&m_staticTo2, 40, 221);
			MoveControl(&m_editAddressFrom, 120, 137);
			MoveControl(&m_editReplyTo, 120, 157);
			MoveControl(&m_comboEmail, 120, 207);
			MoveControl(&m_buttonTest, 371, 206);
			MoveControl(&m_buttonEmail, 120, 223);
			break;

		case 3:
			MoveControl(&m_checkAddressFrom, 40, 79);
			MoveControl(&m_checkReplyTo, 40, 99);
			MoveControl(&m_staticTo1, 40, 124);
			MoveControl(&m_staticTo2, 40, 136);
			MoveControl(&m_editAddressFrom, 120, 77);
			MoveControl(&m_editReplyTo, 120, 97);
			MoveControl(&m_comboEmail, 120, 122);
			MoveControl(&m_buttonTest, 371, 121);
			MoveControl(&m_buttonEmail, 120, 138);
			break;
		}
	}

	m_staticTo1.RedrawWindow();
	m_staticTo2.RedrawWindow();
}

/**********************************************************************/

void CPropPageOptionsEmail::OnButtonPassword()
{
	CDefEditDlg dlg("Set SMTP Email Password", "", MAX_LENGTH_SMTP_PASSWORD, "Password", this);
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_strName != "")
		{
			m_strNewPassword = dlg.m_strName;

			if (TRUE == m_bGotOldPassword)
			{
				m_editPassword.SetWindowText("<The password has been changed>");
			}
			else
			{
				m_editPassword.SetWindowText("<The password has been set>");
			}
		}
	}
}

/**********************************************************************/

void CPropPageOptionsEmail::OnButtonEmailList()
{
	CDataManagerInfo info;
	if (DataManagerNonDb.OpenEmailAddressBuffer(info) == FALSE)
	{
		Prompter.ShareError(info);
		return;
	}

	CString strOldEmail;
	m_comboEmail.GetWindowText(strOldEmail);

	CString strNewEmail = strOldEmail;

	strOldEmail = strOldEmail.SpanExcluding("(");
	::TrimSpaces(strOldEmail, FALSE);

	CListDataEmailAddressDlg dlg(strOldEmail, this);

	if (dlg.DoModal() == IDOK)
	{
		if (DataManagerNonDb.WriteEmailAddressBuffer(info) == FALSE)
		{
			Prompter.WriteError(info);
		}
		else
		{
			strNewEmail = dlg.GetSelectedText();
		}
	}

	DataManagerNonDb.CloseEmailAddressBuffer(info);

	FillEmailCombo();
	m_comboEmail.SetWindowText(strNewEmail);
}

/**********************************************************************/

void CPropPageOptionsEmail::OnButtonTest()
{
	CDataManagerInfo info;
	if (DataManagerNonDb.OpenEmailAddressBuffer(info) == FALSE)
	{
		Prompter.ShareError(info);
	}
	else
	{
		TestInternal();
		DataManagerNonDb.CloseEmailAddressBuffer(info);
	}
}

/**********************************************************************/

void CPropPageOptionsEmail::TestInternal()
{
	SaveRecord();

	CSMTPEmailHelpers SMTPEmailHelpers;
	SMTPEmailHelpers.OpenSession(SMTP_SESSIONTYPE_TEST);

	CString strSubject = "";
	strSubject += AfxGetAppName();
	strSubject += " Test Email";

	CString strNickname = EmailOptions.GetEmailRepmanToAddressToUse();

	if (strNickname == "")
	{
		SMTPEmailHelpers.FlagUnsendableEmail(strSubject, CUSTOMER_STATEMENT_ERROR_NOADDRESS);
	}
	else
	{
		CStringArray arrayEmailAddresses;
		CString strInfo = "";
		int nError = EmailOptions.GetEmailAddresses(strNickname, arrayEmailAddresses, strInfo);

		if (nError != 0)
		{
			SMTPEmailHelpers.FlagUnsendableEmail(strSubject, nError);
		}
		else
		{
			CReportFileDoc Dummy;
			SMTPEmailHelpers.CreatePendingEmail(arrayEmailAddresses, strSubject, strSubject, Dummy, "Email0001");
		}
	}

	SMTPEmailHelpers.CreateIndexFile();
	CSMTPResultsDlg dlg(SMTP_SESSIONTYPE_TEST, SMTPEmailHelpers, this);
	dlg.DoModal();
	SMTPEmailHelpers.DeleteSession();
}

/**********************************************************************/

void CPropPageOptionsEmail::GetRecordData()
{
	m_nMethod = EmailOptions.GetEmailMethodInternal() + 1;
	m_nPortNo = EmailOptions.GetEmailSMTPPortInternal();
	
	if ((m_nPortNo < 1) || (m_nPortNo > 65535))
	{
		m_nPortNo = 25;
	}
}

/**********************************************************************/

void CPropPageOptionsEmail::SaveRecord()
{
	EmailOptions.SetEmailMethodInternal(m_nMethod - 1);

	CString str = "";
	m_editServer.GetWindowText(str);
	EmailOptions.SetEmailSMTPServerInternal(str);
	m_editUsername.GetWindowText(str);
	EmailOptions.SetEmailSMTPUsernameInternal(str);
	
	if (IsTicked(m_checkAddressFrom))
	{
		m_editAddressFrom.GetWindowText(str);
		EmailOptions.SetEmailSMTPFromAddressInternal(str);
	}
	else
	{
		EmailOptions.SetEmailSMTPFromAddressInternal("");
	}

	if (IsTicked(m_checkReplyTo))
	{
		m_editReplyTo.GetWindowText(str);
		EmailOptions.SetEmailSMTPReplyToInternal(str);
	}
	else
	{
		EmailOptions.SetEmailSMTPReplyToInternal("");
	}

	CString strEmail = "";
	m_comboEmail.GetWindowText(strEmail);
	strEmail = strEmail.SpanExcluding("(");
	::TrimSpaces(strEmail, FALSE);
	EmailOptions.SetEmailRepmanToAddressInternal(strEmail);

	if (m_strNewPassword != "")
	{
		EmailOptions.SetEmailSMTPPasswordInternal(m_strNewPassword);
	}

	EmailOptions.SetEmailSMTPPortInternal(m_nPortNo);
	EmailOptions.SetEmailSMTPUseSSLFlagInternal(m_checkSSL.GetCheck() != 0);
}

/**********************************************************************/
