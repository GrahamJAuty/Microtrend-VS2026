#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "PropPageOptionsPlu.h"
#include "..\SPosStockManager5\PropPageOptionsStock.h"
#include "..\SPosStockManager5\PropPageOptionsStock2.h"
#include "PropPageOptionsBackup.h"
#include "PropPageOptionsGlobals.h"
#include "PropPageOptionsFeatures.h"
#include "PropPageOptionsReports1.h"
#include "PropPageOptionsReports2.h"
#include "PropPageOptionsReports3.h"
#include "PropPageOptionsExpire.h"
#include "PropPageOptionsLabels.h"
#include "PropPageOptionsLabels2.h"
#include "PropPageOptionsEmail.h"
/**********************************************************************/

class CPropertySheetOptions : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetOptions)

public:
	CPropertySheetOptions( bool bSchedulerFinished, CWnd* pWndParent = NULL);
	virtual ~CPropertySheetOptions();
		
public:
	//{{AFX_VIRTUAL(CPropertySheetOptions)
	protected:
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropertySheetOptions)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
#ifdef STOCKMAN_SYSTEM
	CPropPageOptionsPlu m_PagePlu;
	CPropPageOptionsStock m_PageStock1;
	CPropPageOptionsStock2 m_PageStock2;
	CPropPageOptionsGlobals m_PageGlobals;
	CPropPageOptionsFeatures m_PageFeatures;
	CPropPageOptionsReports1 m_PageReports1;
	CPropPageOptionsReports2 m_PageReports2;
	CPropPageOptionsReports3 m_PageReports3;
	CPropPageOptionsBackup m_PageBackup;
	CPropPageOptionsExpire m_PageExpire;
	CPropPageOptionsLabels m_PageLabels1;
	CPropPageOptionsLabels2 m_PageLabels2;
	CPropPageOptionsEmail m_PageEmail;
#else
	CPropPageOptionsPlu m_PagePlu;
	CPropPageOptionsGlobals m_PageGlobals;
	CPropPageOptionsFeatures m_PageFeatures;
	CPropPageOptionsReports1 m_PageReports1;
	CPropPageOptionsReports2 m_PageReports2;
	CPropPageOptionsReports3 m_PageReports3;
	CPropPageOptionsBackup m_PageBackup;
	CPropPageOptionsExpire m_PageExpire;
	CPropPageOptionsLabels m_PageLabels1;
	CPropPageOptionsLabels2 m_PageLabels2;
	CPropPageOptionsEmail m_PageEmail;
#endif
};

/**********************************************************************/
#endif
/**********************************************************************/
