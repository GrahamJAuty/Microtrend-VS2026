#pragma once
/**********************************************************************/
 
/**********************************************************************/

class CStockReportSelect
{
public:
	CStockReportSelect(void);

public:
	void LoadReportSelection( bool bSummary = FALSE );
	void SaveReportSelection();

public:
	void ClearSelection(){ m_arrayReports.RemoveAll(); }
	void AddReport( int n ){ m_arrayReports.Add(n); }
	
public:
	int GetReportCount(){ return m_arrayReports.GetSize(); }
	int GetReportType( int nPos );

private:
	CWordArray m_arrayReports;
};

/**********************************************************************/
