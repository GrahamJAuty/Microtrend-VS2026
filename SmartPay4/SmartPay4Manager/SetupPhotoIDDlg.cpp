//*******************************************************************
#include "..\SmartPay4Shared\AlertText.h"
#include "..\SmartPay4Shared\AllergyText.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\UserTypeText.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//*******************************************************************
#include "SmartPay4Manager.h"
//*******************************************************************
#include "SetupPhotoIDDlg.h"
//*******************************************************************

CSetupPhotoIDDlg::CSetupPhotoIDDlg(CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper,CSetupPhotoIDDlg::IDD, pParent)
	, m_nField1Opt(0)
	, m_nField2Opt(0)
	, m_nField3Opt(0)
	, m_nField4Opt(0)
	, m_nField5Opt(0)
	, m_nField6Opt(0)
{
	m_strLabelName	= m_data.m_strLabelName;		// default label  read when class constructed

	SetTranslateTable();							// set display translation array
	SetLabelData();									// set field data
}

//*******************************************************************

void CSetupPhotoIDDlg::SetLabelData()
{
	m_strField1		= GetDisplayText ( m_data.m_strField1 );
	m_strField2		= GetDisplayText ( m_data.m_strField2 );
	m_strField3		= GetDisplayText ( m_data.m_strField3 );
	m_strField4		= GetDisplayText ( m_data.m_strField4 );
	m_strField5		= GetDisplayText ( m_data.m_strField5 );
	m_strField6		= GetDisplayText ( m_data.m_strField6 );

	m_strFieldText1	= m_data.m_strFieldText1;
	m_strFieldText2	= m_data.m_strFieldText2;
	m_strFieldText3	= m_data.m_strFieldText3;
	m_strFieldText4	= m_data.m_strFieldText4;
	m_strFieldText5	= m_data.m_strFieldText5;
	m_strFieldText6	= m_data.m_strFieldText6;
}

//*******************************************************************

void CSetupPhotoIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LABELS, m_comboLabel);
	DDX_Control(pDX, IDC_COMBO_FIELD1, m_comboField1);
	DDX_Control(pDX, IDC_COMBO_FIELD2, m_comboField2);
	DDX_Control(pDX, IDC_COMBO_FIELD3, m_comboField3);
	DDX_Control(pDX, IDC_COMBO_FIELD4, m_comboField4);
	DDX_Control(pDX, IDC_COMBO_FIELD5, m_comboField5);
	DDX_Control(pDX, IDC_COMBO_FIELD6, m_comboField6);
	DDX_Text(pDX, IDC_EDIT_FIELDTEXT1, m_strFieldText1);
	DDX_Text(pDX, IDC_EDIT_FIELDTEXT2, m_strFieldText2);
	DDX_Text(pDX, IDC_EDIT_FIELDTEXT3, m_strFieldText3);
	DDX_Text(pDX, IDC_EDIT_FIELDTEXT4, m_strFieldText4);
	DDX_Text(pDX, IDC_EDIT_FIELDTEXT5, m_strFieldText5);
	DDX_Text(pDX, IDC_EDIT_FIELDTEXT6, m_strFieldText6);
	DDX_CBString(pDX, IDC_COMBO_FIELD1, m_strField1);
	DDX_CBString(pDX, IDC_COMBO_FIELD2, m_strField2);
	DDX_CBString(pDX, IDC_COMBO_FIELD3, m_strField3);
	DDX_CBString(pDX, IDC_COMBO_FIELD4, m_strField4);
	DDX_CBString(pDX, IDC_COMBO_FIELD5, m_strField5);
	DDX_CBString(pDX, IDC_COMBO_FIELD6, m_strField6);
	DDX_CBString(pDX, IDC_COMBO_LABELS, m_strLabelName);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CSetupPhotoIDDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_SETUP, OnButtonSetup)
	ON_CBN_SELCHANGE(IDC_COMBO_LABELS, OnSelchangeComboLabels)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CSetupPhotoIDDlg::CSSAutoShutdownPostInitDialog() 
{
	m_data.CompileLabelCombo ( &m_comboLabel );			// full barman label list

	Server.Read ( Filenames.GetServerDataFilename() );

	int n = 0;
	AddFieldName ( n++, Account::Blank.Label );
	AddFieldName ( n++, Account::Text.Label );
	AddFieldName ( n++, Account::UserID.Label );
	AddFieldName ( n++, Account::Username.Label );
	AddFieldName ( n++, Account::Forename.Label );
	AddFieldName ( n++, Account::Surname.Label );
	AddFieldName ( n++, Account::ForenameSurname.Label );

	CUserTypeText usertext;
	usertext.Open ( Filenames.GetUserTypeFilename() );			// readonly

	AddFieldName ( n++, Account::GroupNo.Label );
	if ( usertext.GetCount() > 0 )	AddFieldName ( n++, Account::GroupType.Label );

	AddFieldName ( n++, Account::GroupUserID.Label );
	if ( usertext.GetCount() > 0 )	AddFieldName ( n++, Account::GroupTypeUserID.Label );

	AddFieldName ( n++, Account::ExpiryDate.Label );
	AddFieldName ( n++, Account::DOB.Label );

	AddFieldName ( n++, Account::Dietary.Label );

	CAllergyText allergy;
//	allergy.Open (Filenames.m_strAllergyFilename );
	if ( allergy.GetAllergyCount() > 0 ) 
	AddFieldName ( n++, Account::AllergyCodes.Label );

	CAlertText alert;
	alert.Open (Filenames.GetAlertFilename() );
	if ( alert.GetAlertCount() > 0 ) 
		AddFieldName ( n++, Account::AlertCode.Label );

	AddFieldName ( n++, Account::MemberID.Label );
	AddFieldName ( n++, Account::Info1.Label );
	AddFieldName ( n++, Account::Info2.Label );
	AddFieldName ( n++, Account::Info3.Label );
	AddFieldName ( n++, Account::Info4.Label );
	AddFieldName ( n++, Account::Info5.Label );
	AddFieldName ( n++, Account::Info6.Label );
	AddFieldName ( n++, Account::Info7.Label );
	AddFieldName ( n++, Account::Info8.Label );

	m_comboField1.SetCurSel ( m_nField1Opt );
	m_comboField2.SetCurSel ( m_nField2Opt );
	m_comboField3.SetCurSel ( m_nField3Opt );
	m_comboField4.SetCurSel ( m_nField4Opt );
	m_comboField5.SetCurSel ( m_nField5Opt );
	m_comboField6.SetCurSel ( m_nField6Opt );
	
	return TRUE;  
}

//*******************************************************************

void CSetupPhotoIDDlg::AddFieldName ( int nOpt, const char* szText ) 
{
	m_comboField1.AddString ( GetDisplayText(szText) );
	m_comboField2.AddString ( GetDisplayText(szText) );
	m_comboField3.AddString ( GetDisplayText(szText) );
	m_comboField4.AddString ( GetDisplayText(szText) );
	m_comboField5.AddString ( GetDisplayText(szText) );
	m_comboField6.AddString ( GetDisplayText(szText) );

	if ( m_data.m_strField1 == szText )		m_nField1Opt = nOpt;
	if ( m_data.m_strField2 == szText )		m_nField2Opt = nOpt;
	if ( m_data.m_strField3 == szText )		m_nField3Opt = nOpt;
	if ( m_data.m_strField4 == szText )		m_nField4Opt = nOpt;
	if ( m_data.m_strField5 == szText )		m_nField5Opt = nOpt;
	if ( m_data.m_strField6 == szText )		m_nField6Opt = nOpt;
}

//***********************************************************************

void CSetupPhotoIDDlg::OnSelchangeComboLabels() 
{
	if ( UpdateData() == TRUE )
	{
		m_data.Load ( m_strLabelName );
		SetLabelData();
		UpdateData(FALSE);
	}
}

//**********************************************************************

void CSetupPhotoIDDlg::OnBnClickedButtonClear()
{
	if ( UpdateData() == TRUE )
	{
		if ( Prompter.YesNo( "Are you sure you wish to clear these label settings", "Please confirm" ) == IDYES )
		{
			m_data.RemoveLabel ( m_strLabelName );			// remove from label data file
			m_data.Load (  m_strLabelName );				// load none existant file 
			SetLabelData();									// set data to default values
			UpdateData(FALSE);
		}
	}
}

//**********************************************************************

bool CSetupPhotoIDDlg::Save() 
{
	if ( UpdateData() == TRUE )
	{
		m_data.m_strLabelName		= m_strLabelName;
		m_data.m_strField1			= GetHeaderText(m_strField1);
		m_data.m_strField2			= GetHeaderText(m_strField2);
		m_data.m_strField3			= GetHeaderText(m_strField3);
		m_data.m_strField4			= GetHeaderText(m_strField4);
		m_data.m_strField5			= GetHeaderText(m_strField5);
		m_data.m_strField6			= GetHeaderText(m_strField6);

		m_data.m_strFieldText1		= m_strFieldText1;
		m_data.m_strFieldText2		= m_strFieldText2;
		m_data.m_strFieldText3		= m_strFieldText3;
		m_data.m_strFieldText4		= m_strFieldText4;
		m_data.m_strFieldText5		= m_strFieldText5;
		m_data.m_strFieldText6		= m_strFieldText6;
		return m_data.SaveDefault();
	}

	return FALSE;
}

//**********************************************************************

void CSetupPhotoIDDlg::OnButtonSave() 
{
	if (Save() == TRUE)
	{
		EndDialog(IDOK);
	}
}

//**********************************************************************

void CSetupPhotoIDDlg::OnButtonTest() 
{
	if ( Save() == TRUE )
	{
		CString strFilename;
		strFilename = Filenames.GetReportFilename ( "R999" );		// temp filename

		CSSFile fileOut;
		if ( fileOut.Open ( strFilename, "wb" ) == TRUE )
		{
			for ( int i = 0 ; i < 20 ; i++ )
				fileOut.WriteLine ( m_data.GetTestReportData() );

			fileOut.Close();
	
			CRepmanHandler repman ( "PhotoID", this );
			int nReply = repman.ShowBarcode ( strFilename, m_data.m_strLabelName );
			repman.DisplayError ( nReply ); 

			m_data.CompileLabelCombo ( &m_comboLabel );
		}
	}
}

//**********************************************************************

void CSetupPhotoIDDlg::OnButtonSetup() 
{
	if ( UpdateData() == TRUE )
	{
		CRepmanHandler repman ( "PhotoID", this );
		int nReply = repman.ShowBarcode ( "SETUP", m_data.m_strLabelName );
		repman.DisplayError ( nReply ); 

		m_data.CompileLabelCombo ( &m_comboLabel );
	}
}

//*******************************************************************/

void CSetupPhotoIDDlg::SetTranslateTable()
{
	CSQLRepositoryUserText Repo;
	CreateDisplayText ( Account::MemberID.Label, Repo.GetMemberIDText ( Account::MemberID.Label ) );
	CreateDisplayText ( Account::Info1.Label, Repo.GetInfo1Text ( Account::Info1.Label ) );
	CreateDisplayText ( Account::Info2.Label, Repo.GetInfo2Text ( Account::Info2.Label ) );
	CreateDisplayText ( Account::Info3.Label, Repo.GetInfo3Text ( Account::Info3.Label ) );
	CreateDisplayText ( Account::Info4.Label, Repo.GetInfo4Text ( Account::Info4.Label ) );
	CreateDisplayText ( Account::Info5.Label, Repo.GetInfo5Text ( Account::Info5.Label ) );
	CreateDisplayText ( Account::Info6.Label, Repo.GetInfo6Text ( Account::Info6.Label ) );
	CreateDisplayText ( Account::Info7.Label, Repo.GetInfo7Text ( Account::Info7.Label ) );
	CreateDisplayText ( Account::Info8.Label, Repo.GetInfo8Text ( Account::Info8.Label ) );
	CreateDisplayText ( Account::DOB.Label, Repo.GetDOBText ( Account::DOB.Label ) );
}

//*******************************************************************/

void CSetupPhotoIDDlg::CreateDisplayText ( const char* szHeaderText, const char* szUserText )
{
	CString strUserText = szUserText;
	if ( strUserText.GetLength() > 0 )
	{
		if ( strUserText != szHeaderText )
		{
			m_arrayFieldIn.Add ( szHeaderText );

			CString strDisplayText;
			strDisplayText.Format ( "%s  { %s }", szHeaderText, szUserText );
			m_arrayFieldOut.Add ( strDisplayText );
		}
	}
}

//*******************************************************************

const char* CSetupPhotoIDDlg::GetDisplayText ( const char* szHeaderText )
{
	for ( int i = 0 ; i < m_arrayFieldIn.GetSize() ; i++ )
	{
		if ( lstrcmp ( szHeaderText, m_arrayFieldIn.GetAt(i) ) == 0 )
			return m_arrayFieldOut.GetAt(i);
	}

	return szHeaderText;
}

//*******************************************************************/

const char* CSetupPhotoIDDlg::GetHeaderText ( const char* szDisplayName )
{
	for ( int i = 0 ; i < m_arrayFieldOut.GetSize() ; i++ )
	{
		if ( lstrcmp ( szDisplayName, m_arrayFieldOut.GetAt(i) ) == 0 )
			return m_arrayFieldIn.GetAt(i);
	}

	return szDisplayName;
}

//*******************************************************************

