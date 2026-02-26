#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "PriceSetItemManager.h"
#include "PropPageDatabaseGeneral.h"
#include "PropPageDatabasePriceSet.h"
/**********************************************************************/

class CPropertySheetDatabase : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetDatabase)

public:
	CPropertySheetDatabase( int nDbIdx, bool bAdd, CWnd* pWndParent = NULL);
	virtual ~CPropertySheetDatabase();
	
public:
	//{{AFX_VIRTUAL(CPropertySheetDatabase)
	protected:
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropertySheetDatabase)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SaveRecord();

public:
	CPropPageDatabaseGeneral m_Page1;
	CPropPageDatabasePriceSet m_Page2;

private:
	int m_nDbIdx;
	bool m_bAdd;
	CDatabaseCSVArray m_DatabaseCSVArray;
	CPriceSetHeaderManager m_PriceSetHeaderManager;
};

/**********************************************************************/
#endif
/**********************************************************************/
