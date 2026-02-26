//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\ServerData.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//$$******************************************************************
#include "ServerPropertyPage2.h"
#include "SetupEposPathsDlg.h"
#include "StandardCombos.h"
//$$******************************************************************
extern CSysset Sysset;
//$$******************************************************************

CServerPropertyPage2::CServerPropertyPage2()
	:CSSPropertyPage(CServerPropertyPage2::IDD)
{
	m_nPhotoIDFolderType = 0;
}

//$$******************************************************************

CServerPropertyPage2::~CServerPropertyPage2()
{
}

//$$******************************************************************

void CServerPropertyPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_FIELD1, m_comboLineType[0]);
	DDX_Control(pDX, IDC_COMBO_FIELD2, m_comboLineType[1]);
	DDX_Control(pDX, IDC_COMBO_FIELD3, m_comboLineType[2]);
	DDX_Control(pDX, IDC_COMBO_FIELD4, m_comboLineType[3]);
	DDX_Control(pDX, IDC_COMBO_FIELD5, m_comboLineType[4]);
	DDX_Control(pDX, IDC_COMBO_FIELD6, m_comboLineType[5]);
	DDX_Control(pDX, IDC_COMBO_FIELD7, m_comboLineType[6]);
	DDX_Control(pDX, IDC_COMBO_FIELD8, m_comboLineType[7]);
	DDX_Control(pDX, IDC_EDIT_TEXT1, m_editLineText[0]);
	DDX_Control(pDX, IDC_EDIT_TEXT2, m_editLineText[1]);
	DDX_Control(pDX, IDC_EDIT_TEXT3, m_editLineText[2]);
	DDX_Control(pDX, IDC_EDIT_TEXT4, m_editLineText[3]);
	DDX_Control(pDX, IDC_EDIT_TEXT5, m_editLineText[4]);
	DDX_Control(pDX, IDC_EDIT_TEXT6, m_editLineText[5]);
	DDX_Control(pDX, IDC_EDIT_TEXT7, m_editLineText[6]);
	DDX_Control(pDX, IDC_EDIT_TEXT8, m_editLineText[7]);
	/*****/
	DDX_Control(pDX, IDC_RADIO_PHOTOID1, m_radioPhotoID1);
	DDX_Control(pDX, IDC_RADIO_PHOTOID2, m_radioPhotoID2);
	DDX_Control(pDX, IDC_RADIO_PHOTOID3, m_radioPhotoID3);
	DDX_Control(pDX, IDC_STATIC_LOYALTYUNCPATH1, m_staticLoyaltyUNCPath1);
	DDX_Control(pDX, IDC_STATIC_LOYALTYUNCPATH2, m_staticLoyaltyUNCPath2);
	DDX_Control(pDX, IDC_EDIT_LOYALTYUNCPATH1, m_editLoyaltyUNCPath1);
	DDX_Control(pDX, IDC_EDIT_LOYALTYUNCPATH2, m_editLoyaltyUNCPath2);
	DDX_Control(pDX, IDC_BUTTON_BROWSE2, m_buttonBrowse2);
	DDX_Control(pDX, IDC_BUTTON_TEST2, m_buttonTest2);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CServerPropertyPage2, CPropertyPage)

	ON_CBN_SELCHANGE(IDC_COMBO_FIELD1, OnSelectField1)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD2, OnSelectField2)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD3, OnSelectField3)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD4, OnSelectField4)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD5, OnSelectField5)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD6, OnSelectField6)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD7, OnSelectField7)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD8, OnSelectField8)
	/******/
	ON_BN_CLICKED(IDC_RADIO_PHOTOID1, OnRadioPhotoID1)
	ON_BN_CLICKED(IDC_RADIO_PHOTOID2, OnRadioPhotoID2)
	ON_BN_CLICKED(IDC_RADIO_PHOTOID3, OnRadioPhotoID3)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE2, OnButtonBrowse2)
	ON_BN_CLICKED(IDC_BUTTON_TEST2, OnButtonTest2)

END_MESSAGE_MAP()

//$$******************************************************************

BOOL CServerPropertyPage2::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	//pos display lines

	AddFieldName(Account::Blank.Label);
	AddFieldName(Account::SkipCheck.Label);
	AddFieldName(Account::Text.Label);
	AddFieldName(Account::DOBCheck.Label);
	AddFieldName(Account::AlertCheck.Label);
	AddFieldName(Account::BonusCheck.Label);
	AddFieldName(Account::UserID.Label);
	AddFieldName(Account::Fullname.Label);
	AddFieldName(Account::CardNameWithBalance.Label);
	AddFieldName(Account::Balance.Label);
	AddFieldName(Account::Purse1Balance.Label);
	AddFieldName(Account::Purse2Balance.Label);
	AddFieldName(Account::AvailableSpend.Label);
	AddFieldName(Account::Surname.Label);
	AddFieldName(Account::Points.Label);
	AddFieldName(Account::PointsToDate.Label);
	AddFieldName(Account::Purse1SpendToDate.Label);
	AddFieldName(Account::TotalSpendToDate.Label);
	AddFieldName(Account::StampOffers.Label);
	AddFieldName(Account::ExpiryDate.Label);
	AddFieldName(Account::Address1.Label);
	AddFieldName(Account::Address2.Label);
	AddFieldName(Account::Address3.Label);
	AddFieldName(Account::Address4.Label);
	AddFieldName(Account::Address5.Label);
	AddFieldName(Account::Phone1.Label);
	AddFieldName(Account::Phone2.Label);
	AddFieldName(Account::Info1.Label);
	AddFieldName(Account::Info2.Label);
	AddFieldName(Account::Info3.Label);
	AddFieldName(Account::Info4.Label);
	AddFieldName(Account::DOB.Label);

	for (int n = 1; n <= 8; n++)
	{
		CStandardCombos::SelectComboStringWithDefaultPos(m_comboLineType[n - 1], Server.GetPosDisplayField(n), 0);
		m_editLineText[n - 1].SetWindowText(Server.GetPosDisplayText(n));
	}

	//PHOTO ID PATHS
	switch (Server.GetPhotoIDFolderType())
	{
	case 1:
		OnRadioPhotoID2();
		break;

	case 2:
		OnRadioPhotoID3();
		break;

	case 0:
	default:
		OnRadioPhotoID1();
		break;
	}

	m_editLoyaltyUNCPath1.SetWindowText(Server.GetLoyaltyUNCSystemPath());
	m_editLoyaltyUNCPath2.SetWindowText(Server.GetLoyaltyUNCImagePath());
	MoveControl(&m_editLoyaltyUNCPath2, 20, 292);

	SetDisplay();

	return TRUE;
}

//$$******************************************************************

const char* CServerPropertyPage2::TextFromEditBox( CEdit& Edit )
{
	Edit.GetWindowText( m_strEditBoxText );
	return m_strEditBoxText;
}

//$$******************************************************************

const char* CServerPropertyPage2::TextFromComboBox( CComboBox& Combo )
{
	int nSel = Combo.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < Combo.GetCount() ) )
		Combo.GetLBText( nSel, m_strComboText );
	else
		m_strComboText = "";

	return m_strComboText;
}

//$$******************************************************************

BOOL CServerPropertyPage2::OnSetActive()
{
	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

BOOL CServerPropertyPage2::OnKillActive()
{
	for ( int n = 1; n <= 8; n++ )
	{
		Server.SetPosDisplayField( n, TextFromComboBox( m_comboLineType[n-1] ) );
		Server.SetPosDisplayText( n, TextFromEditBox( m_editLineText[n-1] ) );
	}

	Server.SetPhotoIDFolderType			( m_nPhotoIDFolderType );
	Server.SetLoyaltyUNCSystemPath		( TextFromEditBox( m_editLoyaltyUNCPath1 ) );
	Server.SetLoyaltyUNCImagePath		( TextFromEditBox( m_editLoyaltyUNCPath2 ) );

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

void CServerPropertyPage2::AddFieldName ( const char* szText ) 
{
	for (int n = 0; n < 8; n++)
	{
		m_comboLineType[n].AddString(szText);
	}
}

//$$******************************************************************

void CServerPropertyPage2::SetDisplay() 
{
	for (int n = 0; n < 8; n++)
	{
		EnableField(m_editLineText[n], TextFromComboBox(m_comboLineType[n]));
	}
}

//$$******************************************************************

void CServerPropertyPage2::EnableField ( CEdit& Edit, const char* szField ) 
{
	bool bEnable = TRUE;

	switch (LoyaltyHeaderHelpers.GetNumericField(szField))
	{
	case LOYALTYFIELD_BLANK:
	case LOYALTYFIELD_SKIP_CHECK:	
	case LOYALTYFIELD_STAMPOFFER:
		bEnable = FALSE;
		break;
	}

	Edit.EnableWindow(bEnable);
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CServerPropertyPage2::OnSelectField1()	{ SelectField(0); }
void CServerPropertyPage2::OnSelectField2()	{ SelectField(1); }
void CServerPropertyPage2::OnSelectField3()	{ SelectField(2); }
void CServerPropertyPage2::OnSelectField4()	{ SelectField(3); }
void CServerPropertyPage2::OnSelectField5()	{ SelectField(4); }
void CServerPropertyPage2::OnSelectField6()	{ SelectField(5); }
void CServerPropertyPage2::OnSelectField7()	{ SelectField(6); }
void CServerPropertyPage2::OnSelectField8()	{ SelectField(7); }

//$$******************************************************************

void CServerPropertyPage2::SelectField( int n )
{ 
	EnableField ( m_editLineText[n], TextFromComboBox( m_comboLineType[n] ) );
} 

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CServerPropertyPage2::OnRadioPhotoID1(){ SetPhotoIDFolderType(0); }
void CServerPropertyPage2::OnRadioPhotoID2(){ SetPhotoIDFolderType(1); }
void CServerPropertyPage2::OnRadioPhotoID3(){ SetPhotoIDFolderType(2); }

//$$******************************************************************

void CServerPropertyPage2::SetPhotoIDFolderType(int n)
{
	m_nPhotoIDFolderType = n;
	m_radioPhotoID1.SetCheck(0==n);
	m_radioPhotoID2.SetCheck(1==n);
	m_radioPhotoID3.SetCheck(2==n);
	m_staticLoyaltyUNCPath1.EnableWindow( n != 0 );
	m_staticLoyaltyUNCPath2.EnableWindow( n != 0 );
	m_buttonBrowse2.EnableWindow( n != 0 );
	m_buttonTest2.EnableWindow( n != 0 );
	ShowAndEnableWindow( &m_editLoyaltyUNCPath1, ( n != 2 ), ( 1 == n ) );
	ShowAndEnableWindow( &m_editLoyaltyUNCPath2, 2 == n );

	if ( 2 == n )
	{
		m_staticLoyaltyUNCPath1.SetWindowText( "Full network path to folder where images are stored." );
		m_staticLoyaltyUNCPath2.SetWindowText( "This path must be reachable from your back office PC and from the Sharp Pos terminal" );
	}
	else
	{
		m_staticLoyaltyUNCPath1.SetWindowText( "Full network path to Loyalty Manager's main data folder" );
		m_staticLoyaltyUNCPath2.SetWindowText( "Please do NOT include P47\\PhotoID in path, as it will be added automatically" );
	}
}

//$$******************************************************************

void CServerPropertyPage2::OnButtonBrowse2()
{
	CString strPathname = "";

	if ( m_nPhotoIDFolderType != 2 )
	{
		if ( BrowseFolder ( strPathname, "Locate Loyalty Manager's main data folder on the network",  NULL, this ) == TRUE )
		{
			m_editLoyaltyUNCPath1.SetWindowText( strPathname );
		}
	}
	else
	{
		if ( BrowseFolder ( strPathname, "Locate the network folder for Loyalty images",  NULL, this ) == TRUE )
		{
			m_editLoyaltyUNCPath2.SetWindowText( strPathname );
		}
	}
}

//$$******************************************************************

void CServerPropertyPage2::OnButtonTest2()
{
	CString strMsg = "";

	if ( m_nPhotoIDFolderType != 2 )
	{
		CString strPath = TextFromEditBox( m_editLoyaltyUNCPath1 );
		
		CString strPhotoIDFolder = "";
		strPhotoIDFolder.Format ( "%s\\%s\\PhotoID", 
			(const char*) strPath, 
			(const char*) Sysset.GetProgramFolder(SYSSET_LOYALTY4) );

		if ( ::ExistSubdirectory ( strPhotoIDFolder ) == FALSE )
		{
			strMsg.Format ( "No ' %s\\PhotoID ' folder found in specified Loyalty Manager's data folder!",
				(const char*) Sysset.GetProgramFolder(SYSSET_LOYALTY4) );
		}
		else
		{
			CString strMask = strPhotoIDFolder + "\\*.*";
			int nFileCount = ::CountFiles ( strMask );
			strMsg.Format ( "Path to Loyalty Manager's data folder is valid.\n\nNumber of files found in PhotoID folder = %d.", nFileCount );
		}
	}
	else
	{
		CString strPhotoIDFolder = TextFromEditBox( m_editLoyaltyUNCPath2 );

		if ( ::ExistSubdirectory ( strPhotoIDFolder ) == FALSE )
		{
			strMsg = "Unable to access the specified image folder";
		}
		else
		{
			CString strMask = strPhotoIDFolder + "\\*.*";
			int nFileCount = ::CountFiles ( strMask );
			strMsg.Format ( "The specific image folder is valid.\n\nNumber of files found in folder = %d.", nFileCount );
		}
	}

	Prompter.Info( strMsg, "Validation Check" );
}

//$$******************************************************************
