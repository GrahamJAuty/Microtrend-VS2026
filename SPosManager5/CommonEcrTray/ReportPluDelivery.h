#pragma once
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
#include "DepartmentSetCSVArray.h"
/**********************************************************************/

class CReportPluDelivery 
{
public:
	CReportPluDelivery();
	void SetDbIdx( int n ) { m_nDbIdx = n; }
	void SetPluRangeFlag ( bool bFlag ) { m_bPluRange = bFlag; }
	void SetPluFrom ( __int64 nPluNo ) { m_nPluFrom = nPluNo; }
	void SetPluTo ( __int64 nPluNo ) { m_nPluTo = nPluNo; }
	void SetDateFrom ( COleDateTime date ) { m_oleDateFrom = date; }
	void SetDateTo ( COleDateTime date ) { m_oleDateTo = date; }
	void SetDepartmentFilter ( int nFilter ) { m_DepartmentSet.CreateDepartmentSet ( nFilter ); }
	void SetSupplierFilter( int nFilter ) { m_nSupplierFilter = nFilter; }
	bool CreateDeliveryAuditReport();
	
	const char* GetEmailSubject(){ return m_strEmailSubject; }

private:
	bool CreateAuditInternal();

private:
	int m_nDbIdx;
	CDepartmentSetCSVRecord m_DepartmentSet;
	int m_nSupplierFilter;
	bool m_bPluRange;
	__int64 m_nPluFrom;
	__int64 m_nPluTo;
	COleDateTime m_oleDateFrom;
	COleDateTime m_oleDateTo;
	CString m_strEmailSubject;
};

/**********************************************************************/
#endif
/**********************************************************************/

