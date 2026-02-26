#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "LocationSelectorEntity.h"
#include "MacroHelpDlg.h"
#include "SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersMacro : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersMacro( CLocationSelectorEntity& LocSelEntity );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
	void GetMacroInfo( CCascadingMacroCSVRecord& Macro, CString& strSource, CString& strName, CString& strData );

private:
	CLocationSelectorEntity& m_LocSelEntity;
	CCascadingMacroCSVArray* m_pMacroArray;
	CString m_strLocalType;
};

/**********************************************************************/

class CListDataMacroDlg : public CListDataDlg
{
public:
	CListDataMacroDlg( CLocationSelectorEntity& LocSelEntity, CWnd* pParent = NULL); 
	~CListDataMacroDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	
	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	//ADD
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton3();	//DELETE
	virtual void HandleButton4();	//PRINT
	virtual void HandleButton5();	//HELP
	virtual void HandleDefaultButton() { HandleButton2(); }
	
private:
	CLocationSelectorEntity& m_LocSelEntity;
	CCascadingMacroCSVArray* m_pMacroArray;
	CMacroHelpDlg* m_pMacroHelpDlg;
};

/**********************************************************************/
#endif
/**********************************************************************/
