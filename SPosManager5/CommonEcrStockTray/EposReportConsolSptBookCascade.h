#pragma once
/**********************************************************************/
#include "EposReportBlockMap.h"
/**********************************************************************/

struct CEposReportConsolSptBookCascade
{
public:
	CEposReportConsolSptBookCascade();
	void Reset();

public:
	int Compare ( CEposReportConsolSptBookCascade& source, int nHint = 0 );
	void Add	( CEposReportConsolSptBookCascade& source );
	
public:
	int GetDifferenceLevel( CEposReportConsolSptBookCascade& source );

public:
	const char* GetYearText( bool bTotal );
	const char* GetMonthText( bool bTotal );
	const char* GetTaxBandText( bool bTotal );
	const char* GetCategoryText( bool bTotal );
	const char* GetDepartmentText();

public:
	int m_nYear;
	int m_nMonth;
	int m_nTaxBand;
	int m_nCategoryNo;
	int m_nDeptNo;
	int m_nTaxDateCode;

public:
	double m_dValue;
	double m_dTax;

private:
	CString m_strYear;
	CString m_strMonth;
	CString m_strTaxBand;
	CString m_strCategory;
	CString m_strDepartment;
};

/**********************************************************************/

struct CEposReportSptBookCascadeBlock : public CEposReportBlockMapEntity
{
public:
	CEposReportSptBookCascadeBlock() { m_pSales = NULL; }

public:
	bool GotData();

public:
	CReportConsolidationArray<CEposReportConsolSptBookCascade>* m_pSales;
	CEposReportConsolSptBookCascade m_BlockTotal;
};

/**********************************************************************/
