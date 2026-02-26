#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersServerPresetTerminal : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersServerPresetTerminal( int nLocIdx );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){}
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent ){}

private:
	int m_nLocIdx;
};

/**********************************************************************/

class CListDataServerPresetTerminalDlg : public CListDataDlg
{
public:
	CListDataServerPresetTerminalDlg( int nLocIdx, CWnd* pParent = NULL); 
	~CListDataServerPresetTerminalDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON1; }
	virtual void HandleButton1();	//EDIT
	virtual void HandleDefaultButton() { HandleButton1(); }

private:
	int m_nLocIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/

