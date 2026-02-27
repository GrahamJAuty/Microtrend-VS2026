//*******************************************************************
#include "SystemData.h"
#include "ReportManager.h"
#include "GlobalFunctions.h"
//*******************************************************************
#include "CashRemovedReport.h"
//*******************************************************************
extern CSysset Sysset;
//*******************************************************************

CCashRemovedReport::CCashRemovedReport(CString strAuditFilename)
{
	m_strAuditFilename = strAuditFilename;
	m_dateFrom.SetDate("01/01/2000");
	m_dateTo.SetDate("31/12/2099");

	m_strReportName.Format("%s\\Rep0005.rep",
		(const char*)Sysset.GetTempPath());

	m_strReportParams = System.AddSysFilePath("Rep0005.prm");
}

//*******************************************************************

bool CCashRemovedReport::Create()
{
	bool bReply = m_Report.Open(m_strReportName);

	if (TRUE == bReply)
	{
		m_Report.SetStyle1("Cash Removed Log");
		m_Report.AddColumn("Date", TA_LEFT, 219);
		m_Report.AddColumn("Time", TA_LEFT, 120);
		m_Report.AddColumn("UserID", TA_LEFT, 200);
		m_Report.AddColumn("Value", TA_RIGHT, 210);
		m_Report.AddColumn("GT", TA_RIGHT, 300);

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

bool CCashRemovedReport::ReportByDate()
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

		if (atc.UserName() == szTEXT_EMPTYCASH)
		{
			if ((atc.CCNo() == 0) && (atc.GroupNo() == 0))
			{
				SaveLine(&atc);
			}
		}
	}

	ShowTotal();
	return TRUE;
}

//**********************************************************************

bool CCashRemovedReport::SaveLine(CPayLinkAuditRecord* pAtc)
{
	CCSV csv('\t');

	double dValue = pAtc->ValueAdded();
	m_dTotalRemoved += dValue;

	csv.Add(pAtc->Date());
	csv.Add(pAtc->Time());
	csv.Add(System.FormatCardNo(pAtc->UserID()));
	csv.Add(dValue, 2);
	csv.Add(pAtc->ValueGT(), 2);

	return m_Report.WriteLine(csv);
}

//**********************************************************************

void CCashRemovedReport::ShowTotal()
{
	CCSV csv('\t');

	CString strLine = "\t\t\t<LI>";

	csv.Add("<..>Total");						// date
	csv.Add("");								// Time
	csv.Add("");								// CardNo
	csv.Add(m_dTotalRemoved, 2);

	m_Report.WriteLine(strLine);
	m_Report.WriteLine(csv);
	m_Report.WriteLine("");
}

//**********************************************************************

void CCashRemovedReport::Display(CWnd* pParent)
{
	ReportManager(pParent, m_strReportName, m_strReportParams, m_strReportKey, FALSE);
}

//**********************************************************************


