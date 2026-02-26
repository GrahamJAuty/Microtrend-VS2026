#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/

class CProcessedOrderLog  
{
public:
	CProcessedOrderLog();
	~CProcessedOrderLog();

	bool Open();
	bool Close();

	int GetLineCount() { return m_nLineCount; }
	bool AddLine ( int nSpIdx, int nProblem );

private:
	bool m_bIsOpen;
	CReportFile m_ReportFile;
	int m_nLineCount;
};

/**********************************************************************/
#endif
/**********************************************************************/
