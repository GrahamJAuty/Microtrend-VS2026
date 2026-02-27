/**********************************************************************/
#include "CardLink.h"
#include "CashRemovedReport.h"
#include "CoinageReport.h"
#include "FailureReport.h"
#include "GlobalFunctions.h"
#include "ImageHandler.h"
#include "LoaderTestDlg.h"
#include "SetupDlg.h"
#include "SystemData.h"
#include "YesNoDlg.h"
/**********************************************************************/
#include "AboutDlg.h"
/**********************************************************************/
extern CSysset	Sysset;
extern const char* szVERSION_CARDLINK_ABOUT;
/**********************************************************************/

CAboutDlg::CAboutDlg (CWnd* pParent)
	: CSSColourDialog(CAboutDlg::IDD)
{
}

/**********************************************************************/

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSColourDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LIBS, m_comboLibs);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CAboutDlg, CSSColourDialog)
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_SETUP, OnButtonSetup)
	ON_BN_CLICKED(IDC_BUTTON_LICENCE, OnButtonLicence)
	ON_BN_CLICKED(IDC_BUTTON_TERMINATE, OnButtonTerminate)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_REPORT1, OnButtonReport1)
	ON_BN_CLICKED(IDC_BUTTON_REPORT2, OnButtonReport2)
	ON_BN_CLICKED(IDC_BUTTON_REPORT3, OnButtonReport3)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CAboutDlg::OnInitDialog()
{
	CSSColourDialog::OnInitDialog(MYCOLOUR_DIALOG, TRUE);

	CString strFilename = "";
	CImageHandler image(System.m_strSysFilesImagePath, "GeneralButton");
	if (image.GetFirst() == TRUE)
	{
		strFilename = image.GetFilepath();
	}

	m_staticCopyright.Init(IDC_STATIC_COPYRIGHT, this, BLACK, -1, "");

	m_staticVersion.Init(IDC_STATIC_VERSION, this, BLACK, -1, "");
	m_staticVersion.SetWindowText(Sysset.GetVersion(szVERSION_CARDLINK_ABOUT));

	CString strSerialNo = "";
	strSerialNo.Format("Serial No.  %6.6d", Sysset.GetSerialNo());
	m_staticSerialNo.Init(IDC_STATIC_SERIALNO, this, BLACK, -1, "");
	m_staticSerialNo.SetWindowText(strSerialNo);

	m_buttonDone.Init(IDOK, this, BLACK, MYCOLOUR_BUTTON, "", 10);
	m_buttonDone.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_buttonSetup.Init(IDC_BUTTON_SETUP, this, BLACK, MYCOLOUR_BUTTON, "", 10);
	m_buttonSetup.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_buttonLicence.Init(IDC_BUTTON_LICENCE, this, BLACK, MYCOLOUR_BUTTON, "", 10);
	m_buttonLicence.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_buttonTerminate.Init(IDC_BUTTON_TERMINATE, this, BLACK, MYCOLOUR_BUTTON, "", 10);
	m_buttonTerminate.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_buttonRefresh.Init(IDC_BUTTON_REFRESH, this, BLACK, MYCOLOUR_BUTTON, "", 10);
	m_buttonRefresh.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_buttonReport1.Init(IDC_BUTTON_REPORT1, this, BLACK, MYCOLOUR_BUTTON, "", 10);
	m_buttonReport1.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_buttonReport2.Init(IDC_BUTTON_REPORT2, this, BLACK, MYCOLOUR_BUTTON, "", 10);
	m_buttonReport2.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_buttonReport3.Init(IDC_BUTTON_REPORT3, this, BLACK, MYCOLOUR_BUTTON, "", 10);
	m_buttonReport3.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_buttonTest.Init(IDC_BUTTON_TEST, this, BLACK, MYCOLOUR_BUTTON, "", 10);
	m_buttonTest.SetImage(strFilename, TRANSPARENT_COLOUR);

	m_comboLibs.ShowWindow(SW_HIDE);
	m_comboLibs.AddString("Libraries");
	m_comboLibs.AddString(szVERSION_REPSUBS);
	m_comboLibs.AddString(szVERSION_DBASE);
	m_comboLibs.AddString(szVERSION_SUBS);
	m_comboLibs.AddString(szVERSION_SYSSET3);
	m_comboLibs.AddString(szVERSION_WINSUBS);
	m_comboLibs.AddString(szVERSION_EXP);
	m_comboLibs.AddString(szVERSION_SMARTUID);
	m_comboLibs.AddString(szVERSION_COMMS);
	m_comboLibs.SetCurSel(0);

	bool bShowReport1 = ::FileExists(System.m_strPendingLogFilename);		// card exceptions
	bool bShowReport2 = ::FileExists(System.m_strPayLinkLogFilename);		// Kiosk log
	bool bShowReport3 = ::FileExists(System.m_strPayLinkLogFilename);		// cash removed

	m_buttonReport1.ShowWindow(bShowReport1 ? SW_SHOW : SW_HIDE);
	m_buttonReport2.ShowWindow(bShowReport2 ? SW_SHOW : SW_HIDE);
	m_buttonReport3.ShowWindow(bShowReport3 ? SW_SHOW : SW_HIDE);

	SetDisplay();

	return TRUE;
}

/**********************************************************************/

void CAboutDlg::SetDisplay()
{
	switch (System.m_nInterfaceType)
	{
	case nINTERFACE_BIOMETRIC:
	case nINTERFACE_DUAL:
		m_buttonRefresh.ShowWindow(SW_SHOW);
		break;

	default:
		m_buttonRefresh.ShowWindow(SW_HIDE);
		break;
	}
}

//**********************************************************************

void CAboutDlg::OnButtonLicence() 
{
	Sysset.DisplayLicenceDetails ( "", "", 0, this );
}

//**********************************************************************

void CAboutDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_comboLibs.ShowWindow ( SW_SHOW );
	CDialog::OnRButtonDown(nFlags, point);
}

//*******************************************************************

void CAboutDlg::OnButtonSetup() 
{
	CSetupDlg dlg ( &System, this );
	if ( dlg.DoModal() == IDOK )
	{
		System.Write();
		SetDisplay();
	}
}

//*******************************************************************

void CAboutDlg::OnButtonTerminate()
{
	CYesNoDlg dlg ( "Confirm - Terminate Kiosk program", this );
	if ( dlg.DoModal() == IDOK )
	{
		if ( IDYES == dlg.m_nReply )
		{
			m_bTerminateReqd = TRUE;
			CDialog::OnOK();
		}
	}
}

//*******************************************************************

void CAboutDlg::OnButtonRefresh()
{
	CYesNoDlg dlg ( "Confirm - Refresh biometric readers database", this );
	if ( dlg.DoModal() == IDOK )
	{
		if ( IDYES == dlg.m_nReply )
		{
			CString strMsg = System.RefreshBiometrics() ? "Refresh was successful" : "Refresh FAILED";
			::DisplayMessage ( this, strMsg, "Refresh Biometrics", MB_ICONINFORMATION );
		}
	}
}

//*******************************************************************

void CAboutDlg::OnButtonReport1()
{
	CFailureReport report(System.m_strPendingLogFilename);

	if (report.Create() == TRUE)
	{
		report.Display(this);
	}
}

//*******************************************************************

void CAboutDlg::OnButtonReport2()
{
	CCoinageReport report(System.m_strPayLinkLogFilename);

	if (report.Create() == TRUE)
	{
		report.Display(this);
	}
}

//*******************************************************************

void CAboutDlg::OnButtonReport3()
{
	CCashRemovedReport report(System.m_strPayLinkLogFilename);

	if (report.Create() == TRUE)
	{
		report.Display(this);
	}
}

//*******************************************************************

void CAboutDlg::OnButtonTest()
{
	CLoaderTestDlg dlg ( this );
	dlg.DoModal();
}

//*******************************************************************
