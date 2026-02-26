#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "PropPageHiddenOptionsEcrman.h"
#include "..\SposStockManager5\PropPageHiddenOptionsStockman.h"
/**********************************************************************/

class CPropertySheetHiddenOptions : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetHiddenOptions)

public:
	CPropertySheetHiddenOptions( CWnd* pWndParent = NULL);
	virtual ~CPropertySheetHiddenOptions();
		
public:
	//{{AFX_VIRTUAL(CPropertySheetHiddenOptions)
	protected:
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropertySheetHiddenOptions)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CPropPageHiddenOptionsEcrman m_Page1;
#ifdef STOCKMAN_SYSTEM
	CPropPageHiddenOptionsStockman m_Page2;
#endif
};

/**********************************************************************/
#endif
/**********************************************************************/
