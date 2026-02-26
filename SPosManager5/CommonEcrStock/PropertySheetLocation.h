#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "Consolidation.h"
#include "LocationCSVArray.h"
#include "MultilineTextBuffer.h"
#include "PriceSetItemManager.h"
#include "PropPageLocationGeneral.h"
#include "PropPageLocationUpload.h"
#include "PropPageLocationIDraught.h"
#include "PropPageLocationBritannia.h"
#include "PropPageLocationPriceSet.h"
#include "PropPageLocationTerminal.h"
#include "PropPageLocationNotes.h"
#include "PropPageLocationPluDownloadRange.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CPropertySheetLocation : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetLocation)

public:
	CPropertySheetLocation( int nLocIdx, bool bAdd, CWnd* pWndParent = NULL);
	virtual ~CPropertySheetLocation();
		
public:
	//{{AFX_VIRTUAL(CPropertySheetLocation)
	protected:
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropertySheetLocation)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SaveRecord( bool bNewLocation );

private:
	void AddTerminal( int nTNo );
	void RemoveTerminal( int nTNo );

public:
	CPropPageLocationGeneral m_Page1;
	CPropPageLocationTerminal m_Page2;
	CPropPageLocationPriceSet m_Page3;
	CPropPageLocationIDraught m_Page4;
	CPropPageLocationUpload m_Page5;
	CPropPageLocationBritannia m_Page6;
	CPropPageLocationNotes m_Page7;
	CPropPageLocationPluDownloadRange m_Page8;

private:
	CReportConsolidationArray<CSortedIntItem> m_TNoOldArray;
	CReportConsolidationArray<CSortedIntItem> m_TNoNewArray;

private:
	CMultilineTextBuffer m_LocationNotes;

private:
	CLocationCSVArray m_LocationCSVArray;
	CPriceSetHeaderManager m_PriceSetHeaderManager;
	int m_nLocIdx;
	bool m_bAdd;
};

/**********************************************************************/
#endif
/**********************************************************************/
