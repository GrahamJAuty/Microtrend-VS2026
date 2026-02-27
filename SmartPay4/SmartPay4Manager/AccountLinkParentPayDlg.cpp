//*******************************************************************
#include "..\SmartPay4Shared\WaitDlg.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\ParentPay.hpp"
#include "..\SmartPay4ManagerBgnd\ParentPayExport.h"
#include "..\SmartPay4ManagerBgnd\WebPaymentExport.h"
#include "..\SmartPay4ManagerBgnd\WebPaymentExportCsvLog.h"
//*******************************************************************
#include "ReportCSVFile.h"
#include "CardPickerDlg.h"
#include "ImportParentPayFile.h"
//*******************************************************************
#include "AccountLinkParentPayDlg.h"
//*******************************************************************

extern const char* szINTERNETMESSAGE;				// "Accessing WebPayment internet site";

//*******************************************************************

CAccountLinkParentPayDlg::CAccountLinkParentPayDlg(CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CAccountLinkParentPayDlg::IDD, pParent)
{
	m_pParent = pParent;
	m_bForceMatchAccounts = FALSE;
	
	m_dataParentPay.Read();
	m_strImportFilename = m_dataParentPay.GetImportDataFilename();		// diner data import
	m_bForceMatchAccounts = (m_dataParentPay.m_nLastDinerSnapshotID == 0) ? TRUE : FALSE;
	m_strBalanceListFile = m_dataParentPay.GetBalanceListFilename();		// select balance listfile
	m_strDinerMatchListFile = m_dataParentPay.GetDinerMatchListFilename();	// select diner match listfile

	m_strLoginReply = System.GetWebPaymentPath("accounts.xml");
}

//*******************************************************************

CAccountLinkParentPayDlg::~CAccountLinkParentPayDlg()
{
}

//*******************************************************************

void CAccountLinkParentPayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_START, m_buttonStart);
	DDX_Control(pDX, IDC_BUTTON_RAW, m_buttonRaw);
	DDX_Control(pDX, IDC_BUTTON_DISPLAY, m_buttonDisplay);
	DDX_Control(pDX, IDC_BUTTON_IMPORT, m_buttonImport);
	DDX_Control(pDX, IDC_BUTTON_BALANCES, m_buttonBalances);
	DDX_Control(pDX, IDC_BUTTON_DINERMATCH, m_buttonDinerMatch);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CAccountLinkParentPayDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_RAW, OnButtonRaw)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, OnButtonDisplay)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_BALANCES, OnButtonBalances)
	ON_BN_CLICKED(IDC_BUTTON_DINERMATCH, OnButtonDinerMatch)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CAccountLinkParentPayDlg::CSSAutoShutdownPostInitDialog()
{
	m_buttonDinerMatch.ShowWindow ( SW_HIDE );
	SetDisplay();
	m_buttonAltKey1.ShowWindow(SW_HIDE);
	return TRUE;
}

//*******************************************************************

BOOL CAccountLinkParentPayDlg::DestroyWindow()
{
	CFileRecycler::SendToRecycleBin ( m_strImportFilename );		// delete file if still about
	CFileRecycler::SendToRecycleBin ( m_strLoginReply );			// delete xml login file

	return CSSAutoShutdownDialog::DestroyWindow();
}

//*******************************************************************

void CAccountLinkParentPayDlg::SetDisplay()
{
	bool bAllowed = ::FileExists(m_strImportFilename);;

	m_buttonStart.EnableWindow(!bAllowed);
	m_buttonRaw.EnableWindow(bAllowed);
	m_buttonDisplay.EnableWindow(bAllowed);
	m_buttonImport.EnableWindow(bAllowed);
}

//*******************************************************************
// access relevant web site
//*******************************************************************

void CAccountLinkParentPayDlg::OnButtonStart()
{
	CString strError = AccessParentPay();

	if (strError != "")
	{
		Prompter.Error(strError, "Access Parent Pay API");
	}

	SetDisplay();
}

//*******************************************************************
// Get ParentPay Diner data

CString CAccountLinkParentPayDlg::AccessParentPay()
{
	CString strError = "";

	{
		CWaitDlg dlgWait ( m_pParent );
		dlgWait.SetDlgItemText ( IDC_STATIC_MESSAGE, szINTERNETMESSAGE );

		CSSParentPay pp ( &m_dataParentPay );
		strError = pp.RequestDinerData ( m_strLoginReply, m_strImportFilename );
	}

	if ( strError == "" )
	{
		CImportParentPayFile ippf ( &m_dataParentPay, this );
		if ( ippf.ValidateFile() == FALSE )
		{
			strError = "No new diner details have been received";
			CFileRecycler::SendToRecycleBin ( m_strImportFilename );
		}
	}

	return strError;
}

//*******************************************************************

// Display raw csv file in csv format

void CAccountLinkParentPayDlg::OnButtonRaw()
{
	CCSVFileReport report ( "R216", m_strImportFilename, this );
	report.Show ( "Raw Import File");
}

//*******************************************************************
// display import file as per webpayment provider

void CAccountLinkParentPayDlg::OnButtonDisplay()
{
	CImportParentPayFile pp(&m_dataParentPay, this);
	pp.ShowImports();
}

//*******************************************************************
// process imported data
//*******************************************************************

void CAccountLinkParentPayDlg::OnButtonImport()
{
	CString strTitle = "Import ParentPay site data";
	
	CStringArray Warning;
	Warning.Add ( "Import ParentPay site data" );
	Warning.Add("ParentPay data will be used to update the account database.");

	{
		if (m_bForceMatchAccounts == TRUE)
		{
			Warning.Add("All accounts will send match information to ParentPay.");
		}
		else
		{
			Warning.Add("Newly matched accounts will send match information to ParentPay.");
		}
	}

	if (Prompter.Warning(Warning) == IDNO)
	{
		return;
	}

	CString strError = ImportParentPay();

	if (strError != "")						
	{
		Prompter.Error(strError, strTitle);
	}
	
	SetDisplay();
}

//*******************************************************************
// process ParentPay imported data / Echo back any new updates

CString CAccountLinkParentPayDlg::ImportParentPay()
{
	CString strError = "";

	CImportParentPayFile pp ( &m_dataParentPay, this );
	pp.m_bForceMatchAccount = m_bForceMatchAccounts;					// set force account match flag

	if ( pp.Import() == TRUE )
	{
		pp.ShowStatistics();											// show import stats

		CString strFilename = m_dataParentPay.GetDinerMatchAdviceFilename();
		if ( ::FileExists ( strFilename ) == TRUE )
		{
			CParentPayData data;
			data.Read();

			CParentPayExport ppExport ( &data, this );
			ppExport.SendDinerMatch ( strFilename );
		}
	}
	else
	{
		strError = pp.GetError();
	}

	return strError;
}

//*******************************************************************

void CAccountLinkParentPayDlg::OnButtonAltKey1()
{
	m_buttonBalances.ShowWindow(SW_SHOW);
	m_buttonDinerMatch.ShowWindow(SW_SHOW);
}

//*******************************************************************

void CAccountLinkParentPayDlg::OnButtonBalances()
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

void CAccountLinkParentPayDlg::OnButtonDinerMatch()
{
	if (System.GetWebPaymentType() != nWEBPAYMENT_PEBBLE)
	{
		CArray<CSQLRowAccountList, CSQLRowAccountList> arrayDummy;
		CCardPickerDlg dlg("Diner Match", m_strDinerMatchListFile, arrayDummy, m_pParent);
		dlg.m_bMembershipList = TRUE;
		if (dlg.DoModal() == IDOK)
		{
			CWebPaymentExport web(this);
			web.SendDinerMatchList(m_strDinerMatchListFile);
		}
	}
}

//*******************************************************************
