#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "PropPagePMSOptionsDinner.h"
/**********************************************************************/

class CPropertySheetPMSDinner : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetPMSDinner)

public:
	CPropertySheetPMSDinner( CPMSOptions* pPMSOptions, CWnd* pWndParent = NULL);
	virtual ~CPropertySheetPMSDinner();
		
public:
	//{{AFX_VIRTUAL(CPropertySheetPMSDinner)
	protected:
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropertySheetPMSDinner)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CPropPagePMSOptionsDinner m_page1;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
