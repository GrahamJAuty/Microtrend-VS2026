//$$******************************************************************
#include "AutoShutdownStacker.h"
#include "ServerPropertySheet.h"
#include "SmartPay4Manager.h"
//$$******************************************************************
#include "ServerPropertyPage2.h"
//$$******************************************************************
extern CSysset Sysset;
//$$******************************************************************

CServerPropertyPage2::CServerPropertyPage2()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CServerPropertyPage2::IDD)
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
	/*****/
	DDX_Control(pDX, IDC_RADIO_PHOTOID1, m_radioPhotoID1);
	DDX_Control(pDX, IDC_RADIO_PHOTOID2, m_radioPhotoID2);
	DDX_Control(pDX, IDC_RADIO_PHOTOID3, m_radioPhotoID3);
	DDX_Control(pDX, IDC_STATIC_SMARTPAYUNCPATH1, m_staticSmartPayUNCPath1);
	DDX_Control(pDX, IDC_STATIC_SMARTPAYUNCPATH2, m_staticSmartPayUNCPath2);
	DDX_Control(pDX, IDC_EDIT_SMARTPAYUNCPATH1, m_editSmartPayUNCPath1);
	DDX_Control(pDX, IDC_EDIT_SMARTPAYUNCPATH2, m_editSmartPayUNCPath2);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
	DDX_Control(pDX, IDC_BUTTON_TEST, m_buttonTest);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CServerPropertyPage2, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED(IDC_RADIO_PHOTOID1, OnRadioPhotoID1)
	ON_BN_CLICKED(IDC_RADIO_PHOTOID2, OnRadioPhotoID2)
	ON_BN_CLICKED(IDC_RADIO_PHOTOID3, OnRadioPhotoID3)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CServerPropertyPage2::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	//PHOTO ID PATHS
	switch( Server.GetPhotoIDFolderType() )
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

	m_editSmartPayUNCPath1.SetWindowText( Server.GetSmartPayUNCSystemPath() );
	m_editSmartPayUNCPath2.SetWindowText( Server.GetSmartPayUNCImagePath() );
	MoveControl( &m_editSmartPayUNCPath2, 24, 97 );

	return TRUE;
}

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
	m_staticSmartPayUNCPath1.EnableWindow( n != 0 );
	m_staticSmartPayUNCPath2.EnableWindow( n != 0 );
	m_buttonBrowse.EnableWindow( n != 0 );
	m_buttonTest.EnableWindow( n != 0 );
	ShowAndEnableWindow( &m_editSmartPayUNCPath1, ( n != 2 ), ( 1 == n ) );
	ShowAndEnableWindow( &m_editSmartPayUNCPath2, 2 == n );

	if ( 2 == n )
	{
		m_staticSmartPayUNCPath1.SetWindowText( "Full network path to folder where images are stored." );
		m_staticSmartPayUNCPath2.SetWindowText( "This path must be reachable from your back office PC and from the Sharp Pos terminal" );
	}
	else
	{
		m_staticSmartPayUNCPath1.SetWindowText( "Full network path to SmartPay Manager's main data folder" );
		m_staticSmartPayUNCPath2.SetWindowText( "Please do NOT include P44\\PhotoID in path, as it will be added automatically" );
	}
}

//$$******************************************************************

void CServerPropertyPage2::OnButtonBrowse()
{
	CString strPathname = "";

	if ( m_nPhotoIDFolderType != 2 )
	{
		if ( DoBrowseFolder ( strPathname, "Locate SmartPay Manager's main data folder on the network",  NULL, this ) == TRUE )
		{
			m_editSmartPayUNCPath1.SetWindowText( strPathname );
		}
	}
	else
	{
		if ( DoBrowseFolder ( strPathname, "Locate the network folder for SmartPay images",  NULL, this ) == TRUE )
		{
			m_editSmartPayUNCPath2.SetWindowText( strPathname );
		}
	}
}

//$$******************************************************************

void CServerPropertyPage2::OnButtonTest()
{
	CString strMsg = "";

	if ( m_nPhotoIDFolderType != 2 )
	{
		CString strPath = GetEditBoxText( m_editSmartPayUNCPath1 );
		
		CString strPhotoIDFolder = "";
		strPhotoIDFolder.Format ( "%s\\%s\\PhotoID", 
			(const char*) strPath, 
			(const char*) Sysset.GetProgramFolder(SYSSET_SMARTPAY4) );

		if ( ::ExistSubdirectory ( strPhotoIDFolder ) == FALSE )
		{
			strMsg.Format ( "No ' %s\\PhotoID ' folder found in specified SmartPay Manager's data folder!", 
				(const char*) Sysset.GetProgramFolder(SYSSET_SMARTPAY4) );
		}
		else
		{
			CString strMask = strPhotoIDFolder + "\\*.*";
			int nFileCount = ::CountFiles ( strMask );
			strMsg.Format ( "Path to SmartPay Manager's data folder is valid.\n\nNumber of files found in PhotoID folder = %d.", 
				nFileCount );
		}
	}
	else
	{
		CString strPhotoIDFolder = GetEditBoxText( m_editSmartPayUNCPath2 );

		if ( ::ExistSubdirectory ( strPhotoIDFolder ) == FALSE )
		{
			strMsg = "Unable to access the specified image folder";
		}
		else
		{
			CString strMask = strPhotoIDFolder + "\\*.*";
			int nFileCount = ::CountFiles ( strMask );
			strMsg.Format ( "The specified image folder is valid.\n\nNumber of files found in folder = %d.", 
				nFileCount );
		}
	}

	Prompter.Info( strMsg, "Validation Check" );	
}

//$$******************************************************************

BOOL CServerPropertyPage2::OnKillActive()
{
	Server.SetPhotoIDFolderType		( m_nPhotoIDFolderType );
	Server.SetSmartPayUNCSystemPath	( GetEditBoxText( m_editSmartPayUNCPath1 ) );
	Server.SetSmartPayUNCImagePath	( GetEditBoxText( m_editSmartPayUNCPath2 ) );
	return CPropertyPage::OnKillActive();
}

//$$******************************************************************
