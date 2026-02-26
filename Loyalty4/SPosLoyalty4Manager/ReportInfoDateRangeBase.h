#pragma once
//$$******************************************************************

class CReportInfoDateRangeBase
{
public:
	CReportInfoDateRangeBase();

public:
	//date
	const char* GetDateFromString();
	const char* GetDateToString();
	CSSDate GetDateFrom() { return m_dateFrom; }
	CSSDate GetDateTo() { return m_dateTo; }
	//options
	bool GetSummaryFlag() { return m_bSummary; }

public:
	const char* GetReportTitle() { return m_strReportTitle; }
	const char* GetReportKey() { return m_strReportKey; }

public:
	//date
	void SetDateFrom(CTime& date);
	void SetDateTo(CTime& date);
	//misc
	void SetSummaryFlag(bool b) { m_bSummary = b; }

public:
	void SetReportKey(const char* sz) { m_strReportKey = sz; }

public:
	const char* GetDateRangeString();
	bool IsValidDate(const char* szDate);

protected:
	//date
	CSSDate m_dateFrom;
	CSSDate m_dateTo;
	CString m_strDateFrom;
	CString m_strDateTo;
	CString m_strDateRange;
	//options
	bool m_bSummary;

protected:
	CString m_strReportTitle;
	CString m_strReportKey;
	CString m_strReportLabel;
};

//$$******************************************************************
