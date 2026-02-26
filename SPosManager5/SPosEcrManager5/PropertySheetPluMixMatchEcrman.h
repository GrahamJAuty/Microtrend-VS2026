#pragma once
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "..\CommonEcrStock\PluFilterArray.h"
#include "..\CommonEcrStock\PropPagePluGeneral.h"
/**********************************************************************/

class CPropertySheetPluMixMatchEcrman : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetPluMixMatchEcrman)

public:
	CPropertySheetPluMixMatchEcrman( int nPluIdx, CWnd* pParentWnd = NULL);

public:
	CPropPagePluGeneral m_Page1;

public:
	//{{AFX_VIRTUAL(CPropertySheetPluMixMatchEcrman)
	//}}AFX_VIRTUAL

public:
	virtual ~CPropertySheetPluMixMatchEcrman();

public:
	void SaveRecord();
	
private:
	void UpdateTitle();
	
protected:
	//{{AFX_MSG(CPropertySheetPluMixMatchEcrman)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CPluCSVRecord m_PluRecord;
	CByteArray m_PageArray;
	int m_nPluIdx;
};

/**********************************************************************/
#endif	// __PROPERTYSHEETPLU_
/**********************************************************************/
#endif
/**********************************************************************/
