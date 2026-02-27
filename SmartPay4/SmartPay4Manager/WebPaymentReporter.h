#pragma once
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\WebPaymentLogRecord.h"
//$$******************************************************************

struct CWebPaymentReporterColumn
{
public:
	CWebPaymentReporterColumn(bool bShow, CString strLabel, int nAlign, int nWidth)
	{
		m_bShow = bShow;
		m_strLabel = strLabel;
		m_nAlign = nAlign;
		m_nWidth = nWidth;
	};

	CWebPaymentReporterColumn()
	{
		m_bShow = FALSE;
		m_strLabel = "";
		m_nAlign = TA_LEFT;
		m_nWidth = 300;
	};

public:
	bool m_bShow;
	CString m_strLabel;
	int m_nAlign;
	int m_nWidth;
};

//$$******************************************************************

class CWebPaymentReporter  
{
public:
	CWebPaymentReporter ( const char* szLabel, int nWebPaymentType, const char* szFilename, CWnd* pParent );
	void Show ( int nMode = P_NOWAIT );

private:
	int Create ( const char* szReportFile );
	void AddColumnText();
	void ResetTotal ( int n );
	bool SaveTotal ( int nIndex );
	bool ShowReportLine ( CWebPaymentLogRecord* pRecord );

private:
	void AddReportColumn(CWebPaymentReporterColumn& Column);
	void AddReportField(CCSV& csv, __int64 n);
	void AddReportField(CCSV& csv, CString str);
	void AddReportField(CCSV& csv, double d, int nDPs);

private:
	CWnd* m_pParent;
	int m_nWebPaymentType;
	CString m_strFilename;
	CString m_strReportLabel;
	bool m_bGotSubtotal;

	CReportFileWithEndText m_Report;
	CString m_strTitle;

	double m_dPaidTotal[2];
	CArray<CWebPaymentReporterColumn, CWebPaymentReporterColumn> m_arrayColumns;
	CArray<int, int> m_arrayColumnOrder;
};

//$$******************************************************************
