#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "PropPageSetupSystem.h"
/**********************************************************************/
#define SETUPTREE_TYPE_NETWORK 0
#define SETUPTREE_TYPE_SYSTEM 1
#define SETUPTREE_TYPE_STOCK 2
#define SETUPTREE_TYPE_REPPOINT 3
#define SETUPTREE_TYPE_SITE 4
#define SETUPTREE_TYPE_LOCSET 5
#define SETUPTREE_TYPE_KBSET 6
#define SETUPTREE_TYPE_SPTBOOK 7
#define SETUPTREE_TYPE_SMARTENT 8
/**********************************************************************/

class CPropertySheetSetup : public CSSPropertySheet
{
	DECLARE_DYNAMIC( CPropertySheetSetup )

public:
	CPropertySheetSetup( CWnd* pWndParent = NULL);
		
public:
	//{{AFX_VIRTUAL(CPropertySheetSetup)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropertySheetSetup)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SaveSystem();

public:
	CPropPageSetupSystem m_Page1;
	CPropPageSetupSystem m_Page2;
	CPropPageSetupSystem m_Page3;
	CPropPageSetupSystem m_Page4;
	CPropPageSetupSystem m_Page5;
	CPropPageSetupSystem m_Page6;	//LOCATION SETS
	CPropPageSetupSystem m_Page7;	//SPORTS BOOKER
	CPropPageSetupSystem m_Page8;	//SMART ENTERTAINMENT
};

/**********************************************************************/
#endif
/**********************************************************************/
