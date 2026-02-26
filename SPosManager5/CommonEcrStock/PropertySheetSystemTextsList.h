#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "DataAccessHelpersSet.h"
#include "PropPageSystemDataList.h"
/**********************************************************************/

class CPropertySheetSystemTextsList : public CSSPropertySheet
{
	DECLARE_DYNAMIC( CPropertySheetSystemTextsList )

public:
	CPropertySheetSystemTextsList( CWnd* pWndParent = NULL);
		
public:
	//{{AFX_VIRTUAL(CPropertySheetSystemTextsLisr)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropertySheetSystemTextsList)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	static bool IsRelevant();
	
public:
	CPropPageSystemDataList m_PageEcrText;
	CPropPageSystemDataList m_PageStockText;
	CPropPageSystemDataList m_PageReportSet;
	CPropPageSystemDataList m_PageMacro;
	CPropPageSystemDataList m_PageReason;
	CPropPageSystemDataList m_PageTable;
	CPropPageSystemDataList m_PageBillText;
	CPropPageSystemDataList m_PageFunction;
	CPropPageSystemDataList m_PageOPGroup;
	CDataAccessHelpersSet m_Helpers;
};

/**********************************************************************/
#endif
/**********************************************************************/
