//$$******************************************************************
#include "ReportInfoPeriodSales.h"
//$$******************************************************************

CReportInfoPeriodSales::CReportInfoPeriodSales() : CReportInfoDateRangeBase(), m_TerminalListHandler(Filenames.GetTerminalListFilename())
{
	m_strReportTitle = "Group Period Sales";
	m_nSalesTypeFilter = 0;
	m_bManualSpend = FALSE;
	SetDbNo(0);
	SetFolderSet(0);
	SetTerminalSelectType(0);
}

//$$******************************************************************

void CReportInfoPeriodSales::SetSalesTypeFilter(int n)
{
	if ((n >= 0) && (n <= 2))
	{
		m_nSalesTypeFilter = n;
	}
}

//$$******************************************************************

void CReportInfoPeriodSales::SetDbNo(int n)
{
	m_nDbNo = n;
	m_TerminalListHandler.SetTargetDbNo(n);	
}

//$$******************************************************************

void CReportInfoPeriodSales::SetFolderSet(int n)
{
	m_nFolderSet = n;
	m_TerminalListHandler.SetTargetFolderSet(n);
}	

//$$******************************************************************

void CReportInfoPeriodSales::SetTerminalSelectType(int n)
{
	m_nTerminalSelectType = n;
	m_TerminalListHandler.SetTerminalSelectType(n);
}

//$$******************************************************************
