//*******************************************************************
#include "..\SmartPay4Shared\WaitDlg.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\WebPaymentExport.h"
#include "..\SmartPay4ManagerBgnd\WisePay.hpp"
//*******************************************************************
#include "ReportCSVFile.h"
#include "CardPickerDlg.h"
#include "ImportWisePayFile.h"
//*******************************************************************
#include "AccountLinkWisePayDlg.h"
//*******************************************************************

extern const char* szINTERNETMESSAGE;				// "Accessing WebPayment internet site";

//*******************************************************************

CAccountLinkWisePayDlg::CAccountLinkWisePayDlg(CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CAccountLinkWisePayDlg::IDD, pParent)
{
	m_pParent = pParent;
		
	m_dataWisePay.Read();
	m_strImportFilename = m_dataWisePay.GetImportDinerDataFilename();	// diner data import
	m_strBalanceListFile = m_dataWisePay.GetBalanceListFilename();		// select balance listfile
		
	m_strLoginReply = System.GetWebPaymentPath("accounts.xml");
}

//*******************************************************************

CAccountLinkWisePayDlg::~CAccountLinkWisePayDlg()
{
}

//*******************************************************************

void CAccountLinkWisePayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_START, m_buttonStart);
	DDX_Control(pDX, IDC_BUTTON_RAW, m_buttonRaw);
	DDX_Control(pDX, IDC_BUTTON_DISPLAY, m_buttonDisplay);
	DDX_Control(pDX, IDC_BUTTON_IMPORT, m_buttonImport);
	DDX_Control(pDX, IDC_BUTTON_BALANCES, m_buttonBalances);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CAccountLinkWisePayDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_RAW, OnButtonRaw)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, OnButtonDisplay)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_BALANCES, OnButtonBalances)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
END_MESSAGE_MAP()

//*******************************************************************
// CWebPaymentAccountImportDlg message handlers

BOOL CAccountLinkWisePayDlg::CSSAutoShutdownPostInitDialog()
{
	SetDisplay();
	m_buttonAltKey1.ShowWindow(SW_HIDE);
	return TRUE;
}

//*******************************************************************

BOOL CAccountLinkWisePayDlg::DestroyWindow()
{
	CFileRecycler::SendToRecycleBin ( m_strImportFilename );		// delete file if still about
	CFileRecycler::SendToRecycleBin ( m_strLoginReply );			// delete xml login file

	return CSSAutoShutdownDialog::DestroyWindow();
}

//*******************************************************************

void CAccountLinkWisePayDlg::SetDisplay()
{
	bool bAllowed = ::FileExists(m_strImportFilename);

	m_buttonStart.EnableWindow(!bAllowed);
	m_buttonRaw.EnableWindow(bAllowed);
	m_buttonDisplay.EnableWindow(bAllowed);
	m_buttonImport.EnableWindow(bAllowed);
}

//*******************************************************************
// access relevant web site
//*******************************************************************

void CAccountLinkWisePayDlg::OnButtonStart()
{
	CString strError = AccessWisePay();	
	
	if (strError != "")
	{
		Prompter.Error(strError, "Access WisePay Web site");
	}

	SetDisplay();
}

//*******************************************************************
// Get WisePay Accounts

CString CAccountLinkWisePayDlg::AccessWisePay()
{
	CWaitDlg dlgWait ( m_pParent );
	dlgWait.SetDlgItemText ( IDC_STATIC_MESSAGE, szINTERNETMESSAGE );

	CSSWisePay web ( &m_dataWisePay );
	CString strError = web.GetChildAccounts ( m_strLoginReply, m_strImportFilename );

	return strError;
}

//*******************************************************************
// Display raw csv file in csv format

void CAccountLinkWisePayDlg::OnButtonRaw()
{
	CCSVFileReport report ( "R216", m_strImportFilename, this );
	report.Show ( "Raw Import File");
}

//*******************************************************************
// display import file as per webpayment provider

void CAccountLinkWisePayDlg::OnButtonDisplay()
{
	CImportWisePayFile pp ( &m_dataWisePay, this );
	pp.ShowImports();
}

//*******************************************************************
// process imported data
//*******************************************************************

void CAccountLinkWisePayDlg::OnButtonImport()
{
	CString strTitle = "Import WisePay site data";

	CStringArray Warning;
	Warning.Add(strTitle);
	Warning.Add("WisePay data will be used to update the account database.");

	if (Prompter.Warning(Warning) == IDNO)
	{
		return;
	}

	CString strError = ImportWisePay();

	if (strError != "")
	{
		Prompter.Error(strError, strTitle);
	}

	SetDisplay();
}

//*******************************************************************
// process WisePay imported data

CString CAccountLinkWisePayDlg::ImportWisePay()
{
	CString strError = "";

	CImportWisePayFile pp ( &m_dataWisePay, this );
	
	if (pp.Import() == TRUE)
	{
		pp.ShowStatistics();					// show import stats
	}
	else
	{
		strError = pp.GetError();
	}

	return strError;
}

//*******************************************************************

void CAccountLinkWisePayDlg::OnButtonAltKey1()
{
	m_buttonBalances.ShowWindow(SW_SHOW);
}

//*******************************************************************

void CAccountLinkWisePayDlg::OnButtonBalances()
{
	CArray<CSQLRowAccountList, CSQLRowAccountList> arrayDummy;
	CCardPickerDlg dlg("Starting Balances", m_strBalanceListFile, arrayDummy, m_pParent);

	dlg.m_bMembershipList = TRUE;
	if (dlg.DoModal() == IDOK)
	{
		CWebPaymentExport web(this);
		web.SendOpeningBalanceList(m_strBalanceListFile);
	}
}

//*******************************************************************

