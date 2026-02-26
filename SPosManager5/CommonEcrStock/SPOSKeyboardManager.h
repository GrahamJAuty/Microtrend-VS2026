#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SPOSKeyboardColourTable.h"
#include "SPOSKeyboardDefaultTextArray.h"
#include "SPOSKeyboardDlgViewLink.h"
#include "SPOSKeyboardFontTable.h"
#include "SPOSKeyboardImageLibrary.h"
#include "SPOSKeyboardImportPluDatabase.h"
#include "SPOSKeyboardKeymapTable.h"
#include "SPOSKeyboardLayoutOptions.h"
#include "SPOSKeyboardListManager.h"
#include "SPOSKeyboardMessageLogger.h"
#include "SPOSKeyboardModelessPointers.h"
#include "SPOSKeyboardMultipleKeySelectInfo.h"
#include "SPOSKeyboardVersion.h"
/**********************************************************************/
#define SPOSKEYBOARD_PLUEDIT_MASTER 0
#define SPOSKEYBOARD_PLUEDIT_PRICESET1 1
#define SPOSKEYBOARD_PLUEDIT_PRICESET2 2
#define SPOSKEYBOARD_PLUEDIT_PRICESET3 3
#define SPOSKEYBOARD_PLUEDIT_CREATE 4
#define SPOSKEYBOARD_PLUEDIT_LINK 5
/**********************************************************************/

struct CSPOSKeyboardManagerParams
{
public:
	CSPOSKeyboardManagerParams();

public:
	int m_nDbIdx;
	int m_nLocIdx;
	bool m_bKbSet;
	int m_nPriceSetCount;
	CString m_strDataPath;
	CString m_strKeyboardName;
	CString m_strSQLFileIn;
	CString m_strSQLFileOut;
	int m_nSPOSVersion;
	CString m_strImageFolder;
	CString m_strSysFilesFolder;
	CString m_strListNameFolder;
	int m_nPasswordIdx;
};

/**********************************************************************/

class CSPOSKeyboardManager
{
public:
	CSPOSKeyboardManager();

public:
	int GetDbIdx(){ return m_nDbIdx; }
	int GetLocIdx(){ return m_nLocIdx; }
	bool GetKbSetFlag(){ return m_bKbSet; }
	int GetPriceSetCount(){ return m_nPriceSetCount; }
	void SetCanDoChangesTaskFlag(){ m_bCanDoChangesTask = TRUE; }
	void SetCanDeleteBarcodeFlag(){ m_bCanDeleteBarcode = TRUE; }
	void SetCanDeleteStockFlag(){ m_bCanDeleteStock = TRUE; }

public:
	void SetMergedLegacyMacrosFlag(){ m_bMergedLegacyMacros = TRUE; }
	bool GetMergedLegacyMacrosFlag(){ return m_bMergedLegacyMacros; }

public:
	void ProcessKeyboard( CWnd* pParentWnd, CSPOSKeyboardManagerParams& Params );
	void PrepareDownload( const char* szSourcePath, const char* szKbdFilename, const char* szTempFilename, int nSPOSVersion, bool bClearMacros );
	void CreatePluItemList();
	void CreateActiveMacroList();
	void MergeParentMacros();

private:
	void Reset();
	void HandlePluChanges( CWnd* pParentWnd );

public:
	CSPOSKeyboardColourTable* GetColourTable(){ return &m_ColourTable; }
	CSPOSKeyboardFontTable* GetFontTable(){ return &m_FontTable; }
	CSPOSKeyboardImageLibrary* GetImageLibrary(){ return &m_ImageLibrary; }
	CSPOSKeyboardImportPluCSVArray* GetPluDatabase(){ return &m_PluDatabase; }
	CSPOSKeyboardKeymapTable* GetKeymapTable(){ return &m_KeymapTable; }
	CSPOSKeyboardListManager* GetListManager(){ return &m_ListManager; }
	CSPOSKeyboardLayoutOptions* GetLayoutOptions(){ return &m_LayoutOptions; }
	CSPOSKeyboardMessageLogger* GetMessageLogger(){ return &m_MessageLogger; }
	CSPOSKeyboardModelessPointers* GetModelessPointers(){ return &m_ModelessPointers; }
	CSPOSKeyboardMultipleKeySelectInfo* GetMultiKeySelectInfo(){ return &m_MultipleKeySelectInfo; }
	CSPOSKeyboardVersion* GetSPOSVersion(){ return &m_SPOSVersion; }

public:
	CSPOSKeyboardDefaultTextArray* GetListTable(){ return &m_ListTable; }
	CSPOSKeyboardDefaultTextArray* GetMacroTable(){ return &m_MacroTable; }
	CSPOSKeyboardDefaultTextArray* GetFunctionTable(){ return &m_FunctionTable; }
	CSPOSKeyboardDefaultTextArray* GetPaymentTable(){ return &m_PaymentTable; }
	CSPOSKeyboardDefaultTextArray* GetControlTable(){ return &m_ControlTable; }
	CSPOSKeyboardDefaultTextArray* GetModifierTable(){ return &m_ModifierTable; }
	CSPOSKeyboardDefaultTextArray* GetSvrPresetTable(){ return &m_SvrPresetTable; }
	CSPOSKeyboardDefaultTextArray* GetEmptyTable(){ return &m_EmptyTable; }

public:
	CSPOSKeyboardDlgViewLink* GetDlgViewLink(){ return &m_DlgViewLink; }

private:
	CSPOSKeyboardColourTable m_ColourTable;
	CSPOSKeyboardFontTable m_FontTable;
	CSPOSKeyboardImageLibrary m_ImageLibrary;
	CSPOSKeyboardImportPluCSVArray m_PluDatabase;
	CSPOSKeyboardKeymapTable m_KeymapTable;
	CSPOSKeyboardListManager m_ListManager;
	CSPOSKeyboardLayoutOptions m_LayoutOptions;
	CSPOSKeyboardMessageLogger m_MessageLogger;
	CSPOSKeyboardModelessPointers m_ModelessPointers;
	CSPOSKeyboardMultipleKeySelectInfo m_MultipleKeySelectInfo;
	CSPOSKeyboardVersion m_SPOSVersion;

private:
	CSPOSKeyboardDefaultTextArray m_ListTable;
	CSPOSKeyboardDefaultTextArray m_MacroTable;
	CSPOSKeyboardDefaultTextArray m_FunctionTable;
	CSPOSKeyboardDefaultTextArray m_PaymentTable;
	CSPOSKeyboardDefaultTextArray m_ControlTable;
	CSPOSKeyboardDefaultTextArray m_ModifierTable;
	CSPOSKeyboardDefaultTextArray m_SvrPresetTable;
	CSPOSKeyboardDefaultTextArray m_EmptyTable;

private:
	CSPOSKeyboardDlgViewLink m_DlgViewLink;

private:
	int m_nDbIdx;
	int m_nLocIdx;
	bool m_bKbSet;
	int m_nPriceSetCount;

private:
	bool m_bCanDoChangesTask;
	bool m_bCanDeleteBarcode;
	bool m_bCanDeleteStock;
	bool m_bMergedLegacyMacros;
};

/**********************************************************************/
extern CSPOSKeyboardManager SPOSKeyboardManager;
/**********************************************************************/
#endif
/**********************************************************************/
