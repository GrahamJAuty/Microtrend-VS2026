#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CReportPluBarcode
{
public:
	CReportPluBarcode( CPluFilterArray& FilterArray, const char* szKey );
	void CreateReport();
	void SetPluFrom( __int64 nPlu ) { m_nPluFrom = nPlu; }
	void SetPluTo( __int64 nPlu ) { m_nPluTo = nPlu; }
	void SetDepartmentFilter( int nFilter ) { m_nDepartmentFilter = nFilter; }
	void SetSortByDeptFlag( bool bFlag ) { m_bSortByDept = bFlag; }
	void SetDirectFlag( bool bFlag ){ m_bDirect = bFlag; }

private:
	int CreatePluBarcodeReport();
	void CreatePluBarcodeReportHeader();
	void CreatePluBarcodeReportLines ( CPluCSVRecord& PluRecord, CStringArray& arrayLines );

private:
	CPluFilterArray& m_FilterArray;
	CReportFile m_ReportFile;
	CString m_strParamsFilename;
	CString m_strKey;
	__int64 m_nPluFrom;
	__int64 m_nPluTo;
	int m_nDepartmentFilter;
	bool m_bSortByDept;
	bool m_bDirect;
};

/**********************************************************************/
