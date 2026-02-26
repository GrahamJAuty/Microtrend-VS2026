#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "Consolidation.h"
#include "CustomPluField.h"
#include "KeyboardFilterPluArray.h"
#include "PriceSetCustomListReportHelpers.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CPriceSetCustomListReport
{
public:
	CPriceSetCustomListReport( CKeyboardFilterPluArray& arrayPluKbd, CPriceSetCustomListReportHelpers& Helpers );
	void CreateReport();
	void SetPluFrom( const char* szPlu ) { m_strPluFrom = szPlu; }
	void SetPluTo( const char* szPlu ) { m_strPluTo = szPlu; }
	void SetDepartmentFilter( int nFilter ) { m_nDepartmentFilter = nFilter; }
	void SetSortByDeptFlag( bool bFlag ) { m_bSortByDept = bFlag; }
	void SetZeroPricedFlag( bool bFlag ) { m_bZeroPriced = bFlag; }
	void SetSeparateFlag( bool bFlag ) { m_bSeparate = bFlag; }
	void SetModifiersFlag( bool bFlag ) { m_bModifiers = bFlag; }

private:
	int CreateReportInternal();
	void CreateHeader();
	void AddFieldToHeader ( CustomPluField Field, bool bPriceSet, int& nColumn );
	void CreateReportLine( CPluCSVRecord& PluRecord );

private:
	CPriceSetCustomListReportHelpers& m_Helpers;
	CKeyboardFilterPluArray& m_PluKbdArray;
	CReportFile m_ReportFile;
	CString m_strParamsFilename;
	CString m_strKey;
	CString m_strPluFrom;
	CString m_strPluTo;
	CString m_strTitle;
	int m_nDepartmentFilter;
	bool m_bSortByDept;
	bool m_bZeroPriced;
	bool m_bModifiers;
	bool m_bSeparate;
};

/**********************************************************************/
#endif
/**********************************************************************/
