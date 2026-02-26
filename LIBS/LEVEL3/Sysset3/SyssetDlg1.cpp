/**********************************************************************/

#include "local.h"
#include "SyssetDlg1.h"

/**********************************************************************/

CSyssetDlg1::CSyssetDlg1 ( CSysset* pSysset, const char* szRandom, const char* szTitle, const char* szGroup, const char* szText, const char* szComment1, const char* szComment2, int nCodeLen, CWnd* pParent )
	: CSSDialog ( IDD_SYSSET_DLG1, pParent )
{
	m_pSysset = pSysset;
	m_strRandom = szRandom;
	m_strTitle = szTitle;
	m_strGroup = szGroup;
	m_strText = szText;
	m_nCodeLen = nCodeLen;
	m_bSecretMode = FALSE;

	m_strCode = "";
	m_strComment1 = szComment1;
	m_strComment2 = szComment2;

	m_strUserName = "";
	m_strUserAdd1 = "";
	m_strUserAdd2 = "";
	m_strUserAdd3 = "";
	m_strUserAdd4 = "";
	m_strUserAdd5 = "";
}

/**********************************************************************/

void CSyssetDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SYSSET_STATIC_TEXT, m_staticText);
	DDX_Control(pDX, IDC_SYSSET_STATIC_GROUP3, m_staticGroup3);
	DDX_Control(pDX, IDC_SYSSET_STATIC_GROUP2, m_staticGroup2);
	DDX_Control(pDX, IDC_SYSSET_STATIC_GROUP1, m_staticGroup1);
	DDX_Control(pDX, IDC_SYSSET_STATIC_COMMENT1, m_staticComment1);
	DDX_Control(pDX, IDC_SYSSET_STATIC_COMMENT2, m_staticComment2);
	DDX_Control(pDX, IDC_SYSSET_STATIC_RANDOM, m_staticRandom);
	DDX_Control(pDX, IDC_SYSSET_BUTTON_REGISTER_ONLINE, m_buttonRegisterOnline);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Text(pDX, IDC_SYSSET_STATIC_USERADD1, m_strUserAdd1);
	DDX_Text(pDX, IDC_SYSSET_STATIC_USERADD2, m_strUserAdd2);
	DDX_Text(pDX, IDC_SYSSET_STATIC_USERADD3, m_strUserAdd3);
	DDX_Text(pDX, IDC_SYSSET_STATIC_USERADD4, m_strUserAdd4);
	DDX_Text(pDX, IDC_SYSSET_STATIC_USERADD5, m_strUserAdd5);
	DDX_Text(pDX, IDC_SYSSET_STATIC_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_SYSSET_STATIC_COMMENT1, m_strComment1);
	DDX_Text(pDX, IDC_SYSSET_STATIC_COMMENT2, m_strComment2);
	DDX_Text(pDX, IDC_SYSSET_EDIT_USERPASSWORD, m_strCode);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSyssetDlg1, CSSDialog)
	ON_BN_CLICKED(IDC_BUTTON_SECRET, OnButtonSecret)
	ON_BN_CLICKED(IDC_SYSSET_BUTTON_REGISTER_ONLINE, OnButtonRegisterOnline)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSyssetDlg1::OnInitDialog() 
{
	m_editUserPassword.SubclassDlgItem ( IDC_SYSSET_EDIT_USERPASSWORD, this );

	CDialog::OnInitDialog();

	m_staticRandom.SetWindowText("");
	m_staticText.SetWindowText("");

	SetWindowText ( m_strTitle );

	m_staticGroup1.SetWindowText ( "This system is licenced to, and only for use by");
	m_staticGroup2.SetWindowText ( m_strGroup );

	if ( m_strRandom.GetLength() == 16 )		// entering registration code
	{
		m_staticText.ShowWindow ( SW_HIDE );
		m_buttonRegisterOnline.SetWindowText ( "Register Online" );	// "Register Online"
		m_buttonRegisterOnline.ShowWindow ( SW_NORMAL );

		CString strDisplayRandom;
		strDisplayRandom.Format ( "%s %s %s %s",
			(const char*) m_strRandom.Left(4),
			(const char*) m_strRandom.Mid(4,4),
			(const char*) m_strRandom.Mid(8,4),
			(const char*) m_strRandom.Right(4) );

		m_staticRandom.SetWindowText ( strDisplayRandom );
		m_editUserPassword.SetLimitText ( m_nCodeLen );
		m_editUserPassword.SetValidChars ( SYSSET_VALID_REGISTER );	// only allow digits and space
		
		MoveControl(&m_staticGroup2, 7, 105, 241, 31);
		MoveControl(&m_editUserPassword, 152, 116, 55, 14);
		MoveControl(&m_staticGroup3, 7, 138);
		MoveControl(&m_staticComment1, 9, 146);
		MoveControl(&m_staticComment2, 9, 157);
		MoveControl(&m_buttonOK, 103, 180);
		MoveControl(&m_buttonRegisterOnline, 179, 180);
		ResizeDialog(255, 202);
	}
	else					// entering password or just displaying licence details
	{
		m_buttonRegisterOnline.ShowWindow ( SW_HIDE );

		if ( m_strText != "" )		// got some text to display
		{
			m_staticRandom.ShowWindow ( SW_HIDE );	// hide access code static which shares same screen space as text static
			m_staticText.ShowWindow ( SW_SHOW );	// show the text static
			m_staticText.SetWindowText ( m_strText );
		}

		if ( m_nCodeLen == 0 )		// just displaying licence details
		{ 
			ShowAndEnableWindow(&m_staticGroup2, FALSE);
			ShowAndEnableWindow(&m_staticText, FALSE);
			ShowAndEnableWindow(&m_staticRandom, FALSE);
			ShowAndEnableWindow(&m_editUserPassword, FALSE);
			MoveControl(&m_staticGroup3, 7, 105);
			MoveControl(&m_staticComment1, 9, 113);
			MoveControl(&m_staticComment2, 9, 124);
			MoveControl(&m_buttonOK, 103, 147);
			MoveControl(&m_buttonRegisterOnline, 179, 147);
			ResizeDialog(255,169);
		}
		else						// entering password
		{
			m_editUserPassword.SetLimitText ( m_nCodeLen );
			m_editUserPassword.SetPasswordChar ( '*' );
			m_editUserPassword.SetValidChars ( "" );	// allow all chars
			MoveControl(&m_editUserPassword, 61, 134);
		}
	}

	return TRUE;
}

/**********************************************************************/

void CSyssetDlg1::OnButtonSecret() 
{
	m_editUserPassword.EnableWindow ( TRUE );
	m_editUserPassword.SetFocus();
	m_editUserPassword.SetLimitText ( 12 );		// allow for dealer expiry password length + a few extra for confusion
	m_editUserPassword.SetPasswordChar ( '*' );
	m_editUserPassword.SetValidChars ( "" );	// allow all chars
	m_editUserPassword.SetBackgroundColour ( RGB(255,255,192) );
	m_bSecretMode = TRUE;
}

/**********************************************************************/

void CSyssetDlg1::OnButtonRegisterOnline()
{
	if ( m_strRandom.GetLength() == 16 )		// entering registration code
	{
		m_buttonRegisterOnline.EnableWindow ( FALSE );

		CString str;
		if ( m_pSysset->RegisterOnline ( m_strRandom, str ) == TRUE )
		{
			m_strCode = str;
			EndDialog ( IDOK );
		}
		else
		{
			AfxMessageBox ( str );
		}

		m_buttonRegisterOnline.EnableWindow ( TRUE );
	}
}

/**********************************************************************/
