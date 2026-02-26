#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "DataAccessHelpersSet.h"
#include "PropPageSystemDataList.h"
/**********************************************************************/

class CPropertySheetSystemDataList : public CSSPropertySheet
{
	DECLARE_DYNAMIC( CPropertySheetSystemDataList )

public:
	CPropertySheetSystemDataList( CWnd* pWndParent = NULL);
		
public:
	//{{AFX_VIRTUAL(CPropertySheetSystemDataList)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropertySheetSystemDataList)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	static bool IsRelevant();

	void SetDataValidationErrorFlag(){ m_bDataValidationError = TRUE; }
	bool GetDataValidationErrorFlag(){ return m_bDataValidationError; }

public:
	CPropPageSystemDataList m_Page1;
	CPropPageSystemDataList m_Page2;
	CPropPageSystemDataList m_Page3;
	CPropPageSystemDataList m_Page4;
	CPropPageSystemDataList m_Page5;
	CPropPageSystemDataList m_Page6;
	CPropPageSystemDataList m_Page7;
	CPropPageSystemDataList m_Page8;
	CDataAccessHelpersSet m_Helpers;
	
private:
	bool m_bDataValidationError;
};

/**********************************************************************/
#endif
/**********************************************************************/
