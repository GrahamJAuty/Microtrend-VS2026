#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "DefTextCategory.h"
/**********************************************************************/
#define CATEGORYSET_VERSION 1
/**********************************************************************/

class CCategorySetCSVArray : public CSharedCSVArray
{
public:
	CCategorySetCSVArray();
	
public:
	const char* GetName ( int nIndex );

private:
	CString m_strName;
};

/**********************************************************************/

class CCategorySetCSVRecord : public CCSVRecord
{
public:
	CCategorySetCSVRecord(); 
	virtual ~CCategorySetCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
	void SetName ( const char* szName ) { m_strName = szName; }
	const char* GetName() { return m_strName; }
	const char* GetFilterName( bool bTrailingComma, bool bHideReportEnabled );
	
	bool IncludeCategory ( int nCategory, bool bFlag );
	void IncludeAllCategories( bool bFlag );
	
	bool GetCategoryStatus ( int nCategory );
	int GetDefaultCategory();
	
	int CreateCategorySet ( int nCategoryFilter );
	int CreateCategorySetSystem ( int nCategoryFilter );

	int CreateReportEnabledCategorySet( CDefTextCategory& CatArray );

	bool GetAllModeFlag(){ return m_bAllMode; }
	bool CreatedEmptyFilter(){ return m_bCreatedEmptyFilter; }

private:
	int CreateCategorySetInternal( CDefTextCategory& CatArray, CCategorySetCSVArray& CatSetArray, int nCategoryFilter );

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );

private:
	bool m_bIncluded [ MAX_CATEGORY + 1 ];
	CString m_strName;  
	CString m_strFilterName;
	CString m_strFilterNameOut;

private:
	bool m_bReportEnabledSet;
	bool m_bAllMode;

private:
	bool m_bCreatedEmptyFilter;
	bool m_bConvertedEmptyFilter;
};

/**********************************************************************/
#endif				//__CATEGORYSETCSVARRAY_H__	
/**********************************************************************/
