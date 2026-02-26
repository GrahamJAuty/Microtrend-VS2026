#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "CustomPluField.h"
#include "KeyboardFilterPluArray.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CReportPluCustomList
{
public:
	CReportPluCustomList( int nType, CPluFilterArray& FilterArray, CKeyboardFilterPluArray& arrayPluKbd, const char* szKey );
	void CreateReport();
	void SetChangesDate( COleDateTime& date ){ m_ChangesDate = date; }
	void SetNoDateFilterFlag( bool b ) { m_bNoDateFilter = b; }
	void SetPluFrom( const char* szPlu ) { m_strPluFrom = szPlu; }
	void SetPluTo( const char* szPlu ) { m_strPluTo = szPlu; }
	void SetDepartmentFilter( int nFilter ) { m_nDepartmentFilter = nFilter; }
	void SetSortByDeptFlag( bool bFlag ) { m_bSortByDept = bFlag; }
	void SetZeroPricedFlag( bool bFlag ) { m_bZeroPriced = bFlag; }
	void SetSeparateFlag( bool bFlag ) { m_bSeparate = bFlag; }
	void SetModifiersFlag( bool bFlag ) { m_bModifiers = bFlag; }

	void SetCustomName( const char* sz ) { m_strCustomName = sz; }

private:
	int CreateReportInternal();
	void CreateHeader();
	void AddFieldToHeader ( CustomPluField Field );
	void CreateReportLine( CPluCSVRecord& PluRecord );

public:
	static void GetCSVReportName( int n, CString& strName );

private:
	CPluFilterArray& m_FilterArray;
	CKeyboardFilterPluArray& m_PluKbdArray;
	CReportFile m_ReportFile;
	CString m_strParamsFilename;
	CString m_strKey;
	CString m_strPluFrom;
	CString m_strPluTo;
	CString m_strTitle;
	COleDateTime m_ChangesDate;
	bool m_bNoDateFilter;
	int m_nDepartmentFilter;
	bool m_bSortByDept;
	bool m_bZeroPriced;
	bool m_bModifiers;
	bool m_bSeparate;

private:
	CString m_strCustomName;
	int m_nType;
};

/**********************************************************************/

