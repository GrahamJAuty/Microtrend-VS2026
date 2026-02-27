//*******************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\WebPaymentList.h"
//*******************************************************************
#include "WebPaymentListReport.h"
//*******************************************************************

CWebPaymentListReport::CWebPaymentListReport(const char* szListFile, const char* szReportLabel, CWnd* pParent)
{
	m_strListFile = szListFile;
	m_strReportLabel = szReportLabel;
	m_pParent = pParent;

	m_strTitle.Format("%s Web Payment Listing Report",
		(const char*)System.GetWebPaymentDisplayName());

	m_bColumn4Reqd = TRUE;										// ServiceID
	m_strColumn4Heading = "ServiceID";

	switch (System.GetWebPaymentType())
	{
	case nWEBPAYMENT_TUCASI:
	case nWEBPAYMENT_PARENTMAIL:
	case nWEBPAYMENT_SCHOOLCOMMS:
	case nWEBPAYMENT_BROMCOM:
	case nWEBPAYMENT_PEBBLE:
		m_bColumn4Reqd = FALSE;			// No ServiceID							
		break;

	case nWEBPAYMENT_TUCASIv2:
		m_strColumn4Heading = "ItemNo";
		break;

	case nWEBPAYMENT_SQUID:
		m_strColumn4Heading = "Seq.No";
		break;

	case nWEBPAYMENT_WISEPAY:
		m_strColumn4Heading = "ProductID";
		break;

	case nWEBPAYMENT_PARENTPAY:
	default:
		break;
	}
}

//*******************************************************************

void CWebPaymentListReport::Show()
{
	CString strReportFile = Filenames.GetReportFilename ( m_strReportLabel );
	CString strParamsFile = Filenames.GetReportParamsFilename ( m_strReportLabel );

	CRepmanHandler repman ( m_strTitle, m_pParent );

	int nErrorNo = 0;
	if ((nErrorNo = Create(strReportFile)) == nREPORT_NOERROR)
	{
		nErrorNo = repman.DisplayReport(strReportFile, strParamsFile, m_strReportLabel);
	}

	repman.DisplayError ( nErrorNo );
}

//*******************************************************************

int CWebPaymentListReport::Create(const char* szReportName)
{
	CWebPaymentListDatabase dbWebPaymentList;
	if (dbWebPaymentList.Open(m_strListFile, DB_READONLY) != DB_ERR_NONE)	// ret 0=ok, 1=file error, 2=file in use
	{
		return nREPORT_NOSOURCE;
	}

	if (m_Report.Open(szReportName) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

	bool bCancelButton = FALSE;

	m_Report.SetStyle1(m_strTitle);
	AddColumnText();

	double dTotal = 0.0;

	if (dbWebPaymentList.GetRecordCount() > 0)
	{
		dbWebPaymentList.MoveFirst();							// get first record

		do
		{
			__int64 nPaymentID = dbWebPaymentList.GetPaymentID();

			switch (System.GetWebPaymentType())
			{
			case nWEBPAYMENT_TUCASI:
			case nWEBPAYMENT_TUCASIv2:
				nPaymentID /= 100;
				break;
			}

			dTotal += dbWebPaymentList.GetAmount();				// add to report total

			CCSV csv('\t');
			csv.Add(nPaymentID);
			csv.Add(dbWebPaymentList.GetDate());
			csv.Add(dbWebPaymentList.GetTime());

			if (m_bColumn4Reqd == TRUE)
			{
				csv.Add(dbWebPaymentList.GetServiceID());
			}

			csv.Add(dbWebPaymentList.GetAmount(), 2);
			csv.Add(System.FormatCardNo(dbWebPaymentList.GetUserID()));
			csv.Add(dbWebPaymentList.GetName());
			m_Report.WriteLine(csv);
		}
		while (dbWebPaymentList.MoveNext() == TRUE);
	}

	AddTotal(dTotal);							// add total to report

	dbWebPaymentList.Close();
	m_Report.Close();

	return nREPORT_NOERROR;
}

//**********************************************************************

void CWebPaymentListReport::AddColumnText()
{
	m_Report.AddColumn ( "PaymentID", TA_LEFT, 210 );
	m_Report.AddColumn ( "Processed", TA_LEFT, 219 );
	m_Report.AddColumn ( "", TA_LEFT, 120 );
	
	if (m_bColumn4Reqd == TRUE)
	{
		m_Report.AddColumn(m_strColumn4Heading, TA_LEFT, 210);
	}

	m_Report.AddColumn ( "Amount", TA_RIGHT, 210 );
	m_Report.AddColumn ( "UserID", TA_LEFT, 210 );
	m_Report.AddColumn ( "Name", TA_LEFT, 390 );
}

//**********************************************************************

void CWebPaymentListReport::AddTotal(double dValue)
{
	CCSV csv('\t');
	CString strLine = "\t\t";

	csv.Add("Total");
	csv.Add("");
	csv.Add("");

	if (m_bColumn4Reqd == TRUE)
	{
		csv.Add("");
		strLine += "\t";
	}

	csv.Add(dValue, 2);
	strLine += "\t<LI>";
	{

		m_Report.WriteLine(strLine);
		m_Report.WriteLine(csv);
	}
}

//**********************************************************************
