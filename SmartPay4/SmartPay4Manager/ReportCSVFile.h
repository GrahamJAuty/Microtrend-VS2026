#pragma once
//*******************************************************************
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
//*******************************************************************

class CCSVFileReport  
{
public:
	CCSVFileReport ( const char* szLabel, const char* szFilename, CWnd* pParent );
	void Show (	const char* szTitle = "" );
	void SetMode ( int nMode ) { m_nMode = nMode; }
	void SetNoHeader() { m_bHaveHeader = FALSE; }

private:
	int Create ( const char* szReportFile );

private:
	void AddFieldType(CReportConsolidationArray<CSortedIntByString>& array, CString strFieldName, int nWidth = 0);
	void GetFieldType(CString strLabel, int& nType, int& nWidth);

private:
	CString m_strFilename;
	CWnd* m_pParent;
	int m_nMode; 
	bool m_bHaveHeader;

	CReportFileWithEndText m_Report;
	CString m_strReportLabel;
	CString m_strReportTitle;

private:
	CReportConsolidationArray<CSortedIntByString> m_arrayDoubleFields;
	CReportConsolidationArray<CSortedIntByString> m_arrayImpliedDoubleFields;
	CReportConsolidationArray<CSortedIntByString> m_arrayIntFields;
	CReportConsolidationArray<CSortedIntByString> m_arrayStringFields;
	CReportConsolidationArray<CSortedIntByString> m_arrayNoShowFields;
};

//*******************************************************************
