#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "Consolidation.h"
#include "NodeTypes.h"
#include "KeyboardFilterSelector.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CKeyboardFilterPluArray
{
public:
	CKeyboardFilterPluArray(void);

public:
	void SelectKeyboard( CKeyboardFilterSelectorLoc& Loc, int nKbNo ); 
	void GetSelection( CKeyboardFilterSelectorLoc& Loc, int& nKbNo );
	
public:
	bool IsFiltered(){ return ( m_CurrentLoc.m_nNodeType != NODE_SYSTEM); }
	bool CheckPluNo( __int64 nPluNo );
	
private:
	CReportConsolidationArray<CSortedInt64Item> m_arrayPluNo;

private:
	CKeyboardFilterSelectorLoc m_CurrentLoc;
	int m_nCurrentKbNo;
};

/**********************************************************************/
#endif
/**********************************************************************/
