/**********************************************************************/
#include "SystemData.h"
#include "ReportManager.h"
#include "GlobalFunctions.h"
/**********************************************************************/
#include "FailureReport.h"
/**********************************************************************/
extern CSysset Sysset;
/**********************************************************************/

CFailureReport::CFailureReport(CString strAuditFilename)
{
	m_strAuditFilename = strAuditFilename;
	m_dateFrom.SetDate("01/01/2000");
	m_dateTo.SetDate("31/12/2099");

	m_strReportName.Format("%s\\Rep0001.rep",
		(const char*)Sysset.GetTempPath());

	m_strReportParams = System.AddSysFilePath("rep0001.prm");
	
	ClearTotals(0);							// day totals
	ClearTotals(1);							// report totals
}

/**********************************************************************/

void CFailureReport::ClearTotals(int nIndex)
{
	m_Totals[nIndex].dValue = 0.0;
	m_Totals[nIndex].nCount = 0;
}

/**********************************************************************/

bool CFailureReport::Create()
{
	bool bReply = m_Report.Open(m_strReportName);

	if (TRUE == bReply)
	{
		m_Report.SetStyle1("Auto Topup Log");
		m_Report.AddColumn("Date", TA_LEFT, 219);
		m_Report.AddColumn("Time", TA_LEFT, 120);
		m_Report.AddColumn("CCNo", TA_LEFT, 141);
		m_Report.AddColumn("UserID", TA_LEFT, 200);
		m_Report.AddColumn("Name", TA_LEFT, 300);
		m_Report.AddColumn("Grp", TA_LEFT, 120);
		m_Report.AddColumn("Value", TA_RIGHT, 180);

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

bool CFailureReport::ReportByDate()
{
	CSSFile file;
	if ( file.Open ( m_strAuditFilename, "rb" ) == FALSE )
	{
		m_nErrorNo = REPORT_NOSOURCE;
		return FALSE;
	}

	int nCount = 0;
	CPayLinkAuditRecord atc;

	CSSDate dateLast;
	dateLast.SetDate("");

	while ( atc.ReadLine( &file ) == TRUE )
	{
		CSSDate dateAudit ( atc.Date() );
		if (dateAudit < m_dateFrom)
		{
			continue;
		}

		if (dateAudit > m_dateTo)
		{
			break;
		}

		SaveLine ( &atc );
	}

	ShowTotal(0);

	return TRUE;
}

//**********************************************************************

bool CFailureReport::SaveLine(CPayLinkAuditRecord* pAtc)
{
	CCSV csv('\t');
	CString strValue;

	csv.Add(pAtc->Date());
	csv.Add(pAtc->Time());
	strValue.Format("%4.4d", pAtc->CCNo());
	csv.Add(strValue);
	csv.Add(System.FormatCardNo(pAtc->UserID()));
	csv.Add(pAtc->UserName());
	csv.Add(pAtc->GroupNo());

	double dValue = pAtc->ValueAdded();
	csv.Add(::FormatDoubleValue(dValue));

	m_Totals[0].dValue += dValue;
	m_Totals[1].dValue += dValue;

	++m_Totals[0].nCount;
	++m_Totals[1].nCount;

	return m_Report.WriteLine(csv);
}

//**********************************************************************

void CFailureReport::ShowTotal(int nIndex)
{
	CString strValue = "";
	CString strLine = "\t\t\t\t\t<LI>\t<LI>";

	CCSV csv('\t');
	csv.Add("<..>Total");						// date
	csv.Add("");								// Time
	csv.Add("");								// ccno
	csv.Add("");								// card no
	csv.Add("");								// name
	csv.Add(m_Totals[nIndex].nCount);			// group
	csv.Add(::FormatDoubleValue(m_Totals[nIndex].dValue));

	m_Report.WriteLine(strLine);
	m_Report.WriteLine(csv);
	m_Report.WriteLine("");
}

//**********************************************************************

void CFailureReport::Display(CWnd* pParent)
{
	ReportManager(pParent, m_strReportName, m_strReportParams, m_strReportKey, FALSE);
}

//**********************************************************************


