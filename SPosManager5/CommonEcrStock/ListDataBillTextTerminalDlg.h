#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersBillTextTerminal : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersBillTextTerminal( int nLocIdx );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){}
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent ){}

private:
	int m_nLocIdx;
};

/**********************************************************************/

class CListDataBillTextTerminalDlg : public CListDataDlg
{
public:
	CListDataBillTextTerminalDlg( int nLocIdx, CWnd* pParent = NULL); 
	~CListDataBillTextTerminalDlg();
	
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

