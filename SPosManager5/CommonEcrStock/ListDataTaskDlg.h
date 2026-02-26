#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "EposTaskDatabase.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/
#include "ListDataDlg.h"
/**********************************************************************/

class CSingleListContentHelpersTask : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersTask( CEposTaskDatabase& TaskDatabase );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){}
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent ){}

private:
	CEposTaskDatabase& m_TaskDatabase;
};

/**********************************************************************/

class CListDataTaskDlg : public CListDataDlg
{
public:
	CListDataTaskDlg( CWnd* pParent = NULL); 
	~CListDataTaskDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual bool HandleOK();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON1; }
	virtual void HandleButton1();	//EDIT
	virtual void HandleDefaultButton() { HandleButton1(); }

private:
	CEposTaskDatabase m_TaskDatabase;
};

/**********************************************************************/
#endif
/**********************************************************************/
