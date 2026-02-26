#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/

class CProcessedOrderBaseReport
{
public:
	CProcessedOrderBaseReport ();
	virtual bool CreateReport( int nDbIdx ) = 0;

public:
	static const char* GetReportParamsFilename ( int nRepType );

protected:
	const char* GetReportTitle();

public:
	void SetConLevel ( int nConLevel ) { m_nConLevel = nConLevel; }
	void SetTableNo ( int nTableNo ) { m_nTableNo = nTableNo; }
	void SetConLevelText ( const char* szConLevel ) { m_strConLevel = szConLevel; }
	void SetSupplierArray ( CWordArray& wSuppIdxArray );
	void SetStockCodeFrom( const char* szFrom ) { m_strStockCodeFrom = szFrom; }
	void SetStockCodeTo( const char* szTo ) { m_strStockCodeTo = szTo; }

public:
	void SetDateFrom ( const char* szDate ) { m_strDateFrom = szDate; }
	void SetDateTo ( const char* szDate ) { m_strDateTo = szDate; }
	void SetReportType ( int nType ) { m_nReportType = nType; }
	void SetReference ( const char* szReference ) { m_strReference = szReference; }
				
protected:
	bool MatchStockpoint ( int nSpIdx );
	void ShowStockpoints ( CReportFile& ReportFile );
		
protected:
	int m_nDbIdx;
	int m_nConLevel;
	int m_nTableNo;
	CWordArray m_wSuppIdxArray;
	CString m_strDateFrom;
	CString m_strDateTo;
	CString m_strStockCodeFrom;
	CString m_strStockCodeTo;
	CString m_strReference;

protected:
	int m_nReportType;
	CString m_strConLevel;
	
protected:
	static CString m_strReportParamsFilename;
	CString m_strReportTitle;
};

/**********************************************************************/
#endif		//__STOCKBASEREPORT_H__
/**********************************************************************/
