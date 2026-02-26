#pragma once
/**********************************************************************/
 
/**********************************************************************/
#define BATCH_EMAIL_ORDER_SHEET 2
/**********************************************************************/

class CBatchEmailLog  
{
public:
	CBatchEmailLog();
	~CBatchEmailLog();

	bool Open( int nType );
	bool Close();

	int GetLineCount() { return m_nLineCount; }
	bool AddLine ( const char* szLine );

private:
	bool m_bIsOpen;
	CReportFile m_ReportFile;
	int m_nLineCount;
};

/**********************************************************************/

