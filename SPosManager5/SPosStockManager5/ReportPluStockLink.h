#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CReportPluStockLink
{
public:
	CReportPluStockLink( CPluFilterArray& FilterArray, const char* szKey );
	void CreateReport();
	void SetPluFrom( const char* szPlu ) { m_strPluFrom = szPlu; }
	void SetPluTo( const char* szPlu ) { m_strPluTo = szPlu; }
	void SetDepartmentFilter( int nFilter ) { m_nDepartmentFilter = nFilter; }
	void SetSortByDeptFlag( bool bFlag ) { m_bSortByDept = bFlag; }

private:
	int CreatePluStockLinkReport();
	void CreatePluStockLinkReportHeader();
	void CreatePluStockLinkReportLine( CPluCSVRecord& PluRecord );

private:
	CPluFilterArray& m_FilterArray;
	CReportFile m_ReportFile;
	CString m_strParamsFilename;
	CString m_strKey;
	CString m_strPluFrom;
	CString m_strPluTo;
	int m_nDepartmentFilter;
	bool m_bSortByDept;
};

/**********************************************************************/
#endif
/**********************************************************************/

