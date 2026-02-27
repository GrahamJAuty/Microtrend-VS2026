//*******************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\WebPaymentExportTxLog.h"
//*******************************************************************
#include "WebPaymentExportTxReport.h"
//*******************************************************************

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWebPaymentExportTxReport::CWebPaymentExportTxReport ( const char* szFilename, CWnd* pParent )
	: m_strFilename(szFilename)
	, m_pParent(pParent)
{
	m_strReportTitle.Format ( "%s Web Payment Export Report", 
		(const char*)System.GetWebPaymentDisplayName() );

	m_strReportLabel = "R206";
}

//*******************************************************************

void CWebPaymentExportTxReport::Show()
{
	int nErrorNo;
	CString strReportFile = Filenames.GetReportFilename ( m_strReportLabel );
	CString strParamsFile = Filenames.GetReportFilename ( m_strReportLabel );

	CRepmanHandler repman ( m_strReportTitle, m_pParent );

	if ( ( nErrorNo = Create ( strReportFile ) ) == nREPORT_NOERROR )
		nErrorNo = repman.DisplayReport ( strReportFile, strParamsFile, m_strReportLabel );
	
	repman.DisplayError ( nErrorNo );
}

//*******************************************************************

int CWebPaymentExportTxReport::Create ( const char* szReportFile )
{
	CSSFile file;
	if ( file.Open ( m_strFilename, "rb" ) == FALSE )
		return nREPORT_NOSOURCE;

	if ( m_Report.Open ( szReportFile ) == FALSE )
		return nREPORT_CREATEFAIL;

	bool bCancelButton = FALSE;

	m_Report.SetStyle1 ( m_strReportTitle );
	AddColumnText();
		
	CWebPaymentExportTxLog log;
	while ( log.ReadLine ( &file ) == TRUE )
	{
		CCSV csv ( '\t' );
		csv.Add ( log.m_date.GetDate() );
		csv.Add ( log.m_time.GetTime() );
		csv.Add ( log.GetTypeString() );
		csv.Add ( log.GetResultString() );
		csv.Add ( log.m_strComment );
		m_Report.WriteLine ( csv );
	}

	file.Close();
	m_Report.Close();

	return nREPORT_NOERROR;
}

//**********************************************************************

void CWebPaymentExportTxReport::AddColumnText()
{
	m_Report.AddColumn ( "Date", TA_LEFT, 210 );
	m_Report.AddColumn ( "Time", TA_LEFT, 120 );
	m_Report.AddColumn ( "Action", TA_LEFT, 240 );
	m_Report.AddColumn ( "Result", TA_LEFT, 240 );
	m_Report.AddColumn ( "Comment", TA_LEFT, 600 );
}


//**********************************************************************
