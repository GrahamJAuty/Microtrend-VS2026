#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "PropPageIDraughtOptionsGeneral.h"
#include "PropPageIDraughtOptionsLocation.h"
#include "PropPageIDraughtOptionsDepartment.h"
/**********************************************************************/

class CPropertySheetIDraughtOptions : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetIDraughtOptions)

public:
	CPropertySheetIDraughtOptions( CWnd* pWndParent = NULL);
	virtual ~CPropertySheetIDraughtOptions();
		
public:
	//{{AFX_VIRTUAL(CPropertySheetIDraughtOptions)
	protected:
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropertySheetIDraughtOptions)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CPropPageIDraughtOptionsGeneral m_Page1;
	CPropPageIDraughtOptionsLocation m_Page2;
	CPropPageIDraughtOptionsDepartment m_Page3;
};

/**********************************************************************/
#endif
/**********************************************************************/
