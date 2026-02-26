#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "PropertySheetSetup.h"
#include "PropPageSetupSystem.h"
/**********************************************************************/

class CPropertySheetKeyboardSets : public CSSPropertySheet
{
	DECLARE_DYNAMIC( CPropertySheetKeyboardSets )

public:
	CPropertySheetKeyboardSets( CWnd* pWndParent = NULL);
		
public:
	//{{AFX_VIRTUAL(CPropertySheetKeyboardSets)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropertySheetKeyboardSets)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SaveSystem();

public:
	CPropPageSetupSystem m_Page1;
};

/**********************************************************************/
#endif
/**********************************************************************/
