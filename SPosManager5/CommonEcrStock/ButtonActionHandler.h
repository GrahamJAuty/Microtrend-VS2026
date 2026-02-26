#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ButtonInfo.h"
#include "Consolidation.h"
#include "DataAccessHelpersSet.h"
#include "EposTaskDatabase.h"
#include "PresetReportBase.h"
/**********************************************************************/

class CButtonActionHandler
{
public:
	CButtonActionHandler(void);
	
public:
	void ShowContextMenuEditKey( CWnd* pMenuParent, int nButtonIdx, CButtonInfo& info );
	void Action( CButtonInfo& infoDisplay, CWnd* pMenuParent, CWnd* pParent );
	void ContextOption( int n );

private:
	void ShowContextMenu( CButtonInfo& infoDisplay, CArray<CSortedIntAndStringItem,CSortedIntAndStringItem>& arrayItems );
	void ShowContextMenuDatabase( CButtonInfo& info );
	void ShowContextMenuDatabaseServer( CButtonInfo& info );
	void ShowContextMenuLocationServer( CButtonInfo& info );
	void ShowContextMenuPresets( CPresetReportArray& PresetArray, CButtonInfo& infoDisplay, bool& bSingleReport );
	bool ShowContextMenuTask( CButtonInfo& infoDisplay );
	void ShowContextMenuPluSalesHistory( CButtonInfo& infoDisplay );
	void ShowContextMenuEposReports( CButtonInfo& infoDisplay );
	void ShowContextMenuCustomerReports( CButtonInfo& infoDisplay );
	void ShowContextMenuTimeAttend( CButtonInfo& infoDisplay );
	void ShowContextMenuKeyboardLocation( CButtonInfo& info );
	void ShowContextMenuKeyboardNumber( CButtonInfo& info );
	void ShowContextMenuPriceSetLocation( CButtonInfo& info );
	void ShowContextMenuPriceSetDatabase( CButtonInfo& info );
	void ShowContextMenuPriceSetNumber( CButtonInfo& info, int nEntityIdx );

#ifdef STOCKMAN_SYSTEM
	void ShowContextMenuStockpoint( CButtonInfo& info );
	void ShowContextMenuStockSalesHistory( CButtonInfo& infoDisplay );
	void ShowContextMenuStockLevel( CButtonInfo& infoDisplay );
	void ShowContextMenuStockAudit( CButtonInfo& infoDisplay );
#else
	void ShowContextMenuEcrmanStock( CButtonInfo& infoDisplay );
#endif

	bool PrepareCommand();
	void DatabaseAction( CButtonInfo& infoDisplay, bool bGlobal );
	void KeyboardAction( CButtonInfo& infoDisplay );
	void PriceSetAction( CButtonInfo& infoDisplay );
	void ServerAction( CButtonInfo& infoDisplay );
	void ActionInternal( CButtonInfo& infoDisplay );

#ifdef STOCKMAN_SYSTEM
	void StockpointAction( CButtonInfo& infoDisplay );
#endif

	void EditServerSystem( CButtonInfo& infoDisplay );
	void EditServerDatabase( CButtonInfo& infoDisplay, int nServerDbIdx );
	void EditServerLocation( CButtonInfo& infoDisplay, int nServerLocIdx );

public:
	void ShowAuthorisationError( int n );
	
private:
	bool CheckAuthorisation( CButtonInfo& infoDisplay );
	void ShowUnknownDatabaseError( int nDb );
	void ShowUnknownLocationError( int nNwkNo, int nLocNo );
	void ShowUnknownPriceSetError( int nNwkNo, int nLocNo, int nPSetNo );
	void ShowUnknownStockpointError( int nDbNo, int nSpNo );
	void ShowUnknownKeyboardLocError( int nDbNo, int nLocNo );
	void ShowUnknownKeyboardSetError( int nDbNo, int nKbsNo );
	
private:
	CButtonInfo m_ButtonContext;
	CArray<UINT,UINT> m_arrayContextIDs;
	CArray<int,int> m_arrayContextOptions;
	int m_nContextMenuMode;

private:
	CDataAccessHelpersSet m_AccessHelpers;
	CKeyboardMapInfo m_KeyboardMapInfo;
	CEposTaskDatabase m_TaskDatabase;
	CWnd* m_pMenuParent;

private:
	int m_nEditButtonIdx;
};

extern CButtonActionHandler ButtonActionHandler;

/**********************************************************************/
#endif
/**********************************************************************/
