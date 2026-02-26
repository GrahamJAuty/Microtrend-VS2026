#pragma once
//$$******************************************************************
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
//$$******************************************************************

class CPasswordLogReport
{
public:
	CPasswordLogReport( CString strStartDate, CString strEndDate, bool bSortByUser, bool bLogOnLogOff);

public:
	void CreateReport();

private:
	void CreateFileArray();
	void ConsolidateReport();
	void BuildLineReport();
	void BuildSessionReport();

private:
	CString m_strStartDate;
	CString m_strEndDate;
	bool m_bSortByUser;
	bool m_bLogOnLogOff;

private:
	CStringArray m_arrayFiles;
	CReportConsolidationArray<CSortedPasswordLine> m_arrayEvents;
};

//$$******************************************************************

