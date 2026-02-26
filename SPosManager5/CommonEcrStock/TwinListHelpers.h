#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CTwinListHelpers
{
public:
	CTwinListHelpers( CSSListMultiSelectCtrlDblClick& listA, CSSListMultiSelectCtrlDblClick& listB, CReportConsolidationArray<CSortedIntWithFlag>& arrayA, CReportConsolidationArray<CSortedIntWithFlag>& arrayB );
	~CTwinListHelpers(void);

public:
	void GetSelectedItemArray( bool bListB, CReportConsolidationArray<CSortedIntItem>& arrayItems );
	bool SelectSingleLine( bool bListB );
	void SelectLine( bool bListB, int nIndex );
	void MoveItems( bool bFromListB );

private:
	CSSListMultiSelectCtrl* GetItemList( bool bListB );
	CReportConsolidationArray<CSortedIntWithFlag>* GetItemArray( bool bListB );

private:
	CSSListMultiSelectCtrl& m_listA;
	CSSListMultiSelectCtrl& m_listB;

private:
	CReportConsolidationArray<CSortedIntWithFlag>& m_arrayA;
	CReportConsolidationArray<CSortedIntWithFlag>& m_arrayB;
	int m_nMoveCount;
};

/**********************************************************************/
#endif
/**********************************************************************/
