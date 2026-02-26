#pragma once
/**********************************************************************/
#define REPMAN_ACTION_DISPLAY 0
#define REPMAN_ACTION_PRINT 1
#define REPMAN_ACTION_REDIRECT1 2
#define REPMAN_ACTION_REDIRECT2 3
/**********************************************************************/

class CReportManager
{
public:
	CReportManager();

public:
	bool DisplayReport ( const char* szReportFilename, const char* szReportParamsFilename, const char* szReportFieldsKey, CWnd* pParentWnd );
	bool DisplaySilent ( const char* szReportFilename, const char* szReportParamsFilename, const char* szReportFieldsKey, CWnd* pParentWnd );
	bool PrintReport ( const char* szReportFilename, const char* szReportParamsFilename, const char* szReportFieldsKey, CWnd* pParentWnd );
	bool PrintSilent ( const char* szReportFilename, const char* szReportParamsFilename, const char* szReportFieldsKey, CWnd* pParentWnd );
	bool DisplayGraph( const char* szGraphFilename );

public:
	void ReportManagerError();
	void GraphManagerError();

private:
	bool ShowReport ( const char* szReportFilename, const char* szReportParamsFilename, const char* szReportFieldsKey, bool bPrint, bool bErrors, CWnd* pParentWnd );
	
private:
	bool m_bShownWarning;
};

/**********************************************************************/
 