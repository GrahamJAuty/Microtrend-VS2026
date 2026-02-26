#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "PosTrayTask.h"
#include "PosTrayTaskInfo.h"
#include "ReportConsolidationArray.h"
#include "PosTrayFileSet.h"
#include "PropPagePosTrayTaskGeneral.h"
#include "PropPagePosTrayTaskTime.h"
/**********************************************************************/

class CPropertySheetPosTrayTask : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetPosTrayTask)

public:
	CPropertySheetPosTrayTask( CPosTrayFileSet& FileSet, CPosTrayTask& task, CWnd* pWndParent = NULL);
	virtual ~CPropertySheetPosTrayTask();

public:
	void SetBatchInfoArray( CReportConsolidationArray<CPosTrayBatchInfo>* pArray ){ m_Page1.SetBatchInfoArray( pArray ); }
	bool GetNotBeforeTime( COleDateTime& date){ return m_Page2.GetNotBeforeTime( date ); }
		
public:
	//{{AFX_VIRTUAL(CPropertySheetPosTrayTask)
	protected:
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropertySheetPosTrayTask)
	//}}AFX_MSG
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	CPropPagePosTrayTaskGeneral m_Page1;
	CPropPagePosTrayTaskTime m_Page2;

private:
	CPosTrayFileSet& m_PosTrayFileSet;
	CPosTrayTask& m_Task;

private:
	CRect m_PageRect;
};

/**********************************************************************/
#endif
/**********************************************************************/
