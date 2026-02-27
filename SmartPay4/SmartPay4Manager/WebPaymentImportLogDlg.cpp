//$$******************************************************************
#include "..\SmartPay4Shared\SmartPayLockManager.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLResultInfo.h"
#include "..\SmartPay4Shared\SQLTable_BromComPendingOrder\SQLRepositoryBromComPendingOrder.h"
#include "..\SmartPay4Shared\SQLTable_PebblePendingTransaction\SQLRepositoryPebblePendingTransaction.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\ParentMailData.h"
#include "..\SmartPay4ManagerBgnd\ParentPayData.h"
#include "..\SmartPay4ManagerBgnd\ParentPayExport.h"
#include "..\SmartPay4ManagerBgnd\SchoolCommsData.h"
#include "..\SmartPay4ManagerBgnd\SquidData.h"
#include "..\SmartPay4ManagerBgnd\TucasiJSONData.h"
#include "..\SmartPay4ManagerBgnd\TucasiJSONExport.h"
#include "..\SmartPay4ManagerBgnd\TucasiSOAPData.h"
#include "..\SmartPay4ManagerBgnd\WebPaymentExportCsvLog.h"
#include "..\SmartPay4ManagerBgnd\WisePayData.h"
#include "..\SmartPay4ManagerBgnd\WebPaymentImport.h"
//$$******************************************************************
#include "BromComPaymentExceptionReport.h"
#include "PebblePaymentExceptionReport.h"
#include "ReportCSVFile.h"
#include "WebPaymentListReport.h"
#include "WebPaymentExceptionReport.h"
#include "WebPaymentExportTxReport.h"
#include "WebPaymentReporter.h"
//$$******************************************************************
#include "WebPaymentImportLogDlg.h"
//$$******************************************************************

static const int nYEARLEN	= 4;
static const int nMONTHLEN	= 2;

//$$******************************************************************

#define nRXX_PAYMENTS			1			// 1 valid imports
#define nRXX_EXCEPTIONS			2			// 2 import exceptions
#define nTXX_SUMMARY			3			// 3 export summary
#define nTXX_TRANSACTIONS		4			// 4 export transaction log (Diner Service)
#define nTXX_TRANSACTIONS2		5			// 5 export transaction log (2nd Service2)
#define nTXX_BALANCE			6			// 6 export balance log
#define nTXX_ECHOPAYMENT		7			// 7 payment echo log
#define nTXX_ACCOUNTMATCH		8			// 8 Account echo log
#define nTXX_SENTTRANSACTIONS	9			// 9 Account echo log
#define nMAX_TYPENO				9

//$$******************************************************************

CWebPaymentImportLogDlg::CWebPaymentImportLogDlg(int nWebPaymentType, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CWebPaymentImportLogDlg::IDD, pParent)
{
	m_nWebPaymentType = nWebPaymentType;
	m_strView1File = "";
	m_strView2File = "";
	m_strView3File = "";

	// detailed transaction Export report type
	m_nView3Type = nTXX_TRANSACTIONS;

	switch (System.GetWebPaymentType())
	{
	case nWEBPAYMENT_PEBBLE:
		m_strView3TransTitle = "Pebble Export Transaction Log";
		break;

	default:
		m_strView3TransTitle.Format("%s Web Payment Export Transaction Log",
			(const char*)System.GetWebPaymentDisplayName(m_nWebPaymentType));
		break;
	}

	// Listing report1 button text
	m_strReportButtonList1 = "Listing";
	m_strRetryButtonList1 = m_strReportButtonList1;
	m_bResendAllowed = FALSE;

	// Listing report2 button text
	m_strReportButtonList2 = "";
	m_strWebPaymentListFilename1 = "";
	m_strWebPaymentExceptionFilename1 = "";

	m_strWebPaymentListFilename2 = "";
	m_strWebPaymentExceptionFilename2 = "";

	switch (m_nWebPaymentType)
	{
	case nWEBPAYMENT_PARENTPAY:
	{
		m_bResendAllowed = TRUE;						// resend already exported lines allowed 

		CParentPayData data;
		data.Read();
		m_strWebPaymentListFilename1 = data.GetWebPaymentListFilename(1);
		m_strWebPaymentExceptionFilename1 = data.GetWebPaymentExceptionFilename(1);
		m_strWebPaymentListFilename2 = data.GetWebPaymentListFilename(2);
		m_strWebPaymentExceptionFilename2 = data.GetWebPaymentExceptionFilename(2);

		m_strReportButtonList1 = (data.m_nAccountID1 == 0) ? "" : "Service 1";
		m_strReportButtonList2 = (data.m_nAccountID2 == 0) ? "" : "Service 2";
		m_strRetryButtonList1 = m_strReportButtonList1;
	}
	break;

	case nWEBPAYMENT_TUCASI:
	{
		CTucasiSoapData data;
		data.Read();
		m_strWebPaymentListFilename1 = data.GetWebPaymentListFilename();
		m_strWebPaymentExceptionFilename1 = data.GetWebPaymentExceptionFilename();
	}
	break;

	case nWEBPAYMENT_WISEPAY:
	{
		CWisePayData data;
		data.Read();
		m_strWebPaymentListFilename1 = data.GetWebPaymentListFilename(1);
		m_strWebPaymentListFilename2 = data.GetWebPaymentListFilename(2);
		m_strWebPaymentExceptionFilename1 = data.GetWebPaymentExceptionFilename(1);
		m_strWebPaymentExceptionFilename2 = data.GetWebPaymentExceptionFilename(2);

		m_strReportButtonList1 = (data.m_nAccountID1 == 0) ? "" : "Service 1";
		m_strReportButtonList2 = (data.m_nAccountID2 == 0) ? "" : "Service 2";
		m_strRetryButtonList1 = m_strReportButtonList1;
	}
	break;

	case nWEBPAYMENT_SCHOOLCOMMS:
	{
		CSchoolcommsData data;
		data.Read();
		m_strWebPaymentListFilename1 = data.GetWebPaymentListFilename();
		m_strWebPaymentExceptionFilename1 = data.GetWebPaymentExceptionFilename();
	}
	break;

	case nWEBPAYMENT_BROMCOM:
	case nWEBPAYMENT_PEBBLE:
		m_strReportButtonList1 = "";		
		break;
		
	case nWEBPAYMENT_TUCASIv2:
	{
		m_bResendAllowed = TRUE;						// resend already exported lines allowed 

		CTucasiJsonData data;
		data.Read();
		m_strWebPaymentListFilename1 = data.GetWebPaymentListFilename();
		m_strWebPaymentExceptionFilename1 = data.GetWebPaymentExceptionFilename();
	}
	break;

	case nWEBPAYMENT_SQUID:
	{
		CSquidData data;
		data.Read();
		m_strWebPaymentListFilename1 = data.GetWebPaymentListFilename();
		m_strWebPaymentExceptionFilename1 = data.GetWebPaymentExceptionFilename();
	}
	break;

	case nWEBPAYMENT_PARENTMAIL:
	{
		CParentMailData data;
		data.Read();
		m_strWebPaymentListFilename1 = data.GetWebPaymentListFilename();
		m_strWebPaymentExceptionFilename1 = data.GetWebPaymentExceptionFilename();
	}
	break;

	case nWEBPAYMENT_EXPORTSALES:
	{
		m_strView3TransTitle = "Export Sales Log";
		m_strReportButtonList1 = "";								// no Listing report button
		m_strRetryButtonList1 = "";
	}
	break;
	}
}

//$$******************************************************************

void CWebPaymentImportLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_RETRYREPORT, m_buttonRetryReport);
	DDX_Control(pDX, IDC_BUTTON_RETRYREPORT2, m_buttonRetryReport2);
	DDX_Control(pDX, IDC_BUTTON_DELETEVIEW1, m_buttonDeleteView1);
	DDX_Control(pDX, IDC_BUTTON_DELETEVIEW2, m_buttonDeleteView2);
	DDX_Control(pDX, IDC_BUTTON_DELETEVIEW3, m_buttonDeleteView3);
	DDX_Control(pDX, IDC_BUTTON_OTHER, m_buttonOther);
	DDX_Control(pDX, IDC_BUTTON_REPORT1, m_buttonReport1);
	DDX_Control(pDX, IDC_BUTTON_REPORT2, m_buttonReport2);
	DDX_Control(pDX, IDC_BUTTON_RESEND, m_buttonResend);
	DDX_Control(pDX, IDC_BUTTON_RETRY, m_buttonRetry);
	DDX_Control(pDX, IDC_BUTTON_VIEW1, m_buttonView1);
	DDX_Control(pDX, IDC_BUTTON_VIEW2, m_buttonView2);
	DDX_Control(pDX, IDC_BUTTON_VIEW3, m_buttonView3);
	DDX_Control(pDX, IDC_BUTTON_VIEW4, m_buttonView4);
	DDX_Control(pDX, IDC_STATIC_EXPORTBOX, m_staticExportBox);
	DDX_Control(pDX, IDC_LIST_VIEW1, m_listboxView1);
	DDX_Control(pDX, IDC_LIST_VIEW2, m_listboxView2);
	DDX_Control(pDX, IDC_LIST_VIEW3, m_listboxView3);
	DDX_Control(pDX, IDC_STATIC_PAYMENTS, m_staticPayments);
	DDX_LBString(pDX, IDC_LIST_VIEW1, m_strView1File);
	DDX_LBString(pDX, IDC_LIST_VIEW2, m_strView2File);
	DDX_LBString(pDX, IDC_LIST_VIEW3, m_strView3File);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CWebPaymentImportLogDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_VIEW1, OnButtonView1)
	ON_BN_CLICKED(IDC_BUTTON_VIEW2, OnButtonView2)
	ON_BN_CLICKED(IDC_BUTTON_VIEW3, OnButtonView3)
	ON_BN_CLICKED(IDC_BUTTON_VIEW4, OnButtonView4)
	ON_BN_CLICKED(IDC_BUTTON_DELETEVIEW1, OnButtonDeleteView1)
	ON_BN_CLICKED(IDC_BUTTON_DELETEVIEW2, OnButtonDeleteView2)
	ON_BN_CLICKED(IDC_BUTTON_DELETEVIEW3, OnButtonDeleteView3)
	ON_BN_CLICKED(IDC_BUTTON_REPORT1, OnButtonReport1)
	ON_BN_CLICKED(IDC_BUTTON_REPORT2, OnButtonReport2)
	ON_BN_CLICKED(IDC_BUTTON_RETRYREPORT, OnButtonRetryReport)
	ON_BN_CLICKED(IDC_BUTTON_RETRYREPORT2, OnButtonRetryReport2)
	ON_BN_CLICKED(IDC_BUTTON_RETRY, OnButtonRetry)
	ON_LBN_DBLCLK(IDC_LIST_VIEW1, OnDoubleClickListView1)
	ON_LBN_DBLCLK(IDC_LIST_VIEW2, OnDoubleClickListView2)
	ON_LBN_DBLCLK(IDC_LIST_VIEW3, OnDoubleClickListView3)
	ON_BN_CLICKED(IDC_BUTTON_OTHER, OnButtonOther)
	ON_BN_CLICKED(IDC_BUTTON_RESEND, OnButtonResend)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CWebPaymentImportLogDlg::CSSAutoShutdownPostInitDialog()
{	
	{
		CString strTitle = "";

		switch (m_nWebPaymentType)
		{
		case nWEBPAYMENT_EXPORTSALES:
			strTitle = "Export Sales Logs";
			break;

		case nWEBPAYMENT_PEBBLE:
			strTitle = "Pebble Transaction Import Exceptions and Logs";
			m_staticPayments.SetWindowText("Imported Transactions");
			break;

		default:
			strTitle.Format("%s Web Payment Exceptions and Logs",
				(const char*)System.GetWebPaymentDisplayName());
			break;
		}

		SetWindowText(strTitle);
	}

	int nCount = 0;
	if ( ( nCount = AddLogFiles ( nRXX_PAYMENTS ) ) == 0 )					// add import logs
	{
		m_buttonView1.EnableWindow(FALSE);
		m_buttonDeleteView1.EnableWindow(FALSE);
	}
	else
	{
		m_listboxView1.SetCurSel(nCount - 1);
	}

	if ( ( nCount = AddLogFiles ( nRXX_EXCEPTIONS ) ) == 0 )
	{
		m_buttonView2.EnableWindow(FALSE);
		m_buttonDeleteView2.EnableWindow(FALSE);
	}
	else
	{
		m_listboxView2.SetCurSel(nCount - 1);
	}

	if ( ( nCount = AddLogFiles ( m_nView3Type ) ) == 0 )
	{
		m_buttonView3.EnableWindow(FALSE);
		m_buttonDeleteView3.EnableWindow(FALSE);
	}
	else
	{
		m_listboxView3.SetCurSel(nCount - 1);
	}

	if (::FileExists(Filenames.GetWebPaymentStopIndicator()) == FALSE)
	{
		m_buttonView4.ShowWindow(SW_HIDE);
	}

// listing report buttons

	if (m_strReportButtonList1 == "")
	{
		m_buttonReport1.ShowWindow(SW_HIDE);
	}
	else
	{
		m_buttonReport1.SetWindowText ( m_strReportButtonList1 );
		m_buttonRetryReport.SetWindowText ( m_strRetryButtonList1 );
	}

	if (m_strReportButtonList2 == "")
	{
		m_buttonReport2.ShowWindow(SW_HIDE);
	}
	else
	{
		m_buttonReport2.SetWindowText ( m_strReportButtonList2 );
		m_buttonRetryReport2.SetWindowText ( m_strReportButtonList2 );
	}

	SetExceptionButtons();

	m_staticExportBox.SetWindowText ( GetExportsBoxTitle() );

	SetExportsMoreButton();

	if (m_bResendAllowed == FALSE)
	{
		m_buttonResend.ShowWindow(SW_HIDE);
	}

	return TRUE;
}

//$$******************************************************************

int CWebPaymentImportLogDlg::AddLogFiles(int nType)
{
	if (m_nWebPaymentType == nWEBPAYMENT_WISEPAY)
	{
		if (nType == nTXX_BALANCE)		// no longer used all goes thru Transactions 1& 2
		{
			return 0;
		}
	}

	// Inyyyymm.dat / Exyyyynn.dat / Txyyyymm.dat / Tnyyyymm.dat / Tzyyyymm.dat / Tayyyymm.dat
	CString strMask = "";
	strMask.Format("%s??????.dat",
		(const char*) GetFilePrefix(nType));

	CFileFind FileFinder;
	CString strMaskPath = System.GetWebPaymentPath(strMask, m_nWebPaymentType);

	BOOL bWorking = FileFinder.FindFile(strMaskPath);
	int nCount = 0;

	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();
		if (FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE)
		{
			continue;
		}

		if (AddToList(nType, FileFinder.GetFileTitle()) == TRUE)
		{
			++nCount;
		}
	}

	return nCount;
}

//$$******************************************************************

bool CWebPaymentImportLogDlg::AddToList ( int nType, const char* szFileTitle )
{
	CString strFileTitle = szFileTitle;							// filetitel = ??yyyymm (no extension)
	if (strFileTitle.GetLength() != 8)
	{
		return FALSE;
	}

	CString strLine = "";
	strLine.Format ( "%s - %s", 
		(const char*) strFileTitle.Mid(2,nYEARLEN), 
		(const char*) strFileTitle.Right(nMONTHLEN) );		// yyyy - mm

	switch (nType)
	{
	case nRXX_PAYMENTS:					// valid imports			
		m_listboxView1.AddString(strLine);	
		break;		

	case nRXX_EXCEPTIONS:				// import exceptions		
		m_listboxView2.AddString(strLine);	
		break;		
	
	case nTXX_SUMMARY:					// export summary
	case nTXX_TRANSACTIONS:				// export transaction log (Diner service)
	case nTXX_TRANSACTIONS2:			// export transaction log (Service2)
	case nTXX_BALANCE:					// export balance log
	case nTXX_ECHOPAYMENT:				// payment echo log
	case nTXX_ACCOUNTMATCH:
	case nTXX_SENTTRANSACTIONS:
		m_listboxView3.AddString(strLine);	
		break;
	}

	return TRUE;
}

//$$******************************************************************

void CWebPaymentImportLogDlg::OnDoubleClickListView1() 
{ 
	OnButtonView1();
}

//$$******************************************************************

void CWebPaymentImportLogDlg::OnDoubleClickListView2()
{
	OnButtonView2();
}

//$$******************************************************************

void CWebPaymentImportLogDlg::OnDoubleClickListView3()
{
	OnButtonView3();
}

//$$******************************************************************

void CWebPaymentImportLogDlg::OnButtonView1()
{
	if (UpdateData() == TRUE)
	{
		DisplayRxReport(nRXX_PAYMENTS, m_strView1File);			// import lines
	}
}

//$$******************************************************************

void CWebPaymentImportLogDlg::OnButtonView2()
{
	if (UpdateData() == TRUE)
	{
		DisplayRxReport(nRXX_EXCEPTIONS, m_strView2File);			// exceptions
	}
}

//$$******************************************************************

void CWebPaymentImportLogDlg::OnButtonView3() 
{
	if ( UpdateData() == TRUE )
	{
		CString strTitle = "";
		CString strLabel = "";
		CString strFilename	= System.GetWebPaymentPath ( GetFilename ( m_nView3Type, m_strView3File ), m_nWebPaymentType );

		switch ( m_nView3Type )
		{
		case nTXX_SUMMARY:
			break;											// web export summary

		case nTXX_TRANSACTIONS:	
			strLabel = "R207";								// web export detailed transactions		
			strTitle = m_strView3TransTitle;				// "%s Web Payment Export Transaction Log", System.GetWebPaymentName()
			break;

		case nTXX_TRANSACTIONS2:
			strLabel = "R207A";								// web export detailed transactions	
			strTitle = m_strView3TransTitle;				// "%s Web Payment Export Transaction Log", System.GetWebPaymentName()
			break;

		case nTXX_BALANCE:
			strLabel = "R208";								// web export detailed balances
			strTitle.Format ( "%s Web Payment Export Balance Log", 
				(const char*) System.GetWebPaymentDisplayName( m_nWebPaymentType ) );
			break;

		case nTXX_ECHOPAYMENT:
			strLabel = "R209";								// web export echo log
			strTitle.Format ( "%s Web Payment Echo Log", 
				(const char*) System.GetWebPaymentDisplayName( m_nWebPaymentType ) );
			break;

		case nTXX_ACCOUNTMATCH:	
			strLabel = "R210";								// web exportdiner data echo log
			strTitle.Format ( "%s Web Payment Account Echo Log",
				(const char*) System.GetWebPaymentDisplayName( m_nWebPaymentType ) );
			break;

		case nTXX_SENTTRANSACTIONS:
			strLabel = "R231";
			strTitle.Format ( "%s Web Payment Sent Transactions Log",
				(const char*) System.GetWebPaymentDisplayName( m_nWebPaymentType ) );
			break;
		}

		if ( m_nView3Type == nTXX_SUMMARY )
		{
			CWebPaymentExportTxReport report ( strFilename, this );
			report.Show();
		}
		else											// other csv exports log
		{
			CCSVFileReport report ( strLabel, strFilename, this );
			report.Show ( strTitle );
		}
	}
}

//$$******************************************************************

void CWebPaymentImportLogDlg::OnButtonView4()
{
	CString strTitle = "Web Payment Suspension Notice";

	CCSVFileReport report ( "R225", Filenames.GetWebPaymentStopIndicator(), this );
	report.SetNoHeader();
	report.SetMode ( P_WAIT );
	report.Show ( strTitle );

	if ( Prompter.YesNo( "Do you wish to clear the suspension notice?", strTitle ) == IDYES )
	{
		CFileRecycler::SendToRecycleBin ( Filenames.GetWebPaymentStopIndicator() );
		::LogError ( "Internet suspension cleared after display", Filenames.GetWebPaymentRxErrorLog() );		// "RxError.log"
		m_buttonView4.ShowWindow(SW_HIDE);
	}
}

//$$******************************************************************

void CWebPaymentImportLogDlg::DisplayRxReport ( int nType, const char* szMask )
{
	CString strLabel	= ( nType == nRXX_PAYMENTS ) ? "R204" : "R203";
	CString strFilename	= System.GetWebPaymentPath ( GetFilename(nType,szMask ), m_nWebPaymentType );

	CWebPaymentReporter report ( strLabel, m_nWebPaymentType, strFilename, this );
	report.Show();
}

//$$******************************************************************

void CWebPaymentImportLogDlg::OnButtonDeleteView1() 
{
	if ( UpdateData() == TRUE )
	{
		int nCurSel = m_listboxView1.GetCurSel();

		if (nCurSel < m_listboxView1.GetCount())
		{
			if (DeleteLog(nRXX_PAYMENTS, m_strView1File) == TRUE)		// valid payments
			{
				m_listboxView1.DeleteString(nCurSel);
			}
		}
	}
}

//$$******************************************************************

void CWebPaymentImportLogDlg::OnButtonDeleteView2()
{
	if (UpdateData() == TRUE)
	{
		int nCurSel = m_listboxView2.GetCurSel();
		if (nCurSel < m_listboxView2.GetCount())
		{
			if (DeleteLog(nRXX_EXCEPTIONS, m_strView2File) == TRUE)		// import exceptions
			{
				m_listboxView2.DeleteString(nCurSel);
			}
		}
	}
}

//$$******************************************************************

void CWebPaymentImportLogDlg::OnButtonDeleteView3() 
{
	if ( UpdateData() == TRUE )
	{
		int nCurSel = m_listboxView3.GetCurSel();
		if (nCurSel < m_listboxView3.GetCount())
		{
			if (DeleteLog(m_nView3Type, m_strView3File) == TRUE)
			{
				m_listboxView3.DeleteString(nCurSel);
			}
		}
	}
}

//$$******************************************************************

bool CWebPaymentImportLogDlg::DeleteLog ( int nType, const char* szMask ) 
{
	CString strType = "";

	switch ( nType )
	{
	case nRXX_PAYMENTS:			strType = "valid payment imports";		break;
	case nRXX_EXCEPTIONS:		strType = "payment import exceptions";	break;
	case nTXX_SUMMARY:			strType = "export summary";				break;
	case nTXX_TRANSACTIONS:		strType = "export transaction";			break;
	case nTXX_BALANCE:			strType = "export balance";				break;
	case nTXX_ECHOPAYMENT:		strType = "payment echo";				break;
	case nTXX_ACCOUNTMATCH:		strType = "account match";				break;
	case nTXX_TRANSACTIONS2:	strType = "export transaction";			break;
	}

	CString strMsg = "";
	strMsg.Format ( "Are you sure you wish to archive the %s log file %s",
		(const char*) strType, 
		szMask );

	if ( Prompter.YesNo( strMsg, "Please confirm" ) == IDYES )
	{
		CString strDestPath = System.GetWebPaymentPath ( "Archive", m_nWebPaymentType  );

		if (::ExistSubdirectory(strDestPath) == FALSE)
		{
			::MakeSubdirectory(strDestPath);
		}

		CString strFilename	= System.GetWebPaymentPath ( GetFilename(nType,szMask ), m_nWebPaymentType );

		strDestPath += "\\";
		strDestPath += GetFilename ( nType, szMask );
		CFileRecycler::SendToRecycleBin ( strDestPath );								// just in case ( else moved won't work)

		if (::MoveFile(strFilename, strDestPath) != 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

//$$******************************************************************

CString CWebPaymentImportLogDlg::GetFilename ( int nType, const char* szMask )
{
	CString strMask = szMask;

	CString strFilename = "";
	strFilename.Format ( "%s%s%s.dat", 
		(const char*) GetFilePrefix(nType), 
		(const char*) strMask.Left(nYEARLEN),
		(const char*) strMask.Right(nMONTHLEN) );
	
	return strFilename;
}

//$$******************************************************************

CString CWebPaymentImportLogDlg::GetFilePrefix ( int nType )
{
	CString strPrefix = "";
		
	switch ( nType )
	{
	case nRXX_PAYMENTS:			strPrefix = "In";	break;				// import - valid payments
	case nRXX_EXCEPTIONS:		strPrefix = "Ex";	break;				// import - exceptions
	case nTXX_SUMMARY:			strPrefix = "Tx";	break;				// export - summary
	case nTXX_TRANSACTIONS:		strPrefix = "Tn";	break;				// export - transations ( Diner service)
	case nTXX_TRANSACTIONS2:	strPrefix = "To";	break;				// export - transations (service2)
	case nTXX_BALANCE:			strPrefix = "Tz";	break;				// export - balances
	case nTXX_ECHOPAYMENT:		strPrefix = "Te";	break;				// export - payment echo
	case nTXX_ACCOUNTMATCH:		strPrefix = "Ta";	break;				// export - account echo
	case nTXX_SENTTRANSACTIONS:	strPrefix = "Ts";	break;				// export - account echo
	}

	return strPrefix;
}

//$$******************************************************************

void CWebPaymentImportLogDlg::OnButtonReport1()
{
	CWebPaymentListReport report ( m_strWebPaymentListFilename1, "R205", this );
	report.Show();
}

//$$******************************************************************

void CWebPaymentImportLogDlg::OnButtonReport2()
{
	CWebPaymentListReport report ( m_strWebPaymentListFilename2, "R205A", this );
	report.Show();
}

//$$******************************************************************

void CWebPaymentImportLogDlg::OnButtonRetryReport()
{
	switch (m_nWebPaymentType)
	{
	case nWEBPAYMENT_BROMCOM:
	{
		CBromComPaymentExceptionReport report("R208", this);
		report.Show();
	}
	break;

	case nWEBPAYMENT_PEBBLE:
	{
		CPebblePaymentExceptionReport report("R208", this);
		report.Show();
	}
	break;

	default:
	{
		CWebPaymentExceptionReport report("R208", m_strWebPaymentExceptionFilename1, this);
		report.Show();
	}
	break;
	}
}

//$$******************************************************************

void CWebPaymentImportLogDlg::OnButtonRetryReport2()
{
	CWebPaymentExceptionReport report ( "R208", m_strWebPaymentExceptionFilename2, this );
	report.Show();
}

//$$******************************************************************

void CWebPaymentImportLogDlg::OnButtonRetry()
{
	CWebPaymentImport import ( this );
	bool bTriedSendUpdates = FALSE;
	import.ImportPayments ( FALSE, bTriedSendUpdates );								
	CFileRecycler::SendToRecycleBin ( Filenames.GetWebPaymentExceptionIndicator() );
	SetExceptionButtons();
}

//$$******************************************************************

void CWebPaymentImportLogDlg::SetExceptionButtons()
{
	bool bHaveReportException1	= ::FileExists ( m_strWebPaymentExceptionFilename1 );
	bool bHaveReportException2	= ::FileExists ( m_strWebPaymentExceptionFilename2 );

	switch (m_nWebPaymentType)
	{
	case nWEBPAYMENT_BROMCOM:
	{
		bHaveReportException1 = FALSE;
		bHaveReportException2 = FALSE;

		CSQLRepositoryBromComPendingOrder PrepStat;

		if (PrepStat.CountRows(NULL).GetSQLResult() > 0)
		{
			bHaveReportException1 = TRUE;
		}
	}
	break;

	case nWEBPAYMENT_PEBBLE:
	{
		bHaveReportException1 = FALSE;
		bHaveReportException2 = FALSE;

		CString strSQL = "";
		strSQL.Format("SELECT TOP 1 1 FROM %s",
			(const char*)SQLTableNames::PebbleUnmatchedTransactions);
			
		CSQLRecordSetConnectionPool RecordSet(NULL);
		if (RecordSet.Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly) == TRUE)
		{
			if ( RecordSet.IsEOF() == FALSE )
			{
				bHaveReportException1 = TRUE;
			}

			RecordSet.Close();
		}
	}
	break;
	}

	if (m_strRetryButtonList1 == "")
	{
		m_buttonRetryReport.ShowWindow(SW_HIDE);
	}
	else
	{
		m_buttonRetryReport.EnableWindow(bHaveReportException1);
	}

	if (m_strReportButtonList2 == "")
	{
		m_buttonRetryReport2.ShowWindow(SW_HIDE);
	}
	else
	{
		m_buttonRetryReport2.EnableWindow(bHaveReportException2);
	}

	m_buttonRetry.EnableWindow (bHaveReportException1 || bHaveReportException2);
}

//$$******************************************************************
// Toggle export type - 3=Export Summary, 4=Export transactions, 5=Export balances, 6=PaymentEcho, 7=account echo

void CWebPaymentImportLogDlg::OnButtonOther()
{
	int nCurrentType = m_nView3Type;

	do
	{
		m_listboxView3.ResetContent();

		m_nView3Type++;
		if (m_nView3Type > nMAX_TYPENO)
		{
			m_nView3Type = nTXX_SUMMARY;
		}

		int nCount = AddLogFiles(m_nView3Type);

		if (nCount > 0)
		{
			m_listboxView3.SetCurSel(nCount - 1);
			m_staticExportBox.SetWindowText(GetExportsBoxTitle());
			break;
		}
	} 
	while (nCurrentType != m_nView3Type);			
}

//$$******************************************************************

CString CWebPaymentImportLogDlg::GetExportsBoxTitle()
{
	CString strCaption = "";

	switch ( m_nView3Type )
	{
	case nTXX_SUMMARY:			strCaption = "Export Summary";				break;
	case nTXX_TRANSACTIONS:		strCaption = "Exported Transactions";		break;
	case nTXX_BALANCE:			strCaption = "Exported Balances";			break;
	case nTXX_ECHOPAYMENT:		strCaption = "Exported Payment Echoes";		break;
	case nTXX_ACCOUNTMATCH:		strCaption = "Exported Account Matches";	break;
	case nTXX_TRANSACTIONS2:	strCaption = "Exported Transactions(2)";	break;
	case nTXX_SENTTRANSACTIONS:	strCaption = "Sent Transactions";			break;
	}

	return strCaption;
}

//$$******************************************************************

void CWebPaymentImportLogDlg::SetExportsMoreButton()
{
	int nTypeCount = 0;

	for (int nType = nTXX_SUMMARY; nType <= nMAX_TYPENO; nType++)
	{
		if (m_nWebPaymentType == nWEBPAYMENT_WISEPAY)
		{
			if (nType == nTXX_BALANCE)						// no longer used all goes thru Transactions 1& 2
			{
				continue;
			}
		}

		CString strMask = "";
		strMask.Format("%s??????.dat", 
			(const char*) GetFilePrefix(nType));

		if (::CountFiles(System.GetWebPaymentPath(strMask, m_nWebPaymentType)) > 0)
		{
			++nTypeCount;
		}
	}

	if (nTypeCount < 2)
	{
		m_buttonOther.ShowWindow(SW_HIDE);
	}
}

//$$******************************************************************
// Re-Send already export lines

void CWebPaymentImportLogDlg::OnButtonResend()
{
	if (UpdateData() == TRUE)
	{
		int nCount = 0;
		CString strFilename = System.GetWebPaymentPath(GetFilename(m_nView3Type, m_strView3File), m_nWebPaymentType);
		if (::GetFileLineCount(strFilename) > 1)
		{
			CSmartPayLockRequest Request;
			Request.m_bWantLockBgnd = TRUE;
			Request.m_bWantLockServerInner = TRUE;
			
			CSmartPayLockManager LockManager;
			int nLockError = LockManager.AcquireLocks(Request, this);

			if (LOCKMANAGER_OK == nLockError)								// pause server \ background
			{
				CString strUpdateFile = System.GetWebPaymentPath(::GetUniqueTempFilename("$20"), m_nWebPaymentType);

				CWebPaymentExportCsvLog log;
				if ((nCount = log.StripLog(strFilename, strUpdateFile)) > 0)		// remove all none essential comment\blank lines, return number of line left in file
				{
					if (m_nWebPaymentType == nWEBPAYMENT_PARENTPAY)
					{
						CParentPayData data;
						data.Read();

						CParentPayExport pp(&data, this);


						if (m_nView3Type == nTXX_ECHOPAYMENT)
						{
							pp.ResendPaymentEcho(strUpdateFile);
						}
						else if (m_nView3Type == nTXX_TRANSACTIONS)
						{
							pp.ResendTransactions(strUpdateFile);
						}
						else if (m_nView3Type == nTXX_ACCOUNTMATCH)
						{
							pp.ResendDinerMatch(strUpdateFile);
						}
					}
					else if (m_nWebPaymentType == nWEBPAYMENT_TUCASIv2)
					{
						CTucasiJsonData data;
						data.Read();

						CTucasiJsonExport tx(&data, this);

						if (m_nView3Type == nTXX_TRANSACTIONS)
						{
							tx.ResendTransactions(strUpdateFile);
						}
						else
						{
							Prompter.Info("Sending Balances is currently inhibited!", "Re-Send Web Payment Data");
						}
					}
				}
				CFileRecycler::SendToRecycleBin(strUpdateFile);						// delete temo file if still around
			}
		}

		if (nCount == 0)
		{
			Prompter.Error("No valid data lines found in log file!", "Re Send Data");
		}
	}
}

//$$******************************************************************

