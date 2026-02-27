#pragma once

//*******************************************************************

class CWebPaymentExportTxReport  
{
public:
	CWebPaymentExportTxReport ( const char* szFilename, CWnd* pParent );
	void Show();

private:
	int Create ( const char* szReportFile );
	void AddColumnText();

private:
	CString m_strFilename;
	CWnd* m_pParent;

	CReportFileWithEndText m_Report;
	CString m_strReportLabel;
	CString m_strReportTitle;
};
