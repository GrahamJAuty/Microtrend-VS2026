#pragma once
//*******************************************************************

class CCSVFileReport  
{
public:
	CCSVFileReport ( const char* szLabel, const char* szFilename, CWnd* pParent );
	void Show ( const char* szTitle = "" );
	void SetMode ( int nMode )	{ m_nMode = nMode; }

private:
	int Create ( const char* szReportFile );
	int DecimalPlacesReqd ( const char* szLabel );

private:
	CString m_strFilename;
	CWnd* m_pParent;
	int m_nMode;

	CReportFile m_Report;
	CString m_strReportLabel;
	CString m_strReportTitle;
};

//*******************************************************************
