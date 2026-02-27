/**********************************************************************/
#include "GlobalFunctions.h"
#include "ReportManager.h"
#include "SystemData.h"
/**********************************************************************/
#include "CoinageReport.h"
/**********************************************************************/
extern CSysset Sysset;
/**********************************************************************/
static const char* szDITTO = "..";
/**********************************************************************/

CCoinageReport::CCoinageReport ( CString strAuditFilename )
{
	m_strAuditFilename = strAuditFilename;
	m_dateFrom.SetDate ( "01/01/2000" );
	m_dateTo.SetDate ( "31/12/2099" );

	m_strReportName.Format ( "%s\\Rep0007.rep", 
		(const char*) Sysset.GetTempPath() );

	m_strReportParams = System.AddSysFilePath ( "Rep0007.prm");
}

/**********************************************************************/

bool CCoinageReport::Create()
{
	bool bReply = m_Report.Open(m_strReportName);

	if (TRUE == bReply)
	{
		m_Report.SetStyle1("Kiosk Log");
		m_Report.AddColumn("Date", TA_LEFT, 219);
		m_Report.AddColumn("Time", TA_LEFT, 120);
		m_Report.AddColumn("CCNo", TA_LEFT, 180);
		m_Report.AddColumn("UserID", TA_LEFT, 200);
		m_Report.AddColumn("Name", TA_LEFT, 300);
		m_Report.AddColumn("Grp", TA_LEFT, 120);
		m_Report.AddColumn("Value", TA_RIGHT, 150);
		m_Report.AddColumn("GT", TA_RIGHT, 240);
		m_Report.AddColumn("Comment", TA_LEFT, 600);

		bReply = ReportByDate();
		m_Report.Close();
	}
	else
	{
		m_nErrorNo = REPORT_CREATEFAIL;
	}

	return bReply;
}

//**********************************************************************

bool CCoinageReport::ReportByDate()
{
	CSSFile file;
	if (file.Open(m_strAuditFilename, "rb") == FALSE)
	{
		m_nErrorNo = REPORT_NOSOURCE;
		return FALSE;
	}

	CPayLinkAuditRecord atc;
	while (atc.ReadLine(&file) == TRUE)
	{
		CSSDate dateAudit(atc.Date());
		if (dateAudit < m_dateFrom)
		{
			continue;
		}

		if (dateAudit > m_dateTo)
		{
			break;
		}

		SaveLine(&atc);
	}

	return TRUE;
}

//**********************************************************************

bool CCoinageReport::SaveLine(CPayLinkAuditRecord* pAtc)
{
	bool bShowGroup = TRUE;
	bool bEmptyCash = FALSE;
	CString strComment = pAtc->GetText();

	CCSV csv('\t');

	csv.Add(pAtc->Date());
	csv.Add(pAtc->Time());

	// CCNo

	int nCCNo = pAtc->CCNo();
	if (0 == nCCNo)						// Empty cash has no ccno
	{
		csv.Add("");
	}
	else
	{
		if (nCCNo != m_nLastCCNo)
		{
			csv.Add(pAtc->CCNo());
			m_nLastCCNo = nCCNo;
		}
		else
		{
			csv.Add(szDITTO);
		}
	}

	// User ID \ CardNo

	CString strUserID = System.FormatCardNo(pAtc->UserID());
	{
		if (strUserID == m_strLastUserID)
		{
			strUserID = szDITTO;
		}
		else
		{
			m_strLastUserID = strUserID;
			m_strLastName = "";
		}
	}
	csv.Add(strUserID);

	// user name

	CString strName = pAtc->UserName();

	if (strName == szTEXT_EMPTYCASH)
	{
		strName = m_strLastName = "";
		strComment = "Empty Cash";
		bEmptyCash = TRUE;
	}
	else					// normal coinage line
	{
		if (strName == m_strLastName)
		{
			strName = szDITTO;
			bShowGroup = FALSE;
		}
		else
		{
			m_strLastName = strName;
		}

		long lCoinageGT = pAtc->CoinageGT();		// use longs stops rounding errors

		if (m_lRunningGT > 0)						// see if running GT setup yet
		{
			if (m_lRunningGT + pAtc->CoinageAdded() != lCoinageGT)
			{
				strComment = "GT out of sequence";
			}
		}
		m_lRunningGT = lCoinageGT;
	}
	csv.Add(strName);

	// Group details

	if (TRUE == bEmptyCash)
	{
		csv.Add("");
	}
	else
	{
		if (TRUE == bShowGroup)
		{
			csv.Add(pAtc->GroupNo());
		}
		else
		{
			csv.Add(szDITTO);
		}
	}

	// value

	if (pAtc->ValueAdded() == 0)
	{
		csv.Add("");
	}
	else
	{
		csv.Add(pAtc->ValueAdded(), 2);
	}

	// GT Total

	if (pAtc->ValueGT() == 0)
	{
		csv.Add("");
	}
	else
	{
		csv.Add(pAtc->ValueGT(), 2);
	}

	// comment line

	csv.Add(strComment);

	// display line

	if (TRUE == bEmptyCash)
	{
		m_Report.WriteLine("<LI>");
	}

	bool bReply = m_Report.WriteLine(csv);

	if (TRUE == bEmptyCash)
	{
		m_Report.WriteLine("<LI>");
	}

	return bReply;
}

//**********************************************************************

void CCoinageReport::Display(CWnd* pParent)
{
	ReportManager(pParent, m_strReportName, m_strReportParams, m_strReportKey, FALSE);
}

//**********************************************************************


