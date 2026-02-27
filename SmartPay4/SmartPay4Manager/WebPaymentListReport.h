#pragma once
//*******************************************************************

class CWebPaymentListReport  
{
public:
	CWebPaymentListReport ( const char* szFilename, const char*szReportLabel, CWnd* pParent );
	void Show();

private:
	int Create ( const char* szReportName );
	void AddColumnText();
	void AddTotal ( double dValue );

private:
	CString m_strListFile;
	CString m_strReportLabel;
	CWnd* m_pParent;

	CReportFileWithEndText m_Report;
	CString m_strTitle;

	bool m_bColumn4Reqd;
	CString m_strColumn4Heading;
};

//*******************************************************************
