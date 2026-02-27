#pragma once
//**********************************************************************

class CReportPebbleBalances
{
public:
	CReportPebbleBalances ( CWnd* pParent );
	bool Show();

private:
	void Reset();
	int Create ( CString strReportFile );
	void AddColumnText();
	void AddNoDataMarker(CCSV& Csv);
	void ShowTotals();

private:
	CWnd* m_pParent;
	CString m_strTitle;	
	CReportFileWithEndText m_Report;	

private:
	CString m_strCommentPadding;
	double m_dDiscrepancyPlus;
	double m_dDiscrepancyMinus;
	double m_dDiscrepancySettled;
	double m_dPebbleSales;
	//double m_dPebbleCredit;
};

//**********************************************************************
