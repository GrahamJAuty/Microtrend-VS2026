#pragma once

//*******************************************************************

class CWebPaymentExceptionReport  
{
public:
	CWebPaymentExceptionReport ( const char* szLabel, const char* szFilename, CWnd* pParent );
	void Show();

private:
	int Create ( const char* szReportFile );
	void AddColumnText();
	void ShowTotalValue();
	void SaveLine();

private:
	CString m_strReportLabel;
	CString m_strFilename;
	CWnd* m_pParent;

	CSSUnsortedDatabase m_dbPayments;

	CReportFileWithEndText m_Report;
	CString m_strReportTitle;

	int m_nSortColumnIndex;
	int m_nValueColumnIndex;
	double m_dTotalValue;
	bool m_bValueInPence;
};
